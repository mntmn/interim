//
// uspienv.c
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
#include <uspienv.h>
#include <uspienv/sysconfig.h>

static TUSPiEnv s_Env;

int USPiEnvInitialize (void)
{
#ifdef ARM_DISABLE_MMU
	MemorySystem (&s_Env.m_Memory, FALSE);
#else
	MemorySystem (&s_Env.m_Memory, TRUE);
#endif

	ScreenDevice (&s_Env.m_Screen, 0, 0);
	if (!ScreenDeviceInitialize (&s_Env.m_Screen))
	{
		_ScreenDevice (&s_Env.m_Screen);

		return 0;
	}

	ExceptionHandler2 (&s_Env.m_ExceptionHandler);
	InterruptSystem (&s_Env.m_Interrupt);
	Timer (&s_Env.m_Timer, &s_Env.m_Interrupt);
	Logger (&s_Env.m_Logger, LogDebug, &s_Env.m_Timer);

	if (   !LoggerInitialize (&s_Env.m_Logger, &s_Env.m_Screen)
	    || !InterruptSystemInitialize (&s_Env.m_Interrupt)
	    || !TimerInitialize (&s_Env.m_Timer))
	{
		_Logger (&s_Env.m_Logger);
		_Timer (&s_Env.m_Timer);
		_InterruptSystem (&s_Env.m_Interrupt);
		_ExceptionHandler (&s_Env.m_ExceptionHandler);
		_ScreenDevice (&s_Env.m_Screen);

		return 0;
	}

	return 1;
}

void USPiEnvClose (void)
{
	_Logger (&s_Env.m_Logger);
	_Timer (&s_Env.m_Timer);
	_InterruptSystem (&s_Env.m_Interrupt);
	_ExceptionHandler (&s_Env.m_ExceptionHandler);
	_ScreenDevice (&s_Env.m_Screen);
}

TScreenDevice *USPiEnvGetScreen (void)
{
	return &s_Env.m_Screen;
}
