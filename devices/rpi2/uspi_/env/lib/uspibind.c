//
// uspibind.cpp
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
#include <uspios.h>
#include <uspienv/timer.h>
#include <uspienv/interrupt.h>
#include <uspienv/bcmpropertytags.h>
#include <uspienv/logger.h>
#include <uspienv/debug.h>
#include <uspienv/util.h>
#include <uspienv/assert.h>

void MsDelay (unsigned nMilliSeconds)
{
	TimerMsDelay (TimerGet (), nMilliSeconds);
}

void usDelay (unsigned nMicroSeconds)
{
	TimerusDelay (TimerGet (), nMicroSeconds);
}

unsigned StartKernelTimer (unsigned nDelay, TKernelTimerHandler *pHandler, void *pParam, void *pContext)
{
	return TimerStartKernelTimer (TimerGet (), nDelay, pHandler, pParam, pContext);
}

void CancelKernelTimer (unsigned hTimer)
{
	TimerCancelKernelTimer (TimerGet (), hTimer);
}

void ConnectInterrupt (unsigned nIRQ, TInterruptHandler *pHandler, void *pParam)
{
	InterruptSystemConnectIRQ (InterruptSystemGet (), nIRQ, pHandler, pParam);
}

int SetPowerStateOn (unsigned nDeviceId)
{
	TBcmPropertyTags Tags;
	BcmPropertyTags (&Tags);
	TPropertyTagPowerState PowerState;
	PowerState.nDeviceId = nDeviceId;
	PowerState.nState = POWER_STATE_ON | POWER_STATE_WAIT;
	if (   !BcmPropertyTagsGetTag (&Tags, PROPTAG_SET_POWER_STATE, &PowerState, sizeof PowerState, 8)
	    ||  (PowerState.nState & POWER_STATE_NO_DEVICE)
	    || !(PowerState.nState & POWER_STATE_ON))
	{
		_BcmPropertyTags (&Tags);

		return 0;
	}
	
	_BcmPropertyTags (&Tags);

	return 1;
}

int GetMACAddress (unsigned char Buffer[6])
{
	TBcmPropertyTags Tags;
	BcmPropertyTags (&Tags);
	TPropertyTagMACAddress MACAddress;
	if (!BcmPropertyTagsGetTag (&Tags, PROPTAG_GET_MAC_ADDRESS, &MACAddress, sizeof MACAddress, 0))
	{
		_BcmPropertyTags (&Tags);

		return 0;
	}

	memcpy (Buffer, MACAddress.Address, 6);
	
	_BcmPropertyTags (&Tags);

	return 1;
}

void LogWrite (const char *pSource, unsigned Severity, const char *pMessage, ...)
{
	va_list var;
	va_start (var, pMessage);

	LoggerWriteV (LoggerGet (), pSource, (TLogSeverity) Severity, pMessage, var);

	va_end (var);
}

#ifndef NDEBUG

void uspi_assertion_failed (const char *pExpr, const char *pFile, unsigned nLine)
{
	assertion_failed (pExpr, pFile, nLine);
}

void DebugHexdump (const void *pBuffer, unsigned nBufLen, const char *pSource)
{
	debug_hexdump (pBuffer, nBufLen, pSource);
}

#endif
