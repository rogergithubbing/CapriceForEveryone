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


#ifndef TCheatActionEqualH
#define TCheatActionEqualH

#include "TCheatAction.h"


class TCheatActionEqual : public TCheatAction
{
private:
  //
  // Members
  //
  tULong mAddress;
  tULong mExpectedValue;
  tULong mMask;
  AnsiString mBranchLabel;
  bool mBranchToLabel;

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
  __fastcall TCheatActionEqual(AnsiString Command);
  virtual __fastcall ~TCheatActionEqual();

  void __fastcall Execute(tNativeCPC* NativeCPC);
  AnsiString __fastcall NextAction(void);
};

//---------------------------------------------------------------------------
#endif
