/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2017 by Frédéric Coste

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


#ifndef TRAWContainerH
#define TRAWContainerH


#include "..\..\Engine\Native_CPC.h"
#include "..\..\Common\RAW.h"
#include "TRAWTrack.h"


class TRAWContainer
{
private:
  //
  // Members
  //
  TList* mTracksList;

  //
  // Methods
  //
  tUChar* _fastcall CheckCAPSRecord(tUChar* Data, Err* Error);
  tUChar* _fastcall ReadDATARecord(tUChar* Data, Err* Error);
  tUChar* _fastcall ReadTRCKRecord(tUChar* Data, Err* Error);
	void __fastcall StrCopy(tChar* destP, tChar* srcP);
  tULong __fastcall GetDSKSize(void);
  TRAWTrack* __fastcall FindTrack(tUChar track, tUChar head);

public:
  //
  // Properties
  //
  __property tULong DSKSize = { read=GetDSKSize };

  //
  // API
  //
  __fastcall TRAWContainer();
  __fastcall ~TRAWContainer();

  Err __fastcall Load(AnsiString Filename);
  Err __fastcall Load(tUChar* FileContent, int iLength);
  tULong __fastcall BuildDSKStructure(tUChar* DSKData, tULong DSKSize);
};

//---------------------------------------------------------------------------
#endif

