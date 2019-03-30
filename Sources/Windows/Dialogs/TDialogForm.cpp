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
#include "TDialogForm.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TDialogForm::TDialogForm() : TList()
//
// Constructor
//
{
  // Initialise size
  mLeft = 0.0f;
  mRight = 0.0f;
  mTop = 0.0f;
  mBottom = 0.0f;

  mOnEndEvent = NULL;
}
//---------------------------------------------------------------------------


__fastcall TDialogForm::~TDialogForm()
//
// Destructor
//
{
  // Release list content
  while (Count)
  {
    delete Components[0];
    Delete(0);
  }
}
//---------------------------------------------------------------------------


TDialogComponent* __fastcall TDialogForm::GetComponent(int Index)
{
  TDialogComponent* ComponentP = NULL;

  if ( (Index >= 0) && (Index < Count) )
  {
    ComponentP = (TDialogComponent*)Items[Index];
  }

  return ComponentP;
}
//---------------------------------------------------------------------------


void TDialogForm::AddComponent(TDialogComponent* NewComponent)
{
  Add(NewComponent);
}
//---------------------------------------------------------------------------


void __fastcall TDialogForm::OnResize(TDrawingContext* DrawingContextP)
{
  mScreenLeft = (int)((float)DrawingContextP->EmulatorScreenWidth * mLeft)
                + DrawingContextP->EmulatorScreenLeft;
  mScreenRight = (int)((float)DrawingContextP->EmulatorScreenWidth * mRight)
                 + DrawingContextP->EmulatorScreenLeft;
  mScreenTop = (int)((float)DrawingContextP->EmulatorScreenHeight * mTop)
               + DrawingContextP->EmulatorScreenTop;
  mScreenBottom = (int)((float)DrawingContextP->EmulatorScreenHeight * mBottom)
                  + DrawingContextP->EmulatorScreenTop;

  // Resize all components
  for (int component = 0; component < Count; component++)
  {
    Components[component]->OnResize(DrawingContextP);
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogForm::OnDisplay(TDrawingContext* DrawingContextP)
{
  // Display all components
  for (int component = 0; component < Count; component++)
  {
    Components[component]->OnDisplay(DrawingContextP);
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogForm::OnMouseMove(int X, int Y)
{
  // Dispatch event to all components
  for (int component = 0; component < Count; component++)
  {
    Components[component]->OnMouseMove(X, Y);
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogForm::OnMouseDown(TMouseButton Button, int X, int Y)
{
  // Dispatch event to all components
  for (int component = 0; component < Count; component++)
  {
    if (Components[component]->OnMouseDown(Button, X, Y) == true)
    {
      break;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogForm::OnMouseClick(int X, int Y)
{
  // Dispatch event to all components
  for (int component = 0; component < Count; component++)
  {
    if (Components[component]->OnMouseClick(X, Y) == true)
    {
      break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogForm::OnMouseDblClick(int X, int Y)
{
  // Dispatch event to all components
  for (int component = 0; component < Count; component++)
  {
    if (Components[component]->OnMouseDblClick(X, Y) == true)
    {
      break;
    }
  }
}
//---------------------------------------------------------------------------

