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


#ifndef TCheatActionWaitH
#define TCheatActionWaitH

#include "TCheatAction.h"


class TCheatActionWait : public TCheatAction
{
private:
  //
  // Members
  //
  int mDurationMS;
  int mCounter;

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
  __fastcall TCheatActionWait(AnsiString Command);
  virtual __fastcall ~TCheatActionWait();

  void __fastcall Start(tNativeCPC* NativeCPC);
  void __fastcall Execute(tNativeCPC* NativeCPC);
  bool __fastcall IsFinished(void);
};

//---------------------------------------------------------------------------
#endif
