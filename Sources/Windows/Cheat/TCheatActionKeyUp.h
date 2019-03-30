/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2017 by Fr�d�ric Coste

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


#ifndef TCheatActionKeyUpH
#define TCheatActionKeyUpH

#include "TCheatAction.h"


class TCheatActionKeyUp : public TCheatAction
{
private:
  //
  // Members
  //
  int mKeyCode;
  bool mAll;

  //
  // Methods
  //
  
protected:
  //
  // Members
  //
  
  //
  // Methods
  //

public:
  //
  // Properties
  //

  //
  // Public API
  //
  __fastcall TCheatActionKeyUp(AnsiString Command);
  virtual __fastcall ~TCheatActionKeyUp();

  void __fastcall Execute(tNativeCPC* NativeCPC);
};

//---------------------------------------------------------------------------
#endif
