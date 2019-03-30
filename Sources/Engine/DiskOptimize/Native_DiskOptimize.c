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


/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


//
// Routines
//
static tVoid MemMove(tUChar* destP,
                     tUChar* sourceP,
                     tULong numBytes);


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
tULong Engine_DiskOptimize(tDiskOperation* DiskOperation)
/***********************************************************************
 *
 *  Engine_DiskOptimize
 *
 ***********************************************************************/
{
tMemPtrNewFct NewMemFct = (tMemPtrNewFct)DiskOperation->NativeCPC->MemPtrNewPtr;
tMemPtrDeleteFct DeleteMemFct = (tMemPtrDeleteFct)DiskOperation->NativeCPC->MemPtrDeletePtr;
tDrive* Drive = DiskOperation->Drive;
tULong dwRealTrackSize;
tUChar track;
tUChar side;
tUChar sector;

  // loop for all tracks
  for (track=0; track < (tUChar)Drive->tracks; track++)
  {
    // loop for all sides
    for (side=0; side <= (tUChar)Drive->sides; side++)
    {
      tTrack* current_track = &Drive->track[track][side];
      
      // track is not formatted?
      if (current_track->size == 0) continue;
      // track has not been modified ?
      if (current_track->new_data == cNull) continue;

      // Get real track size
      dwRealTrackSize = 0;
      // loop for all sectors
      for (sector = 0; sector < (tUChar)current_track->sectors; sector++)
      {
        tSector* current_sector = &current_track->sector[sector];
        dwRealTrackSize += current_sector->data_size;
      }
      dwRealTrackSize = ROUND_UPPER_256(dwRealTrackSize);

      // Real track is smaller ?
      if (dwRealTrackSize < current_track->size)
      {
        // Create new track data
        tUChar* optimized_data = NewMemFct(dwRealTrackSize);
        if (optimized_data == cNull) continue;

        // Update drive size
        Drive->new_data_size =   Drive->new_data_size
                               - current_track->size
                               + dwRealTrackSize;

        // Copy track data
        MemMove(optimized_data, current_track->new_data, dwRealTrackSize);

        // Replace track data
        DeleteMemFct(current_track->new_data);
        current_track->new_data = optimized_data;
        current_track->new_size = dwRealTrackSize;
        current_track->data = current_track->new_data;
        current_track->size = current_track->new_size;
      }
    }
  }


#ifdef _TESTU
  return PerformTestU(DiskOperation->NativeCPC);
#else /* _TESTU */
  return errNone;
#endif /* _TESTU */
}



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


//==============================================================================
//
// Unitary Tests
//
//==============================================================================
#ifdef _TESTU
// Prototypes of TestU fonctions
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

  /* 1 */ if (Result == errNone) Result = TestU_MemMove_1(NativeCPC,
                                                          NoTest++);
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
