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


#ifndef TInputDeviceActionsH
#define TInputDeviceActionsH

#ifdef _USE_DIRECTINPUT8

#include "TInputActionButton.h"
#include "TInputActionPOV.h"
#include "TInputActionAnalog.h"

// According to DIJOYSTATE structure
#define DEVICEACTIONS_NBBUTTONS         32
#define DEVICEACTIONS_NBPOV             4
#define DEVICEACTIONS_NBANALOG          6
#define DEVICEACTIONS_NBSLIDERS         2

#define ANALOG_XAXIS_INDEX              0
#define ANALOG_YAXIS_INDEX              1
#define ANALOG_ZAXIS_INDEX              2
#define ANALOG_RXAXIS_INDEX             3
#define ANALOG_RYAXIS_INDEX             4
#define ANALOG_RZAXIS_INDEX             5
#define ANALOG_SLIDER1_INDEX            6

#endif /* _USE_DIRECTINPUT8 */


class TInputDeviceActions
{
private:
  // Members
#ifdef _USE_DIRECTINPUT8
  TList* mButtonsActionsListP;
  TList* mPOVActionsListP;
  TList* mAnalogActionsListP;
#endif /* _USE_DIRECTINPUT8 */

  // Methods
#ifdef _USE_DIRECTINPUT8
  TInputActionButton* __fastcall GetButtonAction(int Index);
  void __fastcall SetButtonAction(int Index, TInputActionButton* Value);
  TInputActionPOV* __fastcall GetPOVAction(int Index);
  void __fastcall SetPOVAction(int Index, TInputActionPOV* Value);
  TInputActionAnalog* __fastcall GetAnalogAction(int Index);
  void __fastcall SetAnalogAction(int Index, TInputActionAnalog* Value);
#endif /* _USE_DIRECTINPUT8 */

public:
  // Properties
#ifdef _USE_DIRECTINPUT8
  __property TInputActionButton* Buttons[int Index] = { read=GetButtonAction, write=SetButtonAction };
  __property TInputActionPOV* POVs[int Index] = { read=GetPOVAction, write=SetPOVAction };
  __property TInputActionAnalog* Analogs[int Index] = { read=GetAnalogAction, write=SetAnalogAction };
#endif /* _USE_DIRECTINPUT8 */

  // Public API
  __fastcall TInputDeviceActions();
  __fastcall ~TInputDeviceActions();
#ifdef _USE_DIRECTINPUT8
  bool __fastcall AddPOVButtonAction(int POVIndex, int ButtonIndex, TInputKey* KeyP);
  bool __fastcall AddAnalogButtonAction(int POVIndex, int ButtonIndex, TInputKey* KeyP);
#endif /* _USE_DIRECTINPUT8 */
};

//---------------------------------------------------------------------------
#endif

