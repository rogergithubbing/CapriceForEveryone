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


#include <vcl.h>
#pragma hdrstop
#include "TInputDevice.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



#define NB_NOT_RESPONDING_ERRORS          10



__fastcall TInputDevice::TInputDevice(void)
//
// Constructor
//
{
  // Init device capabilities
  mActionsP = new TInputDeviceActions;
  mErrorCount = 0;

#ifdef _USE_DIRECTINPUT8

  ProfileLoaded = false;
  DirectInput8DeviceP = NULL;
  XAxisEnable = false;
  YAxisEnable = false;
  ZAxisEnable = false;
  RxAxisEnable = false;
  RyAxisEnable = false;
  RzAxisEnable = false;
  NbAnalog = DEVICEACTIONS_NBANALOG;
  NbSliders = 0;
  NbButtons = 0;
  NbKeys = 0;
  NbPOV = 0;
  NbUnknown = 0;
  UseDIJOYSTATE2 = false;
  LastDIJOYSTATEP = NULL;
  
#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------


__fastcall TInputDevice::~TInputDevice(void)
//
// Destructor
//
{
  delete mActionsP;

#ifdef _USE_DIRECTINPUT8

  if (DirectInput8DeviceP != NULL)
  {
    DirectInput8DeviceP->Unacquire();
    DirectInput8DeviceP->Release();
  }

  if (LastDIJOYSTATEP != NULL)
  {
    delete LastDIJOYSTATEP;
  }

#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------


void __fastcall TInputDevice::SetActions(TInputDeviceActions* Value)
{
  delete mActionsP;
  mActionsP = Value;
}
//---------------------------------------------------------------------------


void __fastcall TInputDevice::ResetError(void)
{
  mErrorCount = 0;
}
//---------------------------------------------------------------------------

bool __fastcall TInputDevice::IncrementError(void)
{
  mErrorCount++;
  return (mErrorCount > NB_NOT_RESPONDING_ERRORS ? true : false);
}
//---------------------------------------------------------------------------

