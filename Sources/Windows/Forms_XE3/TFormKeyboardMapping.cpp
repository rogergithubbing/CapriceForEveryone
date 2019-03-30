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
#include <tchar.h>
#pragma hdrstop
#include "TFormKeyboardMapping.h"
#include "..\Inputs\keyboard.h"


#define COLOR_KEY_DEFAULT             TColor(0x00D0D0D0) // ABGR
#define COLOR_KEY_SELECTED            TColor(0x00FFF0A0) // ABGR


#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKeyboardMapping *FormKeyboardMapping;
//---------------------------------------------------------------------------
__fastcall TFormKeyboardMapping::TFormKeyboardMapping(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;

  mGetASCIIKeyIndex = 0;
  mGetHardwareKeyIndex = 0;
  mGetJoystickKeyIndex = 0;

  mCurrentGraphicKeyP = NULL;
  mKeyboardCPCP = NULL;

  mASCIIMappingP = new TList;
  mHardwareMappingP = new TList;
  mJoystickMappingP = new TList;

  // Set all keys default color
  for (int Loop=0; Loop < ComponentCount; Loop++)
  {
    TComponent* Component = Components[Loop];
    if (Component->Name.Pos("PanelGraphicKey") == 1)
    {
      TPanel* Panel = dynamic_cast<TPanel*>(Component);
      Panel->Color = COLOR_KEY_DEFAULT;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::FormDestroy(TObject *Sender)
{
  // Destroy ASCII Mapping
  while (mASCIIMappingP->Count > 0)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mASCIIMappingP->Items[0];
    delete KeyMappingP;
    mASCIIMappingP->Delete(0);
  }
  delete mASCIIMappingP;

  // Destroy Hardware Mapping
  while (mHardwareMappingP->Count > 0)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mHardwareMappingP->Items[0];
    delete KeyMappingP;
    mHardwareMappingP->Delete(0);
  }
  delete mHardwareMappingP;

  // Destroy Joystick Mapping
  while (mJoystickMappingP->Count > 0)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mJoystickMappingP->Items[0];
    delete KeyMappingP;
    mJoystickMappingP->Delete(0);
  }
  delete mJoystickMappingP;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::FormShow(TObject *Sender)
{
  if (mSettingsP == NULL)
  {
    ModalResult = mrCancel;
    return;
  }

  ShowHint = mSettingsP->ShowHints;

  // Apply theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  Font->Color = mSettingsP->ThemeText1Color;
  TabControlGraphicLayout->Font->Color = clBlack;
  PanelGraphicKey72->Font->Color = clBlack;
  PanelGraphicKey73->Font->Color = clBlack;
  PanelGraphicKey74->Font->Color = clBlack;
  PanelGraphicKey75->Font->Color = clBlack;
  PanelGraphicKey76->Font->Color = clBlack;
  PanelGraphicKey77->Font->Color = clBlack;
  PanelFormatKeycode->Font->Color = clBlack;
  PanelGraphicWindowsVK->Font->Color = clRed;
  PanelGraphicHardwareKey->Font->Color = clBlue;
  PanelJoystickKey->Font->Color = clGreen;
  PanelGraphicASCIIKey->Font->Color = clBlue;
  CheckBoxUseHardwareKeyboardMapping->Font->Color = mSettingsP->ThemeText2Color;
  LabelHowToUse->Font->Color = mSettingsP->ThemeText2Color;


  OldAppMessage = Application->OnMessage;
  Application->OnMessage = AppMessage;

  CheckBoxUseHardwareKeyboardMapping->Checked = mSettingsP->UseHardwareKeyboardMapping;

  // ASCII Keyboard
  LoadASCIIMapping();

  // Hardware Keyboard
  LoadHardwareMapping();

  // Joystick mapping
  LoadJoystickMapping();

  //
  // Graphic mapping
  //
  TabControlGraphicLayout->TabIndex = 0;
  TabControlGraphicLayoutChange(Sender);
  if (mCurrentGraphicKeyP)
  {
    // SHIFT keys
    if (mCurrentGraphicKeyP->Tag == 21)
    {
      PanelGraphicKey21L->Color = COLOR_KEY_DEFAULT;
      PanelGraphicKey21R->Color = COLOR_KEY_DEFAULT;
    }
    else
    {
      mCurrentGraphicKeyP->Color = COLOR_KEY_DEFAULT;
    }
    mCurrentGraphicKeyP = NULL;
  }
  ButtonGraphicWindowsVKFind->Enabled = false;
  ButtonJoystickSet->Enabled = false;
  ButtonGraphicHardwareSet->Enabled = false;
  ButtonGraphicASCIISet->Enabled = false;
  GroupBoxGraphicHardwareMapping->Visible = false;
  GroupBoxGraphicASCIIMapping->Visible = false;
  GroupBoxJoystickMapping->Visible = false;
  PanelGraphicWindowsVK->Caption = "";
  PanelGraphicHardwareKey->Caption = "";
  PanelJoystickKey->Caption = "";

  mLastWindowsKeyPressed = -1;
  mJoystickKey = -1;
  mHardwareKey = -1;
  mASCIIKey = -1;

  ActiveControl = ButtonCancel;
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::FormHide(TObject *Sender)
{
  if (mSettingsP != NULL)
  {
    Application->OnMessage = OldAppMessage;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::AppMessage(tagMSG &Msg, bool &Handled)
{
  // Capture WM_KEYDOWN event to get VK_TAB
  if (Msg.message == WM_KEYDOWN)
  {
    Perform(WM_KEYDOWN, Msg.wParam, Msg.lParam);
    Handled = true;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::WMKeyDown(TWMKey &Message)
{
TShiftState Shift;

  if (Message.CharCode == VK_RETURN)
  {
    if (Message.KeyData & 0x01000000)
    {
      Message.CharCode = VK_ENTER;
    }
  }

  if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
  {
    Shift = Shift << ssShift;
  }
  if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
  {
    Shift = Shift << ssCtrl;
  }
  if (GetAsyncKeyState(VK_ALT) & 0x8000)
  {
    Shift = Shift << ssAlt;
  }

  FormKeyDown(this, Message.CharCode, Shift);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::ButtonApplyClick(TObject *Sender)
{
  mSettingsP->UseHardwareKeyboardMapping = CheckBoxUseHardwareKeyboardMapping->Checked;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormKeyboardMapping::FormKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
  mLastWindowsKeyPressed = Key;
  PanelGraphicWindowsVK->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
          AnsiString(Key) :
          AnsiString(windows_vk_string[Key]);

  if (PanelGraphicWindowsVK->Caption.Length() > 0)
  {
    ButtonGraphicWindowsVKFind->Enabled = true;
    ButtonJoystickSet->Enabled = true;
    ButtonGraphicHardwareSet->Enabled = true;
    ButtonGraphicASCIISet->Enabled = true;
  }

  Key = 0;

	NOT_USED(Sender);
	NOT_USED(Shift);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::ButtonDefaultClick(TObject *Sender)
{
	if (Application->MessageBox(_T("Are you sure to restore default mapping ?"),
                              _T("Default Mapping"),
                              MB_YESNO) == IDNO)
  {
    return;
  }

  // Restore ASCII mapping
  mSettingsP->RestoreDefaultNonASCIIKeys();
  LoadASCIIMapping();

  // Restore Hardware mapping
  mSettingsP->RestoreDefaultHardwareKeys();
  LoadHardwareMapping();

  // Restore Joystick mapping
  mSettingsP->RestoreDefaultJoystickKeys();
  LoadJoystickMapping();

  if (mCurrentGraphicKeyP)
  {
    // SHIFT keys
    if (mCurrentGraphicKeyP->Tag == 21)
    {
      PanelGraphicKey21L->Color = COLOR_KEY_DEFAULT;
      PanelGraphicKey21R->Color = COLOR_KEY_DEFAULT;
    }
    else
    {
      mCurrentGraphicKeyP->Color = COLOR_KEY_DEFAULT;
    }
    mCurrentGraphicKeyP = NULL;
  }
  ButtonJoystickSet->Enabled = false;
  ButtonGraphicHardwareSet->Enabled = false;
  ButtonGraphicASCIISet->Enabled = false;
  GroupBoxGraphicHardwareMapping->Visible = false;
  GroupBoxGraphicASCIIMapping->Visible = false;
  PanelGraphicWindowsVK->Caption = "";
  PanelGraphicHardwareKey->Caption = "";

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


tKeyboardMappingKey* __fastcall TFormKeyboardMapping::GetFirstASCIIKey(void)
{
  mGetASCIIKeyIndex = -1;

  return (GetNextASCIIKey());
}
//---------------------------------------------------------------------------


tKeyboardMappingKey* __fastcall TFormKeyboardMapping::GetNextASCIIKey(void)
{
tKeyboardMappingKey* KeyP = NULL;

  mGetASCIIKeyIndex++;

  if (mGetASCIIKeyIndex < mASCIIMappingP->Count)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mASCIIMappingP->Items[mGetASCIIKeyIndex];

    KeyP = new tKeyboardMappingKey;
    KeyP->WindowsKey = KeyMappingP->WindowsKey;
    KeyP->CPCKey = KeyMappingP->CPCKey;
  }

  return (KeyP);
}
//---------------------------------------------------------------------------


tKeyboardMappingKey* __fastcall TFormKeyboardMapping::GetFirstHardwareKey(void)
{
  mGetHardwareKeyIndex = -1;

  return (GetNextHardwareKey());
}
//---------------------------------------------------------------------------


tKeyboardMappingKey* __fastcall TFormKeyboardMapping::GetNextHardwareKey(void)
{
tKeyboardMappingKey* ReturnKeyP = NULL;

  mGetHardwareKeyIndex++;

  if (mGetHardwareKeyIndex < mHardwareMappingP->Count)
  {
    tKeyboardMappingKey* HardwareKeyP = (tKeyboardMappingKey*)mHardwareMappingP->Items[mGetHardwareKeyIndex];

    ReturnKeyP = new tKeyboardMappingKey;
    ReturnKeyP->CPCKey = HardwareKeyP->CPCKey;
    ReturnKeyP->WindowsKey = HardwareKeyP->WindowsKey;
  }

  return (ReturnKeyP);
}
//---------------------------------------------------------------------------


tKeyboardMappingKey* __fastcall TFormKeyboardMapping::GetFirstJoystickKey(void)
{
  mGetJoystickKeyIndex = -1;

  return (GetNextJoystickKey());
}
//---------------------------------------------------------------------------


tKeyboardMappingKey* __fastcall TFormKeyboardMapping::GetNextJoystickKey(void)
{
tKeyboardMappingKey* KeyP = NULL;

  mGetJoystickKeyIndex++;

  if (mGetJoystickKeyIndex < mJoystickMappingP->Count)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mJoystickMappingP->Items[mGetJoystickKeyIndex];

    KeyP = new tKeyboardMappingKey;
    KeyP->WindowsKey = KeyMappingP->WindowsKey;
    KeyP->CPCKey = KeyMappingP->CPCKey;
  }

  return (KeyP);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::TabControlGraphicLayoutChange(TObject *Sender)
{
  // Update graphic keyboard layout
  if (TabControlGraphicLayout->TabIndex == 0)
  {
    ShowGraphicLayout(graphic_UK_keys_string);
  }
  else if (TabControlGraphicLayout->TabIndex == 1)
  {
    ShowGraphicLayout(graphic_FR_keys_string);
  }
  else if (TabControlGraphicLayout->TabIndex == 2)
  {
    ShowGraphicLayout(graphic_SP_keys_string);
  }
  else
  {
    ShowGraphicLayout(NULL);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormKeyboardMapping::TabControlGraphicLayoutDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = TabControlGraphicLayout->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }
  else
  {
    Canvas->Font->Color = mSettingsP->ThemeText1Color;
  }

  // Draw tab
  Canvas->FillRect(Rect);
  AnsiString Text = TabControlGraphicLayout->Tabs->Strings[TabIndex];
  int TextWidth = Canvas->TextWidth(Text);
  int TextHeight = Canvas->TextHeight(Text);
  int TextPosX = (Rect.Width() - TextWidth) / 2;
  int TextPosY = (TabControlGraphicLayout->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

	NOT_USED(Control);
}
//---------------------------------------------------------------------------
void __fastcall TFormKeyboardMapping::ShowGraphicLayout(const char ** Layout)
{
  for (int Loop=0; Loop < TabControlGraphicLayout->ControlCount; Loop++)
  {
    TPanel* PanelP = dynamic_cast<TPanel*>(TabControlGraphicLayout->Controls[Loop]);

    if (PanelP)
    {
      if (PanelP->Tag >= 0)
      {
        PanelP->Caption = (Layout != NULL) ? AnsiString(Layout[PanelP->Tag]) : AnsiString(PanelP->Tag);
      }

      if (PanelP->ControlCount == 1)
      {
        TImage* ImageP = dynamic_cast<TImage*>(PanelP->Controls[0]);
        ImageP->Visible = PanelP->Caption.Length() > 0 ? false : true;
      }
    }
  }

  for (int Loop=0; Loop < GroupBoxJoystick0->ControlCount; Loop++)
  {
    TPanel* PanelP = dynamic_cast<TPanel*>(GroupBoxJoystick0->Controls[Loop]);

    if (PanelP)
    {
      if (PanelP->Tag >= 0)
      {
        PanelP->Caption = (Layout != NULL) ? AnsiString(Layout[PanelP->Tag]) : AnsiString(PanelP->Tag);
      }

      if (PanelP->ControlCount == 1)
      {
        TImage* ImageP = dynamic_cast<TImage*>(PanelP->Controls[0]);
        ImageP->Visible = PanelP->Caption.Length() > 0 ? false : true;
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::PanelGraphicKeyClick(
      TObject *Sender)
{
  TPanel* CurrentKeyP = (TPanel*)Sender;

  if (mCurrentGraphicKeyP)
  {
    // SHIFT keys
    if (mCurrentGraphicKeyP->Tag == 21)
    {
      PanelGraphicKey21L->Color = COLOR_KEY_DEFAULT;
      PanelGraphicKey21R->Color = COLOR_KEY_DEFAULT;
    }
    else
    {
      mCurrentGraphicKeyP->Color = COLOR_KEY_DEFAULT;
    }
  }
  mCurrentGraphicKeyP = CurrentKeyP;
  // SHIFT keys
  if (CurrentKeyP->Tag == 21)
  {
    PanelGraphicKey21L->Color = COLOR_KEY_SELECTED;
    PanelGraphicKey21R->Color = COLOR_KEY_SELECTED;
  }
  else
  {
    mCurrentGraphicKeyP->Color = COLOR_KEY_SELECTED;
  }

  mASCIIKey = -1;
  mHardwareKey = -1;
  mJoystickKey = -1;

  //
  // Show Hardware mapping
  //
  for (int index=0; index < mHardwareMappingP->Count; index++)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mHardwareMappingP->Items[index];
    if (mCurrentGraphicKeyP->Tag == CPC_KBD_TO_USERMANUAL(KeyMappingP->CPCKey))
    {
      mHardwareKey = KeyMappingP->WindowsKey;
      PanelGraphicHardwareKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
                AnsiString(mHardwareKey) :
                AnsiString(windows_vk_string[mHardwareKey]);
      GroupBoxGraphicHardwareMapping->Visible = true;
      if (mLastWindowsKeyPressed != -1)
      {
        ButtonGraphicHardwareSet->Enabled = true;
      }
      break;
    }
  }

  //
  // Show ASCII mapping
  //
  GroupBoxGraphicASCIIMapping->Visible = false;
  for (int index=0; index < mASCIIMappingP->Count; index++)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mASCIIMappingP->Items[index];
    if ((mKeyboardCPCP[KeyMappingP->CPCKey] & 0xFF) == USERMANUAL_TO_CPC_KBD(mCurrentGraphicKeyP->Tag))
    {
      mASCIIKey = KeyMappingP->WindowsKey;
      PanelGraphicASCIIKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
            AnsiString(mASCIIKey) :
            AnsiString(windows_vk_string[mASCIIKey]);
      GroupBoxGraphicASCIIMapping->Visible = true;
      if (mLastWindowsKeyPressed != -1)
      {
        ButtonGraphicASCIISet->Enabled = true;
      }
      break;
    }
  }

  // Hide Joystick mapping
  GroupBoxJoystickMapping->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormKeyboardMapping::ImageGraphicKeyClick(
      TObject *Sender)
{
  PanelGraphicKeyClick(((TImage*)Sender)->Parent);
}
//---------------------------------------------------------------------------
void __fastcall TFormKeyboardMapping::PanelGraphicJoystick0KeyClick(
      TObject *Sender)
{
  TPanel* CurrentKeyP = (TPanel*)Sender;

  if (mCurrentGraphicKeyP)
  {
    // SHIFT keys
    if (mCurrentGraphicKeyP->Tag == 21)
    {
      PanelGraphicKey21L->Color = COLOR_KEY_DEFAULT;
      PanelGraphicKey21R->Color = COLOR_KEY_DEFAULT;
    }
    else
    {
      mCurrentGraphicKeyP->Color = COLOR_KEY_DEFAULT;
    }
  }
  mCurrentGraphicKeyP = CurrentKeyP;
  mCurrentGraphicKeyP->Color = COLOR_KEY_SELECTED;

  mASCIIKey = -1;
  mHardwareKey = -1;
  mJoystickKey = -1;

  //
  // Show Keyboard mapping
  //
  GroupBoxGraphicHardwareMapping->Visible = false;

  // Hide ASCII mapping
  GroupBoxGraphicASCIIMapping->Visible = false;

  // Show Joystick mapping
  for (int loop=0; loop < mJoystickMappingP->Count; loop++)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mJoystickMappingP->Items[loop];
    if (default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_UP + KeyMappingP->CPCKey - CPC_KEY_J0_UP]
         == USERMANUAL_TO_CPC_KBD(mCurrentGraphicKeyP->Tag))
    {
      mJoystickKey = KeyMappingP->WindowsKey;
      PanelJoystickKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
            AnsiString(mJoystickKey) :
            AnsiString(windows_vk_string[mJoystickKey]);
      GroupBoxJoystickMapping->Visible = true;
      if (mLastWindowsKeyPressed != -1)
      {
        ButtonJoystickSet->Enabled = true;
      }
      break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormKeyboardMapping::ImageGraphicKeyJoystick0Click(
      TObject *Sender)
{
  PanelGraphicJoystick0KeyClick(((TImage*)Sender)->Parent);
}
//---------------------------------------------------------------------------



void __fastcall TFormKeyboardMapping::ButtonGraphicWindowsVKFindClick(
      TObject *Sender)
{
  if (mCurrentGraphicKeyP)
  {
    // SHIFT keys
    if (mCurrentGraphicKeyP->Tag == 21)
    {
      PanelGraphicKey21L->Color = COLOR_KEY_DEFAULT;
      PanelGraphicKey21R->Color = COLOR_KEY_DEFAULT;
    }
    else
    {
      mCurrentGraphicKeyP->Color = COLOR_KEY_DEFAULT;
    }

    mCurrentGraphicKeyP = NULL;
  }
  
  PanelGraphicHardwareKey->Caption = "";
  ButtonJoystickSet->Enabled = false;
  ButtonGraphicHardwareSet->Enabled = false;
  GroupBoxGraphicHardwareMapping->Visible = false;
  ButtonGraphicASCIISet->Enabled = false;
  GroupBoxGraphicASCIIMapping->Visible = false;

  int CPCKeycode = -1;

  //
  // ASCII mapping
  //
  for (int index=0; index < mASCIIMappingP->Count; index++)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mASCIIMappingP->Items[index];
    if (KeyMappingP->WindowsKey == mLastWindowsKeyPressed)
    {
      CPCKeycode = CPC_KBD_TO_USERMANUAL(mKeyboardCPCP[KeyMappingP->CPCKey]);
      break;
    }
  }

  //
  // Hardware mapping
  //
  if (CPCKeycode == -1)
  {
    for (int index=0; index < mHardwareMappingP->Count; index++)
    {
      tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mHardwareMappingP->Items[index];
      if (KeyMappingP->WindowsKey == mLastWindowsKeyPressed)
      {
        CPCKeycode = CPC_KBD_TO_USERMANUAL(KeyMappingP->CPCKey);
        break;
      }
    }
  }

  //
  // Select Key
  //
  if (CPCKeycode != -1)
  {
    for (int Loop=0; Loop < TabControlGraphicLayout->ControlCount; Loop++)
    {
      TPanel* PanelP = dynamic_cast<TPanel*>(TabControlGraphicLayout->Controls[Loop]);

      if (PanelP->Tag == CPCKeycode)
      {
        mCurrentGraphicKeyP = PanelP;
        break;
      }
    }

    for (int Loop=0; Loop < GroupBoxJoystick0->ControlCount; Loop++)
    {
      TPanel* PanelP = dynamic_cast<TPanel*>(GroupBoxJoystick0->Controls[Loop]);

      if (PanelP->Tag == CPCKeycode)
      {
        mCurrentGraphicKeyP = PanelP;
        break;
      }
    }

    // Select Key
    if (mCurrentGraphicKeyP)
    {
      PanelGraphicKeyClick(mCurrentGraphicKeyP);
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::ButtonJoystickSetClick(TObject *Sender)
{
  if (mJoystickKey != -1)
  {
    for (int loop = 0; loop < NB_JOYSTICK_KEYS; loop++)
    {
      if (default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_UP + loop]
           == USERMANUAL_TO_CPC_KBD(mCurrentGraphicKeyP->Tag))
      {
        mJoystickKey = mLastWindowsKeyPressed;
        PanelJoystickKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
            AnsiString(mJoystickKey) :
            AnsiString(windows_vk_string[mJoystickKey]);
        // Update existing key
        tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mJoystickMappingP->Items[loop];
        KeyMappingP->WindowsKey = mLastWindowsKeyPressed;
        break;
      }
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::ButtonGraphicHardwareSetClick(
      TObject *Sender)
{
  if (mHardwareKey != -1)
  {
    for (int loop=0; loop < mHardwareMappingP->Count; loop++)
    {
      tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mHardwareMappingP->Items[loop];
      if (KeyMappingP->CPCKey == USERMANUAL_TO_CPC_KBD(mCurrentGraphicKeyP->Tag))
      {
        mHardwareKey = mLastWindowsKeyPressed;
        PanelGraphicHardwareKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
            AnsiString(mHardwareKey) :
            AnsiString(windows_vk_string[mHardwareKey]);
        // Update existing key
        KeyMappingP->WindowsKey = mLastWindowsKeyPressed;
        break;
      }
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::ButtonGraphicASCIISetClick(
      TObject *Sender)
{
  for (int loop=0; loop < mASCIIMappingP->Count; loop++)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mASCIIMappingP->Items[loop];
    if ((mKeyboardCPCP[KeyMappingP->CPCKey] & 0xFF) == USERMANUAL_TO_CPC_KBD(mCurrentGraphicKeyP->Tag))
    {
      mASCIIKey = mLastWindowsKeyPressed;
      PanelGraphicASCIIKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
          AnsiString(mASCIIKey) :
          AnsiString(windows_vk_string[mASCIIKey]);
      // Update existing key
      KeyMappingP->WindowsKey = mLastWindowsKeyPressed;
      break;
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::PanelFormatKeycodeClick(
      TObject *Sender)
{
  if (PanelFormatKeycode->Caption.IsEmpty() == true)
  {
    PanelFormatKeycode->Caption = "$";
  }
  else
  {
    PanelFormatKeycode->Caption = "";
  }

  // Update display
  if (mLastWindowsKeyPressed != -1)
  {
    PanelGraphicWindowsVK->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
          AnsiString(mLastWindowsKeyPressed) :
          AnsiString(windows_vk_string[mLastWindowsKeyPressed]);
  }
  if (mJoystickKey != -1)
  {
    PanelJoystickKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
          AnsiString(mJoystickKey) :
          AnsiString(windows_vk_string[mJoystickKey]);
  }
  if (mHardwareKey != -1)
  {
    PanelGraphicHardwareKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
          AnsiString(mHardwareKey) :
          AnsiString(windows_vk_string[mHardwareKey]);
  }
  if (mASCIIKey != -1)
  {
    PanelGraphicASCIIKey->Caption = PanelFormatKeycode->Caption.IsEmpty() == true ?
          AnsiString(mASCIIKey) :
          AnsiString(windows_vk_string[mASCIIKey]);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormKeyboardMapping::LoadASCIIMapping(void)
{
  const tKeyboardMappingKey* Key = mSettingsP->GetFirstNonASCIIKey();

  if (Key == NULL)
  {
    // Restore default mapping
    mSettingsP->RestoreDefaultNonASCIIKeys();
    Key = mSettingsP->GetFirstNonASCIIKey();
  }

  // Clear ASCII mapping list
  while (mASCIIMappingP->Count > 0)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mASCIIMappingP->Items[0];
    delete KeyMappingP;
    mASCIIMappingP->Delete(0);
  }

  // Show all NON ASCII Keys
  while (Key != NULL)
  {
    // Store key mapping
    tKeyboardMappingKey* KeyMappingP = new tKeyboardMappingKey;
    KeyMappingP->CPCKey = Key->CPCKey;
    KeyMappingP->WindowsKey = Key->WindowsKey;
    mASCIIMappingP->Add(KeyMappingP);

    // Next Key
    Key = mSettingsP->GetNextNonASCIIKey();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormKeyboardMapping::LoadHardwareMapping(void)
{
  const tKeyboardMappingKey* Key = mSettingsP->GetFirstHardwareKey();
  if (Key == NULL)
  {
    // Restore default mapping
    mSettingsP->RestoreDefaultHardwareKeys();
    Key = mSettingsP->GetFirstHardwareKey();
  }

  // Clear hardware mapping list
  while (mHardwareMappingP->Count > 0)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mHardwareMappingP->Items[0];
    delete KeyMappingP;
    mHardwareMappingP->Delete(0);
  }

  // Show all Hardware Keys
  while (Key != NULL)
  {
    // Store key mapping
    tKeyboardMappingKey* KeyMappingP = new tKeyboardMappingKey;
    KeyMappingP->CPCKey = Key->CPCKey;
    KeyMappingP->WindowsKey = Key->WindowsKey;
    mHardwareMappingP->Add(KeyMappingP);

    // Next Key
    Key = mSettingsP->GetNextHardwareKey();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormKeyboardMapping::LoadJoystickMapping(void)
{
  const tKeyboardMappingKey* Key = mSettingsP->GetFirstJoystickKey();
  if (Key == NULL)
  {
    // Restore default mapping
    mSettingsP->RestoreDefaultJoystickKeys();
    Key = mSettingsP->GetFirstJoystickKey();
  }

  // Clear joystick mapping list
  while (mJoystickMappingP->Count > 0)
  {
    tKeyboardMappingKey* KeyMappingP = (tKeyboardMappingKey*)mJoystickMappingP->Items[0];
    delete KeyMappingP;
    mJoystickMappingP->Delete(0);
  }

  // Show all Joystick Keys
  while (Key != NULL)
  {
    // Store key mapping
    tKeyboardMappingKey* KeyMappingP = new tKeyboardMappingKey;
    KeyMappingP->CPCKey = Key->CPCKey;
    KeyMappingP->WindowsKey = Key->WindowsKey;
    mJoystickMappingP->Add(KeyMappingP);

    // Next Key
    Key = mSettingsP->GetNextJoystickKey();
  }
}
//---------------------------------------------------------------------------

