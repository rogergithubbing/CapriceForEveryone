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


#ifndef TFormDebugPSGH
#define TFormDebugPSGH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
//roger canviat al final peta pel puto algorithm.. estava 3 cops !!!
//#include <VclTee.TeeGDIPlus.hpp>

#include "..\Settings\TSettings.h"
#include "..\Audio\TAudioContext.h"
#include <VclTee.TeeGDIPlus.hpp>

//---------------------------------------------------------------------------
class TFormDebugPSG : public TForm
{
__published:	// Composants gérés par l'EDI
  TGroupBox *GroupBoxMonitor;
  TGroupBox *GroupBoxLeftSample;
  TLabel *LabelLeftSampleMax;
  TLabel *LabelLeftSample;
  TPanel *PanelLeftSample;
  TPanel *PanelLeftSampleMax;
  TChart *ChartLeft;
  TFastLineSeries *Series1;
  TGroupBox *GroupBoxRightSample;
  TLabel *LabelRightSampleMax;
  TLabel *LabelRightSample;
  TPanel *PanelRightSample;
  TPanel *PanelRightSampleMax;
  TChart *ChartRight;
  TFastLineSeries *FastLineSeries1;
  TButton *ButtonMonitorStartStop;
  TGroupBox *GroupBoxRegisters;
  TLabel *LabelR0;
  TLabel *LabelR1;
  TLabel *LabelR15;
  TLabel *LabelR2;
  TLabel *LabelR8;
  TLabel *LabelR4;
  TLabel *LabelR5;
  TLabel *LabelR6;
  TLabel *LabelR7;
  TLabel *LabelR9;
  TLabel *LabelR3;
  TLabel *LabelR10;
  TLabel *LabelR11;
  TLabel *LabelR12;
  TLabel *LabelR13;
  TLabel *LabelR14;
  TRadioButton *RadioButtonR15;
  TRadioButton *RadioButtonR0;
  TRadioButton *RadioButtonR1;
  TRadioButton *RadioButtonR2;
  TRadioButton *RadioButtonR8;
  TRadioButton *RadioButtonR4;
  TRadioButton *RadioButtonR5;
  TRadioButton *RadioButtonR6;
  TRadioButton *RadioButtonR7;
  TRadioButton *RadioButtonR9;
  TRadioButton *RadioButtonR3;
  TRadioButton *RadioButtonR10;
  TRadioButton *RadioButtonR11;
  TRadioButton *RadioButtonR12;
  TRadioButton *RadioButtonR13;
  TRadioButton *RadioButtonR14;
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
  TGroupBox *GroupBoxChannels;
  TPanel *PanelChannelAFreq;
  TPanel *PanelChannelBFreq;
  TPanel *PanelChannelCFreq;
  TPanel *PanelChannelANoise;
  TPanel *PanelChannelBNoise;
  TPanel *PanelChannelCNoise;
  TProgressBar *ProgressBarChannelAAmplitude;
  TProgressBar *ProgressBarChannelBAmplitude;
  TProgressBar *ProgressBarChannelCAmplitude;
  TGroupBox *GroupBoxEnvelope;
  TPanel *PanelEnveloppeFreq;
  TChart *ChartEnvelope;
  TFastLineSeries *FastLineSeries2;
  TPanel *PanelChannelAEnv;
  TPanel *PanelChannelBEnv;
  TPanel *PanelChannelCEnv;
  TPanel *PanelChannelA;
  TPanel *PanelChannelB;
  TPanel *PanelChannelC;
  TGroupBox *GroupBoxNoise;
  TPanel *PanelNoiseFreq;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall PanelLeftSampleMaxClick(TObject *Sender);
  void __fastcall PanelRightSampleMaxClick(TObject *Sender);
  void __fastcall ButtonMonitorStartStopClick(TObject *Sender);
  void __fastcall PanelFormatClick(TObject *Sender);
  void __fastcall PanelChannelAClick(TObject *Sender);
  void __fastcall PanelChannelBClick(TObject *Sender);
  void __fastcall PanelChannelCClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  bool mMonitoring;
  bool mPaused;
  TSettings* mSettingsP;
  TAudioContext* mAudioContextP;
  tLong mLeftSampleMax;
  tLong mRightSampleMax;
  int mLoopLeft;
  int mLoopRight;
  int mLastEnvelopeShape;
  tULong mLastDisplayedRegisterSelected;
  tULong mLastDisplayedRegistersP[PSG_REGISTERS_NBELEMENT];
  bool mDisplayValueHexa[PSG_REGISTERS_NBELEMENT];
  TRadioButton* mRadioButtonRegister[PSG_REGISTERS_NBELEMENT];
  TPanel* mPanelRegisterFormat[PSG_REGISTERS_NBELEMENT];
  TPanel* mPanelRegister[PSG_REGISTERS_NBELEMENT];
  TLabel* mLabelRegister[PSG_REGISTERS_NBELEMENT];

  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  inline void __fastcall SetAudioContext(TAudioContext* Value) { mAudioContextP=Value; };
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  bool __fastcall OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC);
  void __fastcall InitRegisters(tNativeCPC* NativeCPC);
  void __fastcall UpdateRegisters(tNativeCPC* NativeCPC);
  void __fastcall UpdateRegisterFormat(int Index);
  void __fastcall UpdateRegister(int Index);
  void __fastcall UpdateChannels(tPSG* PSG);
  void __fastcall UpdateEnveloppe(tPSG* PSG);
  void __fastcall UpdateNoise(tPSG* PSG);
  void __fastcall StopMonitoring(void);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property AnsiString Title = { read=GetTitle };
  __property TSettings* Settings = { write=SetSettings };
  __property TAudioContext* AudioContext = { write=SetAudioContext };

  //
  // API
  //
  __fastcall TFormDebugPSG(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugPSG *FormDebugPSG;
//---------------------------------------------------------------------------
#endif
