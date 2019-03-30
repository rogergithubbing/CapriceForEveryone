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
#pragma hdrstop
#include "TCheatActionExit.h"
#include "..\Forms_XE3\TFormMain.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TCheatActionExit::TCheatActionExit(AnsiString Command) : TCheatAction()
//
// Constructor
//
{
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
    mExitCode = Command.SubString(ParamPos+1, ParamLength).Trim().ToInt();
    mValid = true;
  }
}
//---------------------------------------------------------------------------


__fastcall TCheatActionExit::~TCheatActionExit(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TCheatActionExit::Execute(tNativeCPC* NativeCPC)
{
  Application->Tag = mExitCode;
  FormMain->Close();

  NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------

