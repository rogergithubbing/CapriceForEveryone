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
#include "TZIPContainer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//=================================
// Options
//

//=================================



__fastcall TZIPContainer::TZIPContainer(AnsiString Filename)
//
// Constructor
//
{
  mFilenameList = new TStringList();

  mZipFile = unzOpen(Filename.c_str());

  //
  // Retrieve filename list
  //
  int err = unzGoToFirstFile(mZipFile);
  while (err == UNZ_OK)
  {
    // Get current file info
    char filename_inzip[256];
    unz_file_info file_info;
    err = unzGetCurrentFileInfo(mZipFile,
                                &file_info,
                                filename_inzip,
                                sizeof(filename_inzip),
                                NULL,
                                0,
                                NULL,
                                0);

    if (err == UNZ_OK)
    {
      if (file_info.uncompressed_size)
      {
        // Add filename
        mFilenameList->Add(AnsiString(filename_inzip));
      }
    }

    // Go to next file
    err = unzGoToNextFile(mZipFile);
  }

  // Sort filenames
  mFilenameList->Sort();
}
//---------------------------------------------------------------------------


__fastcall TZIPContainer::~TZIPContainer()
//
// Destructor
//
{
  unzClose(mZipFile);
}
//---------------------------------------------------------------------------


String __fastcall TZIPContainer::GetFile(int Index)
{
  if (Index <= mFilenameList->Count)
  {
    return mFilenameList->Strings[Index];
  }
  return "";
}
//---------------------------------------------------------------------------


int __fastcall TZIPContainer::GetFileUncompressedSize(AnsiString Filename)
{
  int err = unzLocateFile(mZipFile,
                      Filename.c_str(),
                      0);

  if (err != UNZ_OK)
  {
    return 0;
  }

  char filename_inzip[256];
  unz_file_info file_info;
  err = unzGetCurrentFileInfo(mZipFile,
                              &file_info,
                              filename_inzip,
                              sizeof(filename_inzip),
                              NULL,
                              0,
                              NULL,
                              0);

  if (err != UNZ_OK)
  {
    return 0;
  }

  return (file_info.uncompressed_size);
}
//---------------------------------------------------------------------------


int __fastcall TZIPContainer::ReadFile(AnsiString Filename,
                                       tUChar* Content,
                                       tULong Length)
{
  int err = unzLocateFile(mZipFile,
                      Filename.c_str(),
                      0);
  if (err != UNZ_OK)
  {
    return err;
  }

  err = unzOpenCurrentFile(mZipFile);
  if (err != UNZ_OK)
  {
    return err;
  }

  int Read = unzReadCurrentFile(mZipFile,
                                Content,
                                Length);

  if (Read != (int)Length)
  {
    err = UNZ_INTERNALERROR;
  }

  unzCloseCurrentFile(mZipFile);

  return err;
}
//---------------------------------------------------------------------------

