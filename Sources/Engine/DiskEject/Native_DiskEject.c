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


#ifdef __PALMOS__

// copied from CoreTraps.h
#define sysTrapMemChunkFree           0xA012

#endif /* __PALMOS__ */



//
// Routines
//
static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value);

#ifdef __PALMOS__                    
//
// Palm OS routines
//
static tUShort MemPtrFree(tVoid* chunkDataP,
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
tULong Engine_DiskEject(tDiskOperation* DiskOperation)
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
tSector* pSector;
tULong track;
tULong side;
tULong sector;
//tUChar CurrentTrack;

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
#if defined(_TESTU)
  NativeCPC = DiskOperation->NativeCPC;
#endif /* _TESTU */
#endif /* __PALMOS__ || __WIN32__ */

  // Free all new formatted tracks
  for (track=0; track < Drive->tracks; track++) // loop for all tracks
  {
    for (side=0; side <= Drive->sides; side++) // loop for all sides
    {
      // Free new formatted track data
      if (Drive->track[track][side].new_data != cNull)
      {
        DeleteMemFct(Drive->track[track][side].new_data);
      }

      // Free declared sized sector data
      for (sector=0; sector < Drive->track[track][side].sectors; sector++)
      {
        pSector = &Drive->track[track][side].sector[sector];
        DeleteMemFct(pSector->declared_data);
      }
    }
  }

  if (Drive->dataP != cNull)
  {
#if defined(__PALMOS__)
    // Free disk data memory
    MemPtrFree(Drive->dataP,
               emulStateP,
               call68KFuncP);
#elif defined(__WIN32__)
    // Free disk data memory
    DeleteMemFct(Drive->dataP);
#else
    // Memory will be freed by application
    DiskOperation->DiskContentP = Drive->dataP;
#endif /* __PALMOS__ */
               
    Drive->dataP = cNull;
  }

  // Pb with some catalogue after disk eject
  //CurrentTrack = Drive->current_track; // save the drive head position
  MemSet((tUChar*)Drive,
         sizeof(tDrive),
         0); // clear drive info structure
  //Drive->current_track = CurrentTrack;

  // Set marker
  Drive->magic = DRIVE_MAGIC;

#ifdef _TESTU
  return PerformTestU(NativeCPC);
#else /* _TESTU */
  return errNone;
#endif /* _TESTU */
}



//==============================================================================
//
// Routines
//
//==============================================================================

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

static tUShort MemPtrFree(tVoid* chunkDataP,
                          const tVoid* emulStateP,
                          Call68KFuncType* call68KFuncP)
{
tUChar args[4];

  // Err MemChunkFree(MemPtr chunkDataP);
  // MemChunkFree Parameters
  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(chunkDataP);

  return (tUShort)call68KFuncP(emulStateP,
                               PceNativeTrapNo(sysTrapMemChunkFree),
                               &args,
                               (sizeof(args)/sizeof(args[0])));
}
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


static tUShort PerformTestU(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PerformTestU
 *
 ***********************************************************************/
{
tUShort Result = errNone;
tUChar NoTest = 1;

  if (Result == errNone) Result = TestU_MemSet_1(NativeCPC,
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

#endif /* _TESTU */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
