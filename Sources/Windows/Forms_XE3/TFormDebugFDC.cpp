/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2015 by Frédéric Coste

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
#include <Clipbrd.hpp>
#pragma hdrstop
#include "TFormDebugFDC.h"
#include "TFormMain.h"


//=================================
// Options

//=================================


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

enum
{
  FDC_NB_TRACKS = 0,
  FDC_CURRENT_TRACK,
  FDC_NB_SECTORS,
  FDC_CURRENT_SECTOR_INDEX,
  FDC_CURRENT_SECTOR,
  // Should be the last
  FDC_NB_CURRENT_VALUES
};


#define ADD_HISTORY_ENTRY_WITH_PHASE(message) \
{ \
  AnsiString HistoryEntry = ""; \
  if (CheckBoxAddTimestamp->Checked == true) \
  { \
    HistoryEntry += AnsiString((FormMain->NativeCPC->timer_from_reset - FDC->motor_on_time) / 4); \
    HistoryEntry += "  "; \
  } \
  if (FDC->signal_phase == EXEC_PHASE) HistoryEntry += "EXEC ";\
  else if (FDC->signal_phase == CMD_PHASE) HistoryEntry += "CMD ";\
  else HistoryEntry += "RESULT ";\
  HistoryEntry += message;\
  ListBoxHistory->Items->Add(HistoryEntry);\
}

#define ADD_HISTORY_ENTRY(message) \
{ \
  AnsiString HistoryEntry = ""; \
  if (CheckBoxAddTimestamp->Checked == true) \
  { \
    HistoryEntry += AnsiString((FormMain->NativeCPC->timer_from_reset - FDC->motor_on_time) / 4); \
    HistoryEntry += "  "; \
  } \
  HistoryEntry += message;\
  ListBoxHistory->Items->Add(HistoryEntry);\
}


TFormDebugFDC *FormDebugFDC;
//---------------------------------------------------------------------------
__fastcall TFormDebugFDC::TFormDebugFDC(TComponent* Owner)
  : TForm(Owner)
{
  bHistoryEnabled = false;
  //ThousandSeparator = ' ';
  mSettingsP = NULL;

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  PanelFormatStateNbTracks->Font->Color = clBlack;
  PanelFormatStateCurrentTrack->Font->Color = clBlack;
  PanelFormatStateNbSectors->Font->Color = clBlack;
  PanelFormatStateCurrentSectorIndex->Font->Color = clBlack;
  PanelFormatStateCurrentSector->Font->Color = clBlack;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::FormShow(TObject *Sender)
{
	// Subscribe to events
	if (FormMain != NULL)
	{
		FormMain->SubscribeToCPCEvents(OnEmulatorEvent);
		FormMain->SubscribeToFDCEvents(OnFDCEvent);
		InitCurrentState(FormMain->NativeCPC);
		Caption = "FDC";
	}
	else
	{
		Close();
	}

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnEmulatorEvent);
    FormMain->UnsubscribeToFDCEvents(OnFDCEvent);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->FDCFormLeft != 0)
       || (mSettingsP->FDCFormTop != 0)
       || (mSettingsP->FDCFormWidth != 0)
       || (mSettingsP->FDCFormHeight != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->FDCFormLeft;
    Top = mSettingsP->FDCFormTop;
    Width = mSettingsP->FDCFormWidth;
    Height = mSettingsP->FDCFormHeight;
  }

  ShowHint = mSettingsP->ShowHints;

  if (mSettingsP)
  {
    // Apply theme
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
    PanelStateMotor->Font->Color = mSettingsP->ThemeText2Color;
    PanelStateNbTracks->Font->Color = mSettingsP->ThemeText2Color;
    PanelStateCurrentTrack->Font->Color = mSettingsP->ThemeText2Color;
    PanelStateNbSectors->Font->Color = mSettingsP->ThemeText2Color;
    PanelStateCurrentSectorIndex->Font->Color = mSettingsP->ThemeText2Color;
    PanelStateCurrentSector->Font->Color = mSettingsP->ThemeText2Color;
    ListBoxHistory->Font->Color = mSettingsP->ThemeText2Color;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::FormResize(TObject *Sender)
{
	if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->FDCFormWidth = Width;
    mSettingsP->FDCFormHeight = Height;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugFDC::WMMove(TMessage &Message)
{
	if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->FDCFormLeft = Left;
    mSettingsP->FDCFormTop = Top;
  }

  NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::ButtonHistoryClearClick(TObject *Sender)
{
  ListBoxHistory->Items->Clear();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::ButtonHistoryStartStopClick(TObject *Sender)
{
	if (ButtonHistoryStartStop->Caption == "Start")
  {
    bHistoryEnabled = true;
    ButtonHistoryStartStop->Caption = "Stop";
    Caption = "FDC - [Monitoring]";
    //FormMain->ResumeStepByStep(OnEmulatorEvent);
    FormMain->NativeCPC->fdc_event_capture = 1;
  }
  else
  {
    bHistoryEnabled = false;
    ButtonHistoryStartStop->Caption = "Start";
    Caption = "FDC";
    //FormMain->UnsubscribeToCPCEvents(OnEmulatorEvent);
    FormMain->NativeCPC->fdc_event_capture = 0;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::Copytoclipboard1Click(TObject *Sender)
{
  TStringList* pBuffer = new TStringList();

  for (int Index=0; Index < ListBoxHistory->Items->Count; Index++)
  {
    pBuffer->Add(ListBoxHistory->Items->Strings[Index]);
  }

  Clipboard()->AsText = pBuffer->Text;

  delete pBuffer;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::CopytoclipboardMenuClick(TObject *Sender)
{
  Copytoclipboard1Click(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::InitCurrentState(tNativeCPC* NativeCPC)
{
tFDC* FDC = NativeCPC->FDC;

  AnsiString NbTracks = "";
  AnsiString CurrentTrack = "";
  AnsiString NbSectors = "";
  AnsiString CurrentSectorIndex = "";
  AnsiString CurrentSector = "";

  if (FDC->active_drive != NULL)
  {
    // NbTracks
    if (PanelFormatStateNbTracks->Caption.IsEmpty() == true)
    {
      NbTracks = AnsiString(FDC->active_drive->tracks);
    }
    else
    {
      NbTracks = AnsiString::IntToHex(FDC->active_drive->tracks,2);
    }
    // CurrentTrack
    if (PanelFormatStateCurrentTrack->Caption.IsEmpty() == true)
    {
      CurrentTrack = AnsiString(FDC->active_drive->current_track);
    }
    else
    {
      CurrentTrack = AnsiString::IntToHex(FDC->active_drive->current_track, 2);
    }

    // CurrentSectorIndex
    if (PanelFormatStateCurrentSectorIndex->Caption.IsEmpty() == true)
    {
      CurrentSectorIndex = AnsiString(FDC->active_drive->current_sector);
    }
    else
    {
      CurrentSectorIndex = AnsiString::IntToHex(FDC->active_drive->current_sector, 2);
    }
  }

  if (FDC->active_track != NULL)
  {
    // NbSectors
    if (PanelFormatStateNbSectors->Caption.IsEmpty() == true)
    {
      NbSectors = AnsiString(FDC->active_track->sectors);
    }
    else
    {
      NbSectors = AnsiString::IntToHex(FDC->active_track->sectors, 2);
    }
  }

  if (FDC->active_sector != NULL)
  {
    // CurrentSector
    if (PanelFormatStateCurrentSector->Caption.IsEmpty() == true)
    {
      CurrentSector = AnsiString(FDC->active_sector->CHRN[SECTOR_CHRN_R]);
    }
    else
    {
      CurrentSector = AnsiString::IntToHex(FDC->active_sector->CHRN[SECTOR_CHRN_R], 2);
    }
  }

  PanelStateMotor->Caption = FDC->motor ? "ON" : "OFF";
  PanelStateNbTracks->Caption = NbTracks;
  PanelStateCurrentTrack->Caption = CurrentTrack;
  PanelStateNbSectors->Caption = NbSectors;
  PanelStateCurrentSectorIndex->Caption = CurrentSectorIndex;
  PanelStateCurrentSector->Caption = CurrentSector;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::PanelFormatStateNbTracksClick(
      TObject *Sender)
{
TPanel* Panel = (TPanel*)Sender;

  // Update Display
  if (Panel->Caption.IsEmpty() == true)
  {
    Panel->Caption = "&&";
    if (PanelStateNbTracks->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = PanelStateNbTracks->Caption.ToInt();
      PanelStateNbTracks->Caption = AnsiString::IntToHex(CurrentValue,2);
    }
  }
  else
  {
    Panel->Caption = "";
    if (PanelStateNbTracks->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = StrToInt("0x" + PanelStateNbTracks->Caption);
      PanelStateNbTracks->Caption = CurrentValue;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::PanelFormatStateCurrentTrackClick(
      TObject *Sender)
{
TPanel* Panel = (TPanel*)Sender;

  // Update Display
  if (Panel->Caption.IsEmpty() == true)
  {
    Panel->Caption = "&&";
    if (PanelStateCurrentTrack->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = PanelStateCurrentTrack->Caption.ToInt();
      PanelStateCurrentTrack->Caption = AnsiString::IntToHex(CurrentValue, 2);
    }
  }
  else
  {
    Panel->Caption = "";
    if (PanelStateCurrentTrack->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = StrToInt("0x" + PanelStateCurrentTrack->Caption);
      PanelStateCurrentTrack->Caption = CurrentValue;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::PanelFormatStateNbSectorsClick(
      TObject *Sender)
{
TPanel* Panel = (TPanel*)Sender;

  // Update Display
  if (Panel->Caption.IsEmpty() == true)
  {
    Panel->Caption = "&&";
    if (PanelStateNbSectors->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = PanelStateNbSectors->Caption.ToInt();
      PanelStateNbSectors->Caption = AnsiString::IntToHex(CurrentValue, 2);
    }
  }
  else
  {
    Panel->Caption = "";
    if (PanelStateNbSectors->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = StrToInt("0x" + PanelStateNbSectors->Caption);
      PanelStateNbSectors->Caption = CurrentValue;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::PanelFormatStateCurrentSectorIndexClick(
      TObject *Sender)
{
TPanel* Panel = (TPanel*)Sender;

  // Update Display
  if (Panel->Caption.IsEmpty() == true)
  {
    Panel->Caption = "&&";
    if (PanelStateCurrentSectorIndex->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = PanelStateCurrentSectorIndex->Caption.ToInt();
      PanelStateCurrentSectorIndex->Caption = AnsiString::IntToHex(CurrentValue, 2);
    }
  }
  else
  {
    Panel->Caption = "";
    if (PanelStateCurrentSectorIndex->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = StrToInt("0x" + PanelStateCurrentSectorIndex->Caption);
      PanelStateCurrentSectorIndex->Caption = CurrentValue;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::PanelFormatStateCurrentSectorClick(
      TObject *Sender)
{
TPanel* Panel = (TPanel*)Sender;

  // Update Display
  if (Panel->Caption.IsEmpty() == true)
  {
    Panel->Caption = "&&";
    if (PanelStateCurrentSector->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = PanelStateCurrentSector->Caption.ToInt();
      PanelStateCurrentSector->Caption = AnsiString::IntToHex(CurrentValue, 2);
    }
  }
  else
  {
    Panel->Caption = "";
    if (PanelStateCurrentSector->Caption.IsEmpty() == false)
    {
      tULong CurrentValue = StrToInt("0x" + PanelStateCurrentSector->Caption);
      PanelStateCurrentSector->Caption = CurrentValue;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugFDC::OnFDCEvent(tFDC* FDC)
{
  // Update motor state
  if (FDC->signal_motor_change)
  {
    PanelStateMotor->Caption = FDC->motor ? "ON" : "OFF";
  }

  // Update position
  if (FDC->signal_new_position)
  {
    AnsiString NbTracks = "";
    AnsiString CurrentTrack = "";
    AnsiString NbSectors = "";
    AnsiString CurrentSectorIndex = "";
    AnsiString CurrentSector = "";

    if (FDC->active_drive != NULL)
    {
      // NbTracks
      if (PanelFormatStateNbTracks->Caption.IsEmpty() == true)
      {
        NbTracks = AnsiString(FDC->active_drive->tracks);
      }
      else
      {
        NbTracks = AnsiString::IntToHex(FDC->active_drive->tracks,2);
      }
      // CurrentTrack
      if (PanelFormatStateCurrentTrack->Caption.IsEmpty() == true)
      {
        CurrentTrack = AnsiString(FDC->active_drive->current_track);
      }
      else
      {
        CurrentTrack = AnsiString::IntToHex(FDC->active_drive->current_track, 2);
      }

      // CurrentSectorIndex
      if (PanelFormatStateCurrentSectorIndex->Caption.IsEmpty() == true)
      {
        CurrentSectorIndex = AnsiString(FDC->active_drive->current_sector);
      }
      else
      {
        CurrentSectorIndex = AnsiString::IntToHex(FDC->active_drive->current_sector, 2);
      }
    }

    if (FDC->active_track != NULL)
    {
      // NbSectors
      if (PanelFormatStateNbSectors->Caption.IsEmpty() == true)
      {
        NbSectors = AnsiString(FDC->active_track->sectors);
      }
      else
      {
        NbSectors = AnsiString::IntToHex(FDC->active_track->sectors, 2);
      }
    }

    if (FDC->active_sector != NULL)
    {
      // CurrentSector
      if (PanelFormatStateCurrentSector->Caption.IsEmpty() == true)
      {
        CurrentSector = AnsiString(FDC->active_sector->CHRN[SECTOR_CHRN_R]);
      }
      else
      {
        CurrentSector = AnsiString::IntToHex(FDC->active_sector->CHRN[SECTOR_CHRN_R], 2);
      }
    }

    PanelStateNbTracks->Caption = NbTracks;
    PanelStateCurrentTrack->Caption = CurrentTrack;
    PanelStateNbSectors->Caption = NbSectors;
    PanelStateCurrentSectorIndex->Caption = CurrentSectorIndex;
    PanelStateCurrentSector->Caption = CurrentSector;
  }
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugFDC::OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  if ((Condition & EC_FDC_EVENT) == 0)
    return Pause;

  tFDC* FDC = NativeCPC->FDC;

  bool HistoryEntryAdded = false;
  unsigned int iCurrentReadTimestamp;

  // Show read status result
  if (FDC->signal_read_status)
  {
    if (CheckBoxShowReadStatus->Checked == true)
    {
      AnsiString Entry = "STATUS=(" + AnsiString::IntToHex(FDC->read_status_value, 2) + ") ";
      if (FDC->read_status_value & MSR_RQM) Entry += "RQM ";
      if (FDC->read_status_value & MSR_DIO) Entry += "DIO ";
      if (FDC->read_status_value & MSR_EXM) Entry += "EXM ";
      if (FDC->read_status_value & MSR_CB)  Entry += "CB ";
      if (FDC->read_status_value & MSR_D3B) Entry += "D3B ";
      if (FDC->read_status_value & MSR_D2B) Entry += "D2B ";
      if (FDC->read_status_value & MSR_D1B) Entry += "D1B ";
      if (FDC->read_status_value & MSR_D0B) Entry += "D0B ";
      ADD_HISTORY_ENTRY_WITH_PHASE(Entry);
      HistoryEntryAdded = true;
    }
  }

  // Show read data value
  if (FDC->signal_read_data)
  {
    if (CheckBoxShowReadData->Checked == true)
    {
      AnsiString Entry = "READ=" + AnsiString::IntToHex(FDC->signal_data, 2);
      iCurrentReadTimestamp = NativeCPC->timer_from_reset / 4;
      if ( (iLastReadTimestamp) && (CheckBoxAddInterval->Checked == true) )
      {
        Entry += " (+";
        Entry += iCurrentReadTimestamp - iLastReadTimestamp;
        Entry += ")";
      }
      iLastReadTimestamp = iCurrentReadTimestamp;
      ADD_HISTORY_ENTRY_WITH_PHASE(Entry);
      HistoryEntryAdded = true;
    }
  }

  // Show write data value
  if (FDC->signal_write_data)
  {
    if (CheckBoxShowWriteData->Checked == true)
    {
      AnsiString Entry = "WRITE=" + AnsiString::IntToHex(FDC->signal_data, 2);
      ADD_HISTORY_ENTRY_WITH_PHASE(Entry);
      HistoryEntryAdded = true;
    }
  }

  // Show new command
  if (FDC->signal_new_command)
  {
    AnsiString Entry = "Command=";
    if (FDC->cmd_index != MAX_CMD_COUNT)
    {
      Entry += AnsiString((char*)FDC->FDCCommandTable[FDC->cmd_index].label) + " : ";

      // Add command parameters
      for (tULong Index=0; Index < FDC->cmd_length; Index++)
      {
        Entry += AnsiString::IntToHex(FDC->command[Index], 2) + " ";
      }
    }
    else
    {
      Entry += "!UNKNOWN : &";
      Entry += AnsiString::IntToHex(FDC->command[0], 2);
    }
    ADD_HISTORY_ENTRY_WITH_PHASE(Entry);
    HistoryEntryAdded = true;

    iLastReadTimestamp = 0;
  }

  // Show command result
  if (FDC->signal_command_result)
  {
    // Add command results
    AnsiString Entry = "Result=";
    for (tULong Index=0; Index < FDC->res_length; Index++)
    {
      Entry += AnsiString::IntToHex(FDC->result[Index], 2) + " ";
    }
    ADD_HISTORY_ENTRY_WITH_PHASE(Entry);
    HistoryEntryAdded = true;

    iLastReadTimestamp = 0;
  }

  // Show motor change
  if (FDC->signal_motor_change)
  {
    // Add command results
    AnsiString Entry = "MOTOR=";
    Entry += (FDC->motor == 1) ? "ON" : "OFF";
    ADD_HISTORY_ENTRY(Entry);
    HistoryEntryAdded = true;
  }

  // Show Disk Index
  if (FDC->signal_index)
  {
    if (CheckBoxShowDiskIndex->Checked == true)
    {
      AnsiString Entry = "DISK INDEX";
      ADD_HISTORY_ENTRY(Entry);
      HistoryEntryAdded = true;
    }
  }

  // Show Disk Track
  if (FDC->signal_track)
  {
    if (CheckBoxShowDiskTrack->Checked == true)
    {
      tTrack* track = FDC->active_track;

      AnsiString Entry = "DISK TRACK=&" + AnsiString::IntToHex(FDC->active_drive->current_track, 2);
      Entry += ",Sectors=" + AnsiString(track->sectors);
      Entry += ",Size=" + AnsiString(track->size);
      Entry += ",Gap3=" + AnsiString(track->gap3_length);
      ADD_HISTORY_ENTRY(Entry);
      HistoryEntryAdded = true;
    }
  }

  // Show Disk Sector
  if (FDC->signal_sector)
  {
    if (CheckBoxShowDiskSector->Checked == true)
    {
      tSector* sector = &FDC->active_track->sector[FDC->active_drive->current_sector];

      AnsiString Entry = "DISK SECTOR:";
      Entry += "CHRN=&" + AnsiString::IntToHex(sector->CHRN[SECTOR_CHRN_C], 2);
      Entry += ",&" + AnsiString::IntToHex(sector->CHRN[SECTOR_CHRN_H], 2);
      Entry += ",&" + AnsiString::IntToHex(sector->CHRN[SECTOR_CHRN_R], 2);
      Entry += ",&" + AnsiString::IntToHex(sector->CHRN[SECTOR_CHRN_N], 2);
      Entry += ",Size=" + AnsiString(sector->data_size);
      ADD_HISTORY_ENTRY(Entry);
      HistoryEntryAdded = true;
    }
  }

  if (HistoryEntryAdded == true)
  {
    // Show last entry
    ListBoxHistory->TopIndex = ListBoxHistory->Items->Count - 1;
  }

  return Pause;
}
//---------------------------------------------------------------------------


