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
#include <tchar.h>
#pragma hdrstop
#include "..\Inputs\Keyboard.h"
#include "TInputContext.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//=================================
// Options
#undef SIMU_ONE_GAMECONTROLLER
//#define SIMU_ONE_GAMECONTROLLER

#undef DEFAULT_ACTIONS
#define DEFAULT_ACTIONS
//=================================


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================



#ifdef _USE_DIRECTINPUT8
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID ContextP);
BOOL CALLBACK EnumJoysticksInfoCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID ContextP);
BOOL CALLBACK EnumObjectsCallback(LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID ContextP);
#endif /* _USE_DIRECTINPUT8 */


// Profile keywords
const AnsiString DeviceKeyword = "Device";
const AnsiString ButtonKeyword = "Button";
const AnsiString ButtonMouseKeyword = "Mouse";
const AnsiString POVKeyword = "POV";
const AnsiString POVButtonUpKeyword = "_ButtonUP";
const AnsiString POVButtonDownKeyword = "_ButtonDOWN";
const AnsiString POVButtonLeftKeyword = "_ButtonLEFT";
const AnsiString POVButtonRightKeyword = "_ButtonRIGHT";
const AnsiString AnalogKeyword = "Analog";
const AnsiString AnalogAxisKeyword[] =
{
  "XAxis",
  "YAxis",
  "ZAxis",
  "RxAxis",
  "RyAxis",
  "RzAxis"
};
const AnsiString AnalogButtonMaxKeyword = "_ButtonMax";
const AnsiString AnalogButtonMinKeyword = "_ButtonMin";
const AnsiString AnalogDeadZoneKeyword = "_DeadZone";
const AnsiString AnalogPressThresholdKeyword = "_PressThreshold";
const AnsiString AnalogMouseXKeyword = "_MouseX";
const AnsiString AnalogMouseYKeyword = "_MouseY";
const AnsiString SliderKeyword = "Slider";

const int AnalogRangeMax = 1000;
const int AnalogRangeMin = -1000;
const int AnalogPressThreshold = 50;


__fastcall TInputContext::TInputContext(void)
//
// Constructor
//
{
  mNbGameControllers = 0;
#ifdef _USE_DIRECTINPUT8
  mDirectInputP = NULL;
#endif /* _USE_DIRECTINPUT8 */
  mGameDevicesPP = NULL;
  mProfileFilename = "";
  mNbProfileDevices = 0;
  mProfileDevicesPP = NULL;
  mFullScreen = false;

  mOnWINKeyDown = DummyKeyboardKeyDown;
  mOnWINKeyUp = DummyKeyboardKeyUp;
  mOnCPCKeyDown = DummyCPCKeycodeKeyDown;
  mOnCPCKeyUp = DummyCPCKeycodeKeyUp;
  mOnRemoveDevice = NULL;
  mOnMouseDown = NULL;
  mOnMouseUp = NULL;
  mOnMouseMove = NULL;
  mOnMouseClick = NULL;
  mOnMouseDblClick = NULL;

  mInfoStrings = new TStringList;
  
  TPoint MousePos = Mouse->CursorPos;
  mLastMouseXPos = MousePos.x;
  mLastMouseYPos = MousePos.y;

}
//---------------------------------------------------------------------------


__fastcall TInputContext::~TInputContext()
//
// Destructor
//
{
#ifdef _USE_DIRECTINPUT8

  if (mDirectInputP != NULL)
  {
    mDirectInputP->Release();
  }

#endif /* _USE_DIRECTINPUT8 */

  for (int loop=0; loop < mNbGameControllers; loop++)
  {
    delete mGameDevicesPP[loop];
  }
  delete [] mGameDevicesPP;

  for (int loop=0; loop < mNbProfileDevices; loop++)
  {
    delete mProfileDevicesPP[loop];
  }
  delete [] mProfileDevicesPP;

  delete mInfoStrings;
}
//---------------------------------------------------------------------------


//
// Access
//
//---------------------------------------------------------------------------
void __fastcall TInputContext::SetProfileFilename(AnsiString Value)
{
  mProfileFilename = Value;
}
//---------------------------------------------------------------------------


TStringList* __fastcall TInputContext::GetInfo(void)
{
  TStringList* InfoStrings = new TStringList;

#ifdef _USE_DIRECTINPUT8

  // Enumerate all game controllers
  mInfoStrings->Clear();
	mDirectInputP->EnumDevices(DI8DEVCLASS_GAMECTRL,
														 EnumJoysticksInfoCallback,
														(LPVOID)this,
														DIEDFL_ATTACHEDONLY);

  InfoStrings->Add("DirectInput8");
  InfoStrings->Add("============");
  if (mInfoStrings->Count > 0)
  {
    InfoStrings->AddStrings(mInfoStrings);
  }
  else
  {
    InfoStrings->Add("");
  }

#endif /* _USE_DIRECTINPUT8 */

  return InfoStrings;
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::AddInfo(TStringList* InfoStrings)
{
  if (mInfoStrings)
  {
    mInfoStrings->AddStrings(InfoStrings);
  }

  delete InfoStrings;
}
//---------------------------------------------------------------------------


int __fastcall TInputContext::Init(void)
{
  do
  {

#ifdef _USE_DIRECTINPUT8

    HRESULT hr;

    // Create the abstract DirectInput connection
    hr = DirectInput8Create(GetModuleHandle(NULL),
                            DIRECTINPUT_VERSION,
                            IID_IDirectInput8,
                            (LPVOID*)&mDirectInputP,
                            NULL);
    if (FAILED(hr))
		{
#ifdef _DEBUG
			String msg = _T("DirectInput8Create Error=") + String::IntToHex(hr, 8);
			Application->MessageBox(msg.c_str(), _T("TInputContext::Init"), MB_OK);
#endif /* _DEBUG */
			continue;
		}

#ifdef SIMU_ONE_GAMECONTROLLER
		AddGameController(NULL);
#endif /* SIMU_ONE_GAMECONTROLLER */

		hr = mDirectInputP->EnumDevices(DI8DEVCLASS_GAMECTRL,
																		EnumJoysticksCallback,
																		(LPVOID)this,
																		DIEDFL_ATTACHEDONLY);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			String msg = _T("EnumDevices Error=") + String::IntToHex(hr, 8);
			Application->MessageBox(msg.c_str(), _T("TInputContext::Init"), MB_OK);
#endif /* _DEBUG */
			continue;
    }

#endif /* _USE_DIRECTINPUT8 */

  }
  while (0);

  return NbGameControllers;
}
//---------------------------------------------------------------------------


bool __fastcall TInputContext::Start(HWND Handle)
{
#ifdef _USE_DIRECTINPUT8

  HRESULT hr = E_INVALIDARG;

  for (int Index=0; Index < mNbGameControllers; Index++)
  {
    do
    {
      LPDIRECTINPUTDEVICE8 DeviceP = mGameDevicesPP[Index]->DirectInput8DeviceP;

#ifdef SIMU_ONE_GAMECONTROLLER
      if (DeviceP != NULL)
#endif /* SIMU_ONE_GAMECONTROLLER */
      {
        // Set the cooperative level to let DInput know how this device should
        // interact with the system and with other DInput applications.
        hr = DeviceP->SetCooperativeLevel(Handle,
                                          DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
        if (FAILED(hr))
        {
#ifdef _DEBUG
				  String msg = _T("SetCooperativeLevel Error=") + String::IntToHex(hr, 8);
				  Application->MessageBox(msg.c_str(), _T("TInputContext::AddGameController"), MB_OK);
#endif /* _DEBUG */
				  continue;
			  }

        // Set the data format to "simple joystick" - a predefined data format
        //
        // A data format specifies which controls on a device we are interested in,
        // and how they should be reported. This tells DInput that we will be
        // passing a DIJOYSTATE or DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
        hr = DeviceP->SetDataFormat(&c_dfDIJoystick);
        if (FAILED(hr))
        {
#ifdef _DEBUG
          String msg = _T("SetDataFormat Error=") + String::IntToHex(hr, 8);
          Application->MessageBox(msg.c_str(), _T("TInputContext::Start"), MB_OK);
#endif /* _DEBUG */
          continue;
        }
      }
#ifdef SIMU_ONE_GAMECONTROLLER
      else
      {
        hr = 0;
      }
#endif /* SIMU_ONE_GAMECONTROLLER */
    }
    while (0);

    if (FAILED(hr))
    {
      break;
    }
  }

  return (SUCCEEDED(hr) ? true : false);

#else /* _USE_DIRECTINPUT8 */

  return false;

#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------


bool __fastcall TInputContext::Stop(void)
{
#ifdef _USE_DIRECTINPUT8

  HRESULT hr = E_INVALIDARG;

  for (int Index=0; Index < mNbGameControllers; Index++)
  {
    LPDIRECTINPUTDEVICE8 DeviceP = mGameDevicesPP[Index]->DirectInput8DeviceP;

#ifdef SIMU_ONE_GAMECONTROLLER
    if (DeviceP != NULL)
    {
#endif /* SIMU_ONE_GAMECONTROLLER */

    HRESULT hr = DeviceP->Unacquire();
    if (FAILED(hr))
    {
#ifdef _DEBUG
			String msg = _T("Unacquire Error=") + String::IntToHex(hr, 8);
			Application->MessageBox(msg.c_str(), _T("TInputContext::Stop"), MB_OK);
#endif /* _DEBUG */
			break;
		}

#ifdef SIMU_ONE_GAMECONTROLLER
    }
#endif /* SIMU_ONE_GAMECONTROLLER */    
  }
  
  return (SUCCEEDED(hr) ? true : false);

#else /* _USE_DIRECTINPUT8 */

  return false;

#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------


AnsiString __fastcall TInputContext::GetDeviceName(int Index)
{
AnsiString DeviceName = "";

  if (Index < mNbGameControllers)
  {
#ifdef _USE_DIRECTINPUT8
    do
    {
      HRESULT hr;
      LPDIRECTINPUTDEVICE8 DeviceP = mGameDevicesPP[Index]->DirectInput8DeviceP;

#ifdef SIMU_ONE_GAMECONTROLLER
      if (DeviceP != NULL)
      {
#endif /* SIMU_ONE_GAMECONTROLLER */

      DIDEVICEINSTANCE didins;
      didins.dwSize = sizeof(didins);

      hr = DeviceP->GetDeviceInfo(&didins);
      if (FAILED(hr))
      {
#ifdef _DEBUG
				String msg = _T("GetDeviceInfo Error=") + String::IntToHex(hr, 8);
				Application->MessageBox(msg.c_str(), _T("TInputContext::GetDeviceName"), MB_OK);
#endif /* _DEBUG */
				continue;
      }

      DeviceName = didins.tszProductName;

#ifdef SIMU_ONE_GAMECONTROLLER
      }
      else
      {
        DeviceName = "Simu";
      }
#endif /* SIMU_ONE_GAMECONTROLLER */

    }
    while (0);
#endif /* _USE_DIRECTINPUT8 */
  }

  return DeviceName;
}
//---------------------------------------------------------------------------


TInputDevice* __fastcall TInputContext::GetDevice(int Index)
{
TInputDevice* DeviceP = NULL;

  if (Index < mNbGameControllers)
  {
    DeviceP = mGameDevicesPP[Index];
  }

  return DeviceP;
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::ShowProperties(HWND Handle, int Index)
{
  if (Index < mNbGameControllers)
  {
#ifdef _USE_DIRECTINPUT8
    do
    {
      LPDIRECTINPUTDEVICE8 DeviceP = mGameDevicesPP[Index]->DirectInput8DeviceP;

#ifdef SIMU_ONE_GAMECONTROLLER
      if (DeviceP != NULL)
      {
#endif /* SIMU_ONE_GAMECONTROLLER */

      HRESULT hr;
      hr = DeviceP->RunControlPanel(Handle, 0);
      if (FAILED(hr))
      {
#ifdef _DEBUG
				String msg = _T("RunControlPanel Error=") + String::IntToHex(hr, 8);
				Application->MessageBox(msg.c_str(), _T("TInputContext::ShowProperties"), MB_OK);
#endif /* _DEBUG */
				continue;
      }

#ifdef SIMU_ONE_GAMECONTROLLER
      }
#endif /* SIMU_ONE_GAMECONTROLLER */

    }
    while (0);
#endif /* _USE_DIRECTINPUT8 */
  }
}
//---------------------------------------------------------------------------


bool __fastcall TInputContext::UpdateGameControllers(void)
{
#ifdef _USE_DIRECTINPUT8
  bool Result = false;

  for (int Index=0; Index < mNbGameControllers; Index++)
  {
    HRESULT hr;
    

    do
    {
      LPDIRECTINPUTDEVICE8 DeviceP = mGameDevicesPP[Index]->DirectInput8DeviceP;
      DIJOYSTATE* CurrentDIJOYSTATEP;

#ifdef SIMU_ONE_GAMECONTROLLER
      if (DeviceP != NULL)
#endif /* SIMU_ONE_GAMECONTROLLER */
      {
        // Poll the device to read the current state
        hr = DeviceP->Poll();
        if (FAILED(hr))
        {
          // DInput is telling us that the input stream has been
          // interrupted. We aren't tracking any state between polls, so
          // we don't have any special reset that needs to be done. We
          // just re-acquire and try again.
          hr = DeviceP->Acquire();
          if (FAILED(hr))
          {
            continue;
          }
        }

        // Get the input's device state
        CurrentDIJOYSTATEP = new DIJOYSTATE;
        hr = DeviceP->GetDeviceState(sizeof(DIJOYSTATE),
                                     CurrentDIJOYSTATEP);
        if (FAILED(hr))
        {
          delete CurrentDIJOYSTATEP;
          continue;
        }
      }
#ifdef SIMU_ONE_GAMECONTROLLER
      else
      {
        if (mGameDevicesPP[Index]->FormGameControllerP->Visible == true)
        {
          hr = S_OK;
          CurrentDIJOYSTATEP = new DIJOYSTATE;
          mGameDevicesPP[Index]->FormGameControllerP->GetState(CurrentDIJOYSTATEP);
        }
        else
        {
          hr = DIERR_INPUTLOST;
          continue;
        }
      }
#endif /* SIMU_ONE_GAMECONTROLLER */

      // Device is responding
      mGameDevicesPP[Index]->ResetError();

      //
      // Perform actions
      //
      TInputDeviceActions* ActionsP = mGameDevicesPP[Index]->ActionsP;
      // First state read ?
      if (mGameDevicesPP[Index]->LastDIJOYSTATEP != NULL)
      {
        //
        // Check buttons
        //
        for (int loop=0; loop < mGameDevicesPP[Index]->NbButtons; loop++)
        {
          // Action defined ?
          if (ActionsP->Buttons[loop] != NULL)
          {
            // Button state has changed
            if (CurrentDIJOYSTATEP->rgbButtons[loop] != mGameDevicesPP[Index]->LastDIJOYSTATEP->rgbButtons[loop])
            {
              ActionsP->Buttons[loop]->OnChange(CurrentDIJOYSTATEP->rgbButtons[loop]);
              Result = true;
            }
          }
        }
        //
        // Check POV
        //
        for (int loop=0; loop < mGameDevicesPP[Index]->NbPOV; loop++)
        {
          // Action defined ?
          if (ActionsP->POVs[loop] != NULL)
          {
            // Button state has changed
            if (CurrentDIJOYSTATEP->rgdwPOV[loop] != mGameDevicesPP[Index]->LastDIJOYSTATEP->rgdwPOV[loop])
            {
              ActionsP->POVs[loop]->OnChange(CurrentDIJOYSTATEP->rgdwPOV[loop]);
              Result = true;
            }
          }
        }
        // Check Analogs
        if (mGameDevicesPP[Index]->XAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_XAXIS_INDEX] != NULL)
          {
            // Button state has changed
            if (abs(CurrentDIJOYSTATEP->lX - mGameDevicesPP[Index]->LastDIJOYSTATEP->lX) >
                ActionsP->Analogs[ANALOG_XAXIS_INDEX]->DeadZone)
            {
              ActionsP->Analogs[ANALOG_XAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lX);
              Result = true;
            }
          }
        }
        if (mGameDevicesPP[Index]->YAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_YAXIS_INDEX] != NULL)
          {
            // Button state has changed
            if (abs(CurrentDIJOYSTATEP->lY - mGameDevicesPP[Index]->LastDIJOYSTATEP->lY) >
                ActionsP->Analogs[ANALOG_YAXIS_INDEX]->DeadZone)
            {
              ActionsP->Analogs[ANALOG_YAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lY);
              Result = true;
            }
          }
        }
        if (mGameDevicesPP[Index]->ZAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_ZAXIS_INDEX] != NULL)
          {
            // Button state has changed
            if (abs(CurrentDIJOYSTATEP->lZ - mGameDevicesPP[Index]->LastDIJOYSTATEP->lZ) >
                ActionsP->Analogs[ANALOG_ZAXIS_INDEX]->DeadZone)
            {
              ActionsP->Analogs[ANALOG_ZAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lZ);
              Result = true;
            }
          }
        }
        if (mGameDevicesPP[Index]->RxAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_RXAXIS_INDEX] != NULL)
          {
            // Button state has changed
            if (abs(CurrentDIJOYSTATEP->lRx - mGameDevicesPP[Index]->LastDIJOYSTATEP->lRx) >
                ActionsP->Analogs[ANALOG_RXAXIS_INDEX]->DeadZone)
            {
              ActionsP->Analogs[ANALOG_RXAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lRx);
              Result = true;
            }
          }
        }
        if (mGameDevicesPP[Index]->RyAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_RYAXIS_INDEX] != NULL)
          {
            // Button state has changed
            if (abs(CurrentDIJOYSTATEP->lRy - mGameDevicesPP[Index]->LastDIJOYSTATEP->lRy) >
                ActionsP->Analogs[ANALOG_RYAXIS_INDEX]->DeadZone)
            {
              ActionsP->Analogs[ANALOG_RYAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lRy);
              Result = true;
            }
          }
        }
        if (mGameDevicesPP[Index]->RzAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_RZAXIS_INDEX] != NULL)
          {
            // Button state has changed
            if (abs(CurrentDIJOYSTATEP->lRz - mGameDevicesPP[Index]->LastDIJOYSTATEP->lRz) >
                ActionsP->Analogs[ANALOG_RZAXIS_INDEX]->DeadZone)
            {
              ActionsP->Analogs[ANALOG_RZAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lRz);
              Result = true;
            }
          }
        }
        //
        // Check Sliders
        //
        for (int loop=0; loop < mGameDevicesPP[Index]->NbSliders; loop++)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_SLIDER1_INDEX+loop] != NULL)
          {
            // Button state has changed
            if (abs(CurrentDIJOYSTATEP->rglSlider[loop] - mGameDevicesPP[Index]->LastDIJOYSTATEP->rglSlider[loop]) >
                ActionsP->Analogs[ANALOG_SLIDER1_INDEX+loop]->DeadZone)
            {
              ActionsP->Analogs[ANALOG_SLIDER1_INDEX+loop]->OnChange(CurrentDIJOYSTATEP->rglSlider[loop]);
              Result = true;
            }
          }
        }
      }
      else
      {
        //
        // Init button state
        //
        for (int loop=0; loop < mGameDevicesPP[Index]->NbButtons; loop++)
        {
          // Action defined ?
          if (ActionsP->Buttons[loop] != NULL)
          {
            ActionsP->Buttons[loop]->OnChange(CurrentDIJOYSTATEP->rgbButtons[loop]);
          }
        }
        //
        // Init POV State
        //
        for (int loop=0; loop < mGameDevicesPP[Index]->NbPOV; loop++)
        {
          // Action defined ?
          if (ActionsP->POVs[loop] != NULL)
          {
            ActionsP->POVs[loop]->OnChange(CurrentDIJOYSTATEP->rgdwPOV[loop]);
          }
        }
        //
        // Init Analog State
        //
        if (mGameDevicesPP[Index]->XAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_XAXIS_INDEX] != NULL)
          {
            ActionsP->Analogs[ANALOG_XAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lX);
          }
        }
        if (mGameDevicesPP[Index]->YAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_YAXIS_INDEX] != NULL)
          {
            ActionsP->Analogs[ANALOG_YAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lY);
          }
        }
        if (mGameDevicesPP[Index]->ZAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_ZAXIS_INDEX] != NULL)
          {
            ActionsP->Analogs[ANALOG_ZAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lZ);
          }
        }
        if (mGameDevicesPP[Index]->RxAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_RXAXIS_INDEX] != NULL)
          {
            ActionsP->Analogs[ANALOG_RXAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lRx);
          }
        }
        if (mGameDevicesPP[Index]->RyAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_RYAXIS_INDEX] != NULL)
          {
            ActionsP->Analogs[ANALOG_RYAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lRy);
          }
        }
        if (mGameDevicesPP[Index]->RzAxisEnable == true)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_RZAXIS_INDEX] != NULL)
          {
            ActionsP->Analogs[ANALOG_RZAXIS_INDEX]->OnChange(CurrentDIJOYSTATEP->lRz);
          }
        }
        //
        // Init Sliders State
        //
        for (int loop=0; loop < mGameDevicesPP[Index]->NbSliders; loop++)
        {
          // Action defined ?
          if (ActionsP->Analogs[ANALOG_SLIDER1_INDEX+loop] != NULL)
          {
            ActionsP->Analogs[ANALOG_SLIDER1_INDEX+loop]->OnChange(CurrentDIJOYSTATEP->rglSlider[loop]);
          }
        }
        Result = true;
      }

      // Save last device state
      delete mGameDevicesPP[Index]->LastDIJOYSTATEP;
      mGameDevicesPP[Index]->LastDIJOYSTATEP = CurrentDIJOYSTATEP;
    }
    while (0);

    // Check device OK
    if (FAILED(hr))
    {
      // Increment not responding error
      if (mGameDevicesPP[Index]->IncrementError() == true)
      {
        RemoveGameController(Index);
      }

      Result = false;
    }
  }

  return Result;
#else /* _USE_DIRECTINPUT8 */
  return false;
#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------


bool __fastcall TInputContext::DummyKeyboardKeyDown(WORD &Key, TShiftState Shift)
{
  NOT_USED(Key);
  NOT_USED(Shift);

  return false;
}
//---------------------------------------------------------------------------
bool __fastcall TInputContext::DummyKeyboardKeyUp(WORD &Key, TShiftState Shift)
{
  NOT_USED(Key);
  NOT_USED(Shift);

  return false;
}
//---------------------------------------------------------------------------

void __fastcall TInputContext::DummyCPCKeycodeKeyDown(tUChar Keycode)
{
  NOT_USED(Keycode);
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::DummyCPCKeycodeKeyUp(tUChar Keycode)
{
  NOT_USED(Keycode);
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::MouseDown(TMouseButton Button, int X, int Y)
{
  // Update mouse position
  mLastMouseXPos = X;
  mLastMouseYPos = Y;

  if (mOnMouseDown)
  {
    mOnMouseDown(Button, mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::MouseUp(TMouseButton Button, int X, int Y)
{
  // Update mouse position
  mLastMouseXPos = X;
  mLastMouseYPos = Y;

  if (mOnMouseUp)
  {
    mOnMouseUp(Button, mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::MouseMove(int X, int Y)
{
  // Update mouse position
  mLastMouseXPos = X;
  mLastMouseYPos = Y;

  if (mOnMouseMove)
  {
    mOnMouseMove(mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::MouseClick(void)
{
  if (mOnMouseClick)
  {
    mOnMouseClick(mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::MouseDblClick(void)
{
  if (mOnMouseDblClick)
  {
    mOnMouseDblClick(mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::OnInputMouseDown(void)
{
TMouseButton Button = mbLeft;

  if ( (mFullScreen == true) && (mOnMouseDown) )
  {
    mOnMouseDown(Button, mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::OnInputMouseUp(void)
{
TMouseButton Button = mbLeft;

  if ( (mFullScreen == true) && (mOnMouseUp) )
  {
    mOnMouseUp(Button, mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::OnInputMouseX(int X)
{
  // Update mouse position
  mLastMouseXPos = X;

  if ( (mFullScreen == true) && (mOnMouseMove) )
  {
    Mouse->CursorPos = TPoint(mLastMouseXPos, mLastMouseYPos);
    mOnMouseMove(mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::OnInputMouseY(int Y)
{
  // Update mouse position
  mLastMouseYPos = Y;

  if ( (mFullScreen == true) && (mOnMouseMove) )
  {
    Mouse->CursorPos = TPoint(mLastMouseXPos, mLastMouseYPos);
    mOnMouseMove(mLastMouseXPos, mLastMouseYPos);
  }
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::OnDisplayResize(int NewWidth,
                                               int NewHeight)
{
  for (int devices=0; devices < mNbGameControllers; devices++)
  {
    TInputDeviceActions* ActionsP = mGameDevicesPP[devices]->ActionsP;

    for (int Analog=0; Analog < mGameDevicesPP[devices]->NbAnalog; Analog++)
    {
      if (ActionsP->Analogs[Analog] != NULL)
      {
        ActionsP->Analogs[Analog]->OnDisplayResize(NewWidth, NewHeight);
      }
    }

    for (int Analog=0; Analog < mGameDevicesPP[devices]->NbSliders; Analog++)
    {
      if (ActionsP->Analogs[ANALOG_SLIDER1_INDEX+Analog] != NULL)
      {
        ActionsP->Analogs[ANALOG_SLIDER1_INDEX+Analog]->OnDisplayResize(NewWidth, NewHeight);
      }
    }
  }
}
//---------------------------------------------------------------------------


#ifdef _USE_DIRECTINPUT8
void __fastcall TInputContext::AddGameController(LPDIRECTINPUTDEVICE8 ControllerP)
{
  TInputDevice* DeviceP = new TInputDevice;
  HRESULT hr;

  do
  {
    // Init device
    DeviceP->DirectInput8DeviceP = ControllerP;

#ifdef SIMU_ONE_GAMECONTROLLER
    if (ControllerP != NULL)
    {
#endif /* SIMU_ONE_GAMECONTROLLER */

    // Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
    hr = ControllerP->EnumObjects(EnumObjectsCallback,
                                  (LPVOID)DeviceP,
                                  DIDFT_ALL);
    if (FAILED(hr))
    {
#ifdef _DEBUG
			String msg = _T("EnumObjects Error=") + String::IntToHex(hr, 8);
			Application->MessageBox(msg.c_str(), _T("TInputContext::AddGameController"), MB_OK);
#endif /* _DEBUG */
			continue;
    }
    
#ifdef SIMU_ONE_GAMECONTROLLER
    }
    else
    {
      DeviceP->FormGameControllerP = new TFormDebugGameController(NULL);
      DeviceP->FormGameControllerP->Show();
      DeviceP->NbButtons = DeviceP->FormGameControllerP->NbButtons;
      DeviceP->NbPOV = DeviceP->FormGameControllerP->NbPOV;
      DeviceP->XAxisEnable = DeviceP->FormGameControllerP->XAxisEnable;
      DeviceP->YAxisEnable = DeviceP->FormGameControllerP->YAxisEnable;
      DeviceP->ZAxisEnable = DeviceP->FormGameControllerP->ZAxisEnable;
      DeviceP->RxAxisEnable = DeviceP->FormGameControllerP->RxAxisEnable;
    }
#endif /* SIMU_ONE_GAMECONTROLLER */

    // Check USE of DIJOYSTATE2
    if ( (DeviceP->NbButtons > DEVICEACTIONS_NBBUTTONS)
         || (DeviceP->NbPOV > DEVICEACTIONS_NBPOV) )
    {
      DeviceP->UseDIJOYSTATE2 = true;
    }

    //
    // Add game controller
    //
    // Store previous list
    TInputDevice** PreviousList = mGameDevicesPP;
    // Create new list
    mNbGameControllers++;
    mGameDevicesPP = new TInputDevice*[mNbGameControllers];
    if (PreviousList != NULL)
    {
      // Copy previous content
      for (int Index=0; Index < (mNbGameControllers - 1); Index++)
      {
        mGameDevicesPP[Index] = PreviousList[Index];
      }

      // Delete previous list
      delete [] PreviousList;
    }

    // store new game controler.
    mGameDevicesPP[mNbGameControllers - 1] = DeviceP;
  }
  while (0);

  if (FAILED(hr))
  {
    delete DeviceP;
  }
}
//---------------------------------------------------------------------------
#endif /* _USE_DIRECTINPUT8 */


void __fastcall TInputContext::RemoveGameController(int RemoveDeviceIndex)
{
  // Store name of remove device
  AnsiString RemoveDeviceName = GetDeviceName(RemoveDeviceIndex);

  // Store previous list
  TInputDevice** PreviousList = mGameDevicesPP;
  if (PreviousList != NULL)
  {
    // Create new list
    mNbGameControllers--;
    if (mNbGameControllers > 0)
    {
      mGameDevicesPP = new TInputDevice*[mNbGameControllers];

      // Copy previous content without callback to remove
      for (int Index=0, NewListIndex=0; Index < (mNbGameControllers + 1); Index++)
      {
        if (Index != RemoveDeviceIndex)
        {
          mGameDevicesPP[NewListIndex] = PreviousList[Index];
          NewListIndex++;
        }
      }
    }
    else
    {
      mGameDevicesPP = NULL;
    }

    // Delete previous list
    delete [] PreviousList;
  }

  // Signal device has been removed
  if (mOnRemoveDevice != NULL)
  {
    mOnRemoveDevice(RemoveDeviceName);
  }
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::AddButtonAction(TInputDevice* DeviceP,
                                               int Index,
                                               TInputKey* InputKeyP)
{
TInputActionButton* ButtonActionP = DeviceP->ActionsP->Buttons[Index];

  if (ButtonActionP == NULL)
  {
    ButtonActionP = new TInputActionButton();
    DeviceP->ActionsP->Buttons[Index] = ButtonActionP;
  }
  
  ButtonActionP->ButtonKey = InputKeyP;
  ButtonActionP->OnWINButtonDown = mOnWINKeyDown;
  ButtonActionP->OnWINButtonUp = mOnWINKeyUp;
  ButtonActionP->OnCPCButtonDown = mOnCPCKeyDown;
  ButtonActionP->OnCPCButtonUp = mOnCPCKeyUp;
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::AddButtonMouse(TInputDevice* DeviceP,
                                              int Index)
{
TInputActionButton* ButtonActionP = DeviceP->ActionsP->Buttons[Index];

  if (ButtonActionP == NULL)
  {
    ButtonActionP = new TInputActionButton();
    DeviceP->ActionsP->Buttons[Index] = ButtonActionP;
  }
  
  ButtonActionP->MouseActive = true;
  ButtonActionP->OnMouseDown = OnInputMouseDown;
  ButtonActionP->OnMouseUp = OnInputMouseUp;
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::AddPOVAction(TInputDevice* DeviceP,
                                            int POVIndex,
                                            int ButtonIndex,
                                            TInputKey* InputKeyP)
{
  if (DeviceP->ActionsP->POVs[POVIndex] == NULL)
  {
    TInputActionPOV* ActionP = new TInputActionPOV;
    ActionP->OnWINButtonDown = mOnWINKeyDown;
    ActionP->OnWINButtonUp = mOnWINKeyUp;
    ActionP->OnCPCButtonDown = mOnCPCKeyDown;
    ActionP->OnCPCButtonUp = mOnCPCKeyUp;
    DeviceP->ActionsP->POVs[POVIndex] = ActionP;
  }

  DeviceP->ActionsP->AddPOVButtonAction(POVIndex,
                                        ButtonIndex,
                                        InputKeyP);
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::AddAnalogAction(TInputDevice* DeviceP,
                                               int AnalogIndex,
                                               int ButtonIndex,
                                               TInputKey* InputKeyP)
{
TInputActionAnalog* ActionP = DeviceP->ActionsP->Analogs[AnalogIndex];

  if (ActionP == NULL)
  {
    ActionP = new TInputActionAnalog;
    ActionP->MaxValue = AnalogRangeMax;
    ActionP->MinValue = AnalogRangeMin;
    ActionP->PressThreshold = AnalogPressThreshold;
    DeviceP->ActionsP->Analogs[AnalogIndex] = ActionP;
  }

  ActionP->OnWINButtonDown = mOnWINKeyDown;
  ActionP->OnWINButtonUp = mOnWINKeyUp;
  ActionP->OnCPCButtonDown = mOnCPCKeyDown;
  ActionP->OnCPCButtonUp = mOnCPCKeyUp;

  DeviceP->ActionsP->AddAnalogButtonAction(AnalogIndex,
                                           ButtonIndex,
                                           InputKeyP);
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::AddAnalogMouseX(TInputDevice* DeviceP,
                                               int AnalogIndex)
{
TInputActionAnalog* ActionP = DeviceP->ActionsP->Analogs[AnalogIndex];

  if (ActionP == NULL)
  {
    ActionP = new TInputActionAnalog;
    ActionP->MaxValue = AnalogRangeMax;
    ActionP->MinValue = AnalogRangeMin;
    ActionP->PressThreshold = AnalogPressThreshold;
    DeviceP->ActionsP->Analogs[AnalogIndex] = ActionP;
  }

  ActionP->MouseXActive = true;
  ActionP->OnMouseX = OnInputMouseX;
}
//---------------------------------------------------------------------------
void __fastcall TInputContext::AddAnalogMouseY(TInputDevice* DeviceP,
                                               int AnalogIndex)
{
TInputActionAnalog* ActionP = DeviceP->ActionsP->Analogs[AnalogIndex];

  if (ActionP == NULL)
  {
    ActionP = new TInputActionAnalog;
    ActionP->OnMouseX = OnInputMouseX;
    ActionP->OnMouseY = OnInputMouseY;
    DeviceP->ActionsP->Analogs[AnalogIndex] = ActionP;
  }

  ActionP->MouseYActive = true;
  ActionP->OnMouseY = OnInputMouseY;
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::AddDummyActions(void)
{
  for (int devices=0; devices < mNbGameControllers; devices++)
  {
    TInputDeviceActions* ActionsP = mGameDevicesPP[devices]->ActionsP;

    for (int buttons=0; buttons < mGameDevicesPP[devices]->NbButtons; buttons++)
    {
      if (ActionsP->Buttons[buttons] == NULL)
      {
        // Create dummy button action
        TInputActionButton* DummyActionP = new TInputActionButton();
        DummyActionP->OnWINButtonDown = mOnWINKeyDown;
        DummyActionP->OnWINButtonUp = mOnWINKeyUp;
        DummyActionP->OnCPCButtonDown = mOnCPCKeyDown;
        DummyActionP->OnCPCButtonUp = mOnCPCKeyUp;
        ActionsP->Buttons[buttons] = DummyActionP;
      }
    }

    for (int POV=0; POV < mGameDevicesPP[devices]->NbPOV; POV++)
    {
      if (ActionsP->POVs[POV] == NULL)
      {
        // Create dummy button action
        TInputActionPOV* DummyActionP = new TInputActionPOV();
        DummyActionP->OnWINButtonDown = mOnWINKeyDown;
        DummyActionP->OnWINButtonUp = mOnWINKeyUp;
        DummyActionP->OnCPCButtonDown = mOnCPCKeyDown;
        DummyActionP->OnCPCButtonUp = mOnCPCKeyUp;
        ActionsP->POVs[POV] = DummyActionP;
      }
    }

    for (int Analog=0; Analog < mGameDevicesPP[devices]->NbAnalog; Analog++)
    {
      if (ActionsP->Analogs[Analog] == NULL)
      {
        // Create dummy button action
        TInputActionAnalog* DummyActionP = new TInputActionAnalog();
        DummyActionP->OnWINButtonDown = mOnWINKeyDown;
        DummyActionP->OnWINButtonUp = mOnWINKeyUp;
        DummyActionP->OnCPCButtonDown = mOnCPCKeyDown;
        DummyActionP->OnCPCButtonUp = mOnCPCKeyUp;
        DummyActionP->OnMouseX = OnInputMouseX;
        DummyActionP->OnMouseY = OnInputMouseY;
        DummyActionP->MaxValue = AnalogRangeMax;
        DummyActionP->MinValue = AnalogRangeMin;
        DummyActionP->PressThreshold = AnalogPressThreshold;
        ActionsP->Analogs[Analog] = DummyActionP;
      }
    }

    for (int Analog=0; Analog < mGameDevicesPP[devices]->NbSliders; Analog++)
    {
      if (ActionsP->Analogs[ANALOG_SLIDER1_INDEX+Analog] == NULL)
      {
        // Create dummy button action
        TInputActionAnalog* DummyActionP = new TInputActionAnalog();
        DummyActionP->OnWINButtonDown = mOnWINKeyDown;
        DummyActionP->OnWINButtonUp = mOnWINKeyUp;
        DummyActionP->OnCPCButtonDown = mOnCPCKeyDown;
        DummyActionP->OnCPCButtonUp = mOnCPCKeyUp;
        DummyActionP->OnMouseX = OnInputMouseX;
        DummyActionP->OnMouseY = OnInputMouseY;
        DummyActionP->MaxValue = AnalogRangeMax;
        DummyActionP->MinValue = AnalogRangeMin;
        DummyActionP->PressThreshold = AnalogPressThreshold;
        ActionsP->Analogs[ANALOG_SLIDER1_INDEX+Analog] = DummyActionP;
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::DeleteDummyActions(void)
{
  for (int devices=0; devices < mNbGameControllers; devices++)
  {
    TInputDeviceActions* ActionsP = mGameDevicesPP[devices]->ActionsP;

    for (int buttons=0; buttons < mGameDevicesPP[devices]->NbButtons; buttons++)
    {
      if (    (ActionsP->Buttons[buttons]->ButtonKey->WINKeycode == 0)
           && (ActionsP->Buttons[buttons]->ButtonKey->CPCKeycodeValid == false)
           && (ActionsP->Buttons[buttons]->MouseActive == false) )
      {
        ActionsP->Buttons[buttons] = NULL;
      }
    }

    for (int POV=0; POV < mGameDevicesPP[devices]->NbPOV; POV++)
    {
      TInputActionPOV* POVP = ActionsP->POVs[POV];
      if (    (POVP->ButtonUpKey->WINKeycode == 0)
           && (POVP->ButtonUpKey->CPCKeycodeValid == false)
           && (POVP->ButtonDownKey->WINKeycode == 0)
           && (POVP->ButtonDownKey->CPCKeycodeValid == false)
           && (POVP->ButtonLeftKey->WINKeycode == 0)
           && (POVP->ButtonLeftKey->CPCKeycodeValid == false)
           && (POVP->ButtonRightKey->WINKeycode == 0)
           && (POVP->ButtonRightKey->CPCKeycodeValid == false) )
      {
        ActionsP->POVs[POV] = NULL;
      }
    }

    for (int Analog=0; Analog < mGameDevicesPP[devices]->NbAnalog; Analog++)
    {
      TInputActionAnalog* AnalogP = ActionsP->Analogs[Analog];
      if (    (AnalogP->ButtonMaxKey->WINKeycode == 0)
           && (AnalogP->ButtonMaxKey->CPCKeycodeValid == false)
           && (AnalogP->ButtonMinKey->WINKeycode == 0)
           && (AnalogP->ButtonMinKey->CPCKeycodeValid == false)
           && (AnalogP->MouseXActive == false)
           && (AnalogP->MouseYActive == false) )
      {
        ActionsP->Analogs[Analog] = NULL;
      }
    }

    for (int Analog=0; Analog < mGameDevicesPP[devices]->NbSliders; Analog++)
    {
      TInputActionAnalog* AnalogP = ActionsP->Analogs[ANALOG_SLIDER1_INDEX+Analog];
      if (    (AnalogP->ButtonMaxKey->WINKeycode == 0)
           && (AnalogP->ButtonMaxKey->CPCKeycodeValid == false)
           && (AnalogP->ButtonMinKey->WINKeycode == 0)
           && (AnalogP->ButtonMinKey->CPCKeycodeValid == false)
           && (AnalogP->MouseXActive == false)
           && (AnalogP->MouseYActive == false) )
      {
        ActionsP->Analogs[ANALOG_SLIDER1_INDEX+Analog] = NULL;
      }
    }
  }
}
//---------------------------------------------------------------------------


AnsiString __fastcall TInputContext::GetPressedKeyString(TInputKey* InputKeyP)
{
AnsiString KeyID = "";

  // CPC Key ?
  if (InputKeyP->CPCKeycodeValid == true)
  {
    KeyID = cpc_keys_string[InputKeyP->CPCKeycode];
  }
  // Windows Key
  else
  {
    KeyID = virtual_kbd_string[(tUChar)InputKeyP->WINKeycode];
    if (KeyID.IsEmpty() == true)
    {
      // Key CTRL released or key ALTGR pressed
      if ( (!InputKeyP->WINShiftState.Contains(ssCtrl))
           || (InputKeyP->WINShiftState.Contains(ssCtrl) && InputKeyP->WINShiftState.Contains(ssAlt)) )
      {
        TKeyboardState ks;
        GetKeyboardState(ks);
        if (InputKeyP->WINShiftState.Contains(ssShift))
        {
          ks[VK_SHIFT] |= 0x80;
        }
        WORD AsciiKey;
        int keycode = ToAscii(InputKeyP->WINKeycode,
                              MapVirtualKey(InputKeyP->WINKeycode, 0),
                              ks,
                              &AsciiKey,
                              0);
        if (keycode == 1)
        {
          KeyID = ascii_kbd_string[(tUChar)AsciiKey];
        }
      }
      // Key CTRL released
      else if (!InputKeyP->WINShiftState.Contains(ssCtrl))
      {
        KeyID = ascii_kbd_string[(tUChar)InputKeyP->WINKeycode];
      }
    }

    if (KeyID.IsEmpty() == false)
    {
      if (InputKeyP->WINShiftState.Contains(ssShift))
      {
        KeyID = AnsiString(shift_key_string) + "+" + KeyID;
      }
      if (InputKeyP->WINShiftState.Contains(ssCtrl))
      {
        KeyID = AnsiString(ctrl_key_string) + "+" + KeyID;
      }
    }
  }

  return KeyID;
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::GetKeycodeFromProfileString(AnsiString KeyString, TInputKey* InputKeyP)
{

  InputKeyP->WINShiftState = TShiftState();

  if (KeyString.Pos(AnsiString(ctrl_key_string)) == 1)
  {
    InputKeyP->WINShiftState = InputKeyP->WINShiftState << ssCtrl;
    KeyString = KeyString.SubString(AnsiString(ctrl_key_string).Length() + 2, KeyString.Length()); // Add "+"
  }
  if (KeyString.Pos(AnsiString(shift_key_string)) == 1)
  {
    InputKeyP->WINShiftState = InputKeyP->WINShiftState << ssShift;
    KeyString = KeyString.SubString(AnsiString(shift_key_string).Length() + 2, KeyString.Length()); // Add "+"
  }

  // IS there CPC Key ?
  AnsiString CPCKey = "";
  int SeparatorPos = KeyString.Pos(",");
  if (SeparatorPos > 0)
  {
    CPCKey = KeyString.SubString(SeparatorPos+1, KeyString.Length());
    KeyString = KeyString.SubString(0, SeparatorPos-1);

    try
    {
      InputKeyP->CPCKeycode = (tUChar)CPCKey.ToInt();
    }
    catch(...)
    {
    }
  }

  try
  {
    InputKeyP->WINKeycode = (WORD)KeyString.ToInt();
  }
  catch(...)
  {
    InputKeyP->WINKeycode = 0;
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TInputContext::GetProfileStringFromKeycode(TInputKey* InputKeyP)
{
AnsiString ProfileString = "";

  if (InputKeyP->WINShiftState.Contains(ssCtrl))
  {
    ProfileString += AnsiString(ctrl_key_string) + "+";
  }
  if (InputKeyP->WINShiftState.Contains(ssShift))
  {
    ProfileString += AnsiString(shift_key_string) + "+";
  }

  ProfileString += AnsiString(InputKeyP->WINKeycode);

  if (InputKeyP->CPCKeycodeValid == true)
  {
    ProfileString += "," + AnsiString(InputKeyP->CPCKeycode);
  }

  return (ProfileString);
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::NewGameControllersProfile(bool CreateDefault)
{
  // Reset all actions
  for (int Index=0; Index < mNbGameControllers; Index++)
  {
    mGameDevicesPP[Index]->ActionsP = new TInputDeviceActions;
    mGameDevicesPP[Index]->ProfileLoaded = false;
  }

  // Release profile devices
  if (mProfileDevicesPP != NULL)
  {
    for (int loop=0; loop < mNbProfileDevices; loop++)
    {
      delete mProfileDevicesPP[loop];
    }
    delete [] mProfileDevicesPP;
    mProfileDevicesPP = NULL;
    mNbProfileDevices = 0;
  }

#ifdef DEFAULT_ACTIONS
  if (CreateDefault == true)
  {
    // Add default actions
    for (int Index=0; Index < mNbGameControllers; Index++)
    {
      CreateDefaultActions(mGameDevicesPP[Index]);
    }
  }
#else /* DEFAULT_ACTIONS */
  NOT_USED(CreateDefault);
#endif /* DEFAULT_ACTIONS */
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::LoadGameControllersProfile(TStringList* ProfileStrings)
{
  TInputDevice* SelectedDevice = NULL;
    
  // Erase previous profile
  NewGameControllersProfile(false);

  // Brows all profile strings
  for (int StringIndex=0; StringIndex < ProfileStrings->Count; StringIndex++)
  {
    AnsiString Text = ProfileStrings->Strings[StringIndex];

    // Ignore empty lines
    if (Text.Trim().Length() == 0)
    {
      continue;
    }
    
    // Device name
    if (Text.Pos(DeviceKeyword) == 1)
    {
      AnsiString DeviceName = Text.SubString(Text.Pos("=")+1, Text.Length());
      SelectedDevice = NULL;
      for (int Loop=0; Loop < mNbGameControllers; Loop++)
      {
        if (DeviceName == GetDeviceName(Loop))
        {
          if (mGameDevicesPP[Loop]->ProfileLoaded == false)
          {
            SelectedDevice = mGameDevicesPP[Loop];
            mGameDevicesPP[Loop]->ProfileLoaded = true;
          }
        }
      }

      // Add not available profile device
      if (SelectedDevice == NULL)
      {
        // Store previous list
        TStringList** PreviousList = mProfileDevicesPP;
        // Create new list
        mNbProfileDevices++;
        mProfileDevicesPP = new TStringList*[mNbProfileDevices];
        if (PreviousList != NULL)
        {
          // Copy previous content
          for (int Index=0; Index < (mNbProfileDevices - 1); Index++)
          {
            mProfileDevicesPP[Index] = PreviousList[Index];
          }

          // Delete previous list
          delete [] PreviousList;
        }

        // create new profile device
        TStringList* ProfileDeviceP = new TStringList;
        mProfileDevicesPP[mNbProfileDevices - 1] = ProfileDeviceP;

        // Add string to profile device
        mProfileDevicesPP[mNbProfileDevices - 1]->Add(Text);
      }
    }
    else if (SelectedDevice != NULL)
    {
      //
      // Check button
      //
      if (Text.Pos(ButtonKeyword) == 1)
      {
        // Get Button index
        int Index = Text.SubString(ButtonKeyword.Length() + 1,
                                   Text.Pos("=") - ButtonKeyword.Length() - 1).ToInt();

        if (Text.Pos(ButtonMouseKeyword) == 0)
        {
          // Get Button action ID
          AnsiString ActionID = Text.SubString(Text.Pos("=") +1, Text.Length());

          // Create button action
          TInputKey* InputKeyP = new TInputKey;
          GetKeycodeFromProfileString(ActionID, InputKeyP);
          AddButtonAction(SelectedDevice, Index-1, InputKeyP);
        }
        else
        {
          // Mouse button
          AddButtonMouse(SelectedDevice, Index-1);
        }
      }
      //
      // Check POV
      //
      else if (Text.Pos(POVKeyword) == 1)
      {
        // Get POV index
        int Index = Text.SubString(POVKeyword.Length() + 1,
                                   Text.Pos("_") - POVKeyword.Length() - 1).ToInt();
        // Get POV Button action ID
        AnsiString ActionID = Text.SubString(Text.Pos("=") +1, Text.Length());

        // Create button action
        TInputKey* InputKeyP = new TInputKey;
        GetKeycodeFromProfileString(ActionID, InputKeyP);

        if (Text.Pos(POVButtonUpKeyword) != 0)
        {
          AddPOVAction(SelectedDevice, Index-1, POV_INDEXBUTTON_UP, InputKeyP);
        }
        else if (Text.Pos(POVButtonDownKeyword) != 0)
        {
          AddPOVAction(SelectedDevice, Index-1, POV_INDEXBUTTON_DOWN, InputKeyP);
        }
        else if (Text.Pos(POVButtonLeftKeyword) != 0)
        {
          AddPOVAction(SelectedDevice, Index-1, POV_INDEXBUTTON_LEFT, InputKeyP);
        }
        else if (Text.Pos(POVButtonRightKeyword) != 0)
        {
          AddPOVAction(SelectedDevice, Index-1, POV_INDEXBUTTON_RIGHT, InputKeyP);
        }
        else
        {
          delete InputKeyP;
        }
      }
      //
      // Check Analogs
      //
      else if (Text.Pos(AnalogKeyword) == 1)
      {
        // Get Axis
        AnsiString AnalogAxis = Text.SubString(AnalogKeyword.Length() + 1,
                                   Text.Pos("_") - AnalogKeyword.Length() - 1);
        int AnalogIndex = -1;
        if (AnalogAxis == AnalogAxisKeyword[ANALOG_XAXIS_INDEX])
        {
          AnalogIndex = ANALOG_XAXIS_INDEX;
        }
        else if (AnalogAxis == AnalogAxisKeyword[ANALOG_YAXIS_INDEX])
        {
          AnalogIndex = ANALOG_YAXIS_INDEX;
        }
        else if (AnalogAxis == AnalogAxisKeyword[ANALOG_ZAXIS_INDEX])
        {
          AnalogIndex = ANALOG_ZAXIS_INDEX;
        }
        else if (AnalogAxis == AnalogAxisKeyword[ANALOG_RXAXIS_INDEX])
        {
          AnalogIndex = ANALOG_RXAXIS_INDEX;
        }
        else if (AnalogAxis == AnalogAxisKeyword[ANALOG_RYAXIS_INDEX])
        {
          AnalogIndex = ANALOG_RYAXIS_INDEX;
        }
        else if (AnalogAxis == AnalogAxisKeyword[ANALOG_RZAXIS_INDEX])
        {
          AnalogIndex = ANALOG_RZAXIS_INDEX;
        }

        if (AnalogIndex != -1)
        {
          if (Text.Pos(AnalogButtonMaxKeyword) != 0)
          {
            // Get Analog Button action ID
            TInputKey* InputKeyP = new TInputKey;
            AnsiString ActionID = Text.SubString(Text.Pos("=") +1, Text.Length());
            GetKeycodeFromProfileString(ActionID, InputKeyP);
            AddAnalogAction(SelectedDevice, AnalogIndex, ANALOG_INDEXBUTTON_MAX, InputKeyP);
          }
          else if (Text.Pos(AnalogButtonMinKeyword) != 0)
          {
            // Get Analog Button action ID
            TInputKey* InputKeyP = new TInputKey;
            AnsiString ActionID = Text.SubString(Text.Pos("=") +1, Text.Length());
            GetKeycodeFromProfileString(ActionID, InputKeyP);
            AddAnalogAction(SelectedDevice, AnalogIndex, ANALOG_INDEXBUTTON_MIN, InputKeyP);
          }
          else if (Text.Pos(AnalogPressThresholdKeyword) != 0)
          {
            int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
            SelectedDevice->ActionsP->Analogs[AnalogIndex]->PressThreshold = Value;
          }
          else if (Text.Pos(AnalogDeadZoneKeyword) != 0)
          {
            int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
            SelectedDevice->ActionsP->Analogs[AnalogIndex]->DeadZone = Value;
          }
          else if (Text.Pos(AnalogMouseXKeyword) != 0)
          {
            int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
            if (Value)
            {
              AddAnalogMouseX(SelectedDevice, AnalogIndex);
            }
          }
          else if (Text.Pos(AnalogMouseYKeyword) != 0)
          {
            int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
            if (Value)
            {
              AddAnalogMouseY(SelectedDevice, AnalogIndex);
            }
          }
        }
      }
      //
      // Check Sliders
      //
      else if (Text.Pos(SliderKeyword) == 1)
      {
        // Get Slider index
        int Index = Text.SubString(SliderKeyword.Length() + 1,
                                   Text.Pos("_") - SliderKeyword.Length() - 1).ToInt();

        if (Text.Pos(AnalogButtonMaxKeyword) != 0)
        {
          // Get Analog Button action ID
          TInputKey* InputKeyP = new TInputKey;
          AnsiString ActionID = Text.SubString(Text.Pos("=") +1, Text.Length());
          GetKeycodeFromProfileString(ActionID, InputKeyP);
          AddAnalogAction(SelectedDevice,
                          ANALOG_SLIDER1_INDEX + Index - 1,
                          ANALOG_INDEXBUTTON_MAX,
                          InputKeyP);
        }
        else if (Text.Pos(AnalogButtonMinKeyword) != 0)
        {
          // Get Analog Button action ID
          TInputKey* InputKeyP = new TInputKey;
          AnsiString ActionID = Text.SubString(Text.Pos("=") +1, Text.Length());
          GetKeycodeFromProfileString(ActionID, InputKeyP);
          AddAnalogAction(SelectedDevice,
                          ANALOG_SLIDER1_INDEX + Index - 1,
                          ANALOG_INDEXBUTTON_MIN,
                          InputKeyP);
        }
        else if (Text.Pos(AnalogPressThresholdKeyword) != 0)
        {
          int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
          SelectedDevice->ActionsP->Analogs[ANALOG_SLIDER1_INDEX + Index - 1]->PressThreshold = Value;
        }
        else if (Text.Pos(AnalogDeadZoneKeyword) != 0)
        {
          int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
          SelectedDevice->ActionsP->Analogs[ANALOG_SLIDER1_INDEX + Index - 1]->DeadZone = Value;
        }
        else if (Text.Pos(AnalogMouseXKeyword) != 0)
        {
          int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
          if (Value)
          {
            AddAnalogMouseX(SelectedDevice,
                            ANALOG_SLIDER1_INDEX + Index - 1);
          }
        }
        else if (Text.Pos(AnalogMouseYKeyword) != 0)
        {
          int Value = Text.SubString(Text.Pos("=") +1, Text.Length()).ToInt();
          if (Value)
          {
            AddAnalogMouseY(SelectedDevice,
                            ANALOG_SLIDER1_INDEX + Index - 1);
          }
        }
      }
    }
    else
    {
      // Add string to last profile device
      mProfileDevicesPP[mNbProfileDevices - 1]->Add(Text);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::SaveGameControllersProfile(TStringList* ProfileStrings)
{
  ProfileStrings->Clear();

  AnsiString Text;
  for (int DeviceIndex=0; DeviceIndex < mNbGameControllers; DeviceIndex++)
  {
    TInputDeviceActions* ActionsP = mGameDevicesPP[DeviceIndex]->ActionsP;

#ifdef _USE_DIRECTINPUT8

    // Add device name
    Text = DeviceKeyword + "=" + GetDeviceName(DeviceIndex);
    ProfileStrings->Add(Text);

    // Add buttons
    for (int CapsIndex=0; CapsIndex < mGameDevicesPP[DeviceIndex]->NbButtons; CapsIndex++)
    {
      if (ActionsP->Buttons[CapsIndex] != NULL)
      {
        Text = ButtonKeyword + AnsiString(CapsIndex+1) + "=";
        if (ActionsP->Buttons[CapsIndex]->MouseActive == false)
        {
          Text += GetProfileStringFromKeycode(ActionsP->Buttons[CapsIndex]->ButtonKey);
        }
        else
        {
          Text += ButtonMouseKeyword;
        }
        ProfileStrings->Add(Text);
      }
    }

    // Add POV
    for (int CapsIndex=0; CapsIndex < mGameDevicesPP[DeviceIndex]->NbPOV; CapsIndex++)
    {
      if (ActionsP->POVs[CapsIndex] != NULL)
      {
        // Button UP
        Text = POVKeyword + AnsiString(CapsIndex+1) + POVButtonUpKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->POVs[CapsIndex]->ButtonUpKey);
        ProfileStrings->Add(Text);

        // Button DOWN
        Text = POVKeyword + AnsiString(CapsIndex+1) + POVButtonDownKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->POVs[CapsIndex]->ButtonDownKey);
        ProfileStrings->Add(Text);

        // Button LEFT
        Text = POVKeyword + AnsiString(CapsIndex+1) + POVButtonLeftKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->POVs[CapsIndex]->ButtonLeftKey);
        ProfileStrings->Add(Text);

        // Button RIGHT
        Text = POVKeyword + AnsiString(CapsIndex+1) + POVButtonRightKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->POVs[CapsIndex]->ButtonRightKey);
        ProfileStrings->Add(Text);
      }
    }

    // Add Analogs
    for (int CapsIndex=0; CapsIndex < mGameDevicesPP[DeviceIndex]->NbAnalog; CapsIndex++)
    {
      if (ActionsP->Analogs[CapsIndex] != NULL)
      {
        // Button MAX
        Text = AnalogKeyword + AnalogAxisKeyword[CapsIndex] + AnalogButtonMaxKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->Analogs[CapsIndex]->ButtonMaxKey);
        ProfileStrings->Add(Text);
        // Button MIN
        Text = AnalogKeyword + AnalogAxisKeyword[CapsIndex] + AnalogButtonMinKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->Analogs[CapsIndex]->ButtonMinKey);
        ProfileStrings->Add(Text);
        // Press threshold
        Text = AnalogKeyword + AnalogAxisKeyword[CapsIndex] + AnalogPressThresholdKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[CapsIndex]->PressThreshold);
        ProfileStrings->Add(Text);
        // Dead Zone
        Text = AnalogKeyword + AnalogAxisKeyword[CapsIndex] + AnalogDeadZoneKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[CapsIndex]->DeadZone);
        ProfileStrings->Add(Text);
        // Mouse X
        Text = AnalogKeyword + AnalogAxisKeyword[CapsIndex] + AnalogMouseXKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[CapsIndex]->MouseXActive == true ? 1 : 0);
        ProfileStrings->Add(Text);
        // Mouse Y
        Text = AnalogKeyword + AnalogAxisKeyword[CapsIndex] + AnalogMouseYKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[CapsIndex]->MouseYActive == true ? 1 : 0);
        ProfileStrings->Add(Text);
      }
    }

    // Add Sliders
    for (int CapsIndex=0; CapsIndex < mGameDevicesPP[DeviceIndex]->NbSliders; CapsIndex++)
    {
      if (ActionsP->Analogs[ANALOG_SLIDER1_INDEX+CapsIndex] != NULL)
      {
        // Button MAX
        Text = SliderKeyword + AnsiString(CapsIndex+1) + AnalogButtonMaxKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->Analogs[ANALOG_SLIDER1_INDEX+CapsIndex]->ButtonMaxKey);
        ProfileStrings->Add(Text);
        // Button MIN
        Text = SliderKeyword + AnsiString(CapsIndex+1) + AnalogButtonMinKeyword + "=";
        Text += GetProfileStringFromKeycode(ActionsP->Analogs[ANALOG_SLIDER1_INDEX+CapsIndex]->ButtonMinKey);
        ProfileStrings->Add(Text);
        // Press threshold
        Text = SliderKeyword + AnsiString(CapsIndex+1) + AnalogPressThresholdKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[ANALOG_SLIDER1_INDEX+CapsIndex]->PressThreshold);
        ProfileStrings->Add(Text);
        // Dead Zone
        Text = SliderKeyword + AnsiString(CapsIndex+1) + AnalogDeadZoneKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[ANALOG_SLIDER1_INDEX+CapsIndex]->DeadZone);
        ProfileStrings->Add(Text);
        // Mouse X
        Text = SliderKeyword + AnsiString(CapsIndex+1) + AnalogMouseXKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[ANALOG_SLIDER1_INDEX+CapsIndex]->MouseXActive == true ? 1 : 0);
        ProfileStrings->Add(Text);
        // Mouse Y
        Text = SliderKeyword + AnsiString(CapsIndex+1) + AnalogMouseYKeyword + "=";
        Text += AnsiString(ActionsP->Analogs[ANALOG_SLIDER1_INDEX+CapsIndex]->MouseYActive == true ? 1 : 0);
        ProfileStrings->Add(Text);
      }
    }

    ProfileStrings->Add("");

#endif /* _USE_DIRECTINPUT8 */
  }

  // Add not available devices profile data
  for (int DeviceIndex=0; DeviceIndex < mNbProfileDevices; DeviceIndex++)
  {
    ProfileStrings->AddStrings(mProfileDevicesPP[DeviceIndex]);
    ProfileStrings->Add("");
  }
}
//---------------------------------------------------------------------------


void __fastcall TInputContext::CreateDefaultActions(TInputDevice* GameControllerP)
{
  // Create default FIRE 2 action
  if (GameControllerP->NbButtons >= 1)
  {
    // Create Button 1 action
    // Windows Key for CPC_JOY0_FIRE2 (0xC6)
    TInputKey* InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("198", InputKeyP);
    AddButtonAction(GameControllerP,
                    0, /* Button 1 */
                    InputKeyP);
  }

  // Create default FIRE 1 action
  if (GameControllerP->NbButtons >= 2)
  {
    // Create Button2 action
    // Windows Key for CPC_JOY0_FIRE1 (0xC5)
    TInputKey* InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("197", InputKeyP);
    AddButtonAction(GameControllerP,
                    1, /* Button 2 */
                    InputKeyP);
  }

  // Create default ESC action
  if (GameControllerP->NbButtons >= 3)
  {
    // Create Button2 action
    // Windows Key for CPC_ESCAPE (0x1B)
    TInputKey* InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("27", InputKeyP);
    AddButtonAction(GameControllerP,
                    2, /* Button 3 */
                    InputKeyP);
  }

  // Create default ENTER action
  if (GameControllerP->NbButtons >= 4)
  {
    // Create Button2 action
    // Windows Key for CPC_RETURN (0x0D)
    TInputKey* InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("13", InputKeyP);
    AddButtonAction(GameControllerP,
                    3, /* Button 4 */
                    InputKeyP);
  }

  // Create default POV with cursor
  if ( (GameControllerP->NbPOV >= 1)
       && (GameControllerP->XAxisEnable == true)
       && (GameControllerP->YAxisEnable == true) )
  {
    // Windows Key for CPC_CUR_LEFT (0x25)
    TInputKey* InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("37", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_LEFT, InputKeyP);
    // Windows Key for CPC_CUR_UP (0x26)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("38", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_UP, InputKeyP);
    // Windows Key for CPC_CUR_RIGHT (0x27)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("39", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_RIGHT, InputKeyP);
    // Windows Key for CPC_CUR_DOWN (0x28)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("40", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_DOWN, InputKeyP);
  }
  // Create default POV with joystick
  else if ( (GameControllerP->NbPOV >= 1)
            && (GameControllerP->XAxisEnable == false)
            && (GameControllerP->YAxisEnable == false) )
  {
    // Windows Key for CPC_JOY0_UP (0xC1)
    TInputKey* InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("193", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_UP, InputKeyP);
    // Windows Key for CPC_JOY0_DOWN (0xC2)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("194", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_DOWN, InputKeyP);
    // Windows Key for CPC_JOY0_LEFT (0xC3)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("195", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_LEFT, InputKeyP);
    // Windows Key for CPC_JOY0_RIGHT (0xC4)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("196", InputKeyP);
    AddPOVAction(GameControllerP, 0 /* POV 1 */, POV_INDEXBUTTON_RIGHT, InputKeyP);
  }

  // Create default Analog with joystick
  if ( (GameControllerP->XAxisEnable == true)
       && (GameControllerP->YAxisEnable == true) )
  {
    // Windows Key for CPC_JOY0_UP (0xC1)
    TInputKey* InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("193", InputKeyP);
    AddAnalogAction(GameControllerP, ANALOG_YAXIS_INDEX, ANALOG_INDEXBUTTON_MIN, InputKeyP);
    // Windows Key for CPC_JOY0_DOWN (0xC2)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("194", InputKeyP);
    AddAnalogAction(GameControllerP, ANALOG_YAXIS_INDEX, ANALOG_INDEXBUTTON_MAX, InputKeyP);
    // Windows Key for CPC_JOY0_LEFT (0xC3)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("195", InputKeyP);
    AddAnalogAction(GameControllerP, ANALOG_XAXIS_INDEX, ANALOG_INDEXBUTTON_MIN, InputKeyP);
    // Windows Key for CPC_JOY0_RIGHT (0xC4)
    InputKeyP = new TInputKey;
    GetKeycodeFromProfileString("196", InputKeyP);
    AddAnalogAction(GameControllerP, ANALOG_XAXIS_INDEX, ANALOG_INDEXBUTTON_MAX, InputKeyP);
  }
}
//---------------------------------------------------------------------------


#ifdef _USE_DIRECTINPUT8

BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID ContextP)
{
TInputContext* InputContextP = (TInputContext*)ContextP;
LPDIRECTINPUTDEVICE8 ControllerP;
HRESULT hr;

  do
  {
    // Obtain an interface to the enumerated game controler.
    hr = InputContextP->DirectInput->CreateDevice(pdidInstance->guidInstance,
                                                  &ControllerP,
                                                  NULL);
    if (FAILED(hr))
    {
#ifdef _DEBUG
			String msg = _T("CreateDevice Error=") + String::IntToHex(hr, 8);
			Application->MessageBox(msg.c_str(), _T("EnumJoysticksCallback"), MB_OK);
#endif /* _DEBUG */
			continue;
		}

		// Get Device info
		DIDEVICEINSTANCE didins;
		didins.dwSize = sizeof(didins);
		hr = ControllerP->GetDeviceInfo(&didins);
		if (FAILED(hr))
		{
#ifdef _DEBUG
			String msg = _T("GetDeviceInfo Error=") + String::IntToHex(hr, 8);
			Application->MessageBox(msg.c_str(), _T("EnumJoysticksCallback"), MB_OK);
#endif /* _DEBUG */
			continue;
    }

    // Check valid device type
    if ( (GET_DIDEVICE_TYPE(didins.dwDevType) == DI8DEVTYPE_GAMEPAD)
         || (GET_DIDEVICE_TYPE(didins.dwDevType) == DI8DEVTYPE_JOYSTICK)
         || (GET_DIDEVICE_TYPE(didins.dwDevType) == DI8DEVTYPE_1STPERSON) )
    {
      InputContextP->AddGameController(ControllerP);
    }
  }
  while (0);

  if (FAILED(hr))
  {
    // Release device
    ControllerP->Release();
  }

  return DIENUM_CONTINUE;
}
//---------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksInfoCallback(LPCDIDEVICEINSTANCE pdidInstance, LPVOID ContextP)
{
TInputContext* InputContextP = (TInputContext*)ContextP;
LPDIRECTINPUTDEVICE8 ControllerP;
HRESULT hr;

  do
  {
    // Obtain an interface to the enumerated game controler.
    hr = InputContextP->DirectInput->CreateDevice(pdidInstance->guidInstance,
                                                  &ControllerP,
                                                  NULL);
    if (FAILED(hr))
    {
			continue;
		}

		// Get Device info
		DIDEVICEINSTANCE didins;
		didins.dwSize = sizeof(didins);
		hr = ControllerP->GetDeviceInfo(&didins);
		if (FAILED(hr))
		{
			continue;
    }

    // Get Device capabilities
    DIDEVCAPS didevcaps;
		didevcaps.dwSize = sizeof(didevcaps);
		hr = ControllerP->GetCapabilities(&didevcaps);
		if (FAILED(hr))
		{
			continue;
    }

    TStringList* InfoStrings = new TStringList;

    AnsiString Info = "Product=" + AnsiString(didins.tszProductName);
    InfoStrings->Add(Info);
    Info = "Instance=" + AnsiString(didins.tszInstanceName);
    InfoStrings->Add(Info);
    Info = "UsagePage= 0x" + AnsiString::IntToHex(didins.wUsagePage, 4);
    InfoStrings->Add(Info);
    Info = "Usage= 0x" + AnsiString::IntToHex(didins.wUsage, 4);
    InfoStrings->Add(Info);
    Info = "DevType= 0x" + AnsiString::IntToHex(didins.dwDevType, 8) + " (DI8DEVTYPE_ into dinput.h)";
    InfoStrings->Add(Info);
    Info = "CapsFlags= 0x" + AnsiString::IntToHex(didevcaps.dwFlags, 8) + " (DIDC_ into dinput.h)";
    InfoStrings->Add(Info);
    Info = "CapsButtons=" + AnsiString(didevcaps.dwButtons);
    InfoStrings->Add(Info);
    Info = "CapsAxes=" + AnsiString(didevcaps.dwAxes);
    InfoStrings->Add(Info);
    Info = "CapsPOVs=" + AnsiString(didevcaps.dwPOVs);
    InfoStrings->Add(Info);
    InfoStrings->Add("");

    InputContextP->AddInfo(InfoStrings);
  }
  while (0);

  // Release device
  ControllerP->Release();

  return DIENUM_CONTINUE;
}
//---------------------------------------------------------------------------


BOOL CALLBACK EnumObjectsCallback(LPCDIDEVICEOBJECTINSTANCE pdidoi, LPVOID ContextP)
{
  TInputDevice* DeviceP = (TInputDevice*)ContextP;
  HRESULT hr;

  // For axes that are returned, set the DIPROP_RANGE property for the
  // enumerated axis in order to scale min/max values.
  if( pdidoi->dwType & DIDFT_AXIS )
  {
    DIPROPRANGE diprg;
    diprg.diph.dwSize = sizeof(DIPROPRANGE);
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diprg.diph.dwHow = DIPH_BYID;
    diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
    diprg.lMax = AnalogRangeMax;
    diprg.lMin = AnalogRangeMin;

    // Set the range for the axis
    hr = DeviceP->DirectInput8DeviceP->SetProperty(DIPROP_RANGE,
                                                   &diprg.diph);
    if (FAILED(hr))
    {
#ifdef _DEBUG
			String msg = _T("SetProperty Error=") + String::IntToHex(hr, 8);
			Application->MessageBox(msg.c_str(), _T("EnumObjectsCallback"), MB_OK);
#endif /* _DEBUG */
		}
  }

  if (pdidoi->guidType == GUID_XAxis)
  {
    DeviceP->XAxisEnable = true;
  }
  else if (pdidoi->guidType == GUID_YAxis)
  {
    DeviceP->YAxisEnable = true;
  }
  else if (pdidoi->guidType == GUID_ZAxis)
  {
    DeviceP->ZAxisEnable = true;
  }
  else if (pdidoi->guidType == GUID_RxAxis)
  {
    DeviceP->RxAxisEnable = true;
  }
  else if (pdidoi->guidType == GUID_RyAxis)
  {
    DeviceP->RyAxisEnable = true;
  }
  else if (pdidoi->guidType == GUID_RzAxis)
  {
    DeviceP->RzAxisEnable = true;
  }
  else if (pdidoi->guidType == GUID_Slider)
  {
    DeviceP->NbSliders++;
  }
  else if (pdidoi->guidType == GUID_Button)
  {
    DeviceP->NbButtons++;
  }
  else if (pdidoi->guidType == GUID_Key)
  {
    DeviceP->NbKeys++;
  }
  else if (pdidoi->guidType == GUID_POV)
  {
    DeviceP->NbPOV++;
  }
  else
  {
    DeviceP->NbUnknown++;
  }

  return DIENUM_CONTINUE;
}
//---------------------------------------------------------------------------

#endif /* _USE_DIRECTINPUT8 */

