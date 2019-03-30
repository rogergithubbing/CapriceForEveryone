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

#if defined(__PALMOS__)
  #include <PceNativeCall.h>
  #include <ByteOrderUtils.h>
#endif

#include "..\Native_CPC.h"


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
                    
#ifdef __PALMOS__
//
// Palm OS routines
//
static tVoid* MemPtrNewLarge(tULong size,
                             tULong routineP,
                             const tVoid* emulStateP,
                             Call68KFuncType* call68KFuncP);
#endif /* __PALMOS__ */

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
tULong Engine_DiskFormat(tDiskOperation* DiskOperation)
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
tMemPtrNewFct NewMemFct = (tMemPtrNewFct)DiskOperation->NativeCPC->MemPtrNewPtr;
#endif /* __WIN32__ */
#if defined(_TESTU) || defined(__PALMOS__)
tNativeCPC* NativeCPC;
#endif /* _TESTU || __PALMOS__ */
const tDiskFormat* disk_format;
tDrive* Drive;
tUChar* pbDiskPtr;
tUChar* pbDataPtr;
tUChar* pbTempPtr;
tULong track;
tULong side;
tULong sector;
tULong dwTrackSize;
tULong dwTrackHeaderSize;
tULong dwSectorSize;
tULong dwSectors;
tULong dwDiskSize;
tULong data_start;
tUChar CHRN[4];

#if defined(__PALMOS__)

  // Palm OS 68K interface
  Drive = (tDrive*)EndianSwap32(DiskOperation->Drive);
  NativeCPC = (tNativeCPC*)EndianSwap32(DiskOperation->NativeCPC);
  disk_format = (tDiskFormat*)EndianSwap32(DiskOperation->DiskFormatTableP);
  
#elif defined(__WIN32__)

  Drive = DiskOperation->Drive;
#ifdef _TESTU
  NativeCPC = DiskOperation->NativeCPC;
#endif /* _TESTU */
  disk_format = DiskOperation->DiskFormatTableP;
  
#endif /* __PALMOS__ || __WIN32__ */


  Drive->tracks = disk_format[DiskOperation->FormatType].tracks;
  if (Drive->tracks > DSK_TRACKMAX) // compare against upper limit
  {
    Drive->tracks = DSK_TRACKMAX; // limit to maximum
  }

  Drive->sides = disk_format[DiskOperation->FormatType].sides;
  if (Drive->sides > DSK_SIDEMAX) // abort if more than maximum
  {
    return vfsErrBadData;
  }

  dwSectorSize = 128 << disk_format[DiskOperation->FormatType].sector_size; // determine sector size in bytes
  dwSectors = disk_format[DiskOperation->FormatType].sectors;
  if (dwSectors > EDSK_SECTORMAX)
  {
    dwSectors =  EDSK_SECTORMAX;
  }

  dwTrackSize = dwSectorSize * dwSectors; // determine track size in bytes, minus track header
  
  //
  // Allocate memory for entire disk
  //
  dwTrackHeaderSize = DSK_TRACKHEADER_SIZE;
  if (dwSectors > DSK_SECTORMAX)
  {
    sector = dwSectors - DSK_SECTORMAX;
    sector = ((sector / DSK_NBSECTORPERHEADER) + 1) * DSK_NBSECTORPERHEADER;
    dwTrackHeaderSize += sector * DSK_SECTORID_SIZE;
  }
  dwDiskSize = dwTrackHeaderSize /* Track Header */ + dwTrackSize /* Track Data */;
  dwDiskSize *= Drive->tracks; // Multiply by the number of tracks
  dwDiskSize *= Drive->sides; // Multiply by the number of sides
  dwDiskSize += DSK_HEADER_SIZE; // Add disk header
  
#if defined(__PALMOS__)
  Drive->dataP = (tUChar*)MemPtrNewLarge(dwDiskSize,
                                         NativeCPC->MemPtrNewPtr,
                                         emulStateP,
                                         call68KFuncP);
#elif defined(__WIN32__)
  Drive->dataP = (tUChar*)NewMemFct(dwDiskSize);
#endif /* __PALMOS__ */

  if (Drive->dataP == cNull)
  {
    return memErrNotEnoughSpace;
  }
  Drive->data_size = dwDiskSize;
  
  // Begin of first track
  pbDiskPtr = Drive->dataP + DSK_HEADER_SIZE;

  // zero base number of sides
  Drive->sides--;

  for (track=0; track < Drive->tracks; track++) // loop for all tracks
  {
    for (side=0; side <= Drive->sides; side++) // loop for all sides
    {
      Drive->track[track][side].sectors = dwSectors; // store sector count
      Drive->track[track][side].size = dwTrackSize; // store track size
			Drive->track[track][side].data = pbDiskPtr + dwTrackHeaderSize; // Begin of first sector

			Drive->track[track][side].gap3_length = disk_format[DiskOperation->FormatType].gap3_length;
			Drive->track[track][side].filler = disk_format[DiskOperation->FormatType].filler_byte;
      
      pbDataPtr = Drive->track[track][side].data; // pointer to start of memory buffer
      pbTempPtr = pbDataPtr; // keep a pointer to the beginning of the buffer for the current track
      CHRN[0] = (tUChar)track;
      CHRN[1] = (tUChar)side;
      CHRN[3] = (tUChar)disk_format[DiskOperation->FormatType].sector_size;
      for (sector = 0; sector < dwSectors; sector++) // loop for all sectors
      {
        CHRN[2] = disk_format[DiskOperation->FormatType].sector_ids[side][sector];

        MemMove(Drive->track[track][side].sector[sector].CHRN,
                CHRN,
                4); // copy CHRN

        Drive->track[track][side].sector[sector].size = dwSectorSize;
        Drive->track[track][side].sector[sector].data = pbDataPtr; // store pointer to sector data

        pbDataPtr += dwSectorSize;
      }

      MemSet(pbTempPtr,
             dwTrackSize,
						 disk_format[DiskOperation->FormatType].filler_byte);

      // Begin of next track
      pbDiskPtr = Drive->track[track][side].data + dwTrackSize;
    }
  }

  Drive->altered = 1; // flag disk as having been modified

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


#ifdef __PALMOS__
//==============================================================================
//
// Palm OS routines
//
//==============================================================================
static tVoid* MemPtrNewLarge(tULong size,
                             tULong routineP,
                             const tVoid* emulStateP,
                             Call68KFuncType* call68KFuncP)
/***********************************************************************
 *
 *  MemPtrNewLarge
 *
 ***********************************************************************/
{
tUChar args[4];

  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(size);

  return (tVoid*)call68KFuncP(emulStateP,
                              routineP,
                              &args,
                              (sizeof(args)/sizeof(args[0])) | kPceNativeWantA0);
}
/*----------------------------------------------------------------------------*/
#endif /* __PALMOS__ */


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
