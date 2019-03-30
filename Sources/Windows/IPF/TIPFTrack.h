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


#ifndef TIPFTrackH
#define TIPFTrackH

#include "..\..\Engine\Native_CPC.h"
#include "..\..\Common\IPF.h"

typedef struct
{
  tUChar* Data;
  tULong Size;
}tFuzzyArea;


class TIPFTrack
{
private:
  //
  // Members
  //
  tIPFTrackBlock* mTrackBlock;
  tULong mEncoderType;
  tUChar* mExtraDataBlock;
  TList* mDataBlockDescriptorsList;
  tUChar* mTrackData;
  tULong mTrackDataSize;
  TList* mSectorsList;
  TList* mFuzzyAreasList;

  //
  // Methods
  //
  void __fastcall SetTrackBlock(tIPFTrackBlock* Value);
  inline tIPFTrackBlock* __fastcall GetTrackBlock(void)  { return mTrackBlock; };
  inline tULong __fastcall GetDataKey(void) { return mTrackBlock->dataKey; };
  inline tULong __fastcall GetBlockCount(void) { return mTrackBlock->blockCount; };
  inline tULong __fastcall GetEncoderType(void) { return mEncoderType; };
  inline void __fastcall SetEncoderType(tULong Value) { mEncoderType = Value; };
  int __fastcall GetNbSectors(void);
  tSector* __fastcall GetSector(int Index);
  tIPFDataBlockDescriptor* __fastcall GetDataBlockDescriptor(int Index);
  inline const tUChar* __fastcall GetTrackData(void) { return (mTrackData); };
  inline int __fastcall GetSize(void) { return (mTrackDataSize); };
  bool __fastcall IsPowerOf128(tULong Value);
  int __fastcall GetDSKSize(void);
  int __fastcall GetDSKHeaderSize(void);
  tUChar __fastcall GetGAP3Length(void);
  inline tULong __fastcall GetCylinder(void) { return mTrackBlock->track; };
	inline tULong __fastcall GetSide(void) { return mTrackBlock->side; };
  //
  tULong __fastcall GetStreamSize(tUChar* Buffer, tUChar Width);
  tUChar __fastcall GetByteFromPulses(tUChar* Bits);
  tUChar __fastcall GetHalfByteFromPulses(tUChar* Bits);
  tUChar* __fastcall AddDataDescriptor(tIPFDataBlockDescriptor* Descriptor,
                                       tUChar* TrackDataPtr);
  tUChar* __fastcall AddSPSGapDescriptor(tIPFDataBlockDescriptor* Descriptor,
                                         tUChar* TrackDataPtr);
  bool __fastcall DetectSYNC(tUChar* Data, tULong* Length);
  bool __fastcall DetectCustomGAP3(tUChar* Data, tULong* Length);
  tULong __fastcall GetRawTrackSize(void);
  //tULong __fastcall GetDataDescriptorDataSize(tIPFDataBlockDescriptor* Descriptor);
  tULong __fastcall GetDataDescriptorFuzzySize(tIPFDataBlockDescriptor* Descriptor);
  tULong __fastcall GetFuzzyTotalLength(void);
  int __fastcall DetectSectors(void);

public:
  //
  // Properties
  //
  __property tIPFTrackBlock* TrackBlock = { read=GetTrackBlock,write=SetTrackBlock };
  __property tULong DataKey = { read=GetDataKey };
  __property tULong BlockCount = { read=GetBlockCount };
  __property tULong EncoderType = { read=GetEncoderType,write=SetEncoderType };
  __property int NbSectors = { read=GetNbSectors };
  __property const tSector* Sector[int Index] = { read=GetSector };
  __property const tIPFDataBlockDescriptor* DataBlockDescriptor[int Index] = { read=GetDataBlockDescriptor };
  __property const tUChar* Data = { read=GetTrackData };
  __property int Size = { read=GetSize };
  __property int DSKHeaderSize = { read=GetDSKHeaderSize };
  __property int DSKSize = { read=GetDSKSize };
  __property tULong Cylinder = { read=GetCylinder };
  __property tULong Side = { read=GetSide };
  __property tUChar GAP3Length = { read=GetGAP3Length };

  //
  // API
  //
  __fastcall TIPFTrack();
  __fastcall ~TIPFTrack();

  void __fastcall AddDataBlockDescriptor(tIPFDataBlockDescriptor* Descriptor);
  void __fastcall AddExtraDataBlock(tUChar* Data);

  void __fastcall CopyData(tUChar* Dest, tUChar* Source, tULong Length);
  void __fastcall CopyFuzzyData(tUChar* Dest, tSector* Sector);

  tULong __fastcall BuildTrack(void);
  tULong __fastcall BuildSPSTrack(tUChar* Buffer, tULong Length);
  int __fastcall BuildSectors(void);
};

//---------------------------------------------------------------------------
#endif

