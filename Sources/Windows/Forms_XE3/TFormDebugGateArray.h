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


#ifndef TFormDebugGateArrayH
#define TFormDebugGateArrayH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormDebugGateArray : public TForm
{
__published:	// Composants gérés par l'EDI
  TGroupBox *GroupBoxPalette;
  TLabel *LabelInk0;
  TRadioButton *RadioButtonInk0;
  TPanel *PanelInkColor0;
  TPanel *PanelInk0;
  TRadioButton *RadioButtonInkBorder;
  TLabel *LabelInkBorder;
  TPanel *PanelInkBorder;
  TPanel *PanelInkColorBorder;
  TGroupBox *GroupBoxModes;
  TPanel *PanelCurrentMode;
  TPanel *PanelRequestedMode;
  TLabel *LabelCurrentMode;
  TLabel *LabelRequestedMode;
  TRadioButton *RadioButtonInk1;
  TLabel *LabelInk1;
  TPanel *PanelInk1;
  TPanel *PanelInkColor1;
  TRadioButton *RadioButtonInk2;
  TLabel *LabelInk2;
  TPanel *PanelInk2;
  TPanel *PanelInkColor2;
  TRadioButton *RadioButtonInk3;
  TLabel *LabelInk3;
  TPanel *PanelInk3;
  TPanel *PanelInkColor3;
  TPanel *PanelInkName0;
  TPanel *PanelInkName1;
  TPanel *PanelInkName2;
  TPanel *PanelInkName3;
  TPanel *PanelInkNameBorder;
  TRadioButton *RadioButtonInk4;
  TLabel *LabelInk4;
  TPanel *PanelInk4;
  TPanel *PanelInkColor4;
  TPanel *PanelInkName4;
  TRadioButton *RadioButtonInk5;
  TLabel *LabelInk5;
  TPanel *PanelInk5;
  TPanel *PanelInkColor5;
  TPanel *PanelInkName5;
  TRadioButton *RadioButtonInk6;
  TLabel *LabelInk6;
  TPanel *PanelInk6;
  TPanel *PanelInkColor6;
  TPanel *PanelInkName6;
  TRadioButton *RadioButtonInk7;
  TLabel *LabelInk7;
  TPanel *PanelInk7;
  TPanel *PanelInkColor7;
  TPanel *PanelInkName7;
  TRadioButton *RadioButtonInk8;
  TLabel *LabelInk8;
  TPanel *PanelInk8;
  TPanel *PanelInkColor8;
  TPanel *PanelInkName8;
  TRadioButton *RadioButtonInk9;
  TLabel *LabelInk9;
  TPanel *PanelInk9;
  TPanel *PanelInkColor9;
  TPanel *PanelInkName9;
  TRadioButton *RadioButtonInk10;
  TLabel *LabelInk10;
  TPanel *PanelInk10;
  TPanel *PanelInkColor10;
  TPanel *PanelInkName10;
  TRadioButton *RadioButtonInk11;
  TLabel *LabelInk11;
  TPanel *PanelInk11;
  TPanel *PanelInkColor11;
  TPanel *PanelInkName11;
  TRadioButton *RadioButtonInk12;
  TLabel *LabelInk12;
  TPanel *PanelInk12;
  TPanel *PanelInkColor12;
  TPanel *PanelInkName12;
  TRadioButton *RadioButtonInk13;
  TLabel *LabelInk13;
  TPanel *PanelInk13;
  TPanel *PanelInkColor13;
  TPanel *PanelInkName13;
  TRadioButton *RadioButtonInk14;
  TLabel *LabelInk14;
  TPanel *PanelInk14;
  TPanel *PanelInkColor14;
  TPanel *PanelInkName14;
  TRadioButton *RadioButtonInk15;
  TLabel *LabelInk15;
  TPanel *PanelInk15;
  TPanel *PanelInkColor15;
  TPanel *PanelInkName15;
  TGroupBox *GroupBoxRAMState;
  TLabel *LabelStateRAMconfig;
  TPanel *PanelRAMConfig;
  TGroupBox *GroupBoxROMState;
  TLabel *LabelStateROMConfig;
  TLabel *LabelUpperROM;
  TCheckBox *CheckBoxLowerROMEnabled;
  TCheckBox *CheckBoxUpperROMEnabled;
  TPanel *PanelROMConfig;
  TPanel *PanelUpperROM;
  TLabel *LabelLowerROMEnabled;
  TLabel *LabelUpperROMEnabled;
  TGroupBox *GroupBoxReadBanks;
  TLabel *LabelReadBank0;
  TPanel *PanelReadBank0;
  TLabel *LabelReadBank1;
  TPanel *PanelReadBank1;
  TLabel *LabelReadBank2;
  TPanel *PanelReadBank2;
  TLabel *LabelReadBank3;
  TPanel *PanelReadBank3;
  TGroupBox *GroupBoxWriteBanks;
  TLabel *LabelWriteBank0;
  TLabel *LabelWriteBank1;
  TLabel *LabelWriteBank2;
  TLabel *LabelWriteBank3;
  TPanel *PanelWriteBank0;
  TPanel *PanelWriteBank1;
  TPanel *PanelWriteBank2;
  TPanel *PanelWriteBank3;
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall PanelInkClick(TObject *Sender);
  void __fastcall PanelInkColorClick(TObject *Sender);
  void __fastcall PanelRequestedModeClick(TObject *Sender);
private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  bool mPaused;
  TRadioButton* mRadioButtonInks[GA_INK_VALUES_NBELEMENT];
  TPanel* mPanelInkValue[GA_INK_VALUES_NBELEMENT];
  TPanel* mPanelInkColor[GA_INK_VALUES_NBELEMENT];
  TPanel* mPanelInkName[GA_INK_VALUES_NBELEMENT];
  int mPreviousInk[GA_INK_VALUES_NBELEMENT];
  int mPreviousPen;
  int mPreviousScreenMode;
  int mPreviousRequestedScreenMode;
  int mPreviousRAMSize;
  int mPreviousRAMSettings;
  int mPreviousROMSettings;
  int mPreviousUpperROM;

  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  void __fastcall SetSettings(TSettings* Value);
  void __fastcall WMMove(TMessage &Message);
  bool __fastcall OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPC);
  void __fastcall InitPalette(tNativeCPC* NativeCPC);
  void __fastcall UpdatePalette(tNativeCPC* NativeCPC);
  void __fastcall InitScreenMode(tNativeCPC* NativeCPC);
  void __fastcall UpdateScreenMode(tNativeCPC* NativeCPC);
  void __fastcall InitRAMSettings(tNativeCPC* NativeCPC);
  void __fastcall UpdateRAMSettings(tNativeCPC* NativeCPC);
  void __fastcall InitROMSettings(tNativeCPC* NativeCPC);
  void __fastcall UpdateROMSettings(tNativeCPC* NativeCPC);
  void __fastcall UpdateReadBanks(tNativeCPC* NativeCPC);
  void __fastcall UpdateWriteBanks(tNativeCPC* NativeCPC);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property AnsiString Title = { read=GetTitle };
  __property TSettings* Settings = { write=SetSettings };

  //
  // API
  //
  __fastcall TFormDebugGateArray(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugGateArray *FormDebugGateArray;
//---------------------------------------------------------------------------
#endif
