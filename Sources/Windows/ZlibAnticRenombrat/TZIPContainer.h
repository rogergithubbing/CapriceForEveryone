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


#ifndef TZIPContainerH
#define TZIPContainerH


#include "unzip.h"
#include "..\..\Engine\Native_CPC.h"


#define CONTAINER_ZIP_EXT           ".zip"


class TZIPContainer
{
private:
  //
  // Members
  //
  unzFile mZipFile;
  TStringList* mFilenameList;

  //
  // Methods
  //
  int __fastcall GetNbFiles(void) { return mFilenameList->Count; };
  String __fastcall GetFile(int Index);

public:
  //
  // Properties
  //
  __property int NbFiles = { read=GetNbFiles };
  __property String Files[int Index] = { read=GetFile };

  //
  // API
  //
  __fastcall TZIPContainer(AnsiString Filename);
  __fastcall ~TZIPContainer();

  int __fastcall GetFileUncompressedSize(AnsiString Filename);
  int __fastcall ReadFile(AnsiString Filename, tUChar* Content, tULong Length);
};

//---------------------------------------------------------------------------
#endif

