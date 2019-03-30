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

#ifndef RAW_H
#define RAW_H

#include "..\Engine\types.h"


#define RAW_CAPS_HEADER_STRING                "CAPS"
#define RAW_DATA_HEADER_STRING                "DATA"
#define RAW_PACK_HEADER_STRING                "PACK"
#define RAW_TRACK_HEADER_STRING               "TRCK"

#define RAW_RECORD_HEADER_SIZE                12
#define RAW_CAPS_RECORD_SIZE                  RAW_RECORD_HEADER_SIZE
#define RAW_DATA_RECORD_SIZE                  28
#define RAW_TRACK_RECORD_SIZE                 28


#pragma pack(push, 1)

typedef struct
{
  tULong Identifier;
  tULong RecordSize;
  tULong RecordCrc32;
} tRAWRecordHeader;


typedef struct
{
  tULong data_size;
  tULong data_size_in_bits;
  tULong crc;
  tULong id;
  tULong density_size;
  tULong track_size;
} tRAWDataBlock;


typedef struct
{
  tULong compressed_size;
  tULong compressed_crc;
  tULong crc;
  tUChar nb_revolutions;
} tRAWPACKTrackBlock;


typedef struct
{
  tULong head_number;
  tULong track_number;
  tULong crc;
  tULong id;
} tRAWTrackBlock;

#pragma pack(pop)



#endif /* ! RAW_H */
