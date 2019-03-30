/*
 CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
 Copyright (C) 2014-2016 by Frédéric Coste

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
#pragma hdrstop

#include "TFormDebugDSKViewer.h"
#include "TFormMain.h"
#include "..\..\Common\ASCII.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

#undef SHOW_ASCII
#define SHOW_ASCII

//=================================


static const AnsiString cTitle = "Disk viewer";
static const AnsiString cTitleMonitoring = " - [Monitoring]";

static const AnsiString cDriveALabel = "Drive A";
static const AnsiString cDriveBLabel = "Drive B";

static const AnsiString cStartMonitoringLabel = "Start";
static const AnsiString cStopMonitoringLabel = "Stop";


#define COLOR_TRACK_OK          clBlack
#define COLOR_TRACK_OVERSIZE    clRed

#define COLOR_GAPS              clLime
#define COLOR_WEAK              clYellow
#define COLOR_ERROR             TColor(RGB(255,70,70))   // Pastel Red
#define COLOR_SIZE_0            TColor(RGB(255,128,240)) // Pastel Pink
#define COLOR_SIZE_1            TColor(RGB(255,128,240)) // Pastel Pink
#define COLOR_SIZE_3            TColor(RGB(255,170,35))  // Pastel Orange
#define COLOR_SIZE_4            TColor(RGB(255,170,35))  // Pastel Orange
#define COLOR_SIZE_5            TColor(RGB(255,170,35))  // Pastel Orange
#define COLOR_SIZE_6            TColor(RGB(255,170,35))  // Pastel Orange
#define COLOR_SIZE_7            TColor(RGB(255,170,35))  // Pastel Orange
#define COLOR_INCOMPLETE        clAqua
#define COLOR_COMMON            clWhite
#define COLOR_EMPTY             clLtGray

const AnsiString GAPSSectorLabel = "GAPS";
const AnsiString WEAKSectorLabel = "WEAK";
const AnsiString ERRORSectorLabel = "ERROR";
const AnsiString SIZE0SectorLabel = "SIZE 0";
const AnsiString SIZE1SectorLabel = "SIZE 1";
const AnsiString SIZE3SectorLabel = "SIZE 3";
const AnsiString SIZE4SectorLabel = "SIZE 4";
const AnsiString SIZE5SectorLabel = "SIZE 5";
const AnsiString INCOMPLETESectorLabel = "INCOMPLETE";
const AnsiString COMMONSectorLabel = "COMMON";
const AnsiString EMPTYSectorLabel = "EMPTY";


TFormDebugDSKViewer *FormDebugDSKViewer;
//---------------------------------------------------------------------------
__fastcall TFormDebugDSKViewer::TFormDebugDSKViewer(TComponent* Owner)
  : TForm(Owner)
{
  mEmulatorP = NULL;
  mSettingsP = NULL;
  mStartDriveP = NULL;
  mMonitoringInProgress = false;

  DoubleBuffered = true;
  StringGridSectors->DoubleBuffered = true;

  // Apply Theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  PanelSectorData->Width = 468;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugDSKViewer::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  Caption = Title;
  ButtonMonitoring->Caption = cStartMonitoringLabel;

  mDataASCII7Bits = false;
  ASCII7bitsMenu->Checked = mDataASCII7Bits;

  // Force display update
  FormMain->NativeCPC->FDC->DriveA->signal_content_change++;
  OnDiskEvent(FormMain->NativeCPC->FDC->DriveA);

  if (mStartDriveP != NULL)
  {
    if (mStartDriveP == FormMain->NativeCPC->FDC->DriveA)
    {
      ComboBoxDrive->ItemIndex = ComboBoxDrive->Items->IndexOf(cDriveALabel);
    }
    else if (mStartDriveP == FormMain->NativeCPC->FDC->DriveB)
    {
      ComboBoxDrive->ItemIndex = ComboBoxDrive->Items->IndexOf(cDriveBLabel);
    }

    mStartDriveP = NULL;

    if (ComboBoxDrive->ItemIndex != -1)
    {
      ShowDriveContent(0);
      StringGridSectors->Invalidate();
    }
  }

  // Subscribe to events
  if (FormMain != NULL)
  {
    FormMain->SubscribeToDiskEvents(OnDiskEvent);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::FormHide(TObject *Sender)
{
  mMonitoringInProgress = false;

  // Unsubscribe to events
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToDiskEvents(OnDiskEvent);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDebugDSKViewer::GetTitle(void)
{
  AnsiString Title = cTitle;

  tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ? FormMain->NativeCPC->FDC->DriveA : FormMain->NativeCPC->FDC->DriveB;
  if (DriveP)
  {
    if (DriveP->tracks)
    {
      if (ComboBoxDrive->Text == cDriveALabel)
      {
        Title += " - " + ExtractFileName(mEmulatorP->DiskAFilename);
      }
      else
      {
        Title += " - " + ExtractFileName(mEmulatorP->DiskBFilename);
      }
    }
  }

  if (mMonitoringInProgress)
  {
    Title += cTitleMonitoring;
  }

  return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->DSKViewerFormLeft != 0)
       || (mSettingsP->DSKViewerFormTop != 0)
       || (mSettingsP->DSKViewerFormWidth != 0)
       || (mSettingsP->DSKViewerFormHeight != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->DSKViewerFormLeft;
    Top = mSettingsP->DSKViewerFormTop;
    Width = mSettingsP->DSKViewerFormWidth;
    Height = mSettingsP->DSKViewerFormHeight;
  }

  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  StringGridSectors->Color = Color;
  MemoSectorInfo->Color = Color;
  MemoSectorInfo->Font->Color = mSettingsP->ThemeText2Color;
  MemoDataSector->Color = Color;
  MemoDataSector->Font->Color = mSettingsP->ThemeText2Color;
  MemoDataGAP3->Color = Color;
  MemoDataGAP3->Font->Color = mSettingsP->ThemeText2Color;
  MemoDataDeclared->Color = Color;
  MemoDataDeclared->Font->Color = mSettingsP->ThemeText2Color;
  tULong Threshold = (((Color & 0xff0000) >> 16) + ((Color & 0xff00) >> 8) + (Color & 0xff)) / 3;
  if (Threshold >= 0x80)
  {
    Threshold = 0x40;
  }
  else
  {
    Threshold = 0xC0;
  }
  mCellFrameColor = TColor((Threshold << 16) | (Threshold << 8) | Threshold);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->DSKViewerFormWidth = Width;
    mSettingsP->DSKViewerFormHeight = Height;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->DSKViewerFormLeft = Left;
    mSettingsP->DSKViewerFormTop = Top;
  }

  NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::OnDiskEvent(tDrive* DriveP)
{
tDrive* DisplayedDrive = (ComboBoxDrive->Text == cDriveALabel) ?
        FormMain->NativeCPC->FDC->DriveA :
        FormMain->NativeCPC->FDC->DriveB;

  if (DriveP->signal_track_format)
  {
    if (DisplayedDrive == DriveP)
    {
      // Update graphic
      ShowDriveContent(StringGridSectors->Col);
    }
  }

  if (DriveP->signal_sector_write)
  {
    if (DisplayedDrive == DriveP)
    {
      // Update graphic
      ShowSideContent(StringGridSectors->Col);

      ShowTrackInfo(StringGridSectors->Col);
      ShowSectorInfo(StringGridSectors->Col, StringGridSectors->Row - 1);
      ShowData(StringGridSectors->Col, StringGridSectors->Row - 1);
    }
  }

  if (DriveP->signal_content_change)
  {
    // Check available DSK
    ComboBoxDrive->Clear();
    if (FormMain->NativeCPC->FDC->DriveA->dataP != NULL) ComboBoxDrive->Items->Add(cDriveALabel);
    if (FormMain->NativeCPC->FDC->DriveB->dataP != NULL) ComboBoxDrive->Items->Add(cDriveBLabel);

    ComboBoxSides->Clear();

    StringGridSectors->Visible = false;
    GroupBoxTrack->Visible = false;
    PanelSectorState->Visible = false;
    PageControlData->Visible = false;
    TabSheetDataSector->TabVisible = false;
    TabSheetDataGAP3->TabVisible = false;
    TabSheetDataDeclared->TabVisible = false;
    ButtonMonitoring->Enabled = ComboBoxDrive->Items->Count ? true : false;
    ComboBoxSides->Enabled = false;

    if (ComboBoxDrive->Items->Count == 2)
    {
      if (FormMain->NativeCPC->FDC->active_drive == FormMain->NativeCPC->FDC->DriveB)
      {
        ComboBoxDrive->ItemIndex = 1;
      }
      else
      {
        ComboBoxDrive->ItemIndex = 0;
      }
      ComboBoxDrive->Enabled = true;
      ShowDriveContent(0);
    }
    else if (ComboBoxDrive->Items->Count == 1)
    {
      ComboBoxDrive->ItemIndex = 0;
      ComboBoxDrive->Enabled = false;
      ShowDriveContent(0);
    }
    else
    {
      ComboBoxDrive->Enabled = false;
      Caption = Title;

      GroupBoxDisk->Visible = true;

      MemoDiskInfo->Lines->BeginUpdate();
      MemoDiskInfo->Clear();
      MemoDiskInfo->Lines->Add("No disk");
      MemoDiskInfo->Lines->EndUpdate();
    }
  }

  if ( (mMonitoringInProgress == true) && DriveP->signal_new_position )
  {
    // Display active drive
    if (DisplayedDrive != DriveP)
    {
      if (DriveP == mEmulatorP->NativeCPC->FDC->DriveA)
      {
        ComboBoxDrive->ItemIndex = ComboBoxDrive->Items->IndexOf(cDriveALabel);
      }
      else
      {
        ComboBoxDrive->ItemIndex = ComboBoxDrive->Items->IndexOf(cDriveBLabel);
      }
      ShowDriveContent(0);
    }

    StringGridSectors->Col = DriveP->current_track;
    StringGridSectors->Row = DriveP->current_sector + 1;

    ShowTrackInfo(DriveP->current_track);
    ShowSectorInfo(DriveP->current_track, DriveP->current_sector);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ComboBoxDriveChange(TObject *Sender)
{
  ShowDriveContent(0);
  StringGridSectors->Invalidate();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugDSKViewer::ComboBoxDriveDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxDrive->Canvas;

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
  AnsiString Text = ComboBoxDrive->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxDrive->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ComboBoxSidesChange(TObject *Sender)
{
  mCurrentSide = ComboBoxSides->ItemIndex;

  tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
          FormMain->NativeCPC->FDC->DriveA :
          FormMain->NativeCPC->FDC->DriveB;

  if (DriveP->sides && DriveP->flipped)
  {
    mCurrentSide ^= 1;
  }

  ShowSideContent(0);

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugDSKViewer::ComboBoxSidesDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxSides->Canvas;

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
  AnsiString Text = ComboBoxSides->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxSides->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ShowDriveContent(int TrackSelected)
{
tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
          FormMain->NativeCPC->FDC->DriveA :
          FormMain->NativeCPC->FDC->DriveB;

  if (DriveP->tracks)
  {
    Caption = Title;

    // Update sides
    if ( ((int)(DriveP->sides+1) != ComboBoxSides->Items->Count)
         || (TrackSelected == 0) )
    {
      ComboBoxSides->Clear();
      for (int Loop=0; Loop < (int)(DriveP->sides+1); Loop++)
      {
        AnsiString SideTitle = "Side " + AnsiString(Loop+1);
        ComboBoxSides->Items->Add(SideTitle);
      }
      mCurrentSide = 0;
    }
    ComboBoxSides->ItemIndex = mCurrentSide;
    if (ComboBoxSides->Items->Count > 1) ComboBoxSides->Enabled = true;

    if (DriveP->sides && DriveP->flipped)
    {
      mCurrentSide ^= 1;
    }

    ShowSideContent(TrackSelected);

    //
    // Show disk info
    //
    MemoDiskInfo->Lines->BeginUpdate();
    MemoDiskInfo->Clear();

    AnsiString Info = "Tracks=" + AnsiString(DriveP->tracks) + " (&" + AnsiString::IntToHex(DriveP->tracks, 2) + ")";
    MemoDiskInfo->Lines->Add(Info);
    Info = "Sides=" + AnsiString(DriveP->sides + 1);
    MemoDiskInfo->Lines->Add(Info);
    Info = "Write Protected=";
    if (DriveP->write_protected)
    {
      Info += "Yes";
    }
    else
    {
      Info += "No";
    }
    MemoDiskInfo->Lines->Add(Info);
    Info = "Header=" + AnsiString((char*)DriveP->dataP).SubString(1, 34);
    MemoDiskInfo->Lines->Add(Info);

    MemoDiskInfo->Lines->EndUpdate();
  }
  else
  {
    StringGridSectors->Visible = false;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ShowSideContent(int TrackSelected)
{
tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
            FormMain->NativeCPC->FDC->DriveA :
            FormMain->NativeCPC->FDC->DriveB;

  StringGridSectors->Visible = true;
  StringGridSectors->ColCount = DriveP->tracks;

  tULong NbMaxSectors = 0;
  
  for (tULong Loop=0; Loop < DriveP->tracks; Loop++)
  {
    StringGridSectors->Cells[Loop][0] = AnsiString(Loop);
    if (DriveP->track[Loop]->sectors > NbMaxSectors)
    {
      NbMaxSectors = DriveP->track[Loop][mCurrentSide].sectors;
    }
  }

  StringGridSectors->RowCount = NbMaxSectors + 1; // Add header row
  StringGridSectors->Col = TrackSelected;
  if ( ((int)DriveP->current_track == TrackSelected)
       && (StringGridSectors->RowCount > 1) )
  {
    StringGridSectors->Row = 1;
  }

  // Any sectors ?
  if (StringGridSectors->RowCount > 1)
  {
    ShowTrackInfo(StringGridSectors->Col);
    ShowSectorInfo(StringGridSectors->Col, StringGridSectors->Row - 1);
    ShowData(StringGridSectors->Col, StringGridSectors->Row - 1);

    if (StringGridSectors->Enabled == true)
    {
      ActiveControl = StringGridSectors;
      StringGridSectors->Invalidate();
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugDSKViewer::StringGridSectorsDrawCell(TObject *Sender,
    int ACol, int ARow, TRect &Rect, TGridDrawState State) 
{
  TRect CellRect;
#if (__BORLANDC__ == 0x651)
  CellRect.Left = Rect.Left + 1 - 4; // Fix Left issue with RADStudio XE3
#else /* __BORLANDC__ */
  CellRect.Left = Rect.Left + 1;
#endif /* __BORLANDC__ */
  CellRect.Right = Rect.Right - 1;
  CellRect.Top = Rect.Top + 1;
  CellRect.Bottom = Rect.Bottom - 1;

  // Draw headers ?
  if (ARow == 0)
  {
    tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
              FormMain->NativeCPC->FDC->DriveA :
              FormMain->NativeCPC->FDC->DriveB;
    tTrack* CurrentTrack = &DriveP->track[ACol][mCurrentSide];

    if (CurrentTrack->real_size > FDC_MAX_BYTES_PER_TRACK)
    {
      StringGridSectors->Canvas->Brush->Color = COLOR_TRACK_OVERSIZE;
    }
    else
    {
      StringGridSectors->Canvas->Brush->Color = COLOR_TRACK_OK;
    }

    StringGridSectors->Canvas->Font->Color = clWhite;
    StringGridSectors->Canvas->FillRect(CellRect);
    
    String CellText = StringGridSectors->Cells[ACol][ARow];
    int TextWidth = StringGridSectors->Canvas->TextWidth(CellText);
    int TextPosH = (StringGridSectors->ColWidths[ACol] - TextWidth) / 2;
    int TextHeight = StringGridSectors->Canvas->TextHeight(CellText);
    int TextPosV = (StringGridSectors->RowHeights[ARow] - TextHeight) / 2;
    StringGridSectors->Canvas->TextOut(CellRect.Left + TextPosH - 1,
        CellRect.Top + TextPosV, CellText);
  }
  else
  {
    // Selected cell ?
    if (State.Contains(gdSelected))
    {
      StringGridSectors->Canvas->Brush->Color = clHighlight;
      StringGridSectors->Canvas->FillRect(CellRect);
    }
    else
    {
      tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
                FormMain->NativeCPC->FDC->DriveA :
                FormMain->NativeCPC->FDC->DriveB;
      tTrack* CurrentTrack = &DriveP->track[ACol][mCurrentSide];

      if (ARow <= (int)CurrentTrack->sectors)
      {
        tSector* SectorP = &CurrentTrack->sector[ARow-1];

        if (SectorP->data_gap3_length)
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_GAPS;
        }
        else if (SectorP->weak_count > 1)
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_WEAK;
        }
        else if (SectorP->data_size < SectorP->declared_size)
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_INCOMPLETE;
        }
        else if ((SectorP->declared_size == 128) && (SectorP->empty == cFalse))
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_SIZE_0;
        }
        else if ((SectorP->declared_size == 256) && (SectorP->empty == cFalse))
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_SIZE_1;
        }
        else if ((SectorP->declared_size == 1024) && (SectorP->empty == cFalse))
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_SIZE_3;
        }
        else if ((SectorP->declared_size == 2048) && (SectorP->empty == cFalse))
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_SIZE_4;
        }
        else if ((SectorP->declared_size == 4096) && (SectorP->empty == cFalse))
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_SIZE_5;
        }
        else if ( (SectorP->flags[0] & ST1_DE) && (SectorP->flags[1] & ST2_DD) )
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_ERROR;
        }
        else if (SectorP->empty == cTrue)
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_EMPTY;
        }
        else
        {
          StringGridSectors->Canvas->Brush->Color = COLOR_COMMON;
        }

        StringGridSectors->Canvas->FillRect(CellRect);

        // Draw border
        StringGridSectors->Canvas->Brush->Color = mCellFrameColor;
        StringGridSectors->Canvas->FrameRect(CellRect);
      }
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugDSKViewer::StringGridSectorsSelectCell(
      TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
  if ( (StringGridSectors->Focused() == true) && (mMonitoringInProgress == false) )
  {
    tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
              FormMain->NativeCPC->FDC->DriveA :
              FormMain->NativeCPC->FDC->DriveB;
    tTrack* TrackP = &DriveP->track[ACol][mCurrentSide];

    // Show track info
    ShowTrackInfo(ACol);

    // Header selected ?
    if (ARow == 0)
    {
      CanSelect = false;
      return;
    }

    // Show Sector info
    if (ARow <= (int)TrackP->sectors)
    {
      PageControlData->Visible = true;
      ShowSectorInfo(ACol, ARow - 1);
      ShowData(ACol, ARow - 1);
    }
    else
    {
      PanelSectorState->Visible = false;
      PageControlData->Visible = false;
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ShowTrackInfo(int TrackNumber)
{
  tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
            FormMain->NativeCPC->FDC->DriveA :
            FormMain->NativeCPC->FDC->DriveB;
  tTrack* TrackP = &DriveP->track[TrackNumber][mCurrentSide];

  GroupBoxTrack->Visible = true;

  MemoTrackInfo->Lines->BeginUpdate();
  MemoTrackInfo->Clear();

  AnsiString Info = "Cylinder=" + AnsiString(TrackNumber) + " (&" + AnsiString::IntToHex(TrackNumber, 2) + ")";
  MemoTrackInfo->Lines->Add(Info);
  Info = "Sectors=" + AnsiString(TrackP->sectors);
  MemoTrackInfo->Lines->Add(Info);
  Info = "DSK Size=" + AnsiString(TrackP->size) + " (&" + AnsiString::IntToHex(TrackP->size, 4) + ")";
  MemoTrackInfo->Lines->Add(Info);
  Info = "Real Size=" + AnsiString(TrackP->real_size) + " (&" + AnsiString::IntToHex(TrackP->real_size, 4) + ")";
  MemoTrackInfo->Lines->Add(Info);
  Info = "GAP3 Length=" + AnsiString(TrackP->gap3_length) + " (&" + AnsiString::IntToHex(TrackP->gap3_length, 2) + ")";
  MemoTrackInfo->Lines->Add(Info);
  Info = "Offset-Info=" + AnsiString(TrackP->offset_info_size) + " (&" + AnsiString::IntToHex(TrackP->offset_info_size, 4) + ")";
  MemoTrackInfo->Lines->Add(Info);
  Info = "Filler=" + AnsiString(TrackP->filler) + " (&" + AnsiString::IntToHex(TrackP->filler, 2) + ")";
  MemoTrackInfo->Lines->Add(Info);

  MemoTrackInfo->Lines->EndUpdate();
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ShowSectorInfo(int TrackNumber, int SectorNumber)
{
  if (StringGridSectors->Visible == true)
  {
    tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
              FormMain->NativeCPC->FDC->DriveA :
              FormMain->NativeCPC->FDC->DriveB;
    tTrack* TrackP = &DriveP->track[TrackNumber][mCurrentSide];
    tSector* SectorP = &TrackP->sector[SectorNumber];

    PageControlData->Visible = true;

    MemoSectorInfo->Lines->BeginUpdate();
    MemoSectorInfo->Clear();

    AnsiString Info = "C=&" + AnsiString::IntToHex(SectorP->CHRN[SECTOR_CHRN_C], 2);
    Info += "  H=&" + AnsiString::IntToHex(SectorP->CHRN[SECTOR_CHRN_H], 2);
    Info += "  R=&" + AnsiString::IntToHex(SectorP->CHRN[SECTOR_CHRN_R], 2);
    Info += "  N=&" + AnsiString::IntToHex(SectorP->CHRN[SECTOR_CHRN_N], 2);
    MemoSectorInfo->Lines->Add(Info);
    Info = "Declared Size=" + AnsiString(SectorP->declared_size) + " (&" + AnsiString::IntToHex(SectorP->declared_size, 4) + ")";
    MemoSectorInfo->Lines->Add(Info);
    MemoSectorInfo->Lines->Add("");
    
    Info = "Flag ST1=&" + AnsiString::IntToHex(SectorP->flags[0], 2);
    if (SectorP->flags[0])
    {
      Info += " (";
      if (SectorP->flags[0] & 0x20) Info += " DE";
      if (SectorP->flags[0] & 0x04) Info += " ND";
      if (SectorP->flags[0] & 0x01) Info += " MA";
      Info += " )";
    }
    MemoSectorInfo->Lines->Add(Info);
    Info = "Flag ST2=&" + AnsiString::IntToHex(SectorP->flags[1], 2);
    if (SectorP->flags[1])
    {
      Info += " (";
      if (SectorP->flags[1] & 0x40) Info += " CM";
      if (SectorP->flags[1] & 0x20) Info += " DD";
      if (SectorP->flags[1] & 0x01) Info += " MD";
      Info += " )";
    }
    MemoSectorInfo->Lines->Add(Info);
    Info = "DSK Size=" + AnsiString(SectorP->size) + " (&" + AnsiString::IntToHex(SectorP->size, 4) + ")";
    MemoSectorInfo->Lines->Add(Info);
    Info = "Data Size=" + AnsiString(SectorP->data_size) + " (&" + AnsiString::IntToHex(SectorP->data_size, 4) + ")";
    MemoSectorInfo->Lines->Add(Info);
    Info = "Weak Count=" + AnsiString(SectorP->weak_count);
    MemoSectorInfo->Lines->Add(Info);
    Info = "GAP3 Length=" + AnsiString(SectorP->info_gap3_length) + " (&" + AnsiString::IntToHex(SectorP->info_gap3_length, 4) + ")";
    MemoSectorInfo->Lines->Add(Info);
    Info = "Data GAP3 Length=" + AnsiString(SectorP->data_gap3_length) + " (&" + AnsiString::IntToHex(SectorP->data_gap3_length, 4) + ")";
    MemoSectorInfo->Lines->Add(Info);
    Info = "Offset-Info=" + AnsiString(SectorP->offset_info) + " (&" + AnsiString::IntToHex(SectorP->offset_info, 4) + ")";
    MemoSectorInfo->Lines->Add(Info);
    tUShort CRC = (SectorP->CHRN[SECTOR_CHRN_CRC_MSB] << 8) + SectorP->CHRN[SECTOR_CHRN_CRC_LSB];
    Info = "CHRN CRC=&" + AnsiString::IntToHex(CRC, 4);
    MemoSectorInfo->Lines->Add(Info);
    CRC = (SectorP->data_crc[SECTOR_CRC_MSB] << 8) + SectorP->data_crc[SECTOR_CRC_LSB];
    Info = "Data CRC=&" + AnsiString::IntToHex(CRC, 4);
    MemoSectorInfo->Lines->Add(Info);

    MemoSectorInfo->Lines->EndUpdate();

    //
    // Show sector state
    //
    PanelSectorState->Visible = true;
    if (SectorP->data_gap3_length)
    {
      PanelSectorState->Color = COLOR_GAPS;
      PanelSectorState->Caption = GAPSSectorLabel;
    }
    else if (SectorP->weak_count > 1)
    {
      PanelSectorState->Color = COLOR_WEAK;
      PanelSectorState->Caption = WEAKSectorLabel;
    }
    else if (SectorP->data_size < SectorP->declared_size)
    {
      PanelSectorState->Color = COLOR_INCOMPLETE;
      PanelSectorState->Caption = INCOMPLETESectorLabel;
    }
    else if ( (SectorP->declared_size == 128)
              && (!SectorP->empty) )
    {
      PanelSectorState->Color = COLOR_SIZE_0;
      PanelSectorState->Caption = SIZE0SectorLabel;
    }
    else if ( (SectorP->declared_size == 256)
              && (!SectorP->empty) )
    {
      PanelSectorState->Color = COLOR_SIZE_1;
      PanelSectorState->Caption = SIZE1SectorLabel;
    }
    else if ( (SectorP->declared_size == 1024)
              && (!SectorP->empty) )
    {
      PanelSectorState->Color = COLOR_SIZE_3;
      PanelSectorState->Caption = SIZE3SectorLabel;
    }
    else if ( (SectorP->declared_size == 2048)
              && (!SectorP->empty) )
    {
      PanelSectorState->Color = COLOR_SIZE_4;
      PanelSectorState->Caption = SIZE4SectorLabel;
    }
    else if ( (SectorP->declared_size == 4096)
              && (!SectorP->empty) )
    {
      PanelSectorState->Color = COLOR_SIZE_5;
      PanelSectorState->Caption = SIZE5SectorLabel;
    }
    else if ( (SectorP->flags[0] & ST1_DE) && (SectorP->flags[1] & ST2_DD) )
    {
      PanelSectorState->Color = COLOR_ERROR;
      PanelSectorState->Caption = ERRORSectorLabel;
    }
    else if (SectorP->empty == cTrue)
    {
      PanelSectorState->Color = COLOR_EMPTY;
      PanelSectorState->Caption = EMPTYSectorLabel;
    }
    else
    {
      PanelSectorState->Color = COLOR_COMMON;
      PanelSectorState->Caption = COMMONSectorLabel;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ShowData(int TrackNumber, int SectorNumber)
{
  if (mMonitoringInProgress == false)
  {
    tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
              FormMain->NativeCPC->FDC->DriveA :
              FormMain->NativeCPC->FDC->DriveB;
    tTrack* TrackP = &DriveP->track[TrackNumber][mCurrentSide];
    tSector* SectorP = &TrackP->sector[SectorNumber];

    tUChar* DataP;
    
    TabSheetDataSector->TabVisible = true;
    TabSheetDataGAP3->TabVisible = false;
    TabSheetDataDeclared->TabVisible = false;

    MemoDataSector->Lines->BeginUpdate();
    MemoDataSector->Clear();

    int TrackOffset = SectorP->track_offset;
    AnsiString TrackOffsetMessage = "Track Offset=" + AnsiString(TrackOffset) + " (&" + AnsiString::IntToHex(TrackOffset, 4) + ")";
    MemoDataSector->Lines->Add(TrackOffsetMessage);
    AnsiString DataSizeMessage = "Data Size=" + AnsiString(SectorP->data_size) + " (&" + AnsiString::IntToHex(SectorP->data_size, 4) + ")";
    MemoDataSector->Lines->Add(DataSizeMessage);
    MemoDataSector->Lines->Add("");

    //
    // Show Sector Data
    //
    // weak sectors ?
    if (SectorP->weak_count > 1)
    {
      for (int WeakLoop=0; WeakLoop < (int)SectorP->weak_count; WeakLoop++)
      {
        if (WeakLoop) MemoDataSector->Lines->Add("");
        AnsiString Header = "Weak sector " + AnsiString(WeakLoop+1) + " (+&" + AnsiString::IntToHex(SectorP->declared_size * WeakLoop, 4) + ")";
        MemoDataSector->Lines->Add(Header);

        DataP = SectorP->data;
        DataP += SectorP->declared_size * WeakLoop;
        int DataSize = SectorP->declared_size;

        // Show sector data
        int DataAddress = 0;
        do
        {
#ifdef SHOW_ASCII
          AnsiString DataASCII = "";
          int RemainingBytes = DataSize < 16 ? 16 - DataSize : 0;
#endif /* SHOW_ASCII */
          AnsiString DataContent = "&" + AnsiString::IntToHex(DataAddress, 4) + "  ";
          for (int Loop=0;
               (Loop < 16) && (DataSize);
               Loop++, DataSize--)
          {
            DataContent += AnsiString::IntToHex(*DataP, 2) + " ";
#ifdef SHOW_ASCII
            if (mDataASCII7Bits == true)
            {
              DataASCII += ASCIIChar[(*DataP & 0x7f)];
            }
            else
            {
              DataASCII += ASCIIChar[*DataP];
            }
#endif /* SHOW_ASCII */
            DataP++;
          }
#ifdef SHOW_ASCII
          for (int Loop=0; Loop < RemainingBytes; Loop++)
          {
            DataContent += "   "; // Blank byte
          }
          DataContent += "  "; // Separator
          DataContent += DataASCII;
#endif /* SHOW_ASCII */
          DataAddress += 16;
          MemoDataSector->Lines->Add(DataContent);
        }
        while (DataSize);
      }
    }
    else
    {
      int DataSize;
      int ExtraDataSize = 0;

      if (SectorP->data_gap3_length)
      {
        DataSize = SectorP->data_size;
      }
      else if (SectorP->data_size < SectorP->size)
      {
        DataSize = SectorP->data_size;
        ExtraDataSize = SectorP->size - SectorP->data_size;
      }
      else
      {
        DataSize = SectorP->size;
      }

      // Show sector data
      DataP = SectorP->data;
      int DataAddress = 0;
      do
      {
#ifdef SHOW_ASCII
        AnsiString DataASCII = "";
        int RemainingBytes = DataSize < 16 ? 16 - DataSize : 0;
#endif /* SHOW_ASCII */
        AnsiString DataContent = "&" + AnsiString::IntToHex(DataAddress, 4) + "  ";
        for (int Loop=0;
             (Loop < 16) && (DataSize);
             Loop++, DataSize--)
        {
          DataContent += AnsiString::IntToHex(*DataP, 2) + " ";
#ifdef SHOW_ASCII
          if (mDataASCII7Bits == true)
          {
            DataASCII += ASCIIChar[(*DataP & 0x7f)];
          }
          else
          {
            DataASCII += ASCIIChar[*DataP];
          }
#endif /* SHOW_ASCII */
          DataP++;
        }
#ifdef SHOW_ASCII
        for (int Loop=0; Loop < RemainingBytes; Loop++)
        {
          DataContent += "   "; // Blank byte
        }
        DataContent += "  "; // Separator
        DataContent += DataASCII;
#endif /* SHOW_ASCII */
        DataAddress += 16;
        MemoDataSector->Lines->Add(DataContent);
      }
      while (DataSize);

      //
      // Show extra sector data
      //
      if (ExtraDataSize)
      {
        MemoDataSector->Lines->Add("");
        AnsiString Header = "Extra data";
        MemoDataSector->Lines->Add(Header);

        DataSize = ExtraDataSize;
        do
        {
#ifdef SHOW_ASCII
          AnsiString DataASCII = "";
          int RemainingBytes = DataSize < 16 ? 16 - DataSize : 0;
#endif /* SHOW_ASCII */
          AnsiString DataContent = "&" + AnsiString::IntToHex(DataAddress, 4) + "  ";
          for (int Loop=0;
               (Loop < 16) && (DataSize);
               Loop++, DataSize--)
          {
            DataContent += AnsiString::IntToHex(*DataP, 2) + " ";
#ifdef SHOW_ASCII
            if (mDataASCII7Bits == true)
            {
              DataASCII += ASCIIChar[(*DataP & 0x7f)];
            }
            else
            {
              DataASCII += ASCIIChar[*DataP];
            }
#endif /* SHOW_ASCII */
            DataP++;
          }
#ifdef SHOW_ASCII
          for (int Loop=0; Loop < RemainingBytes; Loop++)
          {
            DataContent += "   "; // Blank byte
          }
          DataContent += "  "; // Separator
          DataContent += DataASCII;
#endif /* SHOW_ASCII */
          DataAddress += 16;
          MemoDataSector->Lines->Add(DataContent);
        }
        while (DataSize);
      }
    }

    MemoDataSector->Lines->EndUpdate();

    //
    // Show GAP3
    //
    if (SectorP->data_gap3_length)
    {
      // Add GAP3 Tab
      TabSheetDataGAP3->TabVisible = true;

      MemoDataGAP3->Lines->BeginUpdate();
      MemoDataGAP3->Clear();

      TrackOffset = SectorP->track_offset + (SectorP->data_size * SectorP->weak_count);
      TrackOffsetMessage = "Track Offset=" + AnsiString(TrackOffset) + " (&" + AnsiString::IntToHex(TrackOffset, 4) + ")";
      MemoDataGAP3->Lines->Add(TrackOffsetMessage);
      DataSizeMessage = "GAP3 Size=" + AnsiString(SectorP->data_gap3_length) + " (&" + AnsiString::IntToHex(SectorP->data_gap3_length, 4) + ")";
      MemoDataGAP3->Lines->Add(DataSizeMessage);
      MemoDataGAP3->Lines->Add("");

      DataP = SectorP->data;
      DataP += SectorP->data_size * SectorP->weak_count;
      int DataSize = SectorP->data_gap3_length;
      int DataAddress = 0;
      do
      {
#ifdef SHOW_ASCII
        AnsiString DataASCII = "";
        int RemainingBytes = DataSize < 16 ? 16 - DataSize : 0;
#endif /* SHOW_ASCII */
        AnsiString DataContent = "&" + AnsiString::IntToHex(DataAddress, 4) + "  ";
        for (int Loop=0;
             (Loop < 16) && (DataSize);
             Loop++, DataSize--)
        {
          DataContent += AnsiString::IntToHex(*DataP, 2) + " ";
#ifdef SHOW_ASCII
          if (mDataASCII7Bits == true)
          {
            DataASCII += ASCIIChar[(*DataP & 0x7f)];
          }
          else
          {
            DataASCII += ASCIIChar[*DataP];
          }
#endif /* SHOW_ASCII */
          DataP++;
        }
#ifdef SHOW_ASCII
        for (int Loop=0; Loop < RemainingBytes; Loop++)
        {
          DataContent += "   "; // Blank byte
        }
        DataContent += "  "; // Separator
        DataContent += DataASCII;
#endif /* SHOW_ASCII */
        DataAddress += 16;
        MemoDataGAP3->Lines->Add(DataContent);
      }
      while (DataSize);

      MemoDataGAP3->Lines->EndUpdate();
    }

#ifdef _DEBUG
    //
    // Show Declared data
    //
    if (SectorP->declared_data)
    {
      // Add Declared Tab
      TabSheetDataDeclared->TabVisible = true;

      MemoDataDeclared->Clear();
      MemoDataDeclared->Lines->Add("Preparing data...");
      mDeclaredDataPrepared = false;
    }
#endif /* _DEBUG */
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ShowDeclaredData(int TrackNumber, int SectorNumber)
{
#ifdef _DEBUG

  tDrive* DriveP = (ComboBoxDrive->Text == cDriveALabel) ?
              FormMain->NativeCPC->FDC->DriveA :
              FormMain->NativeCPC->FDC->DriveB;
  tTrack* TrackP = &DriveP->track[TrackNumber][mCurrentSide];
  tSector* SectorP = &TrackP->sector[SectorNumber];

  // Show declared data
  MemoDataDeclared->Lines->BeginUpdate();
  MemoDataDeclared->Clear();

  int TrackOffset = SectorP->track_offset;
  AnsiString TrackMessage = "Track Offset=" + AnsiString(TrackOffset) + " (&" + AnsiString::IntToHex(TrackOffset, 4) + ")";
  MemoDataDeclared->Lines->Add(TrackMessage);
  TrackMessage = "Declared Size=" + AnsiString(SectorP->declared_size) + " (&" + AnsiString::IntToHex(SectorP->declared_size, 4) + ")";
  MemoDataDeclared->Lines->Add(TrackMessage);
  MemoDataDeclared->Lines->Add("");

  tUChar* DataP = SectorP->declared_data;
  int DataSize = SectorP->declared_size;
  int DataAddress = 0;
  do
  {
#ifdef SHOW_ASCII
    AnsiString DataASCII = "";
    int RemainingBytes = DataSize < 16 ? 16 - DataSize : 0;
#endif /* SHOW_ASCII */
    AnsiString DataContent = "&" + AnsiString::IntToHex(DataAddress, 4) + "  ";
    for (int Loop=0;
         (Loop < 16) && (DataSize);
         Loop++, DataSize--)
    {
      DataContent += AnsiString::IntToHex(*DataP, 2) + " ";
#ifdef SHOW_ASCII
      if (mDataASCII7Bits == true)
      {
        DataASCII += ASCIIChar[(*DataP & 0x7f)];
      }
      else
      {
        DataASCII += ASCIIChar[*DataP];
      }
#endif /* SHOW_ASCII */
      DataP++;
    }

#ifdef SHOW_ASCII
    for (int Loop=0; Loop < RemainingBytes; Loop++)
    {
      DataContent += "   "; // Blank byte
    }
    DataContent += "  "; // Separator
    DataContent += DataASCII;
#endif /* SHOW_ASCII */

    DataAddress += 16;
    MemoDataDeclared->Lines->Add(DataContent);
  }
  while (DataSize);

  MemoDataDeclared->Lines->EndUpdate();

#else /* _DEBUG */
  NOT_USED(TrackNumber);
  NOT_USED(SectorNumber);
#endif /* _DEBUG */
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ButtonMonitoringClick(TObject *Sender)
{
  if (ButtonMonitoring->Caption == cStartMonitoringLabel)
  {
    ButtonMonitoring->Caption = cStopMonitoringLabel;
    mMonitoringInProgress = true;
    GroupBoxTrack->Enabled = false;
    TabSheetDataSector->TabVisible = false;
    TabSheetDataGAP3->TabVisible = false;
    TabSheetDataDeclared->TabVisible = false;
    StringGridSectors->Enabled = false;
    ComboBoxDrive->Enabled = false;
    ComboBoxSides->Enabled = false;
  }
  else
  {
    ButtonMonitoring->Caption = cStartMonitoringLabel;
    mMonitoringInProgress = false;
    GroupBoxTrack->Enabled = true;
    StringGridSectors->Enabled = true;
    ComboBoxDrive->Enabled = true;
    ComboBoxSides->Enabled = true;

    ShowData(StringGridSectors->Col, StringGridSectors->Row - 1);
  }

  Caption = Title;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::CopyToClipboardMenuClick(TObject *Sender)
{
  TMemo* Memo = (TMemo*)PopupMenuClipboard->PopupComponent;

  TStringList* pBuffer = new TStringList();
  for (int Index=0; Index < Memo->Lines->Count; Index++)
  {
    pBuffer->Add(Memo->Lines->Strings[Index]);
  }

  Clipboard()->AsText = pBuffer->Text;

  delete pBuffer;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::ASCII7bitsMenuClick(TObject *Sender)
{
  mDataASCII7Bits = mDataASCII7Bits == true ? false : true;
  ASCII7bitsMenu->Checked = mDataASCII7Bits;

  ShowData(StringGridSectors->Col, StringGridSectors->Row - 1);

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugDSKViewer::PageControlDataChange(TObject *Sender)
{
#ifdef _DEBUG

  if (PageControlData->ActivePage == TabSheetDataDeclared)
  {
    if (mDeclaredDataPrepared == false)
    {
      ShowDeclaredData(StringGridSectors->Col,
                       StringGridSectors->Row - 1);

      mDeclaredDataPrepared = true;
    }
  }
  
#endif /* _DEBUG */

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugDSKViewer::PageControlDataDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = PageControlData->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }

  // Get sheet corresponding to TabIndex
  int TabCount=0;
  TTabSheet* Sheet = NULL;
  for (int Loop=0; Loop < PageControlData->PageCount; Loop++)
  {
    if (PageControlData->Pages[Loop]->TabVisible == true)
    {
      if (TabCount == TabIndex)
      {
        Sheet = PageControlData->Pages[Loop];
        break;
      }

      TabCount++;
    }
  }

  // Draw tab
  Canvas->FillRect(Rect);
  AnsiString Text = Sheet->Caption;
  int TextWidth = Canvas->TextWidth(Text);
  int TextHeight = Canvas->TextHeight(Text);
  int TextPosX = (Rect.Width() - TextWidth) / 2;
  int TextPosY = (PageControlData->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

  NOT_USED(Control);
}
//---------------------------------------------------------------------------

