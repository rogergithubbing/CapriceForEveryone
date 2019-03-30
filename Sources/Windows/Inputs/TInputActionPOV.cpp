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
#include "TInputActionPOV.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TInputActionPOV::TInputActionPOV() : TInputAction()
//
// Constructor
//
{
  // Add POV 4 buttons
  for (int buttons=0; buttons < POV_NBBUTTONS; buttons++)
  {
    TInputKey* ButtonKeyP = new TInputKey();
    Add(ButtonKeyP);
  }
}
//---------------------------------------------------------------------------


__fastcall TInputActionPOV::~TInputActionPOV(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TInputActionPOV::SetButtonsKey(int Index, TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[Index] = ButtonKeyP;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionPOV::SetButtonUpKey(TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[POV_INDEXBUTTON_UP] = ButtonKeyP;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionPOV::SetButtonDownKey(TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[POV_INDEXBUTTON_DOWN] = ButtonKeyP;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionPOV::SetButtonLeftKey(TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[POV_INDEXBUTTON_LEFT] = ButtonKeyP;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionPOV::SetButtonRightKey(TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[POV_INDEXBUTTON_RIGHT] = ButtonKeyP;
}
//---------------------------------------------------------------------------


void __fastcall TInputActionPOV::OnChange(long NewValue)
{
  // Reset button press state
  bool Pressed[POV_NBBUTTONS];
  for (int buttons=0; buttons < POV_NBBUTTONS; buttons++)
  {
    Pressed[buttons] = false;
    if (IsPressed(buttons) == true)
    {
      ReleaseButton(buttons);
    }
  }

  if (NewValue != -1)
  {

    if (NewValue == 0)
    {
      Pressed[POV_INDEXBUTTON_UP] = true;
    }
    else if ( (NewValue > 0) && (NewValue < 18000) )
    {
      Pressed[POV_INDEXBUTTON_RIGHT] = true;

      if (NewValue < 9000)
      {
        Pressed[POV_INDEXBUTTON_UP] = true;
      }
      else if (NewValue > 9000)
      {
        Pressed[POV_INDEXBUTTON_DOWN] = true;
      }
    }
    else if (NewValue == 18000)
    {
      Pressed[POV_INDEXBUTTON_DOWN] = true;
    }
    else
    {
      Pressed[POV_INDEXBUTTON_LEFT] = true;

      if (NewValue < 27000)
      {
        Pressed[POV_INDEXBUTTON_DOWN] = true;
      }
      else if (NewValue > 27000)
      {
        Pressed[POV_INDEXBUTTON_UP] = true;
      }
    }

    // Reset button press state
    for (int buttons=0; buttons < POV_NBBUTTONS; buttons++)
    {
      if (Pressed[buttons] == true)
      {
        PressButton(buttons);
      }
    }
  }
}
//---------------------------------------------------------------------------

