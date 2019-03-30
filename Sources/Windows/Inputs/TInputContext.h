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


#ifndef TInputContextH
#define TInputContextH

#include "TInputDevice.h"

// Events types
typedef bool __fastcall (__closure *TInputWINKeyEvent)(WORD &Key, TShiftState Shift);
typedef void __fastcall (__closure *TInputCPCKeyEvent)(tUChar CPCKeycode);
typedef void __fastcall (__closure *TInputRemoveDevice)(AnsiString DeviceName);
typedef void __fastcall (__closure *TInputMouseDownEvent)(TMouseButton Button, int X, int Y);
typedef void __fastcall (__closure *TInputMouseUpEvent)(TMouseButton Button, int X, int Y);
typedef void __fastcall (__closure *TInputMouseMoveEvent)(int X, int Y);
typedef void __fastcall (__closure *TInputMouseClickEvent)(int X, int Y);


class TInputContext
{
private:
  //
  // Members
  //
  TInputWINKeyEvent mOnWINKeyDown;
  TInputWINKeyEvent mOnWINKeyUp;
  TInputCPCKeyEvent mOnCPCKeyDown;
  TInputCPCKeyEvent mOnCPCKeyUp;
  TInputRemoveDevice mOnRemoveDevice;
  TInputMouseDownEvent mOnMouseDown;
  TInputMouseUpEvent mOnMouseUp;
  TInputMouseMoveEvent mOnMouseMove;
  TInputMouseClickEvent mOnMouseClick;
  TInputMouseClickEvent mOnMouseDblClick;
  int mNbGameControllers;
  TInputDevice **mGameDevicesPP;
  AnsiString mProfileFilename;
  int mNbProfileDevices;
  TStringList **mProfileDevicesPP;
  TStringList* mInfoStrings;
#ifdef _USE_DIRECTINPUT8
  LPDIRECTINPUT8 mDirectInputP;
#endif /* _USE_DIRECTINPUT8 */
  int mLastMouseXPos;
  int mLastMouseYPos;
  bool mFullScreen;
  //
  // Methods
  //
  inline int __fastcall GetNbGameControllers() { return mNbGameControllers; };
  void __fastcall RemoveGameController(int RemoveDeviceIndex);
  inline TInputWINKeyEvent __fastcall GetOnWINKeyDown(void) { return mOnWINKeyDown; };
  inline void __fastcall SetOnWINKeyDown(TInputWINKeyEvent Value) { mOnWINKeyDown = Value; };
  inline TInputWINKeyEvent __fastcall GetOnWINKeyUp(void) { return mOnWINKeyUp; };
  inline void __fastcall SetOnWINKeyUp(TInputWINKeyEvent Value) { mOnWINKeyUp = Value; };
  inline void __fastcall SetOnCPCKeyDown(TInputCPCKeyEvent Value) { mOnCPCKeyDown = Value; };
  inline void __fastcall SetOnCPCKeyUp(TInputCPCKeyEvent Value) { mOnCPCKeyUp = Value; };
  inline void __fastcall SetOnRemoveDevice(TInputRemoveDevice Value) { mOnRemoveDevice = Value; };
  inline void __fastcall SetOnMouseDown(TInputMouseDownEvent Value) { mOnMouseDown = Value; };
  inline void __fastcall SetOnMouseUp(TInputMouseUpEvent Value) { mOnMouseUp = Value; };
  inline void __fastcall SetOnMouseMove(TInputMouseMoveEvent Value) { mOnMouseMove = Value; };
  inline void __fastcall SetOnMouseClick(TInputMouseClickEvent Value) { mOnMouseClick = Value; };
  inline void __fastcall SetOnMouseDblClick(TInputMouseClickEvent Value) { mOnMouseDblClick = Value; };
#ifdef _USE_DIRECTINPUT8
  inline LPDIRECTINPUT8 __fastcall GetDirectInput() { return mDirectInputP; };
#endif /* _USE_DIRECTINPUT8 */
  inline AnsiString GetProfileFilename(void) { return mProfileFilename; };
  void __fastcall SetProfileFilename(AnsiString Value);
  inline void __fastcall SetFullscreen(bool Value) { mFullScreen = Value; };
  void __fastcall CreateDefaultActions(TInputDevice* GameControllerP);
  bool __fastcall DummyKeyboardKeyDown(WORD &Key, TShiftState Shift);
  bool __fastcall DummyKeyboardKeyUp(WORD &Key, TShiftState Shift);
  void __fastcall DummyCPCKeycodeKeyDown(tUChar Keycode);
  void __fastcall DummyCPCKeycodeKeyUp(tUChar Keycode);

public:
  //
  // Properties
  //
  __property int NbGameControllers = { read=GetNbGameControllers };
  __property TInputWINKeyEvent OnWINKeyDown = { read=GetOnWINKeyDown, write=SetOnWINKeyDown };
  __property TInputWINKeyEvent OnWINKeyUp = { read=GetOnWINKeyUp, write=SetOnWINKeyUp };
  __property TInputCPCKeyEvent OnCPCKeyDown = { write=SetOnCPCKeyDown };
  __property TInputCPCKeyEvent OnCPCKeyUp = { write=SetOnCPCKeyUp };
  __property TInputRemoveDevice OnRemoveDevice = { write=SetOnRemoveDevice };
  __property TInputMouseDownEvent OnMouseDown = { write=SetOnMouseDown };
  __property TInputMouseUpEvent OnMouseUp = { write=SetOnMouseUp };
  __property TInputMouseMoveEvent OnMouseMove = { write=SetOnMouseMove };
  __property TInputMouseClickEvent OnMouseClick = { write=SetOnMouseClick };
  __property TInputMouseClickEvent OnMouseDblClick = { write=SetOnMouseDblClick };
#ifdef _USE_DIRECTINPUT8
  __property LPDIRECTINPUT8 DirectInput = { read=GetDirectInput };
#endif /* _USE_DIRECTINPUT8 */
  __property AnsiString ProfileFilename = { read=GetProfileFilename, write=SetProfileFilename };
  __property bool Fullscreen = { write=SetFullscreen };
  //
  // API
  //
  __fastcall TInputContext(void);
  __fastcall ~TInputContext();

  TStringList* __fastcall GetInfo(void);
  void __fastcall AddInfo(TStringList* InfoStrings);

  int __fastcall Init(void);
  bool __fastcall Start(HWND Handle);
  bool __fastcall Stop(void);

  AnsiString __fastcall GetDeviceName(int Index);
  TInputDevice* __fastcall GetDevice(int Index);
  void __fastcall ShowProperties(HWND Handle, int Index);

  bool __fastcall UpdateGameControllers(void);

  void __fastcall MouseDown(TMouseButton Button, int X, int Y);
  void __fastcall MouseUp(TMouseButton Button, int X, int Y);
  void __fastcall MouseMove(int X, int Y);
  void __fastcall MouseClick(void);
  void __fastcall MouseDblClick(void);
  void __fastcall OnInputMouseDown(void);
  void __fastcall OnInputMouseUp(void);
  void __fastcall OnInputMouseX(int X);
  void __fastcall OnInputMouseY(int Y);

  void __fastcall OnDisplayResize(int NewWidth, int NewHeight);
  
#ifdef _USE_DIRECTINPUT8
  void __fastcall AddGameController(LPDIRECTINPUTDEVICE8 ControllerP);
#endif /* _USE_DIRECTINPUT8 */

  void __fastcall AddButtonAction(TInputDevice* DeviceP,
                                  int Index,
                                  TInputKey* InputKeyP);
  void __fastcall AddButtonMouse(TInputDevice* DeviceP,
                                 int Index);
  void __fastcall AddPOVAction(TInputDevice* DeviceP,
                               int POVIndex,
                               int ButtonIndex,
                               TInputKey* InputKeyP);
  void __fastcall AddAnalogAction(TInputDevice* DeviceP,
                                  int AnalogIndex,
                                  int ButtonIndex,
                                  TInputKey* InputKeyP);
  void __fastcall AddAnalogMouseX(TInputDevice* DeviceP,
                                  int AnalogIndex);
  void __fastcall AddAnalogMouseY(TInputDevice* DeviceP,
                                  int AnalogIndex);
  void __fastcall AddDummyActions(void);
  void __fastcall DeleteDummyActions(void);

  static AnsiString __fastcall GetPressedKeyString(TInputKey* InputKeyP);
  static void __fastcall GetKeycodeFromProfileString(AnsiString KeyString, TInputKey* InputKeyP);
  static AnsiString __fastcall GetProfileStringFromKeycode(TInputKey* InputKeyP);

  void __fastcall NewGameControllersProfile(bool CreateDefault);
  void __fastcall LoadGameControllersProfile(TStringList* ProfileStrings);
  void __fastcall SaveGameControllersProfile(TStringList* ProfileStrings);
};

//---------------------------------------------------------------------------
#endif

