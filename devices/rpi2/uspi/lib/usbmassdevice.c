//
// usbmassdevice.c
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
#include <uspi/usbmassdevice.h>
#include <uspi/usbhostcontroller.h>
#include <uspi/devicenameservice.h>
#include <uspi/util.h>
#include <uspi/macros.h>
#include <uspi/assert.h>
#include <uspios.h>

// USB Mass Storage Bulk-Only Transport

// Command Block Wrapper
typedef struct TCBW
{
	unsigned int	dCWBSignature,
#define CBWSIGNATURE		0x43425355
			dCWBTag,
			dCBWDataTransferLength;		// number of bytes
	unsigned char	bmCBWFlags,
#define CBWFLAGS_DATA_IN	0x80
			bCBWLUN		: 4,
#define CBWLUN			0
			Reserved1	: 4,
			bCBWCBLength	: 5,		// valid length of the CBWCB in bytes
			Reserved2	: 3;
	unsigned char	CBWCB[16];
}
PACKED TCBW;

// Command Status Wrapper
typedef struct TCSW
{
	unsigned int	dCSWSignature,
#define CSWSIGNATURE		0x53425355
			dCSWTag,
			dCSWDataResidue;		// difference in amount of data processed
	unsigned char	bCSWStatus;
#define CSWSTATUS_PASSED	0x00
#define CSWSTATUS_FAILED	0x01
#define CSWSTATUS_PHASE_ERROR	0x02
}
PACKED TCSW;

// SCSI Transparent Command Set

#define SCSI_CONTROL		0x00

typedef struct TSCSIInquiry
{
	unsigned char	OperationCode,
#define SCSI_OP_INQUIRY		0x12
			LogicalUnitNumberEVPD,
			PageCode,
			Reserved,
			AllocationLength,
			Control;
}
PACKED TSCSIInquiry;

typedef struct TSCSIInquiryResponse
{
	unsigned char	PeripheralDeviceType	: 5,
#define SCSI_PDT_DIRECT_ACCESS_BLOCK	0x00			// SBC-2 command set (or above)
#define SCSI_PDT_DIRECT_ACCESS_RBC	0x0E			// RBC command set
			PeripheralQualifier	: 3,		// 0: device is connected to this LUN
			DeviceTypeModifier	: 7,
			RMB			: 1,		// 1: removable media
			ANSIApprovedVersion	: 3,
			ECMAVersion		: 3,
			ISOVersion		: 2,
			Reserved1,
			AdditionalLength,
			Reserved2[3],
			VendorIdentification[8],
			ProductIdentification[16],
			ProductRevisionLevel[4];
}
PACKED TSCSIInquiryResponse;

typedef struct TSCSITestUnitReady
{
	unsigned char	OperationCode;
#define SCSI_OP_TEST_UNIT_READY		0x00
	unsigned int	Reserved;
	unsigned char	Control;
}
PACKED TSCSITestUnitReady;

typedef struct TSCSIRequestSense
{
	unsigned char	OperationCode;
#define SCSI_REQUEST_SENSE		0x03
	unsigned char	DescriptorFormat	: 1,		// set to 0
			Reserved1		: 7;
	unsigned short	Reserved2;
	unsigned char	AllocationLength;
	unsigned char	Control;
}
PACKED TSCSIRequestSense;

typedef struct TSCSIRequestSenseResponse7x
{
	unsigned char	ResponseCode		: 7,
			Valid			: 1;
	unsigned char	Obsolete;
	unsigned char	SenseKey		: 4,
			Reserved		: 1,
			ILI			: 1,
			EOM			: 1,
			FileMark		: 1;
	unsigned int	Information;				// big endian
	unsigned char	AdditionalSenseLength;
	unsigned int	CommandSpecificInformation;		// big endian
	unsigned char	AdditionalSenseCode;
	unsigned char	AdditionalSenseCodeQualifier;
	unsigned char	FieldReplaceableUnitCode;
	unsigned char	SenseKeySpecificHigh	: 7,
			SKSV			: 1;
	unsigned short	SenseKeySpecificLow;
}
PACKED TSCSIRequestSenseResponse7x;

typedef struct TSCSIReadCapacity10
{
	unsigned char	OperationCode;
#define SCSI_OP_READ_CAPACITY10		0x25
	unsigned char	Obsolete		: 1,
			Reserved1		: 7;
	unsigned int	LogicalBlockAddress;			// set to 0
	unsigned short	Reserved2;
	unsigned char	PartialMediumIndicator	: 1,		// set to 0
			Reserved3		: 7;
	unsigned char	Control;
}
PACKED TSCSIReadCapacity10;

typedef struct TSCSIReadCapacityResponse
{
	unsigned int	ReturnedLogicalBlockAddress;		// big endian
	unsigned int	BlockLengthInBytes;			// big endian
}
PACKED TSCSIReadCapacityResponse;

typedef struct TSCSIRead10
{
	unsigned char	OperationCode,
#define SCSI_OP_READ		0x28
			Reserved1;
	unsigned int	LogicalBlockAddress;			// big endian
	unsigned char	Reserved2;
	unsigned short	TransferLength;				// block count, big endian
	unsigned char	Control;
}
PACKED TSCSIRead10;

typedef struct TSCSIWrite10
{
	unsigned char	OperationCode,
#define SCSI_OP_WRITE		0x2A
			Flags;
#define SCSI_WRITE_FUA		0x08
	unsigned int	LogicalBlockAddress;			// big endian
	unsigned char	Reserved;
	unsigned short	TransferLength;				// block count, big endian
	unsigned char	Control;
}
PACKED TSCSIWrite10;

static unsigned s_nDeviceNumber = 1;

static const char FromUmsd[] = "umsd";

int USBBulkOnlyMassStorageDeviceTryRead (TUSBBulkOnlyMassStorageDevice *pThis, void *pBuffer, unsigned nCount);
int USBBulkOnlyMassStorageDeviceTryWrite (TUSBBulkOnlyMassStorageDevice *pThis, const void *pBuffer, unsigned nCount);
int USBBulkOnlyMassStorageDeviceCommand (TUSBBulkOnlyMassStorageDevice *pThis,
					 void *pCmdBlk, unsigned nCmdBlkLen,
					 void *pBuffer, unsigned nBufLen, boolean bIn);
int USBBulkOnlyMassStorageDeviceReset (TUSBBulkOnlyMassStorageDevice *pThis);

void USBBulkOnlyMassStorageDevice (TUSBBulkOnlyMassStorageDevice *pThis, TUSBDevice *pDevice)
{
	assert (pThis != 0);

	USBDeviceCopy (&pThis->m_USBDevice, pDevice);
	pThis->m_USBDevice.Configure = USBBulkOnlyMassStorageDeviceConfigure;

	pThis->m_pEndpointIn = 0;
	pThis->m_pEndpointOut = 0;
	pThis->m_nCWBTag = 0;
	pThis->m_nBlockCount = 0;
	pThis->m_ullOffset = 0;
}

void _USBBulkOnlyMassStorageDevice (TUSBBulkOnlyMassStorageDevice *pThis)
{
	assert (pThis != 0);

	if (pThis->m_pEndpointOut != 0)
	{
		_USBEndpoint (pThis->m_pEndpointOut);
		free (pThis->m_pEndpointOut);
		pThis->m_pEndpointOut =  0;
	}
	
	if (pThis->m_pEndpointIn != 0)
	{
		_USBEndpoint (pThis->m_pEndpointIn);
		free (pThis->m_pEndpointIn);
		pThis->m_pEndpointIn =  0;
	}
	
	_USBDevice (&pThis->m_USBDevice);
}

boolean USBBulkOnlyMassStorageDeviceConfigure (TUSBDevice *pUSBDevice)
{
	TUSBBulkOnlyMassStorageDevice *pThis = (TUSBBulkOnlyMassStorageDevice *) pUSBDevice;
	assert (pThis != 0);

	TUSBConfigurationDescriptor *pConfDesc =
		(TUSBConfigurationDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_CONFIGURATION);
	if (   pConfDesc == 0
	    || pConfDesc->bNumInterfaces <  1)
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromUmsd);

		return FALSE;
	}

	TUSBInterfaceDescriptor *pInterfaceDesc =
		(TUSBInterfaceDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_INTERFACE);
	if (   pInterfaceDesc == 0
	    || pInterfaceDesc->bInterfaceNumber		!= 0x00
	    || pInterfaceDesc->bAlternateSetting	!= 0x00
	    || pInterfaceDesc->bNumEndpoints		<  2
	    || pInterfaceDesc->bInterfaceClass		!= 0x08		// Mass Storage Class
	    || pInterfaceDesc->bInterfaceSubClass	!= 0x06		// SCSI Transparent Command Set
	    || pInterfaceDesc->bInterfaceProtocol	!= 0x50)	// Bulk-Only Transport
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromUmsd);

		return FALSE;
	}

	const TUSBEndpointDescriptor *pEndpointDesc;
	while ((pEndpointDesc = (TUSBEndpointDescriptor *) USBDeviceGetDescriptor (&pThis->m_USBDevice, DESCRIPTOR_ENDPOINT)) != 0)
	{
		if ((pEndpointDesc->bmAttributes & 0x3F) == 0x02)		// Bulk
		{
			if ((pEndpointDesc->bEndpointAddress & 0x80) == 0x80)	// Input
			{
				if (pThis->m_pEndpointIn != 0)
				{
					USBDeviceConfigurationError (&pThis->m_USBDevice, FromUmsd);

					return FALSE;
				}

				pThis->m_pEndpointIn = (TUSBEndpoint *) malloc (sizeof (TUSBEndpoint));
				assert (pThis->m_pEndpointIn != 0);
				USBEndpoint2 (pThis->m_pEndpointIn, &pThis->m_USBDevice, pEndpointDesc);
			}
			else							// Output
			{
				if (pThis->m_pEndpointOut != 0)
				{
					USBDeviceConfigurationError (&pThis->m_USBDevice, FromUmsd);

					return FALSE;
				}

				pThis->m_pEndpointOut = (TUSBEndpoint *) malloc (sizeof (TUSBEndpoint));
				assert (pThis->m_pEndpointOut != 0);
				USBEndpoint2 (pThis->m_pEndpointOut, &pThis->m_USBDevice, pEndpointDesc);
			}
		}
	}

	if (   pThis->m_pEndpointIn  == 0
	    || pThis->m_pEndpointOut == 0)
	{
		USBDeviceConfigurationError (&pThis->m_USBDevice, FromUmsd);

		return FALSE;
	}

	if (!USBDeviceConfigure (&pThis->m_USBDevice))
	{
		LogWrite (FromUmsd, LOG_ERROR, "Cannot set configuration");

		return FALSE;
	}

	TSCSIInquiry SCSIInquiry;
	SCSIInquiry.OperationCode	  = SCSI_OP_INQUIRY;
	SCSIInquiry.LogicalUnitNumberEVPD = 0;
	SCSIInquiry.PageCode		  = 0;
	SCSIInquiry.Reserved		  = 0;
	SCSIInquiry.AllocationLength	  = sizeof (TSCSIInquiryResponse);
	SCSIInquiry.Control		  = SCSI_CONTROL;

	TSCSIInquiryResponse SCSIInquiryResponse;
	if (USBBulkOnlyMassStorageDeviceCommand (pThis, &SCSIInquiry, sizeof SCSIInquiry,
						 &SCSIInquiryResponse, sizeof SCSIInquiryResponse,
						 TRUE) != (int) sizeof SCSIInquiryResponse)
	{
		LogWrite (FromUmsd, LOG_ERROR, "Device does not respond");

		return FALSE;
	}

	if (SCSIInquiryResponse.PeripheralDeviceType != SCSI_PDT_DIRECT_ACCESS_BLOCK)
	{
		LogWrite (FromUmsd, LOG_ERROR, "Unsupported device type: 0x%02X", (unsigned) SCSIInquiryResponse.PeripheralDeviceType);
		
		return FALSE;
	}

	unsigned nTries = 100;
	while (--nTries)
	{
		TSCSITestUnitReady SCSITestUnitReady;
		SCSITestUnitReady.OperationCode = SCSI_OP_TEST_UNIT_READY;
		SCSITestUnitReady.Reserved	= 0;
		SCSITestUnitReady.Control	= SCSI_CONTROL;

		if (USBBulkOnlyMassStorageDeviceCommand (pThis, &SCSITestUnitReady,
							 sizeof SCSITestUnitReady, 0, 0, FALSE) >= 0)
		{
			break;
		}

		TSCSIRequestSense SCSIRequestSense;
		SCSIRequestSense.OperationCode	  = SCSI_REQUEST_SENSE;
		SCSIRequestSense.DescriptorFormat = 0;
		SCSIRequestSense.Reserved1	  = 0;
		SCSIRequestSense.Reserved2	  = 0;
		SCSIRequestSense.AllocationLength = sizeof (TSCSIRequestSenseResponse7x);
		SCSIRequestSense.Control	  = SCSI_CONTROL;

		TSCSIRequestSenseResponse7x SCSIRequestSenseResponse7x;
		if (USBBulkOnlyMassStorageDeviceCommand (pThis, &SCSIRequestSense, sizeof SCSIRequestSense,
							 &SCSIRequestSenseResponse7x, sizeof SCSIRequestSenseResponse7x,
							 TRUE) < 0)
		{
			LogWrite (FromUmsd, LOG_ERROR, "Request sense failed");

			return FALSE;
		}

		MsDelay (100);
	}

	if (nTries == 0)
	{
		LogWrite (FromUmsd, LOG_ERROR, "Unit is not ready");

		return FALSE;
	}

	TSCSIReadCapacity10 SCSIReadCapacity;
	SCSIReadCapacity.OperationCode		= SCSI_OP_READ_CAPACITY10;
	SCSIReadCapacity.Obsolete		= 0;
	SCSIReadCapacity.Reserved1		= 0;
	SCSIReadCapacity.LogicalBlockAddress	= 0;
	SCSIReadCapacity.Reserved2		= 0;
	SCSIReadCapacity.PartialMediumIndicator	= 0;
	SCSIReadCapacity.Reserved3		= 0;
	SCSIReadCapacity.Control		= SCSI_CONTROL;

	TSCSIReadCapacityResponse SCSIReadCapacityResponse;
	if (USBBulkOnlyMassStorageDeviceCommand (pThis, &SCSIReadCapacity, sizeof SCSIReadCapacity,
						 &SCSIReadCapacityResponse, sizeof SCSIReadCapacityResponse,
						 TRUE) != (int) sizeof SCSIReadCapacityResponse)
	{
		LogWrite (FromUmsd, LOG_ERROR, "Read capacity failed");

		return FALSE;
	}

	unsigned nBlockSize = uspi_le2be32 (SCSIReadCapacityResponse.BlockLengthInBytes);
	if (nBlockSize != UMSD_BLOCK_SIZE)
	{
		LogWrite (FromUmsd, LOG_ERROR, "Unsupported block size: %u", nBlockSize);

		return FALSE;
	}

	pThis->m_nBlockCount = uspi_le2be32 (SCSIReadCapacityResponse.ReturnedLogicalBlockAddress);
	if (pThis->m_nBlockCount == (u32) -1)
	{
		LogWrite (FromUmsd, LOG_ERROR, "Unsupported disk size > 2TB");

		return FALSE;
	}

	pThis->m_nBlockCount++;

	LogWrite (FromUmsd, LOG_DEBUG, "Capacity is %u MByte", pThis->m_nBlockCount / (0x100000 / UMSD_BLOCK_SIZE));

	TString DeviceName;
	String (&DeviceName);
	StringFormat (&DeviceName, "umsd%u", s_nDeviceNumber++);
	DeviceNameServiceAddDevice (DeviceNameServiceGet (), StringGet (&DeviceName), pThis, TRUE);

	_String (&DeviceName);
	
	return TRUE;
}

int USBBulkOnlyMassStorageDeviceRead (TUSBBulkOnlyMassStorageDevice *pThis, void *pBuffer, unsigned nCount)
{
	assert (pThis != 0);

	unsigned nTries = 4;

	int nResult;

	do
	{
		nResult = USBBulkOnlyMassStorageDeviceTryRead (pThis, pBuffer, nCount);

		if (nResult != (int) nCount)
		{
			int nStatus = USBBulkOnlyMassStorageDeviceReset (pThis);
			if (nStatus != 0)
			{
				return nStatus;
			}
		}
	}
	while (   nResult != (int) nCount
	       && --nTries > 0);

	return nResult;
}

int USBBulkOnlyMassStorageDeviceWrite (TUSBBulkOnlyMassStorageDevice *pThis, const void *pBuffer, unsigned nCount)
{
	assert (pThis != 0);

	unsigned nTries = 4;

	int nResult;

	do
	{
		nResult = USBBulkOnlyMassStorageDeviceTryWrite (pThis, pBuffer, nCount);

		if (nResult != (int) nCount)
		{
			int nStatus = USBBulkOnlyMassStorageDeviceReset (pThis);
			if (nStatus != 0)
			{
				return nStatus;
			}
		}
	}
	while (   nResult != (int) nCount
	       && --nTries > 0);

	return nResult;
}

unsigned long long USBBulkOnlyMassStorageDeviceSeek (TUSBBulkOnlyMassStorageDevice *pThis, unsigned long long ullOffset)
{
	assert (pThis != 0);

	pThis->m_ullOffset = ullOffset;

	return pThis->m_ullOffset;
}

unsigned USBBulkOnlyMassStorageDeviceGetCapacity (TUSBBulkOnlyMassStorageDevice *pThis)
{
	assert (pThis != 0);

	return pThis->m_nBlockCount;
}

int USBBulkOnlyMassStorageDeviceTryRead (TUSBBulkOnlyMassStorageDevice *pThis, void *pBuffer, unsigned nCount)
{
	assert (pThis != 0);

	assert (pBuffer != 0);

	if (   (pThis->m_ullOffset & UMSD_BLOCK_MASK) != 0
	    || pThis->m_ullOffset > UMSD_MAX_OFFSET)
	{
		return -1;
	}
	unsigned nBlockAddress = (unsigned) (pThis->m_ullOffset >> UMSD_BLOCK_SHIFT);

	if ((nCount & UMSD_BLOCK_MASK) != 0)
	{
		return -1;
	}
	unsigned short usTransferLength = (unsigned short) (nCount >> UMSD_BLOCK_SHIFT);

	//LogWrite (FromUmsd, LOG_DEBUG, "TryRead %u/0x%X/%u", nBlockAddress, (unsigned) pBuffer, (unsigned) usTransferLength);

	TSCSIRead10 SCSIRead;
	SCSIRead.OperationCode		= SCSI_OP_READ;
	SCSIRead.Reserved1		= 0;
	SCSIRead.LogicalBlockAddress	= uspi_le2be32 (nBlockAddress);
	SCSIRead.Reserved2		= 0;
	SCSIRead.TransferLength		= uspi_le2be16 (usTransferLength);
	SCSIRead.Control		= SCSI_CONTROL;

	if (USBBulkOnlyMassStorageDeviceCommand (pThis, &SCSIRead, sizeof SCSIRead,
						 pBuffer, nCount,
						 TRUE) != (int) nCount)
	{
		LogWrite (FromUmsd, LOG_ERROR, "TryRead failed");

		return -1;
	}

	return nCount;
}

int USBBulkOnlyMassStorageDeviceTryWrite (TUSBBulkOnlyMassStorageDevice *pThis, const void *pBuffer, unsigned nCount)
{
	assert (pThis != 0);

	assert (pBuffer != 0);

	if (   (pThis->m_ullOffset & UMSD_BLOCK_MASK) != 0
	    || pThis->m_ullOffset > UMSD_MAX_OFFSET)
	{
		return -1;
	}
	unsigned nBlockAddress = (unsigned) (pThis->m_ullOffset >> UMSD_BLOCK_SHIFT);

	if ((nCount & UMSD_BLOCK_MASK) != 0)
	{
		return -1;
	}
	unsigned short usTransferLength = (unsigned short) (nCount >> UMSD_BLOCK_SHIFT);

	//LogWrite (FromUmsd, LOG_DEBUG, "TryWrite %u/0x%X/%u", nBlockAddress, (unsigned) pBuffer, (unsigned) usTransferLength);

	TSCSIWrite10 SCSIWrite;
	SCSIWrite.OperationCode		= SCSI_OP_WRITE;
	SCSIWrite.Flags			= SCSI_WRITE_FUA;
	SCSIWrite.LogicalBlockAddress	= uspi_le2be32 (nBlockAddress);
	SCSIWrite.Reserved		= 0;
	SCSIWrite.TransferLength	= uspi_le2be16 (usTransferLength);
	SCSIWrite.Control		= SCSI_CONTROL;

	if (USBBulkOnlyMassStorageDeviceCommand (pThis, &SCSIWrite, sizeof SCSIWrite,
						 (void *) pBuffer, nCount,
						 FALSE) < 0)
	{
		LogWrite (FromUmsd, LOG_ERROR, "TryWrite failed");

		return -1;
	}

	return nCount;
}

int USBBulkOnlyMassStorageDeviceCommand (TUSBBulkOnlyMassStorageDevice *pThis,
					 void *pCmdBlk, unsigned nCmdBlkLen,
					 void *pBuffer, unsigned nBufLen, boolean bIn)
{
	assert (pThis != 0);

	assert (pCmdBlk != 0);
	assert (6 <= nCmdBlkLen && nCmdBlkLen <= 16);
	assert (nBufLen == 0 || pBuffer != 0);

	TCBW CBW;
	memset (&CBW, 0, sizeof CBW);

	CBW.dCWBSignature	   = CBWSIGNATURE;
	CBW.dCWBTag		   = ++pThis->m_nCWBTag;
	CBW.dCBWDataTransferLength = nBufLen;
	CBW.bmCBWFlags		   = bIn ? CBWFLAGS_DATA_IN : 0;
	CBW.bCBWLUN		   = CBWLUN;
	CBW.bCBWCBLength	   = (u8) nCmdBlkLen;

	memcpy (CBW.CBWCB, pCmdBlk, nCmdBlkLen);

	TUSBHostController *pHost = USBDeviceGetHost (&pThis->m_USBDevice);
	assert (pHost != 0);
	
	if (DWHCIDeviceTransfer (pHost, pThis->m_pEndpointOut, &CBW, sizeof CBW) < 0)
	{
		LogWrite (FromUmsd, LOG_ERROR, "CBW transfer failed");

		return -1;
	}

	int nResult = 0;
	
	if (nBufLen > 0)
	{
		nResult = DWHCIDeviceTransfer (pHost, bIn ? pThis->m_pEndpointIn : pThis->m_pEndpointOut, pBuffer, nBufLen);
		if (nResult < 0)
		{
			LogWrite (FromUmsd, LOG_ERROR, "Data transfer failed");

			return -1;
		}
	}

	TCSW CSW;

	if (DWHCIDeviceTransfer (pHost, pThis->m_pEndpointIn, &CSW, sizeof CSW) != (int) sizeof CSW)
	{
		LogWrite (FromUmsd, LOG_ERROR, "CSW transfer failed");

		return -1;
	}

	if (CSW.dCSWSignature != CSWSIGNATURE)
	{
		LogWrite (FromUmsd, LOG_ERROR, "CSW signature is wrong");

		return -1;
	}

	if (CSW.dCSWTag != pThis->m_nCWBTag)
	{
		LogWrite (FromUmsd, LOG_ERROR, "CSW tag is wrong");

		return -1;
	}

	if (CSW.bCSWStatus != CSWSTATUS_PASSED)
	{
		return -1;
	}

	if (CSW.dCSWDataResidue != 0)
	{
		LogWrite (FromUmsd, LOG_ERROR, "Data residue is not 0");

		return -1;
	}

	return nResult;
}

int USBBulkOnlyMassStorageDeviceReset (TUSBBulkOnlyMassStorageDevice *pThis)
{
	assert (pThis != 0);

	TUSBHostController *pHost = USBDeviceGetHost (&pThis->m_USBDevice);
	assert (pHost != 0);
	
	if (DWHCIDeviceControlMessage (pHost, USBDeviceGetEndpoint0 (&pThis->m_USBDevice), 0x21, 0xFF, 0, 0x00, 0, 0) < 0)
	{
		LogWrite (FromUmsd, LOG_DEBUG, "Cannot reset device");

		return -1;
	}

	if (DWHCIDeviceControlMessage (pHost, USBDeviceGetEndpoint0 (&pThis->m_USBDevice), 0x02, 1, 0, 1, 0, 0) < 0)
	{
		LogWrite (FromUmsd, LOG_DEBUG, "Cannot clear halt on endpoint 1");

		return -1;
	}

	if (DWHCIDeviceControlMessage (pHost, USBDeviceGetEndpoint0 (&pThis->m_USBDevice), 0x02, 1, 0, 2, 0, 0) < 0)
	{
		LogWrite (FromUmsd, LOG_DEBUG, "Cannot clear halt on endpoint 2");

		return -1;
	}

	USBEndpointResetPID (pThis->m_pEndpointIn);
	USBEndpointResetPID (pThis->m_pEndpointOut);

	return 0;
}
