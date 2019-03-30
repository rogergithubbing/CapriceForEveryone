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
#include "TFormEditKeycode.h"
#include "..\Inputs\Keyboard.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEditKeycode *FormEditKeycode;
//---------------------------------------------------------------------------
__fastcall TFormEditKeycode::TFormEditKeycode(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  ComboBoxNewKeycode->Items->Clear();

  // Add CPC virtual keycodes
  for (int loop=0; loop < 256; loop++)
  {
    AnsiString KeyString = AnsiString(virtual_kbd_string[loop]);

    if (KeyString.Pos("CPC_") == 1)
    {
      ComboBoxNewKeycode->Items->Add(KeyString);
    }
  }

  LabelHitKey = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormEditKeycode::FormShow(TObject *Sender)
{
  if (mSettingsP == NULL)
  {
    ModalResult = mrCancel;
    return;
  }

  // Apply Theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  Font->Color = mSettingsP->ThemeText1Color;
  LabelNewKeycode->Font->Color = mSettingsP->ThemeText2Color;
  PanelNewKeycode->Font->Color = mSettingsP->ThemeText2Color;
  ComboBoxNewKeycode->Font->Color = clBlack;

  TInputKey* InputKeyP = new TInputKey;
  InputKeyP->WINKeycode = mKey;
  InputKeyP->WINShiftState = mShiftState;
  PanelCurrentKeycode->Caption = mInputContextP->GetPressedKeyString(InputKeyP);
  delete InputKeyP;

  ComboBoxNewKeycode->Text = "";
  ButtonOK->Enabled = true;
  CheckBoxModifierCtrl->Checked = mShiftState.Contains(ssCtrl) ? true : false;
  CheckBoxModifierShift->Checked = mShiftState.Contains(ssShift) ? true : false;
  CheckBoxMouseButton->Checked = mMouseButton;

  if (mMouseButton == false)
  {
    PageControlKeycode->ActivePage = TabSheetKeyboard;
  }
  else
  {
    PageControlKeycode->ActivePage = TabSheetMouse;
  }

  // Init to delete button action if button OK pressed.
  mKey = 0;
  mShiftState = TShiftState();

  mLastAppMessage = Application->OnMessage;
  Application->OnMessage = AppMessage;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditKeycode::FormHide(TObject *Sender)
{
  // Restore AppMessage
  Application->OnMessage = mLastAppMessage;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditKeycode::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (PageControlKeycode->Visible == false)
  {
    if ( (Key != VK_SHIFT) && (Key != VK_CONTROL) )
    {
      delete LabelHitKey;
      PanelButtons->Visible = true;
      PageControlKeycode->Visible = true;

      TInputKey* InputKeyP = new TInputKey;
      InputKeyP->WINKeycode = Key;
      InputKeyP->WINShiftState = Shift;
      PanelNewKeycode->Caption = mInputContextP->GetPressedKeyString(InputKeyP);
      if (PanelNewKeycode->Caption.IsEmpty() == false)
      {
        mKey = Key;
        mShiftState = Shift;
      }
      else
      {
        mKey = 0;
        mShiftState = TShiftState();
      }
      delete InputKeyP;

      CheckBoxModifierCtrl->Checked = mShiftState.Contains(ssCtrl) ? true : false;
      CheckBoxModifierShift->Checked = mShiftState.Contains(ssShift) ? true : false;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEditKeycode::ButtonNewKeycodeClick(TObject *Sender)
{
  PageControlKeycode->Visible = false;
  PanelButtons->Visible = false;

  LabelHitKey = new TLabel(this);
  LabelHitKey->Parent = this;
  LabelHitKey->AutoSize = false;
  LabelHitKey->Align = alClient;
  LabelHitKey->Alignment = taCenter;
  LabelHitKey->Layout = tlCenter;
  LabelHitKey->Font->Color = mSettingsP->ThemeText2Color;
  LabelHitKey->Caption = "Now hit any key on the keyboard.";

  ActiveControl = NULL;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEditKeycode::ComboBoxNewKeycodeClick(TObject *Sender)
{
  // Reset modifiers
  CheckBoxModifierCtrl->Checked = false;
  CheckBoxModifierShift->Checked = false;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditKeycode::ComboBoxNewKeycodeDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxNewKeycode->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxNewKeycode->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxNewKeycode->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

	NOT_USED(Control);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditKeycode::ComboBoxNewKeycodeChange(TObject *Sender)
{
  if (ComboBoxNewKeycode->Focused())
  {
    if (ComboBoxNewKeycode->Text.IsEmpty() == false)
    {
      PanelNewKeycode->Caption = ComboBoxNewKeycode->Text;
      ComboBoxNewKeycode->ItemIndex = -1;

      mKey = 0;

      mShiftState = TShiftState();
      CheckBoxModifierShift->Checked = false;
      CheckBoxModifierCtrl->Checked = false;

      for (int loop=0; loop < 256; loop++)
      {
        if (PanelNewKeycode->Caption == AnsiString(virtual_kbd_string[loop]))
        {
          mKey = (WORD)loop;
          break;
        }
      }
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEditKeycode::SetMouseEnable(bool Enable)
{
  CheckBoxMouseButton->Visible = Enable;
  TabSheetMouse->TabVisible = Enable;

  if (Enable == false)
  {
    CheckBoxMouseButton->Checked = false;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormEditKeycode::CheckBoxMouseButtonClick(TObject *Sender)
{
  mMouseButton = CheckBoxMouseButton->Checked;  

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEditKeycode::AppMessage(tagMSG &Msg, bool &Handled)
{
  // Capture WM_SYSKEYDOWN and WM_SYSKEYUP event to get VK_ALT
  if (Msg.message == WM_SYSKEYDOWN)
  {
    Perform(WM_KEYDOWN, Msg.wParam, Msg.lParam);
    Handled = true;
  }
  else if (Msg.message == WM_SYSKEYUP)
  {
    Perform(WM_KEYUP, Msg.wParam, Msg.lParam);
    Handled = true;
  }
  else if (this->Focused() == true)
	{
		// Capture WM_KEYDOWN event to get VK_TAB
		if (Msg.message == WM_KEYDOWN)
		{
			Perform(WM_KEYDOWN, Msg.wParam, Msg.lParam);
			Handled = true;
		}
		else if (Msg.message == WM_KEYUP)
		{
			Perform(WM_KEYUP, Msg.wParam, Msg.lParam);
			Handled = true;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormEditKeycode::CheckBoxModifierShiftClick(TObject *Sender)
{
  if (CheckBoxModifierShift->Focused() == true)
  {
    if (PanelNewKeycode->Caption.IsEmpty() == false)
    {
      if (CheckBoxModifierShift->Checked == true)
      {
        mShiftState = mShiftState << ssShift;
      }
      else
      {
        mShiftState = mShiftState >> ssShift;
      }

      TInputKey* InputKeyP = new TInputKey;
      InputKeyP->WINKeycode = mKey;
      InputKeyP->WINShiftState = mShiftState;
      PanelNewKeycode->Caption = mInputContextP->GetPressedKeyString(InputKeyP);
      delete InputKeyP;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEditKeycode::CheckBoxModifierCtrlClick(TObject *Sender)
{
  if (CheckBoxModifierCtrl->Focused() == true)
  {
    if (PanelNewKeycode->Caption.IsEmpty() == false)
    {
      if (CheckBoxModifierCtrl->Checked == true)
      {
        mShiftState = mShiftState << ssCtrl;
      }
      else
      {
        mShiftState = mShiftState >> ssCtrl;
      }

      TInputKey* InputKeyP = new TInputKey;
      InputKeyP->WINKeycode = mKey;
      InputKeyP->WINShiftState = mShiftState;
      PanelNewKeycode->Caption = mInputContextP->GetPressedKeyString(InputKeyP);
      delete InputKeyP;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

