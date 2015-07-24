//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>
#include <uspienv/macros.h>
#include <uspienv/types.h>

#define	OWN_IP_ADDRESS		{192, 168, 1, 242}	// must be a valid IP address on your LAN

#define MAC_ADDRESS_SIZE	6
#define IP_ADDRESS_SIZE		4

typedef struct TEthernetHeader
{
	u8	MACReceiver[MAC_ADDRESS_SIZE];
	u8	MACSender[MAC_ADDRESS_SIZE];
	u16	nProtocolType;
#define ETH_PROT_ARP		0x806
}
PACKED TEthernetHeader;

typedef struct TARPPacket
{
	u16		nHWAddressSpace;
#define HW_ADDR_ETHER		1
	u16		nProtocolAddressSpace;
#define PROT_ADDR_IP		0x800
	u8		nHWAddressLength;
	u8		nProtocolAddressLength;
	u16		nOPCode;
#define ARP_REQUEST		1
#define ARP_REPLY		2
	u8		HWAddressSender[MAC_ADDRESS_SIZE];
	u8		ProtocolAddressSender[IP_ADDRESS_SIZE];
	u8		HWAddressTarget[MAC_ADDRESS_SIZE];
	u8		ProtocolAddressTarget[IP_ADDRESS_SIZE];
}
PACKED TARPPacket;

typedef struct TARPFrame
{
	TEthernetHeader Ethernet;
	TARPPacket	ARP;
}
PACKED TARPFrame;

static const u8 OwnIPAddress[] = OWN_IP_ADDRESS;

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

	if (!USPiEthernetAvailable ())
	{
		LogWrite (FromSample, LOG_ERROR, "Ethernet device not found");

		USPiEnvClose ();

		return EXIT_HALT;
	}

	u8 OwnMACAddress[MAC_ADDRESS_SIZE];
	USPiGetMACAddress (OwnMACAddress);

	while (1)
	{
		u8 Buffer[USPI_FRAME_BUFFER_SIZE];
		unsigned nFrameLength;
		if (!USPiReceiveFrame (Buffer, &nFrameLength))
		{
			continue;
		}

		LogWrite (FromSample, LOG_NOTICE, "Frame received (length %u)", nFrameLength);

		if (nFrameLength < sizeof (TARPFrame))
		{
			continue;
		}

		TARPFrame *pARPFrame = (TARPFrame *) Buffer;
		if (   pARPFrame->Ethernet.nProtocolType	!= BE (ETH_PROT_ARP)
		    || pARPFrame->ARP.nHWAddressSpace		!= BE (HW_ADDR_ETHER)
		    || pARPFrame->ARP.nProtocolAddressSpace	!= BE (PROT_ADDR_IP)
		    || pARPFrame->ARP.nHWAddressLength		!= MAC_ADDRESS_SIZE
		    || pARPFrame->ARP.nProtocolAddressLength	!= IP_ADDRESS_SIZE
		    || pARPFrame->ARP.nOPCode			!= BE (ARP_REQUEST))
		{
			continue;
		}

		LogWrite (FromSample, LOG_NOTICE, "Valid ARP request from %u.%u.%u.%u received",
			  (unsigned) pARPFrame->ARP.ProtocolAddressSender[0],
			  (unsigned) pARPFrame->ARP.ProtocolAddressSender[1],
			  (unsigned) pARPFrame->ARP.ProtocolAddressSender[2],
			  (unsigned) pARPFrame->ARP.ProtocolAddressSender[3]);

		if (memcmp (pARPFrame->ARP.ProtocolAddressTarget, OwnIPAddress, IP_ADDRESS_SIZE) != 0)
		{
			continue;
		}

		LogWrite (FromSample, LOG_NOTICE, "ARP request is to us");

		// prepare reply packet
		memcpy (pARPFrame->Ethernet.MACReceiver, pARPFrame->ARP.HWAddressSender, MAC_ADDRESS_SIZE);
		memcpy (pARPFrame->Ethernet.MACSender, OwnMACAddress, MAC_ADDRESS_SIZE);
		pARPFrame->ARP.nOPCode = BE (ARP_REPLY);

		memcpy (pARPFrame->ARP.HWAddressTarget, pARPFrame->ARP.HWAddressSender, MAC_ADDRESS_SIZE);
		memcpy (pARPFrame->ARP.ProtocolAddressTarget, pARPFrame->ARP.ProtocolAddressSender, IP_ADDRESS_SIZE);

		memcpy (pARPFrame->ARP.HWAddressSender, OwnMACAddress, MAC_ADDRESS_SIZE);
		memcpy (pARPFrame->ARP.ProtocolAddressSender, OwnIPAddress, IP_ADDRESS_SIZE);
		
		if (!USPiSendFrame (pARPFrame, sizeof *pARPFrame))
		{
			LogWrite (FromSample, LOG_ERROR, "USPiSendFrame failed");

			break;
		}

		LogWrite (FromSample, LOG_NOTICE, "ARP reply successfully sent");
	}

	USPiEnvClose ();

	return EXIT_HALT;
}
