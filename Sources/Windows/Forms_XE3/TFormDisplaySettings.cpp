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
#pragma hdrstop

#include "TFormDisplaySettings.h"
#include "TFormDebugBASICColors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDisplaySettings *FormDisplaySettings;


// GateArray color from BASIC index
static const int CPCColors[] =
{
/*  0 */ 0x000000,
/*  1 */ 0x800000,
/*  2 */ 0xFF0000,
/*  3 */ 0x000080,
/*  4 */ 0x800080,
/*  5 */ 0xFF0080,
/*  6 */ 0x0000FF,
/*  7 */ 0x8000FF,
/*  8 */ 0xFF00FF,
/*  9 */ 0x008000,
/* 10 */ 0x808000,
/* 11 */ 0xFF8000,
/* 12 */ 0x008080,
/* 13 */ 0x808080,
/* 14 */ 0xFF8080,
/* 15 */ 0x0080FF,
/* 16 */ 0x8080FF,
/* 17 */ 0xFF80FF,
/* 18 */ 0x00FF00,
/* 19 */ 0x80FF00,
/* 20 */ 0xFFFF00,
/* 21 */ 0x00FF80,
/* 22 */ 0x80FF80,
/* 23 */ 0xFFFF80,
/* 24 */ 0x00FFFF,
/* 25 */ 0x80FFFF,
/* 26 */ 0xFFFFFF
};

// Theme colors indexes
typedef struct
{
  char* Title;
  tUChar BackgroundColorIndex;
  tUChar Text1ColorIndex;
  tUChar Text2ColorIndex;
} tThemeColors;

const tThemeColors ThemeColors[] =
{
  { "Reset", 1, 24, 20 },
  { "Harrier Attack", 0, 11, 9 },
  { "Zombi", 0, 13, 6 },
  { "Bob Winner", 25, 4, 15 },
  { "ATF", 13, 21, 20 },
  { "Discology", 26, 0, 0 },
  { "Monochrome", 0, 18, 9 },
  // Last one
  { "", 0, 0, 0 }
};



//---------------------------------------------------------------------------
__fastcall TFormDisplaySettings::TFormDisplaySettings(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mDrawingContextP = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormDisplaySettings::FormShow(TObject *Sender)
{
  if ( (mSettingsP == NULL) || (mDrawingContextP == NULL) )
  {
    Close();
    return;
  }

  CheckBoxKeepFullscreen->Checked = mSettingsP->KeepFullscreen;
  CheckBoxFullscreenWithMenu->Checked = mSettingsP->FullscreenMenu;
  CheckBoxDrawScanlines->Checked = mSettingsP->DrawScanlines;
  CheckBoxKeepAspectRatio->Checked = mSettingsP->KeepAspectRatio;
  CheckBoxShowInfoDuringPause->Checked = mSettingsP->ShowPauseInfo;

  CheckBoxOSDIcons->Checked = (mSettingsP->OnscreenDisplay & OSD_ICONS) ? true : false;
  CheckBoxOSDMessages->Checked = (mSettingsP->OnscreenDisplay & OSD_MESSAGES) ? true : false;
  CheckBoxOSDDisk->Checked = (mSettingsP->OnscreenDisplay & OSD_DISK) ? true : false;
  CheckBoxOSDTape->Checked = (mSettingsP->OnscreenDisplay & OSD_TAPE) ? true : false;

  TrackBarVHOLDAdjust->Position = mSettingsP->VHoldAdjust;

  // Add themes
  ComboBoxTheme->Items->Clear();
  ComboBoxTheme->Items->Add("Custom");
  const tThemeColors* Theme = ThemeColors;
  while (AnsiString(Theme->Title).IsEmpty() == false)
  {
    ComboBoxTheme->Items->Add(Theme->Title);
    Theme++;
  }
  ComboBoxTheme->ItemIndex = mSettingsP->ThemeIndex;
  ComboBoxThemeChange(this);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDisplaySettings::ApplyTheme(void)
{
  // Apply theme
  Color = PanelThemeBackground->Color;
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  Font->Color = PanelThemeText1->Color;
  LabelThemeText2->Font->Color = PanelThemeText2->Color;
  CheckBoxOSDIcons->Font->Color = PanelThemeText2->Color;
  CheckBoxOSDMessages->Font->Color = PanelThemeText2->Color;
  CheckBoxOSDDisk->Font->Color = PanelThemeText2->Color;
  CheckBoxOSDTape->Font->Color = PanelThemeText2->Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormDisplaySettings::ButtonOKClick(TObject *Sender)
{
  mSettingsP->KeepFullscreen = CheckBoxKeepFullscreen->Checked;
  mSettingsP->FullscreenMenu = CheckBoxFullscreenWithMenu->Checked;
  mSettingsP->DrawScanlines = CheckBoxDrawScanlines->Checked;
  mSettingsP->KeepAspectRatio = CheckBoxKeepAspectRatio->Checked;
  mSettingsP->ShowPauseInfo = CheckBoxShowInfoDuringPause->Checked;

  mSettingsP->OnscreenDisplay &= ~(OSD_MESSAGES | OSD_ICONS | OSD_TAPE | OSD_DISK);
  if (CheckBoxOSDIcons->Checked == true)
  {
    mSettingsP->OnscreenDisplay |= OSD_ICONS;
  }
  if (CheckBoxOSDMessages->Checked == true)
  {
    mSettingsP->OnscreenDisplay |= OSD_MESSAGES;
  }
  if (CheckBoxOSDDisk->Checked == true)
  {
    mSettingsP->OnscreenDisplay |= OSD_DISK;
  }
  if (CheckBoxOSDTape->Checked == true)
  {
    mSettingsP->OnscreenDisplay |= OSD_TAPE;
  }

  mSettingsP->VHoldAdjust = TrackBarVHOLDAdjust->Position;

  // Theme
  mSettingsP->ThemeIndex = ComboBoxTheme->ItemIndex;
  mSettingsP->ThemeBackgroundColor = PanelThemeBackground->Color;
  mSettingsP->ThemeText1Color = PanelThemeText1->Color;
  mSettingsP->ThemeText2Color = PanelThemeText2->Color;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDisplaySettings::ButtonCancelClick(TObject *Sender)
{
  // Restore settings
  mDrawingContextP->DrawScanlines = mSettingsP->DrawScanlines;

  // Restore V-Hold
  tVDU* VDU = mNativeCPCP->VDU;
  tCRTC* CRTC = mNativeCPCP->CRTC;
  VDU->vhold_min = mSettingsP->VHoldAdjust;
  CRTC->MaxVSync = CRTC->MinVSync + VDU->vhold_range_min +
   (int)ceil((float)((CRTC->MinVSync - VDU->vhold_min) * (VDU->vhold_range_max - VDU->vhold_range_min) / (VDU->vhold_max - VDU->vhold_min)));

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDisplaySettings::CheckBoxDrawScanlinesClick(
      TObject *Sender)
{
  mDrawingContextP->DrawScanlines = CheckBoxDrawScanlines->Checked;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDisplaySettings::ComboBoxThemeChange(TObject *Sender)
{
  // Custom ?
  if (ComboBoxTheme->ItemIndex == 0)
  {
    PanelThemeBackground->Color = mSettingsP->ThemeBackgroundColor;
    PanelThemeText1->Color = mSettingsP->ThemeText1Color;
    PanelThemeText2->Color = mSettingsP->ThemeText2Color;
  }
  else
  {
    const tThemeColors* Theme = &ThemeColors[ComboBoxTheme->ItemIndex-1];

    PanelThemeBackground->Color = TColor(CPCColors[Theme->BackgroundColorIndex]);
    PanelThemeText1->Color = TColor(CPCColors[Theme->Text1ColorIndex]);
    PanelThemeText2->Color = TColor(CPCColors[Theme->Text2ColorIndex]);
  }

  ApplyTheme();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDisplaySettings::ComboBoxThemeDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxTheme->Canvas;

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
  AnsiString Text = ComboBoxTheme->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxTheme->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------

void __fastcall TFormDisplaySettings::PanelThemeBackgroundClick(TObject *Sender)
{
  TFormDebugBASICColors* EditColor = new TFormDebugBASICColors(this);
  EditColor->Settings = mSettingsP;
  for (int Loop=0; Loop < 27; Loop++)
  {
    if (PanelThemeBackground->Color == CPCColors[Loop])
    {
      EditColor->SelectedColor = Loop;
      break;
    }
  }
  if (EditColor->ShowModal() == mrOk)
  {
    int NewColor = CPCColors[EditColor->SelectedColor];
    if (NewColor != PanelThemeBackground->Color)
    {
      PanelThemeBackground->Color = (TColor)NewColor;
      // Set Custom theme
      ComboBoxTheme->ItemIndex = 0;
      ApplyTheme();
    }
  }
  delete EditColor;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDisplaySettings::PanelThemeText1Click(TObject *Sender)
{
  TFormDebugBASICColors* EditColor = new TFormDebugBASICColors(this);
  EditColor->Settings = mSettingsP;
  for (int Loop=0; Loop < 26; Loop++)
  {
    if (PanelThemeText1->Color == CPCColors[Loop])
    {
      EditColor->SelectedColor = Loop;
      break;
    }
  }
  if (EditColor->ShowModal() == mrOk)
  {
    int NewColor = CPCColors[EditColor->SelectedColor];
    if (NewColor != PanelThemeText1->Color)
    {
      PanelThemeText1->Color = (TColor)NewColor;
      // Set Custom theme
      ComboBoxTheme->ItemIndex = 0;
      ApplyTheme();
    }
  }
  delete EditColor;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDisplaySettings::PanelThemeText2Click(TObject *Sender)
{
  TFormDebugBASICColors* EditColor = new TFormDebugBASICColors(this);
  EditColor->Settings = mSettingsP;
  for (int Loop=0; Loop < 26; Loop++)
  {
    if (PanelThemeText2->Color == CPCColors[Loop])
    {
      EditColor->SelectedColor = Loop;
      break;
    }
  }
  if (EditColor->ShowModal() == mrOk)
  {
    int NewColor = CPCColors[EditColor->SelectedColor];
    if (NewColor != PanelThemeText2->Color)
    {
      PanelThemeText2->Color = (TColor)NewColor;
      // Set Custom theme
      ComboBoxTheme->ItemIndex = 0;
      ApplyTheme();
    }
  }
  delete EditColor;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDisplaySettings::TrackBarVHOLDAdjustChange(TObject *Sender)
{
tVDU* VDU = mNativeCPCP->VDU;
tCRTC* CRTC = mNativeCPCP->CRTC;

  VDU->vhold_min = TrackBarVHOLDAdjust->Position;

  CRTC->MaxVSync = CRTC->MinVSync + VDU->vhold_range_min +
   (int)ceil((float)((CRTC->MinVSync - VDU->vhold_min) * (VDU->vhold_range_max - VDU->vhold_range_min) / (VDU->vhold_max - VDU->vhold_min)));

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

