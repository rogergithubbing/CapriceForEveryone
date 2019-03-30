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
#include <FileCtrl.hpp>
#include <tchar.h>
#pragma hdrstop
#include "TFormEmulatorSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


TFormEmulatorSettings *FormEmulatorSettings;
//---------------------------------------------------------------------------
__fastcall TFormEmulatorSettings::TFormEmulatorSettings(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mAvailableROMList = NULL;

#ifndef USE_SILICON_DISC
  CheckBoxUse256kSiliconDisc->Visible = false;
#endif /* USE_SILICON_DISC */

#ifndef ENABLE_TMPI_SPEECH
  CheckBoxUseTechniMusique->Visible = false;
#endif /* ENABLE_TMPI_SPEECH */

#ifndef ENABLE_PLAYCITY
  CheckBoxUsePlayCity->Visible = false;
#endif /* ENABLE_PLAYCITY */
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::FormShow(TObject *Sender)
{
	if (mSettingsP == NULL)
  {
    ModalResult = mrCancel;
    return;
  }

  ShowHint = mSettingsP->ShowHints;

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  StringGridROMMapping->FixedColor = Color;
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  Font->Color = mSettingsP->ThemeText1Color;
  ComboBoxDevices->Font->Color = clBlack;
  ComboBoxBrands->Font->Color = clBlack;
  ComboBoxCRTCType->Font->Color = clBlack;
  ComboBoxAvailableROM->Font->Color = clBlack;
  LabelDevice->Font->Color = mSettingsP->ThemeText2Color;
  LabelBrand->Font->Color = mSettingsP->ThemeText2Color;
  LabelCRTCType->Font->Color = mSettingsP->ThemeText2Color;

  ComboBoxDevices->ItemIndex = mSettingsP->CPCDevice;
  ComboBoxBrands->ItemIndex = mSettingsP->CPCBrand;
  ComboBoxCRTCType->ItemIndex = mSettingsP->CRTCType;
  CheckBoxUseFloppyDrive->Checked = mSettingsP->UseFloppyDrive;
  CheckBoxUse64kMemoryExtension->Checked = mSettingsP->Use64kMemoryExtension;
  CheckBoxUse256kMemoryExtension->Checked = mSettingsP->Use256kMemoryExtension;
  CheckBoxUse256kSiliconDisc->Checked = mSettingsP->Use256kSiliconDisc;
  CheckBoxUseTechniMusique->Checked = mSettingsP->UseTechniMusique;
  CheckBoxUsePlayCity->Checked = mSettingsP->UsePlayCity;

  // Fast FDC
  if (CheckBoxUseFloppyDrive->Checked == true)
  {
    CheckBoxUseFastFDC->Enabled = true;
    CheckBoxUseFastFDC->Checked = mSettingsP->UseFastFDC;
  }
  else
  {
    CheckBoxUseFastFDC->Enabled = false;
    CheckBoxUseFastFDC->Checked = false;
  }

  // Brand name limitations
  ComboBoxBrands->Enabled = (mSettingsP->CPCDevice == CPC_MODEL_464_SP) ? false : true;
  if (mSettingsP->CPCDevice == CPC_MODEL_464_SP)
  {
    ComboBoxBrands->ItemIndex = 0;
  }

  // CRTC Type
  if ( (mSettingsP->CPCDevice == CPC_MODEL_664_UK)
       || (mSettingsP->CPCDevice == CPC_MODEL_664_FR) )
  {
    // CRTC0 Only
    ComboBoxCRTCType->ItemIndex = 0;
    ComboBoxCRTCType->Enabled = false;
  }

  //
  // List available ROMs
  //
  mAvailableROMList = new TStringList;
  AnsiString ROMPath = ExpandFileName(mSettingsP->RomsDirectory);
  if (DirectoryExists(ROMPath))
  {
    TSearchRec F;
    if (FindFirst(ROMPath + "*.rom", faAnyFile, F) == 0)
    {
      int findresult;
      do
      {
        mAvailableROMList->Add(F.Name);
        findresult = FindNext(F);
      }
      while (findresult == 0);
    }
  }

  //
  // Roms mapping
  //
  StringGridROMMapping->RowCount = 17;
  StringGridROMMapping->ColWidths[0] = 100;
  StringGridROMMapping->ColWidths[1] = StringGridROMMapping->Width - StringGridROMMapping->ColWidths[0];
  StringGridROMMapping->Cells[0][0] = "LowerROM";
  for (int loop=0; loop < NB_UPPERROMS; loop++)
  {
    AnsiString Header = "UpperROM ";
    if (loop < 10) Header += " ";
    StringGridROMMapping->Cells[0][loop+1] = Header + AnsiString(loop);
  }
  FillROMMapping();
  StringGridROMMapping->Row = 0;
  UpdateAvailableROMList(StringGridROMMapping->Row);

  ActiveControl = StringGridROMMapping;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::FormHide(TObject *Sender)
{
	delete mAvailableROMList;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::ButtonOKClick(TObject *Sender)
{
	//
  // Apply new settings
  //
  mSettingsP->CPCDevice = ComboBoxDevices->ItemIndex;
  mSettingsP->CPCBrand = ComboBoxBrands->ItemIndex;
  mSettingsP->CRTCType = ComboBoxCRTCType->ItemIndex;
  mSettingsP->UseFloppyDrive = CheckBoxUseFloppyDrive->Checked;
  mSettingsP->UseFastFDC = CheckBoxUseFastFDC->Checked;
  mSettingsP->Use64kMemoryExtension = CheckBoxUse64kMemoryExtension->Checked;
  mSettingsP->Use256kMemoryExtension = CheckBoxUse256kMemoryExtension->Checked;
  mSettingsP->Use256kSiliconDisc = CheckBoxUse256kSiliconDisc->Checked;
  mSettingsP->UseTechniMusique = CheckBoxUseTechniMusique->Checked;
  mSettingsP->UsePlayCity = CheckBoxUsePlayCity->Checked;

  //
  // Apply new ROM mapping
  //
  // Lower ROM
  AnsiString NewROM = StringGridROMMapping->Cells[1][0];
  if (NewROM.Pos(DefaultPrefix) == 1)
  {
    mSettingsP->LowerROMFilename = "";
  }
  else
  {
    mSettingsP->LowerROMFilename = NewROM;
  }
  // Upper ROM
  NewROM = StringGridROMMapping->Cells[1][1];
  if (NewROM.Pos(DefaultPrefix) == 1)
  {
    mSettingsP->UpperROMFilename[0] = "";
  }
  else
  {
    mSettingsP->UpperROMFilename[0] = NewROM;
  }
  // AMSDOS
  NewROM = StringGridROMMapping->Cells[1][8];
  if (NewROM == DefaultAMSDOSRomName)
  {
    mSettingsP->UpperROMFilename[7] = "";
  }
  else if (NewROM.IsEmpty() == true)
  {
    mSettingsP->UpperROMFilename[7] = EmptyRomName;
  }
  else
  {
    mSettingsP->UpperROMFilename[7] = NewROM;
  }
  // All others
  for (int loop=1; loop < NB_UPPERROMS; loop++)
  {
    if (loop == 7) continue; // Already treated

    NewROM = StringGridROMMapping->Cells[1][loop+1];
    if (NewROM.IsEmpty() == true)
    {
      mSettingsP->UpperROMFilename[loop] = EmptyRomName;
    }
    else
    {
      mSettingsP->UpperROMFilename[loop] = NewROM;
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::ButtonDefaultROMMappingClick(
      TObject *Sender)
{
	// Restore Default mapping
  for (int loop=0; loop < StringGridROMMapping->RowCount; loop++)
  {
    StringGridROMMapping->Cells[1][loop] = "";
  }
  StringGridROMMapping->Cells[1][0] = DefaultOSRomName;
  StringGridROMMapping->Cells[1][1] = DefaultBASICRomName;

  // AMSDOS
  switch (ComboBoxDevices->ItemIndex)
  {
    case CPC_MODEL_664_UK:
    case CPC_MODEL_664_FR:
    case CPC_MODEL_6128_UK:
    case CPC_MODEL_6128_FR:
    case CPC_MODEL_6128_SP:
    {
      StringGridROMMapping->Cells[1][8] = DefaultAMSDOSRomName;
    }
    break;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------



void __fastcall TFormEmulatorSettings::FormResize(TObject *Sender)
{
	StringGridROMMapping->ColWidths[1] = StringGridROMMapping->Width - StringGridROMMapping->ColWidths[0];

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmulatorSettings::StringGridROMMappingMouseDown(
			TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
	// Get selected row
  int SelectedCol;
  int SelectedRow;
  StringGridROMMapping->MouseToCell(X, Y, SelectedCol, SelectedRow);
  if (SelectedRow != -1)
  {
    StringGridROMMapping->Row = SelectedRow;
    UpdateAvailableROMList(SelectedRow);
  }

  NOT_USED(Sender);
  NOT_USED(Button);
  NOT_USED(Shift);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::FillROMMapping(void)
{
  //
  // Lower ROM
  //
  if (mSettingsP->LowerROMFilename.IsEmpty() == false)
  {
    StringGridROMMapping->Cells[1][0] = mSettingsP->LowerROMFilename;
  }
  else
  {
    StringGridROMMapping->Cells[1][0] = DefaultOSRomName;
  }

  //
  // Upper ROMs
  //
  for (int loop=0; loop < NB_UPPERROMS; loop++)
  {
    if (mSettingsP->UpperROMFilename[loop].IsEmpty() == false)
    {
      // Default ROM to remove !!
      if (mSettingsP->UpperROMFilename[loop] == EmptyRomName)
      {
        StringGridROMMapping->Cells[1][loop+1] = "";
      }
      else
      {
        StringGridROMMapping->Cells[1][loop+1] = mSettingsP->UpperROMFilename[loop];
      }
    }
    else
    {
      // BASIC ROM
      if (loop == 0)
      {
        StringGridROMMapping->Cells[1][loop+1] = DefaultBASICRomName;
      }
      // AMSDOS ROM
      else if (loop == 7)
      {
        StringGridROMMapping->Cells[1][loop+1] = DefaultAMSDOSRomName;
      }
      // Other ROMs
      else
      {
        StringGridROMMapping->Cells[1][loop+1] = "";
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::UpdateAvailableROMList(int Index)
{
  if (Index < 0) return;

  ComboBoxAvailableROM->Clear();

  //
  // Add Default ROMs
  //
  if (Index == 0) // Lower ROM
  {
    ComboBoxAvailableROM->Items->Add(DefaultOSRomName);
  }
  else if (Index == 1) // Upper ROM 0
  {
    ComboBoxAvailableROM->Items->Add(DefaultBASICRomName);
  }
  else
  {
    // Add Empty for all others
    ComboBoxAvailableROM->Items->Add(EmptyRomName);
    ComboBoxAvailableROM->Items->Add(DefaultAMSDOSRomName);
    ComboBoxAvailableROM->Items->Add(DefaultSiliconDiscRomName);
  }

  // Add list of user ROMs
  for (int Loop=0; Loop < mAvailableROMList->Count; Loop++)
  {
    ComboBoxAvailableROM->Items->Add(mAvailableROMList->Strings[Loop]);
  }

  // Upper ROM ?
  if (Index > 0)
  {
    // Add temporary Upper ROM
    AnsiString UpperROM = mSettingsP->UpperROMFilename[Index-1];
    if (UpperROM.IsEmpty() == false)
    {
      // Temporary ROM ?
      if (ComboBoxAvailableROM->Items->IndexOf(UpperROM) == -1)
      {
        ComboBoxAvailableROM->Items->Add(UpperROM);
      }
    }
  }

  //
  // Select current ROM
  //
  AnsiString CurrentROM = StringGridROMMapping->Cells[1][Index];
  if (CurrentROM.IsEmpty() == true)
  {
    CurrentROM = EmptyRomName;
  }
  ComboBoxAvailableROM->ItemIndex = ComboBoxAvailableROM->Items->IndexOf(CurrentROM);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmulatorSettings::ComboBoxAvailableROMChange(
      TObject *Sender)
{
AnsiString NewROM = ComboBoxAvailableROM->Text;

	if (NewROM == EmptyRomName)
	{
		NewROM = "";
	}

	StringGridROMMapping->Cells[1][StringGridROMMapping->Row] = NewROM;
	ActiveControl = StringGridROMMapping;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmulatorSettings::ComboBoxAvailableROMDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxAvailableROM->Canvas;

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
  AnsiString Text = ComboBoxAvailableROM->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxAvailableROM->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::StringGridROMMappingSelectCell(
			TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
	UpdateAvailableROMList(ARow);

  NOT_USED(Sender);
  NOT_USED(ACol);
  NOT_USED(CanSelect);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::StringGridROMMappingKeyDown(
			TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (StringGridROMMapping->Focused() == true)
  {
    // Select first ROM of the available ROM list
    if ( (Key == VK_DELETE) || (Key == VK_BACK) )
    {
      ComboBoxAvailableROM->ItemIndex = 0;
      ComboBoxAvailableROMChange(Sender);
    }
  }

  NOT_USED(Shift);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::StringGridROMMappingDrawCell(TObject *Sender,
          int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
  // Fixed column ?
  if (ACol == 0)
  {
    StringGridROMMapping->Canvas->Brush->Color = Color;
    StringGridROMMapping->Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }
  else
  {
    if (State.Contains(gdSelected))
    {
      StringGridROMMapping->Canvas->Brush->Color = clHighlight;
      StringGridROMMapping->Canvas->Font->Color = clWhite;
    }
    else
    {
      StringGridROMMapping->Canvas->Brush->Color = Color;
      StringGridROMMapping->Canvas->Font->Color = Font->Color;
    }
  }

  // Draw cell
  StringGridROMMapping->Canvas->FillRect(Rect);
  int TextHeight = StringGridROMMapping->Canvas->TextHeight(StringGridROMMapping->Cells[ACol][ARow]);
  int TextPos = (StringGridROMMapping->RowHeights[ARow] - TextHeight) / 2;
  StringGridROMMapping->Canvas->TextOut(Rect.Left+5,
                                        Rect.Top+TextPos,
                                        StringGridROMMapping->Cells[ACol][ARow]);
  if (State.Contains(gdFocused))
  {
    StringGridROMMapping->Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::ComboBoxDevicesChange(TObject *Sender)
{
  // Floppy drive use
  switch (ComboBoxDevices->ItemIndex)
  {
    case CPC_MODEL_464_UK:
    case CPC_MODEL_464_FR:
    case CPC_MODEL_464_SP:
    {
      CheckBoxUseFloppyDrive->Checked = false;
    }
    break;

    default:
    {
      CheckBoxUseFloppyDrive->Checked = true;
    }
  }

  // Default AMSDOS
  switch (ComboBoxDevices->ItemIndex)
  {
    case CPC_MODEL_464_UK:
    case CPC_MODEL_464_FR:
    case CPC_MODEL_464_SP:
    {
      // Remove Default AMSDOS
      if (StringGridROMMapping->Cells[1][8] == DefaultAMSDOSRomName)
      {
        StringGridROMMapping->Cells[1][8] = "";
      }
    }
    break;

    default:
    {
      // Add Default AMSDOS
      if (StringGridROMMapping->Cells[1][8].IsEmpty() == true)
      {
        StringGridROMMapping->Cells[1][8] = DefaultAMSDOSRomName;
      }
    }
  }

  // Brand name
  switch (ComboBoxDevices->ItemIndex)
  {
    case CPC_MODEL_464_SP:
    {
      // Amstrad ONLY
      ComboBoxBrands->ItemIndex = 0;
      ComboBoxBrands->Enabled = false;
    }
    break;

    default:
    {
      ComboBoxBrands->Enabled = true;
    }
  }

  // CRTC Type
  switch (ComboBoxDevices->ItemIndex)
  {
    case CPC_MODEL_664_UK:
    case CPC_MODEL_664_FR:
    {
      // CRTC0 Only
      ComboBoxCRTCType->ItemIndex = 0;
      ComboBoxCRTCType->Enabled = false;
    }
    break;

    default:
    {
      ComboBoxCRTCType->Enabled = true;
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmulatorSettings::ComboBoxDevicesDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxDevices->Canvas;

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
  AnsiString Text = ComboBoxDevices->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxDevices->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmulatorSettings::ComboBoxBrandsDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxBrands->Canvas;

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
  AnsiString Text = ComboBoxBrands->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxBrands->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::CheckBoxUseFloppyDriveClick(TObject *Sender)

{
  // Add Default AMSDOS
  if (StringGridROMMapping->Cells[1][8].IsEmpty() == true)
  {
    StringGridROMMapping->Cells[1][8] = DefaultAMSDOSRomName;
  }

  if (CheckBoxUseFloppyDrive->Checked == true)
  {
    CheckBoxUseFastFDC->Enabled = true;
  }
  else
  {
    CheckBoxUseFastFDC->Checked = false;
    CheckBoxUseFastFDC->Enabled = false;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::ComboBoxCRTCTypeChange(
      TObject *Sender)
{
  // CRTC 1 choosen
  if (ComboBoxCRTCType->ItemIndex == 1)
  {
    Application->MessageBox(_T("CRTC 1 is still under development and not fully functional.\r\nMany features like lightguns are missing."), _T("Under development feature"), MB_OK);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmulatorSettings::ComboBoxCRTCTypeDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxCRTCType->Canvas;

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
  AnsiString Text = ComboBoxCRTCType->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxCRTCType->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmulatorSettings::CheckBoxUse256kSiliconDiscClick(TObject *Sender)

{
  if (CheckBoxUse256kSiliconDisc->Focused() == false) return;

  if (CheckBoxUse256kSiliconDisc->Checked == true)
  {
    int loop;
    // Find Silicon Disc ROM
    for (loop=1; loop < NB_UPPERROMS; loop++)
    {
      if (loop == 7) continue; // Already treated

      AnsiString ROM = StringGridROMMapping->Cells[1][loop+1];
      if (ROM == DefaultSiliconDiscRomName)
      {
        break;
      }
    }

    // Find empty ROM location for SiliconDisk
    if (loop == NB_UPPERROMS)
    {
      for (loop=1; loop < NB_UPPERROMS; loop++)
      {
        if (loop == 7) continue; // Already treated

        AnsiString ROM = StringGridROMMapping->Cells[1][loop+1];
        if (ROM == DefaultSiliconDiscRomName)
        {
          break;
        }
        else if (ROM.IsEmpty() == true)
        {
          StringGridROMMapping->Cells[1][loop+1] = DefaultSiliconDiscRomName;
          break;
        }
      }
    }

    // No empty location
    if (loop == NB_UPPERROMS)
    {
      Application->MessageBox(_T("No available ROM location for Silicon Disc."), _T("ROM"), MB_OK);
      CheckBoxUse256kSiliconDisc->Checked = false;
    }
  }
  else
  {
    // Remove Silicon Disc ROM
    for (int loop=1; loop < NB_UPPERROMS; loop++)
    {
      if (loop == 7) continue; // Already treated

      AnsiString ROM = StringGridROMMapping->Cells[1][loop+1];
      if (ROM == DefaultSiliconDiscRomName)
      {
        StringGridROMMapping->Cells[1][loop+1] = "";
        break;
      }
    }
  }

  UpdateAvailableROMList(StringGridROMMapping->Row);

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

