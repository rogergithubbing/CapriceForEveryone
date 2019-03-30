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
#include <tchar.h>
#pragma hdrstop
#include "TFormDebugMemoryEditor.h"
#include "TFormDebugEditValue.h"
#include "TFormMain.h"
#include "..\..\Common\ASCII.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

#undef USE_EDIT_DIALOG
#define USE_EDIT_DIALOG

//=================================


#define TEXT_DUMP_FILTER            "Simple text dump (.txt)|*.txt|"\
                                    "All files|*.*"
#define BINARY_DUMP_FILTER          "Binary dump (.bin)|*.bin|"\
                                    "All files|*.*"

static const AnsiString cTitle = "Memory editor";
static const AnsiString cTitlePaused  = " - [PAUSED]";
static const AnsiString cTitleMonitoring  = " - [Monitoring]";

static const AnsiString cBankAddresses[] =
{
  "&&0000",
  "&&4000",
  "&&8000",
  "&&C000"
};


TFormDebugMemoryEditor *FormDebugMemoryEditor;
//---------------------------------------------------------------------------
__fastcall TFormDebugMemoryEditor::TFormDebugMemoryEditor(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mSearchBytes = NULL;

  mChangeValueIncrement = 1;
  mChangeValueNextRequested = false;
  mChangeValueHexa = true;

  mShowMemoryDumpRoutine = NULL;
  mWriteDataRoutine = NULL;
  mSearchASCIIRoutine = NULL;
  mSearchHexRoutine = NULL;

  StringGridDump->ColCount = 19;
  StringGridDump->RowCount = 8;
  StringGridDump->ColWidths[0] = 45;
  StringGridDump->ColWidths[17] = 10;
  StringGridDump->ColWidths[18] = 120;

  SaveDialog->InitialDir = ExtractFilePath(Application->ExeName);
  OpenDialog->InitialDir = ExtractFilePath(Application->ExeName);

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  StringGridDump->ColWidths[0] = 56;
  StringGridDump->ColWidths[18] = 128;
  PanelFormatCurrentAddress->Font->Color = clBlack;
  PanelCurrentAddress->Font->Color = clBlack;
  PanelFormatAddress1->Font->Color = clBlack;
  PanelAddress1->Font->Color = clBlack;
  PanelFormatAddress2->Font->Color = clBlack;
  PanelAddress2->Font->Color = clBlack;
  PanelFormatSearch->Font->Color = clBlack;
  EditSearch->Font->Color = clBlack;
  EditSearch->Font->Size = 12;
  PanelFormatSearch->Height = EditSearch->Height;
  LabelSearch->Height = EditSearch->Height;
  PanelFormatFillFrom->Font->Color = clBlack;
  PanelFillFrom->Font->Color = clBlack;
  PanelFormatFillTo->Font->Color = clBlack;
  PanelFillTo->Font->Color = clBlack;
  PanelFormatFillData->Font->Color = clBlack;
  PanelFillData->Font->Color = clBlack;
  PanelFormatExportFrom->Font->Color = clBlack;
  PanelExportFrom->Font->Color = clBlack;
  PanelFormatExportTo->Font->Color = clBlack;
  PanelExportTo->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::FormDestroy(TObject *Sender)
{
  delete [] mSearchBytes;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  // Select Banked memory
  TabControlMemoryContent->TabIndex = 0;
  mMemoryStartAddress = 0x0000;
  mMemorySize = 0x10000;
  mShowMemoryDumpRoutine = ShowBankedMemoryDump;
  mWriteDataRoutine = WriteDataBankedMemory;
  mSearchASCIIRoutine = SearchASCIIBankedMemory;
  mSearchHexRoutine = SearchHexBankedMemory;
  mEditCellEnabled = true;
#ifndef USE_EDIT_DIALOG
  StringGridDump->Options = StringGridDump->Options << goEditing;
  StringGridDump->Options = StringGridDump->Options << goAlwaysShowEditor;
#endif /* !USE_EDIT_DIALOG */

  mMonitoring = false;
  ButtonMonitoring->Caption = "Start";
  mPaused = FormMain->NativeCPC->paused;
  Caption = Title;

  PageControlOperations->ActivePage = TabSheetAddress;

  mCurrentAddress = 0;
  mTopAddress = 0;
  mAddress1 = 0;
  mAddress2 = 0;

  PanelFormatAddress1->Caption = "&&";
  PanelAddress1->Caption = AnsiString::IntToHex(mAddress1, 5);
  PanelFormatAddress2->Caption = "&&";
  PanelAddress2->Caption = AnsiString::IntToHex(mAddress2, 5);
  PanelFormatCurrentAddress->Caption = "&&";
  PanelCurrentAddress->Caption = AnsiString::IntToHex(mCurrentAddress, 5);

  mFillFromAddress = 0;
  mFillToAddress = 0;
  mFillData = 0;

  PanelFormatFillFrom->Caption = "&&";
  PanelFillFrom->Caption = AnsiString::IntToHex(mFillFromAddress, 5);
  PanelFormatFillTo->Caption = "&&";
  PanelFillTo->Caption = AnsiString::IntToHex(mFillToAddress, 5);
  PanelFormatFillData->Caption = "&&";
  PanelFillData->Caption = AnsiString::IntToHex(mFillData, 2);

  mExportFromAddress = 0;
  mExportToAddress = 0;

  PanelFormatExportFrom->Caption = "&&";
  PanelExportFrom->Caption = AnsiString::IntToHex(mExportFromAddress, 5);
  PanelFormatExportTo->Caption = "&&";
  PanelExportTo->Caption = AnsiString::IntToHex(mExportToAddress, 5);

  StringGridDump->RowCount = StringGridDump->Height / StringGridDump->DefaultRowHeight;
  ScrollBarDump->LargeChange = StringGridDump->RowCount * 16;
  mMaxTopAddress = mMemorySize - ScrollBarDump->LargeChange;

  ScrollBarDump->Position = mTopAddress;
  ScrollBarDump->Min = mMemoryStartAddress;
  ScrollBarDump->Max = mMaxTopAddress;

  FormMain->SubscribeToCPCEvents(OnCPCEvent);

  // Show Banked memory
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  UpdateBanks(FormMain->NativeCPC);

  mCurrentData = StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row];
  ActiveControl = StringGridDump;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugMemoryEditor::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnCPCEvent);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDebugMemoryEditor::GetTitle(void)
{
  AnsiString Title = cTitle;

  if (mMonitoring == true) Title += cTitleMonitoring;
  if (mPaused == true) Title += cTitlePaused;

 return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->MemoryFormLeft != 0)
       || (mSettingsP->MemoryFormTop != 0)
       || (mSettingsP->MemoryFormWidth != 0)
       || (mSettingsP->MemoryFormHeight != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->MemoryFormLeft;
    Top = mSettingsP->MemoryFormTop;
    Width = mSettingsP->MemoryFormWidth;
    Height = mSettingsP->MemoryFormHeight;
  }

  ShowHint = mSettingsP->ShowHints;

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  LabelBanksRead->Font->Color = mSettingsP->ThemeText2Color;
  LabelBanksWrite->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankRead0000->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankWrite0000->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankRead4000->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankWrite4000->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankRead8000->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankWrite8000->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankReadC000->Font->Color = mSettingsP->ThemeText2Color;
  PanelBankWriteC000->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::SetDisplayRows(int Value)
{
  // Adjust selected row
  if (StringGridDump->Row >= Value)
  {
    StringGridDump->Row = Value - 1;
  }

  StringGridDump->RowCount = Value;
  ScrollBarDump->LargeChange = StringGridDump->RowCount * 16;
  mMaxTopAddress = mMemoryStartAddress + mMemorySize - ScrollBarDump->LargeChange;
  if (mTopAddress > mMaxTopAddress) mTopAddress = mMaxTopAddress;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::SetCurrentAddress(tULong Value)
{
  mCurrentAddress = Value;

  if (mCurrentAddress < mMemoryStartAddress)
  {
    mCurrentAddress = mMemoryStartAddress;
  }
  if (mCurrentAddress >= (mMemoryStartAddress + mMemorySize))
  {
    mCurrentAddress = mMemoryStartAddress + mMemorySize - 1;
  }

  PanelCurrentAddress->Caption = (PanelFormatCurrentAddress->Caption.IsEmpty() == false) ?
                                 AnsiString::IntToHex(mCurrentAddress, 5) :
                                 AnsiString(mCurrentAddress);
                                 
  mCurrentData = StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row];
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetMemorySize(tULong Value)
{
  mMemorySize = Value;
  mMaxTopAddress = mMemoryStartAddress + mMemorySize - ScrollBarDump->LargeChange;

  // Adjust current address
  if (mCurrentAddress < mMemoryStartAddress)
  {
    TopAddress = mMemoryStartAddress;
    CurrentAddress = mMemoryStartAddress;
  }
  if (mCurrentAddress >= (mMemoryStartAddress + mMemorySize))
  {
    TopAddress = mMemoryStartAddress + mMemorySize - 1;
    CurrentAddress = mMemoryStartAddress + mMemorySize - 1;
  }

  ScrollBarDump->Max = mMaxTopAddress;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetTopAddress(tULong Value)
{
  tULong Offset = mCurrentAddress - mTopAddress;
  mTopAddress = Value & 0xFFFFFFF0;

  if (mTopAddress > 0xff000000) mTopAddress = 0;
  else if (mTopAddress < mMemoryStartAddress) mTopAddress = mMemoryStartAddress;
  if (mTopAddress > mMaxTopAddress) mTopAddress = mMaxTopAddress;
  
  CurrentAddress = mTopAddress + Offset;
  ScrollBarDump->Position = mTopAddress;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetMemoryStartAddress(tULong Value)
{
  mMemoryStartAddress = Value;
  mMaxTopAddress = mMemoryStartAddress + mMemorySize - ScrollBarDump->LargeChange;

  if (mCurrentAddress < mMemoryStartAddress)
  {
    TopAddress = mMemoryStartAddress;
    CurrentAddress = mMemoryStartAddress;
  }
  if (mCurrentAddress >= (mMemoryStartAddress + mMemorySize))
  {
    TopAddress = mMemoryStartAddress + mMemorySize - 1;
    CurrentAddress = mMemoryStartAddress + mMemorySize - 1;
  }

  ScrollBarDump->Min = mMemoryStartAddress;
  ScrollBarDump->Max = mMaxTopAddress;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetMemoryIndex(int Value)
{
  TabControlMemoryContent->TabIndex = Value;

  switch (TabControlMemoryContent->TabIndex)
  {
    case 1: // Entire RAM
    {
      mMemoryStartAddress = 0x0000;
      mMemorySize = FormMain->NativeCPC->ram_size * 1024;
      mShowMemoryDumpRoutine = ShowEntireRAMDump;
      mWriteDataRoutine = WriteDataEntireRAM;
      mSearchASCIIRoutine = SearchASCIIEntireRAMMemory;
      mSearchHexRoutine = SearchHexEntireRAMMemory;
      mEditCellEnabled = true;
#ifndef USE_EDIT_DIALOG
      StringGridDump->Options = StringGridDump->Options << goEditing;
      StringGridDump->Options = StringGridDump->Options << goAlwaysShowEditor;
#endif /* !USE_EDIT_DIALOG */
    }
    break;

    case 2: // Lower ROM
    {
      mMemoryStartAddress = 0x0000;
      mMemorySize = 16384;
      mShowMemoryDumpRoutine = ShowLowerROMDump;
      mWriteDataRoutine = NULL;
      mSearchASCIIRoutine = SearchASCIILowerROMMemory;
      mSearchHexRoutine = SearchHexLowerROMMemory;
      mEditCellEnabled = false;
#ifndef USE_EDIT_DIALOG
      StringGridDump->Options = StringGridDump->Options >> goEditing;
      StringGridDump->Options = StringGridDump->Options >> goAlwaysShowEditor;
#endif /* !USE_EDIT_DIALOG */
    }
    break;

    case 3: // Upper ROM
    {
      mMemoryStartAddress = 0xC000;
      mMemorySize = 16384;
      mShowMemoryDumpRoutine = ShowUpperROMDump;
      mWriteDataRoutine = NULL;
      mSearchASCIIRoutine = SearchASCIIUpperROMMemory;
      mSearchHexRoutine = SearchHexUpperROMMemory;
      mEditCellEnabled = false;
#ifndef USE_EDIT_DIALOG
      StringGridDump->Options = StringGridDump->Options >> goEditing;
      StringGridDump->Options = StringGridDump->Options >> goAlwaysShowEditor;
#endif /* !USE_EDIT_DIALOG */
    }
    break;

    default: // Banked memory
      mMemoryStartAddress = 0x0000;
      mMemorySize = 0x10000;
      mShowMemoryDumpRoutine = ShowBankedMemoryDump;
      mWriteDataRoutine = WriteDataBankedMemory;
      mSearchASCIIRoutine = SearchASCIIBankedMemory;
      mSearchHexRoutine = SearchHexBankedMemory;
      mEditCellEnabled = true;
#ifndef USE_EDIT_DIALOG
      StringGridDump->Options = StringGridDump->Options << goEditing;
      StringGridDump->Options = StringGridDump->Options << goAlwaysShowEditor;
#endif /* USE_EDIT_DIALOG */
  }

  // Adapt other parameters
  mMaxTopAddress = mMemoryStartAddress + mMemorySize - ScrollBarDump->LargeChange;
  if (mCurrentAddress < mMemoryStartAddress)
  {
    mTopAddress = mMemoryStartAddress;
    mCurrentAddress = mMemoryStartAddress;
  }
  else if (mCurrentAddress >= (mMemoryStartAddress + mMemorySize))
  {
    mTopAddress = mMemoryStartAddress + mMemorySize - 1;
    mCurrentAddress = mMemoryStartAddress + mMemorySize - 1;
  }
  else if (mCurrentAddress < mTopAddress)
  {
    mTopAddress = mCurrentAddress;
  }
  else if (mCurrentAddress >= (mTopAddress+ScrollBarDump->LargeChange))
  {
    mTopAddress = mCurrentAddress;
  }
  mTopAddress = mTopAddress & 0xFFFFFFF0;
  if (mTopAddress > 0xff000000) mTopAddress = 0;
  else if (mTopAddress < mMemoryStartAddress) mTopAddress = mMemoryStartAddress;
  if (mTopAddress > mMaxTopAddress) mTopAddress = mMaxTopAddress;

  // Adapt scrollbar
  tULong OldTopAddress = mTopAddress;
  ScrollBarDump->Min = 0;
  ScrollBarDump->Position = 0;
  ScrollBarDump->Max = mMaxTopAddress;
  ScrollBarDump->Position = OldTopAddress;
  ScrollBarDump->Min = mMemoryStartAddress;

  // Update current address
  PanelCurrentAddress->Caption = (PanelFormatCurrentAddress->Caption.IsEmpty() == false) ?
                                 AnsiString::IntToHex(mCurrentAddress, 5) :
                                 AnsiString(mCurrentAddress);
  mCurrentData = StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row];
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) && (!Tag) )
  {
    mSettingsP->MemoryFormWidth = Width;
    mSettingsP->MemoryFormHeight = Height;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) && (!Tag) )
  {
    mSettingsP->MemoryFormLeft = Left;
    mSettingsP->MemoryFormTop = Top;
  }

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelFormatAddress1Click(
      TObject *Sender)
{
  if (PanelFormatAddress1->Caption.IsEmpty() == true)
  {
    PanelFormatAddress1->Caption = "&&";
    PanelAddress1->Caption = AnsiString::IntToHex(mAddress1, 5);
  }
  else
  {
    PanelFormatAddress1->Caption = "";
    PanelAddress1->Caption = mAddress1;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::PanelAddress1Click(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mAddress1;
  EditValue->Hexadecimal = PanelFormatAddress1->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 1048575;
  if (EditValue->ShowModal() == mrOk)
  {
    mAddress1 = EditValue->NewValue;
    PanelAddress1->Caption = (PanelFormatAddress1->Caption.IsEmpty() == false) ?
                             AnsiString::IntToHex(mAddress1, 5) :
                             AnsiString(mAddress1);

    if (mAddress1 < mTopAddress)
    {
      TopAddress = mAddress1;
    }
    else if (mAddress1 >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = mAddress1;
    }
    CurrentAddress = mAddress1;
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonGoToAddress1Click(TObject *Sender)
{
  if (mAddress1 < mTopAddress)
  {
    TopAddress = mAddress1;
  }
  else if (mAddress1 >= (mTopAddress+ScrollBarDump->LargeChange))
  {
    TopAddress = mAddress1;
  }
  CurrentAddress = mAddress1;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelFormatAddress2Click(
      TObject *Sender)
{
  if (PanelFormatAddress2->Caption.IsEmpty() == true)
  {
    PanelFormatAddress2->Caption = "&&";
    PanelAddress2->Caption = AnsiString::IntToHex(mAddress2, 5);
  }
  else
  {
    PanelFormatAddress2->Caption = "";
    PanelAddress2->Caption = mAddress2;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::PanelAddress2Click(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mAddress2;
  EditValue->Hexadecimal = PanelFormatAddress2->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 1048575;
  if (EditValue->ShowModal() == mrOk)
  {
    mAddress2 = EditValue->NewValue;
    PanelAddress2->Caption = (PanelFormatAddress2->Caption.IsEmpty() == false) ?
                             AnsiString::IntToHex(mAddress2, 5) :
                             AnsiString(mAddress2);

    if (mAddress2 < mTopAddress)
    {
      TopAddress = mAddress2;
    }
    else if (mAddress2 >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = mAddress2;
    }
    CurrentAddress = mAddress2;
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonGoToAddress2Click(TObject *Sender)
{
  if (mAddress2 < mTopAddress)
  {
    TopAddress = mAddress2;
  }
  else if (mAddress2 >= (mTopAddress+ScrollBarDump->LargeChange))
  {
    TopAddress = mAddress2;
  }
  CurrentAddress = mAddress2;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ButtonRegBCClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = _BC;
  CurrentAddress = _BC;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegDEClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = _DE;
  CurrentAddress = _DE;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegHLClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = _HL;
  CurrentAddress = _HL;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegBCAltClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = Z80->Regs.BCx.w.l;
  CurrentAddress = Z80->Regs.BCx.w.l;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegDEAltClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = Z80->Regs.DEx.w.l;
  CurrentAddress = Z80->Regs.DEx.w.l;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegHLAltClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = Z80->Regs.HLx.w.l;
  CurrentAddress = Z80->Regs.HLx.w.l;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegSPClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = _SP;
  CurrentAddress = _SP;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegPCClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = _PC;
  CurrentAddress = _PC;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegIXClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = _IX;
  CurrentAddress = _IX;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonRegIYClick(TObject *Sender)
{
tZ80* Z80 = FormMain->NativeCPC->Z80;

  MemoryIndex = 0; // Select banked memory
  TopAddress = _IY;
  CurrentAddress = _IY;
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  StringGridDump->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelFormatCurrentAddressClick(
      TObject *Sender)
{
  if (PanelFormatCurrentAddress->Caption.IsEmpty() == true)
  {
    PanelFormatCurrentAddress->Caption = "&&";
    PanelCurrentAddress->Caption = AnsiString::IntToHex(mCurrentAddress, 5);
  }
  else
  {
    PanelFormatCurrentAddress->Caption = "";
    PanelCurrentAddress->Caption = mCurrentAddress;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelCurrentAddressClick(
      TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mCurrentAddress;
  EditValue->Hexadecimal = PanelFormatCurrentAddress->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = (1<<20) - 1;
  if (EditValue->ShowModal() == mrOk)
  {
    tULong NewCurrentAddress = EditValue->NewValue;
    if (NewCurrentAddress < mTopAddress)
    {
      TopAddress = NewCurrentAddress;
    }
    else if (NewCurrentAddress >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = NewCurrentAddress;
    }
    CurrentAddress = NewCurrentAddress;
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::CheckBoxASCII7BitsClick(
      TObject *Sender)
{
  // Update memory content
  mShowMemoryDumpRoutine(FormMain->NativeCPC);
  ActiveControl = StringGridDump;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelFormatFillFromClick(
      TObject *Sender)
{
  if (PanelFormatFillFrom->Caption.IsEmpty() == true)
  {
    PanelFormatFillFrom->Caption = "&&";
    PanelFillFrom->Caption = AnsiString::IntToHex(mFillFromAddress, 5);
  }
  else
  {
    PanelFormatFillFrom->Caption = "";
    PanelFillFrom->Caption = mFillFromAddress;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::PanelFormatFillToClick(
      TObject *Sender)
{
  if (PanelFormatFillTo->Caption.IsEmpty() == true)
  {
    PanelFormatFillTo->Caption = "&&";
    PanelFillTo->Caption = AnsiString::IntToHex(mFillToAddress, 5);
  }
  else
  {
    PanelFormatFillTo->Caption = "";
    PanelFillTo->Caption = mFillToAddress;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::PanelFormatFillDataClick(
      TObject *Sender)
{
  if (PanelFormatFillData->Caption.IsEmpty() == true)
  {
    PanelFormatFillData->Caption = "&&";
    PanelFillData->Caption = AnsiString::IntToHex(mFillData, 2);
  }
  else
  {
    PanelFormatFillData->Caption = "";
    PanelFillData->Caption = mFillData;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelFillFromClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mFillFromAddress;
  EditValue->Hexadecimal = PanelFormatFillFrom->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 1048575;
  if (EditValue->ShowModal() == mrOk)
  {
    mFillFromAddress = EditValue->NewValue;
    PanelFillFrom->Caption = (PanelFormatFillFrom->Caption.IsEmpty() == false) ?
                             AnsiString::IntToHex(mFillFromAddress, 5) :
                             AnsiString(mFillFromAddress);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::PanelFillToClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mFillToAddress;
  EditValue->Hexadecimal = PanelFormatFillTo->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 1048575;
  if (EditValue->ShowModal() == mrOk)
  {
    mFillToAddress = EditValue->NewValue;
    PanelFillTo->Caption = (PanelFormatFillTo->Caption.IsEmpty() == false) ?
                           AnsiString::IntToHex(mFillToAddress, 5) :
                           AnsiString(mFillToAddress);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::PanelFillDataClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mFillData;
  EditValue->Hexadecimal = PanelFormatFillData->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 2;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 255;
  if (EditValue->ShowModal() == mrOk)
  {
    mFillData = EditValue->NewValue;
    PanelFillData->Caption = (PanelFormatFillData->Caption.IsEmpty() == false) ?
                             AnsiString::IntToHex(mFillData, 2) :
                             AnsiString(mFillData);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelFormatExportFromClick(
      TObject *Sender)
{
  if (PanelFormatExportFrom->Caption.IsEmpty() == true)
  {
    PanelFormatExportFrom->Caption = "&&";
    PanelExportFrom->Caption = AnsiString::IntToHex(mExportFromAddress, 5);
  }
  else
  {
    PanelFormatExportFrom->Caption = "";
    PanelExportFrom->Caption = mExportFromAddress;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugMemoryEditor::PanelFormatExportToClick(
      TObject *Sender)
{
  if (PanelFormatExportTo->Caption.IsEmpty() == true)
  {
    PanelFormatExportTo->Caption = "&&";
    PanelExportTo->Caption = AnsiString::IntToHex(mExportToAddress, 5);
  }
  else
  {
    PanelFormatExportTo->Caption = "";
    PanelExportTo->Caption = mExportToAddress;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugMemoryEditor::PanelExportFromClick(
      TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mExportFromAddress;
  EditValue->Hexadecimal = PanelFormatExportFrom->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 1048575;
  if (EditValue->ShowModal() == mrOk)
  {
    mExportFromAddress = EditValue->NewValue;
    PanelExportFrom->Caption = (PanelFormatExportFrom->Caption.IsEmpty() == false) ?
                               AnsiString::IntToHex(mExportFromAddress, 5) :
                               AnsiString(mExportFromAddress);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugMemoryEditor::PanelExportToClick(TObject *Sender)
{
  TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
  EditValue->Settings = mSettingsP;
  EditValue->CurrentValue = mExportToAddress;
  EditValue->Hexadecimal = PanelFormatExportTo->Caption.IsEmpty() == true ? false : true;
  EditValue->NbDigits = 5;
  EditValue->MinValue = 0;
  EditValue->MaxValue = 1048575;
  if (EditValue->ShowModal() == mrOk)
  {
    mExportToAddress = EditValue->NewValue;
    PanelExportTo->Caption = (PanelFormatExportTo->Caption.IsEmpty() == false) ?
                               AnsiString::IntToHex(mExportToAddress, 5) :
                               AnsiString(mExportToAddress);
  }
  delete EditValue;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------



void __fastcall TFormDebugMemoryEditor::ButtonFillClick(TObject *Sender)
{
  tULong EndAddress = mFillToAddress;
  if (EndAddress >= FormMain->NativeCPC->ram_size * 1024)
  {
    EndAddress = (FormMain->NativeCPC->ram_size * 1024) - 1;
  }

  for (tULong Address = mFillFromAddress; Address <= EndAddress; Address++)
  {
    *(FormMain->NativeCPC->pbRAM + Address) = (tUChar)mFillData;
  }

  // Refresh dump display
  mShowMemoryDumpRoutine(FormMain->NativeCPC);
  ActiveControl = StringGridDump;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::StringGridDumpSelectCell(
			TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
  // Only data are selectable
  CanSelect = (ACol < 17) ? true : false;
  if (CanSelect == false)
  {
    // Limit to 16th column
    StringGridDump->Col = 16;
  }

	NOT_USED(Sender);
	NOT_USED(ARow);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::StringGridDumpClick(
      TObject *Sender)
{
  // Update current address
  CurrentAddress = mTopAddress + (StringGridDump->Row * 16) + (StringGridDump->Col - 1);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::StringGridDumpDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State)
{
  TStringGrid* StringGrid = (TStringGrid*)Sender;

  if (ACol == 0)
  {
    StringGrid->Canvas->Brush->Color = Color;
    StringGrid->Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }
  else if (ACol == 18)
  {
    StringGrid->Canvas->Brush->Color = Color;
    StringGrid->Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }
  else if (State.Contains(gdSelected))
  {
    StringGrid->Canvas->Brush->Color = clHighlight;
    StringGrid->Canvas->Font->Color = clWhite;
  }
  else
  {
    StringGrid->Canvas->Brush->Color = Color;
    StringGrid->Canvas->Font->Color = StringGrid->Font->Color;
  }

  // Draw cell
  StringGrid->Canvas->FillRect(Rect);
  int TextWidth = StringGrid->Canvas->TextWidth(StringGrid->Cells[ACol][ARow]);
  int TextHeight = StringGrid->Canvas->TextHeight(StringGrid->Cells[ACol][ARow]);
  int TextPosX = (StringGrid->ColWidths[ACol] - TextWidth) / 2;
  int TextPosY = (StringGrid->RowHeights[ARow] - TextHeight) / 2;
  StringGrid->Canvas->TextOut(Rect.Left+TextPosX,
                              Rect.Top+TextPosY,
                              StringGrid->Cells[ACol][ARow]);
  if (State.Contains(gdFocused))
  {
    StringGrid->Canvas->DrawFocusRect(Rect);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::StringGridDumpMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  // Right click button
  if (Button == mbRight)
  {
    int ACol;
    int ARow;

    // Get cell from coordinates
    StringGridDump->MouseToCell(X, Y, ACol, ARow);

    // Select cell
    StringGridDump->Col = ACol;
    StringGridDump->Row = ARow;

    // Update current address
    CurrentAddress = mTopAddress + (StringGridDump->Row * 16) + (StringGridDump->Col - 1);
  }

	NOT_USED(Sender);
	NOT_USED(Shift);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::StringGridDumpKeyDown(
      TObject *Sender, WORD &Key, TShiftState Shift)
{
  switch (Key)
  {
    case VK_UP:
    {
      if (!StringGridDump->Row)
      {
        TopAddress -= 16;
        mShowMemoryDumpRoutine(FormMain->NativeCPC);
      }
    }
    break;

    case VK_PRIOR:
    {
      if (!StringGridDump->Row)
      {
        TopAddress -= StringGridDump->RowCount * 16;
        mShowMemoryDumpRoutine(FormMain->NativeCPC);
      }
    }
    break;

    case VK_NEXT:
    {
      if (StringGridDump->Row == StringGridDump->RowCount-1)
      {
        TopAddress += StringGridDump->RowCount * 16;
        mShowMemoryDumpRoutine(FormMain->NativeCPC);
      }
    }
    break;

    case VK_DOWN:
    {
      if (StringGridDump->Row == StringGridDump->RowCount-1)
      {
        TopAddress += 16;
        mShowMemoryDumpRoutine(FormMain->NativeCPC);
      }
    }
    break;

    case VK_ESCAPE:
    {
      StringGridDump->EditorMode = false;
      StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row] = mCurrentData;
    }
    break;

    case VK_RETURN:
    {
#ifndef USE_EDIT_DIALOG
      if (StringGridDump->EditorMode == true)
      {
        StringGridDump->EditorMode = false;

        // Write new data
        if (mWriteDataRoutine)
        {
          mWriteDataRoutine(FormMain->NativeCPC);
        }
      }

      // Go next byte
      tULong NextAddress = mCurrentAddress + 1;
      if (NextAddress < (mMemoryStartAddress + mMemorySize))
      {
        if (NextAddress >= (mTopAddress+ScrollBarDump->LargeChange))
        {
          TopAddress += 16;
        }
        CurrentAddress = NextAddress;
      }

      // Update memory content
      mShowMemoryDumpRoutine(FormMain->NativeCPC);
#else /* USE_EDIT_DIALOG */
      // Edit memory
      StringGridDumpDblClick(Sender);
#endif /* USE_EDIT_DIALOG */

    }
    break;
  }

	NOT_USED(Sender);
	NOT_USED(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::StringGridDumpKeyPress(
      TObject *Sender, char &Key)
{
  // Allowed keys for hexadecimal edition
  if ((Key >= '0') && (Key <= '9'))
    return;
  if (Key == VK_DELETE)
    return;
  if ( (Key >= 'a') && (Key <= 'f') )
    return;
  if ( (Key >= 'A') && (Key <= 'F') )
    return;

  // All other keys are ignored
  Key = 0;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::StringGridDumpMouseWheelUp(
			TObject *Sender, TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  if (!StringGridDump->Row)
  {
    TopAddress -= 16;
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(Sender);
	NOT_USED(Shift);
	NOT_USED(MousePos);
	NOT_USED(Handled);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::StringGridDumpMouseWheelDown(
      TObject *Sender, TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  if (StringGridDump->Row == StringGridDump->RowCount-1)
  {
    TopAddress += 16;
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(Sender);
	NOT_USED(Shift);
	NOT_USED(MousePos);
	NOT_USED(Handled);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::StringGridDumpGetEditMask(
			TObject *Sender, int ACol, int ARow, AnsiString &Value)
{
  Value = "Aa";

	NOT_USED(Sender);
	NOT_USED(ACol);
	NOT_USED(ARow);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::SetAsAddress1MenuClick(
      TObject *Sender)
{
  mAddress1 = mCurrentAddress;
  PanelAddress1->Caption = (PanelFormatAddress1->Caption.IsEmpty() == false) ?
                           AnsiString::IntToHex(mAddress1, 5) :
                           AnsiString(mAddress1);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugMemoryEditor::SetAsAddress2MenuClick(
      TObject *Sender)
{
  mAddress2 = mCurrentAddress;
  PanelAddress2->Caption = (PanelFormatAddress1->Caption.IsEmpty() == false) ?
                           AnsiString::IntToHex(mAddress2, 5) :
                           AnsiString(mAddress2);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetAsFillStartMenuClick(
      TObject *Sender)
{
  mFillFromAddress = mCurrentAddress;
  PanelFillFrom->Caption = (PanelFormatFillFrom->Caption.IsEmpty() == false) ?
                           AnsiString::IntToHex(mFillFromAddress, 5) :
                           AnsiString(mFillFromAddress);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetAsFillEndMenuClick(
      TObject *Sender)
{
  mFillToAddress = mCurrentAddress;
  PanelFillTo->Caption = (PanelFormatFillTo->Caption.IsEmpty() == false) ?
                         AnsiString::IntToHex(mFillToAddress, 5) :
                         AnsiString(mFillToAddress);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetAsFillDataMenuClick(
      TObject *Sender)
{
  AnsiString HexValue = "0x0" + mCurrentData;
  mFillData = (tULong)StrToInt(HexValue.Trim());
  PanelFillData->Caption = (PanelFormatFillData->Caption.IsEmpty() == false) ?
                           AnsiString::IntToHex(mFillData, 2) :
                           AnsiString(mFillData);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetAsDumpFromMenuClick(
      TObject *Sender)
{
  mExportFromAddress = mCurrentAddress;
  PanelExportFrom->Caption = (PanelFormatExportFrom->Caption.IsEmpty() == false) ?
                             AnsiString::IntToHex(mExportFromAddress, 5) :
                             AnsiString(mExportFromAddress);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SetAsDumpToMenuClick(
      TObject *Sender)
{
  mExportToAddress = mCurrentAddress;
  PanelExportTo->Caption = (PanelFormatExportTo->Caption.IsEmpty() == false) ?
                           AnsiString::IntToHex(mExportToAddress, 5) :
                           AnsiString(mExportToAddress);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ScrollBarDumpChange(
      TObject *Sender)
{
  if (ScrollBarDump->Focused())
  {
    tULong Offset = mCurrentAddress - mTopAddress;
    mTopAddress = (tULong)ScrollBarDump->Position;
    mTopAddress &= 0xFFFFFFF0;
    CurrentAddress = mTopAddress + Offset;

    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PageControlOperationsChange(
      TObject *Sender)
{
  if (PageControlOperations->ActivePage == TabSheetSearch)
  {
    ActiveControl = EditSearch;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::PageControlOperationsDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = PageControlOperations->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }

  // Get sheet corresponding to TabIndex
  int TabCount=0;
  TTabSheet* Sheet = NULL;
  for (int Loop=0; Loop < PageControlOperations->PageCount; Loop++)
  {
    if (PageControlOperations->Pages[Loop]->TabVisible == true)
    {
      if (TabCount == TabIndex)
      {
        Sheet = PageControlOperations->Pages[Loop];
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
  int TextPosY = (PageControlOperations->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::PanelFormatSearchClick(
      TObject *Sender)
{
  if (PanelFormatSearch->Caption == "$")
  {
    PanelFormatSearch->Caption = "&&";
    LabelSearch->Caption = "Hex data";
  }
  else
  {
    PanelFormatSearch->Caption = "$";
    LabelSearch->Caption = "Text";
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ButtonSearchNextClick(
      TObject *Sender)
{
  if (EditSearch->Text.Length() == 0)
    return;

  if (PanelFormatSearch->Caption == "$")
  {
    // Convert AnsiString to char array
    mSearchLength = EditSearch->Text.Length();
    delete [] mSearchBytes;
    mSearchBytes = new char[mSearchLength+1];
    strcpy(mSearchBytes, AnsiString(EditSearch->Text).c_str() );

    if (mSearchASCIIRoutine)
    {
      mSearchASCIIRoutine(FormMain->NativeCPC);
    }
  }
  else
  {
    // Convert Uppercase AnsiString to char array
    char* UpperSearchString = new char[EditSearch->Text.Length()+1];
    strcpy(UpperSearchString, AnsiString(EditSearch->Text).UpperCase().c_str() );

    char* UpperHexString = new char[EditSearch->Text.Length()+1];
    int UpperHexStringLength = 0;

    // Remove non hexa characters
    for (int CharLoop=0; CharLoop < EditSearch->Text.Length(); CharLoop++)
    {
      if ( (UpperSearchString[CharLoop] >= '0') && (UpperSearchString[CharLoop] <= '9'))
      {
        UpperHexString[UpperHexStringLength++] = UpperSearchString[CharLoop];
      }
      if ( (UpperSearchString[CharLoop] >= 'A') && (UpperSearchString[CharLoop] <= 'F'))
      {
        UpperHexString[UpperHexStringLength++] = UpperSearchString[CharLoop];
      }
    }
    UpperHexString[UpperHexStringLength] = '\0';

    delete [] UpperSearchString;

    delete [] mSearchBytes;
    mSearchLength = UpperHexStringLength / 2;
    mSearchBytes = new char[mSearchLength];

    // Create hex array
    int HexIndex = 0;
    for (int CharLoop=0; CharLoop < UpperHexStringLength; CharLoop+=2)
    {
      // High nibble (First character)
      if ( (UpperHexString[CharLoop] >= '0') && (UpperHexString[CharLoop] <= '9'))
      {
        mSearchBytes[HexIndex] = (UpperHexString[CharLoop] - '0') << 4;
      }
      else if ( (UpperHexString[CharLoop] >= 'A') && (UpperHexString[CharLoop] <= 'F'))
      {
        mSearchBytes[HexIndex] = (UpperHexString[CharLoop] - 'A' + 10) << 4;
      }

      // Low nibble (Second character)
      if ( (UpperHexString[CharLoop+1] >= '0') && (UpperHexString[CharLoop+1] <= '9'))
      {
        mSearchBytes[HexIndex++] += UpperHexString[CharLoop+1] - '0';
      }
      else if ( (UpperHexString[CharLoop+1] >= 'A') && (UpperHexString[CharLoop+1] <= 'F'))
      {
        mSearchBytes[HexIndex++] += UpperHexString[CharLoop+1] - 'A' + 10;
      }
    }

    delete [] UpperHexString;

    if (mSearchHexRoutine)
    {
      mSearchHexRoutine(FormMain->NativeCPC);
    }
  }
  
  ActiveControl = StringGridDump;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ButtonRefreshClick(TObject *Sender)
{
  UpdateBanks(FormMain->NativeCPC);
  mShowMemoryDumpRoutine(FormMain->NativeCPC);
  ActiveControl = StringGridDump;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ButtonMonitoringClick(TObject *Sender)
{
  mMonitoring = mMonitoring == true ? false : true;
  Caption = Title;

  if (mMonitoring == true)
  {
    ButtonMonitoring->Caption = "Stop";
  }
  else
  {
    ButtonMonitoring->Caption = "Start";
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::TabControlMemoryContentResize(
      TObject *Sender)
{
  if (Visible == true)
  {
    DisplayRows = StringGridDump->Height / StringGridDump->DefaultRowHeight;
    if (mShowMemoryDumpRoutine)
    {
      mShowMemoryDumpRoutine(FormMain->NativeCPC);
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::TabControlMemoryContentChange(
      TObject *Sender)
{
  MemoryIndex = TabControlMemoryContent->TabIndex;
  ActiveControl = StringGridDump;

  // Update memory content
  mShowMemoryDumpRoutine(FormMain->NativeCPC);

  // Update bank
  CurrentAddress = mCurrentAddress;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::TabControlMemoryContentDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = TabControlMemoryContent->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }
  else
  {
    Canvas->Font->Color = mSettingsP->ThemeText1Color;
  }

  // Draw tab
  Canvas->FillRect(Rect);
  AnsiString Text = TabControlMemoryContent->Tabs->Strings[TabIndex];
  int TextWidth = Canvas->TextWidth(Text);
  int TextHeight = Canvas->TextHeight(Text);
  int TextPosX = (Rect.Width() - TextWidth) / 2;
  int TextPosY = (TabControlMemoryContent->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ShowBankedMemoryDump(tNativeCPC* NativeCPC)
{
tULong BaseAddress = mTopAddress;
AnsiString ASCIIDump;

  // Select current address cell
  int CurrentRow = (mCurrentAddress - mTopAddress) / 16;
  int CurrentCol = ((mCurrentAddress - mTopAddress) % 16) + 1;
  StringGridDump->Row = CurrentRow;
  StringGridDump->Col = CurrentCol;

  int MaxRow = StringGridDump->Height / StringGridDump->DefaultRowHeight;
  int RowLoop;
  for (RowLoop=0; RowLoop < MaxRow; RowLoop++)
  {
    if (BaseAddress >= (mMemoryStartAddress + mMemorySize))
      break;

    StringGridDump->Cells[0][RowLoop] = AnsiString::IntToHex(BaseAddress, 4);
    ASCIIDump = "";

    for (int ByteLoop=0; ByteLoop < 16; ByteLoop++)
    {
      tUChar Data = (tUChar)Engine_ReadMem(NativeCPC, BaseAddress-mMemoryStartAddress+ByteLoop);
      StringGridDump->Cells[ByteLoop+1][RowLoop] = AnsiString::IntToHex(Data, 2);
      ASCIIDump += (CheckBoxASCII7Bits->Checked == true) ? ASCIIChar[Data & 0x7f] : ASCIIChar[Data];
    }

    StringGridDump->Cells[18][RowLoop] = ASCIIDump;
    BaseAddress += 16;
  }

  StringGridDump->RowCount = RowLoop;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ShowEntireRAMDump(tNativeCPC* NativeCPC)
{
tULong BaseAddress = mTopAddress;
AnsiString ASCIIDump;

  // Select current address cell
  int CurrentRow = (mCurrentAddress - mTopAddress) / 16;
  int CurrentCol = ((mCurrentAddress - mTopAddress) % 16) + 1;
  StringGridDump->Row = CurrentRow;
  StringGridDump->Col = CurrentCol;

  int MaxRow = StringGridDump->Height / StringGridDump->DefaultRowHeight;
  int RowLoop;
  for (RowLoop=0; RowLoop < MaxRow; RowLoop++)
  {
    if (BaseAddress >= (mMemoryStartAddress + mMemorySize))
      break;

    StringGridDump->Cells[0][RowLoop] = AnsiString::IntToHex(BaseAddress, 5);
    ASCIIDump = "";
    tUChar* RAMData = (tUChar*)FormMain->NativeCPC->pbRAM;

    for (int ByteLoop=0; ByteLoop < 16; ByteLoop++)
    {
      tUChar Data = *(RAMData + BaseAddress - mMemoryStartAddress + ByteLoop);
      StringGridDump->Cells[ByteLoop+1][RowLoop] = AnsiString::IntToHex(Data, 2);
      ASCIIDump += (CheckBoxASCII7Bits->Checked == true) ? ASCIIChar[Data & 0x7f] : ASCIIChar[Data];
    }

    StringGridDump->Cells[18][RowLoop] = ASCIIDump;
    BaseAddress += 16;
  }

  StringGridDump->RowCount = RowLoop;

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ShowLowerROMDump(tNativeCPC* NativeCPC)
{
tULong BaseAddress = mTopAddress;
AnsiString ASCIIDump;

  // Select current address cell
  int CurrentRow = (mCurrentAddress - mTopAddress) / 16;
  int CurrentCol = ((mCurrentAddress - mTopAddress) % 16) + 1;
  StringGridDump->Row = CurrentRow;
  StringGridDump->Col = CurrentCol;

  int MaxRow = StringGridDump->Height / StringGridDump->DefaultRowHeight;
  int RowLoop;
  for (RowLoop=0; RowLoop < MaxRow; RowLoop++)
  {
    if (BaseAddress >= (mMemoryStartAddress + mMemorySize))
      break;

    StringGridDump->Cells[0][RowLoop] = AnsiString::IntToHex(BaseAddress, 4);
    ASCIIDump = "";
    tUChar* RAMData = (tUChar*)FormMain->NativeCPC->pbROMlo;

    for (int ByteLoop=0; ByteLoop < 16; ByteLoop++)
    {
      tUChar Data = *(RAMData + BaseAddress - mMemoryStartAddress + ByteLoop);
      StringGridDump->Cells[ByteLoop+1][RowLoop] = AnsiString::IntToHex(Data, 2);
      ASCIIDump += (CheckBoxASCII7Bits->Checked == true) ? ASCIIChar[Data & 0x7f] : ASCIIChar[Data];
    }

    StringGridDump->Cells[18][RowLoop] = ASCIIDump;
    BaseAddress += 16;
  }

  StringGridDump->RowCount = RowLoop;

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::ShowUpperROMDump(tNativeCPC* NativeCPC)
{
tULong BaseAddress = mTopAddress;
AnsiString ASCIIDump;

  // Select current address cell
  int CurrentRow = (mCurrentAddress - mTopAddress) / 16;
  int CurrentCol = ((mCurrentAddress - mTopAddress) % 16) + 1;
  StringGridDump->Row = CurrentRow;
  StringGridDump->Col = CurrentCol;

  int MaxRow = StringGridDump->Height / StringGridDump->DefaultRowHeight;
  int RowLoop;
  for (RowLoop=0; RowLoop < MaxRow; RowLoop++)
  {
    if (BaseAddress >= (mMemoryStartAddress + mMemorySize))
      break;

    StringGridDump->Cells[0][RowLoop] = AnsiString::IntToHex(BaseAddress, 4);
    ASCIIDump = "";
    tUChar* RAMData = (tUChar*)FormMain->NativeCPC->pbExpansionROM;

    for (int ByteLoop=0; ByteLoop < 16; ByteLoop++)
    {
      tUChar Data = *(RAMData + BaseAddress - mMemoryStartAddress + ByteLoop);
      StringGridDump->Cells[ByteLoop+1][RowLoop] = AnsiString::IntToHex(Data, 2);
      ASCIIDump += (CheckBoxASCII7Bits->Checked == true) ? ASCIIChar[Data & 0x7f] : ASCIIChar[Data];
    }

    StringGridDump->Cells[18][RowLoop] = ASCIIDump;
    BaseAddress += 16;
  }

  StringGridDump->RowCount = RowLoop;

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::WriteDataBankedMemory(tNativeCPC* NativeCPC)
{
  try
  {
    AnsiString NewData = StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row];
    AnsiString HexValue = "0x0" + NewData;
    tULong Data = (tULong)StrToInt(HexValue.Trim());
    Engine_WriteMem(NativeCPC, mCurrentAddress-mMemoryStartAddress, Data);
  }
  catch(...)
  {
    StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row] = mCurrentData;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::WriteDataEntireRAM(tNativeCPC* NativeCPC)
{
  try
  {
    AnsiString NewData = StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row];
    AnsiString HexValue = "0x0" + NewData;
    tUChar Data = (tUChar)StrToInt(HexValue.Trim());

    *(FormMain->NativeCPC->pbRAM + mCurrentAddress - mMemoryStartAddress) = Data;
  }
  catch(...)
  {
    StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row] = mCurrentData;
  }

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::SearchASCIIBankedMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = (tUChar)Engine_ReadMem(NativeCPC, Address);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Adjust case sensitive
    if (CheckBoxStringCaseSensitive->Checked == false)
    {
      if ( (Data >= 'a') && (Data <= 'z')) Data -= 'a' - 'A';
      if ( (SearchData >= 'a') && (SearchData <= 'z')) SearchData -= 'a' - 'A';
    }

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SearchHexBankedMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = (tUChar)Engine_ReadMem(NativeCPC, Address);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SearchASCIIEntireRAMMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = *(FormMain->NativeCPC->pbRAM + Address - mMemoryStartAddress);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Adjust case sensitive
    if (CheckBoxStringCaseSensitive->Checked == false)
    {
      if ( (Data >= 'a') && (Data <= 'z')) Data -= 'a' - 'A';
      if ( (SearchData >= 'a') && (SearchData <= 'z')) SearchData -= 'a' - 'A';
    }

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SearchHexEntireRAMMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = *(FormMain->NativeCPC->pbRAM + Address - mMemoryStartAddress);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SearchASCIILowerROMMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = *(FormMain->NativeCPC->pbROMlo + Address - mMemoryStartAddress);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Adjust case sensitive
    if (CheckBoxStringCaseSensitive->Checked == false)
    {
      if ( (Data >= 'a') && (Data <= 'z')) Data -= 'a' - 'A';
      if ( (SearchData >= 'a') && (SearchData <= 'z')) SearchData -= 'a' - 'A';
    }

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SearchHexLowerROMMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = *(FormMain->NativeCPC->pbROMlo + Address - mMemoryStartAddress);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SearchASCIIUpperROMMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = *(FormMain->NativeCPC->pbExpansionROM + Address - mMemoryStartAddress);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Adjust case sensitive
    if (CheckBoxStringCaseSensitive->Checked == false)
    {
      if ( (Data >= 'a') && (Data <= 'z')) Data -= 'a' - 'A';
      if ( (SearchData >= 'a') && (SearchData <= 'z')) SearchData -= 'a' - 'A';
    }

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugMemoryEditor::SearchHexUpperROMMemory(tNativeCPC* NativeCPC)
{
tULong Address = mCurrentAddress;
tULong FirstAddress = Address;
tULong FirstFound;
int CompIndex = 0;

  do
  {
    // Next data
    Address++;
    if (Address == (mMemoryStartAddress+mMemorySize))
    {
      Address = mMemoryStartAddress;
    }

    tUChar Data = *(FormMain->NativeCPC->pbExpansionROM + Address - mMemoryStartAddress);
    tUChar SearchData = mSearchBytes[CompIndex];

    // Comparaison OK ?
    if (Data == SearchData)
    {
      if (CompIndex == 0)
      {
        FirstFound = Address;
      }

      CompIndex++;
      // String found ?
      if (CompIndex == mSearchLength)
        break;
    }
    else
    {
      CompIndex = 0;
    }
  }
  while (Address != FirstAddress);

  // Occurence found ?
  if (CompIndex == mSearchLength)
  {
    if (FirstFound < mTopAddress)
    {
      TopAddress = FirstFound;
    }
    else if (FirstFound >= (mTopAddress+ScrollBarDump->LargeChange))
    {
      TopAddress = FirstFound;
    }
    CurrentAddress = FirstFound;

    // Update memory content
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
  }

	NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ButtonDumpSetFullLimitsClick(
      TObject *Sender)
{
  mExportFromAddress = mMemoryStartAddress;
  PanelExportFrom->Caption = (PanelFormatExportFrom->Caption.IsEmpty() == false) ?
                             AnsiString::IntToHex(mExportFromAddress, 5) :
                             AnsiString(mExportFromAddress);

  mExportToAddress = mMemoryStartAddress + mMemorySize - 1;
  PanelExportTo->Caption = (PanelFormatExportTo->Caption.IsEmpty() == false) ?
                           AnsiString::IntToHex(mExportToAddress, 5) :
                           AnsiString(mExportToAddress);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ButtonDumpExportTextClick(
      TObject *Sender)
{
  SaveDialog->Title = "Export Text Dump";
  SaveDialog->Filter = TEXT_DUMP_FILTER;
  SaveDialog->InitialDir = GetCurrentDir();
  switch (TabControlMemoryContent->TabIndex)
  {
    // Banked memory
    case 0: SaveDialog->FileName = "Banked memory dump.txt"; break;
    // Lower ROM
    case 2: SaveDialog->FileName = "Lower ROM dump.txt"; break;
    // Upper ROM
    case 3: SaveDialog->FileName = "Upper ROM dump.txt"; break;
    // Entire RAM
    default: SaveDialog->FileName = "RAM dump.txt"; break;
  }

  if (SaveDialog->Execute() == true)
  {
    // Create file strings
    TStringList* FileStrings = new TStringList();

    tULong StartAddress = (mExportFromAddress < mMemoryStartAddress) ?
                          mMemoryStartAddress : mExportFromAddress;
    tULong EndAddress = (mExportToAddress >= (mMemoryStartAddress+mMemorySize)) ?
                        mMemoryStartAddress+mMemorySize-1 : mExportToAddress;

    for (tULong Address = StartAddress; Address <= EndAddress; Address += 16U)
    {
      // Get size of string
      tUChar StringSize;
      if ( (Address+16U) <= mExportToAddress)
      {
        StringSize = 16;
      }
      else
      {
        StringSize = (tUChar)(mExportToAddress - Address + 1);
      }

      // Prepare beginning of string
      AnsiString FileString = "";

      // Add address in big endian
      FileString += AnsiString::IntToHex((Address >> 16) & 0xff, 2);
      FileString += AnsiString::IntToHex((Address >> 8) & 0xff, 2);
      FileString += AnsiString::IntToHex(Address & 0xff, 2);
      FileString += ":";

      // Add Banked Memory Data
      if (TabControlMemoryContent->TabIndex == 0)
      {
        for (tUChar DataLoop=0; DataLoop < StringSize; DataLoop++)
        {
          tUChar Data = (tUChar)Engine_ReadMem(FormMain->NativeCPC, Address - mMemoryStartAddress + DataLoop);
          FileString += AnsiString::IntToHex(Data, 2);
        }
      }
      // Add Lower ROM data
      else if (TabControlMemoryContent->TabIndex == 2)
      {
        for (tUChar DataLoop=0; DataLoop < StringSize; DataLoop++)
        {
          tUChar Data = *(FormMain->NativeCPC->pbROMlo + Address - mMemoryStartAddress + DataLoop);
          FileString += AnsiString::IntToHex(Data, 2);
        }
      }
      // Add Upper ROM data
      else if (TabControlMemoryContent->TabIndex == 3)
      {
        for (tUChar DataLoop=0; DataLoop < StringSize; DataLoop++)
        {
          tUChar Data = *(FormMain->NativeCPC->pbExpansionROM + Address - mMemoryStartAddress + DataLoop);
          FileString += AnsiString::IntToHex(Data, 2);
        }
      }
      // Entire RAM
      else
      {
        for (tUChar DataLoop=0; DataLoop < StringSize; DataLoop++)
        {
          tUChar Data = *(FormMain->NativeCPC->pbRAM + Address - mMemoryStartAddress + DataLoop);
          FileString += AnsiString::IntToHex(Data, 2);
        }
      }

      FileStrings->Add(FileString);
    }

    // Write file
    FileStrings->SaveToFile(SaveDialog->FileName);

    delete FileStrings;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ButtonDumpExportBinaryClick(TObject *Sender)
{
  SaveDialog->Title = "Export Binary Dump";
  SaveDialog->Filter = BINARY_DUMP_FILTER;
  SaveDialog->InitialDir = GetCurrentDir();
  switch (TabControlMemoryContent->TabIndex)
  {
    // Banked memory
    case 0: SaveDialog->FileName = "Banked memory dump.bin"; break;
    // Lower ROM
    case 2: SaveDialog->FileName = "Lower ROM dump.bin"; break;
    // Upper ROM
    case 3: SaveDialog->FileName = "Upper ROM dump.bin"; break;
    // Entire RAM
    default: SaveDialog->FileName = "RAM dump.bin"; break;
  }

  if (SaveDialog->Execute() == true)
  {
    TFileStream* file = new TFileStream(SaveDialog->FileName, fmCreate);

    tULong StartAddress = (mExportFromAddress < mMemoryStartAddress) ?
                          mMemoryStartAddress : mExportFromAddress;
    tULong EndAddress = (mExportToAddress >= (mMemoryStartAddress+mMemorySize)) ?
                        mMemoryStartAddress+mMemorySize-1 : mExportToAddress;

    // Banked Memory Data
    if (TabControlMemoryContent->TabIndex == 0)
    {
      for (tULong Address = StartAddress; Address <= EndAddress; Address++)
      {
        tUChar Data = (tUChar)Engine_ReadMem(FormMain->NativeCPC, Address - mMemoryStartAddress);
        file->Write(&Data, 1);
      }
    }
    // Add Lower ROM data
    else if (TabControlMemoryContent->TabIndex == 2)
    {
      tUChar* DataP = FormMain->NativeCPC->pbROMlo + StartAddress - mMemoryStartAddress;
      file->Write(DataP, EndAddress - StartAddress + 1);
    }
    // Add Upper ROM data
    else if (TabControlMemoryContent->TabIndex == 3)
    {
      tUChar* DataP = FormMain->NativeCPC->pbExpansionROM + StartAddress - mMemoryStartAddress;
      file->Write(DataP, EndAddress - StartAddress + 1);
    }
    // Entire RAM
    else
    {
      tUChar* DataP = FormMain->NativeCPC->pbRAM + StartAddress - mMemoryStartAddress;
      file->Write(DataP, EndAddress - StartAddress + 1);
    }

    delete file;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::ButtonDumpImportBinaryClick(
      TObject *Sender)
{
  OpenDialog->Title = "Import Binary Dump";
  OpenDialog->Filter = BINARY_DUMP_FILTER;
  OpenDialog->InitialDir = GetCurrentDir();

  if (OpenDialog->Execute() == true)
  {
    int Handle = FileOpen(OpenDialog->FileName, fmOpenRead);
    int Length = FileSeek(Handle, 0, 2);
    FileSeek(Handle, 0, 0);

    tUChar* FileDataP = (tUChar*)malloc(Length);

    FileRead(Handle, FileDataP, Length);
    FileClose(Handle);

    tULong StartAddress = (mExportFromAddress < mMemoryStartAddress) ?
                          mMemoryStartAddress : mExportFromAddress;

    // Banked Memory Data
    if (TabControlMemoryContent->TabIndex == 0)
    {
      for (int NbData=0; NbData <= Length; NbData++)
      {
        Engine_WriteMem(FormMain->NativeCPC,
                        StartAddress - mMemoryStartAddress + NbData,
                        *(FileDataP + NbData));
      }
    }
    // Add Lower ROM data
    else if (TabControlMemoryContent->TabIndex == 2)
    {
			Application->MessageBox(_T("ROM memory cannot be written."), _T("Import binary"), MB_OK);
		}
		// Add Upper ROM data
		else if (TabControlMemoryContent->TabIndex == 3)
		{
			Application->MessageBox(_T("ROM memory cannot be written."), _T("Import binary"), MB_OK);
		}
    // Entire RAM
    else
    {
      tUChar* DataP = FormMain->NativeCPC->pbRAM + StartAddress - mMemoryStartAddress;
      for (int NbData=0; NbData <= Length; NbData++)
      {
        *(DataP + NbData) = *(FileDataP + NbData);
      }
    }

    free(FileDataP);

    // Refresh dump display
    mShowMemoryDumpRoutine(FormMain->NativeCPC);
    ActiveControl = StringGridDump;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugMemoryEditor::UpdateBanks(tNativeCPC* NativeCPC)
{
  //
  // READ
  //
  // Bank 0
  int Address = NativeCPC->membank_read[0] - NativeCPC->pbRAM;
  if (NativeCPC->membank_read[0] == NativeCPC->pbROMlo)
  {
    PanelBankRead0000->Caption = "Lower ROM";
  }
  else
  {
    PanelBankRead0000->Caption = cBankAddresses[Address >> 14];
  }
  // Bank 1
  Address = NativeCPC->membank_read[1] - NativeCPC->pbRAM;
  PanelBankRead4000->Caption = cBankAddresses[Address >> 14];
  // Bank 2
  Address = NativeCPC->membank_read[2] - NativeCPC->pbRAM;
  PanelBankRead8000->Caption = cBankAddresses[Address >> 14];
  // Bank 3
  Address = NativeCPC->membank_read[3] - NativeCPC->pbRAM;
  if (NativeCPC->membank_read[3] == NativeCPC->pbExpansionROM)
  {
    PanelBankReadC000->Caption = "Upper ROM";
  }
  else
  {
    PanelBankReadC000->Caption = cBankAddresses[Address >> 14];
  }


  //
  // WRITE
  //
  // Bank 0
  Address = NativeCPC->membank_write[0] - NativeCPC->pbRAM;
  PanelBankWrite0000->Caption = cBankAddresses[Address >> 14];
  // Bank 1
  Address = NativeCPC->membank_write[1] - NativeCPC->pbRAM;
  PanelBankWrite4000->Caption = cBankAddresses[Address >> 14];
  // Bank 2
  Address = NativeCPC->membank_write[2] - NativeCPC->pbRAM;
  PanelBankWrite8000->Caption = cBankAddresses[Address >> 14];
  // Bank 3
  Address = NativeCPC->membank_write[3] - NativeCPC->pbRAM;
  PanelBankWriteC000->Caption = cBankAddresses[Address >> 14];
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugMemoryEditor::OnCPCEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  if (mPaused != Pause)
  {
    mPaused = Pause;
    Caption = Title;
  }

  if (Condition & EC_END_INSTRUCTION)
  {
    mShowMemoryDumpRoutine(NativeCPC);
    UpdateBanks(NativeCPC);
  }
  if ( (mMonitoring == true) && (Condition & EC_FRAME_COMPLETE) )
  {
    mShowMemoryDumpRoutine(NativeCPC);
    UpdateBanks(NativeCPC);
  }

  return Pause;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugMemoryEditor::StringGridDumpDblClick(TObject *Sender)
{
#ifdef USE_EDIT_DIALOG

  if (mEditCellEnabled)
  {
    bool NextRequested;

    do
    {
      TFormDebugEditValue* EditValue = new TFormDebugEditValue(this);
      EditValue->Settings = mSettingsP;
      EditValue->CurrentValue = (tUChar)StrToInt("0x0" + mCurrentData);
      EditValue->Hexadecimal = mChangeValueHexa;
      EditValue->NbDigits = 2;
      EditValue->MinValue = 0;
      EditValue->MaxValue = 255;
      EditValue->ShowNext = true;
      EditValue->NextRequested = mChangeValueNextRequested;
      EditValue->Increment = mChangeValueIncrement;

      NextRequested = false;

      if (EditValue->ShowModal() == mrOk)
      {
        NextRequested = EditValue->NextRequested;
        mChangeValueNextRequested = NextRequested;
        mChangeValueIncrement = EditValue->Increment;
        mChangeValueHexa = EditValue->Hexadecimal;

        // Update content
        mCurrentData = AnsiString::IntToHex(EditValue->NewValue, 2);
        StringGridDump->Cells[StringGridDump->Col][StringGridDump->Row] = mCurrentData;

        // Update data
        if (mWriteDataRoutine)
        {
          mWriteDataRoutine(FormMain->NativeCPC);
        }

        // Update memory content
        mShowMemoryDumpRoutine(FormMain->NativeCPC);
      }
      delete EditValue;

      // Next requested
      if (NextRequested == true)
      {
        // Go next byte
        tULong NextAddress = mCurrentAddress + 1;
        if (NextAddress < (mMemoryStartAddress + mMemorySize))
        {
          if (NextAddress >= (mTopAddress+ScrollBarDump->LargeChange))
          {
            TopAddress += 16;
          }
          CurrentAddress = NextAddress;
        }

        // Update memory content
        mShowMemoryDumpRoutine(FormMain->NativeCPC);
      }
    }
    while (NextRequested == true);
  }

#endif /* USE_EDIT_DIALOG */

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

