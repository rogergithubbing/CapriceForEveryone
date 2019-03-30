/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2016 by Frédéric Coste

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


#include <vcl.h>
#pragma hdrstop
#include "TRAWTrack.h"
#include "..\..\Common\Native_CRC32.h"
#include "..\..\Common\FDC.h"
#include "..\..\Common\MFM.h"
#include "..\..\Common\CRC_CCITT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//=================================
// Options

//=================================

//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


__fastcall TRAWTrack::TRAWTrack()
//
// Constructor
//
{
  mTrackData = NULL;
  mTrackDataSize = 0;
  mMFMTrack = NULL;
  mMFMTrackSize = 0;
  mNbRevolutions = 0;
  mRevolutionsDataPP = NULL;
  mRevolutionsSizeP = NULL;
  mSectorsList = new TList;
  mTrackNumber = (tULong)-1;
  mHeadNumber = (tULong)-1;

}
//---------------------------------------------------------------------------


__fastcall TRAWTrack::~TRAWTrack()
//
// Destructor
//
{
  delete [] mTrackData;
  delete [] mMFMTrack;
  delete [] mRevolutionsDataPP;
  delete [] mRevolutionsSizeP;

  while (mSectorsList->Count > 0)
  {
    tSector* Item = (tSector*)mSectorsList->Items[0];
    delete Item;
    mSectorsList->Delete(0);
  }
  delete mSectorsList;
}
//---------------------------------------------------------------------------


int __fastcall TRAWTrack::GetNbSectors(void)
{
  return mSectorsList->Count;
}
//---------------------------------------------------------------------------
tSector* __fastcall TRAWTrack::GetSector(int Index)
{
  if (Index >= mSectorsList->Count)
    return NULL;

  return (tSector*)mSectorsList->Items[Index];
}
//---------------------------------------------------------------------------


bool __fastcall TRAWTrack::IsPowerOf128(tULong Value)
{
  /*   128 */      if (Value == (128 << 0)) return true;
  /*   256 */ else if (Value == (128 << 1)) return true;
  /*   512 */ else if (Value == (128 << 2)) return true;
  /*  1024 */ else if (Value == (128 << 3)) return true;
  /*  2048 */ else if (Value == (128 << 4)) return true;
  /*  4096 */ else if (Value == (128 << 5)) return true;
  /*  8192 */ else if (Value == (128 << 6)) return true;
  /* 16384 */ else if (Value == (128 << 7)) return true;
  /* 32768 */ else if (Value == (128 << 8)) return true;

  return false;
}
/*----------------------------------------------------------------------------*/


tUChar* __fastcall TRAWTrack::DecodeMFM(tUChar* DestP, tUChar* SourceP, tULong Size)
{
  while (Size--)
  {
    if (Size)
    {
      *(DestP++) = GetByteFromPulses(SourceP);
      SourceP += 2;
      Size--;
    }
    else
    {
      *(DestP++) = GetHalfByteFromPulses(SourceP);
      SourceP++;
    }
  }

  return SourceP;
}
/*----------------------------------------------------------------------------*/


tUChar* __fastcall TRAWTrack::DecompressTrackData(tUChar* DestP,
                                                  tUChar* SourceP,
                                                  tULong Size,
                                                  const tUChar* FirstRevolutionP)
{
  tUChar* Max = DestP + Size;

  while (DestP < Max)
  {
    tULong size = *(SourceP++);

    if (size & 0x80)
    {
			// copy block
			// 3 bit shift
			tUChar shift = (size >> 4) & 7;

			// 12 bit size
			size  = (size & 0xf) << 8;
      size |= *(SourceP++);

			// 16 bit offset
			tULong ofs = *SourceP++;
			ofs  = ofs << 8;
			ofs |= *(SourceP++);
			const tUChar* buf = FirstRevolutionP + ofs;

      if (shift)
      {
				// bitshift copy
				ofs = *(buf++);
				while (size--)
        {
					ofs  = ofs << 8;
          ofs |= *(buf++);
					*(DestP++) = (tUChar)(ofs >> shift);
				}
      }
      else
      {
				// simple copy
				while (size--)
        {
          *(DestP++) = *(buf++);
        }
      }
    }
    else
    {
      // data block, 15 bit size
      size  = size << 8;
      size |= *(SourceP++);
      while (size--)
      {
        *(DestP++) = *(SourceP++);
      }
    }
  }

  return SourceP;
}
/*----------------------------------------------------------------------------*/


Err __fastcall TRAWTrack::DecodeFromStream(tUChar* StreamDataP, tULong StreamSize)
{
  Err Error = errNone;

  // Empty Object
  delete [] mMFMTrack;
  delete [] mRevolutionsDataPP;
  delete [] mRevolutionsSizeP;
  mMFMTrack = NULL;
  mRevolutionsDataPP = NULL;
  mRevolutionsSizeP = NULL;

  do
  {
    tRAWRecordHeader* Header = (tRAWRecordHeader*)StreamDataP;

    // Bad header identifier ?
    if (*((tULong*)&Header->Identifier) != *((tULong*)&(RAW_DATA_HEADER_STRING)))
    {
      Error = errRAWBadStructure;
      continue;
    }

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != RAW_DATA_RECORD_SIZE)
    {
      Error = errRAWBadRecord;
      continue;
    }

    // Compute Record CRC
    tULong oldCRC = Header->RecordCrc32;
    tULong RecordCrc32 = ENDIAN_SWAP_32(Header->RecordCrc32);
    // To compute CRC32, use Header crc as 0 !!
    Header->RecordCrc32 = 0;
    tULong ComputedCrc32 = CRC32(StreamDataP, RecordSize);
    Header->RecordCrc32 = oldCRC;
    if (RecordCrc32 != ComputedCrc32)
    {
      Error = errRAWBadRecord;
      continue;
    }

    //
    // Read Record INFO
    //
    StreamDataP += RAW_RECORD_HEADER_SIZE;
    tRAWDataBlock* DataBlockInfo = (tRAWDataBlock*)(StreamDataP);

    tULong RecordDataSize = ENDIAN_SWAP_32(DataBlockInfo->data_size);
    //tULong RecordDataSizeInBits = ENDIAN_SWAP_32(DataBlockInfo->data_size_in_bits);
    mID = ENDIAN_SWAP_32(DataBlockInfo->id);
    tULong PACKDensitySize = ENDIAN_SWAP_32(DataBlockInfo->density_size);
    //tULong PACKTrackSize = ENDIAN_SWAP_32(DataBlockInfo->track_size);

    // Check Size
    if ( (RecordDataSize + RecordSize) > StreamSize)
    {
      Error = errRAWBadRecord;
      continue;
    }

    //
    // Get PACK track data info
    //
    StreamDataP += sizeof(tRAWDataBlock) + PACKDensitySize;
    tRAWRecordHeader* PACKTrackHeader = (tRAWRecordHeader*)(StreamDataP);
    //tULong TrackRecordSize = ENDIAN_SWAP_32(PACKTrackHeader->RecordSize);

    // Bad header identifier ?
    if (*((tULong*)&PACKTrackHeader->Identifier) != *((tULong*)&(RAW_PACK_HEADER_STRING)))
    {
      Error = errRAWBadStructure;
      continue;
    }

    //
    // Get revolutions size
    //
    StreamDataP += RAW_RECORD_HEADER_SIZE;
    tRAWPACKTrackBlock* PACKTrackInfo = (tRAWPACKTrackBlock*)(StreamDataP);

    mNbRevolutions = (tUChar)PACKTrackInfo->nb_revolutions;
    mMFMTrackSize = 0;

    mRevolutionsDataPP = new tUChar*[mNbRevolutions];
    mRevolutionsSizeP = new tUShort[mNbRevolutions];
    if ( (mRevolutionsDataPP == NULL) || (mRevolutionsSizeP == NULL) )
    {
      Error = errRAWBadMemoryAllocation;
      continue;
    }

    tUChar* RevolutionsSizeP = StreamDataP + sizeof(tRAWPACKTrackBlock);
    for (tUChar Loop=0; Loop < mNbRevolutions; Loop++)
    {
      tUShort Size = (tUShort)*(RevolutionsSizeP++) << 8;
      Size += *(RevolutionsSizeP++);

      mRevolutionsSizeP[Loop] = Size;
      mRevolutionsDataPP[Loop] = NULL;

      // Update size of track
      mMFMTrackSize += Size;
    }

    // Allocate track memory
    mMFMTrack = new tUChar[mMFMTrackSize];
    if (mMFMTrack == NULL)
    {
      Error = errRAWBadMemoryAllocation;
      continue;
    }

    // Prepare revolution data pointers
    tUChar* TrackDataP = mMFMTrack;
    for (tUChar Loop=0; Loop < mNbRevolutions; Loop++)
    {
      mRevolutionsDataPP[Loop] = TrackDataP;
      TrackDataP += mRevolutionsSizeP[Loop];
    }

    //
    // Copy first revolution
    //
    StreamDataP = RevolutionsSizeP;
    memcpy(mRevolutionsDataPP[0], StreamDataP, mRevolutionsSizeP[0]);

    //
    // Decompress others revolutions
    //
    StreamDataP += mRevolutionsSizeP[0];
    for (tUChar Loop=1; Loop < mNbRevolutions; Loop++)
    {
      StreamDataP = DecompressTrackData(mRevolutionsDataPP[Loop],
                                        StreamDataP,
                                        mRevolutionsSizeP[Loop],
                                        mMFMTrack);
    }
  } while (0);

  return (Error);
}
/*----------------------------------------------------------------------------*/


Err __fastcall TRAWTrack::DecodeRevolutionMFM(tUChar Index)
{
  Err Error = errNone;

  delete [] mTrackData;
  mTrackData = NULL;
  mTrackDataSize = 0;

  do
  {
    // Bad index ?
    if (Index >= mNbRevolutions)
    {
      Error = errRAWBadRevolutionIndex;
      continue;
    }

    // Get Size
    mTrackDataSize  = mRevolutionsSizeP[Index] / 2;
    mTrackDataSize += mRevolutionsSizeP[Index] & 1;

    // Allocate memory
    mTrackData = new tUChar[mTrackDataSize];
    if (mTrackData == NULL)
    {
      Error = errRAWBadMemoryAllocation;
      continue;
    }

    // Decode MFM
    DecodeMFM(mTrackData, mRevolutionsDataPP[Index], mRevolutionsSizeP[Index]);
  }
  while (0);

  return (Error);
}
/*----------------------------------------------------------------------------*/


Err __fastcall TRAWTrack::DetectTrack(void)
{
  Err Error = errNone;

  tUChar* IDAM = NULL;

  for (tUChar Loop=0; Loop < mNbRevolutions; Loop++)
  {
    if (DecodeRevolutionMFM(Loop) == errNone)
    {
      tUChar* ShiftedTrack = new tUChar[mTrackDataSize];

      for (tUChar Loop=0; Loop < 8; Loop++)
      {
        MFMShiftWithCopy(ShiftedTrack, mTrackData, mTrackDataSize, Loop, 0);
        IDAM = DetectIDAM(ShiftedTrack, mTrackDataSize);
        if (IDAM != NULL)
        {
          delete [] mTrackData;
          mTrackData = ShiftedTrack;
          break;
        }

        MFMShiftWithCopy(ShiftedTrack, mTrackData, mTrackDataSize, Loop, 1);
        IDAM = DetectIDAM(ShiftedTrack, mTrackDataSize);
        if (IDAM != NULL)
        {
          delete [] mTrackData;
          mTrackData = ShiftedTrack;
          break;
        }

      }

      if (IDAM == NULL)
      {
        delete [] ShiftedTrack;
      }
      else
      {
        break;
      }
    }
  }

  if (IDAM == NULL)
  {
    Error = errRAWBadStructure;
  }

  return (Error);
}
/*----------------------------------------------------------------------------*/


int __fastcall TRAWTrack::BuildSectors(void)
{
  if (mTrackDataSize == 0) return 0;
  if (mTrackData == NULL) return 0;

  // Detect all IDAM available
  DetectSectors();

  tUChar* TrackEnd = mTrackData + mTrackDataSize;

  // Build all sectors
  for (int SectorLoop=0; SectorLoop < mSectorsList->Count; SectorLoop++)
  {
    tSector* Sector = (tSector*)mSectorsList->Items[SectorLoop];

    // Too big sector ?
    if (Sector->declared_size > FDC_MAX_BYTES_PER_TRACK)
    {
      Sector->flags[0] |= ST1_DE;
      Sector->flags[1] |= ST2_DD;
      Sector->size = mTrackDataSize;
      continue; // Next sector
    }
    // Incomplete sector ?
    else if (Sector->declared_size > (tULong)(TrackEnd - Sector->data))
    {
      Sector->flags[0] |= ST1_DE;
      Sector->flags[1] |= ST2_DD;
      Sector->size = (tULong)(TrackEnd - Sector->data);
      continue; // Next sector
    }

    //
    // DATA
    //
    // Prepare crc
    tUShort crc;
    if (Sector->flags[1] & ST2_CM)
    {
      crc = CRC_CCITT(fdc_dataam_deleted, FDC_BYTES_DATAAM);
    }
    else
    {
      crc = CRC_CCITT(fdc_dataam, FDC_BYTES_DATAAM);
    }
    // Compute crc
    tULong DataLength = Sector->declared_size;
    tUChar* Data = Sector->data;
    tULong real_size = 0;
    while (DataLength--)
    {
      // Update crc
      crc = CRC_CCITT_Add_Byte(crc, *Data);

      // Next byte
      Data++;
      real_size++;
      if (Data == TrackEnd) Data = mTrackData;

      if (crc == 0)
      {
        if (IsPowerOf128(real_size - FDC_BYTES_CRC) == 1)
        {
          real_size -= FDC_BYTES_CRC;
          break;
        }
      }
    }

    Sector->size = real_size;
    Sector->gap3 = Data;

    if (Sector->size == Sector->declared_size)
    {
      // Add CRC
      DataLength = FDC_BYTES_CRC;
      while (DataLength--)
      {
        // Update crc
        crc = CRC_CCITT_Add_Byte(crc, *Data);
        // Next byte
        Data++;
        if (Data == TrackEnd) Data = mTrackData;
      }
    }

    // Bad CRC ?
    if (crc)
    {
      Sector->flags[0] |= ST1_DE;
      Sector->flags[1] |= ST2_DD;
    }

    //
    // GAP3
    //
    DataLength = TrackEnd - Data - 1;
    Sector->gap3_length = FDC_BYTES_CRC;
    Sector->size += FDC_BYTES_CRC;
    tUChar Marker_count = 0;
    while (DataLength--)
    {
      // Detect Next IDAM
      if (Marker_count == 3)
      {
        Marker_count = 0;

        // IDAM detected ?
        if (*Data == FDC_BYTE_IDAM_4)
        {
          Sector->size -= FDC_BYTES_IDAM - 1;
          Sector->gap3_length -= FDC_BYTES_IDAM - 1;
          break;
        }
      }
      Marker_count = (*Data == FDC_BYTE_IDAM_1_3) ? Marker_count + 1 : 0;

      Sector->size++;
      Sector->gap3_length++;

      // Next byte
      Data++;
      if (Data == TrackEnd)
      {
        // Last sector, stop at track end
        if (SectorLoop == (mSectorsList->Count - 1))
        {
          break;
        }
        else
        {
          Data = mTrackData;
        }
      }
    }

    // Custom GAP3 ?
    tULong PreviousGAPLength = Sector->gap3_length;
    if (DetectCustomGAP3(Sector->gap3, &Sector->gap3_length) == false)
    {
      Sector->gap3 = NULL;
      Sector->size -= PreviousGAPLength;
    }
  }

  return NbSectors;
}
//---------------------------------------------------------------------------


int __fastcall TRAWTrack::DetectSectors(void)
{
  // Remove sectors from list
  while (mSectorsList->Count > 0)
  {
    tSector* Item = (tSector*)mSectorsList->Items[0];
    delete Item;
    mSectorsList->Delete(0);
  }

  tSector* NewSector = NULL;

  tUChar Marker_count = 0;
  tUChar CHRN_count = 0;
  bool IDAM_detected = false;
  bool signal_GAP2 = false;
  bool signal_DATA = false;
  tUChar* MarkerStart = NULL;
  tUShort crc;

  for (tUChar* Data = mTrackData;
       Data < (mTrackData + mTrackDataSize);
       Data++)
  {
    //
    // Marker detection
    //
    // End of marker
    if (Marker_count == 3)
    {
      Marker_count = 0;

      if ( (*Data == FDC_BYTE_DATAAM_4)
           || (*Data == FDC_BYTE_DATAAM_4_DELETED) )
      {
        if (NewSector)
        {
          signal_DATA = true;
          // DATAAM Found
          NewSector->flags[0] &= ~(ST1_MA);
          NewSector->flags[1] &= ~(ST2_MD);
          // Control Mark
          NewSector->flags[1] |= (*Data == FDC_BYTE_DATAAM_4_DELETED) ? ST2_CM : 0;
          continue; // Next data
        }
      }
      else if (*Data == FDC_BYTE_IDAM_4)
      {
        IDAM_detected = true;
        CHRN_count = 0;
        // Create and add new sector
        NewSector = new tSector;
        memset(NewSector, 0, sizeof(tSector));
        mSectorsList->Add(NewSector);
        // Prepare crc
        crc = CRC_CCITT(MarkerStart, FDC_BYTES_IDAM);
        MarkerStart = NULL;
        continue; // Next data
      }
      else
      {
        MarkerStart = NULL;
      }
    }
    Marker_count = (*Data == FDC_BYTE_IDAM_1_3) ? Marker_count + 1 : 0;
    if (Marker_count == 1)
    {
      MarkerStart = Data;
    }

    //
    // CHRN
    //
    if (IDAM_detected == true)
    {
      // Store CHRN
      NewSector->CHRN[CHRN_count++] = *Data;
      crc = CRC_CCITT_Add_Byte(crc, *Data);

      // End of CHRN
      if (CHRN_count == (FDC_BYTES_CHRN + FDC_BYTES_CRC))
      {
        IDAM_detected = false;
        signal_GAP2 = true;
        // Set NO DATA flags until DATAAM detection
        NewSector->flags[0] |= ST1_MA;
        NewSector->flags[1] |= ST2_MD;
        // Update info
        NewSector->declared_size = 128 << (NewSector->CHRN[SECTOR_CHRN_N] & 7);
        NewSector->weak_count = 1;
        // Bad CRC ?
        if (crc)
        {
          NewSector->flags[0] |= ST1_DE;
        }
      }
      continue;
    }

    //
    // GAP2
    //
    if (signal_GAP2 == true)
    {
      NewSector->gap2 = Data;
      signal_GAP2 = false;
    }

    //
    // DATA
    //
    if (signal_DATA == true)
    {
      if (!NewSector->data) NewSector->data = Data;
      signal_DATA = false;
    }
  }

  return mSectorsList->Count;
}
//---------------------------------------------------------------------------


bool __fastcall TRAWTrack::DetectCustomGAP3(tUChar* Data, tULong* GAP3Length)
{
  if (Data == NULL) return false;

  tUChar* TrackEnd = mTrackData + mTrackDataSize;
  tULong Length = *GAP3Length;

  // Pass DATA CRC
  Length -= FDC_BYTES_CRC;
  for (tUChar Loop=0; Loop < FDC_BYTES_CRC; Loop++)
  {
    Data++;
    if (Data == TrackEnd) Data = mTrackData;
  }

  bool CustomGAP3 = false;
  while (Length--)
  {
    if (*Data != FDC_BYTE_GAP3)
    {
      tULong SYNCLength;
      if (DetectSYNC(Data, &SYNCLength) == true)
      {
        // Remove SYNC from GAP3
        *GAP3Length -= Length + FDC_BYTES_CRC - 1;
      }
      else
      {
        CustomGAP3 = true;
      }

      // Remove CRC
      if (CustomGAP3 == false)
      {
        *GAP3Length -= FDC_BYTES_CRC;
      }
      break;
    }

    Data++;
  }

  return CustomGAP3;
}
//---------------------------------------------------------------------------


bool __fastcall TRAWTrack::DetectSYNC(tUChar* Data, tULong* Length)
{
  tUChar* TrackEnd = mTrackData + mTrackDataSize;
  tUChar SYNCSize = FDC_BYTES_SYNC - 1; // Remove tolerance

  for (tUChar Loop=0; Loop < SYNCSize; Loop++)
  {
    if (*(Data++) != FDC_BYTE_SYNC) return false;
    if (Data == TrackEnd) Data = mTrackData;
  }

  // Finish SYNC detection
  *Length = SYNCSize;
  while (*(Data++) == FDC_BYTE_SYNC)
  {
    (*Length)++;
    if (Data == TrackEnd) Data = mTrackData;
  }

  return true;
}
//---------------------------------------------------------------------------


int __fastcall TRAWTrack::GetDSKHeaderSize(void)
{
  int TrackHeaderSize = DSK_TRACKHEADER_SIZE;
  if (mSectorsList->Count > DSK_SECTORMAX)
  {
    int NbExtendedSectors = mSectorsList->Count - DSK_SECTORMAX;
    NbExtendedSectors = ((NbExtendedSectors / DSK_NBSECTORPERHEADER) + 1) * DSK_NBSECTORPERHEADER;

    TrackHeaderSize += NbExtendedSectors * DSK_SECTORID_SIZE;
  }

  return TrackHeaderSize;
}
//---------------------------------------------------------------------------


int __fastcall TRAWTrack::GetDSKSize(void)
{
  int Size = 0;
  for (int Loop=0; Loop < mSectorsList->Count; Loop++)
  {
    tSector* Sector = (tSector*)mSectorsList->Items[Loop];
    if (Sector->weak_count == 1)
    {
      Size += Sector->size;
    }
    else
    {
      Size += Sector->declared_size * Sector->weak_count;
    }
    Size += Sector->gap3_length;
  }
  return (ROUND_UPPER_256(Size));
}
//---------------------------------------------------------------------------


tUChar __fastcall TRAWTrack::GetGAP3Length(void)
{
tUChar TrackGAP3Length = 0;

  for (int SectorLoop=0; SectorLoop < (mSectorsList->Count-1); SectorLoop++)
  {
    tSector* Sector = (tSector*)mSectorsList->Items[SectorLoop];

    tULong SectorGAP3Length = 0;
    // Only take standard gap sectors into account
    if (!Sector->gap3)
    {
      SectorGAP3Length = Sector->gap3_length;
    }

    // Saturation
    if (SectorGAP3Length > 255) SectorGAP3Length = 255;

    if (TrackGAP3Length == 0)
    {
      TrackGAP3Length = (tUChar)SectorGAP3Length;
    }
    else if (SectorGAP3Length)
    {
      if ((tUChar)SectorGAP3Length < TrackGAP3Length)
      {
        TrackGAP3Length = (tUChar)SectorGAP3Length;
      }
    }
  }

  return TrackGAP3Length;
}
//---------------------------------------------------------------------------


void __fastcall TRAWTrack::CopyData(tUChar* Dest, tUChar* Source, tULong Length)
{
  tUChar* TrackEnd = mTrackData + mTrackDataSize - 1;

  while (Length--)
  {
    *(Dest++) = *Source;
    Source = (Source == TrackEnd) ? mTrackData : Source+1;
  }
}
//---------------------------------------------------------------------------


