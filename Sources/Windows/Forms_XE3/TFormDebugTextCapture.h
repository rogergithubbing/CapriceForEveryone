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


#ifndef TFormDebugTextCaptureH
#define TFormDebugTextCaptureH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormDebugTextCapture : public TForm
{
__published:	// Composants gérés par l'EDI
  TMemo *MemoCapture;
  TPanel *PanelControls;
  TButton *ButtonStart;
  TButton *ButtonClear;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ButtonStartClick(TObject *Sender);
  void __fastcall ButtonClearClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  AnsiString mCurrentLine;

  //
  // Methods
  //
  void __fastcall SetSettings(TSettings* Value);
  bool __fastcall OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPCP);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };

  //
  // API
  //
  __fastcall TFormDebugTextCapture(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugTextCapture *FormDebugTextCapture;
//---------------------------------------------------------------------------
#endif
