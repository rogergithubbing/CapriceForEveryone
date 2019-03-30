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


#ifndef TFormAudioSettingsH
#define TFormAudioSettingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
#include "..\Audio\TAudioContext.h"
#include "..\Emulator\TEmulator.h"
//---------------------------------------------------------------------------
class TFormAudioSettings : public TForm
{
__published:	// Composants gérés par l'EDI
  TButton *ButtonCancel;
  TButton *ButtonOK;
  TGroupBox *GroupBoxDevice;
  TGroupBox *GroupBoxGainSpeaker;
  TTrackBar *TrackBarGainSpeaker;
  TEdit *EditGainSpeaker;
  TGroupBox *GroupBoxGainFloppy;
  TTrackBar *TrackBarGainFloppy;
  TEdit *EditGainFloppy;
  TGroupBox *GroupBoxGainTape;
  TTrackBar *TrackBarGainTape;
  TEdit *EditGainTape;
  TGroupBox *GroupBoxGainTMPI;
  TTrackBar *TrackBarGainTMPI;
  TEdit *EditGainTMPI;
  TLabel *LabelSpeaker;
  TLabel *LabelFloppy;
  TLabel *LabelTape;
  TLabel *LabelTMPI;
  TGroupBox *GroupBoxGainPlayCity;
  TTrackBar *TrackBarGainPlayCity;
  TEdit *EditGainPlayCity;
  TLabel *LabelPlayCity;
  TComboBox *ComboBoxDevices;
  TRadioButton *RadioButtonPlayCityStereo;
  TRadioButton *RadioButtonPlayCityMono;
  TRadioButton *RadioButtonSpeakerStereo;
  TRadioButton *RadioButtonSpeakerMono;
  TCheckBox *CheckBoxPlayCityInvert;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall TrackBarGainSpeakerChange(TObject *Sender);
  void __fastcall EditGainSpeakerChange(TObject *Sender);
  void __fastcall ButtonOKClick(TObject *Sender);
  void __fastcall TrackBarGainFloppyChange(TObject *Sender);
  void __fastcall EditGainFloppyChange(TObject *Sender);
  void __fastcall EditGainSpeakerClick(TObject *Sender);
  void __fastcall EditGainFloppyClick(TObject *Sender);
  void __fastcall TrackBarGainTapeChange(TObject *Sender);
  void __fastcall EditGainTapeChange(TObject *Sender);
  void __fastcall EditGainTapeClick(TObject *Sender);
  void __fastcall TrackBarGainTMPIChange(TObject *Sender);
  void __fastcall EditGainTMPIChange(TObject *Sender);
  void __fastcall EditGainTMPIClick(TObject *Sender);
  void __fastcall ButtonCancelClick(TObject *Sender);
  void __fastcall TrackBarGainPlayCityChange(TObject *Sender);
  void __fastcall EditGainPlayCityChange(TObject *Sender);
  void __fastcall EditGainPlayCityClick(TObject *Sender);
  void __fastcall ComboBoxDevicesDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall RadioButtonSpeakerStereoClick(TObject *Sender);
  void __fastcall RadioButtonSpeakerMonoClick(TObject *Sender);
  void __fastcall RadioButtonPlayCityStereoClick(TObject *Sender);
  void __fastcall RadioButtonPlayCityMonoClick(TObject *Sender);
  void __fastcall CheckBoxPlayCityInvertClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  TAudioContext* mAudioContextP;
  TEmulator* mEmulatorP;
  bool mOldSpeakerStereo;
  bool mOldPlayCityStereo;
  bool mOldPlayCityInvert;

  //
  // Methods
  //
  inline void SetSettings(TSettings* Value) { mSettingsP = Value; };
  inline void SetAudioContext(TAudioContext* Value) { mAudioContextP = Value; };
  inline void SetEmulator(TEmulator* Value) { mEmulatorP = Value; };

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property TAudioContext* AudioContext = { write=SetAudioContext };
  __property TEmulator* Emulator = { write=SetEmulator };

  //
  // API
  //
  __fastcall TFormAudioSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAudioSettings *FormAudioSettings;
//---------------------------------------------------------------------------
#endif
