/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2018 by Frédéric Coste

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

#include "TFormDebugEditValue.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


TFormDebugEditValue *FormDebugEditValue;
//---------------------------------------------------------------------------
__fastcall TFormDebugEditValue::TFormDebugEditValue(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mCurrentValue = 0;
  mNewValue = 0;
  mHexadecimal = false;
  mNbDigits = 2;
  mMinValue = -1;
  mMaxValue = -1;
  mIncrement = 1;
  mShowNext = false;
  mNextRequested = false;

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  PanelFormatCurrent->Font->Size = 12;
  LabelCurrentValue->Font->Size = 12;
  LabelCurrentValue->Height = 16;
  PanelFormatNew->Font->Size = 12;
  EditNew->Font->Size = 12;
  EditNew->Height = 16;
  EditIncrement->Font->Size = 12;
  EditIncrement->Height = 16;

  LabelCurrentValue->Top = ((PanelCurrent->Height - LabelCurrentValue->Height) / 2) - 1;
  EditNew->Top = ((PanelNew->Height - EditNew->Height) / 2) - 1;
  EditIncrement->Top = ((PanelIncrement->Height - EditIncrement->Height) / 2) - 1;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugEditValue::FormShow(TObject *Sender)
{
  mNewValue = mCurrentValue;

  if (mHexadecimal == true)
  {
    PanelFormatCurrent->Caption = "&&";
    LabelCurrentValue->Caption = AnsiString::IntToHex(mCurrentValue, mNbDigits);
    PanelFormatNew->Caption = "&&";
    EditNew->Text = AnsiString::IntToHex(mNewValue, mNbDigits);
    EditIncrement->Text = AnsiString::IntToHex(mIncrement, 0);
  }
  else
  {
    PanelFormatCurrent->Caption = "";
    LabelCurrentValue->Caption = mCurrentValue;
    PanelFormatNew->Caption = "";
    EditNew->Text = mNewValue;
    EditIncrement->Text = mIncrement;
  }

  EditNew->MaxLength = mNbDigits;
  PanelNext->Visible = mShowNext;

  // Apply theme
  if (mSettingsP)
  {
    Color = mSettingsP->ThemeBackgroundColor;
    LabelCurrent->Font->Color = mSettingsP->ThemeText1Color;
    PanelFormatCurrent->Font->Color = mSettingsP->ThemeText1Color;
    LabelCurrentValue->Font->Color = mSettingsP->ThemeText1Color;
    LabelNew->Font->Color = mSettingsP->ThemeText2Color;
    if (mNextRequested == true)
    {
      PanelNext->Color = mSettingsP->ThemeText2Color;
      PanelNext->Font->Color = mSettingsP->ThemeBackgroundColor;
    }
    else
    {
      PanelNext->Color = mSettingsP->ThemeBackgroundColor;
      PanelNext->Font->Color = mSettingsP->ThemeText2Color;
    }
  }

  EditNew->SetFocus();
  EditNew->SelectAll();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugEditValue::EditNewChange(TObject *Sender)
{
  if ( (EditNew->Focused() == true)
       && (EditNew->Text.Trim().Length() > 0) )
  {
    // Presence of char '&'
    bool Hexa = mHexadecimal;
    AnsiString StringNewValue = EditNew->Text.Trim();
    if (StringNewValue.Pos("&") == 1)
    {
      StringNewValue = StringNewValue.SubString(2, StringNewValue.Length());
      Hexa = true;
    }

    try
    {
      if (Hexa == true)
      {
        mNewValue = StrToInt("0x" + StringNewValue);
      }
      else
      {
        mNewValue = StringNewValue.ToInt();
      }

      // Check valid value
      if ( (mMaxValue != -1) && (mNewValue > mMaxValue) )
      {
        ButtonOK->Enabled = false;
      }
      else if ( (mMinValue != -1) && (mNewValue < mMinValue) )
      {
        ButtonOK->Enabled = false;
      }
      else
      {
        ButtonOK->Enabled = true;
      }
    }
    catch(...)
    {
      mNewValue = mCurrentValue;
      ButtonOK->Enabled = false;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugEditValue::PanelFormatNewClick(TObject *Sender)
{
  EditNew->SetFocus();
  EditNew->SelectAll();

  mHexadecimal = (mHexadecimal == true) ? false : true;
  if (mHexadecimal == true)
  {
    PanelFormatNew->Caption = "&&";
    EditNew->Text = AnsiString::IntToHex(mNewValue, mNbDigits);
    EditIncrement->Text = AnsiString::IntToHex(mIncrement, 0);
    PanelFormatCurrent->Caption = "&&";
    LabelCurrentValue->Caption = AnsiString::IntToHex(mCurrentValue, mNbDigits);
  }
  else
  {
    PanelFormatNew->Caption = "";
    EditNew->Text = mNewValue;
    EditIncrement->Text = mIncrement;
    PanelFormatCurrent->Caption = "";
    LabelCurrentValue->Caption = mCurrentValue;
  }


	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugEditValue::EditIncrementChange(TObject *Sender)
{
  if ( (EditIncrement->Focused() == true)
       && (EditIncrement->Text.Trim().Length() > 0) )
  {
    // Presence of char '&'
    bool Hexa = mHexadecimal;
    AnsiString StringNewValue = EditIncrement->Text.Trim();
    if (StringNewValue.Pos("&") == 1)
    {
      StringNewValue = StringNewValue.SubString(2, StringNewValue.Length());
      Hexa = true;
    }

    try
    {
      if (Hexa == true)
      {
        mIncrement = StrToInt("0x" + StringNewValue);
      }
      else
      {
        mIncrement = StringNewValue.ToInt();
      }

      if (mIncrement < 1)
      {
        mIncrement = 1;
      }
    }
    catch(...)
    {
      mIncrement = 1;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugEditValue::ButtonIncrementClick(TObject *Sender)
{
  try
  {
    if (mHexadecimal == true)
    {
      mNewValue = StrToInt("0x" + EditNew->Text);
    }
    else
    {
      mNewValue = EditNew->Text.ToInt();
    }

    if ( (mMaxValue != -1) && (mMinValue != -1) )
    {
      if ( (mMaxValue - mNewValue) >= mIncrement)
      {
        mNewValue += mIncrement;
      }
      else
      {
        mNewValue = mMinValue + (mIncrement - ((mMaxValue+1) - mNewValue));
      }
    }
    else
    {
      mNewValue += mIncrement;
    }

    if (mHexadecimal == true)
    {
      EditNew->Text = AnsiString::IntToHex(mNewValue, mNbDigits);
    }
    else
    {
      EditNew->Text = mNewValue;
    }

    ButtonOK->Enabled = true;
  }
  catch(...)
  {
    mNewValue = mCurrentValue;
    ButtonOK->Enabled = false;
  }

  ActiveControl = EditNew;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugEditValue::ButtonDecrementClick(TObject *Sender)
{
  try
  {
    if (mHexadecimal == true)
    {
      mNewValue = StrToInt("0x" + EditNew->Text);
    }
    else
    {
      mNewValue = EditNew->Text.ToInt();
    }

    if ( (mMaxValue != -1) && (mMinValue != -1) )
    {
      if ( (mNewValue - mMinValue) >= mIncrement)
      {
        mNewValue -= mIncrement;
      }
      else
      {
        mNewValue = mMaxValue - (mIncrement - ((mNewValue+1) - mMinValue));
      }
    }
    else
    {
      mNewValue -= mIncrement;
    }

    if (mHexadecimal == true)
    {
      EditNew->Text = AnsiString::IntToHex(mNewValue, mNbDigits);
    }
    else
    {
      EditNew->Text = mNewValue;
    }

    ButtonOK->Enabled = true;
  }
  catch(...)
  {
    mNewValue = mCurrentValue;
    ButtonOK->Enabled = false;
  }

  ActiveControl = EditNew;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDebugEditValue::FormKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == '+')
	{
		ButtonIncrementClick(Sender);
		Key = 0; // Stop key press propagation to focused control
	}
	else if (Key == '-')
	{
		ButtonDecrementClick(Sender);
		Key = 0; // Stop key press propagation to focused control
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugEditValue::PanelNextClick(TObject *Sender)
{
  mNextRequested = mNextRequested == true ? false : true;

  if (mNextRequested == true)
  {
    PanelNext->Color = mSettingsP->ThemeText2Color;
    PanelNext->Font->Color = mSettingsP->ThemeBackgroundColor;
  }
  else
  {
    PanelNext->Color = mSettingsP->ThemeBackgroundColor;
    PanelNext->Font->Color = mSettingsP->ThemeText2Color;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

