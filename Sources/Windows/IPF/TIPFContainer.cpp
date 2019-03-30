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
#include <tchar.h>
#pragma hdrstop
#include "TIPFContainer.h"
#include "..\..\Common\Native_CRC32.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


__fastcall TIPFContainer::TIPFContainer()
//
// Constructor
//
{
  mTracksList = new TList;
  mCAPSLibPresent = false;

#ifdef USE_CAPSLIB

  mCAPSLibFilename = ExtractFilePath(Application->ExeName) + "CAPSImg.dll";
  if (FileExists(mCAPSLibFilename) == true)
  {
    do
    {
      // Load library
      mCAPSLibHandle = LoadLibrary(mCAPSLibFilename.c_str());
      if (mCAPSLibHandle == NULL) continue;

      // Get function pointers
      CAPSInit = (CAPSInitPtr)GetProcAddress(mCAPSLibHandle, "CAPSInit");
      if (CAPSInit == NULL) continue;
      CAPSExit = (CAPSExitPtr)GetProcAddress(mCAPSLibHandle, "CAPSExit");
      if (CAPSExit == NULL) continue;
      CAPSAddImage = (CAPSAddImagePtr)GetProcAddress(mCAPSLibHandle, "CAPSAddImage");
      if (CAPSAddImage == NULL) continue;
      CAPSLockImageMemory = (CAPSLockImageMemoryPtr)GetProcAddress(mCAPSLibHandle, "CAPSLockImageMemory");
      if (CAPSLockImageMemory == NULL) continue;
      CAPSLoadImage = (CAPSLoadImagePtr)GetProcAddress(mCAPSLibHandle, "CAPSLoadImage");
      if (CAPSLoadImage == NULL) continue;
      CAPSGetImageInfo = (CAPSGetImageInfoPtr)GetProcAddress(mCAPSLibHandle, "CAPSGetImageInfo");
      if (CAPSGetImageInfo == NULL) continue;
      CAPSLockTrack = (CAPSLockTrackPtr)GetProcAddress(mCAPSLibHandle, "CAPSLockTrack");
      if (CAPSLockTrack == NULL) continue;
      CAPSUnlockTrack = (CAPSUnlockTrackPtr)GetProcAddress(mCAPSLibHandle, "CAPSUnlockTrack");
      if (CAPSUnlockTrack == NULL) continue;
      CAPSUnlockAllTracks = (CAPSUnlockAllTracksPtr)GetProcAddress(mCAPSLibHandle, "CAPSUnlockAllTracks");
      if (CAPSUnlockAllTracks == NULL) continue;
      CAPSUnlockImage = (CAPSUnlockImagePtr)GetProcAddress(mCAPSLibHandle, "CAPSUnlockImage");
      if (CAPSUnlockImage == NULL) continue;
      CAPSRemImage = (CAPSRemImagePtr)GetProcAddress(mCAPSLibHandle, "CAPSRemImage");
      if (CAPSRemImage == NULL) continue;

      // Init CAPS
      if (CAPSInit() != imgeOk) continue;

      mCAPSLibPresent = true;
    }
    while (0);

    // Error ?
    if (mCAPSLibPresent == false)
    {
      if (mCAPSLibHandle != NULL)
      {
        FreeLibrary(mCAPSLibHandle);
        mCAPSLibHandle = NULL;
      }
    }
  }

#endif /* USE_CAPSLIB */
}
//---------------------------------------------------------------------------


__fastcall TIPFContainer::~TIPFContainer()
//
// Destructor
//
{
  while (mTracksList->Count > 0)
  {
    TIPFTrack* Item = (TIPFTrack*)mTracksList->Items[0];
    delete Item;
    mTracksList->Delete(0);
  }
  delete mTracksList;

#ifdef USE_CAPSLIB
  if (mCAPSLibPresent == true)
  {
    CAPSExit();
    FreeLibrary(mCAPSLibHandle);
  }
#endif /* USE_CAPSLIB */
}
//---------------------------------------------------------------------------


int __fastcall TIPFContainer::GetTrackCount(void)
{
  return mTracksList->Count;
}
//---------------------------------------------------------------------------


Err __fastcall TIPFContainer::Load(AnsiString Filename)
{
  if (Filename.IsEmpty() == true)
    return errNone;

  if (FileExists(Filename) == false)
    return errIPFFileDoesNotExist;

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

  // Load IPF
  Err Error = Load(FileContent, iLength);

  free(FileContent);

  return Error;    
}
//---------------------------------------------------------------------------


Err __fastcall TIPFContainer::Load(tUChar* FileContent, int iLength)
{
  Err Error = errNone;

#ifdef USE_CAPSLIB
  if (mCAPSLibPresent == false)
#endif /* USE_CAPSLIB */
  {
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

      // INFO record should be in second place
      FileData = ReadINFORecord(FileData, &Error);
      if (Error != errNone)
      {
        continue;
      }

      // Check Amstrad CPC file
      bool AmstradFound = false;
      for (int Loop=0; Loop < IPF_NB_PLATFORMS; Loop++)
      {
        if (mInfoBlock.platforms[Loop] == IPF_PLATFORM_AMSTRAD_CPC)
        {
          AmstradFound = true;
          break;
        }
      }
      if (AmstradFound == false)
      {
        Error = errIPFBadPlatform;
        continue;
      }

      while (FileData < FileEnd)
      {
        tIPFRecordHeader* Header = (tIPFRecordHeader*)FileData;

        // Image Record
        if (*((tULong*)&Header->Identifier) == *((tULong*)&(IPF_IMAGE_HEADER_STRING)))
        {
          FileData = ReadImageRecord(FileData, &Error);
        }
        // Data Record
        else if (*((tULong*)&Header->Identifier) == *((tULong*)&(IPF_DATA_HEADER_STRING)))
        {
          FileData = ReadDataRecord(FileData, &Error);
        }
        // Unknown record, got to next
        else
        {
          int RecordLength = ENDIAN_SWAP_32(Header->RecordSize);
          FileData += RecordLength;
        }

        if (Error != errNone)
        {
          break;
        }
      }
    }
    while (0);

    //
    // Detect inner structure of each tracks
    //
    if (Error == errNone)
    {
      for (int Loop=0; Loop < mTracksList->Count; Loop++)
      {
        TIPFTrack* Track = (TIPFTrack*)mTracksList->Items[Loop];
        Track->BuildTrack();
        Track->BuildSectors();
      }
    }
  }

#ifdef USE_CAPSLIB
  else
  {
    SDWORD CAPSLibsID;

    do
    {
      CAPSLibsID = CAPSAddImage();
      if (CAPSLibsID < 0)
      {
        Error = errIPFBadMemoryAllocation;
        continue;
      }

      SDWORD CAPSError = CAPSLockImageMemory(CAPSLibsID,
                                             FileContent,
                                             iLength,
                                             DI_LOCK_MEMREF);
      if (CAPSError != imgeOk)
      {
        Error = errIPFCAPSBaseError + CAPSError;
        continue;
      }

      CAPSError = CAPSLoadImage(CAPSLibsID, DI_LOCK_ALIGN | DI_LOCK_INDEX);
      if (CAPSError != imgeOk)
      {
        Error = errIPFCAPSBaseError + CAPSError;
        continue;
      }

      // Get structure of IPF
      CapsImageInfo ImageInfo;
      CAPSError = CAPSGetImageInfo(&ImageInfo,CAPSLibsID);
      if (CAPSError != imgeOk)
      {
        Error = errIPFCAPSBaseError + CAPSError;
        continue;
      }

      // Populate InfoBlock
      mInfoBlock.minTrack = ImageInfo.mincylinder;
      mInfoBlock.maxTrack = ImageInfo.maxcylinder;
      mInfoBlock.minSide = ImageInfo.minhead;
      mInfoBlock.maxSide = ImageInfo.maxhead;

      // Decode all tracks
      for (UDWORD LoopCylinder=ImageInfo.mincylinder;
           LoopCylinder <= ImageInfo.maxcylinder;
           LoopCylinder++)
      {
        for (UDWORD LoopHead=ImageInfo.minhead;
             LoopHead <= ImageInfo.maxhead;
             LoopHead++)
        {
          CapsTrackInfo TrackInfo;
          if (CAPSLockTrack(&TrackInfo,
                            CAPSLibsID,
                            LoopCylinder,
                            LoopHead,
                            DI_LOCK_ALIGN) == imgeOk)
          {
            // Populate Track Info
            tIPFTrackBlock* NewTrackBlock = new tIPFTrackBlock;
            NewTrackBlock->track = TrackInfo.cylinder;
            NewTrackBlock->side = TrackInfo.head;
            // Populate Track
            TIPFTrack* NewTrack = new TIPFTrack;
            NewTrack->TrackBlock = NewTrackBlock;
            if (TrackInfo.tracksize[0] > 0)
            {
              NewTrack->BuildSPSTrack(TrackInfo.trackdata[0], TrackInfo.tracksize[0]);
            }
            mTracksList->Add(NewTrack);

            CAPSUnlockTrack(CAPSLibsID, LoopCylinder, LoopHead);
          }
        }
      }

      // Detect inner structure of each tracks
      for (int Loop=0; Loop < mTracksList->Count; Loop++)
      {
        TIPFTrack* Track = (TIPFTrack*)mTracksList->Items[Loop];
        Track->BuildSectors();
      }
    }
    while (0);

    // Release resources
    if (CAPSLibsID >= 0)
    {
      CAPSUnlockAllTracks(CAPSLibsID);
      CAPSUnlockImage(CAPSLibsID);
      CAPSRemImage(CAPSLibsID);
    }
  }

#endif /* USE_CAPSLIB */

  return Error;
}
//---------------------------------------------------------------------------


tUChar* _fastcall TIPFContainer::CheckCAPSRecord(tUChar* Data, Err* Error)
{
  do
  {
    tIPFRecordHeader* Header = (tIPFRecordHeader*)Data;

    // Bad header identifier ?
    if (*((tULong*)&Header->Identifier) != *((tULong*)&(IPF_CAPS_HEADER_STRING)))
    {
      *Error = errIPFBadStructure;
      continue;
    }

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != 12)
    {
      *Error = errIPFBadRecord;
      continue;
    }

    // Compute Record CRC
    tULong RecordCrc32 = ENDIAN_SWAP_32(Header->RecordCrc32);
    // To compute CRC32, use Header crc as 0 !!
    Header->RecordCrc32 = 0;
    tULong ComputedCrc32 = CRC32(Data, RecordSize);
    if (RecordCrc32 != ComputedCrc32)
    {
      *Error = errIPFBadRecord;
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


tUChar* _fastcall TIPFContainer::ReadINFORecord(tUChar* Data, Err* Error)
{
  do
  {
    tIPFRecordHeader* Header = (tIPFRecordHeader*)Data;

    // Bad header identifier ?
    if (*((tULong*)&Header->Identifier) != *((tULong*)&(IPF_INFO_HEADER_STRING)))
    {
      *Error = errIPFBadStructure;
      continue;
    }

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != 96)
    {
      *Error = errIPFBadRecord;
      continue;
    }

    // Compute Record CRC
    tULong RecordCrc32 = ENDIAN_SWAP_32(Header->RecordCrc32);
    // To compute CRC32, use Header crc as 0 !!
    Header->RecordCrc32 = 0;
    tULong ComputedCrc32 = CRC32(Data, RecordSize);
    if (RecordCrc32 != ComputedCrc32)
    {
      *Error = errIPFBadRecord;
      continue;
    }

    //
    // Read INFO
    //
    tIPFInfoBlock* Info = (tIPFInfoBlock*)(Data + sizeof(tIPFRecordHeader));
    mInfoBlock.mediaType = ENDIAN_SWAP_32(Info->mediaType);
    mInfoBlock.encoderType = ENDIAN_SWAP_32(Info->encoderType);
    mInfoBlock.encoderRev = ENDIAN_SWAP_32(Info->encoderRev);
    mInfoBlock.fileKey = ENDIAN_SWAP_32(Info->fileKey);
    mInfoBlock.fileRev = ENDIAN_SWAP_32(Info->fileRev);
    mInfoBlock.origin = ENDIAN_SWAP_32(Info->origin);
    mInfoBlock.minTrack = ENDIAN_SWAP_32(Info->minTrack);
    mInfoBlock.maxTrack = ENDIAN_SWAP_32(Info->maxTrack);
    mInfoBlock.minSide = ENDIAN_SWAP_32(Info->minSide);
    mInfoBlock.maxSide = ENDIAN_SWAP_32(Info->maxSide);
    mInfoBlock.creationDate = ENDIAN_SWAP_32(Info->creationDate);
    mInfoBlock.creationTime = ENDIAN_SWAP_32(Info->creationTime);
    for (int Loop=0; Loop < IPF_NB_PLATFORMS; Loop++)
    {
      mInfoBlock.platforms[Loop] = ENDIAN_SWAP_32(Info->platforms[Loop]);
    }
    mInfoBlock.diskNumber = ENDIAN_SWAP_32(Info->diskNumber);
    mInfoBlock.creatorId = ENDIAN_SWAP_32(Info->creatorId);

    // Record is correct.
    Data += RecordSize;
    *Error = errNone;
  }
  while (0);

  return Data;
}
//---------------------------------------------------------------------------


tUChar* _fastcall TIPFContainer::ReadImageRecord(tUChar* Data, Err* Error)
{
  do
  {
    tIPFRecordHeader* Header = (tIPFRecordHeader*)Data;

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != 80)
    {
      *Error = errIPFBadRecord;
      continue;
    }

    // Compute Record CRC
    // To compute CRC32, use Header crc as 0 !!
    tULong OldCrc = Header->RecordCrc32;
    Header->RecordCrc32 = 0;
    tULong ComputedCrc32 = CRC32(Data, RecordSize);
    Header->RecordCrc32 = OldCrc;
    // Bad CRC ?
    if (ComputedCrc32 != ENDIAN_SWAP_32(Header->RecordCrc32))
    {
      *Error = errIPFBadRecord;
      continue;
    }

    //
    // Create new track
    //
    tIPFTrackBlock* Block = (tIPFTrackBlock*)(Data + sizeof(tIPFRecordHeader));
    tIPFTrackBlock* NewTrackBlock = new tIPFTrackBlock;
    NewTrackBlock->track = ENDIAN_SWAP_32(Block->track);
    NewTrackBlock->side = ENDIAN_SWAP_32(Block->side);
    NewTrackBlock->density = ENDIAN_SWAP_32(Block->density);
    NewTrackBlock->signalType = ENDIAN_SWAP_32(Block->signalType);
    NewTrackBlock->trackBytes = ENDIAN_SWAP_32(Block->trackBytes);
    NewTrackBlock->startBytePos = ENDIAN_SWAP_32(Block->startBytePos);
    NewTrackBlock->startBitPos = ENDIAN_SWAP_32(Block->startBitPos);
    NewTrackBlock->dataBits = ENDIAN_SWAP_32(Block->dataBits);
    NewTrackBlock->gapBits = ENDIAN_SWAP_32(Block->gapBits);
    NewTrackBlock->trackBits = ENDIAN_SWAP_32(Block->trackBits);
    NewTrackBlock->blockCount = ENDIAN_SWAP_32(Block->blockCount);
    NewTrackBlock->encoderProcess = ENDIAN_SWAP_32(Block->encoderProcess);
    NewTrackBlock->flags.All = ENDIAN_SWAP_32(Block->flags.All);
    NewTrackBlock->dataKey = ENDIAN_SWAP_32(Block->dataKey);
    // Add track
    TIPFTrack* NewTrack = new TIPFTrack;
    NewTrack->TrackBlock = NewTrackBlock;
    NewTrack->EncoderType = mInfoBlock.encoderType;
    mTracksList->Add(NewTrack);
    
    // Record is correct.
    Data += RecordSize;
    *Error = errNone;
  }
  while (0);

  return Data;
}
//---------------------------------------------------------------------------


tUChar* _fastcall TIPFContainer::ReadDataRecord(tUChar* Data, Err* Error)
{
  do
  {
    tIPFRecordHeader* Header = (tIPFRecordHeader*)Data;

    // Bad record size
    tULong RecordSize = ENDIAN_SWAP_32(Header->RecordSize);
    if (RecordSize != 28)
    {
      *Error = errIPFBadRecord;
      continue;
    }

    // Compute Record CRC
    // To compute CRC32, use Header crc as 0 !!
    tULong OldCrc = Header->RecordCrc32;
    Header->RecordCrc32 = 0;
    tULong ComputedCrc32 = CRC32(Data, RecordSize);
    Header->RecordCrc32 = OldCrc;
    if (ComputedCrc32 != ENDIAN_SWAP_32(Header->RecordCrc32))
    {
      *Error = errIPFBadRecord;
      continue;
    }

    // Read Data Record
    tIPFDataBlock* Block = (tIPFDataBlock*)(Data + sizeof(tIPFRecordHeader));
    tULong length = ENDIAN_SWAP_32(Block->length);
    tULong dataKey = ENDIAN_SWAP_32(Block->dataKey);

    // Compute Extra data CRC
    tULong ExtraDataBlockCrc32 = ENDIAN_SWAP_32(Block->crc);
    ComputedCrc32 = CRC32(Data + RecordSize, length);
    if (ExtraDataBlockCrc32 != ComputedCrc32)
    {
      *Error = errIPFBadRecord;
      continue;
    }

    // Get associated track
    TIPFTrack* SelectedTrack = NULL;
    for (int Loop=0; Loop < mTracksList->Count; Loop++)
    {
      TIPFTrack* Track = (TIPFTrack*)mTracksList->Items[Loop];
      if (Track->DataKey == dataKey)
      {
        SelectedTrack = Track;
        break;
      }
    }
    // No track found
    if (SelectedTrack == NULL)
    {
      // Ignore record
      Data += RecordSize + length;
      *Error = errIPFBadStructure;
      continue;
    }

    //
    // Add Data Block Descriptors
    //
    tIPFDataBlockDescriptor* DataBlockDescriptor = (tIPFDataBlockDescriptor*)(Data + RecordSize);
    for (tULong BlockLoop=0; BlockLoop < SelectedTrack->BlockCount; BlockLoop++)
    {
      // Read data block descriptor
      tIPFDataBlockDescriptor* NewDescriptor = new tIPFDataBlockDescriptor;
      NewDescriptor->dataBits = ENDIAN_SWAP_32(DataBlockDescriptor->dataBits);
      NewDescriptor->gapBits = ENDIAN_SWAP_32(DataBlockDescriptor->gapBits);
      NewDescriptor->encoder.CAPS.dataBytes = ENDIAN_SWAP_32(DataBlockDescriptor->encoder.CAPS.dataBytes);
      NewDescriptor->encoder.CAPS.gapBytes = ENDIAN_SWAP_32(DataBlockDescriptor->encoder.CAPS.gapBytes);
      NewDescriptor->encoderType = ENDIAN_SWAP_32(DataBlockDescriptor->encoderType);
      NewDescriptor->flags.All = ENDIAN_SWAP_32(DataBlockDescriptor->flags.All);
      NewDescriptor->gapDefault = ENDIAN_SWAP_32(DataBlockDescriptor->gapDefault);
      NewDescriptor->dataOffset = ENDIAN_SWAP_32(DataBlockDescriptor->dataOffset);
      // Add data descriptor
      SelectedTrack->AddDataBlockDescriptor(NewDescriptor);
      // Next descriptor
      DataBlockDescriptor++;
    }

    //
    // Add Extra Data Block
    //
    tUChar* ExtraDataBlock = new tUChar[length];
    // Data copy
    memcpy(ExtraDataBlock, Data + RecordSize, length);
    SelectedTrack->AddExtraDataBlock(ExtraDataBlock);

    // Record is correct.
    Data += RecordSize + length;
    *Error = errNone;
  }
  while (0);

  return Data;
}
//---------------------------------------------------------------------------


tULong __fastcall TIPFContainer::GetDSKSize(void)
{
  tULong DSKSize = sizeof(tDSKHeader);

  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TIPFTrack* Track = (TIPFTrack*)mTracksList->Items[Loop];
    DSKSize += Track->DSKHeaderSize + Track->DSKSize;
  }

  return DSKSize;
}
//---------------------------------------------------------------------------


TIPFTrack* __fastcall TIPFContainer::FindTrack(tUChar Cylinder, tUChar side)
{
  // Look for track
  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TIPFTrack* Track = (TIPFTrack*)mTracksList->Items[Loop];
    if ( (Track->Cylinder == (tULong)Cylinder) && (Track->Side == (tULong)side) )
    {
      return Track;
    }
  }

  return NULL;
}
//---------------------------------------------------------------------------


tULong __fastcall TIPFContainer::BuildDSKStructure(tUChar* DSKData, tULong DSKSize)
{
  // Get size of sides
  int NbSides = mInfoBlock.maxSide - mInfoBlock.minSide + 1;
  int* SideSizes = new int[NbSides];
  for (int Loop=0; Loop < NbSides; Loop++)
  {
    SideSizes[Loop] = 0;
  }
  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TIPFTrack* Track = (TIPFTrack*)mTracksList->Items[Loop];
    SideSizes[Track->Side] += Track->Size;
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
  dhP->tracks = mInfoBlock.maxTrack - mInfoBlock.minTrack + 1;
  dhP->sides = 0;
  for (int Loop=0; Loop < NbSides; Loop++)
  {
    if (SideSizes[Loop]) dhP->sides++;
  }

  delete [] SideSizes;
  
  //
  // Fill DSK header track size data
  //
  int NbTracks = 0;
  for (tUChar TrackLoop=0; TrackLoop < dhP->tracks; TrackLoop++)
  {
    for (tUChar SideLoop=0; SideLoop < dhP->sides; SideLoop++)
    {
      // Not formatted track
      dhP->track_size[NbTracks] = 0; // Not formatted

      // Store track size
      TIPFTrack* Track = FindTrack(TrackLoop, SideLoop);
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
      TIPFTrack* Track = FindTrack(TrackLoop, SideLoop);

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
            Track->CopyFuzzyData(SectorData, Sector);
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


TIPFTrack* __fastcall TIPFContainer::DecodeImageRecord(tUChar* Data)
{
  // Container should be empty
  if (Data == NULL) return NULL;
  if (mTracksList->Count) return NULL;

  // Decode record
  Err Error = errNone;
  ReadImageRecord(Data, &Error);

  if (Error == errNone)
  {
    if (mTracksList->Count == 1)
      return (TIPFTrack*)mTracksList->Items[0];
  }

  return NULL;
}
//---------------------------------------------------------------------------

TIPFTrack* __fastcall TIPFContainer::DecodeDataRecord(tUChar* Data)
{
  // Decode record
  Err Error = errNone;
  ReadDataRecord(Data, &Error);

  if (Error == errNone)
  {
    if (mTracksList->Count == 1)
      return (TIPFTrack*)mTracksList->Items[0];
  }

  return NULL;
}
//---------------------------------------------------------------------------


void __fastcall TIPFContainer::StrCopy(tChar* destP, tChar* srcP)
{
  while (*srcP != 0)
  {
    *(destP++) = *(srcP++);
  }
}
/*----------------------------------------------------------------------------*/


TIPFTrack* __fastcall TIPFContainer::GetTrack(tULong Cylinder, tULong Side)
{
  // Decode record
  for (int Loop=0; Loop < mTracksList->Count; Loop++)
  {
    TIPFTrack* Track = (TIPFTrack*)mTracksList->Items[Loop];

    if ( (Track->Cylinder == Cylinder)
         && (Track->Side == Side) )
    {
      return Track;
    }
  }

  return NULL;
}
//---------------------------------------------------------------------------

