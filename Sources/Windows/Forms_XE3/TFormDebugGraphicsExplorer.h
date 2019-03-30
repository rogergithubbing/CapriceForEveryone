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


#ifndef TFormDebugGraphicsExplorerH
#define TFormDebugGraphicsExplorerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include "TFormDebugGraphicsEditor.h"
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
#include <Vcl.Buttons.hpp>


// Events types
typedef tULong __fastcall (__closure *TGraphicsExplorerGetAddress)(tULong BaseAddress, int X, int Y);
typedef int __fastcall (__closure *TGraphicsExplorerDrawByte)(tUChar Byte, int X, int Y);
typedef tUChar (*TGraphicsExplorerReadData)(tNativeCPC* NativeCPC, tULong addr);
typedef void __fastcall (__closure *TGraphicsExplorerDrawCells)(int ACol, int ARow, TRect &Rect, TGridDrawState State);
typedef void __fastcall (__closure *TGraphicsExplorerMapPreview)(int Index);
typedef TColor __fastcall (__closure *TGraphicsExplorerGetPixelColor)(tUChar Data, int X);


typedef struct
{
  tUShort Index;
  tULong  Address;
} tGraphicsExplorerTilePreview;


//---------------------------------------------------------------------------
class TFormGraphicsExplorer : public TForm
{
__published:	// Composants gérés par l'EDI
  TStringGrid *StringGridTiles;
  TScrollBar *ScrollBarAddress;
  TPopupMenu *PopupMenuTiles;
  TMenuItem *TileAsTopLeftMenu;
  TMenuItem *CursorAsTopLeftMenu;
  TButton *ButtonByteDecrement;
  TButton *ButtonByteIncrement;
  TPanel *PanelInfo;
  TMenuItem *N1;
  TMenuItem *EditTileMenu;
  TPageControl *PageControlExplorer;
  TTabSheet *TabSheetTiles;
  TLabel *LabelMode;
  TComboBox *ComboBoxMode;
  TLabel *LabelAddressing;
  TComboBox *ComboBoxAddressing;
  TLabel *LabelZoom;
  TEdit *EditZoom;
  TUpDown *UpDownZoom;
  TLabel *LabelTileX;
  TEdit *EditTileX;
  TUpDown *UpDownTileX;
  TLabel *LabelTileY;
  TEdit *EditTileY;
  TUpDown *UpDownTileY;
  TButton *ButtonMonitor;
  TButton *ButtonRefresh;
  TLabel *LabelCursorAddress;
  TPanel *PanelCursorAddress;
  TLabel *LabelTileAddress;
  TPanel *PanelTileAddress;
  TLabel *LabelTopLeftAddress;
  TPanel *PanelFormatAddress;
  TPanel *PanelAddress;
  TPanel *PanelCursorData;
  TLabel *LabelMemory;
  TComboBox *ComboBoxMemory;
  TTabSheet *TabSheetMapper;
  TLabel *LabelFirstTile;
  TPanel *PanelFormatFirstTile;
  TPanel *PanelFirstTile;
  TLabel *LabelLastTile;
  TPanel *PanelLastTile;
  TLabel *LabelMapStart;
  TPanel *PanelFormatMapStart;
  TPanel *PanelMapStart;
  TLabel *LabelMapSizeX;
  TEdit *EditMapX;
  TUpDown *UpDownMapX;
  TLabel *LabelMapSizeY;
  TEdit *EditMapY;
  TUpDown *UpDownMapY;
  TLabel *LabelMapperTiles;
  TSpeedButton *SpeedButtonEdit;
  TPopupMenu *PopupMenuMapper;
  TMenuItem *MapperSetTopLeftTileAddressMenu;
  TMenuItem *MapperSetSelectedTileAddressMenu;
  TButton *ButtonMapperCopy;
  TLabel *LabelMapperIndexing;
  TComboBox *ComboBoxIndexing;
  TMenuItem *N2;
  TMenuItem *SelectedTileAsFirstTileMenu;
  TMenuItem *SelectedTileAsLastTileMenu;
  TEdit *EditMapperTiles;
  TUpDown *UpDownMapperTiles;
  TPanel *PanelMapXReverse;
  TPanel *PanelMapYReverse;
  TPanel *PanelTileIndex;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall StringGridTilesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
  void __fastcall ComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall EditTileXChange(TObject *Sender);
  void __fastcall EditTileYChange(TObject *Sender);
  void __fastcall ComboBoxModeChange(TObject *Sender);
  void __fastcall PanelFormatAddressClick(TObject *Sender);
  void __fastcall PanelAddressClick(TObject *Sender);
  void __fastcall ScrollBarAddressChange(TObject *Sender);
  void __fastcall EditZoomChange(TObject *Sender);
  void __fastcall ButtonRefreshClick(TObject *Sender);
  void __fastcall StringGridTilesSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
  void __fastcall StringGridTilesMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
  void __fastcall StringGridTilesMouseLeave(TObject *Sender);
  void __fastcall TileAsTopLeftMenuClick(TObject *Sender);
  void __fastcall CursorAsTopLeftMenuClick(TObject *Sender);
  void __fastcall ComboBoxAddressingChange(TObject *Sender);
  void __fastcall ButtonByteIncrementClick(TObject *Sender);
  void __fastcall ButtonByteDecrementClick(TObject *Sender);
  void __fastcall ButtonMonitorClick(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall PageControlExplorerDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect,
          bool Active);
  void __fastcall ComboBoxMemoryChange(TObject *Sender);
  void __fastcall PanelFormatFirstTileClick(TObject *Sender);
  void __fastcall PanelFirstTileClick(TObject *Sender);
  void __fastcall PanelLastTileClick(TObject *Sender);
  void __fastcall PanelMapStartClick(TObject *Sender);
  void __fastcall PanelFormatMapStartClick(TObject *Sender);
  void __fastcall SpeedButtonEditClick(TObject *Sender);
  void __fastcall PageControlExplorerChange(TObject *Sender);
  void __fastcall StringGridTilesDblClick(TObject *Sender);
  void __fastcall PageControlExplorerChanging(TObject *Sender, bool &AllowChange);
  void __fastcall EditTileMenuClick(TObject *Sender);
  void __fastcall MapperSetTopLeftTileAddressMenuClick(TObject *Sender);
  void __fastcall MapperSetSelectedTileAddressMenuClick(TObject *Sender);
  void __fastcall EditMapXChange(TObject *Sender);
  void __fastcall EditMapYChange(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall ComboBoxIndexingChange(TObject *Sender);
  void __fastcall ButtonMapperCopyClick(TObject *Sender);
  void __fastcall SelectedTileAsFirstTileMenuClick(TObject *Sender);
  void __fastcall SelectedTileAsLastTileMenuClick(TObject *Sender);
  void __fastcall EditMapperTilesChange(TObject *Sender);
  void __fastcall PanelMapXReverseClick(TObject *Sender);
  void __fastcall PanelMapYReverseClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TEmulator* mEmulatorP;
  TSettings* mSettingsP;
  bool mMonitoring;
  bool mPaused;
  bool mMapPreview;
  int mStartWidth;
  int mStartHeight;
  tUChar mZoom;
  tUChar mTileX;
  tUChar mTileY;
  tULong mTopLeftAddress;
  tULong mTileAddress;
  tULong mTileIndex;
  tULong mCursorAddress;
  tULong mFirstTileAddress;
  tULong mLastTileAddress;
  tULong mNbTiles;
  tULong mMapStartAddress;
  tULong mMapX;
  tULong mMapY;
  tULong mRAMMax;
  tUChar mMode;
  tULong mPreviewAddress;
  tUChar mPreviewOccurences;
  TColor mColor[GA_PALETTE_NBELEMENT];
  TColor mGrayscaleColor[GA_PALETTE_NBELEMENT];
  tGraphicsExplorerTilePreview* mMapPreviewTiles;
  TColor* mColorsP;
  TGraphicsExplorerGetAddress mGetByteAddress;
  TGraphicsExplorerGetAddress mGetTileAddress;
  TGraphicsExplorerDrawByte mDrawByte;
  TGraphicsExplorerReadData mReadData;
  TGraphicsExplorerDrawCells mDrawCells;
  TGraphicsExplorerMapPreview mDrawMapPreview;
  TGraphicsExplorerGetPixelColor mGetPixelColor;
  TFormGraphicsEditor* GraphicsEditorP;

  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  void __fastcall WMMove(TMessage &Message);
  inline void __fastcall SetEmulator(TEmulator* Value) { mEmulatorP=Value; };
  void __fastcall SetSettings(TSettings* Value);
  void __fastcall ResizeTiles(void);
  void __fastcall UpdatePalette(void);
  tULong __fastcall GetByteAddressLinear(tULong Address, int X, int Y);
  tULong __fastcall GetTileAddressLinear(tULong Address, int X, int Y);
  tULong __fastcall GetByteAddressScreen(tULong Address, int X, int Y);
  tULong __fastcall GetTileAddressScreen(tULong Address, int X, int Y);
  int __fastcall DrawByteMode0(tUChar Data, int X, int Y);
  int __fastcall DrawByteMode1(tUChar Data, int X, int Y);
  int __fastcall DrawByteMode2(tUChar Data, int X, int Y);
  inline tULong __fastcall GetTopLeftAddress(void) { return mTopLeftAddress; };
  void __fastcall SetTopLeftAddress(tULong Value);
  inline tULong __fastcall GetTileAddress(void) { return mTileAddress; };
  void __fastcall SetTileAddress(tULong Value);
  inline tULong __fastcall GetCursorAddress(void) { return mCursorAddress; };
  inline void __fastcall SetCursorAddress(tULong Value) { mCursorAddress = Value; };
  inline tULong __fastcall GetFirstTileAddress(void) { return mFirstTileAddress; };
  void __fastcall SetFirstTileAddress(tULong Value);
  inline tULong __fastcall GetLastTileAddress(void) { return mLastTileAddress; };
  void __fastcall SetLastTileAddress(tULong Value);
  inline tULong __fastcall GetMapStartAddress(void) { return mMapStartAddress; };
  void __fastcall SetMapStartAddress(tULong Value);
  bool __fastcall OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPC);
  void __fastcall SpeedButtonPreviewClick(TObject *Sender);
  void __fastcall DrawStringGridTiles(int ACol, int ARow, TRect &Rect, TGridDrawState State);
  TColor __fastcall GetPixelColorMode0(tUChar Data, int X);
  TColor __fastcall GetPixelColorMode1(tUChar Data, int X);
  TColor __fastcall GetPixelColorMode2(tUChar Data, int X);
  void __fastcall DrawStringGridMapPreview(int ACol, int ARow, TRect &Rect, TGridDrawState State);
  void __fastcall UpdateMapPreview(void);
  void __fastcall UpdateMapPreviewDataLinearBytes(int Index);
  void __fastcall UpdateMapPreviewDataCompressedRL0(int Index);
  void __fastcall UpdateMapPreviewDataLinearWordsBigEndian(int Index);
  //
  // Private Properties
  //
  __property AnsiString Title = { read=GetTitle };
  __property tULong TopLeftAddress = { read=GetTopLeftAddress, write=SetTopLeftAddress };
  __property tULong TileAddress = { read=GetTileAddress, write=SetTileAddress };
  __property tULong CursorAddress = { read=GetCursorAddress, write=SetCursorAddress };
  __property tULong FirstTileAddress = { read=GetFirstTileAddress, write=SetFirstTileAddress };
  __property tULong LastTileAddress = { read=GetLastTileAddress, write=SetLastTileAddress };
  __property tULong MapStartAddress = { read=GetMapStartAddress, write=SetMapStartAddress };

public:		// Déclarations utilisateur
  //
  // Public Properties
  //
  __property TEmulator* Emulator = { write=SetEmulator };
  __property TSettings* Settings = { write=SetSettings };
  //
  // API
  //
  __fastcall TFormGraphicsExplorer(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGraphicsExplorer *FormGraphicsExplorer;
//---------------------------------------------------------------------------
#endif
