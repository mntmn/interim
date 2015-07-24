//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/string.h>
#include <uspienv/assert.h>

static const char FromSample[] = "sample";

static void GamePadStatusHandler (unsigned int nDeviceIndex, const USPiGamePadState *pState)
{
	TString Msg;
	String (&Msg);
	StringFormat (&Msg, "GamePad %u: Buttons 0x%X", nDeviceIndex + 1, pState->buttons);

	TString Value;
	String (&Value);

	if (pState->naxes > 0)
	{
		StringAppend (&Msg, " Axes");

		for (unsigned i = 0; i < pState->naxes; i++)
		{
			StringFormat (&Value, " %d", pState->axes[i].value);
			StringAppend (&Msg, StringGet (&Value));
		}
	}

	if (pState->nhats > 0)
	{
		StringAppend (&Msg, " Hats");

		for (unsigned i = 0; i < pState->nhats; i++)
		{
			StringFormat (&Value, " %d", pState->hats[i]);
			StringAppend (&Msg, StringGet (&Value));
		}
	}

	LogWrite (FromSample, LOG_NOTICE, StringGet (&Msg));
	
	_String (&Value);
	_String (&Msg);
}

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

	int nGamePads = USPiGamePadAvailable ();
	if (nGamePads < 1)
	{
		LogWrite (FromSample, LOG_ERROR, "GamePad not found");

		USPiEnvClose ();

		return EXIT_HALT;
	}

	for (unsigned nGamePad = 0; nGamePad < (unsigned) nGamePads; nGamePad++)
	{
		TUSPiDeviceInformation Info;
		if (!USPiDeviceGetInformation (GAMEPAD_CLASS, nGamePad, &Info))
		{
			LogWrite (FromSample, LOG_ERROR, "Cannot get device information");

			USPiEnvClose ();

			return EXIT_HALT;
		}

		LogWrite (FromSample, LOG_NOTICE, "GamePad %u: Vendor 0x%X Product 0x%X Version 0x%X",
			  nGamePad+1, (unsigned) Info.idVendor, (unsigned) Info.idProduct, (unsigned) Info.bcdDevice);

		LogWrite (FromSample, LOG_NOTICE, "GamePad %u: Manufacturer \"%s\" Product \"%s\"",
			  nGamePad+1, Info.pManufacturer, Info.pProduct);

		const USPiGamePadState *pState = USPiGamePadGetStatus (nGamePad);
		assert (pState != 0);

		LogWrite (FromSample, LOG_NOTICE, "GamePad %u: %d Buttons %d Hats",
			  nGamePad+1, pState->nbuttons, pState->nhats);

		for (int i = 0; i < pState->naxes; i++)
		{
			LogWrite (FromSample, LOG_NOTICE, "GamePad %u: Axis %d: Minimum %d Maximum %d",
				  nGamePad+1, i+1, pState->axes[i].minimum, pState->axes[i].maximum);
		}
	}

	USPiGamePadRegisterStatusHandler (GamePadStatusHandler);

	// just wait and turn the rotor
	for (unsigned nCount = 0; 1; nCount++)
	{
		ScreenDeviceRotor (USPiEnvGetScreen (), 0, nCount);
	}

	return EXIT_HALT;
}
