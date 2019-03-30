/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2016 by Frédéric Coste

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

#include "TFormCustomSpeed.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


TFormCustomSpeed *FormCustomSpeed;
//---------------------------------------------------------------------------
__fastcall TFormCustomSpeed::TFormCustomSpeed(TComponent* Owner)
  : TForm(Owner)
{
  mCustomSpeed = -1;
  mSettingsP = NULL;

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  LabelCustomSpeed->Font->Size = 12;
}
//---------------------------------------------------------------------------


void __fastcall TFormCustomSpeed::FormShow(TObject *Sender)
{
  if (mSettingsP == NULL)
  {
    Close();
    return;
  }

  EditCustomSpeed->Text = AnsiString(mCustomSpeed);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCustomSpeed::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Color = mSettingsP->ThemeText1Color;
  LabelCustomSpeed->Font->Color = mSettingsP->ThemeText1Color;
  LabelNote->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormCustomSpeed::EditCustomSpeedChange(TObject *Sender)
{
  if ( (EditCustomSpeed->Focused() == true)
       && (EditCustomSpeed->Text.Trim().Length() > 0) )
  {
    try
    {
      mCustomSpeed = EditCustomSpeed->Text.Trim().ToInt();
      ButtonOK->Enabled = (mCustomSpeed > 1) ? true : false;
    }
    catch(...)
    {
      ButtonOK->Enabled = false;
    }
  }
  else
  {
    ButtonOK->Enabled = false;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
