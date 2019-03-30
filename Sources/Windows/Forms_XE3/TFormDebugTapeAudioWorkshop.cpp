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
#include <math.h>
#include <tchar.h>
#pragma hdrstop
#include "TFormDebugTapeAudioWorkshop.h"
#include "TFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


TFormDebugTapeAudioWorkshop *FormDebugTapeAudioWorkshop;
//---------------------------------------------------------------------------
__fastcall TFormDebugTapeAudioWorkshop::TFormDebugTapeAudioWorkshop(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mEmulator = NULL;
  mAudioContext = NULL;

  // Apply theme
  EditLPF1_A->Font->Color = clBlack;
  EditLPF2_Order->Font->Color = clBlack;
  EditLPF2_Freq->Font->Color = clBlack;
  EditHPF2_Order->Font->Color = clBlack;
  EditHPF2_Freq->Font->Color = clBlack;
  EditPPIThreshold8->Font->Color = clBlack;
  EditPPIThreshold16->Font->Color = clBlack;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::FormShow(TObject *Sender)
{
  if (mAudioContext == NULL)
  {
    Close();
    return;
  }

  // Initialise Display
  CheckBoxCreateDebugOutputAudio->Checked = mAudioContext->TapeCreateOutputAudioFile;
  CheckBoxCreateCSW->Checked = mAudioContext->TapeCreateCSWFile;
  UpdateDisplay();
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->TapeAudioWorkshopFormLeft != 0)
       || (mSettingsP->GateArrayFormTop != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->TapeAudioWorkshopFormLeft;
    Top = mSettingsP->TapeAudioWorkshopFormTop;
  }

  ShowHint = mSettingsP->ShowHints;

  if (mSettingsP)
  {
    // Apply Theme
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
    CheckBoxCreateCSW->Font->Color = mSettingsP->ThemeText2Color;
    Label1LPF1Info->Font->Color = mSettingsP->ThemeText2Color;
    LabelLPF1Param->Font->Color = mSettingsP->ThemeText2Color;
    LabelLPF2Info->Font->Color = mSettingsP->ThemeText2Color;
    LabelLPF2Order->Font->Color = mSettingsP->ThemeText2Color;
    LabelLPF2Freq->Font->Color = mSettingsP->ThemeText2Color;
    LabelHPF2Info->Font->Color = mSettingsP->ThemeText2Color;
    LabelHPF2Order->Font->Color = mSettingsP->ThemeText2Color;
    LabelHPF2Freq->Font->Color = mSettingsP->ThemeText2Color;
    LabelApplyNote->Font->Color = mSettingsP->ThemeText2Color;
    ShapeLine8bits->Pen->Color = mSettingsP->ThemeText2Color;
    ShapeLine8bitsEnd->Pen->Color = mSettingsP->ThemeText2Color;
    ShapeLine16bits->Pen->Color = mSettingsP->ThemeText2Color;
    ShapeLine16bitsEnd->Pen->Color = mSettingsP->ThemeText2Color;
#if (__BORLANDC__ != 0x551)
    ChartFilterGain->Series[0]->Color = mSettingsP->ThemeText2Color;
#endif
    ChartFilterGain->LeftAxis->Axis->Color = mSettingsP->ThemeText1Color;
    ChartFilterGain->LeftAxis->LabelsFont->Color = mSettingsP->ThemeText1Color;
    ChartFilterGain->LeftAxis->Title->Font->Color = mSettingsP->ThemeText1Color;
    ChartFilterGain->BottomAxis->Axis->Color = mSettingsP->ThemeText1Color;
    ChartFilterGain->BottomAxis->LabelsFont->Color = mSettingsP->ThemeText1Color;
    ChartFilterGain->BottomAxis->Title->Font->Color = mSettingsP->ThemeText1Color;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->TapeAudioWorkshopFormLeft = Left;
    mSettingsP->TapeAudioWorkshopFormTop = Top;
  }

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::ButtonReloadAudioClick(
      TObject *Sender)
{
  if (mEmulator == NULL) return;

  // Get Tape Filename
  AnsiString TapeFilename = mEmulator->TapeFilename;
  FormMain->InsertTape(TapeFilename, false);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::CheckBoxCreateDebugOutputAudioClick(
      TObject *Sender)
{
	mAudioContext->TapeCreateOutputAudioFile = CheckBoxCreateDebugOutputAudio->Checked;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::CheckBoxCreateCSWClick(
      TObject *Sender)
{
  mAudioContext->TapeCreateCSWFile = CheckBoxCreateCSW->Checked;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::PanelAutoGainClick(
      TObject *Sender)
{
  mAudioContext->TapeAutoGainActive = mAudioContext->TapeAutoGainActive == true ? false : true;
  PanelAutoGain->Color = mAudioContext->TapeAutoGainActive == true ? clLime : clRed;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::PanelLPF1Click(
      TObject *Sender)
{
  mAudioContext->TapeLPF1Active = mAudioContext->TapeLPF1Active == true ? false : true;
  PanelLPF1->Color = mAudioContext->TapeLPF1Active == true ? clLime : clRed;
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::PanelLPF2Click(
      TObject *Sender)
{
  mAudioContext->TapeLPF2Active = mAudioContext->TapeLPF2Active == true ? false : true;
  PanelLPF2->Color = mAudioContext->TapeLPF2Active == true ? clLime : clRed;
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::PanelHPF2Click(
      TObject *Sender)
{
  mAudioContext->TapeHPF2Active = mAudioContext->TapeHPF2Active == true ? false : true;
  PanelHPF2->Color = mAudioContext->TapeHPF2Active == true ? clLime : clRed;
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::EditPPIThreshold8KeyPress(
      TObject *Sender, char &Key)
{
  if (Key == VK_RETURN)
  {
    try
    {
      mAudioContext->TapePPIThreshold8 = (tUChar)EditPPIThreshold8->Text.ToInt();
      Key = 0;
    }
    catch (...)
    {
      EditPPIThreshold8->Text = mAudioContext->TapePPIThreshold8;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::EditPPIThreshold8Exit(
      TObject *Sender)
{
  try
  {
    mAudioContext->TapePPIThreshold8 = (tUChar)EditPPIThreshold8->Text.ToInt();
  }
  catch (...)
  {
    EditPPIThreshold8->Text = mAudioContext->TapePPIThreshold8;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::EditPPIThreshold16KeyPress(
      TObject *Sender, char &Key)
{
  if (Key == VK_RETURN)
  {
    try
    {
      mAudioContext->TapePPIThreshold16 = (tShort)EditPPIThreshold16->Text.ToInt();
      Key = 0;
    }
    catch (...)
    {
      EditPPIThreshold16->Text = mAudioContext->TapePPIThreshold16;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::EditPPIThreshold16Exit(
      TObject *Sender)
{
  try
  {
    mAudioContext->TapePPIThreshold16 = (tShort)EditPPIThreshold16->Text.ToInt();
  }
  catch (...)
  {
    EditPPIThreshold16->Text = mAudioContext->TapePPIThreshold16;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::EditLPF1_AKeyPress(
      TObject *Sender, char &Key)
{
  if (Key == VK_RETURN)
  {
    try
    {
      float Value = (float)EditLPF1_A->Text.ToDouble();
      mAudioContext->TapeLPF1_A = Value;
      EditLPF1_A->Text = AnsiString::FormatFloat("0.000", Value);
      Key = 0;
    }
    catch (...)
    {
      EditLPF1_A->Text = AnsiString::FormatFloat("0.000", mAudioContext->TapeLPF1_A);
    }
    UpdateGain();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::EditLPF1_AExit(
      TObject *Sender)
{
  try
  {
    float Value = (float)EditLPF1_A->Text.ToDouble();
    mAudioContext->TapeLPF1_A = Value;
    EditLPF1_A->Text = AnsiString::FormatFloat("0.000", Value);
  }
  catch (...)
  {
    EditLPF1_A->Text = AnsiString::FormatFloat("0.000", mAudioContext->TapeLPF1_A);
  }
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::EditLPF2_OrderKeyPress(
      TObject *Sender, char &Key)
{
  if (Key == VK_RETURN)
  {
    try
    {
      mAudioContext->TapeLPF2Order = (tUChar)EditLPF2_Order->Text.ToInt();
      Key = 0;
    }
    catch (...)
    {
      EditLPF2_Order->Text = mAudioContext->TapeLPF2Order;
    }
    UpdateGain();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::EditLPF2_OrderExit(
      TObject *Sender)
{
  try
  {
    mAudioContext->TapeLPF2Order = (tUChar)EditLPF2_Order->Text.ToInt();
  }
  catch (...)
  {
    EditLPF2_Order->Text = mAudioContext->TapeLPF2Order;
  }
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::EditLPF2_FreqKeyPress(
      TObject *Sender, char &Key)
{
  if (Key == VK_RETURN)
  {
    try
    {
      mAudioContext->TapeLPF2Freq = (tUShort)EditLPF2_Freq->Text.ToInt();
      Key = 0;
    }
    catch (...)
    {
      EditLPF2_Freq->Text = mAudioContext->TapeLPF2Freq;
    }
    UpdateGain();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::EditLPF2_FreqExit(
      TObject *Sender)
{
  try
  {
    mAudioContext->TapeLPF2Freq = (tUShort)EditLPF2_Freq->Text.ToInt();
  }
  catch (...)
  {
    EditLPF2_Freq->Text = mAudioContext->TapeLPF2Freq;
  }
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::EditHPF2_OrderKeyPress(
      TObject *Sender, char &Key)
{
  if (Key == VK_RETURN)
  {
    try
    {
      mAudioContext->TapeHPF2Order = (tUChar)EditHPF2_Order->Text.ToInt();
      Key = 0;
    }
    catch (...)
    {
      EditHPF2_Order->Text = mAudioContext->TapeHPF2Order;
    }
    UpdateGain();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::EditHPF2_OrderExit(
      TObject *Sender)
{
  try
  {
    mAudioContext->TapeHPF2Order = (tUChar)EditHPF2_Order->Text.ToInt();
  }
  catch (...)
  {
    EditHPF2_Order->Text = mAudioContext->TapeHPF2Order;
  }
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugTapeAudioWorkshop::EditHPF2_FreqKeyPress(
      TObject *Sender, char &Key)
{
  if (Key == VK_RETURN)
  {
    try
    {
      mAudioContext->TapeHPF2Freq = (tUShort)EditHPF2_Freq->Text.ToInt();
      Key = 0;
    }
    catch (...)
    {
      EditHPF2_Freq->Text = mAudioContext->TapeHPF2Freq;
    }
    UpdateGain();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugTapeAudioWorkshop::EditHPF2_FreqExit(
      TObject *Sender)
{
  try
  {
    mAudioContext->TapeHPF2Freq = (tUShort)EditHPF2_Freq->Text.ToInt();
  }
  catch (...)
  {
    EditHPF2_Freq->Text = mAudioContext->TapeHPF2Freq;
  }
  UpdateGain();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::UpdateDisplay(void)
{
  EditPPIThreshold8->Text = mAudioContext->TapePPIThreshold8;
  EditPPIThreshold16->Text = mAudioContext->TapePPIThreshold16;
  PanelAutoGain->Color = mAudioContext->TapeAutoGainActive == true ? clLime : clRed;
  PanelLPF1->Color = mAudioContext->TapeLPF1Active == true ? clLime : clRed;
  EditLPF1_A->Text = AnsiString::FormatFloat("0.000", mAudioContext->TapeLPF1_A);
  PanelLPF2->Color = mAudioContext->TapeLPF2Active == true ? clLime : clRed;
  EditLPF2_Order->Text = mAudioContext->TapeLPF2Order;
  EditLPF2_Freq->Text = mAudioContext->TapeLPF2Freq;
  PanelHPF2->Color = mAudioContext->TapeHPF2Active == true ? clLime : clRed;
  EditHPF2_Order->Text = mAudioContext->TapeHPF2Order;
  EditHPF2_Freq->Text = mAudioContext->TapeHPF2Freq;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::ButtonRestoreDefaultClick(
      TObject *Sender)
{
	if (Application->MessageBox(_T("Do you want to restore default filter parameters ?"),
															_T("Audio filter parameters"),
                              MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDYES)
  {
    mAudioContext->RestoreDefaultTapeAudioParameters();
    UpdateDisplay();
    UpdateGain();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugTapeAudioWorkshop::UpdateGain(void)
{
  ChartFilterGain->Series[0]->Clear();
  tShort AmplitudeIN = 20000;
  
  for (tUShort Loop=100; Loop < 20000; Loop+=100)
  {
    tShort AmplitudeOUT = mAudioContext->GetTapeFilterOutputAmplitude(Loop, AmplitudeIN);
    double Gain = 20 * log((double)AmplitudeOUT / (double)AmplitudeIN);
    ChartFilterGain->Series[0]->AddXY((double)Loop, Gain, Loop, mSettingsP->ThemeText2Color);
  }
}
//---------------------------------------------------------------------------

