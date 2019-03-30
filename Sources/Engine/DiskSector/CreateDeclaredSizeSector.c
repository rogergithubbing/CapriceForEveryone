/*
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
#include "..\..\Common\FDC.h"
#include "..\..\Common\MFM.h"


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
static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value);
static tVoid MemMove(tUChar* destP,
                     const tUChar* sourceP,
                     tULong numBytes);

                                   
// Table to convert one MFM pulse shift
static tUChar const MFMPulseShiftTable[] = 
{
  0xFF,0xFE,0xFC,0xFC,0xF9,0xF8,0xF8,0xF8,0xF3,0xF2,0xF0,0xF0,0xF1,0xF0,0xF0,0xF0,
  0xE7,0xE6,0xE4,0xE4,0xE1,0xE0,0xE0,0xE0,0xE3,0xE2,0xE0,0xE0,0xE1,0xE0,0xE0,0xE0,
  0xCF,0xCE,0xCC,0xCC,0xC9,0xC8,0xC8,0xC8,0xC3,0xC2,0xC0,0xC0,0xC1,0xC0,0xC0,0xC0,
  0xC7,0xC6,0xC4,0xC4,0xC1,0xC0,0xC0,0xC0,0xC3,0xC2,0xC0,0xC0,0xC1,0xC0,0xC0,0xC0,
  0x9F,0x9E,0x9C,0x9C,0x99,0x98,0x98,0x98,0x93,0x92,0x90,0x90,0x91,0x90,0x90,0x90,
  0x87,0x86,0x84,0x84,0x81,0x80,0x80,0x80,0x83,0x82,0x80,0x80,0x81,0x80,0x80,0x80,
  0x8F,0x8E,0x8C,0x8C,0x89,0x88,0x88,0x88,0x83,0x82,0x80,0x80,0x81,0x80,0x80,0x80,
  0x87,0x86,0x84,0x84,0x81,0x80,0x80,0x80,0x83,0x82,0x80,0x80,0x81,0x80,0x80,0x80,
  0x3F,0x3E,0x3C,0x3C,0x39,0x38,0x38,0x38,0x33,0x32,0x30,0x30,0x31,0x30,0x30,0x30,
  0x27,0x26,0x24,0x24,0x21,0x20,0x20,0x20,0x23,0x22,0x20,0x20,0x21,0x20,0x20,0x20,
  0x0F,0x0E,0x0C,0x0C,0x09,0x08,0x08,0x08,0x03,0x02,0x00,0x00,0x01,0x00,0x00,0x00,
  0x07,0x06,0x04,0x04,0x01,0x00,0x00,0x00,0x03,0x02,0x00,0x00,0x01,0x00,0x00,0x00,
  0x1F,0x1E,0x1C,0x1C,0x19,0x18,0x18,0x18,0x13,0x12,0x10,0x10,0x11,0x10,0x10,0x10,
  0x07,0x06,0x04,0x04,0x01,0x00,0x00,0x00,0x03,0x02,0x00,0x00,0x01,0x00,0x00,0x00,
  0x0F,0x0E,0x0C,0x0C,0x09,0x08,0x08,0x08,0x03,0x02,0x00,0x00,0x01,0x00,0x00,0x00,
  0x07,0x06,0x04,0x04,0x01,0x00,0x00,0x00,0x03,0x02,0x00,0x00,0x01,0x00,0x00,0x00,
};


tVoid CreateDeclaredSizeSector(tTrack* trackP, tULong CurrentSector)
/***********************************************************************
 *
 *  CreateDeclaredSizeSector
 *
 ***********************************************************************/
{
tSector* CurrentSectorP = &trackP->sector[CurrentSector];
tUChar* DeclaredDataP = CurrentSectorP->declared_data;
tULong RemainingSize = CurrentSectorP->declared_size;
tULong AfterIndexLength;
tULong AfterIndexSynchroRemaining;
tULong AddedBytesLength = 0;
tULong sector;
tULong size;
tUChar post_index_byte;
tUChar track_synchro_bitshift;
tUChar track_synchro_pulseshift;
tBool first_sector = cTrue;

  // Memory allocated ?
  if (DeclaredDataP == cNull)
    return;

  if (RemainingSize > (FDC_MAX_BYTES_PER_TRACK - CurrentSectorP->track_offset))
  {
    AfterIndexLength = RemainingSize - (FDC_MAX_BYTES_PER_TRACK - CurrentSectorP->track_offset);
  }
  else
  {
    AfterIndexLength = 0;
  }

  do
  {
    // Add sectors until end of track
    do
    {
      // First sector ?
      if (first_sector == cFalse)
      {
        // Add SYNC
        size = (FDC_BYTES_SYNC > RemainingSize) ? RemainingSize : FDC_BYTES_SYNC;
        MemSet(DeclaredDataP, size, FDC_BYTE_SYNC);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;

        // Add IDAM
        size = (FDC_BYTES_IDAM > RemainingSize) ? RemainingSize : FDC_BYTES_IDAM;
        MemMove(DeclaredDataP, fdc_idam, size);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;

        // Add CHRN
        size = ((FDC_BYTES_CHRN+FDC_BYTES_CRC) > RemainingSize) ? RemainingSize : (FDC_BYTES_CHRN+FDC_BYTES_CRC);
        MemMove(DeclaredDataP, CurrentSectorP->CHRN, size);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;

        // Add GAP2
        size = (FDC_BYTES_GAP2 > RemainingSize) ? RemainingSize : FDC_BYTES_GAP2;
        MemSet(DeclaredDataP, size, FDC_BYTE_GAP2);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;

        // Add SYNC
        size = (FDC_BYTES_SYNC > RemainingSize) ? RemainingSize : FDC_BYTES_SYNC;
        MemSet(DeclaredDataP, size, FDC_BYTE_SYNC);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;

        // Add DATAAM
        size = (FDC_BYTES_DATAAM > RemainingSize) ? RemainingSize : FDC_BYTES_DATAAM;
        if (CurrentSectorP->flags[1] & ST2_CM)
        {
          MemMove(DeclaredDataP, fdc_dataam_deleted, size);
        }
        else
        {
          MemMove(DeclaredDataP, fdc_dataam, size);
        }
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;
      }

      // Add data
      size = (CurrentSectorP->data_size > RemainingSize) ? RemainingSize : CurrentSectorP->data_size;
      MemMove(DeclaredDataP, CurrentSectorP->data, size);
      DeclaredDataP += size;
      if ((RemainingSize -= size) == 0)
        break;

      // Sector GAP3 ?
      if (CurrentSectorP->data_gap3_length)
      {
        // Add Sector GAP3
        size = (CurrentSectorP->data_gap3_length > RemainingSize) ? RemainingSize : CurrentSectorP->data_gap3_length;
        MemMove(DeclaredDataP,
                CurrentSectorP->data + (CurrentSectorP->data_size * CurrentSectorP->weak_count),
                size);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;
      }
      else
      {
        // Add Data CRC
        size = (FDC_BYTES_CRC > RemainingSize) ? RemainingSize : FDC_BYTES_CRC;
        MemMove(DeclaredDataP, CurrentSectorP->data_crc, size);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;

        // Add GAP3
        size = (CurrentSectorP->info_gap3_length > RemainingSize) ? RemainingSize : CurrentSectorP->info_gap3_length;
        MemSet(DeclaredDataP, size, FDC_BYTE_GAP3);
        DeclaredDataP += size;
        if ((RemainingSize -= size) == 0)
          break;
      }

      // Next sector
      first_sector = cFalse;
      CurrentSectorP++;
      if (CurrentSectorP->data == cNull)
        break;
    }
    while ((CurrentSectorP->data != cNull) && (RemainingSize > 0));
    if (RemainingSize == 0)
      break;

    // Add GAP4b
    if (RemainingSize > AfterIndexLength)
    {
      AfterIndexSynchroRemaining = 0;
      size = RemainingSize - AfterIndexLength;
      MemSet(DeclaredDataP, size, FDC_BYTE_GAP4B);
      DeclaredDataP += size;
      if ((RemainingSize -= size) == 0)
        break;
    }
    else
    {
      AfterIndexSynchroRemaining = AfterIndexLength - RemainingSize;
    }

    // First sector
    CurrentSectorP = &trackP->sector[0];

    // Add GAP4a
    size = (FDC_BYTES_GAP4A > RemainingSize) ? RemainingSize : FDC_BYTES_GAP4A;
    if (AfterIndexSynchroRemaining)
    {
      if (AfterIndexSynchroRemaining > size)
      {
        AfterIndexSynchroRemaining -= size;
      }
      else
      {
        // Part copy
        tULong partsize = size - AfterIndexSynchroRemaining;
        AddedBytesLength = RemainingSize;
        AfterIndexSynchroRemaining = 0;
        MemSet(DeclaredDataP + AfterIndexSynchroRemaining, partsize, FDC_BYTE_GAP4A);
        DeclaredDataP += partsize;
        if ((RemainingSize -= partsize) == 0)
          break;
      }
    }
    else
    {
      AfterIndexLength = RemainingSize;
      MemSet(DeclaredDataP, size, FDC_BYTE_GAP4A);
      DeclaredDataP += size;
      if ((RemainingSize -= size) == 0)
        break;
    }

    // Add SYNC
    size = (FDC_BYTES_SYNC > RemainingSize) ? RemainingSize : FDC_BYTES_SYNC;
    if (AfterIndexSynchroRemaining)
    {
      if (AfterIndexSynchroRemaining > size)
      {
        AfterIndexSynchroRemaining -= size;
      }
      else
      {
        // Part copy
        tULong partsize = size - AfterIndexSynchroRemaining;
        AddedBytesLength = RemainingSize;
        AfterIndexSynchroRemaining = 0;
        MemSet(DeclaredDataP + AfterIndexSynchroRemaining, partsize, FDC_BYTE_SYNC);
        DeclaredDataP += partsize;
        if ((RemainingSize -= partsize) == 0)
          break;
      }
    }
    else
    {
      MemSet(DeclaredDataP, size, FDC_BYTE_SYNC);
      DeclaredDataP += size;
      if ((RemainingSize -= size) == 0)
        break;
    }

    // Add IAM
    size = (FDC_BYTES_IAM > RemainingSize) ? RemainingSize : FDC_BYTES_IAM;
    if (AfterIndexSynchroRemaining)
    {
      if (AfterIndexSynchroRemaining > size)
      {
        AfterIndexSynchroRemaining -= size;
      }
      else
      {
        // Part copy
        tULong partsize = size - AfterIndexSynchroRemaining;
        AddedBytesLength = RemainingSize;
        AfterIndexSynchroRemaining = 0;
        MemMove(DeclaredDataP + AfterIndexSynchroRemaining, fdc_iam, partsize);
        DeclaredDataP += partsize;
        if ((RemainingSize -= partsize) == 0)
          break;
      }
    }
    else
    {
      MemMove(DeclaredDataP, fdc_iam, size);
      DeclaredDataP += size;
      if ((RemainingSize -= size) == 0)
        break;
    }

    // Add GAP1
    size = (FDC_BYTES_GAP1 > RemainingSize) ? RemainingSize : FDC_BYTES_GAP1;
    if (AfterIndexSynchroRemaining)
    {
      AddedBytesLength = RemainingSize;
    }
    MemSet(DeclaredDataP, size, FDC_BYTE_GAP1);
    DeclaredDataP += size;
    if ((RemainingSize -= size) == 0)
      break;
  }
  while (RemainingSize > 0);


  //
  // Post-index area synchronization
  //
  if (AfterIndexLength)
  {
    CurrentSectorP = &trackP->sector[CurrentSector];
    
    // Read post index GAP4a byte
    post_index_byte = CurrentSectorP->declared_data[CurrentSectorP->declared_size - AfterIndexLength];

    // Detect bit shift and pulse shift 
    DetectMFMShift(post_index_byte, FDC_BYTE_GAP4A, &track_synchro_bitshift, &track_synchro_pulseshift);

    // Shift post-index area
    MFMShift(CurrentSectorP->declared_data + CurrentSectorP->declared_size - AddedBytesLength,
             AddedBytesLength,
             track_synchro_bitshift,
             track_synchro_pulseshift);
  }
}
/*----------------------------------------------------------------------------*/



//==============================================================================
//
// Routines
//
//==============================================================================

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


static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value)
/***********************************************************************
 *
 *  MemSet
 *
 ***********************************************************************/
{
  while (numBytes--)
  {
    *(destP++) = value;
  }
}
/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

