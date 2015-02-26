/*
 * Copyright (C) 2013  Free Software Foundation, Inc.
 *
 * This file is part of GNU lightning.
 *
 * GNU lightning is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU lightning is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * Authors:
 *	Paulo Cesar Pereira de Andrade
 */

#ifndef _jit_sparc_h
#define _jit_sparc_h

#define JIT_HASH_CONSTS		1
#define JIT_NUM_OPERANDS	3

/*
 * Types
 */
#define JIT_FP			_FP
typedef enum {
#define jit_r(i)		(_G2 + (i))
#define jit_r_num()		3
#define jit_v(i)		(_L0 + (i))
#define jit_v_num()		8
#define jit_f(i)		(_F0 + ((i) << 1))
#define jit_f_num()		8
#define JIT_R0			_G2
#define JIT_R1			_G3
#define JIT_R2			_G4
#define JIT_V0			_L0
#define JIT_V1			_L1
#define JIT_V2			_L2
#define JIT_V3			_L3
#define JIT_V4			_L4
#define JIT_V5			_L5
#define JIT_V6			_L6
#define JIT_V7			_L7
    _G0, _G1, _G2, _G3, _G4, _G5, _G6, _G7,
    _O0, _O1, _O2, _O3, _O4, _O5, _SP, _O7,
    _L0, _L1, _L2, _L3, _L4, _L5, _L6, _L7,
    _I0, _I1, _I2, _I3, _I4, _I5, _FP, _I7,
#define JIT_F0			_F0
#define JIT_F1			_F2
#define JIT_F2			_F4
#define JIT_F3			_F6
#define JIT_F4			_F8
#define JIT_F5			_F10
#define JIT_F6			_F12
#define JIT_F7			_F14
    _F0, _F1,  _F2,  _F3,  _F4,  _F5,  _F6,  _F7,
    _F8, _F9, _F10, _F11, _F12, _F13, _F14, _F15,
#define JIT_NOREG		_NOREG
    _NOREG,
} jit_reg_t;

#endif /* _jit_sparc_h */
