/* RISC-V opcode list
   Copyright (C) 2011-2019 Free Software Foundation, Inc.

   Contributed by Andrew Waterman (andrew@sifive.com).
   Based on MIPS target.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING3. If not,
   see <http://www.gnu.org/licenses/>.  */

#include "sysdep.h"
#include "opcode/riscv.h"
#include <stdio.h>

/* Register names used by gas and objdump.  */

const char * const riscv_gpr_names_numeric[NGPR] =
{
  "x0",   "x1",   "x2",   "x3",   "x4",   "x5",   "x6",   "x7",
  "x8",   "x9",   "x10",  "x11",  "x12",  "x13",  "x14",  "x15",
  "x16",  "x17",  "x18",  "x19",  "x20",  "x21",  "x22",  "x23",
  "x24",  "x25",  "x26",  "x27",  "x28",  "x29",  "x30",  "x31"
};

const char * const riscv_gpr_names_abi[NGPR] = {
  "zero", "ra", "sp",  "gp",  "tp", "t0",  "t1",  "t2",
  "s0",   "s1", "a0",  "a1",  "a2", "a3",  "a4",  "a5",
  "a6",   "a7", "s2",  "s3",  "s4", "s5",  "s6",  "s7",
  "s8",   "s9", "s10", "s11", "t3", "t4",  "t5",  "t6"
};

const char * const riscv_fpr_names_numeric[NFPR] =
{
  "f0",   "f1",   "f2",   "f3",   "f4",   "f5",   "f6",   "f7",
  "f8",   "f9",   "f10",  "f11",  "f12",  "f13",  "f14",  "f15",
  "f16",  "f17",  "f18",  "f19",  "f20",  "f21",  "f22",  "f23",
  "f24",  "f25",  "f26",  "f27",  "f28",  "f29",  "f30",  "f31"
};

const char * const riscv_fpr_names_abi[NFPR] = {
  "ft0", "ft1", "ft2",  "ft3",  "ft4", "ft5", "ft6",  "ft7",
  "fs0", "fs1", "fa0",  "fa1",  "fa2", "fa3", "fa4",  "fa5",
  "fa6", "fa7", "fs2",  "fs3",  "fs4", "fs5", "fs6",  "fs7",
  "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11"
};

const char * const riscv_vec_gpr_names[NVGPR] =
{
 "vv0",  "vv1",  "vv2",  "vv3",  "vv4",  "vv5",  "vv6",  "vv7",
 "vv8",  "vv9",  "vv10",  "vv11",  "vv12",  "vv13",  "vv14",  "vv15",
 "vv16",  "vv17",  "vv18",  "vv19",  "vv20",  "vv21",  "vv22",  "vv23",
 "vv24",  "vv25",  "vv26",  "vv27",  "vv28",  "vv29",  "vv30",  "vv31",
 "vv32",  "vv33",  "vv34",  "vv35",  "vv36",  "vv37",  "vv38",  "vv39",
 "vv40",  "vv41",  "vv42",  "vv43",  "vv44",  "vv45",  "vv46",  "vv47",
 "vv48",  "vv49",  "vv50",  "vv51",  "vv52",  "vv53",  "vv54",  "vv55",
 "vv56",  "vv57",  "vv58",  "vv59",  "vv60",  "vv61",  "vv62",  "vv63",
 "vv64",  "vv65",  "vv66",  "vv67",  "vv68",  "vv69",  "vv70",  "vv71",
 "vv72",  "vv73",  "vv74",  "vv75",  "vv76",  "vv77",  "vv78",  "vv79",
 "vv80",  "vv81",  "vv82",  "vv83",  "vv84",  "vv85",  "vv86",  "vv87",
 "vv88",  "vv89",  "vv90",  "vv91",  "vv92",  "vv93",  "vv94",  "vv95",
 "vv96",  "vv97",  "vv98",  "vv99", "vv100", "vv101", "vv102", "vv103",
 "vv104", "vv105", "vv106", "vv107", "vv108", "vv109", "vv110", "vv111",
 "vv112", "vv113", "vv114", "vv115", "vv116", "vv117", "vv118", "vv119",
 "vv120", "vv121", "vv122", "vv123", "vv124", "vv125", "vv126", "vv127",
 "vv128", "vv129", "vv130", "vv131", "vv132", "vv133", "vv134", "vv135",
 "vv136", "vv137", "vv138", "vv139", "vv140", "vv141", "vv142", "vv143",
 "vv144", "vv145", "vv146", "vv147", "vv148", "vv149", "vv150", "vv151",
 "vv152", "vv153", "vv154", "vv155", "vv156", "vv157", "vv158", "vv159",
 "vv160", "vv161", "vv162", "vv163", "vv164", "vv165", "vv166", "vv167",
 "vv168", "vv169", "vv170", "vv171", "vv172", "vv173", "vv174", "vv175",
 "vv176", "vv177", "vv178", "vv179", "vv180", "vv181", "vv182", "vv183",
 "vv184", "vv185", "vv186", "vv187", "vv188", "vv189", "vv190", "vv191",
 "vv192", "vv193", "vv194", "vv195", "vv196", "vv197", "vv198", "vv199",
 "vv200", "vv201", "vv202", "vv203", "vv204", "vv205", "vv206", "vv207",
 "vv208", "vv209", "vv210", "vv211", "vv212", "vv213", "vv214", "vv215",
 "vv216", "vv217", "vv218", "vv219", "vv220", "vv221", "vv222", "vv223",
 "vv224", "vv225", "vv226", "vv227", "vv228", "vv229", "vv230", "vv231",
 "vv232", "vv233", "vv234", "vv235", "vv236", "vv237", "vv238", "vv239",
 "vv240", "vv241", "vv242", "vv243", "vv244", "vv245", "vv246", "vv247",
 "vv248", "vv249", "vv250", "vv251", "vv252", "vv253", "vv254", "vv255"
};

const char * const riscv_vec_spr_names[NVSPR] =
{
 "vs0",  "vs1",  "vs2",  "vs3",  "vs4",  "vs5",  "vs6",  "vs7",
 "vs8",  "vs9",  "vs10",  "vs11",  "vs12",  "vs13",  "vs14",  "vs15",
 "vs16",  "vs17",  "vs18",  "vs19",  "vs20",  "vs21",  "vs22",  "vs23",
 "vs24",  "vs25",  "vs26",  "vs27",  "vs28",  "vs29",  "vs30",  "vs31",
 "vs32",  "vs33",  "vs34",  "vs35",  "vs36",  "vs37",  "vs38",  "vs39",
 "vs40",  "vs41",  "vs42",  "vs43",  "vs44",  "vs45",  "vs46",  "vs47",
 "vs48",  "vs49",  "vs50",  "vs51",  "vs52",  "vs53",  "vs54",  "vs55",
 "vs56",  "vs57",  "vs58",  "vs59",  "vs60",  "vs61",  "vs62",  "vs63",
 "vs64",  "vs65",  "vs66",  "vs67",  "vs68",  "vs69",  "vs70",  "vs71",
 "vs72",  "vs73",  "vs74",  "vs75",  "vs76",  "vs77",  "vs78",  "vs79",
 "vs80",  "vs81",  "vs82",  "vs83",  "vs84",  "vs85",  "vs86",  "vs87",
 "vs88",  "vs89",  "vs90",  "vs91",  "vs92",  "vs93",  "vs94",  "vs95",
 "vs96",  "vs97",  "vs98",  "vs99", "vs100", "vs101", "vs102", "vs103",
 "vs104", "vs105", "vs106", "vs107", "vs108", "vs109", "vs110", "vs111",
 "vs112", "vs113", "vs114", "vs115", "vs116", "vs117", "vs118", "vs119",
 "vs120", "vs121", "vs122", "vs123", "vs124", "vs125", "vs126", "vs127",
 "vs128", "vs129", "vs130", "vs131", "vs132", "vs133", "vs134", "vs135",
 "vs136", "vs137", "vs138", "vs139", "vs140", "vs141", "vs142", "vs143",
 "vs144", "vs145", "vs146", "vs147", "vs148", "vs149", "vs150", "vs151",
 "vs152", "vs153", "vs154", "vs155", "vs156", "vs157", "vs158", "vs159",
 "vs160", "vs161", "vs162", "vs163", "vs164", "vs165", "vs166", "vs167",
 "vs168", "vs169", "vs170", "vs171", "vs172", "vs173", "vs174", "vs175",
 "vs176", "vs177", "vs178", "vs179", "vs180", "vs181", "vs182", "vs183",
 "vs184", "vs185", "vs186", "vs187", "vs188", "vs189", "vs190", "vs191",
 "vs192", "vs193", "vs194", "vs195", "vs196", "vs197", "vs198", "vs199",
 "vs200", "vs201", "vs202", "vs203", "vs204", "vs205", "vs206", "vs207",
 "vs208", "vs209", "vs210", "vs211", "vs212", "vs213", "vs214", "vs215",
 "vs216", "vs217", "vs218", "vs219", "vs220", "vs221", "vs222", "vs223",
 "vs224", "vs225", "vs226", "vs227", "vs228", "vs229", "vs230", "vs231",
 "vs232", "vs233", "vs234", "vs235", "vs236", "vs237", "vs238", "vs239",
 "vs240", "vs241", "vs242", "vs243", "vs244", "vs245", "vs246", "vs247",
 "vs248", "vs249", "vs250", "vs251", "vs252", "vs253", "vs254", "vs255"
};

const char * const riscv_vec_apr_names[NVAPR] =
{
  "va0",   "va1",   "va2",   "va3",   "va4",   "va5",   "va6",   "va7",
  "va8",   "va9",   "va10",  "va11",  "va12",  "va13",  "va14",  "va15",
  "va16",  "va17",  "va18",  "va19",  "va20",  "va21",  "va22",  "va23",
  "va24",  "va25",  "va26",  "va27",  "va28",  "va29",  "va30",  "va31"
};

const char * const riscv_vec_ppr_names[NVPPR] =
{
  "vp0",   "vp1",   "vp2",   "vp3",   "vp4",   "vp5",   "vp6",   "vp7",
  "vp8",   "vp9",   "vp10",  "vp11",  "vp12",  "vp13",  "vp14",  "vp15"
};

/* The order of overloaded instructions matters.  Label arguments and
   register arguments look the same. Instructions that can have either
   for arguments must apear in the correct order in this table for the
   assembler to pick the right one. In other words, entries with
   immediate operands must apear after the same instruction with
   registers.

   Because of the lookup algorithm used, entries with the same opcode
   name must be contiguous.  */

#define MASK_RS1 (OP_MASK_RS1 << OP_SH_RS1)
#define MASK_RS2 (OP_MASK_RS2 << OP_SH_RS2)
#define MASK_RD (OP_MASK_RD << OP_SH_RD)
#define MASK_CRS2 (OP_MASK_CRS2 << OP_SH_CRS2)
#define MASK_IMM ENCODE_ITYPE_IMM (-1U)
#define MASK_RVC_IMM ENCODE_RVC_IMM (-1U)
#define MASK_UIMM ENCODE_UTYPE_IMM (-1U)
#define MASK_RM (OP_MASK_RM << OP_SH_RM)
#define MASK_PRED (OP_MASK_PRED << OP_SH_PRED)
#define MASK_SUCC (OP_MASK_SUCC << OP_SH_SUCC)
#define MASK_AQ (OP_MASK_AQ << OP_SH_AQ)
#define MASK_RL (OP_MASK_RL << OP_SH_RL)
#define MASK_AQRL (MASK_AQ | MASK_RL)

#define MASK_VRD (OP_MASK_VRD << OP_SH_VRD)
#define MASK_VRS1 ((insn_t)OP_MASK_VRS << OP_SH_VRS)
#define MASK_VRS2 ((insn_t)OP_MASK_VRT << OP_SH_VRT)
#define MASK_VS1 ((insn_t)OP_MASK_VS1 << OP_SH_VS1)
#define MASK_VIMM ENCODE_ITYPE_VIMM(-1U)
#define MASK_VRM ((insn_t)OP_MASK_VRM << OP_SH_VRM)
#define MASK_VAQ ((insn_t)OP_MASK_VAQ << OP_SH_VAQ)
#define MASK_VRL ((insn_t)OP_MASK_VRL << OP_SH_VRL)
#define MASK_VAQRL (MASK_VAQ | MASK_VRL)
#define MASK_VPRED ((insn_t)OP_MASK_VPRED << OP_SH_VPRED)
#define MASK_VN ((insn_t)OP_MASK_VN << OP_SH_VN)
#define MASK_VPREV (OP_MASK_VPREV << OP_SH_VPREV)
#define MASK_VSUCC (OP_MASK_VSUCC << OP_SH_VSUCC)

static int
match_opcode (const struct riscv_opcode *op, insn_t insn)
{
  return ((insn ^ op->match) & op->mask) == 0;
}

static int
match_never (const struct riscv_opcode *op ATTRIBUTE_UNUSED,
	     insn_t insn ATTRIBUTE_UNUSED)
{
  return 0;
}

static int
match_rs1_eq_rs2 (const struct riscv_opcode *op, insn_t insn)
{
  int rs1 = (insn & MASK_RS1) >> OP_SH_RS1;
  int rs2 = (insn & MASK_RS2) >> OP_SH_RS2;
  return match_opcode (op, insn) && rs1 == rs2;
}

static int
match_rd_nonzero (const struct riscv_opcode *op, insn_t insn)
{
  return match_opcode (op, insn) && ((insn & MASK_RD) != 0);
}

static int
match_c_add (const struct riscv_opcode *op, insn_t insn)
{
  return match_rd_nonzero (op, insn) && ((insn & MASK_CRS2) != 0);
}

/* We don't allow mv zero,X to become a c.mv hint, so we need a separate
   matching function for this.  */

static int
match_c_add_with_hint (const struct riscv_opcode *op, insn_t insn)
{
  return match_opcode (op, insn) && ((insn & MASK_CRS2) != 0);
}

static int
match_c_nop (const struct riscv_opcode *op, insn_t insn)
{
  return (match_opcode (op, insn)
	  && (((insn & MASK_RD) >> OP_SH_RD) == 0));
}

static int
match_c_addi16sp (const struct riscv_opcode *op, insn_t insn)
{
  return (match_opcode (op, insn)
	  && (((insn & MASK_RD) >> OP_SH_RD) == 2)
	  && EXTRACT_RVC_ADDI16SP_IMM (insn) != 0);
}

static int
match_c_lui (const struct riscv_opcode *op, insn_t insn)
{
  return (match_rd_nonzero (op, insn)
	  && (((insn & MASK_RD) >> OP_SH_RD) != 2)
	  && EXTRACT_RVC_LUI_IMM (insn) != 0);
}

/* We don't allow lui zero,X to become a c.lui hint, so we need a separate
   matching function for this.  */

static int
match_c_lui_with_hint (const struct riscv_opcode *op, insn_t insn)
{
  return (match_opcode (op, insn)
	  && (((insn & MASK_RD) >> OP_SH_RD) != 2)
	  && EXTRACT_RVC_LUI_IMM (insn) != 0);
}

static int
match_c_addi4spn (const struct riscv_opcode *op, insn_t insn)
{
  return match_opcode (op, insn) && EXTRACT_RVC_ADDI4SPN_IMM (insn) != 0;
}

/* This requires a non-zero shift.  A zero rd is a hint, so is allowed.  */

static int
match_c_slli (const struct riscv_opcode *op, insn_t insn)
{
  return match_opcode (op, insn) && EXTRACT_RVC_IMM (insn) != 0;
}

/* This requires a non-zero rd, and a non-zero shift.  */

static int
match_slli_as_c_slli (const struct riscv_opcode *op, insn_t insn)
{
  return match_rd_nonzero (op, insn) && EXTRACT_RVC_IMM (insn) != 0;
}

/* This requires a zero shift.  A zero rd is a hint, so is allowed.  */

static int
match_c_slli64 (const struct riscv_opcode *op, insn_t insn)
{
  return match_opcode (op, insn) && EXTRACT_RVC_IMM (insn) == 0;
}

/* This is used for both srli and srai.  This requires a non-zero shift.
   A zero rd is not possible.  */

static int
match_srxi_as_c_srxi (const struct riscv_opcode *op, insn_t insn)
{
  return match_opcode (op, insn) && EXTRACT_RVC_IMM (insn) != 0;
}

const struct riscv_opcode riscv_opcodes[] =
{
/* name,     xlen, isa,   operands, match, mask, match_func, pinfo.  */
{"unimp",       0, {"C", 0},   "",  0, 0xffffU,  match_opcode, INSN_ALIAS },
{"unimp",       0, {"I", 0},   "",  MATCH_CSRRW | (CSR_CYCLE << OP_SH_CSR), 0xffffffffU,  match_opcode, 0 }, /* csrw cycle, x0 */
{"ebreak",      0, {"C", 0},   "",  MATCH_C_EBREAK, MASK_C_EBREAK, match_opcode, INSN_ALIAS },
{"ebreak",      0, {"I", 0},   "",    MATCH_EBREAK, MASK_EBREAK, match_opcode, 0 },
{"sbreak",      0, {"C", 0},   "",  MATCH_C_EBREAK, MASK_C_EBREAK, match_opcode, INSN_ALIAS },
{"sbreak",      0, {"I", 0},   "",    MATCH_EBREAK, MASK_EBREAK, match_opcode, INSN_ALIAS },
{"ret",         0, {"C", 0},   "",  MATCH_C_JR | (X_RA << OP_SH_RD), MASK_C_JR | MASK_RD, match_opcode, INSN_ALIAS|INSN_BRANCH },
{"ret",         0, {"I", 0},   "",  MATCH_JALR | (X_RA << OP_SH_RS1), MASK_JALR | MASK_RD | MASK_RS1 | MASK_IMM, match_opcode, INSN_ALIAS|INSN_BRANCH },
{"jr",          0, {"C", 0},   "d",  MATCH_C_JR, MASK_C_JR, match_rd_nonzero, INSN_ALIAS|INSN_BRANCH },
{"jr",          0, {"I", 0},   "s",  MATCH_JALR, MASK_JALR | MASK_RD | MASK_IMM, match_opcode, INSN_ALIAS|INSN_BRANCH },
{"jr",          0, {"I", 0},   "o(s)",  MATCH_JALR, MASK_JALR | MASK_RD, match_opcode, INSN_ALIAS|INSN_BRANCH },
{"jr",          0, {"I", 0},   "s,j",  MATCH_JALR, MASK_JALR | MASK_RD, match_opcode, INSN_ALIAS|INSN_BRANCH },
{"jalr",        0, {"C", 0},   "d",  MATCH_C_JALR, MASK_C_JALR, match_rd_nonzero, INSN_ALIAS|INSN_JSR },
{"jalr",        0, {"I", 0},   "s",  MATCH_JALR | (X_RA << OP_SH_RD), MASK_JALR | MASK_RD | MASK_IMM, match_opcode, INSN_ALIAS|INSN_JSR },
{"jalr",        0, {"I", 0},   "o(s)",  MATCH_JALR | (X_RA << OP_SH_RD), MASK_JALR | MASK_RD, match_opcode, INSN_ALIAS|INSN_JSR },
{"jalr",        0, {"I", 0},   "s,j",  MATCH_JALR | (X_RA << OP_SH_RD), MASK_JALR | MASK_RD, match_opcode, INSN_ALIAS|INSN_JSR },
{"jalr",        0, {"I", 0},   "d,s",  MATCH_JALR, MASK_JALR | MASK_IMM, match_opcode, INSN_ALIAS|INSN_JSR },
{"jalr",        0, {"I", 0},   "d,o(s)",  MATCH_JALR, MASK_JALR, match_opcode, INSN_JSR },
{"jalr",        0, {"I", 0},   "d,s,j",  MATCH_JALR, MASK_JALR, match_opcode, INSN_JSR },
{"j",           0, {"C", 0},   "Ca",  MATCH_C_J, MASK_C_J, match_opcode, INSN_ALIAS|INSN_BRANCH },
{"j",           0, {"I", 0},   "a",  MATCH_JAL, MASK_JAL | MASK_RD, match_opcode, INSN_ALIAS|INSN_BRANCH },
{"jal",         0, {"I", 0},   "d,a",  MATCH_JAL, MASK_JAL, match_opcode, INSN_JSR },
{"jal",        32, {"C", 0},   "Ca",  MATCH_C_JAL, MASK_C_JAL, match_opcode, INSN_ALIAS|INSN_JSR },
{"jal",         0, {"I", 0},   "a",  MATCH_JAL | (X_RA << OP_SH_RD), MASK_JAL | MASK_RD, match_opcode, INSN_ALIAS|INSN_JSR },
{"call",        0, {"I", 0},   "d,c", (X_T1 << OP_SH_RS1), (int) M_CALL,  match_never, INSN_MACRO },
{"call",        0, {"I", 0},   "c", (X_RA << OP_SH_RS1) | (X_RA << OP_SH_RD), (int) M_CALL,  match_never, INSN_MACRO },
{"tail",        0, {"I", 0},   "c", (X_T1 << OP_SH_RS1), (int) M_CALL,  match_never, INSN_MACRO },
{"jump",        0, {"I", 0},   "c,s", 0, (int) M_CALL,  match_never, INSN_MACRO },
{"nop",         0, {"C", 0},   "",  MATCH_C_ADDI, 0xffff, match_opcode, INSN_ALIAS },
{"nop",         0, {"I", 0},   "",         MATCH_ADDI, MASK_ADDI | MASK_RD | MASK_RS1 | MASK_IMM, match_opcode, INSN_ALIAS },
{"lui",         0, {"C", 0},   "d,Cu",  MATCH_C_LUI, MASK_C_LUI, match_c_lui, INSN_ALIAS },
{"lui",         0, {"I", 0},   "d,u",  MATCH_LUI, MASK_LUI, match_opcode, 0 },
{"li",          0, {"C", 0},   "d,Cv",  MATCH_C_LUI, MASK_C_LUI, match_c_lui, INSN_ALIAS },
{"li",          0, {"C", 0},   "d,Co",  MATCH_C_LI, MASK_C_LI, match_rd_nonzero, INSN_ALIAS },
{"li",          0, {"I", 0},   "d,j",      MATCH_ADDI, MASK_ADDI | MASK_RS1, match_opcode, INSN_ALIAS }, /* addi */
{"li",          0, {"I", 0},   "d,I",  0,    (int) M_LI,  match_never, INSN_MACRO },
{"mv",          0, {"C", 0},   "d,CV",  MATCH_C_MV, MASK_C_MV, match_c_add, INSN_ALIAS },
{"mv",          0, {"I", 0},   "d,s",  MATCH_ADDI, MASK_ADDI | MASK_IMM, match_opcode, INSN_ALIAS },
{"move",        0, {"C", 0},   "d,CV",  MATCH_C_MV, MASK_C_MV, match_c_add, INSN_ALIAS },
{"move",        0, {"I", 0},   "d,s",  MATCH_ADDI, MASK_ADDI | MASK_IMM, match_opcode, INSN_ALIAS },
{"andi",        0, {"C", 0},   "Cs,Cw,Co",  MATCH_C_ANDI, MASK_C_ANDI, match_opcode, INSN_ALIAS },
{"andi",        0, {"I", 0},   "d,s,j",  MATCH_ANDI, MASK_ANDI, match_opcode, 0 },
{"and",         0, {"C", 0},   "Cs,Cw,Ct",  MATCH_C_AND, MASK_C_AND, match_opcode, INSN_ALIAS },
{"and",         0, {"C", 0},   "Cs,Ct,Cw",  MATCH_C_AND, MASK_C_AND, match_opcode, INSN_ALIAS },
{"and",         0, {"C", 0},   "Cs,Cw,Co",  MATCH_C_ANDI, MASK_C_ANDI, match_opcode, INSN_ALIAS },
{"and",         0, {"I", 0},   "d,s,t",  MATCH_AND, MASK_AND, match_opcode, 0 },
{"and",         0, {"I", 0},   "d,s,j",  MATCH_ANDI, MASK_ANDI, match_opcode, INSN_ALIAS },
{"beqz",        0, {"C", 0},   "Cs,Cp",  MATCH_C_BEQZ, MASK_C_BEQZ, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"beqz",        0, {"I", 0},   "s,p",  MATCH_BEQ, MASK_BEQ | MASK_RS2, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"beq",         0, {"C", 0},   "Cs,Cz,Cp",  MATCH_C_BEQZ, MASK_C_BEQZ, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"beq",         0, {"I", 0},   "s,t,p",  MATCH_BEQ, MASK_BEQ, match_opcode, INSN_CONDBRANCH },
{"blez",        0, {"I", 0},   "t,p",  MATCH_BGE, MASK_BGE | MASK_RS1, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bgez",        0, {"I", 0},   "s,p",  MATCH_BGE, MASK_BGE | MASK_RS2, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bge",         0, {"I", 0},   "s,t,p",  MATCH_BGE, MASK_BGE, match_opcode, INSN_CONDBRANCH },
{"bgeu",        0, {"I", 0},   "s,t,p",  MATCH_BGEU, MASK_BGEU, match_opcode, INSN_CONDBRANCH },
{"ble",         0, {"I", 0},   "t,s,p",  MATCH_BGE, MASK_BGE, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bleu",        0, {"I", 0},   "t,s,p",  MATCH_BGEU, MASK_BGEU, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bltz",        0, {"I", 0},   "s,p",  MATCH_BLT, MASK_BLT | MASK_RS2, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bgtz",        0, {"I", 0},   "t,p",  MATCH_BLT, MASK_BLT | MASK_RS1, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"blt",         0, {"I", 0},   "s,t,p",  MATCH_BLT, MASK_BLT, match_opcode, INSN_CONDBRANCH },
{"bltu",        0, {"I", 0},   "s,t,p",  MATCH_BLTU, MASK_BLTU, match_opcode, INSN_CONDBRANCH },
{"bgt",         0, {"I", 0},   "t,s,p",  MATCH_BLT, MASK_BLT, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bgtu",        0, {"I", 0},   "t,s,p",  MATCH_BLTU, MASK_BLTU, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bnez",        0, {"C", 0},   "Cs,Cp",  MATCH_C_BNEZ, MASK_C_BNEZ, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bnez",        0, {"I", 0},   "s,p",  MATCH_BNE, MASK_BNE | MASK_RS2, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bne",         0, {"C", 0},   "Cs,Cz,Cp",  MATCH_C_BNEZ, MASK_C_BNEZ, match_opcode, INSN_ALIAS|INSN_CONDBRANCH },
{"bne",         0, {"I", 0},   "s,t,p",  MATCH_BNE, MASK_BNE, match_opcode, INSN_CONDBRANCH },
{"addi",        0, {"C", 0},   "Ct,Cc,CK", MATCH_C_ADDI4SPN, MASK_C_ADDI4SPN, match_c_addi4spn, INSN_ALIAS },
{"addi",        0, {"C", 0},   "d,CU,Cj",  MATCH_C_ADDI, MASK_C_ADDI, match_rd_nonzero, INSN_ALIAS },
{"addi",        0, {"C", 0},   "d,CU,z",    MATCH_C_NOP, MASK_C_ADDI | MASK_RVC_IMM, match_c_nop, INSN_ALIAS },
{"addi",        0, {"C", 0},   "Cc,Cc,CL", MATCH_C_ADDI16SP, MASK_C_ADDI16SP, match_c_addi16sp, INSN_ALIAS },
{"addi",        0, {"I", 0},   "d,s,j",  MATCH_ADDI, MASK_ADDI, match_opcode, 0 },
{"add",         0, {"C", 0},   "d,CU,CV",  MATCH_C_ADD, MASK_C_ADD, match_c_add, INSN_ALIAS },
{"add",         0, {"C", 0},   "d,CV,CU",  MATCH_C_ADD, MASK_C_ADD, match_c_add, INSN_ALIAS },
{"add",         0, {"C", 0},   "d,CU,Co",  MATCH_C_ADDI, MASK_C_ADDI, match_rd_nonzero, INSN_ALIAS },
{"add",         0, {"C", 0},   "Ct,Cc,CK", MATCH_C_ADDI4SPN, MASK_C_ADDI4SPN, match_c_addi4spn, INSN_ALIAS },
{"add",         0, {"C", 0},   "Cc,Cc,CL", MATCH_C_ADDI16SP, MASK_C_ADDI16SP, match_c_addi16sp, INSN_ALIAS },
{"add",         0, {"I", 0},   "d,s,t",  MATCH_ADD, MASK_ADD, match_opcode, 0 },
/* This is used for TLS, where the fourth arg is %tprel_add, to get a reloc
   applied to an add instruction, for relaxation to use.  */
{"add",         0, {"I", 0},   "d,s,t,1",MATCH_ADD, MASK_ADD, match_opcode, 0 },
{"add",         0, {"I", 0},   "d,s,j",  MATCH_ADDI, MASK_ADDI, match_opcode, INSN_ALIAS },
{"la",          0, {"I", 0},   "d,B",  0,    (int) M_LA,  match_never, INSN_MACRO },
{"lla",         0, {"I", 0},   "d,B",  0,    (int) M_LLA,  match_never, INSN_MACRO },
{"la.tls.gd",   0, {"I", 0},   "d,A",  0,    (int) M_LA_TLS_GD,  match_never, INSN_MACRO },
{"la.tls.ie",   0, {"I", 0},   "d,A",  0,    (int) M_LA_TLS_IE,  match_never, INSN_MACRO },
{"neg",         0, {"I", 0},   "d,t",  MATCH_SUB, MASK_SUB | MASK_RS1, match_opcode, INSN_ALIAS }, /* sub 0 */
{"slli",        0, {"C", 0},   "d,CU,C>",  MATCH_C_SLLI, MASK_C_SLLI, match_slli_as_c_slli, INSN_ALIAS },
{"slli",        0, {"I", 0},   "d,s,>",   MATCH_SLLI, MASK_SLLI, match_opcode, 0 },
{"sll",         0, {"C", 0},   "d,CU,C>",  MATCH_C_SLLI, MASK_C_SLLI, match_slli_as_c_slli, INSN_ALIAS },
{"sll",         0, {"I", 0},   "d,s,t",   MATCH_SLL, MASK_SLL, match_opcode, 0 },
{"sll",         0, {"I", 0},   "d,s,>",   MATCH_SLLI, MASK_SLLI, match_opcode, INSN_ALIAS },
{"srli",        0, {"C", 0},   "Cs,Cw,C>",  MATCH_C_SRLI, MASK_C_SRLI, match_srxi_as_c_srxi, INSN_ALIAS },
{"srli",        0, {"I", 0},   "d,s,>",   MATCH_SRLI, MASK_SRLI, match_opcode, 0 },
{"srl",         0, {"C", 0},   "Cs,Cw,C>",  MATCH_C_SRLI, MASK_C_SRLI, match_srxi_as_c_srxi, INSN_ALIAS },
{"srl",         0, {"I", 0},   "d,s,t",   MATCH_SRL, MASK_SRL, match_opcode, 0 },
{"srl",         0, {"I", 0},   "d,s,>",   MATCH_SRLI, MASK_SRLI, match_opcode, INSN_ALIAS },
{"srai",        0, {"C", 0},   "Cs,Cw,C>",  MATCH_C_SRAI, MASK_C_SRAI, match_srxi_as_c_srxi, INSN_ALIAS },
{"srai",        0, {"I", 0},   "d,s,>",   MATCH_SRAI, MASK_SRAI, match_opcode, 0 },
{"sra",         0, {"C", 0},   "Cs,Cw,C>",  MATCH_C_SRAI, MASK_C_SRAI, match_srxi_as_c_srxi, INSN_ALIAS },
{"sra",         0, {"I", 0},   "d,s,t",   MATCH_SRA, MASK_SRA, match_opcode, 0 },
{"sra",         0, {"I", 0},   "d,s,>",   MATCH_SRAI, MASK_SRAI, match_opcode, INSN_ALIAS },
{"sub",         0, {"C", 0},   "Cs,Cw,Ct",  MATCH_C_SUB, MASK_C_SUB, match_opcode, INSN_ALIAS },
{"sub",         0, {"I", 0},   "d,s,t",  MATCH_SUB, MASK_SUB, match_opcode, 0 },
{"lb",          0, {"I", 0},   "d,o(s)",  MATCH_LB, MASK_LB, match_opcode, INSN_DREF|INSN_1_BYTE },
{"lb",          0, {"I", 0},   "d,A",  0, (int) M_LB, match_never, INSN_MACRO },
{"lbu",         0, {"I", 0},   "d,o(s)",  MATCH_LBU, MASK_LBU, match_opcode, INSN_DREF|INSN_1_BYTE },
{"lbu",         0, {"I", 0},   "d,A",  0, (int) M_LBU, match_never, INSN_MACRO },
{"lh",          0, {"I", 0},   "d,o(s)",  MATCH_LH, MASK_LH, match_opcode, INSN_DREF|INSN_2_BYTE },
{"lh",          0, {"I", 0},   "d,A",  0, (int) M_LH, match_never, INSN_MACRO },
{"lhu",         0, {"I", 0},   "d,o(s)",  MATCH_LHU, MASK_LHU, match_opcode, INSN_DREF|INSN_2_BYTE },
{"lhu",         0, {"I", 0},   "d,A",  0, (int) M_LHU, match_never, INSN_MACRO },
{"lw",          0, {"C", 0},   "d,Cm(Cc)",  MATCH_C_LWSP, MASK_C_LWSP, match_rd_nonzero, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"lw",          0, {"C", 0},   "Ct,Ck(Cs)",  MATCH_C_LW, MASK_C_LW, match_opcode, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"lw",          0, {"I", 0},   "d,o(s)",  MATCH_LW, MASK_LW, match_opcode, INSN_DREF|INSN_4_BYTE },
{"lw",          0, {"I", 0},   "d,A",  0, (int) M_LW, match_never, INSN_MACRO },
{"not",         0, {"I", 0},   "d,s",  MATCH_XORI | MASK_IMM, MASK_XORI | MASK_IMM, match_opcode, INSN_ALIAS },
{"ori",         0, {"I", 0},   "d,s,j",  MATCH_ORI, MASK_ORI, match_opcode, 0 },
{"or",          0, {"C", 0},   "Cs,Cw,Ct",  MATCH_C_OR, MASK_C_OR, match_opcode, INSN_ALIAS },
{"or",          0, {"C", 0},   "Cs,Ct,Cw",  MATCH_C_OR, MASK_C_OR, match_opcode, INSN_ALIAS },
{"or",          0, {"I", 0},   "d,s,t",  MATCH_OR, MASK_OR, match_opcode, 0 },
{"or",          0, {"I", 0},   "d,s,j",  MATCH_ORI, MASK_ORI, match_opcode, INSN_ALIAS },
{"auipc",       0, {"I", 0},   "d,u",  MATCH_AUIPC, MASK_AUIPC, match_opcode, 0 },
{"seqz",        0, {"I", 0},   "d,s",  MATCH_SLTIU | ENCODE_ITYPE_IMM (1), MASK_SLTIU | MASK_IMM, match_opcode, INSN_ALIAS },
{"snez",        0, {"I", 0},   "d,t",  MATCH_SLTU, MASK_SLTU | MASK_RS1, match_opcode, INSN_ALIAS },
{"sltz",        0, {"I", 0},   "d,s",  MATCH_SLT, MASK_SLT | MASK_RS2, match_opcode, INSN_ALIAS },
{"sgtz",        0, {"I", 0},   "d,t",  MATCH_SLT, MASK_SLT | MASK_RS1, match_opcode, INSN_ALIAS },
{"slti",        0, {"I", 0},   "d,s,j",  MATCH_SLTI, MASK_SLTI, match_opcode, 0 },
{"slt",         0, {"I", 0},   "d,s,t",  MATCH_SLT, MASK_SLT, match_opcode, 0 },
{"slt",         0, {"I", 0},   "d,s,j",  MATCH_SLTI, MASK_SLTI, match_opcode, INSN_ALIAS },
{"sltiu",       0, {"I", 0},   "d,s,j",  MATCH_SLTIU, MASK_SLTIU, match_opcode, 0 },
{"sltu",        0, {"I", 0},   "d,s,t",  MATCH_SLTU, MASK_SLTU, match_opcode, 0 },
{"sltu",        0, {"I", 0},   "d,s,j",  MATCH_SLTIU, MASK_SLTIU, match_opcode, INSN_ALIAS },
{"sgt",         0, {"I", 0},   "d,t,s",  MATCH_SLT, MASK_SLT, match_opcode, INSN_ALIAS },
{"sgtu",        0, {"I", 0},   "d,t,s",  MATCH_SLTU, MASK_SLTU, match_opcode, INSN_ALIAS },
{"sb",          0, {"I", 0},   "t,q(s)",  MATCH_SB, MASK_SB, match_opcode, INSN_DREF|INSN_1_BYTE },
{"sb",          0, {"I", 0},   "t,A,s",  0, (int) M_SB, match_never, INSN_MACRO },
{"sh",          0, {"I", 0},   "t,q(s)",  MATCH_SH, MASK_SH, match_opcode, INSN_DREF|INSN_2_BYTE },
{"sh",          0, {"I", 0},   "t,A,s",  0, (int) M_SH, match_never, INSN_MACRO },
{"sw",          0, {"C", 0},   "CV,CM(Cc)",  MATCH_C_SWSP, MASK_C_SWSP, match_opcode, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"sw",          0, {"C", 0},   "Ct,Ck(Cs)",  MATCH_C_SW, MASK_C_SW, match_opcode, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"sw",          0, {"I", 0},   "t,q(s)",  MATCH_SW, MASK_SW, match_opcode, INSN_DREF|INSN_4_BYTE },
{"sw",          0, {"I", 0},   "t,A,s",  0, (int) M_SW, match_never, INSN_MACRO },
{"fence",       0, {"I", 0},   "",  MATCH_FENCE | MASK_PRED | MASK_SUCC, MASK_FENCE | MASK_RD | MASK_RS1 | MASK_IMM, match_opcode, INSN_ALIAS },
{"fence",       0, {"I", 0},   "P,Q",  MATCH_FENCE, MASK_FENCE | MASK_RD | MASK_RS1 | (MASK_IMM & ~MASK_PRED & ~MASK_SUCC), match_opcode, 0 },
{"fence.i",     0, {"I", 0},   "",  MATCH_FENCE_I, MASK_FENCE | MASK_RD | MASK_RS1 | MASK_IMM, match_opcode, 0 },
{"fence.tso",   0, {"I", 0},   "",  MATCH_FENCE_TSO, MASK_FENCE_TSO | MASK_RD | MASK_RS1, match_opcode, INSN_ALIAS },
{"rdcycle",     0, {"I", 0},   "d",  MATCH_RDCYCLE, MASK_RDCYCLE, match_opcode, INSN_ALIAS },
{"rdinstret",   0, {"I", 0},   "d",  MATCH_RDINSTRET, MASK_RDINSTRET, match_opcode, INSN_ALIAS },
{"rdtime",      0, {"I", 0},   "d",  MATCH_RDTIME, MASK_RDTIME, match_opcode, INSN_ALIAS },
{"rdcycleh",   32, {"I", 0},   "d",  MATCH_RDCYCLEH, MASK_RDCYCLEH, match_opcode, INSN_ALIAS },
{"rdinstreth", 32, {"I", 0},   "d",  MATCH_RDINSTRETH, MASK_RDINSTRETH, match_opcode, INSN_ALIAS },
{"rdtimeh",    32, {"I", 0},   "d",  MATCH_RDTIMEH, MASK_RDTIMEH, match_opcode, INSN_ALIAS },
{"ecall",       0, {"I", 0},   "",    MATCH_SCALL, MASK_SCALL, match_opcode, 0 },
{"scall",       0, {"I", 0},   "",    MATCH_SCALL, MASK_SCALL, match_opcode, 0 },
{"xori",        0, {"I", 0},   "d,s,j",  MATCH_XORI, MASK_XORI, match_opcode, 0 },
{"xor",         0, {"C", 0},   "Cs,Cw,Ct",  MATCH_C_XOR, MASK_C_XOR, match_opcode, INSN_ALIAS },
{"xor",         0, {"C", 0},   "Cs,Ct,Cw",  MATCH_C_XOR, MASK_C_XOR, match_opcode, INSN_ALIAS },
{"xor",         0, {"I", 0},   "d,s,t",  MATCH_XOR, MASK_XOR, match_opcode, 0 },
{"xor",         0, {"I", 0},   "d,s,j",  MATCH_XORI, MASK_XORI, match_opcode, INSN_ALIAS },
{"lwu",        64, {"I", 0}, "d,o(s)",  MATCH_LWU, MASK_LWU, match_opcode, INSN_DREF|INSN_4_BYTE },
{"lwu",        64, {"I", 0}, "d,A",  0, (int) M_LWU, match_never, INSN_MACRO },
{"ld",         64, {"C", 0}, "d,Cn(Cc)",  MATCH_C_LDSP, MASK_C_LDSP, match_rd_nonzero, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"ld",         64, {"C", 0}, "Ct,Cl(Cs)",  MATCH_C_LD, MASK_C_LD, match_opcode, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"ld",         64, {"I", 0}, "d,o(s)", MATCH_LD, MASK_LD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"ld",         64, {"I", 0}, "d,A",  0, (int) M_LD, match_never, INSN_MACRO },
{"sd",         64, {"C", 0}, "CV,CN(Cc)",  MATCH_C_SDSP, MASK_C_SDSP, match_opcode, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"sd",         64, {"C", 0}, "Ct,Cl(Cs)",  MATCH_C_SD, MASK_C_SD, match_opcode, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"sd",         64, {"I", 0}, "t,q(s)",  MATCH_SD, MASK_SD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"sd",         64, {"I", 0}, "t,A,s",  0, (int) M_SD, match_never, INSN_MACRO },
{"sext.w",     64, {"C", 0}, "d,CU",  MATCH_C_ADDIW, MASK_C_ADDIW | MASK_RVC_IMM, match_rd_nonzero, INSN_ALIAS },
{"sext.w",     64, {"I", 0}, "d,s",  MATCH_ADDIW, MASK_ADDIW | MASK_IMM, match_opcode, INSN_ALIAS },
{"addiw",      64, {"C", 0}, "d,CU,Co",  MATCH_C_ADDIW, MASK_C_ADDIW, match_rd_nonzero, INSN_ALIAS },
{"addiw",      64, {"I", 0}, "d,s,j",  MATCH_ADDIW, MASK_ADDIW, match_opcode, 0 },
{"addw",       64, {"C", 0}, "Cs,Cw,Ct",  MATCH_C_ADDW, MASK_C_ADDW, match_opcode, INSN_ALIAS },
{"addw",       64, {"C", 0}, "Cs,Ct,Cw",  MATCH_C_ADDW, MASK_C_ADDW, match_opcode, INSN_ALIAS },
{"addw",       64, {"C", 0}, "d,CU,Co",  MATCH_C_ADDIW, MASK_C_ADDIW, match_rd_nonzero, INSN_ALIAS },
{"addw",       64, {"I", 0}, "d,s,t",  MATCH_ADDW, MASK_ADDW, match_opcode, 0 },
{"addw",       64, {"I", 0}, "d,s,j",  MATCH_ADDIW, MASK_ADDIW, match_opcode, INSN_ALIAS },
{"negw",       64, {"I", 0}, "d,t",  MATCH_SUBW, MASK_SUBW | MASK_RS1, match_opcode, INSN_ALIAS }, /* sub 0 */
{"slliw",      64, {"I", 0}, "d,s,<",   MATCH_SLLIW, MASK_SLLIW, match_opcode, 0 },
{"sllw",       64, {"I", 0}, "d,s,t",   MATCH_SLLW, MASK_SLLW, match_opcode, 0 },
{"sllw",       64, {"I", 0}, "d,s,<",   MATCH_SLLIW, MASK_SLLIW, match_opcode, INSN_ALIAS },
{"srliw",      64, {"I", 0}, "d,s,<",   MATCH_SRLIW, MASK_SRLIW, match_opcode, 0 },
{"srlw",       64, {"I", 0}, "d,s,t",   MATCH_SRLW, MASK_SRLW, match_opcode, 0 },
{"srlw",       64, {"I", 0}, "d,s,<",   MATCH_SRLIW, MASK_SRLIW, match_opcode, INSN_ALIAS },
{"sraiw",      64, {"I", 0}, "d,s,<",   MATCH_SRAIW, MASK_SRAIW, match_opcode, 0 },
{"sraw",       64, {"I", 0}, "d,s,t",   MATCH_SRAW, MASK_SRAW, match_opcode, 0 },
{"sraw",       64, {"I", 0}, "d,s,<",   MATCH_SRAIW, MASK_SRAIW, match_opcode, INSN_ALIAS },
{"subw",       64, {"C", 0}, "Cs,Cw,Ct",  MATCH_C_SUBW, MASK_C_SUBW, match_opcode, INSN_ALIAS },
{"subw",       64, {"I", 0}, "d,s,t",  MATCH_SUBW, MASK_SUBW, match_opcode, 0 },

/* Atomic memory operation instruction subset */
{"lr.w",         0, {"A", 0},   "d,0(s)",    MATCH_LR_W, MASK_LR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"sc.w",         0, {"A", 0},   "d,t,0(s)",  MATCH_SC_W, MASK_SC_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoadd.w",     0, {"A", 0},   "d,t,0(s)",  MATCH_AMOADD_W, MASK_AMOADD_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoswap.w",    0, {"A", 0},   "d,t,0(s)",  MATCH_AMOSWAP_W, MASK_AMOSWAP_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoand.w",     0, {"A", 0},   "d,t,0(s)",  MATCH_AMOAND_W, MASK_AMOAND_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoor.w",      0, {"A", 0},   "d,t,0(s)",  MATCH_AMOOR_W, MASK_AMOOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoxor.w",     0, {"A", 0},   "d,t,0(s)",  MATCH_AMOXOR_W, MASK_AMOXOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomax.w",     0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAX_W, MASK_AMOMAX_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomaxu.w",    0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAXU_W, MASK_AMOMAXU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomin.w",     0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMIN_W, MASK_AMOMIN_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amominu.w",    0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMINU_W, MASK_AMOMINU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"lr.w.aq",      0, {"A", 0},   "d,0(s)",    MATCH_LR_W | MASK_AQ, MASK_LR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"sc.w.aq",      0, {"A", 0},   "d,t,0(s)",  MATCH_SC_W | MASK_AQ, MASK_SC_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoadd.w.aq",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOADD_W | MASK_AQ, MASK_AMOADD_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoswap.w.aq", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOSWAP_W | MASK_AQ, MASK_AMOSWAP_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoand.w.aq",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOAND_W | MASK_AQ, MASK_AMOAND_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoor.w.aq",   0, {"A", 0},   "d,t,0(s)",  MATCH_AMOOR_W | MASK_AQ, MASK_AMOOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoxor.w.aq",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOXOR_W | MASK_AQ, MASK_AMOXOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomax.w.aq",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAX_W | MASK_AQ, MASK_AMOMAX_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomaxu.w.aq", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAXU_W | MASK_AQ, MASK_AMOMAXU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomin.w.aq",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMIN_W | MASK_AQ, MASK_AMOMIN_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amominu.w.aq", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMINU_W | MASK_AQ, MASK_AMOMINU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"lr.w.rl",      0, {"A", 0},   "d,0(s)",    MATCH_LR_W | MASK_RL, MASK_LR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"sc.w.rl",      0, {"A", 0},   "d,t,0(s)",  MATCH_SC_W | MASK_RL, MASK_SC_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoadd.w.rl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOADD_W | MASK_RL, MASK_AMOADD_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoswap.w.rl", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOSWAP_W | MASK_RL, MASK_AMOSWAP_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoand.w.rl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOAND_W | MASK_RL, MASK_AMOAND_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoor.w.rl",   0, {"A", 0},   "d,t,0(s)",  MATCH_AMOOR_W | MASK_RL, MASK_AMOOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoxor.w.rl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOXOR_W | MASK_RL, MASK_AMOXOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomax.w.rl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAX_W | MASK_RL, MASK_AMOMAX_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomaxu.w.rl", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAXU_W | MASK_RL, MASK_AMOMAXU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomin.w.rl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMIN_W | MASK_RL, MASK_AMOMIN_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amominu.w.rl", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMINU_W | MASK_RL, MASK_AMOMINU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"lr.w.aqrl",    0, {"A", 0},   "d,0(s)",    MATCH_LR_W | MASK_AQRL, MASK_LR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"sc.w.aqrl",    0, {"A", 0},   "d,t,0(s)",  MATCH_SC_W | MASK_AQRL, MASK_SC_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoadd.w.aqrl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOADD_W | MASK_AQRL, MASK_AMOADD_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoswap.w.aqrl", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOSWAP_W | MASK_AQRL, MASK_AMOSWAP_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoand.w.aqrl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOAND_W | MASK_AQRL, MASK_AMOAND_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoor.w.aqrl",   0, {"A", 0},   "d,t,0(s)",  MATCH_AMOOR_W | MASK_AQRL, MASK_AMOOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amoxor.w.aqrl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOXOR_W | MASK_AQRL, MASK_AMOXOR_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomax.w.aqrl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAX_W | MASK_AQRL, MASK_AMOMAX_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomaxu.w.aqrl", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMAXU_W | MASK_AQRL, MASK_AMOMAXU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amomin.w.aqrl",  0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMIN_W | MASK_AQRL, MASK_AMOMIN_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"amominu.w.aqrl", 0, {"A", 0},   "d,t,0(s)",  MATCH_AMOMINU_W | MASK_AQRL, MASK_AMOMINU_W | MASK_AQRL, match_opcode, INSN_DREF|INSN_4_BYTE },
{"lr.d",         64, {"A", 0} , "d,0(s)",    MATCH_LR_D, MASK_LR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"sc.d",         64, {"A", 0} , "d,t,0(s)",  MATCH_SC_D, MASK_SC_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoadd.d",     64, {"A", 0} , "d,t,0(s)",  MATCH_AMOADD_D, MASK_AMOADD_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoswap.d",    64, {"A", 0} , "d,t,0(s)",  MATCH_AMOSWAP_D, MASK_AMOSWAP_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoand.d",     64, {"A", 0} , "d,t,0(s)",  MATCH_AMOAND_D, MASK_AMOAND_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoor.d",      64, {"A", 0} , "d,t,0(s)",  MATCH_AMOOR_D, MASK_AMOOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoxor.d",     64, {"A", 0} , "d,t,0(s)",  MATCH_AMOXOR_D, MASK_AMOXOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomax.d",     64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAX_D, MASK_AMOMAX_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomaxu.d",    64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAXU_D, MASK_AMOMAXU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomin.d",     64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMIN_D, MASK_AMOMIN_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amominu.d",    64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMINU_D, MASK_AMOMINU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"lr.d.aq",      64, {"A", 0} , "d,0(s)",    MATCH_LR_D | MASK_AQ, MASK_LR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"sc.d.aq",      64, {"A", 0} , "d,t,0(s)",  MATCH_SC_D | MASK_AQ, MASK_SC_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoadd.d.aq",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOADD_D | MASK_AQ, MASK_AMOADD_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoswap.d.aq", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOSWAP_D | MASK_AQ, MASK_AMOSWAP_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoand.d.aq",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOAND_D | MASK_AQ, MASK_AMOAND_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoor.d.aq",   64, {"A", 0} , "d,t,0(s)",  MATCH_AMOOR_D | MASK_AQ, MASK_AMOOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoxor.d.aq",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOXOR_D | MASK_AQ, MASK_AMOXOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomax.d.aq",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAX_D | MASK_AQ, MASK_AMOMAX_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomaxu.d.aq", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAXU_D | MASK_AQ, MASK_AMOMAXU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomin.d.aq",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMIN_D | MASK_AQ, MASK_AMOMIN_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amominu.d.aq", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMINU_D | MASK_AQ, MASK_AMOMINU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"lr.d.rl",      64, {"A", 0} , "d,0(s)",    MATCH_LR_D | MASK_RL, MASK_LR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"sc.d.rl",      64, {"A", 0} , "d,t,0(s)",  MATCH_SC_D | MASK_RL, MASK_SC_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoadd.d.rl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOADD_D | MASK_RL, MASK_AMOADD_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoswap.d.rl", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOSWAP_D | MASK_RL, MASK_AMOSWAP_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoand.d.rl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOAND_D | MASK_RL, MASK_AMOAND_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoor.d.rl",   64, {"A", 0} , "d,t,0(s)",  MATCH_AMOOR_D | MASK_RL, MASK_AMOOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoxor.d.rl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOXOR_D | MASK_RL, MASK_AMOXOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomax.d.rl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAX_D | MASK_RL, MASK_AMOMAX_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomaxu.d.rl", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAXU_D | MASK_RL, MASK_AMOMAXU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomin.d.rl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMIN_D | MASK_RL, MASK_AMOMIN_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amominu.d.rl", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMINU_D | MASK_RL, MASK_AMOMINU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"lr.d.aqrl",    64, {"A", 0} , "d,0(s)",    MATCH_LR_D | MASK_AQRL, MASK_LR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"sc.d.aqrl",    64, {"A", 0} , "d,t,0(s)",  MATCH_SC_D | MASK_AQRL, MASK_SC_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoadd.d.aqrl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOADD_D | MASK_AQRL, MASK_AMOADD_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoswap.d.aqrl", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOSWAP_D | MASK_AQRL, MASK_AMOSWAP_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoand.d.aqrl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOAND_D | MASK_AQRL, MASK_AMOAND_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoor.d.aqrl",   64, {"A", 0} , "d,t,0(s)",  MATCH_AMOOR_D | MASK_AQRL, MASK_AMOOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amoxor.d.aqrl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOXOR_D | MASK_AQRL, MASK_AMOXOR_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomax.d.aqrl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAX_D | MASK_AQRL, MASK_AMOMAX_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomaxu.d.aqrl", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMAXU_D | MASK_AQRL, MASK_AMOMAXU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amomin.d.aqrl",  64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMIN_D | MASK_AQRL, MASK_AMOMIN_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },
{"amominu.d.aqrl", 64, {"A", 0} , "d,t,0(s)",  MATCH_AMOMINU_D | MASK_AQRL, MASK_AMOMINU_D | MASK_AQRL, match_opcode, INSN_DREF|INSN_8_BYTE },

/* Multiply/Divide instruction subset */
{"mul",       0, {"M", 0},   "d,s,t",  MATCH_MUL, MASK_MUL, match_opcode, 0 },
{"mulh",      0, {"M", 0},   "d,s,t",  MATCH_MULH, MASK_MULH, match_opcode, 0 },
{"mulhu",     0, {"M", 0},   "d,s,t",  MATCH_MULHU, MASK_MULHU, match_opcode, 0 },
{"mulhsu",    0, {"M", 0},   "d,s,t",  MATCH_MULHSU, MASK_MULHSU, match_opcode, 0 },
{"div",       0, {"M", 0},   "d,s,t",  MATCH_DIV, MASK_DIV, match_opcode, 0 },
{"divu",      0, {"M", 0},   "d,s,t",  MATCH_DIVU, MASK_DIVU, match_opcode, 0 },
{"rem",       0, {"M", 0},   "d,s,t",  MATCH_REM, MASK_REM, match_opcode, 0 },
{"remu",      0, {"M", 0},   "d,s,t",  MATCH_REMU, MASK_REMU, match_opcode, 0 },
{"mulw",     64, {"M", 0}, "d,s,t",  MATCH_MULW, MASK_MULW, match_opcode, 0 },
{"divw",     64, {"M", 0}, "d,s,t",  MATCH_DIVW, MASK_DIVW, match_opcode, 0 },
{"divuw",    64, {"M", 0}, "d,s,t",  MATCH_DIVUW, MASK_DIVUW, match_opcode, 0 },
{"remw",     64, {"M", 0}, "d,s,t",  MATCH_REMW, MASK_REMW, match_opcode, 0 },
{"remuw",    64, {"M", 0}, "d,s,t",  MATCH_REMUW, MASK_REMUW, match_opcode, 0 },

/* Single-precision floating-point instruction subset */
{"frcsr",     0, {"F", 0},   "d",  MATCH_FRCSR, MASK_FRCSR, match_opcode, INSN_ALIAS },
{"frsr",      0, {"F", 0},   "d",  MATCH_FRCSR, MASK_FRCSR, match_opcode, INSN_ALIAS },
{"fscsr",     0, {"F", 0},   "s",  MATCH_FSCSR, MASK_FSCSR | MASK_RD, match_opcode, INSN_ALIAS },
{"fscsr",     0, {"F", 0},   "d,s",  MATCH_FSCSR, MASK_FSCSR, match_opcode, INSN_ALIAS },
{"fssr",      0, {"F", 0},   "s",  MATCH_FSCSR, MASK_FSCSR | MASK_RD, match_opcode, INSN_ALIAS },
{"fssr",      0, {"F", 0},   "d,s",  MATCH_FSCSR, MASK_FSCSR, match_opcode, INSN_ALIAS },
{"frrm",      0, {"F", 0},   "d",  MATCH_FRRM, MASK_FRRM, match_opcode, INSN_ALIAS },
{"fsrm",      0, {"F", 0},   "s",  MATCH_FSRM, MASK_FSRM | MASK_RD, match_opcode, INSN_ALIAS },
{"fsrm",      0, {"F", 0},   "d,s",  MATCH_FSRM, MASK_FSRM, match_opcode, INSN_ALIAS },
{"fsrmi",     0, {"F", 0},   "d,Z",  MATCH_FSRMI, MASK_FSRMI, match_opcode, INSN_ALIAS },
{"fsrmi",     0, {"F", 0},   "Z",  MATCH_FSRMI, MASK_FSRMI | MASK_RD, match_opcode, INSN_ALIAS },
{"frflags",   0, {"F", 0},   "d",  MATCH_FRFLAGS, MASK_FRFLAGS, match_opcode, INSN_ALIAS },
{"fsflags",   0, {"F", 0},   "s",  MATCH_FSFLAGS, MASK_FSFLAGS | MASK_RD, match_opcode, INSN_ALIAS },
{"fsflags",   0, {"F", 0},   "d,s",  MATCH_FSFLAGS, MASK_FSFLAGS, match_opcode, INSN_ALIAS },
{"fsflagsi",  0, {"F", 0},   "d,Z",  MATCH_FSFLAGSI, MASK_FSFLAGSI, match_opcode, INSN_ALIAS },
{"fsflagsi",  0, {"F", 0},   "Z",  MATCH_FSFLAGSI, MASK_FSFLAGSI | MASK_RD, match_opcode, INSN_ALIAS },
{"flw",      32, {"F", "C", 0}, "D,Cm(Cc)",  MATCH_C_FLWSP, MASK_C_FLWSP, match_opcode, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"flw",      32, {"F", "C", 0}, "CD,Ck(Cs)",  MATCH_C_FLW, MASK_C_FLW, match_opcode, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"flw",       0, {"F", 0},   "D,o(s)",  MATCH_FLW, MASK_FLW, match_opcode, INSN_DREF|INSN_4_BYTE },
{"flw",       0, {"F", 0},   "D,A,s",  0, (int) M_FLW, match_never, INSN_MACRO },
{"fsw",      32, {"F", "C", 0}, "CT,CM(Cc)",  MATCH_C_FSWSP, MASK_C_FSWSP, match_opcode, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"fsw",      32, {"F", "C", 0}, "CD,Ck(Cs)",  MATCH_C_FSW, MASK_C_FSW, match_opcode, INSN_ALIAS|INSN_DREF|INSN_4_BYTE },
{"fsw",       0, {"F", 0},   "T,q(s)",  MATCH_FSW, MASK_FSW, match_opcode, INSN_DREF|INSN_4_BYTE },
{"fsw",       0, {"F", 0},   "T,A,s",  0, (int) M_FSW, match_never, INSN_MACRO },

{"fmv.x.w",    0, {"F", 0},   "d,S",  MATCH_FMV_X_S, MASK_FMV_X_S, match_opcode, 0 },
{"fmv.w.x",    0, {"F", 0},   "D,s",  MATCH_FMV_S_X, MASK_FMV_S_X, match_opcode, 0 },

{"fmv.x.s",    0, {"F", 0},   "d,S",  MATCH_FMV_X_S, MASK_FMV_X_S, match_opcode, 0 },
{"fmv.s.x",    0, {"F", 0},   "D,s",  MATCH_FMV_S_X, MASK_FMV_S_X, match_opcode, 0 },

{"fmv.s",      0, {"F", 0},   "D,U",  MATCH_FSGNJ_S, MASK_FSGNJ_S, match_rs1_eq_rs2, INSN_ALIAS },
{"fneg.s",     0, {"F", 0},   "D,U",  MATCH_FSGNJN_S, MASK_FSGNJN_S, match_rs1_eq_rs2, INSN_ALIAS },
{"fabs.s",     0, {"F", 0},   "D,U",  MATCH_FSGNJX_S, MASK_FSGNJX_S, match_rs1_eq_rs2, INSN_ALIAS },
{"fsgnj.s",    0, {"F", 0},   "D,S,T",  MATCH_FSGNJ_S, MASK_FSGNJ_S, match_opcode, 0 },
{"fsgnjn.s",   0, {"F", 0},   "D,S,T",  MATCH_FSGNJN_S, MASK_FSGNJN_S, match_opcode, 0 },
{"fsgnjx.s",   0, {"F", 0},   "D,S,T",  MATCH_FSGNJX_S, MASK_FSGNJX_S, match_opcode, 0 },
{"fadd.s",     0, {"F", 0},   "D,S,T",  MATCH_FADD_S | MASK_RM, MASK_FADD_S | MASK_RM, match_opcode, 0 },
{"fadd.s",     0, {"F", 0},   "D,S,T,m",  MATCH_FADD_S, MASK_FADD_S, match_opcode, 0 },
{"fsub.s",     0, {"F", 0},   "D,S,T",  MATCH_FSUB_S | MASK_RM, MASK_FSUB_S | MASK_RM, match_opcode, 0 },
{"fsub.s",     0, {"F", 0},   "D,S,T,m",  MATCH_FSUB_S, MASK_FSUB_S, match_opcode, 0 },
{"fmul.s",     0, {"F", 0},   "D,S,T",  MATCH_FMUL_S | MASK_RM, MASK_FMUL_S | MASK_RM, match_opcode, 0 },
{"fmul.s",     0, {"F", 0},   "D,S,T,m",  MATCH_FMUL_S, MASK_FMUL_S, match_opcode, 0 },
{"fdiv.s",     0, {"F", 0},   "D,S,T",  MATCH_FDIV_S | MASK_RM, MASK_FDIV_S | MASK_RM, match_opcode, 0 },
{"fdiv.s",     0, {"F", 0},   "D,S,T,m",  MATCH_FDIV_S, MASK_FDIV_S, match_opcode, 0 },
{"fsqrt.s",    0, {"F", 0},   "D,S",  MATCH_FSQRT_S | MASK_RM, MASK_FSQRT_S | MASK_RM, match_opcode, 0 },
{"fsqrt.s",    0, {"F", 0},   "D,S,m",  MATCH_FSQRT_S, MASK_FSQRT_S, match_opcode, 0 },
{"fmin.s",     0, {"F", 0},   "D,S,T",  MATCH_FMIN_S, MASK_FMIN_S, match_opcode, 0 },
{"fmax.s",     0, {"F", 0},   "D,S,T",  MATCH_FMAX_S, MASK_FMAX_S, match_opcode, 0 },
{"fmadd.s",    0, {"F", 0},   "D,S,T,R",  MATCH_FMADD_S | MASK_RM, MASK_FMADD_S | MASK_RM, match_opcode, 0 },
{"fmadd.s",    0, {"F", 0},   "D,S,T,R,m",  MATCH_FMADD_S, MASK_FMADD_S, match_opcode, 0 },
{"fnmadd.s",   0, {"F", 0},   "D,S,T,R",  MATCH_FNMADD_S | MASK_RM, MASK_FNMADD_S | MASK_RM, match_opcode, 0 },
{"fnmadd.s",   0, {"F", 0},   "D,S,T,R,m",  MATCH_FNMADD_S, MASK_FNMADD_S, match_opcode, 0 },
{"fmsub.s",    0, {"F", 0},   "D,S,T,R",  MATCH_FMSUB_S | MASK_RM, MASK_FMSUB_S | MASK_RM, match_opcode, 0 },
{"fmsub.s",    0, {"F", 0},   "D,S,T,R,m",  MATCH_FMSUB_S, MASK_FMSUB_S, match_opcode, 0 },
{"fnmsub.s",   0, {"F", 0},   "D,S,T,R",  MATCH_FNMSUB_S | MASK_RM, MASK_FNMSUB_S | MASK_RM, match_opcode, 0 },
{"fnmsub.s",   0, {"F", 0},   "D,S,T,R,m",  MATCH_FNMSUB_S, MASK_FNMSUB_S, match_opcode, 0 },
{"fcvt.w.s",   0, {"F", 0},   "d,S",  MATCH_FCVT_W_S | MASK_RM, MASK_FCVT_W_S | MASK_RM, match_opcode, 0 },
{"fcvt.w.s",   0, {"F", 0},   "d,S,m",  MATCH_FCVT_W_S, MASK_FCVT_W_S, match_opcode, 0 },
{"fcvt.wu.s",  0, {"F", 0},   "d,S",  MATCH_FCVT_WU_S | MASK_RM, MASK_FCVT_WU_S | MASK_RM, match_opcode, 0 },
{"fcvt.wu.s",  0, {"F", 0},   "d,S,m",  MATCH_FCVT_WU_S, MASK_FCVT_WU_S, match_opcode, 0 },
{"fcvt.s.w",   0, {"F", 0},   "D,s",  MATCH_FCVT_S_W | MASK_RM, MASK_FCVT_S_W | MASK_RM, match_opcode, 0 },
{"fcvt.s.w",   0, {"F", 0},   "D,s,m",  MATCH_FCVT_S_W, MASK_FCVT_S_W, match_opcode, 0 },
{"fcvt.s.wu",  0, {"F", 0},   "D,s",  MATCH_FCVT_S_WU | MASK_RM, MASK_FCVT_S_W | MASK_RM, match_opcode, 0 },
{"fcvt.s.wu",  0, {"F", 0},   "D,s,m",  MATCH_FCVT_S_WU, MASK_FCVT_S_WU, match_opcode, 0 },
{"fclass.s",   0, {"F", 0},   "d,S",  MATCH_FCLASS_S, MASK_FCLASS_S, match_opcode, 0 },
{"feq.s",      0, {"F", 0},   "d,S,T",    MATCH_FEQ_S, MASK_FEQ_S, match_opcode, 0 },
{"flt.s",      0, {"F", 0},   "d,S,T",    MATCH_FLT_S, MASK_FLT_S, match_opcode, 0 },
{"fle.s",      0, {"F", 0},   "d,S,T",    MATCH_FLE_S, MASK_FLE_S, match_opcode, 0 },
{"fgt.s",      0, {"F", 0},   "d,T,S",    MATCH_FLT_S, MASK_FLT_S, match_opcode, 0 },
{"fge.s",      0, {"F", 0},   "d,T,S",    MATCH_FLE_S, MASK_FLE_S, match_opcode, 0 },
{"fcvt.l.s",  64, {"F", 0}, "d,S",  MATCH_FCVT_L_S | MASK_RM, MASK_FCVT_L_S | MASK_RM, match_opcode, 0 },
{"fcvt.l.s",  64, {"F", 0}, "d,S,m",  MATCH_FCVT_L_S, MASK_FCVT_L_S, match_opcode, 0 },
{"fcvt.lu.s", 64, {"F", 0}, "d,S",  MATCH_FCVT_LU_S | MASK_RM, MASK_FCVT_LU_S | MASK_RM, match_opcode, 0 },
{"fcvt.lu.s", 64, {"F", 0}, "d,S,m",  MATCH_FCVT_LU_S, MASK_FCVT_LU_S, match_opcode, 0 },
{"fcvt.s.l",  64, {"F", 0}, "D,s",  MATCH_FCVT_S_L | MASK_RM, MASK_FCVT_S_L | MASK_RM, match_opcode, 0 },
{"fcvt.s.l",  64, {"F", 0}, "D,s,m",  MATCH_FCVT_S_L, MASK_FCVT_S_L, match_opcode, 0 },
{"fcvt.s.lu", 64, {"F", 0}, "D,s",  MATCH_FCVT_S_LU | MASK_RM, MASK_FCVT_S_L | MASK_RM, match_opcode, 0 },
{"fcvt.s.lu", 64, {"F", 0}, "D,s,m",  MATCH_FCVT_S_LU, MASK_FCVT_S_LU, match_opcode, 0 },

/* Double-precision floating-point instruction subset */
{"fld",        0, {"D", "C", 0},   "D,Cn(Cc)",  MATCH_C_FLDSP, MASK_C_FLDSP, match_opcode, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"fld",        0, {"D", "C", 0},   "CD,Cl(Cs)",  MATCH_C_FLD, MASK_C_FLD, match_opcode, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"fld",        0, {"D", 0},   "D,o(s)",  MATCH_FLD, MASK_FLD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"fld",        0, {"D", 0},   "D,A,s",  0, (int) M_FLD, match_never, INSN_MACRO },
{"fsd",        0, {"D", "C", 0},   "CT,CN(Cc)",  MATCH_C_FSDSP, MASK_C_FSDSP, match_opcode, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"fsd",        0, {"D", "C", 0},   "CD,Cl(Cs)",  MATCH_C_FSD, MASK_C_FSD, match_opcode, INSN_ALIAS|INSN_DREF|INSN_8_BYTE },
{"fsd",        0, {"D", 0},   "T,q(s)",  MATCH_FSD, MASK_FSD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"fsd",        0, {"D", 0},   "T,A,s",  0, (int) M_FSD, match_never, INSN_MACRO },
{"fmv.d",      0, {"D", 0},   "D,U",  MATCH_FSGNJ_D, MASK_FSGNJ_D, match_rs1_eq_rs2, INSN_ALIAS },
{"fneg.d",     0, {"D", 0},   "D,U",  MATCH_FSGNJN_D, MASK_FSGNJN_D, match_rs1_eq_rs2, INSN_ALIAS },
{"fabs.d",     0, {"D", 0},   "D,U",  MATCH_FSGNJX_D, MASK_FSGNJX_D, match_rs1_eq_rs2, INSN_ALIAS },
{"fsgnj.d",    0, {"D", 0},   "D,S,T",  MATCH_FSGNJ_D, MASK_FSGNJ_D, match_opcode, 0 },
{"fsgnjn.d",   0, {"D", 0},   "D,S,T",  MATCH_FSGNJN_D, MASK_FSGNJN_D, match_opcode, 0 },
{"fsgnjx.d",   0, {"D", 0},   "D,S,T",  MATCH_FSGNJX_D, MASK_FSGNJX_D, match_opcode, 0 },
{"fadd.d",     0, {"D", 0},   "D,S,T",  MATCH_FADD_D | MASK_RM, MASK_FADD_D | MASK_RM, match_opcode, 0 },
{"fadd.d",     0, {"D", 0},   "D,S,T,m",  MATCH_FADD_D, MASK_FADD_D, match_opcode, 0 },
{"fsub.d",     0, {"D", 0},   "D,S,T",  MATCH_FSUB_D | MASK_RM, MASK_FSUB_D | MASK_RM, match_opcode, 0 },
{"fsub.d",     0, {"D", 0},   "D,S,T,m",  MATCH_FSUB_D, MASK_FSUB_D, match_opcode, 0 },
{"fmul.d",     0, {"D", 0},   "D,S,T",  MATCH_FMUL_D | MASK_RM, MASK_FMUL_D | MASK_RM, match_opcode, 0 },
{"fmul.d",     0, {"D", 0},   "D,S,T,m",  MATCH_FMUL_D, MASK_FMUL_D, match_opcode, 0 },
{"fdiv.d",     0, {"D", 0},   "D,S,T",  MATCH_FDIV_D | MASK_RM, MASK_FDIV_D | MASK_RM, match_opcode, 0 },
{"fdiv.d",     0, {"D", 0},   "D,S,T,m",  MATCH_FDIV_D, MASK_FDIV_D, match_opcode, 0 },
{"fsqrt.d",    0, {"D", 0},   "D,S",  MATCH_FSQRT_D | MASK_RM, MASK_FSQRT_D | MASK_RM, match_opcode, 0 },
{"fsqrt.d",    0, {"D", 0},   "D,S,m",  MATCH_FSQRT_D, MASK_FSQRT_D, match_opcode, 0 },
{"fmin.d",     0, {"D", 0},   "D,S,T",  MATCH_FMIN_D, MASK_FMIN_D, match_opcode, 0 },
{"fmax.d",     0, {"D", 0},   "D,S,T",  MATCH_FMAX_D, MASK_FMAX_D, match_opcode, 0 },
{"fmadd.d",    0, {"D", 0},   "D,S,T,R",  MATCH_FMADD_D | MASK_RM, MASK_FMADD_D | MASK_RM, match_opcode, 0 },
{"fmadd.d",    0, {"D", 0},   "D,S,T,R,m",  MATCH_FMADD_D, MASK_FMADD_D, match_opcode, 0 },
{"fnmadd.d",   0, {"D", 0},   "D,S,T,R",  MATCH_FNMADD_D | MASK_RM, MASK_FNMADD_D | MASK_RM, match_opcode, 0 },
{"fnmadd.d",   0, {"D", 0},   "D,S,T,R,m",  MATCH_FNMADD_D, MASK_FNMADD_D, match_opcode, 0 },
{"fmsub.d",    0, {"D", 0},   "D,S,T,R",  MATCH_FMSUB_D | MASK_RM, MASK_FMSUB_D | MASK_RM, match_opcode, 0 },
{"fmsub.d",    0, {"D", 0},   "D,S,T,R,m",  MATCH_FMSUB_D, MASK_FMSUB_D, match_opcode, 0 },
{"fnmsub.d",   0, {"D", 0},   "D,S,T,R",  MATCH_FNMSUB_D | MASK_RM, MASK_FNMSUB_D | MASK_RM, match_opcode, 0 },
{"fnmsub.d",   0, {"D", 0},   "D,S,T,R,m",  MATCH_FNMSUB_D, MASK_FNMSUB_D, match_opcode, 0 },
{"fcvt.w.d",   0, {"D", 0},   "d,S",  MATCH_FCVT_W_D | MASK_RM, MASK_FCVT_W_D | MASK_RM, match_opcode, 0 },
{"fcvt.w.d",   0, {"D", 0},   "d,S,m",  MATCH_FCVT_W_D, MASK_FCVT_W_D, match_opcode, 0 },
{"fcvt.wu.d",  0, {"D", 0},   "d,S",  MATCH_FCVT_WU_D | MASK_RM, MASK_FCVT_WU_D | MASK_RM, match_opcode, 0 },
{"fcvt.wu.d",  0, {"D", 0},   "d,S,m",  MATCH_FCVT_WU_D, MASK_FCVT_WU_D, match_opcode, 0 },
{"fcvt.d.w",   0, {"D", 0},   "D,s",  MATCH_FCVT_D_W, MASK_FCVT_D_W | MASK_RM, match_opcode, 0 },
{"fcvt.d.wu",  0, {"D", 0},   "D,s",  MATCH_FCVT_D_WU, MASK_FCVT_D_WU | MASK_RM, match_opcode, 0 },
{"fcvt.d.s",   0, {"D", 0},   "D,S",  MATCH_FCVT_D_S, MASK_FCVT_D_S | MASK_RM, match_opcode, 0 },
{"fcvt.s.d",   0, {"D", 0},   "D,S",  MATCH_FCVT_S_D | MASK_RM, MASK_FCVT_S_D | MASK_RM, match_opcode, 0 },
{"fcvt.s.d",   0, {"D", 0},   "D,S,m",  MATCH_FCVT_S_D, MASK_FCVT_S_D, match_opcode, 0 },
{"fclass.d",   0, {"D", 0},   "d,S",  MATCH_FCLASS_D, MASK_FCLASS_D, match_opcode, 0 },
{"feq.d",      0, {"D", 0},   "d,S,T",    MATCH_FEQ_D, MASK_FEQ_D, match_opcode, 0 },
{"flt.d",      0, {"D", 0},   "d,S,T",    MATCH_FLT_D, MASK_FLT_D, match_opcode, 0 },
{"fle.d",      0, {"D", 0},   "d,S,T",    MATCH_FLE_D, MASK_FLE_D, match_opcode, 0 },
{"fgt.d",      0, {"D", 0},   "d,T,S",    MATCH_FLT_D, MASK_FLT_D, match_opcode, 0 },
{"fge.d",      0, {"D", 0},   "d,T,S",    MATCH_FLE_D, MASK_FLE_D, match_opcode, 0 },
{"fmv.x.d",   64, {"D", 0}, "d,S",  MATCH_FMV_X_D, MASK_FMV_X_D, match_opcode, 0 },
{"fmv.d.x",   64, {"D", 0}, "D,s",  MATCH_FMV_D_X, MASK_FMV_D_X, match_opcode, 0 },
{"fcvt.l.d",  64, {"D", 0}, "d,S",  MATCH_FCVT_L_D | MASK_RM, MASK_FCVT_L_D | MASK_RM, match_opcode, 0 },
{"fcvt.l.d",  64, {"D", 0}, "d,S,m",  MATCH_FCVT_L_D, MASK_FCVT_L_D, match_opcode, 0 },
{"fcvt.lu.d", 64, {"D", 0}, "d,S",  MATCH_FCVT_LU_D | MASK_RM, MASK_FCVT_LU_D | MASK_RM, match_opcode, 0 },
{"fcvt.lu.d", 64, {"D", 0}, "d,S,m",  MATCH_FCVT_LU_D, MASK_FCVT_LU_D, match_opcode, 0 },
{"fcvt.d.l",  64, {"D", 0}, "D,s",  MATCH_FCVT_D_L | MASK_RM, MASK_FCVT_D_L | MASK_RM, match_opcode, 0 },
{"fcvt.d.l",  64, {"D", 0}, "D,s,m",  MATCH_FCVT_D_L, MASK_FCVT_D_L, match_opcode, 0 },
{"fcvt.d.lu", 64, {"D", 0}, "D,s",  MATCH_FCVT_D_LU | MASK_RM, MASK_FCVT_D_L | MASK_RM, match_opcode, 0 },
{"fcvt.d.lu", 64, {"D", 0}, "D,s,m",  MATCH_FCVT_D_LU, MASK_FCVT_D_LU, match_opcode, 0 },

/* Quad-precision floating-point instruction subset */
{"flq",        0, {"Q", 0},   "D,o(s)",  MATCH_FLQ, MASK_FLQ, match_opcode, INSN_DREF|INSN_16_BYTE },
{"flq",        0, {"Q", 0},   "D,A,s",  0, (int) M_FLQ, match_never, INSN_MACRO },
{"fsq",        0, {"Q", 0},   "T,q(s)",  MATCH_FSQ, MASK_FSQ, match_opcode, INSN_DREF|INSN_16_BYTE },
{"fsq",        0, {"Q", 0},   "T,A,s",  0, (int) M_FSQ, match_never, INSN_MACRO },
{"fmv.q",      0, {"Q", 0},   "D,U",  MATCH_FSGNJ_Q, MASK_FSGNJ_Q, match_rs1_eq_rs2, INSN_ALIAS },
{"fneg.q",     0, {"Q", 0},   "D,U",  MATCH_FSGNJN_Q, MASK_FSGNJN_Q, match_rs1_eq_rs2, INSN_ALIAS },
{"fabs.q",     0, {"Q", 0},   "D,U",  MATCH_FSGNJX_Q, MASK_FSGNJX_Q, match_rs1_eq_rs2, INSN_ALIAS },
{"fsgnj.q",    0, {"Q", 0},   "D,S,T",  MATCH_FSGNJ_Q, MASK_FSGNJ_Q, match_opcode, 0 },
{"fsgnjn.q",   0, {"Q", 0},   "D,S,T",  MATCH_FSGNJN_Q, MASK_FSGNJN_Q, match_opcode, 0 },
{"fsgnjx.q",   0, {"Q", 0},   "D,S,T",  MATCH_FSGNJX_Q, MASK_FSGNJX_Q, match_opcode, 0 },
{"fadd.q",     0, {"Q", 0},   "D,S,T",  MATCH_FADD_Q | MASK_RM, MASK_FADD_Q | MASK_RM, match_opcode, 0 },
{"fadd.q",     0, {"Q", 0},   "D,S,T,m",  MATCH_FADD_Q, MASK_FADD_Q, match_opcode, 0 },
{"fsub.q",     0, {"Q", 0},   "D,S,T",  MATCH_FSUB_Q | MASK_RM, MASK_FSUB_Q | MASK_RM, match_opcode, 0 },
{"fsub.q",     0, {"Q", 0},   "D,S,T,m",  MATCH_FSUB_Q, MASK_FSUB_Q, match_opcode, 0 },
{"fmul.q",     0, {"Q", 0},   "D,S,T",  MATCH_FMUL_Q | MASK_RM, MASK_FMUL_Q | MASK_RM, match_opcode, 0 },
{"fmul.q",     0, {"Q", 0},   "D,S,T,m",  MATCH_FMUL_Q, MASK_FMUL_Q, match_opcode, 0 },
{"fdiv.q",     0, {"Q", 0},   "D,S,T",  MATCH_FDIV_Q | MASK_RM, MASK_FDIV_Q | MASK_RM, match_opcode, 0 },
{"fdiv.q",     0, {"Q", 0},   "D,S,T,m",  MATCH_FDIV_Q, MASK_FDIV_Q, match_opcode, 0 },
{"fsqrt.q",    0, {"Q", 0},   "D,S",  MATCH_FSQRT_Q | MASK_RM, MASK_FSQRT_Q | MASK_RM, match_opcode, 0 },
{"fsqrt.q",    0, {"Q", 0},   "D,S,m",  MATCH_FSQRT_Q, MASK_FSQRT_Q, match_opcode, 0 },
{"fmin.q",     0, {"Q", 0},   "D,S,T",  MATCH_FMIN_Q, MASK_FMIN_Q, match_opcode, 0 },
{"fmax.q",     0, {"Q", 0},   "D,S,T",  MATCH_FMAX_Q, MASK_FMAX_Q, match_opcode, 0 },
{"fmadd.q",    0, {"Q", 0},   "D,S,T,R",  MATCH_FMADD_Q | MASK_RM, MASK_FMADD_Q | MASK_RM, match_opcode, 0 },
{"fmadd.q",    0, {"Q", 0},   "D,S,T,R,m",  MATCH_FMADD_Q, MASK_FMADD_Q, match_opcode, 0 },
{"fnmadd.q",   0, {"Q", 0},   "D,S,T,R",  MATCH_FNMADD_Q | MASK_RM, MASK_FNMADD_Q | MASK_RM, match_opcode, 0 },
{"fnmadd.q",   0, {"Q", 0},   "D,S,T,R,m",  MATCH_FNMADD_Q, MASK_FNMADD_Q, match_opcode, 0 },
{"fmsub.q",    0, {"Q", 0},   "D,S,T,R",  MATCH_FMSUB_Q | MASK_RM, MASK_FMSUB_Q | MASK_RM, match_opcode, 0 },
{"fmsub.q",    0, {"Q", 0},   "D,S,T,R,m",  MATCH_FMSUB_Q, MASK_FMSUB_Q, match_opcode, 0 },
{"fnmsub.q",   0, {"Q", 0},   "D,S,T,R",  MATCH_FNMSUB_Q | MASK_RM, MASK_FNMSUB_Q | MASK_RM, match_opcode, 0 },
{"fnmsub.q",   0, {"Q", 0},   "D,S,T,R,m",  MATCH_FNMSUB_Q, MASK_FNMSUB_Q, match_opcode, 0 },
{"fcvt.w.q",   0, {"Q", 0},   "d,S",  MATCH_FCVT_W_Q | MASK_RM, MASK_FCVT_W_Q | MASK_RM, match_opcode, 0 },
{"fcvt.w.q",   0, {"Q", 0},   "d,S,m",  MATCH_FCVT_W_Q, MASK_FCVT_W_Q, match_opcode, 0 },
{"fcvt.wu.q",  0, {"Q", 0},   "d,S",  MATCH_FCVT_WU_Q | MASK_RM, MASK_FCVT_WU_Q | MASK_RM, match_opcode, 0 },
{"fcvt.wu.q",  0, {"Q", 0},   "d,S,m",  MATCH_FCVT_WU_Q, MASK_FCVT_WU_Q, match_opcode, 0 },
{"fcvt.q.w",   0, {"Q", 0},   "D,s",  MATCH_FCVT_Q_W, MASK_FCVT_Q_W | MASK_RM, match_opcode, 0 },
{"fcvt.q.wu",  0, {"Q", 0},   "D,s",  MATCH_FCVT_Q_WU, MASK_FCVT_Q_WU | MASK_RM, match_opcode, 0 },
{"fcvt.q.s",   0, {"Q", 0},   "D,S",  MATCH_FCVT_Q_S, MASK_FCVT_Q_S | MASK_RM, match_opcode, 0 },
{"fcvt.q.d",   0, {"Q", 0},   "D,S",  MATCH_FCVT_Q_D, MASK_FCVT_Q_D | MASK_RM, match_opcode, 0 },
{"fcvt.s.q",   0, {"Q", 0},   "D,S",  MATCH_FCVT_S_Q | MASK_RM, MASK_FCVT_S_Q | MASK_RM, match_opcode, 0 },
{"fcvt.s.q",   0, {"Q", 0},   "D,S,m",  MATCH_FCVT_S_Q, MASK_FCVT_S_Q, match_opcode, 0 },
{"fcvt.d.q",   0, {"Q", 0},   "D,S",  MATCH_FCVT_D_Q | MASK_RM, MASK_FCVT_D_Q | MASK_RM, match_opcode, 0 },
{"fcvt.d.q",   0, {"Q", 0},   "D,S,m",  MATCH_FCVT_D_Q, MASK_FCVT_D_Q, match_opcode, 0 },
{"fclass.q",   0, {"Q", 0},   "d,S",  MATCH_FCLASS_Q, MASK_FCLASS_Q, match_opcode, 0 },
{"feq.q",      0, {"Q", 0},   "d,S,T",    MATCH_FEQ_Q, MASK_FEQ_Q, match_opcode, 0 },
{"flt.q",      0, {"Q", 0},   "d,S,T",    MATCH_FLT_Q, MASK_FLT_Q, match_opcode, 0 },
{"fle.q",      0, {"Q", 0},   "d,S,T",    MATCH_FLE_Q, MASK_FLE_Q, match_opcode, 0 },
{"fgt.q",      0, {"Q", 0},   "d,T,S",    MATCH_FLT_Q, MASK_FLT_Q, match_opcode, 0 },
{"fge.q",      0, {"Q", 0},   "d,T,S",    MATCH_FLE_Q, MASK_FLE_Q, match_opcode, 0 },
{"fmv.x.q",   64, {"Q", 0}, "d,S",  MATCH_FMV_X_Q, MASK_FMV_X_Q, match_opcode, 0 },
{"fmv.q.x",   64, {"Q", 0}, "D,s",  MATCH_FMV_Q_X, MASK_FMV_Q_X, match_opcode, 0 },
{"fcvt.l.q",  64, {"Q", 0}, "d,S",  MATCH_FCVT_L_Q | MASK_RM, MASK_FCVT_L_Q | MASK_RM, match_opcode, 0 },
{"fcvt.l.q",  64, {"Q", 0}, "d,S,m",  MATCH_FCVT_L_Q, MASK_FCVT_L_Q, match_opcode, 0 },
{"fcvt.lu.q", 64, {"Q", 0}, "d,S",  MATCH_FCVT_LU_Q | MASK_RM, MASK_FCVT_LU_Q | MASK_RM, match_opcode, 0 },
{"fcvt.lu.q", 64, {"Q", 0}, "d,S,m",  MATCH_FCVT_LU_Q, MASK_FCVT_LU_Q, match_opcode, 0 },
{"fcvt.q.l",  64, {"Q", 0}, "D,s",  MATCH_FCVT_Q_L | MASK_RM, MASK_FCVT_Q_L | MASK_RM, match_opcode, 0 },
{"fcvt.q.l",  64, {"Q", 0}, "D,s,m",  MATCH_FCVT_Q_L, MASK_FCVT_Q_L, match_opcode, 0 },
{"fcvt.q.lu", 64, {"Q", 0}, "D,s",  MATCH_FCVT_Q_LU | MASK_RM, MASK_FCVT_Q_L | MASK_RM, match_opcode, 0 },
{"fcvt.q.lu", 64, {"Q", 0}, "D,s,m",  MATCH_FCVT_Q_LU, MASK_FCVT_Q_LU, match_opcode, 0 },

/* Compressed instructions.  */
{"c.unimp",    0, {"C", 0},   "",  0, 0xffffU,  match_opcode, 0 },
{"c.ebreak",   0, {"C", 0},   "",  MATCH_C_EBREAK, MASK_C_EBREAK, match_opcode, 0 },
{"c.jr",       0, {"C", 0},   "d",  MATCH_C_JR, MASK_C_JR, match_rd_nonzero, INSN_BRANCH },
{"c.jalr",     0, {"C", 0},   "d",  MATCH_C_JALR, MASK_C_JALR, match_rd_nonzero, INSN_JSR },
{"c.j",        0, {"C", 0},   "Ca",  MATCH_C_J, MASK_C_J, match_opcode, INSN_BRANCH },
{"c.jal",     32, {"C", 0}, "Ca",  MATCH_C_JAL, MASK_C_JAL, match_opcode, INSN_JSR },
{"c.beqz",     0, {"C", 0},   "Cs,Cp",  MATCH_C_BEQZ, MASK_C_BEQZ, match_opcode, INSN_CONDBRANCH },
{"c.bnez",     0, {"C", 0},   "Cs,Cp",  MATCH_C_BNEZ, MASK_C_BNEZ, match_opcode, INSN_CONDBRANCH },
{"c.lwsp",     0, {"C", 0},   "d,Cm(Cc)",  MATCH_C_LWSP, MASK_C_LWSP, match_rd_nonzero, 0 },
{"c.lw",       0, {"C", 0},   "Ct,Ck(Cs)",  MATCH_C_LW, MASK_C_LW, match_opcode, INSN_DREF|INSN_4_BYTE },
{"c.swsp",     0, {"C", 0},   "CV,CM(Cc)",  MATCH_C_SWSP, MASK_C_SWSP, match_opcode, INSN_DREF|INSN_4_BYTE },
{"c.sw",       0, {"C", 0},   "Ct,Ck(Cs)",  MATCH_C_SW, MASK_C_SW, match_opcode, INSN_DREF|INSN_4_BYTE },
{"c.nop",      0, {"C", 0},   "",  MATCH_C_ADDI, 0xffff, match_opcode, INSN_ALIAS },
{"c.nop",      0, {"C", 0},   "Cj",  MATCH_C_ADDI, MASK_C_ADDI | MASK_RD, match_opcode, INSN_ALIAS },
{"c.mv",       0, {"C", 0},   "d,CV",  MATCH_C_MV, MASK_C_MV, match_c_add_with_hint, 0 },
{"c.lui",      0, {"C", 0},   "d,Cu",  MATCH_C_LUI, MASK_C_LUI, match_c_lui_with_hint, 0 },
{"c.li",       0, {"C", 0},   "d,Co",  MATCH_C_LI, MASK_C_LI, match_opcode, 0 },
{"c.addi4spn", 0, {"C", 0},   "Ct,Cc,CK", MATCH_C_ADDI4SPN, MASK_C_ADDI4SPN, match_c_addi4spn, 0 },
{"c.addi16sp", 0, {"C", 0},   "Cc,CL", MATCH_C_ADDI16SP, MASK_C_ADDI16SP, match_c_addi16sp, 0 },
{"c.addi",     0, {"C", 0},   "d,Co",  MATCH_C_ADDI, MASK_C_ADDI, match_opcode, 0 },
{"c.add",      0, {"C", 0},   "d,CV",  MATCH_C_ADD, MASK_C_ADD, match_c_add_with_hint, 0 },
{"c.sub",      0, {"C", 0},   "Cs,Ct",  MATCH_C_SUB, MASK_C_SUB, match_opcode, 0 },
{"c.and",      0, {"C", 0},   "Cs,Ct",  MATCH_C_AND, MASK_C_AND, match_opcode, 0 },
{"c.or",       0, {"C", 0},   "Cs,Ct",  MATCH_C_OR, MASK_C_OR, match_opcode, 0 },
{"c.xor",      0, {"C", 0},   "Cs,Ct",  MATCH_C_XOR, MASK_C_XOR, match_opcode, 0 },
{"c.slli",     0, {"C", 0},   "d,C>",  MATCH_C_SLLI, MASK_C_SLLI, match_c_slli, 0 },
{"c.srli",     0, {"C", 0},   "Cs,C>",  MATCH_C_SRLI, MASK_C_SRLI, match_c_slli, 0 },
{"c.srai",     0, {"C", 0},   "Cs,C>",  MATCH_C_SRAI, MASK_C_SRAI, match_c_slli, 0 },
{"c.slli64",   0, {"C", 0},   "d",  MATCH_C_SLLI64, MASK_C_SLLI64, match_c_slli64, 0 },
{"c.srli64",   0, {"C", 0},   "Cs",  MATCH_C_SRLI64, MASK_C_SRLI64, match_c_slli64, 0 },
{"c.srai64",   0, {"C", 0},   "Cs",  MATCH_C_SRAI64, MASK_C_SRAI64, match_c_slli64, 0 },
{"c.andi",     0, {"C", 0},   "Cs,Co",  MATCH_C_ANDI, MASK_C_ANDI, match_opcode, 0 },
{"c.addiw",   64, {"C", 0}, "d,Co",  MATCH_C_ADDIW, MASK_C_ADDIW, match_rd_nonzero, 0 },
{"c.addw",    64, {"C", 0}, "Cs,Ct",  MATCH_C_ADDW, MASK_C_ADDW, match_opcode, 0 },
{"c.subw",    64, {"C", 0}, "Cs,Ct",  MATCH_C_SUBW, MASK_C_SUBW, match_opcode, 0 },
{"c.ldsp",    64, {"C", 0}, "d,Cn(Cc)",  MATCH_C_LDSP, MASK_C_LDSP, match_rd_nonzero, INSN_DREF|INSN_8_BYTE },
{"c.ld",      64, {"C", 0}, "Ct,Cl(Cs)",  MATCH_C_LD, MASK_C_LD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"c.sdsp",    64, {"C", 0}, "CV,CN(Cc)",  MATCH_C_SDSP, MASK_C_SDSP, match_opcode, INSN_DREF|INSN_8_BYTE },
{"c.sd",      64, {"C", 0}, "Ct,Cl(Cs)",  MATCH_C_SD, MASK_C_SD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"c.fldsp",    0, {"D", "C", 0},   "D,Cn(Cc)",  MATCH_C_FLDSP, MASK_C_FLDSP, match_opcode, INSN_DREF|INSN_8_BYTE },
{"c.fld",      0, {"D", "C", 0},   "CD,Cl(Cs)",  MATCH_C_FLD, MASK_C_FLD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"c.fsdsp",    0, {"D", "C", 0},   "CT,CN(Cc)",  MATCH_C_FSDSP, MASK_C_FSDSP, match_opcode, INSN_DREF|INSN_8_BYTE },
{"c.fsd",      0, {"D", "C", 0},   "CD,Cl(Cs)",  MATCH_C_FSD, MASK_C_FSD, match_opcode, INSN_DREF|INSN_8_BYTE },
{"c.flwsp",   32, {"F", "C", 0}, "D,Cm(Cc)",  MATCH_C_FLWSP, MASK_C_FLWSP, match_opcode, INSN_DREF|INSN_4_BYTE },
{"c.flw",     32, {"F", "C", 0}, "CD,Ck(Cs)",  MATCH_C_FLW, MASK_C_FLW, match_opcode, INSN_DREF|INSN_4_BYTE },
{"c.fswsp",   32, {"F", "C", 0}, "CT,CM(Cc)",  MATCH_C_FSWSP, MASK_C_FSWSP, match_opcode, INSN_DREF|INSN_4_BYTE },
{"c.fsw",     32, {"F", "C", 0}, "CD,Ck(Cs)",  MATCH_C_FSW, MASK_C_FSW, match_opcode, INSN_DREF|INSN_4_BYTE },

/* Supervisor instructions */
{"csrr",       0, {"I", 0},   "d,E",  MATCH_CSRRS, MASK_CSRRS | MASK_RS1, match_opcode, INSN_ALIAS },
{"csrwi",      0, {"I", 0},   "E,Z",  MATCH_CSRRWI, MASK_CSRRWI | MASK_RD, match_opcode, INSN_ALIAS },
{"csrsi",      0, {"I", 0},   "E,Z",  MATCH_CSRRSI, MASK_CSRRSI | MASK_RD, match_opcode, INSN_ALIAS },
{"csrci",      0, {"I", 0},   "E,Z",  MATCH_CSRRCI, MASK_CSRRCI | MASK_RD, match_opcode, INSN_ALIAS },
{"csrw",       0, {"I", 0},   "E,s",  MATCH_CSRRW, MASK_CSRRW | MASK_RD, match_opcode, INSN_ALIAS },
{"csrw",       0, {"I", 0},   "E,Z",  MATCH_CSRRWI, MASK_CSRRWI | MASK_RD, match_opcode, INSN_ALIAS },
{"csrs",       0, {"I", 0},   "E,s",  MATCH_CSRRS, MASK_CSRRS | MASK_RD, match_opcode, INSN_ALIAS },
{"csrs",       0, {"I", 0},   "E,Z",  MATCH_CSRRSI, MASK_CSRRSI | MASK_RD, match_opcode, INSN_ALIAS },
{"csrc",       0, {"I", 0},   "E,s",  MATCH_CSRRC, MASK_CSRRC | MASK_RD, match_opcode, INSN_ALIAS },
{"csrc",       0, {"I", 0},   "E,Z",  MATCH_CSRRCI, MASK_CSRRCI | MASK_RD, match_opcode, INSN_ALIAS },
{"csrrwi",     0, {"I", 0},   "d,E,Z",  MATCH_CSRRWI, MASK_CSRRWI, match_opcode, 0 },
{"csrrsi",     0, {"I", 0},   "d,E,Z",  MATCH_CSRRSI, MASK_CSRRSI, match_opcode, 0 },
{"csrrci",     0, {"I", 0},   "d,E,Z",  MATCH_CSRRCI, MASK_CSRRCI, match_opcode, 0 },
{"csrrw",      0, {"I", 0},   "d,E,s",  MATCH_CSRRW, MASK_CSRRW, match_opcode, 0 },
{"csrrw",      0, {"I", 0},   "d,E,Z",  MATCH_CSRRWI, MASK_CSRRWI, match_opcode, INSN_ALIAS },
{"csrrs",      0, {"I", 0},   "d,E,s",  MATCH_CSRRS, MASK_CSRRS, match_opcode, 0 },
{"csrrs",      0, {"I", 0},   "d,E,Z",  MATCH_CSRRSI, MASK_CSRRSI, match_opcode, INSN_ALIAS },
{"csrrc",      0, {"I", 0},   "d,E,s",  MATCH_CSRRC, MASK_CSRRC, match_opcode, 0 },
{"csrrc",      0, {"I", 0},   "d,E,Z",  MATCH_CSRRCI, MASK_CSRRCI, match_opcode, INSN_ALIAS },
{"uret",       0, {"I", 0},   "",     MATCH_URET, MASK_URET, match_opcode, 0 },
{"sret",       0, {"I", 0},   "",     MATCH_SRET, MASK_SRET, match_opcode, 0 },
{"hret",       0, {"I", 0},   "",     MATCH_HRET, MASK_HRET, match_opcode, 0 },
{"mret",       0, {"I", 0},   "",     MATCH_MRET, MASK_MRET, match_opcode, 0 },
{"dret",       0, {"I", 0},   "",     MATCH_DRET, MASK_DRET, match_opcode, 0 },
{"sfence.vm",  0, {"I", 0},   "",     MATCH_SFENCE_VM, MASK_SFENCE_VM | MASK_RS1, match_opcode, 0 },
{"sfence.vm",  0, {"I", 0},   "s",    MATCH_SFENCE_VM, MASK_SFENCE_VM, match_opcode, 0 },
{"sfence.vma", 0, {"I", 0},   "",     MATCH_SFENCE_VMA, MASK_SFENCE_VMA | MASK_RS1 | MASK_RS2, match_opcode, INSN_ALIAS },
{"sfence.vma", 0, {"I", 0},   "s",    MATCH_SFENCE_VMA, MASK_SFENCE_VMA | MASK_RS2, match_opcode, INSN_ALIAS },
{"sfence.vma", 0, {"I", 0},   "s,t",  MATCH_SFENCE_VMA, MASK_SFENCE_VMA, match_opcode, 0 },
{"wfi",        0, {"I", 0},   "",     MATCH_WFI, MASK_WFI, match_opcode, 0 },

/* Rocket Custom Coprocessor extension */
{"custom2",    0, {"Xcustom", 0}, "d,s,t,^j", MATCH_CUSTOM2_RD_RS1_RS2, MASK_CUSTOM2_RD_RS1_RS2, match_opcode, 0},
{"custom2",    0, {"Xcustom", 0}, "d,s,^t,^j", MATCH_CUSTOM2_RD_RS1, MASK_CUSTOM2_RD_RS1, match_opcode, 0},
{"custom2",    0, {"Xcustom", 0}, "d,^s,^t,^j", MATCH_CUSTOM2_RD, MASK_CUSTOM2_RD, match_opcode, 0},
{"custom2",    0, {"Xcustom", 0}, "^d,s,t,^j", MATCH_CUSTOM2_RS1_RS2, MASK_CUSTOM2_RS1_RS2, match_opcode, 0},
{"custom2",    0, {"Xcustom", 0}, "^d,s,^t,^j", MATCH_CUSTOM2_RS1, MASK_CUSTOM2_RS1, match_opcode, 0},
{"custom2",    0, {"Xcustom", 0}, "^d,^s,^t,^j", MATCH_CUSTOM2, MASK_CUSTOM2, match_opcode, 0},
{"custom3",    0, {"Xcustom", 0}, "d,s,t,^j", MATCH_CUSTOM3_RD_RS1_RS2, MASK_CUSTOM3_RD_RS1_RS2, match_opcode, 0},
{"custom3",    0, {"Xcustom", 0}, "d,s,^t,^j", MATCH_CUSTOM3_RD_RS1, MASK_CUSTOM3_RD_RS1, match_opcode, 0},
{"custom3",    0, {"Xcustom", 0}, "d,^s,^t,^j", MATCH_CUSTOM3_RD, MASK_CUSTOM3_RD, match_opcode, 0},
{"custom3",    0, {"Xcustom", 0}, "^d,s,t,^j", MATCH_CUSTOM3_RS1_RS2, MASK_CUSTOM3_RS1_RS2, match_opcode, 0},
{"custom3",    0, {"Xcustom", 0}, "^d,s,^t,^j", MATCH_CUSTOM3_RS1, MASK_CUSTOM3_RS1, match_opcode, 0},
{"custom3",    0, {"Xcustom", 0}, "^d,^s,^t,^j", MATCH_CUSTOM3, MASK_CUSTOM3, match_opcode, 0},

/* Xhwacha extension ct instructions*/
{"vsetcfg",    0, {"Xhwacha", 0}, "s", MATCH_VSETCFG, MASK_VSETCFG | MASK_IMM, match_opcode, 0},
{"vsetcfg",    0, {"Xhwacha", 0}, "#g,#f", MATCH_VSETCFG, MASK_VSETCFG | MASK_RS1, match_opcode, 0},
{"vsetcfg",    0, {"Xhwacha", 0}, "s,#g,#f", MATCH_VSETCFG, MASK_VSETCFG, match_opcode, 0},
{"vsetcfg",    0, {"Xhwacha", 0}, "d,#C", 0, (int) M_VSETCFG, match_never, INSN_MACRO},
{"vsetucfg",   0, {"Xhwacha", 0}, "d,u", MATCH_LUI, MASK_LUI, match_opcode, INSN_ALIAS},
{"vsetvl",     0, {"Xhwacha", 0}, "d,s", MATCH_VSETVL, MASK_VSETVL, match_opcode, 0},
{"vgetcfg",    0, {"Xhwacha", 0}, "d", MATCH_VGETCFG, MASK_VGETCFG, match_opcode, 0},
{"vgetvl",     0, {"Xhwacha", 0}, "d", MATCH_VGETVL, MASK_VGETVL, match_opcode, 0},
{"vuncfg",     0, {"Xhwacha", 0}, "", MATCH_VUNCFG, MASK_VUNCFG, match_opcode, 0},

{"vmcs",       0, {"Xhwacha", 0}, "#E,s", MATCH_VMCS, MASK_VMCS, match_opcode, 0},
{"vmca",       0, {"Xhwacha", 0}, "#e,s", MATCH_VMCA, MASK_VMCA, match_opcode, 0},

{"vf",         0, {"Xhwacha", 0}, "q(s)", MATCH_VF, MASK_VF, match_opcode, 0},
{"vf",         0, {"Xhwacha", 0}, "A,s", 0, (int) M_VF, match_never, INSN_MACRO },

{"vxcptcause",   0, {"Xhwacha", 0}, "d", MATCH_VXCPTCAUSE, MASK_VXCPTCAUSE, match_opcode, 0},
{"vxcptaux",     0, {"Xhwacha", 0}, "d", MATCH_VXCPTAUX, MASK_VXCPTAUX, match_opcode, 0},

{"vxcptsave",    0, {"Xhwacha", 0}, "s", MATCH_VXCPTSAVE, MASK_VXCPTSAVE, match_opcode, 0},
{"vxcptrestore", 0, {"Xhwacha", 0}, "s", MATCH_VXCPTRESTORE, MASK_VXCPTRESTORE, match_opcode, 0},
{"vxcptkill",    0, {"Xhwacha", 0}, "", MATCH_VXCPTKILL, MASK_VXCPTKILL, match_opcode, 0},

{"vxcptevac",    0, {"Xhwacha", 0}, "s", MATCH_VXCPTEVAC, MASK_VXCPTEVAC, match_opcode, 0},
{"vxcpthold",    0, {"Xhwacha", 0}, "s", MATCH_VXCPTHOLD, MASK_VXCPTHOLD, match_opcode, 0},
{"venqcmd",      0, {"Xhwacha", 0}, "s,t", MATCH_VENQCMD, MASK_VENQCMD, match_opcode, 0},
{"venqimm1",     0, {"Xhwacha", 0}, "s,t", MATCH_VENQIMM1, MASK_VENQIMM1, match_opcode, 0},
{"venqimm2",     0, {"Xhwacha", 0}, "s,t", MATCH_VENQIMM2, MASK_VENQIMM2, match_opcode, 0},
{"venqcnt",      0, {"Xhwacha", 0}, "s,t", MATCH_VENQCNT, MASK_VENQCNT, match_opcode, 0},

/* Xhwacha extension work thread instructions*/
{"vstop",      0, {"Xhwacha", 0}, "", MATCH_VSTOP, MASK_VSTOP, match_opcode, 0},
{"vfence",     0, {"Xhwacha", 0}, "#G,#H", MATCH_VFENCE, MASK_VFENCE, match_opcode, 0},
{"vfence",     0, {"Xhwacha", 0}, "",  MATCH_VFENCE | MASK_VPREV | MASK_VSUCC, MASK_VFENCE | MASK_VRD,  match_opcode,   INSN_ALIAS },
{"veidx",      0, {"Xhwacha", 0}, "#N,#p,#d", MATCH_VEIDX, MASK_VEIDX | MASK_VRS1 | MASK_VS1, match_opcode, INSN_ALIAS},
{"veidx",      0, {"Xhwacha", 0}, "#N,#p,#w,#d,#s", MATCH_VEIDX, MASK_VEIDX, match_opcode, 0},
{"vfirst",     0, {"Xhwacha", 0}, "#N,#p,#D,#s", MATCH_VFIRST, MASK_VFIRST, match_opcode, 0},

/* work thread control flow instructions */
{"vcjal",      0, { 0, {"Xhwacha", 0}, 0}, "#N,#p,#c,#D,#k", MATCH_VCJAL, MASK_VCJAL, match_opcode, 0},
{"vcjalr",     0, { 0, {"Xhwacha", 0}, 0}, "#N,#p,#c,#D,#S,#k", MATCH_VCJALR, MASK_VCJALR, match_opcode, 0},

/* scalar load upper */
{"vlui",       0, {"Xhwacha", 0}, "#D,#j", MATCH_VLUI, MASK_VLUI, match_opcode, 0},
{"vauipc",     0, {"Xhwacha", 0}, "#D,#j", MATCH_VAUIPC, MASK_VAUIPC, match_opcode, 0},

/* vnop */
{"vnop",       0, {"Xhwacha", 0}, "",  MATCH_VADDI, MASK_VADDI | MASK_VRD | MASK_VRS1 | MASK_VIMM | MASK_VPRED | MASK_VN, match_opcode,  INSN_ALIAS},

/* vector loads */
/* unit stride */
/* xloads */
{"vld",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VLD, MASK_VLD, match_opcode, 0},
{"vlw",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VLW, MASK_VLW, match_opcode, 0},
{"vlwu",       0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VLWU, MASK_VLWU, match_opcode, 0},
{"vlh",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VLH, MASK_VLH, match_opcode, 0},
{"vlhu",       0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VLHU, MASK_VLHU, match_opcode, 0},
{"vlb",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VLB, MASK_VLB, match_opcode, 0},
{"vlbu",       0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VLBU, MASK_VLBU, match_opcode, 0},

/* stride */
/* xloads */
{"vlstd",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VLSTD, MASK_VLSTD, match_opcode, 0},
{"vlstw",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VLSTW, MASK_VLSTW, match_opcode, 0},
{"vlstwu",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VLSTWU, MASK_VLSTWU, match_opcode, 0},
{"vlsth",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VLSTH, MASK_VLSTH, match_opcode, 0},
{"vlsthu",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VLSTHU, MASK_VLSTHU, match_opcode, 0},
{"vlstb",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VLSTB, MASK_VLSTB, match_opcode, 0},
{"vlstbu",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VLSTBU, MASK_VLSTBU, match_opcode, 0},

/* segment */
/* xloads */
{"vlsegd",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VLSEGD, MASK_VLSEGD, match_opcode, 0},
{"vlsegw",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VLSEGW, MASK_VLSEGW, match_opcode, 0},
{"vlsegwu",    0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VLSEGWU, MASK_VLSEGWU, match_opcode, 0},
{"vlsegh",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VLSEGH, MASK_VLSEGH, match_opcode, 0},
{"vlseghu",    0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VLSEGHU, MASK_VLSEGHU, match_opcode, 0},
{"vlsegb",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VLSEGB, MASK_VLSEGB, match_opcode, 0},
{"vlsegbu",    0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VLSEGBU, MASK_VLSEGBU, match_opcode, 0},

/* stride segment */
/* xloads */
{"vlsegstd",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VLSEGSTD, MASK_VLSEGSTD, match_opcode, 0},
{"vlsegstw",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VLSEGSTW, MASK_VLSEGSTW, match_opcode, 0},
{"vlsegstwu",  0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VLSEGSTWU, MASK_VLSEGSTWU, match_opcode, 0},
{"vlsegsth",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VLSEGSTH, MASK_VLSEGSTH, match_opcode, 0},
{"vlsegsthu",  0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VLSEGSTHU, MASK_VLSEGSTHU, match_opcode, 0},
{"vlsegstb",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VLSEGSTB, MASK_VLSEGSTB, match_opcode, 0},
{"vlsegstbu",  0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VLSEGSTBU, MASK_VLSEGSTBU, match_opcode, 0},

/* unit stride */
/* xstores */
{"vsd",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VSD, MASK_VSD, match_opcode, 0},
{"vsw",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VSW, MASK_VSW, match_opcode, 0},
{"vsh",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VSH, MASK_VSH, match_opcode, 0},
{"vsb",        0, {"Xhwacha", 0}, "#N,#p,#d,#A", MATCH_VSB, MASK_VSB, match_opcode, 0},

/* stride */
/* xstores */
{"vsstd",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VSSTD, MASK_VSSTD, match_opcode, 0},
{"vsstw",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VSSTW, MASK_VSSTW, match_opcode, 0},
{"vssth",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VSSTH, MASK_VSSTH, match_opcode, 0},
{"vsstb",      0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B", MATCH_VSSTB, MASK_VSSTB, match_opcode, 0},

/* segment */
/* xstores */
{"vssegd",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VSSEGD, MASK_VSSEGD, match_opcode, 0},
{"vssegw",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VSSEGW, MASK_VSSEGW, match_opcode, 0},
{"vssegh",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VSSEGH, MASK_VSSEGH, match_opcode, 0},
{"vssegb",     0, {"Xhwacha", 0}, "#N,#p,#d,#A,#n", MATCH_VSSEGB, MASK_VSSEGB, match_opcode, 0},

/* stride segment */
/* xsegstores */
{"vssegstd",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VSSEGSTD, MASK_VSSEGSTD, match_opcode, 0},
{"vssegstw",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VSSEGSTW, MASK_VSSEGSTW, match_opcode, 0},
{"vssegsth",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VSSEGSTH, MASK_VSSEGSTH, match_opcode, 0},
{"vssegstb",   0, {"Xhwacha", 0}, "#N,#p,#d,#A,#B,#n", MATCH_VSSEGSTB, MASK_VSSEGSTB, match_opcode, 0},

/* indexed load/stores */
/* xloads */
{"vlxd",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VLXD, MASK_VLXD, match_opcode, 0},
{"vlxw",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VLXW, MASK_VLXW, match_opcode, 0},
{"vlxwu",      0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VLXWU,MASK_VLXWU, match_opcode, 0},
{"vlxh",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VLXH, MASK_VLXH, match_opcode, 0},
{"vlxhu",      0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VLXHU,MASK_VLXHU, match_opcode, 0},
{"vlxb",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VLXB, MASK_VLXB, match_opcode, 0},
{"vlxbu",      0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VLXBU,MASK_VLXBU, match_opcode, 0},
/* xstores */
{"vsxd",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VSXD, MASK_VSXD, match_opcode, 0},
{"vsxw",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VSXW, MASK_VSXW, match_opcode, 0},
{"vsxh",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VSXH, MASK_VSXH, match_opcode, 0},
{"vsxb",       0, {"Xhwacha", 0}, "#N,#p,#d,#S,#t", MATCH_VSXB, MASK_VSXB, match_opcode, 0},

/* shared loads from shared registers */
/* xloads */
{"vlsd",       0, {"Xhwacha", 0}, "#d,#S", MATCH_VLSD, MASK_VLSD, match_opcode, 0},
{"vlsw",       0, {"Xhwacha", 0}, "#d,#S", MATCH_VLSW, MASK_VLSW, match_opcode, 0},
{"vlswu",      0, {"Xhwacha", 0}, "#d,#S", MATCH_VLSWU,MASK_VLSWU, match_opcode, 0},
{"vlsh",       0, {"Xhwacha", 0}, "#d,#S", MATCH_VLSH, MASK_VLSH, match_opcode, 0},
{"vlshu",      0, {"Xhwacha", 0}, "#d,#S", MATCH_VLSHU,MASK_VLSHU, match_opcode, 0},
{"vlsb",       0, {"Xhwacha", 0}, "#d,#S", MATCH_VLSB, MASK_VLSB, match_opcode, 0},
{"vlsbu",      0, {"Xhwacha", 0}, "#d,#S", MATCH_VLSBU,MASK_VLSBU, match_opcode, 0},
/* xstores */
{"vssd",       0, {"Xhwacha", 0}, "#S,#T", MATCH_VSSD, MASK_VSSD, match_opcode, 0},
{"vssw",       0, {"Xhwacha", 0}, "#S,#T", MATCH_VSSW, MASK_VSSW, match_opcode, 0},
{"vssh",       0, {"Xhwacha", 0}, "#S,#T", MATCH_VSSH, MASK_VSSH, match_opcode, 0},
{"vssb",       0, {"Xhwacha", 0}, "#S,#T", MATCH_VSSB, MASK_VSSB, match_opcode, 0},

/* shared loads from address registers */
/* xloads */
{"vlad",       0, {"Xhwacha", 0}, "#d,#A", MATCH_VLAD, MASK_VLAD, match_opcode, 0},
{"vlaw",       0, {"Xhwacha", 0}, "#d,#A", MATCH_VLAW, MASK_VLAW, match_opcode, 0},
{"vlawu",      0, {"Xhwacha", 0}, "#d,#A", MATCH_VLAWU,MASK_VLAWU, match_opcode, 0},
{"vlah",       0, {"Xhwacha", 0}, "#d,#A", MATCH_VLAH, MASK_VLAH, match_opcode, 0},
{"vlahu",      0, {"Xhwacha", 0}, "#d,#A", MATCH_VLAHU,MASK_VLAHU, match_opcode, 0},
{"vlab",       0, {"Xhwacha", 0}, "#d,#A", MATCH_VLAB, MASK_VLAB, match_opcode, 0},
{"vlabu",      0, {"Xhwacha", 0}, "#d,#A", MATCH_VLABU,MASK_VLABU, match_opcode, 0},
/* xstores */
{"vsad",       0, {"Xhwacha", 0}, "#A,#T", MATCH_VSAD, MASK_VSAD, match_opcode, 0},
{"vsaw",       0, {"Xhwacha", 0}, "#A,#T", MATCH_VSAW, MASK_VSAW, match_opcode, 0},
{"vsah",       0, {"Xhwacha", 0}, "#A,#T", MATCH_VSAH, MASK_VSAH, match_opcode, 0},
{"vsab",       0, {"Xhwacha", 0}, "#A,#T", MATCH_VSAB, MASK_VSAB, match_opcode, 0},

/* Vector-vector ut operations */
{"vandi",      0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VANDI, MASK_VANDI, match_opcode,   0 },
{"vand",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VAND, MASK_VAND, match_opcode,   0 },
{"vand",       0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VANDI, MASK_VANDI, match_opcode,   INSN_ALIAS },
{"vaddi",      0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VADDI, MASK_VADDI, match_opcode,  0 },
{"vadd",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VADD, MASK_VADD, match_opcode,  0 },
{"vadd",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,0",MATCH_VADD, MASK_VADD, match_opcode,  0 },
{"vadd",       0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VADDI, MASK_VADDI, match_opcode,  INSN_ALIAS },
{"vslli",      0, {"Xhwacha", 0}, "#d,#s,#>",   MATCH_VSLLI, MASK_VSLLI, match_opcode,   0 },
{"vsll",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",   MATCH_VSLL, MASK_VSLL, match_opcode,   0 },
{"vsll",       0, {"Xhwacha", 0}, "#d,#s,#>",   MATCH_VSLLI, MASK_VSLLI, match_opcode,   INSN_ALIAS },
{"vsrli",      0, {"Xhwacha", 0}, "#d,#s,#>",   MATCH_VSRLI, MASK_VSRLI, match_opcode,   0 },
{"vsrl",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",   MATCH_VSRL, MASK_VSRL, match_opcode,   0 },
{"vsrl",       0, {"Xhwacha", 0}, "#d,#s,#>",   MATCH_VSRLI, MASK_VSRLI, match_opcode,   INSN_ALIAS },
{"vsrai",      0, {"Xhwacha", 0}, "#d,#s,#>",   MATCH_VSRAI, MASK_VSRAI, match_opcode,   0 },
{"vsra",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",   MATCH_VSRA, MASK_VSRA, match_opcode,   0 },
{"vsra",       0, {"Xhwacha", 0}, "#d,#s,#>",   MATCH_VSRAI, MASK_VSRAI, match_opcode,   INSN_ALIAS },
{"vsub",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VSUB, MASK_VSUB, match_opcode,   0 },
{"vori",       0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VORI, MASK_VORI, match_opcode,   0 },
{"vor",        0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VOR, MASK_VOR, match_opcode,   0 },
{"vor",        0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VORI, MASK_VORI, match_opcode,   INSN_ALIAS },
{"vseqz",      0, {"Xhwacha", 0}, "#d,#s",  MATCH_VSLTIU | ENCODE_ITYPE_VIMM(1), MASK_VSLTIU | MASK_VIMM, match_opcode,   INSN_ALIAS },
{"vsnez",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#t",  MATCH_VSLTU, MASK_VSLTU | MASK_VRS1, match_opcode,   INSN_ALIAS },
{"vsltz",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s",  MATCH_VSLT, MASK_VSLT | MASK_VRS2, match_opcode,   INSN_ALIAS },
{"vsgtz",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#t",  MATCH_VSLT, MASK_VSLT | MASK_VRS1, match_opcode,   INSN_ALIAS },
{"vslti",      0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VSLTI, MASK_VSLTI, match_opcode,   INSN_ALIAS },
{"vslt",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VSLT, MASK_VSLT, match_opcode,   0 },
{"vslt",       0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VSLTI, MASK_VSLTI, match_opcode,   0 },
{"vsltiu",     0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VSLTIU, MASK_VSLTIU, match_opcode,   0 },
{"vsltu",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VSLTU, MASK_VSLTU, match_opcode,   0 },
{"vsltu",      0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VSLTIU, MASK_VSLTIU, match_opcode,   INSN_ALIAS },
{"vsgt",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#t,#s",  MATCH_VSLT, MASK_VSLT, match_opcode,   INSN_ALIAS },
{"vsgtu",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#t,#s",  MATCH_VSLTU, MASK_VSLTU, match_opcode,   INSN_ALIAS },
{"vxori",      0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VXORI, MASK_VXORI, match_opcode,   0 },
{"vxor",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VXOR, MASK_VXOR, match_opcode,   0 },
{"vxor",       0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VXORI, MASK_VXORI, match_opcode,   INSN_ALIAS },
{"vsext.w",    0, {"Xhwacha", 0}, "#d,#s",  MATCH_VADDIW, MASK_VADDIW | MASK_VIMM, match_opcode,   INSN_ALIAS },
{"vaddiw",     0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VADDIW, MASK_VADDIW, match_opcode,   0 },
{"vaddw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VADDW, MASK_VADDW, match_opcode,   0 },
{"vaddw",      0, {"Xhwacha", 0}, "#d,#s,#j",  MATCH_VADDIW, MASK_VADDIW, match_opcode,   INSN_ALIAS },
{"vnegw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#t",  MATCH_VSUBW, MASK_VSUBW | MASK_VRS1, match_opcode,   INSN_ALIAS }, /* sub 0 */
{"vslliw",     0, {"Xhwacha", 0}, "#d,#s,#<",   MATCH_VSLLIW, MASK_VSLLIW, match_opcode,   0 },
{"vsllw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",   MATCH_VSLLW, MASK_VSLLW, match_opcode,   0 },
{"vsllw",      0, {"Xhwacha", 0}, "#d,#s,#<",   MATCH_VSLLIW, MASK_VSLLIW, match_opcode,   INSN_ALIAS },
{"vsrliw",     0, {"Xhwacha", 0}, "#d,#s,#<",   MATCH_VSRLIW, MASK_VSRLIW, match_opcode,   0 },
{"vsrlw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",   MATCH_VSRLW, MASK_VSRLW, match_opcode,   0 },
{"vsrlw",      0, {"Xhwacha", 0}, "#d,#s,#<",   MATCH_VSRLIW, MASK_VSRLIW, match_opcode,   INSN_ALIAS },
{"vsraiw",     0, {"Xhwacha", 0}, "#d,#s,#<",   MATCH_VSRAIW, MASK_VSRAIW, match_opcode,   0 },
{"vsraw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",   MATCH_VSRAW, MASK_VSRAW, match_opcode,   0 },
{"vsraw",      0, {"Xhwacha", 0}, "#d,#s,#<",   MATCH_VSRAIW, MASK_VSRAIW, match_opcode,   INSN_ALIAS },
{"vsubw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VSUBW, MASK_VSUBW, match_opcode,   0 },

/* Vector Comparison worker-thread operations */
{"vcmpeq",     0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPEQ, MASK_VCMPEQ, match_opcode,  0 },
{"vcmplt",     0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPLT, MASK_VCMPLT, match_opcode,  0 },
{"vcmpltu",    0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPLTU, MASK_VCMPLTU, match_opcode,  0 },

/* Vector predicate register load/stores */
{"vpl",        0, {"Xhwacha", 0}, "#N,#p,#F,#A", MATCH_VPL, MASK_VPL, match_opcode, 0},
{"vps",        0, {"Xhwacha", 0}, "#N,#p,#P,#A", MATCH_VPS, MASK_VPS, match_opcode, 0},

/* Vector predicate register arithmetic */
{"vpop",       0, {"Xhwacha", 0},   "#F,#O,#P,#Q,#z", MATCH_VPOP, MASK_VPOP, match_opcode, 0 },
{"vpclear",    0, {"Xhwacha", 0},   "#F",    MATCH_VPCLEAR, MASK_VPCLEAR, match_opcode, 0 },
{"vpset",      0, {"Xhwacha", 0},   "#F",    MATCH_VPSET, MASK_VPSET, match_opcode, 0 },
{"vpxorxor",   0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPXORXOR, MASK_VPXORXOR, match_opcode, 0 },
{"vpxorand",   0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPXORAND, MASK_VPXORAND, match_opcode, 0 },
{"vpxoror",    0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPXOROR, MASK_VPXOROR, match_opcode, 0 },
{"vpandxor",   0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPANDXOR, MASK_VPANDXOR, match_opcode, 0 },
{"vpandand",   0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPANDAND, MASK_VPANDAND, match_opcode, 0 },
{"vpandor",    0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPANDOR, MASK_VPANDOR, match_opcode, 0 },
{"vporxor",    0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPORXOR, MASK_VPORXOR, match_opcode, 0 },
{"vporand",    0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPORAND, MASK_VPORAND, match_opcode, 0 },
{"vporor",     0, {"Xhwacha", 0},   "#F,#O,#P,#Q",    MATCH_VPOROR, MASK_VPOROR, match_opcode, 0 },

/* Atomic memory operation instruction subset */
{"vamoadd.w",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_W, MASK_VAMOADD_W | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.w",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_W, MASK_VAMOSWAP_W | MASK_VAQRL, match_opcode,   0 },
{"vamoand.w",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_W, MASK_VAMOAND_W | MASK_VAQRL, match_opcode,   0 },
{"vamoor.w",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_W, MASK_VAMOOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.w",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_W, MASK_VAMOXOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamomax.w",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_W, MASK_VAMOMAX_W | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.w",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_W, MASK_VAMOMAXU_W | MASK_VAQRL, match_opcode,   0 },
{"vamomin.w",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_W, MASK_VAMOMIN_W | MASK_VAQRL, match_opcode,   0 },
{"vamominu.w",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_W, MASK_VAMOMINU_W | MASK_VAQRL, match_opcode,   0 },
{"vamoadd.w.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_W | MASK_VAQ, MASK_VAMOADD_W | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.w.aq",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_W | MASK_VAQ, MASK_VAMOSWAP_W | MASK_VAQRL, match_opcode,   0 },
{"vamoand.w.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_W | MASK_VAQ, MASK_VAMOAND_W | MASK_VAQRL, match_opcode,   0 },
{"vamoor.w.aq",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_W | MASK_VAQ, MASK_VAMOOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.w.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_W | MASK_VAQ, MASK_VAMOXOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamomax.w.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_W | MASK_VAQ, MASK_VAMOMAX_W | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.w.aq",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_W | MASK_VAQ, MASK_VAMOMAXU_W | MASK_VAQRL, match_opcode,   0 },
{"vamomin.w.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_W | MASK_VAQ, MASK_VAMOMIN_W | MASK_VAQRL, match_opcode,   0 },
{"vamominu.w.aq",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_W | MASK_VAQ, MASK_VAMOMINU_W | MASK_VAQRL, match_opcode,   0 },
{"vamoadd.w.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_W | MASK_VRL, MASK_VAMOADD_W | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.w.rl",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_W | MASK_VRL, MASK_VAMOSWAP_W | MASK_VAQRL, match_opcode,   0 },
{"vamoand.w.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_W | MASK_VRL, MASK_VAMOAND_W | MASK_VAQRL, match_opcode,   0 },
{"vamoor.w.rl",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_W | MASK_VRL, MASK_VAMOOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.w.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_W | MASK_VRL, MASK_VAMOXOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamomax.w.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_W | MASK_VRL, MASK_VAMOMAX_W | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.w.rl",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_W | MASK_VRL, MASK_VAMOMAXU_W | MASK_VAQRL, match_opcode,   0 },
{"vamomin.w.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_W | MASK_VRL, MASK_VAMOMIN_W | MASK_VAQRL, match_opcode,   0 },
{"vamominu.w.rl",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_W | MASK_VRL, MASK_VAMOMINU_W | MASK_VAQRL, match_opcode,   0 },
{"vamoadd.w.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_W | MASK_VAQRL, MASK_VAMOADD_W | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.w.sc",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_W | MASK_VAQRL, MASK_VAMOSWAP_W | MASK_VAQRL, match_opcode,   0 },
{"vamoand.w.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_W | MASK_VAQRL, MASK_VAMOAND_W | MASK_VAQRL, match_opcode,   0 },
{"vamoor.w.sc",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_W | MASK_VAQRL, MASK_VAMOOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.w.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_W | MASK_VAQRL, MASK_VAMOXOR_W | MASK_VAQRL, match_opcode,   0 },
{"vamomax.w.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_W | MASK_VAQRL, MASK_VAMOMAX_W | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.w.sc",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_W | MASK_VAQRL, MASK_VAMOMAXU_W | MASK_VAQRL, match_opcode,   0 },
{"vamomin.w.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_W | MASK_VAQRL, MASK_VAMOMIN_W | MASK_VAQRL, match_opcode,   0 },
{"vamominu.w.sc",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_W | MASK_VAQRL, MASK_VAMOMINU_W | MASK_VAQRL, match_opcode,   0 },
{"vamoadd.d",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_D, MASK_VAMOADD_D | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_D, MASK_VAMOSWAP_D | MASK_VAQRL, match_opcode,   0 },
{"vamoand.d",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_D, MASK_VAMOAND_D | MASK_VAQRL, match_opcode,   0 },
{"vamoor.d",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_D, MASK_VAMOOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.d",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_D, MASK_VAMOXOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamomax.d",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_D, MASK_VAMOMAX_D | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_D, MASK_VAMOMAXU_D | MASK_VAQRL, match_opcode,   0 },
{"vamomin.d",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_D, MASK_VAMOMIN_D | MASK_VAQRL, match_opcode,   0 },
{"vamominu.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_D, MASK_VAMOMINU_D | MASK_VAQRL, match_opcode,   0 },
{"vamoadd.d.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_D | MASK_VAQ, MASK_VAMOADD_D | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.d.aq",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_D | MASK_VAQ, MASK_VAMOSWAP_D | MASK_VAQRL, match_opcode,   0 },
{"vamoand.d.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_D | MASK_VAQ, MASK_VAMOAND_D | MASK_VAQRL, match_opcode,   0 },
{"vamoor.d.aq",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_D | MASK_VAQ, MASK_VAMOOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.d.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_D | MASK_VAQ, MASK_VAMOXOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamomax.d.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_D | MASK_VAQ, MASK_VAMOMAX_D | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.d.aq",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_D | MASK_VAQ, MASK_VAMOMAXU_D | MASK_VAQRL, match_opcode,   0 },
{"vamomin.d.aq",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_D | MASK_VAQ, MASK_VAMOMIN_D | MASK_VAQRL, match_opcode,   0 },
{"vamominu.d.aq",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_D | MASK_VAQ, MASK_VAMOMINU_D | MASK_VAQRL, match_opcode,   0 },
{"vamoadd.d.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_D | MASK_VRL, MASK_VAMOADD_D | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.d.rl",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_D | MASK_VRL, MASK_VAMOSWAP_D | MASK_VAQRL, match_opcode,   0 },
{"vamoand.d.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_D | MASK_VRL, MASK_VAMOAND_D | MASK_VAQRL, match_opcode,   0 },
{"vamoor.d.rl",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_D | MASK_VRL, MASK_VAMOOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.d.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_D | MASK_VRL, MASK_VAMOXOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamomax.d.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_D | MASK_VRL, MASK_VAMOMAX_D | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.d.rl",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_D | MASK_VRL, MASK_VAMOMAXU_D | MASK_VAQRL, match_opcode,   0 },
{"vamomin.d.rl",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_D | MASK_VRL, MASK_VAMOMIN_D | MASK_VAQRL, match_opcode,   0 },
{"vamominu.d.rl",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_D | MASK_VRL, MASK_VAMOMINU_D | MASK_VAQRL, match_opcode,   0 },
{"vamoadd.d.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOADD_D | MASK_VAQRL, MASK_VAMOADD_D | MASK_VAQRL, match_opcode,   0 },
{"vamoswap.d.sc",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOSWAP_D | MASK_VAQRL, MASK_VAMOSWAP_D | MASK_VAQRL, match_opcode,   0 },
{"vamoand.d.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOAND_D | MASK_VAQRL, MASK_VAMOAND_D | MASK_VAQRL, match_opcode,   0 },
{"vamoor.d.sc",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOOR_D | MASK_VAQRL, MASK_VAMOOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamoxor.d.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOXOR_D | MASK_VAQRL, MASK_VAMOXOR_D | MASK_VAQRL, match_opcode,   0 },
{"vamomax.d.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAX_D | MASK_VAQRL, MASK_VAMOMAX_D | MASK_VAQRL, match_opcode,   0 },
{"vamomaxu.d.sc",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMAXU_D | MASK_VAQRL, MASK_VAMOMAXU_D | MASK_VAQRL, match_opcode,   0 },
{"vamomin.d.sc",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMIN_D | MASK_VAQRL, MASK_VAMOMIN_D | MASK_VAQRL, match_opcode,   0 },
{"vamominu.d.sc",  0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,0(#s),#t",  MATCH_VAMOMINU_D | MASK_VAQRL, MASK_VAMOMINU_D | MASK_VAQRL, match_opcode,   0 },

/* Multiply/Divide instruction subset */
{"vmul",        0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VMUL, MASK_VMUL, match_opcode,  0 },
{"vmulh",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VMULH, MASK_VMULH, match_opcode,  0 },
{"vmulhu",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VMULHU, MASK_VMULHU, match_opcode,  0 },
{"vmulhsu",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VMULHSU, MASK_VMULHSU, match_opcode,  0 },
{"vdiv",        0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VDIV, MASK_VDIV, match_opcode,  0 },
{"vdivu",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VDIVU, MASK_VDIVU, match_opcode,  0 },
{"vrem",        0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VREM, MASK_VREM, match_opcode,  0 },
{"vremu",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VREMU, MASK_VREMU, match_opcode,  0 },
{"vmulw",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VMULW, MASK_VMULW, match_opcode,  0 },
{"vdivw",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VDIVW, MASK_VDIVW, match_opcode,  0 },
{"vdivuw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VDIVUW, MASK_VDIVUW, match_opcode,  0 },
{"vremw",       0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VREMW, MASK_VREMW, match_opcode,  0 },
{"vremuw",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VREMUW, MASK_VREMUW, match_opcode,  0 },

/* Single-precision floating-point instruction subset */
{"vfmv.s",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#u",  MATCH_VFSGNJ_S, MASK_VFSGNJ_S, match_rs1_eq_rs2,   INSN_ALIAS },
{"vfneg.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#u",  MATCH_VFSGNJN_S, MASK_VFSGNJN_S, match_rs1_eq_rs2,   INSN_ALIAS },
{"vfabs.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#u",  MATCH_VFSGNJX_S, MASK_VFSGNJX_S, match_rs1_eq_rs2,   INSN_ALIAS },
{"vfsgnj.s",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJ_S, MASK_VFSGNJ_S, match_opcode,   0 },
{"vfsgnjn.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJN_S, MASK_VFSGNJN_S, match_opcode,   0 },
{"vfsgnjx.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJX_S, MASK_VFSGNJX_S, match_opcode,   0 },
{"vfadd.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFADD_S | MASK_VRM, MASK_VFADD_S | MASK_VRM, match_opcode,   0 },
{"vfadd.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFADD_S, MASK_VFADD_S, match_opcode,   0 },
{"vfsub.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSUB_S | MASK_VRM, MASK_VFSUB_S | MASK_VRM, match_opcode,   0 },
{"vfsub.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFSUB_S, MASK_VFSUB_S, match_opcode,   0 },
{"vfmul.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMUL_S | MASK_VRM, MASK_VFMUL_S | MASK_VRM, match_opcode,   0 },
{"vfmul.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFMUL_S, MASK_VFMUL_S, match_opcode,   0 },
{"vfdiv.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFDIV_S | MASK_VRM, MASK_VFDIV_S | MASK_VRM, match_opcode,   0 },
{"vfdiv.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFDIV_S, MASK_VFDIV_S, match_opcode,   0 },
{"vfsqrt.s",    0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",  MATCH_VFSQRT_S | MASK_VRM, MASK_VFSQRT_S | MASK_VRM, match_opcode,  0 },
{"vfsqrt.s",    0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFSQRT_S, MASK_VFSQRT_S, match_opcode,  0 },
{"vfmin.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMIN_S, MASK_VFMIN_S, match_opcode,   0 },
{"vfmax.s",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMAX_S, MASK_VFMAX_S, match_opcode,   0 },
{"vfmadd.s",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMADD_S | MASK_VRM, MASK_VFMADD_S | MASK_VRM, match_opcode,   0 },
{"vfmadd.s",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMADD_S, MASK_VFMADD_S, match_opcode,   0 },
{"vfnmadd.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMADD_S | MASK_VRM, MASK_VFNMADD_S | MASK_VRM, match_opcode,   0 },
{"vfnmadd.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMADD_S, MASK_VFNMADD_S, match_opcode,   0 },
{"vfmsub.s",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMSUB_S | MASK_VRM, MASK_VFMSUB_S | MASK_VRM, match_opcode,   0 },
{"vfmsub.s",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMSUB_S, MASK_VFMSUB_S, match_opcode,   0 },
{"vfnmsub.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMSUB_S | MASK_VRM, MASK_VFNMSUB_S | MASK_VRM, match_opcode,   0 },
{"vfnmsub.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMSUB_S, MASK_VFNMSUB_S, match_opcode,   0 },
{"vfclass.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",  MATCH_VFCLASS_S, MASK_VFCLASS_S, match_opcode,   0 },
{"vcmpfeq.s",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFEQ_S, MASK_VCMPFEQ_S, match_opcode,  0 },
{"vcmpflt.s",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFLT_S, MASK_VCMPFLT_S, match_opcode,  0 },
{"vcmpfle.s",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFLE_S, MASK_VCMPFLE_S, match_opcode,  0 },
{"vcmpfgt.s",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#t,#s",    MATCH_VCMPFLT_S, MASK_VCMPFLT_S, match_opcode,  0 },
{"vcmpfge.s",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#t,#s",    MATCH_VCMPFLE_S, MASK_VCMPFLE_S, match_opcode,  0 },
{"vfcvt.w.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_W_S | MASK_VRM, MASK_VFCVT_W_S | MASK_VRM, match_opcode,  0 },
{"vfcvt.w.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_W_S, MASK_VFCVT_W_S, match_opcode,  0 },
{"vfcvt.wu.s",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_WU_S | MASK_VRM, MASK_VFCVT_WU_S | MASK_VRM, match_opcode,  0 },
{"vfcvt.wu.s",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_WU_S, MASK_VFCVT_WU_S, match_opcode,  0 },
{"vfcvt.s.w",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_S_W | MASK_VRM, MASK_VFCVT_S_W | MASK_VRM, match_opcode,   0 },
{"vfcvt.s.w",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_S_W, MASK_VFCVT_S_W, match_opcode,   0 },
{"vfcvt.s.wu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_S_WU | MASK_VRM, MASK_VFCVT_S_W | MASK_VRM, match_opcode,   0 },
{"vfcvt.s.wu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_S_WU, MASK_VFCVT_S_WU, match_opcode,   0 },
{"vfcvt.l.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_L_S | MASK_VRM, MASK_VFCVT_L_S | MASK_VRM, match_opcode,  0 },
{"vfcvt.l.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_L_S, MASK_VFCVT_L_S, match_opcode,  0 },
{"vfcvt.lu.s",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_LU_S | MASK_VRM, MASK_VFCVT_LU_S | MASK_VRM, match_opcode,  0 },
{"vfcvt.lu.s",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_LU_S, MASK_VFCVT_LU_S, match_opcode,  0 },
{"vfcvt.s.l",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_S_L | MASK_VRM, MASK_VFCVT_S_L | MASK_VRM, match_opcode,   0 },
{"vfcvt.s.l",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_S_L, MASK_VFCVT_S_L, match_opcode,   0 },
{"vfcvt.s.lu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_S_LU | MASK_VRM, MASK_VFCVT_S_L | MASK_VRM, match_opcode,   0 },
{"vfcvt.s.lu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_S_LU, MASK_VFCVT_S_LU, match_opcode,   0 },

/* Double-precision floating-point instruction subset */
{"vfmv.d",      0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#u",  MATCH_VFSGNJ_D, MASK_VFSGNJ_D, match_rs1_eq_rs2,   INSN_ALIAS },
{"vfneg.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#u",  MATCH_VFSGNJN_D, MASK_VFSGNJN_D, match_rs1_eq_rs2,   INSN_ALIAS },
{"vfabs.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#u",  MATCH_VFSGNJX_D, MASK_VFSGNJX_D, match_rs1_eq_rs2,   INSN_ALIAS },
{"vfsgnj.d",    0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJ_D, MASK_VFSGNJ_D, match_opcode,   0 },
{"vfsgnjn.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJN_D, MASK_VFSGNJN_D, match_opcode,   0 },
{"vfsgnjx.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJX_D, MASK_VFSGNJX_D, match_opcode,   0 },
{"vfadd.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFADD_D | MASK_VRM, MASK_VFADD_D | MASK_VRM, match_opcode,   0 },
{"vfadd.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFADD_D, MASK_VFADD_D, match_opcode,   0 },
{"vfsub.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSUB_D | MASK_VRM, MASK_VFSUB_D | MASK_VRM, match_opcode,   0 },
{"vfsub.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFSUB_D, MASK_VFSUB_D, match_opcode,   0 },
{"vfmul.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMUL_D | MASK_VRM, MASK_VFMUL_D | MASK_VRM, match_opcode,   0 },
{"vfmul.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFMUL_D, MASK_VFMUL_D, match_opcode,   0 },
{"vfdiv.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFDIV_D | MASK_VRM, MASK_VFDIV_D | MASK_VRM, match_opcode,   0 },
{"vfdiv.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFDIV_D, MASK_VFDIV_D, match_opcode,   0 },
{"vfsqrt.d",    0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",  MATCH_VFSQRT_D | MASK_VRM, MASK_VFSQRT_D | MASK_VRM, match_opcode,  0 },
{"vfsqrt.d",    0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFSQRT_D, MASK_VFSQRT_D, match_opcode,  0 },
{"vfmin.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMIN_D, MASK_VFMIN_D, match_opcode,   0 },
{"vfmax.d",     0, {"Xhwacha", 0}, "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMAX_D, MASK_VFMAX_D, match_opcode,   0 },
{"vfmadd.d",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMADD_D | MASK_VRM, MASK_VFMADD_D | MASK_VRM, match_opcode,   0 },
{"vfmadd.d",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMADD_D, MASK_VFMADD_D, match_opcode,   0 },
{"vfnmadd.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMADD_D | MASK_VRM, MASK_VFNMADD_D | MASK_VRM, match_opcode,   0 },
{"vfnmadd.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMADD_D, MASK_VFNMADD_D, match_opcode,   0 },
{"vfmsub.d",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMSUB_D | MASK_VRM, MASK_VFMSUB_D | MASK_VRM, match_opcode,   0 },
{"vfmsub.d",    0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMSUB_D, MASK_VFMSUB_D, match_opcode,   0 },
{"vfnmsub.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMSUB_D | MASK_VRM, MASK_VFNMSUB_D | MASK_VRM, match_opcode,   0 },
{"vfnmsub.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMSUB_D, MASK_VFNMSUB_D, match_opcode,   0 },
{"vfcvt.d.s",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",  MATCH_VFCVT_D_S, MASK_VFCVT_D_S | MASK_VRM, match_opcode,   0 },
{"vfcvt.s.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",  MATCH_VFCVT_S_D | MASK_VRM, MASK_VFCVT_S_D | MASK_VRM, match_opcode,   0 },
{"vfcvt.s.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_S_D, MASK_VFCVT_S_D, match_opcode,   0 },
{"vfclass.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",  MATCH_VFCLASS_D, MASK_VFCLASS_D, match_opcode,   0 },
{"vcmpfeq.d",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFEQ_D, MASK_VCMPFEQ_D, match_opcode,  0 },
{"vcmpflt.d",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFLT_D, MASK_VCMPFLT_D, match_opcode,  0 },
{"vcmpfle.d",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFLE_D, MASK_VCMPFLE_D, match_opcode,  0 },
{"vcmpfgt.d",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#t,#s",    MATCH_VCMPFLT_D, MASK_VCMPFLT_D, match_opcode,  0 },
{"vcmpfge.d",   0, {"Xhwacha", 0}, "#N,#p,#x,#F,#t,#s",    MATCH_VCMPFLE_D, MASK_VCMPFLE_D, match_opcode,  0 },
{"vfcvt.w.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_W_D | MASK_VRM, MASK_VFCVT_W_D | MASK_VRM, match_opcode,  0 },
{"vfcvt.w.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_W_D, MASK_VFCVT_W_D, match_opcode,  0 },
{"vfcvt.wu.d",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_WU_D | MASK_VRM, MASK_VFCVT_WU_D | MASK_VRM, match_opcode,  0 },
{"vfcvt.wu.d",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_WU_D, MASK_VFCVT_WU_D, match_opcode,  0 },
{"vfcvt.d.w",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_D_W | MASK_VRM, MASK_VFCVT_D_W | MASK_VRM, match_opcode,   0 },
{"vfcvt.d.w",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_D_W, MASK_VFCVT_D_W, match_opcode,   0 },
{"vfcvt.d.wu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_D_WU | MASK_VRM, MASK_VFCVT_D_W | MASK_VRM, match_opcode,   0 },
{"vfcvt.d.wu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_D_WU, MASK_VFCVT_D_WU, match_opcode,   0 },
{"vfcvt.l.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_L_D | MASK_VRM, MASK_VFCVT_L_D | MASK_VRM, match_opcode,  0 },
{"vfcvt.l.d",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_L_D, MASK_VFCVT_L_D, match_opcode,  0 },
{"vfcvt.lu.d",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_LU_D | MASK_VRM, MASK_VFCVT_LU_D | MASK_VRM, match_opcode,  0 },
{"vfcvt.lu.d",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_LU_D, MASK_VFCVT_LU_D, match_opcode,  0 },
{"vfcvt.d.l",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_D_L | MASK_VRM, MASK_VFCVT_D_L | MASK_VRM, match_opcode,   0 },
{"vfcvt.d.l",   0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_D_L, MASK_VFCVT_D_L, match_opcode,   0 },
{"vfcvt.d.lu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_D_LU | MASK_VRM, MASK_VFCVT_D_L | MASK_VRM, match_opcode,   0 },
{"vfcvt.d.lu",  0, {"Xhwacha", 0}, "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_D_LU, MASK_VFCVT_D_LU, match_opcode,   0 },

/* Half-precision floating-point instruction subset */
{"vfsgnj.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJ_H, MASK_VFSGNJ_H, match_opcode,  0 },
{"vfsgnjn.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJN_H, MASK_VFSGNJN_H, match_opcode,  0 },
{"vfsgnjx.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSGNJX_H, MASK_VFSGNJX_H, match_opcode,  0 },
{"vfadd.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFADD_H | MASK_VRM, MASK_VFADD_H | MASK_VRM, match_opcode,  0 },
{"vfadd.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFADD_H, MASK_VFADD_H, match_opcode,  0 },
{"vfsub.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSUB_H | MASK_VRM, MASK_VFSUB_H | MASK_VRM, match_opcode,  0 },
{"vfsub.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFSUB_H, MASK_VFSUB_H, match_opcode,  0 },
{"vfmul.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMUL_H | MASK_VRM, MASK_VFMUL_H | MASK_VRM, match_opcode,  0 },
{"vfmul.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFMUL_H, MASK_VFMUL_H, match_opcode,  0 },
{"vfdiv.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFDIV_H | MASK_VRM, MASK_VFDIV_H | MASK_VRM, match_opcode,  0 },
{"vfdiv.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFDIV_H, MASK_VFDIV_H, match_opcode,  0 },
{"vfsqrt.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",  MATCH_VFSQRT_H | MASK_VRM, MASK_VFSQRT_H | MASK_VRM, match_opcode, 0 },
{"vfsqrt.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFSQRT_H, MASK_VFSQRT_H, match_opcode, 0 },
{"vfmin.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMIN_H, MASK_VFMIN_H, match_opcode,  0 },
{"vfmax.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMAX_H, MASK_VFMAX_H, match_opcode,  0 },
{"vfmadd.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMADD_H | MASK_VRM, MASK_VFMADD_H | MASK_VRM, match_opcode,  0 },
{"vfmadd.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMADD_H, MASK_VFMADD_H, match_opcode,  0 },
{"vfnmadd.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMADD_H | MASK_VRM, MASK_VFNMADD_H | MASK_VRM, match_opcode,  0 },
{"vfnmadd.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMADD_H, MASK_VFNMADD_H, match_opcode,  0 },
{"vfmsub.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMSUB_H | MASK_VRM, MASK_VFMSUB_H | MASK_VRM, match_opcode,  0 },
{"vfmsub.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMSUB_H, MASK_VFMSUB_H, match_opcode,  0 },
{"vfnmsub.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMSUB_H | MASK_VRM, MASK_VFNMSUB_H | MASK_VRM, match_opcode,  0 },
{"vfnmsub.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMSUB_H, MASK_VFNMSUB_H, match_opcode,  0 },
{"vfcvt.s.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",  MATCH_VFCVT_S_H, MASK_VFCVT_S_H | MASK_VRM, match_opcode,  0 },
{"vfcvt.h.s",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",  MATCH_VFCVT_H_S | MASK_VRM, MASK_VFCVT_H_S | MASK_VRM, match_opcode,  0 },
{"vfcvt.h.s",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_H_S, MASK_VFCVT_H_S, match_opcode,  0 },
{"vfcvt.d.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",  MATCH_VFCVT_D_H, MASK_VFCVT_D_H | MASK_VRM, match_opcode,  0 },
{"vfcvt.h.d",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",  MATCH_VFCVT_H_D | MASK_VRM, MASK_VFCVT_H_D | MASK_VRM, match_opcode,  0 },
{"vfcvt.h.d",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_H_D, MASK_VFCVT_H_D, match_opcode,  0 },
{"vcmpfeq.h",   0, {"Xhwacha", 0},   "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFEQ_H, MASK_VCMPFEQ_H, match_opcode, 0 },
{"vcmpflt.h",   0, {"Xhwacha", 0},   "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFLT_H, MASK_VCMPFLT_H, match_opcode, 0 },
{"vcmpfle.h",   0, {"Xhwacha", 0},   "#N,#p,#x,#F,#s,#t",    MATCH_VCMPFLE_H, MASK_VCMPFLE_H, match_opcode, 0 },
{"vcmpfgt.h",   0, {"Xhwacha", 0},   "#N,#p,#x,#F,#t,#s",    MATCH_VCMPFLT_H, MASK_VCMPFLT_H, match_opcode, 0 },
{"vcmpfge.h",   0, {"Xhwacha", 0},   "#N,#p,#x,#F,#t,#s",    MATCH_VCMPFLE_H, MASK_VCMPFLE_H, match_opcode, 0 },
{"vfcvt.w.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_W_H | MASK_VRM, MASK_VFCVT_W_H | MASK_VRM, match_opcode,  0 },
{"vfcvt.w.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_W_H, MASK_VFCVT_W_H, match_opcode,  0 },
{"vfcvt.wu.h",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_WU_H | MASK_VRM, MASK_VFCVT_WU_H | MASK_VRM, match_opcode,  0 },
{"vfcvt.wu.h",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_WU_H, MASK_VFCVT_WU_H, match_opcode,  0 },
{"vfcvt.h.w",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_H_W | MASK_VRM, MASK_VFCVT_H_W | MASK_VRM, match_opcode,   0 },
{"vfcvt.h.w",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_H_W, MASK_VFCVT_H_W, match_opcode,   0 },
{"vfcvt.h.wu",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_H_WU | MASK_VRM, MASK_VFCVT_H_W | MASK_VRM, match_opcode,   0 },
{"vfcvt.h.wu",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_H_WU, MASK_VFCVT_H_WU, match_opcode,   0 },
{"vfcvt.l.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_L_H | MASK_VRM, MASK_VFCVT_L_H | MASK_VRM, match_opcode,  0 },
{"vfcvt.l.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_L_H, MASK_VFCVT_L_H, match_opcode,  0 },
{"vfcvt.lu.h",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_LU_H | MASK_VRM, MASK_VFCVT_LU_H | MASK_VRM, match_opcode,  0 },
{"vfcvt.lu.h",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_LU_H, MASK_VFCVT_LU_H, match_opcode,  0 },
{"vfcvt.h.l",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_H_L | MASK_VRM, MASK_VFCVT_H_L | MASK_VRM, match_opcode,   0 },
{"vfcvt.h.l",   0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_H_L, MASK_VFCVT_H_L, match_opcode,   0 },
{"vfcvt.h.lu",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s",     MATCH_VFCVT_H_LU | MASK_VRM, MASK_VFCVT_H_L | MASK_VRM, match_opcode,   0 },
{"vfcvt.h.lu",  0, {"Xhwacha", 0},   "#N,#p,#v,#w,#d,#s,#m",  MATCH_VFCVT_H_LU, MASK_VFCVT_H_LU, match_opcode,   0 },

/* Mixed-precision floating-point instruction subset */
{"vfadd.s.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFADD_S_H | MASK_VRM, MASK_VFADD_S_H | MASK_VRM, match_opcode,  0 },
{"vfadd.s.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFADD_S_H, MASK_VFADD_S_H, match_opcode,  0 },
{"vfsub.s.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSUB_S_H | MASK_VRM, MASK_VFSUB_S_H | MASK_VRM, match_opcode,  0 },
{"vfsub.s.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFSUB_S_H, MASK_VFSUB_S_H, match_opcode,  0 },
{"vfmul.s.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMUL_S_H | MASK_VRM, MASK_VFMUL_S_H | MASK_VRM, match_opcode,  0 },
{"vfmul.s.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFMUL_S_H, MASK_VFMUL_S_H, match_opcode,  0 },
{"vfadd.d.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFADD_D_H | MASK_VRM, MASK_VFADD_D_H | MASK_VRM, match_opcode,  0 },
{"vfadd.d.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFADD_D_H, MASK_VFADD_D_H, match_opcode,  0 },
{"vfsub.d.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSUB_D_H | MASK_VRM, MASK_VFSUB_D_H | MASK_VRM, match_opcode,  0 },
{"vfsub.d.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFSUB_D_H, MASK_VFSUB_D_H, match_opcode,  0 },
{"vfmul.d.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMUL_D_H | MASK_VRM, MASK_VFMUL_D_H | MASK_VRM, match_opcode,  0 },
{"vfmul.d.h",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFMUL_D_H, MASK_VFMUL_D_H, match_opcode,  0 },
{"vfmadd.d.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMADD_D_H | MASK_VRM, MASK_VFMADD_D_H | MASK_VRM, match_opcode,  0 },
{"vfmadd.d.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMADD_D_H, MASK_VFMADD_D_H, match_opcode,  0 },
{"vfnmadd.d.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMADD_D_H | MASK_VRM, MASK_VFNMADD_D_H | MASK_VRM, match_opcode,  0 },
{"vfnmadd.d.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMADD_D_H, MASK_VFNMADD_D_H, match_opcode,  0 },
{"vfmsub.d.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFMSUB_D_H | MASK_VRM, MASK_VFMSUB_D_H | MASK_VRM, match_opcode,  0 },
{"vfmsub.d.h",    0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFMSUB_D_H, MASK_VFMSUB_D_H, match_opcode,  0 },
{"vfnmsub.d.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r",  MATCH_VFNMSUB_D_H | MASK_VRM, MASK_VFNMSUB_D_H | MASK_VRM, match_opcode,  0 },
{"vfnmsub.d.h",   0, {"Xhwacha", 0},   "#N,#p,#v,#y,#d,#s,#t,#r,#m",  MATCH_VFNMSUB_D_H, MASK_VFNMSUB_D_H, match_opcode,  0 },
{"vfadd.d.s",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFADD_D_S | MASK_VRM, MASK_VFADD_D_S | MASK_VRM, match_opcode,  0 },
{"vfadd.d.s",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFADD_D_S, MASK_VFADD_D_S, match_opcode,  0 },
{"vfsub.d.s",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFSUB_D_S | MASK_VRM, MASK_VFSUB_D_S | MASK_VRM, match_opcode,  0 },
{"vfsub.d.s",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFSUB_D_S, MASK_VFSUB_D_S, match_opcode,  0 },
{"vfmul.d.s",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t",  MATCH_VFMUL_D_S | MASK_VRM, MASK_VFMUL_D_S | MASK_VRM, match_opcode,  0 },
{"vfmul.d.s",     0, {"Xhwacha", 0},   "#N,#p,#v,#x,#d,#s,#t,#m",  MATCH_VFMUL_D_S, MASK_VFMUL_D_S, match_opcode,  0 },

/* Terminate the list.  */
{0, 0, {0}, 0, 0, 0, 0, 0}
};

/* Instruction format for .insn directive.  */
const struct riscv_opcode riscv_insn_types[] =
{
/* name, xlen, isa,          operands, match, mask,    match_func, pinfo.  */
{"r",       0, {"I", 0},  "O4,F3,F7,d,s,t",     0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F7,D,s,t",     0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F7,d,S,t",     0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F7,D,S,t",     0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F7,d,s,T",     0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F7,D,s,T",     0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F7,d,S,T",     0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F7,D,S,T",     0,    0,  match_opcode, 0 },

{"r",       0, {"I", 0},  "O4,F3,F2,d,s,t,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,s,t,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,d,S,t,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,S,t,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,d,s,T,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,s,T,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,d,S,T,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,S,T,r",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,d,s,t,R",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,s,t,R",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,d,S,t,R",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,S,t,R",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,d,s,T,R",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,s,T,R",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,d,S,T,R",   0,    0,  match_opcode, 0 },
{"r",       0, {"I", 0},  "O4,F3,F2,D,S,T,R",   0,    0,  match_opcode, 0 },

{"r4",      0, {"I", 0},  "O4,F3,F2,d,s,t,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,s,t,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,d,S,t,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,S,t,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,d,s,T,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,s,T,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,d,S,T,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,S,T,r",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,d,s,t,R",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,s,t,R",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,d,S,t,R",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,S,t,R",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,d,s,T,R",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,s,T,R",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,d,S,T,R",   0,    0,  match_opcode, 0 },
{"r4",      0, {"I", 0},  "O4,F3,F2,D,S,T,R",   0,    0,  match_opcode, 0 },

{"i",       0, {"I", 0},  "O4,F3,d,s,j",        0,    0,  match_opcode, 0 },
{"i",       0, {"I", 0},  "O4,F3,D,s,j",        0,    0,  match_opcode, 0 },
{"i",       0, {"I", 0},  "O4,F3,d,S,j",        0,    0,  match_opcode, 0 },
{"i",       0, {"I", 0},  "O4,F3,D,S,j",        0,    0,  match_opcode, 0 },

{"i",       0, {"I", 0},  "O4,F3,d,o(s)",       0,    0,  match_opcode, 0 },
{"i",       0, {"I", 0},  "O4,F3,D,o(s)",       0,    0,  match_opcode, 0 },

{"s",       0, {"I", 0},  "O4,F3,t,q(s)",       0,    0,  match_opcode, 0 },
{"s",       0, {"I", 0},  "O4,F3,T,q(s)",       0,    0,  match_opcode, 0 },

{"sb",      0, {"I", 0},  "O4,F3,s,t,p",        0,    0,  match_opcode, 0 },
{"sb",      0, {"I", 0},  "O4,F3,S,t,p",        0,    0,  match_opcode, 0 },
{"sb",      0, {"I", 0},  "O4,F3,s,T,p",        0,    0,  match_opcode, 0 },
{"sb",      0, {"I", 0},  "O4,F3,S,T,p",        0,    0,  match_opcode, 0 },

{"b",      0, {"I", 0},  "O4,F3,s,t,p",        0,    0,  match_opcode, 0 },
{"b",      0, {"I", 0},  "O4,F3,S,t,p",        0,    0,  match_opcode, 0 },
{"b",      0, {"I", 0},  "O4,F3,s,T,p",        0,    0,  match_opcode, 0 },
{"b",      0, {"I", 0},  "O4,F3,S,T,p",        0,    0,  match_opcode, 0 },

{"u",       0, {"I", 0},  "O4,d,u",             0,    0,  match_opcode, 0 },
{"u",       0, {"I", 0},  "O4,D,u",             0,    0,  match_opcode, 0 },

{"uj",      0, {"I", 0},  "O4,d,a",             0,    0,  match_opcode, 0 },
{"uj",      0, {"I", 0},  "O4,D,a",             0,    0,  match_opcode, 0 },

{"j",      0, {"I", 0},  "O4,d,a",             0,    0,  match_opcode, 0 },
{"j",      0, {"I", 0},  "O4,D,a",             0,    0,  match_opcode, 0 },

{"cr",      0, {"C", 0},  "O2,CF4,d,CV",        0,    0,  match_opcode, 0 },
{"cr",      0, {"C", 0},  "O2,CF4,D,CV",        0,    0,  match_opcode, 0 },
{"cr",      0, {"C", 0},  "O2,CF4,d,CT",        0,    0,  match_opcode, 0 },
{"cr",      0, {"C", 0},  "O2,CF4,D,CT",        0,    0,  match_opcode, 0 },

{"ci",      0, {"C", 0},  "O2,CF3,d,Co",        0,    0,  match_opcode, 0 },
{"ci",      0, {"C", 0},  "O2,CF3,D,Co",        0,    0,  match_opcode, 0 },

{"ciw",     0, {"C", 0},  "O2,CF3,Ct,C8",       0,    0,  match_opcode, 0 },
{"ciw",     0, {"C", 0},  "O2,CF3,CD,C8",       0,    0,  match_opcode, 0 },

{"ca",      0, {"C", 0},  "O2,CF6,CF2,Cs,Ct",   0,    0,  match_opcode, 0 },
{"ca",      0, {"C", 0},  "O2,CF6,CF2,CS,Ct",   0,    0,  match_opcode, 0 },
{"ca",      0, {"C", 0},  "O2,CF6,CF2,Cs,CD",   0,    0,  match_opcode, 0 },
{"ca",      0, {"C", 0},  "O2,CF6,CF2,CS,CD",   0,    0,  match_opcode, 0 },

{"cb",      0, {"C", 0},  "O2,CF3,Cs,Cp",       0,    0,  match_opcode, 0 },
{"cb",      0, {"C", 0},  "O2,CF3,CS,Cp",       0,    0,  match_opcode, 0 },

{"cj",      0, {"C", 0},  "O2,CF3,Ca",          0,    0,  match_opcode, 0 },
/* Terminate the list.  */
{0, 0, {0}, 0, 0, 0, 0, 0}
};
