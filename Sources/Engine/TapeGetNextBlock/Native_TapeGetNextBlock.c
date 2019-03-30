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


#undef USE_BLOCK_2B
#define USE_BLOCK_2B


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


//
// Routines
//
extern tVoid Tape_GetCycleCount(tTape* Tape);
extern tLong Tape_ReadDataBit(tTape* Tape);
extern tLong Tape_ReadSampleDataBit(tTape* Tape);


//
// TestU
//


/***********************************************************************
 *
 *  Entry Points
 *
 ***********************************************************************/
tLong Engine_TapeGetNextBlock(tTape* Tape)
/***********************************************************************
 *
 *  Engine_TapeGetNextBlock
 *
 ***********************************************************************/
{
  while (Tape->pbTapeBlock < Tape->pbTapeImageEnd) // loop until a valid block is found
  {
    switch (*Tape->pbTapeBlock)
    {
      case 0x10: // standard speed data block
      {
        Tape->dwTapeStage = TAPE_PILOT_STAGE; // block starts with a pilot tone
        Tape->dwTapePulseCycles = CYCLE_ADJUST(2168);
#ifdef USE_TAPE_SPEED_ADJUST
        Tape->dwTapePulseCycles = Tape->dwTapePulseCycles * 100 / Tape->dwTapeSpeedAdjust;
#endif /* USE_TAPE_SPEED_ADJUST */
        Tape->iTapeCycleCount += (tLong)Tape->dwTapePulseCycles; // set cycle count for current level
        Tape->dwTapePulseCount = 3220;
        return 1;
      }

      case 0x11: // turbo loading data block
      {
        Tape->dwTapeStage = TAPE_PILOT_STAGE; // block starts with a pilot tone
        Tape->dwTapePulseCycles = CYCLE_ADJUST(*(tUShort *)(Tape->pbTapeBlock+0x01));
#ifdef USE_TAPE_SPEED_ADJUST
        Tape->dwTapePulseCycles = Tape->dwTapePulseCycles * 100 / Tape->dwTapeSpeedAdjust;
#endif /* USE_TAPE_SPEED_ADJUST */
        Tape->iTapeCycleCount += (tLong)Tape->dwTapePulseCycles; // set cycle count for current level
        Tape->dwTapePulseCount = *(tUShort *)(Tape->pbTapeBlock+0x01+0x0a);
        return 1;
      }

      case 0x12: // pure tone
      {
        Tape->dwTapeStage = TAPE_PILOT_STAGE; // block starts with a pilot tone
        Tape->dwTapePulseCycles = CYCLE_ADJUST(*(tUShort *)(Tape->pbTapeBlock+0x01));
#ifdef USE_TAPE_SPEED_ADJUST
        Tape->dwTapePulseCycles = Tape->dwTapePulseCycles * 100 / Tape->dwTapeSpeedAdjust;
#endif /* USE_TAPE_SPEED_ADJUST */
        Tape->iTapeCycleCount += (tLong)Tape->dwTapePulseCycles; // set cycle count for current level
        Tape->dwTapePulseCount = *(tUShort*)(Tape->pbTapeBlock+0x01+0x02);
        return 1;
      }

      case 0x13: // sequence of pulses of different length
      {
        Tape->dwTapeStage = TAPE_SYNC_STAGE;
        Tape->dwTapePulseCount = *(Tape->pbTapeBlock+0x01);
        Tape->pwTapePulseTable =
        Tape->pwTapePulseTablePtr = (tUShort*)(Tape->pbTapeBlock+0x01+0x01);
        Tape->pwTapePulseTableEnd = Tape->pwTapePulseTable + Tape->dwTapePulseCount;
        Tape_GetCycleCount(Tape);
        Tape->iTapeCycleCount += (tLong)Tape->dwTapePulseCycles; // set cycle count for current level
        return 1;
      }

      case 0x14: // pure data block
      {
        Tape->dwTapeStage = TAPE_DATA_STAGE;
        Tape->dwTapeZeroPulseCycles = CYCLE_ADJUST(*(tUShort *)(Tape->pbTapeBlock+0x01)); // pulse length for a zero bit
        Tape->dwTapeOnePulseCycles = CYCLE_ADJUST(*(tUShort *)(Tape->pbTapeBlock+0x01+0x02)); // pulse length for a one bit
        Tape->dwTapeDataCount = ((*(tULong*)(Tape->pbTapeBlock+0x01+0x07) & 0x00ffffff) - 1) << 3; // (byte count - 1) * 8 bits
        Tape->dwTapeDataCount += *(Tape->pbTapeBlock+0x01+0x04); // add the number of bits in the last data byte
        Tape->pbTapeBlockData = Tape->pbTapeBlock+0x01+0x0a; // pointer to the tape data
        Tape->dwTapeBitsToShift = 0;
        Tape_ReadDataBit(Tape); // get the first bit of the first data byte
        Tape->iTapeCycleCount += (tLong)Tape->dwTapePulseCycles; // set cycle count for current level
        return 1;
      }

      case 0x15: // direct recording
      {
        Tape->dwTapeStage = TAPE_SAMPLE_DATA_STAGE;
        Tape->dwTapePulseCycles = CYCLE_ADJUST(*(tUShort *)(Tape->pbTapeBlock+0x01)); // number of T states per sample
#ifdef USE_TAPE_SPEED_ADJUST
        Tape->dwTapePulseCycles = Tape->dwTapePulseCycles * 100 / Tape->dwTapeSpeedAdjust;
#endif /* USE_TAPE_SPEED_ADJUST */
        Tape->dwTapeDataCount = ((*(tULong*)(Tape->pbTapeBlock+0x01+0x05) & 0x00ffffff) - 1) << 3; // (byte count - 1) * 8 bits
        Tape->dwTapeDataCount += *(Tape->pbTapeBlock+0x01+0x04); // add the number of bits in the last data byte
        Tape->pbTapeBlockData = Tape->pbTapeBlock+0x01+0x08; // pointer to the tape data
        Tape->dwTapeBitsToShift = 0;
        Tape_ReadSampleDataBit(Tape); // get the first bit of the first data byte
        return 1;
      }

      case 0x20: // pause
      {
        if (*(tUShort *)(Tape->pbTapeBlock+0x01)) // was a pause requested?
        {
          Tape->dwTapeStage = TAPE_PAUSE_STAGE;
          Tape->dwTapePulseCycles = TAPE_PAUSE_DELAY; // start with a level opposite to the one last played
#ifdef USE_TAPE_SPEED_ADJUST
          Tape->dwTapePulseCycles = Tape->dwTapePulseCycles * 100 / Tape->dwTapeSpeedAdjust;
#endif /* USE_TAPE_SPEED_ADJUST */
          Tape->iTapeCycleCount += (tLong)Tape->dwTapePulseCycles; // set cycle count for current level
          Tape->dwTapePulseCycles = MS_TO_CYCLES(*(tUShort *)(Tape->pbTapeBlock+0x01) - 1); // get the actual pause length
#ifdef USE_TAPE_SPEED_ADJUST
          Tape->dwTapePulseCycles = Tape->dwTapePulseCycles * 100 / Tape->dwTapeSpeedAdjust;
#endif /* USE_TAPE_SPEED_ADJUST */
          Tape->dwTapePulseCount = 2; // just one pulse
          return 1;
        }
        else
        {
          Tape->pbTapeBlock += 2 + 1; // skip block if pause length is 0
        }
      }
      break;

      case 0x21: // group start
      {
        Tape->pbTapeBlock += *(Tape->pbTapeBlock+0x01) + 1 + 1; // nothing to do, skip the block
      }
      break;

      case 0x22: // group end
      {
        Tape->pbTapeBlock += 1; // nothing to do, skip the block
      }
      break;

#ifdef USE_BLOCK_2B
      case 0x2B: // signal level
      {
        Tape->bTapeLevel = (*(Tape->pbTapeBlock+0x01+0x04)) ? TAPE_LEVEL_HIGH : TAPE_LEVEL_LOW;
        Tape->pbTapeBlock += *(tULong*)(Tape->pbTapeBlock+0x01) + 4 + 1; // nothing to do, skip the block
      }
      break;
#endif /* USE_BLOCK_2B */

      case 0x30: // text description
      {
        tULong length = *(Tape->pbTapeBlock+0x01) + 1 + 1;
        if (!Tape->pbMessagesStart)
        {
          Tape->pbMessagesStart = Tape->pbTapeBlock;
        }
        Tape->pbMessagesEnd = Tape->pbTapeBlock + length - 1;
        Tape->pbTapeBlock += length; // nothing to do, skip the block
      }
      break;

      case 0x31: // message block
      {
        tULong length = *(Tape->pbTapeBlock+0x01+0x01) + 2 + 1;
        if (!Tape->pbMessagesStart)
        {
          Tape->pbMessagesStart = Tape->pbTapeBlock;
        }
        Tape->pbMessagesEnd = Tape->pbTapeBlock + length - 1; 
        Tape->pbTapeBlock += length; // nothing to do, skip the block
      }
      break;

      case 0x32: // archive info
      {
        Tape->pbTapeBlock += *(tUShort*)(Tape->pbTapeBlock+0x01) + 2 + 1; // nothing to do, skip the block
      }
      break;

      case 0x33: // hardware type
      {
        Tape->pbTapeBlock += (*(Tape->pbTapeBlock+0x01) * 3) + 1 + 1; // nothing to do, skip the block
      }
      break;

      case 0x34: // emulation info
      {
        Tape->pbTapeBlock += 8 + 1; // nothing to do, skip the block
      }
      break;

      case 0x35: // custom info block
      {
        Tape->pbTapeBlock += *(tULong*)(Tape->pbTapeBlock+0x01+0x10) + 0x14 + 1; // nothing to do, skip the block
      }
      break;

      case 0x40: // snapshot block
      {
        Tape->pbTapeBlock += (*(tULong*)(Tape->pbTapeBlock+0x01+0x01) & 0x00ffffff) + 0x04 + 1; // nothing to do, skip the block
      }
      break;

      case 0x5A: // another tzx/cdt file
      {
        Tape->pbTapeBlock += 9 + 1; // nothing to do, skip the block
      }
      break;

      default: // "extension rule"
      {
        Tape->pbTapeBlock += *(tULong*)(Tape->pbTapeBlock+0x01) + 4 + 1; // nothing to do, skip the block
      }
      break;
    }

    // Next block
    Tape->current_block++;
  }

  return 0; // we've reached the end of the image
}


//==============================================================================
//
// Routines
//
//==============================================================================


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
