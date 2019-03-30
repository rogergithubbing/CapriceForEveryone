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

#include "TFormDebugGateArray.h"
#include "TFormMain.h"
#include "TFormDebugEditValue.h"
#include "TFormDebugBASICColors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


TFormDebugGateArray *FormDebugGateArray;


const AnsiString cTitle = "Gate Array";
const AnsiString cTitlePaused  = " - [PAUSED]";


static const char* GateArrayColorsName[] =
{
/*  0 */ "White",
/*  1 */ "White",
/*  2 */ "Sea Green",
/*  3 */ "Pastel Yellow",
/*  4 */ "Blue",
/*  5 */ "Purple",
/*  6 */ "Cyan",
/*  7 */ "Pink",
/*  8 */ "Purple",
/*  9 */ "Pastel Yellow",
/* 10 */ "Bright Yellow",
/* 11 */ "Bright White",
/* 12 */ "Bright Red",
/* 13 */ "Bright Magenta",
/* 14 */ "Orange",
/* 15 */ "Pastel Magenta",
/* 16 */ "Blue",
/* 17 */ "Sea Green",
/* 18 */ "Bright Green",
/* 19 */ "Bright Cyan",
/* 20 */ "Black",
/* 21 */ "Bright Blue",
/* 22 */ "Green",
/* 23 */ "Sky Blue",
/* 24 */ "Magenta",
/* 25 */ "Pastel Green",
/* 26 */ "Lime Green",
/* 27 */ "Pastel Cyan",
/* 28 */ "Red",
/* 29 */ "Mauve",
/* 30 */ "Yellow",
/* 31 */ "Pastel Blue",
};


//---------------------------------------------------------------------------
__fastcall TFormDebugGateArray::TFormDebugGateArray(TComponent* Owner)
  : TForm(Owner)
{
  // Init component lists
  for (int Loop=0; Loop < 16; Loop++)
  {
    mRadioButtonInks[Loop] = (TRadioButton*)FindComponent("RadioButtonInk" + AnsiString(Loop));
    mPanelInkValue[Loop] = (TPanel*)FindComponent("PanelInk" + AnsiString(Loop));
    mPanelInkColor[Loop] = (TPanel*)FindComponent("PanelInkColor" + AnsiString(Loop));
    mPanelInkName[Loop] = (TPanel*)FindComponent("PanelInkName" + AnsiString(Loop));
  }
  mRadioButtonInks[16]      = RadioButtonInkBorder;
  mPanelInkValue[16]        = PanelInkBorder;
  mPanelInkColor[16]        = PanelInkColorBorder;
  mPanelInkName[16]         = PanelInkNameBorder;

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  PanelInkBorder->Font->Color = clBlack;
  PanelInk0->Font->Color = clBlack;
  PanelInk1->Font->Color = clBlack;
  PanelInk2->Font->Color = clBlack;
  PanelInk3->Font->Color = clBlack;
  PanelInk4->Font->Color = clBlack;
  PanelInk5->Font->Color = clBlack;
  PanelInk6->Font->Color = clBlack;
  PanelInk7->Font->Color = clBlack;
  PanelInk8->Font->Color = clBlack;
  PanelInk9->Font->Color = clBlack;
  PanelInk10->Font->Color = clBlack;
  PanelInk11->Font->Color = clBlack;
  PanelInk12->Font->Color = clBlack;
  PanelInk13->Font->Color = clBlack;
  PanelInk14->Font->Color = clBlack;
  PanelInk15->Font->Color = clBlack;
  PanelRequestedMode->Font->Color = clBlack;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::FormShow(TObject *Sender)
{
  // Subscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->SubscribeToCPCEvents(OnStepByStepEvent);

    tNativeCPC* NativeCPC = FormMain->EmulatorP->NativeCPC;

    // Display palette
    InitPalette(NativeCPC);
    // Display current mode
    InitScreenMode(NativeCPC);
    InitRAMSettings(NativeCPC);
    InitROMSettings(NativeCPC);
    UpdateReadBanks(NativeCPC);
    UpdateWriteBanks(NativeCPC);

    mPaused = FormMain->NativeCPC->paused;
    Caption = Title;
  }
  else
  {
    Close();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnStepByStepEvent);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDebugGateArray::GetTitle(void)
{
  AnsiString Title = cTitle;
  if (mPaused == true) Title += cTitlePaused;
 return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->GateArrayFormLeft != 0)
       || (mSettingsP->GateArrayFormTop != 0)
       /*|| (mSettingsP->GateArrayFormWidth != 0)
       || (mSettingsP->GateArrayFormHeight != 0)*/ )
  {
    Position = poDesigned;
    Left = mSettingsP->GateArrayFormLeft;
    Top = mSettingsP->GateArrayFormTop;
    //Width = mSettingsP->GateArrayFormWidth;
    //Height = mSettingsP->GateArrayFormHeight;
  }

  // Apply theme
  if (mSettingsP)
  {
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
    PanelInkNameBorder->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName0->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName1->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName2->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName3->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName4->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName5->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName6->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName7->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName8->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName9->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName10->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName11->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName12->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName13->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName14->Font->Color = mSettingsP->ThemeText2Color;
    PanelInkName15->Font->Color = mSettingsP->ThemeText2Color;
    LabelRequestedMode->Font->Color = mSettingsP->ThemeText2Color;
    LabelLowerROMEnabled->Font->Color = mSettingsP->ThemeText2Color;
    LabelUpperROMEnabled->Font->Color = mSettingsP->ThemeText2Color;
    PanelReadBank0->Font->Color = mSettingsP->ThemeText2Color;
    PanelReadBank1->Font->Color = mSettingsP->ThemeText2Color;
    PanelReadBank2->Font->Color = mSettingsP->ThemeText2Color;
    PanelReadBank3->Font->Color = mSettingsP->ThemeText2Color;
    PanelWriteBank0->Font->Color = mSettingsP->ThemeText2Color;
    PanelWriteBank1->Font->Color = mSettingsP->ThemeText2Color;
    PanelWriteBank2->Font->Color = mSettingsP->ThemeText2Color;
    PanelWriteBank3->Font->Color = mSettingsP->ThemeText2Color;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->GateArrayFormWidth = Width;
    mSettingsP->GateArrayFormHeight = Height;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugGateArray::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->GateArrayFormLeft = Left;
    mSettingsP->GateArrayFormTop = Top;
  }

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::InitPalette(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Initialiser inks value
  for (int Index=0; Index < GA_INK_VALUES_NBELEMENT; Index++)
  {
    mPreviousInk[Index] = GateArray->ink_values[Index];

    mPanelInkColor[Index]->Color = (TColor)RGB(NativeCPC->colours_rgb[mPreviousInk[Index]].colour_r,
                                               NativeCPC->colours_rgb[mPreviousInk[Index]].colour_g,
                                               NativeCPC->colours_rgb[mPreviousInk[Index]].colour_b);
    mPanelInkValue[Index]->Caption = AnsiString(GateArray->BasicColorsP[mPreviousInk[Index]])
                                     + "(" + AnsiString(mPreviousInk[Index]) + ")";
    mPanelInkName[Index]->Caption = AnsiString(GateArrayColorsName[mPreviousInk[Index]]);
  }

  mPreviousPen = GateArray->pen;
  mRadioButtonInks[mPreviousPen]->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugGateArray::UpdatePalette(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Update inks value
  for (int Index=0; Index < GA_INK_VALUES_NBELEMENT; Index++)
  {
    int Ink = GateArray->ink_values[Index];
    if (Ink != mPreviousInk[Index])
    {
      mPreviousInk[Index] =  Ink;

      mPanelInkColor[Index]->Color = (TColor)RGB(NativeCPC->colours_rgb[Ink].colour_r,
                                                 NativeCPC->colours_rgb[Ink].colour_g,
                                                 NativeCPC->colours_rgb[Ink].colour_b);
      mPanelInkValue[Index]->Caption = AnsiString(GateArray->BasicColorsP[Ink]) + "(" + AnsiString(Ink) + ")";
      mPanelInkName[Index]->Caption = AnsiString(GateArrayColorsName[Ink]);
    }
  }

  // Update selected pen
  if (mPreviousPen != (int)GateArray->pen)
  {
    mPreviousPen = GateArray->pen;
    mRadioButtonInks[mPreviousPen]->Checked = true;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::InitScreenMode(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Display current mode
  mPreviousScreenMode = GateArray->scr_mode;
  mPreviousRequestedScreenMode = GateArray->requested_scr_mode;
  PanelCurrentMode->Caption = AnsiString(mPreviousScreenMode);
  PanelRequestedMode->Caption = AnsiString(mPreviousRequestedScreenMode);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugGateArray::UpdateScreenMode(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Update current mode
  if (mPreviousScreenMode != (int)GateArray->scr_mode)
  {
    mPreviousScreenMode = GateArray->scr_mode;
    PanelCurrentMode->Caption = AnsiString(mPreviousScreenMode);
  }

  if (mPreviousRequestedScreenMode != (int)GateArray->requested_scr_mode)
  {
    mPreviousRequestedScreenMode = GateArray->requested_scr_mode;
    PanelRequestedMode->Caption = AnsiString(mPreviousRequestedScreenMode);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::InitRAMSettings(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Display RAM config
  mPreviousRAMSettings = (int)GateArray->RAM_config;
  PanelRAMConfig->Caption = "&&" + AnsiString::IntToHex(mPreviousRAMSettings, 2);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugGateArray::UpdateRAMSettings(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Display RAM config
  if (mPreviousRAMSettings != (int)GateArray->RAM_config)
  {
    mPreviousRAMSettings = (int)GateArray->RAM_config;
    PanelRAMConfig->Caption = "&&" + AnsiString::IntToHex(mPreviousRAMSettings, 2);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::InitROMSettings(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Display ROM config
  mPreviousROMSettings = (int)GateArray->ROM_config;
  PanelROMConfig->Caption = "&&" + AnsiString::IntToHex(mPreviousROMSettings, 2);
  CheckBoxLowerROMEnabled->Checked = (mPreviousROMSettings & 0x04) ? false : true;
  CheckBoxUpperROMEnabled->Checked = (mPreviousROMSettings & 0x08) ? false : true;
  mPreviousUpperROM = (int)GateArray->upper_ROM;
  PanelUpperROM->Caption = AnsiString(mPreviousUpperROM);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugGateArray::UpdateROMSettings(tNativeCPC* NativeCPC)
{
tGateArray* GateArray = NativeCPC->GateArray;

  // Update ROM config
  if (mPreviousROMSettings != (int)GateArray->ROM_config)
  {
    mPreviousROMSettings = (int)GateArray->ROM_config;
    
    PanelROMConfig->Caption = "&&" + AnsiString::IntToHex(mPreviousROMSettings, 2);
    CheckBoxLowerROMEnabled->Checked = (mPreviousROMSettings & 0x04) ? false : true;
    CheckBoxUpperROMEnabled->Checked = (mPreviousROMSettings & 0x08) ? false : true;
  }

  // Update Upper ROM
  if (mPreviousUpperROM != (int)GateArray->upper_ROM)
  {
    mPreviousUpperROM = (int)GateArray->upper_ROM;
    PanelUpperROM->Caption = AnsiString(mPreviousUpperROM);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::UpdateReadBanks(tNativeCPC* NativeCPC)
{
  // Bank 0
  if (NativeCPC->membank_read[0] == NativeCPC->pbROMlo)
  {
    PanelReadBank0->Caption = "Lower ROM";
  }
  else
  {
    int Address = NativeCPC->membank_read[0] - NativeCPC->pbRAM;
    PanelReadBank0->Caption = "&&" + AnsiString::IntToHex(Address, 4);
  }

  // Bank 1
  int Address = NativeCPC->membank_read[1] - NativeCPC->pbRAM;
  PanelReadBank1->Caption = "&&" + AnsiString::IntToHex(Address, 4);

  // Bank 2
  Address = NativeCPC->membank_read[2] - NativeCPC->pbRAM;
  PanelReadBank2->Caption = "&&" + AnsiString::IntToHex(Address, 4);

  // Bank 3
  if (NativeCPC->membank_read[3] == NativeCPC->pbExpansionROM)
  {
    PanelReadBank3->Caption = "Upper ROM";
  }
  else
  {
    int Address = NativeCPC->membank_read[3] - NativeCPC->pbRAM;
    PanelReadBank3->Caption = "&&" + AnsiString::IntToHex(Address, 4);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::UpdateWriteBanks(tNativeCPC* NativeCPC)
{
  // Bank 0
  int Address = NativeCPC->membank_write[0] - NativeCPC->pbRAM;
  PanelWriteBank0->Caption = "&&" + AnsiString::IntToHex(Address, 4);

  // Bank 1
  Address = NativeCPC->membank_write[1] - NativeCPC->pbRAM;
  PanelWriteBank1->Caption = "&&" + AnsiString::IntToHex(Address, 4);

  // Bank 2
  Address = NativeCPC->membank_write[2] - NativeCPC->pbRAM;
  PanelWriteBank2->Caption = "&&" + AnsiString::IntToHex(Address, 4);

  // Bank 3
  Address = NativeCPC->membank_write[3] - NativeCPC->pbRAM;
  PanelWriteBank3->Caption = "&&" + AnsiString::IntToHex(Address, 4);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::PanelInkClick(TObject *Sender)
{
  tGateArray* GateArrayP = FormMain->EmulatorP->NativeCPC->GateArray;

  int Index = ((TPanel*)Sender)->Tag;
  int Ink = GateArrayP->ink_values[Index];

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->CurrentValue = GateArrayP->BasicColorsP[Ink];
  EditValue->Hexadecimal = false;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 31;
  if (EditValue->ShowModal() == mrOk)
  {
    tNativeCPC* NativeCPCP = FormMain->EmulatorP->NativeCPC;

    // Update new ink
    GateArrayP->ink_values[Index] = GateArrayP->GateArrayColorsP[EditValue->NewValue];
    GateArrayP->palette[Index] = NativeCPCP->colours[GateArrayP->ink_values[Index]];
    UpdatePalette(NativeCPCP);
  }
  delete EditValue;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::PanelInkColorClick(TObject *Sender)
{
  tGateArray* GateArrayP = FormMain->EmulatorP->NativeCPC->GateArray;

  int Index = ((TPanel*)Sender)->Tag;
  int Ink = GateArrayP->ink_values[Index];

  TFormDebugBASICColors* EditColor = new TFormDebugBASICColors(this);
  EditColor->Settings = mSettingsP;
  EditColor->SelectedColor = GateArrayP->BasicColorsP[Ink];
  if (EditColor->ShowModal() == mrOk)
  {
    int NewColor = GateArrayP->GateArrayColorsP[EditColor->SelectedColor];
    tNativeCPC* NativeCPCP = FormMain->EmulatorP->NativeCPC;

    // Update new ink
    GateArrayP->ink_values[Index] = NewColor;
    GateArrayP->palette[Index] = NativeCPCP->colours[NewColor];
    UpdatePalette(NativeCPCP);
  }
  delete EditColor;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGateArray::PanelRequestedModeClick(
      TObject *Sender)
{
  tGateArray* GateArrayP = FormMain->EmulatorP->NativeCPC->GateArray;

  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->CurrentValue = GateArrayP->requested_scr_mode;
  EditValue->Hexadecimal = false;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 2;
  if (EditValue->ShowModal() == mrOk)
  {
    GateArrayP->requested_scr_mode = EditValue->NewValue;
    UpdateScreenMode(FormMain->EmulatorP->NativeCPC);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugGateArray::OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  // Update palette
  if (NativeCPC->GateArray->signal_palette_update)
  {
    NativeCPC->GateArray->signal_palette_update = 0;
    UpdatePalette(NativeCPC);
  }

  UpdateScreenMode(NativeCPC);
  UpdateRAMSettings(NativeCPC);
  UpdateROMSettings(NativeCPC);
  UpdateReadBanks(NativeCPC);
  UpdateWriteBanks(NativeCPC);

  if (Pause != mPaused)
  {
    mPaused = Pause;
    Caption = Title;
  }

  if ( ((Condition & EC_END_INSTRUCTION) == 0) && (Pause == false) )
    return false;

  return Pause;
}
//---------------------------------------------------------------------------

