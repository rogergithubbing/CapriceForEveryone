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


#ifndef TCheatActionH
#define TCheatActionH

#include "..\..\..\Sources\Engine\Native_CPC.h"


class TCheatAction
{
private:
  //
  // Members
  //

  //
  // Methods
  //

protected:
  //
  // Members
  //
  bool mValid;
  AnsiString mLabel;

  //
  // Methods
  //
  inline bool GetValid(void) { return mValid; };
  inline AnsiString GetLabel(void) { return mLabel; };
  inline void SetLabel(AnsiString Value) { mLabel = Value; };

public:
  //
  // Properties
  //
  __property bool Valid = { read=GetValid };
  __property AnsiString Label = { read=GetLabel, write=SetLabel };

  //
  // Public API
  //
  __fastcall TCheatAction();
  __fastcall ~TCheatAction();

  virtual void __fastcall Start(tNativeCPC* NativeCPC) { };
  virtual void __fastcall Execute(tNativeCPC* NativeCPC) { };
  virtual bool __fastcall IsFinished(void) { return true; };
  virtual bool __fastcall IsScriptTerminated(void) { return false; };
  virtual AnsiString __fastcall NextAction(void) { return NULL; };
};

//---------------------------------------------------------------------------
#endif
