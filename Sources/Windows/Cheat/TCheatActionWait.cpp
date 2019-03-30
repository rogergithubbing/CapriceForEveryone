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
#include "TCheatActionWait.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TCheatActionWait::TCheatActionWait(AnsiString Command) : TCheatAction()
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
    mDurationMS = Command.SubString(ParamPos+1, ParamLength).Trim().ToInt();
    mValid = true;
  }
}
//---------------------------------------------------------------------------


__fastcall TCheatActionWait::~TCheatActionWait(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TCheatActionWait::Start(tNativeCPC* NativeCPC)
{
  mCounter = (mDurationMS / 20) + 1; // 20ms per cycle

  NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------

void __fastcall TCheatActionWait::Execute(tNativeCPC* NativeCPC)
{
  if (mCounter)
  {
    mCounter--;
  }

  NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------


bool __fastcall TCheatActionWait::IsFinished(void)
{
  return (mCounter) ? false : true;
}
//---------------------------------------------------------------------------

