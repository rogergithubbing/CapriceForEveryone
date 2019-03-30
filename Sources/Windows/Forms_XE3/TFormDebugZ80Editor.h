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


#ifndef TFormDebugZ80EditorH
#define TFormDebugZ80EditorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"

//---------------------------------------------------------------------------
class TFormDebugZ80Editor : public TForm
{
__published:	// Composants gérés par l'EDI
  TGroupBox *GroupBoxRegisters;
  TLabel *LabelRegAF;
  TPanel *PanelFormatRegAF;
  TPanel *PanelFormatRegBC;
  TPanel *PanelRegA;
  TPanel *PanelRegB;
  TPanel *PanelRegF;
  TPanel *PanelRegC;
  TPanel *PanelFormatRegAFAlt;
  TPanel *PanelRegAAlt;
  TPanel *PanelRegFAlt;
  TPanel *PanelFormatRegBCAlt;
  TPanel *PanelRegBAlt;
  TPanel *PanelRegCAlt;
  TPanel *PanelFormatRegIX;
  TPanel *PanelFormatRegIY;
  TPanel *PanelFormatRegDE;
  TPanel *PanelRegD;
  TPanel *PanelRegE;
  TPanel *PanelFormatRegHL;
  TPanel *PanelRegH;
  TPanel *PanelRegL;
  TPanel *PanelFormatRegI;
  TPanel *PanelRegI;
  TPanel *PanelFormatRegR;
  TPanel *PanelRegR;
  TLabel *LabelRegBC;
  TLabel *LabelRegDE;
  TLabel *LabelRegHL;
  TLabel *LabelRegAFAlt;
  TLabel *LabelRegBCAlt;
  TLabel *LabelRegIX;
  TLabel *LabelRegIY;
  TLabel *LabelRegI;
  TLabel *LabelRegR;
  TLabel *LabelRegDEAlt;
  TPanel *PanelFormatRegDEAlt;
  TPanel *PanelRegDAlt;
  TPanel *PanelRegEAlt;
  TLabel *LabelRegHLAlt;
  TPanel *PanelFormatRegHLAlt;
  TPanel *PanelRegHAlt;
  TPanel *PanelRegLAlt;
  TLabel *LabelRegPC;
  TPanel *PanelFormatRegPC;
  TPanel *PanelRegPC;
  TPanel *PanelRegIY;
  TPanel *PanelRegIX;
  TLabel *LabelRegSP;
  TPanel *PanelFormatRegSP;
  TPanel *PanelRegSP;
  TGroupBox *GroupBoxInstructions;
  TGroupBox *GroupBoxStack;
  TGroupBox *GroupBoxFlags;
  TStringGrid *StringGridInstructions;
  TStringGrid *StringGridStack;
  TComboBox *ComboBoxFlag7;
  TComboBox *ComboBoxFlag6;
  TComboBox *ComboBoxFlag5;
  TComboBox *ComboBoxFlag4;
  TComboBox *ComboBoxFlag3;
  TComboBox *ComboBoxFlag2;
  TComboBox *ComboBoxFlag1;
  TComboBox *ComboBoxFlag0;
  TGroupBox *GroupBoxInterrupt;
  TLabel *LabelIFF1;
  TPanel *PanelIFF1;
  TLabel *LabelIFF2;
  TPanel *PanelIFF2;
  TComboBox *ComboBoxITMode;
  TLabel *LabelITMode;
  TGroupBox *GroupBoxBreakpoint1;
  TCheckBox *CheckBoxBreakpoint1;
  TPanel *PanelFormatBreakpoint1;
  TPanel *PanelBreakpoint1;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall PanelFormatRegPCClick(TObject *Sender);
  void __fastcall PanelFormatRegAFClick(TObject *Sender);
  void __fastcall PanelFormatRegBCClick(TObject *Sender);
  void __fastcall PanelFormatRegDEClick(TObject *Sender);
  void __fastcall PanelFormatRegHLClick(TObject *Sender);
  void __fastcall PanelFormatRegAFAltClick(TObject *Sender);
  void __fastcall PanelFormatRegBCAltClick(TObject *Sender);
  void __fastcall PanelFormatRegDEAltClick(TObject *Sender);
  void __fastcall PanelFormatRegHLAltClick(TObject *Sender);
  void __fastcall PanelFormatRegIXClick(TObject *Sender);
  void __fastcall PanelFormatRegIYClick(TObject *Sender);
  void __fastcall PanelFormatRegSPClick(TObject *Sender);
  void __fastcall PanelFormatRegIClick(TObject *Sender);
  void __fastcall PanelFormatRegRClick(TObject *Sender);
  void __fastcall ComboBoxFlagChange(TObject *Sender);
  void __fastcall PanelRegAClick(TObject *Sender);
  void __fastcall PanelRegFClick(TObject *Sender);
  void __fastcall PanelRegBClick(TObject *Sender);
  void __fastcall PanelRegCClick(TObject *Sender);
  void __fastcall PanelRegDClick(TObject *Sender);
  void __fastcall PanelRegEClick(TObject *Sender);
  void __fastcall PanelRegHClick(TObject *Sender);
  void __fastcall PanelRegLClick(TObject *Sender);
  void __fastcall PanelRegAAltClick(TObject *Sender);
  void __fastcall PanelRegFAltClick(TObject *Sender);
  void __fastcall PanelRegBAltClick(TObject *Sender);
  void __fastcall PanelRegCAltClick(TObject *Sender);
  void __fastcall PanelRegDAltClick(TObject *Sender);
  void __fastcall PanelRegEAltClick(TObject *Sender);
  void __fastcall PanelRegHAltClick(TObject *Sender);
  void __fastcall PanelRegLAltClick(TObject *Sender);
  void __fastcall PanelRegIXClick(TObject *Sender);
  void __fastcall PanelRegIYClick(TObject *Sender);
  void __fastcall PanelRegSPClick(TObject *Sender);
  void __fastcall PanelRegPCClick(TObject *Sender);
  void __fastcall PanelRegIClick(TObject *Sender);
  void __fastcall PanelRegRClick(TObject *Sender);
  void __fastcall ComboBoxITModeChange(TObject *Sender);
  void __fastcall PanelFormatBreakpoint1Click(TObject *Sender);
  void __fastcall CheckBoxBreakpoint1Click(TObject *Sender);
  void __fastcall PanelBreakpoint1Click(TObject *Sender);
  void __fastcall ComboBoxITModeDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag7DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag6DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag5DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag4DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag3DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag2DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag1DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxFlag0DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall StringGridInstructionsDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  tULong* mNextInstructions;
  tULong mBreakpoint1;
  bool mBreakpoint1Highlighted;
  bool mPaused;

  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  bool __fastcall OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPC);
  tULong __fastcall GetInstruction(tNativeCPC* NativeCPC, tULong addr, AnsiString& MachineCode, AnsiString& Instruction,
                                   AnsiString& MemoryContent);
  void __fastcall UpdateRegisters(tNativeCPC* NativeCPC);
  void __fastcall UpdateFlags(tNativeCPC* NativeCPC);
  void __fastcall UpdateInstructions(tNativeCPC* NativeCPC);
  void __fastcall UpdateStack(tNativeCPC* NativeCPC);
  void __fastcall UpdateInterrupt(tNativeCPC* NativeCPC);
  void __fastcall InitBreakpoint(tNativeCPC* NativeCPC);
  void __fastcall UpdateBreakpoint(tNativeCPC* NativeCPC);
  void __fastcall UpdateRegistersEdition(bool Allow);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property AnsiString Title = { read=GetTitle };
  __property TSettings* Settings = { write=SetSettings };

  //
  // API
  //
  __fastcall TFormDebugZ80Editor(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugZ80Editor *FormDebugZ80Editor;
//---------------------------------------------------------------------------
#endif
