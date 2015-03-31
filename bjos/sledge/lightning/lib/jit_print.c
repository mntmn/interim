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

#include <lightning.h>
#include <lightning/jit_private.h>

#define print_chr(value)		fputc(value, stdout)
#define print_hex(value)		fprintf(stdout, "0x%lx", value)
#define print_dec(value)		fprintf(stdout, "%ld", value)
#define print_flt(value)		fprintf(stdout, "%g", value)
#define print_str(value)		fprintf(stdout, "%s", value)
#define print_ptr(value)		fprintf(stdout, "%p", value)
#define print_reg(value)						\
    do {								\
	if ((value) & jit_regno_patch)					\
	    print_chr('?');						\
	print_str(_rvs[jit_regno(value)].name);				\
    } while (0)

/*
 * Initialization
 */
#include "jit_names.c"

/*
 * Implementation
 */
void
_jit_print(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_block_t		*block;
    jit_bool_t		 first;
    jit_int32_t		 value;
    jit_int32_t		 offset;

    first = 0;
    for (node = _jitc->head; node; node = node->next) {
	if (!first)
	    print_chr('\n');
	else
	    first = 0;
	if (node->code == jit_code_label ||
	    node->code == jit_code_prolog || node->code == jit_code_epilog) {
	    print_chr('L');
	    print_dec(node->v.w);
	    print_chr(':');
	    block = _jitc->blocks.ptr + node->v.w;
	    for (offset = 0; offset < _jitc->reglen; offset++) {
		if (jit_regset_tstbit(&block->reglive, offset)) {
		    print_chr(' ');
		    print_reg(offset);
		}
	    }
	    if (node->code == jit_code_prolog ||
		node->code == jit_code_epilog) {
		print_str(" /* ");
		print_str(code_name[node->code]);
		print_str(" */");
	    }
	    continue;
	}
	value = jit_classify(node->code) &
	    (jit_cc_a0_int|jit_cc_a0_jmp|jit_cc_a0_reg|jit_cc_a0_rlh|
	     jit_cc_a1_reg|jit_cc_a1_int|jit_cc_a1_flt|jit_cc_a1_dbl|
	     jit_cc_a2_reg|jit_cc_a2_int|jit_cc_a2_flt|jit_cc_a2_dbl);
	if (value & jit_cc_a0_jmp)
	    print_str("    ");
	else
	    print_chr('\t');
	print_str(code_name[node->code]);
	switch (node->code) {
	r:
	    print_chr(' ');		print_reg(node->u.w);	continue;
	w:
	    print_chr(' ');		print_hex(node->u.w);	continue;
	n:
	    print_chr(' ');
	    if (!(node->flag & jit_flag_node))
		print_ptr(node->u.p);
	    else {
		print_chr('L');
		print_dec(node->u.n->v.w);
	    }
	    continue;
	r_r:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');		print_reg(node->v.w);	continue;
	r_w:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');		print_hex(node->v.w);	continue;
	r_f:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');
	    if (node->flag & jit_flag_data)
		print_flt(*(jit_float32_t *)node->v.n->u.w);
	    else
		print_flt(node->v.f);
	    continue;
	r_d:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');
	    if (node->flag & jit_flag_data)
		print_flt(*(jit_float64_t *)node->v.n->u.w);
	    else
		print_flt(node->v.d);
	    continue;
	w_r:
	    print_chr(' ');		print_hex(node->u.w);
	    print_chr(' ');		print_reg(node->v.w);	continue;
	r_r_r:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');		print_reg(node->w.w);	continue;
	r_r_w:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');		print_hex(node->w.w);	continue;
	q_r_r:
	    print_str(" (");		print_reg(node->u.q.l);
	    print_chr(' ');		print_reg(node->u.q.h);
	    print_str(") ");		print_reg(node->v.w);
	    print_chr(' ');		print_reg(node->w.w);	continue;
	q_r_w:
	    print_str(" (");		print_reg(node->u.q.l);
	    print_chr(' ');		print_reg(node->u.q.h);
	    print_str(") ");		print_reg(node->v.w);
	    print_chr(' ');		print_hex(node->w.w);	continue;
	r_r_f:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');
	    if (node->flag & jit_flag_data)
		print_flt(*(jit_float32_t *)node->w.n->u.w);
	    else
		print_flt(node->w.f);
	    continue;
	r_r_d:
	    print_chr(' ');		print_reg(node->u.w);
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');
	    if (node->flag & jit_flag_data)
		print_flt(*(jit_float64_t *)node->w.n->u.w);
	    else
		print_flt(node->w.d);
	    continue;
	w_r_r:
	    print_chr(' ');		print_hex(node->u.w);
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');		print_reg(node->w.w);	continue;
	n_r_r:
	    print_chr(' ');
	    if (!(node->flag & jit_flag_node))
		print_ptr(node->u.p);
	    else {
		print_chr('L');
		print_dec(node->u.n->v.w);
	    }
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');		print_reg(node->w.w);	continue;
	n_r_w:
	    print_chr(' ');
	    if (!(node->flag & jit_flag_node))
		print_ptr(node->u.p);
	    else {
		print_chr('L');
		print_dec(node->u.n->v.w);
	    }
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');		print_hex(node->w.w);	continue;
	n_r_f:
	    print_chr(' ');
	    if (!(node->flag & jit_flag_node))
		print_ptr(node->u.p);
	    else{
		print_chr('L');
		print_dec(node->u.n->v.w);
	    }
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');
	    if (node->flag & jit_flag_data)
		print_flt(*(jit_float32_t *)node->w.n->u.w);
	    else
		print_flt(node->w.f);
	    continue;
	n_r_d:
	    print_chr(' ');
	    if (!(node->flag & jit_flag_node))
		print_ptr(node->u.p);
	    else {
		print_chr('L');
		print_dec(node->u.n->v.w);
	    }
	    print_chr(' ');		print_reg(node->v.w);
	    print_chr(' ');
	    if (node->flag & jit_flag_data)
		print_flt(*(jit_float64_t *)node->w.n->u.w);
	    else
		print_flt(node->w.d);
	    continue;
	    case jit_code_name:
		print_chr(' ');
		if (node->v.p && _jitc->emit)
		    print_str(node->v.n->u.p);
		break;
	    case jit_code_note:
		print_chr(' ');
		if (node->v.p && _jitc->emit)
		    print_str(node->v.n->u.p);
		if (node->v.p && _jitc->emit && node->w.w)
		    print_chr(':');
		if (node->w.w)
		    print_dec(node->w.w);
		break;
	    case jit_code_data:
	    case jit_code_label:
	    case jit_code_prolog:	case jit_code_epilog:
		break;
	    case jit_code_save:		case jit_code_load:
		goto r;
	    default:
		switch (value) {
		    case jit_cc_a0_reg:
		    case jit_cc_a0_reg|jit_cc_a0_chg:
		    case jit_cc_a0_reg|jit_cc_a0_jmp:
			goto r;
		    case jit_cc_a0_int:
			goto w;
		    case jit_cc_a0_jmp:
			goto n;
		    case jit_cc_a0_reg|jit_cc_a1_reg:
			goto r_r;
		    case jit_cc_a0_reg|jit_cc_a1_int:
			goto r_w;
		    case jit_cc_a0_reg|jit_cc_a1_flt:
			goto r_f;
		    case jit_cc_a0_reg|jit_cc_a1_dbl:
			goto r_d;
		    case jit_cc_a0_int|jit_cc_a1_reg:
			goto w_r;
		    case jit_cc_a0_reg|jit_cc_a1_reg|jit_cc_a2_reg:
			goto r_r_r;
		    case jit_cc_a0_reg|jit_cc_a1_reg|jit_cc_a2_int:
			goto r_r_w;
		    case jit_cc_a0_reg|jit_cc_a0_rlh|
			 jit_cc_a1_reg|jit_cc_a2_reg:
			goto q_r_r;
		    case jit_cc_a0_reg|jit_cc_a0_rlh|
			 jit_cc_a1_reg|jit_cc_a2_int:
			goto q_r_w;
		    case jit_cc_a0_reg|jit_cc_a1_reg|jit_cc_a2_flt:
			goto r_r_f;
		    case jit_cc_a0_reg|jit_cc_a1_reg|jit_cc_a2_dbl:
			goto r_r_d;
		    case jit_cc_a0_int|jit_cc_a1_reg|jit_cc_a2_reg:
			goto w_r_r;
		    case jit_cc_a0_jmp|jit_cc_a1_reg|jit_cc_a2_reg:
			goto n_r_r;
		    case jit_cc_a0_jmp|jit_cc_a1_reg|jit_cc_a2_int:
			goto n_r_w;
		    case jit_cc_a0_jmp|jit_cc_a1_reg|jit_cc_a2_flt:
			goto n_r_f;
		    case jit_cc_a0_jmp|jit_cc_a1_reg|jit_cc_a2_dbl:
			goto n_r_d;
		    default:
			abort();
		}
		break;
	}
    }
    print_chr('\n');
}
