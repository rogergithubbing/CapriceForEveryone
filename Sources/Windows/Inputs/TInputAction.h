/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2015 by Frédéric Coste

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


#ifndef TInputActionH
#define TInputActionH

#include "TInputKey.h"


typedef bool __fastcall (__closure *TInputActionWINEvent)(WORD &Key, TShiftState Shift);
typedef void __fastcall (__closure *TInputActionCPCEvent)(tUChar CPCKeycode);


class TInputAction : public TList
{
private:
  // Members
  TInputActionWINEvent mOnWINButtonDown;
  TInputActionWINEvent mOnWINButtonUp;
  TInputActionCPCEvent mOnCPCButtonDown;
  TInputActionCPCEvent mOnCPCButtonUp;
  // methods
  TInputKey* __fastcall GetKey(int iIndex);
  void __fastcall SetKey(int iIndex, TInputKey* Value);
  inline void __fastcall SetOnWINButtonDown(TInputActionWINEvent Value) { mOnWINButtonDown = Value; };
  inline void __fastcall SetOnWINButtonUp(TInputActionWINEvent Value) { mOnWINButtonUp = Value; };
  inline void __fastcall SetOnCPCButtonDown(TInputActionCPCEvent Value) { mOnCPCButtonDown = Value; };
  inline void __fastcall SetOnCPCButtonUp(TInputActionCPCEvent Value) { mOnCPCButtonUp = Value; };

protected:

public:
  // Properties
  __property TInputKey* Keys[int iIndex] = {read=GetKey, write=SetKey};
  __property TInputActionWINEvent OnWINButtonDown = { write=SetOnWINButtonDown };
  __property TInputActionWINEvent OnWINButtonUp = { write=SetOnWINButtonUp };
  __property TInputActionCPCEvent OnCPCButtonDown = { write=SetOnCPCButtonDown };
  __property TInputActionCPCEvent OnCPCButtonUp = { write=SetOnCPCButtonUp };
  // Public API
  __fastcall TInputAction();
  virtual __fastcall ~TInputAction();

  virtual void __fastcall OnChange(long NewValue) = 0;

  void __fastcall PressButton(int Index);
  void __fastcall ReleaseButton(int Index);
  bool __fastcall IsPressed(int Index);
};

//---------------------------------------------------------------------------
#endif
