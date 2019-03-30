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
#include "TInputActionButton.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TInputActionButton::TInputActionButton() : TInputAction()
//
// Constructor
//
{
  TInputKey* ButtonKeyP = new TInputKey();
  Add(ButtonKeyP);

  mMouseActive = false;
  mOnMouseDownEvent = NULL;
  mOnMouseUpEvent = NULL;
}
//---------------------------------------------------------------------------


__fastcall TInputActionButton::~TInputActionButton(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TInputActionButton::SetButtonKey(TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[0] = ButtonKeyP;
}
//---------------------------------------------------------------------------


void __fastcall TInputActionButton::OnChange(long NewValue)
{
  if (NewValue)
  {
    PressButton(0);
    
    if ( (mMouseActive == true) && (mOnMouseDownEvent != NULL) )
    {
      mOnMouseDownEvent();
    }
  }
  else
  {
    ReleaseButton(0);

    if ( (mMouseActive == true) && (mOnMouseUpEvent != NULL) )
    {
      mOnMouseUpEvent();
    }
  }
}
//---------------------------------------------------------------------------

