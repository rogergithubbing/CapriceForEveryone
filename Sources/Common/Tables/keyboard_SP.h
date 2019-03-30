/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2016 by Frédéric Coste

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


#ifndef KEYBOARD_SP_H
#define KEYBOARD_SP_H

#include "..\..\Engine\types.h"
#include "..\..\Engine\Keys.h"


#define MOD_CPC_SHIFT   (0x01 << 8)
#define MOD_CPC_CTRL    (0x02 << 8)
#define MOD_EMU_KEY     (0x10 << 8)


// Spanish CPC keyboard
static tUShort cpc_kbd_sp[CPC_KEY_NONE] =
{
  0x40,                   // CPC_KEY_0
  0x80,                   // CPC_KEY_1
  0x81,                   // CPC_KEY_2
  0x71,                   // CPC_KEY_3
  0x70,                   // CPC_KEY_4
  0x61,                   // CPC_KEY_5
  0x60,                   // CPC_KEY_6
  0x51,                   // CPC_KEY_7
  0x50,                   // CPC_KEY_8
  0x41,                   // CPC_KEY_9
  0x85 | MOD_CPC_SHIFT,   // CPC_KEY_A
  0x66 | MOD_CPC_SHIFT,   // CPC_KEY_B
  0x76 | MOD_CPC_SHIFT,   // CPC_KEY_C
  0x75 | MOD_CPC_SHIFT,   // CPC_KEY_D
  0x72 | MOD_CPC_SHIFT,   // CPC_KEY_E
  0x65 | MOD_CPC_SHIFT,   // CPC_KEY_F
  0x64 | MOD_CPC_SHIFT,   // CPC_KEY_G
  0x54 | MOD_CPC_SHIFT,   // CPC_KEY_H
  0x43 | MOD_CPC_SHIFT,   // CPC_KEY_I
  0x55 | MOD_CPC_SHIFT,   // CPC_KEY_J
  0x45 | MOD_CPC_SHIFT,   // CPC_KEY_K
  0x44 | MOD_CPC_SHIFT,   // CPC_KEY_L
  0x46 | MOD_CPC_SHIFT,   // CPC_KEY_M
  0x56 | MOD_CPC_SHIFT,   // CPC_KEY_N
  0x42 | MOD_CPC_SHIFT,   // CPC_KEY_O
  0x33 | MOD_CPC_SHIFT,   // CPC_KEY_P
  0x83 | MOD_CPC_SHIFT,   // CPC_KEY_Q
  0x62 | MOD_CPC_SHIFT,   // CPC_KEY_R
  0x74 | MOD_CPC_SHIFT,   // CPC_KEY_S
  0x63 | MOD_CPC_SHIFT,   // CPC_KEY_T
  0x52 | MOD_CPC_SHIFT,   // CPC_KEY_U
  0x67 | MOD_CPC_SHIFT,   // CPC_KEY_V
  0x73 | MOD_CPC_SHIFT,   // CPC_KEY_W
  0x77 | MOD_CPC_SHIFT,   // CPC_KEY_X
  0x53 | MOD_CPC_SHIFT,   // CPC_KEY_Y
  0x87 | MOD_CPC_SHIFT,   // CPC_KEY_Z
  0x85,                   // CPC_KEY_a
  0x66,                   // CPC_KEY_b
  0x76,                   // CPC_KEY_c
  0x75,                   // CPC_KEY_d
  0x72,                   // CPC_KEY_e
  0x65,                   // CPC_KEY_f
  0x64,                   // CPC_KEY_g
  0x54,                   // CPC_KEY_h
  0x43,                   // CPC_KEY_i
  0x55,                   // CPC_KEY_j
  0x45,                   // CPC_KEY_k
  0x44,                   // CPC_KEY_l
  0x46,                   // CPC_KEY_m
  0x56,                   // CPC_KEY_n
  0x42,                   // CPC_KEY_o
  0x33,                   // CPC_KEY_p
  0x83,                   // CPC_KEY_q
  0x62,                   // CPC_KEY_r
  0x74,                   // CPC_KEY_s
  0x63,                   // CPC_KEY_t
  0x52,                   // CPC_KEY_u
  0x67,                   // CPC_KEY_v
  0x73,                   // CPC_KEY_w
  0x77,                   // CPC_KEY_x
  0x53,                   // CPC_KEY_y
  0x87,                   // CPC_KEY_z
  0x60 | MOD_CPC_SHIFT,   // CPC_KEY_AMPERSAND
  0x21 | MOD_CPC_SHIFT,   // CPC_KEY_ASTERISK
  0x32,                   // CPC_KEY_AT
  0x26 | MOD_CPC_SHIFT,   // CPC_KEY_BACKQUOTE
  0x26,                   // CPC_KEY_BACKSLASH
  0x86,                   // CPC_KEY_CAPSLOCK
  0x20,                   // CPC_KEY_CLR
  0x34 | MOD_CPC_SHIFT,   // CPC_KEY_COLON
  0x47,                   // CPC_KEY_COMMA
  0x27 | MOD_CPC_CTRL,    // CPC_KEY_CONTROL
  0x11,                   // CPC_KEY_COPY
  0x02 | MOD_CPC_SHIFT,   // CPC_KEY_CPY_DOWN
  0x10 | MOD_CPC_SHIFT,   // CPC_KEY_CPY_LEFT
  0x01 | MOD_CPC_SHIFT,   // CPC_KEY_CPY_RIGHT
  0x00 | MOD_CPC_SHIFT,   // CPC_KEY_CPY_UP
  0x02,                   // CPC_KEY_CUR_DOWN
  0x10,                   // CPC_KEY_CUR_LEFT
  0x01,                   // CPC_KEY_CUR_RIGHT
  0x00,                   // CPC_KEY_CUR_UP
  0x02 | MOD_CPC_CTRL,    // CPC_KEY_CUR_ENDBL
  0x10 | MOD_CPC_CTRL,    // CPC_KEY_CUR_HOMELN
  0x01 | MOD_CPC_CTRL,    // CPC_KEY_CUR_ENDLN
  0x00 | MOD_CPC_CTRL,    // CPC_KEY_CUR_HOMEBL
  0x81 | MOD_CPC_SHIFT,   // CPC_KEY_DBLQUOTE
  0x97,                   // CPC_KEY_DEL
  0x70 | MOD_CPC_SHIFT,   // CPC_KEY_DOLLAR
  0x06,                   // CPC_KEY_ENTER
  0x31 | MOD_CPC_SHIFT,   // CPC_KEY_EQUAL
  0x82,                   // CPC_KEY_ESC
  0x80 | MOD_CPC_SHIFT,   // CPC_KEY_EXCLAMATN
  0x17,                   // CPC_KEY_F0
  0x15,                   // CPC_KEY_F1
  0x16,                   // CPC_KEY_F2
  0x05,                   // CPC_KEY_F3
  0x24,                   // CPC_KEY_F4
  0x14,                   // CPC_KEY_F5
  0x04,                   // CPC_KEY_F6
  0x12,                   // CPC_KEY_F7
  0x13,                   // CPC_KEY_F8
  0x03,                   // CPC_KEY_F9
  0x07,                   // CPC_KEY_FPERIOD
  0x37 | MOD_CPC_SHIFT,   // CPC_KEY_GREATER
  0x71 | MOD_CPC_SHIFT,   // CPC_KEY_HASH
  0x21,                   // CPC_KEY_LBRACKET
  CPC_KEY_INVALID,        // CPC_KEY_LCBRACE
  0x50 | MOD_CPC_SHIFT,   // CPC_KEY_LEFTPAREN
  0x47 | MOD_CPC_SHIFT,   // CPC_KEY_LESS
  0x25 | MOD_CPC_SHIFT,   // CPC_KEY_LSHIFT
  0x31,                   // CPC_KEY_MINUS
  0x61 | MOD_CPC_SHIFT,   // CPC_KEY_PERCENT
  0x37,                   // CPC_KEY_PERIOD
  0x32 | MOD_CPC_SHIFT,   // CPC_KEY_PIPE
  0x23 | MOD_CPC_SHIFT,   // CPC_KEY_PLUS
  CPC_KEY_INVALID,        // CPC_KEY_POUND
  0x30,                   // CPC_KEY_POWER
  0x36 | MOD_CPC_SHIFT,   // CPC_KEY_QUESTION
  0x51 | MOD_CPC_SHIFT,   // CPC_KEY_QUOTE
  0x23,                   // CPC_KEY_RBRACKET
  CPC_KEY_INVALID,        // CPC_KEY_RCBRACE
  0x22,                   // CPC_KEY_RETURN
  0x41 | MOD_CPC_SHIFT,   // CPC_KEY_RIGHTPAREN
  0x25 | MOD_CPC_SHIFT,   // CPC_KEY_RSHIFT
  0x34,                   // CPC_KEY_SEMICOLON
  0x36,                   // CPC_KEY_SLASH
  0x57,                   // CPC_KEY_SPACE
  0x84,                   // CPC_KEY_TAB
  0x40 | MOD_CPC_SHIFT,   // CPC_KEY_UNDERSCORE
  0x90,                   // CPC_KEY_J0_UP
  0x91,                   // CPC_KEY_J0_DOWN
  0x92,                   // CPC_KEY_J0_LEFT
  0x93,                   // CPC_KEY_J0_RIGHT
  0x95,                   // CPC_KEY_J0_FIRE1
  0x94,                   // CPC_KEY_J0_FIRE2
  0x96,                   // CPC_KEY_J0_SPARE
  0x60,                   // CPC_KEY_J1_UP
  0x61,                   // CPC_KEY_J1_DOWN
  0x62,                   // CPC_KEY_J1_LEFT
  0x63,                   // CPC_KEY_J1_RIGHT
  0x65,                   // CPC_KEY_J1_FIRE1
  0x64,                   // CPC_KEY_J1_FIRE2
  0x66,                   // CPC_KEY_J1_SPARE
  0x35 | MOD_CPC_SHIFT,   // CPC_KEY_ES_NTILDE
  0x35,                   // CPC_KEY_ES_nTILDE
  0x30 | MOD_CPC_SHIFT,   // CPC_KEY_ES_PESETA
  CPC_KEY_INVALID,        // CPC_KEY_FR_eACUTE
  CPC_KEY_INVALID,        // CPC_KEY_FR_eGRAVE
  CPC_KEY_INVALID,        // CPC_KEY_FR_cCEDIL
  CPC_KEY_INVALID,        // CPC_KEY_FR_aGRAVE
  CPC_KEY_INVALID,        // CPC_KEY_FR_uGRAVE
};

#endif /* KEYBOARD_SP_H */
