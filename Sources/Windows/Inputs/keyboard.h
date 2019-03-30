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


#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "..\..\Engine\types.h"
#include "..\..\Engine\Keys.h"


//
// Custom Virtual Key codes
//
#define NB_KEYBOARD_KEYS                256
#define VK_ALT                          0x12
#define VK_ENTER                        0x9C
#define VK_JOY0_UP                      0xC1
#define VK_JOY0_DOWN                    0xC2
#define VK_JOY0_LEFT                    0xC3
#define VK_JOY0_RIGHT                   0xC4
#define VK_JOY0_FIRE1                   0xC5
#define VK_JOY0_FIRE2                   0xC6
#define VK_JOY0_SPARE                   0xC7
#define VK_JOY1_UP                      0xC8
#define VK_JOY1_DOWN                    0xC9
#define VK_JOY1_LEFT                    0xCA
#define VK_JOY1_RIGHT                   0xCB
#define VK_JOY1_FIRE1                   0xCC
#define VK_JOY1_FIRE2                   0xCD
#define VK_JOY1_SPARE                   0xCE


//
// Joystick keys
//
#define NB_JOYSTICK_KEYS                6
#define KEY_JOY0_UP                     0
#define KEY_JOY0_DOWN                   1
#define KEY_JOY0_LEFT                   2
#define KEY_JOY0_RIGHT                  3
#define KEY_JOY0_FIRE1                  4
#define KEY_JOY0_FIRE2                  5


typedef struct
{
  tUShort WindowsKey;
  tUChar CPCKey;
} tKeyboardMappingKey;


//
// Windows virtual keyboards
//
// Default cpc matrix keyboard for hardware mapping
extern const tUChar default_hardware_virtual_to_cpc_kbdmatrix[];
// Default virtual key for keyboard as joystick
extern const tKeyboardMappingKey default_joystick_keys[];

// CPC key index for ASCII code
extern const tUChar ascii_to_cpc_keyindex[];

extern const tKeyboardMappingKey default_non_ascii_keys[];
extern const tUChar non_ascii_keys[];
extern const tUChar usermanual_keys[];


//
// Identifier strings
//
extern const char* ctrl_key_string;
extern const char* shift_key_string;
extern const char* alt_key_string;

extern const char* cpc_keys_string[];
extern const char* windows_vk_string[];

extern const char* graphic_UK_keys_string[];
extern const char* graphic_FR_keys_string[];
extern const char* graphic_SP_keys_string[];

extern const char* virtual_kbd_string[];
extern const char* ascii_kbd_string[];

#endif /* ! KEYBOARD_H */
