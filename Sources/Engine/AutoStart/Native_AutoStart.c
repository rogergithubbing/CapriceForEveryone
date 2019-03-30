/*
    CaPriCe for Palm OS - Amstrad CPC 464/664/6128 emulator for Palm devices
    Copyright (C) 2006-2011 by Frédéric Coste
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2015-2016 by Frédéric Coste

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


//
// Internal functions
//
static tULong DetectCatalog(tDrive* driveP,
                            tTrack** trackPP,
                            tUChar** firstFileEntryPP
                           );


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
tULong Engine_AutoStart(tDiskOperation* DiskOperation)
#endif /* __PALMOS__ || __WIN32__ */
/***********************************************************************
 *
 *  PNOMain
 *
 ***********************************************************************/
{
#if defined(__PALMOS__)
tDiskOperation* DiskOperation = (tDiskOperation*)userData68KP;
#endif /* __PALMOS__ */
#if defined(_TRACE)
tNativeCPC* NativeCPC;
#endif /* _TRACE */
tDrive* driveP;
tTrack* expectedCatalogTrackP;
tTrack* effectiveCatalogTrackP;
tSector* sectorP;
tChar* CommandP;
tUChar* firstValidEntryP;
tULong nbValidEntries;
tULong detectCPM;
tULong loop;
tULong nbCharOfCommand;

#if defined(__PALMOS__)

  //
  // Palm OS 68K interface
  //
  // Disk content read by Native_DiskLoad
  driveP = (tDrive*)EndianSwap32(DiskOperation->Drive);
  // Pointer on autostart command
  CommandP = (tChar*)EndianSwap32(DiskOperation->Param);

#else /* __PALMOS__ */

  driveP = DiskOperation->Drive;
  // Pointer on autostart command
  CommandP = DiskOperation->Param;

#endif /* __PALMOS__ */


  // Default autostart command is NULL
  *CommandP = 0;

  //
  // Read disk format
  //
  sectorP = &driveP->track[0][0].sector[0]; // First sector pointer

  switch (sectorP->CHRN[SECTOR_CHRN_R] & 0xC0)
  {
    case 0xC0: // DATA format
    {
  	  // For DATA format, Catalogue is expected in Track 0 Side 0.
  	  expectedCatalogTrackP = &driveP->track[0][0];
  	  detectCPM = 0;
    }
    break;
    
    case 0x40: // VENDOR format
    {
  	  // For VENDOR format, Catalogue is expected in Track 2 Side 0.
  	  expectedCatalogTrackP = &driveP->track[2][0];
  	  detectCPM = 1;
    }
    break;
    
    case 0x00: // IBM format
    {
  	  // For IBM format, Catalogue is expected in Track 1 Side 0.
  	  expectedCatalogTrackP = &driveP->track[1][0];
  	  detectCPM = 0;
    }
    break;
    
    default:
    {
  	  return DiskFormatUnknown;
    }
  }

  //
  // Find and Parse Catalog
  //
  nbValidEntries = DetectCatalog(driveP,
                                 &effectiveCatalogTrackP,
                                 &firstValidEntryP
                                 );

  //
  // Detect and Launch CP/M
  //
  if (detectCPM)
  {
  	tULong CPMDetected = 0;
    
    // No valid entries, it should be CP/M !!
    if (!nbValidEntries)
    {
    	CPMDetected = 1;
    }
    // Catalog is not located where expected !!
    else if (effectiveCatalogTrackP != expectedCatalogTrackP)
    {
    	CPMDetected = 1;
    }
    
    if (CPMDetected)
    {
      // Prepare command to execute
      CommandP[0] = '|';
      CommandP[1] = 'C';
      CommandP[2] = 'P';
      CommandP[3] = 'M';
      CommandP[4] = '\r'; // RETURN
      CommandP[5] = '\0';

      return AutoStartOK;
    }
  }
  
  //
  // Try to RUN
  //
  if (!nbValidEntries)
    return NoValidFileEntry;
  if (nbValidEntries > 1)
    return TooManyFileEntries;

  // Prepare command to execute
  nbCharOfCommand = 0;
  CommandP[nbCharOfCommand++] = 'R';
  CommandP[nbCharOfCommand++] = 'U';
  CommandP[nbCharOfCommand++] = 'N';
  CommandP[nbCharOfCommand++] = '\"';

  // Add filename
  for (loop=1; loop <= 8; loop++)
  {
  	// Do not take spaces into account
  	if ((*(firstValidEntryP+loop) & 0x7f) != 0x20)
  	{
  	  CommandP[nbCharOfCommand++] = *(firstValidEntryP+loop) & 0x7f;
  	}
  }

  // Add Point to separation filename and extension
  CommandP[nbCharOfCommand++] = '.';

  // Add extension
  for (loop=9; loop <= 11; loop++)
  {
  	// Do not take spaces into account
  	if (*(firstValidEntryP+loop) != 0x20)
  	{
  	  CommandP[nbCharOfCommand++] = *(firstValidEntryP+loop) & 0x7f; // Do not copy Read-only and Hidden flags
  	}
  }

  // End of command
  CommandP[nbCharOfCommand++] = '\r'; // RETURN
  CommandP[nbCharOfCommand] = '\0';

  return AutoStartOK;
}
/*----------------------------------------------------------------------------*/


static tULong DetectCatalog(tDrive* driveP,
                            tTrack** trackPP,
                            tUChar** firstFileEntryPP)
/***********************************************************************
 *
 *  DetectCatalog
 *
 ***********************************************************************/
{
tTrack* actualTrackP;
tSector* sectorP;
tUChar* notVisiblefirstFileP = 0;
tTrack* notVisiblefirstFileTrackP = 0;
tULong nbValidEntries = 0;
tULong nbNotVisibleValidEntries = 0;
tULong indexTrack;
tULong indexSector;
tULong loopSector;
tULong loopEntry;
tULong loopChar;
tULong CATArtDetected;

  // Initialize caller pointer to NULL as non detection default values
  *trackPP = 0;
  *firstFileEntryPP = 0;

  //
  // Parse catalog from Track 0 to Track 2 until valid entries detected
  //
  for (indexTrack = 0; (indexTrack <= 2) && !nbValidEntries; indexTrack++)
  {
    actualTrackP = &driveP->track[indexTrack][0]; // Side = 0
    // Do not take into account empty or missing tracks
    if (!actualTrackP->size)
      continue;
      
    indexSector = 1; // Begin to find the first sector

    CATArtDetected = 0;

    // Only find file entries into four first sectors.
    while (indexSector <= 4)
    {
      tULong nbofentries;
  	  tUChar* fileEntryP;
  	  tULong ValidExtension;
  	  
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

      nbofentries = sectorP->declared_size / FILE_ENTRY_SIZE; // 32 bytes per file entry
  	  fileEntryP = sectorP->data; // Point at beginning of sector.
  	  
      for (loopEntry=0; loopEntry < nbofentries; loopEntry++, fileEntryP += FILE_ENTRY_SIZE)
      {
    	  tUChar* bytePtr;

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

    	  // Check filename and extension
    	  bytePtr = &fileEntryP[0x1]; // Point at the beginning of the filename.
    	  while ( ((*bytePtr & 0x7f) >= 0x20 /*SPACE*/) &&
    	          ((*bytePtr & 0x7f) <= 0x5a /*Z*/)) bytePtr++; // Loop until non printable character
        if (bytePtr < (fileEntryP+0xc))
        {
          CATArtDetected = 1;
          continue;
        }

    	  // Check filename with valid letters/figures
    	  bytePtr = &fileEntryP[0x1]; // Point at the beginning of the filename.
        while (bytePtr <= fileEntryP+0xc)
        {
          // Figure ?
          if ( ((*bytePtr & 0x7f) >= 0x30 /*0*/) && ((*bytePtr & 0x7f) <= 0x39 /*9*/) ) break;
          // Letter ?
          if ( ((*bytePtr & 0x7f) >= 0x41 /*A*/) && ((*bytePtr & 0x7f) <= 0x5a /*Z*/) ) break;
          bytePtr++;
        }
        // No valid characters ?
        if (bytePtr > fileEntryP+0xc)
          continue;

        // Entry size is non null
        if (!fileEntryP[0xf])
          continue;

        // Check executable extension
        ValidExtension = 0;
        // "   " Extension
        if ( ((fileEntryP[0x9] & 0x7f) == ' ') &&
             ((fileEntryP[0xa] & 0x7f) == ' ') &&
             ((fileEntryP[0xb] & 0x7f) == ' ') )
        {
        	ValidExtension = 1;
        }
        // "BAS" Extension
        else if ( ((fileEntryP[0x9] & 0x7f) == 'B') && 
                  ((fileEntryP[0xa] & 0x7f) == 'A') && 
                  ((fileEntryP[0xb] & 0x7f) == 'S') )
        {
        	ValidExtension = 1;
        }
        // "BIN" Extension
        else if ( ((fileEntryP[0x9] & 0x7f) == 'B') && 
                  ((fileEntryP[0xa] & 0x7f) == 'I') && 
                  ((fileEntryP[0xb] & 0x7f) == 'N') )
        {
        	ValidExtension = 1;
        }
        if (!ValidExtension)
          continue;
        
    	  // File is not visible OR CATArt detected
    	  if ( (fileEntryP[0xa] & 0x80) || (CATArtDetected) )
    	  {
    	    if (!notVisiblefirstFileP)
    	    {
    	      notVisiblefirstFileP = fileEntryP;
    	      notVisiblefirstFileTrackP = actualTrackP;
            nbNotVisibleValidEntries++;
    	    }
          else
          {
    		    // Compare filename and extension to the first valid entry
    		    for (loopChar=0x1; loopChar < 0xc; loopChar++)
    		    {
    		      if (fileEntryP[loopChar] != notVisiblefirstFileP[loopChar])
    		        break;
    		    }

    		    // Filename is different, increment valid entries count
    		    if (loopChar < 0xc)
            {
              nbNotVisibleValidEntries++;
            }
          }
          continue;
    	  }

        //
        // Entry is valid
        //
    		// ONLY store location of first valid entry.
    		if (!*firstFileEntryPP)
    		{
    	    *trackPP = actualTrackP;
    		  *firstFileEntryPP = fileEntryP;

    		  nbValidEntries++;
    		}
    		else // Check possible other valid entries
    		{
    		  // Compare filename and extension to the first valid entry
    		  for (loopChar=0x1; loopChar < 0xc; loopChar++)
    		  {
    		    if (fileEntryP[loopChar] != (*firstFileEntryPP)[loopChar])
    		      break;
    		  }
    		  
    		  // Filename is different, increment valid entries count
    		  if (loopChar < 0xc)
    		  {
    		    nbValidEntries++;
    		  }
    		}
      }

      indexSector++; // Next sector
    }
  }
  
  // No valid entry found
  if (!nbValidEntries)
  {
    // If only 1 not visible file found
    if ( (notVisiblefirstFileP) && (nbNotVisibleValidEntries == 1) )
    {
    	*firstFileEntryPP = notVisiblefirstFileP;
    	*trackPP = notVisiblefirstFileTrackP;

      nbValidEntries++;
    }
  }
  
  return nbValidEntries;
}


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
