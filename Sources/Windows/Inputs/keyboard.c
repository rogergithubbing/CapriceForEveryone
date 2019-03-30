/*
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


#include "Keyboard.h"

//
// Windows virtual keyboards
//
// Default hardware mapping to convert windows virtual key to CPC keyboard matrix
const tUChar default_hardware_virtual_to_cpc_kbdmatrix[NB_KEYBOARD_KEYS] =
{
  CPC_KEY_INVALID,            // 0x00
  CPC_KEY_INVALID,            // 0x01 VK_LBUTTON     Left mouse button
  CPC_KEY_INVALID,            // 0x02 VK_RBUTTON     Right mouse button
  CPC_KEY_INVALID,            // 0x03 VK_CANCEL      Control-break processing
  CPC_KEY_INVALID,            // 0x04 VK_MBUTTON     Middle mouse button
  CPC_KEY_INVALID,            // 0x05 VK_XBUTTON1    X1 mouse button
  CPC_KEY_INVALID,            // 0x06 VK_XBUTTON2    X2 mouse button
  CPC_KEY_INVALID,            // 0x07
  0x97,                       // 0x08 VK_BACK        BACKSPACE key
  0x84,                       // 0x09 VK_TAB         TAB key
  CPC_KEY_INVALID,            // 0x0A
  CPC_KEY_INVALID,            // 0x0B
  CPC_KEY_INVALID,            // 0x0C VK_CLEAR       CLEAR key (numeric 5 key)
  0x22,                       // 0x0D VK_RETURN      ENTER key
  CPC_KEY_INVALID,            // 0x0E
  CPC_KEY_INVALID,            // 0x0F
  0x25,                       // 0x10 VK_SHIFT       SHIFT key
  0x27,                       // 0x11 VK_CONTROL     CTRL key
  0x11,                       // 0x12 VK_ALT         ALT key
  CPC_KEY_INVALID,            // 0x13 VK_PAUSE       PAUSE key
  0x86,                       // 0x14 VK_CAPITAL     CAPS LOCK key
  CPC_KEY_INVALID,            // 0x15 VK_KANA (VK_HANGUL)
  CPC_KEY_INVALID,            // 0x16
  CPC_KEY_INVALID,            // 0x17 VK_JUNJA
  CPC_KEY_INVALID,            // 0x18 VK_FINAL
  CPC_KEY_INVALID,            // 0x19 VK_KANJI (VK_HANJA)
  CPC_KEY_INVALID,            // 0x1A
  0x82,                       // 0x1B VK_ESCAPE      ESC key
  CPC_KEY_INVALID,            // 0x1C VK_CONVERT
  CPC_KEY_INVALID,            // 0x1D VK_NONCONVERT
  CPC_KEY_INVALID,            // 0x1E VK_ACCEPT
  CPC_KEY_INVALID,            // 0x1F VK_MODECHANGE
  0x57,                       // 0x20 VK_SPACE       SPACEBAR
  CPC_KEY_INVALID,            // 0x21 VK_PRIOR       PAGE UP key
  CPC_KEY_INVALID,            // 0x22 VK_NEXT        PAGE DOWN key
  CPC_KEY_INVALID,            // 0x23 VK_END         END key
  CPC_KEY_INVALID,            // 0x24 VK_HOME        HOME key
  0x10,                       // 0x25 VK_LEFT        LEFT ARROW key
  0x00,                       // 0x26 VK_UP          UP ARROW key
  0x01,                       // 0x27 VK_RIGHT       RIGHT ARROW key
  0x02,                       // 0x28 VK_DOWN        DOWN ARROW key
  CPC_KEY_INVALID,            // 0x29 VK_SELECT      SELECT key
  CPC_KEY_INVALID,            // 0x2A VK_PRINT       PRINT key
  CPC_KEY_INVALID,            // 0x2B VK_EXECUTE     EXECUTE key
  CPC_KEY_INVALID,            // 0x2C VK_SNAPSHOT    PRINT SCREEN key
  CPC_KEY_INVALID,            // 0x2D VK_INSERT      INS key
  0x20,                       // 0x2E VK_DELETE      DEL key
  CPC_KEY_INVALID,            // 0x2F VK_HELP        HELP key
  0x40,                       // 0x30 VK_KEY_0       0 key
  0x80,                       // 0x31 VK_KEY_1       1 key
  0x81,                       // 0x32 VK_KEY_2       2 key
  0x71,                       // 0x33 VK_KEY_3       3 key
  0x70,                       // 0x34 VK_KEY_4       4 key
  0x61,                       // 0x35 VK_KEY_5       5 key
  0x60,                       // 0x36 VK_KEY_6       6 key
  0x51,                       // 0x37 VK_KEY_7       7 key
  0x50,                       // 0x38 VK_KEY_8       8 key
  0x41,                       // 0x39 VK_KEY_9       9 key
  CPC_KEY_INVALID,            // 0x3A Undefined
  CPC_KEY_INVALID,            // 0x3B Undefined
  CPC_KEY_INVALID,            // 0x3C Undefined
  CPC_KEY_INVALID,            // 0x3D Undefined
  CPC_KEY_INVALID,            // 0x3E Undefined
  CPC_KEY_INVALID,            // 0x3F Undefined
  CPC_KEY_INVALID,            // 0x40 Undefined
  0x83,                       // 0x41 VK_KEY_A       A key
  0x66,                       // 0x42 VK_KEY_B       B key
  0x76,                       // 0x43 VK_KEY_C       C key
  0x75,                       // 0x44 VK_KEY_D       D key
  0x72,                       // 0x45 VK_KEY_E       E key
  0x65,                       // 0x46 VK_KEY_F       F key
  0x64,                       // 0x47 VK_KEY_G       G key
  0x54,                       // 0x48 VK_KEY_H       H key
  0x43,                       // 0x49 VK_KEY_I       I key
  0x55,                       // 0x4A VK_KEY_J       J key
  0x45,                       // 0x4B VK_KEY_K       K key
  0x44,                       // 0x4C VK_KEY_L       L key
  0x35,                       // 0x4D VK_KEY_M       M key
  0x56,                       // 0x4E VK_KEY_N       N key
  0x42,                       // 0x4F VK_KEY_O       O key
  0x33,                       // 0x50 VK_KEY_P       P key
  0x85,                       // 0x51 VK_KEY_Q       Q key
  0x62,                       // 0x52 VK_KEY_R       R key
  0x74,                       // 0x53 VK_KEY_S       S key
  0x63,                       // 0x54 VK_KEY_T       T key
  0x52,                       // 0x55 VK_KEY_U       U key
  0x67,                       // 0x56 VK_KEY_V       V key
  0x87,                       // 0x57 VK_KEY_W       W key
  0x77,                       // 0x58 VK_KEY_X       X key
  0x53,                       // 0x59 VK_KEY_Y       Y key
  0x73,                       // 0x5A VK_KEY_Z       Z key
  CPC_KEY_INVALID,            // 0x5B VK_LWIN        Left Windows key
  CPC_KEY_INVALID,            // 0x5C VK_RWIN        Right Windows key
  CPC_KEY_INVALID,            // 0x5D VK_APPS        Applications key
  CPC_KEY_INVALID,            // 0x5E Reserved
  CPC_KEY_INVALID,            // 0x5F VK_SLEEP       Computer Sleep key
  0x17,                       // 0x60 VK_NUMPAD0     Numpad 0 key
  0x15,                       // 0x61 VK_NUMPAD1     Numpad 1 key
  0x16,                       // 0x62 VK_NUMPAD2     Numpad 2 key
  0x05,                       // 0x63 VK_NUMPAD3     Numpad 3 key
  0x24,                       // 0x64 VK_NUMPAD4     Numpad 4 key
  0x14,                       // 0x65 VK_NUMPAD5     Numpad 5 key
  0x04,                       // 0x66 VK_NUMPAD6     Numpad 6 key
  0x12,                       // 0x67 VK_NUMPAD7     Numpad 7 key
  0x13,                       // 0x68 VK_NUMPAD8     Numpad 8 key
  0x03,                       // 0x69 VK_NUMPAD9     Numpad 9 key
  CPC_KEY_INVALID,            // 0x6A VK_MULTIPLY    Numpad Multiply key
  CPC_KEY_INVALID,            // 0x6B VK_ADD         Numpad Add key
  CPC_KEY_INVALID,            // 0x6C VK_SEPARATOR   Separator key
  CPC_KEY_INVALID,            // 0x6D VK_SUBTRACT    Subtract key
  0x07,                       // 0x6E VK_DECIMAL     Numpad Decimal key
  CPC_KEY_INVALID,            // 0x6F VK_DIVIDE      Numpad Divide key
  CPC_KEY_INVALID,            // 0x70 VK_F1          F1 key
  CPC_KEY_INVALID,            // 0x71 VK_F2          F2 key
  CPC_KEY_INVALID,            // 0x72 VK_F3          F3 key
  CPC_KEY_INVALID,            // 0x73 VK_F4          F4 key
  CPC_KEY_INVALID,            // 0x74 VK_F5          F5 key
  CPC_KEY_INVALID,            // 0x75 VK_F6          F6 key
  CPC_KEY_INVALID,            // 0x76 VK_F7          F7 key
  CPC_KEY_INVALID,            // 0x77 VK_F8          F8 key
  CPC_KEY_INVALID,            // 0x78 VK_F9          F9 key
  CPC_KEY_INVALID,            // 0x79 VK_F10         F10 key
  CPC_KEY_INVALID,            // 0x7A VK_F11         F11 key
  CPC_KEY_INVALID,            // 0x7B VK_F12         F12 key
  CPC_KEY_INVALID,            // 0x7C VK_F13         F13 key
  CPC_KEY_INVALID,            // 0x7D VK_F14         F14 key
  CPC_KEY_INVALID,            // 0x7E VK_F15         F15 key
  CPC_KEY_INVALID,            // 0x7F VK_F16         F16 key
  CPC_KEY_INVALID,            // 0x80 VK_F17         F17 key
  CPC_KEY_INVALID,            // 0x81 VK_F18         F18 key
  CPC_KEY_INVALID,            // 0x82 VK_F19         F19 key
  CPC_KEY_INVALID,            // 0x83 VK_F20         F20 key
  CPC_KEY_INVALID,            // 0x84 VK_F21         F21 key
  CPC_KEY_INVALID,            // 0x85 VK_F22         F22 key
  CPC_KEY_INVALID,            // 0x86 VK_F23         F23 key
  CPC_KEY_INVALID,            // 0x87 VK_F24         F24 key
  CPC_KEY_INVALID,            // 0x88 Unassigned
  CPC_KEY_INVALID,            // 0x89 Unassigned
  CPC_KEY_INVALID,            // 0x8A Unassigned
  CPC_KEY_INVALID,            // 0x8B Unassigned
  CPC_KEY_INVALID,            // 0x8C Unassigned
  CPC_KEY_INVALID,            // 0x8D Unassigned
  CPC_KEY_INVALID,            // 0x8E Unassigned
  CPC_KEY_INVALID,            // 0x8F Unassigned
  CPC_KEY_INVALID,            // 0x90 VK_NUMLOCK     NUM LOCK key
  CPC_KEY_INVALID,            // 0x91 VK_SCROLL      SCROLL LOCK key
  CPC_KEY_INVALID,            // 0x92 VK_OEM_FJ_JISHO
  CPC_KEY_INVALID,            // 0x93 VK_OEM_FJ_MASSHOU
  CPC_KEY_INVALID,            // 0x94 VK_OEM_FJ_TOUROKU
  CPC_KEY_INVALID,            // 0x95 VK_OEM_FJ_LOYA
  CPC_KEY_INVALID,            // 0x96 VK_OEM_FJ_ROYA
  CPC_KEY_INVALID,            // 0x97 Unassigned
  CPC_KEY_INVALID,            // 0x98 Unassigned
  CPC_KEY_INVALID,            // 0x99 Unassigned
  CPC_KEY_INVALID,            // 0x9A Unassigned
  CPC_KEY_INVALID,            // 0x9B Unassigned
  0x06,                       // 0x9C Unassigned (VK_ENTER)
  CPC_KEY_INVALID,            // 0x9D Unassigned
  CPC_KEY_INVALID,            // 0x9E Unassigned
  CPC_KEY_INVALID,            // 0x9F Unassigned
  0x25,                       // 0xA0 VK_LSHIFT      Left SHIFT key
  0x25,                       // 0xA1 VK_RSHIFT      Right SHIFT key
  0x27,                       // 0xA2 VK_LCONTROL    Left CONTROL key
  0x27,                       // 0xA3 VK_RCONTROL    Right CONTROL key
  CPC_KEY_INVALID,            // 0xA4 VK_LMENU       Left MENU key
  CPC_KEY_INVALID,            // 0xA5 VK_RMENU       Right MENU key
  CPC_KEY_INVALID,            // 0xA6 VK_BROWSER_BACK
  CPC_KEY_INVALID,            // 0xA7 VK_BROWSER_FORWARD
  CPC_KEY_INVALID,            // 0xA8 VK_BROWSER_REFRESH
  CPC_KEY_INVALID,            // 0xA9 VK_BROWSER_STOP
  CPC_KEY_INVALID,            // 0xAA VK_BROWSER_SEARCH
  CPC_KEY_INVALID,            // 0xAB VK_BROWSER_FAVORITES
  CPC_KEY_INVALID,            // 0xAC VK_BROWSER_HOME
  CPC_KEY_INVALID,            // 0xAD VK_VOLUME_MUTE
  CPC_KEY_INVALID,            // 0xAE VK_VOLUME_DOWN
  CPC_KEY_INVALID,            // 0xAF VK_VOLUME_UP
  CPC_KEY_INVALID,            // 0xB0 VK_MEDIA_NEXT_TRACK
  CPC_KEY_INVALID,            // 0xB1 VK_MEDIA_PREV_TRACK
  CPC_KEY_INVALID,            // 0xB2 VK_MEDIA_STOP
  CPC_KEY_INVALID,            // 0xB3 VK_MEDIA_PLAY_PAUSE
  CPC_KEY_INVALID,            // 0xB4 VK_LAUNCH_MAIL
  CPC_KEY_INVALID,            // 0xB5 VK_LAUNCH_MEDIA_SELECT
  CPC_KEY_INVALID,            // 0xB6 VK_LAUNCH_APP1
  CPC_KEY_INVALID,            // 0xB7 VK_LAUNCH_APP2
  CPC_KEY_INVALID,            // 0xB8 Reserved
  CPC_KEY_INVALID,            // 0xB9 Reserved
  0x21,                       // 0xBA VK_OEM_1       OEM_1 (: ;)
  0x30,                       // 0xBB VK_OEM_PLUS
  0x46,                       // 0xBC VK_OEM_COMMA
  CPC_KEY_INVALID,            // 0xBD VK_OEM_MINUS
  0x47,                       // 0xBE VK_OEM_PERIOD
  0x37,                       // 0xBF VK_OEM_2       OEM_2 (? /)
  0x34,                       // 0xC0 VK_OEM_3       OEM_3 (~ `)
  0x90,                       // 0xC1 Reserved (VK_JOY0_UP)
  0x91,                       // 0xC2 Reserved (VK_JOY0_DOWN)
  0x92,                       // 0xC3 Reserved (VK_JOY0_LEFT)
  0x93,                       // 0xC4 Reserved (VK_JOY0_RIGHT)
  0x95,                       // 0xC5 Reserved (VK_JOY0_FIRE1)
  0x94,                       // 0xC6 Reserved (VK_JOY0_FIRE2)
  0x96,                       // 0xC7 Reserved (VK_JOY0_SPARE)
  0x60,                       // 0xC8 Reserved (VK_JOY1_UP)
  0x61,                       // 0xC9 Reserved (VK_JOY1_DOWN)
  0x62,                       // 0xCA Reserved (VK_JOY1_LEFT)
  0x63,                       // 0xCB Reserved (VK_JOY1_RIGHT)
  0x65,                       // 0xCC Reserved (VK_JOY1_FIRE1)
  0x64,                       // 0xCD Reserved (VK_JOY1_FIRE2)
  0x66,                       // 0xCE Reserved (VK_JOY1_SPARE)
  CPC_KEY_INVALID,            // 0xCF Reserved
  CPC_KEY_INVALID,            // 0xD0 Reserved
  CPC_KEY_INVALID,            // 0xD1 Reserved
  CPC_KEY_INVALID,            // 0xD2 Reserved
  CPC_KEY_INVALID,            // 0xD3 Reserved
  CPC_KEY_INVALID,            // 0xD4 Reserved
  CPC_KEY_INVALID,            // 0xD5 Reserved
  CPC_KEY_INVALID,            // 0xD6 Reserved
  CPC_KEY_INVALID,            // 0xD7 Reserved
  CPC_KEY_INVALID,            // 0xD8 Unassigned
  CPC_KEY_INVALID,            // 0xD9 Unassigned
  CPC_KEY_INVALID,            // 0xDA Unassigned
  0x31,                       // 0xDB VK_OEM_4       OEM_4 ({ [)
  0x23,                       // 0xDC VK_OEM_5       OEM_5 (| \)
  0x32,                       // 0xDD VK_OEM_6       OEM_6 (} ])
  CPC_KEY_INVALID,            // 0xDE VK_OEM_7       OEM_7 SQUARE '²' Key
  0x36,                       // 0xDF VK_OEM_8       OEM_8 (§ !)
  CPC_KEY_INVALID,            // 0xE0 Reserved
  CPC_KEY_INVALID,            // 0xE1 VK_OEM_AX
  0x26,                       // 0xE2 VK_OEM_102     OEM_102 (> <)
  CPC_KEY_INVALID,            // 0xE3 VK_ICO_HELP
  CPC_KEY_INVALID,            // 0xE4 VK_ICO_00
  CPC_KEY_INVALID,            // 0xE5 VK_PROCESSKEY
  CPC_KEY_INVALID,            // 0xE6 VK_ICO_CLEAR   
  CPC_KEY_INVALID,            // 0xE7 VK_PACKET
  CPC_KEY_INVALID,            // 0xE8 Unassigned
  CPC_KEY_INVALID,            // 0xE9 VK_OEM_RESET
  CPC_KEY_INVALID,            // 0xEA VK_OEM_JUMP
  CPC_KEY_INVALID,            // 0xEB VK_OEM_PA1
  CPC_KEY_INVALID,            // 0xEC VK_OEM_PA2
  CPC_KEY_INVALID,            // 0xED VK_OEM_PA3
  CPC_KEY_INVALID,            // 0xEE VK_OEM_WSCTRL
  CPC_KEY_INVALID,            // 0xEF VK_OEM_CUSEL
  CPC_KEY_INVALID,            // 0xF0 VK_OEM_ATTN
  CPC_KEY_INVALID,            // 0xF1 VK_FINISH
  CPC_KEY_INVALID,            // 0xF2 VK_OEM_COPY
  CPC_KEY_INVALID,            // 0xF3 VK_OEM_AUTO
  CPC_KEY_INVALID,            // 0xF4 VK_OEM_ENLW
  CPC_KEY_INVALID,            // 0xF5 VK_OEM_BACKTAB
  CPC_KEY_INVALID,            // 0xF6 VK_ATTN
  CPC_KEY_INVALID,            // 0xF7 VK_CRSEL
  CPC_KEY_INVALID,            // 0xF8 VK_EXSEL
  CPC_KEY_INVALID,            // 0xF9 VK_EREOF
  CPC_KEY_INVALID,            // 0xFA VK_PLAY
  CPC_KEY_INVALID,            // 0xFB VK_ZOOM
  CPC_KEY_INVALID,            // 0xFC VK_NONAME
  CPC_KEY_INVALID,            // 0xFD VK_PA1
  CPC_KEY_INVALID,            // 0xFE VK_OEM_CLEAR
  CPC_KEY_INVALID,            // 0xFF
};


const tKeyboardMappingKey default_joystick_keys[] =
{
  { 0x0026, CPC_KEY_J0_UP },
  { 0x0028, CPC_KEY_J0_DOWN },
  { 0x0025, CPC_KEY_J0_LEFT },
  { 0x0027, CPC_KEY_J0_RIGHT },
  { 0x0011, CPC_KEY_J0_FIRE1 },
  { 0x0020, CPC_KEY_J0_FIRE2 },
  // Last one
  { 0, 0 }
};


//
// ASCII mapped CPC keys
//
const tUChar ascii_to_cpc_keyindex[256] =
{
  CPC_KEY_INVALID,            // 0x00 'NUL'
  CPC_KEY_INVALID,            // 0x01 'SOH'
  CPC_KEY_INVALID,            // 0x02 'STX'
  CPC_KEY_INVALID,            // 0x03 'ETX'
  CPC_KEY_INVALID,            // 0x04 'EOT'
  CPC_KEY_INVALID,            // 0x05 'ENQ'
  CPC_KEY_INVALID,            // 0x06 'ACK'
  CPC_KEY_INVALID,            // 0x07 'BEL'
  CPC_KEY_DEL,                // 0x08 'BS'
  CPC_KEY_TAB,                // 0x09 'TAB'
  CPC_KEY_RETURN,             // 0x0A 'LF"
  CPC_KEY_INVALID,            // 0x0B 'VT'
  CPC_KEY_INVALID,            // 0x0C 'FF'
  CPC_KEY_RETURN,             // 0x0D 'CR'
  CPC_KEY_INVALID,            // 0x0E 'SO'
  CPC_KEY_INVALID,            // 0x0F 'SI'
  CPC_KEY_INVALID,            // 0x10 'DLE'
  CPC_KEY_INVALID,            // 0x11 'DC1'
  CPC_KEY_INVALID,            // 0x12 'DC2'
  CPC_KEY_INVALID,            // 0x13 'DC3'
  CPC_KEY_INVALID,            // 0x14 'DC4'
  CPC_KEY_INVALID,            // 0x15 'NAK'
  CPC_KEY_INVALID,            // 0x16 'SYN'
  CPC_KEY_INVALID,            // 0x17 'ETB'
  CPC_KEY_INVALID,            // 0x18 'CAN'
  CPC_KEY_INVALID,            // 0x19 'EM'
  CPC_KEY_INVALID,            // 0x1A 'SUB'
  CPC_KEY_ESC,                // 0x1B 'ESC'
  CPC_KEY_INVALID,            // 0x1C 'FS'
  CPC_KEY_INVALID,            // 0x1D 'GS'
  CPC_KEY_INVALID,            // 0x1E 'RS'
  CPC_KEY_INVALID,            // 0x1F 'US'
  CPC_KEY_SPACE,              // 0x20 ' '
  CPC_KEY_EXCLAMATN,          // 0x21 '!'
  CPC_KEY_DBLQUOTE,           // 0x22 '"'
  CPC_KEY_HASH,               // 0x23 '#'
  CPC_KEY_DOLLAR,             // 0x24 '$'
  CPC_KEY_PERCENT,            // 0x25 '%'
  CPC_KEY_AMPERSAND,          // 0x26 '&'
  CPC_KEY_QUOTE,              // 0x27 '''
  CPC_KEY_LEFTPAREN,          // 0x28 '('
  CPC_KEY_RIGHTPAREN,         // 0x29 ')'
  CPC_KEY_ASTERISK,           // 0x2A '*'
  CPC_KEY_PLUS,               // 0x2B '+'
  CPC_KEY_COMMA,              // 0x2C ','
  CPC_KEY_MINUS,              // 0x2D '-'
  CPC_KEY_PERIOD,             // 0x2E '.'
  CPC_KEY_SLASH,              // 0x2F '/'
  CPC_KEY_0,                  // 0x30 '0'
  CPC_KEY_1,                  // 0x31 '1'
  CPC_KEY_2,                  // 0x32 '2'
  CPC_KEY_3,                  // 0x33 '3'
  CPC_KEY_4,                  // 0x34 '4'
  CPC_KEY_5,                  // 0x35 '5'
  CPC_KEY_6,                  // 0x36 '6'
  CPC_KEY_7,                  // 0x37 '7'
  CPC_KEY_8,                  // 0x38 '8'
  CPC_KEY_9,                  // 0x39 '9'
  CPC_KEY_COLON,              // 0x3A ':'
  CPC_KEY_SEMICOLON,          // 0x3B ';'
  CPC_KEY_LESS,               // 0x3C '<'
  CPC_KEY_EQUAL,              // 0x3D '='
  CPC_KEY_GREATER,            // 0x3E '>'
  CPC_KEY_QUESTION,           // 0x3F '?'
  CPC_KEY_AT,                 // 0x40 '@'
  CPC_KEY_A,                  // 0x41 'A'
  CPC_KEY_B,                  // 0x42 'B'
  CPC_KEY_C,                  // 0x43 'C'
  CPC_KEY_D,                  // 0x44 'D'
  CPC_KEY_E,                  // 0x45 'E'
  CPC_KEY_F,                  // 0x46 'F'
  CPC_KEY_G,                  // 0x47 'G'
  CPC_KEY_H,                  // 0x48 'H'
  CPC_KEY_I,                  // 0x49 'I'
  CPC_KEY_J,                  // 0x4A 'J'
  CPC_KEY_K,                  // 0x4B 'K'
  CPC_KEY_L,                  // 0x4C 'L'
  CPC_KEY_M,                  // 0x4D 'M'
  CPC_KEY_N,                  // 0x4E 'N'
  CPC_KEY_O,                  // 0x4F 'O'
  CPC_KEY_P,                  // 0x50 'P'
  CPC_KEY_Q,                  // 0x51 'Q'
  CPC_KEY_R,                  // 0x52 'R'
  CPC_KEY_S,                  // 0x53 'S'
  CPC_KEY_T,                  // 0x54 'T'
  CPC_KEY_U,                  // 0x55 'U'
  CPC_KEY_V,                  // 0x56 'V'
  CPC_KEY_W,                  // 0x57 'W'
  CPC_KEY_X,                  // 0x58 'X'
  CPC_KEY_Y,                  // 0x59 'Y'
  CPC_KEY_Z,                  // 0x5A 'Z'
  CPC_KEY_LBRACKET,           // 0x5B '['
  CPC_KEY_BACKSLASH,          // 0x5C '\'
  CPC_KEY_RBRACKET,           // 0x5D ']'
  CPC_KEY_POWER,              // 0x5E '^'
  CPC_KEY_UNDERSCORE,         // 0x5F '_'
  CPC_KEY_INVALID,            // 0x60 '`'
  CPC_KEY_a,                  // 0x61 'a'
  CPC_KEY_b,                  // 0x62 'b'
  CPC_KEY_c,                  // 0x63 'c'
  CPC_KEY_d,                  // 0x64 'd'
  CPC_KEY_e,                  // 0x65 'e'
  CPC_KEY_f,                  // 0x66 'f'
  CPC_KEY_g,                  // 0x67 'g'
  CPC_KEY_h,                  // 0x68 'h'
  CPC_KEY_i,                  // 0x69 'i'
  CPC_KEY_j,                  // 0x6A 'j'
  CPC_KEY_k,                  // 0x6B 'k'
  CPC_KEY_l,                  // 0x6C 'l'
  CPC_KEY_m,                  // 0x6D 'm'
  CPC_KEY_n,                  // 0x6E 'n'
  CPC_KEY_o,                  // 0x6F 'o'
  CPC_KEY_p,                  // 0x70 'p'
  CPC_KEY_q,                  // 0x71 'q'
  CPC_KEY_r,                  // 0x72 'r'
  CPC_KEY_s,                  // 0x73 's'
  CPC_KEY_t,                  // 0x74 't'
  CPC_KEY_u,                  // 0x75 'u'
  CPC_KEY_v,                  // 0x76 'v'
  CPC_KEY_w,                  // 0x77 'w'
  CPC_KEY_x,                  // 0x78 'x'
  CPC_KEY_y,                  // 0x79 'y'
  CPC_KEY_z,                  // 0x7A 'z'
  CPC_KEY_LCBRACE,            // 0x7B '{'
  CPC_KEY_PIPE,               // 0x7C '|'
  CPC_KEY_RCBRACE,            // 0x7D '}'
  CPC_KEY_INVALID,            // 0x7E '~'
  CPC_KEY_CLR,                // 0x7F 'DEL'
  CPC_KEY_INVALID,            // 0x80
  CPC_KEY_INVALID,            // 0x81
  CPC_KEY_INVALID,            // 0x82
  CPC_KEY_INVALID,            // 0x83
  CPC_KEY_INVALID,            // 0x84
  CPC_KEY_INVALID,            // 0x85
  CPC_KEY_INVALID,            // 0x86
  CPC_KEY_INVALID,            // 0x87
  CPC_KEY_INVALID,            // 0x88
  CPC_KEY_INVALID,            // 0x89
  CPC_KEY_INVALID,            // 0x8A
  CPC_KEY_INVALID,            // 0x8B
  CPC_KEY_INVALID,            // 0x8C
  CPC_KEY_INVALID,            // 0x8D
  CPC_KEY_INVALID,            // 0x8E
  CPC_KEY_INVALID,            // 0x8F
  CPC_KEY_INVALID,            // 0x90
  CPC_KEY_INVALID,            // 0x91
  CPC_KEY_INVALID,            // 0x92
  CPC_KEY_INVALID,            // 0x93
  CPC_KEY_INVALID,            // 0x94
  CPC_KEY_INVALID,            // 0x95
  CPC_KEY_INVALID,            // 0x96
  CPC_KEY_INVALID,            // 0x97
  CPC_KEY_INVALID,            // 0x98
  CPC_KEY_INVALID,            // 0x99
  CPC_KEY_INVALID,            // 0x9A
  CPC_KEY_INVALID,            // 0x9B
  CPC_KEY_INVALID,            // 0x9C
  CPC_KEY_INVALID,            // 0x9D
  CPC_KEY_ES_PESETA,          // 0x9E Peseta
  CPC_KEY_INVALID,            // 0x9F
  CPC_KEY_INVALID,            // 0xA0
  CPC_KEY_INVALID,            // 0xA1
  CPC_KEY_INVALID,            // 0xA2
  CPC_KEY_POUND,              // 0xA3 '£'
  CPC_KEY_INVALID,            // 0xA4
  CPC_KEY_INVALID,            // 0xA5
  CPC_KEY_INVALID,            // 0xA6
  CPC_KEY_INVALID,            // 0xA7 '§'
  CPC_KEY_INVALID,            // 0xA8
  CPC_KEY_INVALID,            // 0xA9
  CPC_KEY_INVALID,            // 0xAA
  CPC_KEY_INVALID,            // 0xAB
  CPC_KEY_INVALID,            // 0xAC
  CPC_KEY_INVALID,            // 0xAD
  CPC_KEY_INVALID,            // 0xAE
  CPC_KEY_INVALID,            // 0xAF
  CPC_KEY_INVALID,            // 0xB0
  CPC_KEY_INVALID,            // 0xB1
  CPC_KEY_INVALID,            // 0xB2
  CPC_KEY_INVALID,            // 0xB3
  CPC_KEY_INVALID,            // 0xB4
  CPC_KEY_INVALID,            // 0xB5
  CPC_KEY_INVALID,            // 0xB6
  CPC_KEY_INVALID,            // 0xB7
  CPC_KEY_INVALID,            // 0xB8
  CPC_KEY_INVALID,            // 0xB9
  CPC_KEY_INVALID,            // 0xBA
  CPC_KEY_INVALID,            // 0xBB
  CPC_KEY_INVALID,            // 0xBC
  CPC_KEY_INVALID,            // 0xBD
  CPC_KEY_INVALID,            // 0xBE
  CPC_KEY_INVALID,            // 0xBF
  CPC_KEY_INVALID,            // 0xC0
  CPC_KEY_INVALID,            // 0xC1
  CPC_KEY_INVALID,            // 0xC2
  CPC_KEY_INVALID,            // 0xC3
  CPC_KEY_INVALID,            // 0xC4
  CPC_KEY_INVALID,            // 0xC5
  CPC_KEY_INVALID,            // 0xC6
  CPC_KEY_INVALID,            // 0xC7
  CPC_KEY_INVALID,            // 0xC8
  CPC_KEY_INVALID,            // 0xC9
  CPC_KEY_INVALID,            // 0xCA
  CPC_KEY_INVALID,            // 0xCB
  CPC_KEY_INVALID,            // 0xCC
  CPC_KEY_INVALID,            // 0xCD
  CPC_KEY_INVALID,            // 0xCE
  CPC_KEY_INVALID,            // 0xCF
  CPC_KEY_INVALID,            // 0xD0
  CPC_KEY_ES_NTILDE,          // 0xD1 'Ñ'
  CPC_KEY_INVALID,            // 0xD2
  CPC_KEY_INVALID,            // 0xD3
  CPC_KEY_INVALID,            // 0xD4
  CPC_KEY_INVALID,            // 0xD5
  CPC_KEY_INVALID,            // 0xD6
  CPC_KEY_INVALID,            // 0xD7
  CPC_KEY_INVALID,            // 0xD8
  CPC_KEY_INVALID,            // 0xD9
  CPC_KEY_INVALID,            // 0xDA
  CPC_KEY_INVALID,            // 0xDB
  CPC_KEY_INVALID,            // 0xDC
  CPC_KEY_INVALID,            // 0xDD
  CPC_KEY_INVALID,            // 0xDE
  CPC_KEY_INVALID,            // 0xDF
  CPC_KEY_FR_aGRAVE,          // 0xE0 'à'
  CPC_KEY_INVALID,            // 0xE1
  CPC_KEY_INVALID,            // 0xE2
  CPC_KEY_INVALID,            // 0xE3
  CPC_KEY_INVALID,            // 0xE4
  CPC_KEY_INVALID,            // 0xE5
  CPC_KEY_INVALID,            // 0xE6
  CPC_KEY_FR_cCEDIL,          // 0xE7 'ç'
  CPC_KEY_FR_eGRAVE,          // 0xE8 'è'
  CPC_KEY_FR_eACUTE,          // 0xE9 'é'
  CPC_KEY_INVALID,            // 0xEA
  CPC_KEY_INVALID,            // 0xEB
  CPC_KEY_INVALID,            // 0xEC
  CPC_KEY_INVALID,            // 0xED
  CPC_KEY_INVALID,            // 0xEE
  CPC_KEY_INVALID,            // 0xEF
  CPC_KEY_INVALID,            // 0xF0
  CPC_KEY_ES_nTILDE,          // 0xF1 'ñ'
  CPC_KEY_INVALID,            // 0xF2
  CPC_KEY_INVALID,            // 0xF3
  CPC_KEY_INVALID,            // 0xF4
  CPC_KEY_INVALID,            // 0xF5
  CPC_KEY_INVALID,            // 0xF6
  CPC_KEY_INVALID,            // 0xF7
  CPC_KEY_INVALID,            // 0xF8
  CPC_KEY_FR_uGRAVE,          // 0xF9 'ù'
  CPC_KEY_INVALID,            // 0xFA
  CPC_KEY_INVALID,            // 0xFB
  CPC_KEY_INVALID,            // 0xFC
  CPC_KEY_INVALID,            // 0xFD
  CPC_KEY_INVALID,            // 0xFE
  CPC_KEY_INVALID,            // 0xFF
};


// Windows VirtualKey and CPC Keyindex ASCII mapping 
const tKeyboardMappingKey default_non_ascii_keys[] =
{
  { 0x0008, CPC_KEY_DEL },        // VK_BACK
  { 0x0009, CPC_KEY_TAB },        // VK_TAB
  { 0x000D, CPC_KEY_RETURN },     // VK_RETURN
  { 0x0014, CPC_KEY_CAPSLOCK },   // VK_CAPITAL
  { 0x001B, CPC_KEY_ESC },        // VK_ESCAPE
  { 0x002E, CPC_KEY_CLR },        // VK_DELETE
  { 0x0024, CPC_KEY_COPY },       // VK_HOME
  { 0x0022, CPC_KEY_CONTROL },    // VK_NEXT
  { 0x0021, CPC_KEY_LSHIFT },     // VK_PRIOR
  { 0x009C, CPC_KEY_ENTER },      // VK_ENTER
  { 0x0025, CPC_KEY_CUR_LEFT },   // VK_LEFT
  { 0x0026, CPC_KEY_CUR_UP },     // VK_UP
  { 0x0027, CPC_KEY_CUR_RIGHT },  // VK_RIGHT
  { 0x0028, CPC_KEY_CUR_DOWN },   // VK_DOWN
  { 0x0060, CPC_KEY_F0 },         // VK_NUMPAD0
  { 0x0061, CPC_KEY_F1 },         // VK_NUMPAD1
  { 0x0062, CPC_KEY_F2 },         // VK_NUMPAD2
  { 0x0063, CPC_KEY_F3 },         // VK_NUMPAD3
  { 0x0064, CPC_KEY_F4 },         // VK_NUMPAD4
  { 0x0065, CPC_KEY_F5 },         // VK_NUMPAD5
  { 0x0066, CPC_KEY_F6 },         // VK_NUMPAD6
  { 0x0067, CPC_KEY_F7 },         // VK_NUMPAD7
  { 0x0068, CPC_KEY_F8 },         // VK_NUMPAD8
  { 0x0069, CPC_KEY_F9 },         // VK_NUMPAD9
  { 0x006E, CPC_KEY_FPERIOD },    // VK_DECIMAL
  { 0x00C1, CPC_KEY_J0_UP },      // VK_JOY0_UP
  { 0x00C2, CPC_KEY_J0_DOWN },    // VK_JOY0_DOWN
  { 0x00C3, CPC_KEY_J0_LEFT },    // VK_JOY0_LEFT
  { 0x00C4, CPC_KEY_J0_RIGHT },   // VK_JOY0_RIGHT
  { 0x00C5, CPC_KEY_J0_FIRE1 },   // VK_JOY0_FIRE1
  { 0x00C6, CPC_KEY_J0_FIRE2 },   // VK_JOY0_FIRE2
  { 0x00C7, CPC_KEY_J0_SPARE },   // VK_JOY0_SPARE
  { 0x00C8, CPC_KEY_J1_UP },      // VK_JOY1_UP
  { 0x00C9, CPC_KEY_J1_DOWN },    // VK_JOY1_DOWN
  { 0x00CA, CPC_KEY_J1_LEFT },    // VK_JOY1_LEFT
  { 0x00CB, CPC_KEY_J1_RIGHT },   // VK_JOY1_RIGHT
  { 0x00CC, CPC_KEY_J1_FIRE1 },   // VK_JOY1_FIRE1
  { 0x00CD, CPC_KEY_J1_FIRE2 },   // VK_JOY1_FIRE2
  { 0x00CE, CPC_KEY_J1_SPARE },   // VK_JOY1_SPARE
  // Last one
  { 0, CPC_KEY_INVALID }
};
const tUChar non_ascii_keys[] =
{
  CPC_KEY_CAPSLOCK,
  CPC_KEY_CLR,
  CPC_KEY_COPY,
  CPC_KEY_CONTROL,
  CPC_KEY_DEL,
  CPC_KEY_ESC,
  CPC_KEY_ENTER,
  CPC_KEY_LSHIFT,
  CPC_KEY_RETURN,
  CPC_KEY_TAB,
  CPC_KEY_CUR_LEFT,
  CPC_KEY_CUR_UP,
  CPC_KEY_CUR_RIGHT,
  CPC_KEY_CUR_DOWN,
  // End of array
  CPC_KEY_INVALID
};


const char* ctrl_key_string = "Ctrl";
const char* shift_key_string = "Shift";
const char* alt_key_string = "Alt";


//
// CPC key strings
//
const char* cpc_keys_string[] =
{
  "CPC_KEY_0",
  "CPC_KEY_1",
  "CPC_KEY_2",
  "CPC_KEY_3",
  "CPC_KEY_4",
  "CPC_KEY_5",
  "CPC_KEY_6",
  "CPC_KEY_7",
  "CPC_KEY_8",
  "CPC_KEY_9",
  "CPC_KEY_A",
  "CPC_KEY_B",
  "CPC_KEY_C",
  "CPC_KEY_D",
  "CPC_KEY_E",
  "CPC_KEY_F",
  "CPC_KEY_G",
  "CPC_KEY_H",
  "CPC_KEY_I",
  "CPC_KEY_J",
  "CPC_KEY_K",
  "CPC_KEY_L",
  "CPC_KEY_M",
  "CPC_KEY_N",
  "CPC_KEY_O",
  "CPC_KEY_P",
  "CPC_KEY_Q",
  "CPC_KEY_R",
  "CPC_KEY_S",
  "CPC_KEY_T",
  "CPC_KEY_U",
  "CPC_KEY_V",
  "CPC_KEY_W",
  "CPC_KEY_X",
  "CPC_KEY_Y",
  "CPC_KEY_Z",
  "CPC_KEY_a",
  "CPC_KEY_b",
  "CPC_KEY_c",
  "CPC_KEY_d",
  "CPC_KEY_e",
  "CPC_KEY_f",
  "CPC_KEY_g",
  "CPC_KEY_h",
  "CPC_KEY_i",
  "CPC_KEY_j",
  "CPC_KEY_k",
  "CPC_KEY_l",
  "CPC_KEY_m",
  "CPC_KEY_n",
  "CPC_KEY_o",
  "CPC_KEY_p",
  "CPC_KEY_q",
  "CPC_KEY_r",
  "CPC_KEY_s",
  "CPC_KEY_t",
  "CPC_KEY_u",
  "CPC_KEY_v",
  "CPC_KEY_w",
  "CPC_KEY_x",
  "CPC_KEY_y",
  "CPC_KEY_z",
  "CPC_KEY_AMPERSAND",
  "CPC_KEY_ASTERISK",
  "CPC_KEY_AT",
  "CPC_KEY_BACKQUOTE",
  "CPC_KEY_BACKSLASH",
  "CPC_KEY_CAPSLOCK",
  "CPC_KEY_CLR",
  "CPC_KEY_COLON",
  "CPC_KEY_COMMA",
  "CPC_KEY_CONTROL",
  "CPC_KEY_COPY",
  "CPC_KEY_CPY_DOWN",
  "CPC_KEY_CPY_LEFT",
  "CPC_KEY_CPY_RIGHT",
  "CPC_KEY_CPY_UP",
  "CPC_KEY_CUR_DOWN",
  "CPC_KEY_CUR_LEFT",
  "CPC_KEY_CUR_RIGHT",
  "CPC_KEY_CUR_UP",
  "CPC_KEY_CUR_ENDBL",
  "CPC_KEY_CUR_HOMELN",
  "CPC_KEY_CUR_ENDLN",
  "CPC_KEY_CUR_HOMEBL",
  "CPC_KEY_DBLQUOTE",
  "CPC_KEY_DEL",
  "CPC_KEY_DOLLAR",
  "CPC_KEY_ENTER",
  "CPC_KEY_EQUAL",
  "CPC_KEY_ESC",
  "CPC_KEY_EXCLAMATN",   
  "CPC_KEY_F0",
  "CPC_KEY_F1",
  "CPC_KEY_F2",
  "CPC_KEY_F3",
  "CPC_KEY_F4",
  "CPC_KEY_F5",
  "CPC_KEY_F6",
  "CPC_KEY_F7",
  "CPC_KEY_F8",
  "CPC_KEY_F9",
  "CPC_KEY_FPERIOD",
  "CPC_KEY_GREATER",
  "CPC_KEY_HASH",
  "CPC_KEY_LBRACKET",
  "CPC_KEY_LCBRACE",
  "CPC_KEY_LEFTPAREN",
  "CPC_KEY_LESS",         
  "CPC_KEY_SHIFT",
  "CPC_KEY_MINUS",
  "CPC_KEY_PERCENT",
  "CPC_KEY_PERIOD",
  "CPC_KEY_PIPE",
  "CPC_KEY_PLUS",
  "CPC_KEY_POUND",
  "CPC_KEY_POWER",
  "CPC_KEY_QUESTION",
  "CPC_KEY_QUOTE",
  "CPC_KEY_RBRACKET",
  "CPC_KEY_RCBRACE",
  "CPC_KEY_RETURN",
  "CPC_KEY_RIGHTPAREN",
  "CPC_KEY_SHIFT",
  "CPC_KEY_SEMICOLON",
  "CPC_KEY_SLASH",
  "CPC_KEY_SPACE",
  "CPC_KEY_TAB",
  "CPC_KEY_UNDERSCORE",
  "CPC_KEY_J0_UP",
  "CPC_KEY_J0_DOWN",
  "CPC_KEY_J0_LEFT",
  "CPC_KEY_J0_RIGHT",
  "CPC_KEY_J0_FIRE1",
  "CPC_KEY_J0_FIRE2",
  "CPC_KEY_J0_SPARE",
  "CPC_KEY_J1_UP",
  "CPC_KEY_J1_DOWN",
  "CPC_KEY_J1_LEFT",
  "CPC_KEY_J1_RIGHT",
  "CPC_KEY_J1_FIRE1",
  "CPC_KEY_J1_FIRE2",
  "CPC_KEY_J1_SPARE",
  "CPC_KEY_ES_NTILDE",
  "CPC_KEY_ES_nTILDE",
  "CPC_KEY_ES_PESETA",
  "CPC_KEY_FR_eACUTE",
  "CPC_KEY_FR_eGRAVE",
  "CPC_KEY_FR_cCEDIL",
  "CPC_KEY_FR_aGRAVE",
  "CPC_KEY_FR_uGRAVE",
  // Should be the last one. Used for number of keys
  "CPC_KEY_NONE"
};


// Windows Virtual Key strings
const char* windows_vk_string[] =
{
  /* 0x00 */ "",
  /* 0x01 */ "LBUTTON",
  /* 0x02 */ "RBUTTON",
  /* 0x03 */ "CANCEL",
  /* 0x04 */ "MBUTTON",
  /* 0x05 */ "XBUTTON1",
  /* 0x06 */ "XBUTTON2",
  /* 0x07 */ "",
  /* 0x08 */ "BACK",
  /* 0x09 */ "TAB",
  /* 0x0A */ "",
  /* 0x0B */ "",
  /* 0x0C */ "CLEAR",
  /* 0x0D */ "RETURN",
  /* 0x0E */ "",
  /* 0x0F */ "",
  /* 0x10 */ "SHIFT",
  /* 0x11 */ "CONTROL",
  /* 0x12 */ "ALT",
  /* 0x13 */ "PAUSE",
  /* 0x14 */ "CAPITAL",
  /* 0x15 */ "KANA",
  /* 0x16 */ "",
  /* 0x17 */ "JUNJA",
  /* 0x18 */ "FINAL",
  /* 0x19 */ "KANJI",
  /* 0x1A */ "",
  /* 0x1B */ "ESCAPE",
  /* 0x1C */ "CONVERT",
  /* 0x1D */ "NONCONVERT",
  /* 0x1E */ "ACCEPT",
  /* 0x1F */ "MODECHANGE",
  /* 0x20 */ "SPACE",
  /* 0x21 */ "PRIOR",
  /* 0x22 */ "NEXT",
  /* 0x23 */ "END",
  /* 0x24 */ "HOME",
  /* 0x25 */ "LEFT",
  /* 0x26 */ "UP",
  /* 0x27 */ "RIGHT",
  /* 0x28 */ "DOWN",
  /* 0x29 */ "SELECT",
  /* 0x2A */ "PRINT",
  /* 0x2B */ "EXECUTE",
  /* 0x2C */ "SNAPSHOT",
  /* 0x2D */ "INSERT",
  /* 0x2E */ "DELETE",
  /* 0x2F */ "HELP",
  /* 0x30 */ "KEY_0",
  /* 0x31 */ "KEY_1",
  /* 0x32 */ "KEY_2",
  /* 0x33 */ "KEY_3",
  /* 0x34 */ "KEY_4",
  /* 0x35 */ "KEY_5",
  /* 0x36 */ "KEY_6",
  /* 0x37 */ "KEY_7",
  /* 0x38 */ "KEY_8",
  /* 0x39 */ "KEY_9",
  /* 0x3A */ "",
  /* 0x3B */ "",
  /* 0x3C */ "",
  /* 0x3D */ "",
  /* 0x3E */ "",
  /* 0x3F */ "",
  /* 0x40 */ "",
  /* 0x41 */ "KEY_A",
  /* 0x42 */ "KEY_B",
  /* 0x43 */ "KEY_C",
  /* 0x44 */ "KEY_D",
  /* 0x45 */ "KEY_E",
  /* 0x46 */ "KEY_F",
  /* 0x47 */ "KEY_G",
  /* 0x48 */ "KEY_H",
  /* 0x49 */ "KEY_I",
  /* 0x4A */ "KEY_J",
  /* 0x4B */ "KEY_K",
  /* 0x4C */ "KEY_L",
  /* 0x4D */ "KEY_M",
  /* 0x4E */ "KEY_N",
  /* 0x4F */ "KEY_O",
  /* 0x50 */ "KEY_P",
  /* 0x51 */ "KEY_Q",
  /* 0x52 */ "KEY_R",
  /* 0x53 */ "KEY_S",
  /* 0x54 */ "KEY_T",
  /* 0x55 */ "KEY_U",
  /* 0x56 */ "KEY_V",
  /* 0x57 */ "KEY_W",
  /* 0x58 */ "KEY_X",
  /* 0x59 */ "KEY_Y",
  /* 0x5A */ "KEY_Z",
  /* 0x5B */ "LWIN",
  /* 0x5C */ "RWIN",
  /* 0x5D */ "APPS",
  /* 0x5E */ "",
  /* 0x5F */ "SLEEP",
  /* 0x60 */ "NUMPAD0",
  /* 0x61 */ "NUMPAD1",
  /* 0x62 */ "NUMPAD2",
  /* 0x63 */ "NUMPAD3",
  /* 0x64 */ "NUMPAD4",
  /* 0x65 */ "NUMPAD5",
  /* 0x66 */ "NUMPAD6",
  /* 0x67 */ "NUMPAD7",
  /* 0x68 */ "NUMPAD8",
  /* 0x69 */ "NUMPAD9",
  /* 0x6A */ "MULTIPLY",
  /* 0x6B */ "ADD",
  /* 0x6C */ "SEPARATOR",
  /* 0x6D */ "SUBTRACT",
  /* 0x6E */ "DECIMAL",
  /* 0x6F */ "DIVIDE",
  /* 0x70 */ "F1",
  /* 0x71 */ "F2",
  /* 0x72 */ "F3",
  /* 0x73 */ "F4",
  /* 0x74 */ "F5",
  /* 0x75 */ "F6",
  /* 0x76 */ "F7",
  /* 0x77 */ "F8",
  /* 0x78 */ "F9",
  /* 0x79 */ "F10",
  /* 0x7A */ "F11",
  /* 0x7B */ "F12",
  /* 0x7C */ "F13",
  /* 0x7D */ "F14",
  /* 0x7E */ "F15",
  /* 0x7F */ "F16",
  /* 0x80 */ "F17",
  /* 0x81 */ "F18",
  /* 0x82 */ "F19",
  /* 0x83 */ "F20",
  /* 0x84 */ "F21",
  /* 0x85 */ "F22",
  /* 0x86 */ "F23",
  /* 0x87 */ "F24",
  /* 0x88 */ "",
  /* 0x89 */ "",
  /* 0x8A */ "",
  /* 0x8B */ "",
  /* 0x8C */ "",
  /* 0x8D */ "",
  /* 0x8E */ "",
  /* 0x8F */ "",
  /* 0x90 */ "NUMLOCK",
  /* 0x91 */ "SCROLL",
  /* 0x92 */ "OEM_FJ_JISHO",
  /* 0x93 */ "OEM_FJ_MASSHOU",
  /* 0x94 */ "OEM_FJ_TOUROKU",
  /* 0x95 */ "OEM_FJ_LOYA",
  /* 0x96 */ "OEM_FJ_ROYA",
  /* 0x97 */ "",
  /* 0x98 */ "",
  /* 0x99 */ "",
  /* 0x9A */ "",
  /* 0x9B */ "",
  /* 0x9C */ "ENTER",
  /* 0x9D */ "",
  /* 0x9E */ "",
  /* 0x9F */ "",
  /* 0xA0 */ "LSHIFT",
  /* 0xA1 */ "RSHIFT",
  /* 0xA2 */ "LCONTROL",
  /* 0xA3 */ "RCONTROL",
  /* 0xA4 */ "LMENU",
  /* 0xA5 */ "RMENU",
  /* 0xA6 */ "BROWSER_BACK",
  /* 0xA7 */ "BROWSER_FORWARD",
  /* 0xA8 */ "BROWSER_REFRESH",
  /* 0xA9 */ "BROWSER_STOP",
  /* 0xAA */ "BROWSER_SEARCH",
  /* 0xAB */ "BROWSER_FAVORITES",
  /* 0xAC */ "BROWSER_HOME",
  /* 0xAD */ "VOLUME_MUTE",
  /* 0xAE */ "VOLUME_DOWN",
  /* 0xAF */ "VOLUME_UP",
  /* 0xB0 */ "MEDIA_NEXT_TRACK",
  /* 0xB1 */ "MEDIA_PREV_TRACK",
  /* 0xB2 */ "MEDIA_STOP",
  /* 0xB3 */ "MEDIA_PLAY_PAUSE",
  /* 0xB4 */ "LAUNCH_MAIL",
  /* 0xB5 */ "LAUNCH_MEDIA_SELECT",
  /* 0xB6 */ "LAUNCH_APP1",
  /* 0xB7 */ "LAUNCH_APP2",
  /* 0xB8 */ "",
  /* 0xB9 */ "",
  /* 0xBA */ "OEM_1",
  /* 0xBB */ "OEM_PLUS",
  /* 0xBC */ "OEM_COMMA",
  /* 0xBD */ "OEM_MINUS",
  /* 0xBE */ "OEM_PERIOD",
  /* 0xBF */ "OEM_2",
  /* 0xC0 */ "OEM_3",
  /* 0xC1 */ "JOY0_UP",
  /* 0xC2 */ "JOY0_DOWN",
  /* 0xC3 */ "JOY0_LEFT",
  /* 0xC4 */ "JOY0_RIGHT",
  /* 0xC5 */ "JOY0_FIRE1",
  /* 0xC6 */ "JOY0_FIRE2",
  /* 0xC7 */ "JOY0_SPARE",
  /* 0xC8 */ "JOY1_UP",
  /* 0xC9 */ "JOY1_DOWN",
  /* 0xCA */ "JOY1_LEFT",
  /* 0xCB */ "JOY1_RIGHT",
  /* 0xCC */ "JOY1_FIRE1",
  /* 0xCD */ "JOY1_FIRE2",
  /* 0xCE */ "JOY1_SPARE",
  /* 0xCF */ "",
  /* 0xD0 */ "",
  /* 0xD1 */ "",
  /* 0xD2 */ "",
  /* 0xD3 */ "",
  /* 0xD4 */ "",
  /* 0xD5 */ "",
  /* 0xD6 */ "",
  /* 0xD7 */ "",
  /* 0xD8 */ "",
  /* 0xD9 */ "",
  /* 0xDA */ "",
  /* 0xDB */ "OEM_4",
  /* 0xDC */ "OEM_5",
  /* 0xDD */ "OEM_6",
  /* 0xDE */ "OEM_7",
  /* 0xDF */ "OEM_8",
  /* 0xE0 */ "",
  /* 0xE1 */ "OEM_AX",
  /* 0xE2 */ "OEM_102",
  /* 0xE3 */ "ICO_HELP",
  /* 0xE4 */ "ICO_00",
  /* 0xE5 */ "PROCESSKEY",
  /* 0xE6 */ "ICO_CLEAR",
  /* 0xE7 */ "PACKET",
  /* 0xE8 */ "",
  /* 0xE9 */ "OEM_RESET",
  /* 0xEA */ "OEM_JUMP",
  /* 0xEB */ "OEM_PA1",
  /* 0xEC */ "OEM_PA2",
  /* 0xED */ "OEM_PA3",
  /* 0xEE */ "OEM_WSCTRL",
  /* 0xEF */ "OEM_CUSEL",
  /* 0xF0 */ "OEM_ATTN",
  /* 0xF1 */ "FINISH",
  /* 0xF2 */ "OEM_COPY",
  /* 0xF3 */ "OEM_AUTO",
  /* 0xF4 */ "OEM_ENLW",
  /* 0xF5 */ "OEM_BACKTAB",
  /* 0xF6 */ "ATTN",
  /* 0xF7 */ "CRSEL",
  /* 0xF8 */ "EXSEL",
  /* 0xF9 */ "EREOF",
  /* 0xFA */ "PLAY",
  /* 0xFB */ "ZOOM",
  /* 0xFC */ "NONAME",
  /* 0xFD */ "PA1",
  /* 0xFE */ "OEM_CLEAR",
  /* 0xFF */ "",
};


//
// Usermanual keys
//
const tUChar usermanual_keys[] =
{
  CPC_KEY_CUR_UP,     // 0
  CPC_KEY_CUR_RIGHT,  // 1
  CPC_KEY_CUR_DOWN,   // 2
  CPC_KEY_F9,         // 3
  CPC_KEY_F6,         // 4
  CPC_KEY_F3,         // 5
  CPC_KEY_ENTER,      // 6
  CPC_KEY_FPERIOD,    // 7
  CPC_KEY_CUR_LEFT,   // 8
  CPC_KEY_COPY,       // 9
  CPC_KEY_F7,         // 10
  CPC_KEY_F8,         // 11
  CPC_KEY_F5,         // 12
  CPC_KEY_F1,         // 13
  CPC_KEY_F2,         // 14
  CPC_KEY_F0,         // 15
  CPC_KEY_CLR,        // 16
  CPC_KEY_LBRACKET,   // 17
  CPC_KEY_RETURN,     // 18
  CPC_KEY_RBRACKET,   // 19
  CPC_KEY_F4,         // 20
  CPC_KEY_LSHIFT,     // 21
  CPC_KEY_BACKSLASH,  // 22
  CPC_KEY_CONTROL,    // 23
  CPC_KEY_POWER,      // 24
  CPC_KEY_MINUS,      // 25
  CPC_KEY_AT,         // 26
  CPC_KEY_P,          // 27
  CPC_KEY_SEMICOLON,  // 28
  CPC_KEY_COLON,      // 29
  CPC_KEY_SLASH,      // 30
  CPC_KEY_PERIOD,     // 31
  CPC_KEY_0,          // 32
  CPC_KEY_9,          // 33
  CPC_KEY_O,          // 34
  CPC_KEY_I,          // 35
  CPC_KEY_L,          // 36
  CPC_KEY_K,          // 37
  CPC_KEY_M,          // 38
  CPC_KEY_COMMA,      // 39
  CPC_KEY_8,          // 40
  CPC_KEY_7,          // 41
  CPC_KEY_U,          // 42
  CPC_KEY_Y,          // 43
  CPC_KEY_H,          // 44
  CPC_KEY_J,          // 45
  CPC_KEY_N,          // 46
  CPC_KEY_SPACE,      // 47
  CPC_KEY_6,          // 48
  CPC_KEY_5,          // 49
  CPC_KEY_R,          // 50
  CPC_KEY_T,          // 51
  CPC_KEY_G,          // 52
  CPC_KEY_F,          // 53
  CPC_KEY_B,          // 54
  CPC_KEY_V,          // 55
  CPC_KEY_4,          // 56
  CPC_KEY_3,          // 57
  CPC_KEY_E,          // 58
  CPC_KEY_W,          // 59
  CPC_KEY_S,          // 60
  CPC_KEY_D,          // 61
  CPC_KEY_C,          // 62
  CPC_KEY_X,          // 63
  CPC_KEY_1,          // 64
  CPC_KEY_2,          // 65
  CPC_KEY_ESC,        // 66
  CPC_KEY_Q,          // 67
  CPC_KEY_TAB,        // 68
  CPC_KEY_A,          // 69
  CPC_KEY_CAPSLOCK,   // 70
  CPC_KEY_Z,          // 71
  CPC_KEY_J0_UP,      // 72
  CPC_KEY_J0_DOWN,    // 73
  CPC_KEY_J0_LEFT,    // 74
  CPC_KEY_J0_RIGHT,   // 75
  CPC_KEY_J0_FIRE2,   // 76
  CPC_KEY_J0_FIRE1,   // 77
  CPC_KEY_INVALID,    // 78
  CPC_KEY_DEL,        // 79
};


const char* graphic_UK_keys_string[] =
{
  "",         // 0
  "",         // 1
  "",         // 2
  "f9",       // 3
  "f6",       // 4
  "f3",       // 5
  "ENTER",    // 6
  ".",        // 7
  "",         // 8
  "COPY",     // 9
  "f7",       // 10
  "f8",       // 11
  "f5",       // 12
  "f1",       // 13
  "f2",       // 14
  "f0",       // 15
  "CLR",      // 16
  "[",        // 17
  "RET",      // 18
  "]",        // 19
  "f4",       // 20
  "SHIFT",    // 21
  "\\",       // 22
  "CONTROL",  // 23
  "^",        // 24
  "-",        // 25
  "@",        // 26
  "P",        // 27
  ";",        // 28
  ":",        // 29
  "/",        // 30
  ".",        // 31
  "0",        // 32
  "9",        // 33
  "O",        // 34
  "I",        // 35
  "L",        // 36
  "K",        // 37
  "M",        // 38
  ",",        // 39
  "8",        // 40
  "7",        // 41
  "U",        // 42
  "Y",        // 43
  "H",        // 44
  "J",        // 45
  "N",        // 46
  "",         // 47
  "6",        // 48
  "5",        // 49
  "R",        // 50
  "T",        // 51
  "G",        // 52
  "F",        // 53
  "B",        // 54
  "V",        // 55
  "4",        // 56
  "3",        // 57
  "E",        // 58
  "W",        // 59
  "S",        // 60
  "D",        // 61
  "C",        // 62
  "X",        // 63
  "1",        // 64
  "2",        // 65
  "ESC",      // 66
  "Q",        // 67
  "TAB",      // 68
  "A",        // 69
  "CAPS",     // 70
  "Z",        // 71
  "",         // 72
  "",         // 73
  "",         // 74
  "",         // 75
  "F2",       // 76
  "F1",       // 77
  "",         // 78
  "DEL",      // 79
};

const char* graphic_FR_keys_string[] =
{
  "",         // 0
  "",         // 1
  "",         // 2
  "f9",       // 3
  "f6",       // 4
  "f3",       // 5
  "ENTER",    // 6
  ".",        // 7
  "",         // 8
  "COPY",     // 9
  "f7",       // 10
  "f8",       // 11
  "f5",       // 12
  "f1",       // 13
  "f2",       // 14
  "f0",       // 15
  "CLR",      // 16
  "*",        // 17
  "RET",      // 18
  "#",        // 19
  "f4",       // 20
  "SHIFT",    // 21
  "$",        // 22
  "CONTROL",  // 23
  "_",        // 24
  "[",        // 25
  "^",        // 26
  "P",        // 27
  "ù",        // 28
  "M",        // 29
  "=",        // 30
  ":",        // 31
  "0",        // 32
  "9",        // 33
  "O",        // 34
  "I",        // 35
  "L",        // 36
  "K",        // 37
  ",",        // 38
  ";",        // 39
  "8",        // 40
  "7",        // 41
  "U",        // 42
  "Y",        // 43
  "H",        // 44
  "J",        // 45
  "N",        // 46
  "",         // 47
  "6",        // 48
  "5",        // 49
  "R",        // 50
  "T",        // 51
  "G",        // 52
  "F",        // 53
  "B",        // 54
  "V",        // 55
  "4",        // 56
  "3",        // 57
  "E",        // 58
  "Z",        // 59
  "S",        // 60
  "D",        // 61
  "C",        // 62
  "X",        // 63
  "1",        // 64
  "2",        // 65
  "ESC",      // 66
  "A",        // 67
  "TAB",      // 68
  "Q",        // 69
  "CAPS",     // 70
  "W",        // 71
  "",         // 72
  "",         // 73
  "",         // 74
  "",         // 75
  "F2",       // 76
  "F1",       // 77
  "",         // 78
  "DEL",      // 79
};

const char* graphic_SP_keys_string[] =
{
  "",         // 0
  "",         // 1
  "",         // 2
  "f9",       // 3
  "f6",       // 4
  "f3",       // 5
  "INTRO",    // 6
  ".",        // 7
  "",         // 8
  "COPIA",     // 9
  "f7",       // 10
  "f8",       // 11
  "f5",       // 12
  "f1",       // 13
  "f2",       // 14
  "f0",       // 15
  "CLR",      // 16
  "[",        // 17
  "RET",      // 18
  "]",        // 19
  "f4",       // 20
  "MAYS",     // 21
  "\\",       // 22
  "CONTROL",  // 23
  "^",        // 24
  "-",        // 25
  "@",        // 26
  "P",        // 27
  ";",        // 28
  "Ñ",        // 29
  "/",        // 30
  ".",        // 31
  "0",        // 32
  "9",        // 33
  "O",        // 34
  "I",        // 35
  "L",        // 36
  "K",        // 37
  "M",        // 38
  ",",        // 39
  "8",        // 40
  "7",        // 41
  "U",        // 42
  "Y",        // 43
  "H",        // 44
  "J",        // 45
  "N",        // 46
  "",         // 47
  "6",        // 48
  "5",        // 49
  "R",        // 50
  "T",        // 51
  "G",        // 52
  "F",        // 53
  "B",        // 54
  "V",        // 55
  "4",        // 56
  "3",        // 57
  "E",        // 58
  "W",        // 59
  "S",        // 60
  "D",        // 61
  "C",        // 62
  "X",        // 63
  "1",        // 64
  "2",        // 65
  "ESC",      // 66
  "Q",        // 67
  "TAB",      // 68
  "A",        // 69
  "FIJA",     // 70
  "Z",        // 71
  "",         // 72
  "",         // 73
  "",         // 74
  "",         // 75
  "F2",       // 76
  "F1",       // 77
  "",         // 78
  "BOR",      // 79
};


//
// virtual keyboard keys
//
const char* virtual_kbd_string[] =
{
  /* 0x00 */ "",
  /* 0x01 */ "",
  /* 0x02 */ "",
  /* 0x03 */ "",
  /* 0x04 */ "",
  /* 0x05 */ "",
  /* 0x06 */ "",
  /* 0x07 */ "",
  /* 0x08 */ "CPC_DEL",
  /* 0x09 */ "CPC_TAB",
  /* 0x0A */ "",
  /* 0x0B */ "",
  /* 0x0C */ "",
  /* 0x0D */ "CPC_RETURN",
  /* 0x0E */ "",
  /* 0x0F */ "",
  /* 0x10 */ "",
  /* 0x11 */ "",
  /* 0x12 */ "",
  /* 0x13 */ "",
  /* 0x14 */ "CPC_CAPSLOCK",
  /* 0x15 */ "",
  /* 0x16 */ "",
  /* 0x17 */ "",
  /* 0x18 */ "",
  /* 0x19 */ "",
  /* 0x1A */ "",
  /* 0x1B */ "CPC_ESCAPE",
  /* 0x1C */ "",
  /* 0x1D */ "",
  /* 0x1E */ "",
  /* 0x1F */ "",
  /* 0x20 */ "WIN_SPACE",
  /* 0x21 */ "",
  /* 0x22 */ "",
  /* 0x23 */ "",
  /* 0x24 */ "",
  /* 0x25 */ "CPC_CUR_LEFT",
  /* 0x26 */ "CPC_CUR_UP",
  /* 0x27 */ "CPC_CUR_RIGHT",
  /* 0x28 */ "CPC_CUR_DOWN",
  /* 0x29 */ "",
  /* 0x2A */ "",
  /* 0x2B */ "",
  /* 0x2C */ "",
  /* 0x2D */ "",
  /* 0x2E */ "CPC_CLR",
  /* 0x2F */ "",
  /* 0x30 */ "",
  /* 0x31 */ "",
  /* 0x32 */ "",
  /* 0x33 */ "",
  /* 0x34 */ "",
  /* 0x35 */ "",
  /* 0x36 */ "",
  /* 0x37 */ "",
  /* 0x38 */ "",
  /* 0x39 */ "",
  /* 0x3A */ "",
  /* 0x3B */ "",
  /* 0x3C */ "",
  /* 0x3D */ "",
  /* 0x3E */ "",
  /* 0x3F */ "",
  /* 0x40 */ "",
  /* 0x41 */ "",
  /* 0x42 */ "",
  /* 0x43 */ "",
  /* 0x44 */ "",
  /* 0x45 */ "",
  /* 0x46 */ "",
  /* 0x47 */ "",
  /* 0x48 */ "",
  /* 0x49 */ "",
  /* 0x4A */ "",
  /* 0x4B */ "",
  /* 0x4C */ "",
  /* 0x4D */ "",
  /* 0x4E */ "",
  /* 0x4F */ "",
  /* 0x50 */ "",
  /* 0x51 */ "",
  /* 0x52 */ "",
  /* 0x53 */ "",
  /* 0x54 */ "",
  /* 0x55 */ "",
  /* 0x56 */ "",
  /* 0x57 */ "",
  /* 0x58 */ "",
  /* 0x59 */ "",
  /* 0x5A */ "",
  /* 0x5B */ "",
  /* 0x5C */ "",
  /* 0x5D */ "",
  /* 0x5E */ "",
  /* 0x5F */ "",
  /* 0x60 */ "CPC_F0",
  /* 0x61 */ "CPC_F1",
  /* 0x62 */ "CPC_F2",
  /* 0x63 */ "CPC_F3",
  /* 0x64 */ "CPC_F4",
  /* 0x65 */ "CPC_F5",
  /* 0x66 */ "CPC_F6",
  /* 0x67 */ "CPC_F7",
  /* 0x68 */ "CPC_F8",
  /* 0x69 */ "CPC_F9",
  /* 0x6A */ "",
  /* 0x6B */ "WIN_ENTER",
  /* 0x6C */ "WIN_SEPARATOR",
  /* 0x6D */ "",
  /* 0x6E */ "",
  /* 0x6F */ "",
  /* 0x70 */ "WIN_F1",
  /* 0x71 */ "WIN_F2",
  /* 0x72 */ "WIN_F3",
  /* 0x73 */ "WIN_F4",
  /* 0x74 */ "WIN_F5",
  /* 0x75 */ "WIN_F6",
  /* 0x76 */ "WIN_F7",
  /* 0x77 */ "WIN_F8",
  /* 0x78 */ "WIN_F9",
  /* 0x79 */ "WIN_F10",
  /* 0x7A */ "WIN_F11",
  /* 0x7B */ "WIN_F12",
  /* 0x7C */ "WIN_F13",
  /* 0x7D */ "WIN_F14",
  /* 0x7E */ "WIN_F15",
  /* 0x7F */ "WIN_F16",
  /* 0x80 */ "WIN_F17",
  /* 0x81 */ "WIN_F18",
  /* 0x82 */ "WIN_F19",
  /* 0x83 */ "WIN_F20",
  /* 0x84 */ "WIN_F21",
  /* 0x85 */ "WIN_F22",
  /* 0x86 */ "WIN_F23",
  /* 0x87 */ "WIN_F24",
  /* 0x88 */ "",
  /* 0x89 */ "",
  /* 0x8A */ "",
  /* 0x8B */ "",
  /* 0x8C */ "",
  /* 0x8D */ "",
  /* 0x8E */ "",
  /* 0x8F */ "",
  /* 0x90 */ "",
  /* 0x91 */ "",
  /* 0x92 */ "CPC_ENTER",
  /* 0x93 */ "",
  /* 0x94 */ "",
  /* 0x95 */ "",
  /* 0x96 */ "",
  /* 0x97 */ "",
  /* 0x98 */ "",
  /* 0x99 */ "",
  /* 0x9A */ "",
  /* 0x9B */ "",
  /* 0x9C */ "",
  /* 0x9D */ "",
  /* 0x9E */ "",
  /* 0x9F */ "",
  /* 0xA0 */ "",
  /* 0xA1 */ "",
  /* 0xA2 */ "",
  /* 0xA3 */ "",
  /* 0xA4 */ "",
  /* 0xA5 */ "",
  /* 0xA6 */ "",
  /* 0xA7 */ "",
  /* 0xA8 */ "",
  /* 0xA9 */ "",
  /* 0xAA */ "",
  /* 0xAB */ "",
  /* 0xAC */ "",
  /* 0xAD */ "",
  /* 0xAE */ "",
  /* 0xAF */ "",
  /* 0xB0 */ "",
  /* 0xB1 */ "",
  /* 0xB2 */ "",
  /* 0xB3 */ "",
  /* 0xB4 */ "",
  /* 0xB5 */ "",
  /* 0xB6 */ "",
  /* 0xB7 */ "",
  /* 0xB8 */ "",
  /* 0xB9 */ "",
  /* 0xBA */ "",
  /* 0xBB */ "",
  /* 0xBC */ "",
  /* 0xBD */ "",
  /* 0xBE */ "",
  /* 0xBF */ "",
  /* 0xC0 */ "",
  /* 0xC1 */ "CPC_JOY0_UP",
  /* 0xC2 */ "CPC_JOY0_DOWN",
  /* 0xC3 */ "CPC_JOY0_LEFT",
  /* 0xC4 */ "CPC_JOY0_RIGHT",
  /* 0xC5 */ "CPC_JOY0_FIRE1",
  /* 0xC6 */ "CPC_JOY0_FIRE2",
  /* 0xC7 */ "CPC_JOY0_SPARE",
  /* 0xC8 */ "CPC_JOY1_UP",
  /* 0xC9 */ "CPC_JOY1_DOWN",
  /* 0xCA */ "CPC_JOY1_LEFT",
  /* 0xCB */ "CPC_JOY1_RIGHT",
  /* 0xCC */ "CPC_JOY1_FIRE1",
  /* 0xCD */ "CPC_JOY1_FIRE2",
  /* 0xCE */ "CPC_JOY1_SPARE",
  /* 0xCF */ "",
  /* 0xD0 */ "",
  /* 0xD1 */ "",
  /* 0xD2 */ "",
  /* 0xD3 */ "",
  /* 0xD4 */ "",
  /* 0xD5 */ "",
  /* 0xD6 */ "",
  /* 0xD7 */ "",
  /* 0xD8 */ "",
  /* 0xD9 */ "",
  /* 0xDA */ "",
  /* 0xDB */ "",
  /* 0xDC */ "",
  /* 0xDD */ "",
  /* 0xDE */ "",
  /* 0xDF */ "",
  /* 0xE0 */ "",
  /* 0xE1 */ "",
  /* 0xE2 */ "",
  /* 0xE3 */ "",
  /* 0xE4 */ "",
  /* 0xE5 */ "",
  /* 0xE6 */ "",
  /* 0xE7 */ "",
  /* 0xE8 */ "",
  /* 0xE9 */ "",
  /* 0xEA */ "",
  /* 0xEB */ "",
  /* 0xEC */ "",
  /* 0xED */ "",
  /* 0xEE */ "",
  /* 0xEF */ "",
  /* 0xF0 */ "",
  /* 0xF1 */ "",
  /* 0xF2 */ "",
  /* 0xF3 */ "",
  /* 0xF4 */ "",
  /* 0xF5 */ "",
  /* 0xF6 */ "",
  /* 0xF7 */ "",
  /* 0xF8 */ "",
  /* 0xF9 */ "",
  /* 0xFA */ "",
  /* 0xFB */ "",
  /* 0xFC */ "",
  /* 0xFD */ "",
  /* 0xFE */ "",
  /* 0xFF */ "",
};


//
// Windows ASCII mapped keys
//
const char* ascii_kbd_string[] =
{
  /* 0x00 */ "",
  /* 0x01 */ "",
  /* 0x02 */ "",
  /* 0x03 */ "",
  /* 0x04 */ "",
  /* 0x05 */ "",
  /* 0x06 */ "",
  /* 0x07 */ "",
  /* 0x08 */ "CPC_DEL",
  /* 0x09 */ "CPC_TAB",
  /* 0x0A */ "CPC_RETURN",
  /* 0x0B */ "",
  /* 0x0C */ "",
  /* 0x0D */ "CPC_RETURN",
  /* 0x0E */ "",
  /* 0x0F */ "",
  /* 0x10 */ "",
  /* 0x11 */ "",
  /* 0x12 */ "",
  /* 0x13 */ "",
  /* 0x14 */ "CPC_CAPSLOCK",
  /* 0x15 */ "",
  /* 0x16 */ "",
  /* 0x17 */ "",
  /* 0x18 */ "",
  /* 0x19 */ "",
  /* 0x1A */ "",
  /* 0x1B */ "CPC_ESC",
  /* 0x1C */ "",
  /* 0x1D */ "",
  /* 0x1E */ "",
  /* 0x1F */ "",
  /* 0x20 */ "CPC_SPACE",
  /* 0x21 */ "CPC_EXCLAMATN",
  /* 0x22 */ "CPC_DBLQUOTE",
  /* 0x23 */ "CPC_HASH",
  /* 0x24 */ "CPC_DOLLAR",
  /* 0x25 */ "CPC_PERCENT",
  /* 0x26 */ "CPC_AMPERSAND",
  /* 0x27 */ "CPC_QUOTE",
  /* 0x28 */ "CPC_LEFTPAREN",
  /* 0x29 */ "CPC_RIGHTPAREN",
  /* 0x2A */ "CPC_ASTERISK",
  /* 0x2B */ "CPC_PLUS",
  /* 0x2C */ "CPC_COMMA",
  /* 0x2D */ "CPC_MINUS",
  /* 0x2E */ "CPC_PERIOD",
  /* 0x2F */ "CPC_SLASH",
  /* 0x30 */ "CPC_0",
  /* 0x31 */ "CPC_1",
  /* 0x32 */ "CPC_2",
  /* 0x33 */ "CPC_3",
  /* 0x34 */ "CPC_4",
  /* 0x35 */ "CPC_5",
  /* 0x36 */ "CPC_6",
  /* 0x37 */ "CPC_7",
  /* 0x38 */ "CPC_8",
  /* 0x39 */ "CPC_9",
  /* 0x3A */ "CPC_COLON",
  /* 0x3B */ "CPC_SEMICOLON",
  /* 0x3C */ "CPC_LESS",
  /* 0x3D */ "CPC_EQUAL",
  /* 0x3E */ "CPC_GREATER",
  /* 0x3F */ "CPC_QUESTION",
  /* 0x40 */ "CPC_AT",
  /* 0x41 */ "CPC_A",
  /* 0x42 */ "CPC_B",
  /* 0x43 */ "CPC_C",
  /* 0x44 */ "CPC_D",
  /* 0x45 */ "CPC_E",
  /* 0x46 */ "CPC_F",
  /* 0x47 */ "CPC_G",
  /* 0x48 */ "CPC_H",
  /* 0x49 */ "CPC_I",
  /* 0x4A */ "CPC_J",
  /* 0x4B */ "CPC_K",
  /* 0x4C */ "CPC_L",
  /* 0x4D */ "CPC_M",
  /* 0x4E */ "CPC_N",
  /* 0x4F */ "CPC_O",
  /* 0x50 */ "CPC_P",
  /* 0x51 */ "CPC_Q",
  /* 0x52 */ "CPC_R",
  /* 0x53 */ "CPC_S",
  /* 0x54 */ "CPC_T",
  /* 0x55 */ "CPC_U",
  /* 0x56 */ "CPC_V",
  /* 0x57 */ "CPC_W",
  /* 0x58 */ "CPC_X",
  /* 0x59 */ "CPC_Y",
  /* 0x5A */ "CPC_Z",
  /* 0x5B */ "CPC_LBRACKET",
  /* 0x5C */ "CPC_BACKSLASH",
  /* 0x5D */ "CPC_RBRACKET",
  /* 0x5E */ "CPC_POWER",
  /* 0x5F */ "CPC_UNDERSCORE",
  /* 0x60 */ "",
  /* 0x61 */ "CPC_a",
  /* 0x62 */ "CPC_b",
  /* 0x63 */ "CPC_c",
  /* 0x64 */ "CPC_d",
  /* 0x65 */ "CPC_e",
  /* 0x66 */ "CPC_f",
  /* 0x67 */ "CPC_g",
  /* 0x68 */ "CPC_h",
  /* 0x69 */ "CPC_i",
  /* 0x6A */ "CPC_j",
  /* 0x6B */ "CPC_k",
  /* 0x6C */ "CPC_l",
  /* 0x6D */ "CPC_m",
  /* 0x6E */ "CPC_n",
  /* 0x6F */ "CPC_o",
  /* 0x70 */ "CPC_p",
  /* 0x71 */ "CPC_q",
  /* 0x72 */ "CPC_r",
  /* 0x73 */ "CPC_s",
  /* 0x74 */ "CPC_t",
  /* 0x75 */ "CPC_u",
  /* 0x76 */ "CPC_v",
  /* 0x77 */ "CPC_w",
  /* 0x78 */ "CPC_x",
  /* 0x79 */ "CPC_y",
  /* 0x7A */ "CPC_z",
  /* 0x7B */ "CPC_LCBRACE",
  /* 0x7C */ "CPC_PIPE",
  /* 0x7D */ "CPC_RCBRACE",
  /* 0x7E */ "CPC_CLR",
  /* 0x7F */ "",
  /* 0x80 */ "",
  /* 0x81 */ "",
  /* 0x82 */ "",
  /* 0x83 */ "",
  /* 0x84 */ "",
  /* 0x85 */ "",
  /* 0x86 */ "",
  /* 0x87 */ "",
  /* 0x88 */ "",
  /* 0x89 */ "",
  /* 0x8A */ "",
  /* 0x8B */ "",
  /* 0x8C */ "",
  /* 0x8D */ "",
  /* 0x8E */ "",
  /* 0x8F */ "",
  /* 0x90 */ "",
  /* 0x91 */ "",
  /* 0x92 */ "",
  /* 0x93 */ "",
  /* 0x94 */ "",
  /* 0x95 */ "",
  /* 0x96 */ "",
  /* 0x97 */ "",
  /* 0x98 */ "",
  /* 0x99 */ "",
  /* 0x9A */ "",
  /* 0x9B */ "",
  /* 0x9C */ "",
  /* 0x9D */ "",
  /* 0x9E */ "CPC_ES_PESETA",
  /* 0x9F */ "",
  /* 0xA0 */ "",
  /* 0xA1 */ "",
  /* 0xA2 */ "",
  /* 0xA3 */ "CPC_POUND",
  /* 0xA4 */ "",
  /* 0xA5 */ "",
  /* 0xA6 */ "",
  /* 0xA7 */ "",
  /* 0xA8 */ "",
  /* 0xA9 */ "",
  /* 0xAA */ "",
  /* 0xAB */ "",
  /* 0xAC */ "",
  /* 0xAD */ "",
  /* 0xAE */ "",
  /* 0xAF */ "",
  /* 0xB0 */ "",
  /* 0xB1 */ "",
  /* 0xB2 */ "",
  /* 0xB3 */ "",
  /* 0xB4 */ "",
  /* 0xB5 */ "",
  /* 0xB6 */ "",
  /* 0xB7 */ "",
  /* 0xB8 */ "",
  /* 0xB9 */ "",
  /* 0xBA */ "",
  /* 0xBB */ "",
  /* 0xBC */ "",
  /* 0xBD */ "",
  /* 0xBE */ "",
  /* 0xBF */ "",
  /* 0xC0 */ "",
  /* 0xC1 */ "",
  /* 0xC2 */ "",
  /* 0xC3 */ "",
  /* 0xC4 */ "",
  /* 0xC5 */ "",
  /* 0xC6 */ "",
  /* 0xC7 */ "",
  /* 0xC8 */ "",
  /* 0xC9 */ "",
  /* 0xCA */ "",
  /* 0xCB */ "",
  /* 0xCC */ "",
  /* 0xCD */ "",
  /* 0xCE */ "",
  /* 0xCF */ "",
  /* 0xD0 */ "",
  /* 0xD1 */ "CPC_ES_NTILDE",
  /* 0xD2 */ "",
  /* 0xD3 */ "",
  /* 0xD4 */ "",
  /* 0xD5 */ "",
  /* 0xD6 */ "",
  /* 0xD7 */ "",
  /* 0xD8 */ "",
  /* 0xD9 */ "",
  /* 0xDA */ "",
  /* 0xDB */ "",
  /* 0xDC */ "",
  /* 0xDD */ "",
  /* 0xDE */ "",
  /* 0xDF */ "",
  /* 0xE0 */ "CPC_FR_aGRAVE",
  /* 0xE1 */ "",
  /* 0xE2 */ "",
  /* 0xE3 */ "",
  /* 0xE4 */ "",
  /* 0xE5 */ "",
  /* 0xE6 */ "",
  /* 0xE7 */ "CPC_FR_cCEDIL",
  /* 0xE8 */ "CPC_FR_eGRAVE",
  /* 0xE9 */ "CPC_FR_aACUTE",
  /* 0xEA */ "",
  /* 0xEB */ "",
  /* 0xEC */ "",
  /* 0xED */ "",
  /* 0xEE */ "",
  /* 0xEF */ "",
  /* 0xF0 */ "",
  /* 0xF1 */ "CPC_ES_nTILDE",
  /* 0xF2 */ "",
  /* 0xF3 */ "",
  /* 0xF4 */ "",
  /* 0xF5 */ "",
  /* 0xF6 */ "",
  /* 0xF7 */ "",
  /* 0xF8 */ "",
  /* 0xF9 */ "CPC_FR_uGRAVE",
  /* 0xFA */ "",
  /* 0xFB */ "",
  /* 0xFC */ "",
  /* 0xFD */ "",
  /* 0xFE */ "",
  /* 0xFF */ "",
};


