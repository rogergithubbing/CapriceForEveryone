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
#include "..\..\Common\CRC_CCITT.h"
#include "..\..\Common\FDC.h"
#include "..\..\Common\MFM.h"


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#undef PATCH_1
#undef PATCH_2
#undef PATCH_3
#define PATCH_1  // Conditions to create Declared_data = OK
#define PATCH_2  // Weak sector : The CPC Buggy Boy Pack = OK
#define PATCH_3  // Detect MFM bit shifted SYNC = OK

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
static tUShort MemCmp(tUChar* s1,
                      tUChar* s2,
                      tULong numBytes);
static tVoid MemMove(tUChar* destP,
                     const tUChar* sourceP,
                     tULong numBytes);
static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value);
static tUShort CalcCRC_CHRN(tUChar* chrn_p);
static tUShort CalcCRC_DATA(tSector* SectorP);
static tULong DetectNextSector(tSector* CurrentSectorP,
                               tSector* NextSectorP,
                               tUShort* NewCRCP);
static tULong DetectWeakSectors(tSector* SectorP);
static tBool DetectGAP3(tSector* SectorP, tULong GAP3Offset);
static tVoid RemoveLastSYNC(tSector* SectorP);
static tULong RemoveGAP3(tSector* SectorP);
static tBool IsPowerOf128(tULong Value);
static tBool IsEmptySector(tSector* SectorP, tUChar filler);
extern void CreateDeclaredSizeSector(tTrack* trackP, tULong CurrentSector);


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
tULong Engine_DiskLoad(tDiskOperation* DiskOperation)
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
#if defined(_TRACE) || defined(_TESTU)
tNativeCPC* NativeCPC;
#endif /* _TRACE || _TESTU */
tDrive* Drive;
tSector* pSector;
tUChar* pbPtr;
tUChar* pbEndPtr;
tUChar* pbDataPtr;
tUChar* pbTrackSizeTable;
tUShort Result = errNone;
tULong track;
tULong side;
tULong sector;
tULong data_start;
tULong dwTrackSize;
tULong dwSectorSize;
tULong dwSectors;
tUShort computed_crc;

#ifndef _TRACE
#ifdef __PALMOS__
  NOT_USED(emulStateP);
  NOT_USED(call68KFuncP);
#endif /* __PALMOS__ */
#endif /* !_TRACE */

#if defined(__PALMOS__)

  // Palm OS 68K interface
  Drive = (tDrive*)EndianSwap32(DiskOperation->Drive);
#if defined(_TRACE) || defined(_TESTU)
  NativeCPC = (tNativeCPC*)EndianSwap32(DiskOperation->NativeCPC);
#endif /* _TRACE || _TESTU */
  Drive->dataP = (tUChar*)EndianSwap32(DiskOperation->DiskContentP);
  Drive->data_size = (tULong)EndianSwap32(DiskOperation->disk_size);
  Drive->new_data_size = 0;
  
#elif defined(__WIN32__)

  Drive = DiskOperation->Drive;
#if defined(_TRACE) || defined(_TESTU)
  NativeCPC = DiskOperation->NativeCPC;
#endif /* _TRACE || _TESTU */
  Drive->dataP = DiskOperation->DiskContentP;
  Drive->data_size = DiskOperation->disk_size;

#endif /* __PALMOS__ || __WIN32__ */

  if ( (Drive->dataP == cNull) || (Drive->data_size == 0) )
  {
    return vfsErrDSKNoData;
  }

  pbPtr = Drive->dataP;
  pbEndPtr = pbPtr + Drive->data_size;

  //
  // Normal DSK image
  //
  if (MemCmp(pbPtr, "MV - CPC", 8) == 0) // normal DSK image?
  {
    Drive->tracks = *(pbPtr + 0x30); // number of tracks
    if (Drive->tracks > DSK_TRACKMAX) // limit to maximum possible
    {
      Drive->tracks = DSK_TRACKMAX;
    }

    Drive->sides = *(pbPtr + 0x31); // number of sides
    if (Drive->sides > DSK_SIDEMAX) // abort if more than maximum
    {
      return vfsErrDSKTooManySides;
    }
    Drive->sides--; // zero base number of sides
    
    dwTrackSize = *(pbPtr + 0x32) + (*(pbPtr + 0x33) << 8) - DSK_TRACKHEADER_SIZE; // determine track size in bytes, minus track header

    // Begin of first track
    pbPtr += DSK_HEADER_SIZE;

    for (track = 0; track < Drive->tracks; track++) // loop for all tracks
    {
      for (side = 0; side <= Drive->sides; side++) // loop for all sides
      {
        if (MemCmp(pbPtr, "Track-Info", 10) != 0) // abort if ID does not match
        {
          return vfsErrDSKNoTrackInfo;
        }

        dwSectorSize = 0x80 << *(pbPtr + 0x14); // determine sector size in bytes
        dwSectors = *(pbPtr + 0x15); // grab number of sectors
        if (dwSectors > EDSK_SECTORMAX) // abort if sector count greater than maximum
        {
            dwSectors = EDSK_SECTORMAX;
        }
        Drive->track[track][side].sectors = dwSectors; // store sector count
        Drive->track[track][side].original_size = dwTrackSize; // store track size
        Drive->track[track][side].new_size = 0; 
        Drive->track[track][side].size = Drive->track[track][side].original_size;
        Drive->track[track][side].original_data = pbPtr + DSK_TRACKHEADER_SIZE;
        Drive->track[track][side].new_data = cNull;
        Drive->track[track][side].data = Drive->track[track][side].original_data;
        Drive->track[track][side].gap3_length = *(pbPtr + 0x16); // GAP#3 size
        Drive->track[track][side].filler = *(pbPtr + 0x17); // Track filler

        Drive->track[track][side].real_size = FDC_BYTES_FROM_INDEX_TO_GAP1;
        
        pbDataPtr = Drive->track[track][side].data; // pointer to start of memory buffer

        for (sector = 0; sector < dwSectors; sector++) // loop for all sectors
        {
          pSector = &Drive->track[track][side].sector[sector];
          MemMove(pSector->CHRN,
                  (pbPtr + 0x18), 
                  4); // copy CHRN
          MemMove(pSector->flags,
                  (pbPtr + 0x1c), 
                  2); // copy ST1 & ST2
          computed_crc = CalcCRC_CHRN(pSector->CHRN); // Compute CHRN CRC
          pSector->CHRN[SECTOR_CHRN_CRC_MSB] = (computed_crc >> 8) & 0xff;
          pSector->CHRN[SECTOR_CHRN_CRC_LSB] = computed_crc & 0xff;
          
          pSector->size = dwSectorSize;
          pSector->declared_size = dwSectorSize;
          pSector->data_size = dwSectorSize;
          pSector->weak_count = 1;
          pSector->data = pbDataPtr; // store pointer to sector data
          pSector->data_gap3_length = 0;
          pSector->info_gap3_length = Drive->track[track][side].gap3_length;
          
          // Update track real size
          Drive->track[track][side].real_size += FDC_BYTES_FROM_GAP1_TO_CHRN + FDC_BYTES_FROM_CHRN_TO_DATA;
          pSector->track_offset = Drive->track[track][side].real_size;
          Drive->track[track][side].real_size += dwSectorSize + pSector->info_gap3_length;

          // Compute DATA CRC
          computed_crc = CalcCRC_DATA(pSector);
          pSector->data_crc[SECTOR_CRC_MSB] = (computed_crc >> 8) & 0xff;
          pSector->data_crc[SECTOR_CRC_LSB] = computed_crc & 0xff;
          pbDataPtr += dwSectorSize;
          pbPtr += DSK_SECTORID_SIZE;

          // Detect empty sector
          pSector->empty = IsEmptySector(pSector, Drive->track[track][side].filler);
        }

        // Begin of next track
        pbPtr = Drive->track[track][side].data + dwTrackSize;
      }
    }
    Drive->altered = 0; // disk is as yet unmodified
  }

  //
  // Extended DSK image
  //
  else if (MemCmp(pbPtr, "EXTENDED", 8) == 0)
  {
    Drive->tracks = *(pbPtr + 0x30); // number of tracks
    if (Drive->tracks > DSK_TRACKMAX) // limit to maximum possible
    {
      Drive->tracks = DSK_TRACKMAX;
    }

    Drive->random_DEs = *(pbPtr + 0x31) & 0x80; // simulate random Data Errors?
    Drive->sides = *(pbPtr + 0x31) & 3; // number of sides
    if (Drive->sides > DSK_SIDEMAX) // abort if more than maximum
    {
      return vfsErrDSKTooManySides;
    }

    pbTrackSizeTable = pbPtr + 0x34; // pointer to track size table in DSK header
    Drive->sides--; // zero base number of sides

    // Begin of first track
    pbPtr += DSK_HEADER_SIZE;

    for (track = 0; track < Drive->tracks; track++) // loop for all tracks
    {
      for (side = 0; side <= Drive->sides; side++) // loop for all sides
      {
        dwTrackSize = (*pbTrackSizeTable++ << 8); // track size in bytes
        if (dwTrackSize != 0) // only process if track contains data
        {
          if (MemCmp(pbPtr, "Track-Info", 10) != 0) // abort if ID does not match
          {
            return vfsErrDSKNoTrackInfo;
          }

          if ((pbEndPtr - pbPtr) < (int)dwTrackSize)
          {
            dwTrackSize = pbEndPtr - pbPtr;
          }

          dwSectors = *(pbPtr + 0x15); // number of sectors for this track
          if (dwSectors > EDSK_SECTORMAX) // limit to maximum
          {
            dwSectors = EDSK_SECTORMAX;
          }

          // Compute data origin depending on sectors number
          data_start = DSK_TRACKHEADER_SIZE;
          if (dwSectors > DSK_SECTORMAX)
          {
            sector = dwSectors - DSK_SECTORMAX;
            sector = ((sector / DSK_NBSECTORPERHEADER) + 1) * DSK_NBSECTORPERHEADER;
            data_start += sector * DSK_SECTORID_SIZE;
          }
          dwTrackSize -= data_start; // compensate for track header

          Drive->track[track][side].sectors = dwSectors; // store sector count
          Drive->track[track][side].size = dwTrackSize; // store track size
          Drive->track[track][side].data = pbPtr + data_start;
          Drive->track[track][side].gap3_length = *(pbPtr + 0x16); // GAP#3 size
          Drive->track[track][side].filler = *(pbPtr + 0x17); // Track filler
          Drive->track[track][side].real_size = FDC_BYTES_FROM_INDEX_TO_GAP1;

          pbDataPtr = Drive->track[track][side].data; // pointer to start of memory buffer
          for (sector = 0; sector < dwSectors; sector++) // loop for all sectors
          {
            pSector = &Drive->track[track][side].sector[sector];
            MemMove(pSector->CHRN,
                    (pbPtr + 0x18),
                    4); // copy CHRN
            MemMove(pSector->flags,
                    (pbPtr + 0x1c),
                    2); // copy ST1 & ST2
            computed_crc = CalcCRC_CHRN(pSector->CHRN); // Compute CHRN CRC
            pSector->CHRN[SECTOR_CHRN_CRC_MSB] = (computed_crc >> 8) & 0xff;
            pSector->CHRN[SECTOR_CHRN_CRC_LSB] = computed_crc & 0xff;
            // store pointer to sector data
            pSector->data = pbDataPtr;
            // Compute sector size
            dwSectorSize = *(pbPtr + 0x1e) + (*(pbPtr + 0x1f) << 8); // sector size in bytes
            pSector->size = dwSectorSize;
            pSector->declared_size = 128 << (*(pbPtr + 0x1b) <= 8 ? *(pbPtr + 0x1b) : 8);
            pSector->info_gap3_length = Drive->track[track][side].gap3_length;
            if (pSector->size > pSector->declared_size)
            {
              // Data error in data field ?
              if ( (pSector->flags[0] & ST1_DE) && (pSector->flags[1] & ST2_DD) )
              {
                pSector->data_size = RemoveGAP3(pSector);
                pSector->weak_count = DetectWeakSectors(pSector);
                pSector->data_gap3_length = pSector->size - (pSector->data_size * pSector->weak_count);

                // Weak sectors ?
                if (pSector->weak_count > 1)
                {
                  pSector->data_size = pSector->declared_size;
                }
              }
              else
              {
                // Valid sector with specific GAP3
                pSector->weak_count = 1;
                pSector->data_gap3_length = pSector->size - pSector->declared_size;
                pSector->data_size = pSector->declared_size;
              }
            }
            else
            {
              // Data error in data field ?
              if ( (pSector->flags[0] & ST1_DE) && (pSector->flags[1] & ST2_DD) )
              {
                pSector->data_size = RemoveGAP3(pSector);
                pSector->weak_count = 1;
                pSector->data_gap3_length = pSector->size - pSector->data_size;
              }
              else
              {
                pSector->weak_count = 1;
                pSector->data_gap3_length = 0;
                pSector->data_size = pSector->size;
              }
            }

            // Check minimum GAP3 length
            if (pSector->data_gap3_length < DSK_MIN_GAP3_LENGTH)
            {
              pSector->data_gap3_length = 0;
            }

            // Not the last sector
            if (sector < (dwSectors-1))
            {
              // Remove SYNC into specific GAP3
              RemoveLastSYNC(pSector);
            }

            // Compute DATA CRC
            computed_crc = CalcCRC_DATA(pSector);
            pSector->data_crc[SECTOR_CRC_MSB] = (computed_crc >> 8) & 0xff;
            pSector->data_crc[SECTOR_CRC_LSB] = computed_crc & 0xff;

            // Detect true GAP3
            if (pSector->data_gap3_length)
            {
              if (DetectGAP3(pSector,
                             pSector->data_size * pSector->weak_count) == cFalse)
              {
                pSector->data_gap3_length = 0;
              }
            }

            // Detect empty sector
            pSector->empty = IsEmptySector(pSector, Drive->track[track][side].filler);

            // Update track real size
            Drive->track[track][side].real_size += FDC_BYTES_FROM_GAP1_TO_CHRN + FDC_BYTES_FROM_CHRN_TO_DATA;
            pSector->track_offset = Drive->track[track][side].real_size;
            Drive->track[track][side].real_size += pSector->data_size;
            Drive->track[track][side].real_size += (pSector->data_gap3_length) ? pSector->data_gap3_length : (FDC_BYTES_CRC + pSector->info_gap3_length);

            pbDataPtr += dwSectorSize;
            pbPtr += DSK_SECTORID_SIZE;
          }

          // Detect nested sectors
          if (dwSectors >= 2)
          {
            for (sector = 0; sector < (dwSectors-1); sector++) // loop for all sectors
            {
              pSector = &Drive->track[track][side].sector[sector];
              if (pSector->size)
              {
                tUShort new_data_crc;
                tULong real_size = DetectNextSector(pSector,
                                                    pSector+1,
                                                    &new_data_crc);
                if (real_size)
                {
                  tULong loop;
                  tULong size_diff = pSector->size - real_size;

                  // Update track size
                  Drive->track[track][side].real_size -= size_diff;

                  // Use real sector size
                  pSector->size = real_size;
                  pSector->data_crc[SECTOR_CRC_MSB] = (new_data_crc >> 8) & 0xff;
                  pSector->data_crc[SECTOR_CRC_LSB] = new_data_crc & 0xff;

                  // Update all other sectors real offset
                  for (loop = sector+1; loop < dwSectors; loop++)
                  {
                    if (Drive->track[track][side].sector[loop].track_offset)
                    {
                      Drive->track[track][side].sector[loop].track_offset -= size_diff;
                    }
                  }
                }
              }
            }
          }

          // Create declared data
          for (sector = 0; sector < dwSectors; sector++) // loop for all sectors
          {
            pSector = &Drive->track[track][side].sector[sector];
#ifndef PATCH_1
            if ( (pSector->data_size != pSector->declared_size)
                 && (pSector->declared_size > FDC_MAX_BYTES_PER_TRACK) )
#else /* PATCH_1 */
            if (pSector->data_size != pSector->declared_size)
#endif /* PATCH_1 */
            {
              pSector->declared_data = (tUChar*)NewMemFct(pSector->declared_size);
              CreateDeclaredSizeSector(&Drive->track[track][side], sector);
            }
          }

          // Begin of next track
          pbPtr = Drive->track[track][side].data + dwTrackSize;
        }
        else
        {
          MemSet((tUChar*)&Drive->track[track][side],
                 sizeof(tTrack),
                 0); // track not formatted
        }
      }
    }

    // Still something ?
    if (pbPtr < pbEndPtr)
    {
      // "Offset-Info" section exists ?
      if (MemCmp(pbPtr, "Offset-Info", 11) == 0)
      {
        Drive->offset_info_available = cTrue;
        pbPtr += EDSK_OFFSETINFOHEADERSIZE;

        for (track = 0; track < Drive->tracks; track++) // loop for all tracks
        {
          for (side = 0; side <= Drive->sides; side++) // loop for all sides
          {
            tTrack* pTrack = &Drive->track[track][side];

            if (pTrack->sectors)
            {
              // Track size
              pTrack->offset_info_size = *pbPtr + (*(pbPtr+1) << 8);
              pbPtr += 2;

              for (sector = 0; sector < pTrack->sectors; sector++) // loop for all sectors
              {
                pSector = &pTrack->sector[sector];

                // Sector offset
                pSector->offset_info = *pbPtr + (*(pbPtr+1) << 8);
                pbPtr += 2;
              }
            }
            else
            {
              pTrack->offset_info_size = 0;
            }
          }
        }

        // Adjust GAP3 length for all sectors
        for (track = 0; track < Drive->tracks; track++) // loop for all tracks
        {
          for (side = 0; side <= Drive->sides; side++) // loop for all sides
          {
            tTrack* pTrack = &Drive->track[track][side];

            // Formatted track ?
            if (pTrack->data)
            {
              // Offset-info available ?
              if (pTrack->offset_info_size)
              {
                if (pTrack->sectors > 1)
                {
                  // loop for all sectors but the last
                  for (sector = 0; sector < (pTrack->sectors - 1); sector++)
                  {
                    pSector = &pTrack->sector[sector];

                    // Entire sector ?
                    if (pSector->declared_size == pSector->data_size)
                    {
                      tSector* pNextSector = &pTrack->sector[sector+1];
                      tLong gap_diff;

                      // Get Offset difference
                      gap_diff = pNextSector->offset_info - pSector->offset_info
                                 - (pNextSector->track_offset - pSector->track_offset);

                      if (gap_diff > EDSK_GAP3ADJUSTTHRESHOLD)
                      {
                        tULong sector2;

                        // Apply GAP3 difference
                        pSector->info_gap3_length += gap_diff;
                        pTrack->real_size += gap_diff;

                        // Adjust remaining sectors track offset
                        for (sector2 = sector+1; sector2 < pTrack->sectors; sector2++)
                        {
                          pTrack->sector[sector2].track_offset += gap_diff;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    Drive->altered = 0; // disk is as yet unmodified
  }
  else
  {
    Result = vfsErrDSKBadSignature;
  }

#ifdef _TESTU
  if (Result == errNone)
  {
    Result = PerformTestU(NativeCPC);
  }
#endif /* _TESTU */

  return Result;
}
/*----------------------------------------------------------------------------*/



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


static tUShort CalcCRC_CHRN(tUChar* chrn_p)
/***********************************************************************
 *
 *  CalcCRC_CHRN
 *
 ***********************************************************************/
{
tUShort crc;

  // Compute CRC (Header + CHRN)
  crc = CRC_CCITT(fdc_idam, 4);
  crc = CRC_CCITT_Add(crc, chrn_p, 4);

  return (crc);
}
/*----------------------------------------------------------------------------*/


static tUShort CalcCRC_DATA(tSector* SectorP)
/***********************************************************************
 *
 *  CalcCRC_DATA
 *
 ***********************************************************************/
{
tUShort crc;

  // Compute CRC (Header + CHRN)
  if ((SectorP->flags[1] & ST2_CM) == 0)
  {
    crc = CRC_CCITT(fdc_dataam, 4);
  }
  else
  {
    crc = CRC_CCITT(fdc_dataam_deleted, 4);
  }

  // Compute Data CRC
  crc = CRC_CCITT_Add(crc, SectorP->data, SectorP->data_size);
  
  return (crc);
}
/*----------------------------------------------------------------------------*/


static tULong DetectNextSector(tSector* CurrentSectorP, 
                               tSector* NextSectorP,
                               tUShort* NewCRCP)
/***********************************************************************
 *
 *  DetectNextSector
 *
 ***********************************************************************/
{
tUChar* dataP;
tUChar* next_sector_dataP;
tULong index;
tULong next_sector_data_start;
tULong real_size = 0;
tUShort data_crc;
tUChar State = 0;

  if (CurrentSectorP->data_gap3_length)
    return 0;
    
  if (CurrentSectorP->weak_count > 1)
    return 0;

  if (CurrentSectorP->data == cNull)
    return 0;

  // Start CRC
  if ((CurrentSectorP->flags[1] & ST2_CM) == 0)
  {
    data_crc = CRC_CCITT(fdc_dataam, 4);
  }
  else
  {
    data_crc = CRC_CCITT(fdc_dataam_deleted, 4);
  }

  // For all current sector data
  dataP = CurrentSectorP->data;
  for (index = 0; index < CurrentSectorP->size; index++)
  {
    // DATA CRC
    if ( (((tUShort)*dataP) << 8) + ((tUShort)*(dataP+1)) == data_crc)
    {
      real_size = index;
      *NewCRCP = data_crc;
      State = 1;
    }
    
    switch (State)
    {
      // Next sector CHRN + CRC
      case 1:
      {
        if (    (*(dataP+0) == NextSectorP->CHRN[0])
             && (*(dataP+1) == NextSectorP->CHRN[1])
             && (*(dataP+2) == NextSectorP->CHRN[2])
             && (*(dataP+3) == NextSectorP->CHRN[3])
             && (*(dataP+4) == NextSectorP->CHRN[4])
             && (*(dataP+5) == NextSectorP->CHRN[5]) )
        {
          // Next step
          State = 2;
        }
      }
      break;
        
      // Next sector IDDATA
      case 2: 
      {
        if (    (*(dataP+0) == fdc_dataam[0])
             && (*(dataP+1) == fdc_dataam[1])
             && (*(dataP+2) == fdc_dataam[2]) )
        {
           // Next step
           State = 3;
           next_sector_data_start = index + 4;
        }
      }
      break;
      
      // Wait for sector DATA
      case 3:
      {
        if (index == next_sector_data_start)
        {
           // Next step
           State = 4;
           next_sector_dataP = NextSectorP->data;
        }
      }
      break;

      // Next sector DATA
      case 4:
      {
        if (*dataP != *(next_sector_dataP++))
        {
          // Not matching sector... restart detection
          State = 0;
        }
      }
      break;
    }
    
    // Next byte
    data_crc = CRC_CCITT_Add(data_crc, dataP, 1);
    dataP++;
  }

  // Next sector not detected
  if (State < 4)
  {
    real_size = 0;
    *NewCRCP = 0;
  }

  return real_size;
}
/*----------------------------------------------------------------------------*/


static tULong DetectWeakSectors(tSector* SectorP)
/***********************************************************************
 *
 *  DetectWeakSectors
 *
 ***********************************************************************/
{
tULong Result = 1; // No weak sector

  tULong WeakCount = SectorP->data_size / SectorP->declared_size;

  //
  // Into weak sectors, content should be different
  //
  if ( (WeakCount == 2) || (WeakCount == 3) )
  {
    tBool FoundDifferent = cFalse;
    do
    {
      tULong Loop;
      tUChar* OriginalSector = SectorP->data;
      tUChar* WeakSector = SectorP->data + (SectorP->declared_size * (WeakCount-1));
      for (Loop=0; Loop < SectorP->declared_size; Loop++)
      {
        if (*OriginalSector++ != *WeakSector++)
        {
          FoundDifferent = cTrue;
        }
      }

      // Same sector, not weak
      if (FoundDifferent == cFalse)
      {
        WeakCount--;
      }
    }
    while ( (WeakCount > 1)
            && (FoundDifferent == cFalse) );
  }

  //
  // Detect GAP3 in weak sectors
  //
  if ( (WeakCount == 2) || (WeakCount == 3) )
  {
    while (DetectGAP3(SectorP,
                      SectorP->declared_size * (WeakCount-1)) == cTrue)
    {
      WeakCount--;
    }
    Result = WeakCount;
  }

  return Result;
}
/*----------------------------------------------------------------------------*/


static tBool DetectGAP3(tSector* SectorP, tULong GAP3Offset)
/***********************************************************************
 *
 *  DetectGAP3
 *
 ***********************************************************************/
{
tUChar* GapDataP = SectorP->data + GAP3Offset;
tBool Gap3Found = cFalse;
tUShort computed_crc;

  if ((SectorP->size - GAP3Offset) >= FDC_BYTES_CRC)
  {
    // Compute DATA CRC
    computed_crc = CalcCRC_DATA(SectorP);

    // First bytes are CRC ?
    if (computed_crc == ((*GapDataP << 8) + *(GapDataP+1)))
    {
      // True GAP3
      return cTrue;
    }
  }

  return Gap3Found;
}
/*----------------------------------------------------------------------------*/


static void RemoveLastSYNC(tSector* SectorP)
/***********************************************************************
 *
 *  RemoveLastSYNC
 *
 ***********************************************************************/
{
tULong Loop;
tUChar NbSYNCByte = 0;
#ifdef PATCH_3
tUChar synchro_bitshift;
tUChar synchro_pulseshift;
#endif /* PATCH_3 */

  if (SectorP->data_gap3_length > FDC_BYTES_SYNC)
  {
    // Get GAP3 Start
    tUChar* GAPDataP = SectorP->data;
    GAPDataP += SectorP->data_size * SectorP->weak_count;
    GAPDataP += SectorP->data_gap3_length - 1;

    for (Loop = SectorP->data_gap3_length; Loop; Loop--, GAPDataP--)
    {
#ifndef PATCH_3
      // SYNC byte detection
      if (*GAPDataP == FDC_BYTE_SYNC)
      {
        NbSYNCByte++;
      }
#else /* PATCH_3 */
      // Detect bit shift and pulse shift
      DetectMFMShift(*GAPDataP, FDC_BYTE_SYNC, &synchro_bitshift, &synchro_pulseshift);

      // SYNC byte detection
      if (*GAPDataP == FDC_BYTE_SYNC)
      {
        NbSYNCByte++;
      }
      // MFM shifted SYNC byte detection
      else if ( (synchro_bitshift != 0) || (synchro_pulseshift != 0) )
      {
        NbSYNCByte++;
      }
#endif /* PATCH_3 */
      else
      {
        // Update sector size
        if (NbSYNCByte >= (FDC_BYTES_SYNC - 1)) // SYNC size tolerance
        {
          // Set new GAP3 length without Last SYNC
          SectorP->data_gap3_length -= NbSYNCByte; // Remove current last sync byte
        }
        break;
      }
    }
  }
}
/*----------------------------------------------------------------------------*/


static tULong RemoveGAP3(tSector* SectorP)
/***********************************************************************
 *
 *  RemoveGAP3
 *
 ***********************************************************************/
{
tUChar* dataP;
tULong new_size = SectorP->size;
tULong index;
tUShort data_crc;

#ifdef PATCH_2
  if (IsPowerOf128(new_size) == cTrue)
    return new_size;
#endif /* PATCH_2 */

  // Start CRC
  if ((SectorP->flags[1] & ST2_CM) == 0)
  {
    data_crc = CRC_CCITT(fdc_dataam, 4);
  }
  else
  {
    data_crc = CRC_CCITT(fdc_dataam_deleted, 4);
  }

  if (SectorP->size > FDC_BYTES_CRC)
  {
    // For all current sector data
    dataP = SectorP->data;
    for (index = 0; index < SectorP->size; index++)
    {
      // DATA CRC
      if ( (((tUShort)*dataP) << 8) + ((tUShort)*(dataP+1)) == data_crc)
      {
        if (IsPowerOf128(index) == cTrue)
        {
          new_size = index;
          break;
        }
      }

      // Next byte
      data_crc = CRC_CCITT_Add(data_crc, dataP, 1);
      dataP++;
    }
  }

  return new_size;
}
/*----------------------------------------------------------------------------*/


static tBool IsPowerOf128(tULong Value)
/***********************************************************************
 *
 *  IsPowerOf128
 *
 ***********************************************************************/
{
  /*   128 */      if (Value == (128 << 0)) return cTrue;
  /*   256 */ else if (Value == (128 << 1)) return cTrue;
  /*   512 */ else if (Value == (128 << 2)) return cTrue;
  /*  1024 */ else if (Value == (128 << 3)) return cTrue;
  /*  2048 */ else if (Value == (128 << 4)) return cTrue;
  /*  4096 */ else if (Value == (128 << 5)) return cTrue;
  /*  8192 */ else if (Value == (128 << 6)) return cTrue;
  /* 16384 */ else if (Value == (128 << 7)) return cTrue;

  return cFalse;
}
/*----------------------------------------------------------------------------*/


static tBool IsEmptySector(tSector* SectorP, tUChar filler)
/***********************************************************************
 *
 *  IsEmptySector
 *
 ***********************************************************************/
{
tUChar* dataP;
tULong index;
tBool Empty = cTrue;

  if (SectorP->weak_count == 1)
  {
    // For all current sector data
    for (index = 0, dataP = SectorP->data;
         index < SectorP->data_size;
         index++, dataP++)
    {
      if (*dataP != filler)
      {
        Empty = cFalse;
        break;
      }
    }
  }
  else
  {
    Empty = cFalse;
  }

  return Empty;
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
static tUShort TestU_MemCmp_1(tNativeCPC* NativeCPC,
                              tUChar NoTest);
static tUShort TestU_CRC_CCITT_1(tNativeCPC* NativeCPC,
                                 tUChar NoTest);
static tUShort TestU_CRC_CHRN_1(tNativeCPC* NativeCPC,
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
  /* 3 */ if (Result == errNone) Result = TestU_MemCmp_1(NativeCPC,
                                                         NoTest++);
  /* 4 */ if (Result == errNone) Result = TestU_CRC_CCITT_1(NativeCPC,
                                                            NoTest++);
  /* 5 */ if (Result == errNone) Result = TestU_CRC_CHRN_1(NativeCPC,
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

static tUShort TestU_MemCmp_1(tNativeCPC* NativeCPC,
                              tUChar NoTest)
/***********************************************************************
 *
 *  TestU_MemCmp_1
 *
 ***********************************************************************/
{
tUChar S1ArrayA[100];
tUChar S2ArrayA[100];
tUChar Loop;
tUChar Result = errNone;

	NOT_USED(NativeCPC);

  // Prepare conditions
  for (Loop=0; Loop<50; Loop++)
  {
    S1ArrayA[Loop] = Loop;
    S2ArrayA[Loop] = Loop;
  }
  for (Loop=50; Loop<100; Loop++)
  {
    S1ArrayA[Loop] = 1;
    S2ArrayA[Loop] = 2;
  }
  // Perform operation
  // and Check Result
  if (MemCmp(S1ArrayA, S2ArrayA, 50) != 0)
      Result=testUErrorClass+NoTest;

  if (MemCmp(S1ArrayA, S2ArrayA, 51) != 1)
      Result=testUErrorClass+NoTest;
  
  return (Result);
}
/*----------------------------------------------------------------------------*/

static tUShort TestU_CRC_CCITT_1(tNativeCPC* NativeCPC,
                                 tUChar NoTest)
/***********************************************************************
 *
 *  TestU_CRC_CCITT_1
 *
 ***********************************************************************/
{
tUChar Result = errNone;
tUChar string[10] = "Amstrad";
tUShort crc;

	NOT_USED(NativeCPC);

  // Prepare conditions

  // Perform operation
  crc = CRC_CCITT(string, 7);
  
  // Check result
  if (crc != 0x5C99)
      Result=testUErrorClass+NoTest;
  
  return (Result);
}
/*----------------------------------------------------------------------------*/

static tUShort TestU_CRC_CHRN_1(tNativeCPC* NativeCPC,
                                tUChar NoTest)
/***********************************************************************
 *
 *  TestU_CRC_CHRN_1
 *
 ***********************************************************************/
{
tUChar Result = errNone;
tUChar string[10];
tUShort crc;

	NOT_USED(NativeCPC);

  // Prepare conditions (taken from DSK image dump)
  string[0] = 0x04;  // C
  string[1] = 0x00;  // H
  string[2] = 0x44;  // R
  string[3] = 0x01;  // N
  string[4] = 0xC2;  // CRC MSB
  string[5] = 0xC4;  // CRC LSB

  // Perform operation
  crc = CalcCRC_CHRN(string);
  
  // Check result
  if (crc != 0xC2C4) 
      Result=testUErrorClass+NoTest;
  
  return (Result);
}
/*----------------------------------------------------------------------------*/

#endif /* _TESTU */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif



