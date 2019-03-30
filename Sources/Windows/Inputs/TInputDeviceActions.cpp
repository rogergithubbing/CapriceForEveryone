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
//roger
#define _USE_DIRECTINPUT8
#include <vcl.h>
#pragma hdrstop
#include "TInputDeviceActions.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TInputDeviceActions::TInputDeviceActions()
//
// Constructor
//
{
#ifdef _USE_DIRECTINPUT8
  mButtonsActionsListP = new TList;
  for (int loop=0; loop < DEVICEACTIONS_NBBUTTONS; loop++)
  {
    mButtonsActionsListP->Add(NULL);
  }
  mPOVActionsListP = new TList;
  for (int loop=0; loop < DEVICEACTIONS_NBPOV; loop++)
  {
    mPOVActionsListP->Add(NULL);
  }
  mAnalogActionsListP = new TList;
  for (int loop=0; loop < (DEVICEACTIONS_NBANALOG + DEVICEACTIONS_NBSLIDERS); loop++)
  {
    mAnalogActionsListP->Add(NULL);
  }
#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------


__fastcall TInputDeviceActions::~TInputDeviceActions(void)
//
// Destructor
//
{
#ifdef _USE_DIRECTINPUT8
  while (mButtonsActionsListP->Count)
  {
    TInputActionButton* ButtonP = (TInputActionButton*)mButtonsActionsListP->Items[0];
    if (ButtonP != NULL)
    {
      delete ButtonP;
    }
    mButtonsActionsListP->Delete(0);
  }
  delete mButtonsActionsListP;

  while (mPOVActionsListP->Count)
  {
    TInputActionPOV* POVP = (TInputActionPOV*)mPOVActionsListP->Items[0];
    if (POVP != NULL)
    {
      delete POVP;
    }
    mPOVActionsListP->Delete(0);
  }
  delete mPOVActionsListP;

  while (mAnalogActionsListP->Count)
  {
    TInputActionAnalog* AnalogP = (TInputActionAnalog*)mAnalogActionsListP->Items[0];
    if (AnalogP != NULL)
    {
      delete AnalogP;
    }
    mAnalogActionsListP->Delete(0);
  }
  delete mAnalogActionsListP;
#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------

//roger
#ifdef _USE_DIRECTINPUT8

TInputActionButton* __fastcall TInputDeviceActions::GetButtonAction(int Index)
{
  return (TInputActionButton*)mButtonsActionsListP->Items[Index];
}
//---------------------------------------------------------------------------
void __fastcall TInputDeviceActions::SetButtonAction(int Index, TInputActionButton* ActionP)
{
  if (Index < mButtonsActionsListP->Count)
  {
    TInputActionButton* ButtonP = (TInputActionButton*)mButtonsActionsListP->Items[Index];
    if (ButtonP != NULL)
    {
      delete ButtonP;
    }
    
    mButtonsActionsListP->Items[Index] = ActionP;
  }
}
//---------------------------------------------------------------------------


TInputActionPOV* __fastcall TInputDeviceActions::GetPOVAction(int Index)
{
  return (TInputActionPOV*)mPOVActionsListP->Items[Index];
}
//---------------------------------------------------------------------------
void __fastcall TInputDeviceActions::SetPOVAction(int Index, TInputActionPOV* Value)
{
  if (Index < mPOVActionsListP->Count)
  {
    TInputActionPOV* POVP = (TInputActionPOV*)mPOVActionsListP->Items[Index];
    if (POVP != NULL)
    {
      delete POVP;
    }
    
    mPOVActionsListP->Items[Index] = Value;
  }
}
//---------------------------------------------------------------------------
bool __fastcall TInputDeviceActions::AddPOVButtonAction(int POVIndex,
                                                        int ButtonIndex,
                                                        TInputKey* KeyP)
{
bool Result = false;

  if (POVIndex < DEVICEACTIONS_NBPOV)
  {
    TInputActionPOV* POVP = (TInputActionPOV*)mPOVActionsListP->Items[POVIndex];
    if (POVP != NULL)
    {
      if (ButtonIndex < POV_NBBUTTONS)
      {
        POVP->Buttons[ButtonIndex] = KeyP;
        Result = true;
      }
    }
  }

  return Result;
}
//---------------------------------------------------------------------------


TInputActionAnalog* __fastcall TInputDeviceActions::GetAnalogAction(int Index)
{
  return (TInputActionAnalog*)mAnalogActionsListP->Items[Index];
};
//---------------------------------------------------------------------------
void __fastcall TInputDeviceActions::SetAnalogAction(int Index, TInputActionAnalog* Value)
{
  if (Index < mAnalogActionsListP->Count)
  {
    TInputActionAnalog* AnalogP = (TInputActionAnalog*)mAnalogActionsListP->Items[Index];
    if (AnalogP != NULL)
    {
      delete AnalogP;
    }
    
    mAnalogActionsListP->Items[Index] = Value;
  }
}
//---------------------------------------------------------------------------


bool __fastcall TInputDeviceActions::AddAnalogButtonAction(int AnalogIndex,
                                                           int ButtonIndex,
                                                           TInputKey* KeyP)
{
bool Result = false;

  if (AnalogIndex < (DEVICEACTIONS_NBANALOG+DEVICEACTIONS_NBSLIDERS))
  {
    TInputActionAnalog* AnalogP = (TInputActionAnalog*)mAnalogActionsListP->Items[AnalogIndex];
    if (AnalogP != NULL)
    {
      if (ButtonIndex < ANALOG_NBBUTTONS)
      {
        AnalogP->Buttons[ButtonIndex] = KeyP;
        Result = true;
      }
    }
  }

  return Result;
}
//---------------------------------------------------------------------------

#endif /* _USE_DIRECTINPUT8 */


