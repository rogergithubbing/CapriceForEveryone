/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2018 by Frédéric Coste

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


#ifndef TFormDebugEditValueH
#define TFormDebugEditValueH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormDebugEditValue : public TForm
{
__published:	// Composants gérés par l'EDI
  TLabel *LabelCurrent;
  TLabel *LabelNew;
  TButton *ButtonOK;
  TButton *ButtonCancel;
  TPanel *PanelCurrent;
  TPanel *PanelFormatNew;
  TPanel *PanelFormatCurrent;
  TButton *ButtonIncrement;
  TButton *ButtonDecrement;
  TPanel *PanelNew;
  TEdit *EditNew;
  TLabel *LabelCurrentValue;
  TPanel *PanelIncrement;
  TEdit *EditIncrement;
  TPanel *PanelNext;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall EditNewChange(TObject *Sender);
  void __fastcall PanelFormatNewClick(TObject *Sender);
  void __fastcall ButtonIncrementClick(TObject *Sender);
  void __fastcall ButtonDecrementClick(TObject *Sender);
  void __fastcall EditIncrementChange(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);
  void __fastcall PanelNextClick(TObject *Sender);
  
private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  int mCurrentValue;
  int mNewValue;
  bool mHexadecimal;
  int mNbDigits;
  int mMinValue;
  int mMaxValue;
  int mIncrement;
  bool mShowNext;
  bool mNextRequested;

  //
  // Methods
  //
  inline void __fastcall SetSettings(TSettings* Value) { mSettingsP = Value; };
  inline int __fastcall GetCurrentValue(void) { return mCurrentValue; };
  inline void __fastcall SetCurrentValue(int Value) { mCurrentValue = Value; };
  inline int __fastcall GetNewValue(void) { return mNewValue; };
  inline void __fastcall SetNbDigits(int Value) { mNbDigits = Value; };
  inline void __fastcall SetHexadecimal(bool Value) { mHexadecimal = Value; };
  inline bool __fastcall GetHexadecimal(void) { return mHexadecimal; };
  inline void __fastcall SetMinValue(int Value) { mMinValue = Value; };
  inline void __fastcall SetMaxValue(int Value) { mMaxValue = Value; };
  inline int __fastcall GetIncrement(void) { return mIncrement; };
  inline void __fastcall SetIncrement(int Value) { mIncrement = Value; };
  inline void __fastcall SetShowNext(bool Value) { mShowNext = Value; };
  inline bool __fastcall GetNextRequested(void) { return mNextRequested; };
  inline void __fastcall SetNextRequested(bool Value) { mNextRequested = Value; };

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property int CurrentValue = { read=GetCurrentValue, write=SetCurrentValue };
  __property int NewValue = { read=GetNewValue };
  __property bool Hexadecimal = { read=GetHexadecimal, write=SetHexadecimal };
  __property int NbDigits = { write=SetNbDigits };
  __property int MinValue = { write=SetMinValue };
  __property int MaxValue = { write=SetMaxValue };
  __property int Increment = { read=GetIncrement, write=SetIncrement };
  __property bool ShowNext = { write=SetShowNext };
  __property bool NextRequested = { read=GetNextRequested, write=SetNextRequested };

  //
  // API
  //
  __fastcall TFormDebugEditValue(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugEditValue *FormDebugEditValue;
//---------------------------------------------------------------------------
#endif
