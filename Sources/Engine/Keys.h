/*
    Caprice32 - Amstrad CPC Emulator
    (c) Copyright 1997-2005 Ulrich Doewich

    CaPriCe for Palm OS - Amstrad CPC 464/664/6128 emulator for Palm devices
    Copyright (C) 2006-2011 by Frédéric Coste
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2015 by Frédéric Coste

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


#ifndef KEYS_H
#define KEYS_H


typedef enum
{
  CPC_KEY_0 = 0,
  CPC_KEY_1,
  CPC_KEY_2,
  CPC_KEY_3,
  CPC_KEY_4,
  CPC_KEY_5,
  CPC_KEY_6,
  CPC_KEY_7,
  CPC_KEY_8,
  CPC_KEY_9,
  CPC_KEY_A,
  CPC_KEY_B,
  CPC_KEY_C,
  CPC_KEY_D,
  CPC_KEY_E,
  CPC_KEY_F,
  CPC_KEY_G,
  CPC_KEY_H,
  CPC_KEY_I,
  CPC_KEY_J,
  CPC_KEY_K,
  CPC_KEY_L,
  CPC_KEY_M,
  CPC_KEY_N,
  CPC_KEY_O,
  CPC_KEY_P,
  CPC_KEY_Q,
  CPC_KEY_R,
  CPC_KEY_S,
  CPC_KEY_T,
  CPC_KEY_U,
  CPC_KEY_V,
  CPC_KEY_W,
  CPC_KEY_X,
  CPC_KEY_Y,
  CPC_KEY_Z,
  CPC_KEY_a,
  CPC_KEY_b,
  CPC_KEY_c,
  CPC_KEY_d,
  CPC_KEY_e,
  CPC_KEY_f,
  CPC_KEY_g,
  CPC_KEY_h,
  CPC_KEY_i,
  CPC_KEY_j,
  CPC_KEY_k,
  CPC_KEY_l,
  CPC_KEY_m,
  CPC_KEY_n,
  CPC_KEY_o,
  CPC_KEY_p,
  CPC_KEY_q,
  CPC_KEY_r,
  CPC_KEY_s,
  CPC_KEY_t,
  CPC_KEY_u,
  CPC_KEY_v,
  CPC_KEY_w,
  CPC_KEY_x,
  CPC_KEY_y,
  CPC_KEY_z,
  CPC_KEY_AMPERSAND,    // '&'
  CPC_KEY_ASTERISK,     // '*'
  CPC_KEY_AT,           // '@'
  CPC_KEY_BACKQUOTE,    // '`'
  CPC_KEY_BACKSLASH,    // '\'
  CPC_KEY_CAPSLOCK,
  CPC_KEY_CLR,
  CPC_KEY_COLON,        // ':'
  CPC_KEY_COMMA,        // ','
  CPC_KEY_CONTROL,
  CPC_KEY_COPY,
  CPC_KEY_CPY_DOWN,
  CPC_KEY_CPY_LEFT,
  CPC_KEY_CPY_RIGHT,
  CPC_KEY_CPY_UP,
  CPC_KEY_CUR_DOWN,
  CPC_KEY_CUR_LEFT,
  CPC_KEY_CUR_RIGHT,
  CPC_KEY_CUR_UP,
  CPC_KEY_CUR_ENDBL,
  CPC_KEY_CUR_HOMELN,
  CPC_KEY_CUR_ENDLN,
  CPC_KEY_CUR_HOMEBL,
  CPC_KEY_DBLQUOTE,     // '"'
  CPC_KEY_DEL,
  CPC_KEY_DOLLAR,       // '$'
  CPC_KEY_ENTER,
  CPC_KEY_EQUAL,        // '='
  CPC_KEY_ESC,
  CPC_KEY_EXCLAMATN,    // '!'
  CPC_KEY_F0,
  CPC_KEY_F1,
  CPC_KEY_F2,
  CPC_KEY_F3,
  CPC_KEY_F4,
  CPC_KEY_F5,
  CPC_KEY_F6,
  CPC_KEY_F7,
  CPC_KEY_F8,
  CPC_KEY_F9,
  CPC_KEY_FPERIOD,      // '.'
  CPC_KEY_GREATER,      // '>'
  CPC_KEY_HASH,         // '#'
  CPC_KEY_LBRACKET,     // '['
  CPC_KEY_LCBRACE,
  CPC_KEY_LEFTPAREN,    // '('
  CPC_KEY_LESS,         // '<'
  CPC_KEY_LSHIFT,
  CPC_KEY_MINUS,        // '-'
  CPC_KEY_PERCENT,      // '%'
  CPC_KEY_PERIOD,       // '.'
  CPC_KEY_PIPE,         // '|'
  CPC_KEY_PLUS,         // '+'
  CPC_KEY_POUND,        // '£'
  CPC_KEY_POWER,        // '^'
  CPC_KEY_QUESTION,     // '?'
  CPC_KEY_QUOTE,        // '''
  CPC_KEY_RBRACKET,     // ']'
  CPC_KEY_RCBRACE,
  CPC_KEY_RETURN,
  CPC_KEY_RIGHTPAREN,   // ')'
  CPC_KEY_RSHIFT,
  CPC_KEY_SEMICOLON,    // ';'
  CPC_KEY_SLASH,        // '/'
  CPC_KEY_SPACE,        // ' '
  CPC_KEY_TAB,
  CPC_KEY_UNDERSCORE,   // '_'
  CPC_KEY_J0_UP,
  CPC_KEY_J0_DOWN,
  CPC_KEY_J0_LEFT,
  CPC_KEY_J0_RIGHT,
  CPC_KEY_J0_FIRE1,
  CPC_KEY_J0_FIRE2,
  CPC_KEY_J0_SPARE,
  CPC_KEY_J1_UP,
  CPC_KEY_J1_DOWN,
  CPC_KEY_J1_LEFT,
  CPC_KEY_J1_RIGHT,
  CPC_KEY_J1_FIRE1,
  CPC_KEY_J1_FIRE2,
  CPC_KEY_J1_SPARE,
  CPC_KEY_ES_NTILDE,
  CPC_KEY_ES_nTILDE,
  CPC_KEY_ES_PESETA,
  CPC_KEY_FR_eACUTE,    // 'é'
  CPC_KEY_FR_eGRAVE,    // 'è'
  CPC_KEY_FR_cCEDIL,    // 'ç'
  CPC_KEY_FR_aGRAVE,    // 'à'
  CPC_KEY_FR_uGRAVE,    // 'ù'
  // Should be the last one. Used for number of keys
  CPC_KEY_NONE
} CPC_KEYS;

#define CPC_KEY_INVALID        0xFF


#define CPC_KBD_SHIFT             0x25
#define CPC_KBD_CTRL              0x27
#define CPC_KBD_J0_UP             0x90
#define CPC_KBD_J0_DOWN           0x91
#define CPC_KBD_J0_LEFT           0x92 // CPC_KEY_J0_LEFT
#define CPC_KBD_J0_RIGHT          0x93 // CPC_KEY_J0_RIGHT
#define CPC_KBD_J0_FIRE1          0x94
#define CPC_KBD_J0_FIRE2          0x95
#define CPC_KBD_J0_SPARE          0x96


#if defined (__PALMOS__)

#define WEST_PHASER_TRIG_KEY      CPC_KBD_J0_FIRE2
#define WEST_PHASER_TRIG_TICKS    3 // Best found = 3
#define WEST_PHASER_BEAM_DETECT   0 // Best found = 0
#define WEST_PHASER_BEAM_KEY      CPC_KBD_J0_UP
#define WEST_PHASER_BEAM_DURATION 7 // Best found = 7
#define WEST_PHASER_SENSITIVITY   192 // Best found = 192

#define GUNSTICK_TRIG_KEY         CPC_KBD_J0_FIRE1
#define GUNSTICK_BEAM_KEY         CPC_KBD_J0_DOWN
#define GUNSTICK_BEAM_DURATION    10 // Best found = 80
#define GUNSTICK_SENSITIVITY      128 // Best found = 32

// AMX Mouse keys (Left, Center, Right)
#define AMX_MOUSE_BUTTON_L     CPC_KEY_J0_FIRE1
#define AMX_MOUSE_BUTTON_C     CPC_KEY_J0_SPARE
#define AMX_MOUSE_BUTTON_R     CPC_KEY_J0_FIRE2

#else if defined(__BORLANDC__)

#define WEST_PHASER_TRIG_KEY      CPC_KBD_J0_FIRE2
#define WEST_PHASER_TRIG_DURATION 20   // ms (BestFound=20)
#define WEST_PHASER_TRIG_DELAY    0    // ms (BestFound=0)
#define WEST_PHASER_Y_RADIUS      3
#define WEST_PHASER_SENSITIVITY   224  // (BestFound=192)
#define WEST_PHASER_BEAM_KEY      CPC_KBD_J0_UP
#define WEST_PHASER_BEAM_DURATION 10   // us (BestFound=20)

#define GUNSTICK_TRIG_KEY         CPC_KBD_J0_FIRE1
#define GUNSTICK_TRIG_DURATION    0
#define GUNSTICK_TRIG_DELAY       40   // ms (BestFound=40)
#define GUNSTICK_Y_RADIUS         0
#define GUNSTICK_SENSITIVITY      128  // (BestFound=192)
#define GUNSTICK_BEAM_KEY         CPC_KBD_J0_DOWN
#define GUNSTICK_BEAM_DURATION    100  // us (BestFound=100)

// AMX Mouse keys (Left, Center, Right)
#define AMX_MOUSE_BUTTON_L        CPC_KBD_J0_FIRE1
#define AMX_MOUSE_BUTTON_C        CPC_KBD_J0_SPARE
#define AMX_MOUSE_BUTTON_R        CPC_KBD_J0_FIRE2

#endif

#define AMX_MOUSE_UP_KEY          CPC_KBD_J0_UP
#define AMX_MOUSE_DOWN_KEY        CPC_KBD_J0_DOWN
#define AMX_MOUSE_LEFT_KEY        CPC_KBD_J0_LEFT
#define AMX_MOUSE_RIGHT_KEY       CPC_KBD_J0_RIGHT


#endif /* KEYS_H */
