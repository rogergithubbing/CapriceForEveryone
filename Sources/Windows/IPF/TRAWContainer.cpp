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


#include <vcl.h>
#include <tchar.h>
#pragma hdrstop
#include "TRAWContainer.h"
#include "..\..\Common\Native_CRC32.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


__fastcall TRAWContainer::TRAWContainer()
//
// Constructor
//
{
  mTracksList = new TList;
}
//---------------------------------------------------------------------------


__fastcall TRAWContainer::~TRAWContainer()
//
// Destructor
//
{
  while (mTracksList->Count > 0)
  {
    TRAWTrack* Item = (TRAWTrack*)mTracksList->Items[0];
    delete Item;
    mTracksList->Delete(0);
  }
  delete mTracksList;
}
//---------------------------------------------------------------------------


void __fastcall TRAWContainer::StrCopy(tChar* destP, tChar* srcP)
{
  while (*srcP != 0)
  {
    *(destP++) = *(srcP++);
  }
}
/*----------------------------------------------------------------------------*/


tULong __fastcall TRAWContainer::GetDSKSize(void)
{
  tULong DSKSize = sizeof(tDSKHeader);

  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TRAWTrack* Track = (TRAWTrack*)mTracksList->Items[Loop];
    DSKSize += Track->DSKHeaderSize + Track->DSKSize;
  }

  return DSKSize;
}
//---------------------------------------------------------------------------


TRAWTrack* __fastcall TRAWContainer::FindTrack(tUChar track, tUChar head)
{
  // Look for track
  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TRAWTrack* Track = (TRAWTrack*)mTracksList->Items[Loop];
    if ( (Track->TrackNumber == (tULong)track) && (Track->HeadNumber == (tULong)head) )
    {
      return Track;
    }
  }

  return NULL;
}
//---------------------------------------------------------------------------


Err __fastcall TRAWContainer::Load(AnsiString Filename)
{
  if (Filename.IsEmpty() == true)
    return errNone;

  if (FileExists(Filename) == false)
    return errRAWFileDoesNotExist;

  // Ouverture du fichier en lecture
  int iHandle = FileOpen(Filename, fmOpenRead);
  // Met le curseur à la fin du fichier pour récupérer la taille du fichier
  int iLength = FileSeek(iHandle, 0, 2);
  // Remet le curseur en début de fichier pour la prochaine lecture
  FileSeek(iHandle, 0, 0);

  // Read file
  tUChar* FileContent = (tUChar*)malloc(iLength);
  FileRead(iHandle, FileContent, iLength);
  FileClose(iHandle);

  // Load RAW
  Err Error = Load(FileContent, iLength);

  free(FileContent);

  return Error;
}
//---------------------------------------------------------------------------


Err __fastcall TRAWContainer::Load(tUChar* FileContent, int iLength)
{
  Err Error = errNone;

  do
  {
    tUChar* FileData = FileContent;
    tUChar* FileEnd = FileContent + iLength;

    // CAPS record should be first
    FileData = CheckCAPSRecord(FileData, &Error);
    if (Error != errNone)
    {
      continue;
    }

    while (FileData < FileEnd)
    {
      tRAWRecordHeader* Header = (tRAWRecordHeader*)FileData;

      // DATA Record
      if (*((tULong*)&Header->Identifier) == *((tULong*)&(RAW_DATA_HEADER_STRING)))
      {
        FileData = ReadDATARecord(FileData, &Error);
      }
      // TRCK Record
      else if (*((tULong*)&Header->Identifier) == *((tULong*)&(RAW_TRACK_HEADER_STRING)))
      {
        FileData = ReadTRCKRecord(FileData, &Error);
      }
    }

    //
    // Detect inner structure of each tracks
    //
    if (Error == errNone)
    {
      for (int Loop=0; Loop < mTracksList->Count; Loop++)
      {
        TRAWTrack* Track = (TRAWTrack*)mTracksList->Items[Loop];
        if (Track->DetectTrack() == errNone)
        {
          Track->BuildSectors();
        }
      }
    }
  }
  while (0);

  return Error;
}
//---------------------------------------------------------------------------


tUChar* _fastcall TRAWContainer::CheckCAPSRecord(tUChar* Data, Err* Error)
{
  do
  {
    tRAWRecordHeader* Header = (tRAWRecordHeader*)Data;

    // Bad header identifier ?
    if (*((tULong*)&Header->Identifier) != *((tULong*)&(RAW_CAPS_HEADER_STRING)))
    {
      *Error = errRAWBadStructure;
      continue;
    }

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != 12)
    {
      *Error = errRAWBadRecord;
      continue;
    }

    // Compute Record CRC
    tULong RecordCrc32 = ENDIAN_SWAP_32(Header->RecordCrc32);
    // To compute CRC32, use Header crc as 0 !!
    Header->RecordCrc32 = 0;
    tULong ComputedCrc32 = CRC32(Data, RecordSize);
    if (RecordCrc32 != ComputedCrc32)
    {
      *Error = errRAWBadRecord;
      continue;
    }

    // Record is correct.
    Data += RecordSize;
    *Error = errNone;
  }
  while (0);

  return Data;
}
//---------------------------------------------------------------------------


tUChar* _fastcall TRAWContainer::ReadDATARecord(tUChar* Data, Err* Error)
{
  do
  {
    tRAWRecordHeader* Header = (tRAWRecordHeader*)Data;

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != RAW_DATA_RECORD_SIZE)
    {
      *Error = errRAWBadRecord;
      continue;
    }

    tRAWDataBlock* Record = (tRAWDataBlock*)(Data + RAW_RECORD_HEADER_SIZE);
    tULong DataSize = ENDIAN_SWAP_32(Record->data_size);
    tULong ID = ENDIAN_SWAP_32(Record->id);

    TRAWTrack* FoundTrack = NULL;

    // Look for already created track
    for (int Loop=0; Loop < mTracksList->Count; Loop++)
    {
      TRAWTrack* Track = (TRAWTrack*)mTracksList->Items[Loop];
      if (Track->ID == ID)
      {
        FoundTrack = Track;
        break;
      }
    }
    // Create new track
    if (FoundTrack == NULL)
    {
      FoundTrack = new TRAWTrack();
      mTracksList->Add(FoundTrack);
    }

    // Load Track
    *Error = FoundTrack->DecodeFromStream(Data, DataSize+RecordSize);

    Data += DataSize + RecordSize;
  }
  while (0);

  return Data;
}
//---------------------------------------------------------------------------

tUChar* _fastcall TRAWContainer::ReadTRCKRecord(tUChar* Data, Err* Error)
{
  do
  {
    tRAWRecordHeader* Header = (tRAWRecordHeader*)Data;

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != RAW_TRACK_RECORD_SIZE)
    {
      *Error = errRAWBadRecord;
      continue;
    }

    tRAWTrackBlock* Record = (tRAWTrackBlock*)(Data + RAW_RECORD_HEADER_SIZE);
    tULong ID = ENDIAN_SWAP_32(Record->id);

    TRAWTrack* FoundTrack = NULL;

    // Look for already created track
    for (int Loop=0; Loop < mTracksList->Count; Loop++)
    {
      TRAWTrack* Track = (TRAWTrack*)mTracksList->Items[Loop];
      if (Track->ID == ID)
      {
        FoundTrack = Track;
        break;
      }
    }
    // Create new track
    if (FoundTrack == NULL)
    {
      FoundTrack = new TRAWTrack();
      mTracksList->Add(FoundTrack);
    }

    FoundTrack->TrackNumber = ENDIAN_SWAP_32(Record->track_number);
    FoundTrack->HeadNumber = ENDIAN_SWAP_32(Record->head_number);

    Data += RecordSize;
  }
  while (0);

  return Data;
}
//---------------------------------------------------------------------------


tULong __fastcall TRAWContainer::BuildDSKStructure(tUChar* DSKData, tULong DSKSize)
{
  //
  // Get size of sides
  //
  int NbSides = 0;
  int NbTracks = 0;
  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TRAWTrack* Track = (TRAWTrack*)mTracksList->Items[Loop];
    if ((Track->HeadNumber+1) > (tULong)NbSides)
    {
      NbSides = Track->HeadNumber+1;
    }
    if ((Track->TrackNumber+1) > (tULong)NbTracks)
    {
      NbTracks = Track->TrackNumber+1;
    }
  }
  int* SideSizes = new int[NbSides];
  for (int Loop=0; Loop < NbSides; Loop++)
  {
    SideSizes[Loop] = 0;
  }
  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TRAWTrack* Track = (TRAWTrack*)mTracksList->Items[Loop];
    SideSizes[Track->HeadNumber] += Track->TrackSize;
  }

  //
  // Prepare DSK Header
  //
  tDSKHeader* dhP = (tDSKHeader*)DSKData;
  memset((tUChar*)dhP,
         DSK_HEADER_SIZE,
         0);
  StrCopy(dhP->id,
          (tChar*)"EXTENDED CPC DSK File\r\nDisk-Info\r\n");
  StrCopy(dhP->creator,
          (tChar*)"CaPriCe 4Ever");
  dhP->tracks = NbTracks;
  dhP->sides = 0;
  for (int Loop=0; Loop < NbSides; Loop++)
  {
    if (SideSizes[Loop]) dhP->sides++;
  }

  delete [] SideSizes;

  //
  // Fill DSK header track size data
  //
  NbTracks = 0;
  for (tUChar TrackLoop=0; TrackLoop < dhP->tracks; TrackLoop++)
  {
    for (tUChar SideLoop=0; SideLoop < dhP->sides; SideLoop++)
    {
      // Not formatted track
      dhP->track_size[NbTracks] = 0; // Not formatted

      // Store track size
      TRAWTrack* Track = FindTrack(TrackLoop, SideLoop);
      if (Track)
      {
        if (Track->DSKSize)
        {
          dhP->track_size[NbTracks] = (Track->DSKHeaderSize + Track->DSKSize) >> 8; // track size + header in bytes
        }
      }

      NbTracks++;
    }
  }

  //
  // Fill track data
  //
  tUChar* CurrentData = DSKData + DSK_HEADER_SIZE;
  for (tUChar TrackLoop=0; TrackLoop < dhP->tracks; TrackLoop++)
  {
    for (tUChar SideLoop=0; SideLoop < dhP->sides; SideLoop++)
    {
      TRAWTrack* Track = FindTrack(TrackLoop, SideLoop);

      // Track found
      if ( (Track) && (Track->DSKSize) )
      {
        tTrackHeader* thP = (tTrackHeader*)CurrentData;
        memset((tUChar*)thP,
               (Track) ? Track->DSKHeaderSize : DSK_TRACKHEADER_SIZE,
               0);
        StrCopy(thP->id,
                (tChar*)"Track-Info\r\n");

        thP->track = TrackLoop;
        thP->side = SideLoop;
        thP->bps = 2;
        thP->sectors = 0;
        thP->gap3_length = 0;
        thP->filler = 0xe5;

        thP->sectors = (tUChar)Track->NbSectors;
        thP->gap3_length = Track->GAP3Length;

        // Prepare track header
        tUChar* SectorData = CurrentData + Track->DSKHeaderSize;
        for (tUChar SectorLoop=0; SectorLoop < thP->sectors; SectorLoop++)
        {
          tSector* Sector = Track->Sector[SectorLoop];

          // Copy CHRN and flags
          memcpy(&thP->sector[SectorLoop][0],
                 Sector->CHRN,
                 4); // copy CHRN
          memcpy(&thP->sector[SectorLoop][4],
                 Sector->flags,
                 2); // copy ST1 & ST2

          if (Sector->weak_count == 1)
          {
            tULong SectorSize = Sector->size;
            if (Sector->gap3) SectorSize += Sector->gap3_length;
            thP->sector[SectorLoop][6] = SectorSize & 0xff;
            thP->sector[SectorLoop][7] = (SectorSize >> 8) & 0xff; // sector size in bytes

            // Copy sector data
            Track->CopyData(SectorData, Sector->data, Sector->size);
            SectorData += Sector->size;
          }
          else
          {
            tULong SectorSize = Sector->declared_size * Sector->weak_count;
            if (Sector->gap3) SectorSize += Sector->gap3_length;
            thP->sector[SectorLoop][6] = SectorSize & 0xff;
            thP->sector[SectorLoop][7] = (SectorSize >> 8) & 0xff; // sector size in bytes

            // Copy Weak sector data
            //Track->CopyFuzzyData(SectorData, Sector);
            SectorData += Sector->declared_size * Sector->weak_count;
          }

          // Copy GAP3
          if (Sector->gap3)
          {
            Track->CopyData(SectorData, Sector->gap3, Sector->gap3_length);
            SectorData += Sector->gap3_length;
          }
        }

        // Next track
        CurrentData += Track->DSKHeaderSize + Track->DSKSize;
      }
    }
  }

  return DSKSize;
}
//---------------------------------------------------------------------------

