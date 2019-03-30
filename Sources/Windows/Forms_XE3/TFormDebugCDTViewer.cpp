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

#include "TFormDebugCDTViewer.h"
#include "TFormMain.h"
#include "..\..\Common\CRC_CCITT.h"
#include "..\..\Common\ASCII.h"
#include "..\..\Engine\Tape_CDT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

#undef SHOW_ASCII
#define SHOW_ASCII

//=================================


#define DURATION_MN_S_FORMAT             "%.2dmn %.2ds"


static const AnsiString WindowTitle = "CDT Tape viewer";


#define COLOR_BLOCK_TYPE_10              TColor(RGB(0xCA,0xCE,0xFF))
#define COLOR_BLOCK_TYPE_11              TColor(RGB(0xBD,0xFF,0xD6))
#define COLOR_BLOCK_TYPE_12              TColor(RGB(0x5A,0xF7,0xCE))
#define COLOR_BLOCK_TYPE_13              TColor(RGB(0x5A,0xF3,0xF7))
#define COLOR_BLOCK_TYPE_14              TColor(RGB(0xC6,0xF7,0x5A))
#define COLOR_BLOCK_TYPE_15              TColor(RGB(0x00,0xC5,0x4B))
#define COLOR_BLOCK_TYPE_18              TColor(RGB(0xE0,0xA8,0x78))
#define COLOR_BLOCK_TYPE_20              TColor(RGB(0xFF,0xFF,0xBD))
#define COLOR_BLOCK_TYPE_21              TColor(RGB(0xF7,0xB2,0x5A))
#define COLOR_BLOCK_TYPE_22              TColor(RGB(0xF7,0xB2,0x5A))
#define COLOR_BLOCK_TYPE_2B              TColor(RGB(0xA0,0xCC,0xA0))
#define COLOR_BLOCK_TYPE_30              TColor(RGB(0xFF,0xFF,0xFF))
#define COLOR_BLOCK_TYPE_31              TColor(RGB(0xF6,0xDD,0xFD))
#define COLOR_BLOCK_TYPE_32              TColor(RGB(0xFF,0x80,0x80))
#define COLOR_BLOCK_TYPE_33              TColor(RGB(0xFF,0xFF,0x80))


const AnsiString BlockType10FullLabel = "STANDARD SPEED DATA";
const AnsiString BlockType10ShortLabel = "STD DATA";
const AnsiString BlockType11FullLabel = "TURBO LOADING DATA";
const AnsiString BlockType11ShortLabel = "TURBO DATA";
const AnsiString BlockType12FullLabel = "PURE TONE";
const AnsiString BlockType12ShortLabel = "PURE TONE";
const AnsiString BlockType13FullLabel = "SEQUENCE OF PULSES OF DIFFERENT LENGTHS";
const AnsiString BlockType13ShortLabel = "PULSES";
const AnsiString BlockType14FullLabel = "PURE DATA BLOCK";
const AnsiString BlockType14ShortLabel = "PURE DATA";
const AnsiString BlockType15FullLabel = "DIRECT RECORDING";
const AnsiString BlockType15ShortLabel = "DIRECT REC";
const AnsiString BlockType18FullLabel = "CSW RECORDING";
const AnsiString BlockType18ShortLabel = "CSW REC";
const AnsiString BlockType20FullLabel = "PAUSE";
const AnsiString BlockType20ShortLabel = "PAUSE";
const AnsiString BlockType21FullLabel = "GROUP START";
const AnsiString BlockType21ShortLabel = "GROUP=";
const AnsiString BlockType22FullLabel = "GROUP END";
const AnsiString BlockType22ShortLabel = "GROUP END";
const AnsiString BlockType2BFullLabel = "SET SIGNAL LEVEL";
const AnsiString BlockType2BShortLabel = "SIGNAL LVL";
const AnsiString BlockType30FullLabel = "TEXT DESCRIPTION";
const AnsiString BlockType30ShortLabel = "TEXT=";
const AnsiString BlockType31FullLabel = "MESSAGE BLOCK";
const AnsiString BlockType31ShortLabel = "MESSAGE";
const AnsiString BlockType32FullLabel = "ARCHIVE INFO";
const AnsiString BlockType32ShortLabel = "ARCHIVE INFO";
const AnsiString BlockType33FullLabel = "HARDWARE TYPE";
const AnsiString BlockType33ShortLabel = "HARDWARE TYPE";

const AnsiString HardwareValue[] =
{
  /* 00 */ "The tape RUNS on this machine or with this hardware, "\
           "but may or may not use the hardware or "\
           "special features of the machine.",
  /* 01 */ "The tape USES the hardware or special features of the machine, "\
           "such as extra memory or a sound chip.",
  /* 02 */ "The tape RUNS but it DOESN'T use the hardware or "\
           "special features of the machine.",
  /* 03 */ "The tape DOESN'T RUN on this machine or with this hardware...",
};

const AnsiString HardwareTypeLabel[] =
{
  /* 00 */ "Computers",
  /* 01 */ "External Storage",
  /* 02 */ "ROM/RAM type add-ons",
  /* 03 */ "Sound devices",
  /* 04 */ "Joysticks",
  /* 05 */ "Mice",
  /* 06 */ "Other controllers",
  /* 07 */ "Serial Ports",
  /* 08 */ "Parallel ports",
  /* 09 */ "Printers",
  /* 0A */ "Modems",
  /* 0B */ "Digitisers",
  /* 0C */ "Network adapters",
  /* 0D */ "Keyboards & keypads",
  /* 0E */ "AD/DA converters",
  /* 0F */ "EPROM Programmers",
  /* 10 */ "Graphics",
};

const AnsiString ComputersLabel[] =
{
  /* 00 */ "ZX Spectrum 16k",
  /* 01 */ "ZX Spectrum 48k, Plus",
  /* 02 */ "ZX Spectrum 48k ISSUE 1",
  /* 03 */ "ZX Spectrum 128k (Sinclair)",
  /* 04 */ "ZX Spectrum 128k +2 (Grey case)",
  /* 05 */ "ZX Spectrum 128k +2A, +3",
  /* 06 */ "Timex Sinclair TC-2048",
  /* 07 */ "Timex Sinclair TS-2068",
  /* 08 */ "Pentagon 128",
  /* 09 */ "Sam Coupe",
  /* 0A */ "Didaktik M",
  /* 0B */ "Didaktik Gama",
  /* 0C */ "ZX-81 with 1k RAM",
  /* 0D */ "ZX-81 with 16k RAM or more",
  /* 0E */ "ZX Spectrum 128k, Spanish version",
  /* 0F */ "ZX Spectrum, Arabic version",
  /* 10 */ "TK 90-X",
  /* 11 */ "TK 95",
  /* 12 */ "Byte",
  /* 13 */ "Elwro",
  /* 14 */ "ZS Scorpion",
  /* 15 */ "Amstrad CPC 464",
  /* 16 */ "Amstrad CPC 664",
  /* 17 */ "Amstrad CPC 6128",
  /* 18 */ "Amstrad CPC 464+",
  /* 19 */ "Amstrad CPC 6128+",
  /* 1A */ "Jupiter ACE",
  /* 1B */ "Enterprise",
  /* 1C */ "Commodore 64",
  /* 1D */ "Commodore 128",
  /* 1E */ "Inves Spectrum+",
  /* 1F */ "Profi",
  /* 20 */ "GrandRomMax",
  /* 21 */ "Kay 1024",
  /* 22 */ "Ice Felix HC 91",
  /* 23 */ "Ice Felix HC 2000",
  /* 24 */ "Amaterske RADIO Mistrum",
  /* 25 */ "Quorum 128",
  /* 26 */ "MicroART ATM",
  /* 27 */ "MicroART ATM Turbo 2",
  /* 28 */ "Chrome",
  /* 29 */ "ZX Badaloc",
  /* 2A */ "TS-1500",
  /* 2B */ "Lambda",
  /* 2C */ "TK-65",
  /* 2D */ "ZX-97",
};

const AnsiString ExternalStorageLabel[] =
{
  /* 00 */ "Microdrive",
  /* 01 */ "Opus Discovery",
  /* 02 */ "Disciple",
  /* 03 */ "Plus-D",
  /* 04 */ "Rotronics Wafadrive",
  /* 05 */ "TR-DOS (BetaDisk)",
  /* 06 */ "Byte Drive",
  /* 07 */ "Watsford",
  /* 08 */ "FIZ",
  /* 09 */ "Radofin",
  /* 0A */ "Didaktik disk drives",
  /* 0B */ "BS-DOS (MB-02)",
  /* 0C */ "ZX Spectrum +3 disk drive",
  /* 0D */ "JLO (Oliger) disk interface",
  /* 0E */ "FDD3000",
  /* 0F */ "Zebra disk drive",
  /* 10 */ "Ramex Millenia",
  /* 11 */ "Larken",
  /* 12 */ "Kempston disk interface",
  /* 13 */ "Sandy",
  /* 14 */ "ZX Spectrum +3e hard disk",
  /* 15 */ "ZXATASP",
  /* 16 */ "DivIDE",
  /* 17 */ "ZXCF",
};

const AnsiString ROMRAMLabel[] =
{
  /* 00 */ "Sam Ram",
  /* 01 */ "Multiface",
  /* 02 */ "Multiface 128k",
  /* 03 */ "Multiface +3",
  /* 04 */ "MultiPrint",
  /* 05 */ "MB-02 ROM/RAM expansion",
  /* 06 */ "SoftROM",
  /* 07 */ "1k",
  /* 08 */ "16k",
  /* 09 */ "48k",
  /* 0A */ "Memory in 8-16k used",
};

const AnsiString SoundDevicesLabel[] =
{
  /* 00 */ "Classic AY hardware (compatible with 128k ZXs)",
  /* 01 */ "Fuller Box AY sound hardware",
  /* 02 */ "Currah microSpeech",
  /* 03 */ "SpecDrum",
  /* 04 */ "AY ACB stereo (A+C=left, B+C=right); Melodik",
  /* 05 */ "AY ABC stereo (A+B=left, B+C=right)",
  /* 06 */ "RAM Music Machine",
  /* 07 */ "Covox",
  /* 08 */ "General Sound",
  /* 09 */ "Intec Electronics Digital Interface B8001",
  /* 0A */ "Zon-X AY",
  /* 0B */ "QuickSilva AY",
  /* 0C */ "Jupiter ACE",
};

const AnsiString JoystickLabel[] =
{
  /* 00 */ "Kempston",
  /* 01 */ "Cursor, Protek, AGF",
  /* 02 */ "Sinclair 2 Left (12345)",
  /* 03 */ "Sinclair 1 Right (67890)",
  /* 04 */ "Fuller",
};

const AnsiString MouseLabel[] =
{
  /* 00 */ "AMX mouse",
  /* 01 */ "Kempston mouse",
};

const AnsiString OtherControllersLabel[] =
{
  /* 00 */ "Trickstick",
  /* 01 */ "ZX Light Gun",
  /* 02 */ "Zebra Graphics Tablet",
  /* 03 */ "Defender Light Gun",
};

const AnsiString SerialPortsLabel[] =
{
  /* 00 */ "ZX Interface 1",
  /* 01 */ "ZX Spectrum 128k",
};

const AnsiString ParallelPortsLabel[] =
{
  /* 00 */ "Kempston S",
  /* 01 */ "Kempston E",
  /* 02 */ "ZX Spectrum +3",
  /* 03 */ "Tasman",
  /* 04 */ "DK'Tronics",
  /* 05 */ "Hilderbay",
  /* 06 */ "INES Printerface",
  /* 07 */ "ZX LPrint Interface 3",
  /* 08 */ "MultiPrint",
  /* 09 */ "Opus Discovery",
  /* 0A */ "Standard 8255 chip with ports 31,63,95",
};

const AnsiString PrintersLabel[] =
{
  /* 00 */ "ZX Printer, Alphacom 32 & compatibles",
  /* 01 */ "Generic Printer",
  /* 02 */ "EPSON Compatible",
};

const AnsiString ModemsLabel[] =
{
  /* 00 */ "VTX 5000",
  /* 01 */ "T/S 2050 or Westridge 2050",
};

const AnsiString DigitisersLabel[] =
{
  /* 00 */ "RD Digital Tracer",
  /* 01 */ "DK'Tronics Light Pen",
  /* 02 */ "British MicroGraph Pad",
  /* 03 */ "Romantic Robot Videoface",
};

const AnsiString NetworkAdaptersLabel[] =
{
  /* 00 */ "ZX Interface 1",
};

const AnsiString KeyboardsKeypadsLabel[] =
{
  /* 00 */ "Keypad for ZX Spectrum 128k",
};

const AnsiString ADDAConvertersLabel[] =
{
  /* 00 */ "Harley Systems ADC 8.2",
  /* 01 */ "Blackboard Electronics",
};

const AnsiString EPROMProgrammersLabel[] =
{
  /* 00 */ "Orme Electronics",
};

const AnsiString GraphicsLabel[] =
{
  /* 00 */ "WRX Hi-Res",
  /* 01 */ "G007",
  /* 02 */ "Memotech",
  /* 03 */ "Lambda Colour",
};

const AnsiString* HardwareIDList[] =
{
  /* 00 */ ComputersLabel,
  /* 01 */ ExternalStorageLabel,
  /* 02 */ ROMRAMLabel,
  /* 03 */ SoundDevicesLabel,
  /* 04 */ JoystickLabel,
  /* 05 */ MouseLabel,
  /* 06 */ OtherControllersLabel,
  /* 07 */ SerialPortsLabel,
  /* 08 */ ParallelPortsLabel,
  /* 09 */ PrintersLabel,
  /* 0A */ ModemsLabel,
  /* 0B */ DigitisersLabel,
  /* 0C */ NetworkAdaptersLabel,
  /* 0D */ KeyboardsKeypadsLabel,
  /* 0E */ ADDAConvertersLabel,
  /* 0F */ EPROMProgrammersLabel,
  /* 10 */ GraphicsLabel,
};


const AnsiString ArchiveIDLabel[] =
{
  /* 00 */ "Full Title",
  /* 01 */ "Software House / Publisher",
  /* 02 */ "Author(s)",
  /* 03 */ "Year of Publication",
  /* 04 */ "Language",
  /* 05 */ "Game/Utility Type",
  /* 06 */ "Price",
  /* 07 */ "Protection Scheme / Loader",
  /* 08 */ "Origin",
};
const AnsiString ArchiveCommentLabel = "Comment(s)";



TFormDebugCDTViewer *FormDebugCDTViewer;
//---------------------------------------------------------------------------
__fastcall TFormDebugCDTViewer::TFormDebugCDTViewer(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mEmulatorP = NULL;

  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  PanelBlockType->Font->Color = clBlack;
  PanelTape->Width = 520;
  StringGridBlocks->Font->Color = clBlack;
  EditGoToBlock->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  Caption = WindowTitle;

  OnCDTEvent(FormMain->NativeCPC->Tape);

  ButtonGoToBlock->Height = EditGoToBlock->Height;

  // Subscribe to step by step event
  FormMain->SubscribeToCPCEvents(OnStepByStepEvent);
  FormMain->SubscribeToTapeEvents(OnCDTEvent);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnStepByStepEvent);
    FormMain->UnsubscribeToTapeEvents(OnCDTEvent);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->CDTViewerFormLeft != 0)
       || (mSettingsP->CDTViewerFormTop != 0)
       || (mSettingsP->CDTViewerFormWidth != 0)
       || (mSettingsP->CDTViewerFormHeight != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->CDTViewerFormLeft;
    Top = mSettingsP->CDTViewerFormTop;
    Width = mSettingsP->CDTViewerFormWidth;
    Height = mSettingsP->CDTViewerFormHeight;
  }

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  MemoBlockInfo->Font->Color = mSettingsP->ThemeText2Color;
  MemoBlockData->Font->Color = mSettingsP->ThemeText2Color;
  MemoHeaderRecord->Font->Color = mSettingsP->ThemeText2Color;
  LabelBlocks->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->CDTViewerFormWidth = Width;
    mSettingsP->CDTViewerFormHeight = Height;

    StringGridBlocks->ColWidths[1] = StringGridBlocks->Width - StringGridBlocks->ColWidths[0];
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->CDTViewerFormLeft = Left;
    mSettingsP->CDTViewerFormTop = Top;
  }

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::StringGridBlocksDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
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

  // Draw played block
  if (ACol == 0)
  {
    StringGridBlocks->Canvas->Brush->Color = clWhite;
    StringGridBlocks->Canvas->FillRect(CellRect);

    // Draw "Play" triangle
    if (ARow == (int)FormMain->NativeCPC->Tape->current_block)
    {
      StringGridBlocks->Canvas->Pen->Color = clBlack;
			StringGridBlocks->Canvas->Brush->Color = clBlack;
#if (__BORLANDC__ == 0x551)
			Windows::TPoint points[3];
#else /* __BORLANDC__ */
			TPoint points[3];
#endif /* __BORLANDC__ */
			points[0].x = CellRect.Left + 10;
      points[0].y = CellRect.Top;
      points[1].x = CellRect.Left + 30;
      points[1].y = CellRect.Top + (CellRect.Height() / 2);
      points[2].x = CellRect.Left + 10;
      points[2].y = CellRect.Bottom;
      StringGridBlocks->Canvas->Polygon(points, 2);
    }

    // Draw "000" counter
    if (ARow == (int)FormMain->NativeCPC->Tape->counter000_block)
    {
      StringGridBlocks->Canvas->Font->Color = clRed;
      StringGridBlocks->Canvas->Brush->Style = bsClear;

      String CellText = "000";
      int TextWidth = StringGridBlocks->Canvas->TextWidth(CellText);
      int TextPosH = (StringGridBlocks->ColWidths[ACol] - TextWidth) / 2;
      int TextHeight = StringGridBlocks->Canvas->TextHeight(CellText);
      int TextPosV = (StringGridBlocks->RowHeights[ARow] - TextHeight) / 2;
	  StringGridBlocks->Canvas->TextOut(CellRect.Left+TextPosH-1,
										CellRect.Top+TextPosV,
										CellText);
    }
  }
  else
  {
    // Selected cell ?
    if (State.Contains(gdSelected))
    {
      StringGridBlocks->Canvas->Brush->Color = clHighlight;
    }
    else
    {
      tTapeBlock TapeBlock = FormMain->NativeCPC->Tape->blocks[ARow];
      switch (TapeBlock.block_id)
      {
        case 0x10: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_10; break;
        case 0x11: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_11; break;
        case 0x12: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_12; break;
        case 0x13: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_13; break;
        case 0x14: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_14; break;
        case 0x15: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_15; break;
        case 0x18: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_18; break;
        case 0x20: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_20; break;
        case 0x21: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_21; break;
        case 0x22: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_22; break;
        case 0x2B: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_2B; break;
        case 0x30: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_30; break;
        case 0x31: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_31; break;
        case 0x32: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_32; break;
        case 0x33: StringGridBlocks->Canvas->Brush->Color = COLOR_BLOCK_TYPE_33; break;
        default: StringGridBlocks->Canvas->Brush->Color = clWhite;
      }
    }
    StringGridBlocks->Canvas->FillRect(CellRect);

    String CellText = StringGridBlocks->Cells[ACol][ARow];
    int TextPosH = 5;
    int TextHeight = StringGridBlocks->Canvas->TextHeight(CellText);
    int TextPosV = (StringGridBlocks->RowHeights[ARow] - TextHeight) / 2;
	  StringGridBlocks->Canvas->TextOut(CellRect.Left+TextPosH-1,
									                    CellRect.Top+TextPosV,
									                    CellText);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::StringGridBlocksSelectCell(
			TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
  if (StringGridBlocks->Focused() == true)
  {
    ShowBlock(FormMain->NativeCPC->Tape, ARow);
  }

	NOT_USED(Sender);
	NOT_USED(ACol);
	NOT_USED(CanSelect);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::ButtonPlayBlockClick(
      TObject *Sender)
{
  mEmulatorP->TapeSetCurrentBlock(StringGridBlocks->Row);
  ActiveControl = StringGridBlocks;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugCDTViewer::ButtonSelectPlayedClick(
      TObject *Sender)
{
  StringGridBlocks->Row = mEmulatorP->NativeCPC->Tape->current_block;
  ShowBlock(FormMain->NativeCPC->Tape, StringGridBlocks->Row);
  ActiveControl = StringGridBlocks;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugCDTViewer::ButtonSelect000Click(TObject *Sender)
{
  StringGridBlocks->Row = mEmulatorP->NativeCPC->Tape->counter000_block;
  ShowBlock(FormMain->NativeCPC->Tape, StringGridBlocks->Row);
  ActiveControl = StringGridBlocks;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::OnCDTEvent(tTape* TapeP)
{
  if (TapeP->pbTapeImage)
  {
    Caption = WindowTitle + " - " + ExtractFileName(mEmulatorP->TapeFilename);
    StringGridBlocks->RowCount = TapeP->nb_blocks;

    GroupBoxBlockInfo->Visible = true;
    PanelBlocks->Visible = true;
    PanelControls->Visible = true;

    MemoTapeInfo->Lines->BeginUpdate();
    MemoTapeInfo->Clear();
    MemoTapeInfo->Lines->Add("NbBlocks = " + AnsiString(TapeP->nb_blocks));
    // Display total duration
    int TapeNbMinutes = TapeP->total_duration_ms / 60000;
    int TapeNbSeconds = (TapeP->total_duration_ms - (TapeNbMinutes * 60000)) / 1000;
    AnsiString DurationString = "Duration = ";
    DurationString += AnsiString::Format("%.2dmn %.2ds",
                                         ARRAYOFCONST(((TapeNbMinutes), (TapeNbSeconds))));
    MemoTapeInfo->Lines->Add(DurationString);
    MemoTapeInfo->Lines->EndUpdate();
    
    // Prepare blocks
    for (int Loop = 0; Loop < StringGridBlocks->RowCount; Loop++)
    {
      tTapeBlock TapeBlock = TapeP->blocks[Loop];
      AnsiString BlockID = AnsiString(Loop+1) + ": ";
      switch (TapeBlock.block_id)
      {
        case 0x10: BlockID += BlockType10ShortLabel; break;
        case 0x11:
        {
          BlockID += BlockType11ShortLabel;
          tTapeBlockType11* BlockType11 = (tTapeBlockType11*)(TapeBlock.pbImageData + 1);
          // Header record
          if (IsHeaderRecordValid((tUChar*)(&BlockType11->FirstData)) == true)
          {
            tTapeHeaderRecord* HeaderRecordP = (tTapeHeaderRecord*)(&BlockType11->FirstData+1);
            AnsiString Filename = (char*)HeaderRecordP->Filename;
            Filename = Filename.SubString(1, 16); // 16 characters maximum
            BlockID += " File \"" + AnsiString(Filename) + "\"";
            BlockID += " Block " + AnsiString(HeaderRecordP->BlockNumber);
          }
        }
        break;
        case 0x12: BlockID += BlockType12ShortLabel; break;
        case 0x13: BlockID += BlockType13ShortLabel; break;
        case 0x14: BlockID += BlockType14ShortLabel; break;
        case 0x15: BlockID += BlockType15ShortLabel; break;
        case 0x18: BlockID += BlockType18ShortLabel; break;
        case 0x20: BlockID += BlockType20ShortLabel; break;
        case 0x21:
        {
          BlockID += BlockType21ShortLabel;
          tTapeBlockType21* BlockType21 = (tTapeBlockType21*)(TapeBlock.pbImageData + 1);
          AnsiString Name = AnsiString((char*)&(BlockType21->FirstNameChar));
          BlockID += Name.SubString(1, BlockType21->NameLength);
        }
        break;
        case 0x22: BlockID += BlockType22ShortLabel; break;
        case 0x2B: BlockID += BlockType2BShortLabel; break;
        case 0x30:
        {
          BlockID += BlockType30ShortLabel;
          tTapeBlockType30* BlockType30 = (tTapeBlockType30*)(TapeBlock.pbImageData + 1);
          AnsiString Description = AnsiString((char*)&(BlockType30->FirstTextChar));
          BlockID += Description.SubString(1, BlockType30->TextLength);
        }
        break;
        case 0x31: BlockID += BlockType31ShortLabel; break;
        case 0x32: BlockID += BlockType32ShortLabel; break;
        case 0x33: BlockID += BlockType33ShortLabel; break;
      }

      StringGridBlocks->Cells[1][Loop] = BlockID;
    }

    mCurrentBlock = TapeP->current_block;
    mCurrentCounter000Block = TapeP->counter000_block;
    StringGridBlocks->Row = TapeP->current_block;

    ShowCurrentBlock(TapeP);
    if (TapeP->pbTapeImage)
    {
      ShowBlock(TapeP, StringGridBlocks->Row);
    }

    ActiveControl = StringGridBlocks;
  }
  else
  {
    Caption = WindowTitle;
    StringGridBlocks->RowCount = 0;

    GroupBoxBlockInfo->Visible = false;
    PanelBlocks->Visible = false;
    PanelControls->Visible = false;

    MemoTapeInfo->Lines->BeginUpdate();
    MemoTapeInfo->Clear();
    MemoTapeInfo->Lines->Add("No CDT tape inserted !!");
    MemoTapeInfo->Lines->EndUpdate();

    ActiveControl = MemoTapeInfo;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::ShowCurrentBlock(tTape* TapeP)
{
  StringGridBlocks->Invalidate();

	NOT_USED(TapeP);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::ShowBlock(tTape* TapeP, int BlockNum)
{
  GroupBoxBlockInfo->Caption = " Block " + AnsiString(BlockNum+1) + " ";

  tTapeBlock* TapeBlock = &TapeP->blocks[BlockNum];
  switch (TapeBlock->block_id)
  {
    case 0x10: ShowBlockType10(TapeBlock); break;
    case 0x11: ShowBlockType11(TapeBlock); break;
    case 0x12: ShowBlockType12(TapeBlock); break;
    case 0x13: ShowBlockType13(TapeBlock); break;
    case 0x14: ShowBlockType14(TapeBlock); break;
    case 0x15: ShowBlockType15(TapeBlock); break;
    case 0x20: ShowBlockType20(TapeBlock); break;
    case 0x21: ShowBlockType21(TapeBlock); break;
    case 0x22: ShowBlockType22(TapeBlock); break;
    case 0x2B: ShowBlockType2B(TapeBlock); break;
    case 0x30: ShowBlockType30(TapeBlock); break;
    case 0x31: ShowBlockType31(TapeBlock); break;
    case 0x32: ShowBlockType32(TapeBlock); break;
    case 0x33: ShowBlockType33(TapeBlock); break;
    default: GroupBoxBlockInfo->Visible = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockData(tUChar* DataP, int Length)
{
  GroupBoxBlockInfo->Visible = true;
  TabSheetBlockData->TabVisible = true;

  MemoBlockData->Clear();
  MemoBlockData->Lines->Add("Preparing data...");
  MemoBlockData->Update();

  Screen->Cursor = crHourGlass;
  MemoBlockData->Lines->BeginUpdate();
  MemoBlockData->Clear();

  int DataAddress = 0;
  do
  {
    AnsiString DataContent = AnsiString::IntToHex(DataAddress, 6) + "  ";
#ifdef SHOW_ASCII
    AnsiString DataASCII = "";
    int RemainingBytes = Length < 16 ? 16 - Length : 0;
#endif /* SHOW_ASCII */
    for (int Loop=0;
         (Loop < 16) && (Length);
         Loop++, Length--)
    {
      DataContent += AnsiString::IntToHex(*DataP, 2) + " ";
#ifdef SHOW_ASCII
      DataASCII += ASCIIChar[*DataP];
#endif /* SHOW_ASCII */
      DataP++;
    }
#ifdef SHOW_ASCII
    for (int Loop=0; Loop < RemainingBytes; Loop++)
    {
      DataContent += "   "; // Blank byte
    }
    DataContent += "   "; // Separator
    DataContent += DataASCII;
#endif /* SHOW_ASCII */
    DataAddress += 16;
    MemoBlockData->Lines->Add(DataContent);
  }
  while (Length);

  MemoBlockData->Lines->EndUpdate();
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugCDTViewer::PageControlBlockChange(
      TObject *Sender)
{
  if (PageControlBlock->ActivePage == TabSheetBlockData)
  {
    if (mBlockDataAvailable == false)
    {
      ShowBlockData(mBlockDataP, mBlockDataLength);
      mBlockDataAvailable = true;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugCDTViewer::PageControlBlockDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = PageControlBlock->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }

  // Get sheet corresponding to TabIndex
  int TabCount=0;
  TTabSheet* Sheet = NULL;
  for (int Loop=0; Loop < PageControlBlock->PageCount; Loop++)
  {
    if (PageControlBlock->Pages[Loop]->TabVisible == true)
    {
      if (TabCount == TabIndex)
      {
        Sheet = PageControlBlock->Pages[Loop];
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
  int TextPosY = (PageControlBlock->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

  NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::ShowBlockType10(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_10;
  PanelBlockType->Caption = BlockType10FullLabel;

  tTapeBlockType10* BlockType10 = (tTapeBlockType10*)(TapeBlock->pbImageData + 1);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  // Prepare block duration
  int BlockNbMinutes = TapeBlock->duration_ms / 60000;
  int BlockNbSeconds = (TapeBlock->duration_ms - (BlockNbMinutes * 60000)) / 1000;
  AnsiString DurationString = "Duration = ";
  if ( (BlockNbMinutes == 0) && (BlockNbSeconds == 0) )
  {
    DurationString += "< 01s";
  }
  else
  {
    DurationString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                       ARRAYOFCONST(((BlockNbMinutes), (BlockNbSeconds))));
  }

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &10");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add(DurationString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Pause After this block (in ms) = "
                            + AnsiString(BlockType10->PauseLength));
  MemoBlockInfo->Lines->Add("Data length (in bytes) = "
                            + AnsiString(BlockType10->DataLength)
                            + " (&" + AnsiString::IntToHex(BlockType10->DataLength, 4) + ")");
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                            + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = true;
  mBlockDataP = &BlockType10->FirstData;
  mBlockDataLength = BlockType10->DataLength;
  mBlockDataAvailable = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType11(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_11;
  PanelBlockType->Caption = BlockType11FullLabel;

  tTapeBlockType11* BlockType11 = (tTapeBlockType11*)(TapeBlock->pbImageData + 1);
  tULong DataLength = BlockType11->DataLengthLSB
                      + (BlockType11->DataLengthMSB << 16);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  // Prepare block duration
  int BlockNbMinutes = TapeBlock->duration_ms / 60000;
  int BlockNbSeconds = (TapeBlock->duration_ms - (BlockNbMinutes * 60000)) / 1000;
  AnsiString DurationString = "Duration = ";
  if ( (BlockNbMinutes == 0) && (BlockNbSeconds == 0) )
  {
    DurationString += "< 01s";
  }
  else
  {
    DurationString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                       ARRAYOFCONST(((BlockNbMinutes), (BlockNbSeconds))));
  }

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &11");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add(DurationString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Length of PILOT pulse (in ticks) = "
                            + AnsiString(BlockType11->PilotPulseLength));
  MemoBlockInfo->Lines->Add("Length of SYNC First pulse (in ticks) = "
                            + AnsiString(BlockType11->SyncFirstPulseLength));
  MemoBlockInfo->Lines->Add("Length of SYNC Second pulse (in ticks) = "
                            + AnsiString(BlockType11->SyncSecondPulseLength));
  MemoBlockInfo->Lines->Add("Length of Zero bit pulse (in ticks) = "
                            + AnsiString(BlockType11->ZeroBitPulseLength));
  MemoBlockInfo->Lines->Add("Length of One bit pulse (in ticks) = "
                            + AnsiString(BlockType11->OneBitPulseLength));
  MemoBlockInfo->Lines->Add("Length of PILOT tone (in pilot pulses) = "
                            + AnsiString(BlockType11->PilotToneLength));
  MemoBlockInfo->Lines->Add("Used bits in last byte = "
                            + AnsiString(BlockType11->UsedBitsLastByte));
  MemoBlockInfo->Lines->Add("Pause After this block (in ms) = "
                            + AnsiString(BlockType11->PauseLength));
  MemoBlockInfo->Lines->Add("Data length (in bytes) = "
                            + AnsiString(DataLength)
                            + " (&" + AnsiString::IntToHex(DataLength, 6) + ")");
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                            + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = true;
  mBlockDataP = &BlockType11->FirstData;
  mBlockDataLength = DataLength;
  mBlockDataAvailable = false;

  //
  // Header
  //
  if (IsHeaderRecordValid((tUChar*)(&BlockType11->FirstData)) == true)
  {
    TabSheetBlockHeader->TabVisible = true;
    ShowHeaderRecord((void*)(&BlockType11->FirstData+1)); // Pass sync byte
  }
  else
  {
    TabSheetBlockHeader->TabVisible = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType12(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_12;
  PanelBlockType->Caption = BlockType12FullLabel;

  tTapeBlockType12* BlockType12 = (tTapeBlockType12*)(TapeBlock->pbImageData + 1);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  // Prepare block duration
  int BlockNbMinutes = TapeBlock->duration_ms / 60000;
  int BlockNbSeconds = (TapeBlock->duration_ms - (BlockNbMinutes * 60000)) / 1000;
  AnsiString DurationString = "Duration = ";
  if ( (BlockNbMinutes == 0) && (BlockNbSeconds == 0) )
  {
    DurationString += "< 01s";
  }
  else
  {
    DurationString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                       ARRAYOFCONST(((BlockNbMinutes), (BlockNbSeconds))));
  }

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &12");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add(DurationString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Length of pulse (in ticks) = "
                            + AnsiString(BlockType12->PulseLength));
  MemoBlockInfo->Lines->Add("Number of pulses = "
                            + AnsiString(BlockType12->NumberPulses)
                            + " (&" + AnsiString::IntToHex(BlockType12->NumberPulses, 4) + ")");
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                            + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType13(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_13;
  PanelBlockType->Caption = BlockType13FullLabel;

  tTapeBlockType13* BlockType13 = (tTapeBlockType13*)(TapeBlock->pbImageData + 1);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  // Prepare block duration
  int BlockNbMinutes = TapeBlock->duration_ms / 60000;
  int BlockNbSeconds = (TapeBlock->duration_ms - (BlockNbMinutes * 60000)) / 1000;
  AnsiString DurationString = "Duration = ";
  if ( (BlockNbMinutes == 0) && (BlockNbSeconds == 0) )
  {
    DurationString += "< 01s";
  }
  else
  {
    DurationString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                       ARRAYOFCONST(((BlockNbMinutes), (BlockNbSeconds))));
  }

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &13");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add(DurationString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Number of pulses = "
                            + AnsiString(BlockType13->NumberPulses));
  tUShort* PulseLength = (tUShort*)&BlockType13->FirstPulseLength;
  for (tUShort Loop=0; Loop < BlockType13->NumberPulses; Loop++)
  {
    AnsiString PulseInfo = "Length of Pulse "
                           + AnsiString(Loop+1)
                           + " (in ticks) = " + AnsiString(*PulseLength++);
    MemoBlockInfo->Lines->Add(PulseInfo);
  }
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                            + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType14(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_14;
  PanelBlockType->Caption = BlockType14FullLabel;

  tTapeBlockType14* BlockType14 = (tTapeBlockType14*)(TapeBlock->pbImageData + 1);
  tULong DataLength = BlockType14->DataLengthLSB
                      + (BlockType14->DataLengthMSB << 16);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  // Prepare block duration
  int BlockNbMinutes = TapeBlock->duration_ms / 60000;
  int BlockNbSeconds = (TapeBlock->duration_ms - (BlockNbMinutes * 60000)) / 1000;
  AnsiString DurationString = "Duration = ";
  if ( (BlockNbMinutes == 0) && (BlockNbSeconds == 0) )
  {
    DurationString += "< 01s";
  }
  else
  {
    DurationString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                       ARRAYOFCONST(((BlockNbMinutes), (BlockNbSeconds))));
  }

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &14");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add(DurationString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Length of Zero bit pulse (in ticks) = "
                            + AnsiString(BlockType14->ZeroBitPulseLength));
  MemoBlockInfo->Lines->Add("Length of One bit pulse (in ticks) = "
                            + AnsiString(BlockType14->OneBitPulseLength));
  MemoBlockInfo->Lines->Add("Used bits in last byte = "
                            + AnsiString(BlockType14->UsedBitsLastByte));
  MemoBlockInfo->Lines->Add("Pause After this block (in ms) = "
                            + AnsiString(BlockType14->PauseLength));
  MemoBlockInfo->Lines->Add("Data length (in bytes) = "
                            + AnsiString(DataLength)
                            + " (&" + AnsiString::IntToHex(DataLength, 6) + ")");
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                            + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = true;
  mBlockDataP = &BlockType14->FirstData;
  mBlockDataLength = DataLength;
  mBlockDataAvailable = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType15(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_15;
  PanelBlockType->Caption = BlockType15FullLabel;

  tTapeBlockType15* BlockType15 = (tTapeBlockType15*)(TapeBlock->pbImageData + 1);
  tULong DataLength = BlockType15->DataLengthLSB
                      + (BlockType15->DataLengthMSB << 16);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  // Prepare block duration
  int BlockNbMinutes = TapeBlock->duration_ms / 60000;
  int BlockNbSeconds = (TapeBlock->duration_ms - (BlockNbMinutes * 60000)) / 1000;
  AnsiString DurationString = "Duration = ";
  if ( (BlockNbMinutes == 0) && (BlockNbSeconds == 0) )
  {
    DurationString += "< 01s";
  }
  else
  {
    DurationString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                       ARRAYOFCONST(((BlockNbMinutes), (BlockNbSeconds))));
  }

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &15");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add(DurationString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Number of T states per sample = "
                            + AnsiString(BlockType15->TStatesPerSampleNumber));
  MemoBlockInfo->Lines->Add("Pause After this block (in ms) = "
                            + AnsiString(BlockType15->PauseLength));
  MemoBlockInfo->Lines->Add("Used bits in last byte = "
                            + AnsiString(BlockType15->UsedBitsLastByte));
  MemoBlockInfo->Lines->Add("Data length (in bytes) = "
                            + AnsiString(DataLength)
                            + " (&" + AnsiString::IntToHex(DataLength, 6) + ")");
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                            + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = true;
  mBlockDataP = &BlockType15->FirstData;
  mBlockDataLength = DataLength;
  mBlockDataAvailable = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType20(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_20;
  PanelBlockType->Caption = BlockType20FullLabel;

  tTapeBlockType20* BlockType20 = (tTapeBlockType20*)(TapeBlock->pbImageData + 1);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  // Prepare block duration
  int BlockNbMinutes = TapeBlock->duration_ms / 60000;
  int BlockNbSeconds = (TapeBlock->duration_ms - (BlockNbMinutes * 60000)) / 1000;
  AnsiString DurationString = "Duration = ";
  if ( (BlockNbMinutes == 0) && (BlockNbSeconds == 0) )
  {
    DurationString += "< 01s";
  }
  else
  {
    DurationString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                       ARRAYOFCONST(((BlockNbMinutes), (BlockNbSeconds))));
  }

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &20");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add(DurationString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Pause duration (in ms) = "
                            + AnsiString(BlockType20->PauseLength));
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                            + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType21(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_21;
  PanelBlockType->Caption = BlockType21FullLabel;

  tTapeBlockType21* BlockType21 = (tTapeBlockType21*)(TapeBlock->pbImageData + 1);

  AnsiString Name = AnsiString((char*)&(BlockType21->FirstNameChar));
  Name = Name.SubString(1, BlockType21->NameLength);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &21");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Name = " + Name);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                             + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType22(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_22;
  PanelBlockType->Caption = BlockType22FullLabel;

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &22");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType2B(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_2B;
  PanelBlockType->Caption = BlockType2BFullLabel;

  tTapeBlockType2B* BlockType2B = (tTapeBlockType2B*)(TapeBlock->pbImageData + 1);
  AnsiString SignalLevel = BlockType2B->SignalLevel ? "High" : "Low";

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &2B");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Signal Level = "
                            + AnsiString(BlockType2B->SignalLevel)
                            + " (" + SignalLevel + ")");
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                             + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType30(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_30;
  PanelBlockType->Caption = BlockType30FullLabel;

  tTapeBlockType30* BlockType30 = (tTapeBlockType30*)(TapeBlock->pbImageData + 1);

  AnsiString Description = AnsiString((char*)&(BlockType30->FirstTextChar));
  Description = Description.SubString(1, BlockType30->TextLength);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &30");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Text = " + Description);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                             + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType31(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_31;
  PanelBlockType->Caption = BlockType31FullLabel;

  tTapeBlockType31* BlockType31 = (tTapeBlockType31*)(TapeBlock->pbImageData + 1);

  AnsiString Description = AnsiString((char*)&(BlockType31->FirstMessageChar));
  Description = Description.SubString(1, BlockType31->MessageLength);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &31");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Duration (in seconds) = " + AnsiString(BlockType31->DisplayDuration));
  MemoBlockInfo->Lines->Add("Message = " + Description);
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                             + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType32(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_32;
  PanelBlockType->Caption = BlockType32FullLabel;

  tTapeBlockType32* BlockType32 = (tTapeBlockType32*)(TapeBlock->pbImageData + 1);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &32");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add("");
  tUChar* TextID = (tUChar*)&BlockType32->FirstTextID;
  for (tUShort Loop=0; Loop < BlockType32->NumberOfTextStrings; Loop++)
  {
    AnsiString Info;
    if (*TextID == 0xff)
    {
      Info = ArchiveCommentLabel + ":";
    }
    else
    {
      Info = ArchiveIDLabel[*TextID] + ":";
    }
    MemoBlockInfo->Lines->Add(Info);

    AnsiString Description = AnsiString((char*)(TextID+2));
    tUChar DescriptionLength = *(TextID+1);
    Info = "> " + Description.SubString(1, DescriptionLength);
    MemoBlockInfo->Lines->Add(Info);

    TextID += DescriptionLength + 2; // Next archive info
  }
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                             + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugCDTViewer::ShowBlockType33(tTapeBlock* TapeBlock)
{
  GroupBoxBlockInfo->Visible = true;

  PanelBlockType->Color = COLOR_BLOCK_TYPE_33;
  PanelBlockType->Caption = BlockType33FullLabel;

  tTapeBlockType33* BlockType33 = (tTapeBlockType33*)(TapeBlock->pbImageData + 1);

  // Prepare block time offset
  int OffsetMinutes = TapeBlock->time_offset_ms / 60000;
  int OffsetSeconds = (TapeBlock->time_offset_ms - (OffsetMinutes * 60000)) / 1000;
  AnsiString TimeOffsetString = "Time offset = ";
  TimeOffsetString += AnsiString::Format(DURATION_MN_S_FORMAT,
                                         ARRAYOFCONST(((OffsetMinutes), (OffsetSeconds))));

  //
  // Block Info
  //
  TabSheetBlockInfo->TabVisible = true;
  PageControlBlock->ActivePage = TabSheetBlockInfo;
  MemoBlockInfo->Lines->BeginUpdate();
  MemoBlockInfo->Clear();
  MemoBlockInfo->Lines->Add("Type &33");
  MemoBlockInfo->Lines->Add(TimeOffsetString);
  MemoBlockInfo->Lines->Add("");
  tUChar* MachineInfo = (tUChar*)&BlockType33->FirstHardwareType;
  for (tUShort Loop=0; Loop < BlockType33->NumberOfMachines; Loop++)
  {
    AnsiString Info = "[" + HardwareTypeLabel[*MachineInfo] + "] ";
    const AnsiString* HardwareID = HardwareIDList[*MachineInfo++];
    Info += HardwareID[*(MachineInfo++)];
    Info += ":";
    MemoBlockInfo->Lines->Add(Info);
    MemoBlockInfo->Lines->Add(HardwareValue[*(MachineInfo++)]);
    MemoBlockInfo->Lines->Add("");
  }
  MemoBlockInfo->Lines->Add("");
  MemoBlockInfo->Lines->Add("Block size (in bytes) = "
                             + AnsiString(TapeBlock->length)
                            + " (&" + AnsiString::IntToHex(TapeBlock->length, 4) + ")");
  MemoBlockInfo->Lines->EndUpdate();

  //
  // Data
  //
  TabSheetBlockData->TabVisible = false;

  //
  // Header
  //
  TabSheetBlockHeader->TabVisible = false;
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugCDTViewer::IsHeaderRecordValid(tUChar* DataP)
{
bool Result = false;

  do
  {
    // Check first byte ID
    if (*DataP != 0x2C)
    {
      continue;
    }

    // Valid Filename
    tUChar* Filename = DataP+1;
    tUChar LastChar = '0';
    int Loop;
    for (Loop=0; Loop < 16; Loop++)
    {
      if (*Filename == 0)
      {
      }
      else if ( (*Filename < ' ') || (*Filename > 'z' ) )
      {
        break;
      }
      else if (LastChar == 0)
      {
        break;
      }

      LastChar = *(Filename++);
    }
    if (Loop < 16)
    {
      continue;
    }

    // Check CRC
    tUChar* HeaderP = DataP+1;
    tUShort ComputedCRC = CRC_CCITT((const tUChar*)HeaderP, 256) ^ 0xFFFF;
    tUShort RecordCRC = (*(HeaderP+256) << 8) + *(HeaderP+257);
    if (ComputedCRC != RecordCRC)
    {
      continue;
    }

    // Record is a header
    Result = true;
  }
  while (0);

  return (Result);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugCDTViewer::ShowHeaderRecord(void* DataP)
{
tTapeHeaderRecord* HeaderRecordP = (tTapeHeaderRecord*)DataP;

  MemoHeaderRecord->Lines->BeginUpdate();
  MemoHeaderRecord->Clear();

  // Prepare file type
  AnsiString FileType = "";
  if (HeaderRecordP->FileType & 0x01) FileType += "Protected ";
  switch ((HeaderRecordP->FileType >> 1) & 0x07)
  {
    case 0: FileType += "Internal BASIC"; break;
    case 1: FileType += "Binary"; break;
    case 2: FileType += "Screen image"; break;
    case 3: FileType += "ASCII"; break;
    default : FileType += "unallocated"; break;
  }
  FileType += " version=" + AnsiString((HeaderRecordP->FileType >> 4) & 0x0F);

  AnsiString Filename = (char*)HeaderRecordP->Filename;
  Filename = Filename.SubString(1, 16); // 16 characters maximum

  MemoHeaderRecord->Lines->Add("System fields");
  MemoHeaderRecord->Lines->Add("=============");
  MemoHeaderRecord->Lines->Add("Filename = \""
                               + Filename + "\"");
  MemoHeaderRecord->Lines->Add("Block number = "
                               + AnsiString(HeaderRecordP->BlockNumber)
                               + " (&" + AnsiString::IntToHex(HeaderRecordP->BlockNumber, 2) + ")");
  MemoHeaderRecord->Lines->Add("File type = " + FileType);
  MemoHeaderRecord->Lines->Add("Data length = "
                               + AnsiString(HeaderRecordP->DataLength)
                               + " (&" + AnsiString::IntToHex(HeaderRecordP->DataLength, 4) + ")");
  MemoHeaderRecord->Lines->Add("Data location = &"
                               + AnsiString::IntToHex(HeaderRecordP->DataLocation, 4));
  MemoHeaderRecord->Lines->Add("First block = "
                               + AnsiString(HeaderRecordP->FirstBlock)
                               + " (&" + AnsiString::IntToHex(HeaderRecordP->FirstBlock, 2) + ")");
  MemoHeaderRecord->Lines->Add("Last block = "
                               + AnsiString(HeaderRecordP->LastBlock)
                               + " (&" + AnsiString::IntToHex(HeaderRecordP->LastBlock, 2) + ")");

  MemoHeaderRecord->Lines->Add("");
  MemoHeaderRecord->Lines->Add("User fields");
  MemoHeaderRecord->Lines->Add("===========");
  MemoHeaderRecord->Lines->Add("Logical length = "
                               + AnsiString(HeaderRecordP->LogicalLength)
                               + " (&" + AnsiString::IntToHex(HeaderRecordP->LogicalLength, 4) + ")");
  MemoHeaderRecord->Lines->Add("Entry address = &"
                               + AnsiString::IntToHex(HeaderRecordP->EntryAddress, 4));

  // Computed CRC
  tUShort ComputedCRC = CRC_CCITT((const tUChar*)DataP, 256) ^ 0xFFFF;
  MemoHeaderRecord->Lines->Add("");
  MemoHeaderRecord->Lines->Add("Computed CRC = &"
                               + AnsiString::IntToHex(ComputedCRC, 4));

  MemoHeaderRecord->Lines->EndUpdate();
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugCDTViewer::OnStepByStepEvent(tULong Condition, tNativeCPC* NativeCPCP)
{
  bool Pause = NativeCPCP->paused;

  // Current block has changed ?
  if (NativeCPCP->Tape->current_block != mCurrentBlock)
  {
    mCurrentBlock = NativeCPCP->Tape->current_block;
    ShowCurrentBlock(NativeCPCP->Tape);
  }

  // Counter 000 block has changed ?
  if (NativeCPCP->Tape->counter000_block != mCurrentCounter000Block)
  {
    mCurrentCounter000Block = NativeCPCP->Tape->counter000_block;
    StringGridBlocks->Invalidate();
  }

  NOT_USED(Condition);

  return Pause;
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugCDTViewer::CopyToClipboardMenuClick(
      TObject *Sender)
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


void __fastcall TFormDebugCDTViewer::ButtonGoToBlockClick(TObject *Sender)
{
  if (EditGoToBlock->Text.IsEmpty() == true) return;

  int Block = EditGoToBlock->Text.ToInt() - 1;
  if (Block < 0) Block = 0;
  if (Block >= StringGridBlocks->RowCount) Block = StringGridBlocks->RowCount - 1;

  ActiveControl = StringGridBlocks;
  StringGridBlocks->TopRow = Block;
  StringGridBlocks->Row = Block;
  ShowBlock(FormMain->NativeCPC->Tape, StringGridBlocks->Row);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

