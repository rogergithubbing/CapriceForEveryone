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
#include "TIPFTrack.h"
#include "..\..\Common\FDC.h"
#include "..\..\Common\CRC_CCITT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//=================================
// Options

#undef USE_HALF_WEAK
#define USE_HALF_WEAK

//=================================

//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


#define BYTES_SYNC_MIN      10
#define BYTES_SYNC_MAX      14



__fastcall TIPFTrack::TIPFTrack()
//
// Constructor
//
{
  mTrackBlock = new tIPFTrackBlock;
  mEncoderType = IPF_ENCODERTYPE_CAPS;
  mExtraDataBlock = NULL;
  mDataBlockDescriptorsList = new TList;
  mSectorsList = new TList;
  mFuzzyAreasList = new TList;

  mTrackData = NULL;
  mTrackDataSize = 0;
}
//---------------------------------------------------------------------------


__fastcall TIPFTrack::~TIPFTrack()
//
// Destructor
//
{
  while (mDataBlockDescriptorsList->Count > 0)
  {
    tIPFDataBlockDescriptor* Item = (tIPFDataBlockDescriptor*)mDataBlockDescriptorsList->Items[0];
    delete Item;
    mDataBlockDescriptorsList->Delete(0);
  }
  delete mDataBlockDescriptorsList;

  while (mSectorsList->Count > 0)
  {
    tSector* Item = (tSector*)mSectorsList->Items[0];
    delete Item;
    mSectorsList->Delete(0);
  }
  delete mSectorsList;

  while (mFuzzyAreasList->Count > 0)
  {
    tFuzzyArea* Item = (tFuzzyArea*)mFuzzyAreasList->Items[0];
    delete Item;
    mFuzzyAreasList->Delete(0);
  }
  delete mFuzzyAreasList;

  delete [] mExtraDataBlock;
  delete mTrackBlock;

  delete [] mTrackData;
}
//---------------------------------------------------------------------------


bool __fastcall TIPFTrack::IsPowerOf128(tULong Value)
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


int __fastcall TIPFTrack::GetNbSectors(void)
{
  return mSectorsList->Count;
};
//---------------------------------------------------------------------------


tSector* __fastcall TIPFTrack::GetSector(int Index)
{
  if (Index >= mSectorsList->Count)
    return NULL;

  return (tSector*)mSectorsList->Items[Index];
};
//---------------------------------------------------------------------------
tIPFDataBlockDescriptor* __fastcall TIPFTrack::GetDataBlockDescriptor(int Index)
{
  if (Index >= mDataBlockDescriptorsList->Count)
    return NULL;

  return (tIPFDataBlockDescriptor*)mDataBlockDescriptorsList->Items[Index];
}
//---------------------------------------------------------------------------


int __fastcall TIPFTrack::GetDSKHeaderSize(void)
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


int __fastcall TIPFTrack::GetDSKSize(void)
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


tUChar __fastcall TIPFTrack::GetGAP3Length(void)
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


void __fastcall TIPFTrack::SetTrackBlock(tIPFTrackBlock* Value)
{
  // Replace existing trackBlock
  delete mTrackBlock;
  mTrackBlock = Value;
};
//---------------------------------------------------------------------------


void __fastcall TIPFTrack::AddDataBlockDescriptor(tIPFDataBlockDescriptor* Descriptor)
{
  mDataBlockDescriptorsList->Add(Descriptor);
}
//---------------------------------------------------------------------------


void __fastcall TIPFTrack::AddExtraDataBlock(tUChar* DataArea)
{
  delete [] mExtraDataBlock;
  mExtraDataBlock = DataArea;
}
//---------------------------------------------------------------------------


tULong __fastcall TIPFTrack::GetStreamSize(tUChar* SizePtr, tUChar Width)
{
  tULong dataSize;
  switch (Width)
  {
    case 2:
    {
      dataSize  = *(SizePtr+0) << 8;
      dataSize += *(SizePtr+1) << 0;
    }
    break;

    case 3:
    {
      dataSize  = *(SizePtr+0) << 16;
      dataSize += *(SizePtr+1) << 8;
      dataSize += *(SizePtr+2) << 0;
    }
    break;

    case 4:
    {
      dataSize  = *(SizePtr+0) << 24;
      dataSize += *(SizePtr+1) << 16;
      dataSize += *(SizePtr+2) << 8;
      dataSize += *(SizePtr+3) << 0;
    }
    break;

    default:
      dataSize = *(SizePtr+0);
  }

  return dataSize;
}
//---------------------------------------------------------------------------


void __fastcall TIPFTrack::CopyData(tUChar* Dest, tUChar* Source, tULong Length)
{
  tUChar* TrackEnd = mTrackData + mTrackDataSize - 1;

  while (Length--)
  {
    *(Dest++) = *Source;
    Source = (Source == TrackEnd) ? mTrackData : Source+1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TIPFTrack::CopyFuzzyData(tUChar* Dest, tSector* Sector)
{
#ifndef USE_HALF_WEAK
  for (tULong Loop=0; Loop < Sector->declared_size; Loop++)
  {
    *(Dest++) = 0xE5;
  }
  for (tULong Loop=0; Loop < Sector->declared_size; Loop++)
  {
    *(Dest++) = 0xFF;
  }
#else /* USE_HALF_WEAK */
  for (tULong Loop=0; Loop < (Sector->declared_size/2); Loop++)
  {
    *(Dest++) = 0xE5;
  }
  for (tULong Loop=0; Loop < (Sector->declared_size/2); Loop++)
  {
    *(Dest++) = 0x0F;
  }
  for (tULong Loop=0; Loop < (Sector->declared_size/2); Loop++)
  {
    *(Dest++) = 0xE5;
  }
  for (tULong Loop=0; Loop < (Sector->declared_size/2); Loop++)
  {
    *(Dest++) = 0xF0;
  }
#endif /* USE_HALF_WEAK */
}
//---------------------------------------------------------------------------


tULong __fastcall TIPFTrack::BuildTrack(void)
{
  delete [] mTrackData;
  mTrackData = NULL;

  mTrackDataSize = mTrackBlock->dataBits + mTrackBlock->gapBits;
  mTrackDataSize /= 2; /* MFM pulses per bit */
  mTrackDataSize = ROUND_UPPER_8(mTrackDataSize) / 8; // size in bytes

  // Not formatted track
  if (mTrackDataSize == 0) return 0;
  if (mDataBlockDescriptorsList->Count == 0) return 0;

  // Create new track data buffer
  mTrackData = new tUChar[mTrackDataSize];
  if (mTrackData == NULL) return 0;
  memset(mTrackData, 0, mTrackDataSize);

  tUChar* TrackDataCreatePtr = mTrackData + mTrackBlock->startBytePos;

  // For all data block descriptors
  for (tULong Loop=0; Loop < mTrackBlock->blockCount; Loop++)
  {
    tIPFDataBlockDescriptor* Descriptor = (tIPFDataBlockDescriptor*)mDataBlockDescriptorsList->Items[Loop];

    if (Descriptor->dataBits)
    {
      TrackDataCreatePtr = AddDataDescriptor(Descriptor,
                                             TrackDataCreatePtr);
    }

    if ( (mEncoderType == IPF_ENCODERTYPE_SPS)
         && (Descriptor->gapBits)
         && (Descriptor->flags.All) )
    {
      // Add described gap
      TrackDataCreatePtr = AddSPSGapDescriptor(Descriptor,
                                               TrackDataCreatePtr);
    }
  }

  return mTrackDataSize;
}
//---------------------------------------------------------------------------


tULong __fastcall TIPFTrack::BuildSPSTrack(tUChar* Buffer, tULong Length)
{
  delete [] mTrackData;
  mTrackData = NULL;

  mTrackDataSize = Length / 2;

  // Not formatted track
  if (mTrackDataSize == 0) return 0;

  // Create new track data buffer
  mTrackData = new tUChar[mTrackDataSize];
  if (mTrackData == NULL) return 0;
  memset(mTrackData, 0, mTrackDataSize);

  tUChar* TrackData = mTrackData;
  tUChar* SPSBuffer = Buffer;
  while (Length)
  {
    if (Length > 1)
    {
      *(TrackData++) = GetByteFromPulses(SPSBuffer);
      SPSBuffer += 2;
      Length -= 2;
    }
    else
    {
      *(TrackData++) = GetHalfByteFromPulses(SPSBuffer);
      SPSBuffer ++;
      Length--;
    }
  }
  
  return mTrackDataSize;
}
//---------------------------------------------------------------------------



tUChar __fastcall TIPFTrack::GetByteFromPulses(tUChar* Bits)
{
  tUChar Byte = 0;
  tUShort Pulses = (*Bits << 8) + *(Bits+1);

  for (tUChar Loop=0; Loop < 8; Loop++)
  {
    tUShort Bit = Pulses >> 14;

    Byte = Byte << 1;
    if (Bit == 1)
    {
      Byte |= 0x01;
    }

    Pulses = Pulses << 2;
  }

  return Byte;
}
//---------------------------------------------------------------------------
tUChar __fastcall TIPFTrack::GetHalfByteFromPulses(tUChar* Bits)
{
  tUChar Byte = 0;
  tUShort Pulses = (*Bits << 8);

  for (tUChar Loop=0; Loop < 8; Loop++)
  {
    tUShort Bit = Pulses >> 14;

    Byte = Byte << 1;
    if (Bit == 1)
    {
      Byte |= 0x01;
    }

    Pulses = Pulses << 2;
  }

  return Byte;
}
//---------------------------------------------------------------------------


tUChar* __fastcall TIPFTrack::AddDataDescriptor(tIPFDataBlockDescriptor* Descriptor,
                                                tUChar* TrackDataPtr)
{
  tUChar* TrackDataEndPtr = mTrackData + mTrackDataSize;

  // Browse data stream
  tUChar* DataStream = mExtraDataBlock + Descriptor->dataOffset;
  while (*DataStream)
  {
    tUChar dataSizeWidth = *DataStream >> 5;
    tUChar dataType = *DataStream & 0x1f;
    DataStream++; // Pass header
    tULong dataSize = GetStreamSize(DataStream, dataSizeWidth);
    tUChar* Data = DataStream + dataSizeWidth;

    switch (dataType)
    {
      case IPF_DATATYPE_SYNC:
      {
        if (Descriptor->flags.bits.DataInBit == 1)
        {
          dataSize = ROUND_UPPER_8(dataSize) / 8;
        }

        tULong SyncSize = dataSize & ~1; // Only whole bytes
        for (tULong Loop=0; Loop < SyncSize; Loop +=2)
        {
          *(TrackDataPtr++) = GetByteFromPulses(Data+Loop);
          if (TrackDataPtr == TrackDataEndPtr) TrackDataPtr = mTrackData;
        }
      }
      break;

      case IPF_DATATYPE_DATA:
      case IPF_DATATYPE_GAP:
      {
        if (Descriptor->flags.bits.DataInBit == 1)
        {
          dataSize /= 8;
        }

        for (tULong Loop=0; Loop < dataSize; Loop++)
        {
          *(TrackDataPtr++) = *(Data++);
          if (TrackDataPtr == TrackDataEndPtr) TrackDataPtr = mTrackData;
        }
      }
      break;

      case IPF_DATATYPE_FUZZY:
      {
        if (Descriptor->flags.bits.DataInBit == 1)
        {
          dataSize /= 8;
        }
        // Add Fuzzy area
        tFuzzyArea* FuzzyAreaP = (tFuzzyArea*) new tFuzzyArea;
        FuzzyAreaP->Data = TrackDataPtr;
        FuzzyAreaP->Size = dataSize;
        mFuzzyAreasList->Add(FuzzyAreaP);

        // Mark Fuzzy area
        for (tULong Loop=0; Loop < dataSize; Loop++)
        {
          *(TrackDataPtr++) = 0xFC;
          if (TrackDataPtr == TrackDataEndPtr) TrackDataPtr = mTrackData;
        }
        dataSize = 0; // For next datastream
      }
      break;
    }

    DataStream += dataSizeWidth + dataSize;
  }

  return TrackDataPtr;
}
//---------------------------------------------------------------------------


#if 0
tULong __fastcall TIPFTrack::GetDataDescriptorDataSize(tIPFDataBlockDescriptor* Descriptor)
{
  tULong Size = 0;

  // Browse data stream
  tUChar* DataStream = mExtraDataBlock + Descriptor->dataOffset;
  while (*DataStream)
  {
    tUChar dataSizeWidth = *DataStream >> 5;
    tUChar dataType = *DataStream & 0x1f;
    DataStream++; // Pass header
    tULong dataSize = GetStreamSize(DataStream, dataSizeWidth);

    switch (dataType)
    {
      case DATATYPE_SYNC:
      {
        Size += dataSize / 2;
      }
      break;

      case DATATYPE_DATA:
      case DATATYPE_GAP:
      {
        Size += dataSize;
      }
      break;

      case DATATYPE_FUZZY:
      {
        Size += dataSize;
        dataSize = 0;  // For next datastream
      }
      break;
    }

    DataStream += dataSizeWidth + dataSize;
  }

  return Size;
//---------------------------------------------------------------------------
#endif


tULong __fastcall TIPFTrack::GetDataDescriptorFuzzySize(tIPFDataBlockDescriptor* Descriptor)
{
  tULong Size = 0;

  // Browse data stream
  tUChar* DataStream = mExtraDataBlock + Descriptor->dataOffset;
  while (*DataStream)
  {
    tUChar dataSizeWidth = *DataStream >> 5;
    tUChar dataType = *DataStream & 0x1f;
    DataStream++; // Pass header
    tULong dataSize = GetStreamSize(DataStream, dataSizeWidth);

    switch (dataType)
    {
      case IPF_DATATYPE_FUZZY:
      {
        Size += dataSize;
        dataSize = 0;  // For next datastream
      }
      break;
    }

    DataStream += dataSizeWidth + dataSize;
  }

  return Size;
}
//---------------------------------------------------------------------------


tULong __fastcall TIPFTrack::GetFuzzyTotalLength(void)
{
tULong FuzzyLength = 0;

  // For all data block descriptors
  for (tULong Loop=0; Loop < mTrackBlock->blockCount; Loop++)
  {
    tIPFDataBlockDescriptor* Descriptor = (tIPFDataBlockDescriptor*)mDataBlockDescriptorsList->Items[Loop];

    if (Descriptor->dataBits)
    {
      FuzzyLength += GetDataDescriptorFuzzySize(Descriptor);
    }
  }

  return FuzzyLength;
}
//---------------------------------------------------------------------------


tUChar* __fastcall TIPFTrack::AddSPSGapDescriptor(tIPFDataBlockDescriptor* Descriptor,
                                                  tUChar* TrackDataPtr)
{
  tUChar* TrackDataEndPtr = mTrackData + mTrackDataSize;
  tULong GapLength = Descriptor->gapBits / 2 / 8;
  tUChar* GapEnd;
  if ((tULong)(TrackDataEndPtr - TrackDataPtr) < GapLength)
  {
    GapEnd = mTrackData + (GapLength - (TrackDataEndPtr - TrackDataPtr));
  }
  else
  {
    GapEnd = TrackDataPtr + GapLength;
  }

  // Initialise gap area using default value
  tUChar* GapPtr = TrackDataPtr;
  for (tULong Loop=0; Loop < GapLength; Loop++)
  {
    *(GapPtr++) = (tUChar)Descriptor->gapDefault;
    if (GapPtr == TrackDataEndPtr) GapPtr = mTrackData;
  }

  // Browse gap stream
  tUChar* GapStream = mExtraDataBlock + Descriptor->encoder.SPS.gapOffset;
  while (*GapStream)
  {
    tUChar gapSizeWidth = *GapStream >> 5;
    tUChar gapElemType = *GapStream & 0x1f;
    tULong gapSize = GetStreamSize(GapStream+1, gapSizeWidth);

    if (gapElemType == 1) // GapLength
    {
      GapLength = gapSize / 8;
      // Next stream
      GapStream += 1 /* gapHead */ + gapSizeWidth;
    }
    else // SampleLength
    {
      tULong SampleSize = gapSize / 8;
      tUChar* SamplePtr = GapStream + 1 /* gapHead */ + gapSizeWidth;
      while (GapLength)
      {
        // Copy sample to fill gap
        for (tULong Loop=0; (Loop < SampleSize) && GapLength; Loop++, GapLength--)
        {
          *(TrackDataPtr++) = SamplePtr[Loop];
          if (TrackDataPtr == TrackDataEndPtr) TrackDataPtr = mTrackData;
        }
      }

      // Next stream
      GapStream += 1 /* gapHead */ + gapSizeWidth + SampleSize;
    }
  }

  return GapEnd;
}
//---------------------------------------------------------------------------


bool __fastcall TIPFTrack::DetectSYNC(tUChar* Data, tULong* Length)
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


bool __fastcall TIPFTrack::DetectCustomGAP3(tUChar* Data, tULong* GAP3Length)
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


int __fastcall TIPFTrack::BuildSectors(void)
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
    else if (Sector->declared_size > (tULong)(TrackEnd - Sector->data + mTrackBlock->startBytePos))
    {
      Sector->flags[0] |= ST1_DE;
      Sector->flags[1] |= ST2_DD;
      Sector->size = (tULong)(TrackEnd - Sector->data + mTrackBlock->startBytePos);
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


int __fastcall TIPFTrack::DetectSectors(void)
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

  // For all sectors
  for (int SectorLoop=0; SectorLoop < mSectorsList->Count; SectorLoop++)
  {
    tSector* Sector = (tSector*)mSectorsList->Items[SectorLoop];

    // For all fuzzy areas
    for (int FuzzyLoop=0; FuzzyLoop < mFuzzyAreasList->Count; FuzzyLoop++)
    {
      tFuzzyArea* FuzzyArea = (tFuzzyArea*)mFuzzyAreasList->Items[FuzzyLoop];

      if ( (FuzzyArea->Data >= Sector->data)
           && (FuzzyArea->Data < (Sector->data + Sector->declared_size)) )
      {
        Sector->weak_count = 2;
        break;
      }
    }  
  }

  return mSectorsList->Count;
}
//---------------------------------------------------------------------------


