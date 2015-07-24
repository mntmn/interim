//
// keymap.h
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
#ifndef _uspi_keymap_h
#define _uspi_keymap_h

#include <uspi/types.h>

#define PHY_MAX_CODE	127

#define K_NORMTAB	0
#define K_SHIFTTAB	1
#define K_ALTTAB	2

typedef enum
{
	KeyNone  = 0x00,
	KeySpace = 0x80,
	KeyEscape,
	KeyBackspace,
	KeyTabulator,
	KeyReturn,
	KeyInsert,
	KeyHome,
	KeyPageUp,
	KeyDelete,
	KeyEnd,
	KeyPageDown,
	KeyUp,
	KeyDown,
	KeyLeft,
	KeyRight,
	KeyF1,
	KeyF2,
	KeyF3,
	KeyF4,
	KeyF5,
	KeyF6,
	KeyF7,
	KeyF8,
	KeyF9,
	KeyF10,
	KeyF11,
	KeyF12,
	KeyApplication,
	KeyCapsLock,
	KeyPrintScreen,
	KeyScrollLock,
	KeyPause,
	KeyNumLock,
	KeyKP_Divide,
	KeyKP_Multiply,
	KeyKP_Subtract,
	KeyKP_Add,
	KeyKP_Enter,
	KeyKP_1,
	KeyKP_2,
	KeyKP_3,
	KeyKP_4,
	KeyKP_5,
	KeyKP_6,
	KeyKP_7,
	KeyKP_8,
	KeyKP_9,
	KeyKP_0,
	KeyKP_Center,
	KeyKP_Comma,
	KeyKP_Period,
	KeyMaxCode
}
TSpecialKey;

typedef enum
{
	ActionSwitchCapsLock = KeyMaxCode,
	ActionSwitchNumLock,
	ActionSwitchScrollLock,
	ActionSelectConsole1,
	ActionSelectConsole2,
	ActionSelectConsole3,
	ActionSelectConsole4,
	ActionSelectConsole5,
	ActionSelectConsole6,
	ActionSelectConsole7,
	ActionSelectConsole8,
	ActionSelectConsole9,
	ActionSelectConsole10,
	ActionSelectConsole11,
	ActionSelectConsole12,
	ActionShutdown,
	ActionNone
}
TSpecialAction;

typedef struct TKeyMap
{
	u8 m_KeyMap[PHY_MAX_CODE+1][K_ALTTAB+1];

	boolean m_bCapsLock;
	boolean m_bNumLock;
	boolean m_bScrollLock;
}
TKeyMap;

void KeyMap (TKeyMap *pThis);
void _KeyMap (TKeyMap *pThis);

boolean KeyMapClearTable (TKeyMap *pThis, u8 nTable);
boolean KeyMapSetEntry (TKeyMap *pThis, u8 nTable, u8 nPhyCode, u8 nValue);

u8 KeyMapTranslate (TKeyMap *pThis, u8 nPhyCode, u8 nModifiers);
const char *KeyMapGetString (TKeyMap *pThis, u8 nKeyCode, u8 nModifiers, char Buffer[2]);

u8 KeyMapGetLEDStatus (TKeyMap *pThis);

#endif
