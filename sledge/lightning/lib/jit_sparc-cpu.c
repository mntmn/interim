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

#if PROTO
#  define _SP_REGNO				0x0e
#  define _FP_REGNO				0x1e
#  define _O7_REGNO				0x0f
#  define _L0_REGNO				0x10
#  define _L1_REGNO				0x11
#  define _L2_REGNO				0x12
#  define _L3_REGNO				0x13
#  define _L4_REGNO				0x14
#  define _L5_REGNO				0x15
#  define _L6_REGNO				0x16
#  define _L7_REGNO				0x17
#  define _I7_REGNO				0x1f
/*
 *					- previous stack frame
 * fp	----
 * fp-	local variables (in lightning, 8 bytes space for float conversion)
 * fp-	alloca
 * sp+	stack arguments
 * sp+	6 words to save register arguments
 * sp+	1 word for hidden address of aggregate return value
 * sp+	16 words for in and local registers
 * sp	----
 *	decreasing memory address	- next stack frame (not yet allocated)
 */
#  define stack_framesize			((16 + 1 + 6) * 4)
typedef union {
    struct {				jit_uint32_t b: 2;	} op;
    struct {	jit_uint32_t _: 2;	jit_uint32_t b: 1;	} a;
    struct {	jit_uint32_t _: 2;	jit_uint32_t b: 5;	} rd;
    struct {	jit_uint32_t _: 2;	jit_uint32_t b: 30;	} disp30;
    struct {	jit_uint32_t _: 3;	jit_uint32_t b: 4;	} cond;
    struct {	jit_uint32_t _: 7;	jit_uint32_t b: 3;	} op2;
    struct {	jit_uint32_t _: 7;	jit_uint32_t b: 6;	} op3;
    struct {	jit_uint32_t _: 10;	jit_uint32_t b: 22;	} imm22;
    struct {	jit_uint32_t _: 10;	jit_uint32_t b: 22;	} disp22;
    struct {	jit_uint32_t _: 13;	jit_uint32_t b: 5;	} rs1;
    struct {	jit_uint32_t _: 18;	jit_uint32_t b: 1;	} i;
    struct {	jit_uint32_t _: 18;	jit_uint32_t b: 9;	} opf;
    struct {	jit_uint32_t _: 19;	jit_uint32_t b: 8;	} asi;
    struct {	jit_uint32_t _: 19;	jit_uint32_t b: 6;	} res;
    struct {	jit_uint32_t _: 19;	jit_uint32_t b: 13;	} simm13;
    struct {	jit_uint32_t _: 27;	jit_uint32_t b: 5;	} rs2;
    struct {	jit_uint32_t _: 25;	jit_uint32_t b: 7;	} imm7;
    jit_int32_t							  v;
} jit_instr_t;
#  define ii(i)				*_jit->pc.ui++ = i
#  define s7_p(imm)			((imm) <= 63 && (imm) >= -64)
#  define s13_p(imm)			((imm) <= 4095 && (imm) >= -4096)
#  define s22_p(imm)			((imm) <= 2097151 && (imm) >= -20971512)
#  define s30_p(imm)			((imm) <= 536870911 && (imm) >= -536870912)
#  define f1(op, disp30)		_f1(_jit, op, disp30)
static void _f1(jit_state_t*,jit_int32_t,jit_int32_t);
#  define f2r(op, rd, op2, imm22)	_f2r(_jit, op, rd, op2, imm22)
static void _f2r(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define f2b(op, a, cond, op2, disp22)	_f2b(_jit, op, a, cond, op2, disp22)
static void
_f2b(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define f3r(op, rd, op3, rs1, rs2)	_f3r(_jit, op, rd, op3, rs1, rs2)
static void _f3r(jit_state_t*,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define f3i(op, rd, op3, rs1, simm13)	_f3i(_jit, op, rd, op3, rs1, simm13)
static void _f3i(jit_state_t*,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define f3t(cond, rs1, i, ri)		_f3t(_jit, cond, rs1, i, ri)
static void _f3t(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t)
    maybe_unused;
#  define f3a(op, rd, op3, rs1, rs2)	_f3a(_jit, op, rd, op3, rs1, asi, rs2)
static void _f3a(jit_state_t*,jit_int32_t,
		 jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t)
    maybe_unused;
#  define LDSB(rs1, rs2, rd)		f3r(3, rd, 9, rs1, rs2)
#  define LDSBI(rs1, imm, rd)		f3i(3, rd, 9, rs1, imm)
#  define LDSH(rs1, rs2, rd)		f3r(3, rd, 10, rs1, rs2)
#  define LDSHI(rs1, imm, rd)		f3i(3, rd, 10, rs1, imm)
#  define LDUB(rs1, rs2, rd)		f3r(3, rd, 1, rs1, rs2)
#  define LDUBI(rs1, imm, rd)		f3i(3, rd, 1, rs1, imm)
#  define LDUH(rs1, rs2, rd)		f3r(3, rd, 2, rs1, rs2)
#  define LDUHI(rs1, imm, rd)		f3i(3, rd, 2, rs1, imm)
#  define LD(rs1, rs2, rd)		f3r(3, rd, 0, rs1, rs2)
#  define LDI(rs1, imm, rd)		f3i(3, rd, 0, rs1, imm)
#  define LDD(rs1, rs2, rd)		f3r(3, rd, 3, rs1, rs2)
#  define LDDI(rs1, imm, rd)		f3i(3, rd, 3, rs1, imm)
#  define LDSBA(rs1, rs2, asi, rd)	f3a(3, rd, 25, rs1, asi, rs2)
#  define LDSHA(rs1, rs2, asi, rd)	f3a(3, rd, 26, rs1, asi, rs2)
#  define LDUBA(rs1, rs2, asi, rd)	f3a(3, rd, 17, rs1, asi, rs2)
#  define LDUHA(rs1, rs2, asi, rd)	f3a(3, rd, 18, rs1, asi, rs2)
#  define LDA(rs1, rs2, asi, rd)	f3a(3, rd, 16, rs1, asi, rs2)
#  define LDDA(rs1, rs2, asi, rd)	f3a(3, rd, 19, rs1, asi, rs2)
#  define LDC(rs1, rs2, rd)		f3r(3, rd, 48, rs1, rs2)
#  define LDCI(rs1, imm, rd)		f3i(3, rd, 48, rs1, imm)
#  define LDDC(rs1, rs2, rd)		f3r(3, rd, 51, rs1, rs2)
#  define LDDCI(rs1, imm, rd)		f3i(3, rd, 51, rs1, imm)
#  define LDCSR(rs1, rs2, rd)		f3r(3, rd, 49, rs1, rs2)
#  define LDCSRI(rs1, imm, rd)		f3i(3, rd, 49, rs1, imm)
#  define STB(rd, rs1, rs2)		f3r(3, rd, 5, rs1, rs2)
#  define STBI(rd, rs1, imm)		f3i(3, rd, 5, rs1, imm)
#  define STH(rd, rs1, rs2)		f3r(3, rd, 6, rs1, rs2)
#  define STHI(rd, rs1, imm)		f3i(3, rd, 6, rs1, imm)
#  define ST(rd, rs1, rs2)		f3r(3, rd, 4, rs1, rs2)
#  define STI(rd, rs1, imm)		f3i(3, rd, 4, rs1, imm)
#  define STD(rrd, s1, rs2)		f3r(3, rd, 7, rs1, rs2)
#  define STDI(rd, rs1, imm)		f3i(3, rd, 7, rs1, imm)
#  define STBA(rd, rs1, rs2)		f3a(3, rd, 21, rs1, asi, rs2)
#  define STHA(rd, rs1, rs2)		f3a(3, rd, 22, rs1, asi, rs2)
#  define STA(rd, rs1, rs2)		f3a(3, rd, 20, rs1, asi, rs2)
#  define STDA(rd, rs1, rs2)		f3a(3, rd, 23, rs1, asi, rs2)
#  define STC(rd, rs1, rs2)		f3r(3, rd, 52, rs1, rs2)
#  define STCI(rd, rs1, imm)		f3i(3, rd, 52, rs1, imm)
#  define STDC(rd, rs1, rs2)		f3r(3, rd, 55, rs1, rs2)
#  define STDCI(rd, rs1, imm)		f3i(3, rd, 55, rs1, imm)
#  define STCSR(rd, rs1, rs2)		f3r(3, rd, 53, rs1, rs2)
#  define STCSRI(rd, rs1, imm)		f3i(3, rd, 53, rs1, imm)
#  define STDCQ(rd, rs1, rs2)		f3r(3, rd, 54, rs1, rs2)
#  define STDCQI(rd, rs1, imm)		f3i(3, rd, 54, rs1, imm)
#  define LDSTUB(rs1, rs2, rd)		f3r(3, rd, 13, rs1, rs2)
#  define LDSTUBI(rs1, imm, rd)		f3r(3, rd, 13, rs1, imm)
#  define LDSTUBA(rs1, rs2, asi, rd)	f3a(3, rd, 21, rs1, asi, rs2)
#  define SWAP(rs1, rs2, rd)		f3r(3, rd, 15, rs1, rs2)
#  define SWAPI(rs1, imm, rd)		f3r(3, rd, 15, rs1, imm)
#  define SWAPA(rs1, rs2, asi, rd)	f3a(3, rd, 23, rs1, asi, rs2)
#  define NOP()				SETHI(0, 0)
#  define HI(im)			((im) >> 10)
#  define LO(im)			((im) & 0x3ff)
#  define SETHI(im, rd)			f2r(0, rd, 4, im)
#  define AND(rs1, rs2, rd)		f3r(2, rd, 1, rs1, rs2)
#  define ANDI(rs1, imm, rd)		f3i(2, rd, 1, rs1, imm)
#  define ANDcc(rs1, rs2, rd)		f3r(2, rd, 17, rs1, rs2)
#  define ANDIcc(rs1, imm, rd)		f3i(2, rd, 17, rs1, imm)
#  define BTST(rs1, rs2)		ANDcc(rs1, rs2, 0)
#  define BTSTI(rs1, imm)		ANDIcc(rs1, imm, 0)
#  define ANDN(rs1, rs2, rd)		f3r(2, rd, 5, rs1, rs2)
#  define ANDNI(rs1, imm, rd)		f3i(2, rd, 5, rs1, imm)
#  define ANDNcc(rs1, rs2, rd)		f3r(2, rd, 21, rs1, rs2)
#  define ANDNIcc(rs1, imm, rd)		f3i(2, rd, 21, rs1, imm)
#  define OR(rs1, rs2, rd)		f3r(2, rd, 2, rs1, rs2)
#  define ORI(rs1, imm, rd)		f3i(2, rd, 2, rs1, imm)
#  define ORcc(rs1, rs2, rd)		f3r(2, rd, 18, rs1, rs2)
#  define ORIcc(rs1, imm, rd)		f3i(2, rd, 18, rs1, imm)
#  define ORN(rs1, rs2, rd)		f3r(2, rd, 6, rs1, rs2)
#  define ORNI(rs1, imm, rd)		f3i(2, rd, 6, rs1, imm)
#  define ORNcc(rs1, rs2, rd)		f3r(2, rd, 22, rs1, rs2)
#  define ORNIcc(rs1, imm, rd)		f3i(2, rd, 22, rs1, imm)
#  define XOR(rs1, rs2, rd)		f3r(2, rd, 3, rs1, rs2)
#  define XORI(rs1, imm, rd)		f3i(2, rd, 3, rs1, imm)
#  define XORcc(rs1, rs2, rd)		f3r(2, rd, 19, rs1, rs2)
#  define XORIcc(rs1, imm, rd)		f3i(2, rd, 19, rs1, imm)
#  define XNOR(rs1, rs2, rd)		f3r(2, rd, 7, rs1, rs2)
#  define XNORI(rs1, imm, rd)		f3i(2, rd, 7, rs1, imm)
#  define XNORcc(rs1, rs2, rd)		f3r(2, rd, 23, rs1, rs2)
#  define XNORIcc(rs1, imm, rd)		f3i(2, rd, 23, rs1, imm)
#  define SLL(rs1, rs2, rd)		f3r(2, rd, 37, rs1, rs2)
#  define SLLI(rs1, imm, rd)		f3i(2, rd, 37, rs1, imm)
#  define SRL(rs1, rs2, rd)		f3r(2, rd, 38, rs1, rs2)
#  define SRLI(rs1, imm, rd)		f3i(2, rd, 38, rs1, imm)
#  define SRA(rs1, rs2, rd)		f3r(2, rd, 39, rs1, rs2)
#  define SRAI(rs1, imm, rd)		f3i(2, rd, 39, rs1, imm)
#  define ADD(rs1, rs2, rd)		f3r(2, rd, 0, rs1, rs2)
#  define ADDI(rs1, imm, rd)		f3i(2, rd, 0, rs1, imm)
#  define ADDcc(rs1, rs2, rd)		f3r(2, rd, 16, rs1, rs2)
#  define ADDIcc(rs1, imm, rd)		f3i(2, rd, 16, rs1, imm)
#  define ADDX(rs1, rs2, rd)		f3r(2, rd, 8, rs1, rs2)
#  define ADDXI(rs1, imm, rd)		f3i(2, rd, 8, rs1, imm)
#  define ADDXcc(rs1, rs2, rd)		f3r(2, rd, 24, rs1, rs2)
#  define ADDXIcc(rs1, imm, rd)		f3i(2, rd, 24, rs1, imm)
#  define TADDcc(rs1, rs2, rd)		f3r(2, rd, 32, rs1, rs2)
#  define TADDIcc(rs1, imm, rd)		f3i(2, rd, 32, rs1, imm)
#  define TADDccTV(rs1, rs2, rd)	f3r(2, rd, 34, rs1, rs2)
#  define TADDIccTV(rs1, imm, rd)	f3i(2, rd, 34, rs1, imm)
#  define SUB(rs1, rs2, rd)		f3r(2, rd, 4, rs1, rs2)
#  define NEG(rs1, rd)			SUB(0, rs1, rd)
#  define SUBI(rs1, imm, rd)		f3i(2, rd, 4, rs1, imm)
#  define SUBcc(rs1, rs2, rd)		f3r(2, rd, 20, rs1, rs2)
#  define SUBIcc(rs1, imm, rd)		f3i(2, rd, 20, rs1, imm)
#  define CMP(rs1, rs2)			SUBcc(rs1, rs2, 0)
#  define CMPI(rs1, imm)		SUBIcc(rs1, imm, 0)
#  define SUBX(rs1, rs2, rd)		f3r(2, rd, 12, rs1, rs2)
#  define SUBXI(rs1, imm, rd)		f3i(2, rd, 12, rs1, imm)
#  define SUBXcc(rs1, rs2, rd)		f3r(2, rd, 28, rs1, rs2)
#  define SUBXIcc(rs1, imm, rd)		f3i(2, rd, 28, rs1, imm)
#  define TSUBcc(rs1, rs2, rd)		f3r(2, rd, 33, rs1, rs2)
#  define TDADDIcc(rs1, imm, rd)	f3i(2, rd, 33, rs1, imm)
#  define TSUBccTV(rs1, rs2, rd)	f3r(2, rd, 35, rs1, rs2)
#  define TSUBIccTV(rs1, imm, rd)	f3i(2, rd, 35, rs1, imm)
#  define MULScc(rs1, rs2, rd)		f3r(2, rd, 36, rs1, rs2)
#  define MULSIcc(rs1, imm, rd)		f3i(2, rd, 36, rs1, imm)
#  define UMUL(rs1, rs2, rd)		f3r(2, rd, 10, rs1, rs2)
#  define UMULI(rs1, imm, rd)		f3i(2, rd, 10, rs1, imm)
#  define SMUL(rs1, rs2, rd)		f3r(2, rd, 11, rs1, rs2)
#  define SMULI(rs1, imm, rd)		f3i(2, rd, 11, rs1, imm)
#  define UMULcc(rs1, rs2, rd)		f3r(2, rd, 26, rs1, rs2)
#  define UMULIcc(rs1, imm, rd)		f3i(2, rd, 26, rs1, imm)
#  define SMULcc(rs1, rs2, rd)		f3r(2, rd, 27, rs1, rs2)
#  define SMULIcc(rs1, imm, rd)		f3i(2, rd, 27, rs1, imm)
#  define UDIV(rs1, rs2, rd)		f3r(2, rd, 14, rs1, rs2)
#  define UDIVI(rs1, imm, rd)		f3i(2, rd, 14, rs1, imm)
#  define SDIV(rs1, rs2, rd)		f3r(2, rd, 15, rs1, rs2)
#  define SDIVI(rs1, imm, rd)		f3i(2, rd, 15, rs1, imm)
#  define UDIVcc(rs1, rs2, rd)		f3r(2, rd, 30, rs1, rs2)
#  define UDIVIcc(rs1, imm, rd)		f3i(2, rd, 30, rs1, imm)
#  define SDIVcc(rs1, rs2, rd)		f3r(2, rd, 31, rs1, rs2)
#  define SDIVIcc(rs1, imm, rd)		f3i(2, rd, 31, rs1, imm)
#  define SAVE(rs1, rs2, rd)		f3r(2, rd, 60, rs1, rs2)
#  define SAVEI(rs1, imm, rd)		f3i(2, rd, 60, rs1, imm)
#  define RESTORE(rs1, rs2, rd)		f3r(2, rd, 61, rs1, rs2)
#  define RESTOREI(rs1, imm, rd)	f3i(2, rd, 61, rs1, imm)
#  define SPARC_BA			8	/* always */
#  define SPARC_BN			0	/* never */
#  define SPARC_BNE			9	/* not equal - not Z */
#  define SPARC_BNZ			SPARC_BNE
#  define SPARC_BE			1	/* equal - Z */
#  define SPARC_BZ			SPARC_BE
#  define SPARC_BG			10	/* greater - not (Z or (N xor V)) */
#  define SPARC_BLE			2	/* less or equal - Z or (N xor V) */
#  define SPARC_BGE			11	/* greater or equal - not (N xor V) */
#  define SPARC_BL			3	/* less - N xor V */
#  define SPARC_BGU			12	/* greater unsigned - not (C or Z) */
#  define SPARC_BLEU			4	/* less or equal unsigned - C or Z */
#  define SPARC_BCC			13	/* carry clear - not C */
#  define SPARC_BGEU			SPARC_BCC
#  define SPARC_BCS			5	/* carry set - C */
#  define SPARC_BLU			SPARC_BCS
#  define SPARC_BPOS			14	/* positive - not N */
#  define SPARC_BNEG			6	/* negative - N */
#  define SPARC_BVC			15	/* overflow clear - not V */
#  define SPARC_BVS			7	/* overflow set - V */
#  define B(cc, imm)			f2b(0, 0, cc, 2, imm)
#  define Ba(cc, imm)			f2b(0, 1, cc, 2, imm)
#  define BA(imm)			B(SPARC_BA, imm)
#  define BAa(imm)			Ba(SPARC_BA, imm)
#  define BN(imm)			B(SPARC_BN, imm)
#  define BNa(imm)			Ba(SPARC_BN, imm)
#  define BNE(imm)			B(SPARC_BNE, imm)
#  define BNEa(imm)			Ba(SPARC_BNE, imm)
#  define BNZ(imm)			BNE(imm)
#  define BNZa(imm)			BNEa(imm)
#  define BE(imm)			B(SPARC_BE, imm)
#  define BEa(imm)			Ba(SPARC_BE, imm)
#  define BZ(imm)			BE(imm)
#  define BZa(imm)			BEa(imm)
#  define BG(imm)			B(SPARC_BG, imm)
#  define BGa(imm)			Ba(SPARC_BG, imm)
#  define BLE(imm)			B(SPARC_BLE, imm)
#  define BLEa(imm)			Ba(SPARC_BLE, imm)
#  define BGE(imm)			B(SPARC_BGE, imm)
#  define BGEa(imm)			Ba(SPARC_BGE, imm)
#  define BL(imm)			B(SPARC_BL, imm)
#  define BLa(imm)			Ba(SPARC_BL, imm)
#  define BGU(imm)			B(SPARC_BGU, imm)
#  define BGUa(imm)			Ba(SPARC_BGU, imm)
#  define BLEU(imm)			B(SPARC_BLEU, imm)
#  define BLEUa(imm)			Ba(SPARC_BLEU, imm)
#  define BCC(imm)			B(SPARC_BCC, imm)
#  define BCCa(imm)			Ba(SPARC_BCC, imm)
#  define BGEU(imm)			BCC(imm)
#  define BGEUa(imm)			BCCa(imm)
#  define BCS(imm)			B(SPARC_BCS, imm)
#  define BCSa(imm)			Ba(SPARC_BCS, imm)
#  define BLU(imm)			BCS(imm)
#  define BLUa(imm)			BCSa(imm)
#  define BPOS(imm)			B(SPARC_BPOS, imm)
#  define BPOSa(imm)			Ba(SPARC_BPOS, imm)
#  define BNEG(imm)			B(SPARC_BNEG, imm)
#  define BNEGa(imm)			Ba(SPARC_BNEG, imm)
#  define BVC(imm)			B(SPARC_BVC, imm)
#  define BVCa(imm)			Ba(SPARC_BVC, imm)
#  define BVS(imm)			B(SPARC_BVS, imm)
#  define BVSa(imm)			Ba(SPARC_BVS, imm)
#  define SPARC_CBA			8	/* always */
#  define SPARC_CBN			0	/* never */
#  define SPARC_CB3			7	/* 3 */
#  define SPARC_CB2			6	/* 2 */
#  define SPARC_CB23			5	/* 2 or 3 */
#  define SPARC_CB1			4	/* 1 */
#  define SPARC_CB13			3	/* 1 or 3 */
#  define SPARC_CB12			2	/* 1 or 2 */
#  define SPARC_CB123			1	/* 1 or 2 or 3 */
#  define SPARC_CB0			9	/* 0 */
#  define SPARC_CB03			10	/* 0 or 3 */
#  define SPARC_CB02			11	/* 0 or 2 */
#  define SPARC_CB023			12	/* 0 or 2 or 3 */
#  define SPARC_CB01			13	/* 0 or 1 */
#  define SPARC_CB013			14	/* 0 or 1 or 3 */
#  define SPARC_CB012			15	/* 0 or 1 or 2 */
#  define CB(cc, imm)			f2b(0, 0, cc, 7, imm)
#  define CBa(cc, imm)			f2b(0, 1, cc, 7, imm)
#  define CBA(imm)			CB(SPARC_CBA, imm)
#  define CBAa(imm)			CBa(SPARC_CBA, imm)
#  define CBN(imm)			CB(SPARC_CBN, imm)
#  define CBNa(imm)			CBa(SPARC_CBN, imm)
#  define CB3(imm)			CB(SPARC_CB3, imm)
#  define CB3a(imm)			CBa(SPARC_CB3, imm)
#  define CB2(imm)			CB(SPARC_CB2, imm)
#  define CB2a(imm)			CBa(SPARC_CB2, imm)
#  define CB23(imm)			CB(SPARC_CB23, imm)
#  define CB23a(imm)			CBa(SPARC_CB23, imm)
#  define CB1(imm)			CB(SPARC_CB1, imm)
#  define CB1a(imm)			CBa(SPARC_CB1, imm)
#  define CB13(imm)			CB(SPARC_CB13, imm)
#  define CB13a(imm)			CBa(SPARC_CB13, imm)
#  define CB12(imm)			CB(SPARC_CB12, imm)
#  define CB12a(imm)			CBa(SPARC_CB12, imm)
#  define CB123(imm)			CB(SPARC_CB123, imm)
#  define CB123a(imm)			CBa(SPARC_CB123, imm)
#  define CB0(imm)			CB(SPARC_CB0, imm)
#  define CB0a(imm)			CBa(SPARC_CB0, imm)
#  define CB03(imm)			CB(SPARC_CB03, imm)
#  define CB03a(imm)			CBa(SPARC_CB03, imm)
#  define CB02(imm)			CB(SPARC_CB02, imm)
#  define CB02a(imm)			CBa(SPARC_CB02, imm)
#  define CB023(imm)			CB(SPARC_CB103, imm)
#  define CB023a(imm)			CBa(SPARC_CB023, imm)
#  define CB01(imm)			CB(SPARC_CB01, imm)
#  define CB01a(imm)			CBa(SPARC_CB01, imm)
#  define CB013(imm)			CB(SPARC_CB013, imm)
#  define CB013a(imm)			CBa(SPARC_CB013, imm)
#  define CB012(imm)			CB(SPARC_CB012, imm)
#  define CB012a(imm)			CBa(SPARC_CB012, imm)
#  define CALLI(imm)			f1(1, imm)
#  define CALL(r0)			JMPL(_O7_REGNO, r0, 0)
#  define RETL()			JMPLI(0, _O7_REGNO, 8)
#  define RET()				JMPLI(0, _I7_REGNO, 8)
#  define JMPL(rd, rs1, rs2)		f3r(2, rd, 56, rs1, rs2)
#  define JMPLI(rd, rs1, imm)		f3i(2, rd, 56, rs1, imm)
#  define RETT(rs1, rs2)		f3r(2, 0, 57, rs1, rs2)
#  define RETTI(rs1, imm)		f3i(2, 0, 57, rs1, imm)
#  define SPARC_TA			8	/* always */
#  define SPARC_TN			0	/* never */
#  define SPARC_TNE			9	/* not equal - not Z */
#  define SPARC_TNZ			SPARC_BNE
#  define SPARC_TE			1	/* equal - Z */
#  define SPARC_TZ			SPARC_BE
#  define SPARC_TG			10	/* greater - not (Z or (N xor V)) */
#  define SPARC_TLE			2	/* less or equal - Z or (N xor V) */
#  define SPARC_TGE			11	/* greater or equal - not (N xor V) */
#  define SPARC_TL			3	/* less - N xor V */
#  define SPARC_TGU			12	/* greater unsigned - not (C or Z) */
#  define SPARC_TLEU			4	/* less or equal unsigned - C or Z */
#  define SPARC_TCC			13	/* carry clear - not C */
#  define SPARC_TGEU			SPARC_BCC
#  define SPARC_TCS			5	/* carry set - C */
#  define SPARC_TLU			SPARC_BCS
#  define SPARC_TPOS			14	/* positive - not N */
#  define SPARC_TNEG			6	/* negative - N */
#  define SPARC_TVC			15	/* overflow clear - not V */
#  define SPARC_TVS			7	/* overflow set - V */
#  define T(cc, rs1, rs2)		f3t(cc, rs1, 0, rs2)
#  define TI(cc, rs1, imm)		f3t(cc, rs1, 1, imm)
#  define TA(rs1, rs2)			T(SPARC_TA, rs1, rs2)
#  define TAI(rs1, imm)			TI(SPARC_TA, rs1, imm)
#  define TN(rs1, rs2)			T(SPARC_TN, rs1, rs2)
#  define TNI(rs1, imm)			TI(SPARC_TN, rs1, imm)
#  define TNE(rs1, rs2)			T(SPARC_TNE, rs1, rs2)
#  define TNEI(rs1, imm)		TI(SPARC_TNE, rs1, imm)
#  define TNZ(rs1, rs2)			TNE(rs1, rs2)
#  define TNZI(rs1, imm)		TNEI(rs1, imm)
#  define TE(rs1, rs2)			T(SPARC_TE, rs1, rs2)
#  define TEI(rs1, imm)			TI(SPARC_TE, rs1, imm)
#  define TZ(rs1, rs2)			TE(rs1, rs2)
#  define TZI(rs1, imm)			TEI(rs1, imm)
#  define TG(rs1, rs2)			T(SPARC_TG, rs1, rs2)
#  define TGI(rs1, imm)			TI(SPARC_TG, rs1, imm)
#  define TLE(rs1, rs2)			T(SPARC_TLE, rs1, rs2)
#  define TLEI(rs1, imm)		TI(SPARC_TLE, rs1, imm)
#  define TGE(rs1, rs2)			T(SPARC_TGE, rs1, rs2)
#  define TGEI(rs1, imm)		TI(SPARC_TGE, rs1, imm)
#  define TL(rs1, rs2)			T(SPARC_TL, rs1, rs2)
#  define TLI(rs1, imm)			TI(SPARC_TL, rs1, imm)
#  define TGU(rs1, rs2)			T(SPARC_TGU, rs1, rs2)
#  define TGUI(rs1, imm)		TI(SPARC_TGU, rs1, imm)
#  define TLEU(rs1, rs2)		T(SPARC_TLEU, rs1, rs2)
#  define TLEUI(rs1, imm)		TI(SPARC_TLEU, rs1, imm)
#  define TCC(rs1, rs2)			T(SPARC_TCC, rs1, rs2)
#  define TCCI(rs1, imm)		TI(SPARC_TCC, rs1, imm)
#  define TGEU(rs1, rs2)		TCC(rs1, rs2)
#  define TGEUI(rs1, imm)		TCCI(rs1, imm)
#  define TCS(rs1, rs2)			T(SPARC_TCC, rs1, rs2)
#  define TCSI(rs1, imm)		TI(SPARC_TCC, rs1, imm)
#  define TLU(rs1, rs2)			TCS(rs1, rs2)
#  define TLUI(rs1, imm)		TCSI(rs1, imm)
#  define TPOS(rs1, rs2)		T(SPARC_TPOS, rs1, rs2)
#  define TPOSI(rs1, imm)		TI(SPARC_TPOS, rs1, imm)
#  define TNEG(rs1, rs2)		T(SPARC_TNEG, rs1, rs2)
#  define TNEGI(rs1, imm)		TI(SPARC_TNEG, rs1, imm)
#  define TVC(rs1, rs2)			T(SPARC_TVC, rs1, rs2)
#  define TVCI(rs1, imm)		TI(SPARC_TVC, rs1, imm)
#  define TVS(rs1, rs2)			T(SPARC_TVS, rs1, rs2)
#  define TVSI(rs1, imm)		TI(SPARC_TVS, rs1, imm)
#  define RDY(rd)			f3r(2, rd, 40, 0, 0)
#  define RDASR(rs1, rd)		f3r(2, rd, 40, rs1, 0)
#  define RDPSR(rd)			f3r(2, rd, 41, 0, 0)
#  define RDWIM(rd)			f3r(2, rd, 42, 0, 0)
#  define RDTBR(rd)			f3r(2, rd, 43, 0, 0)
#  define WRY(rs1, rs2)			f3r(2, 0, 48, rs1, rs2)
#  define WRYI(rs1, imm)		f3i(2, 0, 48, rs1, imm)
#  define WRASR(rs1, rs2, rd)		f3r(2, rd, 48, rs1, rs2)
#  define WRASRI(rs1, imm, rd)		f3i(2, rd, 48, rs1, imm)
#  define WRPSR(rs1, rs2, rd)		f3r(2, rd, 49, rs1, rs2)
#  define WRPSRI(rs1, imm, rd)		f3i(2, rd, 49, rs1, imm)
#  define WRWIM(rs1, rs2, rd)		f3r(2, rd, 50, rs1, rs2)
#  define WRWIMI(rs1, imm, rd)		f3i(2, rd, 50, rs1, imm)
#  define WRTBR(rs1, rs2, rd)		f3r(2, rd, 51, rs1, rs2)
#  define WRTBRI(rs1, imm, rd)		f3i(2, rd, 51, rs1, imm)
#  define STBAR()			f3i(2, 0, 40, 15, 0)
#  define UNIMP(imm)			f2r(0, 0, 0, imm)
#  define FLUSH(rs1, rs2)		f3r(2, 0, 59, rs1, rs2)
#  define FLUSHI(rs1, im)		f3i(2, 0, 59, rs1, imm)
#  define nop(i0)			_nop(_jit, i0)
static void _nop(jit_state_t*, jit_int32_t);
#  define movr(r0, r1)			_movr(_jit, r0, r1)
static void _movr(jit_state_t*, jit_int32_t, jit_int32_t);
#  define movi(r0, i0)			_movi(_jit, r0, i0)
static void _movi(jit_state_t*, jit_int32_t, jit_word_t);
#  define movi_p(r0, i0)		_movi_p(_jit, r0, i0)
static jit_word_t _movi_p(jit_state_t*, jit_int32_t, jit_word_t);
#  define comr(r0, r1)			XNOR(r1, 0, r0)
#  define negr(r0, r1)			NEG(r1, r0)
#  define addr(r0, r1, r2)		ADD(r1, r2, r0)
#  define addi(r0, r1, i0)		_addi(_jit, r0, r1, i0)
static void _addi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define addcr(r0, r1, r2)		ADDcc(r1, r2, r0)
#  define addci(r0, r1, i0)		_addci(_jit, r0, r1, i0)
static void _addci(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define addxr(r0, r1, r2)		ADDXcc(r1, r2, r0)
#  define addxi(r0, r1, i0)		_addxi(_jit, r0, r1, i0)
static void _addxi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define subr(r0, r1, r2)		SUB(r1, r2, r0)
#  define subi(r0, r1, i0)		_subi(_jit, r0, r1, i0)
static void _subi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define subcr(r0, r1, r2)		SUBcc(r1, r2, r0)
#  define subci(r0, r1, i0)		_subci(_jit, r0, r1, i0)
static void _subci(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define subxr(r0, r1, r2)		SUBXcc(r1, r2, r0)
#  define subxi(r0, r1, i0)		_subxi(_jit, r0, r1, i0)
static void _subxi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define mulr(r0, r1, r2)		UMUL(r1, r2, r0)
#  define muli(r0, r1, i0)		_muli(_jit, r0, r1, i0)
static void _muli(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define qmulr(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,1)
#  define qmulr_u(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,0)
#  define iqmulr(r0,r1,r2,r3,cc)	_iqmulr(_jit,r0,r1,r2,r3,cc)
static void _iqmulr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#  define qmuli(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,1)
#  define qmuli_u(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,0)
#  define iqmuli(r0,r1,r2,i0,cc)	_iqmuli(_jit,r0,r1,r2,i0,cc)
static void _iqmuli(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#  define divr(r0, r1, r2)		_divr(_jit, r0, r1, r2)
static void _divr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define divi(r0, r1, i0)		_divi(_jit, r0, r1, i0)
static void _divi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define divr_u(r0, r1, r2)		_divr_u(_jit, r0, r1, r2)
static void _divr_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define divi_u(r0, r1, i0)		_divi_u(_jit, r0, r1, i0)
static void _divi_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define qdivr(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,1)
#  define qdivr_u(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,0)
#  define iqdivr(r0,r1,r2,r3,cc)	_iqdivr(_jit,r0,r1,r2,r3,cc)
static void _iqdivr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#  define qdivi(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,1)
#  define qdivi_u(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,0)
#  define iqdivi(r0,r1,r2,i0,cc)	_iqdivi(_jit,r0,r1,r2,i0,cc)
static void _iqdivi(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#  define remr(r0, r1, r2)		_remr(_jit, r0, r1, r2)
static void _remr(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define remi(r0, r1, i0)		_remi(_jit, r0, r1, i0)
static void _remi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define remr_u(r0, r1, r2)		_remr_u(_jit, r0, r1, r2)
static void _remr_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define remi_u(r0, r1, i0)		_remi_u(_jit, r0, r1, i0)
static void _remi_u(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define andr(r0, r1, r2)		AND(r1, r2, r0)
#  define andi(r0, r1, i0)		_andi(_jit, r0, r1, i0)
static void _andi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define orr(r0, r1, r2)		OR(r1, r2, r0)
#  define ori(r0, r1, i0)		_ori(_jit, r0, r1, i0)
static void _ori(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define xorr(r0, r1, r2)		XOR(r1, r2, r0)
#  define xori(r0, r1, i0)		_xori(_jit, r0, r1, i0)
static void _xori(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);
#  define lshr(r0, r1, r2)		SLL(r1, r2, r0)
#  define lshi(r0, r1, i0)		SLLI(r1, i0, r0)
#  define rshr(r0, r1, r2)		SRA(r1, r2, r0)
#  define rshi(r0, r1, i0)		SRAI(r1, i0, r0)
#  define rshr_u(r0, r1, r2)		SRL(r1, r2, r0)
#  define rshi_u(r0, r1, i0)		SRLI(r1, i0, r0)
#  define htonr_us(r0,r1)		extr_us(r0,r1)
#  define htonr_ui(r0,r1)		movr(r0,r1)
#  define extr_c(r0,r1)			_extr_c(_jit,r0,r1)
static void _extr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_uc(r0,r1)		andi(r0, r1, 0xff)
#  define extr_s(r0,r1)			_extr_s(_jit,r0,r1)
static void _extr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_us(r0,r1)		_extr_us(_jit,r0,r1)
static void _extr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define cr(cc, r0, r1, r2)		_cr(_jit, cc, r0, r1, r2)
static void _cr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define cw(cc, r0, r1, i0)		_cw(_jit, cc, r0, r1, i0)
static void _cw(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define ltr(r0, r1, r2)		cr(SPARC_BL, r0, r1, r2)
#  define lti(r0, r1, i0)		cw(SPARC_BL, r0, r1, i0)
#  define ltr_u(r0, r1, r2)		cr(SPARC_BLU, r0, r1, r2)
#  define lti_u(r0, r1, i0)		cw(SPARC_BLU, r0, r1, i0)
#  define ler(r0, r1, r2)		cr(SPARC_BLE, r0, r1, r2)
#  define lei(r0, r1, i0)		cw(SPARC_BLE, r0, r1, i0)
#  define ler_u(r0, r1, r2)		cr(SPARC_BLEU, r0, r1, r2)
#  define lei_u(r0, r1, i0)		cw(SPARC_BLEU, r0, r1, i0)
#  define eqr(r0, r1, r2)		cr(SPARC_BE, r0, r1, r2)
#  define eqi(r0, r1, i0)		cw(SPARC_BE, r0, r1, i0)
#  define ger(r0, r1, r2)		cr(SPARC_BGE, r0, r1, r2)
#  define gei(r0, r1, i0)		cw(SPARC_BGE, r0, r1, i0)
#  define ger_u(r0, r1, r2)		cr(SPARC_BGEU, r0, r1, r2)
#  define gei_u(r0, r1, i0)		cw(SPARC_BGEU, r0, r1, i0)
#  define gtr(r0, r1, r2)		cr(SPARC_BG, r0, r1, r2)
#  define gti(r0, r1, i0)		cw(SPARC_BG, r0, r1, i0)
#  define gtr_u(r0, r1, r2)		cr(SPARC_BGU, r0, r1, r2)
#  define gti_u(r0, r1, i0)		cw(SPARC_BGU, r0, r1, i0)
#  define ner(r0, r1, r2)		cr(SPARC_BNE, r0, r1, r2)
#  define nei(r0, r1, i0)		cw(SPARC_BNE, r0, r1, i0)
#  define ldr_c(r0, r1)			LDSB(r1, 0, r0)
#  define ldi_c(r0, i0)			_ldi_c(_jit, r0, i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_uc(r0, r1)		LDUB(r1, 0, r0)
#  define ldi_uc(r0, i0)		_ldi_uc(_jit, r0, i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_s(r0, r1)			LDSH(r1, 0, r0)
#  define ldi_s(r0, i0)			_ldi_s(_jit, r0, i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_us(r0, r1)		LDUH(r1, 0, r0)
#  define ldi_us(r0, i0)		_ldi_us(_jit, r0, i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr(u, v)			ldr_i(u, v)
#  define ldr_i(r0, r1)			LD(r1, 0, r0)
#  define ldi(u, v)			ldi_i(u, v)
#  define ldi_i(r0, i0)			_ldi_i(_jit, r0, i0)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_c(r0, r1, r2)		LDSB(r1, r2, r0)
#  define ldxi_c(r0, r1, i0)		_ldxi_c(_jit, r0, r1, i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_uc(r0, r1, r2)		LDUB(r1, r2, r0)
#  define ldxi_uc(r0, r1, i0)		_ldxi_uc(_jit, r0, r1, i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_s(r0, r1, r2)		LDSH(r1, r2, r0)
#  define ldxi_s(r0, r1, i0)		_ldxi_s(_jit, r0, r1, i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_us(r0, r1, r2)		LDUH(r1, r2, r0)
#  define ldxi_us(r0, r1, i0)		_ldxi_us(_jit, r0, r1, i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr(u, v, w)			ldxr_i(u, v, w)
#  define ldxr_i(r0, r1, r2)		LD(r1, r2, r0)
#  define ldxi(u, v, w)			ldxi_i(u, v, w)
#  define ldxi_i(r0, r1, i0)		_ldxi_i(_jit, r0, r1, i0)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_c(r0, r1)			STB(r1, r0, 0)
#  define sti_c(i0, r0)			_sti_c(_jit, i0, r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_s(r0, r1)			STH(r1, r0, 0)
#  define sti_s(i0, r0)			_sti_s(_jit, i0, r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#  define str(u, v)			str_i(u, v)
#  define str_i(r0, r1)			STI(r1, r0, 0)
#  define sti(u, v)			sti_i(u, v)
#  define sti_i(i0, r0)			_sti_i(_jit, i0, r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_c(r0, r1, r2)		STB(r2, r1, r0)
#  define stxi_c(i0, r0, r1)		_stxi_c(_jit, i0, r0, r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_s(r0, r1, r2)		STH(r2, r1, r0)
#  define stxi_s(i0, r0, r1)		_stxi_s(_jit, i0, r0, r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr(u, v, w)			stxr_i(u, v, w)
#  define stxr_i(r0, r1, r2)		ST(r2, r1, r0)
#  define stxi(u, v, w)			stxi_i(u, v, w)
#  define stxi_i(i0, r0, r1)		_stxi_i(_jit, i0, r0, r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define br(cc, i0, r0, r1)		_br(_jit, cc, i0, r0, r1)
static jit_word_t
_br(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define bw(cc, i0, r0, i1)		_bw(_jit, cc, i0, r0, i1)
static jit_word_t
_bw(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_word_t);
#  define bltr(i0, r0, r1)		br(SPARC_BL, i0, r0, r1)
#  define blti(i0, r0, i1)		bw(SPARC_BL, i0, r0, i1)
#  define bltr_u(i0, r0, r1)		br(SPARC_BLU, i0, r0, r1)
#  define blti_u(i0, r0, i1)		bw(SPARC_BLU, i0, r0, i1)
#  define bler(i0, r0, r1)		br(SPARC_BLE, i0, r0, r1)
#  define blei(i0, r0, i1)		bw(SPARC_BLE, i0, r0, i1)
#  define bler_u(i0, r0, r1)		br(SPARC_BLEU, i0, r0, r1)
#  define blei_u(i0, r0, i1)		bw(SPARC_BLEU, i0, r0, i1)
#  define beqr(i0, r0, r1)		br(SPARC_BE, i0, r0, r1)
#  define beqi(i0, r0, i1)		bw(SPARC_BE, i0, r0, i1)
#  define bger(i0, r0, r1)		br(SPARC_BGE, i0, r0, r1)
#  define bgei(i0, r0, i1)		bw(SPARC_BGE, i0, r0, i1)
#  define bger_u(i0, r0, r1)		br(SPARC_BGEU, i0, r0, r1)
#  define bgei_u(i0, r0, i1)		bw(SPARC_BGEU, i0, r0, i1)
#  define bgtr(i0, r0, r1)		br(SPARC_BG, i0, r0, r1)
#  define bgti(i0, r0, i1)		bw(SPARC_BG, i0, r0, i1)
#  define bgtr_u(i0, r0, r1)		br(SPARC_BGU, i0, r0, r1)
#  define bgti_u(i0, r0, i1)		bw(SPARC_BGU, i0, r0, i1)
#  define bner(i0, r0, r1)		br(SPARC_BNE, i0, r0, r1)
#  define bnei(i0, r0, i1)		bw(SPARC_BNE, i0, r0, i1)
#  define b_asr(jif,add,sgn,i0,r0,r1)	_b_asr(_jit,jif,add,sgn,i0,r0,r1)
static jit_word_t
_b_asr(jit_state_t*,jit_bool_t,jit_bool_t,jit_bool_t,
       jit_word_t,jit_int32_t,jit_int32_t);
#  define b_asw(jif,add,sgn,i0,r0,i1)	_b_asw(_jit,jif,add,sgn,i0,r0,i1)
static jit_word_t
_b_asw(jit_state_t*,jit_bool_t,jit_bool_t,jit_bool_t,
       jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr(i0, r0, r1)		b_asr(1, 1, 1, i0, r0, r1)
#  define boaddi(i0, r0, i1)		b_asw(1, 1, 1, i0, r0, i1)
#  define boaddr_u(i0, r0, r1)		b_asr(1, 1, 0, i0, r0, r1)
#  define boaddi_u(i0, r0, i1)		b_asw(1, 1, 0, i0, r0, i1)
#  define bxaddr(i0, r0, r1)		b_asr(0, 1, 1, i0, r0, r1)
#  define bxaddi(i0, r0, i1)		b_asw(0, 1, 1, i0, r0, i1)
#  define bxaddr_u(i0, r0, r1)		b_asr(0, 1, 0, i0, r0, r1)
#  define bxaddi_u(i0, r0, i1)		b_asw(0, 1, 0, i0, r0, i1)
#  define bosubr(i0, r0, r1)		b_asr(1, 0, 1, i0, r0, r1)
#  define bosubi(i0, r0, i1)		b_asw(1, 0, 1, i0, r0, i1)
#  define bosubr_u(i0, r0, r1)		b_asr(1, 0, 0, i0, r0, r1)
#  define bosubi_u(i0, r0, i1)		b_asw(1, 0, 0, i0, r0, i1)
#  define bxsubr(i0, r0, r1)		b_asr(0, 0, 1, i0, r0, r1)
#  define bxsubi(i0, r0, i1)		b_asw(0, 0, 1, i0, r0, i1)
#  define bxsubr_u(i0, r0, r1)		b_asr(0, 0, 0, i0, r0, r1)
#  define bxsubi_u(i0, r0, i1)		b_asw(0, 0, 0, i0, r0, i1)
#  define bm_r(set, i0, r0, r1)		_bm_r(_jit,set,i0,r0,r1)
static jit_word_t
_bm_r(jit_state_t*,jit_bool_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define bm_w(set,i0,r0,i1)		_bm_w(_jit,set,i0,r0,i1)
static jit_word_t
_bm_w(jit_state_t*,jit_bool_t,jit_word_t,jit_int32_t,jit_word_t);
#  define bmsr(i0, r0, r1)		bm_r(1, i0, r0, r1)
#  define bmsi(i0, r0, i1)		bm_w(1, i0, r0, i1)
#  define bmcr(i0, r0, r1)		bm_r(0, i0, r0, r1)
#  define bmci(i0, r0, i1)		bm_w(0, i0, r0, i1)
#  define jmpr(r0)			_jmpr(_jit, r0)
static void _jmpr(jit_state_t*,jit_int32_t);
#  define jmpi(i0)			_jmpi(_jit, i0)
static void _jmpi(jit_state_t*,jit_word_t);
#  define jmpi_p(i0)			_jmpi_p(_jit, i0)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t);
#  define callr(r0)			_callr(_jit, r0)
static void _callr(jit_state_t*,jit_int32_t);
#  define calli(i0)			_calli(_jit, i0)
static void _calli(jit_state_t*,jit_word_t);
#  define calli_p(i0)			_calli_p(_jit, i0)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#  define prolog(node)			_prolog(_jit, node)
static void _prolog(jit_state_t*,jit_node_t*);
#  define epilog(node)			_epilog(_jit, node)
static void _epilog(jit_state_t*,jit_node_t*);
#define patch_at(jump, label)		_patch_at(_jit, jump, label)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
#endif

#if CODE
static void
_f2r(jit_state_t *_jit,
     jit_int32_t op, jit_int32_t rd, jit_int32_t op2, jit_int32_t imm22)
{
    jit_instr_t		v;
    assert(!(op  & 0xfffffffc));
    assert(!(rd  & 0xffffffe0));
    assert(!(op2 & 0xfffffff8));
    assert(s22_p(imm22));
    v.op.b    = op;
    v.rd.b    = rd;
    v.op2.b   = op2;
    v.imm22.b = imm22;
    ii(v.v);
}

static void
_f2b(jit_state_t *_jit,
     jit_int32_t op, jit_int32_t a, jit_int32_t cond, jit_int32_t op2,
     jit_int32_t disp22)
{
    jit_instr_t		v;
    assert(!(op   & 0xfffffffc));
    assert(!(a    & 0xfffffffe));
    assert(!(cond & 0xfffffff0));
    assert(!(op2  & 0xfffffff8));
    assert(s22_p(disp22));
    v.op.b     = op;
    v.a.b      = a;
    v.cond.b   = cond;
    v.op2.b    = op2;
    v.disp22.b = disp22;
    ii(v.v);
}

static void
_f3r(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd,
     jit_int32_t op3, jit_int32_t rs1, jit_int32_t rs2)
{
    jit_instr_t		v;
    assert(!(op  & 0xfffffffc));
    assert(!(rd  & 0xffffffe0));
    assert(!(op3 & 0xffffffc0));
    assert(!(rs1 & 0xffffffe0));
    assert(!(rs2 & 0xffffffe0));
    v.op.b    = op;
    v.rd.b    = rd;
    v.op3.b   = op3;
    v.rs1.b   = rs1;
    v.i.b     = 0;
    v.asi.b   = 0;
    v.rs2.b   = rs2;
    ii(v.v);
}

static void
_f3i(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd,
     jit_int32_t op3, jit_int32_t rs1, jit_int32_t simm13)
{
    jit_instr_t		v;
    assert(!(op  & 0xfffffffc));
    assert(!(rd  & 0xffffffe0));
    assert(!(op3 & 0xffffffc0));
    assert(!(rs1 & 0xffffffe0));
    assert(s13_p(simm13));
    v.op.b     = op;
    v.rd.b     = rd;
    v.op3.b    = op3;
    v.rs1.b    = rs1;
    v.i.b      = 1;
    v.simm13.b = simm13;
    ii(v.v);
}

static void
_f3t(jit_state_t *_jit, jit_int32_t cond,
     jit_int32_t rs1, jit_int32_t i, jit_int32_t rs2_imm7)
{
    jit_instr_t		v;
    assert(!(cond & 0xfffffff0));
    assert(!(i    & 0xfffffffe));
    assert(!(rs1 & 0xffffffe0));
    v.op.b     = 2;
    v.rd.b     = cond;
    v.op3.b    = 58;
    v.i.b      = i;
    if (i) {
	assert(s7_p(rs2_imm7));
	v.res.b  = 0;
	v.imm7.b = rs2_imm7;
    }
    else {
	assert(!(rs2_imm7 & 0xffffffe0));
	v.asi.b = 0;
	v.rs2.b = rs2_imm7;
    }
    ii(v.v);
}

static void
_f3a(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd,
     jit_int32_t op3, jit_int32_t rs1, jit_int32_t asi, jit_int32_t rs2)
{
    jit_instr_t		v;
    assert(!(op  & 0xfffffffc));
    assert(!(rd  & 0xffffffe0));
    assert(!(op3 & 0xffffffc0));
    assert(!(rs1 & 0xffffffe0));
    assert(!(asi & 0xffffff00));
    assert(!(rs2 & 0xffffffe0));
    v.op.b    = op;
    v.rd.b    = rd;
    v.op3.b   = op3;
    v.rs1.b   = rs1;
    v.i.b     = 0;
    v.asi.b   = asi;
    v.rs2.b   = rs2;
    ii(v.v);
}

static void
_f1(jit_state_t *_jit, jit_int32_t op, jit_int32_t disp30)
{
    jit_instr_t		v;
    assert(!(op  & 0xfffffffc));
    assert(s30_p(disp30));
    v.op.b     = op;
    v.disp30.b = disp30;
    ii(v.v);
}

static void
_nop(jit_state_t *_jit, jit_int32_t i0)
{
    for (; i0 > 0; i0 -= 4)
	NOP();
    assert(i0 == 0);
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	ORI(r1, 0, r0);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    if (s13_p(i0))
	ORI(0, i0, r0);
    else {
	SETHI(HI(i0), r0);
	if (LO(i0))
	    ORI(r0, LO(i0), r0);
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    w = _jit->pc.w;
    SETHI(HI(i0), r0);
    ORI(r0, LO(i0), r0);
    return (w);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	ADDI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	ADDIcc(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addcr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	ADDXIcc(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addxr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	SUBI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	SUBIcc(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subcr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	SUBXIcc(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subxr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_rsbi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    subi(r0, r1, i0);
    negr(r0, r0);
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	UMULI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	mulr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_iqmulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    if (sign)
	SMUL(r2, r3, r0);
    else
	UMUL(r2, r3, r0);
    RDY(r1);
}

static void
_iqmuli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    if (s13_p(i0)) {
	if (sign)
	    SMULI(r2, i0, r0);
	else
	    UMULI(r2, i0, r0);
	RDY(r1);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	iqmulr(r0, r1, r2, rn(reg), sign);
	jit_unget_reg(reg);
    }
}

static void
_divr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    rshi(rn(reg), r1, 31);
    WRY(rn(reg), 0);
    SDIV(r1, r2, r0);
    jit_unget_reg(reg);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    if (s13_p(i0)) {
	rshi(rn(reg), r1, 31);
	WRY(rn(reg), 0);
	SDIVI(r1, i0, r0);
    }
    else {
	movi(rn(reg), i0);
	divr(r0, r1, rn(reg));
    }
    jit_unget_reg(reg);
}

static void
_divr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    WRYI(0, 0);
    UDIV(r1, r2, r0);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0)) {
	WRYI(0, 0);
	UDIVI(r1, i0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	divr_u(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_iqdivr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		sv0, rg0;
    jit_int32_t		sv1, rg1;

    if (r0 == r2 || r0 == r3) {
	sv0 = jit_get_reg(jit_class_gpr);
	rg0 = rn(sv0);
    }
    else
	rg0 = r0;
    if (r1 == r2 || r1 == r3) {
	sv1 = jit_get_reg(jit_class_gpr);
	rg1 = rn(sv1);
    }
    else
	rg1 = r1;

    if (sign)
	divr(rg0, r2, r3);
    else
	divr_u(rg0, r2, r3);
    mulr(rg1, r3, rg0);
    subr(rg1, r2, rg1);
    if (rg0 != r0) {
	movr(r0, rg0);
	jit_unget_reg(sv0);
    }
    if (rg1 != r1) {
	movr(r1, rg1);
	jit_unget_reg(sv1);
    }
}

static void
_iqdivi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    iqdivr(r0, r1, r2, rn(reg), sign);
    jit_unget_reg(reg);
}

static void
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r1 || r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	divr(rn(reg), r1, r2);
	mulr(rn(reg), r2, rn(reg));
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	divr(r0, r1, r2);
	mulr(r0, r2, r0);
	subr(r0, r1, r0);
    }
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_remr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r1 || r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	divr_u(rn(reg), r1, r2);
	mulr(rn(reg), r2, rn(reg));
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	divr_u(r0, r1, r2);
	mulr(r0, r2, r0);
	subr(r0, r1, r0);
    }
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	ANDI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	andr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	ORI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	orr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	XORI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	xorr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_extr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 24);
    rshi(r0, r0, 24);
}

static void
_extr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 16);
    rshi(r0, r0, 16);
}

static void
_extr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 16);
    rshi_u(r0, r0, 16);
}

static void
_cr(jit_state_t *_jit, jit_int32_t cc,
    jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP(r1, r2);
    Ba(cc, 3);
    movi(r0, 1);
    movi(r0, 0);
}

static void
_cw(jit_state_t *_jit, jit_int32_t cc,
    jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0)) {
	CMPI(r1, i0);
	Ba(cc, 3);
	movi(r0, 1);
	movi(r0, 0);
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	cr(cc, r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDSBI(0, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_c(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDUBI(0, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_uc(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDSHI(0, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_s(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDUHI(0, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_us(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDI(0, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldr_i(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDSBI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_c(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDUBI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_uc(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDSHI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_s(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDUHI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_us(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	LDI(r1, i0, r0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	ldxr_i(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STBI(r0, 0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_c(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STHI(r0, 0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_s(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STI(r0, 0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	str_i(rn(reg), r0);
	jit_unget_reg(reg);
    }
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STBI(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_c(r0, rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STHI(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_s(r0, rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (s13_p(i0))
	STI(r1, r0, i0);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	stxr_i(r0, rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static jit_word_t
_br(jit_state_t *_jit, jit_int32_t cc,
    jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    CMP(r0, r1);
    w = _jit->pc.w;
    B(cc, (i0 - w) >> 2);
    NOP();
    return (w);
}

static jit_word_t
_bw(jit_state_t *_jit, jit_int32_t cc,
    jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (s13_p(i1)) {
	CMPI(r0, i1);
	w = _jit->pc.w;
	B(cc, (i0 - w) >> 2);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = br(cc, i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_b_asr(jit_state_t *_jit, jit_bool_t jif, jit_bool_t add, jit_bool_t sgn,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    if (add)
	ADDcc(r0, r1, r0);
    else
	SUBcc(r0, r1, r0);
    w = _jit->pc.w;
    B(sgn ?
      (jif ? SPARC_BVS : SPARC_BVC) :
      (jif ? SPARC_BCS : SPARC_BCC),
      (i0 - w) >> 2);
    NOP();
    return (w);
}

static jit_word_t
_b_asw(jit_state_t *_jit, jit_bool_t jif, jit_bool_t add, jit_bool_t sgn,
       jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (s13_p(i1)) {
	if (add)
	    ADDIcc(r0, i1, r0);
	else
	    SUBIcc(r0, i1, r0);
	w = _jit->pc.w;
	B(sgn ?
	  (jif ? SPARC_BVS : SPARC_BVC) :
	  (jif ? SPARC_BCS : SPARC_BCC),
	  (i0 - w) >> 2);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = b_asr(jif, add, sgn, i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bm_r(jit_state_t *_jit, jit_bool_t set,
      jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    BTST(r0, r1);
    w = _jit->pc.w;
    B(set ? SPARC_BNZ : SPARC_BZ, (i0 - w) >> 2);
    NOP();
    return (w);
}

static jit_word_t
_bm_w(jit_state_t *_jit, jit_bool_t set,
      jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    if (s13_p(i1)) {
	BTSTI(r0, i1);
	w = _jit->pc.w;
	B(set ? SPARC_BNZ : SPARC_BZ, (i0 - w) >> 2);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i1);
	w = bm_r(set, i0, r0, rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static void
_jmpr(jit_state_t *_jit, jit_int32_t r0)
{
    JMPL(0, r0, 0);
    NOP();
}

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    w = (i0 - _jit->pc.w) >> 2;
    if (s22_p(w)) {
	BA(w);
	NOP();
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i0);
	jmpr(rn(reg));
	jit_unget_reg(reg);
    }
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    w = movi_p(rn(reg), i0);
    jmpr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_callr(jit_state_t *_jit, jit_int32_t r0)
{
    CALL(r0);
    NOP();
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    w = (i0 - _jit->pc.w) >> 2;
    CALLI(w);
    NOP();
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    w = movi_p(rn(reg), i0);
    callr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_prolog(jit_state_t *_jit, jit_node_t *node)
{
    if (_jitc->function->define_frame || _jitc->function->assume_frame) {
	jit_int32_t	frame = -_jitc->function->frame;
	assert(_jitc->function->self.aoff >= frame);
	if (_jitc->function->assume_frame)
	    return;
	_jitc->function->self.aoff = frame;
    }
    /* align at 16 bytes boundary */
    _jitc->function->stack = ((stack_framesize +
			      _jitc->function->self.alen -
			      _jitc->function->self.aoff) + 15) & -16;
    SAVEI(_SP_REGNO, -_jitc->function->stack, _SP_REGNO);

    /* (most) other backends do not save incoming arguments, so,
     * only save locals here */
    if (jit_regset_tstbit(&_jitc->function->regset, _L0))
	stxi(0, _SP_REGNO, _L0_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _L1))
	stxi(4, _SP_REGNO, _L1_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _L2))
	stxi(8, _SP_REGNO, _L2_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _L3))
	stxi(12, _SP_REGNO, _L3_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _L4))
	stxi(16, _SP_REGNO, _L4_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _L5))
	stxi(20, _SP_REGNO, _L5_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _L6))
	stxi(24, _SP_REGNO, _L6_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _L7))
	stxi(28, _SP_REGNO, _L7_REGNO);
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    if (_jitc->function->assume_frame)
	return;
    /* (most) other backends do not save incoming arguments, so,
     * only save locals here */
    if (jit_regset_tstbit(&_jitc->function->regset, _L0))
	ldxi(_L0_REGNO, _SP_REGNO, 0);
    if (jit_regset_tstbit(&_jitc->function->regset, _L1))
	ldxi(_L1_REGNO, _SP_REGNO, 4);
    if (jit_regset_tstbit(&_jitc->function->regset, _L2))
	ldxi(_L2_REGNO, _SP_REGNO, 8);
    if (jit_regset_tstbit(&_jitc->function->regset, _L3))
	ldxi(_L3_REGNO, _SP_REGNO, 12);
    if (jit_regset_tstbit(&_jitc->function->regset, _L4))
	ldxi(_L4_REGNO, _SP_REGNO, 16);
    if (jit_regset_tstbit(&_jitc->function->regset, _L5))
	ldxi(_L5_REGNO, _SP_REGNO, 20);
    if (jit_regset_tstbit(&_jitc->function->regset, _L6))
	ldxi(_L6_REGNO, _SP_REGNO, 24);
    if (jit_regset_tstbit(&_jitc->function->regset, _L7))
	ldxi(_L7_REGNO, _SP_REGNO, 28);
    RESTOREI(0, 0, 0);
    RETL();
    NOP();
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    jit_instr_t		 i;
    union {
	jit_int32_t	*i;
	jit_word_t	 w;
    } u;

    u.w = instr;
    i.v = u.i[0];

    if (i.op.b == 0) {				/* conditional branch */
	if (i.op2.b == 2 || i.op2.b == 6) {	/* int or float condition */
	    i.disp22.b = (label - instr) >> 2;
	    u.i[0] = i.v;
	}
	else if (i.op2.b == 4) {	/* movi_p */
	    /* SETHI */
	    i.imm22.b = HI(label);
	    u.i[0] = i.v;
	    i.v = u.i[1];
	    if (i.op.b == 2 && i.op3.b == 2) {
		/* ORI */
		i.simm13.b = LO(label);
		u.i[1] = i.v;
	    }
	    else
		abort();
	}
	else
	    abort();
    }
    else
	abort();
}
#endif
