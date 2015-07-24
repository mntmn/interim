//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/macros.h>

typedef struct TCHSAddress
{
	unsigned char Head;
	unsigned char Sector	   : 6,
		      CylinderHigh : 2;
	unsigned char CylinderLow;
}
PACKED TCHSAddress;

typedef struct TPartitionEntry
{
	unsigned char	Status;
	TCHSAddress	FirstSector;
	unsigned char	Type;
	TCHSAddress	LastSector;
	unsigned	LBAFirstSector;
	unsigned	NumberOfSectors;
}
PACKED TPartitionEntry;

typedef struct TMasterBootRecord
{
	unsigned char	BootCode[0x1BE];
	TPartitionEntry	Partition[4];
	unsigned short	BootSignature;
	#define BOOT_SIGNATURE		0xAA55
}
PACKED TMasterBootRecord;

static const char FromSample[] = "sample";

int main (void)
{
	if (!USPiEnvInitialize ())
	{
		return EXIT_HALT;
	}
	
	if (!USPiInitialize ())
	{
		LogWrite (FromSample, LOG_ERROR, "Cannot initialize USPi");

		USPiEnvClose ();

		return EXIT_HALT;
	}

	int nDevices = USPiMassStorageDeviceAvailable ();
	if (nDevices < 1)
	{
		LogWrite (FromSample, LOG_ERROR, "Mass storage device not found");

		USPiEnvClose ();

		return EXIT_HALT;
	}

	for (unsigned nDeviceIndex = 0; nDeviceIndex < (unsigned) nDevices; nDeviceIndex++)
	{
		TMasterBootRecord MBR;
		if (USPiMassStorageDeviceRead (0, &MBR, sizeof MBR, nDeviceIndex) != sizeof MBR)
		{
			LogWrite (FromSample, LOG_ERROR, "Read error");

			continue;
		}

		if (MBR.BootSignature != BOOT_SIGNATURE)
		{
			LogWrite (FromSample, LOG_ERROR, "Boot signature not found");

			continue;
		}

		LogWrite (FromSample, LOG_NOTICE, "Dumping the partition table");
		LogWrite (FromSample, LOG_NOTICE, "# Status Type  1stSector    Sectors");

		for (unsigned nPartition = 0; nPartition < 4; nPartition++)
		{
			LogWrite (FromSample, LOG_NOTICE, "%u %02X     %02X   %10u %10u",
				  nPartition+1,
				  (unsigned) MBR.Partition[nPartition].Status,
				  (unsigned) MBR.Partition[nPartition].Type,
				  MBR.Partition[nPartition].LBAFirstSector,
				  MBR.Partition[nPartition].NumberOfSectors);
		}
	}
	
	USPiEnvClose ();

	return EXIT_HALT;
}
