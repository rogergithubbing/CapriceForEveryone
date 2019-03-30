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


#ifndef TFormDebugTapeAudioWorkshopH
#define TFormDebugTapeAudioWorkshopH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//#include <VclTee.TeeGDIPlus.hpp>
#include "..\Audio\TAudioContext.h"
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
#include <VclTee.TeeGDIPlus.hpp>

//---------------------------------------------------------------------------
class TFormDebugTapeAudioWorkshop : public TForm
{
__published:	// Composants gérés par l'EDI
  TCheckBox *CheckBoxCreateDebugOutputAudio;
  TButton *ButtonReloadAudio;
  TShape *ShapeLine8bits;
  TLabel *Label1;
  TPanel *PanelPPIThreshold8;
  TShape *ShapeLine8bitsEnd;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *EditPPIThreshold8;
  TPanel *Panel1;
  TLabel *Label4;
  TEdit *EditPPIThreshold16;
  TShape *ShapeLine16bitsEnd;
  TLabel *Label5;
  TShape *ShapeLine16bits;
  TLabel *Label6;
  TGroupBox *GroupBoxLPF1;
  TPanel *PanelAutoGain;
  TLabel *Label7;
  TPanel *PanelLPF1;
  TLabel *Label8;
  TGroupBox *GroupBoxLPF2;
  TGroupBox *GroupBoxHPF2;
  TLabel *LabelLPF1Param;
  TEdit *EditLPF1_A;
  TPanel *PanelLPF2;
  TLabel *Label9;
  TLabel *Label1LPF1Info;
  TLabel *LabelLPF2Info;
  TLabel *LabelHPF2Info;
  TLabel *LabelLPF2Order;
  TEdit *EditLPF2_Order;
  TLabel *LabelLPF2Freq;
  TEdit *EditLPF2_Freq;
  TLabel *LabelHPF2Order;
  TEdit *EditHPF2_Order;
  TLabel *LabelHPF2Freq;
  TEdit *EditHPF2_Freq;
  TLabel *Label18;
  TPanel *PanelHPF2;
  TLabel *LabelApplyNote;
  TButton *ButtonRestoreDefault;
  TGroupBox *GroupBoxFilterTest;
  TChart *ChartFilterGain;
  TLineSeries *Series1;
  TCheckBox *CheckBoxCreateCSW;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall EditLPF1_AKeyPress(TObject *Sender, char &Key);
  void __fastcall EditLPF1_AExit(TObject *Sender);
  void __fastcall EditPPIThreshold8KeyPress(TObject *Sender, char &Key);
  void __fastcall EditPPIThreshold8Exit(TObject *Sender);
  void __fastcall EditPPIThreshold16KeyPress(TObject *Sender, char &Key);
  void __fastcall EditPPIThreshold16Exit(TObject *Sender);
  void __fastcall CheckBoxCreateDebugOutputAudioClick(TObject *Sender);
  void __fastcall ButtonReloadAudioClick(TObject *Sender);
  void __fastcall PanelAutoGainClick(TObject *Sender);
  void __fastcall PanelLPF1Click(TObject *Sender);
  void __fastcall PanelLPF2Click(TObject *Sender);
  void __fastcall EditLPF2_OrderKeyPress(TObject *Sender, char &Key);
  void __fastcall EditLPF2_OrderExit(TObject *Sender);
  void __fastcall EditLPF2_FreqExit(TObject *Sender);
  void __fastcall EditLPF2_FreqKeyPress(TObject *Sender, char &Key);
  void __fastcall PanelHPF2Click(TObject *Sender);
  void __fastcall EditHPF2_OrderKeyPress(TObject *Sender, char &Key);
  void __fastcall EditHPF2_OrderExit(TObject *Sender);
  void __fastcall EditHPF2_FreqKeyPress(TObject *Sender, char &Key);
  void __fastcall EditHPF2_FreqExit(TObject *Sender);
  void __fastcall ButtonRestoreDefaultClick(TObject *Sender);
  void __fastcall CheckBoxCreateCSWClick(TObject *Sender);
private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  TEmulator* mEmulator;
  TAudioContext* mAudioContext;

  //
  // Methods
  //
  void __fastcall SetSettings(TSettings* Value);
  inline void __fastcall SetAudioContext(TAudioContext* Value) { mAudioContext=Value; };
  inline void __fastcall SetEmulator(TEmulator* Value) { mEmulator=Value; };
  void __fastcall WMMove(TMessage &Message);
  void __fastcall UpdateDisplay(void);
  void __fastcall UpdateGain(void);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property TEmulator* Emulator = { write=SetEmulator };
  __property TAudioContext* AudioContext = { write=SetAudioContext };


  //
  // API
  //
  __fastcall TFormDebugTapeAudioWorkshop(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugTapeAudioWorkshop *FormDebugTapeAudioWorkshop;
//---------------------------------------------------------------------------
#endif
