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
#include "TInputActionAnalog.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


#define cDeadZoneMin    1
#define cDeadZoneMax    20



__fastcall TInputActionAnalog::TInputActionAnalog() : TInputAction()
//
// Constructor
//
{
  // Add Analog 2 buttons
  for (int buttons=0; buttons < ANALOG_NBBUTTONS; buttons++)
  {
    TInputKey* ButtonKeyP = new TInputKey();
    Add(ButtonKeyP);
  }

  mMaxValue = 0;
  mMinValue = 0;
  mPressThreshold = 0;
  mMaxThreshold = 0;
  mMinThreshold = 0;

  mDeadZone = 4;

  mMouseXActive = false;
  mMouseYActive = false;
  mDisplayWidth = 0;
  mDisplayHeight = 0;
  mOnMouseXEvent = NULL;
  mOnMouseYEvent = NULL;
}
//---------------------------------------------------------------------------


__fastcall TInputActionAnalog::~TInputActionAnalog(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TInputActionAnalog::SetButtonsKey(int Index, TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[Index] = ButtonKeyP;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionAnalog::SetButtonMaxKey(TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[ANALOG_INDEXBUTTON_MAX] = ButtonKeyP;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionAnalog::SetButtonMinKey(TInputKey* ButtonKeyP)
{
  ButtonKeyP->Pressed = false;
  Keys[ANALOG_INDEXBUTTON_MIN] = ButtonKeyP;
}
//---------------------------------------------------------------------------


void __fastcall TInputActionAnalog::SetMaxValue(long Value)
{
  mMaxValue = Value;

  // Update thresholds
  long Range = mMaxValue - mMinValue;
  long Radius = Range / 2;
  long Midrange = mMinValue + Radius;
  long Threshold = Radius * mPressThreshold / 100;
  mMaxThreshold = Midrange + Threshold;
  mMinThreshold = Midrange - Threshold;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionAnalog::SetMinValue(long Value)
{
  mMinValue = Value;

  // Update thresholds
  long Range = mMaxValue - mMinValue;
  long Radius = Range / 2;
  long Midrange = mMinValue + Radius;
  long Threshold = Radius * mPressThreshold / 100;
  mMaxThreshold = Midrange + Threshold;
  mMinThreshold = Midrange - Threshold;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionAnalog::SetPressThreshold(long Value)
{
  mPressThreshold = Value;

  // Update thresholds
  long Range = mMaxValue - mMinValue;
  long Radius = Range / 2;
  long Midrange = mMinValue + Radius;
  long Threshold = Radius * mPressThreshold / 100;
  mMaxThreshold = Midrange + Threshold;
  mMinThreshold = Midrange - Threshold;
}
//---------------------------------------------------------------------------


void __fastcall TInputActionAnalog::OnDisplayResize(int NewWidth, int NewHeight)
{
  mDisplayWidth = NewWidth;
  mDisplayHeight = NewHeight;
}
//---------------------------------------------------------------------------


void __fastcall TInputActionAnalog::OnChange(long NewValue)
{
  // Max button area
  if (NewValue > mMaxThreshold)
  {
    PressButton(ANALOG_INDEXBUTTON_MAX);
  }
  // Min button area
  else if (NewValue < mMinThreshold)
  {
    PressButton(ANALOG_INDEXBUTTON_MIN);
  }
  else
  {
    // Release pressed buttons
    ReleaseButton(ANALOG_INDEXBUTTON_MAX);
    ReleaseButton(ANALOG_INDEXBUTTON_MIN);
  }

  if ((mMouseXActive == true) && (mOnMouseXEvent != NULL) )
  {
    int X = (NewValue - mMinValue) * mDisplayWidth / (mMaxValue - mMinValue);
    mOnMouseXEvent(X);
  }

  if ( (mMouseYActive == true) && (mOnMouseYEvent != NULL) )
  {
    int Y = (NewValue - mMinValue) * mDisplayHeight / (mMaxValue - mMinValue);
    mOnMouseYEvent(Y);
  }
}
//---------------------------------------------------------------------------


long __fastcall TInputActionAnalog::GetDeadZoneMin(void)
{
  return cDeadZoneMin;
}
//---------------------------------------------------------------------------
long __fastcall TInputActionAnalog::GetDeadZoneMax(void)
{
  return cDeadZoneMax;
}
//---------------------------------------------------------------------------
void __fastcall TInputActionAnalog::SetDeadZone(long Value)
{
  if (Value <= cDeadZoneMin)
  {
    mDeadZone = cDeadZoneMin;
  }
  if (Value >= cDeadZoneMax)
  {
    mDeadZone = cDeadZoneMax;
  }
  else
  {
    mDeadZone = Value;
  }
};
//---------------------------------------------------------------------------

