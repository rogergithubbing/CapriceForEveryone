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


#ifndef TFormDebugGraphicsEditorH
#define TFormDebugGraphicsEditorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>


// Events types
typedef TColor __fastcall (__closure *TGraphicsEditorGetPixelColorEvent)(int X, int Y);
typedef void __fastcall (__closure *TGraphicsEditorWritePixelEvent)(int ACol, int ARow, tUChar ColorIndex);


//---------------------------------------------------------------------------
class TFormGraphicsEditor : public TForm
{
__published:	// Composants gérés par l'EDI
  TPanel *PanelControl;
  TStringGrid *StringGridPixels;
  TLabel *LabelZoom;
  TEdit *EditZoom;
  TUpDown *UpDownZoom;
  TLabel *LabelLeftColor;
  TPanel *PanelLeftColor;
  TLabel *LabelRightColor;
  TPanel *PanelRightColor;
  TGroupBox *GroupBoxPalette;
  TPanel *PanelColor0;
  TPanel *PanelColor1;
  TPanel *PanelColor2;
  TPanel *PanelColor3;
  TPanel *PanelColor4;
  TPanel *PanelColor5;
  TPanel *PanelColor6;
  TPanel *PanelColor7;
  TPanel *PanelColor8;
  TPanel *PanelColor9;
  TPanel *PanelColor10;
  TPanel *PanelColor11;
  TPanel *PanelColor12;
  TPanel *PanelColor13;
  TPanel *PanelColor14;
  TPanel *PanelColor15;
  TButton *ButtonCancel;
  TButton *ButtonApply;
  TButton *ButtonCopy;
  TSpeedButton *SpeedButtonPipette;
  TSpeedButton *SpeedButtonPencil;
  TSpeedButton *SpeedButtonFill;
  TSpeedButton *SpeedButtonPalette;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall StringGridPixelsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
  void __fastcall EditZoomChange(TObject *Sender);
  void __fastcall PanelColorMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
  void __fastcall StringGridPixelsMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall StringGridPixelsMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
  void __fastcall StringGridPixelsMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
  void __fastcall StringGridPixelsMouseLeave(TObject *Sender);
  void __fastcall ButtonCopyClick(TObject *Sender);
  void __fastcall SpeedButtonToolClick(TObject *Sender);
  void __fastcall SpeedButtonPaletteClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TEmulator* mEmulatorP;
  TSettings* mSettingsP;
  tUChar mZoom;
  tUChar mTileX;
  tUChar mTileY;
  tUChar mMode;
  tUChar mLeftPaletteIndex;
  tUChar mRightPaletteIndex;
  TColor mColor[GA_PALETTE_NBELEMENT];
  tUChar* mData;
  TGraphicsEditorGetPixelColorEvent mGetPixelColorEvent;
  TGraphicsEditorWritePixelEvent mWritePixelEvent;
  bool mMouseDown;
  TMouseButton mMouseButton;
  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  inline void __fastcall SetEmulator(TEmulator* Value) { mEmulatorP=Value; };
  void __fastcall SetSettings(TSettings* Value);
  void __fastcall SetTileX(tUChar Value);
  void __fastcall SetTileY(tUChar Value);
  inline void __fastcall SetMode(tUChar Value) { mMode=Value; };
  tUChar __fastcall GetData(int Index);
  void __fastcall SetData(int Index, tUChar Value);
  void __fastcall ResizeTiles(void);
  void __fastcall UpdatePalette(void);
  TColor __fastcall GetPixelColorMode0(int X, int Y);
  TColor __fastcall GetPixelColorMode1(int X, int Y);
  TColor __fastcall GetPixelColorMode2(int X, int Y);
  void __fastcall WritePixelMode0(int ACol, int ARow, tUChar ColorIndex);
  void __fastcall WritePixelMode1(int ACol, int ARow, tUChar ColorIndex);
  void __fastcall WritePixelMode2(int ACol, int ARow, tUChar ColorIndex);
  void __fastcall Fill(int X, int Y, tUChar OldIndex, tUChar NewIndex);
  //
  // Private Properties
  //
  __property AnsiString Title = { read=GetTitle };

public:		// Déclarations utilisateur
  //
  // Public Properties
  //
  __property TEmulator* Emulator = { write=SetEmulator };
  __property TSettings* Settings = { write=SetSettings };
  __property tUChar TileX = { write=SetTileX };
  __property tUChar TileY = { write=SetTileY };
  __property tUChar Mode = { write=SetMode };
  __property tUChar Data[int Index] = { read=GetData, write=SetData };
  //
  // API
  //
  __fastcall TFormGraphicsEditor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGraphicsEditor *FormGraphicsEditor;
//---------------------------------------------------------------------------
#endif
