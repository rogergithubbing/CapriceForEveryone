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


#ifndef TInputDeviceH
#define TInputDeviceH

#ifdef _USE_DIRECTINPUT8
#include <initguid.h>
#include "..\DirectInput\dinput.h"
#include "..\DirectInput\dinputd.h"
#endif /* _USE_DIRECTINPUT8 */

#if (__BORLANDC__ == 0x551)
  #include "..\Forms\TFormDebugGameController.h"
#else /* __BORLANDC__ */
  #include "..\Forms_XE3\TFormDebugGameController.h"
#endif /* __BORLANDC__ */

#include "TInputDeviceActions.h"


class TInputDevice
{
private:
  // Members
  TInputDeviceActions* mActionsP;
  int mErrorCount;

  // Methods
  inline TInputDeviceActions* __fastcall GetActions(void) { return mActionsP; };
  void __fastcall SetActions(TInputDeviceActions* Value);

protected:

public:
  // Members
  bool ProfileLoaded;
#ifdef _USE_DIRECTINPUT8
  LPDIRECTINPUTDEVICE8 DirectInput8DeviceP;
  bool XAxisEnable;
  bool YAxisEnable;
  bool ZAxisEnable;
  bool RxAxisEnable;
  bool RyAxisEnable;
  bool RzAxisEnable;
  int NbAnalog;
  int NbSliders;
  int NbButtons;
  int NbKeys;
  int NbPOV;
  int NbUnknown;
  bool UseDIJOYSTATE2;
  DIJOYSTATE* LastDIJOYSTATEP;
#endif /* _USE_DIRECTINPUT8 */
  TFormDebugGameController* FormGameControllerP;
  // Properties
  __property TInputDeviceActions* ActionsP = { read=GetActions, write=SetActions };

  // Public API
  __fastcall TInputDevice();
  __fastcall ~TInputDevice();

  void __fastcall ResetError(void);
  bool __fastcall IncrementError(void);
};

//---------------------------------------------------------------------------
#endif
