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

#include "TFormDebugTextCapture.h"
#include "TFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


TFormDebugTextCapture *FormDebugTextCapture;
//---------------------------------------------------------------------------
__fastcall TFormDebugTextCapture::TFormDebugTextCapture(TComponent* Owner)
  : TForm(Owner)
{
  DoubleBuffered = true;

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugTextCapture::FormShow(TObject *Sender)
{
  // Subscribe to step by step event
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  FormMain->SubscribeToCPCEvents(OnStepByStepEvent);

  MemoCapture->Lines->Clear();
  mCurrentLine = "";

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugTextCapture::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnStepByStepEvent);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTextCapture::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  ShowHint = mSettingsP->ShowHints;

  // Apply theme
  if (mSettingsP)
  {
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
  }
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugTextCapture::OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  if (Condition & EC_TEXT_CAPTURE)
  {
    tZ80* Z80 = NativeCPC->Z80;

    // TXT WRITE CHAR firmware indirection
    if (_PC == 0xbdd3)
    {
      char* CaptureString = " ";
      *CaptureString = _A;
      mCurrentLine += CaptureString;
    }
    // TXT OUT ACTION firmware indirection
    else if (_PC == 0xbdd9)
    {
      if (_A == 0x0D)
      {
        MemoCapture->Lines->Add(mCurrentLine);
        mCurrentLine = "";
      }
    }

  }

  return Pause;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugTextCapture::ButtonStartClick(TObject *Sender)
{
  FormMain->NativeCPC->text_capture = FormMain->NativeCPC->text_capture ? 0 : 1;
  if (FormMain->NativeCPC->text_capture)
  {
    Caption = "Text Capture - [Capturing]";
  }
  else
  {
    Caption = "Text Capture";
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugTextCapture::ButtonClearClick(TObject *Sender)
{
  MemoCapture->Lines->Clear();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

