//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>

static const char FromSample[] = "sample";

static void KeyPressedHandler (const char *pString)
{
	ScreenDeviceWrite (USPiEnvGetScreen (), pString, strlen (pString));
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
	
	if (!USPiKeyboardAvailable ())
	{
		LogWrite (FromSample, LOG_ERROR, "Keyboard not found");

		USPiEnvClose ();

		return EXIT_HALT;
	}

	USPiKeyboardRegisterKeyPressedHandler (KeyPressedHandler);

	LogWrite (FromSample, LOG_NOTICE, "Just type something!");

	// just wait and turn the rotor
	for (unsigned nCount = 0; 1; nCount++)
	{
		ScreenDeviceRotor (USPiEnvGetScreen (), 0, nCount);
	}

	return EXIT_HALT;
}
