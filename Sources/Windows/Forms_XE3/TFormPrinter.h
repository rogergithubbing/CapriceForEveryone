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


#ifndef TFormPrinterH
#define TFormPrinterH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormPrinter : public TForm
{
__published:	// Composants gérés par l'EDI
  TPageControl *PageControlPrinter;
  TTabSheet *TabSheetText;
  TTabSheet *TabSheetCommands;
  TButton *ButtonCommandsClear;
  TButton *ButtonCommandsStartStop;
  TListBox *ListBoxHistory;
  TButton *ButtonOnline;
  TButton *ButtonNew;
  TImage *ImageBorderTile;
  TPanel *PanelText;
  TImage *ImageBorderLeft;
  TRichEdit *RichEditPrinter;
  TImage *ImageBorderRight;
  TComboBox *ComboBoxCountry;
  TPopupMenu *PopupMenuHistory;
  TMenuItem *Copytoclipboard1;
  TRichEdit *RichEditLegend;
  TLabel *LabelTextCaution;
  TComboBox *ComboBoxCRLF;
  TButton *ButtonPrint;
  TPrintDialog *PrintDialog1;
  TButton *ButtonDebug;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ButtonCommandsClearClick(TObject *Sender);
  void __fastcall ButtonCommandsStartStopClick(TObject *Sender);
  void __fastcall ButtonOnlineClick(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall ButtonNewClick(TObject *Sender);
  void __fastcall ComboBoxCountryChange(TObject *Sender);
  void __fastcall Copytoclipboard1Click(TObject *Sender);
  void __fastcall ComboBoxCRLFChange(TObject *Sender);
  void __fastcall ButtonPrintClick(TObject *Sender);
  void __fastcall ButtonDebugClick(TObject *Sender);
  void __fastcall RichEditPrinterSelectionChange(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall PageControlPrinterDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);

private:	// Déclarations utilisateur
  //
  // Members
  //
  bool mMonitoring;
  TSettings* mSettingsP;
  const char* mCountryCode;
  int mPageWidthInInch;
  int mPageLengthInInch;
  int mLineSpacing216PerInch;
  int mFileHandle;
  int mNbCharPerLine;
  tUChar mLastCommand;
  int mNbCharPrinted;
  tUChar* mDataBuffer;
  tULong mDataCounter;
  // flags
  bool mEnlarged;
  bool mEnlargedSO;
  bool mDoubleStrike;
  bool mEmphasized;
  bool mCondensed;
  bool mElite;
  bool mDoubleWidth;
  bool mItalic;
  bool mNLQ;
  bool mSuperscriptMode;
  bool mSubscriptMode;
  bool mIncrementalMode;
  bool mProportionalSpacing;
  bool mUnderline;
  //
  // Methods
  //
  bool __fastcall OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC);
  AnsiString __fastcall GetTitle(void);
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  tPrinter* __fastcall GetPrinter(void);
  void __fastcall CreateDocument(void);
  tULong __fastcall Print(tUChar* buffer, tULong Size);
  tULong __fastcall PrintEscapeCode(tUChar* buffer, tULong Size);
  void __fastcall KeepCaretVisible(void);
  void __fastcall ResetAttributes(void);
  void __fastcall UpdateAttributeColor(void);
  void __fastcall UpdateAttributeStyle(void);
  void __fastcall UpdateAttributeSize(void);
  void __fastcall UpdateAttributePitch(void);
  void __fastcall AddMonitoringCharacter(char Command);
  void __fastcall AddMonitoringCharacter(AnsiString Command);
  void __fastcall AddMonitoringMissingCharacter(char Command);
  void __fastcall AddMonitoringEscapeCode(tUChar* buffer, int Size, bool Illegal);
  void __fastcall AddMonitoringMissingEscapeCode(char Command);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property tPrinter* Printer = { read=GetPrinter };

  //
  // API
  //
  __fastcall TFormPrinter(TComponent* Owner);
  void __fastcall SetTextFocus(void);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPrinter *FormPrinter;
//---------------------------------------------------------------------------
#endif
