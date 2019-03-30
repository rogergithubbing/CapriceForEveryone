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


#ifndef TFormDebugMEA8000H
#define TFormDebugMEA8000H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include "..\Settings\TSettings.h"
#include "..\Audio\TAudioContext.h"
//---------------------------------------------------------------------------
class TFormDebugMEA8000 : public TForm
{
__published:	// Composants gérés par l'EDI
  TLabel *LabelState;
  TPanel *PanelState;
  TLabel *LabelPitch;
  TPanel *PanelPitch;
  TLabel *LabelAmplitude;
  TPanel *PanelAmplitude;
  TGroupBox *GroupBoxOutput;
  TChart *ChartSample;
  TFastLineSeries *Series1;
  TGroupBox *GroupBoxMonitoring;
  TPanel *PanelHistoryControls;
  TButton *ButtonMonitorStartStop;
  TButton *ButtonHistoryClear;
  TListBox *ListBoxHistory;
  TCheckBox *CheckBoxShowReadStatus;
  TCheckBox *CheckBoxShowSamples;
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ButtonMonitorStartStopClick(TObject *Sender);
  void __fastcall ButtonHistoryClearClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  TAudioContext* mAudioContext;
  bool mMonitoring;
  int mSampleCount;
  tTMPISpeechState mLastState;
  int mLastPitch;
  int mLastAmplitude;
   
  //
  // Methods
  //
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  inline void __fastcall SetAudioContext(TAudioContext* Value) { mAudioContext=Value; };
  void __fastcall InitCurrentState(tNativeCPC* NativeCPC);
  bool __fastcall OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property TAudioContext* AudioContext = { write=SetAudioContext };

  //
  // API
  //
  __fastcall TFormDebugMEA8000(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugMEA8000 *FormDebugMEA8000;
//---------------------------------------------------------------------------
#endif
