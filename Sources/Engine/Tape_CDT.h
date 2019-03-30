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

#ifndef TAPE_CDT_H
#define TAPE_CDT_H

#include "Native_CPC.h"


#pragma pack(push, 1)

//
// Type 0x10: STANDARD SPEED DATA
//
// 00 2  Pause After this block in milliseconds (ms)
// 02 2  Length of following data
// 04 x  Data
//
typedef struct
{
  tUShort PauseLength;
  tUShort DataLength;
  tUChar  FirstData;
} tTapeBlockType10;

//
// Type 0x11: TURBO LOADING DATA
//
// 00 2  Length of PILOT pulse
// 02 2  Length of SYNC First pulse
// 04 2  Length of SYNC Second pulse
// 06 2  Length of ZERO bit pulse
// 08 2  Length of ONE bit pulse
// 0A 2  Length of PILOT tone (in PILOT pulses)
// 0C 1  Used bits in last byte (other bits should be 0)
//       i.e. if this is 6 then the bits (x) used in last byte are: xxxxxx00
// 0D 2  Pause After this block in milliseconds (ms)
// 0F 3  Length of following data
// 12 x  Data;
//
typedef struct
{
  tUShort PilotPulseLength;
  tUShort SyncFirstPulseLength;
  tUShort SyncSecondPulseLength;
  tUShort ZeroBitPulseLength;
  tUShort OneBitPulseLength;
  tUShort PilotToneLength;
  tUChar  UsedBitsLastByte;
  tUShort PauseLength;
  tUShort DataLengthLSB;
  tUChar  DataLengthMSB;
  tUChar  FirstData;
} tTapeBlockType11;

//
// Type 0x12: PURE TONE
//
// 00 2  Length of pulse in T-States
// 02 2  Number of pulses
//
typedef struct
{
  tUShort PulseLength;
  tUShort NumberPulses;
} tTapeBlockType12;

//
// Type 0x13: SEQUENCE OF PULSES OF DIFFERENT LENGTHS
//
// 00 1  Number of pulses
// 01 2  Length of first pulse in T-States
// 03 2  Length of second pulse...
// .. .  etc.
//
typedef struct
{
  tUChar  NumberPulses;
  tUShort FirstPulseLength;
} tTapeBlockType13;

//
// Type 0x14: PURE DATA BLOCK
//
// 00 2  Length of ZERO bit pulse
// 02 2  Length of ONE bit pulse
// 04 1  Used bits in LAST Byte
// 05 2  Pause after this block in milliseconds (ms)
// 07 3  Length of following data
// 0A x  Data (MSb first)
//
typedef struct
{
  tUShort ZeroBitPulseLength;
  tUShort OneBitPulseLength;
  tUChar  UsedBitsLastByte;
  tUShort PauseLength;
  tUShort DataLengthLSB;
  tUChar  DataLengthMSB;
  tUChar  FirstData;
} tTapeBlockType14;

//
// Type 0x15: DIRECT RECORDING
//
// 00 2  Number of T states per sample (bit of data)
// 02 2  Pause after this block in milliseconds (ms)
// 04 1  Used bits (samples) in last byte of data (1-8)
//       i.e. If this is 2 only first two samples of the last byte will be played
// 05 3  Data length
// 08 x  Data. Each bit represents a state on the EAR port (i.e. one sample);
//       MSb is played first.
//
typedef struct
{
  tUShort TStatesPerSampleNumber;
  tUShort PauseLength;
  tUChar  UsedBitsLastByte;
  tUShort DataLengthLSB;
  tUChar  DataLengthMSB;
  tUChar  FirstData;
} tTapeBlockType15;

//
// Type 0x20: PAUSE
//
// 00 2  Value of the pause in milliseconds (ms)
//
typedef struct
{
  tUShort  PauseLength;
} tTapeBlockType20;

//
// Type 0x21: GROUP START
//
// 00 1  Length of the Group Name
// 01 x  Group name in ASCII (please keep it under 30 characters long)
//
typedef struct
{
  tUChar  NameLength;
  tUChar  FirstNameChar;
} tTapeBlockType21;

//
// Type 0x2B: SET SIGNAL LEVEL
//
typedef struct
{
  tULong  BlockLength;
  tUChar  SignalLevel;
} tTapeBlockType2B;

//
// Type 0x30: TEXT DESCRIPTION
//
// 00 1  Length of the Text
// 01 x  Text in ASCII
//
typedef struct
{
  tUChar  TextLength;
  tUChar  FirstTextChar;
} tTapeBlockType30;

//
// Type 0x31: MESSAGE BLOCK
//
// 00 1  Time for which the message should be displayed in seconds (s)
// 01 1  Length of the message
// 02 x  Message that should be displayed (in ASCII)
//       Suggested format:
//       - Stick to a maximum of 30 chars per line
//       - Use single 0x0D (13 decimal) to separate lines.
//       - Stick to a maximum of 8 lines.
//       If you do not obey these rules, emulators may display your message in
//       any way they like!
typedef struct
{
  tUChar  DisplayDuration;
  tUChar  MessageLength;
  tUChar  FirstMessageChar;
} tTapeBlockType31;

//
// Type 0x32: ARCHIVE INFO
//
// 00 2  Length of the block (without these two bytes)
// 02 1  Number of text strings
// 03 x  Text strings:
//         00 1  Text Identification byte:  00 - Full Title
//                                          01 - Software House / Publisher
//                                          02 - Author(s)
//                                          03 - Year of Publication
//                                          04 - Language
//                                          05 - Game/Utility Type
//                                          06 - Price
//                                          07 - Protection Scheme / Loader
//                                          08 - Origin
//                                          FF - Comment(s)
//         01 1  Length of text
//         02 x  Text in ASCII format
//         .. .  Next Text//
//
typedef struct
{
  tUShort BlockLength;
  tUChar  NumberOfTextStrings;
  tUChar  FirstTextID;
  tUChar  FirstTextLength;
  tUChar  FirstTextChar;
} tTapeBlockType32;

//
// Type 0x33: HARDWARE TYPE
//
// 00 1  Number of machines and hardware types for which info is supplied
// 01 x  List of machines and hardware:
//       00 1  Hardware type
//       01 1  Hardware ID
//       02 1  Value
//             00 - The tape RUNS on this machine or with this hardware, but may
//                  or may not use the hardware or special features of the
//                  machine.
//             01 - The tape USES the hardware or special features of the
//                  machine, such as extra memory or a sound chip.
//             02 - The tape RUNS but it DOESN'T use the hardware or special
//                  features of the machine.
//             03 - The tape DOESN'T RUN on this machine or with this hardware.
//       .. .  Next Machine/Hardware Info (if any)
//
//         The list of types and IDs is somewhat large, and may be found at the
//         end of the format description.
//
typedef struct
{
  tUChar  NumberOfMachines;
  tUChar  FirstHardwareType;
  tUChar  FirstHardwareID;
  tUChar  FirstValue;
} tTapeBlockType33;


//
// Header record
//
typedef struct
{
  tChar Filename[16];
  tUChar BlockNumber;
  tUChar LastBlock;
  tUChar FileType;
  tUShort DataLength;
  tUShort DataLocation;
  tUChar FirstBlock;
  tUShort LogicalLength;
  tUShort EntryAddress;
} tTapeHeaderRecord;

#pragma pack(pop)

#endif /* ! TAPE_CDT_H */
