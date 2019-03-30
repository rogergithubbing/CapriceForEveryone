/*
    Caprice32 - Amstrad CPC Emulator
    (c) Copyright 1997-2005 Ulrich Doewich

    CaPriCe for Palm OS - Amstrad CPC 464/664/6128 emulator for Palm devices
    Copyright (C) 2006-2011 by Frédéric Coste
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

#ifdef __PALMOS__
#include <PceNativeCall.h>
#include <ByteOrderUtils.h>
#endif /* __PALMOS__ */

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


#ifdef __PALMOS__
tULong PNOMain(const tVoid*,
               tVoid*,
               Call68KFuncType*);
#endif /* __PALMOS__ */


#if defined(__GNUC__)
// This seemingly bogus function satisfies the Windows LoadLibrary() call,
// (and must be the first function in our PNO .dll).
int /*WINAPI*/ my_bogus_cygwin_dll_entry(unsigned long /*HANDLE*/ h,
                                         unsigned long /*DWORD*/ reason,
                                         void* p)
{
  return 1;
}
#endif /* __GNUC__ */


//
// Routines
//
static tVoid MemMove(tUChar* destP,
                     tUChar* sourceP,
                     tULong numBytes);
static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value);
static tVoid StrCopy(tChar* destP,
                     tChar* srcP);
//
// TestU
//
#ifdef _TESTU
static tUShort PerformTestU(tNativeCPC* NativeCPC);
#endif /* _TESTU */


/***********************************************************************
 *
 *  Entry Points
 *
 ***********************************************************************/
#if defined(__PALMOS__)
tULong PNOMain(const tVoid* emulStateP,
               tVoid* userData68KP,
               Call68KFuncType* call68KFuncP)
#elif defined(__WIN32__)
tULong Engine_DiskSave(tDiskOperation* DiskOperation)
#endif /* __PALMOS__ || __WIN32__ */
/***********************************************************************
 *
 *  PNOMain
 *
 ***********************************************************************/
{
#ifdef __PALMOS__
tDiskOperation* DiskOperation = (tDiskOperation*)userData68KP;
#endif /* __PALMOS__ */
#ifdef __WIN32__
tMemPtrDeleteFct DeleteMemFct = (tMemPtrDeleteFct)DiskOperation->NativeCPC->MemPtrDeletePtr;
#endif /* __WIN32__ */
#if defined(_TESTU)
tNativeCPC* NativeCPC;
#endif /* _TESTU */
tDrive* Drive;
tDSKHeader* dhP;
tTrackHeader* thP;
tUChar* dataP;
tUChar* olddataP;
tULong dwTrackHeaderSize;
tUShort pos;
tUChar track;
tUChar side;
tUChar sector;

#ifdef __PALMOS__
  NOT_USED(emulStateP);
  NOT_USED(call68KFuncP);
#endif /* __PALMOS__ */

#if defined(__PALMOS__)

  // Palm OS 68K interface
  Drive = (tDrive*)EndianSwap32(DiskOperation->Drive);
#if defined(_TESTU)
  NativeCPC = (tNativeCPC*)EndianSwap32(DiskOperation->NativeCPC);
#endif /* _TESTU */

#elif defined(__WIN32__)

  Drive = DiskOperation->Drive;
#if defined(_TRACE) || defined(_TESTU)
  NativeCPC = DiskOperation->NativeCPC;
#endif /* _TRACE || _TESTU */

#endif /* __PALMOS__ || __WIN32__ */

  if (Drive->dataP == cNull)
    return memErrInvalidParam;
  
  // Formatted tracks to insert
  if (Drive->new_dataP != cNull)
  {
    // Keep old drive content
    olddataP = Drive->dataP;
    
    // Replace new data content
    Drive->dataP = Drive->new_dataP;
    Drive->data_size = Drive->new_data_size;
  }
  else
  {
    olddataP = cNull;
  }

  // Memory allocation
  dhP = (tDSKHeader*)Drive->dataP;
  
  MemSet((tUChar*)dhP,
         DSK_HEADER_SIZE,
         0);
  StrCopy(dhP->id, 
          "EXTENDED CPC DSK File\r\nDisk-Info\r\n");
  StrCopy(dhP->creator, 
          "CaPriCe 4Ever");
  dhP->tracks = Drive->tracks;
  dhP->sides = (Drive->sides+1) | (Drive->random_DEs); // correct side count and indicate random DEs, if necessary

  //
  // Fill Disk header track size data
  //
  pos=0;
  // loop for all tracks
  for (track=0; track < (tUChar)Drive->tracks; track++)
  {
    // loop for all sides
    for (side=0; side <= (tUChar)Drive->sides; side++)
    {
      // track is formatted?
      if (Drive->track[track][side].size)
      {
        dwTrackHeaderSize = DSK_TRACKHEADER_SIZE;
        if (Drive->track[track][side].sectors > DSK_SECTORMAX)
        {
          sector = Drive->track[track][side].sectors - DSK_SECTORMAX;
          sector = ((sector / DSK_NBSECTORPERHEADER) + 1) * DSK_NBSECTORPERHEADER;
          dwTrackHeaderSize += sector * DSK_SECTORID_SIZE;
        }

        dhP->track_size[pos] = (Drive->track[track][side].size + dwTrackHeaderSize) >> 8; // track size + header in bytes
      }
      else
      {
        dhP->track_size[pos] = 0; // Not formatted
      }
      pos++;
    }
  }

  // Begin of first track
  dataP = Drive->dataP + DSK_HEADER_SIZE;

  for (track=0; track < (tUChar)Drive->tracks; track++) // loop for all tracks
  {
    for (side=0; side <= (tUChar)Drive->sides; side++) // loop for all sides
    {
      // track is formatted?
      if (Drive->track[track][side].size)
      {
        thP = (tTrackHeader*)dataP;

        MemSet((tUChar*)thP,
               DSK_TRACKHEADER_SIZE,
               0);
        StrCopy(thP->id,
                "Track-Info\r\n");
      
        thP->track = track;
        thP->side = side;
        thP->bps = 2;
        thP->sectors = Drive->track[track][side].sectors;
        thP->gap3_length = Drive->track[track][side].gap3_length;
        thP->filler = Drive->track[track][side].filler;
        
        dwTrackHeaderSize = DSK_TRACKHEADER_SIZE;
        if (thP->sectors > DSK_SECTORMAX)
        {
          sector = thP->sectors - DSK_SECTORMAX;
          sector = ((sector / DSK_NBSECTORPERHEADER) + 1) * DSK_NBSECTORPERHEADER;
          dwTrackHeaderSize += sector * DSK_SECTORID_SIZE;
        }

        // Prepare track header
        for (sector=0; sector < thP->sectors; sector++)
        {
          MemMove(&thP->sector[sector][0],
                  Drive->track[track][side].sector[sector].CHRN,
                  4); // copy CHRN
          MemMove(&thP->sector[sector][4],
                  Drive->track[track][side].sector[sector].flags,
                  2); // copy ST1 & ST2

          thP->sector[sector][6] = Drive->track[track][side].sector[sector].size & 0xff;
          thP->sector[sector][7] = (Drive->track[track][side].sector[sector].size >> 8) & 0xff; // sector size in bytes
        }

        // Copy track content
        if (olddataP)
        {
          MemMove(dataP+dwTrackHeaderSize,
                  Drive->track[track][side].data,
                  Drive->track[track][side].size);
        }

        dataP += Drive->track[track][side].size + dwTrackHeaderSize; // Next track
      }
    }
  }

  // "Offset-Info" section available ?
  if (Drive->offset_info_available == cTrue)
  {
    // Write section header
    MemSet(dataP, EDSK_OFFSETINFOHEADERSIZE, 0);
    StrCopy(dataP, "Offset-Info\r\n");
    dataP += EDSK_OFFSETINFOHEADERSIZE;

    for (track = 0; track < (tUChar)Drive->tracks; track++) // loop for all tracks
    {
      for (side = 0; side <= (tUChar)Drive->sides; side++) // loop for all sides
      {
        tTrack* pTrack = &Drive->track[track][side];

        // Track size
        *dataP++ = pTrack->offset_info_size & 0xff;
        *dataP++ = (pTrack->offset_info_size >> 8) & 0xff;

        for (sector = 0; sector < (tUChar)pTrack->sectors; sector++) // loop for all sectors
        {
          tSector *pSector = &pTrack->sector[sector];

          // Sector offset
          *dataP++ = pSector->offset_info & 0xff;
          *dataP++ = (pSector->offset_info >> 8) & 0xff;
        }
      }
    }
  }
  
  // Free old drive content
  if (olddataP)
  {
#if defined(__WIN32__)
    DeleteMemFct(olddataP);
#endif /* __WIN32__ */
    Drive->new_dataP = cNull;
  }
  
#ifdef _TESTU
  return PerformTestU(NativeCPC);
#else /* _TESTU */
  return errNone;
#endif /* _TESTU */
}
/*----------------------------------------------------------------------------*/



//==============================================================================
//
// Routines
//
//==============================================================================

static tVoid MemMove(tUChar* destP,
                     tUChar* sourceP,
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


static tVoid StrCopy(tChar* destP,
                     tChar* srcP)
/***********************************************************************
 *
 *  MemSet
 *
 ***********************************************************************/
{
  while (*srcP != 0)
  {
    *(destP++) = *(srcP++);
  }
}
/*----------------------------------------------------------------------------*/


//==============================================================================
//
// Unitary Tests
//
//==============================================================================
#ifdef _TESTU
// Prototypes of TestU fonctions
static tUShort TestU_MemSet_1(tNativeCPC* NativeCPC,
                              tUChar NoTest);
static tUShort TestU_MemMove_1(tNativeCPC* NativeCPC,
                               tUChar NoTest);


static tUShort PerformTestU(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PerformTestU
 *
 ***********************************************************************/
{
tUShort Result = errNone;
tUChar NoTest = 1;

  /* 1 */ if (Result == errNone) Result = TestU_MemSet_1(NativeCPC,
                                                         NoTest++);
  /* 2 */ if (Result == errNone) Result = TestU_MemMove_1(NativeCPC,
                                                          NoTest++);
  
  return (Result);
}
/*----------------------------------------------------------------------------*/


static tUShort TestU_MemSet_1(tNativeCPC* NativeCPC,
                              tUChar NoTest)
/***********************************************************************
 *
 *  TestU_MemSet_1
 *
 ***********************************************************************/
{
tUChar ArrayA[100];
tUChar Loop;
tUChar Result = errNone;

	NOT_USED(NativeCPC);

  // Prepare conditions
  for (Loop=0; Loop<100; Loop++)
    ArrayA[Loop] = 0x55;
  // Perform operation
  MemSet(ArrayA, 95, 0xAA);
  // Check Result
  for (Loop=0; Loop<95; Loop++)
  {
    if (ArrayA[Loop] != 0xAA)
      Result=testUErrorClass+NoTest;
  }
  for (Loop=95; Loop<100; Loop++)
  {
    if (ArrayA[Loop] != 0x55)
      Result=testUErrorClass+NoTest;
  }
  
  return (Result);
}
/*----------------------------------------------------------------------------*/

static tUShort TestU_MemMove_1(tNativeCPC* NativeCPC,
                               tUChar NoTest)
/***********************************************************************
 *
 *  TestU_MemMove_1
 *
 ***********************************************************************/
{
tUChar SrcArrayA[100];
tUChar DstArrayA[100];
tUChar Loop;
tUChar Result = errNone;

	NOT_USED(NativeCPC);

  // Prepare conditions
  for (Loop=0; Loop<100; Loop++)
  {
    SrcArrayA[Loop] = 0x55;
    DstArrayA[Loop] = 0xAA;
  }
  // Perform operation
  MemMove(DstArrayA, SrcArrayA, 95);
  // Check Result
  for (Loop=0; Loop<100; Loop++)
  {
    if (SrcArrayA[Loop] != 0x55)
      Result=testUErrorClass+NoTest;
  }
  for (Loop=0; Loop<95; Loop++)
  {
    if (DstArrayA[Loop] != 0x55)
      Result=testUErrorClass+NoTest;
  }
  for (Loop=95; Loop<100; Loop++)
  {
    if (DstArrayA[Loop] != 0xAA)
      Result=testUErrorClass+NoTest;
  }
  
  return (Result);
}
/*----------------------------------------------------------------------------*/

#endif /* _TESTU */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif



