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
#include "TCheatActionEnd.h"
#include "..\Forms_XE3\TFormMain.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TCheatActionEnd::TCheatActionEnd(void) : TCheatAction()
//
// Constructor
//
{
  mValid = true;
}
//---------------------------------------------------------------------------


__fastcall TCheatActionEnd::~TCheatActionEnd(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------
