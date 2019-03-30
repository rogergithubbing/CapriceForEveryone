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


#ifndef TFormDebugMemoryEditorH
#define TFormDebugMemoryEditorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"


// Events types
typedef void __fastcall (__closure *TMemoryEditorEvent)(tNativeCPC* NativeCPC);


//---------------------------------------------------------------------------
class TFormDebugMemoryEditor : public TForm
{
__published:	// Composants gérés par l'EDI
  TTabControl *TabControlMemoryContent;
  TScrollBar *ScrollBarDump;
  TStringGrid *StringGridDump;
  TPageControl *PageControlOperations;
  TTabSheet *TabSheetAddress;
  TTabSheet *TabSheetSearch;
  TTabSheet *TabSheetFill;
  TLabel *LabelAddress1;
  TPanel *PanelFormatAddress1;
  TPanel *PanelAddress1;
  TLabel *LabelRegisters;
  TPanel *PanelFormatSearch;
  TEdit *EditSearch;
  TButton *ButtonSearchNext;
  TPanel *PanelStatus;
  TLabel *LabelCurrentAddress;
  TPanel *PanelFormatCurrentAddress;
  TPanel *PanelCurrentAddress;
  TCheckBox *CheckBoxStringCaseSensitive;
  TLabel *LabelFillRAMAddress;
  TPanel *PanelFormatFillFrom;
  TPanel *PanelFillFrom;
  TLabel *LabelFillFrom;
  TLabel *LabelFillTo;
  TPanel *PanelFormatFillTo;
  TPanel *PanelFillTo;
  TLabel *LabelFillData;
  TPanel *PanelFormatFillData;
  TPanel *PanelFillData;
  TButton *ButtonFill;
  TCheckBox *CheckBoxASCII7Bits;
  TPanel *PanelFormatAddress2;
  TPanel *PanelAddress2;
  TButton *ButtonRefresh;
  TLabel *LabelAddress2;
  TPopupMenu *PopupMenuMemoryContent;
  TMenuItem *SetAsAddress1Menu;
  TMenuItem *SetAsAddress2Menu;
  TMenuItem *N1;
  TMenuItem *SetAsFillStartMenu;
  TMenuItem *SetAsFillEndMenu;
  TMenuItem *SetAsFillDataMenu;
  TLabel *LabelSearch;
  TTabSheet *TabSheetDump;
  TLabel *LabelDumpFrom;
  TPanel *PanelFormatExportFrom;
  TPanel *PanelExportFrom;
  TLabel *LabelDumpTo;
  TPanel *PanelFormatExportTo;
  TPanel *PanelExportTo;
  TButton *ButtonDumpExportText;
  TMenuItem *N2;
  TMenuItem *SetAsDumpFromMenu;
  TMenuItem *SetAsDumpToMenu;
  TSaveDialog *SaveDialog;
  TButton *ButtonDumpSetFullLimits;
  TButton *ButtonDumpImportBinary;
  TButton *ButtonDumpExportBinary;
  TOpenDialog *OpenDialog;
  TButton *ButtonGoToAddress1;
  TButton *ButtonGoToAddress2;
  TButton *ButtonRegBC;
  TButton *ButtonRegDE;
  TButton *ButtonRegHL;
  TButton *ButtonRegBCAlt;
  TButton *ButtonRegDEAlt;
  TButton *ButtonRegHLAlt;
  TButton *ButtonRegSP;
  TButton *ButtonRegPC;
  TButton *ButtonRegIX;
  TButton *ButtonRegIY;
  TTabSheet *TabSheetBanks;
  TLabel *LabelBanksRead;
  TLabel *LabelBanksWrite;
  TLabel *LabelBanksC000;
  TLabel *LabelBanks8000;
  TLabel *LabelBanks4000;
  TLabel *LabelBanks0000;
  TLabel *LabelBanksBank;
  TPanel *PanelBankRead4000;
  TPanel *PanelBankWrite4000;
  TPanel *PanelBankRead8000;
  TPanel *PanelBankWrite8000;
  TPanel *PanelBankWrite0000;
  TPanel *PanelBankWriteC000;
  TPanel *PanelBankRead0000;
  TPanel *PanelBankReadC000;
  TButton *ButtonMonitoring;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall StringGridDumpSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
  void __fastcall ScrollBarDumpChange(TObject *Sender);
  void __fastcall PanelFormatCurrentAddressClick(TObject *Sender);
  void __fastcall PanelFormatAddress1Click(TObject *Sender);
  void __fastcall PanelAddress1Click(TObject *Sender);
  void __fastcall StringGridDumpKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall ButtonRefreshClick(TObject *Sender);
  void __fastcall StringGridDumpMouseWheelDown(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
  void __fastcall StringGridDumpMouseWheelUp(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
  void __fastcall TabControlMemoryContentResize(TObject *Sender);
  void __fastcall TabControlMemoryContentChange(TObject *Sender);
  void __fastcall StringGridDumpClick(TObject *Sender);
  void __fastcall PanelFormatSearchClick(TObject *Sender);
  void __fastcall PageControlOperationsChange(TObject *Sender);
  void __fastcall PanelFormatFillFromClick(TObject *Sender);
  void __fastcall PanelFormatFillToClick(TObject *Sender);
  void __fastcall PanelFormatFillDataClick(TObject *Sender);
  void __fastcall PanelFillFromClick(TObject *Sender);
  void __fastcall PanelFillToClick(TObject *Sender);
  void __fastcall PanelFillDataClick(TObject *Sender);
  void __fastcall ButtonFillClick(TObject *Sender);
  void __fastcall PanelCurrentAddressClick(TObject *Sender);
  void __fastcall CheckBoxASCII7BitsClick(TObject *Sender);
  void __fastcall StringGridDumpGetEditMask(TObject *Sender, int ACol,
          int ARow, AnsiString &Value);
  void __fastcall PanelFormatAddress2Click(TObject *Sender);
  void __fastcall PanelAddress2Click(TObject *Sender);
  void __fastcall SetAsAddress1MenuClick(TObject *Sender);
  void __fastcall SetAsAddress2MenuClick(TObject *Sender);
  void __fastcall SetAsFillStartMenuClick(TObject *Sender);
  void __fastcall SetAsFillEndMenuClick(TObject *Sender);
  void __fastcall SetAsFillDataMenuClick(TObject *Sender);
  void __fastcall StringGridDumpKeyPress(TObject *Sender, char &Key);
  void __fastcall ButtonSearchNextClick(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall PanelFormatExportFromClick(TObject *Sender);
  void __fastcall PanelFormatExportToClick(TObject *Sender);
  void __fastcall StringGridDumpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall SetAsDumpFromMenuClick(TObject *Sender);
  void __fastcall SetAsDumpToMenuClick(TObject *Sender);
  void __fastcall ButtonDumpExportTextClick(TObject *Sender);
  void __fastcall PanelExportFromClick(TObject *Sender);
  void __fastcall PanelExportToClick(TObject *Sender);
  void __fastcall ButtonDumpSetFullLimitsClick(TObject *Sender);
  void __fastcall ButtonDumpExportBinaryClick(TObject *Sender);
  void __fastcall ButtonDumpImportBinaryClick(TObject *Sender);
  void __fastcall ButtonGoToAddress1Click(TObject *Sender);
  void __fastcall ButtonGoToAddress2Click(TObject *Sender);
  void __fastcall ButtonRegBCClick(TObject *Sender);
  void __fastcall ButtonRegDEClick(TObject *Sender);
  void __fastcall ButtonRegHLClick(TObject *Sender);
  void __fastcall ButtonRegBCAltClick(TObject *Sender);
  void __fastcall ButtonRegDEAltClick(TObject *Sender);
  void __fastcall ButtonRegHLAltClick(TObject *Sender);
  void __fastcall ButtonRegSPClick(TObject *Sender);
  void __fastcall ButtonRegPCClick(TObject *Sender);
  void __fastcall ButtonRegIXClick(TObject *Sender);
  void __fastcall ButtonRegIYClick(TObject *Sender);
  void __fastcall TabControlMemoryContentDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);
  void __fastcall StringGridDumpDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
  void __fastcall PageControlOperationsDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);
  void __fastcall StringGridDumpDblClick(TObject *Sender);
  void __fastcall ButtonMonitoringClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  bool mPaused;
  bool mMonitoring;
  tULong mCurrentAddress;
  tULong mTopAddress;
  tULong mMaxTopAddress;
  tULong mMemoryStartAddress;
  tULong mMemorySize;
  tULong mAddress1;
  tULong mAddress2;
  TMemoryEditorEvent mShowMemoryDumpRoutine;
  TMemoryEditorEvent mWriteDataRoutine;
  TMemoryEditorEvent mSearchASCIIRoutine;
  TMemoryEditorEvent mSearchHexRoutine;
  AnsiString mCurrentData;
  tULong mFillFromAddress;
  tULong mFillToAddress;
  tULong mFillData;
  tULong mExportFromAddress;
  tULong mExportToAddress;
  char* mSearchBytes;
  int mSearchLength;
  bool mEditCellEnabled;
  int mChangeValueIncrement;
  int mChangeValueNextRequested;
  bool mChangeValueHexa;

  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  void __fastcall SetDisplayRows(int Value);
  inline tULong __fastcall GetCurrentAddress(void) { return mCurrentAddress; };
  void __fastcall SetCurrentAddress(tULong Value);
  void __fastcall SetMemorySize(tULong Value);
  inline tULong __fastcall GetTopAddress(void) { return mTopAddress; };
  void __fastcall SetTopAddress(tULong Value);
  void __fastcall SetMemoryStartAddress(tULong Value);
  void __fastcall SetMemoryIndex(int Value);
  void __fastcall ShowBankedMemoryDump(tNativeCPC* NativeCPC);
  void __fastcall ShowEntireRAMDump(tNativeCPC* NativeCPC);
  void __fastcall ShowLowerROMDump(tNativeCPC* NativeCPC);
  void __fastcall ShowUpperROMDump(tNativeCPC* NativeCPC);
  void __fastcall WriteDataBankedMemory(tNativeCPC* NativeCPC);
  void __fastcall WriteDataEntireRAM(tNativeCPC* NativeCPC);
  void __fastcall SearchASCIIBankedMemory(tNativeCPC* NativeCPC);
  void __fastcall SearchHexBankedMemory(tNativeCPC* NativeCPC);
  void __fastcall SearchASCIIEntireRAMMemory(tNativeCPC* NativeCPC);
  void __fastcall SearchHexEntireRAMMemory(tNativeCPC* NativeCPC);
  void __fastcall SearchASCIILowerROMMemory(tNativeCPC* NativeCPC);
  void __fastcall SearchHexLowerROMMemory(tNativeCPC* NativeCPC);
  void __fastcall SearchASCIIUpperROMMemory(tNativeCPC* NativeCPC);
  void __fastcall SearchHexUpperROMMemory(tNativeCPC* NativeCPC);
  void __fastcall UpdateBanks(tNativeCPC* NativeCPC);
  bool __fastcall OnCPCEvent(tULong Condition, tNativeCPC* NativeCPC);
  //
  // Properties
  //
  __property int DisplayRows = { write=SetDisplayRows };
  __property tULong CurrentAddress = { read=GetCurrentAddress, write=SetCurrentAddress };
  __property tULong MemorySize = { write=SetMemorySize };
  __property tULong MemoryStartAddress = { write=SetMemoryStartAddress };
  __property tULong TopAddress = { read=GetTopAddress, write=SetTopAddress };
  __property int MemoryIndex = { write=SetMemoryIndex };

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property AnsiString Title = { read=GetTitle };
  __property TSettings* Settings = { write=SetSettings };

  //
  // API
  //
  __fastcall TFormDebugMemoryEditor(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugMemoryEditor *FormDebugMemoryEditor;
//---------------------------------------------------------------------------
#endif
