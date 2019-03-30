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
#pragma hdrstop
#include "TCheatActionScreenDiff.h"
#if (__BORLANDC__ == 0x551) // Borland C++ Builder 5 Update 1
#include "..\Forms\TFormMain.h"
#else /* __BORLANDC__ */
#include "..\Forms_XE3\TFormMain.h"
#endif /* __BORLANDC__ */

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TCheatActionScreenDiff::TCheatActionScreenDiff(AnsiString Command) : TCheatAction()
//
// Constructor
//
{
  mBranchToLabel = false;

  int ParamPos = Command.Pos("=");
  int ParamLength = Command.Length() - ParamPos;
  int CommentPos = Command.Pos(";");

  if (CommentPos)
  {
    ParamLength = CommentPos - ParamPos - 1;
  }
  else
  {
    CommentPos = Command.Length();
  }

  if (ParamPos < CommentPos)
  {
    AnsiString Param = Command.SubString(ParamPos+1, ParamLength).Trim();
    int SeparatorPos = Param.Pos(":");

	  // Screen filename
    mScreenFilename = Param.SubString(1, SeparatorPos-1).Trim();

    // Branch label
    mBranchLabel = Param.SubString(SeparatorPos+1, Param.Length()).Trim();

    if ( (mScreenFilename.IsEmpty() == false)
         && (mBranchLabel.IsEmpty() == false) )
    {
      mValid = true;
    }
  }
}
//---------------------------------------------------------------------------


__fastcall TCheatActionScreenDiff::~TCheatActionScreenDiff(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TCheatActionScreenDiff::Execute(tNativeCPC* NativeCPC)
{
  AnsiString Filename = FormMain->SettingsP->ScreenshotDirectory + mScreenFilename;

  if (FormMain->DrawingContextP->CompareScreenshot(Filename) == false)
  {
    mBranchToLabel = true;
  }

  NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TCheatActionScreenDiff::NextAction(void)
{
  return (mBranchToLabel == true ? mBranchLabel : (AnsiString)NULL);
}
//---------------------------------------------------------------------------


