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

#ifndef IPF_H
#define IPF_H

#include "..\Engine\types.h"


#define IPF_CAPS_HEADER_STRING                "CAPS"
#define IPF_INFO_HEADER_STRING                "INFO"
#define IPF_IMAGE_HEADER_STRING               "IMGE"
#define IPF_DATA_HEADER_STRING                "DATA"

#define IPF_RECORD_HEADER_SIZE                12
#define IPF_CAPS_RECORD_SIZE                  IPF_RECORD_HEADER_SIZE
#define IPF_INFO_RECORD_SIZE                  96
#define IPF_IMGE_RECORD_SIZE                  80
#define IPF_DATA_RECORD_SIZE                  28
#define IPF_DATA_BLOCK_DESCRIPTOR_SIZE        32


#define IPF_NB_PLATFORMS                      4
#define IPF_PLATFORM_AMSTRAD_CPC              4

#define IPF_ENCODERTYPE_CAPS                  1
#define IPF_ENCODERTYPE_SPS                   2

#define IPF_DATATYPE_SYNC                     1
#define IPF_DATATYPE_DATA                     2
#define IPF_DATATYPE_GAP                      3
#define IPF_DATATYPE_RAW                      4
#define IPF_DATATYPE_FUZZY                    5


typedef struct
{
  tULong Identifier;
  tULong RecordSize;
  tULong RecordCrc32;
} tIPFRecordHeader;


typedef struct
{
  tULong mediaType;
  tULong encoderType;
  tULong encoderRev;
  tULong fileKey;
  tULong fileRev;
  tULong origin;
  tULong minTrack;
  tULong maxTrack;
  tULong minSide;
  tULong maxSide;
  tULong creationDate;
  tULong creationTime;
  tULong platforms[IPF_NB_PLATFORMS];
  tULong diskNumber;
  tULong creatorId;
  tULong reserved[3];
} tIPFInfoBlock;


typedef struct
{
  tULong track;
  tULong side;
  tULong density;
  tULong signalType;
  tULong trackBytes;
  tULong startBytePos;
  tULong startBitPos;
  tULong dataBits;
  tULong gapBits;
  tULong trackBits;
  tULong blockCount;
  tULong encoderProcess;
  union
  {
    struct
    {
      tULong Fuzzy:1;
    } bits;
    tULong All;
  } flags;
  tULong dataKey;
  tULong reserved[3];
} tIPFTrackBlock;


typedef struct
{
  tULong length;
  tULong bitSize;
  tULong crc;
  tULong dataKey;
} tIPFDataBlock;


typedef struct
{
  tULong dataBits;
  tULong gapBits;
  union
  {
    struct
    {
      tULong dataBytes;
      tULong gapBytes;
    } CAPS;
    struct
    {
      tULong gapOffset;
      tULong cellType;
    } SPS;
  } encoder;
  tULong encoderType;
  union
  {
    struct
    {
      tULong ForwardGap:1;
      tULong BackwardGap:1;
      tULong DataInBit:1;
    } bits;
    tULong All;
  } flags;
  tULong gapDefault;
  tULong dataOffset;
} tIPFDataBlockDescriptor;

#endif /* ! IPF_H */
