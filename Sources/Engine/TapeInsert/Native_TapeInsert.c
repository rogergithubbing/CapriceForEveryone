/*
    Caprice32 - Amstrad CPC Emulator
    (c) Copyright 1997-2005 Ulrich Doewich

    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2016 by Frédéric Coste

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


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================



/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

//
// Routines
//
static tUShort MemCmp(tUChar* s1,
                      tUChar* s2,
                      tULong numBytes);
static tVoid MemMove(tUChar* destP,
                     const tUChar* sourceP,
                     tULong numBytes);
static tULong GetNbBlocks(tTape* TapeP);
static tUChar* InsertPause(tTape* TapeP, tUChar* CurrentBlock);
static tVoid ComputeDuration(tTape* TapeP);
static tULong ConvertSamplesToMs(tUShort TStatesPerSample, tULong NbSamples);
static tULong GetOneBitsNumber(tTape* Tape,
                               tUChar* Data,
                               tULong Length);
static tULong GetBlock10Duration(tTape* Tape, tUChar* BlockData);
static tULong GetBlock11Duration(tTape* Tape, tUChar* BlockData);
static tULong GetBlock12Duration(tUChar* BlockData);
static tULong GetBlock13Duration(tUChar* BlockData);
static tULong GetBlock14Duration(tTape* Tape, tUChar* BlockData);
static tULong GetBlock15Duration(tUChar* BlockData);
static tULong GetBlock20Duration(tUChar* BlockData);


//
// TestU
//


/***********************************************************************
 *
 *  Entry Points
 *
 ***********************************************************************/
tULong Engine_TapeInsert(tTape* Tape, tUChar* TapeDataP, tULong Length)
/***********************************************************************
 *
 *  Engine_TapeInsert
 *
 ***********************************************************************/
{
tMemPtrNewFct NewMemFct = (tMemPtrNewFct)Tape->MemPtrNewPtr;
tUChar* pbPtr;
tUChar* pbBlock;
tTapeBlock* pbTapeBlock;
tLong FileSize;
tULong BlockLength;
tBool GotDataBlock;
#ifdef ENABLE_TAPE_INSERT_PAUSE
tBool GotPauseBlock;
#endif /* ENABLE_TAPE_INSERT_PAUSE */
Err Result = errNone;

  pbPtr = TapeDataP;

  if (MemCmp(pbPtr, "ZXTape!\032", 8) != 0) // valid CDT file?
  {
    return vfsErrTapeBadSignature;
  }
  if (*(pbPtr + 0x08) != 1) // major version must be 1
  {
    return vfsErrTapeBadSignature;
  }
  FileSize = Length - TAPE_CDT_HEADER_SIZE;
  if (FileSize <= 0) // the tape image should have at least one block...
  {
    return vfsErrTapeBadSignature;
  }

  Tape->pbTapeImage = (tUChar*)NewMemFct(FileSize + 6); // +6 for possible 2 pauses add
  if (Tape->pbTapeImage == cNull)
  {
    return memErrNotEnoughSpace;
  }

#ifdef ENABLE_TAPE_FORCE_PAUSES

  *Tape->pbTapeImage = 0x20; // start off with a pause block
  *(tUShort*)(Tape->pbTapeImage+1) = 2000; // set the length to 2 seconds

  // append the entire CDT file
  MemMove(Tape->pbTapeImage+3,
          TapeDataP + TAPE_CDT_HEADER_SIZE,
          FileSize);

  *(Tape->pbTapeImage + FileSize + 3) = 0x20; // end with a pause block
  *(tUShort*)(Tape->pbTapeImage + FileSize + 3 + 1) = 2000; // set the length to 2 seconds

  Tape->pbTapeImageEnd = Tape->pbTapeImage + FileSize + 6;

#else /* ENABLE_TAPE_FORCE_PAUSES */

  // append the entire CDT file
  MemMove(Tape->pbTapeImage,
          TapeDataP + TAPE_CDT_HEADER_SIZE,
          FileSize);
  Tape->pbTapeImageEnd = Tape->pbTapeImage + FileSize;

#endif /* ENABLE_TAPE_FORCE_PAUSES */

  Tape->nb_blocks = GetNbBlocks(Tape);

#ifndef ENABLE_TAPE_INSERT_PAUSE
  Tape->blocks = (tTapeBlock*)NewMemFct(sizeof(tTapeBlock) * Tape->nb_blocks);
#else /* ENABLE_TAPE_INSERT_PAUSE */
  Tape->blocks = (tTapeBlock*)NewMemFct(sizeof(tTapeBlock) * (Tape->nb_blocks+1));
#endif /* ENABLE_TAPE_INSERT_PAUSE */
  if (Tape->blocks == cNull)
  {
    return memErrNotEnoughSpace;
  }

  pbTapeBlock = Tape->blocks;

  //
  // Browse all blocks
  //
  GotDataBlock = cFalse;
#ifdef ENABLE_TAPE_INSERT_PAUSE
  GotPauseBlock = cFalse;
#endif /* ENABLE_TAPE_INSERT_PAUSE */
  pbBlock = Tape->pbTapeImage;
  while (pbBlock < Tape->pbTapeImageEnd)
  {
    tUChar bID = *pbBlock;

    // Prepare block data
    pbTapeBlock->block_id = bID;
    pbTapeBlock->pbImageData = pbBlock;

    pbBlock++;
    
    switch(bID)
    {
      case 0x10: // standard speed data block
      {
#ifdef ENABLE_TAPE_INSERT_PAUSE
        if (!GotPauseBlock)
        {
          // Prepare block data
          pbTapeBlock->block_id = 0x20; // Pause
          pbTapeBlock->pbImageData = (pbBlock-1);
          // Insert 2 seconds pause
          InsertPause(Tape, pbBlock-1);
          GotPauseBlock = cTrue;
          BlockLength = 2;
          break;
        }
#endif /* ENABLE_TAPE_INSERT_PAUSE */
        BlockLength = *(tUShort*)(pbBlock+2) + 4;
        GotDataBlock = cTrue;
      }
      break;

      case 0x11: // turbo loading data block
      {
#ifdef ENABLE_TAPE_INSERT_PAUSE
        if (!GotPauseBlock)
        {
          // Prepare block data
          pbTapeBlock->block_id = 0x20; // Pause
          pbTapeBlock->pbImageData = (pbBlock-1);
          // Insert 2 seconds pause
          InsertPause(Tape, pbBlock-1);
          GotPauseBlock = cTrue;
          BlockLength = 2;
          break;
        }
#endif /* ENABLE_TAPE_INSERT_PAUSE */
        BlockLength = (*(tULong*)(pbBlock+0x0f) & 0x00ffffff) + 0x12;
        GotDataBlock = cTrue;
      }
      break;

      case 0x12: // pure tone
      {
#ifdef ENABLE_TAPE_INSERT_PAUSE
        if (!GotPauseBlock)
        {
          // Prepare block data
          pbTapeBlock->block_id = 0x20; // Pause
          pbTapeBlock->pbImageData = (pbBlock-1);
          // Insert 2 seconds pause
          InsertPause(Tape, pbBlock-1);
          GotPauseBlock = cTrue;
          BlockLength = 2;
          break;
        }
#endif /* ENABLE_TAPE_INSERT_PAUSE */
        BlockLength = 4;
        GotDataBlock = cTrue;
      }
      break;

      case 0x13: // sequence of pulses of different length
      {
#ifdef ENABLE_TAPE_INSERT_PAUSE
        if (!GotPauseBlock)
        {
          // Prepare block data
          pbTapeBlock->block_id = 0x20; // Pause
          pbTapeBlock->pbImageData = (pbBlock-1);
          // Insert 2 seconds pause
          InsertPause(Tape, pbBlock-1);
          GotPauseBlock = cTrue;
          BlockLength = 2;
          break;
        }
#endif /* ENABLE_TAPE_INSERT_PAUSE */
        BlockLength = *pbBlock * 2 + 1;
        GotDataBlock = cTrue;
      }
      break;

      case 0x14: // pure data block
      {
#ifdef ENABLE_TAPE_INSERT_PAUSE
        if (!GotPauseBlock)
        {
          // Prepare block data
          pbTapeBlock->block_id = 0x20; // Pause
          pbTapeBlock->pbImageData = (pbBlock-1);
          // Insert 2 seconds pause
          InsertPause(Tape, pbBlock-1);
          GotPauseBlock = cTrue;
          BlockLength = 2;
          break;
        }
#endif /* ENABLE_TAPE_INSERT_PAUSE */
        BlockLength = (*(tULong*)(pbBlock+0x07) & 0x00ffffff) + 0x0a;
        GotDataBlock = cTrue;
      }
      break;

      case 0x15: // direct recording
      {
#ifdef ENABLE_TAPE_INSERT_PAUSE
        if (!GotPauseBlock)
        {
          // Prepare block data
          pbTapeBlock->block_id = 0x20; // Pause
          pbTapeBlock->pbImageData = (pbBlock-1);
          // Insert 2 seconds pause
          InsertPause(Tape, pbBlock-1);
          GotPauseBlock = cTrue;
          BlockLength = 2;
          break;
        }
#endif /* ENABLE_TAPE_INSERT_PAUSE */
        BlockLength = (*(tULong*)(pbBlock+0x05) & 0x00ffffff) + 0x08;
        GotDataBlock = cTrue;
      }
      break;

      case 0x20: // pause
      {
        if ((!GotDataBlock) && (pbBlock != Tape->pbTapeImage+1))
        {
#ifdef ENABLE_TAPE_FORCE_PAUSES
          *(tUShort*)pbBlock = 0; // remove any pauses (except ours) before the data starts
#endif /* ENABLE_TAPE_FORCE_PAUSES */
        }
#ifdef ENABLE_TAPE_INSERT_PAUSE
        if ( !GotDataBlock && !GotPauseBlock )
        {
          // First pause should be at least 2000ms duration
          if ( (*(tUShort*)pbBlock) < 2000)
          {
            *(tUShort*)pbBlock = 2000;
          }
          GotPauseBlock = cTrue;
        }
#endif /* ENABLE_TAPE_INSERT_PAUSE */
        BlockLength = 2;
      }
      break;

      case 0x21: // group start
      {
        BlockLength = *pbBlock + 1;
      }
      break;

      case 0x22: // group end
      {
        BlockLength = 0;
      }
      break;

      case 0x23: // jump to block
      {
        Result = vfsErrTapeUnsupported;
        BlockLength = 2;
      }
      break;

      case 0x24: // loop start
      {
        Result = vfsErrTapeUnsupported;
        BlockLength = 2;
      }
      break;

      case 0x25: // loop end
      {
        Result = vfsErrTapeUnsupported;
        BlockLength = 0;
      }
      break;

      case 0x26: // call sequence
      {
        Result = vfsErrTapeUnsupported;
        BlockLength = (*(tUShort*)pbBlock * 2) + 2;
      }
      break;

      case 0x27: // return from sequence
      {
        Result = vfsErrTapeUnsupported;
        BlockLength = 0;
      }
      break;

      case 0x28: // select block
      {
        Result = vfsErrTapeUnsupported;
        BlockLength = *(tUShort*)pbBlock + 2;
      }
      break;

      case 0x30: // text description
      {
        BlockLength = *pbBlock + 1;
      }
      break;

      case 0x31: // message block
      {
        BlockLength = *(pbBlock+1) + 2;
      }
      break;
      
      case 0x32: // archive info
      {
        BlockLength = *(tUShort*)pbBlock + 2;
      }
      break;

      case 0x33: // hardware type
      {
        BlockLength = (*pbBlock * 3) + 1;
      }
      break;
      
      case 0x34: // emulation info
      {
        BlockLength = 8;
      }
      break;
      
      case 0x35: // custom info block
      {
        BlockLength = *(tULong*)(pbBlock+0x10) + 0x14;
      }
      break;

      case 0x40: // snapshot block
      {
        BlockLength = (*(tULong*)(pbBlock+0x01) & 0x00ffffff) + 0x04;
      }
      break;

      case 0x5A: // another tzx/cdt file
      {
        BlockLength = 9;
      }
      break;

      default: // "extension rule"
        BlockLength = *(tULong*)pbBlock + 4;
    }

    pbBlock += BlockLength;
    pbTapeBlock->length = BlockLength + 1; // Add block id
    pbTapeBlock++;
  }

  Tape->current_block = 0;
  Tape->counter000_block = 0;
  Tape->timer_from_play = 0;
  Tape->timer_play_second = 0;

  // Compute blocks durations
  ComputeDuration(Tape);

  if (pbBlock != Tape->pbTapeImageEnd)
  {
    return vfsErrTapeInvalid;
  }
  
  return Result;
}


//==============================================================================
//
// Routines
//
//==============================================================================

static tUShort MemCmp(tUChar* s1,
                      tUChar* s2,
                      tULong numBytes)
/***********************************************************************
 *
 *  MemCmp
 *
 ***********************************************************************/
{
  while (numBytes--)
  {
    if (*(s1++) != *(s2++))
      return 1;
  }
  
  return 0;
}
/*----------------------------------------------------------------------------*/

static tVoid MemMove(tUChar* destP,
                     const tUChar* sourceP,
                     tULong numBytes)
/***********************************************************************
 *
 *  MemMove
 *
 ***********************************************************************/
{
  while (numBytes--)
  {
    *(destP++) = *(sourceP++);
  }
}
/*----------------------------------------------------------------------------*/


static tULong GetNbBlocks(tTape* Tape)
/***********************************************************************
 *
 *  GetNbBlocks
 *
 ***********************************************************************/
{
tUChar* pbBlock;
tULong BlockLength;
tULong NbBlocks;

  pbBlock = Tape->pbTapeImage;
  NbBlocks = 0;

  while (pbBlock < Tape->pbTapeImageEnd)
  {
    tUChar bID = *pbBlock++;
    NbBlocks++;
    switch(bID)
    {
      case 0x10: // standard speed data block
      {
        BlockLength = *(tUShort*)(pbBlock+2) + 4;
      }
      break;

      case 0x11: // turbo loading data block
      {
        BlockLength = (*(tULong*)(pbBlock+0x0f) & 0x00ffffff) + 0x12;
      }
      break;

      case 0x12: // pure tone
      {
        BlockLength = 4;
      }
      break;

      case 0x13: // sequence of pulses of different length
      {
        BlockLength = *pbBlock * 2 + 1;
      }
      break;

      case 0x14: // pure data block
      {
        BlockLength = (*(tULong*)(pbBlock+0x07) & 0x00ffffff) + 0x0a;
      }
      break;

      case 0x15: // direct recording
      {
        BlockLength = (*(tULong*)(pbBlock+0x05) & 0x00ffffff) + 0x08;
      }
      break;

      case 0x20: // pause
      {
        BlockLength = 2;
      }
      break;

      case 0x21: // group start
      {
        BlockLength = *pbBlock + 1;
      }
      break;

      case 0x22: // group end
      {
        BlockLength = 0;
      }
      break;

      case 0x23: // jump to block
      {
        BlockLength = 2;
      }
      break;

      case 0x24: // loop start
      {
        BlockLength = 2;
      }
      break;

      case 0x25: // loop end
      {
        BlockLength = 0;
      }
      break;

      case 0x26: // call sequence
      {
        BlockLength = (*(tUShort*)pbBlock * 2) + 2;
      }
      break;

      case 0x27: // return from sequence
      {
        BlockLength = 0;
      }
      break;

      case 0x28: // select block
      {
        BlockLength = *(tUShort*)pbBlock + 2;
      }
      break;

      case 0x30: // text description
      {
        BlockLength = *pbBlock + 1;
      }
      break;

      case 0x31: // message block
      {
        BlockLength = *(pbBlock+1) + 2;
      }
      break;
      
      case 0x32: // archive info
      {
        BlockLength = *(tUShort*)pbBlock + 2;
      }
      break;

      case 0x33: // hardware type
      {
        BlockLength = (*pbBlock * 3) + 1;
      }
      break;
      
      case 0x34: // emulation info
      {
        BlockLength = 8;
      }
      break;
      
      case 0x35: // custom info block
      {
        BlockLength = *(tULong*)(pbBlock+0x10) + 0x14;
      }
      break;

      case 0x40: // snapshot block
      {
        BlockLength = (*(tULong*)(pbBlock+0x01) & 0x00ffffff) + 0x04;
      }
      break;

      case 0x5A: // another tzx/cdt file
      {
        BlockLength = 9;
      }
      break;

      default: // "extension rule"
        BlockLength = *(tULong*)pbBlock + 4;
    }

    pbBlock += BlockLength;
  }

  return (NbBlocks);
}
/*----------------------------------------------------------------------------*/


static tUChar* InsertPause(tTape* TapeP, tUChar* CurrentBlock)
/***********************************************************************
 *
 *  InsertPause
 *
 ***********************************************************************/
{
tULong FileSize = TapeP->pbTapeImageEnd - TapeP->pbTapeImage;
tULong RemainSize = FileSize - (CurrentBlock - TapeP->pbTapeImage);

  // Move remaining CDT content
  tUChar* sourceP = CurrentBlock+RemainSize-1;
  while (RemainSize--)
  {
    *(sourceP+3) = *(sourceP);
    sourceP--;
  }

  // Insert Pause
  *(CurrentBlock) = 0x20; // Pause ID
  *(tUShort*)(CurrentBlock + 1) = 2000; // set the length to 2 seconds
  TapeP->pbTapeImageEnd += 3;
  TapeP->nb_blocks++;

  return (CurrentBlock+3);
}


static tVoid ComputeDuration(tTape* Tape)
/***********************************************************************
 *
 *  ComputeDuration
 *
 ***********************************************************************/
{
tTapeBlock* pbTapeBlock;
tUChar* BlockData;
tULong Loop;

  // Reset total duration
  Tape->total_duration_ms = 0;

  for (Loop=0, pbTapeBlock = Tape->blocks;
       Loop < Tape->nb_blocks;
       Loop++, pbTapeBlock++)
  {
    tULong BlockDuration_ms = 0;

    BlockData = pbTapeBlock->pbImageData + 0x01; // Pass ID

    switch (pbTapeBlock->block_id)
    {
      case 0x10: // standard speed data block
      {
        BlockDuration_ms = GetBlock10Duration(Tape, BlockData);
      }
      break;

      case 0x11: // turbo loading data block
      {
        BlockDuration_ms = GetBlock11Duration(Tape, BlockData);
      }
      break;

      case 0x12: // pure tone
      {
        BlockDuration_ms = GetBlock12Duration(BlockData);
      }
      break;

      case 0x13: // sequence of pulses of different length
      {
        BlockDuration_ms = GetBlock13Duration(BlockData);
      }
      break;

      case 0x14: // pure data block
      {
        BlockDuration_ms = GetBlock14Duration(Tape, BlockData);
      }
      break;

      case 0x15: // direct recording
      {
        BlockDuration_ms = GetBlock15Duration(BlockData);
      }
      break;

      case 0x20: // pause
      {
        BlockDuration_ms = GetBlock20Duration(BlockData);
      }
      break;
    }

    // Store block duration and offset
    pbTapeBlock->time_offset_ms = Tape->total_duration_ms;
    pbTapeBlock->duration_ms = BlockDuration_ms;

    // Update total tape duration
    Tape->total_duration_ms += BlockDuration_ms;
  }

  // Update duration in seconds
  Tape->total_duration_s = Tape->total_duration_ms / 1000;
  if (Tape->total_duration_s > TAPE_MAX_DURATION)
  {
    Tape->total_duration_s = TAPE_MAX_DURATION;
  } 
}
/*----------------------------------------------------------------------------*/


static tULong ConvertSamplesToMs(tUShort TStatesPerSample, tULong NbSamples)
/***********************************************************************
 *
 *  ConvertSamplesToMs
 *
 ***********************************************************************/
{
tULong Duration_ms;
//tULong SampleDuration_us = 1000 * TStatesPerSample / 3500;
  //Duration_ms = NbSamples * SampleDuration_us / 1000;
  Duration_ms = NbSamples * TStatesPerSample / 3500;

  return Duration_ms;
}


static tULong GetOneBitsNumber(tTape* Tape,
                               tUChar* Data,
                               tULong Length)
/***********************************************************************
 *
 *  GetOneBitsNumber
 *
 ***********************************************************************/
{
tULong NbOneBits = 0;

  while (Length--)
  {
    NbOneBits += Tape->onebit_table[*(Data++)];
  }

  return NbOneBits;
}

static tULong GetBlock10Duration(tTape* Tape, tUChar* BlockData)
/***********************************************************************
 *
 *  GetBlock10Duration
 *
 ***********************************************************************/
{
tULong NbSamples;
tULong NbOneSamples;
tUShort PauseLength = *(tUShort *)(BlockData+0x00);
tUShort DataLength = *(tUShort *)(BlockData+0x02);
tULong BlockDuration_ms = 0;

  // Add Pilot duration
  BlockDuration_ms += ConvertSamplesToMs(2168, 3220);
  // Add Sync duration
  BlockDuration_ms += ConvertSamplesToMs(667 + 735, 1);

  NbSamples = DataLength * 8;
  NbOneSamples = GetOneBitsNumber(Tape, BlockData+0x4, DataLength);

  // Add One samples duration
  BlockDuration_ms += ConvertSamplesToMs(1710, NbOneSamples * 2); // 1 bit = 2 pulses
  // Add Zero samples duration
  BlockDuration_ms += ConvertSamplesToMs(855, (NbSamples - NbOneSamples) * 2); // 1 bit = 2 pulses

  // Add pause duration
  BlockDuration_ms += PauseLength;

  return BlockDuration_ms;
}


static tULong GetBlock11Duration(tTape* Tape, tUChar* BlockData)
/***********************************************************************
 *
 *  GetBlock11Duration
 *
 ***********************************************************************/
{
tULong DataLength;
tULong NbSamples;
tULong NbOneSamples;
tUShort LengthPilotPulse = *(tUShort *)(BlockData+0x00);
tUShort LengthPilotTone = *(tUShort *)(BlockData+0x0A);
tUShort LengthFirstSyncPulse = *(tUShort *)(BlockData+0x02);
tUShort LengthSecondSyncPulse = *(tUShort *)(BlockData+0x04);
tUShort PauseLength = *(tUShort *)(BlockData+0x0D);
tUShort NumberTStatesPerZero = *(tUShort *)(BlockData+0x06);
tUShort NumberTStatesPerOne = *(tUShort *)(BlockData+0x08);
tULong BlockDuration_ms = 0;

  // Add Pilot duration
  BlockDuration_ms += ConvertSamplesToMs(LengthPilotPulse, LengthPilotTone);
  // Add Sync duration
  BlockDuration_ms += ConvertSamplesToMs(LengthFirstSyncPulse + LengthSecondSyncPulse, 1);

  DataLength = *(tUShort *)(BlockData+0x0F) + (*(BlockData+0x11) << 16);
  NbSamples = ((DataLength-1) * 8) + *(BlockData+0x0C);
  NbOneSamples = GetOneBitsNumber(Tape, BlockData+0x12, DataLength);

  // Add One samples duration
  BlockDuration_ms += ConvertSamplesToMs(NumberTStatesPerOne, NbOneSamples * 2); // 1 bit = 2 pulses
  // Add Zero samples duration
  BlockDuration_ms += ConvertSamplesToMs(NumberTStatesPerZero, (NbSamples - NbOneSamples) * 2); // 1 bit = 2 pulses

  // Add pause duration
  BlockDuration_ms += PauseLength;

  return BlockDuration_ms;
}

static tULong GetBlock12Duration(tUChar* BlockData)
/***********************************************************************
 *
 *  GetBlock12Duration
 *
 ***********************************************************************/
{
tUShort NumberTStates = *(tUShort *)(BlockData+0x00);
tUShort NbPulses = *(tUShort *)(BlockData+0x02);

  return (ConvertSamplesToMs(NumberTStates, NbPulses));
}

static tULong GetBlock13Duration(tUChar* BlockData)
/***********************************************************************
 *
 *  GetBlock13Duration
 *
 ***********************************************************************/
{
tULong NbTStates = 0;
tUShort* PulseTStates = ((tUShort*)BlockData+0x01);
tUChar NbPulses = *(BlockData+0x00);

  while (NbPulses--)
  {
    NbTStates += *(PulseTStates++);
  }

  return (ConvertSamplesToMs(NbTStates, 1));
}

static tULong GetBlock14Duration(tTape* Tape, tUChar* BlockData)
/***********************************************************************
 *
 *  GetBlock14Duration
 *
 ***********************************************************************/
{
tULong DataLength;
tULong NbSamples;
tULong NbOneSamples;
tUShort PauseLength = *(tUShort *)(BlockData+0x05);
tUShort NumberTStatesPerZero = *(tUShort *)(BlockData+0x00);
tUShort NumberTStatesPerOne = *(tUShort *)(BlockData+0x02);
tULong BlockDuration_ms = 0;

  DataLength = *(tUShort *)(BlockData+0x07) + (*(BlockData+0x09) << 16);
  NbSamples = ((DataLength-1) * 8) + *(BlockData+0x04);
  NbOneSamples = GetOneBitsNumber(Tape, BlockData+0x0A, DataLength);

  // Add One samples duration
  BlockDuration_ms += ConvertSamplesToMs(NumberTStatesPerOne, NbOneSamples * 2); // 1 bit = 2 pulses
  // Add Zero samples duration
  BlockDuration_ms += ConvertSamplesToMs(NumberTStatesPerZero, (NbSamples - NbOneSamples) * 2); // 1 bit = 2 pulses

  // Add pause duration
  BlockDuration_ms += PauseLength;

  return BlockDuration_ms;
}

static tULong GetBlock15Duration(tUChar* BlockData)
/***********************************************************************
 *
 *  GetBlock15Duration
 *
 ***********************************************************************/
{
tULong NbSamples;
tUShort PauseLength = *(tUShort *)(BlockData+0x02);
tUShort NumberTStatesPerSample = *(tUShort *)(BlockData+0x00);
tULong BlockDuration_ms = 0;

  NbSamples = *(tUShort *)(BlockData+0x05) - 1; // Remove last byte
  NbSamples += *(BlockData+0x07) << 16;
  NbSamples *= 8; // 1 sample = 1 bit
  NbSamples += *(BlockData+0x04);

  // Add samples duration
  BlockDuration_ms += ConvertSamplesToMs(NumberTStatesPerSample, NbSamples);
  // Add pause duration
  BlockDuration_ms += PauseLength;

  return BlockDuration_ms;
}

static tULong GetBlock20Duration(tUChar* BlockData)
/***********************************************************************
 *
 *  GetBlock20Duration
 *
 ***********************************************************************/
{
tUShort PauseLength = *(tUShort *)(BlockData+0x00);

  return PauseLength;
}


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
