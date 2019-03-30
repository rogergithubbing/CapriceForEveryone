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

#include "TFormDebugCRTC.h"
#include "TFormDebugEditValue.h"
#include "TFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


static const AnsiString cTitle = "CRTC";
static const AnsiString cTitlePaused  = " - [PAUSED]";


TFormDebugCRTC *FormDebugCRTC;
//---------------------------------------------------------------------------
__fastcall TFormDebugCRTC::TFormDebugCRTC(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;

  for (int i=0; i < CRTC_REGISTERS_NBELEMENT; i++)
  {
    mDisplayValueHexa[i] = true;
  }

  // Init component lists
  for (int Loop=0; Loop < CRTC_REGISTERS_NBELEMENT; Loop++)
  {
    mRadioButtonRegister[Loop] = (TRadioButton*)FindComponent("RadioButtonR" + AnsiString(Loop));
    mPanelRegisterFormat[Loop] = (TPanel*)FindComponent("PanelFormatR" + AnsiString(Loop));
    mPanelRegister[Loop] = (TPanel*)FindComponent("PanelR" + AnsiString(Loop));
    mLabelRegister[Loop] = (TLabel*)FindComponent("LabelR" + AnsiString(Loop));
  }

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  for (int Loop=0; Loop <= 17; Loop++)
  {
    TPanel* PanelP = (TPanel*)FindComponent("PanelFormatR" + AnsiString(Loop));
    if (PanelP)
    {
      PanelP->Font->Color = clBlack;
    }
    PanelP = (TPanel*)FindComponent("PanelR" + AnsiString(Loop));
    if (PanelP)
    {
      PanelP->Font->Color = clBlack;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::FormShow(TObject *Sender)
{
NOT_USED(Sender);

  // Subscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->SubscribeToCPCEvents(OnCPCEvent);

    tNativeCPC* NativeCPC = FormMain->EmulatorP->NativeCPC;
    InitRegisters(NativeCPC);

    mPaused = FormMain->NativeCPC->paused;
    mStepByStep = FormMain->NativeCPC->step_by_step;
    if (mStepByStep == true)
    {
      UpdateStatus(NativeCPC);
      UpdatePosition(NativeCPC);
    }
    else
    {
      ClearStatus();
      ClearPosition();
    }

    Caption = Title;
  }
  else
  {
    Close();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCRTC::FormHide(TObject *Sender)
{
	// Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnCPCEvent);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDebugCRTC::GetTitle(void)
{
  AnsiString Title = cTitle;
  if (mPaused == true) Title += cTitlePaused;
 return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->CRTCFormLeft != 0)
       || (mSettingsP->CRTCFormTop != 0)
       /*|| (mSettings->CRTCFormWidth != 0)
       || (mSettings->CRTCFormHeight != 0)*/ )
  {
    Position = poDesigned;
    Left = mSettingsP->CRTCFormLeft;
    Top = mSettingsP->CRTCFormTop;
    //Width = mSettings->CRTCFormWidth;
    //Height = mSettings->CRTCFormHeight;
  }

  ShowHint = mSettingsP->ShowHints;

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  PanelCharCount->Font->Color = mSettingsP->ThemeText2Color;
  PanelLineCount->Font->Color = mSettingsP->ThemeText2Color;
  PanelRasterCount->Font->Color = mSettingsP->ThemeText2Color;
  PanelRasterCount->Font->Color = mSettingsP->ThemeText2Color;
  PanelBeamPosX->Font->Color = mSettingsP->ThemeText2Color;
  PanelBeamPosY->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::FormResize(TObject *Sender)
{
NOT_USED(Sender);

	if ( (Visible == true) && (mSettingsP) )
	{
		mSettingsP->CRTCFormWidth = Width;
		mSettingsP->CRTCFormHeight = Height;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCRTC::WMMove(TMessage &Message)
{
NOT_USED(Message);

	if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->CRTCFormLeft = Left;
    mSettingsP->CRTCFormTop = Top;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::InitRegisters(tNativeCPC* NativeCPC)
{
  // Initialiser registers value
  for (int Index=0; Index < CRTC_REGISTERS_NBELEMENT; Index++)
  {
    mPreviousRegistersP[Index] = NativeCPC->CRTC->registers[Index];
    mLastDisplayedRegistersP[Index] = NativeCPC->CRTC->registers[Index];

    if (mDisplayValueHexa[Index])
    {
      mPanelRegister[Index]->Caption = AnsiString::IntToHex(mLastDisplayedRegistersP[Index], 2);
    }
    else
    {
      mPanelRegister[Index]->Caption = mLastDisplayedRegistersP[Index];
    }
  }
  
  mLastDisplayedRegisterSelected = NativeCPC->CRTC->reg_select;
  mRadioButtonRegister[mLastDisplayedRegisterSelected]->Checked = true;
  mLabelRegister[mLastDisplayedRegisterSelected]->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::UpdateRegisters(tNativeCPC* NativeCPC)
{
  // Initialiser registers value
  for (int Index=0; Index < CRTC_REGISTERS_NBELEMENT; Index++)
  {
    if (mLastDisplayedRegistersP[Index] != NativeCPC->CRTC->registers[Index])
    {
      mLastDisplayedRegistersP[Index] = NativeCPC->CRTC->registers[Index];

      if (mDisplayValueHexa[Index])
      {
        mPanelRegister[Index]->Caption = AnsiString::IntToHex(mLastDisplayedRegistersP[Index], 2);
      }
      else
      {
        mPanelRegister[Index]->Caption = mLastDisplayedRegistersP[Index];
      }
    }
  }

  if (NativeCPC->CRTC->reg_select < 18)
  {
    if (mLastDisplayedRegisterSelected != NativeCPC->CRTC->reg_select)
    {
      mLabelRegister[mLastDisplayedRegisterSelected]->Font->Color = mSettingsP->ThemeText1Color;
      mLastDisplayedRegisterSelected = NativeCPC->CRTC->reg_select;
      mRadioButtonRegister[mLastDisplayedRegisterSelected]->Checked = true;
      mLabelRegister[mLastDisplayedRegisterSelected]->Font->Color = mSettingsP->ThemeText2Color;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::UpdateStatus(tNativeCPC* NativeCPC)
{
  CheckBoxVBL->Checked = Engine_IsVideoVBL(NativeCPC);
  CheckBoxHBL->Checked = Engine_IsVideoHBL(NativeCPC);

  // Display Char Count
  AnsiString Message = "&&" + AnsiString::IntToHex(Engine_GetVideoCharCount(NativeCPC), 2);
  Message += " / &&" + AnsiString::IntToHex(NativeCPC->CRTC->registers[0], 2);
  PanelCharCount->Caption = Message;

  // Display Line Count
  Message = "&&" + AnsiString::IntToHex(Engine_GetVideoLineCount(NativeCPC), 2);
  Message += " / &&" + AnsiString::IntToHex(NativeCPC->CRTC->registers[4], 2);
  PanelLineCount->Caption = Message;

  // Display Raster Count
  Message = "&&" + AnsiString::IntToHex(Engine_GetVideoRasterCount(NativeCPC), 2);
  Message += " / &&" + AnsiString::IntToHex(NativeCPC->CRTC->registers[9], 2);
  PanelRasterCount->Caption = Message;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugCRTC::ClearStatus(void)
{
  CheckBoxVBL->Checked = false;
  CheckBoxHBL->Checked = false;
  PanelCharCount->Caption = "";
  PanelLineCount->Caption = "";
  PanelRasterCount->Caption = "";
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::UpdatePosition(tNativeCPC* NativeCPC)
{
  // Display Scanline
  PanelBeamPosX->Caption = Engine_GetVideoBeamPosX(NativeCPC);
  PanelBeamPosY->Caption = Engine_GetVideoBeamPosY(NativeCPC);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugCRTC::ClearPosition(void)
{
  PanelBeamPosX->Caption = "";
  PanelBeamPosY->Caption = "";
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugCRTC::OnCPCEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  UpdateRegisters(NativeCPC);

  bool Pause = NativeCPC->paused;
  bool StepByStep = NativeCPC->step_by_step;

  if ( (mStepByStep == true) && (mStepByStep != StepByStep) )
  {
    mStepByStep = false;
    Caption = Title;

    if (Pause == false)
    {
      // Clear display
      ClearStatus();
      ClearPosition();
    }
  }
  else if (Pause != mPaused)
  {
    if (Pause == true)
    {
      UpdateStatus(NativeCPC);
      UpdatePosition(NativeCPC);
    }
    else
    {
      ClearStatus();
      ClearPosition();
    }

    mPaused = Pause;
    Caption = Title;
  }

  if ( ((Condition & EC_END_INSTRUCTION) == 0) && (Pause == false) )
    return false;

  // Register update
  if (StepByStep == true)
  {
    mStepByStep = true;

    UpdateStatus(NativeCPC);
    UpdatePosition(NativeCPC);
  }

  return Pause;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::UpdateRegister(int Index)
{
  if (mDisplayValueHexa[Index])
  {
    mPanelRegister[Index]->Caption = AnsiString::IntToHex(mLastDisplayedRegistersP[Index], 2);
  }
  else
  {
    mPanelRegister[Index]->Caption = mLastDisplayedRegistersP[Index];
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::UpdateRegisterFormat(int Index)
{
  mDisplayValueHexa[Index] ^= 1; // Invert

  if (mDisplayValueHexa[Index])
  {
    mPanelRegisterFormat[Index]->Caption = "&&";
  }
  else
  {
    mPanelRegisterFormat[Index]->Caption = "";
  }
  
  UpdateRegister(Index);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::PanelFormatClick(TObject *Sender)
{
  UpdateRegisterFormat(((TPanel*)Sender)->Tag);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCRTC::PanelRegClick(TObject *Sender)
{
  int Index = ((TPanel*)Sender)->Tag;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = FormMain->EmulatorP->NativeCPC->CRTC->registers[Index];
  EditValue->Hexadecimal = mDisplayValueHexa[Index];
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    int NewValue = EditValue->NewValue;
    tNativeCPC* NativeCPC = FormMain->EmulatorP->NativeCPC;
    
    if (NativeCPC->SetCRTCRegisterPtr)
    {
      NativeCPC->SetCRTCRegisterPtr(NativeCPC, Index, NewValue);
    }
    else
    {
      NativeCPC->CRTC->registers[Index] = NewValue;
    }
    NativeCPC->CRTC->signal_register_update = 1;
    mLastDisplayedRegistersP[Index] = NewValue;
    UpdateRegister(Index);
  }
  delete EditValue;
}
//---------------------------------------------------------------------------


