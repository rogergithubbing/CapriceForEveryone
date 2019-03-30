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
tLong Engine_TapeGetPrevMessageBlock(tTape* Tape)
/***********************************************************************
 *
 *  Engine_TapeGetPrevMessageBlock
 *
 ***********************************************************************/
{
tULong Block = Tape->current_block;
tTapeBlock* TapeBlock;

  // No blocks ?
  if (!Tape->nb_blocks) return (0);

  // First block ?
  if (!Block) return (0);

  // First find data block
  while (--Block)
  {
    if ( (Tape->blocks[Block].block_id == 0x10)
         || (Tape->blocks[Block].block_id == 0x11)
         || (Tape->blocks[Block].block_id == 0x12)
         || (Tape->blocks[Block].block_id == 0x13)
         || (Tape->blocks[Block].block_id == 0x14)
         || (Tape->blocks[Block].block_id == 0x15) )
    {
      break;
    }
  }

  // No Data block found ?
  if (!Block) Block = Tape->current_block;

  // Now find message block
  while (--Block)
  {
    if ( (Tape->blocks[Block].block_id == 0x30)
       || (Tape->blocks[Block].block_id == 0x31) )
    {
      break;
    }
  }

  // No message block found ?
  if (!Block)
  {
    Engine_TapeRewind(Tape);
    return (0); // Begin of tape
  }

  // Find first message block
  while ( (Tape->blocks[Block].block_id == 0x30)
          || (Tape->blocks[Block].block_id == 0x31) )
  {
    Block--;
  }

  TapeBlock = &Tape->blocks[Block+1];
  Tape->pbTapeBlock = TapeBlock->pbImageData;
  Tape->current_block = Block+1;
  Tape->iTapeCycleCount = TAPE_PLAY_START_PAUSE;
  Tape->bTapeLevel = TAPE_LEVEL_LOW;

  return (Engine_TapeGetNextBlock(Tape));
}



//==============================================================================
//
// Routines
//
//==============================================================================


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
