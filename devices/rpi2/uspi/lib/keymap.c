//
// keymap.c
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
#include <uspi/keymap.h>
#include <uspi/usbhid.h>
#include <uspi/util.h>
#include <uspi/assert.h>
#include <uspios.h>

#define KEYPAD_FIRST	0x53
#define KEYPAD_LAST	0x63

// order must match TSpecialKey beginning at KeySpace
static const char *s_KeyStrings[KeyMaxCode-KeySpace] =
{
	" ",			// KeySpace
	"\x1b",			// KeyEscape
	"\x7f",			// KeyBackspace
	"\t",			// KeyTabulator
	"\n",			// KeyReturn
	"\x1b[2~",		// KeyInsert
	"\x1b[1~",		// KeyHome
	"\x1b[5~",		// KeyPageUp
	"\x1b[3~",		// KeyDelete
	"\x1b[4~",		// KeyEnd
	"\x1b[6~",		// KeyPageDown
	"\x1b[A",		// KeyUp
	"\x1b[B",		// KeyDown
	"\x1b[D",		// KeyLeft
	"\x1b[C",		// KeyRight
	"\x1b[[A",		// KeyF1
	"\x1b[[B",		// KeyF2
	"\x1b[[C",		// KeyF3
	"\x1b[[D",		// KeyF4
	"\x1b[[E",		// KeyF5
	"\x1b[17~",		// KeyF6
	"\x1b[18~",		// KeyF7
	"\x1b[19~",		// KeyF8
	"\x1b[20~",		// KeyF9
	0,			// KeyF10
	0,			// KeyF11
	0,			// KeyF12
	0,			// KeyApplication
	0,			// KeyCapsLock
	0,			// KeyPrintScreen
	0,			// KeyScrollLock
	0,			// KeyPause
	0,			// KeyNumLock
	"/",			// KeyKP_Divide
	"*",			// KeyKP_Multiply
	"-",			// KeyKP_Subtract
	"+",			// KeyKP_Add
	"\n",			// KeyKP_Enter
	"1",			// KeyKP_1
	"2",			// KeyKP_2
	"3",			// KeyKP_3
	"4",			// KeyKP_4
	"5",			// KeyKP_5
	"6",			// KeyKP_6
	"7",			// KeyKP_7
	"8",			// KeyKP_8
	"9",			// KeyKP_9
	"0",			// KeyKP_0
	"\x1b[G",		// KeyKP_Center
	",",			// KeyKP_Comma
	"."			// KeyKP_Period
};

static const u8 s_DefaultMap[PHY_MAX_CODE+1][K_ALTTAB+1] =
{
#if defined (USPI_DEFAULT_KEYMAP_UK)
	#include "keymap_uk.h"
#elif defined (USPI_DEFAULT_KEYMAP_DE)
	#include "keymap_de.h"
#else
	{KeyNone}		// ...
#endif
};

void KeyMap (TKeyMap *pThis)
{
	assert (pThis != 0);

	pThis->m_bCapsLock = FALSE;
	pThis->m_bNumLock = TRUE;
	pThis->m_bScrollLock = FALSE;

	assert (sizeof pThis->m_KeyMap == sizeof s_DefaultMap);
	memcpy (pThis->m_KeyMap, s_DefaultMap, sizeof pThis->m_KeyMap);
}

void _KeyMap (TKeyMap *pThis)
{
}

boolean KeyMapClearTable (TKeyMap *pThis, u8 nTable)
{
	assert (pThis != 0);

	if (nTable > K_ALTTAB)
	{
		return FALSE;
	}

	for (unsigned nPhyCode = 0; nPhyCode <= PHY_MAX_CODE; nPhyCode++)
	{
		pThis->m_KeyMap[nPhyCode][nTable] = KeyNone;
	}

	return TRUE;
}

boolean KeyMapSetEntry (TKeyMap *pThis, u8 nTable, u8 nPhyCode, u8 nValue)
{
	assert (pThis != 0);

	if (   nTable   > K_ALTTAB
	    || nPhyCode == 0
	    || nPhyCode > PHY_MAX_CODE
	    || nValue   >= KeyMaxCode)
	{
		return FALSE;
	}

	pThis->m_KeyMap[nPhyCode][nTable] = nValue;

	return TRUE;
}

u8 KeyMapTranslate (TKeyMap *pThis, u8 nPhyCode, u8 nModifiers)
{
	assert (pThis != 0);

	if (   nPhyCode == 0
	    || nPhyCode > PHY_MAX_CODE)
	{
		return KeyNone;
	}

	u8 nLogCodeNorm = pThis->m_KeyMap[nPhyCode][K_NORMTAB];

	if (   nLogCodeNorm == KeyDelete
	    && (nModifiers & (LCTRL | RCTRL))
	    && (nModifiers & ALT))
	{
		return ActionShutdown; 
	}

	if (   (KeyF1 <= nLogCodeNorm && nLogCodeNorm <= KeyF12)
	    && (nModifiers & ALT))
	{
		return ActionSelectConsole1 + (nLogCodeNorm - KeyF1);
	}

	if (nModifiers & (ALT | LWIN | RWIN))
	{
		return KeyNone;
	}
	
	unsigned nTable = K_NORMTAB;

	// TODO: hard-wired to keypad
	if (KEYPAD_FIRST <= nPhyCode && nPhyCode <= KEYPAD_LAST)
	{
		if (pThis->m_bNumLock)
		{
			nTable = K_SHIFTTAB;
		}
	}
	else if (nModifiers & ALTGR)
	{
		nTable = K_ALTTAB;
	}
	else if (nModifiers & (LSHIFT | RSHIFT))
	{
		nTable = K_SHIFTTAB;
	}

	u8 nLogCode = pThis->m_KeyMap[nPhyCode][nTable];

	switch (nLogCode)
	{
	case KeyCapsLock:
		pThis->m_bCapsLock = !pThis->m_bCapsLock;
		return ActionSwitchCapsLock;
	
	case KeyNumLock:
		pThis->m_bNumLock = !pThis->m_bNumLock;
		return ActionSwitchNumLock;

	case KeyScrollLock:
		pThis->m_bScrollLock = !pThis->m_bScrollLock;
		return ActionSwitchScrollLock;
	}

	return nLogCode;
}

const char *KeyMapGetString (TKeyMap *pThis, u8 nKeyCode, u8 nModifiers, char Buffer[2])
{
	assert (pThis != 0);

	if (   nKeyCode <= ' '
	    || nKeyCode >= KeyMaxCode)
	{
		return 0;
	}

	if (KeySpace <= nKeyCode && nKeyCode < KeyMaxCode)
	{
		return s_KeyStrings[nKeyCode-KeySpace];
	}

	char chChar = (char) nKeyCode;
		
	if (nModifiers & (LCTRL | RCTRL))
	{
		chChar -= 'a';
		if ('\0' <= chChar && chChar <= 'z'-'a')
		{
			Buffer[0] = chChar + 1;
			Buffer[1] = '\0';

			return Buffer;
		}
		
		return 0;
	}

	if (pThis->m_bCapsLock)
	{
		if ('A' <= chChar && chChar <= 'Z')
		{
			chChar += 'a'-'A';
		}
		else if ('a' <= chChar && chChar <= 'z')
		{
			chChar -= 'a'-'A';
		}
	}

	Buffer[0] = chChar;
	Buffer[1] = '\0';

	return Buffer;
}

u8 KeyMapGetLEDStatus (TKeyMap *pThis)
{
	assert (pThis != 0);

	u8 nResult = 0;

	if (pThis->m_bCapsLock)
	{
		nResult |= LED_CAPS_LOCK;
	}

	if (pThis->m_bNumLock)
	{
		nResult |= LED_NUM_LOCK;
	}

	if (pThis->m_bScrollLock)
	{
		nResult |= LED_SCROLL_LOCK;
	}

	return nResult;
}
