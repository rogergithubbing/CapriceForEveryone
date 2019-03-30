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
#include "TCheatActionKeyUp.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TCheatActionKeyUp::TCheatActionKeyUp(AnsiString Command) : TCheatAction()
//
// Constructor
//
{
  mAll = false;
  
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
    if (Command.Pos("all"))
    {
      mValid = true;
      mAll = true;
    }
    else
    {
      mKeyCode = Command.SubString(ParamPos+1, ParamLength).Trim().ToInt();

      if ( (mKeyCode >= 0) && (mKeyCode < 128) )
      {
        mValid = true;
      }
    }
  }
}
//---------------------------------------------------------------------------


__fastcall TCheatActionKeyUp::~TCheatActionKeyUp(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TCheatActionKeyUp::Execute(tNativeCPC* NativeCPC)
{
  if (mAll == false)
  {
    RELEASE_KEY(USERMANUAL_TO_CPC_KBD(mKeyCode));
  }
  else
  {
    for (int Loop=0; Loop < NATIVECPC_KEYBOARD_MATRIX_NBELEMENT; Loop++)
    {
      NativeCPC->keyboard_matrix[Loop] = 0xFF;
    }
  }
}
//---------------------------------------------------------------------------


