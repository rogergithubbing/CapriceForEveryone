/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2017 by Frédéric Coste

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
#include "TFormPrinter.h"
#include "TFormMain.h"
//#include "TFormDebugPrinter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


#undef CREATE_PRN_FILE
//#define CREATE_PRN_FILE

#undef USE_PRINT_BUFFER
//#define USE_PRINT_BUFFER


#define PRINTER_BUFFER_SIZE                2048


static const AnsiString cTitle = "DMP2160 Printer";

//
// Country characters tables
//
const char cCountryUSA[] =
{
/* 0x20 */ ' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',  '-', '.', '/',
/* 0x30 */ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<',  '=', '>', '?',
/* 0x40 */ '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',  'M', 'N', 'O',
/* 0x50 */ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_',
/* 0x60 */ '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
/* 0x70 */ 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', ' ',
};

const char cCountryFrance[] =
{
/* 0x20 */ ' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',',  '-', '.', '/',
/* 0x30 */ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<',  '=', '>', '?',
/* 0x40 */ 'à', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',  'M', 'N', 'O',
/* 0x50 */ 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '°', 'ç', '§', '^', '_',
/* 0x60 */ '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
/* 0x70 */ 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'é', 'ù', 'è', '¨', ' ',
};


typedef struct
{
  const char* Label;
  const char* Table;
} tCountryTable;

const tCountryTable Countries[]=
{
  { "U.S.A.", cCountryUSA },
  { "FRANCE", cCountryFrance },
  // Last one
  { "", NULL },
};



#define ADD_HISTORY_ENTRY(message) \
{ \
  AnsiString HistoryEntry = ""; \
  if (CheckBoxAddTimestamp->Checked == true) \
  { \
    HistoryEntry += AnsiString(FormMain->NativeCPC->timer_from_reset / 4); \
    HistoryEntry += "  "; \
  } \
  HistoryEntry += message;\
  ListBoxHistory->Items->Add(HistoryEntry);\
}


TFormPrinter *FormPrinter;
//---------------------------------------------------------------------------
__fastcall TFormPrinter::TFormPrinter(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;

  // Add countries
  ComboBoxCountry->Clear();
  for (const tCountryTable* CountryP = Countries;
       CountryP->Table != NULL;
       CountryP++)
  {
    ComboBoxCountry->Items->Add(CountryP->Label);
  }

  ComboBoxCountry->ItemIndex = 0;
  ComboBoxCRLF->ItemIndex = 0;
  mCountryCode = Countries[ComboBoxCountry->ItemIndex].Table;

  mFileHandle = -1;

  mLastCommand = 0;
  mNbCharPrinted = 0;

  mDataBuffer = new tUChar[PRINTER_BUFFER_SIZE];

  // DMP2160 settings
  mPageWidthInInch = 8;
  mPageLengthInInch = 12; // 12 Inches for 220V
  mLineSpacing216PerInch = 36; // 1/6 Inch default line spacing
  mNbCharPerLine = 80; // Normal

  // Fill Legend
  RichEditLegend->DefAttributes->Color = clBlack;
  RichEditLegend->Text = "Legend:  ";
  RichEditLegend->SelStart = RichEditLegend->GetTextLen(); 
  RichEditLegend->SelAttributes->Color = clBlue;
  RichEditLegend->SelText = "Subscripted  ";
  RichEditLegend->SelAttributes->Color = clRed;
  RichEditLegend->SelText = "Superscripted  ";

#ifndef _DEBUG
  ButtonDebug->Visible = false;
#endif /* _DEBUG */

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::FormDestroy(TObject *Sender)
{
  delete [] mDataBuffer;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  mMonitoring = false;
  Caption = GetTitle();

  // Printer become online
  FormMain->NativeCPC->Printer->online = 1;
  ButtonOnline->Caption = "Online";
  ButtonOnlineClick(Sender);

  PageControlPrinter->ActivePage = TabSheetText;

  ImageBorderLeft->Canvas->Brush->Bitmap = ImageBorderTile->Picture->Bitmap;
  ImageBorderRight->Canvas->Brush->Bitmap = ImageBorderTile->Picture->Bitmap;

  CreateDocument();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


tPrinter* __fastcall TFormPrinter::GetPrinter(void)
{
  return FormMain->NativeCPC->Printer;
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->PrinterFormLeft != 0)
       || (mSettingsP->PrinterFormTop != 0)
       || (mSettingsP->PrinterFormWidth != 0)
       || (mSettingsP->PrinterFormHeight != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->PrinterFormLeft;
    Top = mSettingsP->PrinterFormTop;
    Width = mSettingsP->PrinterFormWidth;
    Height = mSettingsP->PrinterFormHeight;
  }

  // Apply Country code
  ComboBoxCountry->ItemIndex = mSettingsP->PrinterCountryCode;
  ComboBoxCountry->OnChange(ComboBoxCountry);
  ComboBoxCRLF->ItemIndex = mSettingsP->PrinterCRLF;

  ShowHint = mSettingsP->ShowHints;

  if (mSettingsP)
  {
    // Apply theme
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::FormResize(TObject *Sender)
{
	if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->PrinterFormWidth = Width;
    mSettingsP->PrinterFormHeight = Height;
  }

  // Redraw Printer borders
  ImageBorderLeft->Picture->Bitmap->Width = ImageBorderLeft->Width;
  ImageBorderLeft->Picture->Bitmap->Height = ImageBorderLeft->Height;
  ImageBorderLeft->Canvas->FillRect(ImageBorderLeft->Canvas->ClipRect);
  ImageBorderRight->Picture->Bitmap->Width = ImageBorderRight->Width;
  ImageBorderRight->Picture->Bitmap->Height = ImageBorderRight->Height;
  ImageBorderRight->Canvas->FillRect(ImageBorderRight->Canvas->ClipRect);

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::WMMove(TMessage &Message)
{
	if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->PrinterFormLeft = Left;
    mSettingsP->PrinterFormTop = Top;
  }

  NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ComboBoxCountryChange(TObject *Sender)
{
  mSettingsP->PrinterCountryCode = ComboBoxCountry->ItemIndex;
  mCountryCode = Countries[ComboBoxCountry->ItemIndex].Table;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ComboBoxCRLFChange(TObject *Sender)
{
  mSettingsP->PrinterCRLF = ComboBoxCRLF->ItemIndex;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormPrinter::GetTitle(void)
{
  AnsiString Title = cTitle;

  if (FormMain)
  {
    tPrinter* PrinterP = FormMain->NativeCPC->Printer;

    if (PrinterP->online)
    {
      Title += " - [Online]";
    }

    if (mMonitoring)
    {
      Title += " - [Monitoring]";
    }
  }

  return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnEmulatorEvent);
  }

  // Close file
  if (mFileHandle >= 0)
  {
    FileClose(mFileHandle);
    mFileHandle = -1;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ButtonNewClick(TObject *Sender)
{
  PageControlPrinter->ActivePage = TabSheetText;

  CreateDocument();
  
	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::CreateDocument(void)
{
  // Close pending file
  if (mFileHandle >= 0)
  {
    FileClose(mFileHandle);
    mFileHandle = -1;
  }

  ResetAttributes();

  RichEditPrinter->Clear();

  // Apply Selection attributes
  RichEditPrinter->SelText = " ";
  RichEditPrinter->SelStart = 0;
  RichEditPrinter->SelLength = 1;
  RichEditPrinter->SelAttributes->Assign(RichEditPrinter->DefAttributes);
  RichEditPrinter->SelAttributes->Size = RichEditPrinter->DefAttributes->Size;
  RichEditPrinter->SelText = " ";
  RichEditPrinter->SelStart = 0;
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ButtonOnlineClick(TObject *Sender)
{
  if (ButtonOnline->Caption == "Online")
  {
    ButtonOnline->Caption = "Offline";
		FormMain->SubscribeToCPCEvents(OnEmulatorEvent);
    FormMain->EmulatorP->SetPrinterOnline();

    // Scroll at the end of page
    RichEditPrinter->SelStart = RichEditPrinter->GetTextLen();
    RichEditPrinter->Perform(WM_VSCROLL, SB_BOTTOM, 0);
  }
  else
  {
    ButtonOnline->Caption = "Online";
    FormMain->UnsubscribeToCPCEvents(OnEmulatorEvent);
    FormMain->EmulatorP->SetPrinterOffline();

    // Close file
    if (mFileHandle >= 0)
    {
      FileClose(mFileHandle);
      mFileHandle = -1;
    }
  }

  Caption = GetTitle();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ButtonPrintClick(TObject *Sender)
{
  if (PrintDialog1->Execute() == true)
  {
    if (PageControlPrinter->ActivePage == TabSheetText)
    {
      RichEditPrinter->Print("CaPriCe Forever");
    }
  } 

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ButtonDebugClick(TObject *Sender)
{
//  FormDebugPrinter->Visible = !FormDebugPrinter->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ButtonCommandsClearClick(TObject *Sender)
{
  ListBoxHistory->Clear();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ButtonCommandsStartStopClick(
      TObject *Sender)
{
  if (ButtonCommandsStartStop->Caption == "Start")
  {
    mMonitoring = true;
    ButtonCommandsStartStop->Caption = "Stop";
    FormMain->NativeCPC->printer_event_capture = 1;
  }
  else
  {
    mMonitoring = false;
    ButtonCommandsStartStop->Caption = "Start";
    FormMain->NativeCPC->printer_event_capture = 0;
  }

  Caption = GetTitle();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


bool __fastcall TFormPrinter::OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  tPrinter* PrinterP = NativeCPC->Printer;

  if (Condition & EC_CYCLE_COUNT)
  {
    // Data to print ?
    if (PrinterP->data_counter)
    {
#ifdef CREATE_PRN_FILE
      // Add to output file
      if (mFileHandle == -1)
      {
        AnsiString Filename = ExtractFilePath(Application->ExeName) + "printer.prn";
        if (FileExists(Filename) == false)
        {
          mFileHandle = FileCreate(Filename);
        }
        else
        {
          mFileHandle = FileOpen(Filename, fmOpenWrite);
        }
      }
      if (mFileHandle >= 0)
      {
        FileWrite(mFileHandle, PrinterP->data_buffer, PrinterP->data_counter);
      }
#endif /* CREATE_PRN_FILE */

      //
      // Add to data buffer
      //
      if ( (mDataCounter+PrinterP->data_counter) > PRINTER_BUFFER_SIZE)
      {
        // Back to begin of buffer
        mDataCounter = 0;
      }
      // Copy to data buffer
      memcpy(mDataBuffer + mDataCounter,
             PrinterP->data_buffer,
             PrinterP->data_counter);
      mDataCounter += PrinterP->data_counter;
      PrinterP->data_counter = 0;

#ifndef USE_PRINT_BUFFER

      // Print
      tULong Printed = Print(mDataBuffer, mDataCounter);
      mDataCounter -= Printed;
      if (mDataCounter)
      {
        // Keep not printed commands
        memcpy(mDataBuffer,
               mDataBuffer + Printed,
               mDataCounter);
      }

#else /* USE_PRINT_BUFFER */

#endif /* USE_PRINT_BUFFER */

    }
  }

  return Pause;
}
//---------------------------------------------------------------------------


tULong __fastcall TFormPrinter::Print(tUChar* buffer, tULong Size)
{
  tULong Printed = 0;

  while (Size--)
  {
    tUChar Data = *(buffer++) & 0x7f;

    // Escape Code
    if (Data == 27)
    {
      if (Size)
      {
        tULong EscapeCodeSize = PrintEscapeCode(buffer, Size);
        if (EscapeCodeSize)
        {
          // Take ESC command into account
          buffer += EscapeCodeSize;
          Size -= EscapeCodeSize;
          Printed += EscapeCodeSize + 1; // Add ESC
        }
        else break; // Not enough data
      }
    }
    else if (Data < 32)
    {
      switch (Data)
      {
        // BS: Backspace
        case 0x08:
        {
          // Place one character before
          RichEditPrinter->SelStart--;
          AddMonitoringCharacter("BS");
          Printed++;
        }
        break;

        // HT: Horizontal Tab
        case 0x09:
        {
          AddMonitoringCharacter("HT");
          Printed++;
        }
        break;


        // CR: Carriage Return
        case 0x0D:
        {
          if (mNbCharPrinted)
          {
            // Set Caret at line begin
            TPoint CaretPos = RichEditPrinter->CaretPos;
            RichEditPrinter->SelStart -= CaretPos.x;
          }

          AddMonitoringCharacter("CR");

          // No CR+LF
          if (mSettingsP->PrinterCRLF == 0)
          {
            Printed++;
            break;
          }

          // Line Feed must be just after to perform CR+LF at the same time
        }

        // LF: Line Feed
        case 0x0A:
        {
          // Go to the end of line
          TPoint CaretPos = RichEditPrinter->CaretPos;
          int Length = RichEditPrinter->Lines->Strings[CaretPos.y].Length();
          RichEditPrinter->SelStart += Length - CaretPos.x;
          // Last line
          if ((CaretPos.y+1) == RichEditPrinter->Lines->Count)
          {
            // add line
            RichEditPrinter->SelAttributes->Assign(RichEditPrinter->DefAttributes);
            RichEditPrinter->SelAttributes->Size = RichEditPrinter->DefAttributes->Size;
            RichEditPrinter->SelAttributes->Style = RichEditPrinter->SelAttributes->Style >> fsUnderline;
            RichEditPrinter->SelText = AnsiString("\n ");
            RichEditPrinter->SelStart--;
          }
#if (__BORLANDC__ != 0x551) // Different than Borland C++ Builder 5 Update 1
          else
          {
            RichEditPrinter->SelStart++;
          }
#endif /* __BORLANDC__ */

          KeepCaretVisible();

          mEnlargedSO = false;
          UpdateAttributeSize();

          if (Data == 0x0A)
          {
            AddMonitoringCharacter("LF");
          }
          Printed++;
          mNbCharPrinted = 0;
        }
        break;

        // VT: Vertical Tab
        case 0x0B:
        {
          AddMonitoringCharacter("VT");
          Printed++;
        }
        break;

        // FF: Form Feed
        case 0x0C:
        {
          RichEditPrinter->SelAttributes->Assign(RichEditPrinter->DefAttributes);
          RichEditPrinter->SelAttributes->Size = RichEditPrinter->DefAttributes->Size;

          TPoint CaretPos = RichEditPrinter->CaretPos;
          int NbLinesPerPage = mPageLengthInInch * 216 / mLineSpacing216PerInch;
          int TopNextPage = ((CaretPos.y / NbLinesPerPage) + 1) * NbLinesPerPage;

          // Add line until next page
          while (CaretPos.y < TopNextPage)
          {
            RichEditPrinter->SelText = AnsiString((char)0x0A);
            CaretPos = RichEditPrinter->CaretPos;
          }
          KeepCaretVisible();
          AddMonitoringCharacter("FF");
          Printed++;
        }
        break;

        // SO: Shift Out (condensed mode)
        case 0x0E:
        {
          mEnlargedSO = true;
          AddMonitoringCharacter("SO");
          Printed++;
        }
        break;

        // SI: Shift In (condensed mode)
        case 0x0F:
        {
          mCondensed = true;
          UpdateAttributeSize();
          AddMonitoringCharacter("SI");
          Printed++;
        }
        break;

        // DC2: Cancel condensed mode
        case 0x12:
        {
          mCondensed = false;
          UpdateAttributeStyle();
          UpdateAttributeSize();
          AddMonitoringCharacter("DC2");
          Printed++;
        }
        break;

        // DC4: Cancel enlarged mode
        case 0x16:
        {
          mEnlargedSO = false;
          UpdateAttributeSize();
          AddMonitoringCharacter("DC4");
          Printed++;
        }
        break;

        // CAN: Cancel
        case 24:
        {
          AddMonitoringCharacter("CAN");
          Printed++;
        }
        break;

        // DEL: Delete
        case 0x7F:
        {
          TPoint CaretPos = RichEditPrinter->CaretPos;
          // Existing character ?
          if (CaretPos.x)
          {
            // Replace last character by space
            RichEditPrinter->SelStart--;
            RichEditPrinter->SelLength = 1;
            RichEditPrinter->SelAttributes->Assign(RichEditPrinter->DefAttributes);
            RichEditPrinter->SelAttributes->Style = RichEditPrinter->SelAttributes->Style >> fsUnderline;
            RichEditPrinter->SelAttributes->Size = RichEditPrinter->DefAttributes->Size;
            RichEditPrinter->SelText = " ";
          }
          AddMonitoringCharacter("DEL");
          Printed++;
        }
        break;

        default:
        {
          AddMonitoringMissingCharacter(Data);
          Printed++;
        }
      }
    }
    // Space = no print
    else if (Data == ' ')
    {
      // Already a character ?
      TPoint CaretPos = RichEditPrinter->CaretPos;
      int TextLength = RichEditPrinter->Lines->Strings[CaretPos.y].Length();
#if (__BORLANDC__ == 0x551) // Borland C++ Builder 5 Update 1
      // Not last line
      if (CaretPos.y < RichEditPrinter->Lines->Count)
      {
        TextLength--; // Remove LF character
      }
#endif /* __BORLANDC__ */
      if (CaretPos.x < TextLength)
      {
        // Select next char
        RichEditPrinter->SelStart++;
      }
      else
      {
        // Add space
        RichEditPrinter->SelAttributes->Assign(RichEditPrinter->DefAttributes);
        RichEditPrinter->SelAttributes->Size = RichEditPrinter->DefAttributes->Size;
        RichEditPrinter->SelText = AnsiString(" ");
      }
      AddMonitoringCharacter("SPACE");
      Printed++;
    }
    else
    {
      // Already a character ?
      TPoint CaretPos = RichEditPrinter->CaretPos;
      int TextLength = RichEditPrinter->Lines->Strings[CaretPos.y].Length();
      if (CaretPos.x < TextLength)
      {
        // Select next char
        RichEditPrinter->SelLength = 1;
      }
      RichEditPrinter->SelAttributes->Assign(RichEditPrinter->DefAttributes);
      RichEditPrinter->SelAttributes->Size = RichEditPrinter->DefAttributes->Size;
      // End of line ?
      if (CaretPos.x == mNbCharPerLine)
      {
        // Add space for word wrapping
        RichEditPrinter->SelText = AnsiString(" ");
      }
      // Add printable character
      char Code = mCountryCode[Data - 0x20];
      RichEditPrinter->SelText = AnsiString(Code);
      AddMonitoringCharacter(Data);
      Printed++;
      mNbCharPrinted++;
    }

    // Keep last command
    mLastCommand = Data;
  }

  return (Printed);
}
//---------------------------------------------------------------------------


tULong __fastcall TFormPrinter::PrintEscapeCode(tUChar* buffer, tULong Size)
{
  tULong EscapeCodeSize = 0;
  tUChar* buffer_start = buffer;
  bool IllegalCommand = false;
  bool MissingCommand = false;

  tUChar ESCCode = *(buffer++);
  switch (ESCCode)
  {
    // ESC - 1 : Select underlining
    // ESC - 0 : Cancel underlining
    case '-':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        if ( (Data == 1) || (Data == '1') )
        {
          // Add Underline
          mUnderline = true;
          UpdateAttributeStyle();
        }
        else if ( (Data == 0) || (Data == '0') )
        {
          // Remove Underline
          mUnderline = false;
          UpdateAttributeStyle();
        }
        else
        {
          IllegalCommand = true;
        }
      }
    }
    break;

    // ESC ! XX : Print mode selection
    case '!':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        
        mEnlarged = (Data & 0x20) ? true : false;
        mDoubleStrike = (Data & 0x10) ? true : false;
        mEmphasized = (Data & 0x08) ? true : false;
        mCondensed = (Data & 0x04) ? true : false;
        mElite = (Data & 0x01) ? true : false;

        mEnlargedSO = false;

        UpdateAttributeStyle();
        UpdateAttributeSize();
      }
    }
    break;

    // ESC 0 : Select 1/8 line spacing
    case '0':
    {
      mLineSpacing216PerInch = 27;
      
      // Command taken into account
      EscapeCodeSize = 1;
    }
    break;

    // ESC 1 : Select 7/72 line spacing
    case '1':
    {
      mLineSpacing216PerInch = 21;
      
      // Command taken into account
      EscapeCodeSize = 1;
    }
    break;

    // ESC 2 : Select 1/6 line spacing
    case '2':
    {
      mLineSpacing216PerInch = 36;
      
      // Command taken into account
      EscapeCodeSize = 1;
    }
    break;

    // ESC 4 : Select italic mode
    // ESC 5 : Cancel italic mode
    case '4':
    case '5':
    {
      // Command taken into account
      EscapeCodeSize = 1;

      mItalic = (ESCCode == '4') ? true : false;
      
      UpdateAttributeStyle();
    }
    break;

    // ESC E : Select emphasized mode
    // ESC F : Cancel emphasized mode
    case 'E':
    case 'F':
    {
      // Command taken into account
      EscapeCodeSize = 1;

      mEmphasized = (ESCCode == 'E') ? true : false;

      UpdateAttributeStyle();
    }
    break;

    // ESC G : Select double strike
    // ESC H : Cancel double strike
    case 'G':
    case 'H':
    {
      // Command taken into account
      EscapeCodeSize = 1;

      mDoubleStrike = (ESCCode == 'G') ? true : false;

      UpdateAttributeColor();
    }
    break;

    // ESC i 1 : Select Incremental and view print
    // ESC i 0 : Cancel Incremental and view print
    case 'i':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        if ( (Data == 1) || (Data == '1') )
        {
          mIncrementalMode = true;
        }
        else if ( (Data == 0) || (Data == '0') )
        {
          mIncrementalMode = false;
        }
        else
        {
          IllegalCommand = true;
        }
      }
    }
    break;

    // ESC J XX : feed
    // ESC j XX : Reverse feed
    case 'J':
    case 'j':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        TPoint CaretPos = RichEditPrinter->CaretPos;

        // Reverse feed
        if (ESCCode == 'j')
        {
          // Do not reverse more than 1/2 Inch
          if (Data > 108) Data = 108;

          int NbLines = Data / mLineSpacing216PerInch;
          int NbLinesPerPage = mPageLengthInInch * 216 / mLineSpacing216PerInch;

          int TargetLine = CaretPos.y - NbLines;
          int TopPage = (CaretPos.y / NbLinesPerPage) * NbLinesPerPage;
          // Above top of page ?
          if (TargetLine < TopPage) TargetLine = TopPage;

          // Back to begin of line
          RichEditPrinter->SelStart -= CaretPos.x;
          while (CaretPos.y > TargetLine)
          {
            // Go to last line
            RichEditPrinter->SelStart--;
            CaretPos = RichEditPrinter->CaretPos;
            RichEditPrinter->SelStart -= CaretPos.x;
          }

          KeepCaretVisible();
        }
        // Feed
        else
        {
          MissingCommand = true;
          AddMonitoringMissingEscapeCode(ESCCode);
        }
      }
    }
    break;

    // ESC p 1 : Select Proportional spacing
    // ESC p 0 : Cancel Proportional spacing
    case 'p':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        if ( (Data == 1) || (Data == '1') )
        {
          mProportionalSpacing = true;
          UpdateAttributePitch();
        }
        else if ( (Data == 0) || (Data == '0') )
        {
          mProportionalSpacing = false;
          UpdateAttributePitch();
        }
        else
        {
          IllegalCommand = true;
        }
      }
    }
    break;

    // ESC S 1 : Select subscript mode
    // ESC S 0 : Select superscript mode
    case 'S':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        if ( (Data == 1) || (Data == '1') )
        {
          mSubscriptMode = true;
          UpdateAttributeColor();
          UpdateAttributeSize();
        }
        else if ( (Data == 0) || (Data == '0') )
        {
          mSuperscriptMode = true;
          UpdateAttributeColor();
          UpdateAttributeSize();
        }
        else
        {
          IllegalCommand = true;
        }
      }
    }
    break;

    // ESC T : Cancel Superscript and SubScript mode
    case 'T':
    {
      // Command taken into account
      EscapeCodeSize = 1;

      mSuperscriptMode = false;
      mSubscriptMode = false;

      UpdateAttributeColor();
      UpdateAttributeSize();
    }
    break;

    // ESC W 1 : Select enlarged
    // ESC W 0 : Cancel enlarged
    case 'W':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        if ( (Data == 1) || (Data == '1') )
        {
          mEnlarged = true;
          mEnlargedSO = false;
          UpdateAttributeSize();
        }
        else if ( (Data == 0) || (Data == '0') )
        {
          mEnlarged = false;
          mEnlargedSO = false;
          UpdateAttributeSize();
        }
        else
        {
          IllegalCommand = true;
        }
      }
    }
    break;

    // ESC x 1 : Select NLQ
    // ESC x 0 : Cancel NLQ
    case 'x':
    {
      if (Size >= 2)
      {
        // Command taken into account
        EscapeCodeSize = 2;

        tUChar Data = *buffer;
        if ( (Data == 1) || (Data == '1') )
        {
          mNLQ = true;
        }
        else if ( (Data == 0) || (Data == '0') )
        {
          mNLQ = false;
        }
        else
        {
          IllegalCommand = true;
        }
      }
    }
    break;

    default:
    {
      MissingCommand = true;
      // Command taken into account
      EscapeCodeSize = 1;
      AddMonitoringMissingEscapeCode(ESCCode);
    }
  }

  if ( (EscapeCodeSize) && (MissingCommand == false) )
  {
    AddMonitoringEscapeCode(buffer_start, EscapeCodeSize, IllegalCommand);
  }

  return (EscapeCodeSize);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::KeepCaretVisible(void)
{
  // Get client position
  TPoint ClientPos;
  bool Visible = false;
  do
  {
    RichEditPrinter->Perform(EM_POSFROMCHAR, (WPARAM)&ClientPos, (LPARAM)RichEditPrinter->SelStart);
    if (ClientPos.y < 0)
    {
      RichEditPrinter->Perform(EM_SCROLL, SB_LINEUP, 0);
    }
    else if (ClientPos.y > RichEditPrinter->Height)
    {
      RichEditPrinter->Perform(EM_SCROLL, SB_LINEDOWN, 0);
    }
    else
    {
      Visible = true;
    }
  }
  while (Visible == false);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::ResetAttributes(void)
{
  // Init printer flags
  mEnlarged = false;
  mEnlargedSO = false;
  mDoubleStrike = false;
  mEmphasized = false;
  mCondensed = false;
  mElite = false;
  mDoubleWidth = false;
  mItalic = false;
  mNLQ = false;
  mSuperscriptMode = false;
  mSubscriptMode = false;
  mIncrementalMode = false;
  mProportionalSpacing = false;
  mUnderline = false;

  UpdateAttributeColor();
  UpdateAttributeStyle();
  UpdateAttributeSize();
  UpdateAttributePitch();
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::UpdateAttributeColor(void)
{
  TColor Color = clGray;

  if (mDoubleStrike == true) Color = clBlack;
  else if (mSubscriptMode == true) Color = clBlue;
  else if (mSuperscriptMode == true) Color = clRed;

  RichEditPrinter->DefAttributes->Color = Color;
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::UpdateAttributeStyle(void)
{
  TFontStyles Style = TFontStyles();

  if (mUnderline == true)
  {
    Style = Style << fsUnderline;
  }

  if (mItalic == true)
  {
    Style = Style << fsItalic;
  }

  if (mEmphasized == true)
  {
    Style = Style << fsBold;
  }

  RichEditPrinter->DefAttributes->Style = Style;
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::UpdateAttributeSize(void)
{
  int Size = 10; // Normal

  // Enlarged
  if ( (mEnlarged == true) || (mEnlargedSO == true) )
  {
    Size = 12;
  }
  else if (mCondensed == true)
  {
    Size = 8;
  }
  else if ( (mSubscriptMode == true) || (mSuperscriptMode == true) )
  {
    Size = 6;
  }

  RichEditPrinter->DefAttributes->Size = Size;
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::UpdateAttributePitch(void)
{
#if (__BORLANDC__ == 0x551) // Borland C++ Builder 5 Update 1
  RichEditPrinter->DefAttributes->Pitch = (mProportionalSpacing == true) ? fpVariable	: fpFixed;
#else /* __BORLANDC__ */
  RichEditPrinter->DefAttributes->Pitch = (mProportionalSpacing == true) ? TFontPitch::fpVariable	: TFontPitch::fpFixed;
#endif /* __BORLANDC__ */
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::AddMonitoringCharacter(char Command)
{
  if (mMonitoring == false) return;

  AnsiString Entry = "'" + AnsiString(Command) + "' (&" + AnsiString::IntToHex((tUChar)Command, 2) + ")";
  ListBoxHistory->Items->Add(Entry);
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::AddMonitoringCharacter(AnsiString Command)
{
  if (mMonitoring == false) return;

  ListBoxHistory->Items->Add(Command);
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::AddMonitoringMissingCharacter(char Command)
{
  if (mMonitoring == false) return;

  AnsiString Entry = ">> MISSING or ILLEGAL '" + AnsiString(Command) + "' (&" + AnsiString::IntToHex(Command, 2) + ")";
  ListBoxHistory->Items->Add(Entry);
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::AddMonitoringEscapeCode(tUChar* buffer, int Size, bool Illegal)
{
  if (mMonitoring == false) return;

  AnsiString Entry = "";
  if (Illegal == true)
  {
    Entry += ">> ILLEGAL ";
  }
  Entry += "ESC " + AnsiString((char)*buffer) + " (&" + AnsiString::IntToHex(*buffer, 2) + ")";

  buffer++;
  for (int Loop=0; Loop < (Size-1); Loop++)
  {
    Entry += " &" + AnsiString::IntToHex(*buffer++, 2);
  }

  ListBoxHistory->Items->Add(Entry);
}
//---------------------------------------------------------------------------
void __fastcall TFormPrinter::AddMonitoringMissingEscapeCode(char Command)
{
  if (mMonitoring == false) return;

  AnsiString Entry = ">> MISSING or ILLEGAL ESC '" + AnsiString(Command) + "' (&" + AnsiString::IntToHex(Command, 2) + ")";
  ListBoxHistory->Items->Add(Entry);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::Copytoclipboard1Click(TObject *Sender)
{
  TStringList* pBuffer = new TStringList();

  for (int Index=0; Index < ListBoxHistory->Items->Count; Index++)
  {
    pBuffer->Add(ListBoxHistory->Items->Strings[Index]);
  }

  Clipboard()->AsText = pBuffer->Text;

  delete pBuffer;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::RichEditPrinterSelectionChange(TObject *Sender)
{
#ifdef _DEBUG

  AnsiString Status = "Status: ";

  TPoint CaretPos = RichEditPrinter->CaretPos;
  Status += "Pos X,Y= " + AnsiString(CaretPos.x) + "," + AnsiString(CaretPos.y);

  Status += "  LineLength= " + AnsiString(RichEditPrinter->Lines->Strings[CaretPos.y].Length());

  Status += "  NbLines= " + AnsiString(RichEditPrinter->Lines->Count);

//  if (FormDebugPrinter->Visible)
//  {
//	FormDebugPrinter->Status = Status;
//  }

#endif /* _DEBUG */

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormPrinter::SetTextFocus(void)
{
#ifdef _DEBUG

  if (RichEditPrinter->CanFocus() == true)
  {
    RichEditPrinter->SetFocus();
  }

#endif /* _DEBUG */
}
//---------------------------------------------------------------------------

void __fastcall TFormPrinter::PageControlPrinterDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = PageControlPrinter->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }

  // Get sheet corresponding to TabIndex
  int TabCount=0;
  TTabSheet* Sheet = NULL;
  for (int Loop=0; Loop < PageControlPrinter->PageCount; Loop++)
  {
    if (PageControlPrinter->Pages[Loop]->TabVisible == true)
    {
      if (TabCount == TabIndex)
      {
        Sheet = PageControlPrinter->Pages[Loop];
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
  int TextPosY = (PageControlPrinter->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

  NOT_USED(Control);
}
//---------------------------------------------------------------------------

