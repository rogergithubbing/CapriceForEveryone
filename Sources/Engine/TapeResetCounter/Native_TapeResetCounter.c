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
tVoid Engine_TapeResetCounter(tTape* Tape)
/***********************************************************************
 *
 *  Engine_TapeResetCounter
 *
 ***********************************************************************/
{
  // Reset counter during pause ?
  if ( (Tape->dwTapeStage == TAPE_PAUSE_STAGE)
       && (*Tape->pbTapeBlock != 0x20) )
  {
    // Start with next block
    Tape->counter000_block = Tape->current_block + 1;
    if (Tape->counter000_block == Tape->nb_blocks)
    {
      Tape->counter000_block--;
    }
  }
  else
  {
    // Counter 000 at current block
    Tape->counter000_block = Tape->current_block;
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
