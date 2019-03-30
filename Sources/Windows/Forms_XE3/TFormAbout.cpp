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
#include <tchar.h>
#pragma hdrstop
#include "TFormAbout.h"

#define HISTORY_FILENAME           "History.txt"


typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

static _TCHAR Presentation[] =
	_T("CaPriCe Forever is an Amstrad CPC Emulator.\r\n")
	_T("\r\n")
	_T("Original CPC Engine, Caprice32 v4.2, written by Ulrich Doewich.\r\n")
	_T("\r\n")
	_T("Adapted in 2006 for PalmOS then upgraded since 2014 for Windows by Frederic Coste")
	_T("Contact: coste.frederic@free.fr");

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAbout *FormAbout;
//---------------------------------------------------------------------------
__fastcall TFormAbout::TFormAbout(TComponent* Owner)
  : TForm(Owner)
{
  mAudioContextP = NULL;
  mDrawingContextP = NULL;
  mInputContextP = NULL;
  mSettingsP = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormAbout::FormShow(TObject *Sender)
{
  PageControl->ActivePage = TabSheetVersions;

	// Presentation without annoying TMemo end blank line
	MemoAuthor->SetTextBuf(Presentation);

  MemoVersions->Clear();
  MemoCredits->Clear();
  MemoHistory->Clear();
  MemoDebugInfo->Clear();

  // Get Windows version
  OSVERSIONINFO osvi;
  ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osvi);
  LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
  BOOL bIsWow64 = FALSE;
  if(fnIsWow64Process)
  {
    fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
  }
  AnsiString VersionMsg = (bIsWow64 == TRUE) ? "Windows 64bits = " : "Windows 32bits = ";
  VersionMsg += AnsiString(osvi.dwMajorVersion);
  VersionMsg += "." + AnsiString(osvi.dwMinorVersion);
  if (AnsiString(osvi.szCSDVersion).Length() > 0)
  {
    VersionMsg += " (" + AnsiString(osvi.szCSDVersion) + ")";
  }
  MemoDebugInfo->Lines->Add(VersionMsg);
  MemoDebugInfo->Lines->Add("");

  //
  // Credits
  //
  MemoCredits->Lines->Add("Special thanks to Kukulcan and Maxit from CPC-Power team for testing and inspiring improvements.");
  MemoCredits->Lines->Add("");
  MemoCredits->Lines->Add("I want to thank Richard Wilson, author of WinAPE, for his help and support to understand his SNR format.");
  MemoCredits->Lines->Add("I want to thank Kevin Thacker for his help to get a better FDC emulation.");
  MemoCredits->Lines->Add("");
  MemoCredits->Lines->Add("Almost all ever written software for Amstrad CPC can be found on CPC-Power website.");
  MemoCredits->Lines->Add("");
  MemoCredits->Lines->Add("Parts of code come from third parties. Credits are included and been preserved into Caprice Forever source code.");
  MemoCredits->Lines->Add("");
  MemoCredits->Lines->Add("Please find hereafter list of third party code:");
  MemoCredits->Lines->Add("> Some Z80 opcode optimizations and CRTC upgrades from Caprice Reloaded;");
  MemoCredits->Lines->Add("> Some Z80 opcode optimizations from MAME;");
  MemoCredits->Lines->Add("> IEEE754 embedded mathematics from Sun Microsystems, Inc. All rights reserved;");
  MemoCredits->Lines->Add("> MEA8000 emulation, written by Antoine Miné, from MESS;");
  MemoCredits->Lines->Add("> Butterworth filter, written by stefan, from Exstrom Laboratories LLC Copyright (C) 2014;");
  MemoCredits->Lines->Add("> ZIP inflating from zlib 1.2.8, (C) 1995-2013 Jean-loup Gailly and Mark Adler.");
  MemoCredits->Lines->Add("");
  //
  AnsiString GuysString  = "Bruno Broult, Stéphane Buisard, Cedrick Collomb, Guillaume Crozet, Philippe Depre, ";
  GuysString += "Thomas Guillemin, Jean-Philippe Latruffe, Marcos Gonzalez Troyas";
  MemoCredits->Lines->Add("List of guys helping me to build this software (Alphabetical order):");
  MemoCredits->Lines->Add(GuysString);

  //
  // Versions
  //
  // Add Beta message
  MemoVersions->Lines->Add("BETA VERSION UNDER DEVELOPMENT");
  MemoVersions->Lines->Add("");
  // Get version number
  VersionMsg = "Version = " + GetFileFullVersion(Application->ExeName);
  MemoVersions->Lines->Add(VersionMsg);
  MemoVersions->Lines->Add("");
  MemoDebugInfo->Lines->Add(VersionMsg);

  //
  // Show all DLL files version
  //
  TSearchRec F;
  AnsiString Path = ExtractFilePath(Application->ExeName);
  if (FindFirst(ExtractFilePath(Application->ExeName) + "*.dll", faAnyFile, F) == 0)
  {
    int findresult;
    do
    {
      VersionMsg = F.Name + " version = " + GetFileFullVersion(Path + F.Name);
      MemoVersions->Lines->Add(VersionMsg);
      MemoDebugInfo->Lines->Add(VersionMsg);

      findresult = FindNext(F);
    }
    while (findresult == 0);
  }

#ifdef _USE_DIRECTX
  //
  // Show DirectX DLL files version
  //
  _TCHAR SystemPath[MAX_PATH];
  GetSystemDirectory(SystemPath, MAX_PATH);
  VersionMsg = "d3d9.dll version = " + GetFileFullVersion(AnsiString(SystemPath) + "\\d3d9.dll");
  MemoVersions->Lines->Add(VersionMsg);
  MemoDebugInfo->Lines->Add(VersionMsg);
  VersionMsg = "d3dx9_43.dll version = " + GetFileFullVersion(AnsiString(SystemPath) + "\\d3dx9_43.dll");
  MemoVersions->Lines->Add(VersionMsg);
  MemoDebugInfo->Lines->Add(VersionMsg);

#endif /* _USE_DIRECTX */

  //
  // HISTORY
  //
  AnsiString HistoryFilename = ExtractFilePath(Application->ExeName) + HISTORY_FILENAME;
  if (FileExists(HistoryFilename) == true)
  {
    MemoHistory->Lines->LoadFromFile(HistoryFilename);
  }
  else
  {
    TabSheetHistory->TabVisible = false;
  }

  //
  // DEBUG Info
  //
  MemoDebugInfo->Lines->Add("");
  // Add Drawing Info
  if (mDrawingContextP != NULL)
  {
    TStringList* Info = mDrawingContextP->GetInfo();
    MemoDebugInfo->Lines->AddStrings(Info);
    delete Info;
  }

  // Add Audio Info
  if (mAudioContextP != NULL)
  {
    MemoDebugInfo->Lines->Add("");
    TStringList* Info = mAudioContextP->GetInfo();
    MemoDebugInfo->Lines->AddStrings(Info);
    delete Info;
  }

  // Add Inputs Info
  if (mInputContextP != NULL)
  {
    MemoDebugInfo->Lines->Add("");
    TStringList* Info = mInputContextP->GetInfo();
    MemoDebugInfo->Lines->AddStrings(Info);
    delete Info;
  }

  // Add settings INfo
  if (mSettingsP != NULL)
  {
    MemoDebugInfo->Lines->Add("");
    TStringList* Info = mSettingsP->GetInfo();
    MemoDebugInfo->Lines->AddStrings(Info);
    delete Info;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormAbout::GetFileVersion(String Filename)
{
  AnsiString Version;

  // Get version number
  DWORD dwTemp;
  DWORD dwInfoSize = GetFileVersionInfoSize ((LPTSTR)Filename.c_str (),
                                              &dwTemp);
  char * pFileInfo = new char [dwInfoSize];

  if (GetFileVersionInfo ((LPTSTR)Filename.c_str (),
                          0,
                          dwInfoSize,
                          (LPVOID)pFileInfo))
  {
    UINT Size = sizeof (VS_FIXEDFILEINFO);
    LPVOID lpBuffer;
    VerQueryValue ((LPVOID)pFileInfo, _T("\\"), &lpBuffer, &Size);

    int iFileVersionMS = ((VS_FIXEDFILEINFO *)lpBuffer)->dwFileVersionMS;
    short pMajeure = (short)(iFileVersionMS >> 16);
    short pMineure = (short)iFileVersionMS;
    Version = AnsiString(pMajeure) + "." + AnsiString(pMineure);

    int iFileVersionLS = ((VS_FIXEDFILEINFO *)lpBuffer)->dwFileVersionLS;
    short pRevision = (short)(iFileVersionLS >> 16);
    short pBuild = (short)iFileVersionLS;
    if (pRevision)
    {
      Version += "." + AnsiString(pRevision);
    }
    if (pBuild)
    {
      Version += "." + AnsiString(pBuild);
    }
  }

  delete [] pFileInfo;

  return Version;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormAbout::GetFileFullVersion(String Filename)
{
  AnsiString Version;
  
  // Get version number
  DWORD dwTemp;
  DWORD dwInfoSize = GetFileVersionInfoSize ((LPTSTR)Filename.c_str (),
                                              &dwTemp);
  char * pFileInfo = new char [dwInfoSize];

  if (GetFileVersionInfo ((LPTSTR)Filename.c_str (),
                          0,
                          dwInfoSize,
                          (LPVOID)pFileInfo))
  {
    UINT Size = sizeof (VS_FIXEDFILEINFO);
    LPVOID lpBuffer;
    VerQueryValue ((LPVOID)pFileInfo, _T("\\"), &lpBuffer, &Size);

    int iFileVersionMS = ((VS_FIXEDFILEINFO *)lpBuffer)->dwFileVersionMS;
    short pMineure = (short)iFileVersionMS;
    short pMajeure = (short)(iFileVersionMS >> 16);

    int iFileVersionLS = ((VS_FIXEDFILEINFO *)lpBuffer)->dwFileVersionLS;
    short pConstruction = (short)iFileVersionLS;
    short pSousVersion = (short)(iFileVersionLS >> 16);

    Version = AnsiString (pMajeure)
              + '.' + AnsiString (pMineure)
              + '.' + AnsiString (pSousVersion)
              + '.' + AnsiString (pConstruction);
  }

  delete [] pFileInfo;

  return Version;
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::Copytoclipboard1Click(TObject *Sender)
{
  TStringList* pBuffer = new TStringList();

  TMemo* Memo = (TMemo*)PopupMenuInfo->PopupComponent;

  for (int Index=0; Index < Memo->Lines->Count; Index++)
  {
    pBuffer->Add(Memo->Lines->Strings[Index]);
  }

  Clipboard()->AsText = pBuffer->Text;

  delete pBuffer;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAbout::MemoContextPopup(TObject *Sender,
			TPoint &MousePos, bool &Handled)
{
	Handled = true;

	NOT_USED(Sender);
	NOT_USED(MousePos);
}
//---------------------------------------------------------------------------

