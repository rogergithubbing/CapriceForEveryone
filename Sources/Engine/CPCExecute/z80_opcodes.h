/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2016 by Frédéric Coste

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef Z80_OPCODES_H
#define Z80_OPCODES_H


#undef Z80_ORIGINAL_TIMINGS
//#define Z80_ORIGINAL_TIMINGS
#undef Z80_NOP_TIMINGS
//#define Z80_NOP_TIMINGS
#undef Z80_TEST_TIMINGS
//#define Z80_TEST_TIMINGS

#undef Z80_USE_WS_ADJUST
#define Z80_USE_WS_ADJUST

enum opcodes
{
  /*00*/ nop, ld_bc_word, ld_mbc_a, inc_bc, inc_b, dec_b, ld_b_byte, rlca,
  /*08*/ ex_af_af, add_hl_bc, ld_a_mbc, dec_bc, inc_c, dec_c, ld_c_byte, rrca,
  /*10*/ djnz, ld_de_word, ld_mde_a, inc_de, inc_d, dec_d, ld_d_byte, rla,
  /*18*/ jr, add_hl_de, ld_a_mde, dec_de, inc_e, dec_e, ld_e_byte, rra,
  /*20*/ jr_nz, ld_hl_word, ld_mword_hl, inc_hl, inc_h, dec_h, ld_h_byte, daa,
  /*28*/ jr_z, add_hl_hl, ld_hl_mword, dec_hl, inc_l, dec_l, ld_l_byte, cpl,
  /*30*/ jr_nc, ld_sp_word, ld_mword_a, inc_sp, inc_mhl, dec_mhl, ld_mhl_byte, scf,
  /*38*/ jr_c, add_hl_sp, ld_a_mword, dec_sp, inc_a, dec_a, ld_a_byte, ccf,
  /*40*/ ld_b_b, ld_b_c, ld_b_d, ld_b_e, ld_b_h, ld_b_l, ld_b_mhl, ld_b_a,
  /*48*/ ld_c_b, ld_c_c, ld_c_d, ld_c_e, ld_c_h, ld_c_l, ld_c_mhl, ld_c_a,
  /*50*/ ld_d_b, ld_d_c, ld_d_d, ld_d_e, ld_d_h, ld_d_l, ld_d_mhl, ld_d_a,
  /*58*/ ld_e_b, ld_e_c, ld_e_d, ld_e_e, ld_e_h, ld_e_l, ld_e_mhl, ld_e_a,
  /*60*/ ld_h_b, ld_h_c, ld_h_d, ld_h_e, ld_h_h, ld_h_l, ld_h_mhl, ld_h_a,
  /*68*/ ld_l_b, ld_l_c, ld_l_d, ld_l_e, ld_l_h, ld_l_l, ld_l_mhl, ld_l_a,
  /*70*/ ld_mhl_b, ld_mhl_c, ld_mhl_d, ld_mhl_e, ld_mhl_h, ld_mhl_l, halt, ld_mhl_a,
  /*78*/ ld_a_b, ld_a_c, ld_a_d, ld_a_e, ld_a_h, ld_a_l, ld_a_mhl, ld_a_a,
  /*80*/ add_b, add_c, add_d, add_e, add_h, add_l, add_mhl, add_a,
  /*88*/ adc_b, adc_c, adc_d, adc_e, adc_h, adc_l, adc_mhl, adc_a,
  /*90*/ sub_b, sub_c, sub_d, sub_e, sub_h, sub_l, sub_mhl, sub_a,
  /*98*/ sbc_b, sbc_c, sbc_d, sbc_e, sbc_h, sbc_l, sbc_mhl, sbc_a,
  /*a0*/ and_b, and_c, and_d, and_e, and_h, and_l, and_mhl, and_a,
  /*a8*/ xor_b, xor_c, xor_d, xor_e, xor_h, xor_l, xor_mhl, xor_a,
  /*b0*/ or_b, or_c, or_d, or_e, or_h, or_l, or_mhl, or_a,
  /*b8*/ cp_b, cp_c, cp_d, cp_e, cp_h, cp_l, cp_mhl, cp_a,
  /*c0*/ ret_nz, pop_bc, jp_nz, jp, call_nz, push_bc, add_byte, rst00,
  /*c8*/ ret_z, ret, jp_z, pfx_cb, call_z, call, adc_byte, rst08,
  /*d0*/ ret_nc, pop_de, jp_nc, outa, call_nc, push_de, sub_byte, rst10,
  /*d8*/ ret_c, exx, jp_c, ina, call_c, pfx_dd, sbc_byte, rst18,
  /*e0*/ ret_po, pop_hl, jp_po, ex_msp_hl, call_po, push_hl, and_byte, rst20,
  /*e8*/ ret_pe, ld_pc_hl, jp_pe, ex_de_hl, call_pe, pfx_ed, xor_byte, rst28,
  /*f0*/ ret_p, pop_af, jp_p, di, call_p, push_af, or_byte, rst30,
  /*f8*/ ret_m, ld_sp_hl, jp_m, ei, call_m, pfx_fd, cp_byte, rst38
};

enum CBcodes
{
  /*00*/ rlc_b, rlc_c, rlc_d, rlc_e, rlc_h, rlc_l, rlc_mhl, rlc_a,
  /*08*/ rrc_b, rrc_c, rrc_d, rrc_e, rrc_h, rrc_l, rrc_mhl, rrc_a,
  /*10*/ rl_b, rl_c, rl_d, rl_e, rl_h, rl_l, rl_mhl, rl_a,
  /*18*/ rr_b, rr_c, rr_d, rr_e, rr_h, rr_l, rr_mhl, rr_a,
  /*20*/ sla_b, sla_c, sla_d, sla_e, sla_h, sla_l, sla_mhl, sla_a,
  /*28*/ sra_b, sra_c, sra_d, sra_e, sra_h, sra_l, sra_mhl, sra_a,
  /*30*/ sll_b, sll_c, sll_d, sll_e, sll_h, sll_l, sll_mhl, sll_a,
  /*38*/ srl_b, srl_c, srl_d, srl_e, srl_h, srl_l, srl_mhl, srl_a,
  /*40*/ bit0_b, bit0_c, bit0_d, bit0_e, bit0_h, bit0_l, bit0_mhl, bit0_a,
  /*48*/ bit1_b, bit1_c, bit1_d, bit1_e, bit1_h, bit1_l, bit1_mhl, bit1_a,
  /*50*/ bit2_b, bit2_c, bit2_d, bit2_e, bit2_h, bit2_l, bit2_mhl, bit2_a,
  /*58*/ bit3_b, bit3_c, bit3_d, bit3_e, bit3_h, bit3_l, bit3_mhl, bit3_a,
  /*60*/ bit4_b, bit4_c, bit4_d, bit4_e, bit4_h, bit4_l, bit4_mhl, bit4_a,
  /*68*/ bit5_b, bit5_c, bit5_d, bit5_e, bit5_h, bit5_l, bit5_mhl, bit5_a,
  /*70*/ bit6_b, bit6_c, bit6_d, bit6_e, bit6_h, bit6_l, bit6_mhl, bit6_a,
  /*78*/ bit7_b, bit7_c, bit7_d, bit7_e, bit7_h, bit7_l, bit7_mhl, bit7_a,
  /*80*/ res0_b, res0_c, res0_d, res0_e, res0_h, res0_l, res0_mhl, res0_a,
  /*88*/ res1_b, res1_c, res1_d, res1_e, res1_h, res1_l, res1_mhl, res1_a,
  /*90*/ res2_b, res2_c, res2_d, res2_e, res2_h, res2_l, res2_mhl, res2_a,
  /*98*/ res3_b, res3_c, res3_d, res3_e, res3_h, res3_l, res3_mhl, res3_a,
  /*a0*/ res4_b, res4_c, res4_d, res4_e, res4_h, res4_l, res4_mhl, res4_a,
  /*a8*/ res5_b, res5_c, res5_d, res5_e, res5_h, res5_l, res5_mhl, res5_a,
  /*b0*/ res6_b, res6_c, res6_d, res6_e, res6_h, res6_l, res6_mhl, res6_a,
  /*b8*/ res7_b, res7_c, res7_d, res7_e, res7_h, res7_l, res7_mhl, res7_a,
  /*c0*/ set0_b, set0_c, set0_d, set0_e, set0_h, set0_l, set0_mhl, set0_a,
  /*c8*/ set1_b, set1_c, set1_d, set1_e, set1_h, set1_l, set1_mhl, set1_a,
  /*d0*/ set2_b, set2_c, set2_d, set2_e, set2_h, set2_l, set2_mhl, set2_a,
  /*d8*/ set3_b, set3_c, set3_d, set3_e, set3_h, set3_l, set3_mhl, set3_a,
  /*e0*/ set4_b, set4_c, set4_d, set4_e, set4_h, set4_l, set4_mhl, set4_a,
  /*e8*/ set5_b, set5_c, set5_d, set5_e, set5_h, set5_l, set5_mhl, set5_a,
  /*f0*/ set6_b, set6_c, set6_d, set6_e, set6_h, set6_l, set6_mhl, set6_a,
  /*f8*/ set7_b, set7_c, set7_d, set7_e, set7_h, set7_l, set7_mhl, set7_a
};

enum EDcodes
{
  /*00*/ ed_00, ed_01, ed_02, ed_03, ed_04, ed_05, ed_06, ed_07,
  /*08*/ ed_08, ed_09, ed_0a, ed_0b, ed_0c, ed_0d, ed_0e, ed_0f,
  /*10*/ ed_10, ed_11, ed_12, ed_13, ed_14, ed_15, ed_16, ed_17,
  /*18*/ ed_18, ed_19, ed_1a, ed_1b, ed_1c, ed_1d, ed_1e, ed_1f,
  /*20*/ ed_20, ed_21, ed_22, ed_23, ed_24, ed_25, ed_26, ed_27,
  /*28*/ ed_28, ed_29, ed_2a, ed_2b, ed_2c, ed_2d, ed_2e, ed_2f,
  /*30*/ ed_30, ed_31, ed_32, ed_33, ed_34, ed_35, ed_36, ed_37,
  /*38*/ ed_38, ed_39, ed_3a, ed_3b, ed_3c, ed_3d, ed_3e, ed_3f,
  /*40*/ in_b_c, out_c_b, sbc_hl_bc, ld_EDmword_bc, neg, retn, im_0, ld_i_a,
  /*48*/ in_c_c, out_c_c, adc_hl_bc, ld_EDbc_mword, neg_1, reti, im_0_1, ld_r_a,
  /*50*/ in_d_c, out_c_d, sbc_hl_de, ld_EDmword_de, neg_2, retn_1, im_1, ld_a_i,
  /*58*/ in_e_c, out_c_e, adc_hl_de, ld_EDde_mword, neg_3, reti_1, im_2, ld_a_r,
  /*60*/ in_h_c, out_c_h, sbc_hl_hl, ld_EDmword_hl, neg_4, retn_2, im_0_2, rrd,
  /*68*/ in_l_c, out_c_l, adc_hl_hl, ld_EDhl_mword, neg_5, reti_2, im_0_3, rld,
  /*70*/ in_f_c, out_c_0, sbc_hl_sp, ld_EDmword_sp, neg_6, retn_3, im_1_1, ed_77,
  /*78*/ in_a_c, out_c_a, adc_hl_sp, ld_EDsp_mword, neg_7, reti_3, im_2_1, ed_7f,
  /*80*/ ed_80, ed_81, ed_82, ed_83, ed_84, ed_85, ed_86, ed_87,
  /*88*/ ed_88, ed_89, ed_8a, ed_8b, ed_8c, ed_8d, ed_8e, ed_8f,
  /*90*/ ed_90, ed_91, ed_92, ed_93, ed_94, ed_95, ed_96, ed_97,
  /*98*/ ed_98, ed_99, ed_9a, ed_9b, ed_9c, ed_9d, ed_9e, ed_9f,
  /*a0*/ ldi, cpi, ini, outi, ed_a4, ed_a5, ed_a6, ed_a7,
  /*a8*/ ldd, cpd, ind, outd, ed_ac, ed_ad, ed_ae, ed_af,
  /*b0*/ ldir, cpir, inir, otir, ed_b4, ed_b5, ed_b6, ed_b7,
  /*b8*/ lddr, cpdr, indr, otdr, ed_bc, ed_bd, ed_be, ed_bf,
  /*c0*/ ed_c0, ed_c1, ed_c2, ed_c3, ed_c4, ed_c5, ed_c6, ed_c7,
  /*c8*/ ed_c8, ed_c9, ed_ca, ed_cb, ed_cc, ed_cd, ed_ce, ed_cf,
  /*d0*/ ed_d0, ed_d1, ed_d2, ed_d3, ed_d4, ed_d5, ed_d6, ed_d7,
  /*d8*/ ed_d8, ed_d9, ed_da, ed_db, ed_dc, ed_dd, ed_de, ed_df,
  /*e0*/ ed_e0, ed_e1, ed_e2, ed_e3, ed_e4, ed_e5, ed_e6, ed_e7,
  /*e8*/ ed_e8, ed_e9, ed_ea, ed_eb, ed_ec, ed_ed, ed_ee, ed_ef,
  /*f0*/ ed_f0, ed_f1, ed_f2, ed_f3, ed_f4, ed_f5, ed_f6, ed_f7,
  /*f8*/ ed_f8, ed_f9, ed_fa, ed_fb, ed_fc, ed_fd, ed_fe, ed_ff
};


//+--------------------------------------------------------------+
//|                                                              |
//|                            Zilog                             |
//|                                                              |
//|                 ZZZZZZZ    88888      000                    |
//|                      Z    8     8    0   0                   |
//|                     Z     8     8   0   0 0                  |
//|                    Z       88888    0  0  0                  |
//|                   Z       8     8   0 0   0                  |
//|                  Z        8     8    0   0                   |
//|                 ZZZZZZZ    88888      000                    |
//|                                                              |
//|          Z80 MICROPROCESSOR Instruction Set Summary          |
//|                                                              |
//+--------------------------------------------------------------+


//|----------+------+--------------------------------------------|
//| F        |-*01? |Flag unaffected/affected/reset/set/unknown  |
//| S        |S     |Sign flag (Bit 7)                           |
//| Z        | Z    |Zero flag (Bit 6)                           |
//| HC       |  H   |Half Carry flag (Bit 4)                     |
//| P/V      |   P  |Parity/Overflow flag (Bit 2, V=overflow)    |
//| N        |    N |Add/Subtract flag (Bit 1)                   |
//| CY       |     C|Carry flag (Bit 0)                          |
//|-----------------+--------------------------------------------|
//| n               |Immediate addressing                        |
//| nn              |Immediate extended addressing               |
//| e               |Relative addressing (PC=PC+2+offset)        |
//| [nn]            |Extended addressing                         |
//| [xx+d]          |Indexed addressing                          |
//| r               |Register addressing                         |
//| [rr]            |Register indirect addressing                |
//|                 |Implied addressing                          |
//| b               |Bit addressing                              |
//| p               |Modified page zero addressing (see RST)     |
//|-----------------+--------------------------------------------|
//|-----------------+--------------------------------------------|
//| A  B  C  D  E   |Registers (8-bit)                           |
//| AF  BC  DE  HL  |Register pairs (16-bit)                     |
//| F               |Flag register (8-bit)                       |
//| I               |Interrupt page address register (8-bit)     |
//| IX IY           |Index registers (16-bit)                    |
//| PC              |Program Counter register (16-bit)           |
//| R               |Memory Refresh register                     |
//| SP              |Stack Pointer register (16-bit)             |
//|-----------------+--------------------------------------------|
//| b               |One bit (0 to 7)                            |
//| cc              |Condition (C,M,NC,NZ,P,PE,PO,Z)             |
//| d               |One-byte expression (-128 to +127)          |
//| dst             |Destination s, ss, [BC], [DE], [HL], [nn]   |
//| e               |One-byte expression (-126 to +129)          |
//| m               |Any register r, [HL] or [xx+d]              |
//| n               |One-byte expression (0 to 255)              |
//| nn              |Two-byte expression (0 to 65535)            |
//| pp              |Register pair BC, DE, IX or SP              |
//| qq              |Register pair AF, BC, DE or HL              |
//| qq'             |Alternative register pair AF, BC, DE or HL  |
//| r               |Register A, B, C, D, E, H or L              |
//| rr              |Register pair BC, DE, IY or SP              |
//| s               |Any register r, value n, [HL] or [xx+d]     |
//| src             |Source s, ss, [BC], [DE], [HL], nn, [nn]    |
//| ss              |Register pair BC, DE, HL or SP              |
//| xx              |Index register IX or IY                     |
//|-----------------+--------------------------------------------|
//| +  -  *  /  ^   |Add/subtract/multiply/divide/exponent       |
//| &  ~  v  x      |Logical AND/NOT/inclusive OR/exclusive OR   |
//| <-  ->          |Rotate left/right                           |
//| [ ]             |Indirect addressing                         |
//| [ ]+  -[ ]      |Indirect addressing auto-increment/decrement|
//| { }             |Combination of operands                     |
//| #               |Also BC=BC-1,DE=DE-1                        |
//| ##              |Only lower 4 bits of accumulator A used     |
//----------------------------------------------------------------


// On the Amstrad CPC/Plus, the Z80 is clocked to 4MHz.
// All instructions are rounded up to the nearest M-Cycle
// (because of the hardware design and bus sharing with the video devices).

// Nb cycles counts per NOPs
#define CC_1_NOP          4
#define CC_2_NOP          8
#define CC_3_NOP          12
#define CC_4_NOP          16
#define CC_5_NOP          20
#define CC_6_NOP          24


// Number of NOPs have been measured
// Source: http://www.cpctech.org.uk/docs/instrtim.html

// 1 NOP added by prefix CB
#define cc_pfx_cb         CC_1_NOP
// 1 NOP added by prefix ED
#define cc_pfx_ed         CC_1_NOP
// 1 NOP added by prefix DD or FD
#define cc_pfx_dd         CC_1_NOP
#define cc_pfx_fd         CC_1_NOP


//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADC A,(HL)     1  8E              2    7     2    ***V0*  A=A+[HL]+CY
//+----------------------------------------------------------------------------
#define cc_adc_mhl        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADC A,(IX+n)   3  DD 8E XX        5   19     5    ***V0*  A=A+[IX+n]+CY
// ADC A,(IY+n)   3  FD 8E XX        5   19     5    ***V0*  A=A+[IY+n]+CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_adc_mxy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADC A,B        1  88              1    4     1    ***V0*  A=A+B+CY
// ADC A,C        1  89              1    4     1    ***V0*  A=A+C+CY
// ADC A,D        1  8A              1    4     1    ***V0*  A=A+D+CY
// ADC A,E        1  8B              1    4     1    ***V0*  A=A+E+CY
// ADC A,H        1  8C              1    4     1    ***V0*  A=A+H+CY
// ADC A,L        1  8D              1    4     1    ***V0*  A=A+L+CY
// ADC A,A        1  8F              1    4     1    ***V0*  A=A+A+CY
//+----------------------------------------------------------------------------
#define cc_adc_b          CC_1_NOP
#define cc_adc_c          CC_1_NOP
#define cc_adc_d          CC_1_NOP
#define cc_adc_e          CC_1_NOP
#define cc_adc_h          CC_1_NOP
#define cc_adc_l          CC_1_NOP
#define cc_adc_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// ADC A,IXH      2  DD 8C           2    9     ***V0*  A=A+IXH+CY
// ADC A,IYH      2  FD 8C           2    9     ***V0*  A=A+IYH+CY
// ADC A,IXL      2  DD 8D           2    9     ***V0*  A=A+IXL+CY
// ADC A,IYL      2  FD 8D           2    9     ***V0*  A=A+IYL+CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_adc_xyh        CC_1_NOP
#define cc_adc_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADC A,N        2  CE XX           2    7     2    ***V0*  A=A+N+CY
//+----------------------------------------------------------------------------
#define cc_adc_byte       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADC HL,BC      2  ED 4A           4   15     4    ***V0*  HL=HL+BC+CY
// ADC HL,DE      2  ED 5A           4   15     4    ***V0*  HL=HL+DE+CY
// ADC HL,HL      2  ED 6A           4   15     4    ***V0*  HL=HL+HL+CY
// ADC HL,SP      2  ED 7A           4   15     4    ***V0*  HL=HL+SP+CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_adc_hl_bc      CC_3_NOP
#define cc_adc_hl_de      CC_3_NOP
#define cc_adc_hl_hl      CC_3_NOP
#define cc_adc_hl_sp      CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADD A,(HL)     1  86              2    7     2    ***V0*  A=A+[HL]
//+----------------------------------------------------------------------------
#define cc_add_mhl        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// ADD A,(IX+n)   3  DD 86 XX        5   19     ***V0*  A=A+[IX+n]
// ADD A,(IY+n)   3  FD 86 XX        5   19     ***V0*  A=A+[IY+n]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_add_mxy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+--------------------------
// ADD A,B        1  80              1    4     1    ***V0*  A=A+B
// ADD A,C        1  81              1    4     1    ***V0*  A=A+C
// ADD A,D        1  82              1    4     1    ***V0*  A=A+D
// ADD A,E        1  83              1    4     1    ***V0*  A=A+E
// ADD A,H        1  84              1    4     1    ***V0*  A=A+H
// ADD A,L        1  85              1    4     1    ***V0*  A=A+L
// ADD A,A        1  87              1    4     1    ***V0*  A=A+A
//+----------------------------------------------------------------------------
#define cc_add_b          CC_1_NOP
#define cc_add_c          CC_1_NOP
#define cc_add_d          CC_1_NOP
#define cc_add_e          CC_1_NOP
#define cc_add_h          CC_1_NOP
#define cc_add_l          CC_1_NOP
#define cc_add_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// ADD A,IXH      2  DD 84           2    9     ***V0*  A=A+IXH
// ADD A,IYH      2  FD 84           2    9     ***V0*  A=A+IYH
// ADD A,IXL      2  DD 85           2    9     ***V0*  A=A+IXL
// ADD A,IYL      2  FD 85           2    9     ***V0*  A=A+IYL
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_add_xyh        CC_1_NOP
#define cc_add_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADD A,N        2  C6 XX           2    7     2    ***V0*  A=A+N
//+----------------------------------------------------------------------------
#define cc_add_byte       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// ADD HL,BC      1  09              3   11     3    --*-0*  HL=HL+BC
// ADD HL,DE      1  19              3   11     3    --*-0*  HL=HL+DE
// ADD HL,HL      1  29              3   11     3    --*-0*  HL=HL+HL
// ADD HL,SP      1  39              3   11     3    --*-0*  HL=HL+SP
//+----------------------------------------------------------------------------
#define cc_add_hl_bc      CC_3_NOP
#define cc_add_hl_de      CC_3_NOP
#define cc_add_hl_hl      CC_3_NOP
#define cc_add_hl_sp      CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// ADD IX,BC      2  DD 09           4   15     --*-0*  IX=IX+BC
// ADD IY,BC      2  FD 09           4   15     --*-0*  IY=IY+BC
// ADD IX,DE      2  DD 19           4   15     --*-0*  IX=IX+DE
// ADD IY,DE      2  FD 19           4   15     --*-0*  IY=IY+DE
// ADD IX,IX      2  DD 29           4   15     --*-0*  IX=IX+IX
// ADD IY,IY      2  FD 29           4   15     --*-0*  IY=IY+IY
// ADD IX,SP      2  DD 39           4   15     --*-0*  IX=IX+SP
// ADD IY,SP      2  FD 39           4   15     --*-0*  IY=IY+SP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_add_xy_bc      CC_3_NOP
#define cc_add_xy_de      CC_3_NOP
#define cc_add_xy_xy      CC_3_NOP
#define cc_add_xy_sp      CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-----------------------
// AND (HL)       1  A6              2    7     2    ***P00  A=A&[HL]
//+----------------------------------------------------------------------------
#define cc_and_mhl        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// AND (IX+n)     3  DD A6 XX        5   19     5    ***P00  A=A&[IX+n]
// AND (IY+n)     3  FD A6 XX        5   19     5    ***P00  A=A&[IY+n]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_and_mxy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+--------------------------
// AND B          1  A0              1    4     1    ***P00  A=A&B
// AND C          1  A1              1    4     1    ***P00  A=A&C
// AND D          1  A2              1    4     1    ***P00  A=A&D
// AND E          1  A3              1    4     1    ***P00  A=A&E
// AND H          1  A4              1    4     1    ***P00  A=A&H
// AND L          1  A5              1    4     1    ***P00  A=A&L
// AND A          1  A7              1    4     1    ***P00  A=A&A
//+----------------------------------------------------------------------------
#define cc_and_b          CC_1_NOP
#define cc_and_c          CC_1_NOP
#define cc_and_d          CC_1_NOP
#define cc_and_e          CC_1_NOP
#define cc_and_h          CC_1_NOP
#define cc_and_l          CC_1_NOP
#define cc_and_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// AND IXH        2  DD A4           2    9     ***P00  A=A&IXH
// AND IYH        2  FD A4           2    9     ***P00  A=A&IYH
// AND IXL        2  DD A5           2    9     ***P00  A=A&IXL
// AND IYL        2  FD A5           2    9     ***P00  A=A&IYL
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_and_xyh        CC_1_NOP
#define cc_and_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// AND N          2  E6 XX           2    7     2    ***P00  A=A&N
//+----------------------------------------------------------------------------
#define cc_and_byte       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// BIT 0,(HL)     2  CB 46           3   12     3    **1*0-  [HL]&{2^0}
// BIT 1,(HL)     2  CB 4E           3   12     3    **1*0-  [HL]&{2^1}
// BIT 2,(HL)     2  CB 56           3   12     3    **1*0-  [HL]&{2^2}
// BIT 3,(HL)     2  CB 5E           3   12     3    **1*0-  [HL]&{2^3}
// BIT 4,(HL)     2  CB 66           3   12     3    **1*0-  [HL]&{2^4}
// BIT 5,(HL)     2  CB 6E           3   12     3    **1*0-  [HL]&{2^5}
// BIT 6,(HL)     2  CB 76           3   12     3    **1*0-  [HL]&{2^6}
// BIT 7,(HL)     2  CB 7E           3   12     3    **1*0-  [HL]&{2^7}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_bit_mhl        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// BIT b,(IX+n)   4  DD CB XX 46+8*b 6   20     5    **1*0-  [IX+n]&{2^b}
// BIT b,(IY+n)   4  FD CB XX 46+8*b 6   20     5    **1*0-  [IY+n]&{2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_bit_mxy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// BIT b,B        2  CB 40+8*b       2    8     2    **1*0-  B&{2^b}
// BIT b,C        2  CB 41+8*b       2    8     2    **1*0-  C&{2^b}
// BIT b,D        2  CB 42+8*b       2    8     2    **1*0-  D&{2^b}
// BIT b,E        2  CB 43+8*b       2    8     2    **1*0-  E&{2^b}
// BIT b,H        2  CB 44+8*b       2    8     2    **1*0-  H&{2^b}
// BIT b,L        2  CB 45+8*b       2    8     2    **1*0-  L&{2^b}
// BIT b,A        2  CB 47+8*b       2    8     2    **1*0-  A&{2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_bit_b          CC_1_NOP
#define cc_bit_c          CC_1_NOP
#define cc_bit_d          CC_1_NOP
#define cc_bit_e          CC_1_NOP
#define cc_bit_h          CC_1_NOP
#define cc_bit_l          CC_1_NOP
#define cc_bit_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CALL NN        3  CD XX XX        5   17     5    ------  SP-=2,[SP+1,SP]=PC,PC=NN
//+----------------------------------------------------------------------------
#define cc_call           CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CALL C,NN      3  DC XX XX        3/5 10/17  3/5  ------  If CY then [SP-=2]=PC,PC=NN
// CALL M,NN      3  FC XX XX        3/5 10/17  3/5  ------  If S then [SP-=2]=PC,PC=NN
// CALL NC,NN     3  D4 XX XX        3/5 10/17  3/5  ------  If !CY then [SP-=2]=PC,PC=NN
// CALL NZ,NN     3  C4 XX XX        3/5 10/17  3/5  ------  If !Z then [SP-=2]=PC,PC=NN
// CALL P,NN      3  F4 XX XX        3/5 10/17  3/5  ------  If !S then [SP-=2]=PC,PC=NN
// CALL PE,NN     3  EC XX XX        3/5 10/17  3/5  ------  If !P then [SP-=2]=PC,PC=NN
// CALL PO,NN     3  E4 XX XX        3/5 10/17  3/5  ------  If P then [SP-=2]=PC,PC=NN
// CALL Z,NN      3  CC XX XX        3/5 10/17  3/5  ------  If Z then [SP-=2]=PC,PC=NN
//+----------------------------------------------------------------------------
// if Non Condition => 3 NOP, if Condition => 5 NOP
//+----------------------------------------------------------------------------
#define cc_call_c         CC_3_NOP
#define cc_call_m         CC_3_NOP
#define cc_call_nc        CC_3_NOP
#define cc_call_nz        CC_3_NOP
#define cc_call_p         CC_3_NOP
#define cc_call_pe        CC_3_NOP
#define cc_call_po        CC_3_NOP
#define cc_call_z         CC_3_NOP
#define cc_ex_call_c      CC_2_NOP
#define cc_ex_call_m      CC_2_NOP
#define cc_ex_call_nc     CC_2_NOP
#define cc_ex_call_nz     CC_2_NOP
#define cc_ex_call_p      CC_2_NOP
#define cc_ex_call_pe     CC_2_NOP
#define cc_ex_call_po     CC_2_NOP
#define cc_ex_call_z      CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+--------------------------
// CCF            1  3F              1    4     1    --*-00  CY=~CY
//+----------------------------------------------------------------------------
#define cc_ccf            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CP (HL)        1  BE              2    7     2    ***V1*  A-[HL]
//+----------------------------------------------------------------------------
#define cc_cp_mhl         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CP (IX+n)      3  DD BE XX        5   19     5    ***V1*  A-[IX+n]
// CP (IY+n)      3  FD BE XX        5   19     5    ***V1*  A-[IY+n]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_cp_mxy         CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CP B           1  B8              1    4     1    ***V1*  A-B
// CP C           1  B9              1    4     1    ***V1*  A-C
// CP D           1  BA              1    4     1    ***V1*  A-D
// CP E           1  BB              1    4     1    ***V1*  A-E
// CP H           1  BC              1    4     1    ***V1*  A-H
// CP L           1  BD              1    4     1    ***V1*  A-L
// CP A           1  BF              1    4     1    ***V1*  A-A
//+----------------------------------------------------------------------------
#define cc_cp_b           CC_1_NOP
#define cc_cp_c           CC_1_NOP
#define cc_cp_d           CC_1_NOP
#define cc_cp_e           CC_1_NOP
#define cc_cp_h           CC_1_NOP
#define cc_cp_l           CC_1_NOP
#define cc_cp_a           CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// CP IXH         2  DD BC           2    9     ***V1*  A-IXH
// CP IYH         2  FD BC           2    9     ***V1*  A-IYH
// CP IXL         2  DD BD           2    9     ***V1*  A-IXL
// CP IYL         2  FD BD           2    9     ***V1*  A-IYL
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_cp_xyh         CC_1_NOP
#define cc_cp_xyl         CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CP N           2  FE XX           2    7     2    ***V1*  A-N
//+----------------------------------------------------------------------------
#define cc_cp_byte        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// CPD            2  ED A9           5   16     4    ****1-  A-[HL],HL=HL-1,BC=BC-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#ifndef Z80_NOP_TIMINGS
#define cc_cpd            12
#else /* Z80_NOP_TIMINGS */
#define cc_cpd            CC_4_NOP
#endif /* Z80_NOP_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// CPDR           2  ED B9           6/5 21/16  5/4  ****1-  CPD until A=[HL] or BC=0
//+----------------------------------------------------------------------------
// if BC-1<>0 => 6 NOP, if BC-1=0 => 5 NOP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#ifndef Z80_ORIGINAL_TIMINGS
#define cc_cpdr           12
#define cc_ex_cpdr        4
#else /* Z80_ORIGINAL_TIMINGS */
#ifndef Z80_NOP_TIMINGS
#define cc_cpdr           12
#define cc_ex_cpdr        8
#else /* Z80_NOP_TIMINGS */
#define cc_cpdr           CC_4_NOP
#define cc_ex_cpdr        CC_1_NOP
#endif /* Z80_NOP_TIMINGS */
#endif /* Z80_ORIGINAL_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CPI            2  ED A1           5   16     4    ****1-  A-[HL],HL=HL+1,BC=BC-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#ifndef Z80_NOP_TIMINGS
#define cc_cpi            12
#else /* Z80_NOP_TIMINGS */
#define cc_cpi            CC_4_NOP
#endif /* Z80_NOP_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CPIR           2  ED B1           6/5 21/16  5/4  ****1-  CPI until A=[HL] or BC=0
//+----------------------------------------------------------------------------
// if BC-1<>0 => 6 NOP, if BC-1=0 => 5 NOP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#ifndef Z80_ORIGINAL_TIMINGS
#define cc_cpir           12
#define cc_ex_cpir        4
#else /* Z80_ORIGINAL_TIMINGS */
#ifndef Z80_NOP_TIMINGS
#define cc_cpir           12
#define cc_ex_cpir        8
#else /* Z80_NOP_TIMINGS */
#define cc_cpir           CC_4_NOP
#define cc_ex_cpir        CC_1_NOP
#endif /* Z80_NOP_TIMINGS */
#endif /* Z80_ORIGINAL_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// CPL            1  2F              1    4     1    --1-1-  A=~A
//+----------------------------------------------------------------------------
#define cc_cpl            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DAA            1  27              1    4     1    ***P-*  A=adjust result to BCD-format
//+----------------------------------------------------------------------------
#define cc_daa            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DEC (HL)       1  35              3   11     3    ***V1-  [HL]=[HL]-1
//+----------------------------------------------------------------------------
#define cc_dec_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DEC (IX+n)     3  DD 35 XX        6   23     6    ***V1-  [IX+n]=[IX+n]-1
// DEC (IY+n)     3  FD 35 XX        6   23     6    ***V1-  [IY+n]=[IY+n]-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_dec_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DEC B          1  05              1    4     1    ***V1-  B=B-1
// DEC C          1  0D              1    4     1    ***V1-  C=C-1
// DEC D          1  15              1    4     1    ***V1-  D=D-1
// DEC E          1  1D              1    4     1    ***V1-  E=E-1
// DEC H          1  25              1    4     1    ***V1-  H=H-1
// DEC L          2  2D              1    4     1    ***V1-  L=L-1
// DEC A          1  3D              1    4     1    ***V1-  A=A-1
//+----------------------------------------------------------------------------
#define cc_dec_b          CC_1_NOP
#define cc_dec_c          CC_1_NOP
#define cc_dec_d          CC_1_NOP
#define cc_dec_e          CC_1_NOP
#define cc_dec_h          CC_1_NOP
#define cc_dec_l          CC_1_NOP
#define cc_dec_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DEC BC         1  0B              2    6     1    ------  BC=BC-1
// DEC DE         1  1B              2    6     1    ------  DE=DE-1
// DEC HL         1  2B              2    6     1    ------  HL=HL-1
// DEC SP         1  3B              2    6     1    ------  SP=SP-1
//+----------------------------------------------------------------------------
#define cc_dec_bc         CC_2_NOP
#define cc_dec_de         CC_2_NOP
#define cc_dec_hl         CC_2_NOP
#define cc_dec_sp         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DEC IX         2  DD 2B           3   10     2    ------  IX=IX-1
// DEC IY         2  FD 2B           3   10     2    ------  IY=IY-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_dec_xy         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// DEC IXH        2  DD 25           2    9     ------  IXH=IXH-1
// DEC IYH        2  FD 25           2    9     ------  IYH=IYH-1
// DEC IXL        2  DD 2D           2    9     ------  IXL=IXL-1
// DEC IYL        2  FD 2D           2    9     ------  IYL=IYL-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_dec_xyh        CC_1_NOP
#define cc_dec_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DI             1  F3              1    4     1    ------  disable interrupts
//+----------------------------------------------------------------------------
#define cc_di             CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// DJNZ n         2  10 XX           4/3 13/8   3/2  ------  B=B-1, if B != 0 then PC+=n
//+----------------------------------------------------------------------------
// B=B-1; if B<>0 => 4 NOP, if B=0 => 3 NOP
//+----------------------------------------------------------------------------
#define cc_djnz           CC_3_NOP
#define cc_ex_djnz        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// EI             1  FB              1    4     1    ------  enable interrupts
//+----------------------------------------------------------------------------
#define cc_ei             CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// EX (SP),HL     1  E3              6   19     5    ------  [SP]<->HL
//+----------------------------------------------------------------------------
#define cc_ex_msp_hl      CC_6_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// EX (SP),IX     2  DD E3           7   23     6    ------  [SP]<->IX
// EX (SP),IY     2  FD E3           7   23     6    ------  [SP]<->IY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ex_msp_xy      CC_6_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// EX AF,AF'      1  08              1    4     1    ******  AF<->AF'
//+----------------------------------------------------------------------------
#define cc_ex_af_af       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// EX DE,HL       1  EB              1    4     1    ------  DE<->HL
//+----------------------------------------------------------------------------
#define cc_ex_de_hl       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-----------------------
// EXX            1  D9              1    4     1    ------  BC<->BC',DE<->DE',HL<->HL'
//+----------------------------------------------------------------------------
#define cc_exx            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// HALT           1  76              1    4     1    ------  repeat NOP until interrupt
//+----------------------------------------------------------------------------
#define cc_halt           CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// IM 0           2  ED 46           2    8     2    ------  set interrupt 0
// IM 0 ?         2  ED 4E           2    8     2    ------  set interrupt 0
// IM 0 ?         2  ED 66           2    8     2    ------  set interrupt 0
// IM 0 ?         2  ED 6E           2    8     2    ------  set interrupt 0
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#ifndef Z80_NOP_TIMINGS
#define cc_im_0           8
#define cc_im_0_1         4
#define cc_im_0_2         4
#define cc_im_0_3         4
#else /* Z80_NOP_TIMINGS */
#define cc_im_0           CC_1_NOP
#define cc_im_0_1         CC_1_NOP
#define cc_im_0_2         CC_1_NOP
#define cc_im_0_3         CC_1_NOP
#endif /* Z80_NOP_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// IM 1           2  ED 56           2    8     2    ------  set interrupt 1
// IM 1 ?         2  ED 7E           2    8     2    ------  set interrupt 1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#ifndef Z80_NOP_TIMINGS
#define cc_im_1           8
#define cc_im_1_1         4
#else /* Z80_NOP_TIMINGS */
#define cc_im_1           CC_1_NOP
#define cc_im_1_1         CC_1_NOP
#endif /* Z80_NOP_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// IM 2           2  ED 5E           2    8     2    ------  set interrupt 2
// IM 2 ?         2  ED 7E           2    8     2    ------  set interrupt 2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#ifndef Z80_NOP_TIMINGS
#define cc_im_2           8
#define cc_im_2_1         4
#else /* Z80_NOP_TIMINGS */
#define cc_im_2           CC_1_NOP
#define cc_im_2_1         CC_1_NOP
#endif /* Z80_NOP_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// IN B,(C)       2  ED 40           4   12     3    ***P0-  B=[C]
// IN C,(C)       2  ED 48           4   12     3    ***P0-  C=[C]
// IN D,(C)       2  ED 50           4   12     3    ***P0-  D=[C]
// IN E,(C)       2  ED 58           4   12     3    ***P0-  E=[C]
// IN H,(C)       2  ED 60           4   12     3    ***P0-  H=[C]
// IN L,(C)       2  ED 68           4   12     3    ***P0-  L=[C]
// IN A,(C)       2  ED 78           4   12     3    ***P0-  A=[C]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_in_r_c         CC_3_NOP
#define cc_in_r_c_        0

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// IN F,(C)       2  ED 70           4   12     3    ***P0-
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_in_f_c         CC_3_NOP
#define cc_in_f_c_        0

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// IN A,(N)       2  DB XX           3   11     3    ------  A=[N]
//+----------------------------------------------------------------------------
#define cc_ina            CC_3_NOP
#define cc_ina_           0

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// INC (HL)       1  34              3   11     3    ***V0-  [HL]=[HL]+1
//+----------------------------------------------------------------------------
#define cc_inc_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// INC (IX+n)     3  DD 34 XX        6   23     6     ***V0-  [IY+n]=[IX+n]+1
// INC (IY+n)     3  FD 34 XX        6   23     6     ***V0-  [IY+n]=[IY+n]+1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_inc_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// INC B          1  04              1    4     1    ***V0-  B=B+1
// INC C          1  0C              1    4     1    ***V0-  C=C+1
// INC D          1  14              1    4     1    ***V0-  D=D+1
// INC E          1  1C              1    4     1    ***V0-  E=E+1
// INC H          1  24              1    4     1    ***V0-  H=H+1
// INC L          1  2C              1    4     1    ***V0-  L=L+1
// INC A          1  3C              1    4     1    ***V0-  A=A+1
//+----------------------------------------------------------------------------
#define cc_inc_b          CC_1_NOP
#define cc_inc_c          CC_1_NOP
#define cc_inc_d          CC_1_NOP
#define cc_inc_e          CC_1_NOP
#define cc_inc_h          CC_1_NOP
#define cc_inc_l          CC_1_NOP
#define cc_inc_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// INC BC         1  03              2    6     1    ------  BC=BC+1
// INC DE         1  13              2    6     1    ------  DE=DE+1
// INC HL         1  23              2    6     1    ------  HL=HL+1
// INC SP         1  33              2    6     1    ------  SP=SP+1
//+----------------------------------------------------------------------------
#define cc_inc_bc         CC_2_NOP
#define cc_inc_de         CC_2_NOP
#define cc_inc_hl         CC_2_NOP
#define cc_inc_sp         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// INC IX         2  DD 23           3   10     2    ------  IX=IX+1
// INC IY         2  FD 23           3   10     2    ------  IY=IY+1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_inc_xy         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// INC IXH        2  DD 24           2    9     ------  IXH=IXH+1
// INC IYH        2  FD 24           2    9     ------  IYH=IYH+1
// INC IXL        2  DD 2C           2    9     ------  IXL=IXL+1
// INC IYL        2  FD 2C           2    9     ------  IYL=IYL+1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_inc_xyh        CC_1_NOP
#define cc_inc_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// IND            2  ED AA           5   16     4    ***?1-  [HL]=[C],HL=HL-1,B=B-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ind            CC_4_NOP
#define cc_ind_           0

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// INDR           2  ED BA           6/5 21/16  5/4  01*?1-  IND until B=0
//+----------------------------------------------------------------------------
// if B<>0 => 6 NOP, if B=0 => 5 NOP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_indr           CC_4_NOP
#define cc_indr_          0
#define cc_ex_indr        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// INI            2  ED A2           5   16     4    ***?1-  [HL]=[C],HL=HL+1,B=B-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ini            CC_4_NOP
#define cc_ini_           0

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// INIR           2  ED B2           6/5 21/16  5/4  01*?1-  INI until B=0
//+----------------------------------------------------------------------------
// if B<>0 => 6 NOP, if B=0 => 5 NOP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_inir           CC_4_NOP
#define cc_inir_          0
#define cc_ex_inir        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// JP NN          3  C3 XX XX        3   10     3    ------  PC=NN
//+----------------------------------------------------------------------------
#define cc_jp             CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// JP (HL)        1  E9              1    4     1    ------  PC=HL
//+----------------------------------------------------------------------------
#define cc_ld_pc_hl       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// JP (IX)        2  DD E9           2    8     2    ------  PC=IX
// JP (IY)        2  FD E9           2    8     2    ------  PC=IY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_pc_xy       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// JP C,NN        3  DA XX XX        3   10     3    ------  If CY then PC=NN
// JP M,NN        3  FA XX XX        3   10     3    ------  If S then PC=NN
// JP NC,NN       3  D2 XX XX        3   10     3    ------  If !CY then PC=NN
// JP NZ,NN       3  C2 XX XX        3   10     3    ------  If !Z then PC=NN
// JP P,NN        3  F2 XX XX        3   10     3    ------  If !S then PC=NN
// JP PE,NN       3  EA XX XX        3   10     3    ------  If !P then PC=NN
// JP PO,NN       3  E2 XX XX        3   10     3    ------  If P then PC=NN
// JP Z,NN        3  CA XX XX        3   10     3    ------  If Z then PC=NN
//+----------------------------------------------------------------------------
#define cc_jp_c           CC_3_NOP
#define cc_jp_m           CC_3_NOP
#define cc_jp_nc          CC_3_NOP
#define cc_jp_nz          CC_3_NOP
#define cc_jp_p           CC_3_NOP
#define cc_jp_pe          CC_3_NOP
#define cc_jp_po          CC_3_NOP
#define cc_jp_z           CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// JR n           2  18 XX           3   12     3    ------  PC=PC+n
//+----------------------------------------------------------------------------
#define cc_jr             CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// JR C,n         2  38 XX           3/2 12/7   3/2  ------  If CY then PC=PC+n
// JR NC,n        2  30 XX           3/2 12/7   3/2  ------  If !CY then PC=PC+n
// JR NZ,n        2  20 XX           3/2 12/7   3/2  ------  If !Z then PC=PC+n
// JR Z,n         2  28 XX           3/2 12/7   3/2  ------  If Z then PC=PC+n
//+----------------------------------------------------------------------------
// if Condition => 3 NOP, if Not Condition => 2 NOP
//+----------------------------------------------------------------------------
#define cc_jr_c           CC_2_NOP
#define cc_jr_nc          CC_2_NOP
#define cc_jr_nz          CC_2_NOP
#define cc_jr_z           CC_2_NOP
#define cc_ex_jr_c        CC_1_NOP
#define cc_ex_jr_nc       CC_1_NOP
#define cc_ex_jr_nz       CC_1_NOP
#define cc_ex_jr_z        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (BC),A      1  02              2    7     2    ------  [BC]=A
// LD (DE),A      1  12              2    7     2    ------  [DE]=A
//+----------------------------------------------------------------------------
#define cc_ld_mbc_a       CC_2_NOP
#define cc_ld_mde_a       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (HL),B      1  70              2    7     2    ------  [HL]=B
// LD (HL),C      1  71              2    7     2    ------  [HL]=C
// LD (HL),D      1  72              2    7     2    ------  [HL]=D
// LD (HL),E      1  73              2    7     2    ------  [HL]=E
// LD (HL),H      1  74              2    7     2    ------  [HL]=H
// LD (HL),L      1  75              2    7     2    ------  [HL]=L
// LD (HL),A      1  77              2    7     2    ------  [HL]=A
//+----------------------------------------------------------------------------
#define cc_ld_mhl_b       CC_2_NOP
#define cc_ld_mhl_c       CC_2_NOP
#define cc_ld_mhl_d       CC_2_NOP
#define cc_ld_mhl_e       CC_2_NOP
#define cc_ld_mhl_h       CC_2_NOP
#define cc_ld_mhl_l       CC_2_NOP
#define cc_ld_mhl_a       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (IX+n),B    3  DD 70 XX        5   19     5    ------  [IX+n]=B
// LD (IY+n),B    3  FD 70 XX        5   19     5    ------  [IY+n]=B
// LD (IX+n),C    3  DD 71 XX        5   19     5    ------  [IX+n]=C
// LD (IY+n),C    3  FD 71 XX        5   19     5    ------  [IY+n]=C
// LD (IX+n),D    3  DD 72 XX        5   19     5    ------  [IX+n]=D
// LD (IY+n),D    3  FD 72 XX        5   19     5    ------  [IY+n]=D
// LD (IX+n),E    3  DD 73 XX        5   19     5    ------  [IX+n]=E
// LD (IY+n),E    3  FD 73 XX        5   19     5    ------  [IY+n]=E
// LD (IX+n),H    3  DD 74 XX        5   19     5    ------  [IX+n]=H
// LD (IY+n),H    3  FD 74 XX        5   19     5    ------  [IY+n]=H
// LD (IX+n),L    3  DD 75 XX        5   19     5    ------  [IX+n]=L
// LD (IY+n),L    3  FD 75 XX        5   19     5    ------  [IY+n]=L
// LD (IX+n),A    3  DD 77 XX        5   19     5    ------  [IX+n]=A
// LD (IY+n),A    3  FD 77 XX        5   19     5    ------  [IY+n]=A
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_mxy_b       CC_4_NOP
#define cc_ld_mxy_c       CC_4_NOP
#define cc_ld_mxy_d       CC_4_NOP
#define cc_ld_mxy_e       CC_4_NOP
#define cc_ld_mxy_h       CC_4_NOP
#define cc_ld_mxy_l       CC_4_NOP
#define cc_ld_mxy_a       CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (HL),N      2  36 XX           3   10     3    ------  [HL]=N
//+----------------------------------------------------------------------------
#define cc_ld_mhl_byte    CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (IX+n),N    4  DD 36 XX XX     6   19     5    ------  [IX+n]=N
// LD (IY+n),N    4  FD 36 XX XX     6   19     5    ------  [IY+n]=N
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_mxy_byte    CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// LD (NN),A      3  32 XX XX        4   13     4    ------  [NN]=A
//+----------------------------------------------------------------------------
#define cc_ld_mword_a     CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (NN),BC     4  ED 43 XX XX     6   20     6    ------  [NN]=C, (NN+1)=B
// LD (NN),DE     4  ED 53 XX XX     6   20     6    ------  [NN]=E, (NN+1)=D
// LD (NN),HL     4  ED 63 XX XX     6   20     6    ------  [NN]=L, (NN+1)=H
// LD (NN),SP     4  ED 73 XX XX     6   20     6    ------  [NN,NN+1]=SP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ld_EDmword_bc  CC_5_NOP
#define cc_ld_EDmword_de  CC_5_NOP
#define cc_ld_EDmword_hl  CC_5_NOP
#define cc_ld_EDmword_sp  CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (NN),HL     3  22 XX XX        5   16     5    ------  [NN]=L, (NN+1)=H
//+----------------------------------------------------------------------------
#define cc_ld_mword_hl    CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD (NN),IX     4  DD 22 XX XX     6   20     6    ------  [NN,NN+1]=IX
// LD (NN),IY     4  FD 22 XX XX     6   20     6    ------  [NN,NN+1]=IY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_mword_xy    CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD A,(BC)      1  0A              2    7     2    ------  A=[BC]
// LD A,(DE)      1  1A              2    7     2    ------  A=[DE]
//+----------------------------------------------------------------------------
#define cc_ld_a_mbc       CC_2_NOP
#define cc_ld_a_mde       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD B,(HL)      1  46              2    7     2    ------  B=[HL]
// LD C,(HL)      1  4E              2    7     2    ------  C=[HL]
// LD D,(HL)      1  56              2    7     2    ------  D=[HL]
// LD E,(HL)      1  5E              2    7     2    ------  E=[HL]
// LD H,(HL)      1  66              2    7     2    ------  H=[HL]
// LD L,(HL)      1  6E              2    7     2    ------  L=[HL]
// LD A,(HL)      1  7E              2    7     2    ------  A=[HL]
//+----------------------------------------------------------------------------
#define cc_ld_b_mhl       CC_2_NOP
#define cc_ld_c_mhl       CC_2_NOP
#define cc_ld_d_mhl       CC_2_NOP
#define cc_ld_e_mhl       CC_2_NOP
#define cc_ld_h_mhl       CC_2_NOP
#define cc_ld_l_mhl       CC_2_NOP
#define cc_ld_a_mhl       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD B,(IX+n)    3  DD 46 XX        5   19     5    ------  B=[IX+n]
// LD B,(IY+n)    3  FD 46 XX        5   19     5    ------  B=[IY+n]
// LD C,(IX+n)    3  DD 4E XX        5   19     5    ------  C=[IX+n]
// LD C,(IY+n)    3  FD 4E XX        5   19     5    ------  C=[IY+n]
// LD D,(IX+n)    3  DD 56 XX        5   19     5    ------  D=[IX+n]
// LD D,(IY+n)    3  FD 56 XX        5   19     5    ------  D=[IY+n]
// LD E,(IX+n)    3  DD 5E XX        5   19     5    ------  E=[IX+n]
// LD E,(IY+n)    3  FD 5E XX        5   19     5    ------  E=[IY+n]
// LD H,(IX+n)    3  DD 66 XX        5   19     5    ------  H=[IX+n]
// LD H,(IY+n)    3  FD 66 XX        5   19     5    ------  H=[IY+n]
// LD L,(IX+n)    3  DD 6E XX        5   19     5    ------  L=[IX+n]
// LD L,(IY+n)    3  FD 6E XX        5   19     5    ------  L=[IY+n]
// LD A,(IX+n)    3  DD 7E XX        5   19     5    ------  A=[IX+n]
// LD A,(IY+n)    3  FD 7E XX        5   19     5    ------  A=[IY+n]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_b_mxy       CC_4_NOP
#define cc_ld_c_mxy       CC_4_NOP
#define cc_ld_d_mxy       CC_4_NOP
#define cc_ld_e_mxy       CC_4_NOP
#define cc_ld_h_mxy       CC_4_NOP
#define cc_ld_l_mxy       CC_4_NOP
#define cc_ld_a_mxy       CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// LD A,(NN)      3  3A XX XX        4   13     4    ------  A=[NN]
//+----------------------------------------------------------------------------
#define cc_ld_a_mword     CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD B,B         1  40              1    4     1    ------  B=B
// LD B,C         1  41              1    4     1    ------  B=C
// LD B,D         1  42              1    4     1    ------  B=D
// LD B,E         1  43              1    4     1    ------  B=E
// LD B,H         1  44              1    4     1    ------  B=H
// LD B,L         1  45              1    4     1    ------  B=L
// LD B,A         1  47              1    4     1    ------  B=A
// LD C,B         1  48              1    4     1    ------  C=B
// LD C,C         1  49              1    4     1    ------  C=C
// LD C,D         1  4A              1    4     1    ------  C=D
// LD C,E         1  4B              1    4     1    ------  C=E
// LD C,H         1  4C              1    4     1    ------  C=H
// LD C,L         1  4D              1    4     1    ------  C=L
// LD C,A         1  4F              1    4     1    ------  C=A
// LD D,B         1  50              1    4     1    ------  D=B
// LD D,C         1  51              1    4     1    ------  D=C
// LD D,D         1  52              1    4     1    ------  D=D
// LD D,E         1  53              1    4     1    ------  D=E
// LD D,H         1  54              1    4     1    ------  D=H
// LD D,L         1  55              1    4     1    ------  D=L
// LD D,A         1  57              1    4     1    ------  D=A
// LD E,B         1  58              1    4     1    ------  E=B
// LD E,C         1  59              1    4     1    ------  E=C
// LD E,D         1  5A              1    4     1    ------  E=D
// LD E,E         1  5B              1    4     1    ------  E=E
// LD E,H         1  5C              1    4     1    ------  E=H
// LD E,L         1  5D              1    4     1    ------  E=L
// LD E,A         1  5F              1    4     1    ------  E=A
// LD H,B         1  60              1    4     1    ------  H=B
// LD H,C         1  61              1    4     1    ------  H=C
// LD H,D         1  62              1    4     1    ------  H=D
// LD H,E         1  63              1    4     1    ------  H=E
// LD H,H         1  64              1    4     1    ------  H=H
// LD H,L         1  65              1    4     1    ------  H=L
// LD H,A         1  67              1    4     1    ------  H=A
// LD L,B         1  68              1    4     1    ------  L=B
// LD L,C         1  69              1    4     1    ------  L=C
// LD L,D         1  6A              1    4     1    ------  L=D
// LD L,E         1  6B              1    4     1    ------  L=E
// LD L,H         1  6C              1    4     1    ------  L=H
// LD L,L         1  6D              1    4     1    ------  L=L
// LD L,A         1  6F              1    4     1    ------  L=A
// LD A,B         1  78              1    4     1    ------  A=B
// LD A,C         1  79              1    4     1    ------  A=C
// LD A,D         1  7A              1    4     1    ------  A=D
// LD A,E         1  7B              1    4     1    ------  A=E
// LD A,H         1  7C              1    4     1    ------  A=H
// LD A,L         1  7D              1    4     1    ------  A=L
// LD A,A         1  7F              1    4     1    ------  A=A
//+----------------------------------------------------------------------------
#define cc_ld_b_b         CC_1_NOP
#define cc_ld_b_c         CC_1_NOP
#define cc_ld_b_d         CC_1_NOP
#define cc_ld_b_e         CC_1_NOP
#define cc_ld_b_h         CC_1_NOP
#define cc_ld_b_l         CC_1_NOP
#define cc_ld_b_a         CC_1_NOP
#define cc_ld_c_b         CC_1_NOP
#define cc_ld_c_c         CC_1_NOP
#define cc_ld_c_d         CC_1_NOP
#define cc_ld_c_e         CC_1_NOP
#define cc_ld_c_h         CC_1_NOP
#define cc_ld_c_l         CC_1_NOP
#define cc_ld_c_a         CC_1_NOP
#define cc_ld_d_b         CC_1_NOP
#define cc_ld_d_c         CC_1_NOP
#define cc_ld_d_d         CC_1_NOP
#define cc_ld_d_e         CC_1_NOP
#define cc_ld_d_h         CC_1_NOP
#define cc_ld_d_l         CC_1_NOP
#define cc_ld_d_a         CC_1_NOP
#define cc_ld_e_b         CC_1_NOP
#define cc_ld_e_c         CC_1_NOP
#define cc_ld_e_d         CC_1_NOP
#define cc_ld_e_e         CC_1_NOP
#define cc_ld_e_h         CC_1_NOP
#define cc_ld_e_l         CC_1_NOP
#define cc_ld_e_a         CC_1_NOP
#define cc_ld_h_b         CC_1_NOP
#define cc_ld_h_c         CC_1_NOP
#define cc_ld_h_d         CC_1_NOP
#define cc_ld_h_e         CC_1_NOP
#define cc_ld_h_h         CC_1_NOP
#define cc_ld_h_l         CC_1_NOP
#define cc_ld_h_a         CC_1_NOP
#define cc_ld_l_b         CC_1_NOP
#define cc_ld_l_c         CC_1_NOP
#define cc_ld_l_d         CC_1_NOP
#define cc_ld_l_e         CC_1_NOP
#define cc_ld_l_h         CC_1_NOP
#define cc_ld_l_l         CC_1_NOP
#define cc_ld_l_a         CC_1_NOP
#define cc_ld_a_b         CC_1_NOP
#define cc_ld_a_c         CC_1_NOP
#define cc_ld_a_d         CC_1_NOP
#define cc_ld_a_e         CC_1_NOP
#define cc_ld_a_h         CC_1_NOP
#define cc_ld_a_l         CC_1_NOP
#define cc_ld_a_a         CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// LD B,IXH       2  DD 44           2    9     ------  B=IXH
// LD B,IYH       2  FD 44           2    9     ------  B=IYH
// LD B,IXL       2  DD 45           2    9     ------  B=IXL
// LD B,IYL       2  FD 45           2    9     ------  B=IYL
// LD C,IXH       2  DD 4C           2    9     ------  C=IXH
// LD C,IYH       2  FD 4C           2    9     ------  C=IYH
// LD C,IXL       2  DD 4D           2    9     ------  C=IXL
// LD C,IYL       2  FD 4D           2    9     ------  C=IYL
// LD D,IXH       2  DD 54           2    9     ------  D=IXH
// LD D,IYH       2  FD 54           2    9     ------  D=IYH
// LD D,IXL       2  DD 55           2    9     ------  D=IXL
// LD D,IYL       2  FD 55           2    9     ------  D=IYL
// LD E,IXH       2  DD 5C           2    9     ------  E=IXH
// LD E,IYH       2  FD 5C           2    9     ------  E=IYH
// LD E,IXL       2  DD 5D           2    9     ------  E=IXL
// LD E,IYL       2  FD 5D           2    9     ------  E=IYL
// LD A,IXH       3  DD 7C           2    9     ------  A=IXH
// LD A,IYH       3  FD 7C           2    9     ------  A=IYH
// LD A,IXL       3  DD 7D           2    9     ------  A=IXL
// LD A,IYL       3  FD 7D           2    9     ------  A=IYL
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_b_xyh       CC_1_NOP
#define cc_ld_b_xyl       CC_1_NOP
#define cc_ld_c_xyh       CC_1_NOP
#define cc_ld_c_xyl       CC_1_NOP
#define cc_ld_d_xyh       CC_1_NOP
#define cc_ld_d_xyl       CC_1_NOP
#define cc_ld_e_xyh       CC_1_NOP
#define cc_ld_e_xyl       CC_1_NOP
#define cc_ld_a_xyh       CC_1_NOP
#define cc_ld_a_xyl       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD A,I         2  ED 57           3    9     2    **0*0-  A=I
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ld_a_i         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD B,N         2  06 XX           2    7     2    ------  B=N
// LD C,N         2  0E XX           2    7     2    ------  C=N
// LD D,N         2  16 XX           2    7     2    ------  D=N
// LD E,N         2  1E XX           2    7     2    ------  E=N
// LD H,N         2  26 XX           2    7     2    ------  H=N
// LD L,N         2  2E XX           2    7     2    ------  L=N
// LD A,N         2  3E XX           2    7     2    ------  A=N
//+----------------------------------------------------------------------------
#define cc_ld_b_byte      CC_2_NOP
#define cc_ld_c_byte      CC_2_NOP
#define cc_ld_d_byte      CC_2_NOP
#define cc_ld_e_byte      CC_2_NOP
#define cc_ld_h_byte      CC_2_NOP
#define cc_ld_l_byte      CC_2_NOP
#define cc_ld_a_byte      CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD A,R         2  ED 5F           3    9     2    **0*0-  A=R
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ld_a_r         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD HL,(NN)     3  2A XX XX        5   16     5    ------  L=[NN],H=[NN+1]
//+----------------------------------------------------------------------------
#define cc_ld_hl_mword    CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD BC,(NN)     4  ED 4B XX XX     6   20     6    ------  C=[NN],B=[NN+1]
// LD DE,(NN)     4  ED 5B XX XX     6   20     6    ------  E=[NN],D=[NN+1]
// LD HL,(NN)     4  ED 6B XX XX     6   20     6    ------  L=[NN],H=[NN+1]
// LD SP,(NN)     4  ED 7B XX XX     6   20     6    ------  SP=[NN,NN+1]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ld_EDbc_mword  CC_5_NOP
#define cc_ld_EDde_mword  CC_5_NOP
#define cc_ld_EDhl_mword  CC_5_NOP
#define cc_ld_EDsp_mword  CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD BC,NN       3  01 XX XX        3   10     2    ------  BC=NN
// LD DE,NN       3  11 XX XX        3   10     2    ------  DE=NN
// LD HL,NN       3  21 XX XX        3   10     2    ------  HL=NN
// LD SP,NN       3  31 XX XX        3   10     2    ------  SP=NN
//+----------------------------------------------------------------------------
#define cc_ld_bc_word     CC_3_NOP
#define cc_ld_de_word     CC_3_NOP
#define cc_ld_hl_word     CC_3_NOP
#define cc_ld_sp_word     CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD I,A         2  ED 47           3   9     2    ------  I=A
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ld_i_a         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// LD IXH,B       2  DD 60           2    9     ------  IXH=B
// LD IYH,B       2  FD 60           2    9     ------  IYH=B
// LD IXH,C       2  DD 61           2    9     ------  IXH=C
// LD IYH,C       2  FD 61           2    9     ------  IYH=C
// LD IXH,D       2  DD 62           2    9     ------  IXH=D
// LD IYH,D       2  FD 62           2    9     ------  IYH=D
// LD IXH,E       2  DD 63           2    9     ------  IXH=E
// LD IYH,E       2  FD 63           2    9     ------  IYH=E
// LD IXH,IXH     2  DD 64           2    9     ------  IXH=IXH
// LD IYH,IYH     2  FD 64           2    9     ------  IYH=IYH
// LD IXH,IXL     2  DD 65           2    9     ------  IXH=IXL
// LD IYH,IYL     2  FD 65           2    9     ------  IYH=IYL
// LD IXH,A       2  DD 67           2    9     ------  IXH=A
// LD IYH,A       2  FD 67           2    9     ------  IYH=A
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_xyh_b       CC_1_NOP
#define cc_ld_xyh_c       CC_1_NOP
#define cc_ld_xyh_d       CC_1_NOP
#define cc_ld_xyh_e       CC_1_NOP
#define cc_ld_xyh_xyh     CC_1_NOP
#define cc_ld_xyh_xyl     CC_1_NOP
#define cc_ld_xyh_a       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// LD IXH,N       3  DD 26 XX        3    9     ------  IXH=N
// LD IYH,N       3  FD 26 XX        3    9     ------  IYH=N
// LD IXL,N       3  DD 2E XX        3    9     ------  IXL=N
// LD IYL,N       3  FD 2E XX        3    9     ------  IYL=N
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_xyh_byte    CC_2_NOP
#define cc_ld_xyl_byte    CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// LD IXL,B       2  DD 68           2    9     ------  IXL=B
// LD IYL,B       2  FD 68           2    9     ------  IYL=B
// LD IXL,C       2  DD 69           2    9     ------  IXL=C
// LD IYL,C       2  FD 69           2    9     ------  IYL=C
// LD IXL,D       2  DD 6A           2    9     ------  IXL=D
// LD IYL,D       2  FD 6A           2    9     ------  IYL=D
// LD IXL,E       2  DD 6B           2    9     ------  IXL=E
// LD IYL,E       2  FD 6B           2    9     ------  IYL=E
// LD IXL,IXH     2  DD 6C           2    9     ------  IXL=IXH
// LD IYL,IYH     2  FD 6C           2    9     ------  IYL=IYH
// LD IXL,IXL     2  DD 6D           2    9     ------  IXL=IXL
// LD IYL,IYL     2  FD 6D           2    9     ------  IYL=IYL
// LD IXL,A       2  DD 6F           2    9     ------  IXL=A
// LD IYL,A       2  FD 6F           2    9     ------  IYL=A
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_xyl_b       CC_1_NOP
#define cc_ld_xyl_c       CC_1_NOP
#define cc_ld_xyl_d       CC_1_NOP
#define cc_ld_xyl_e       CC_1_NOP
#define cc_ld_xyl_xyh     CC_1_NOP
#define cc_ld_xyl_xyl     CC_1_NOP
#define cc_ld_xyl_a       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD IX,(NN)     4  DD 2A XX XX     6   20     6    ------  IX=[NN,NN+1]
// LD IY,(NN)     4  FD 2A XX XX     6   20     6    ------  IY=[NN,NN+1]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_xy_mword    CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD IX,NN       4  DD 21 XX XX     4   14     4    ------  IX=NN
// LD IY,NN       4  FD 21 XX XX     4   14     4    ------  IY=NN
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_xy_word     CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD R,A         2  ED 4F           3    9     2    ------  R=A
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ld_r_a         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD SP,HL       1  F9              2    6     1    ------  SP=HL
//+----------------------------------------------------------------------------
#define cc_ld_sp_hl       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LD SP,IX       2  DD F9           3   10     2    ------  SP=IX
// LD SP,IY       2  FD F9           3   10     2    ------  SP=IY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_ld_sp_xy       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LDD            2  ED A8           5   16     4    --0*0-  [DE]=[HL],HL-=1,DE-=1,BC-=1
// LDI            2  ED A0           5   16     4    --0*0-  [DE]=[HL],HL+=1,DE+=1,BC-=1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_ldd            CC_4_NOP
#define cc_ldi            CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// LDDR           2  ED B8           6/5 21/16  5/4  --000-  LDD until BC=0
// LDIR           2  ED B0           6/5 21/16  5/4  --000-  LDI until BC=0
//+----------------------------------------------------------------------------
// if BC<>0 => 6 NOP, if BC=0 => 5 NOP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_lddr           CC_4_NOP
#define cc_ldir           CC_4_NOP
#define cc_ex_lddr        CC_1_NOP
#define cc_ex_ldir        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// NEG            2  ED 44           2    8     2    ***V1*  A=-A
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_neg            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// NOP            1  00              1    4     1    ------
//+----------------------------------------------------------------------------
#define cc_nop            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OR (HL)        1  B6              2    7     2    ***P00  A=Av[HL]
//+----------------------------------------------------------------------------
#define cc_or_mhl         CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OR (IX+n)      3  DD B6 XX        5   19     5    ***P00  A=Av[IX+n]
// OR (IY+n)      3  FD B6 XX        5   19     5    ***P00  A=Av[IY+n]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_or_mxy         CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OR B           1  B0              1    4     1    ***P00  A=AvB
// OR C           1  B1              1    4     1    ***P00  A=AvC
// OR D           1  B2              1    4     1    ***P00  A=AvD
// OR E           1  B3              1    4     1    ***P00  A=AvE
// OR H           1  B4              1    4     1    ***P00  A=AvH
// OR L           1  B5              1    4     1    ***P00  A=AvL
// OR A           1  B7              1    4     1    ***P00  A=AvA
//+----------------------------------------------------------------------------
#define cc_or_b           CC_1_NOP
#define cc_or_c           CC_1_NOP
#define cc_or_d           CC_1_NOP
#define cc_or_e           CC_1_NOP
#define cc_or_h           CC_1_NOP
#define cc_or_l           CC_1_NOP
#define cc_or_a           CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// OR IXH         2  DD B4           2    9     ***P00  A=AvIXH
// OR IYH         2  FD B4           2    9     ***P00  A=AvIYH
// OR IXL         2  DD B5           2    9     ***P00  A=AvIXL
// OR IYL         2  FD B5           2    9     ***P00  A=AvIYL
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_or_xyh         CC_1_NOP
#define cc_or_xyl         CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OR N           2  F6 XX           2    7     2    ***P00  A=AvN
//+----------------------------------------------------------------------------
#define cc_or_byte        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OTDR           2  ED BB           6/5 21/16  5/4  01*?1-  OUTD until B=0
//+----------------------------------------------------------------------------
// if B<>0 => 6 NOP, if B=0 => 5 NOP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_otdr           CC_3_NOP
#define cc_otdr_          CC_1_NOP
#define cc_ex_otdr        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OTIR           2  ED B3           6/5 21/16  5/4  01*?1-  OUTI until B=0
//+----------------------------------------------------------------------------
// if B<>0 => 6 NOP, if B=0 => 5 NOP
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_otir           CC_3_NOP
#define cc_otir_          CC_1_NOP
#define cc_ex_otir        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OUT (C),B      2  ED 41           4   12     3    ------  [C]=B
// OUT (C),C      2  ED 49           4   12     3    ------  [C]=C
// OUT (C),D      2  ED 51           4   12     3    ------  [C]=D
// OUT (C),E      2  ED 59           4   12     3    ------  [C]=E
// OUT (C),H      2  ED 61           4   12     3    ------  [C]=H
// OUT (C),L      2  ED 69           4   12     3    ------  [C]=L
// OUT (C),A      2  ED 79           4   12     3    ------  [C]=A
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_out_c_r        CC_2_NOP
#define cc_out_c_r_       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// OUT (C),0      2  ED 71           4   12     ------
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_out_c_0        CC_2_NOP
//#define cc_out_c_r_       CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OUT (N),A      2  D3 XX           3   11     3    ------  [N]=A
//+----------------------------------------------------------------------------
#define cc_outa           CC_2_NOP
#define cc_outa_          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OUTD           2  ED AB           5   16     4    ***?1-  [C]=[HL],HL=HL-1,B=B-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_outd           CC_3_NOP
#define cc_outd_          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// OUTI           2  ED A3           5   16     4    ***?1-  [C]=[HL],HL=HL+1,B=B-1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_outi           CC_3_NOP
#define cc_outi_          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// POP BC         1  C1              3   10     3    ------  C=[SP],SP+,B=[SP],SP+
// POP DE         1  D1              3   10     3    ------  E=[SP],SP+,D=[SP],SP+
// POP HL         1  E1              3   10     3    ------  L=[SP],SP+,H=[SP],SP+
// POP AF         1  F1              3   10     3    ******  F=[SP],SP+,A=[SP],SP+
//+----------------------------------------------------------------------------
#define cc_pop_bc         CC_3_NOP
#define cc_pop_de         CC_3_NOP
#define cc_pop_hl         CC_3_NOP
#define cc_pop_af         CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// POP IX         2  DD E1           5   14     4    ------  IX=[SP,SP+1],SP+,SP+
// POP IY         2  FD E1           5   14     4    ------  IY=[SP,SP+1],SP+,SP+
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#ifndef Z80_NOP_TIMINGS
#define cc_pop_xy         12
#else /* Z80_NOP_TIMINGS */
#define cc_pop_xy         CC_4_NOP
#endif /* Z80_NOP_TIMINGS */

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// PUSH BC        1  C5              4   11     3    ------  -SP,[SP]=B,-SP,[SP]=C
// PUSH DE        1  D5              4   11     3    ------  -SP,[SP]=D,-SP,[SP]=E
// PUSH HL        1  E5              4   11     3    ------  -SP,[SP]=H,-SP,[SP]=L
// PUSH AF        1  F5              4   11     3    ------  -SP,[SP]=A,-SP,[SP]=F
//+----------------------------------------------------------------------------
#define cc_push_bc        CC_4_NOP
#define cc_push_de        CC_4_NOP
#define cc_push_hl        CC_4_NOP
#define cc_push_af        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// PUSH IX        2  DD E5           5   15     4    ------  -SP,-SP,[SP,SP+1]=IX
// PUSH IY        2  FD E5           5   15     4    ------  -SP,-SP,[SP,SP+1]=IY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_push_xy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RES 0,(HL)     2  CB 86           4   15     4    ------  [HL]=[HL]&{~2^0}
// RES 1,(HL)     2  CB 8E           4   15     4    ------  [HL]=[HL]&{~2^1}
// RES 2,(HL)     2  CB 96           4   15     4    ------  [HL]=[HL]&{~2^2}
// RES 3,(HL)     2  CB 9E           4   15     4    ------  [HL]=[HL]&{~2^3}
// RES 4,(HL)     2  CB A6           4   15     4    ------  [HL]=[HL]&{~2^4}
// RES 5,(HL)     2  CB AE           4   15     4    ------  [HL]=[HL]&{~2^5}
// RES 6,(HL)     2  CB B6           4   15     4    ------  [HL]=[HL]&{~2^6}
// RES 7,(HL)     2  CB BE           4   15     4    ------  [HL]=[HL]&{~2^7}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_res_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RES b,(IX+n)   4  DD CB XX 86+8*b 7   23     6    ------  [IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 86+8*b 7   23     6    ------  [IY+n]=[IY+n]&{~2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_res_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RES b,(IX+n)   4  DD CB XX 80+8*b 7   23     6    ------  B=[IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 80+8*b 7   23     6    ------  B=[IY+n]=[IY+n]&{~2^b}
// RES b,(IX+n)   4  DD CB XX 81+8*b 7   23     6    ------  C=[IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 81+8*b 7   23     6    ------  C=[IY+n]=[IY+n]&{~2^b}
// RES b,(IX+n)   4  DD CB XX 82+8*b 7   23     6    ------  D=[IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 82+8*b 7   23     6    ------  D=[IY+n]=[IY+n]&{~2^b}
// RES b,(IX+n)   4  DD CB XX 83+8*b 7   23     6    ------  E=[IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 83+8*b 7   23     6    ------  E=[IY+n]=[IY+n]&{~2^b}
// RES b,(IX+n)   4  DD CB XX 84+8*b 7   23     6    ------  H=[IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 84+8*b 7   23     6    ------  H=[IY+n]=[IY+n]&{~2^b}
// RES b,(IX+n)   4  DD CB XX 85+8*b 7   23     6    ------  L=[IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 85+8*b 7   23     6    ------  L=[IY+n]=[IY+n]&{~2^b}
// RES b,(IX+n)   4  DD CB XX 87+8*b 7   23     6    ------  A=[IX+n]=[IX+n]&{~2^b}
// RES b,(IY+n)   4  FD CB XX 87+8*b 7   23     6    ------  A=[IY+n]=[IY+n]&{~2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_res_mxy_b      CC_5_NOP
#define cc_res_mxy_c      CC_5_NOP
#define cc_res_mxy_d      CC_5_NOP
#define cc_res_mxy_e      CC_5_NOP
#define cc_res_mxy_h      CC_5_NOP
#define cc_res_mxy_l      CC_5_NOP
#define cc_res_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RES b,B        2  CB 80+8*b       2    8     2    ------  B=B&{~2^b}
// RES b,C        2  CB 81+8*b       2    8     2    ------  C=C&{~2^b}
// RES b,D        2  CB 82+8*b       2    8     2    ------  D=D&{~2^b}
// RES b,E        2  CB 83+8*b       2    8     2    ------  E=E&{~2^b}
// RES b,H        2  CB 84+8*b       2    8     2    ------  H=H&{~2^b}
// RES b,L        2  CB 85+8*b       2    8     2    ------  L=L&{~2^b}
// RES b,A        2  CB 87+8*b       2    8     2    ------  A=A&{~2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_res_b          CC_1_NOP
#define cc_res_c          CC_1_NOP
#define cc_res_d          CC_1_NOP
#define cc_res_e          CC_1_NOP
#define cc_res_h          CC_1_NOP
#define cc_res_l          CC_1_NOP
#define cc_res_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RET            1  C9              3   10     3    ------  PC=[SP,SP+1],SP+,SP+
//+----------------------------------------------------------------------------
#define cc_ret            CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RET C          1  D8              4/2 11/5   3/1  ------  If CY then PC=[SP,SP+1],SP+=2
// RET M          1  F8              4/2 11/5   3/1  ------  If S then PC=[SP,SP+1],SP+=2
// RET NC         1  D0              4/2 11/5   3/1  ------  If !CY then PC=[SP,SP+1],SP+=2
// RET NZ         1  C0              4/2 11/5   3/1  ------  If !Z then PC=[SP,SP+1],SP+=2
// RET P          1  F0              4/2 11/5   3/1  ------  If !S then PC=[SP,SP+1],SP+=2
// RET PE         1  E8              4/2 11/5   3/1  ------  If !P then PC=[SP,SP+1],SP+=2
// RET PO         1  E0              4/2 11/5   3/1  ------  If P then PC=[SP,SP+1],SP+=2
// RET Z          1  C8              4/2 11/5   3/1  ------  If Z then PC=[SP,SP+1],SP+=2
//+----------------------------------------------------------------------------
#define cc_ret_c          CC_2_NOP
#define cc_ret_m          CC_2_NOP
#define cc_ret_nc         CC_2_NOP
#define cc_ret_nz         CC_2_NOP
#define cc_ret_p          CC_2_NOP
#define cc_ret_pe         CC_2_NOP
#define cc_ret_po         CC_2_NOP
#define cc_ret_z          CC_2_NOP
#define cc_ex_ret_c       CC_2_NOP
#define cc_ex_ret_m       CC_2_NOP
#define cc_ex_ret_nc      CC_2_NOP
#define cc_ex_ret_nz      CC_2_NOP
#define cc_ex_ret_p       CC_2_NOP
#define cc_ex_ret_pe      CC_2_NOP
#define cc_ex_ret_po      CC_2_NOP
#define cc_ex_ret_z       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RETI           2  ED 4D           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
// RETI ?         2  ED 5D           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
// RETI ?         2  ED 6D           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
// RETI ?         2  ED 7D           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_reti           CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RETN           2  ED 45           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
// RETN ?         2  ED 55           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
// RETN ?         2  ED 65           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
// RETN ?         2  ED 75           4   14     4    ------  PC=[SP,SP+1],SP+,SP+
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_retn           CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RL (HL)        2  CB 16           4   15     4    **0P0*  [HL]={CY,[HL]}<<CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rl_mhl         CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RL (IX+n)      4  DD CB XX 16     7   23     6    **0P0*  [IX+n]={CY,[IX+n]}<<CY
// RL (IY+n)      4  FD CB XX 16     7   23     6    **0P0*  [IY+n]={CY,[IY+n]}<<CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rl_mxy         CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RL (IX+n),B    4  DD CB XX 10     7   23     6    **0P0*  B=[IX+n]={CY,[IX+n]}<<CY
// RL (IY+n),B    4  FD CB XX 10     7   23     6    **0P0*  B=[IY+n]={CY,[IY+n]}<<CY
// RL (IX+n),C    4  DD CB XX 11     7   23     6    **0P0*  C=[IX+n]={CY,[IX+n]}<<CY
// RL (IY+n),C    4  FD CB XX 11     7   23     6    **0P0*  C=[IY+n]={CY,[IY+n]}<<CY
// RL (IX+n),D    4  DD CB XX 12     7   23     6    **0P0*  D=[IX+n]={CY,[IX+n]}<<CY
// RL (IY+n),D    4  FD CB XX 12     7   23     6    **0P0*  D=[IY+n]={CY,[IY+n]}<<CY
// RL (IX+n),E    4  DD CB XX 13     7   23     6    **0P0*  E=[IX+n]={CY,[IX+n]}<<CY
// RL (IY+n),E    4  FD CB XX 13     7   23     6    **0P0*  E=[IY+n]={CY,[IY+n]}<<CY
// RL (IX+n),H    4  DD CB XX 14     7   23     6    **0P0*  H=[IX+n]={CY,[IX+n]}<<CY
// RL (IY+n),H    4  FD CB XX 14     7   23     6    **0P0*  H=[IY+n]={CY,[IY+n]}<<CY
// RL (IX+n),L    4  DD CB XX 15     7   23     6    **0P0*  L=[IX+n]={CY,[IX+n]}<<CY
// RL (IY+n),L    4  FD CB XX 15     7   23     6    **0P0*  L=[IY+n]={CY,[IY+n]}<<CY
// RL (IX+n),A    4  DD CB XX 17     7   23     6    **0P0*  A=[IX+n]={CY,[IX+n]}<<CY
// RL (IY+n),A    4  FD CB XX 17     7   23     6    **0P0*  A=[IY+n]={CY,[IY+n]}<<CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rl_mxy_b       CC_5_NOP
#define cc_rl_mxy_c       CC_5_NOP
#define cc_rl_mxy_d       CC_5_NOP
#define cc_rl_mxy_e       CC_5_NOP
#define cc_rl_mxy_h       CC_5_NOP
#define cc_rl_mxy_l       CC_5_NOP
#define cc_rl_mxy_a       CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RL B           2  CB 10           2   8     2    **0P0*  B={CY,B}<<CY
// RL C           2  CB 11           2   8     2    **0P0*  C={CY,C}<<CY
// RL D           2  CB 12           2   8     2    **0P0*  D={CY,D}<<CY
// RL E           2  CB 13           2   8     2    **0P0*  E={CY,E}<<CY
// RL H           2  CB 14           2   8     2    **0P0*  H={CY,H}<<CY
// RL L           2  CB 15           2   8     2    **0P0*  L={CY,L}<<CY
// RL A           2  CB 17           2   8     2    **0P0*  A={CY,A}<<CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rl_b           CC_1_NOP
#define cc_rl_c           CC_1_NOP
#define cc_rl_d           CC_1_NOP
#define cc_rl_e           CC_1_NOP
#define cc_rl_h           CC_1_NOP
#define cc_rl_l           CC_1_NOP
#define cc_rl_a           CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RLA            1  17              1    4     1    --0-0*  A={CY,A}<<CY
//+----------------------------------------------------------------------------
#define cc_rla            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RLC (HL)       2  CB 06           4   15     4    **0P0*  [HL]={[HL]}<<
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rlc_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RLC (IX+n)     4  DD CB XX 06     7   23     6    **0P0*  [IX+n]={[IX+n]}<<
// RLC (IY+n)     4  FD CB XX 06     7   23     6    **0P0*  [IY+n]={[IY+n]}<<
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rlc_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RLC (IX+n),B   4  DD CB XX 00     7   23     6    **0P0*  B=[IX+n]={[IX+n]}<<
// RLC (IY+n),B   4  FD CB XX 00     7   23     6    **0P0*  B=[IX+n]={[IX+n]}<<
// RLC (IX+n),C   4  DD CB XX 01     7   23     6    **0P0*  C=[IX+n]={[IX+n]}<<
// RLC (IY+n),C   4  FD CB XX 01     7   23     6    **0P0*  C=[IX+n]={[IX+n]}<<
// RLC (IX+n),D   4  DD CB XX 02     7   23     6    **0P0*  D=[IX+n]={[IX+n]}<<
// RLC (IY+n),D   4  FD CB XX 02     7   23     6    **0P0*  D=[IX+n]={[IX+n]}<<
// RLC (IX+n),E   4  DD CB XX 03     7   23     6    **0P0*  E=[IX+n]={[IX+n]}<<
// RLC (IY+n),E   4  FD CB XX 03     7   23     6    **0P0*  E=[IX+n]={[IX+n]}<<
// RLC (IX+n),H   4  DD CB XX 04     7   23     6    **0P0*  H=[IX+n]={[IX+n]}<<
// RLC (IY+n),H   4  FD CB XX 04     7   23     6    **0P0*  H=[IX+n]={[IX+n]}<<
// RLC (IX+n),L   4  DD CB XX 05     7   23     6    **0P0*  L=[IX+n]={[IX+n]}<<
// RLC (IY+n),L   4  FD CB XX 05     7   23     6    **0P0*  L=[IX+n]={[IX+n]}<<
// RLC (IX+n),A   4  DD CB XX 07     7   23     6    **0P0*  A=[IX+n]={[IX+n]}<<
// RLC (IY+n),A   4  FD CB XX 07     7   23     6    **0P0*  A=[IX+n]={[IX+n]}<<
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rlc_mxy_b      CC_5_NOP
#define cc_rlc_mxy_c      CC_5_NOP
#define cc_rlc_mxy_d      CC_5_NOP
#define cc_rlc_mxy_e      CC_5_NOP
#define cc_rlc_mxy_h      CC_5_NOP
#define cc_rlc_mxy_l      CC_5_NOP
#define cc_rlc_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RLC B          2  CB 00           2   8     2    **0P0*  B={B}<<
// RLC C          2  CB 01           2   8     2    **0P0*  C={C}<<
// RLC D          2  CB 02           2   8     2    **0P0*  D={D}<<
// RLC E          2  CB 03           2   8     2    **0P0*  E={E}<<
// RLC H          2  CB 04           2   8     2    **0P0*  H={H}<<
// RLC L          2  CB 05           2   8     2    **0P0*  L={L}<<
// RLC A          2  CB 07           2   8     2    **0P0*  A={A}<<
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rlc_b          CC_1_NOP
#define cc_rlc_c          CC_1_NOP
#define cc_rlc_d          CC_1_NOP
#define cc_rlc_e          CC_1_NOP
#define cc_rlc_h          CC_1_NOP
#define cc_rlc_l          CC_1_NOP
#define cc_rlc_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RLCA           1  07              1    4     1    --0-0*  A={A}<<
//+----------------------------------------------------------------------------
#define cc_rlca           CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RLD            2  ED 6F           5   18     5    **0P0-  {A,[HL]}={A,[HL]}<-4
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_rld            CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RR (HL)        2  CB 1E           4   15     4    **0P0*  [HL]=CY>>{CY,[HL]}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rr_mhl         CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RR (IX+n)      4  DD CB XX 1E     7   23     6    **0P0*  [IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n)      4  FD CB XX 1E     7   23     6    **0P0*  [IY+n]=CY>>{CY,[IY+n]}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rr_mxy         CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RR (IX+n),B    4  DD CB XX 18     7   23     6    **0P0*  B=[IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n),B    4  FD CB XX 18     7   23     6    **0P0*  B=[IY+n]=CY>>{CY,[IY+n]}
// RR (IX+n),C    4  DD CB XX 19     7   23     6    **0P0*  C=[IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n),C    4  FD CB XX 19     7   23     6    **0P0*  C=[IY+n]=CY>>{CY,[IY+n]}
// RR (IX+n),D    4  DD CB XX 1A     7   23     6    **0P0*  D=[IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n),D    4  FD CB XX 1A     7   23     6    **0P0*  D=[IY+n]=CY>>{CY,[IY+n]}
// RR (IX+n),E    4  DD CB XX 1B     7   23     6    **0P0*  E=[IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n),E    4  FD CB XX 1B     7   23     6    **0P0*  E=[IY+n]=CY>>{CY,[IY+n]}
// RR (IX+n),H    4  DD CB XX 1C     7   23     6    **0P0*  H=[IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n),H    4  FD CB XX 1C     7   23     6    **0P0*  H=[IY+n]=CY>>{CY,[IY+n]}
// RR (IX+n),L    4  DD CB XX 1D     7   23     6    **0P0*  L=[IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n),L    4  FD CB XX 1D     7   23     6    **0P0*  L=[IY+n]=CY>>{CY,[IY+n]}
// RR (IX+n),A    4  DD CB XX 1F     7   23     6    **0P0*  A=[IX+n]=CY>>{CY,[IX+n]}
// RR (IY+n),A    4  FD CB XX 1F     7   23     6    **0P0*  A=[IY+n]=CY>>{CY,[IY+n]}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rr_mxy_b       CC_5_NOP
#define cc_rr_mxy_c       CC_5_NOP
#define cc_rr_mxy_d       CC_5_NOP
#define cc_rr_mxy_e       CC_5_NOP
#define cc_rr_mxy_h       CC_5_NOP
#define cc_rr_mxy_l       CC_5_NOP
#define cc_rr_mxy_a       CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RR B           2  CB 18           2    8     2    **0P0*  B=CY>>{CY,B}
// RR C           2  CB 19           2    8     2    **0P0*  C=CY>>{CY,C}
// RR D           2  CB 1A           2    8     2    **0P0*  D=CY>>{CY,D}
// RR E           2  CB 1B           2    8     2    **0P0*  E=CY>>{CY,E}
// RR H           2  CB 1C           2    8     2    **0P0*  H=CY>>{CY,H}
// RR L           2  CB 1D           2    8     2    **0P0*  L=CY>>{CY,L}
// RR A           2  CB 1F           2    8     2    **0P0*  A=CY>>{CY,A}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rr_b           CC_1_NOP
#define cc_rr_c           CC_1_NOP
#define cc_rr_d           CC_1_NOP
#define cc_rr_e           CC_1_NOP
#define cc_rr_h           CC_1_NOP
#define cc_rr_l           CC_1_NOP
#define cc_rr_a           CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RRA            1  1F              1    4     1    --0-0*  A=CY>>{CY,A}
//+----------------------------------------------------------------------------
#define cc_rra            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// RRC (HL)       2  CB 0E           4   15     4    **0P0*  [HL]=>>{[HL]}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rrc_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RRC (IX+n)     4  DD CB XX 0E     7   23     6    **0P0*  [IX+n]=>>{[IX+n]}
// RRC (IY+n)     4  FD CB XX 0E     7   23     6    **0P0*  [IY+n]=>>{[IY+n]}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rrc_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RRC (IX+n),B   4  DD CB XX 08     7   23     6    **0P0*  B=[IX+n]=>>{[IX+n]}
// RRC (IY+n),B   4  FD CB XX 08     7   23     6    **0P0*  B=[IY+n]=>>{[IY+n]}
// RRC (IX+n),C   4  DD CB XX 09     7   23     6    **0P0*  C=[IX+n]=>>{[IX+n]}
// RRC (IY+n),C   4  FD CB XX 09     7   23     6    **0P0*  C=[IY+n]=>>{[IY+n]}
// RRC (IX+n),D   4  DD CB XX 0A     7   23     6    **0P0*  D=[IX+n]=>>{[IX+n]}
// RRC (IY+n),D   4  FD CB XX 0A     7   23     6    **0P0*  D=[IY+n]=>>{[IY+n]}
// RRC (IX+n),E   4  DD CB XX 0B     7   23     6    **0P0*  E=[IX+n]=>>{[IX+n]}
// RRC (IY+n),E   4  FD CB XX 0B     7   23     6    **0P0*  E=[IY+n]=>>{[IY+n]}
// RRC (IX+n),H   4  DD CB XX 0C     7   23     6    **0P0*  H=[IX+n]=>>{[IX+n]}
// RRC (IY+n),H   4  FD CB XX 0C     7   23     6    **0P0*  H=[IY+n]=>>{[IY+n]}
// RRC (IX+n),L   4  DD CB XX 0D     7   23     6    **0P0*  L=[IX+n]=>>{[IX+n]}
// RRC (IY+n),L   4  FD CB XX 0D     7   23     6    **0P0*  L=[IY+n]=>>{[IY+n]}
// RRC (IX+n),A   4  DD CB XX 0F     7   23     6    **0P0*  A=[IX+n]=>>{[IX+n]}
// RRC (IY+n),A   4  FD CB XX 0F     7   23     6    **0P0*  A=[IY+n]=>>{[IY+n]}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_rrc_mxy_b      CC_5_NOP
#define cc_rrc_mxy_c      CC_5_NOP
#define cc_rrc_mxy_d      CC_5_NOP
#define cc_rrc_mxy_e      CC_5_NOP
#define cc_rrc_mxy_h      CC_5_NOP
#define cc_rrc_mxy_l      CC_5_NOP
#define cc_rrc_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// RRC B          2  CB 08           2    8     2    **0P0*  B=>>{B}
// RRC C          2  CB 09           2    8     2    **0P0*  C=>>{C}
// RRC D          2  CB 0A           2    8     2    **0P0*  D=>>{D}
// RRC E          2  CB 0B           2    8     2    **0P0*  E=>>{E}
// RRC H          2  CB 0C           2    8     2    **0P0*  H=>>{H}
// RRC L          2  CB 0D           2    8     2    **0P0*  L=>>{L}
// RRC A          2  CB 0F           2    8     2    **0P0*  A=>>{A}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_rrc_b          CC_1_NOP
#define cc_rrc_c          CC_1_NOP
#define cc_rrc_d          CC_1_NOP
#define cc_rrc_e          CC_1_NOP
#define cc_rrc_h          CC_1_NOP
#define cc_rrc_l          CC_1_NOP
#define cc_rrc_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// RRCA           1  0F              1    4     1    --0-0*  A=>>{A}
//+----------------------------------------------------------------------------
#define cc_rrca           CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// RRD            2  ED 67           5   18     5    **0P0-  {A,[HL]}=4->{A,[HL]}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_rrd            CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// RST 0          1  C7              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=00
// RST 8H         1  CF              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=08
// RST 10H        1  D7              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=10
// RST 18H        1  DF              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=18
// RST 20H        1  E7              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=20
// RST 28H        1  EF              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=28
// RST 30H        1  F7              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=30
// RST 38H        1  FF              4   11     3    ------  -SP,-SP,[SP+1,SP]=PC,PC=38
//+----------------------------------------------------------------------------
#define cc_rst00          CC_4_NOP
#define cc_rst08          CC_4_NOP
#define cc_rst10          CC_4_NOP
#define cc_rst18          CC_4_NOP
#define cc_rst20          CC_4_NOP
#define cc_rst28          CC_4_NOP
#define cc_rst30          CC_4_NOP
#define cc_rst38          CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SBC A,(HL)     1  9E              2    7     2    ***V1*  A=A-[HL]-CY
//+----------------------------------------------------------------------------
#define cc_sbc_mhl        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SBC A,(IX+n)   3  DD 9E XX        5   19     5    ***V1*  A=A-[IX+n]-CY
// SBC A,(IY+n)   3  FD 9E XX        5   19     5    ***V1*  A=A-[IY+n]-CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_sbc_mxy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SBC A,N        2  DE XX           2    7     2    ***V1*  A=A-r-CY
//+----------------------------------------------------------------------------
#define cc_sbc_byte       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SBC A,B        1  98              1    4     1    ***V1*  A=A-B-CY
// SBC A,C        1  99              1    4     1    ***V1*  A=A-C-CY
// SBC A,D        1  9A              1    4     1    ***V1*  A=A-D-CY
// SBC A,E        1  9B              1    4     1    ***V1*  A=A-E-CY
// SBC A,H        1  9C              1    4     1    ***V1*  A=A-H-CY
// SBC A,L        1  9D              1    4     1    ***V1*  A=A-L-CY
// SBC A,A        1  9F              1    4     1    ***V1*  A=A-A-CY
//+----------------------------------------------------------------------------
#define cc_sbc_b          CC_1_NOP
#define cc_sbc_c          CC_1_NOP
#define cc_sbc_d          CC_1_NOP
#define cc_sbc_e          CC_1_NOP
#define cc_sbc_h          CC_1_NOP
#define cc_sbc_l          CC_1_NOP
#define cc_sbc_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SBC A,IXH      2  DD 9C           2    9     ***V1*  A=A-IXH-CY
// SBC A,IYH      2  FD 9C           2    9     ***V1*  A=A-IYH-CY
// SBC A,IXL      2  DD 9D           2    9     ***V1*  A=A-IXL-CY
// SBC A,IYL      2  FD 9D           2    9     ***V1*  A=A-IYL-CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_sbc_xyh        CC_1_NOP
#define cc_sbc_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SBC HL,BC      2  ED 42           4   15     4    ***V1*  HL=HL-BC-CY
// SBC HL,DE      2  ED 52           4   15     4    ***V1*  HL=HL-DE-CY
// SBC HL,HL      2  ED 62           4   15     4    ***V1*  HL=HL-HL-CY
// SBC HL,SP      2  ED 72           4   15     4    ***V1*  HL=HL-SP-CY
//+----------------------------------------------------------------------------
// 1 NOP added by prefix ED
#define cc_sbc_hl_bc      CC_3_NOP
#define cc_sbc_hl_de      CC_3_NOP
#define cc_sbc_hl_hl      CC_3_NOP
#define cc_sbc_hl_sp      CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SCF            1  37              1    4     1    --0-01  CY=1
//+----------------------------------------------------------------------------
#define cc_scf            CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SET 0,(HL)     2  CB C6           4   15     4    ------  [HL]=[HL]v{2^0}
// SET 1,(HL)     2  CB CE           4   15     4    ------  [HL]=[HL]v{2^1}
// SET 2,(HL)     2  CB D6           4   15     4    ------  [HL]=[HL]v{2^2}
// SET 3,(HL)     2  CB DE           4   15     4    ------  [HL]=[HL]v{2^3}
// SET 4,(HL)     2  CB E6           4   15     4    ------  [HL]=[HL]v{2^4}
// SET 5,(HL)     2  CB EE           4   15     4    ------  [HL]=[HL]v{2^5}
// SET 6,(HL)     2  CB F6           4   15     4    ------  [HL]=[HL]v{2^6}
// SET 7,(HL)     2  CB FE           4   15     4    ------  [HL]=[HL]v{2^7}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_set_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SET b,(IX+n)   4  DD CB XX C6+8*b 7   23     6    ------  [IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C6+8*b 7   23     6    ------  [IY+n]=[IY+n]v{2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_set_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SET b,(IX+n)   4  DD CB XX C0+8*b 7   23     6    ------  B=[IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C0+8*b 7   23     6    ------  B=[IY+n]=[IY+n]v{2^b}
// SET b,(IX+n)   4  DD CB XX C1+8*b 7   23     6    ------  C=[IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C1+8*b 7   23     6    ------  C=[IY+n]=[IY+n]v{2^b}
// SET b,(IX+n)   4  DD CB XX C2+8*b 7   23     6    ------  D=[IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C2+8*b 7   23     6    ------  D=[IY+n]=[IY+n]v{2^b}
// SET b,(IX+n)   4  DD CB XX C3+8*b 7   23     6    ------  E=[IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C3+8*b 7   23     6    ------  E=[IY+n]=[IY+n]v{2^b}
// SET b,(IX+n)   4  DD CB XX C4+8*b 7   23     6    ------  H=[IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C4+8*b 7   23     6    ------  H=[IY+n]=[IY+n]v{2^b}
// SET b,(IX+n)   4  DD CB XX C5+8*b 7   23     6    ------  L=[IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C5+8*b 7   23     6    ------  L=[IY+n]=[IY+n]v{2^b}
// SET b,(IX+n)   4  DD CB XX C7+8*b 7   23     6    ------  A=[IX+n]=[IX+n]v{2^b}
// SET b,(IY+n)   4  FD CB XX C7+8*b 7   23     6    ------  A=[IY+n]=[IY+n]v{2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_set_mxy_b      CC_5_NOP
#define cc_set_mxy_c      CC_5_NOP
#define cc_set_mxy_d      CC_5_NOP
#define cc_set_mxy_e      CC_5_NOP
#define cc_set_mxy_h      CC_5_NOP
#define cc_set_mxy_l      CC_5_NOP
#define cc_set_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SET b,B        2  CB C0+8*b       2    8     2    ------  B=Bv{2^b}
// SET b,C        2  CB C1+8*b       2    8     2    ------  C=Cv{2^b}
// SET b,D        2  CB C2+8*b       2    8     2    ------  D=Dv{2^b}
// SET b,E        2  CB C3+8*b       2    8     2    ------  E=Ev{2^b}
// SET b,H        2  CB C4+8*b       2    8     2    ------  H=Hv{2^b}
// SET b,L        2  CB C5+8*b       2    8     2    ------  L=Lv{2^b}
// SET b,A        2  CB C7+8*b       2    8     2    ------  A=Av{2^b}
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_set_b          CC_1_NOP
#define cc_set_c          CC_1_NOP
#define cc_set_d          CC_1_NOP
#define cc_set_e          CC_1_NOP
#define cc_set_h          CC_1_NOP
#define cc_set_l          CC_1_NOP
#define cc_set_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SLA (HL)       2  CB 26           4   15     4    **0P0*  [HL]=[HL]*2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_sla_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SLA (IX+n)     4  DD CB XX 26     7   23     6    **0P0*  [IX+n]=[IX+n]*2
// SLA (IY+n)     4  FD CB XX 26     7   23     6    **0P0*  [IY+n]=[IY+n]*2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_sla_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SLA (IX+n),B   4  DD CB XX 20     7   23     6    **0P0*  B=[IX+n]=[IX+n]*2
// SLA (IY+n),B   4  FD CB XX 20     7   23     6    **0P0*  B=[IY+n]=[IY+n]*2
// SLA (IX+n),C   4  DD CB XX 21     7   23     6    **0P0*  C=[IX+n]=[IX+n]*2
// SLA (IY+n),C   4  FD CB XX 21     7   23     6    **0P0*  C=[IY+n]=[IY+n]*2
// SLA (IX+n),D   4  DD CB XX 22     7   23     6    **0P0*  D=[IX+n]=[IX+n]*2
// SLA (IY+n),D   4  FD CB XX 22     7   23     6    **0P0*  D=[IY+n]=[IY+n]*2
// SLA (IX+n),E   4  DD CB XX 23     7   23     6    **0P0*  E=[IX+n]=[IX+n]*2
// SLA (IY+n),E   4  FD CB XX 23     7   23     6    **0P0*  E=[IY+n]=[IY+n]*2
// SLA (IX+n),H   4  DD CB XX 24     7   23     6    **0P0*  H=[IX+n]=[IX+n]*2
// SLA (IY+n),H   4  FD CB XX 24     7   23     6    **0P0*  H=[IY+n]=[IY+n]*2
// SLA (IX+n),L   4  DD CB XX 25     7   23     6    **0P0*  L=[IX+n]=[IX+n]*2
// SLA (IY+n),L   4  FD CB XX 25     7   23     6    **0P0*  L=[IY+n]=[IY+n]*2
// SLA (IX+n),A   4  DD CB XX 27     7   23     6    **0P0*  A=[IX+n]=[IX+n]*2
// SLA (IY+n),A   4  FD CB XX 27     7   23     6    **0P0*  A=[IY+n]=[IY+n]*2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_sla_mxy_b      CC_5_NOP
#define cc_sla_mxy_c      CC_5_NOP
#define cc_sla_mxy_d      CC_5_NOP
#define cc_sla_mxy_e      CC_5_NOP
#define cc_sla_mxy_h      CC_5_NOP
#define cc_sla_mxy_l      CC_5_NOP
#define cc_sla_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SLA B          2  CB 20           2    8     2    **0P0*  B=B*2
// SLA C          2  CB 21           2    8     2    **0P0*  C=C*2
// SLA D          2  CB 22           2    8     2    **0P0*  D=D*2
// SLA E          2  CB 23           2    8     2    **0P0*  E=E*2
// SLA H          2  CB 24           2    8     2    **0P0*  H=H*2
// SLA L          2  CB 25           2    8     2    **0P0*  L=L*2
// SLA A          2  CB 27           2    8     2    **0P0*  A=A*2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_sla_b          CC_1_NOP
#define cc_sla_c          CC_1_NOP
#define cc_sla_d          CC_1_NOP
#define cc_sla_e          CC_1_NOP
#define cc_sla_h          CC_1_NOP
#define cc_sla_l          CC_1_NOP
#define cc_sla_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+------------------------------------
// SLL (HL)       2  CB 36           4   15     **0P0*  [HL]=[HL]*2+1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_sll_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SLL (IX+n)     4  DD CB XX 36     7   23     **0P0*  [IX+n]=[IX+n]*2+1
// SLL (IY+n)     4  FD CB XX 36     7   23     **0P0*  [IY+n]=[IY+n]*2+1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_sll_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SLL (IX+n),B   4  DD CB XX 30     7   23     **0P0*  B=[IX+n]=[IX+n]*2+1
// SLL (IY+n),B   4  FD CB XX 30     7   23     **0P0*  B=[IY+n]=[IY+n]*2+1
// SLL (IX+n),C   4  DD CB XX 31     7   23     **0P0*  C=[IX+n]=[IX+n]*2+1
// SLL (IY+n),C   4  FD CB XX 31     7   23     **0P0*  C=[IY+n]=[IY+n]*2+1
// SLL (IX+n),D   4  DD CB XX 32     7   23     **0P0*  D=[IX+n]=[IX+n]*2+1
// SLL (IY+n),D   4  FD CB XX 32     7   23     **0P0*  D=[IY+n]=[IY+n]*2+1
// SLL (IX+n),E   4  DD CB XX 33     7   23     **0P0*  E=[IX+n]=[IX+n]*2+1
// SLL (IY+n),E   4  FD CB XX 33     7   23     **0P0*  E=[IY+n]=[IY+n]*2+1
// SLL (IX+n),H   4  DD CB XX 34     7   23     **0P0*  H=[IX+n]=[IX+n]*2+1
// SLL (IY+n),H   4  FD CB XX 34     7   23     **0P0*  H=[IY+n]=[IY+n]*2+1
// SLL (IX+n),L   4  DD CB XX 35     7   23     **0P0*  L=[IX+n]=[IX+n]*2+1
// SLL (IY+n),L   4  FD CB XX 35     7   23     **0P0*  L=[IY+n]=[IY+n]*2+1
// SLL (IX+n),A   4  DD CB XX 37     7   23     **0P0*  A=[IX+n]=[IX+n]*2+1
// SLL (IY+n),A   4  FD CB XX 37     7   23     **0P0*  A=[IY+n]=[IY+n]*2+1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_sll_mxy_b      CC_5_NOP
#define cc_sll_mxy_c      CC_5_NOP
#define cc_sll_mxy_d      CC_5_NOP
#define cc_sll_mxy_e      CC_5_NOP
#define cc_sll_mxy_h      CC_5_NOP
#define cc_sll_mxy_l      CC_5_NOP
#define cc_sll_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SLL B          2  CB 30           2    8     **0P0*  B=B*2+1
// SLL C          2  CB 31           2    8     **0P0*  C=C*2+1
// SLL D          2  CB 32           2    8     **0P0*  D=D*2+1
// SLL E          2  CB 33           2    8     **0P0*  E=E*2+1
// SLL H          2  CB 34           2    8     **0P0*  H=H*2+1
// SLL L          2  CB 35           2    8     **0P0*  L=L*2+1
// SLL A          2  CB 37           2    8     **0P0*  A=A*2+1
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_sll_b          CC_1_NOP
#define cc_sll_c          CC_1_NOP
#define cc_sll_d          CC_1_NOP
#define cc_sll_e          CC_1_NOP
#define cc_sll_h          CC_1_NOP
#define cc_sll_l          CC_1_NOP
#define cc_sll_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SRA (HL)       2  CB 2E           4   15     4   **0P0*  [HL]=(signed)[HL]/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_sra_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SRA (IX+n)     4  DD CB XX 2E     7   23     6    **0P0*  [IX+n]=(signed)[IX+n]/2
// SRA (IY+n)     4  FD CB XX 2E     7   23     6    **0P0*  [IY+n]=(signed)[IY+n]/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_sra_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SRA (IX+n),B   4  DD CB XX 28     7   23     6    **0P0*  B=[IX+n]=(signed)[IX+n]/2
// SRA (IY+n),B   4  FD CB XX 28     7   23     6    **0P0*  B=[IY+n]=(signed)[IY+n]/2
// SRA (IX+n),C   4  DD CB XX 29     7   23     6    **0P0*  C=[IX+n]=(signed)[IX+n]/2
// SRA (IY+n),C   4  FD CB XX 29     7   23     6    **0P0*  C=[IY+n]=(signed)[IY+n]/2
// SRA (IX+n),D   4  DD CB XX 2A     7   23     6    **0P0*  D=[IX+n]=(signed)[IX+n]/2
// SRA (IY+n),D   4  FD CB XX 2A     7   23     6    **0P0*  D=[IY+n]=(signed)[IY+n]/2
// SRA (IX+n),E   4  DD CB XX 2B     7   23     6    **0P0*  E=[IX+n]=(signed)[IX+n]/2
// SRA (IY+n),E   4  FD CB XX 2B     7   23     6    **0P0*  E=[IY+n]=(signed)[IY+n]/2
// SRA (IX+n),H   4  DD CB XX 2C     7   23     6    **0P0*  H=[IX+n]=(signed)[IX+n]/2
// SRA (IY+n),H   4  FD CB XX 2C     7   23     6    **0P0*  H=[IY+n]=(signed)[IY+n]/2
// SRA (IX+n),L   4  DD CB XX 2D     7   23     6    **0P0*  L=[IX+n]=(signed)[IX+n]/2
// SRA (IY+n),L   4  FD CB XX 2D     7   23     6    **0P0*  L=[IY+n]=(signed)[IY+n]/2
// SRA (IX+n),A   4  DD CB XX 2F     7   23     6    **0P0*  A=[IX+n]=(signed)[IX+n]/2
// SRA (IY+n),A   4  FD CB XX 2F     7   23     6    **0P0*  A=[IY+n]=(signed)[IY+n]/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_sra_mxy_b      CC_5_NOP
#define cc_sra_mxy_c      CC_5_NOP
#define cc_sra_mxy_d      CC_5_NOP
#define cc_sra_mxy_e      CC_5_NOP
#define cc_sra_mxy_h      CC_5_NOP
#define cc_sra_mxy_l      CC_5_NOP
#define cc_sra_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SRA B          2  CB 28           2    8     2    **0P0*  B=(signed)B/2
// SRA C          2  CB 29           2    8     2    **0P0*  C=(signed)C/2
// SRA D          2  CB 2A           2    8     2    **0P0*  D=(signed)D/2
// SRA E          2  CB 2B           2    8     2    **0P0*  E=(signed)E/2
// SRA H          2  CB 2C           2    8     2    **0P0*  H=(signed)H/2
// SRA L          2  CB 2D           2    8     2    **0P0*  L=(signed)L/2
// SRA A          2  CB 2F           2    8     2    **0P0*  A=(signed)A/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_sra_b          CC_1_NOP
#define cc_sra_c          CC_1_NOP
#define cc_sra_d          CC_1_NOP
#define cc_sra_e          CC_1_NOP
#define cc_sra_h          CC_1_NOP
#define cc_sra_l          CC_1_NOP
#define cc_sra_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SRL (HL)       2  CB 3E           4   15     4    **0P0*  [HL]=(unsigned)[HL]/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_srl_mhl        CC_3_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SRL (IX+n)     4  DD CB XX 3E     7   23     6    **0P0*  [IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n)     4  FD CB XX 3E     7   23     6    **0P0*  [IY+n]=(unsigned)[IY+n]/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_srl_mxy        CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SRL (IX+n),B   4  DD CB XX 38     7   23     6    **0P0*  B=[IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n),B   4  FD CB XX 38     7   23     6    **0P0*  B=[IY+n]=(unsigned)[IY+n]/2
// SRL (IX+n),C   4  DD CB XX 39     7   23     6    **0P0*  C=[IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n),C   4  FD CB XX 39     7   23     6    **0P0*  C=[IY+n]=(unsigned)[IY+n]/2
// SRL (IX+n),D   4  DD CB XX 3A     7   23     6    **0P0*  D=[IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n),D   4  FD CB XX 3A     7   23     6    **0P0*  D=[IY+n]=(unsigned)[IY+n]/2
// SRL (IX+n),E   4  DD CB XX 3B     7   23     6    **0P0*  E=[IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n),E   4  FD CB XX 3B     7   23     6    **0P0*  E=[IY+n]=(unsigned)[IY+n]/2
// SRL (IX+n),H   4  DD CB XX 3C     7   23     6    **0P0*  H=[IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n),H   4  FD CB XX 3C     7   23     6    **0P0*  H=[IY+n]=(unsigned)[IY+n]/2
// SRL (IX+n),L   4  DD CB XX 3D     7   23     6    **0P0*  L=[IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n),L   4  FD CB XX 3D     7   23     6    **0P0*  L=[IY+n]=(unsigned)[IY+n]/2
// SRL (IX+n),A   4  DD CB XX 3F     7   23     6    **0P0*  A=[IX+n]=(unsigned)[IX+n]/2
// SRL (IY+n),A   4  FD CB XX 3F     7   23     6    **0P0*  A=[IY+n]=(unsigned)[IY+n]/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
// 1 NOP added by prefix CB
#define cc_srl_mxy_b      CC_5_NOP
#define cc_srl_mxy_c      CC_5_NOP
#define cc_srl_mxy_d      CC_5_NOP
#define cc_srl_mxy_e      CC_5_NOP
#define cc_srl_mxy_h      CC_5_NOP
#define cc_srl_mxy_l      CC_5_NOP
#define cc_srl_mxy_a      CC_5_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SRL B          2  CB 38           2    8     2    **0P0*  B=(unsigned)B/2
// SRL C          2  CB 39           2    8     2    **0P0*  C=(unsigned)C/2
// SRL D          2  CB 3A           2    8     2    **0P0*  D=(unsigned)D/2
// SRL E          2  CB 3B           2    8     2    **0P0*  E=(unsigned)E/2
// SRL H          2  CB 3C           2    8     2    **0P0*  H=(unsigned)H/2
// SRL L          2  CB 3D           2    8     2    **0P0*  L=(unsigned)L/2
// SRL A          2  CB 3F           2    8     2    **0P0*  A=(unsigned)A/2
//+----------------------------------------------------------------------------
// 1 NOP added by prefix CB
#define cc_srl_b          CC_1_NOP
#define cc_srl_c          CC_1_NOP
#define cc_srl_d          CC_1_NOP
#define cc_srl_e          CC_1_NOP
#define cc_srl_h          CC_1_NOP
#define cc_srl_l          CC_1_NOP
#define cc_srl_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SUB (HL)       1  96              2    7     2    ***V1*  A=A-[HL]
//+----------------------------------------------------------------------------
#define cc_sub_mhl        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SUB (IX+n)     3  DD 96 XX        5   19     5    ***V1*  A=A-[IX+n]
// SUB (IY+n)     3  FD 96 XX        5   19     5    ***V1*  A=A-[IY+n]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_sub_mxy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SUB B          1  90              1    4     1    ***V1*  A=A-B
// SUB C          1  91              1    4     1    ***V1*  A=A-C
// SUB D          1  92              1    4     1    ***V1*  A=A-D
// SUB E          1  93              1    4     1    ***V1*  A=A-E
// SUB H          1  94              1    4     1    ***V1*  A=A-H
// SUB L          1  95              1    4     1    ***V1*  A=A-L
// SUB A          1  97              1    4     1    ***V1*  A=A-A
//+----------------------------------------------------------------------------
#define cc_sub_b         CC_1_NOP
#define cc_sub_c         CC_1_NOP
#define cc_sub_d         CC_1_NOP
#define cc_sub_e         CC_1_NOP
#define cc_sub_h         CC_1_NOP
#define cc_sub_l         CC_1_NOP
#define cc_sub_a         CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// SUB A,IXH      2  DD 94           2    9     ***V1*  A=A-IXH
// SUB A,IYH      2  FD 94           2    9     ***V1*  A=A-IYH
// SUB A,IXL      2  DD 95           2    9     ***V1*  A=A-IXL
// SUB A,IYL      2  FD 95           2    9     ***V1*  A=A-IYL
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_sub_xyh        CC_1_NOP
#define cc_sub_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// SUB N          2  D6 XX           2    7     2    ***V1*  A=A-N
//+----------------------------------------------------------------------------
#define cc_sub_byte       CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// XOR (HL)       1  AE              2    7     2    ***P00  A=Ax[HL]
//+----------------------------------------------------------------------------
#define cc_xor_mhl        CC_2_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// XOR (IX+n)     3  DD AE XX        5   19     5    ***P00  A=Ax[IX+n]
// XOR (IY+n)     3  FD AE XX        5   19     5    ***P00  A=Ax[IY+n]
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_xor_mxy        CC_4_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+-----+------+-------------------
// XOR B          1  A8              1    4     1    ***P00  A=AxB
// XOR C          1  A9              1    4     1    ***P00  A=AxC
// XOR D          1  AA              1    4     1    ***P00  A=AxD
// XOR E          1  AB              1    4     1    ***P00  A=AxE
// XOR H          1  AC              1    4     1    ***P00  A=AxH
// XOR L          1  AD              1    4     1    ***P00  A=AxL
// XOR A          1  AF              1    4     1    ***P00  A=AxA
//+----------------------------------------------------------------------------
#define cc_xor_b          CC_1_NOP
#define cc_xor_c          CC_1_NOP
#define cc_xor_d          CC_1_NOP
#define cc_xor_e          CC_1_NOP
#define cc_xor_h          CC_1_NOP
#define cc_xor_l          CC_1_NOP
#define cc_xor_a          CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock| SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// XOR IXH        2  DD AC           2    9     ***P00  A=AxIXH
// XOR IYH        2  FD AC           2    9     ***P00  A=AxIYH
// XOR IXL        2  DD AD           2    9     ***P00  A=AxIXL
// XOR IYL        2  FD AD           2    9     ***P00  A=AxIYL
//+----------------------------------------------------------------------------
// 1 NOP added by prefix DD or FD
#define cc_xor_xyh        CC_1_NOP
#define cc_xor_xyl        CC_1_NOP

//+----------------------------------------------------------------------------
//|Mnemonic    |Size|OP-Code        |NOP|Clock|M Cyc|SZHPNC| Effect
//+------------+----+---------------+---+-----+--------------------------------
// XOR N          2  EE XX           2    7     2    ***P00  A=AxN
//+----------------------------------------------------------------------------
#define cc_xor_byte       CC_2_NOP


#endif /* Z80_OPCODES_H */

