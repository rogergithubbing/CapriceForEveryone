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
#include "TDialogComponent.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TDialogComponent::TDialogComponent()
//
// Constructor
//
{
  mLeft = 0.0f;
  mRight = 0.0f;
  mTop = 0.0f;
  mBottom = 0.0f;
  mVisible = true;
}
//---------------------------------------------------------------------------


__fastcall TDialogComponent::~TDialogComponent(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TDialogComponent::OnResize(TDrawingContext* DrawingContextP)
{
  mScreenLeft = (int)((float)DrawingContextP->EmulatorScreenWidth * mLeft)
                + DrawingContextP->EmulatorScreenLeft;
  mScreenRight = (int)((float)DrawingContextP->EmulatorScreenWidth * mRight)
                 + DrawingContextP->EmulatorScreenLeft;
  mScreenTop = (int)((float)DrawingContextP->EmulatorScreenHeight * mTop)
               + DrawingContextP->EmulatorScreenTop;
  mScreenBottom = (int)((float)DrawingContextP->EmulatorScreenHeight * mBottom)
                  + DrawingContextP->EmulatorScreenTop;
}
//---------------------------------------------------------------------------


bool __fastcall TDialogComponent::OnMouseDown(TMouseButton Button, int X, int Y)
{
	NOT_USED(Button);

	return TDialogComponent::OnMouseClick(X, Y);
}
//---------------------------------------------------------------------------


bool __fastcall TDialogComponent::OnMouseClick(int X, int Y)
{
  if (mVisible == false) return false;

  bool Inside = false;
  if ( (X >= mScreenLeft) && (X <= mScreenRight) )
  {
    if ( (Y >= mScreenTop) && (Y <= mScreenBottom) )
    {
      Inside = true;
    }
  }

  return Inside;
}
//---------------------------------------------------------------------------


bool __fastcall TDialogComponent::OnMouseDblClick(int X, int Y)
{
  return OnMouseClick(X, Y);
}
//---------------------------------------------------------------------------

