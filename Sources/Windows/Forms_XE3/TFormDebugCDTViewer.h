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


#ifndef TFormDebugCDTViewerH
#define TFormDebugCDTViewerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormDebugCDTViewer : public TForm
{
__published:	// Composants gérés par l'EDI
  TPanel *PanelTape;
  TPanel *PanelBlocks;
  TPanel *PanelBlocksHeader;
  TStringGrid *StringGridBlocks;
  TGroupBox *GroupBoxTapeData;
  TMemo *MemoTapeInfo;
  TGroupBox *GroupBoxBlockInfo;
  TPanel *PanelBlockType;
  TPageControl *PageControlBlock;
  TTabSheet *TabSheetBlockInfo;
  TTabSheet *TabSheetBlockData;
  TMemo *MemoBlockInfo;
  TMemo *MemoBlockData;
  TLabel *LabelBlockPlayed;
  TLabel *LabelBlocks;
  TPanel *PanelControls;
  TButton *ButtonSelectPlayed;
  TButton *ButtonSelect000;
  TButton *ButtonPlayBlock;
  TTabSheet *TabSheetBlockHeader;
  TMemo *MemoHeaderRecord;
  TPopupMenu *PopupMenuClipboard;
  TMenuItem *CopyToClipboardMenu;
  TEdit *EditGoToBlock;
  TButton *ButtonGoToBlock;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall StringGridBlocksDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
  void __fastcall StringGridBlocksSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
  void __fastcall ButtonPlayBlockClick(TObject *Sender);
  void __fastcall ButtonSelectPlayedClick(TObject *Sender);
  void __fastcall ButtonSelect000Click(TObject *Sender);
  void __fastcall PageControlBlockChange(TObject *Sender);
  void __fastcall CopyToClipboardMenuClick(TObject *Sender);
  void __fastcall PageControlBlockDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);
  void __fastcall ButtonGoToBlockClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  TEmulator* mEmulatorP;
  tULong mCurrentBlock;
  tULong mCurrentCounter000Block;
  bool mBlockDataAvailable;
  tUChar* mBlockDataP;
  tULong mBlockDataLength;
  //
  // Methods
  //
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  inline void __fastcall SetEmulator(TEmulator* Value) { mEmulatorP=Value; };
  bool __fastcall OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPCP);
  void __fastcall OnCDTEvent(tTape* TapeP);
  void __fastcall ShowCurrentBlock(tTape* TapeP);
  void __fastcall ShowBlock(tTape* TapeP, int BlockNum);
  void __fastcall ShowBlockData(tUChar* DataP, int Length);
  void __fastcall ShowBlockType10(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType11(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType12(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType13(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType14(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType15(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType20(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType21(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType22(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType2B(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType30(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType31(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType32(tTapeBlock* TapeBlock);
  void __fastcall ShowBlockType33(tTapeBlock* TapeBlock);
  bool __fastcall IsHeaderRecordValid(tUChar* DataP);
  void __fastcall ShowHeaderRecord(void* DataP);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property TEmulator* Emulator = { write=SetEmulator };
  //
  // API
  //
  __fastcall TFormDebugCDTViewer(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugCDTViewer *FormDebugCDTViewer;
//---------------------------------------------------------------------------
#endif
