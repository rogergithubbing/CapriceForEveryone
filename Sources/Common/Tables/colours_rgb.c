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

#include "colours_rgb.h"


/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


colours_rgb_entry colours_rgb[NB_COLORS] =
{
  // Red,      Green,    Blue,     Intensity, Shadow_index
   { LUM_HALF, LUM_HALF, LUM_HALF, 0,         32 },   // 00 Blanc            (13) -> #40
   { LUM_HALF, LUM_HALF, LUM_HALF, 0,         32 },   // 01 Blanc            (13) -> #41
   { LUM_LOW , LUM_HIGH, LUM_HALF, 0,         22 },   // 02 Vert Marin       (19) -> #42
   { LUM_HIGH, LUM_HIGH, LUM_HALF, 0,         30 },   // 03 Jaune Pastel     (25) -> #43
   { LUM_LOW , LUM_LOW , LUM_HALF, 0,         32 },   // 04 Bleu              (1) -> #44
   { LUM_HIGH, LUM_LOW , LUM_HALF, 0,         24 },   // 05 Pourpre           (7) -> #45
   { LUM_LOW , LUM_HALF, LUM_HALF, 0,         04 },   // 06 Turquoise        (10) -> #46
   { LUM_HIGH, LUM_HALF, LUM_HALF, 0,         14 },   // 07 Rose             (16) -> #47
   { LUM_HIGH, LUM_LOW , LUM_HALF, 0,         24 },   // 08 Pourpre           (7) -> #48
   { LUM_HIGH, LUM_HIGH, LUM_HALF, 0,         30 },   // 09 Jaune vif        (24) -> #49
   { LUM_HIGH, LUM_HIGH, LUM_LOW , 0,         30 },   // 10 Jaune vif        (24) -> #4A
   { LUM_HIGH, LUM_HIGH, LUM_HIGH, 0,          0 },   // 11 Blanc Brillant   (26) -> #4B
   { LUM_HIGH, LUM_LOW , LUM_LOW , 0,         28 },   // 12 Rouge vif         (6) -> #4C
   { LUM_HIGH, LUM_LOW , LUM_HIGH, 0,         24 },   // 13 Magenta vif       (8) -> #4D
   { LUM_HIGH, LUM_HALF, LUM_LOW , 0,          0 },   // 14 Orange           (15) -> #4E
   { LUM_HIGH, LUM_HALF, LUM_HIGH, 0,          5 },   // 15 Magenta pastel   (17) -> #4F
   { LUM_LOW , LUM_LOW , LUM_HALF, 0,         32 },   // 16 Bleu              (1) -> #50
   { LUM_LOW , LUM_HIGH, LUM_HALF, 0,         22 },   // 17 Vert Marin       (19) -> #51
   { LUM_LOW , LUM_HIGH, LUM_LOW , 0,         22 },   // 18 Vert vif         (18) -> #52
   { LUM_LOW , LUM_HIGH, LUM_HIGH, 0,          6 },   // 19 Turquoise vif    (20) -> #53
   { LUM_LOW , LUM_LOW , LUM_LOW , 0,         32 },   // 20 Noir              (0) -> #54
   { LUM_LOW , LUM_LOW , LUM_HIGH, 0,          4 },   // 21 Bleu vif          (2) -> #55
   { LUM_LOW , LUM_HALF, LUM_LOW , 0,         32 },   // 22 Vert              (9) -> #56
   { LUM_LOW , LUM_HALF, LUM_HIGH, 0,         21 },   // 23 Bleu ciel        (11) -> #57
   { LUM_HALF, LUM_LOW , LUM_HALF, 0,         32 },   // 24 Magenta           (4) -> #58
   { LUM_HALF, LUM_HIGH, LUM_HALF, 0,         22 },   // 25 Vert pastel      (22) -> #59
   { LUM_HALF, LUM_HIGH, LUM_LOW , 0,         22 },   // 26 Vert citron      (21) -> #5A
   { LUM_HALF, LUM_HIGH, LUM_HIGH, 0,          6 },   // 27 Turquoise pastel (23) -> #5B
   { LUM_HALF, LUM_LOW , LUM_LOW , 0,         32 },   // 28 Rouge             (3) -> #5C
   { LUM_HALF, LUM_LOW , LUM_HIGH, 0,         32 },   // 29 Mauve             (5) -> #5D
   { LUM_HALF, LUM_HALF, LUM_LOW , 0,         32 },   // 30 Jaune            (12) -> #5E
   { LUM_HALF, LUM_HALF, LUM_HIGH, 0,         21 }    // 31 Bleu pastel      (14) -> #5F
};


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
