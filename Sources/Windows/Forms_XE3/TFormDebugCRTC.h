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


#ifndef TFormDebugCRTCH
#define TFormDebugCRTCH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormDebugCRTC : public TForm
{
__published:	// Composants gérés par l'EDI
  TGroupBox *GroupBoxRegisters;
  TRadioButton *RadioButtonR17;
  TRadioButton *RadioButtonR0;
  TRadioButton *RadioButtonR1;
  TLabel *LabelR0;
  TLabel *LabelR1;
  TLabel *LabelR17;
  TRadioButton *RadioButtonR2;
  TLabel *LabelR2;
  TRadioButton *RadioButtonR9;
  TLabel *LabelR9;
  TRadioButton *RadioButtonR10;
  TLabel *LabelR10;
  TRadioButton *RadioButtonR4;
  TLabel *LabelR4;
  TRadioButton *RadioButtonR5;
  TLabel *LabelR5;
  TRadioButton *RadioButtonR6;
  TLabel *LabelR6;
  TRadioButton *RadioButtonR7;
  TLabel *LabelR7;
  TRadioButton *RadioButtonR8;
  TLabel *LabelR8;
  TRadioButton *RadioButtonR11;
  TLabel *LabelR11;
  TRadioButton *RadioButtonR3;
  TLabel *LabelR3;
  TRadioButton *RadioButtonR12;
  TLabel *LabelR12;
  TRadioButton *RadioButtonR13;
  TLabel *LabelR13;
  TRadioButton *RadioButtonR14;
  TLabel *LabelR14;
  TRadioButton *RadioButtonR15;
  TLabel *LabelR15;
  TRadioButton *RadioButtonR16;
  TLabel *LabelR16;
  TGroupBox *GroupBoxStatus;
  TCheckBox *CheckBoxVBL;
  TLabel *LabelVBL;
  TCheckBox *CheckBoxHBL;
  TLabel *LabelHBL;
  TLabel *LabelCharCount;
  TLabel *LabelLineCount;
  TLabel *LabelRasterCount;
  TPanel *PanelFormatR0;
  TPanel *PanelFormatR1;
  TPanel *PanelFormatR2;
  TPanel *PanelFormatR3;
  TPanel *PanelFormatR4;
  TPanel *PanelFormatR5;
  TPanel *PanelFormatR6;
  TPanel *PanelFormatR7;
  TPanel *PanelFormatR8;
  TPanel *PanelFormatR9;
  TPanel *PanelFormatR10;
  TPanel *PanelFormatR11;
  TPanel *PanelFormatR12;
  TPanel *PanelFormatR13;
  TPanel *PanelFormatR14;
  TPanel *PanelFormatR15;
  TPanel *PanelFormatR16;
  TPanel *PanelFormatR17;
  TPanel *PanelR0;
  TPanel *PanelR1;
  TPanel *PanelR2;
  TPanel *PanelR3;
  TPanel *PanelR4;
  TPanel *PanelR5;
  TPanel *PanelR6;
  TPanel *PanelR7;
  TPanel *PanelR8;
  TPanel *PanelR9;
  TPanel *PanelR10;
  TPanel *PanelR11;
  TPanel *PanelR12;
  TPanel *PanelR13;
  TPanel *PanelR14;
  TPanel *PanelR15;
  TPanel *PanelR16;
  TPanel *PanelR17;
  TPanel *PanelCharCount;
  TPanel *PanelLineCount;
  TPanel *PanelRasterCount;
  TGroupBox *GroupBoxPosition;
  TLabel *LabelBeamPosX;
  TPanel *PanelBeamPosX;
  TLabel *LabelBeamPosY;
  TPanel *PanelBeamPosY;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall PanelFormatClick(TObject *Sender);
  void __fastcall PanelRegClick(TObject *Sender);
private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  bool mPaused;
  bool mStepByStep;
  tULong mLastDisplayedRegistersP[CRTC_REGISTERS_NBELEMENT];
  tULong mLastDisplayedRegisterSelected;
  tULong mPreviousRegistersP[CRTC_REGISTERS_NBELEMENT];
  bool mDisplayValueHexa[CRTC_REGISTERS_NBELEMENT];
  TRadioButton* mRadioButtonRegister[CRTC_REGISTERS_NBELEMENT];
  TPanel* mPanelRegisterFormat[CRTC_REGISTERS_NBELEMENT];
  TPanel* mPanelRegister[CRTC_REGISTERS_NBELEMENT];
  TLabel* mLabelRegister[CRTC_REGISTERS_NBELEMENT];
  
  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  bool __fastcall OnCPCEvent(tULong Condition, tNativeCPC* NativeCPCP);
  void __fastcall InitRegisters(tNativeCPC* NativeCPC);
  void __fastcall UpdateRegisters(tNativeCPC* NativeCPC);
  void __fastcall UpdateStatus(tNativeCPC* NativeCPC);
  void __fastcall UpdatePosition(tNativeCPC* NativeCPC);
  void __fastcall ClearStatus(void);
  void __fastcall ClearPosition(void);
  void __fastcall UpdateRegister(int Index);
  void __fastcall UpdateRegisterFormat(int Index);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property AnsiString Title = { read=GetTitle };
  __property TSettings* Settings = { write=SetSettings };
  //
  // API
  //
  __fastcall TFormDebugCRTC(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugCRTC *FormDebugCRTC;
//---------------------------------------------------------------------------
#endif
