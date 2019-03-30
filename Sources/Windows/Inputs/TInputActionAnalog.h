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


#ifndef TInputActionAnalogH
#define TInputActionAnalogH

#include "TInputAction.h"

#define ANALOG_NBBUTTONS             2
#define ANALOG_INDEXBUTTON_MAX       0
#define ANALOG_INDEXBUTTON_MIN       1


// Events types
typedef void __fastcall (__closure *TInputAnalogMouseEvent)(int Pos);


class TInputActionAnalog : public TInputAction
{
private:
  //
  // Members
  //
  long mMaxValue;
  long mMinValue;
  long mPressThreshold;
  long mMaxThreshold;
  long mMinThreshold;
  long mDeadZone;
  bool mMouseXActive;
  bool mMouseYActive;
  int mDisplayWidth;
  int mDisplayHeight;
  TInputAnalogMouseEvent mOnMouseXEvent;
  TInputAnalogMouseEvent mOnMouseYEvent;
  //
  // Methods
  //
  inline int GetNbButtons(void) { return ANALOG_NBBUTTONS; };
  inline TInputKey* __fastcall GetButtonsKey(int Index) { return (TInputKey*)Items[Index]; };
  void __fastcall SetButtonsKey(int Index, TInputKey* ButtonKeyP);
  inline TInputKey* __fastcall GetButtonMaxKey(void) { return (TInputKey*)Items[ANALOG_INDEXBUTTON_MAX]; };
  void __fastcall SetButtonMaxKey(TInputKey* ButtonKeyP);
  inline TInputKey* __fastcall GetButtonMinKey(void) { return (TInputKey*)Items[ANALOG_INDEXBUTTON_MIN]; };
  void __fastcall SetButtonMinKey(TInputKey* ButtonKeyP);
  inline long __fastcall GetMaxValue(void) { return mMaxValue; };
  void __fastcall SetMaxValue(long Value);
  inline long __fastcall GetMinValue(void) { return mMinValue; };
  void __fastcall SetMinValue(long Value);
  inline long __fastcall GetPressThreshold(void) { return mPressThreshold; };
  void __fastcall SetPressThreshold(long Value);
  inline long __fastcall GetDeadZone(void) { return mDeadZone; };
  void __fastcall SetDeadZone(long Value);
  long __fastcall GetDeadZoneMin(void);
  long __fastcall GetDeadZoneMax(void);
  inline bool __fastcall GetMouseXActive(void) { return mMouseXActive; };
  inline void __fastcall SetMouseXActive(bool Value) { mMouseXActive = Value; };
  inline bool __fastcall GetMouseYActive(void) { return mMouseYActive; };
  inline void __fastcall SetMouseYActive(bool Value) { mMouseYActive = Value; };
  inline TInputAnalogMouseEvent __fastcall GetOnMouseXEvent(void) { return mOnMouseXEvent; };
  inline void __fastcall SetOnMouseXEvent(TInputAnalogMouseEvent Value) { mOnMouseXEvent = Value; };
  inline TInputAnalogMouseEvent __fastcall GetOnMouseYEvent(void) { return mOnMouseYEvent; };
  inline void __fastcall SetOnMouseYEvent(TInputAnalogMouseEvent Value) { mOnMouseYEvent = Value; };

public:
  //
  // Properties
  //
  __property int NbButtons = { read=GetNbButtons };
  __property TInputKey* Buttons[int Index] = { read=GetButtonsKey, write=SetButtonsKey };
  __property TInputKey* ButtonMaxKey = { read=GetButtonMaxKey, write=SetButtonMaxKey };
  __property TInputKey* ButtonMinKey = { read=GetButtonMinKey, write=SetButtonMinKey };
  __property long MaxValue = { read=GetMaxValue, write=SetMaxValue };
  __property long MinValue = { read=GetMinValue, write=SetMinValue };
  __property long PressThreshold = { read=GetPressThreshold, write=SetPressThreshold };
  __property long DeadZone = { read=GetDeadZone, write=SetDeadZone };
  __property long DeadZoneMin = { read=GetDeadZoneMin };
  __property long DeadZoneMax = { read=GetDeadZoneMax };
  __property bool MouseXActive = { read=GetMouseXActive, write=SetMouseXActive };
  __property bool MouseYActive = { read=GetMouseYActive, write=SetMouseYActive };
  __property TInputAnalogMouseEvent OnMouseX = { read=GetOnMouseXEvent, write=SetOnMouseXEvent };
  __property TInputAnalogMouseEvent OnMouseY = { read=GetOnMouseYEvent, write=SetOnMouseYEvent };
  //
  // API
  //
  __fastcall TInputActionAnalog();
  virtual __fastcall ~TInputActionAnalog();

  void __fastcall OnDisplayResize(int NewWidth, int NewHeight);
  void __fastcall OnChange(long NewValue);
};

//---------------------------------------------------------------------------
#endif
