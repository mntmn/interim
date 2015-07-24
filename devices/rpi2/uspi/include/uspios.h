//
// uspios.h
//
// External functions used by the USPi library
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
#ifndef _uspios_h
#define _uspios_h

#ifdef __cplusplus
extern "C" {
#endif

//
// System configuration
//
// (Change this before you build the USPi library!)
//
#define GPU_L2_CACHE_ENABLED		// normally enabled (can be disabled in config.txt)

#define HZ	100			// timer ticks / second (set this to your timer interrupt frequency)

// Default keyboard map (enable only one)
#define USPI_DEFAULT_KEYMAP_UK
//#define USPI_DEFAULT_KEYMAP_DE

// Undefine this if you want to use your own implementation of the functions in uspi/util.h
#define USPI_PROVIDE_MEM_FUNCTIONS	// mem*()
#define USPI_PROVIDE_STR_FUNCTIONS	// str*()

//
// Memory allocation
//
// (Must work from interrupt context)
//
void *malloc (unsigned nSize);		// result must be 4-byte aligned
void free (void *pBlock);

//
// Timer
//
void MsDelay (unsigned nMilliSeconds);	
void usDelay (unsigned nMicroSeconds);

typedef void TKernelTimerHandler (unsigned hTimer, void *pParam, void *pContext);

// returns the timer handle (hTimer)
unsigned StartKernelTimer (unsigned	        nHzDelay,	// in HZ units (see "system configuration" above)
			   TKernelTimerHandler *pHandler,
			   void *pParam, void *pContext);	// handed over to the timer handler

void CancelKernelTimer (unsigned hTimer);

//
// Interrupt handling
//
typedef void TInterruptHandler (void *pParam);

// USPi uses USB IRQ 9
void ConnectInterrupt (unsigned nIRQ, TInterruptHandler *pHandler, void *pParam);

//
// Property tags (ARM -> VC)
//
// See: https://github.com/raspberrypi/firmware/wiki/Mailboxes
//

// returns 0 on failure
int SetPowerStateOn (unsigned nDeviceId);	// "set power state" to "on", wait until completed

// returns 0 on failure
int GetMACAddress (unsigned char Buffer[6]);	// "get board MAC address"

//
// Logging
//

// Severity (change this before building if you want different values)
#define LOG_ERROR	1
#define LOG_WARNING	2
#define LOG_NOTICE	3
#define LOG_DEBUG	4

void LogWrite (const char *pSource,		// short name of module
	       unsigned	   Severity,		// see above
	       const char *pMessage, ...);	// uses printf format options

//
// Debug support
//
#ifndef NDEBUG

// display "assertion failed" message and halt
void uspi_assertion_failed (const char *pExpr, const char *pFile, unsigned nLine);

// display hex dump (pSource can be 0)
void DebugHexdump (const void *pBuffer, unsigned nBufLen, const char *pSource /* = 0 */);

#endif

#ifdef __cplusplus
}
#endif

#endif
