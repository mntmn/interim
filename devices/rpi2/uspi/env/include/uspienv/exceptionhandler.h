//
// exceptionhandler.h
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
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
#ifndef _uspienv_exceptionhandler_h
#define _uspienv_exceptionhandler_h

#include <uspienv/exception.h>
#include <uspienv/exceptionstub.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TExceptionHandler
{
}
TExceptionHandler;

void ExceptionHandler2 (TExceptionHandler *pThis);
void _ExceptionHandler (TExceptionHandler *pThis);

void ExceptionHandlerThrow (TExceptionHandler *pThis, unsigned nException);

void ExceptionHandlerThrow2 (TExceptionHandler *pThis, unsigned nException, TAbortFrame *pFrame);

TExceptionHandler *ExceptionHandlerGet (void);

#ifdef __cplusplus
}
#endif

#endif
