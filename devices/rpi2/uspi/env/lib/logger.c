//
// logger.c
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
#include <uspienv/logger.h>
#include <uspienv/string.h>
#include <uspienv/synchronize.h>
#include <uspienv/alloc.h>
#include <uspienv/util.h>

static TLogger *s_pThis = 0;

void LoggerWrite2 (TLogger *pThis, const char *pString);

void Logger (TLogger *pThis, unsigned nLogLevel, TTimer *pTimer)
{
	pThis->m_nLogLevel = nLogLevel;
	pThis->m_pTimer = pTimer;
	pThis->m_pTarget = 0;

	s_pThis = pThis;
}

void _Logger (TLogger *pThis)
{
	s_pThis = 0;

	pThis->m_pTarget = 0;
	pThis->m_pTimer = 0;
}

boolean LoggerInitialize (TLogger *pThis, TScreenDevice *pTarget)
{
	pThis->m_pTarget = pTarget;
	
	LoggerWrite (pThis, "logger", LogNotice, "Logging started");

	return TRUE;
}

void LoggerWrite (TLogger *pThis, const char *pSource, TLogSeverity Severity, const char *pMessage, ...)
{
	va_list var;
	va_start (var, pMessage);

	LoggerWriteV (pThis, pSource, Severity, pMessage, var);

	va_end (var);
}

void LoggerWriteV (TLogger *pThis, const char *pSource, TLogSeverity Severity, const char *pMessage, va_list Args)
{
	if (Severity > pThis->m_nLogLevel)
	{
		return;
	}

	if (Severity == LogPanic)
	{
		LoggerWrite2 (pThis, "\x1b[1m");
	}

	if (pThis->m_pTimer != 0)
	{
		TString *pTimeString = TimerGetTimeString (pThis->m_pTimer);
		if (pTimeString != 0)
		{
			LoggerWrite2 (pThis, StringGet (pTimeString));
			LoggerWrite2 (pThis, " ");

			_String (pTimeString);
			free (pTimeString);
		}
	}

	LoggerWrite2 (pThis, pSource);
	LoggerWrite2 (pThis, ": ");

	TString Message;
	String (&Message);
	StringFormatV (&Message, pMessage, Args);

	LoggerWrite2 (pThis, StringGet (&Message));

	if (Severity == LogPanic)
	{
		LoggerWrite2 (pThis, "\x1b[0m");
	}

	LoggerWrite2 (pThis, "\n");

	if (Severity == LogPanic)
	{
		DisableInterrupts ();

		while (1)
		{
			// wait forever
		}
	}

	_String (&Message);
}

TLogger *LoggerGet (void)
{
	return s_pThis;
}

void LoggerWrite2 (TLogger *pThis, const char *pString)
{
	size_t nLength = strlen (pString);

	ScreenDeviceWrite (pThis->m_pTarget, pString, nLength);
}
