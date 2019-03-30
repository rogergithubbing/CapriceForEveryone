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

#ifndef COLOURS_GREEN_H
#define COLOURS_GREEN_H

#include "..\..\Engine\Native_CPC.h"

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


#define NB_COLORS            32

extern colours_rgb_entry green_rgb[NB_COLORS];

extern void PrepareGreenColoursTable(void);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* COLOURS_GREEN_H */
