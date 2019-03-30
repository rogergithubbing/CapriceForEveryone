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
#include "TFormDebugGameController.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


TFormDebugGameController *FormDebugGameController;
//---------------------------------------------------------------------------
__fastcall TFormDebugGameController::TFormDebugGameController(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugGameController::GetState(DIJOYSTATE* StateP)
{
  // Set buttons
  StateP->rgbButtons[0] = SpeedButtonButton1->Down == true ? 128 : 0;
  StateP->rgbButtons[1] = SpeedButtonButton2->Down == true ? 128 : 0;
  StateP->rgbButtons[2] = SpeedButtonButton3->Down == true ? 128 : 0;
  StateP->rgbButtons[3] = SpeedButtonButton4->Down == true ? 128 : 0;

  // Set POV1
  if (SpeedButtonPOV1Up->Down == true)
  {
    if (SpeedButtonPOV1Left->Down == true)
    {
      StateP->rgdwPOV[0] = 32500;
    }
    else if (SpeedButtonPOV1Right->Down == true)
    {
      StateP->rgdwPOV[0] = 4500;
    }
    else
    {
      StateP->rgdwPOV[0] = 0;
    }
  }
  else if (SpeedButtonPOV1Down->Down == true)
  {
    if (SpeedButtonPOV1Left->Down == true)
    {
      StateP->rgdwPOV[0] = 22500;
    }
    else if (SpeedButtonPOV1Right->Down == true)
    {
      StateP->rgdwPOV[0] = 13500;
    }
    else
    {
      StateP->rgdwPOV[0] = 18000;
    }
  }
  else if (SpeedButtonPOV1Left->Down == true)
  {
    StateP->rgdwPOV[0] = 27000;
  }
  else if (SpeedButtonPOV1Right->Down == true)
  {
    StateP->rgdwPOV[0] = 9000;
  }
  else
  {
    StateP->rgdwPOV[0] = -1;
  }

  // Set analogs
  StateP->lX = TrackBarXAxis->Position;
  StateP->lY = TrackBarYAxis->Position;
  StateP->lZ = TrackBarZAxis->Position;
  StateP->lRx = TrackBarRxAxis->Position;
}
//---------------------------------------------------------------------------

