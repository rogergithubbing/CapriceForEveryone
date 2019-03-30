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


#ifndef TRAWTrackH
#define TRAWTrackH

#include "..\..\Engine\Native_CPC.h"
#include "..\..\Common\RAW.h"


// Additional errors
#define errRAWFileDoesNotExist            0x1101
#define errRAWBadMemoryAllocation         0x1102
#define errRAWBadStructure                0x1103
#define errRAWBadRecord                   0x1104
#define errRAWNotEmpty                    0x1105
#define errRAWBadRevolutionIndex          0x1106


class TRAWTrack
{
private:
  //
  // Members
  //
  tUChar* mTrackData;
  tULong mTrackDataSize;
  tUChar* mMFMTrack;
  tUShort mMFMTrackSize;
  tULong mID;
  tULong mTrackNumber;
  tULong mHeadNumber;
  tUChar mNbRevolutions;
  tUChar** mRevolutionsDataPP;
  tUShort* mRevolutionsSizeP;
  TList* mSectorsList;
  //
  // Methods
  //
  inline tULong __fastcall GetID(void) { return mID; };
  inline tULong __fastcall GetTrackNumber(void) { return mTrackNumber; };
  inline void __fastcall SetTrackNumber(tULong value) { mTrackNumber = value; };
  inline tULong __fastcall GetHeadNumber(void) { return mHeadNumber; };
  inline void __fastcall SetHeadNumber(tULong value) { mHeadNumber = value; };
  inline tUChar __fastcall GetNbRevolutions(void) { return mNbRevolutions; };
  inline tUChar* __fastcall GetTrackData(void) { return mTrackData; };
  inline tULong __fastcall GetTrackSize(void) { return mTrackDataSize; };
  bool __fastcall IsPowerOf128(tULong Value);
  tUChar* __fastcall DecodeMFM(tUChar* DestP, tUChar* SourceP, tULong Size);
  tUChar* __fastcall DecompressTrackData(tUChar* DestP,
                                         tUChar* SourceP,
                                         tULong Size,
                                         const tUChar* FirstRevolutionP);
  int __fastcall GetNbSectors(void);
  tSector* __fastcall GetSector(int Index);
  int __fastcall DetectSectors(void);
  bool __fastcall DetectCustomGAP3(tUChar* Data, tULong* Length);
  bool __fastcall DetectSYNC(tUChar* Data, tULong* Length);
  int __fastcall GetDSKSize(void);
  int __fastcall GetDSKHeaderSize(void);
  tUChar __fastcall GetGAP3Length(void);

public:
  //
  // Properties
  //
  __property tULong ID = { read=GetID };
  __property tULong TrackNumber = { read=GetTrackNumber, write=SetTrackNumber };
  __property tULong HeadNumber = { read=GetHeadNumber, write=SetHeadNumber };
  __property tUChar NbRevolutions = { read=GetNbRevolutions };
  __property tUChar* TrackData = { read=GetTrackData };
  __property tULong TrackSize = { read=GetTrackSize };
  __property int NbSectors = { read=GetNbSectors };
  __property int DSKHeaderSize = { read=GetDSKHeaderSize };
  __property int DSKSize = { read=GetDSKSize };
  __property tUChar GAP3Length = { read=GetGAP3Length };
  __property const tSector* Sector[int Index] = { read=GetSector };

  //
  // API
  //
  __fastcall TRAWTrack();
  __fastcall ~TRAWTrack();

  Err __fastcall DecodeFromStream(tUChar* StreamDataP, tULong StreamSize);
  Err __fastcall DecodeRevolutionMFM(tUChar Index);
  Err __fastcall DetectTrack(void);

  int __fastcall BuildSectors(void);

  void __fastcall CopyData(tUChar* Dest, tUChar* Source, tULong Length);
};

//---------------------------------------------------------------------------
#endif

