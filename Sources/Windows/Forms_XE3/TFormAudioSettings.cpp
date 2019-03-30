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

#include "TFormAudioSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAudioSettings *FormAudioSettings;
//---------------------------------------------------------------------------
__fastcall TFormAudioSettings::TFormAudioSettings(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mAudioContextP = NULL;
  mEmulatorP = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormAudioSettings::FormShow(TObject *Sender)
{
  if ( (mSettingsP == NULL) || (mAudioContextP == NULL) || (mEmulatorP == NULL) )
  {
    Close();
    return;
  }

  // Apply Theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  Font->Color = mSettingsP->ThemeText1Color;
  EditGainSpeaker->Font->Color = clBlack;
  EditGainFloppy->Font->Color = clBlack;
  EditGainTape->Font->Color = clBlack;
  EditGainTMPI->Font->Color = clBlack;
  EditGainPlayCity->Font->Color = clBlack;
  ComboBoxDevices->Font->Color = clBlack;
  RadioButtonSpeakerStereo->Font->Color = mSettingsP->ThemeText2Color;
  RadioButtonSpeakerMono->Font->Color = mSettingsP->ThemeText2Color;
  RadioButtonPlayCityStereo->Font->Color = mSettingsP->ThemeText2Color;
  RadioButtonPlayCityMono->Font->Color = mSettingsP->ThemeText2Color;
  CheckBoxPlayCityInvert->Font->Color = mSettingsP->ThemeText2Color;

  TrackBarGainSpeaker->Position = -mSettingsP->AudioGainSpeaker;
  EditGainSpeaker->Text = AnsiString(mSettingsP->AudioGainSpeaker);
  TrackBarGainFloppy->Position = -mSettingsP->AudioGainFloppy;
  EditGainFloppy->Text = AnsiString(mSettingsP->AudioGainFloppy);
  TrackBarGainTape->Position = -mSettingsP->AudioGainTape;
  EditGainTape->Text = AnsiString(mSettingsP->AudioGainTape);
  TrackBarGainTMPI->Position = -mSettingsP->AudioGainTMPI;
  EditGainTMPI->Text = AnsiString(mSettingsP->AudioGainTMPI);
  TrackBarGainPlayCity->Position = -mSettingsP->AudioGainPlayCity;
  EditGainPlayCity->Text = AnsiString(mSettingsP->AudioGainPlayCity);
  mOldSpeakerStereo = mSettingsP->AudioSpeakerStereo;
  if (mSettingsP->AudioSpeakerStereo == true)
  {
    RadioButtonSpeakerStereo->Checked = true;
  }
  else
  {
    RadioButtonSpeakerMono->Checked = true;
  }
  mOldPlayCityStereo = mSettingsP->AudioPlayCityStereo;
  if (mSettingsP->AudioPlayCityStereo == true)
  {
    RadioButtonPlayCityStereo->Checked = true;
  }
  else
  {
    RadioButtonPlayCityMono->Checked = true;
  }
  mOldPlayCityInvert = mSettingsP->AudioPlayCityInvert;
  CheckBoxPlayCityInvert->Checked = mOldPlayCityInvert;

  // Display audio devices
  ComboBoxDevices->Clear();
  for (int Loop=0; Loop < mAudioContextP->DevicesCount; Loop++)
  {
    ComboBoxDevices->Items->Add(mAudioContextP->Device[Loop]);
    if ( (mAudioContextP->Device[Loop] == mSettingsP->AudioRenderer)
         || (Loop == 0) /* First Renderer by default */ )
    {
      ComboBoxDevices->ItemIndex = Loop;
    }
  }
  ComboBoxDevices->Enabled = (mAudioContextP->DevicesCount > 1) ? true : false;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormAudioSettings::ButtonOKClick(TObject *Sender)
{
  mSettingsP->AudioGainSpeaker = -TrackBarGainSpeaker->Position;
  mSettingsP->AudioGainFloppy = -TrackBarGainFloppy->Position;
  mSettingsP->AudioGainTape = -TrackBarGainTape->Position;
  mSettingsP->AudioGainTMPI = -TrackBarGainTMPI->Position;
  mSettingsP->AudioGainPlayCity = -TrackBarGainPlayCity->Position;
  mSettingsP->AudioRenderer = ComboBoxDevices->Text;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormAudioSettings::ButtonCancelClick(TObject *Sender)
{
  // Restore audio gains
  mAudioContextP->GainSpeaker = mSettingsP->AudioGainSpeaker;
  mAudioContextP->GainFloppy = mSettingsP->AudioGainFloppy;
  mAudioContextP->GainTape = mSettingsP->AudioGainTape;
  mAudioContextP->GainTMPI = mSettingsP->AudioGainTMPI;
  mAudioContextP->GainPlayCity = mSettingsP->AudioGainPlayCity;

  // Restore Speaker mono/stereo
  mSettingsP->AudioSpeakerStereo = mOldSpeakerStereo;
  mAudioContextP->SpeakerStereo = mOldSpeakerStereo;
  if (mOldSpeakerStereo == true)
  {
    mEmulatorP->NativeCPC->SpeakerSynthetizerPtr = mEmulatorP->NativeCPC->SpeakerSynthetizerStereo16Ptr;
  }
  else
  {
    mEmulatorP->NativeCPC->SpeakerSynthetizerPtr = mEmulatorP->NativeCPC->SpeakerSynthetizerMono16Ptr;
  }
  Sound_Calculate_Level_Tables(mEmulatorP->NativeCPC->PSG, mAudioContextP->SpeakerStereo, mAudioContextP->Speaker16Bits);

  // Restore PlayCity mono/stereo
  mSettingsP->AudioPlayCityStereo = mOldPlayCityStereo;
  mSettingsP->AudioPlayCityInvert = mOldPlayCityInvert;
  mAudioContextP->PlayCityStereo = mOldPlayCityStereo;
  if (mOldPlayCityStereo == true)
  {
    if (mOldPlayCityInvert)
    {
      mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerStereo16InvertPtr;
    }
    else
    {
      mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerStereo16Ptr;
    }
  }
  else
  {
    mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerMono16Ptr;
  }
  Sound_Calculate_Level_Tables(mEmulatorP->NativeCPC->PlayCity->PSG_Left,
                      mAudioContextP->PlayCityStereo, mAudioContextP->PlayCity16Bits);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormAudioSettings::TrackBarGainSpeakerChange(
      TObject *Sender)
{
  if (TrackBarGainSpeaker->Focused() == false) return;
  EditGainSpeaker->Text = AnsiString(-TrackBarGainSpeaker->Position);
  mAudioContextP->GainSpeaker = -TrackBarGainSpeaker->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainSpeakerChange(TObject *Sender)
{
  if (EditGainSpeaker->Focused() == false) return;

  if (EditGainSpeaker->Text.Trim().Length() > 0)
  {
    try
    {
      int NewValue = EditGainSpeaker->Text.Trim().ToInt();

      // Check valid value
      if ( (NewValue >=0) && (NewValue <= 100))
      {
        TrackBarGainSpeaker->Position = -NewValue;
      }
      else if (NewValue > 100)
      {
        TrackBarGainSpeaker->Position = -100;
      }
      else if (NewValue < 0)
      {
        TrackBarGainSpeaker->Position = 0;
      }
    }
    catch(...)
    {
      TrackBarGainSpeaker->Position = 0;
    }
  }
  else
  {
    TrackBarGainSpeaker->Position = 0;
  }

  mAudioContextP->GainSpeaker = -TrackBarGainSpeaker->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainSpeakerClick(TObject *Sender)
{
  EditGainSpeaker->SelectAll();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormAudioSettings::TrackBarGainFloppyChange(
      TObject *Sender)
{
  if (TrackBarGainFloppy->Focused() == false) return;
  EditGainFloppy->Text = AnsiString(-TrackBarGainFloppy->Position);
  mAudioContextP->GainFloppy = -TrackBarGainFloppy->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainFloppyChange(TObject *Sender)
{
  if (EditGainFloppy->Focused() == false) return;

  if (EditGainFloppy->Text.Trim().Length() > 0)
  {
    try
    {
      int NewValue = EditGainFloppy->Text.Trim().ToInt();

      // Check valid value
      if ( (NewValue >=0) && (NewValue <= 100))
      {
        TrackBarGainFloppy->Position = -NewValue;
      }
      else if (NewValue > 100)
      {
        TrackBarGainFloppy->Position = -100;
      }
      else if (NewValue < 0)
      {
        TrackBarGainFloppy->Position = 0;
      }
    }
    catch(...)
    {
      TrackBarGainFloppy->Position = 0;
    }
  }
  else
  {
    TrackBarGainFloppy->Position = 0;
  }

  mAudioContextP->GainFloppy = -TrackBarGainFloppy->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainFloppyClick(TObject *Sender)
{
  EditGainFloppy->SelectAll();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormAudioSettings::TrackBarGainTapeChange(TObject *Sender)
{
  if (TrackBarGainTape->Focused() == false) return;
  EditGainTape->Text = AnsiString(-TrackBarGainTape->Position);
  mAudioContextP->GainTape = -TrackBarGainTape->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainTapeChange(TObject *Sender)
{
  if (EditGainTape->Focused() == false) return;

  if (EditGainTape->Text.Trim().Length() > 0)
  {
    try
    {
      int NewValue = EditGainTape->Text.Trim().ToInt();

      // Check valid value
      if ( (NewValue >=0) && (NewValue <= 100))
      {
        TrackBarGainTape->Position = -NewValue;
      }
      else if (NewValue > 100)
      {
        TrackBarGainTape->Position = -100;
      }
      else if (NewValue < 0)
      {
        TrackBarGainTape->Position = 0;
      }
    }
    catch(...)
    {
      TrackBarGainTape->Position = 0;
    }
  }
  else
  {
    TrackBarGainTape->Position = 0;
  }

  mAudioContextP->GainTape = -TrackBarGainTape->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainTapeClick(TObject *Sender)
{
  EditGainTape->SelectAll();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------



void __fastcall TFormAudioSettings::TrackBarGainTMPIChange(TObject *Sender)
{
  if (TrackBarGainTMPI->Focused() == false) return;
  EditGainTMPI->Text = AnsiString(-TrackBarGainTMPI->Position);
  mAudioContextP->GainTMPI = -TrackBarGainTMPI->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainTMPIChange(TObject *Sender)
{
  if (EditGainTMPI->Focused() == false) return;

  if (EditGainTMPI->Text.Trim().Length() > 0)
  {
    try
    {
      int NewValue = EditGainTMPI->Text.Trim().ToInt();

      // Check valid value
      if ( (NewValue >=0) && (NewValue <= 100))
      {
        TrackBarGainTMPI->Position = -NewValue;
      }
      else if (NewValue > 100)
      {
        TrackBarGainTMPI->Position = -100;
      }
      else if (NewValue < 0)
      {
        TrackBarGainTMPI->Position = 0;
      }
    }
    catch(...)
    {
    }
  }
  else
  {
    TrackBarGainTMPI->Position = 0;
  }

  mAudioContextP->GainTMPI = -TrackBarGainTMPI->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainTMPIClick(TObject *Sender)
{
  EditGainTMPI->SelectAll();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormAudioSettings::TrackBarGainPlayCityChange(TObject *Sender)
{
  if (TrackBarGainPlayCity->Focused() == false) return;
  EditGainPlayCity->Text = AnsiString(-TrackBarGainPlayCity->Position);
  mAudioContextP->GainPlayCity = -TrackBarGainPlayCity->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainPlayCityChange(TObject *Sender)
{
  if (EditGainPlayCity->Focused() == false) return;

  if (EditGainPlayCity->Text.Trim().Length() > 0)
  {
    try
    {
      int NewValue = EditGainPlayCity->Text.Trim().ToInt();

      // Check valid value
      if ( (NewValue >=0) && (NewValue <= 100))
      {
        TrackBarGainPlayCity->Position = -NewValue;
      }
      else if (NewValue > 100)
      {
        TrackBarGainPlayCity->Position = -100;
      }
      else if (NewValue < 0)
      {
        TrackBarGainPlayCity->Position = 0;
      }
    }
    catch(...)
    {
    }
  }
  else
  {
    TrackBarGainPlayCity->Position = 0;
  }

  mAudioContextP->GainPlayCity = -TrackBarGainPlayCity->Position;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAudioSettings::EditGainPlayCityClick(TObject *Sender)
{
  EditGainPlayCity->SelectAll();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAudioSettings::ComboBoxDevicesDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxDevices->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxDevices->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxDevices->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------

void __fastcall TFormAudioSettings::RadioButtonSpeakerStereoClick(TObject *Sender)

{
  if (((TRadioButton*)Sender)->Focused() == true)
  {
    mAudioContextP->SpeakerStereo = true;
    mEmulatorP->NativeCPC->SpeakerSynthetizerPtr = mEmulatorP->NativeCPC->SpeakerSynthetizerStereo16Ptr;
    Sound_Calculate_Level_Tables(mEmulatorP->NativeCPC->PSG,
          mAudioContextP->SpeakerStereo, mAudioContextP->Speaker16Bits);
    mAudioContextP->GainSpeaker = -TrackBarGainSpeaker->Position;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormAudioSettings::RadioButtonSpeakerMonoClick(TObject *Sender)
{
  if (((TRadioButton*)Sender)->Focused() == true)
  {
    mAudioContextP->SpeakerStereo = false;
    mEmulatorP->NativeCPC->SpeakerSynthetizerPtr = mEmulatorP->NativeCPC->SpeakerSynthetizerMono16Ptr;
    Sound_Calculate_Level_Tables(mEmulatorP->NativeCPC->PSG,
          mAudioContextP->SpeakerStereo, mAudioContextP->Speaker16Bits);
    mAudioContextP->GainSpeaker = -TrackBarGainSpeaker->Position;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormAudioSettings::RadioButtonPlayCityStereoClick(TObject *Sender)

{
  if (((TRadioButton*)Sender)->Focused() == true)
  {
    mAudioContextP->PlayCityStereo = true;
    if (mSettingsP->AudioPlayCityInvert)
    {
      mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerStereo16InvertPtr;
    }
    else
    {
      mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerStereo16Ptr;
    }

    Sound_Calculate_Level_Tables(mEmulatorP->NativeCPC->PlayCity->PSG_Left,
          mAudioContextP->PlayCityStereo, mAudioContextP->PlayCity16Bits);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormAudioSettings::RadioButtonPlayCityMonoClick(TObject *Sender)

{
  if (((TRadioButton*)Sender)->Focused() == true)
  {
    mAudioContextP->PlayCityStereo = false;
    mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerMono16Ptr;
    Sound_Calculate_Level_Tables(mEmulatorP->NativeCPC->PlayCity->PSG_Left,
          mAudioContextP->PlayCityStereo, mAudioContextP->PlayCity16Bits);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormAudioSettings::CheckBoxPlayCityInvertClick(TObject *Sender)
{
  if (((TCheckBox*)Sender)->Focused() == true)
  {
    mSettingsP->AudioPlayCityInvert = ((TCheckBox*)Sender)->Checked;
    mEmulatorP->NativeCPC->prefP->PlayCityInvert = mSettingsP->AudioPlayCityInvert;
    if (mSettingsP->AudioPlayCityInvert)
    {
      mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerStereo16InvertPtr;
    }
    else
    {
      mEmulatorP->NativeCPC->PlayCitySynthetizerPtr = mEmulatorP->NativeCPC->PlayCitySynthetizerStereo16Ptr;
    }
  }
}
//---------------------------------------------------------------------------

