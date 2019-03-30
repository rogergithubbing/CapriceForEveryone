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
#include "TCheatActionFlipSides.h"
#if (__BORLANDC__ == 0x551) // Borland C++ Builder 5 Update 1
#include "..\Forms\TFormMain.h"
#else /* __BORLANDC__ */
#include "..\Forms_XE3\TFormMain.h"
#endif /* __BORLANDC__ */

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TCheatActionFlipSides::TCheatActionFlipSides(AnsiString Command) : TCheatAction()
//
// Constructor
//
{
  if (Command.Pos(";") != 1)
  {
    mValid = true;
  }
}
//---------------------------------------------------------------------------


__fastcall TCheatActionFlipSides::~TCheatActionFlipSides(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TCheatActionFlipSides::Execute(tNativeCPC* NativeCPC)
{
  // Drive A Flip sides
  FormMain->FlipSidesDriveA();
  
  NOT_USED(NativeCPC);
}
//---------------------------------------------------------------------------
