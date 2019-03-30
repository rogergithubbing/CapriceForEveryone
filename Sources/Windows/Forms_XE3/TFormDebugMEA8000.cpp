/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2016 by Frédéric Coste

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

#include "TFormDebugMEA8000.h"
#include "TFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


const AnsiString StateString[] =
{
  "STOPPED",
  "WAIT_FIRST",
  "STARTED",
  "SLOWING"
};

static const AnsiString cTitle = "Techni-Musique";

#define ADD_HISTORY_ENTRY(message) \
{ \
  AnsiString HistoryEntry = AnsiString(FormMain->NativeCPC->timer_from_reset / 4); \
  HistoryEntry += "  "; \
  HistoryEntry += message;\
  ListBoxHistory->Items->Add(HistoryEntry);\
}


TFormDebugMEA8000 *FormDebugMEA8000;
//---------------------------------------------------------------------------
__fastcall TFormDebugMEA8000::TFormDebugMEA8000(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mAudioContext = NULL;

#ifndef _DEBUG
  GroupBoxMonitoring->Visible = false;
  BorderStyle = bsSingle;
  Constraints->MinWidth = 0;
  Width = 239;
  Height = 213;
#endif /* _DEBUG */

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMEA8000::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
  }

  mSampleCount = 0;
  mMonitoring = false;

  Caption = cTitle;
  InitCurrentState(FormMain->NativeCPC);

  FormMain->SubscribeToCPCEvents(OnEmulatorEvent);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMEA8000::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnEmulatorEvent);
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMEA8000::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->MEA8000FormLeft != 0)
       || (mSettingsP->MEA8000FormTop != 0)
#ifdef _DEBUG
       || (mSettingsP->MEA8000FormWidth != 0)
       || (mSettingsP->MEA8000FormHeight != 0)
#endif /* _DEBUG */
       )
  {
    Position = poDesigned;
    Left = mSettingsP->MEA8000FormLeft;
    Top = mSettingsP->MEA8000FormTop;
#ifdef _DEBUG
    Width = mSettingsP->MEA8000FormWidth;
    Height = mSettingsP->MEA8000FormHeight;
#endif /* _DEBUG */
  }

  ShowHint = mSettingsP->ShowHints;

  if (mSettingsP)
  {
    // Apply theme
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
    PanelState->Font->Color = mSettingsP->ThemeText2Color;
    PanelPitch->Font->Color = mSettingsP->ThemeText2Color;
    PanelAmplitude->Font->Color = mSettingsP->ThemeText2Color;
#if (__BORLANDC__ != 0x551)
    ChartSample->Series[0]->Color = mSettingsP->ThemeText2Color;
#endif
    ListBoxHistory->Font->Color = mSettingsP->ThemeText2Color;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMEA8000::FormResize(TObject *Sender)
{
#ifdef _DEBUG
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->MEA8000FormWidth = Width;
    mSettingsP->MEA8000FormHeight = Height;
  }
#endif /* _DEBUG */

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMEA8000::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->MEA8000FormLeft = Left;
    mSettingsP->MEA8000FormTop = Top;
	}

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMEA8000::InitCurrentState(tNativeCPC* NativeCPC)
{
  tTMPISpeech* TMPISpeech = NativeCPC->TMPISpeech;

  // State
  mLastState = TMPISpeech->state;
  PanelState->Caption = StateString[mLastState];

  // Pitch
  mLastPitch = TMPISpeech->last_pitch;
  PanelPitch->Caption = AnsiString(mLastPitch);

  // Amplitude
  mLastAmplitude = TMPISpeech->last_ampl;
  PanelAmplitude->Caption = AnsiString(mLastAmplitude);

  // Oscilloscope
  ChartSample->Series[0]->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMEA8000::ButtonMonitorStartStopClick(
      TObject *Sender)
{
  if (ButtonMonitorStartStop->Caption == "Start")
  {
    mMonitoring = true;
    ButtonMonitorStartStop->Caption = "Stop";
    Caption = cTitle + " - [Monitoring]";
    FormMain->ResumeStepByStep(OnEmulatorEvent);

    // Reset events
    tTMPISpeech* TMPISpeech = FormMain->NativeCPC->TMPISpeech;
    TMPISpeech->signal_Write_Command = 0;
    TMPISpeech->signal_Write_Pitch = 0;
    TMPISpeech->signal_Write_Frame = 0;
    TMPISpeech->signal_Write_Overflow = 0;
    TMPISpeech->signal_Read_Status = 0;
  }
  else
  {
    mMonitoring = false;
    ButtonMonitorStartStop->Caption = "Start";
    Caption = cTitle;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMEA8000::ButtonHistoryClearClick(TObject *Sender)
{
	ListBoxHistory->Clear();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugMEA8000::OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;
  tTMPISpeech* TMPISpeech = NativeCPC->TMPISpeech;

  //
  // Update state
  //
  if (mLastState != TMPISpeech->state)
  {
    mLastState = TMPISpeech->state;
    PanelState->Caption = StateString[mLastState];
  }
  if (mLastPitch != TMPISpeech->last_pitch)
  {
    mLastPitch = TMPISpeech->last_pitch;
    PanelPitch->Caption = AnsiString(mLastPitch);
  }
  if (mLastAmplitude != TMPISpeech->last_ampl)
  {
    mLastAmplitude = TMPISpeech->last_ampl;
    PanelAmplitude->Caption = AnsiString(mLastAmplitude);
  }

  //
  // Oscilloscope update
  //
  if ( (Condition & EC_SOUND_BUFFER) && (!NativeCPC->step_by_step) )
  {
    int Count = FormMain->EmulatorP->GetAudioTMPIDataSize() / 2; // Mono 16 bits

    // No oscillsocope mode, reset display
    ChartSample->Series[0]->Clear();
    tShort* AudioData = (tShort*)FormMain->EmulatorP->GetAudioTMPIData();
    for (int Loop=0; Loop < Count; Loop++)
    {
      ChartSample->Series[0]->AddXY(Loop, *AudioData++, "", mSettingsP->ThemeText2Color);
    }
    mSampleCount = Count;
    TMPISpeech->signal_Sample_Ready = 0;
  }

  if (!NativeCPC->step_by_step)
    return Pause;

#ifdef _DEBUG
  bool HistoryEntryAdded = false;
#endif /* _DEBUG */

  if (TMPISpeech->signal_Sample_Ready)
  {
    TMPISpeech->signal_Sample_Ready = 0;

    //
    // Update oscilloscope
    //
    ChartSample->Series[0]->AddXY(mSampleCount++, TMPISpeech->lastsample * 16, "", clBlue);
    int Count = ChartSample->Series[0]->Count();
    while (Count > ChartSample->Width)
    {
      ChartSample->Series[0]->Delete(0);
      Count--;
    }

#ifdef _DEBUG
    if ( (mMonitoring == true) && (CheckBoxShowSamples->Checked == true) )
    {
      AnsiString Entry = "Sample=" + AnsiString(TMPISpeech->lastsample * 16);
      ADD_HISTORY_ENTRY(Entry);
      HistoryEntryAdded = true;
    }
#endif /* _DEBUG */
  }

#ifdef _DEBUG

  if (mMonitoring == false)
    return false;

  // Write Command
  if (TMPISpeech->signal_Write_Command)
  {
    TMPISpeech->signal_Write_Command = 0;

    tUChar Command = (tUChar)TMPISpeech->signal_Write_Command_Value;
    AnsiString Entry = "Write COMMAND=&" + AnsiString::IntToHex(Command, 2) + " :";
    Entry += " STOP=";
    Entry += ((Command >> 4) & 1) ? "1" : "0";
    switch (Command & 0x0c)
    {
      case 0x0c: Entry += " CONT=CONTINUE"; break;
      case 0x08: Entry += " CONT=SLOW STOP"; break;
      default: Entry += " CONT=INVALID"; break;
    }
    switch (Command & 0x03)
    {
      case 0x03: Entry += " ROE=ENABLE"; break;
      case 0x02: Entry += " ROE=DISABLE"; break;
      default: Entry += " ROE=INVALID"; break;
    }
    ADD_HISTORY_ENTRY(Entry);
    HistoryEntryAdded = true;
  }

  // Write Pitch
  if (TMPISpeech->signal_Write_Pitch)
  {
    TMPISpeech->signal_Write_Pitch = 0;

    AnsiString Entry = "Write PITCH=" + AnsiString((tUChar)TMPISpeech->signal_Write_Pitch_Value);
    ADD_HISTORY_ENTRY(Entry);
    HistoryEntryAdded = true;
  }

  // Write Frame
  if (TMPISpeech->signal_Write_Frame)
  {
    TMPISpeech->signal_Write_Frame = 0;

    AnsiString Entry = "Write FRAME=&";
    for (int Loop=0; Loop < 4; Loop++)
    {
      Entry += AnsiString::IntToHex((tUChar)TMPISpeech->buf[Loop], 2) + " ";
    }
    ADD_HISTORY_ENTRY(Entry);
    HistoryEntryAdded = true;
  }

  // Write Overflow
  if (TMPISpeech->signal_Write_Overflow)
  {
    TMPISpeech->signal_Write_Overflow = 0;

    AnsiString Entry = "Write OVERFLOW !!";
    ADD_HISTORY_ENTRY(Entry);
    HistoryEntryAdded = true;
  }

  // Read Status
  if (CheckBoxShowReadStatus->Checked == true)
  {
    if (TMPISpeech->signal_Read_Status)
    {
      TMPISpeech->signal_Read_Status = 0;

      AnsiString Entry = "Read STATUS=" + AnsiString((tUChar)TMPISpeech->signal_Read_Status_Value);
      ADD_HISTORY_ENTRY(Entry);
      HistoryEntryAdded = true;
    }
  }

  if (HistoryEntryAdded == true)
  {
    // Show last entry
    ListBoxHistory->TopIndex = ListBoxHistory->Items->Count - 1;
  }

#endif /* _DEBUG */

  return Pause;
}
//---------------------------------------------------------------------------

