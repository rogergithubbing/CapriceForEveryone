/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014 by Frédéric Coste

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


#ifndef DISK_FORMAT_H
#define DISK_FORMAT_H

#include "..\Engine\types.h"

//---------------------------------------------------------------------------
#define MAX_DISK_FORMAT 										2

#if defined(__PALMOS__)
typedef struct
{
   tUChar label[40]; // label to display in options dialog
   tULong tracks; // number of tracks
   tULong sides; // number of sides
   tULong sectors; // sectors per track
   tULong sector_size; // sector size as N value
   tULong gap3_length; // GAP#3 size
   tUChar filler_byte; // default byte to use
   tUChar sector_ids[2][16]; // sector IDs
} tDiskFormat;
#endif


const tDiskFormat disk_format[MAX_DISK_FORMAT] =
{
	{ "178K Data Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0xc1, 0xc6, 0xc2, 0xc7, 0xc3, 0xc8, 0xc4, 0xc9, 0xc5 }} },
  { "169K Vendor Format", 40, 1, 9, 2, 0x52, 0xe5, {{ 0x41, 0x46, 0x42, 0x47, 0x43, 0x48, 0x44, 0x49, 0x45 }} }
};


#endif /* DISK_FORMAT_H */
