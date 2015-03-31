/*
 * Copyright (C) 2012, 2013  Free Software Foundation, Inc.
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

#define rc(value)			jit_class_##value
#define rn(reg)				(jit_regno(_rvs[jit_regno(reg)].spec))
#define jit_arg_reg_p(i)		((i) >= 0 && (i) < 8)
#define jit_arg_f_reg_p(i)		((i) >= 0 && (i) < 13)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#  define C_DISP			0
#  define S_DISP			0
#  define I_DISP			0
#  define F_DISP			0
#else
#  define C_DISP			(__WORDSIZE >> 3) - sizeof(jit_int8_t)
#  define S_DISP			(__WORDSIZE >> 3) - sizeof(jit_int16_t)
#  define I_DISP			(__WORDSIZE >> 3) - sizeof(jit_int32_t)
#  define F_DISP			(__WORDSIZE >> 3) - sizeof(jit_float32_t)
#endif

/*
 * Prototypes
 */
#define patch(instr, node)		_patch(_jit, instr, node)
static void _patch(jit_state_t*,jit_word_t,jit_node_t*);

/* libgcc */
extern void __clear_cache(void *, void *);

#define PROTO				1
#  include "jit_ppc-cpu.c"
#  include "jit_ppc-fpu.c"
#undef PROTO

/*
 * Initialization
 */
jit_register_t		_rvs[] = {
    { rc(gpr) | 0,			"r0" },
#if __ppc__
    { rc(gpr) | 11,			"r11" },
    { rc(gpr) | 12,			"r12" },
    { rc(gpr) | 13,			"r13" },
    { rc(gpr) | 2,			"r2" },
#else
    { rc(sav) | 11,			"r11" },	/* env */
    { rc(sav) | 12,			"r12" },	/* exception */
    { rc(sav) | 13,			"r13" },	/* thread */
    { rc(sav) | 2,			"r2" },		/* toc */
#endif
    { rc(sav) | rc(gpr) | 14,		"r14" },
    { rc(sav) | rc(gpr) | 15,		"r15" },
    { rc(sav) | rc(gpr) | 16,		"r16" },
    { rc(sav) | rc(gpr) | 17,		"r17" },
    { rc(sav) | rc(gpr) | 18,		"r18" },
    { rc(sav) | rc(gpr) | 19,		"r19" },
    { rc(sav) | rc(gpr) | 20,		"r20" },
    { rc(sav) | rc(gpr) | 21,		"r21" },
    { rc(sav) | rc(gpr) | 22,		"r22" },
    { rc(sav) | rc(gpr) | 23,		"r23" },
    { rc(sav) | rc(gpr) | 24,		"r24" },
    { rc(sav) | rc(gpr) | 25,		"r25" },
    { rc(sav) | rc(gpr) | 26,		"r26" },
    { rc(sav) | rc(gpr) | 27,		"r27" },
    { rc(sav) | rc(gpr) | 28,		"r28" },
    { rc(sav) | rc(gpr) | 29,		"r29" },
    { rc(sav) | rc(gpr) | 30,		"r30" },
    { rc(sav) | 1,			"r1" },
    { rc(sav) | 31,			"r31" },
    { rc(arg) | rc(gpr) | 10,		"r10" },
    { rc(arg) | rc(gpr) | 9,		"r9" },
    { rc(arg) | rc(gpr) | 8,		"r8" },
    { rc(arg) | rc(gpr) | 7,		"r7" },
    { rc(arg) | rc(gpr) | 6,		"r6" },
    { rc(arg) | rc(gpr) | 5,		"r5" },
    { rc(arg) | rc(gpr) | 4,		"r4" },
    { rc(arg) | rc(gpr) | 3,		"r3" },
    { rc(fpr) | 0,			"f0" },
    { rc(sav) | rc(fpr) | 14,		"f14" },
    { rc(sav) | rc(fpr) | 15,		"f15" },
    { rc(sav) | rc(fpr) | 16,		"f16" },
    { rc(sav) | rc(fpr) | 17,		"f17" },
    { rc(sav) | rc(fpr) | 18,		"f18" },
    { rc(sav) | rc(fpr) | 19,		"f19" },
    { rc(sav) | rc(fpr) | 20,		"f20" },
    { rc(sav) | rc(fpr) | 21,		"f21" },
    { rc(sav) | rc(fpr) | 22,		"f22" },
    { rc(sav) | rc(fpr) | 23,		"f23" },
    { rc(sav) | rc(fpr) | 24,		"f24" },
    { rc(sav) | rc(fpr) | 25,		"f25" },
    { rc(sav) | rc(fpr) | 26,		"f26" },
    { rc(sav) | rc(fpr) | 27,		"f27" },
    { rc(sav) | rc(fpr) | 28,		"f28" },
    { rc(sav) | rc(fpr) | 29,		"f29" },
    { rc(sav) | rc(fpr) | 30,		"f30" },
    { rc(sav) | rc(fpr) | 31,		"f31" },
    { rc(arg) | rc(fpr) | 13,		"f13" },
    { rc(arg) | rc(fpr) | 12,		"f12" },
    { rc(arg) | rc(fpr) | 11,		"f11" },
    { rc(arg) | rc(fpr) | 10,		"f10" },
    { rc(arg) | rc(fpr) | 9,		"f9" },
    { rc(arg) | rc(fpr) | 8,		"f8" },
    { rc(arg) | rc(fpr) | 7,		"f7" },
    { rc(arg) | rc(fpr) | 6,		"f6" },
    { rc(arg) | rc(fpr) | 5,		"f5" },
    { rc(arg) | rc(fpr) | 4,		"f4" },
    { rc(arg) | rc(fpr) | 3,		"f3" },
    { rc(arg) | rc(fpr) | 2,		"f2" },
    { rc(arg) | rc(fpr) | 1,		"f1" },
    { _NOREG,				"<none>" },
};

/*
 * Implementation
 */
void
jit_get_cpu(void)
{
}

void
_jit_init(jit_state_t *_jit)
{
    _jitc->reglen = jit_size(_rvs) - 1;
}

void
_jit_prolog(jit_state_t *_jit)
{
    jit_int32_t		 offset;

    if (_jitc->function)
	jit_epilog();
    assert(jit_regset_cmp_ui(&_jitc->regarg, 0) == 0);
    jit_regset_set_ui(&_jitc->regsav, 0);
    offset = _jitc->functions.offset;
    if (offset >= _jitc->functions.length) {
	jit_realloc((jit_pointer_t *)&_jitc->functions.ptr,
		    _jitc->functions.length * sizeof(jit_function_t),
		    (_jitc->functions.length + 16) * sizeof(jit_function_t));
	_jitc->functions.length += 16;
    }
    _jitc->function = _jitc->functions.ptr + _jitc->functions.offset++;
    _jitc->function->self.size = params_offset;
    _jitc->function->self.argi = _jitc->function->self.argf =
	_jitc->function->self.alen = 0;
    /* float conversion */
    _jitc->function->self.aoff = alloca_offset - 8;
    _jitc->function->self.call = jit_call_default;
    jit_alloc((jit_pointer_t *)&_jitc->function->regoff,
	      _jitc->reglen * sizeof(jit_int32_t));

    _jitc->function->prolog = jit_new_node_no_link(jit_code_prolog);
    jit_link(_jitc->function->prolog);
    _jitc->function->prolog->w.w = offset;
    _jitc->function->epilog = jit_new_node_no_link(jit_code_epilog);
    /*	u:	label value
     *	v:	offset in blocks vector
     *	w:	offset in functions vector
     */
    _jitc->function->epilog->w.w = offset;

    jit_regset_new(&_jitc->function->regset);
}

jit_int32_t
_jit_allocai(jit_state_t *_jit, jit_int32_t length)
{
    assert(_jitc->function);
    switch (length) {
	case 0:	case 1:						break;
	case 2:		_jitc->function->self.aoff &= -2;	break;
	case 3:	case 4:	_jitc->function->self.aoff &= -4;	break;
	default:	_jitc->function->self.aoff &= -8;	break;
    }
    _jitc->function->self.aoff -= length;
    return (_jitc->function->self.aoff);
}

void
_jit_ret(jit_state_t *_jit)
{
    jit_node_t		*instr;

    assert(_jitc->function);

    /* jump to epilog */
    instr = jit_jmpi();
    jit_patch_at(instr, _jitc->function->epilog);
}

void
_jit_retr(jit_state_t *_jit, jit_int32_t u)
{
    if (JIT_RET != u)
	jit_movr(JIT_RET, u);
    else
	jit_live(JIT_RET);
    jit_ret();
}

void
_jit_reti(jit_state_t *_jit, jit_word_t u)
{
    jit_movi(JIT_RET, u);
    jit_ret();
}

void
_jit_retr_f(jit_state_t *_jit, jit_int32_t u)
{
    if (JIT_RET != u)
	jit_movr_f(JIT_FRET, u);
    else
	jit_live(JIT_FRET);
    jit_ret();
}

void
_jit_reti_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_movi_f(JIT_FRET, u);
    jit_ret();
}

void
_jit_retr_d(jit_state_t *_jit, jit_int32_t u)
{
    if (JIT_FRET != u)
	jit_movr_d(JIT_FRET, u);
    else
	jit_live(JIT_FRET);
    jit_ret();
}

void
_jit_reti_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_movi_d(JIT_FRET, u);
    jit_ret();
}

/* must be called internally only */
void
_jit_epilog(jit_state_t *_jit)
{
    assert(_jitc->function);
    assert(_jitc->function->epilog->next == NULL);
    jit_link(_jitc->function->epilog);
    _jitc->function = NULL;
}

jit_bool_t
_jit_arg_register_p(jit_state_t *_jit, jit_node_t *u)
{
    if (u->code == jit_code_arg)
	return (jit_arg_reg_p(u->u.w));
    assert(u->code == jit_code_arg_f || u->code == jit_code_arg_d);
    return (jit_arg_f_reg_p(u->u.w));
}

jit_node_t *
_jit_arg(jit_state_t *_jit)
{
    jit_int32_t		offset;
    assert(_jitc->function);
    if (jit_arg_reg_p(_jitc->function->self.argi))
	offset = _jitc->function->self.argi++;
    else
	offset = _jitc->function->self.size;
    _jitc->function->self.size += sizeof(jit_word_t);
    return (jit_new_node_w(jit_code_arg, offset));
}

jit_node_t *
_jit_arg_f(jit_state_t *_jit)
{
    jit_int32_t		offset;
    assert(_jitc->function);
    if (jit_arg_f_reg_p(_jitc->function->self.argf))
	offset = _jitc->function->self.argf++;
    else
	offset = _jitc->function->self.size + F_DISP;
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
#if __WORDSIZE == 32
	_jitc->function->self.argi += 2;
#else
	_jitc->function->self.argi++;
#endif
    }
    _jitc->function->self.size += sizeof(jit_word_t);
    return (jit_new_node_w(jit_code_arg_f, offset));
}

jit_node_t *
_jit_arg_d(jit_state_t *_jit)
{
    jit_int32_t		offset;
    assert(_jitc->function);
    if (jit_arg_f_reg_p(_jitc->function->self.argf))
	offset = _jitc->function->self.argf++;
    else
	offset = _jitc->function->self.size;
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
#if __WORDSIZE == 32
	_jitc->function->self.argi += 2;
#else
	_jitc->function->self.argi++;
#endif
    }
    _jitc->function->self.size += sizeof(jit_float64_t);
    return (jit_new_node_w(jit_code_arg_d, offset));
}

void
_jit_getarg_c(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_c(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_c(u, JIT_FP, v->u.w + C_DISP);
}

void
_jit_getarg_uc(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_uc(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_uc(u, JIT_FP, v->u.w + C_DISP);
}

void
_jit_getarg_s(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_s(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_s(u, JIT_FP, v->u.w + S_DISP);
}

void
_jit_getarg_us(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_us(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_us(u, JIT_FP, v->u.w + S_DISP);
}

void
_jit_getarg_i(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w)) {
#if __WORDSIZE == 32
	jit_movr(u, JIT_RA0 - v->u.w);
#else
	jit_extr_i(u, JIT_RA0 - v->u.w);
#endif
    }
    else
	jit_ldxi_i(u, JIT_FP, v->u.w + I_DISP);
}

#if __WORDSIZE == 64
void
_jit_getarg_ui(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_ui(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_ui(u, JIT_FP, v->u.w + I_DISP);
}

void
_jit_getarg_l(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_movr(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_l(u, JIT_FP, v->u.w);
}
#endif

void
_jit_putargr(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_movr(JIT_RA0 - v->u.w, u);
    else
	jit_stxi(v->u.w, JIT_FP, u);
}

void
_jit_putargi(jit_state_t *_jit, jit_word_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg);
    if (jit_arg_reg_p(v->u.w))
	jit_movi(JIT_RA0 - v->u.w, u);
    else {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(v->u.w, JIT_FP, regno);
	jit_unget_reg(regno);
    }
}

void
_jit_getarg_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_d(u, JIT_FA0 - v->u.w);
    else
	jit_ldxi_f(u, JIT_FP, v->u.w);
}

void
_jit_putargr_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_d(JIT_FA0 - v->u.w, u);
    else
	jit_stxi_f(v->u.w, JIT_FP, u);
}

void
_jit_putargi_f(jit_state_t *_jit, jit_float32_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_f);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movi_d(JIT_FA0 - v->u.w, u);
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
	jit_stxi_f(v->u.w, JIT_FP, regno);
	jit_unget_reg(regno);
    }
}

void
_jit_getarg_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_d(u, JIT_FA0 - v->u.w);
    else
	jit_ldxi_d(u, JIT_FP, v->u.w);
}

void
_jit_putargr_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_d(JIT_FA0 - v->u.w, u);
    else
	jit_stxi_d(v->u.w, JIT_FP, u);
}

void
_jit_putargi_d(jit_state_t *_jit, jit_float64_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_d);
    if (jit_arg_f_reg_p(v->u.w))
	jit_movi_d(JIT_FA0 - v->u.w, u);
    else {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
	jit_stxi_d(v->u.w, JIT_FP, regno);
	jit_unget_reg(regno);
    }
}

void
_jit_pushargr(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else
	jit_stxi(_jitc->function->call.size + params_offset, JIT_SP, u);
    _jitc->function->call.size += sizeof(jit_word_t);
}

void
_jit_pushargi(jit_state_t *_jit, jit_word_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
    }
    else {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(_jitc->function->call.size + params_offset, JIT_SP, regno);
	jit_unget_reg(regno);
    }
    _jitc->function->call.size += sizeof(jit_word_t);
}

void
_jit_pushargr_f(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movr_d(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    /* in case of excess arguments */
	    if (jit_arg_reg_p(_jitc->function->call.argi)) {
#if __WORDSIZE == 32
		_jitc->function->call.argi += 2;
#else
		_jitc->function->call.argi++;
#endif
	    }
	    _jitc->function->call.size += sizeof(jit_word_t);
	    return;
	}
    }
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	/* use reserved 8 bytes area */
	jit_stxi_d(alloca_offset - 8, JIT_FP, u);
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 8);
	_jitc->function->call.argi++;
#if __WORDSIZE == 32
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 4);
	_jitc->function->call.argi++;
#endif
    }
    else
	jit_stxi_f(_jitc->function->call.size + params_offset + F_DISP,
		   JIT_SP, u);
    _jitc->function->call.size += sizeof(jit_word_t);
}

void
_jit_pushargi_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_int32_t		 regno;

    assert(_jitc->function);
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movi_d(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    /* in case of excess arguments */
	    if (jit_arg_reg_p(_jitc->function->call.argi)) {
#if __WORDSIZE == 32
		_jitc->function->call.argi += 2;
#else
		_jitc->function->call.argi++;
#endif
	    }
	    _jitc->function->call.size += sizeof(jit_word_t);
	    return;
	}
    }
    regno = jit_get_reg(jit_class_fpr);
    jit_movi_f(regno, u);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	/* use reserved 8 bytes area */
	jit_stxi_d(alloca_offset - 8, JIT_FP, regno);
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 8);
	_jitc->function->call.argi++;
#if __WORDSIZE == 32
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 4);
	_jitc->function->call.argi++;
#endif
    }
    else
	jit_stxi_f(_jitc->function->call.size + params_offset + F_DISP,
		   JIT_SP, regno);
    _jitc->function->call.size += sizeof(jit_word_t);
    jit_unget_reg(regno);
}

void
_jit_pushargr_d(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movr_d(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    /* in case of excess arguments */
	    if (jit_arg_reg_p(_jitc->function->call.argi)) {
#if __WORDSIZE == 32
		_jitc->function->call.argi += 2;
#else
		_jitc->function->call.argi++;
#endif
	    }
	    _jitc->function->call.size += sizeof(jit_float64_t);
	    return;
	}
    }
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	/* use reserved 8 bytes area */
	jit_stxi_d(alloca_offset - 8, JIT_FP, u);
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 8);
	_jitc->function->call.argi++;
#if __WORDSIZE == 32
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 4);
	_jitc->function->call.argi++;
#endif
    }
    else
	jit_stxi_d(_jitc->function->call.size + params_offset, JIT_SP, u);
    _jitc->function->call.size += sizeof(jit_float64_t);
}

void
_jit_pushargi_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_int32_t		 regno;

    assert(_jitc->function);
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movi_d(JIT_FA0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
	if (!(_jitc->function->call.call & jit_call_varargs)) {
	    /* in case of excess arguments */
	    if (jit_arg_reg_p(_jitc->function->call.argi)) {
#if __WORDSIZE == 32
		_jitc->function->call.argi += 2;
#else
		_jitc->function->call.argi++;
#endif
	    }
	    _jitc->function->call.size += sizeof(jit_float64_t);
	    return;
	}
    }
    regno = jit_get_reg(jit_class_fpr);
    jit_movi_d(regno, u);
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	/* use reserved 8 bytes area */
	jit_stxi_d(alloca_offset - 8, JIT_FP, regno);
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 8);
	_jitc->function->call.argi++;
#if __WORDSIZE == 32
	jit_ldxi(JIT_RA0 - _jitc->function->call.argi, JIT_FP,
		 alloca_offset - 4);
	_jitc->function->call.argi++;
#endif
    }
    else
	jit_stxi_d(_jitc->function->call.size + params_offset, JIT_SP, regno);
    _jitc->function->call.size += sizeof(jit_float64_t);
    jit_unget_reg(regno);
}

jit_bool_t
_jit_regarg_p(jit_state_t *_jit, jit_node_t *node, jit_int32_t regno)
{
    jit_int32_t		spec;
    spec = jit_class(_rvs[regno].spec);
    if (spec & jit_class_arg) {
	if (spec & jit_class_gpr) {
	    regno = JIT_RA0 - regno;
	    if (regno >= 0 && regno < node->v.w)
		return (1);
	}
	else if (spec & jit_class_fpr) {
	    regno = JIT_FA0 - regno;
	    if (regno >= 0 && regno < node->w.w)
		return (1);
	}
    }
    return (0);
}

void
_jit_finishr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_node_t		*call;
    assert(_jitc->function);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    call = jit_callr(r0);
    call->v.w = _jitc->function->call.argi;
    call->w.w = _jitc->function->call.argf;
    _jitc->function->call.argi = _jitc->function->call.argf = 0;
    _jitc->prepare = 0;
}

jit_node_t *
_jit_finishi(jit_state_t *_jit, jit_pointer_t i0)
{
    jit_node_t		*node;
    assert(_jitc->function);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
    node = jit_calli(i0);
    node->v.w = _jitc->function->call.argi;
    node->w.w = _jitc->function->call.argf;
    _jitc->function->call.argi = _jitc->function->call.argf = 0;
    _jitc->prepare = 0;
    return (node);
}

void
_jit_retval_c(jit_state_t *_jit, jit_int32_t r0)
{
    jit_extr_c(r0, JIT_RET);
}

void
_jit_retval_uc(jit_state_t *_jit, jit_int32_t r0)
{
    jit_extr_uc(r0, JIT_RET);
}

void
_jit_retval_s(jit_state_t *_jit, jit_int32_t r0)
{
    jit_extr_s(r0, JIT_RET);
}

void
_jit_retval_us(jit_state_t *_jit, jit_int32_t r0)
{
    jit_extr_us(r0, JIT_RET);
}

void
_jit_retval_i(jit_state_t *_jit, jit_int32_t r0)
{
#if __WORDSIZE == 32
    if (r0 != JIT_RET)
	jit_movr(r0, JIT_RET);
#else
    jit_extr_i(r0, JIT_RET);
#endif
}

#if __WORDSIZE == 64
void
_jit_retval_ui(jit_state_t *_jit, jit_int32_t r0)
{
    jit_extr_ui(r0, JIT_RET);
}

void
_jit_retval_l(jit_state_t *_jit, jit_int32_t r0)
{
    if (r0 != JIT_RET)
	jit_movr(r0, JIT_RET);
}
#endif

void
_jit_retval_f(jit_state_t *_jit, jit_int32_t r0)
{
    jit_retval_d(r0);
}

void
_jit_retval_d(jit_state_t *_jit, jit_int32_t r0)
{
    if (r0 != JIT_FRET)
	jit_movr_d(r0, JIT_FRET);
}

jit_pointer_t
_emit_code(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_node_t		*temp;
    jit_word_t		 word;
    jit_int32_t		 value;
    jit_int32_t		 offset;
    struct {
	jit_node_t	*node;
	jit_word_t	 word;
	jit_word_t	 patch_offset;
#if __powerpc__
	jit_word_t	 prolog_offset;
#endif
    } undo;

    _jitc->function = NULL;

    jit_reglive_setup();

    undo.word = 0;
    undo.node = NULL;
    undo.patch_offset = 0;

#if __powerpc__ && !ABI_ELFv2
    undo.prolog_offset = 0;
    for (node = _jitc->head; node; node = node->next)
	if (node->code != jit_code_label &&
	    node->code != jit_code_note &&
	    node->code != jit_code_name)
	    break;
    if (node && (node->code != jit_code_prolog ||
		 !(_jitc->functions.ptr + node->w.w)->assume_frame)) {
	/* code may start with a jump so add an initial function descriptor */
	word = _jit->pc.w + sizeof(void*) * 3;
	iw(word);			/* addr */
	iw(0);				/* toc */
	iw(0);				/* env */
    }
#endif

#define case_rr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w), rn(node->v.w));		\
		break
#define case_rw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w), node->v.w);		\
		break
#define case_wr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w, rn(node->v.w));		\
		break
#define case_rrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w),				\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_rrrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_rrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w), rn(node->v.w), node->w.w);	\
		break
#define case_rrrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), node->w.w);		\
		break
#define case_rrf(name, type, size)					\
	    case jit_code_##name##i##type:				\
		assert(node->flag & jit_flag_data);			\
		name##i##type(rn(node->u.w), rn(node->v.w),		\
			      (jit_float##size##_t *)node->w.n->u.w);	\
		break
#define case_wrr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w, rn(node->v.w), rn(node->w.w));	\
		break
#define case_brr(name, type)						\
	    case jit_code_##name##r##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##r##type(temp->u.w, rn(node->v.w),		\
				  rn(node->w.w));			\
		else {							\
		    word = name##r##type(_jit->pc.w,			\
					 rn(node->v.w), rn(node->w.w));	\
		    patch(word, node);					\
		}							\
		break
#define case_brw(name, type)						\
	    case jit_code_##name##i##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i##type(temp->u.w,				\
				  rn(node->v.w), node->w.w);		\
		else {							\
		    word = name##i##type(_jit->pc.w,			\
					 rn(node->v.w), node->w.w);	\
		    patch(word, node);					\
		}							\
		break
#define case_brf(name, type, size)					\
	    case jit_code_##name##i##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i##type(temp->u.w, rn(node->v.w),		\
				(jit_float##size##_t *)node->w.n->u.w);	\
		else {							\
		    word = name##i##type(_jit->pc.w, rn(node->v.w),	\
				(jit_float##size##_t *)node->w.n->u.w);	\
		    patch(word, node);					\
		}							\
		break
    for (node = _jitc->head; node; node = node->next) {
	if (_jit->pc.uc >= _jitc->code.end)
	    return (NULL);

	value = jit_classify(node->code);
	jit_regarg_set(node, value);
	switch (node->code) {
	    case jit_code_align:
		assert(!(node->u.w & (node->u.w - 1)) &&
		       node->u.w <= sizeof(jit_word_t));
		if (node->u.w == sizeof(jit_word_t) &&
		    (word = _jit->pc.w & (sizeof(jit_word_t) - 1)))
		    nop(sizeof(jit_word_t) - word);
		break;
	    case jit_code_note:		case jit_code_name:
		node->u.w = _jit->pc.w;
		break;
	    case jit_code_label:
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		break;
		case_rrr(add,);
		case_rrw(add,);
		case_rrr(addc,);
		case_rrw(addc,);
		case_rrr(addx,);
		case_rrw(addx,);
		case_rrr(sub,);
		case_rrw(sub,);
		case_rrr(subc,);
		case_rrw(subc,);
		case_rrr(subx,);
		case_rrw(subx,);
		case_rrw(rsb,);
		case_rrr(mul,);
		case_rrw(mul,);
		case_rrrr(qmul,);
		case_rrrw(qmul,);
		case_rrrr(qmul, _u);
		case_rrrw(qmul, _u);
		case_rrr(div,);
		case_rrw(div,);
		case_rrr(div, _u);
		case_rrw(div, _u);
		case_rrrr(qdiv,);
		case_rrrw(qdiv,);
		case_rrrr(qdiv, _u);
		case_rrrw(qdiv, _u);
		case_rrr(rem,);
		case_rrw(rem,);
		case_rrr(rem, _u);
		case_rrw(rem, _u);
		case_rrr(and,);
		case_rrw(and,);
		case_rrr(or,);
		case_rrw(or,);
		case_rrr(xor,);
		case_rrw(xor,);
		case_rrr(lsh,);
		case_rrw(lsh,);
		case_rrr(rsh,);
		case_rrw(rsh,);
		case_rrr(rsh, _u);
		case_rrw(rsh, _u);
		case_rr(ext, _c);
		case_rr(ext, _uc);
		case_rr(ext, _s);
		case_rr(ext, _us);
#  if __WORDSIZE == 64
		case_rr(ext, _i);
		case_rr(ext, _ui);
#  endif
		case_rr(hton, _us);
		case_rr(hton, _ui);
#  if __WORDSIZE == 64
		case_rr(hton, _ul);
#  endif
		case_rr(neg,);
		case_rr(com,);
		case_rr(mov,);
	    case jit_code_movi:
		if (node->flag & jit_flag_node) {
		    temp = node->v.n;
		    if (temp->code == jit_code_data ||
			(temp->code == jit_code_label &&
			 (temp->flag & jit_flag_patch)))
			movi(rn(node->u.w), temp->u.w);
		    else {
			assert(temp->code == jit_code_label ||
			       temp->code == jit_code_epilog);
			word = movi_p(rn(node->u.w), node->v.w);
			patch(word, node);
		    }
		}
		else
		    movi(rn(node->u.w), node->v.w);
		break;
		case_rr(trunc, _f_i);
		case_rr(trunc, _d_i);
#  if __WORDSIZE == 64
		case_rr(trunc, _f_l);
		case_rr(trunc, _d_l);
#  endif
		case_rrr(lt,);
		case_rrw(lt,);
		case_rrr(lt, _u);
		case_rrw(lt, _u);
		case_rrr(le,);
		case_rrw(le,);
		case_rrr(le, _u);
		case_rrw(le, _u);
		case_rrr(eq,);
		case_rrw(eq,);
		case_rrr(ge,);
		case_rrw(ge,);
		case_rrr(ge, _u);
		case_rrw(ge, _u);
		case_rrr(gt,);
		case_rrw(gt,);
		case_rrr(gt, _u);
		case_rrw(gt, _u);
		case_rrr(ne,);
		case_rrw(ne,);
		case_rr(ld, _c);
		case_rw(ld, _c);
		case_brr(blt,);
		case_brw(blt,);
		case_brr(blt, _u);
		case_brw(blt, _u);
		case_brr(ble,);
		case_brw(ble,);
		case_brr(ble, _u);
		case_brw(ble, _u);
		case_brr(beq,);
		case_brw(beq,);
		case_brr(bge,);
		case_brw(bge,);
		case_brr(bge, _u);
		case_brw(bge, _u);
		case_brr(bgt,);
		case_brw(bgt,);
		case_brr(bgt, _u);
		case_brw(bgt, _u);
		case_brr(bne,);
		case_brw(bne,);
		case_brr(bms,);
		case_brw(bms,);
		case_brr(bmc,);
		case_brw(bmc,);
		case_brr(boadd,);
		case_brw(boadd,);
		case_brr(boadd, _u);
		case_brw(boadd, _u);
		case_brr(bxadd,);
		case_brw(bxadd,);
		case_brr(bxadd, _u);
		case_brw(bxadd, _u);
		case_brr(bosub,);
		case_brw(bosub,);
		case_brr(bosub, _u);
		case_brw(bosub, _u);
		case_brr(bxsub,);
		case_brw(bxsub,);
		case_brr(bxsub, _u);
		case_brw(bxsub, _u);
		case_rrr(ldx, _c);
		case_rrw(ldx, _c);
		case_rr(ld, _uc);
		case_rw(ld, _uc);
		case_rrr(ldx, _uc);
		case_rrw(ldx, _uc);
		case_rr(ld, _s);
		case_rw(ld, _s);
		case_rrr(ldx, _s);
		case_rrw(ldx, _s);
		case_rr(ld, _us);
		case_rw(ld, _us);
		case_rrr(ldx, _us);
		case_rrw(ldx, _us);
		case_rr(ld, _i);
		case_rw(ld, _i);
		case_rrr(ldx, _i);
		case_rrw(ldx, _i);
#if __WORDSIZE == 64
		case_rr(ld, _ui);
		case_rw(ld, _ui);
		case_rrr(ldx, _ui);
		case_rrw(ldx, _ui);
		case_rr(ld, _l);
		case_rw(ld, _l);
		case_rrr(ldx, _l);
		case_rrw(ldx, _l);
#endif
		case_rr(st, _c);
		case_wr(st, _c);
		case_rrr(stx, _c);
		case_wrr(stx, _c);
		case_rr(st, _s);
		case_wr(st, _s);
		case_rrr(stx, _s);
		case_wrr(stx, _s);
		case_rr(st, _i);
		case_wr(st, _i);
		case_rrr(stx, _i);
		case_wrr(stx, _i);
#if __WORDSIZE == 64
		case_rr(st, _l);
		case_wr(st, _l);
		case_rrr(stx, _l);
		case_wrr(stx, _l);
#endif
		case_rr(mov, _f);
	    case jit_code_movi_f:
		assert(node->flag & jit_flag_data);
		movi_f(rn(node->u.w), (jit_float32_t *)node->v.n->u.w);
		break;
		case_rr(ext, _f);
		case_rr(ext, _d_f);
		case_rr(abs, _f);
		case_rr(neg, _f);
		case_rr(sqrt, _f);
		case_rrr(add, _f);
		case_rrf(add, _f, 32);
		case_rrr(sub, _f);
		case_rrf(sub, _f, 32);
		case_rrf(rsb, _f, 32);
		case_rrr(mul, _f);
		case_rrf(mul, _f, 32);
		case_rrr(div, _f);
		case_rrf(div, _f, 32);
		case_rrr(lt, _f);
		case_rrf(lt, _f, 32);
		case_rrr(le, _f);
		case_rrf(le, _f, 32);
		case_rrr(eq, _f);
		case_rrf(eq, _f, 32);
		case_rrr(ge, _f);
		case_rrf(ge, _f, 32);
		case_rrr(gt, _f);
		case_rrf(gt, _f, 32);
		case_rrr(ne, _f);
		case_rrf(ne, _f, 32);
		case_rrr(unlt, _f);
		case_rrf(unlt, _f, 32);
		case_rrr(unle, _f);
		case_rrf(unle, _f, 32);
		case_rrr(uneq, _f);
		case_rrf(uneq, _f, 32);
		case_rrr(unge, _f);
		case_rrf(unge, _f, 32);
		case_rrr(ungt, _f);
		case_rrf(ungt, _f, 32);
		case_rrr(ltgt, _f);
		case_rrf(ltgt, _f, 32);
		case_rrr(ord, _f);
		case_rrf(ord, _f, 32);
		case_rrr(unord, _f);
		case_rrf(unord, _f, 32);
		case_brr(blt, _f);
		case_brf(blt, _f, 32);
		case_brr(ble, _f);
		case_brf(ble, _f, 32);
		case_brr(beq, _f);
		case_brf(beq, _f, 32);
		case_brr(bge, _f);
		case_brf(bge, _f, 32);
		case_brr(bgt, _f);
		case_brf(bgt, _f, 32);
		case_brr(bne, _f);
		case_brf(bne, _f, 32);
		case_brr(bunlt, _f);
		case_brf(bunlt, _f, 32);
		case_brr(bunle, _f);
		case_brf(bunle, _f, 32);
		case_brr(buneq, _f);
		case_brf(buneq, _f, 32);
		case_brr(bunge, _f);
		case_brf(bunge, _f, 32);
		case_brr(bungt, _f);
		case_brf(bungt, _f, 32);
		case_brr(bltgt, _f);
		case_brf(bltgt, _f, 32);
		case_brr(bord, _f);
		case_brf(bord, _f, 32);
		case_brr(bunord, _f);
		case_brf(bunord, _f, 32);
		case_rr(ld, _f);
		case_rw(ld, _f);
		case_rrr(ldx, _f);
		case_rrw(ldx, _f);
		case_rr(st, _f);
		case_wr(st, _f);
		case_rrr(stx, _f);
		case_wrr(stx, _f);
		case_rr(mov, _d);
	    case jit_code_movi_d:
		assert(node->flag & jit_flag_data);
		movi_d(rn(node->u.w), (jit_float64_t *)node->v.n->u.w);
		break;
		case_rr(ext, _d);
		case_rr(ext, _f_d);
		case_rr(abs, _d);
		case_rr(neg, _d);
		case_rr(sqrt, _d);
		case_rrr(add, _d);
		case_rrf(add, _d, 64);
		case_rrr(sub, _d);
		case_rrf(sub, _d, 64);
		case_rrf(rsb, _d, 64);
		case_rrr(mul, _d);
		case_rrf(mul, _d, 64);
		case_rrr(div, _d);
		case_rrf(div, _d, 64);
		case_rrr(lt, _d);
		case_rrf(lt, _d, 64);
		case_rrr(le, _d);
		case_rrf(le, _d, 64);
		case_rrr(eq, _d);
		case_rrf(eq, _d, 64);
		case_rrr(ge, _d);
		case_rrf(ge, _d, 64);
		case_rrr(gt, _d);
		case_rrf(gt, _d, 64);
		case_rrr(ne, _d);
		case_rrf(ne, _d, 64);
		case_rrr(unlt, _d);
		case_rrf(unlt, _d, 64);
		case_rrr(unle, _d);
		case_rrf(unle, _d, 64);
		case_rrr(uneq, _d);
		case_rrf(uneq, _d, 64);
		case_rrr(unge, _d);
		case_rrf(unge, _d, 64);
		case_rrr(ungt, _d);
		case_rrf(ungt, _d, 64);
		case_rrr(ltgt, _d);
		case_rrf(ltgt, _d, 64);
		case_rrr(ord, _d);
		case_rrf(ord, _d, 64);
		case_rrr(unord, _d);
		case_rrf(unord, _d, 64);
		case_brr(blt, _d);
		case_brf(blt, _d, 64);
		case_brr(ble, _d);
		case_brf(ble, _d, 64);
		case_brr(beq, _d);
		case_brf(beq, _d, 64);
		case_brr(bge, _d);
		case_brf(bge, _d, 64);
		case_brr(bgt, _d);
		case_brf(bgt, _d, 64);
		case_brr(bne, _d);
		case_brf(bne, _d, 64);
		case_brr(bunlt, _d);
		case_brf(bunlt, _d, 64);
		case_brr(bunle, _d);
		case_brf(bunle, _d, 64);
		case_brr(buneq, _d);
		case_brf(buneq, _d, 64);
		case_brr(bunge, _d);
		case_brf(bunge, _d, 64);
		case_brr(bungt, _d);
		case_brf(bungt, _d, 64);
		case_brr(bltgt, _d);
		case_brf(bltgt, _d, 64);
		case_brr(bord, _d);
		case_brf(bord, _d, 64);
		case_brr(bunord, _d);
		case_brf(bunord, _d, 64);
		case_rr(ld, _d);
		case_rw(ld, _d);
		case_rrr(ldx, _d);
		case_rrw(ldx, _d);
		case_rr(st, _d);
		case_wr(st, _d);
		case_rrr(stx, _d);
		case_wrr(stx, _d);
	    case jit_code_jmpr:
		jmpr(rn(node->u.w));
		break;
	    case jit_code_jmpi:
		if (node->flag & jit_flag_node) {
#if __powerpc__ && !ABI_ELFv2
		    if (_jit->pc.uc == _jit->code.ptr + sizeof(void*) * 3)
			_jitc->jump = 1;
#endif
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    if (temp->flag & jit_flag_patch)
			jmpi(temp->u.w);
		    else {
			word = jmpi(_jit->pc.w);
			patch(word, node);
		    }
		}
		else
		    (void)jmpi_p(node->u.w);
		break;
	    case jit_code_callr:
		callr(rn(node->u.w));
		break;
	    case jit_code_calli:
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    word = calli_p(temp->u.w);
		    if (!(temp->flag & jit_flag_patch))
			patch(word, node);
		}
		else
		    calli(node->u.w);
		break;
	    case jit_code_prolog:
		_jitc->function = _jitc->functions.ptr + node->w.w;
		undo.node = node;
		undo.word = _jit->pc.w;
		undo.patch_offset = _jitc->patches.offset;
#if __powerpc__ && !ABI_ELFv2
		undo.prolog_offset = _jitc->prolog.offset;
#endif
	    restart_function:
		_jitc->again = 0;
#if __powerpc__ && !ABI_ELFv2
		if (_jitc->jump && !_jitc->function->assume_frame) {
		    /* remember prolog to hide offset adjustment for a jump
		     * to the start of a function, what is expected to be
		     * a common practice as first jit instruction */
		    if (_jitc->prolog.offset >= _jitc->prolog.length) {
			_jitc->prolog.length += 16;
			jit_realloc((jit_pointer_t *)&_jitc->prolog.ptr,
				    (_jitc->prolog.length - 16) *
				    sizeof(jit_word_t),
				    _jitc->prolog.length * sizeof(jit_word_t));
		    }
		    _jitc->prolog.ptr[_jitc->prolog.offset++] = _jit->pc.w;
		    /* function descriptor */
		    word = _jit->pc.w + sizeof(void*) * 3;
		    iw(word);				/* addr */
		    iw(0);				/* toc */
		    iw(0);				/* env */
		}
#endif
		prolog(node);
		break;
	    case jit_code_epilog:
		assert(_jitc->function == _jitc->functions.ptr + node->w.w);
		if (_jitc->again) {
		    for (temp = undo.node->next;
			 temp != node; temp = temp->next) {
			if (temp->code == jit_code_label ||
			    temp->code == jit_code_epilog)
			    temp->flag &= ~jit_flag_patch;
		    }
		    temp->flag &= ~jit_flag_patch;
		    node = undo.node;
		    _jit->pc.w = undo.word;
		    _jitc->patches.offset = undo.patch_offset;
#if __powerpc__ && !ABI_ELFv2
		    _jitc->prolog.offset = undo.prolog_offset;
#endif
		    goto restart_function;
		}
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		epilog(node);
		_jitc->function = NULL;
		break;
	    case jit_code_live:
	    case jit_code_arg:
	    case jit_code_arg_f:		case jit_code_arg_d:
		break;
	    default:
		abort();
	}
	jit_regarg_clr(node, value);
	assert(_jitc->regarg == 0);
	/* update register live state */
	jit_reglive(node);
    }
#undef case_brf
#undef case_brw
#undef case_brr
#undef case_wrr
#undef case_rrf
#undef case_rrw
#undef case_rrr
#undef case_wr
#undef case_rw
#undef case_rr

    for (offset = 0; offset < _jitc->patches.offset; offset++) {
	node = _jitc->patches.ptr[offset].node;
	word = node->code == jit_code_movi ? node->v.n->u.w : node->u.n->u.w;
	patch_at(_jitc->patches.ptr[offset].inst, word);
    }

    jit_flush(_jit->code.ptr, _jit->pc.uc);

    return (_jit->code.ptr);
}

#define CODE				1
#  include "jit_ppc-cpu.c"
#  include "jit_ppc-fpu.c"
#undef CODE

void
jit_flush(void *fptr, void *tptr)
{
#if defined(__GNUC__)
    jit_word_t		f, t, s;

    s = sysconf(_SC_PAGE_SIZE);
    f = (jit_word_t)fptr & -s;
    t = (((jit_word_t)tptr) + s - 1) & -s;
    __clear_cache((void *)f, (void *)t);
#endif
}

void
_emit_ldxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
#if __WORDSIZE == 32
    ldxi_i(rn(r0), rn(r1), i0);
#else
    ldxi_l(rn(r0), rn(r1), i0);
#endif
}

void
_emit_stxi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
#if __WORDSIZE == 32
    stxi_i(i0, rn(r0), rn(r1));
#else
    stxi_l(i0, rn(r0), rn(r1));
#endif
}

void
_emit_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    ldxi_d(rn(r0), rn(r1), i0);
}

void
_emit_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    stxi_d(i0, rn(r0), rn(r1));
}

static void
_patch(jit_state_t *_jit, jit_word_t instr, jit_node_t *node)
{
    jit_int32_t		 flag;

    assert(node->flag & jit_flag_node);
    if (node->code == jit_code_movi)
	flag = node->v.n->flag;
    else
	flag = node->u.n->flag;
    assert(!(flag & jit_flag_patch));
    if (_jitc->patches.offset >= _jitc->patches.length) {
	jit_realloc((jit_pointer_t *)&_jitc->patches.ptr,
		    _jitc->patches.length * sizeof(jit_patch_t),
		    (_jitc->patches.length + 1024) * sizeof(jit_patch_t));
	_jitc->patches.length += 1024;
    }
    _jitc->patches.ptr[_jitc->patches.offset].inst = instr;
    _jitc->patches.ptr[_jitc->patches.offset].node = node;
    ++_jitc->patches.offset;
}
