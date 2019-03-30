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
#include "TEmulatorMessage.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


#define EMULATORMESSAGE_FIRSTALPHA      1.0f
#define EMULATORMESSAGE_ALPHASTEP       0.05f


__fastcall TEmulatorMessage::TEmulatorMessage(String Message, TBaseTime TillEvent)
//
// Constructor
//
{
  mMessage = Message;
  mTillEvent = TillEvent;
  mTransparency = EMULATORMESSAGE_FIRSTALPHA;
}
//---------------------------------------------------------------------------


__fastcall TEmulatorMessage::~TEmulatorMessage(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


bool __fastcall TEmulatorMessage::IsDurationElapsed(TBaseTimer* BaseTimerP)
{
  return  BaseTimerP->IsNextOccurenceElapsed(&mTillEvent);
}
//---------------------------------------------------------------------------


bool __fastcall TEmulatorMessage::DecreaseTransparency(void)
{
bool bFullTransparent = false;

  if (mTransparency > EMULATORMESSAGE_ALPHASTEP)
  {
    mTransparency -= EMULATORMESSAGE_ALPHASTEP;
  }
  else
  {
    bFullTransparent = true;
  }

  return bFullTransparent;
}
//---------------------------------------------------------------------------

