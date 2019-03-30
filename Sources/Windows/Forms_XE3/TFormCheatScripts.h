/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2017 by Frédéric Coste

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


#ifndef TFormCheatScriptsH
#define TFormCheatScriptsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormCheatScripts : public TForm
{
__published:	// Composants gérés par l'EDI
  TPageControl *PageControl;
  TTabSheet *TabSheetScripts;
  TTabSheet *TabSheetCode;
  TButton *ButtonExecute;
  TSaveDialog *SaveDialog;
  TButton *ButtonNew;
  TListBox *ListBoxScripts;
  TGroupBox *GroupBoxSelectedScript;
  TTabSheet *TabSheetInstructions;
  TButton *ButtonSave;
  TMemo *MemoCode;
  TMemo *MemoInstructions;
  TLabel *LabelSelectedScript;
  TButton *ButtonDelete;
  TButton *ButtonStop;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ButtonNewClick(TObject *Sender);
  void __fastcall ButtonSaveClick(TObject *Sender);
  void __fastcall ButtonExecuteClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall ListBoxScriptsClick(TObject *Sender);
  void __fastcall ListBoxScriptsDblClick(TObject *Sender);
  void __fastcall ButtonDeleteClick(TObject *Sender);
  void __fastcall ButtonStopClick(TObject *Sender);
  void __fastcall PageControlDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect,
          bool Active);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  TList* mListActions;
  int mCurrentActionIndex;
  AnsiString mSelectedScriptFilename;

  //
  // Methods
  //
  void __fastcall SetSettings(TSettings* Value);
  void __fastcall UpdateScriptsList(void);
  void __fastcall LoadScript(AnsiString Filename);
  bool __fastcall OnCPCEvent(tULong Condition, tNativeCPC* NativeCPCP);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };

  //
  // API
  //
  __fastcall TFormCheatScripts(TComponent* Owner);
  void __fastcall ExecuteScript(AnsiString Filename);
  void __fastcall StopCurrentScript(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCheatScripts *FormCheatScripts;
//---------------------------------------------------------------------------
#endif
