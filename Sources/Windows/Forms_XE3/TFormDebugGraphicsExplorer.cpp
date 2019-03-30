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
#include "TFormDebugGraphicsExplorer.h"
#include "TFormDebugEditValue.h"
#include "TFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


static const AnsiString cTitle = "Graphics explorer";
static const AnsiString cTitlePaused  = " - [PAUSED]";
static const AnsiString cTitleMonitoring  = " - [Monitoring]";
static const AnsiString cTitlePreview  = " - [PREVIEW]";



tUChar Engine_ReadRAM(tNativeCPC* NativeCPC, tULong addr)
//---------------------------------------------------------------------------
{
  return (NativeCPC->pbRAM[addr]);
}



TFormGraphicsExplorer *FormGraphicsExplorer;
//---------------------------------------------------------------------------
__fastcall TFormGraphicsExplorer::TFormGraphicsExplorer(TComponent* Owner)
  : TForm(Owner)
{
  mEmulatorP = NULL;
  mSettingsP = NULL;

  mStartWidth = Width;
  mStartHeight = Height;

  mZoom = 1;
  mTileX = 2;
  mTileY = 8;
  mMapX = 32;
  mMapY = 8;
  mTopLeftAddress = 0;
  mTileAddress = 0;
  mFirstTileAddress = 0;
  mLastTileAddress = 0;
  mNbTiles = 1;
  mMapStartAddress = 0;

  mReadData = NULL;
  mMapPreviewTiles = new tGraphicsExplorerTilePreview[mMapX * mMapY];
  mDrawMapPreview = UpdateMapPreviewDataLinearBytes;

  UpDownZoom->Position = mZoom;
  UpDownTileX->Position = mTileX;
  UpDownTileY->Position = mTileY;
  UpDownMapX->Position = mMapX;
  UpDownMapY->Position = mMapY;
  UpDownMapperTiles->Position = mNbTiles;

  mDrawCells = DrawStringGridTiles;

  UpDownMapperTiles->Position = 1;

  // Apply Theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  PanelFormatFirstTile->Font->Color = clBlack;
  PanelFirstTile->Font->Color = clBlack;
  PanelLastTile->Font->Color = clBlack;
  PanelFormatMapStart->Font->Color = clBlack;
  PanelMapStart->Font->Color = clBlack;
  PanelMapXReverse->Font->Color = clBlack;
  PanelMapYReverse->Font->Color = clBlack;
  EditZoom->Font->Color = clBlack;
  EditZoom->Font->Size = 12;
  EditTileX->Font->Color = clBlack;
  EditTileX->Font->Size = 12;
  EditTileY->Font->Color = clBlack;
  EditTileY->Font->Size = 12;
  PanelFormatAddress->Font->Color = clBlack;
  PanelAddress->Font->Color = clBlack;
  EditMapX->Font->Color = clBlack;
  EditMapX->Font->Size = 12;
  EditMapY->Font->Color = clBlack;
  EditMapY->Font->Size = 12;
  SpeedButtonEdit->Font->Color = clBlack;
  EditMapperTiles->Font->Size = 12;
  EditMapperTiles->Font->Color = clBlack;
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::FormDestroy(TObject *Sender)
{
  delete [] mMapPreviewTiles;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  ShowHint = FormMain->ShowHint;

  mMonitoring = false;
  mMapPreview = false;
  mPaused = FormMain->NativeCPC->paused;
  ButtonMonitor->Caption = "Start";
  Caption = Title;

  ComboBoxMode->ItemIndex = 0;
  ComboBoxModeChange(Sender);
  ComboBoxAddressing->ItemIndex = 0;
  ComboBoxAddressingChange(Sender);
  ComboBoxMemory->ItemIndex = 0;
  ComboBoxMemoryChange(Sender);
  ComboBoxIndexing->ItemIndex = 0;
  ComboBoxIndexingChange(Sender);
  UpDownZoom->Position = mZoom;
  UpDownTileX->Position = mTileX;
  UpDownTileY->Position = mTileY;
  PanelFormatAddress->Caption = "&&";
  PanelAddress->Caption = AnsiString::IntToHex(mTopLeftAddress, 5);
  PanelTileAddress->Caption = AnsiString::IntToHex(mTileAddress, 5);
  ScrollBarAddress->Position = mTopLeftAddress;
  PanelCursorAddress->Caption = "";
  PanelCursorData->Caption = "";
  PanelFormatFirstTile->Caption = "&&";
  PanelFirstTile->Caption = AnsiString::IntToHex(mFirstTileAddress, 5);
  PanelLastTile->Caption = AnsiString::IntToHex(mLastTileAddress, 5);
  PanelFormatMapStart->Caption = "&&";
  PanelMapStart->Caption = AnsiString::IntToHex(mMapStartAddress, 5);
  PageControlExplorer->ActivePage = TabSheetTiles;
  SpeedButtonEdit->Down = false;
  PageControlExplorerChange(Sender);
  SpeedButtonPreviewClick(Sender);

  StringGridTiles->Col = 0;
  StringGridTiles->Row = 0;

  ResizeTiles();
  UpdatePalette();

  GraphicsEditorP = new TFormGraphicsEditor(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnStepByStepEvent);
  }

  delete GraphicsEditorP;
  GraphicsEditorP = NULL;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormGraphicsExplorer::GetTitle(void)
{
  AnsiString Title = cTitle;

  if (mMonitoring == true) Title += cTitleMonitoring;
  if (mPaused == true) Title += cTitlePaused;
  if (mMapPreview == true) Title += cTitlePreview;

  return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->GraphicsExplorerFormLeft != 0)
       || (mSettingsP->GraphicsExplorerFormTop != 0)
       || (mSettingsP->GraphicsExplorerFormWidth != 0)
       || (mSettingsP->GraphicsExplorerFormHeight != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->GraphicsExplorerFormLeft;
    Top = mSettingsP->GraphicsExplorerFormTop;
    Width = mSettingsP->GraphicsExplorerFormWidth;
    Height = mSettingsP->GraphicsExplorerFormHeight;
  }

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  StringGridTiles->Color = mSettingsP->ThemeBackgroundColor;
  PanelTileAddress->Font->Color = mSettingsP->ThemeText2Color;
  PanelCursorAddress->Font->Color = mSettingsP->ThemeText2Color;
  PanelCursorData->Font->Color = mSettingsP->ThemeText2Color;
  PanelInfo->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->GraphicsExplorerFormWidth = Width;
    if (mSettingsP->GraphicsExplorerFormWidth < mStartWidth)
    {
      mSettingsP->GraphicsExplorerFormWidth = mStartWidth;
    }
    mSettingsP->GraphicsExplorerFormHeight = Height;
    if (mSettingsP->GraphicsExplorerFormHeight < mStartHeight)
    {
      mSettingsP->GraphicsExplorerFormHeight = mStartHeight;
    }

    ResizeTiles();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->GraphicsExplorerFormLeft = Left;
    mSettingsP->GraphicsExplorerFormTop = Top;
  }

  NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::ResizeTiles(void)
{
  int SizeX = (8 * mZoom * mTileX) + 2; // Add 1 pixel margin
  int SizeY = (2 * mZoom * mTileY) + 2; // Add 1 pixel margin

  // Update tile size
  StringGridTiles->DefaultColWidth = SizeX;
  StringGridTiles->DefaultRowHeight = SizeY;

  // Update number of tiles shown
  if (mMapPreview == false)
  {
    StringGridTiles->ColCount = StringGridTiles->Width / SizeX;
    StringGridTiles->RowCount = StringGridTiles->Height / SizeY;
  }
  else
  {
    StringGridTiles->ColCount = mMapX;
    StringGridTiles->RowCount = mMapY;
  }

  if (mMapPreview == false)
  {
    // Update tile address
    TileAddress = mTopLeftAddress
                  + (StringGridTiles->Col * mTileY * mTileX * StringGridTiles->RowCount)
                  + (StringGridTiles->Row * mTileY * mTileX);

    // Linear addressing ?
    if (ComboBoxAddressing->ItemIndex == 0)
    {
      ScrollBarAddress->PageSize =  StringGridTiles->ColCount * StringGridTiles->RowCount *
                                    mTileX * mTileY;
      ScrollBarAddress->SmallChange = StringGridTiles->RowCount * mTileX * mTileY;
    }
    else
    {
      ScrollBarAddress->PageSize =  StringGridTiles->ColCount * StringGridTiles->RowCount *
                                    mTileX;
      ScrollBarAddress->SmallChange = StringGridTiles->ColCount * mTileX;
    }

    // Update Info
    AnsiString Info = "Grid="
                      + AnsiString(StringGridTiles->ColCount)
                      + "x"
                      + AnsiString(StringGridTiles->RowCount);
    PanelInfo->Caption = Info;
  }

  StringGridTiles->Refresh();

  // Prevent scrollbar update display bug
  if (ScrollBarAddress->Focused() == true)
  {
    StringGridTiles->SetFocus();
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::UpdatePalette(void)
{
  tNativeCPC* NativeCPC = FormMain->NativeCPC;

  for (int Loop=0; Loop < GA_INK_VALUES_NBELEMENT; Loop++)
  {
    colours_rgb_entry Color = NativeCPC->colours_rgb[NativeCPC->GateArray->ink_values[Loop]];
    mColor[Loop] = (TColor)RGB(Color.colour_r, Color.colour_g, Color.colour_b);

    // Negative grayscale
    int GrayScale = 255 - ((Color.colour_r + Color.colour_g + Color.colour_b) / 3);
    mGrayscaleColor[Loop] = (TColor)RGB(GrayScale, GrayScale, GrayScale);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::ComboBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TComboBox* ComboBox = (TComboBox*)Control;
  TCanvas* Canvas = ComboBox->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBox->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBox->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::EditZoomChange(TObject *Sender)
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
void __fastcall TFormGraphicsExplorer::EditTileXChange(TObject *Sender)
{
  if (EditTileX->Text.IsEmpty() == false)
  {
    mTileX = EditTileX->Text.ToInt();
    if (mTileX < 1) mTileX = 1;

    ResizeTiles();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormGraphicsExplorer::EditTileYChange(TObject *Sender)
{
  if (EditTileY->Text.IsEmpty() == false)
  {
    mTileY = EditTileY->Text.ToInt();
    if (mTileY < 1) mTileY = 1;

    ResizeTiles();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::StringGridTilesDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
  mDrawCells(ACol, ARow, Rect, State);
  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::DrawStringGridTiles(int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
  TCanvas* Canvas = StringGridTiles->Canvas;

  int BaseAddress = mGetTileAddress(mTopLeftAddress, ACol, ARow);
  if (BaseAddress <= (int)mRAMMax)
  {
    // Draw frame
    if (State.Contains(gdSelected))
    {
      Canvas->Brush->Color = mSettingsP->ThemeText1Color;
      Canvas->FrameRect(Rect);
      mColorsP = mGrayscaleColor;
    }
    else
    {
      Canvas->Pen->Color = clLtGray;
      Canvas->MoveTo(Rect.left, Rect.top);
      Canvas->LineTo(Rect.right-1, Rect.top);
      Canvas->LineTo(Rect.right-1, Rect.bottom-1);
      Canvas->Pen->Color = clDkGray;
      Canvas->LineTo(Rect.left, Rect.bottom-1);
      Canvas->LineTo(Rect.left, Rect.top);
      mColorsP = mColor;
    }

    int Y = Rect.Top + 1;
    for (int LoopY = 0; LoopY < mTileY; LoopY++)
    {
      int X = Rect.Left + 1;
      for (int LoopX = 0; LoopX < mTileX; LoopX++)
      {
        tULong Address = mGetByteAddress(BaseAddress, LoopX, LoopY);
        if (Address <= mRAMMax)
        {
          // Read Byte
          tUChar Data = (tUChar)mReadData(FormMain->NativeCPC, Address);
          // Draw Byte
          X = mDrawByte(Data, X, Y);
        }
        else
        {
          // Empty Byte
          Canvas->Brush->Color = mSettingsP->ThemeBackgroundColor;
          Canvas->FillRect(TRect(X, Y, X+(mZoom*8), Y+mZoom*2));
          X += 8*mZoom;
        }
      }

      Y += 2*mZoom;
    }
  }
  else
  {
    // Empty tile
    Canvas->Brush->Color = mSettingsP->ThemeBackgroundColor;
    Canvas->FillRect(Rect);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::DrawStringGridMapPreview(int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
  TCanvas* Canvas = StringGridTiles->Canvas;

  int Index = ACol + (ARow * StringGridTiles->ColCount);
  int Address = mMapStartAddress + Index;
  if (Address <= (int)mRAMMax)
  {
    tUShort Data = mMapPreviewTiles[Index].Index;
    if (Data < (int)mNbTiles)
    {
      tULong TileAddress = mFirstTileAddress + (Data * mTileX * mTileY);

      // Draw frame
      if (State.Contains(gdSelected))
      {
        Canvas->Brush->Color = mSettingsP->ThemeText1Color;
        Canvas->FrameRect(Rect);
      }
      else
      {
        Canvas->Pen->Color = clLtGray;
        Canvas->MoveTo(Rect.left, Rect.top);
        Canvas->LineTo(Rect.right-1, Rect.top);
        Canvas->LineTo(Rect.right-1, Rect.bottom-1);
        Canvas->Pen->Color = clDkGray;
        Canvas->LineTo(Rect.left, Rect.bottom-1);
        Canvas->LineTo(Rect.left, Rect.top);
      }

      mColorsP = mColor;

      // Draw Tile
      int Y = Rect.Top + 1;
      for (int LoopY = 0; LoopY < mTileY; LoopY++)
      {
        int X = Rect.Left + 1;
        for (int LoopX = 0; LoopX < mTileX; LoopX++)
        {
          tULong Address = mGetByteAddress(TileAddress, LoopX, LoopY);
          if (Address <= mRAMMax)
          {
            // Read Byte
            tUChar Data = (tUChar)mReadData(FormMain->NativeCPC, Address);
            // Draw Byte
            X = mDrawByte(Data, X, Y);
          }
          else
          {
            // Empty Byte
            Canvas->Brush->Color = mSettingsP->ThemeBackgroundColor;
            Canvas->FillRect(TRect(X, Y, X+(mZoom*8), Y+mZoom*2));
            X += 8*mZoom;
          }
        }

        Y += 2*mZoom;
      }

    }
    else
    {
      // Empty tile
      Canvas->Brush->Color = mSettingsP->ThemeBackgroundColor;
      Canvas->FillRect(Rect);

      if (State.Contains(gdSelected))
      {
        Canvas->Brush->Color = mSettingsP->ThemeText1Color;
        Canvas->FrameRect(Rect);
        mColorsP = mGrayscaleColor;
      }
    }
  }
  else
  {
    // Empty tile
    Canvas->Brush->Color = mSettingsP->ThemeBackgroundColor;
    Canvas->FillRect(Rect);

    if (State.Contains(gdSelected))
    {
      Canvas->Brush->Color = mSettingsP->ThemeText1Color;
      Canvas->FrameRect(Rect);
      mColorsP = mGrayscaleColor;
    }
  }

  NOT_USED(State);
}
//---------------------------------------------------------------------------




void __fastcall TFormGraphicsExplorer::StringGridTilesDblClick(TObject *Sender)
{
  if (mMapPreview == false)
  {
    SpeedButtonEditClick(Sender);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::ComboBoxModeChange(TObject *Sender)
{
  int Index = ComboBoxMode->ItemIndex;

  if (Index == 0)
  {
    // Read current mode
    mMode = (tUChar)FormMain->NativeCPC->GateArray->scr_mode;
  }
  else
  {
    mMode = (tUChar)(Index - 1);
  }

  switch (mMode)
  {
    case 0:
    {
      mDrawByte = DrawByteMode0;
      mGetPixelColor = GetPixelColorMode0;
    }
    break;

    case 1:
    {
      mDrawByte = DrawByteMode1;
      mGetPixelColor = GetPixelColorMode1;
    }
    break;

    case 2:
    {
      mDrawByte = DrawByteMode2;
      mGetPixelColor = GetPixelColorMode2;
    }
    break;
  }

  StringGridTiles->Refresh();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


tULong __fastcall TFormGraphicsExplorer::GetTileAddressLinear(tULong Address, int X, int Y)
{
  return (Address
          + (X * mTileY * mTileX * StringGridTiles->RowCount)
          + (Y * mTileY * mTileX));
}
//---------------------------------------------------------------------------
tULong __fastcall TFormGraphicsExplorer::GetByteAddressLinear(tULong Address, int X, int Y)
{
  return (Address + (Y * mTileX) + X);
}
//---------------------------------------------------------------------------

tULong __fastcall TFormGraphicsExplorer::GetTileAddressScreen(tULong Address, int X, int Y)
{
  return (Address
          + (X * mTileX)
          + (Y * mTileX * StringGridTiles->ColCount));
}
//---------------------------------------------------------------------------
tULong __fastcall TFormGraphicsExplorer::GetByteAddressScreen(tULong Address, int X, int Y)
{
  return (Address + (0x800 * Y) + X);
}
//---------------------------------------------------------------------------


int __fastcall TFormGraphicsExplorer::DrawByteMode0(tUChar Data, int X, int Y)
{
  tUChar Pixel0 =   ((Data & 0x02) << 2)
                  | ((Data & 0x20) >> 3)
                  | ((Data & 0x08) >> 2)
                  | ((Data & 0x80) >> 7);
  tUChar Pixel1 =   ((Data & 0x01) << 3)
                  | ((Data & 0x10) >> 2)
                  | ((Data & 0x04) >> 1)
                  | ((Data & 0x40) >> 6);

  // Draw 2 pixels
  StringGridTiles->Canvas->Brush->Color = mColorsP[Pixel0];
  StringGridTiles->Canvas->FillRect(TRect(X, Y, X+(mZoom*4), Y+mZoom*2));
  X += 4*mZoom;
  StringGridTiles->Canvas->Brush->Color = mColorsP[Pixel1];
  StringGridTiles->Canvas->FillRect(TRect(X, Y, X+(mZoom*4), Y+mZoom*2));
  X += 4*mZoom;

  return X;
}
//---------------------------------------------------------------------------
int __fastcall TFormGraphicsExplorer::DrawByteMode1(tUChar Data, int X, int Y)
{
  tUChar Pixel1 = ((Data & 0x08) >> 2) + ((Data & 0x80) >> 7);
  tUChar Pixel2 = ((Data & 0x04) >> 1) + ((Data & 0x40) >> 6);
  tUChar Pixel3 = ((Data & 0x02) >> 0) + ((Data & 0x20) >> 5);
  tUChar Pixel4 = ((Data & 0x01) << 1) + ((Data & 0x10) >> 4);

  // Draw 4 pixels
  StringGridTiles->Canvas->Brush->Color = mColorsP[Pixel1];
  StringGridTiles->Canvas->FillRect(TRect(X, Y, X+(mZoom*2), Y+mZoom*2));
  X += 2*mZoom;
  StringGridTiles->Canvas->Brush->Color = mColorsP[Pixel2];
  StringGridTiles->Canvas->FillRect(TRect(X, Y, X+(mZoom*2), Y+mZoom*2));
  X += 2*mZoom;
  StringGridTiles->Canvas->Brush->Color = mColorsP[Pixel3];
  StringGridTiles->Canvas->FillRect(TRect(X, Y, X+(mZoom*2), Y+mZoom*2));
  X += 2*mZoom;
  StringGridTiles->Canvas->Brush->Color = mColorsP[Pixel4];
  StringGridTiles->Canvas->FillRect(TRect(X, Y, X+(mZoom*2), Y+mZoom*2));
  X += 2*mZoom;

  return X;
}
//---------------------------------------------------------------------------
int __fastcall TFormGraphicsExplorer::DrawByteMode2(tUChar Data, int X, int Y)
{
  // Draw 8 pixels
  for (int Loop=0; Loop < 8; Loop++)
  {
    StringGridTiles->Canvas->Brush->Color = mColorsP[Data & 0x80 ? 1 : 0];
    StringGridTiles->Canvas->FillRect(TRect(X, Y, X+mZoom, Y+mZoom*2));
    X += mZoom;
    Data = Data << 1;
  }

  return X;
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::SetTopLeftAddress(tULong Value)
{
  mTopLeftAddress = Value;

  if (PanelFormatAddress->Caption.IsEmpty() == true)
  {
    PanelAddress->Caption = mTopLeftAddress;
  }
  else
  {
    PanelAddress->Caption = AnsiString::IntToHex(mTopLeftAddress, 5);
  }

  // Update Scrollbar position
  if (mMapPreview == false)
  {
    ScrollBarAddress->Position = mTopLeftAddress;
  }

  StringGridTiles->Refresh();

  // Update Tile address
  TileAddress = mTopLeftAddress
                + (StringGridTiles->Col * mTileY * mTileX * StringGridTiles->RowCount)
                + (StringGridTiles->Row * mTileY * mTileX);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::SetTileAddress(tULong Value)
{
  mTileAddress = Value;

  bool bHexa;
  if (mMapPreview == false)
  {
    bHexa = (PanelFormatAddress->Caption.IsEmpty() == true) ? false : true;
  }
  else
  {
    bHexa = (PanelFormatMapStart->Caption.IsEmpty() == true) ? false : true;
  }

  if (bHexa == false)
  {
    PanelTileAddress->Caption = mTileAddress;
  }
  else
  {
    PanelTileAddress->Caption = AnsiString::IntToHex(mTileAddress, 5);
  }

  // Update Tile Index
  int TileIndex = (int)(mTileAddress - mFirstTileAddress) / (mTileX * mTileY);
  if (mMapPreview == false)
  {
    if (TileIndex >= 0)
    {
      mTileIndex = (tULong)TileIndex;
      PanelTileIndex->Visible = true;

      if (PanelFormatAddress->Caption.IsEmpty() == true)
      {
        PanelTileIndex->Caption = mTileIndex;
      }
      else
      {
        PanelTileIndex->Caption = AnsiString::IntToHex(mTileIndex, 2);
      }
    }
    else
    {
      PanelTileIndex->Visible = false;
    }
  }
  else
  {
    PanelTileIndex->Visible = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::SetFirstTileAddress(tULong Value)
{
  mFirstTileAddress = Value;

  if (PanelFormatFirstTile->Caption.IsEmpty() == true)
  {
    PanelFirstTile->Caption = mFirstTileAddress;
  }
  else
  {
    PanelFirstTile->Caption = AnsiString::IntToHex(mFirstTileAddress, 5);
  }

  if (mLastTileAddress >= mFirstTileAddress)
  {
    mNbTiles = 1 + ((mLastTileAddress - mFirstTileAddress) / (mTileX * mTileY));
    UpDownMapperTiles->Position = mNbTiles;

    UpdateMapPreview();

    // Update Scrollbar position
    if (mMapPreview == true)
    {
      StringGridTiles->Refresh();
    }
  }
  else
  {
    mNbTiles = 1;
    UpDownMapperTiles->Position = mNbTiles;
  }

  // Update Tile Index
  int TileIndex = (int)(mTileAddress - mFirstTileAddress) / (mTileX * mTileY);
  if (mMapPreview == false)
  {
    if (TileIndex >= 0)
    {
      mTileIndex = (tULong)TileIndex;
      PanelTileIndex->Visible = true;

      if (PanelFormatAddress->Caption.IsEmpty() == true)
      {
        PanelTileIndex->Caption = mTileIndex;
      }
      else
      {
        PanelTileIndex->Caption = AnsiString::IntToHex(mTileIndex, 2);
      }
    }
    else
    {
      PanelTileIndex->Visible = false;
    }
  }
  else
  {
    PanelTileIndex->Visible = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::SetLastTileAddress(tULong Value)
{
  mLastTileAddress = Value;

  if (PanelFormatFirstTile->Caption.IsEmpty() == true)
  {
    PanelLastTile->Caption = mLastTileAddress;
  }
  else
  {
    PanelLastTile->Caption = AnsiString::IntToHex(mLastTileAddress, 5);
  }

  if (mLastTileAddress >= mFirstTileAddress)
  {
    mNbTiles = 1 + ((mLastTileAddress - mFirstTileAddress) / (mTileX * mTileY));
    UpDownMapperTiles->Position = mNbTiles;

    UpdateMapPreview();

    // Update Scrollbar position
    if (mMapPreview == true)
    {
      StringGridTiles->Refresh();
    }
  }
  else
  {
    mNbTiles = 1;
    UpDownMapperTiles->Position = mNbTiles;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::SetMapStartAddress(tULong Value)
{
  mMapStartAddress = Value;

  UpdateMapPreview();

  if (PanelFormatMapStart->Caption.IsEmpty() == true)
  {
    PanelMapStart->Caption = mMapStartAddress;
  }
  else
  {
    PanelMapStart->Caption = AnsiString::IntToHex(mMapStartAddress, 5);
  }

  // Update Scrollbar position
  if (mMapPreview == true)
  {
    ScrollBarAddress->Position = mMapStartAddress;
    StringGridTiles->Refresh();
  }

  // Update Tile address
  TileAddress = MapStartAddress + StringGridTiles->Col + (StringGridTiles->Row * mMapX);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::PanelFormatAddressClick(TObject *Sender)
{
  if (PanelFormatAddress->Caption.IsEmpty() == true)
  {
    PanelFormatAddress->Caption = "&&";
    PanelAddress->Caption = AnsiString::IntToHex(mTopLeftAddress, 5);
    PanelTileAddress->Caption = AnsiString::IntToHex(mTileAddress, 5);
    PanelTileIndex->Caption = AnsiString::IntToHex(mTileIndex, 2);
  }
  else
  {
    PanelFormatAddress->Caption = "";
    PanelAddress->Caption = mTopLeftAddress;
    PanelTileAddress->Caption = mTileAddress;
    PanelTileIndex->Caption = mTileIndex;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PanelAddressClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mTopLeftAddress;
  EditValue->Hexadecimal = PanelFormatAddress->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = PanelFormatAddress->Caption.IsEmpty() == true ? 6 : 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = mRAMMax;
  if (EditValue->ShowModal() == mrOk)
  {
    TopLeftAddress = EditValue->NewValue;
    StringGridTiles->Refresh();
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::ScrollBarAddressChange(TObject *Sender)
{
  if (ScrollBarAddress->Focused() == true)
  {
    if (mMapPreview == false)
    {
      TopLeftAddress = ScrollBarAddress->Position;
    }
    else
    {
      MapStartAddress = ScrollBarAddress->Position;
    }
    StringGridTiles->Refresh();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::ButtonRefreshClick(TObject *Sender)
{
  ComboBoxModeChange(Sender);
  UpdatePalette();
  UpdateMapPreview();

  StringGridTiles->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::StringGridTilesSelectCell(TObject *Sender,
          int ACol, int ARow, bool &CanSelect)
{
  if (mMapPreview == false)
  {
    TileAddress = mGetTileAddress(TopLeftAddress, ACol, ARow);
  }
  else
  {
    int Index = ACol + (ARow * mMapX);
    TileAddress = mMapPreviewTiles[Index].Address;
  }

  NOT_USED(Sender);
  NOT_USED(CanSelect);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::StringGridTilesMouseLeave(TObject *Sender)

{
  PanelCursorAddress->Caption = "";
  PanelCursorData->Caption = "";

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::StringGridTilesMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y)
{
  int ACol = X / StringGridTiles->DefaultColWidth;
  int ARow = Y / StringGridTiles->DefaultRowHeight;

  // Update cursor address
  int PixelX = X - (ACol * StringGridTiles->DefaultColWidth);
  PixelX = PixelX / ((StringGridTiles->DefaultColWidth-2) / mTileX);
  if (PixelX > (mTileX-1))
  {
    PixelX = mTileX-1;
  }
  int PixelY = Y - (ARow * StringGridTiles->DefaultRowHeight);
  PixelY = PixelY / ((StringGridTiles->DefaultRowHeight-2) / mTileY);
  if (PixelY > (mTileY-1))
  {
    PixelY = mTileY-1;
  }

  // Get Byte Address
  int TileAddress = mGetTileAddress(TopLeftAddress, ACol, ARow);
  CursorAddress = mGetByteAddress(TileAddress, PixelX, PixelY);
  tUChar Data = (tUChar)mReadData(FormMain->NativeCPC, CursorAddress);
  if (PanelFormatAddress->Caption.IsEmpty() == true)
  {
    PanelCursorAddress->Caption = CursorAddress;
    PanelCursorData->Caption = Data;
  }
  else
  {
    PanelCursorAddress->Caption = AnsiString::IntToHex(CursorAddress, 5);
    PanelCursorData->Caption = AnsiString::IntToHex(Data, 2);
  }

  NOT_USED(Sender);
  NOT_USED(Shift);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::SpeedButtonEditClick(TObject *Sender)
{
  // Prepare tile edition
  GraphicsEditorP->ShowHint = ShowHint;
  GraphicsEditorP->Emulator = mEmulatorP;
  GraphicsEditorP->Settings = mSettingsP;
  GraphicsEditorP->TileX = mTileX;
  GraphicsEditorP->TileY = mTileY;
  GraphicsEditorP->Mode = mMode;
  int Pixels=0;
  for (int LoopY=0; LoopY < mTileY; LoopY++)
  {
    for (int LoopX=0; LoopX < mTileX; LoopX++)
    {
      tULong Address = mGetByteAddress(mTileAddress, LoopX, LoopY);
      if (Address <= mRAMMax)
      {
        GraphicsEditorP->Data[Pixels++] = (tUChar)mReadData(FormMain->NativeCPC, Address);
      }
      else
      {
        GraphicsEditorP->Data[Pixels++] = 0;
      }
    }
  }
  if (GraphicsEditorP->ShowModal() == mrOk)
  {
    // Apply tile updates
    Pixels=0;
    for (int LoopY=0; LoopY < mTileY; LoopY++)
    {
      for (int LoopX=0; LoopX < mTileX; LoopX++)
      {
        tULong Address = mGetByteAddress(mTileAddress, LoopX, LoopY);
        if (Address <= mRAMMax)
        {
          Engine_WriteMem(FormMain->NativeCPC, Address, GraphicsEditorP->Data[Pixels++]);
        }
      }
    }
    StringGridTiles->Refresh();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::SpeedButtonPreviewClick(TObject *Sender)
{
  mMapPreview = SpeedButtonEdit->Down == true;
  Caption = Title;

  // Update controls accesses
  bool ShowWhenEdit = (mMapPreview == true) ? false : true;
  PanelInfo->Visible = ShowWhenEdit;
  LabelTopLeftAddress->Visible = ShowWhenEdit;
  PanelFormatAddress->Visible = ShowWhenEdit;
  PanelAddress->Visible = ShowWhenEdit;
  LabelCursorAddress->Visible = ShowWhenEdit;
  PanelCursorAddress->Visible = ShowWhenEdit;
  PanelCursorData->Visible = ShowWhenEdit;
  StringGridTiles->ColCount=1;
  StringGridTiles->RowCount=1;
  StringGridTiles->Col = 0;
  StringGridTiles->Row = 0;
  StringGridTiles->ScrollBars = (mMapPreview == true) ? ssBoth : ssNone;
  ScrollBarAddress->Position = (mMapPreview == true) ? mMapStartAddress : mTopLeftAddress;
  StringGridTiles->PopupMenu = (mMapPreview == true) ? NULL : PopupMenuTiles;

  if (mMapPreview == true)
  {
    mDrawCells = DrawStringGridMapPreview;

    // Update Tile address
    int Index = StringGridTiles->Col + (StringGridTiles->Row * mMapX);
    TileAddress = mMapPreviewTiles[Index].Address;
  }
  else
  {
    mDrawCells = DrawStringGridTiles;

    LabelTileAddress->Visible = true;
    PanelTileAddress->Visible = true;

    // Update tile address
    TileAddress = mTopLeftAddress
                  + (StringGridTiles->Col * mTileY * mTileX * StringGridTiles->RowCount)
                  + (StringGridTiles->Row * mTileY * mTileX);
  }

  ResizeTiles();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::EditTileMenuClick(TObject *Sender)
{
  if (mMapPreview == false)
  {
    SpeedButtonEditClick(Sender);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::TileAsTopLeftMenuClick(TObject *Sender)
{
  // Set Tile address as Top Left address
  TopLeftAddress = TileAddress;
  StringGridTiles->Refresh();
  StringGridTiles->Row = 0;
  StringGridTiles->Col = 0;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::CursorAsTopLeftMenuClick(TObject *Sender)
{
  // Set Cursor address as Top Left address
  TopLeftAddress = CursorAddress;
  StringGridTiles->Refresh();
  StringGridTiles->Row = 0;
  StringGridTiles->Col = 0;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::SelectedTileAsFirstTileMenuClick(TObject *Sender)

{
  FirstTileAddress = TileAddress;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::SelectedTileAsLastTileMenuClick(TObject *Sender)

{
  LastTileAddress = TileAddress;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::ComboBoxAddressingChange(TObject *Sender)
{
  switch (ComboBoxAddressing->ItemIndex)
  {
    case 0:
    {
      mGetByteAddress = GetByteAddressLinear;
      mGetTileAddress = GetTileAddressLinear;
    }
    break;

    case 1:
    {
      mGetByteAddress = GetByteAddressScreen;
      mGetTileAddress = GetTileAddressScreen;
      TopLeftAddress = 0xC000;

    }
    break;
  }

  ResizeTiles();
  StringGridTiles->Refresh();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::ComboBoxMemoryChange(TObject *Sender)
{
  switch (ComboBoxMemory->ItemIndex)
  {
    case 0:
    {
      mReadData = Engine_ReadMem;
      mRAMMax = 65535;
    }
    break;

    case 1:
    {
      mReadData = Engine_ReadRAM;
      mRAMMax = (FormMain->NativeCPC->ram_size * 1024) - 1;
    }
    break;
  }

  if (ScrollBarAddress->Position > (int)mRAMMax) ScrollBarAddress->Position = (int)mRAMMax;
  ScrollBarAddress->Max = mRAMMax;

  ResizeTiles();
  UpdateMapPreview();
  StringGridTiles->Refresh();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::ButtonByteIncrementClick(TObject *Sender)
{
  if (mMapPreview == false)
  {
    if (TopLeftAddress < mRAMMax) TopLeftAddress++;
  }
  else
  {
    if (MapStartAddress < mRAMMax) MapStartAddress++;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::ButtonByteDecrementClick(TObject *Sender)
{
  if (mMapPreview == false)
  {
    if (TopLeftAddress > 0) TopLeftAddress--;
  }
  else
  {
    if (MapStartAddress > 0) MapStartAddress--;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::ButtonMonitorClick(TObject *Sender)
{
  mMonitoring = mMonitoring == true ? false : true;
  Caption = Title;

  if (mMonitoring == true)
  {
    ButtonMonitor->Caption = "Stop";
    if (FormMain != NULL)
    {
      FormMain->SubscribeToCPCEvents(OnStepByStepEvent);
    }
  }
  else
  {
    ButtonMonitor->Caption = "Start";
    if (FormMain != NULL)
    {
      FormMain->UnsubscribeToCPCEvents(OnStepByStepEvent);
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


bool __fastcall TFormGraphicsExplorer::OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  if (mPaused != Pause)
  {
    mPaused = Pause;
    Caption = Title;
  }

  if (Condition & (EC_FRAME_COMPLETE | EC_END_INSTRUCTION))
  {
    UpdatePalette();
    StringGridTiles->Refresh();
  }

  return Pause;
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PageControlExplorerDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = PageControlExplorer->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }

  // Get sheet corresponding to TabIndex
  int TabCount=0;
  TTabSheet* Sheet = NULL;
  for (int Loop=0; Loop < PageControlExplorer->PageCount; Loop++)
  {
    if (PageControlExplorer->Pages[Loop]->TabVisible == true)
    {
      if (TabCount == TabIndex)
      {
        Sheet = PageControlExplorer->Pages[Loop];
        break;
      }

      TabCount++;
    }
  }

  // Draw tab
  if (Sheet != NULL)
  {
    Canvas->FillRect(Rect);
    AnsiString Text = Sheet->Caption;
    int TextWidth = Canvas->TextWidth(Text);
    int TextHeight = Canvas->TextHeight(Text);
    int TextPosX = (Rect.Width() - TextWidth) / 2;
    int TextPosY = (PageControlExplorer->TabHeight - TextHeight) / 2;
    Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------



void __fastcall TFormGraphicsExplorer::PanelFormatFirstTileClick(TObject *Sender)

{
  if (PanelFormatFirstTile->Caption.IsEmpty() == true)
  {
    PanelFormatFirstTile->Caption = "&&";
    PanelFirstTile->Caption = AnsiString::IntToHex(mFirstTileAddress, 5);
    PanelLastTile->Caption = AnsiString::IntToHex(mLastTileAddress, 5);
  }
  else
  {
    PanelFormatFirstTile->Caption = "";
    PanelFirstTile->Caption = mFirstTileAddress;
    PanelLastTile->Caption = mLastTileAddress;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PanelFirstTileClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mFirstTileAddress;
  EditValue->Hexadecimal = PanelFormatAddress->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = PanelFormatAddress->Caption.IsEmpty() == true ? 6 : 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = mRAMMax;
  EditValue->Increment = mTileX * mTileY;
  if (EditValue->ShowModal() == mrOk)
  {
    FirstTileAddress = EditValue->NewValue;
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PanelLastTileClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mLastTileAddress;
  EditValue->Hexadecimal = PanelFormatAddress->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = PanelFormatAddress->Caption.IsEmpty() == true ? 6 : 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = mRAMMax;
  EditValue->Increment = mTileX * mTileY;
  if (EditValue->ShowModal() == mrOk)
  {
    LastTileAddress = EditValue->NewValue;
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::EditMapperTilesChange(TObject *Sender)
{
  if (EditMapperTiles->Text.IsEmpty() == false)
  {
    mNbTiles = EditMapperTiles->Text.ToInt();
    if (mNbTiles < 1) mNbTiles = 1;

    // Update Last Tile Address
    LastTileAddress = mFirstTileAddress + ((mNbTiles - 1) * (mTileX * mTileY));
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::PanelFormatMapStartClick(TObject *Sender)
{
  if (PanelFormatMapStart->Caption.IsEmpty() == true)
  {
    PanelFormatMapStart->Caption = "&&";
    PanelMapStart->Caption = AnsiString::IntToHex(mMapStartAddress, 5);
    if (mMapPreview == true)
    {
      PanelTileAddress->Caption = AnsiString::IntToHex(mTileAddress, 5);
    }
  }
  else
  {
    PanelFormatMapStart->Caption = "";
    PanelMapStart->Caption = mMapStartAddress;
    if (mMapPreview == true)
    {
      PanelTileAddress->Caption = mTileAddress;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PanelMapStartClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mMapStartAddress;
  EditValue->Hexadecimal = PanelFormatMapStart->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = PanelFormatMapStart->Caption.IsEmpty() == true ? 6 : 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = mRAMMax;
  if (EditValue->ShowModal() == mrOk)
  {
    MapStartAddress = EditValue->NewValue;
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::EditMapXChange(TObject *Sender)
{
  if (EditMapX->Text.IsEmpty() == false)
  {
    mMapX = EditMapX->Text.ToInt();
    if (mMapX < 1) mMapX = 1;

    ResizeTiles();
    UpdateMapPreview();
    if (mMapPreview == true)
    {
      StringGridTiles->Refresh();
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PanelMapXReverseClick(TObject *Sender)
{
  if (PanelMapXReverse->Caption.IsEmpty() == true)
  {
    PanelMapXReverse->Caption = "R";
  }
  else
  {
    PanelMapXReverse->Caption = "";
  }

  UpdateMapPreview();
  if (mMapPreview == true)
  {
    StringGridTiles->Refresh();
  }

  if (mMapPreview == true)
  {
    // Update Tile address
    int Index = StringGridTiles->Col + (StringGridTiles->Row * mMapX);
    TileAddress = mMapPreviewTiles[Index].Address;
  }
  else
  {
    // Update tile address
    TileAddress = mTopLeftAddress
                  + (StringGridTiles->Col * mTileY * mTileX * StringGridTiles->RowCount)
                  + (StringGridTiles->Row * mTileY * mTileX);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::EditMapYChange(TObject *Sender)
{
  if (EditMapY->Text.IsEmpty() == false)
  {
    mMapY = EditMapY->Text.ToInt();
    if (mMapY < 1) mMapY = 1;

    ResizeTiles();
    UpdateMapPreview();
    if (mMapPreview == true)
    {
      StringGridTiles->Refresh();
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PanelMapYReverseClick(TObject *Sender)
{
  if (PanelMapYReverse->Caption.IsEmpty() == true)
  {
    PanelMapYReverse->Caption = "R";
  }
  else
  {
    PanelMapYReverse->Caption = "";
  }

  UpdateMapPreview();
  if (mMapPreview == true)
  {
    StringGridTiles->Refresh();
  }

  if (mMapPreview == true)
  {
    // Update Tile address
    int Index = StringGridTiles->Col + (StringGridTiles->Row * mMapX);
    TileAddress = mMapPreviewTiles[Index].Address;
  }
  else
  {
    // Update tile address
    TileAddress = mTopLeftAddress
                  + (StringGridTiles->Col * mTileY * mTileX * StringGridTiles->RowCount)
                  + (StringGridTiles->Row * mTileY * mTileX);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::PageControlExplorerChange(TObject *Sender)
{
  if (PageControlExplorer->ActivePage == TabSheetTiles)
  {
    // Restore Edit Button
    SpeedButtonEdit->Caption = "Edit Tile";
    SpeedButtonEdit->OnClick = SpeedButtonEditClick;
    SpeedButtonEdit->AllowAllUp = false;
    SpeedButtonEdit->GroupIndex = 0;
  }
  else if (PageControlExplorer->ActivePage == TabSheetMapper)
  {
    // Change Edit Button
    SpeedButtonEdit->Caption = "Preview";
    SpeedButtonEdit->OnClick = SpeedButtonPreviewClick;
    SpeedButtonEdit->AllowAllUp = true;
    SpeedButtonEdit->GroupIndex = 1;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::PageControlExplorerChanging(TObject *Sender,
          bool &AllowChange)
{
  if (mMapPreview == true)
  {
    AllowChange = false;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::MapperSetTopLeftTileAddressMenuClick(TObject *Sender)

{
  TPanel* Panel = (TPanel*)PopupMenuMapper->PopupComponent;

  if (Panel == PanelMapStart) MapStartAddress = TopLeftAddress;
  else if (Panel == PanelFirstTile) FirstTileAddress = TopLeftAddress;
  else if (Panel == PanelLastTile) LastTileAddress = TopLeftAddress;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::MapperSetSelectedTileAddressMenuClick(TObject *Sender)

{
  TPanel* Panel = (TPanel*)PopupMenuMapper->PopupComponent;

  if (Panel == PanelMapStart) MapStartAddress = TileAddress;
  else if (Panel == PanelFirstTile) FirstTileAddress = TileAddress;
  else if (Panel == PanelLastTile) LastTileAddress = TileAddress;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------





void __fastcall TFormGraphicsExplorer::ButtonMapperCopyClick(TObject *Sender)
{
  // Create bitmap
  TBitmap* BitmapP = new TBitmap();

  if (BitmapP)
  {
    int SizeX;
    switch (mMode)
    {
      case 0: SizeX = 4; break;
      case 1: SizeX = 2; break;
      case 2: SizeX = 1; break;
    }

    // Populate bitmap
    if (mMapPreview == false)
    {
      BitmapP->PixelFormat = pf32bit;
      BitmapP->Width = mTileX * 8 * StringGridTiles->ColCount;
      BitmapP->Height = mTileY * 2 * StringGridTiles->RowCount;

      //
      // Copy Current Tiles Display
      //
      for (int TileY = 0; TileY < StringGridTiles->RowCount; TileY++)
      {
        for (int TileX = 0; TileX < StringGridTiles->ColCount; TileX++)
        {
          int TileAddress = mGetTileAddress(TopLeftAddress, TileX, TileY);

          int BaseMapX = TileX * mTileX * 8;
          int BaseMapY = TileY * mTileY * 2;

          for (int ByteY = 0; ByteY < mTileY; ByteY++)
          {
            for (int ByteX = 0; ByteX < mTileX; ByteX++)
            {
              tULong Address = mGetByteAddress(TileAddress, ByteX, ByteY);
              tUChar Data = 0;
              if (Address <= mRAMMax)
              {
                // Read data
                Data = (tUChar)mReadData(FormMain->NativeCPC, Address);
              }

              int PixelX = BaseMapX + (ByteX * 8);
              int PixelY = BaseMapY + (ByteY * 2);
              for (int X=0; X < 8; X++)
              {
                //BitmapP->Canvas->Pixels[X+(ByteX * 8)][Y+(ByteY * 2)] = mGetPixelColor(Data, X/SizeX);
                TColor Color = mGetPixelColor(Data, X/SizeX);
                BitmapP->Canvas->Pixels[PixelX + X][PixelY+0] = Color;
                BitmapP->Canvas->Pixels[PixelX + X][PixelY+1] = Color;
              }
            }
          }
        }
      }

      // Display message
			Application->MessageBox(_T("All shown tiles have been copied to clipboard."), _T("Clipboard"), MB_OK);
    }
    else
    {
      BitmapP->PixelFormat = pf32bit;
      BitmapP->Width = mTileX * 8 * mMapX;
      BitmapP->Height = mTileY * 2 * mMapY;

      Screen->Cursor = crHourGlass;

      //
      // Copy Map
      //
      for (int TileY = 0; TileY < (int)mMapY; TileY++)
      {
        for (int TileX = 0; TileX < (int)mMapX; TileX++)
        {
          tUShort TileIndex = mMapPreviewTiles[TileX + (TileY * mMapX)].Index;
          tULong TileAddress = mFirstTileAddress + (TileIndex * mTileX * mTileY);

          int BaseMapX = TileX * mTileX * 8;
          int BaseMapY = TileY * mTileY * 2;

          for (int ByteY = 0; ByteY < mTileY; ByteY++)
          {
            for (int ByteX = 0; ByteX < mTileX; ByteX++)
            {
              tULong Address = mGetByteAddress(TileAddress, ByteX, ByteY);
              tUChar Data = 0;
              if ( (Address <= mRAMMax) && (TileIndex < (tUShort)mNbTiles) )
              {
                // Read data
                Data = (tUChar)mReadData(FormMain->NativeCPC, Address);
              }

              int PixelX = BaseMapX + (ByteX * 8);
              int PixelY = BaseMapY + (ByteY * 2);
              for (int X=0; X < 8; X++)
              {
                TColor Color = mGetPixelColor(Data, X/SizeX);
                BitmapP->Canvas->Pixels[PixelX + X][PixelY+0] = Color;
                BitmapP->Canvas->Pixels[PixelX + X][PixelY+1] = Color;
              }
            }
          }
        }
      }

      Screen->Cursor = crDefault;

      // Display message
			Application->MessageBox(_T("Entire map preview has been copied to clipboard."), _T("Clipboard"), MB_OK);
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
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


TColor __fastcall TFormGraphicsExplorer::GetPixelColorMode0(tUChar Data, int X)
{
  tUChar Color;
  switch (X % 2)
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
TColor __fastcall TFormGraphicsExplorer::GetPixelColorMode1(tUChar Data, int X)
{
  tUChar Color;
  switch (X % 4)
  {
    case 0: Color = ((Data & 0x08) >> 2) + ((Data & 0x80) >> 7); break;
    case 1: Color = ((Data & 0x04) >> 1) + ((Data & 0x40) >> 6); break;
    case 2: Color = ((Data & 0x02) >> 0) + ((Data & 0x20) >> 5); break;
    case 3: Color = ((Data & 0x01) << 1) + ((Data & 0x10) >> 4); break;
  }

  return (mColor[Color]);
}
//---------------------------------------------------------------------------
TColor __fastcall TFormGraphicsExplorer::GetPixelColorMode2(tUChar Data, int X)
{
  return (mColor[(Data >> (7 - (X % 8))) & 0x01]);
}
//---------------------------------------------------------------------------



void __fastcall TFormGraphicsExplorer::ComboBoxIndexingChange(TObject *Sender)
{
  switch (ComboBoxIndexing->ItemIndex)
  {
    // Compresses RL0
    case 1:
    {
      mDrawMapPreview = UpdateMapPreviewDataCompressedRL0;
    }
    break;

    // Linear words Big Endian
    case 2:
    {
      mDrawMapPreview = UpdateMapPreviewDataLinearWordsBigEndian;
    }
    break;

    // Linear bytes
    default:
    {
      mDrawMapPreview = UpdateMapPreviewDataLinearBytes;
    }
    break;
  }

  UpdateMapPreview();

  if (mMapPreview == true)
  {
    StringGridTiles->Refresh();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::UpdateMapPreview(void)
{
  if (FormMain->NativeCPC == NULL) return;
  if (mReadData == NULL) return;

  delete [] mMapPreviewTiles;
  mMapPreviewTiles = new tGraphicsExplorerTilePreview[mMapX * mMapY];

  Screen->Cursor = crHourGlass;

  mPreviewAddress = mMapStartAddress;
  mPreviewOccurences = 0;

  // Forward X
  if (PanelMapXReverse->Caption.IsEmpty() == true)
  {
    // Forward Y
    if (PanelMapYReverse->Caption.IsEmpty() == true)
    {
      for (int LoopY=0; LoopY < (int)mMapY; LoopY++)
      {
        for (int LoopX=0; LoopX < (int)mMapX; LoopX++)
        {
          if (mPreviewAddress <= mRAMMax)
          {
            mDrawMapPreview((LoopY * mMapX) + LoopX);
          }
        }
      }
    }
    // Reverse Y
    else
    {
      for (int LoopY=(int)mMapY; LoopY; LoopY--)
      {
        for (int LoopX=0; LoopX < (int)mMapX; LoopX++)
        {
          if (mPreviewAddress <= mRAMMax)
          {
            mDrawMapPreview(((LoopY-1) * mMapX) + LoopX);
          }
        }
      }
    }
  }
  // Reverse X
  else
  {
    // Forward Y
    if (PanelMapYReverse->Caption.IsEmpty() == true)
    {
      for (int LoopY=0; LoopY < (int)mMapY; LoopY++)
      {
        for (int LoopX=(int)mMapX; LoopX; LoopX--)
        {
          if (mPreviewAddress <= mRAMMax)
          {
            mDrawMapPreview((LoopY * mMapX) + (LoopX-1));
          }
        }
      }
    }
    // Reverse Y
    else
    {
      for (int LoopY=(int)mMapY; LoopY; LoopY--)
      {
        for (int LoopX=(int)mMapX; LoopX; LoopX--)
        {
          if (mPreviewAddress <= mRAMMax)
          {
            mDrawMapPreview(((LoopY-1) * mMapX) + (LoopX-1));
          }
        }
      }
    }
  }

  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------


void __fastcall TFormGraphicsExplorer::UpdateMapPreviewDataLinearBytes(int Index)
{
  mMapPreviewTiles[Index].Index = (tUChar)mReadData(FormMain->NativeCPC, mPreviewAddress) % mNbTiles;
  mMapPreviewTiles[Index].Address = mPreviewAddress;
  mPreviewAddress++;
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::UpdateMapPreviewDataCompressedRL0(int Index)
{
  if (mPreviewOccurences > 0)
  {
    mMapPreviewTiles[Index].Index = 0U;
    mMapPreviewTiles[Index].Address = mPreviewAddress;
    mPreviewOccurences--;
    if (mPreviewOccurences == 0U)
    {
      mPreviewAddress += 2;
    }
  }
  else
  {
    if (mPreviewAddress <= mRAMMax)
    {
      tUChar Data = (tUChar)mReadData(FormMain->NativeCPC, mPreviewAddress);
      mMapPreviewTiles[Index].Index = Data;
      mMapPreviewTiles[Index].Address = mPreviewAddress;

      if (Data == 0U)
      {
        if ((mPreviewAddress+1) <= mRAMMax)
        {
          mPreviewOccurences = (tUChar)mReadData(FormMain->NativeCPC, mPreviewAddress+1) - 1;
          if (mPreviewOccurences == 0U)
          {
            mPreviewAddress += 2;
          }
        }
      }
      else
      {
        mPreviewAddress++;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormGraphicsExplorer::UpdateMapPreviewDataLinearWordsBigEndian(int Index)
{
  tUShort TileIndex = (tUShort)mReadData(FormMain->NativeCPC, mPreviewAddress);
  TileIndex = TileIndex << 8;
  TileIndex = TileIndex + (tUShort)mReadData(FormMain->NativeCPC, mPreviewAddress+1);
  TileIndex = TileIndex % mNbTiles;

  mMapPreviewTiles[Index].Index = TileIndex;
  mMapPreviewTiles[Index].Address = mPreviewAddress;
  mPreviewAddress += 2;
}
//---------------------------------------------------------------------------

