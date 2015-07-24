//
// dwhcidevice.c
//
// Supports:
//	internal DMA only,
//	no ISO transfers
//	no dynamic attachments
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <uspi/dwhcidevice.h>
#include <uspios.h>
#include <uspi/bcm2835.h>
#include <uspi/synchronize.h>
#include <uspi/assert.h>

#define ARM_IRQ_USB		9		// for ConnectInterrupt()

#define DEVICE_ID_USB_HCD	3		// for SetPowerStateOn()

//
// Configuration
//
#define DWC_CFG_DYNAMIC_FIFO				// re-program FIFOs with these sizes:
	#define DWC_CFG_HOST_RX_FIFO_SIZE	1024	// number of 32 bit words
	#define DWC_CFG_HOST_NPER_TX_FIFO_SIZE	1024	// number of 32 bit words
	#define DWC_CFG_HOST_PER_TX_FIFO_SIZE	1024	// number of 32 bit words

#define MSEC2HZ(msec)		((msec) * HZ / 1000)

typedef enum
{
	StageStateNoSplitTransfer,
	StageStateStartSplit,
	StageStateCompleteSplit,
	StageStatePeriodicDelay,
	StageStateUnknown
}
TStageState;

typedef enum
{
	StageSubStateWaitForChannelDisable,
	StageSubStateWaitForTransactionComplete,
	StageSubStateUnknown
}
TStageSubState;

static const char FromDWHCI[] = "dwhci";

boolean DWHCIDeviceInitCore (TDWHCIDevice *pThis);
boolean DWHCIDeviceInitHost (TDWHCIDevice *pThis);
boolean DWHCIDeviceEnableRootPort (TDWHCIDevice *pThis);
boolean DWHCIDeviceReset (TDWHCIDevice *pThis);
void DWHCIDeviceEnableGlobalInterrupts (TDWHCIDevice *pThis);
void DWHCIDeviceEnableCommonInterrupts (TDWHCIDevice *pThis);
void DWHCIDeviceEnableHostInterrupts (TDWHCIDevice *pThis);
void DWHCIDeviceEnableChannelInterrupt (TDWHCIDevice *pThis, unsigned nChannel);
void DWHCIDeviceDisableChannelInterrupt (TDWHCIDevice *pThis, unsigned nChannel);
void DWHCIDeviceFlushTxFIFO (TDWHCIDevice *pThis, unsigned nFIFO);
void DWHCIDeviceFlushRxFIFO (TDWHCIDevice *pThis);
boolean DWHCIDeviceTransferStage (TDWHCIDevice *pThis, TUSBRequest *pURB, boolean bIn, boolean bStatusStage);
void DWHCIDeviceCompletionRoutine (TUSBRequest *pURB, void *pParam, void *pContext);
boolean DWHCIDeviceTransferStageAsync (TDWHCIDevice *pThis, TUSBRequest *pURB, boolean bIn, boolean bStatusStage);
void DWHCIDeviceStartTransaction (TDWHCIDevice *pThis, TDWHCITransferStageData *pStageData);
void DWHCIDeviceStartChannel (TDWHCIDevice *pThis, TDWHCITransferStageData *pStageData);
void DWHCIDeviceChannelInterruptHandler (TDWHCIDevice *pThis, unsigned nChannel);
void DWHCIDeviceInterruptHandler (void *pParam);
void DWHCIDeviceTimerHandler (unsigned hTimer, void *pParam, void *pContext);
unsigned DWHCIDeviceAllocateChannel (TDWHCIDevice *pThis);
void DWHCIDeviceFreeChannel (TDWHCIDevice *pThis, unsigned nChannel);
boolean DWHCIDeviceWaitForBit (TDWHCIDevice *pThis, TDWHCIRegister *pRegister, u32 nMask,boolean bWaitUntilSet, unsigned nMsTimeout);
#ifndef NDEBUG
void DWHCIDeviceDumpRegister (TDWHCIDevice *pThis, const char *pName, u32 nAddress);
void DWHCIDeviceDumpStatus (TDWHCIDevice *pThis, unsigned nChannel /* = 0 */);
#endif

void DWHCIDevice (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	pThis->m_nChannels = 0;
	pThis->m_nChannelAllocated = 0;
	pThis->m_bWaiting = FALSE;
	DWHCIRootPort (&pThis->m_RootPort, pThis);

	for (unsigned nChannel = 0; nChannel < DWHCI_MAX_CHANNELS; nChannel++)
	{
		pThis->m_pStageData[nChannel] = 0;
	}
}

void _DWHCIDevice (TDWHCIDevice *pThis)
{
	_DWHCIRootPort (&pThis->m_RootPort);
}

boolean DWHCIDeviceInitialize (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	DataMemBarrier ();

	TDWHCIRegister VendorId;
	DWHCIRegister (&VendorId, DWHCI_CORE_VENDOR_ID);
	if (DWHCIRegisterRead (&VendorId) != 0x4F54280A)
	{
		LogWrite (FromDWHCI, LOG_ERROR, "Unknown vendor 0x%0X", DWHCIRegisterGet (&VendorId));
		_DWHCIRegister (&VendorId);
		return FALSE;
	}

	if (!SetPowerStateOn (DEVICE_ID_USB_HCD))
	{
		LogWrite (FromDWHCI, LOG_ERROR, "Cannot power on");
		_DWHCIRegister (&VendorId);
		return FALSE;
	}
	
	// Disable all interrupts
	TDWHCIRegister AHBConfig;
	DWHCIRegister (&AHBConfig, DWHCI_CORE_AHB_CFG);
	DWHCIRegisterRead (&AHBConfig);
	DWHCIRegisterAnd (&AHBConfig, ~DWHCI_CORE_AHB_CFG_GLOBALINT_MASK);
	DWHCIRegisterWrite (&AHBConfig);
	
	ConnectInterrupt (ARM_IRQ_USB, DWHCIDeviceInterruptHandler, pThis);

	if (!DWHCIDeviceInitCore (pThis))
	{
		LogWrite (FromDWHCI, LOG_ERROR, "Cannot initialize core");
		_DWHCIRegister (&AHBConfig);
		_DWHCIRegister (&VendorId);
		return FALSE;
	}
	
	DWHCIDeviceEnableGlobalInterrupts (pThis);
	
	if (!DWHCIDeviceInitHost (pThis))
	{
		LogWrite (FromDWHCI, LOG_ERROR, "Cannot initialize host");
		_DWHCIRegister (&AHBConfig);
		_DWHCIRegister (&VendorId);
		return FALSE;
	}

	// The following calls will fail if there is no device or no supported device connected
	// to root port. This is not an error because the system may run without an USB device.

	if (!DWHCIDeviceEnableRootPort (pThis))
	{
		LogWrite (FromDWHCI, LOG_WARNING, "No device connected to root port");
		_DWHCIRegister (&AHBConfig);
		_DWHCIRegister (&VendorId);
		return TRUE;
	}

	if (!DWHCIRootPortInitialize (&pThis->m_RootPort))
	{
		LogWrite (FromDWHCI, LOG_WARNING, "Cannot initialize root port");
		_DWHCIRegister (&AHBConfig);
		_DWHCIRegister (&VendorId);
		return TRUE;
	}
	
	DataMemBarrier ();

	_DWHCIRegister (&AHBConfig);
	_DWHCIRegister (&VendorId);

	return TRUE;
}

int DWHCIDeviceGetDescriptor (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint,
			      unsigned char ucType, unsigned char ucIndex,
			      void *pBuffer, unsigned nBufSize,
			      unsigned char ucRequestType)
{
	assert (pThis != 0);

	return DWHCIDeviceControlMessage (pThis, pEndpoint,
					ucRequestType, GET_DESCRIPTOR,
					(ucType << 8) | ucIndex, 0,
					pBuffer, nBufSize);
}

boolean DWHCIDeviceSetAddress (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint, u8 ucDeviceAddress)
{
	assert (pThis != 0);

	if (DWHCIDeviceControlMessage (pThis, pEndpoint, REQUEST_OUT, SET_ADDRESS, ucDeviceAddress, 0, 0, 0) < 0)
	{
		return FALSE;
	}
	
	MsDelay (50);		// see USB 2.0 spec (tDSETADDR)
	
	return TRUE;
}

boolean DWHCIDeviceSetConfiguration (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint, u8 ucConfigurationValue)
{
	assert (pThis != 0);

	if (DWHCIDeviceControlMessage (pThis, pEndpoint, REQUEST_OUT, SET_CONFIGURATION, ucConfigurationValue, 0, 0, 0) < 0)
	{
		return FALSE;
	}
	
	MsDelay (50);
	
	return TRUE;
}

int DWHCIDeviceControlMessage (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint,
			u8 ucRequestType, u8 ucRequest, u16 usValue, u16 usIndex,
			void *pData, u16 usDataSize)
{
	assert (pThis != 0);

	TSetupData *pSetup = (TSetupData *) malloc (sizeof (TSetupData));
	assert (pSetup != 0);

	pSetup->bmRequestType = ucRequestType;
	pSetup->bRequest      = ucRequest;
	pSetup->wValue	      = usValue;
	pSetup->wIndex	      = usIndex;
	pSetup->wLength	      = usDataSize;

	TUSBRequest URB;
	USBRequest (&URB, pEndpoint, pData, usDataSize, pSetup);

	int nResult = -1;

	if (DWHCIDeviceSubmitBlockingRequest (pThis, &URB))
	{
		nResult = USBRequestGetResultLength (&URB);
	}
	
	free (pSetup);

	_USBRequest (&URB);

	return nResult;
}

int DWHCIDeviceTransfer (TDWHCIDevice *pThis, TUSBEndpoint *pEndpoint, void *pBuffer, unsigned nBufSize)
{
	assert (pThis != 0);

	TUSBRequest URB;
	USBRequest (&URB, pEndpoint, pBuffer, nBufSize, 0);

	int nResult = -1;

	if (DWHCIDeviceSubmitBlockingRequest (pThis, &URB))
	{
		nResult = USBRequestGetResultLength (&URB);
	}

	_USBRequest (&URB);

	return nResult;
}

boolean DWHCIDeviceSubmitBlockingRequest (TDWHCIDevice *pThis, TUSBRequest *pURB)
{
	assert (pThis != 0);

	DataMemBarrier ();

	assert (pURB != 0);
	USBRequestSetStatus (pURB, 0);
	
	if (USBEndpointGetType (USBRequestGetEndpoint (pURB)) == EndpointTypeControl)
	{
		TSetupData *pSetup = USBRequestGetSetupData (pURB);
		assert (pSetup != 0);

		if (pSetup->bmRequestType & REQUEST_IN)
		{
			assert (USBRequestGetBufLen (pURB) > 0);
			
			if (   !DWHCIDeviceTransferStage (pThis, pURB, FALSE, FALSE)
			    || !DWHCIDeviceTransferStage (pThis, pURB, TRUE,  FALSE)
			    || !DWHCIDeviceTransferStage (pThis, pURB, FALSE, TRUE))
			{
				return FALSE;
			}
		}
		else
		{
			if (USBRequestGetBufLen (pURB) == 0)
			{
				if (   !DWHCIDeviceTransferStage (pThis, pURB, FALSE, FALSE)
				    || !DWHCIDeviceTransferStage (pThis, pURB, TRUE,  TRUE))
				{
					return FALSE;
				}
			}
			else
			{
				if (   !DWHCIDeviceTransferStage (pThis, pURB, FALSE, FALSE)
				    || !DWHCIDeviceTransferStage (pThis, pURB, FALSE, FALSE)
				    || !DWHCIDeviceTransferStage (pThis, pURB, TRUE,  TRUE))
				{
					return FALSE;
				}
			}
		}
	}
	else
	{
		assert (   USBEndpointGetType (USBRequestGetEndpoint (pURB)) == EndpointTypeBulk
		        || USBEndpointGetType (USBRequestGetEndpoint (pURB)) == EndpointTypeInterrupt);
		assert (USBRequestGetBufLen (pURB) > 0);
		
		if (!DWHCIDeviceTransferStage (pThis, pURB, USBEndpointIsDirectionIn (USBRequestGetEndpoint (pURB)), FALSE))
		{
			return FALSE;
		}
	}
	
	DataMemBarrier ();

	return TRUE;
}

boolean DWHCIDeviceSubmitAsyncRequest (TDWHCIDevice *pThis, TUSBRequest *pURB)
{
	assert (pThis != 0);

	DataMemBarrier ();

	assert (pURB != 0);
	assert (   USBEndpointGetType (USBRequestGetEndpoint (pURB)) == EndpointTypeBulk
		|| USBEndpointGetType (USBRequestGetEndpoint (pURB)) == EndpointTypeInterrupt);
	assert (USBRequestGetBufLen (pURB) > 0);
	
	USBRequestSetStatus (pURB, 0);
	
	boolean bOK = DWHCIDeviceTransferStageAsync (pThis, pURB, USBEndpointIsDirectionIn (USBRequestGetEndpoint (pURB)), FALSE);

	DataMemBarrier ();

	return bOK;
}

boolean DWHCIDeviceInitCore (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister USBConfig;
	DWHCIRegister (&USBConfig, DWHCI_CORE_USB_CFG);
	DWHCIRegisterRead (&USBConfig);
	DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_ULPI_EXT_VBUS_DRV);
	DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_TERM_SEL_DL_PULSE);
	DWHCIRegisterWrite (&USBConfig);

	if (!DWHCIDeviceReset (pThis))
	{
		LogWrite (FromDWHCI, LOG_ERROR, "Reset failed");
		return FALSE;
	}

	DWHCIRegisterRead (&USBConfig);
	DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_ULPI_UTMI_SEL);	// select UTMI+
	DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_PHYIF);		// UTMI width is 8
	DWHCIRegisterWrite (&USBConfig);

	// Internal DMA mode only
	TDWHCIRegister HWConfig2;
	DWHCIRegister (&HWConfig2, DWHCI_CORE_HW_CFG2);
	DWHCIRegisterRead (&HWConfig2);
	assert (DWHCI_CORE_HW_CFG2_ARCHITECTURE (DWHCIRegisterGet (&HWConfig2)) == 2);
	
	DWHCIRegisterRead (&USBConfig);
	if (   DWHCI_CORE_HW_CFG2_HS_PHY_TYPE (DWHCIRegisterGet (&HWConfig2)) == DWHCI_CORE_HW_CFG2_HS_PHY_TYPE_ULPI
	    && DWHCI_CORE_HW_CFG2_FS_PHY_TYPE (DWHCIRegisterGet (&HWConfig2)) == DWHCI_CORE_HW_CFG2_FS_PHY_TYPE_DEDICATED)
	{
		DWHCIRegisterOr (&USBConfig, DWHCI_CORE_USB_CFG_ULPI_FSLS);
		DWHCIRegisterOr (&USBConfig, DWHCI_CORE_USB_CFG_ULPI_CLK_SUS_M);
	}
	else
	{
		DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_ULPI_FSLS);
		DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_ULPI_CLK_SUS_M);
	}
	DWHCIRegisterWrite (&USBConfig);

	assert (pThis->m_nChannels == 0);
	pThis->m_nChannels = DWHCI_CORE_HW_CFG2_NUM_HOST_CHANNELS (DWHCIRegisterGet (&HWConfig2));
	assert (4 <= pThis->m_nChannels && pThis->m_nChannels <= DWHCI_MAX_CHANNELS);

	TDWHCIRegister AHBConfig;
	DWHCIRegister (&AHBConfig, DWHCI_CORE_AHB_CFG);
	DWHCIRegisterRead (&AHBConfig);
	DWHCIRegisterOr (&AHBConfig, DWHCI_CORE_AHB_CFG_DMAENABLE);
	//DWHCIRegisterOr (&AHBConfig, DWHCI_CORE_AHB_CFG_AHB_SINGLE);	// if DMA single mode should be used
	DWHCIRegisterOr (&AHBConfig, DWHCI_CORE_AHB_CFG_WAIT_AXI_WRITES);
	DWHCIRegisterAnd (&AHBConfig, ~DWHCI_CORE_AHB_CFG_MAX_AXI_BURST__MASK);
	//DWHCIRegisterOr (&AHBConfig, 0 << DWHCI_CORE_AHB_CFG_MAX_AXI_BURST__SHIFT);	// max. AXI burst length 4
	DWHCIRegisterWrite (&AHBConfig);

	// HNP and SRP are not used
	DWHCIRegisterRead (&USBConfig);
	DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_HNP_CAPABLE);
	DWHCIRegisterAnd (&USBConfig, ~DWHCI_CORE_USB_CFG_SRP_CAPABLE);
	DWHCIRegisterWrite (&USBConfig);

	DWHCIDeviceEnableCommonInterrupts (pThis);

	_DWHCIRegister (&AHBConfig);
	_DWHCIRegister (&HWConfig2);
	_DWHCIRegister (&USBConfig);

	return TRUE;
}

boolean DWHCIDeviceInitHost (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	// Restart the PHY clock
	TDWHCIRegister Power;
	DWHCIRegister2 (&Power, ARM_USB_POWER, 0);
	DWHCIRegisterWrite (&Power);

	TDWHCIRegister HostConfig;
	DWHCIRegister (&HostConfig, DWHCI_HOST_CFG);
	DWHCIRegisterRead (&HostConfig);
	DWHCIRegisterAnd (&HostConfig, ~DWHCI_HOST_CFG_FSLS_PCLK_SEL__MASK);

	TDWHCIRegister HWConfig2;
	DWHCIRegister (&HWConfig2, DWHCI_CORE_HW_CFG2);
	TDWHCIRegister USBConfig;
	DWHCIRegister (&USBConfig, DWHCI_CORE_USB_CFG);
	if (   DWHCI_CORE_HW_CFG2_HS_PHY_TYPE (DWHCIRegisterRead (&HWConfig2)) == DWHCI_CORE_HW_CFG2_HS_PHY_TYPE_ULPI
	    && DWHCI_CORE_HW_CFG2_FS_PHY_TYPE (DWHCIRegisterGet (&HWConfig2)) == DWHCI_CORE_HW_CFG2_FS_PHY_TYPE_DEDICATED
	    && (DWHCIRegisterRead (&USBConfig) & DWHCI_CORE_USB_CFG_ULPI_FSLS))
	{
		DWHCIRegisterOr (&HostConfig, DWHCI_HOST_CFG_FSLS_PCLK_SEL_48_MHZ);
	}
	else
	{
		DWHCIRegisterOr (&HostConfig, DWHCI_HOST_CFG_FSLS_PCLK_SEL_30_60_MHZ);
	}

	DWHCIRegisterWrite (&HostConfig);

#ifdef DWC_CFG_DYNAMIC_FIFO
	TDWHCIRegister RxFIFOSize;
	DWHCIRegister2 (&RxFIFOSize, DWHCI_CORE_RX_FIFO_SIZ, DWC_CFG_HOST_RX_FIFO_SIZE);
	DWHCIRegisterWrite (&RxFIFOSize);
	
	TDWHCIRegister NonPeriodicTxFIFOSize;
	DWHCIRegister2 (&NonPeriodicTxFIFOSize, DWHCI_CORE_NPER_TX_FIFO_SIZ, 0);
	DWHCIRegisterOr (&NonPeriodicTxFIFOSize, DWC_CFG_HOST_RX_FIFO_SIZE);
	DWHCIRegisterOr (&NonPeriodicTxFIFOSize, DWC_CFG_HOST_NPER_TX_FIFO_SIZE << 16);
	DWHCIRegisterWrite (&NonPeriodicTxFIFOSize);
	
	TDWHCIRegister HostPeriodicTxFIFOSize;
	DWHCIRegister2 (&HostPeriodicTxFIFOSize, DWHCI_CORE_HOST_PER_TX_FIFO_SIZ, 0);
	DWHCIRegisterOr (&HostPeriodicTxFIFOSize, DWC_CFG_HOST_RX_FIFO_SIZE + DWC_CFG_HOST_NPER_TX_FIFO_SIZE);
	DWHCIRegisterOr (&HostPeriodicTxFIFOSize, DWC_CFG_HOST_PER_TX_FIFO_SIZE << 16);
	DWHCIRegisterWrite (&HostPeriodicTxFIFOSize);
#endif

	DWHCIDeviceFlushTxFIFO (pThis, 0x10);	 	// Flush all TX FIFOs
	DWHCIDeviceFlushRxFIFO (pThis);

	TDWHCIRegister HostPort;
	DWHCIRegister (&HostPort, DWHCI_HOST_PORT);
	DWHCIRegisterRead (&HostPort);
	DWHCIRegisterAnd (&HostPort, ~DWHCI_HOST_PORT_DEFAULT_MASK);
	if (!(DWHCIRegisterGet (&HostPort) & DWHCI_HOST_PORT_POWER))
	{
		DWHCIRegisterOr (&HostPort, DWHCI_HOST_PORT_POWER);
		DWHCIRegisterWrite (&HostPort);
	}
	
	DWHCIDeviceEnableHostInterrupts (pThis);

	_DWHCIRegister (&HostPort);
#ifdef DWC_CFG_DYNAMIC_FIFO
	_DWHCIRegister (&HostPeriodicTxFIFOSize);
	_DWHCIRegister (&NonPeriodicTxFIFOSize);
	_DWHCIRegister (&RxFIFOSize);
#endif
	_DWHCIRegister (&USBConfig);
	_DWHCIRegister (&HWConfig2);
	_DWHCIRegister (&HostConfig);
	_DWHCIRegister (&Power);

	return TRUE;
}

boolean DWHCIDeviceEnableRootPort (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister HostPort;
	DWHCIRegister (&HostPort, DWHCI_HOST_PORT);
	if (!DWHCIDeviceWaitForBit (pThis, &HostPort, DWHCI_HOST_PORT_CONNECT, TRUE, 20))
	{
		_DWHCIRegister (&HostPort);

		return FALSE;
	}
	
	MsDelay (100);			// see USB 2.0 spec

	DWHCIRegisterRead (&HostPort);
	DWHCIRegisterAnd (&HostPort, ~DWHCI_HOST_PORT_DEFAULT_MASK);
	DWHCIRegisterOr (&HostPort, DWHCI_HOST_PORT_RESET);
	DWHCIRegisterWrite (&HostPort);
	
	MsDelay (50);			// see USB 2.0 spec (tDRSTR)

	DWHCIRegisterRead (&HostPort);
	DWHCIRegisterAnd (&HostPort, ~DWHCI_HOST_PORT_DEFAULT_MASK);
	DWHCIRegisterAnd (&HostPort, ~DWHCI_HOST_PORT_RESET);
	DWHCIRegisterWrite (&HostPort);

	// normally 10ms, seems to be too short for some devices
	MsDelay (20);			// see USB 2.0 spec (tRSTRCY)

	_DWHCIRegister (&HostPort);

	return TRUE;
}

boolean DWHCIDeviceReset (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister Reset;
	DWHCIRegister2 (&Reset, DWHCI_CORE_RESET, 0);
	
	// wait for AHB master IDLE state
	if (!DWHCIDeviceWaitForBit (pThis, &Reset, DWHCI_CORE_RESET_AHB_IDLE, TRUE, 100))
	{
		_DWHCIRegister (&Reset);

		return FALSE;
	}
	
	// core soft reset
	DWHCIRegisterOr (&Reset, DWHCI_CORE_RESET_SOFT_RESET);
	DWHCIRegisterWrite (&Reset);

	if (!DWHCIDeviceWaitForBit (pThis, &Reset, DWHCI_CORE_RESET_SOFT_RESET, FALSE, 10))
	{
		_DWHCIRegister (&Reset);

		return FALSE;
	}
	
	MsDelay (100);

	_DWHCIRegister (&Reset);

	return TRUE;
}

void DWHCIDeviceEnableGlobalInterrupts (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister AHBConfig;
	DWHCIRegister (&AHBConfig, DWHCI_CORE_AHB_CFG);
	DWHCIRegisterRead (&AHBConfig);
	DWHCIRegisterOr (&AHBConfig, DWHCI_CORE_AHB_CFG_GLOBALINT_MASK);
	DWHCIRegisterWrite (&AHBConfig);

	_DWHCIRegister (&AHBConfig);
}

void DWHCIDeviceEnableCommonInterrupts (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister IntStatus;
	DWHCIRegister (&IntStatus, DWHCI_CORE_INT_STAT);	// Clear any pending interrupts
	DWHCIRegisterSetAll (&IntStatus);
	DWHCIRegisterWrite (&IntStatus);

#if 0	// not used
	TDWHCIRegister IntMask;
	DWHCIRegister2 (&IntMask, DWHCI_CORE_INT_MASK,   DWHCI_CORE_INT_MASK_MODE_MISMATCH
					              | DWHCI_CORE_INT_MASK_USB_SUSPEND
						      | DWHCI_CORE_INT_MASK_CON_ID_STS_CHNG
						      | DWHCI_CORE_INT_MASK_SESS_REQ_INTR
						      | DWHCI_CORE_INT_MASK_WKUP_INTR);
	DWHCIRegisterWrite (&IntMask);
	
	_DWHCIRegister (&IntMask);
#endif

	_DWHCIRegister (&IntStatus);
}

void DWHCIDeviceEnableHostInterrupts (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister IntMask;
	DWHCIRegister2 (&IntMask, DWHCI_CORE_INT_MASK, 0);
	DWHCIRegisterWrite (&IntMask);				// Disable all interrupts

	DWHCIDeviceEnableCommonInterrupts (pThis);

	DWHCIRegisterRead (&IntMask);
	DWHCIRegisterOr (&IntMask,   DWHCI_CORE_INT_MASK_HC_INTR
				//| DWHCI_CORE_INT_MASK_PORT_INTR
				//| DWHCI_CORE_INT_MASK_DISCONNECT
			);
	DWHCIRegisterWrite (&IntMask);
	
	_DWHCIRegister (&IntMask);
}

void DWHCIDeviceEnableChannelInterrupt (TDWHCIDevice *pThis, unsigned nChannel)
{
	assert (pThis != 0);

	TDWHCIRegister AllChanInterruptMask;
	DWHCIRegister (&AllChanInterruptMask, DWHCI_HOST_ALLCHAN_INT_MASK);

	uspi_EnterCritical ();

	DWHCIRegisterRead (&AllChanInterruptMask);
	DWHCIRegisterOr (&AllChanInterruptMask, 1 << nChannel);
	DWHCIRegisterWrite (&AllChanInterruptMask);

	uspi_LeaveCritical ();

	_DWHCIRegister (&AllChanInterruptMask);
}

void DWHCIDeviceDisableChannelInterrupt (TDWHCIDevice *pThis, unsigned nChannel)
{
	assert (pThis != 0);

	TDWHCIRegister AllChanInterruptMask;
	DWHCIRegister (&AllChanInterruptMask, DWHCI_HOST_ALLCHAN_INT_MASK);

	uspi_EnterCritical ();

	DWHCIRegisterRead (&AllChanInterruptMask);
	DWHCIRegisterAnd (&AllChanInterruptMask, ~(1 << nChannel));
	DWHCIRegisterWrite (&AllChanInterruptMask);

	uspi_LeaveCritical ();
	
	_DWHCIRegister (&AllChanInterruptMask);
}

void DWHCIDeviceFlushTxFIFO (TDWHCIDevice *pThis, unsigned nFIFO)
{
	assert (pThis != 0);

	TDWHCIRegister Reset;
	DWHCIRegister2 (&Reset, DWHCI_CORE_RESET, 0);
	DWHCIRegisterOr (&Reset, DWHCI_CORE_RESET_TX_FIFO_FLUSH);
	DWHCIRegisterAnd (&Reset, ~DWHCI_CORE_RESET_TX_FIFO_NUM__MASK);
	DWHCIRegisterOr (&Reset, nFIFO << DWHCI_CORE_RESET_TX_FIFO_NUM__SHIFT);
	DWHCIRegisterWrite (&Reset);

	if (DWHCIDeviceWaitForBit (pThis, &Reset, DWHCI_CORE_RESET_TX_FIFO_FLUSH, FALSE, 10))
	{
		usDelay (1);		// Wait for 3 PHY clocks
	}

	_DWHCIRegister (&Reset);
}

void DWHCIDeviceFlushRxFIFO (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister Reset;
	DWHCIRegister2 (&Reset, DWHCI_CORE_RESET, 0);
	DWHCIRegisterOr (&Reset, DWHCI_CORE_RESET_RX_FIFO_FLUSH);
	DWHCIRegisterWrite (&Reset);

	if (DWHCIDeviceWaitForBit (pThis, &Reset, DWHCI_CORE_RESET_RX_FIFO_FLUSH, FALSE, 10))
	{
		usDelay (1);			// Wait for 3 PHY clocks
	}

	_DWHCIRegister (&Reset);
}

boolean DWHCIDeviceTransferStage (TDWHCIDevice *pThis, TUSBRequest *pURB, boolean bIn, boolean bStatusStage)
{
	assert (pThis != 0);

	assert (pURB != 0);
	USBRequestSetCompletionRoutine (pURB, DWHCIDeviceCompletionRoutine, 0, pThis);

	assert (!pThis->m_bWaiting);
	pThis->m_bWaiting = TRUE;

	if (!DWHCIDeviceTransferStageAsync (pThis, pURB, bIn, bStatusStage))
	{
		pThis->m_bWaiting = FALSE;

		return FALSE;
	}

	while (pThis->m_bWaiting)
	{
		// do nothing
	}

	return USBRequestGetStatus (pURB);
}

void DWHCIDeviceCompletionRoutine (TUSBRequest *pURB, void *pParam, void *pContext)
{
	TDWHCIDevice *pThis = (TDWHCIDevice *) pContext;
	assert (pThis != 0);

	pThis->m_bWaiting = FALSE;
}

boolean DWHCIDeviceTransferStageAsync (TDWHCIDevice *pThis, TUSBRequest *pURB, boolean bIn, boolean bStatusStage)
{
	assert (pThis != 0);
	assert (pURB != 0);
	
	unsigned nChannel = DWHCIDeviceAllocateChannel (pThis);
	if (nChannel >= pThis->m_nChannels)
	{
		return FALSE;
	}
	
	TDWHCITransferStageData *pStageData =
		(TDWHCITransferStageData *) malloc (sizeof (TDWHCITransferStageData));
	assert (pStageData != 0);
	DWHCITransferStageData (pStageData, nChannel, pURB, bIn, bStatusStage);

	assert (pThis->m_pStageData[nChannel] == 0);
	pThis->m_pStageData[nChannel] = pStageData;

	DWHCIDeviceEnableChannelInterrupt (pThis, nChannel);
	
	if (!DWHCITransferStageDataIsSplit (pStageData))
	{
		DWHCITransferStageDataSetState (pStageData, StageStateNoSplitTransfer);
	}
	else
	{
		if (!DWHCITransferStageDataBeginSplitCycle (pStageData))
		{
			DWHCIDeviceDisableChannelInterrupt (pThis, nChannel);

			_DWHCITransferStageData (pStageData);
			free (pStageData);

			pThis->m_pStageData[nChannel] = 0;
			
			DWHCIDeviceFreeChannel (pThis, nChannel);
			
			return FALSE;
		}

		DWHCITransferStageDataSetState (pStageData, StageStateStartSplit);
		DWHCITransferStageDataSetSplitComplete (pStageData, FALSE);
		
		TDWHCIFrameScheduler *pFrameScheduler =
			DWHCITransferStageDataGetFrameScheduler (pStageData);
		assert (pFrameScheduler != 0);
		pFrameScheduler->StartSplit (pFrameScheduler);
	}

	DWHCIDeviceStartTransaction (pThis, pStageData);
	
	return TRUE;
}

void DWHCIDeviceStartTransaction (TDWHCIDevice *pThis, TDWHCITransferStageData *pStageData)
{
	assert (pThis != 0);

	assert (pStageData != 0);
	unsigned nChannel = DWHCITransferStageDataGetChannelNumber (pStageData);
	assert (nChannel < pThis->m_nChannels);
	
	// channel must be disabled, if not already done but controller
	TDWHCIRegister Character;
	DWHCIRegister (&Character, DWHCI_HOST_CHAN_CHARACTER (nChannel));
	DWHCIRegisterRead (&Character);
	if (DWHCIRegisterIsSet (&Character, DWHCI_HOST_CHAN_CHARACTER_ENABLE))
	{
		DWHCITransferStageDataSetSubState (pStageData, StageSubStateWaitForChannelDisable);
		
		DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_ENABLE);
		DWHCIRegisterOr (&Character, DWHCI_HOST_CHAN_CHARACTER_DISABLE);
		DWHCIRegisterWrite (&Character);

		TDWHCIRegister ChanInterruptMask;
		DWHCIRegister (&ChanInterruptMask, DWHCI_HOST_CHAN_INT_MASK (nChannel));
		DWHCIRegisterSet (&ChanInterruptMask, DWHCI_HOST_CHAN_INT_HALTED);
		DWHCIRegisterWrite (&ChanInterruptMask);

		_DWHCIRegister (&ChanInterruptMask);
	}
	else
	{
		DWHCIDeviceStartChannel (pThis, pStageData);
	}

	_DWHCIRegister (&Character);
}

void DWHCIDeviceStartChannel (TDWHCIDevice *pThis, TDWHCITransferStageData *pStageData)
{
	assert (pThis != 0);

	assert (pStageData != 0);
	unsigned nChannel = DWHCITransferStageDataGetChannelNumber (pStageData);
	assert (nChannel < pThis->m_nChannels);
	
	DWHCITransferStageDataSetSubState (pStageData, StageSubStateWaitForTransactionComplete);

	// reset all pending channel interrupts
	TDWHCIRegister ChanInterrupt;
	DWHCIRegister (&ChanInterrupt, DWHCI_HOST_CHAN_INT (nChannel));
	DWHCIRegisterSetAll (&ChanInterrupt);
	DWHCIRegisterWrite (&ChanInterrupt);
	
	// set transfer size, packet count and pid
	TDWHCIRegister TransferSize;
	DWHCIRegister2 (&TransferSize, DWHCI_HOST_CHAN_XFER_SIZ (nChannel), 0);
	DWHCIRegisterOr (&TransferSize, DWHCITransferStageDataGetBytesToTransfer (pStageData) & DWHCI_HOST_CHAN_XFER_SIZ_BYTES__MASK);
	DWHCIRegisterOr (&TransferSize, (DWHCITransferStageDataGetPacketsToTransfer (pStageData) << DWHCI_HOST_CHAN_XFER_SIZ_PACKETS__SHIFT)
					& DWHCI_HOST_CHAN_XFER_SIZ_PACKETS__MASK);
	DWHCIRegisterOr (&TransferSize, DWHCITransferStageDataGetPID (pStageData) << DWHCI_HOST_CHAN_XFER_SIZ_PID__SHIFT);
	DWHCIRegisterWrite (&TransferSize);

	// set DMA address
	TDWHCIRegister DMAAddress;
	DWHCIRegister2 (&DMAAddress, DWHCI_HOST_CHAN_DMA_ADDR (nChannel),
			DWHCITransferStageDataGetDMAAddress (pStageData) + GPU_MEM_BASE);
	DWHCIRegisterWrite (&DMAAddress);

#if RASPPI == 1
	CleanDataCache ();
	InvalidateDataCache ();
#else
	uspi_CleanAndInvalidateDataCacheRange (DWHCITransferStageDataGetDMAAddress (pStageData),
					       DWHCITransferStageDataGetBytesToTransfer (pStageData));
#endif
	DataMemBarrier ();

	// set split control
	TDWHCIRegister SplitControl;
	DWHCIRegister2 (&SplitControl, DWHCI_HOST_CHAN_SPLIT_CTRL (nChannel), 0);
	if (DWHCITransferStageDataIsSplit (pStageData))
	{
		DWHCIRegisterOr (&SplitControl, DWHCITransferStageDataGetHubPortAddress (pStageData));
		DWHCIRegisterOr (&SplitControl,    DWHCITransferStageDataGetHubAddress (pStageData)
						<< DWHCI_HOST_CHAN_SPLIT_CTRL_HUB_ADDRESS__SHIFT);
		DWHCIRegisterOr (&SplitControl,    DWHCITransferStageDataGetSplitPosition (pStageData)
						<< DWHCI_HOST_CHAN_SPLIT_CTRL_XACT_POS__SHIFT);
		if (DWHCITransferStageDataIsSplitComplete (pStageData))
		{
			DWHCIRegisterOr (&SplitControl, DWHCI_HOST_CHAN_SPLIT_CTRL_COMPLETE_SPLIT);
		}
		DWHCIRegisterOr (&SplitControl, DWHCI_HOST_CHAN_SPLIT_CTRL_SPLIT_ENABLE);
	}
	DWHCIRegisterWrite (&SplitControl);

	// set channel parameters
	TDWHCIRegister Character;
	DWHCIRegister (&Character, DWHCI_HOST_CHAN_CHARACTER (nChannel));
	DWHCIRegisterRead (&Character);
	DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_MAX_PKT_SIZ__MASK);
	DWHCIRegisterOr (&Character, DWHCITransferStageDataGetMaxPacketSize (pStageData) & DWHCI_HOST_CHAN_CHARACTER_MAX_PKT_SIZ__MASK);

	DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_MULTI_CNT__MASK);
	DWHCIRegisterOr (&Character, 1 << DWHCI_HOST_CHAN_CHARACTER_MULTI_CNT__SHIFT);	// TODO: optimize

	if (DWHCITransferStageDataIsDirectionIn (pStageData))
	{
		DWHCIRegisterOr (&Character, DWHCI_HOST_CHAN_CHARACTER_EP_DIRECTION_IN);
	}
	else
	{
		DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_EP_DIRECTION_IN);
	}

	if (DWHCITransferStageDataGetSpeed (pStageData) == USBSpeedLow)
	{
		DWHCIRegisterOr (&Character, DWHCI_HOST_CHAN_CHARACTER_LOW_SPEED_DEVICE);
	}
	else
	{
		DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_LOW_SPEED_DEVICE);
	}

	DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_DEVICE_ADDRESS__MASK);
	DWHCIRegisterOr (&Character, DWHCITransferStageDataGetDeviceAddress (pStageData) << DWHCI_HOST_CHAN_CHARACTER_DEVICE_ADDRESS__SHIFT);

	DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_EP_TYPE__MASK);
	DWHCIRegisterOr (&Character, DWHCITransferStageDataGetEndpointType (pStageData) << DWHCI_HOST_CHAN_CHARACTER_EP_TYPE__SHIFT);

	DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_EP_NUMBER__MASK);
	DWHCIRegisterOr (&Character, DWHCITransferStageDataGetEndpointNumber (pStageData) << DWHCI_HOST_CHAN_CHARACTER_EP_NUMBER__SHIFT);

	TDWHCIFrameScheduler *pFrameScheduler = DWHCITransferStageDataGetFrameScheduler (pStageData);
	if (pFrameScheduler != 0)
	{
		pFrameScheduler->WaitForFrame (pFrameScheduler);

		if (pFrameScheduler->IsOddFrame (pFrameScheduler))
		{
			DWHCIRegisterOr (&Character, DWHCI_HOST_CHAN_CHARACTER_PER_ODD_FRAME);
		}
		else
		{
			DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_PER_ODD_FRAME);
		}
	}

	TDWHCIRegister ChanInterruptMask;
	DWHCIRegister (&ChanInterruptMask, DWHCI_HOST_CHAN_INT_MASK (nChannel));
	DWHCIRegisterSet (&ChanInterruptMask, DWHCITransferStageDataGetStatusMask (pStageData));
	DWHCIRegisterWrite (&ChanInterruptMask);
	
	DWHCIRegisterOr (&Character, DWHCI_HOST_CHAN_CHARACTER_ENABLE);
	DWHCIRegisterAnd (&Character, ~DWHCI_HOST_CHAN_CHARACTER_DISABLE);
	DWHCIRegisterWrite (&Character);

	_DWHCIRegister (&ChanInterruptMask);
	_DWHCIRegister (&Character);
	_DWHCIRegister (&SplitControl);
	_DWHCIRegister (&DMAAddress);
	_DWHCIRegister (&TransferSize);
	_DWHCIRegister (&ChanInterrupt);
}

void DWHCIDeviceChannelInterruptHandler (TDWHCIDevice *pThis, unsigned nChannel)
{
	assert (pThis != 0);

	TDWHCITransferStageData *pStageData = pThis->m_pStageData[nChannel];
	assert (pStageData != 0);
	TDWHCIFrameScheduler *pFrameScheduler = DWHCITransferStageDataGetFrameScheduler (pStageData);
	TUSBRequest *pURB = DWHCITransferStageDataGetURB (pStageData);
	assert (pURB != 0);

	switch (DWHCITransferStageDataGetSubState (pStageData))
	{
	case StageSubStateWaitForChannelDisable:
		DWHCIDeviceStartChannel (pThis, pStageData);
		return;

	case StageSubStateWaitForTransactionComplete: {
#if RASPPI == 1
		CleanDataCache ();
		InvalidateDataCache ();
#else
		uspi_CleanAndInvalidateDataCacheRange (DWHCITransferStageDataGetDMAAddress (pStageData),
						       DWHCITransferStageDataGetBytesToTransfer (pStageData));
#endif
		DataMemBarrier ();

		TDWHCIRegister TransferSize;
		DWHCIRegister (&TransferSize, DWHCI_HOST_CHAN_XFER_SIZ (nChannel));
		DWHCIRegisterRead (&TransferSize);

		TDWHCIRegister ChanInterrupt;
		DWHCIRegister (&ChanInterrupt, DWHCI_HOST_CHAN_INT (nChannel));

		assert (   !DWHCITransferStageDataIsPeriodic (pStageData)
			||    DWHCI_HOST_CHAN_XFER_SIZ_PID (DWHCIRegisterGet (&TransferSize))
			   != DWHCI_HOST_CHAN_XFER_SIZ_PID_MDATA);

		DWHCITransferStageDataTransactionComplete (pStageData, DWHCIRegisterRead (&ChanInterrupt),
			DWHCI_HOST_CHAN_XFER_SIZ_PACKETS (DWHCIRegisterGet (&TransferSize)),
			DWHCIRegisterGet (&TransferSize) & DWHCI_HOST_CHAN_XFER_SIZ_BYTES__MASK);

		_DWHCIRegister (&ChanInterrupt);
		_DWHCIRegister (&TransferSize);
		} break;
	
	default:
		assert (0);
		break;
	}
	
	unsigned nStatus;
	
	switch (DWHCITransferStageDataGetState (pStageData))
	{
	case StageStateNoSplitTransfer:
		nStatus = DWHCITransferStageDataGetTransactionStatus (pStageData);
		if (nStatus & DWHCI_HOST_CHAN_INT_ERROR_MASK)
		{
			LogWrite (FromDWHCI, LOG_ERROR, "Transaction failed (status 0x%X)", nStatus);

			USBRequestSetStatus (pURB, 0);
		}
		else if (   (nStatus & (DWHCI_HOST_CHAN_INT_NAK | DWHCI_HOST_CHAN_INT_NYET))
			 && DWHCITransferStageDataIsPeriodic (pStageData))
		{
			DWHCITransferStageDataSetState (pStageData, StageStatePeriodicDelay);

			unsigned nInterval = USBEndpointGetInterval (USBRequestGetEndpoint (pURB));

			StartKernelTimer (MSEC2HZ (nInterval), DWHCIDeviceTimerHandler, pStageData, pThis);

			break;
		}
		else
		{
			if (!DWHCITransferStageDataIsStatusStage (pStageData))
			{
				USBRequestSetResultLen (pURB, DWHCITransferStageDataGetResultLen (pStageData));
			}

			USBRequestSetStatus (pURB, 1);
		}

		DWHCIDeviceDisableChannelInterrupt (pThis, nChannel);
	
		_DWHCITransferStageData (pStageData);
		free (pStageData);
		pThis->m_pStageData[nChannel] = 0;

		DWHCIDeviceFreeChannel (pThis, nChannel);

		USBRequestCallCompletionRoutine (pURB);
		break;

	case StageStateStartSplit:
		nStatus = DWHCITransferStageDataGetTransactionStatus (pStageData);
		if (   (nStatus & DWHCI_HOST_CHAN_INT_ERROR_MASK)
		    || (nStatus & DWHCI_HOST_CHAN_INT_NAK)
		    || (nStatus & DWHCI_HOST_CHAN_INT_NYET))
		{
			LogWrite (FromDWHCI, LOG_ERROR, "Transaction failed (status 0x%X)", nStatus);

			USBRequestSetStatus (pURB, 0);

			DWHCIDeviceDisableChannelInterrupt (pThis, nChannel);

			_DWHCITransferStageData (pStageData);
			free (pStageData);
			pThis->m_pStageData[nChannel] = 0;

			DWHCIDeviceFreeChannel (pThis, nChannel);

			USBRequestCallCompletionRoutine (pURB);
			break;
		}

		pFrameScheduler->TransactionComplete (pFrameScheduler, nStatus);

		DWHCITransferStageDataSetState (pStageData, StageStateCompleteSplit);
		DWHCITransferStageDataSetSplitComplete (pStageData, TRUE);

		if (!pFrameScheduler->CompleteSplit (pFrameScheduler))
		{
			goto LeaveCompleteSplit;
		}
		
		DWHCIDeviceStartTransaction (pThis, pStageData);
		break;
		
	case StageStateCompleteSplit:
		nStatus = DWHCITransferStageDataGetTransactionStatus (pStageData);
		if (nStatus & DWHCI_HOST_CHAN_INT_ERROR_MASK)
		{
			LogWrite (FromDWHCI, LOG_ERROR, "Transaction failed (status 0x%X)", nStatus);

			USBRequestSetStatus (pURB, 0);

			DWHCIDeviceDisableChannelInterrupt (pThis, nChannel);

			_DWHCITransferStageData (pStageData);
			free (pStageData);
			pThis->m_pStageData[nChannel] = 0;

			DWHCIDeviceFreeChannel (pThis, nChannel);

			USBRequestCallCompletionRoutine (pURB);
			break;
		}
		
		pFrameScheduler->TransactionComplete (pFrameScheduler, nStatus);

		if (pFrameScheduler->CompleteSplit (pFrameScheduler))
		{
			DWHCIDeviceStartTransaction (pThis, pStageData);
			break;
		}

	LeaveCompleteSplit:
		if (!DWHCITransferStageDataIsStageComplete (pStageData))
		{
			if (!DWHCITransferStageDataBeginSplitCycle (pStageData))
			{
				USBRequestSetStatus (pURB, 0);

				DWHCIDeviceDisableChannelInterrupt (pThis, nChannel);

				_DWHCITransferStageData (pStageData);
				free (pStageData);
				pThis->m_pStageData[nChannel] = 0;

				DWHCIDeviceFreeChannel (pThis, nChannel);

				USBRequestCallCompletionRoutine (pURB);
				break;
			}

			if (!DWHCITransferStageDataIsPeriodic (pStageData))
			{
				DWHCITransferStageDataSetState (pStageData, StageStateStartSplit);
				DWHCITransferStageDataSetSplitComplete (pStageData, FALSE);

				pFrameScheduler->StartSplit (pFrameScheduler);

				DWHCIDeviceStartTransaction (pThis, pStageData);
			}
			else
			{
				DWHCITransferStageDataSetState (pStageData, StageStatePeriodicDelay);

				unsigned nInterval = USBEndpointGetInterval (USBRequestGetEndpoint (pURB));

				StartKernelTimer (MSEC2HZ (nInterval), DWHCIDeviceTimerHandler, pStageData, pThis);
			}
			break;
		}

		DWHCIDeviceDisableChannelInterrupt (pThis, nChannel);

		if (!DWHCITransferStageDataIsStatusStage (pStageData))
		{
			USBRequestSetResultLen (pURB, DWHCITransferStageDataGetResultLen (pStageData));
		}
		USBRequestSetStatus (pURB, 1);

		_DWHCITransferStageData (pStageData);
		free (pStageData);
		pThis->m_pStageData[nChannel] = 0;

		DWHCIDeviceFreeChannel (pThis, nChannel);

		USBRequestCallCompletionRoutine (pURB);
		break;

	default:
		assert (0);
		break;
	}
}

void DWHCIDeviceInterruptHandler (void *pParam)
{
	TDWHCIDevice *pThis = (TDWHCIDevice *) pParam;
	assert (pThis != 0);

	DataMemBarrier ();

	TDWHCIRegister IntStatus;
	DWHCIRegister (&IntStatus, DWHCI_CORE_INT_STAT);
	DWHCIRegisterRead (&IntStatus);

	if (DWHCIRegisterGet (&IntStatus) & DWHCI_CORE_INT_STAT_HC_INTR)
	{
		TDWHCIRegister AllChanInterrupt;
		DWHCIRegister (&AllChanInterrupt, DWHCI_HOST_ALLCHAN_INT);
		DWHCIRegisterRead (&AllChanInterrupt);
		DWHCIRegisterWrite (&AllChanInterrupt);
		
		unsigned nChannelMask = 1;
		for (unsigned nChannel = 0; nChannel < pThis->m_nChannels; nChannel++)
		{
			if (DWHCIRegisterGet (&AllChanInterrupt) & nChannelMask)
			{
				TDWHCIRegister ChanInterruptMask;
				DWHCIRegister2 (&ChanInterruptMask, DWHCI_HOST_CHAN_INT_MASK(nChannel), 0);
				DWHCIRegisterWrite (&ChanInterruptMask);
				
				DWHCIDeviceChannelInterruptHandler (pThis, nChannel);

				_DWHCIRegister (&ChanInterruptMask);
			}
			
			nChannelMask <<= 1;
		}

		_DWHCIRegister (&AllChanInterrupt);
	}
#if 0	
	if (IntStatus.Get () & DWHCI_CORE_INT_STAT_PORT_INTR)
	{
		CDWHCIRegister HostPort (DWHCI_HOST_PORT);
		HostPort.Read ();
		
		CLogger::Get ()->Write (FromDWHCI, LOG_DEBUG, "Port interrupt (status 0x%08X)", HostPort.Get ());
		
		HostPort.And (~DWHCI_HOST_PORT_ENABLE);
		HostPort.Or (  DWHCI_HOST_PORT_CONNECT_CHANGED
			     | DWHCI_HOST_PORT_ENABLE_CHANGED
			     | DWHCI_HOST_PORT_OVERCURRENT_CHANGED);
		HostPort.Write ();
		
		IntStatus.Or (DWHCI_CORE_INT_STAT_PORT_INTR);
	}
#endif
	DWHCIRegisterWrite (&IntStatus);

	DataMemBarrier ();
	
	_DWHCIRegister (&IntStatus);
}

void DWHCIDeviceTimerHandler (unsigned hTimer, void *pParam, void *pContext)
{
	TDWHCIDevice *pThis = (TDWHCIDevice *) pContext;
	assert (pThis != 0);
	
	TDWHCITransferStageData *pStageData = (TDWHCITransferStageData *) pParam;
	assert (pStageData != 0);
	
	DataMemBarrier ();

	assert (pStageData != 0);
	assert (DWHCITransferStageDataGetState (pStageData) == StageStatePeriodicDelay);

	if (DWHCITransferStageDataIsSplit (pStageData))
	{
		DWHCITransferStageDataSetState (pStageData, StageStateStartSplit);
		
		DWHCITransferStageDataSetSplitComplete (pStageData, FALSE);
		TDWHCIFrameScheduler *pFrameScheduler =
			DWHCITransferStageDataGetFrameScheduler (pStageData);
		assert (pFrameScheduler != 0);
		pFrameScheduler->StartSplit (pFrameScheduler);
	}
	else
	{
		DWHCITransferStageDataSetState (pStageData, StageStateNoSplitTransfer);
	}

	DWHCIDeviceStartTransaction (pThis, pStageData);

	DataMemBarrier ();
}

unsigned DWHCIDeviceAllocateChannel (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	uspi_EnterCritical ();

	unsigned nChannelMask = 1;
	for (unsigned nChannel = 0; nChannel < pThis->m_nChannels; nChannel++)
	{
		if (!(pThis->m_nChannelAllocated & nChannelMask))
		{
			pThis->m_nChannelAllocated |= nChannelMask;

			uspi_LeaveCritical ();
			
			return nChannel;
		}
		
		nChannelMask <<= 1;
	}
	
	uspi_LeaveCritical ();
	
	return DWHCI_MAX_CHANNELS;
}

void DWHCIDeviceFreeChannel (TDWHCIDevice *pThis, unsigned nChannel)
{
	assert (pThis != 0);

	assert (nChannel < pThis->m_nChannels);
	unsigned nChannelMask = 1 << nChannel; 
	
	uspi_EnterCritical ();
	
	assert (pThis->m_nChannelAllocated & nChannelMask);
	pThis->m_nChannelAllocated &= ~nChannelMask;
	
	uspi_LeaveCritical ();
}

boolean DWHCIDeviceWaitForBit (TDWHCIDevice *pThis, TDWHCIRegister *pRegister, u32 nMask, boolean bWaitUntilSet, unsigned nMsTimeout)
{
	assert (pThis != 0);

	assert (pRegister != 0);
	assert (nMask != 0);
	assert (nMsTimeout > 0);

	while ((DWHCIRegisterRead (pRegister) & nMask) ? !bWaitUntilSet : bWaitUntilSet)
	{
		MsDelay (1);

		if (--nMsTimeout == 0)
		{
			//LogWrite (FromDWHCI, LOG_WARNING, "Timeout");
#ifndef NDEBUG
			//DWHCIRegisterDump (pRegister);
#endif
			return FALSE;
		}
	}
	
	return TRUE;
}

TUSBSpeed DWHCIDeviceGetPortSpeed (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TUSBSpeed Result = USBSpeedUnknown;
	
	TDWHCIRegister HostPort;
	DWHCIRegister (&HostPort, DWHCI_HOST_PORT);

	switch (DWHCI_HOST_PORT_SPEED (DWHCIRegisterRead (&HostPort)))
	{
	case DWHCI_HOST_PORT_SPEED_HIGH:
		Result = USBSpeedHigh;
		break;

	case DWHCI_HOST_PORT_SPEED_FULL:
		Result = USBSpeedFull;
		break;

	case DWHCI_HOST_PORT_SPEED_LOW:
		Result = USBSpeedLow;
		break;

	default:
		break;
	}

	_DWHCIRegister (&HostPort);

	return Result;
}

boolean DWHCIDeviceOvercurrentDetected (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister HostPort;
	DWHCIRegister (&HostPort, DWHCI_HOST_PORT);

	if (DWHCIRegisterRead (&HostPort) & DWHCI_HOST_PORT_OVERCURRENT)
	{
		_DWHCIRegister (&HostPort);

		return TRUE;
	}

	_DWHCIRegister (&HostPort);

	return FALSE;
}

void DWHCIDeviceDisableRootPort (TDWHCIDevice *pThis)
{
	assert (pThis != 0);

	TDWHCIRegister HostPort;
	DWHCIRegister (&HostPort, DWHCI_HOST_PORT);

	DWHCIRegisterRead (&HostPort);
	DWHCIRegisterAnd (&HostPort, ~DWHCI_HOST_PORT_POWER);
	DWHCIRegisterWrite (&HostPort);

	_DWHCIRegister (&HostPort);
}

#ifndef NDEBUG

void DWHCIDeviceDumpRegister (TDWHCIDevice *pThis, const char *pName, u32 nAddress)
{
	assert (pThis != 0);

	TDWHCIRegister Register;
	DWHCIRegister (&Register, nAddress);

	DataMemBarrier ();

	LogWrite (FromDWHCI, LOG_DEBUG, "0x%08X %s", DWHCIRegisterRead (&Register), pName);

	_DWHCIRegister (&Register);
}

void DWHCIDeviceDumpStatus (TDWHCIDevice *pThis, unsigned nChannel)
{
	assert (pThis != 0);

	DWHCIDeviceDumpRegister (pThis, "OTG_CTRL",                DWHCI_CORE_OTG_CTRL);
	DWHCIDeviceDumpRegister (pThis, "AHB_CFG",                 DWHCI_CORE_AHB_CFG);
	DWHCIDeviceDumpRegister (pThis, "USB_CFG",                 DWHCI_CORE_USB_CFG);
	DWHCIDeviceDumpRegister (pThis, "RESET",                   DWHCI_CORE_RESET);
	DWHCIDeviceDumpRegister (pThis, "INT_STAT",                DWHCI_CORE_INT_STAT);
	DWHCIDeviceDumpRegister (pThis, "INT_MASK",                DWHCI_CORE_INT_MASK);
	DWHCIDeviceDumpRegister (pThis, "RX_FIFO_SIZ",             DWHCI_CORE_RX_FIFO_SIZ);
	DWHCIDeviceDumpRegister (pThis, "NPER_TX_FIFO_SIZ",        DWHCI_CORE_NPER_TX_FIFO_SIZ);
	DWHCIDeviceDumpRegister (pThis, "NPER_TX_STAT",            DWHCI_CORE_NPER_TX_STAT);
	DWHCIDeviceDumpRegister (pThis, "HOST_PER_TX_FIFO_SIZ",    DWHCI_CORE_HOST_PER_TX_FIFO_SIZ);

	DWHCIDeviceDumpRegister (pThis, "HOST_CFG",                DWHCI_HOST_CFG);
	DWHCIDeviceDumpRegister (pThis, "HOST_PER_TX_FIFO_STAT",   DWHCI_HOST_PER_TX_FIFO_STAT);
	DWHCIDeviceDumpRegister (pThis, "HOST_ALLCHAN_INT",        DWHCI_HOST_ALLCHAN_INT);
	DWHCIDeviceDumpRegister (pThis, "HOST_ALLCHAN_INT_MASK",   DWHCI_HOST_ALLCHAN_INT_MASK);
	DWHCIDeviceDumpRegister (pThis, "HOST_PORT",               DWHCI_HOST_PORT);

	DWHCIDeviceDumpRegister (pThis, "HOST_CHAN_CHARACTER(n)",  DWHCI_HOST_CHAN_CHARACTER (nChannel));
	DWHCIDeviceDumpRegister (pThis, "HOST_CHAN_SPLIT_CTRL(n)", DWHCI_HOST_CHAN_SPLIT_CTRL (nChannel));
	DWHCIDeviceDumpRegister (pThis, "HOST_CHAN_INT(n)",        DWHCI_HOST_CHAN_INT (nChannel));
	DWHCIDeviceDumpRegister (pThis, "HOST_CHAN_INT_MASK(n)",   DWHCI_HOST_CHAN_INT_MASK (nChannel));
	DWHCIDeviceDumpRegister (pThis, "HOST_CHAN_XFER_SIZ(n)",   DWHCI_HOST_CHAN_XFER_SIZ (nChannel));
	DWHCIDeviceDumpRegister (pThis, "HOST_CHAN_DMA_ADDR(n)",   DWHCI_HOST_CHAN_DMA_ADDR (nChannel));
}

#endif
