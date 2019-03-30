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
#pragma hdrstop
#include "TFormDebugger.h"
#include "TFormMain.h"


//=================================
// Options

//=================================


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

static const AnsiString cTitle = "Execute";
static const AnsiString cTitlePaused = " - [PAUSED]";
static const AnsiString cTitlePlaying = " - [Playing]";
static const AnsiString cTitleRunning = " - [Running]";

TFormDebugger *FormDebugger;
//---------------------------------------------------------------------------
__fastcall TFormDebugger::TFormDebugger(TComponent* Owner)
        : TForm(Owner)
{
  mSettingsP = NULL;

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugger::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  mPaused = false;
  mPauseRequested = false;
  mStepByStep = false;
  mRunTillVBL = false;
  mRunTillHBL = false;

  FormMain->SubscribeToCPCEvents(OnCPCEvent);

  UpdateControls(FormMain->EmulatorP->Paused,
                 FormMain->EmulatorP->StepByStepActive);
  Caption = Title;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugger::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnCPCEvent);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDebugger::GetTitle(void)
{
  AnsiString Title = cTitle;
  if (mPaused == true) { Title += cTitlePaused; }
  else if (mStepByStep == true) { Title += cTitlePlaying; }
  else { Title += cTitleRunning; }
  
 return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugger::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->DebuggerFormLeft != 0)
       || (mSettingsP->DebuggerFormTop != 0)
       /*|| (mSettings->DebuggerFormWidth != 0)
       || (mSettings->DebuggerFormHeight != 0)*/ )
  {
    Position = poDesigned;
    Left = mSettingsP->DebuggerFormLeft;
    Top = mSettingsP->DebuggerFormTop;
    /*Width = mSettings->DebuggerFormWidth;
    Height = mSettings->DebuggerFormHeight;*/
  }

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugger::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->DebuggerFormWidth = Width;
    mSettingsP->DebuggerFormHeight = Height;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugger::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->DebuggerFormLeft = Left;
    mSettingsP->DebuggerFormTop = Top;
  }

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugger::ButtonStepClick(TObject *Sender)
{
  mRunTillVBL = false;
  mRunTillHBL = false;

  mPauseRequested = true;
  FormMain->ResumeStepByStep(OnCPCEvent);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugger::ButtonPlayClick(TObject *Sender)
{
  mRunTillVBL = false;
  mRunTillHBL = false;

  if (ButtonPlay->Caption == "Play")
  {
    FormMain->ResumeStepByStep(OnCPCEvent);
    ButtonPlay->Caption = "Stop";
  }
  else
  {
    mPauseRequested = true;
    FormMain->ResumeStepByStep(OnCPCEvent);
    ButtonPlay->Caption = "Play";
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugger::ButtonRunClick(TObject *Sender)
{
  mRunTillVBL = false;
  mRunTillHBL = false;

  if (ButtonRun->Caption == "Run")
  {
    FormMain->StopStepByStep(false);
    ButtonRun->Caption = "Pause";
  }
  else
  {
    mPauseRequested = true;
    FormMain->Pause(true);
    ButtonRun->Caption = "Run";
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugger::ButtonPlayTillVBLClick(TObject *Sender)
{
  mRunTillVBL = true;
  mRunTillHBL = false;

  mPreviousVBL = Engine_IsVideoVBL(FormMain->EmulatorP->NativeCPC);
  FormMain->ResumeStepByStep(OnCPCEvent);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugger::ButtonPlayTillHBLClick(TObject *Sender)
{
  mRunTillVBL = false;
  mRunTillHBL = true;

  mPreviousHBL = Engine_IsVideoHBL(FormMain->EmulatorP->NativeCPC);
  FormMain->ResumeStepByStep(OnCPCEvent);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugger::UpdateControls(bool Pause, bool StepByStep)
{
  if (StepByStep == true)
  {
    ButtonStep->Enabled = Pause;
    ButtonPlay->Enabled = true;
    ButtonPlay->Caption = (Pause == false) ? "Stop" : "Play";
    ButtonRun->Caption = "Run";
  }
  else
  {
    ButtonStep->Enabled = Pause;
    ButtonPlay->Enabled = Pause;
    ButtonPlay->Caption = "Play";
    ButtonRun->Caption = (Pause == false) ? "Pause" : "Run";
  }
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugger::OnCPCEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;
  bool StepByStep = NativeCPC->step_by_step;

  if (mPauseRequested == true)
  {
    Pause = true;
    mPauseRequested = false;
  }

  if ( (Pause != mPaused) || (StepByStep != mStepByStep) )
  {
    UpdateControls(Pause, StepByStep);

    mPaused = Pause;
    mStepByStep = StepByStep;
    Caption = Title;
  }

  if ( (Condition & EC_END_INSTRUCTION) == 0)
    return Pause;

  // Check and Update VBL
  if (mRunTillVBL == true)
  {
    bool CurrentVBL = Engine_IsVideoVBL(NativeCPC);
    if (mPreviousVBL != CurrentVBL)
    {
      if (CurrentVBL == true)
      {
        mRunTillVBL = false;
        Pause = true;
      }

      mPreviousVBL = CurrentVBL;
    }
  }

  // Check and Update HBL
  if (mRunTillHBL == true)
  {
    bool CurrentHBL = Engine_IsVideoHBL(NativeCPC);
    if (mPreviousHBL != CurrentHBL)
    {
      if (CurrentHBL == true)
      {
        mRunTillHBL = false;
        Pause = true;
      }

      mPreviousHBL = CurrentHBL;
    }
  }

	NOT_USED(Condition);

	return Pause;
}
//---------------------------------------------------------------------------

