//
// smsc951x.c
//
// Information to implement this driver was taken
// from the Linux smsc95xx driver which is:
// 	Copyright (C) 2007-2008 SMSC
// and the Embedded Xinu SMSC LAN9512 USB driver which is:
//	Copyright (c) 2008, Douglas Comer and Dennis Brylow
// See the file lib/README for details!
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
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
#include <uspi/smsc951x.h>
#include <uspios.h>
#include <uspi/usbhostcontroller.h>
#include <uspi/devicenameservice.h>
#include <uspi/util.h>
#include <uspi/assert.h>

// USB vendor requests
#define WRITE_REGISTER			0xA0
#define READ_REGISTER			0xA1

// Registers
#define ID_REV				0x00
#define INT_STS				0x08
#define RX_CFG				0x0C
#define TX_CFG				0x10
	#define TX_CFG_ON			0x00000004
#define HW_CFG				0x14
	#define HW_CFG_BIR			0x00001000
#define RX_FIFO_INF			0x18
#define PM_CTRL				0x20
#define LED_GPIO_CFG			0x24
	#define LED_GPIO_CFG_SPD_LED		0x01000000
	#define LED_GPIO_CFG_LNK_LED		0x00100000
	#define LED_GPIO_CFG_FDX_LED		0x00010000
#define GPIO_CFG			0x28
#define AFC_CFG				0x2C
#define E2P_CMD				0x30
#define E2P_DATA			0x34
#define BURST_CAP			0x38
#define GPIO_WAKE			0x64
#define INT_EP_CTL			0x68
#define BULK_IN_DLY			0x6C
#define MAC_CR				0x100
	#define MAC_CR_RCVOWN			0x00800000
	#define MAC_CR_MCPAS			0x00080000
	#define MAC_CR_PRMS			0x00040000
	#define MAC_CR_BCAST			0x00000800
	#define MAC_CR_TXEN			0x00000008
	#define MAC_CR_RXEN			0x00000004
#define ADDRH				0x104
#define ADDRL				0x108
#define HASHH				0x10C
#define HASHL				0x110
#define MII_ADDR			0x114
	#define MII_BUSY			0x01
	#define MII_WRITE			0x02
	#define PHY_ID_MASK			0x1F
	#define PHY_ID_INTERNAL			0x01
	#define REG_NUM_MASK			0x1F
#define MII_DATA			0x118
#define FLOW				0x11C
#define VLAN1				0x120
#define VLAN2				0x124
#define WUFF				0x128
#define WUCSR				0x12C
#define COE_CR				0x130

// TX commands (first two 32-bit words in buffer)
#define TX_CMD_A_DATA_OFFSET		0x001F0000
#define TX_CMD_A_FIRST_SEG		0x00002000
#define TX_CMD_A_LAST_SEG		0x00001000
#define TX_CMD_A_BUF_SIZE		0x000007FF

#define TX_CMD_B_CSUM_ENABLE		0x00004000
#define TX_CMD_B_ADD_CRC_DISABLE	0x00002000
#define TX_CMD_B_DISABLE_PADDING	0x00001000
#define TX_CMD_B_PKT_BYTE_LENGTH	0x000007FF

// RX status (first 32-bit word in buffer)
#define RX_STS_FF			0x40000000	// Filter Fail
#define RX_STS_FL			0x3FFF0000	// Frame Length
	#define RX_STS_FRAMELEN(reg)	(((reg) & RX_STS_FL) >> 16)
#define RX_STS_ES			0x00008000	// Error Summary
#define RX_STS_BF			0x00002000	// Broadcast Frame
#define RX_STS_LE			0x00001000	// Length Error
#define RX_STS_RF			0x00000800	// Runt Frame
#define RX_STS_MF			0x00000400	// Multicast Frame
#define RX_STS_TL			0x00000080	// Frame too long
#define RX_STS_CS			0x00000040	// Collision Seen
#define RX_STS_FT			0x00000020	// Frame Type
#define RX_STS_RW			0x00000010	// Receive Watchdog
#define RX_STS_ME			0x00000008	// Mii Error
#define RX_STS_DB			0x00000004	// Dribbling
#define RX_STS_CRC			0x00000002	// CRC Error

#define RX_STS_ERROR			(  RX_STS_FF	\
					 | RX_STS_ES	\
					 | RX_STS_LE	\
					 | RX_STS_TL	\
					 | RX_STS_CS	\
					 | RX_STS_RW	\
					 | RX_STS_ME	\
					 | RX_STS_DB	\
					 | RX_STS_CRC)

static const char FromSMSC951x[] = "smsc951x";

static unsigned s_nDeviceNumber = 0;

boolean SMSC951xDeviceWriteReg (TSMSC951xDevice *pThis, u32 nIndex, u32 nValue);
boolean SMSC951xDeviceReadReg (TSMSC951xDevice *pThis, u32 nIndex, u32 *pValue);
#ifndef NDEBUG
void SMSC951xDeviceDumpReg (TSMSC951xDevice *pThis, const char *pName, u32 nIndex);
void SMSC951xDeviceDumpRegs (TSMSC951xDevice *pThis);
#endif

void SMSC951xDevice (TSMSC951xDevice *pThis, TUSBDevice *pDevice)
{
	assert (pThis != 0);

	USBDeviceCopy (&pThis->m_USBDevice, pDevice);
	pThis->m_USBDevice.Configure = SMSC951xDeviceConfigure;

	pThis->m_pEndpointBulkIn = 0;
	pThis->m_pEndpointBulkOut = 0;
	pThis->m_pTxBuffer = 0;

	pThis->m_pTxBuffer = malloc (FRAME_BUFFER_SIZE);
	assert (pThis->m_pTxBuffer != 0);
}

void _SMSC951xDevice (TSMSC951xDevice *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pTxBuffer != 0)
	{
		free (pThis->m_pTxBuffer);
		pThis->m_pTxBuffer = 0;
	}
	
	if (pThis->m_pEndpointBulkOut != 0)
	{
		_USBEndpoint (pThis->m_pEndpointBulkOut);
		free (pThis->m_pEndpointBulkOut);
		pThis->m_pEndpointBulkOut = 0;
	}
	
	if (pThis->m_pEndpointBulkIn != 0)
	{
		_USBEndpoint (pThis->m_pEndpointBulkIn);
		free (pThis->m_pEndpointBulkIn);
		pThis->m_pEndpointBulkIn = 0;
	}
	
	_USBDevice (&pThis->m_USBDevice);
}

boolean SMSC951xDeviceConfigure (TUSBDevice *pUSBDevice)
{
	TSMSC951xDevice *pThis = (TSMSC951xDevice *) pUSBDevice;
	assert (pThis != 0);

	u8 MACAddress[MAC_ADDRESS_SIZE];
	if (GetMACAddress (MACAddress))
	{
		MACAddressSet (&pThis->m_MACAddress, MACAddress);
	}
	else
	{
		LogWrite (FromSMSC951x, LOG_ERROR, "Cannot get MAC address");

		return FALSE;
	}
	TString MACString;
	String (&MACString);
	MACAddressFormat (&pThis->m_MACAddress, &MACString);
	LogWrite (FromSMSC951x, LOG_DEBUG, "MAC address is %s", StringGet (&MACString));

	const TUSBConfigurationDescriptor *pConfigDesc =
		(TUSBConfigurationDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_CONFIGURATION);
	if (   pConfigDesc == 0
	    || pConfigDesc->bNumInterfaces != 1)
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromSMSC951x);

		_String (&MACString);

		return FALSE;
	}

	const TUSBInterfaceDescriptor *pInterfaceDesc =
		(TUSBInterfaceDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_INTERFACE);
	if (   pInterfaceDesc == 0
	    || pInterfaceDesc->bInterfaceNumber	 != 0x00
	    || pInterfaceDesc->bAlternateSetting != 0x00
	    || pInterfaceDesc->bNumEndpoints	 != 3)
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromSMSC951x);

		_String (&MACString);

		return FALSE;
	}

	const TUSBEndpointDescriptor *pEndpointDesc;
	while ((pEndpointDesc = (TUSBEndpointDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_ENDPOINT)) != 0)
	{
		if ((pEndpointDesc->bmAttributes & 0x3F) == 0x02)		// Bulk
		{
			if ((pEndpointDesc->bEndpointAddress & 0x80) == 0x80)	// Input
			{
				if (pThis->m_pEndpointBulkIn != 0)
				{
					USBDeviceConfigurationError (&pThis->m_USBDevice, FromSMSC951x);

					_String (&MACString);

					return FALSE;
				}

				pThis->m_pEndpointBulkIn = (TUSBEndpoint *) malloc (sizeof (TUSBEndpoint));
				assert (pThis->m_pEndpointBulkIn);
				USBEndpoint2 (pThis->m_pEndpointBulkIn, &pThis->m_USBDevice, pEndpointDesc);
			}
			else							// Output
			{
				if (pThis->m_pEndpointBulkOut != 0)
				{
					USBDeviceConfigurationError (&pThis->m_USBDevice, FromSMSC951x);

					_String (&MACString);

					return FALSE;
				}

				pThis->m_pEndpointBulkOut = (TUSBEndpoint *) malloc (sizeof (TUSBEndpoint));
				assert (pThis->m_pEndpointBulkOut);
				USBEndpoint2 (pThis->m_pEndpointBulkOut, &pThis->m_USBDevice, pEndpointDesc);
			}
		}
	}

	if (   pThis->m_pEndpointBulkIn  == 0
	    || pThis->m_pEndpointBulkOut == 0)
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromSMSC951x);

		_String (&MACString);

		return FALSE;
	}

	if (!USBDeviceConfigure (&pThis->m_USBDevice))
	{
		LogWrite (FromSMSC951x, LOG_ERROR, "Cannot set configuration");

		_String (&MACString);

		return FALSE;
	}

	u8 MACAddressBuffer[MAC_ADDRESS_SIZE];
	MACAddressCopyTo (&pThis->m_MACAddress, MACAddressBuffer);
	u16 usMACAddressHigh = *(u16 *) &MACAddressBuffer[4];
	u32 nMACAddressLow   = *(u32 *) &MACAddressBuffer[0];
	if (   !SMSC951xDeviceWriteReg (pThis, ADDRH, usMACAddressHigh)
	    || !SMSC951xDeviceWriteReg (pThis, ADDRL, nMACAddressLow))
	{
		LogWrite (FromSMSC951x, LOG_ERROR, "Cannot set MAC address");

		_String (&MACString);

		return FALSE;
	}

	if (   !SMSC951xDeviceWriteReg (pThis, LED_GPIO_CFG,   LED_GPIO_CFG_SPD_LED
							     | LED_GPIO_CFG_LNK_LED
							     | LED_GPIO_CFG_FDX_LED)
	    || !SMSC951xDeviceWriteReg (pThis, MAC_CR,  MAC_CR_RCVOWN
						       //| MAC_CR_PRMS		// promiscous mode
						       | MAC_CR_TXEN
						       | MAC_CR_RXEN)
	    || !SMSC951xDeviceWriteReg (pThis, TX_CFG, TX_CFG_ON))
	{
		LogWrite (FromSMSC951x, LOG_ERROR, "Cannot start device");

		_String (&MACString);

		return FALSE;
	}

	// TODO: check if PHY is up (wait for it)

	TString DeviceName;
	String (&DeviceName);
	StringFormat (&DeviceName, "eth%u", s_nDeviceNumber++);
	DeviceNameServiceAddDevice (DeviceNameServiceGet (), StringGet (&DeviceName), pThis, FALSE);

	_String (&DeviceName);
	_String (&MACString);

	return TRUE;
}

TMACAddress *SMSC951xDeviceGetMACAddress (TSMSC951xDevice *pThis)
{
	assert (pThis != 0);

	return &pThis->m_MACAddress;
}

boolean SMSC951xDeviceSendFrame (TSMSC951xDevice *pThis, const void *pBuffer, unsigned nLength)
{
	assert (pThis != 0);

	if (nLength >= FRAME_BUFFER_SIZE-8)
	{
		return FALSE;
	}

	assert (pThis->m_pTxBuffer != 0);
	assert (pBuffer != 0);
	memcpy (pThis->m_pTxBuffer+8, pBuffer, nLength);
	
	*(u32 *) &pThis->m_pTxBuffer[0] = TX_CMD_A_FIRST_SEG | TX_CMD_A_LAST_SEG | nLength;
	*(u32 *) &pThis->m_pTxBuffer[4] = nLength;
	
	assert (pThis->m_pEndpointBulkOut != 0);
	return DWHCIDeviceTransfer (USBDeviceGetHost (&pThis->m_USBDevice), pThis->m_pEndpointBulkOut, pThis->m_pTxBuffer, nLength+8) >= 0;
}

boolean SMSC951xDeviceReceiveFrame (TSMSC951xDevice *pThis, void *pBuffer, unsigned *pResultLength)
{
	assert (pThis != 0);

	assert (pThis->m_pEndpointBulkIn != 0);
	assert (pBuffer != 0);
	TUSBRequest URB;
	USBRequest (&URB, pThis->m_pEndpointBulkIn, pBuffer, FRAME_BUFFER_SIZE, 0);

	if (!DWHCIDeviceSubmitBlockingRequest (USBDeviceGetHost (&pThis->m_USBDevice), &URB))
	{
		_USBRequest (&URB);

		return FALSE;
	}

	u32 nResultLength = USBRequestGetResultLength (&URB);
	if (nResultLength < 4)				// should not happen with HW_CFG_BIR set
	{
		_USBRequest (&URB);

		return FALSE;
	}

	u32 nRxStatus = *(u32 *) pBuffer;
	if (nRxStatus & RX_STS_ERROR)
	{
		LogWrite (FromSMSC951x, LOG_WARNING, "RX error (status 0x%X)", nRxStatus);

		_USBRequest (&URB);

		return FALSE;
	}
	
	u32 nFrameLength = RX_STS_FRAMELEN (nRxStatus);
	assert (nFrameLength == nResultLength-4);
	assert (nFrameLength > 4);
	if (nFrameLength <= 4)
	{
		_USBRequest (&URB);

		return FALSE;
	}
	nFrameLength -= 4;	// ignore CRC

	//LogWrite (FromSMSC951x, LOG_DEBUG, "Frame received (status 0x%X)", nRxStatus);

	memcpy (pBuffer, (u8 *) pBuffer + 4, nFrameLength);	// overwrite RX status

	assert (pResultLength != 0);
	*pResultLength = nFrameLength;
	
	_USBRequest (&URB);

	return TRUE;
}

boolean SMSC951xDeviceWriteReg (TSMSC951xDevice *pThis, u32 nIndex, u32 nValue)
{
	assert (pThis != 0);

	return DWHCIDeviceControlMessage (USBDeviceGetHost (&pThis->m_USBDevice),
					  USBDeviceGetEndpoint0 (&pThis->m_USBDevice),
					  REQUEST_OUT | REQUEST_VENDOR, WRITE_REGISTER,
					  0, nIndex, &nValue, sizeof nValue) >= 0;
}

boolean SMSC951xDeviceReadReg (TSMSC951xDevice *pThis, u32 nIndex, u32 *pValue)
{
	assert (pThis != 0);

	return DWHCIDeviceControlMessage (USBDeviceGetHost (&pThis->m_USBDevice),
					  USBDeviceGetEndpoint0 (&pThis->m_USBDevice),
					  REQUEST_IN | REQUEST_VENDOR, READ_REGISTER,
					  0, nIndex, pValue, sizeof *pValue) == (int) sizeof *pValue;
}

#ifndef NDEBUG

void SMSC951xDeviceDumpReg (TSMSC951xDevice *pThis, const char *pName, u32 nIndex)
{
	assert (pThis != 0);

	u32 nValue;
	if (!SMSC951xDeviceReadReg (pThis, nIndex, &nValue))
	{
		LogWrite (FromSMSC951x, LOG_ERROR, "Cannot read register 0x%X", nIndex);

		return;
	}

	LogWrite (FromSMSC951x, LOG_DEBUG, "%08X %s", nValue, pName);
}

void SMSC951xDeviceDumpRegs (TSMSC951xDevice *pThis)
{
	assert (pThis != 0);

	SMSC951xDeviceDumpReg (pThis, "ID_REV",       ID_REV);
	SMSC951xDeviceDumpReg (pThis, "INT_STS",      INT_STS);
	SMSC951xDeviceDumpReg (pThis, "RX_CFG",       RX_CFG);
	SMSC951xDeviceDumpReg (pThis, "TX_CFG",       TX_CFG);
	SMSC951xDeviceDumpReg (pThis, "HW_CFG",       HW_CFG);
	SMSC951xDeviceDumpReg (pThis, "RX_FIFO_INF",  RX_FIFO_INF);
	SMSC951xDeviceDumpReg (pThis, "PM_CTRL",      PM_CTRL);
	SMSC951xDeviceDumpReg (pThis, "LED_GPIO_CFG", LED_GPIO_CFG);
	SMSC951xDeviceDumpReg (pThis, "GPIO_CFG",     GPIO_CFG);
	SMSC951xDeviceDumpReg (pThis, "AFC_CFG",      AFC_CFG);
	SMSC951xDeviceDumpReg (pThis, "BURST_CAP",    BURST_CAP);
	SMSC951xDeviceDumpReg (pThis, "INT_EP_CTL",   INT_EP_CTL);
	SMSC951xDeviceDumpReg (pThis, "BULK_IN_DLY",  BULK_IN_DLY);
	SMSC951xDeviceDumpReg (pThis, "MAC_CR",       MAC_CR);
	SMSC951xDeviceDumpReg (pThis, "ADDRH",        ADDRH);
	SMSC951xDeviceDumpReg (pThis, "ADDRL",        ADDRL);
	SMSC951xDeviceDumpReg (pThis, "HASHH",        HASHH);
	SMSC951xDeviceDumpReg (pThis, "HASHL",        HASHL);
	SMSC951xDeviceDumpReg (pThis, "FLOW",         FLOW);
	SMSC951xDeviceDumpReg (pThis, "WUCSR",        WUCSR);
}

#endif
