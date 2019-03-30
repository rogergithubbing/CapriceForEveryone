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

#include "TFormDebugBASICColors.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


TFormDebugBASICColors *FormDebugBASICColors;
//---------------------------------------------------------------------------
__fastcall TFormDebugBASICColors::TFormDebugBASICColors(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;

  // Init component lists
  for (int Loop=0; Loop <= 26; Loop++)
  {
    mPanelSelect[Loop] = (TPanel*)FindComponent("PanelSelect" + AnsiString(Loop));
  }

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugBASICColors::FormShow(TObject *Sender)
{
  // Apply theme
  if (mSettingsP)
  {
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
    for (int Loop=0; Loop <= 26; Loop++)
    {
      TPanel* PanelP = (TPanel*)FindComponent("PanelName" + AnsiString(Loop));
      if (PanelP)
      {
        PanelP->Font->Color = mSettingsP->ThemeText2Color;
      }
    }
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugBASICColors::SetColor(int Value)
{
  mColor = Value;

  // Select new color
  if (mColor <= 26)
  {
    mPanelSelect[mColor]->BevelInner = bvRaised;
    mPanelSelect[mColor]->BevelOuter = bvRaised;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugBASICColors::PanelSelectClick(TObject *Sender)
{
  mColor = ((TPanel*)Sender)->Tag;

  ModalResult = mrOk;
}
//---------------------------------------------------------------------------

