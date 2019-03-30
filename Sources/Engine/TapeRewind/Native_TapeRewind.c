/*
    Caprice32 - Amstrad CPC Emulator
    (c) Copyright 1997-2005 Ulrich Doewich

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

#include "..\Native_CPC.h"


/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif



//
// Routines
//


//
// TestU
//


/***********************************************************************
 *
 *  Entry Points
 *
 ***********************************************************************/
tVoid Engine_TapeRewind(tTape* Tape)
/***********************************************************************
 *
 *  Engine_TapeRewind
 *
 ***********************************************************************/
{
  // CDT blocks ?
  if (Tape->nb_blocks)
  {
    Tape->pbTapeBlock = Tape->pbTapeImage;
    Tape->current_block = 0;

    Tape->bTapeLevel = TAPE_LEVEL_LOW;
    Tape->iTapeCycleCount = 0;

    Engine_TapeGetNextBlock(Tape);
  }
  // CSW
  else if (Tape->pbCSWBuffer)
  {
    // Rewind to begin of tape
    Tape->pbCSWData = Tape->pbCSWBuffer;

    // Set first Pulse
    Tape->csw_nbsamples = *Tape->pbCSWData++;
    if (!Tape->csw_nbsamples)
    {
      if ((Tape->pbCSWEnd - Tape->pbCSWData) >= 4)
      {
        Tape->csw_nbsamples =   ((*(Tape->pbCSWData++)) << 0)
                              + ((*(Tape->pbCSWData++)) << 8)
                              + ((*(Tape->pbCSWData++)) << 16)
                              + ((*(Tape->pbCSWData++)) << 24);
      }
    }
  }
}



//==============================================================================
//
// Routines
//
//==============================================================================


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
