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
#include "TInputAction.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TInputAction::TInputAction(void) : TList()
//
// Constructor
//
{
  mOnWINButtonDown = NULL;
  mOnWINButtonUp = NULL;
  mOnCPCButtonDown = NULL;
  mOnCPCButtonUp = NULL;
}
//---------------------------------------------------------------------------


__fastcall TInputAction::~TInputAction(void)
//
// Destructor
//
{
  // Release list content
  while (Count)
  {
    delete Keys[0];
    Delete(0);
  }
}
//---------------------------------------------------------------------------


TInputKey* __fastcall TInputAction::GetKey(int Index)
{
  TInputKey* KeyP = NULL;

  if ( (Index >= 0) && (Index < Count) )
  {
    KeyP = (TInputKey*)Items[Index];
  }

  return KeyP;
}
//---------------------------------------------------------------------------
void __fastcall TInputAction::SetKey(int Index, TInputKey* Value)
{
  if ( (Index >= 0) && (Index < Count) )
  {
    delete Keys[Index];
    Items[Index] = Value;
  }
}
//---------------------------------------------------------------------------


void __fastcall TInputAction::PressButton(int Index)
{
  if (Index < Count)
  {
    TInputKey* InputKeyP = (TInputKey*)Items[Index];

    if (InputKeyP->Pressed == false)
    {
      InputKeyP->Pressed = true;

      if (InputKeyP->CPCKeycodeValid == false)
      {
        if (mOnWINButtonDown != NULL)
        {
          WORD Keycode = InputKeyP->WINKeycode;
          mOnWINButtonDown(Keycode, InputKeyP->WINShiftState);
        }
      }
      else
      {
        if (mOnCPCButtonDown != NULL)
        {
          mOnCPCButtonDown(InputKeyP->CPCKeycode);
        }
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TInputAction::ReleaseButton(int Index)
{
  if (Index < Count)
  {
    TInputKey* InputKeyP = (TInputKey*)Items[Index];

    if (InputKeyP->Pressed == true)
    {
      InputKeyP->Pressed = false;

      if (InputKeyP->CPCKeycodeValid == false)
      {
        if (mOnWINButtonUp != NULL)
        {
          WORD Keycode = InputKeyP->WINKeycode;
          mOnWINButtonUp(Keycode, InputKeyP->WINShiftState);
        }
      }
      else
      {
        if (mOnCPCButtonUp != NULL)
        {
          mOnCPCButtonUp(InputKeyP->CPCKeycode);
        }
      }
    }
  }
}
//---------------------------------------------------------------------------


bool __fastcall TInputAction::IsPressed(int Index)
{
bool Pressed = false;

  if (Index < Count)
  {
    TInputKey* InputKeyP = (TInputKey*)Items[Index];
    Pressed = InputKeyP->Pressed;
  }

  return Pressed;
}
//---------------------------------------------------------------------------



