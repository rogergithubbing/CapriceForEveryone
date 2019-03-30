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


#ifndef TInputActionButtonH
#define TInputActionButtonH

#include "TInputAction.h"

// Events types
typedef void __fastcall (__closure *TInputButtonMouseEvent)(void);


class TInputActionButton : public TInputAction
{
private:
  //
  // Members
  //
  bool mMouseActive;
  TInputButtonMouseEvent mOnMouseDownEvent;
  TInputButtonMouseEvent mOnMouseUpEvent;
  //
  // Methods
  //
  inline TInputKey* __fastcall GetButtonKey(void) { return (TInputKey*)Items[0]; };
  void __fastcall SetButtonKey(TInputKey* ButtonKeyP);
  inline bool __fastcall GetMouseActive(void) { return mMouseActive; };
  inline void __fastcall SetMouseActive(bool Value) { mMouseActive = Value; };
  inline TInputButtonMouseEvent __fastcall GetOnMouseDownEvent(void) { return mOnMouseDownEvent; };
  inline void __fastcall SetOnMouseDownEvent(TInputButtonMouseEvent Value) { mOnMouseDownEvent = Value; };
  inline TInputButtonMouseEvent __fastcall GetOnMouseUpEvent(void) { return mOnMouseUpEvent; };
  inline void __fastcall SetOnMouseUpEvent(TInputButtonMouseEvent Value) { mOnMouseUpEvent = Value; };

public:
  //
  // Properties
  //
  __property TInputKey* ButtonKey = { read=GetButtonKey, write = SetButtonKey };
  __property bool MouseActive = { read=GetMouseActive, write=SetMouseActive };
  __property TInputButtonMouseEvent OnMouseDown = { read=GetOnMouseDownEvent, write=SetOnMouseDownEvent };
  __property TInputButtonMouseEvent OnMouseUp = { read=GetOnMouseUpEvent, write=SetOnMouseUpEvent };
  //
  // API
  //
  __fastcall TInputActionButton();
  virtual __fastcall ~TInputActionButton();

  void __fastcall OnChange(long NewValue);
};

//---------------------------------------------------------------------------
#endif
