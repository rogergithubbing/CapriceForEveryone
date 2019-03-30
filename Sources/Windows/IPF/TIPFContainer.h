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


#ifndef TIPFContainerH
#define TIPFContainerH


//=================================
// Options
//
#undef USE_CAPSLIB
#define USE_CAPSLIB
//=================================


#ifdef USE_CAPSLIB
#include "CommonTypes.h"
#include "CapsAPI.h"
#endif /* USE_CAPSLIB */
#include "..\..\Engine\Native_CPC.h"
#include "..\..\Common\IPF.h"
#include "TIPFTrack.h"


// Additional errors
#define errIPFFileDoesNotExist            0x1101
#define errIPFBadMemoryAllocation         0x1102
#define errIPFBadStructure                0x1103
#define errIPFBadRecord                   0x1104
#define errIPFBadPlatform                 0x1105
#define errIPFCAPSBaseError               0x1200

// CAPS Error
/*enum {
	imgeOk=0,
	imgeUnsupported,
	imgeGeneric,
	imgeOutOfRange,
	imgeReadOnly,
	imgeOpen,
	imgeType,
	imgeShort,
	imgeTrackHeader,
	imgeTrackStream,
	imgeTrackData,
	imgeDensityHeader,
	imgeDensityStream,
	imgeDensityData,
	imgeIncompatible,
	imgeUnsupportedType,
	imgeBadBlockType,
	imgeBadBlockSize,
	imgeBadDataStart,
	imgeBufferShort
};*/


// CAPSImg DLL Prototypes
#ifdef USE_CAPSLIB
typedef SDWORD (__cdecl *CAPSInitPtr)(void);
typedef SDWORD (__cdecl *CAPSExitPtr)(void);
typedef SDWORD (__cdecl *CAPSAddImagePtr)(void);
typedef SDWORD (__cdecl *CAPSLockImageMemoryPtr)(SDWORD id, PUBYTE buffer, UDWORD length, UDWORD flag);
typedef SDWORD (__cdecl *CAPSLoadImagePtr)(SDWORD id, UDWORD flag);
typedef SDWORD (__cdecl *CAPSGetImageInfoPtr)(PCAPSIMAGEINFO pi, SDWORD id);
typedef SDWORD (__cdecl *CAPSLockTrackPtr)(PVOID ptrackinfo, SDWORD id, UDWORD cylinder, UDWORD head, UDWORD flag);
typedef SDWORD (__cdecl *CAPSUnlockTrackPtr)(SDWORD id, UDWORD cylinder, UDWORD head);
typedef SDWORD (__cdecl *CAPSUnlockAllTracksPtr)(SDWORD id);
typedef SDWORD (__cdecl *CAPSUnlockImagePtr)(SDWORD id);
typedef SDWORD (__cdecl *CAPSRemImagePtr)(SDWORD id);
#endif /* USE_CAPSLIB */


class TIPFContainer
{
private:
  //
  // Members
  //
  tIPFInfoBlock mInfoBlock;
  TList* mTracksList;
  bool mCAPSLibPresent;
#ifdef USE_CAPSLIB
  String mCAPSLibFilename;
  HINSTANCE mCAPSLibHandle;
  CAPSInitPtr CAPSInit;
  CAPSExitPtr CAPSExit;
  CAPSAddImagePtr CAPSAddImage;
  CAPSLockImageMemoryPtr CAPSLockImageMemory;
  CAPSLoadImagePtr CAPSLoadImage;
  CAPSGetImageInfoPtr CAPSGetImageInfo;
  CAPSLockTrackPtr CAPSLockTrack;
  CAPSUnlockTrackPtr CAPSUnlockTrack;
  CAPSUnlockAllTracksPtr CAPSUnlockAllTracks;
  CAPSUnlockImagePtr CAPSUnlockImage;
  CAPSRemImagePtr CAPSRemImage;
#endif /* USE_CAPSLIB */
  
  //
  // Methods
  //
  tUChar* _fastcall CheckCAPSRecord(tUChar* Data, Err* Error);
  tUChar* _fastcall ReadINFORecord(tUChar* Data, Err* Error);
  tUChar* _fastcall ReadImageRecord(tUChar* Data, Err* Error);
  tUChar* _fastcall ReadDataRecord(tUChar* Data, Err* Error);
  tULong __fastcall GetDSKSize(void);
	TIPFTrack* __fastcall FindTrack(tUChar Cylinder, tUChar side);
	void __fastcall StrCopy(tChar* destP, tChar* srcP);
  inline bool __fastcall GetCAPSLibPresent(void) { return mCAPSLibPresent; };
  int __fastcall GetTrackCount(void);

public:
  //
  // Properties
  //
  __property tULong DSKSize = { read=GetDSKSize };
  __property int TrackCount = { read=GetTrackCount };
  __property bool CAPSLibPresent = { read=GetCAPSLibPresent };
  //
  // API
  //
  __fastcall TIPFContainer();
  __fastcall ~TIPFContainer();

  Err __fastcall Load(AnsiString Filename);
  Err __fastcall Load(tUChar* FileContent, int iLength);
  tULong __fastcall BuildDSKStructure(tUChar* DSKData, tULong DSKSize);
  TIPFTrack* __fastcall DecodeImageRecord(tUChar* Data);
  TIPFTrack* __fastcall DecodeDataRecord(tUChar* Data);
  TIPFTrack* __fastcall GetTrack(tULong Cylinder, tULong Side);
};

//---------------------------------------------------------------------------
#endif

