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


#ifndef TFormDebuggerH
#define TFormDebuggerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormDebugger : public TForm
{
__published:	// Composants gérés par l'EDI
  TButton *ButtonPlayTillVBL;
  TButton *ButtonPlayTillHBL;
  TButton *ButtonStep;
  TButton *ButtonPlay;
  TButton *ButtonRun;
        void __fastcall FormResize(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ButtonPlayTillVBLClick(TObject *Sender);
  void __fastcall ButtonPlayTillHBLClick(TObject *Sender);
  void __fastcall ButtonRunClick(TObject *Sender);
  void __fastcall ButtonPlayClick(TObject *Sender);
  void __fastcall ButtonStepClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  bool mPaused;
  bool mPauseRequested;
  bool mStepByStep;
  bool mRunTillVBL;
  bool mRunTillHBL;
  bool mPreviousVBL;
  bool mPreviousHBL;
  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  void __fastcall UpdateControls(bool Pause, bool StepByStep);
  bool __fastcall OnCPCEvent(tULong Condition, tNativeCPC* NativeCPC);
  //
  // Private properties
  //
  __property AnsiString Title = { read=GetTitle };

public:		// Déclarations utilisateur
  //
  // Public properties
  //
  __property TSettings* Settings = { write=SetSettings };

  //
  // API
  //
  __fastcall TFormDebugger(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugger *FormDebugger;
//---------------------------------------------------------------------------
#endif
