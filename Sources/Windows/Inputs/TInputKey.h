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


#ifndef TInputKeyH
#define TInputKeyH

#include "..\..\Engine\types.h"



class TInputKey
{
private:
  // Members
  WORD mWINKeycode;
  TShiftState mWINShiftState;
  tUChar mCPCKeycode;
  bool mCPCKeycodeValid;
  bool mPressed;

  // methods
  inline WORD __fastcall GetWINKeycode(void) { return mWINKeycode; };
  inline void __fastcall SetWINKeycode(WORD Value) { mWINKeycode = Value; };
  inline TShiftState __fastcall GetWINShiftState(void) { return mWINShiftState; };
  inline void __fastcall SetWINShiftState(TShiftState Value) { mWINShiftState = Value; };
  tUChar __fastcall GetCPCKeycode(void);
  void __fastcall SetCPCKeycode(tUChar Value);
  inline bool __fastcall GetCPCKeycodeValid(void) { return mCPCKeycodeValid; };
  inline bool __fastcall GetPressed(void) { return mPressed; };
  inline void __fastcall SetPressed(bool Value) { mPressed = Value; };

protected:

public:
  // Properties
  __property WORD WINKeycode = { read=GetWINKeycode, write=SetWINKeycode };
  __property TShiftState WINShiftState = { read=GetWINShiftState, write=SetWINShiftState };
  __property tUChar CPCKeycode = { read=GetCPCKeycode, write=SetCPCKeycode };
  __property bool CPCKeycodeValid = { read=GetCPCKeycodeValid };
  __property bool Pressed = { read=GetPressed, write=SetPressed };

  // Public API
  __fastcall TInputKey();
  __fastcall ~TInputKey();
};

//---------------------------------------------------------------------------
#endif
