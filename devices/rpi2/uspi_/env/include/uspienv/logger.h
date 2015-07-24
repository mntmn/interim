//
// logger.h
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
#ifndef _uspienv_logger_h
#define _uspienv_logger_h

#include <uspienv/screen.h>
#include <uspienv/timer.h>
#include <uspienv/stdarg.h>
#include <uspienv/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum TLogSeverity
{
	LogPanic,
	LogError,
	LogWarning,
	LogNotice,
	LogDebug
}
TLogSeverity;

typedef struct TLogger
{
	unsigned m_nLogLevel;
	TTimer *m_pTimer;

	TScreenDevice *m_pTarget;
}
TLogger;

void Logger (TLogger *pThis, unsigned nLogLevel, TTimer *pTimer /* = 0 */);	// time is not logged if pTimer is 0
void _Logger (TLogger *pThis);

boolean LoggerInitialize (TLogger *pThis, TScreenDevice *pTarget);

void LoggerWrite (TLogger *pThis, const char *pSource, TLogSeverity Severity, const char *pMessage, ...);
void LoggerWriteV (TLogger *pThis, const char *pSource, TLogSeverity Severity, const char *pMessage, va_list Args);

TLogger *LoggerGet (void);

#ifdef __cplusplus
}
#endif

#endif
