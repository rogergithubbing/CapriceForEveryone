/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2013-2016 by Frédéric Coste

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


#ifndef FDC_COMMAND_H
#define FDC_COMMAND_H

#include "..\Engine\types.h"
#include "..\Engine\Native_CPC.h"

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CMD_COUNT 15
#define FDC_TO_CPU	0
#define CPU_TO_FDC	1

#ifdef ENABLE_METROLOGY
  #define ADD_FDCCOMMANDTABLE_ENTRY(code, cmd_length, res_length, direction, label) \
  { cNull, code, cmd_length, res_length, direction, label },
#else /* ENABLE_METROLOGY */
  #define ADD_FDCCOMMANDTABLE_ENTRY(code, cmd_length, res_length, direction, label) \
  { cNull, code, cmd_length, res_length, direction },
#endif /* ENABLE_METROLOGY */


fdc_cmd_table_def FDCCommandTable[MAX_CMD_COUNT] =
{
// syntax is:
//   command code, number of bytes for command, number of bytes for result, direction, pointer to command handler
   ADD_FDCCOMMANDTABLE_ENTRY(0x03, 3, 0, FDC_TO_CPU, "SPECIFY")       // specify
   ADD_FDCCOMMANDTABLE_ENTRY(0x04, 2, 1, FDC_TO_CPU, "DRVSTAT")       // sense device status
   ADD_FDCCOMMANDTABLE_ENTRY(0x07, 2, 0, FDC_TO_CPU, "RECALIB")       // recalibrate
   ADD_FDCCOMMANDTABLE_ENTRY(0x08, 1, 2, FDC_TO_CPU, "INTSTAT")       // sense interrupt status
   ADD_FDCCOMMANDTABLE_ENTRY(0x0f, 3, 0, FDC_TO_CPU, "SEEK")          // seek
   ADD_FDCCOMMANDTABLE_ENTRY(0x42, 9, 7, FDC_TO_CPU, "READTRK")       // read diagnostic
   ADD_FDCCOMMANDTABLE_ENTRY(0x45, 9, 7, CPU_TO_FDC, "WRITE")         // write data
   ADD_FDCCOMMANDTABLE_ENTRY(0x46, 9, 7, FDC_TO_CPU, "READ")          // read data
   ADD_FDCCOMMANDTABLE_ENTRY(0x49, 9, 7, CPU_TO_FDC, "WRITE")         // write deleted data
   ADD_FDCCOMMANDTABLE_ENTRY(0x4a, 2, 7, FDC_TO_CPU, "READID")        // read id
   ADD_FDCCOMMANDTABLE_ENTRY(0x4c, 9, 7, FDC_TO_CPU, "READ")          // read deleted data
   ADD_FDCCOMMANDTABLE_ENTRY(0x4d, 6, 7, CPU_TO_FDC, "FORMAT")        // format a track
   ADD_FDCCOMMANDTABLE_ENTRY(0x51, 9, 7, CPU_TO_FDC, "SCAN EQ")       // scan equal
   ADD_FDCCOMMANDTABLE_ENTRY(0x59, 9, 7, CPU_TO_FDC, "SCAN LE")       // scan low or equal
   ADD_FDCCOMMANDTABLE_ENTRY(0x5d, 9, 7, CPU_TO_FDC, "SCAN HE")       // scan high or equal
};

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* DISK_FORMAT_H */
