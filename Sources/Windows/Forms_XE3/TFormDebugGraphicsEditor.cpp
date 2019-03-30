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


#include <vcl.h>
#include <Clipbrd.hpp>
#include <tchar.h>
#pragma hdrstop
#include "TFormMain.h"
#include "TFormDebugGraphicsEditor.h"
#include "TFormDebugEditValue.h"
#include "TFormDebugBASICColors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


static const AnsiString cTitle = "Tile editor";
static const AnsiString cTitlePipette = " - Pipette";
static const AnsiString cTitlePencil = " - Pencil";
static const AnsiString cTitleFill = " - Fill";

static const AnsiString cPalette = " Palette ";
static const AnsiString cPaletteUpdate = "update ";

static const AnsiString cHintPalettePipette = "Left or Right mouse click to select color.";
static const AnsiString cHintPaletteUpdate = "Select new color.";


TFormGraphicsEditor *FormGraphicsEditor;
//---------------------------------------------------------------------------
__fastcall TFormGraphicsEditor::TFormGraphicsEditor(TComponent* Owner)
  : TForm(Owner)
{
  mEmulatorP = NULL;
  mSettingsP = NULL;

  mZoom = 8;
  mTileX = 2;
  mTileY = 4;
  mData = NULL;

  // Apply Theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;

  ShowHint = ((TForm*)Owner)->ShowHint;
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  mLeftPaletteIndex = 0;
  mRightPaletteIndex = 1;
  Caption = Title;
  GroupBoxPalette->Caption = cPalette;
  GroupBoxPalette->Hint = cHintPalettePipette;
  UpDownZoom->Position = mZoom;
  mMouseDown = false;

  switch (mMode)
  {
    case 0:
    {
      mGetPixelColorEvent = GetPixelColorMode0;
      mWritePixelEvent = WritePixelMode0;
    }
    break;

    case 1:
    {
      mGetPixelColorEvent = GetPixelColorMode1;
      mWritePixelEvent = WritePixelMode1;
    }
    break;

    case 2:
    {
      mGetPixelColorEvent = GetPixelColorMode2;
      mWritePixelEvent = WritePixelMode2;
    }
    break;
  }

  SpeedButtonPipette->Down = true;
  SpeedButtonPalette->Down = false;

  ResizeTiles();
  UpdatePalette();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormGraphicsEditor::GetTitle(void)
{
  AnsiString Title = cTitle;

  if (SpeedButtonPipette->Down == true) Title += cTitlePipette;
  else if (SpeedButtonPencil->Down == true) Title += cTitlePencil;
  else if (SpeedButtonFill->Down == true) Title += cTitleFill;

  return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::SetTileX(tUChar Value)
{
  mTileX=Value;
  ResizeTiles();
}
//---------------------------------------------------------------------------
void __fastcall TFormGraphicsEditor::SetTileY(tUChar Value)
{
  mTileY=Value;
  ResizeTiles();
}
//---------------------------------------------------------------------------


tUChar __fastcall TFormGraphicsEditor::GetData(int Index)
{
  tUChar Data = 0;
  if (mData)
  {
    if (Index < (mTileX * mTileY))
    {
      Data = mData[Index];
    }
  }
  return Data;
}
//---------------------------------------------------------------------------
void __fastcall TFormGraphicsEditor::SetData(int Index, tUChar Value)
{
  if (mData)
  {
    if (Index < (mTileX * mTileY))
    {
      mData[Index] = Value;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  StringGridPixels->Color = mSettingsP->ThemeBackgroundColor;
  EditZoom->Font->Color = clBlack;
  EditZoom->Font->Size = 12;
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::ResizeTiles(void)
{
  int NbPixels;
  switch (mMode)
  {
    case 0: NbPixels = 4; break;
    case 1: NbPixels = 2; break;
    case 2: NbPixels = 1; break;
  }

  int SizeX = (NbPixels * mZoom) + 2; // Add 1 pixel margin
  int SizeY = (2 * mZoom) + 2; // Add 1 pixel margin

  delete [] mData;
  mData = new tUChar[mTileX*mTileY];

  // Update tile size
  StringGridPixels->DefaultColWidth = SizeX;
  StringGridPixels->DefaultRowHeight = SizeY;

  // Update number of tiles shown
  StringGridPixels->ColCount = (8 / NbPixels) * mTileX;
  StringGridPixels->RowCount = mTileY;

  StringGridPixels->Refresh();
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::UpdatePalette(void)
{
  tNativeCPC* NativeCPC = FormMain->NativeCPC;

  for (int Loop=0; Loop < GA_INK_VALUES_NBELEMENT; Loop++)
  {
    tULong Color = NativeCPC->GateArray->ink_values[Loop];
    mColor[Loop] = (TColor)RGB(NativeCPC->colours_rgb[Color].colour_r,
                               NativeCPC->colours_rgb[Color].colour_g,
                               NativeCPC->colours_rgb[Color].colour_b);
  }

  int MaxColors;
  switch (mMode)
  {
    case 0: MaxColors = 16; break;
    case 1: MaxColors = 4; break;
    case 2: MaxColors = 2; break;
  }

  for (int Loop=0; Loop < 16; Loop++)
  {
    TPanel* PanelColor = (TPanel*)FindComponent("PanelColor" + AnsiString(Loop));
    PanelColor->Visible = (Loop < MaxColors) ? true : false;
    PanelColor->Color = mColor[Loop];
  }

  PanelLeftColor->Color = mColor[mLeftPaletteIndex];
  PanelRightColor->Color = mColor[mRightPaletteIndex];
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::EditZoomChange(TObject *Sender)
{
  if (EditZoom->Text.IsEmpty() == false)
  {
    mZoom = EditZoom->Text.ToInt();
    if (mZoom < 1) mZoom = 1;

    ResizeTiles();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::StringGridPixelsDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
  // Draw frame
  StringGridPixels->Canvas->Pen->Color = clLtGray;
  StringGridPixels->Canvas->MoveTo(Rect.left, Rect.top);
  StringGridPixels->Canvas->LineTo(Rect.right-1, Rect.top);
  StringGridPixels->Canvas->LineTo(Rect.right-1, Rect.bottom-1);
  StringGridPixels->Canvas->Pen->Color = clDkGray;
  StringGridPixels->Canvas->LineTo(Rect.left, Rect.bottom-1);
  StringGridPixels->Canvas->LineTo(Rect.left, Rect.top);

  // Draw Pixel
  StringGridPixels->Canvas->Brush->Color = mGetPixelColorEvent(ACol, ARow);
  StringGridPixels->Canvas->FillRect(TRect(Rect.left+1, Rect.top+1, Rect.right-1, Rect.bottom-1));

  NOT_USED(Sender);
  NOT_USED(State);
}
//---------------------------------------------------------------------------


TColor __fastcall TFormGraphicsEditor::GetPixelColorMode0(int X, int Y)
{
  if (mData == NULL) return clBlack;

  // Get Data address
  tUChar Data = mData[(X / 2) + (Y * mTileX)];
  tUChar Pixel = X % 2;

  tUChar Color;
  switch (Pixel)
  {
    case 0:
    {
      Color =   ((Data & 0x02) << 2) | ((Data & 0x20) >> 3)
              | ((Data & 0x08) >> 2) | ((Data & 0x80) >> 7);
    }
    break;
    case 1:
    {
      Color =   ((Data & 0x01) << 3) | ((Data & 0x10) >> 2)
              | ((Data & 0x04) >> 1) | ((Data & 0x40) >> 6);
    }
    break;
  }

  return (mColor[Color]);
}
//---------------------------------------------------------------------------
TColor __fastcall TFormGraphicsEditor::GetPixelColorMode1(int X, int Y)
{
  if (mData == NULL) return clBlack;

  // Get Data address
  tUChar Data = mData[(X / 4) + (Y * mTileX)];
  tUChar Pixel = X % 4;

  tUChar Color;
  switch (Pixel)
  {
    case 0: Color = ((Data & 0x08) >> 2) + ((Data & 0x80) >> 7); break;
    case 1: Color = ((Data & 0x04) >> 1) + ((Data & 0x40) >> 6); break;
    case 2: Color = ((Data & 0x02) >> 0) + ((Data & 0x20) >> 5); break;
    case 3: Color = ((Data & 0x01) << 1) + ((Data & 0x10) >> 4); break;
  }

  return (mColor[Color]);
}
//---------------------------------------------------------------------------
TColor __fastcall TFormGraphicsEditor::GetPixelColorMode2(int X, int Y)
{
  if (mData == NULL) return clBlack;

  // Get Data address
  tUChar Data = mData[(X / 8) + (Y * mTileX)];
  tUChar Pixel = 7 - (X % 8);

  return (mColor[(Data >> Pixel) & 0x01]);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::WritePixelMode0(int ACol, int ARow, tUChar ColorIndex)
{
  if (mData == NULL) return;

  int Index = (ACol / 2) + (ARow * mTileX);
  tUChar* DataP = &mData[Index];

  switch (ACol % 2)
  {
    case 0:
    {
      *DataP &= ~(0xAA);
      *DataP |=   ((ColorIndex & 0x01) << 7)  // Bit 0 => 0x80
                | ((ColorIndex & 0x02) << 2)  // Bit 1 => 0x08
                | ((ColorIndex & 0x04) << 3)  // Bit 2 => 0x20
                | ((ColorIndex & 0x08) >> 2); // Bit 3 => 0x02
    }
    break;

    case 1:
    {
      *DataP &= ~(0x55);
      *DataP |=   ((ColorIndex & 0x01) << 6)  // Bit 0 => 0x40
                | ((ColorIndex & 0x02) << 1)  // Bit 1 => 0x04
                | ((ColorIndex & 0x04) << 2)  // Bit 2 => 0x10
                | ((ColorIndex & 0x08) >> 3); // Bit 3 => 0x01
    }
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormGraphicsEditor::WritePixelMode1(int ACol, int ARow, tUChar ColorIndex)
{
  if (mData == NULL) return;

  int Index = (ACol / 4) + (ARow * mTileX);
  tUChar* DataP = &mData[Index];

  switch (ACol % 4)
  {
    case 0:
    {
      *DataP &= ~(0x88);
      *DataP |= ((ColorIndex & 0x01) << 7) + ((ColorIndex & 0x02) << 2);
    }
    break;
    case 1:
    {
      *DataP &= ~(0x44);
      *DataP |= ((ColorIndex & 0x01) << 6) + ((ColorIndex & 0x02) << 1);
    }
    break;
    case 2:
    {
      *DataP &= ~(0x22);
      *DataP |= ((ColorIndex & 0x01) << 5) + ((ColorIndex & 0x02) << 0);
    }
    break;
    case 3:
    {
      *DataP &= ~(0x11);
      *DataP |= ((ColorIndex & 0x01) << 4) + ((ColorIndex & 0x02) >> 1);
    }
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormGraphicsEditor::WritePixelMode2(int ACol, int ARow, tUChar ColorIndex)
{
  if (mData == NULL) return;

  int Index = (ACol / 8) + (ARow * mTileX);
  tUChar* DataP = &mData[Index];

  *DataP &= ~(1 << (7-(ACol % 8)));
  *DataP |= (ColorIndex & 0x01) << (7 - (ACol % 8));
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::PanelColorMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  for (int Loop=0; Loop < 16; Loop++)
  {
    TPanel* PanelColor = (TPanel*)FindComponent("PanelColor" + AnsiString(Loop));

    if (Sender == PanelColor)
    {
      if (SpeedButtonPalette->Down == false)
      {
        if (Button == mbLeft)
        {
          PanelLeftColor->Color = PanelColor->Color;
          mLeftPaletteIndex = Loop;
        }
        else
        {
          PanelRightColor->Color = PanelColor->Color;
          mRightPaletteIndex = Loop;
        }
      }
      else
      {
        tNativeCPC* NativeCPC = FormMain->EmulatorP->NativeCPC;
        tGateArray* GateArray = NativeCPC->GateArray;
        int Ink = GateArray->ink_values[Loop];

        TFormDebugBASICColors* EditColor = new TFormDebugBASICColors(this);
        EditColor->Settings = mSettingsP;
        EditColor->SelectedColor = GateArray->BasicColorsP[Ink];
        if (EditColor->ShowModal() == mrOk)
        {
          int NewColor = GateArray->GateArrayColorsP[EditColor->SelectedColor];

          // Update new ink
          GateArray->ink_values[Loop] = NewColor;
          GateArray->palette[Loop] = NativeCPC->colours[NewColor];
          GateArray->signal_palette_update++;

          UpdatePalette();
          ResizeTiles();
        }
        delete EditColor;
      }

      break;
    }
  }

  NOT_USED(Shift);
  NOT_USED(X);
  NOT_USED(Y);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsEditor::StringGridPixelsMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  mMouseDown = true;
  mMouseButton = Button;

  int ACol;
  int ARow;
  StringGridPixels->MouseToCell(X, Y, ACol, ARow);

  if (SpeedButtonPipette->Down == true)
  {
    TColor Color = mGetPixelColorEvent(ACol, ARow);
    tUChar Index;
    for (int Loop=0; Loop < GA_PALETTE_NBELEMENT; Loop++)
    {
      if (Color == mColor[Loop])
      {
        Index = Loop;
        break;
      }
    }

    if (mMouseButton == mbLeft)
    {
      PanelLeftColor->Color = Color;
      mLeftPaletteIndex = Index;
    }
    else
    {
      PanelRightColor->Color = Color;
      mRightPaletteIndex = Index;
    }
  }
  else if (SpeedButtonPencil->Down == true)
  {
    mWritePixelEvent(ACol,
                     ARow,
                     (mMouseButton == mbLeft) ? mLeftPaletteIndex : mRightPaletteIndex);
    StringGridPixels->Refresh();
  }
  else if (SpeedButtonFill->Down == true)
  {
    TColor OldColor = mGetPixelColorEvent(ACol, ARow);
    tUChar OldIndex;
    for (int Loop=0; Loop < GA_PALETTE_NBELEMENT; Loop++)
    {
      if (OldColor == mColor[Loop])
      {
        OldIndex = Loop;
        break;
      }
    }
    tUChar NewIndex = mMouseButton == mbLeft ? mLeftPaletteIndex : mRightPaletteIndex;
    if (OldIndex != NewIndex)
    {
      Fill(ACol, ARow, OldIndex, NewIndex);
      StringGridPixels->Refresh();
    }
  }

  NOT_USED(Sender);
  NOT_USED(Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsEditor::StringGridPixelsMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
  if (mMouseDown == true)
  {
    int ACol;
    int ARow;
    StringGridPixels->MouseToCell(X, Y, ACol, ARow);

    if (SpeedButtonPencil->Down == true)
    {
      mWritePixelEvent(ACol,
                       ARow,
                       (mMouseButton == mbLeft) ? mLeftPaletteIndex : mRightPaletteIndex);
      StringGridPixels->Refresh();
    }
  }

  NOT_USED(Sender);
  NOT_USED(Shift);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsEditor::StringGridPixelsMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  mMouseDown = false;

  NOT_USED(Sender);
  NOT_USED(Button);
  NOT_USED(Shift);
  NOT_USED(X);
  NOT_USED(Y);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsEditor::StringGridPixelsMouseLeave(TObject *Sender)
{
  mMouseDown = false;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::ButtonCopyClick(TObject *Sender)
{
  // Create bitmap
  TBitmap* BitmapP = new TBitmap();

  if (BitmapP)
  {
    int SizeX = 1;
    int SizeY = 2;
    switch (mMode)
    {
      case 0: SizeX = 4; break;
      case 1: SizeX = 2; break;
      case 2: SizeX = 1; break;
    }

    // Populate bitmap
    BitmapP->PixelFormat = pf32bit;
    BitmapP->Width = StringGridPixels->ColCount * SizeX;
    BitmapP->Height = StringGridPixels->RowCount * SizeY;
    for (int Y=0; Y < BitmapP->Height; Y++)
    {
      for (int X=0; X < BitmapP->Width; X++)
      {
        BitmapP->Canvas->Pixels[X][Y] = mGetPixelColorEvent(X/SizeX, Y/SizeY);
      }
    }

    // Copy Bitmap to clipboard
    WORD AFormat;
    unsigned int AData;
    HPALETTE APalette;
    BitmapP->SaveToClipboardFormat(AFormat, AData, APalette);
    TClipboard *pCB = Clipboard();
    pCB->Clear();
    pCB->SetAsHandle(AFormat, AData);

    delete BitmapP;

    // Display message
    Application->MessageBox(_T("Tile has been copied to clipboard."), _T("Clipboard"), MB_OK);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::Fill(int X, int Y, tUChar OldIndex, tUChar NewIndex)
{
  if (X < 0) return;
  if (X >= StringGridPixels->ColCount) return;
  if (Y < 0) return;
  if (Y >= StringGridPixels->RowCount) return;

  TColor Color = mGetPixelColorEvent(X, Y);
  tUChar Index;
  for (int Loop=0; Loop < GA_PALETTE_NBELEMENT; Loop++)
  {
    if (Color == mColor[Loop])
    {
      Index = Loop;
      break;
    }
  }

  if (Index == OldIndex)
  {
    mWritePixelEvent(X, Y, NewIndex);

    // Fill adjacent cells
    Fill(X-1, Y,   OldIndex, NewIndex);
    Fill(X+1, Y,   OldIndex, NewIndex);
    Fill(X,   Y-1, OldIndex, NewIndex);
    Fill(X,   Y+1, OldIndex, NewIndex);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::SpeedButtonToolClick(TObject *Sender)
{
  Caption = Title;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsEditor::SpeedButtonPaletteClick(TObject *Sender)

{
  if (SpeedButtonPalette->Down == true)
  {
    GroupBoxPalette->Caption = cPalette + cPaletteUpdate;
    GroupBoxPalette->Hint = cHintPaletteUpdate;
  }
  else
  {
    GroupBoxPalette->Caption = cPalette;
    GroupBoxPalette->Hint = cHintPalettePipette;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

