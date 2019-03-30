/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2015 by Frédéric Coste

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
#endif /* __PALMOS__ */


#include "..\Native_CPC.h"



/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================



#define FILE_ENTRY_SIZE                32


#if defined(__PALMOS__)
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
tULong Engine_GetCatalog(tDiskOperation* DiskOperation)
#endif /* __PALMOS__ || __WIN32__ */
/***********************************************************************
 *
 *  Engine_GetCatalog
 *
 ***********************************************************************/
{
#if defined(__PALMOS__)
tDiskOperation* DiskOperation = (tDiskOperation*)userData68KP;
#endif /* __PALMOS__ */
tDrive* driveP;
tULong indexTrack;
tULong indexSector;
tULong loopSector;
tULong loopEntry;
tULong loopChar;
tULong loopCatalogEntry;

#if defined(__PALMOS__)

  //
  // Palm OS 68K interface
  //
  // Disk content read by Native_DiskLoad
  driveP = (tDrive*)EndianSwap32(DiskOperation->Drive);

#else /* __PALMOS__ */

  driveP = DiskOperation->Drive;

#endif /* __PALMOS__ */

  DiskOperation->NbCatalogEntries = 0;

  //
  // Parse catalog from Track 0 to Track 2 until valid entries detected
  //
  for (indexTrack = 0;
       (indexTrack <= 2) && !DiskOperation->NbCatalogEntries;
       indexTrack++)
  {
    tTrack* actualTrackP = &driveP->track[indexTrack][0]; // Side = 0

    // Do not take into account empty or missing tracks
    if (!actualTrackP->size)
      continue;
      
    indexSector = 1; // Begin to find the first sector
    
    // Only find file entries into four first sectors.
    while (indexSector <= 4)
    {
      tULong nbofentries;
  	  tUChar* fileEntryP;
      tSector* sectorP;

      //
      // Find sector
      //
      sectorP = 0;
      for (loopSector=0; loopSector < actualTrackP->sectors; loopSector++)
      {
    	  if ( ((actualTrackP->sector[loopSector].CHRN[SECTOR_CHRN_R] & 0x1FU) == indexSector)
             && (actualTrackP->sector[loopSector].CHRN[SECTOR_CHRN_N] == 2) )
    	  {
          sectorP = &actualTrackP->sector[loopSector];
          break;
    	  }
      }
  	  // Do not take into account empty or missing sector
  	  if (!sectorP || (!sectorP->size) )
  	  {
  	  	// Next sector
  	  	indexSector++;
  	  	continue;
  	  }

      // Prepare sector read
      nbofentries = sectorP->size / FILE_ENTRY_SIZE; // 32 bytes per file entry
  	  fileEntryP = sectorP->data; // Point at beginning of sector.
  	  
      for (loopEntry=0; loopEntry < nbofentries; loopEntry++, fileEntryP += FILE_ENTRY_SIZE)
      {
    	  tUChar* bytePtr;
        tBool FilenameFound;

    	  //
    	  // Parse File Entry
    	  //
    	  // Entry documentation
    	  // ===================
    	  // Byte 0x0 = USER; 229(0xe5) for deleted files, 0 by default.
    	  // Byte 0x1 to 0x8 = filename
    	  // Byte 0x9 to 0xb = extension. Byte 0x9 bit7 = 1 = Read Only. Byte 0xa bit7 = 1 = Hidden.
    	  // Byte 0xc to 0xe = Not used ??
    	  // Byte 0xf = size in records (1 record=128 bytes). 0x80 for full entry => check another entry
    	  // Byte 0x10 to 0x1f = list of numbers of 1k blocks (2 sectors length).
    	  
    	  // How to locate a block
    	  // =====================
    	  // Cylinder (C) = INT(block * 2 / 9); 9 sectors per track !! For EXTENDED, check number of sectors per Tracks
    	  // Head     (H) = Always 0
    	  // Sector   (R) = (block * 2) MODULO 9 + 1 + TYPE (0xC0 for DATA, 0x40 for VENDOR)
    	  // Size     (N) = Size (*256 bytes)
    	  
    	  // USER must be 0.
    	  if (fileEntryP[0x0] != 0)
    	    continue;

        // Entry size is non null
        if (!fileEntryP[0xf])
          continue;
    	  
    	  // Check filename and extension
    	  bytePtr = &fileEntryP[0x1]; // Point at the beginning of the filename.
    	  while ( ((*bytePtr & 0x7f) >= 0x20 /*SPACE*/) && 
    	          ((*bytePtr & 0x7f) <= 0x5a /*Z*/)) bytePtr++; // Loop until non printable character
        if (bytePtr < fileEntryP+0xc)
          continue;

        //
        // Entry is valid
        //
        FilenameFound = cFalse;
        // Search already stored filename
        for (loopCatalogEntry=0;
             loopCatalogEntry < DiskOperation->NbCatalogEntries;
             loopCatalogEntry++)
        {
    		  // Compare filename and extension
  	      tUChar* fileStoredEntryP = DiskOperation->CatalogEntries[loopCatalogEntry];
    	    bytePtr = &fileEntryP[0x1]; // Point at the beginning of the filename.
    		  for (loopChar=0; loopChar < 11; loopChar++)
    		  {
    		    if ((*bytePtr++ & 0x7f) != *fileStoredEntryP++)
    		      break;
    		  }
          // Matching filename ?
          if (loopChar == 11)
          {
            FilenameFound = cTrue;
            break;
          }
        }

        // Add new entry
        if (FilenameFound == cFalse)
        {
          tUChar* fileStoredEntryP = DiskOperation->CatalogEntries[DiskOperation->NbCatalogEntries];
    	    bytePtr = &fileEntryP[0x1]; // Point at the beginning of the filename.

          DiskOperation->NbCatalogEntries++;

          // Add filename + extension
          for (loopChar=0; loopChar < 11; loopChar++)
          {
            *fileStoredEntryP++ = *bytePtr++ & 0x7f;  // Do not copy Read-only and Hidden flags
          }
        }
      }

      indexSector++; // Next sector
    }
  }

  return DiskOperation->NbCatalogEntries;
}
/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
