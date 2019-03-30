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


#ifndef TFormKeyboardMappingH
#define TFormKeyboardMappingH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormKeyboardMapping : public TForm
{
__published:	// Composants gérés par l'EDI
  TPanel *PanelButtons;
  TButton *ButtonCancel;
  TButton *ButtonApply;
  TCheckBox *CheckBoxUseHardwareKeyboardMapping;
  TButton *ButtonDefault;
  TTabControl *TabControlGraphicLayout;
  TPanel *PanelGraphicKey66;
  TPanel *PanelGraphicKey64;
  TPanel *PanelGraphicKey65;
  TPanel *PanelGraphicKey68;
  TPanel *PanelGraphicKey70;
  TPanel *PanelGraphicKey21L;
  TPanel *PanelGraphicKey23;
  TPanel *PanelGraphicKey57;
  TPanel *PanelGraphicKey56;
  TPanel *PanelGraphicKey49;
  TPanel *PanelGraphicKey48;
  TPanel *PanelGraphicKey41;
  TPanel *PanelGraphicKey40;
  TPanel *PanelGraphicKey33;
  TPanel *PanelGraphicKey32;
  TPanel *PanelGraphicKey25;
  TPanel *PanelGraphicKey24;
  TPanel *PanelGraphicKey16;
  TPanel *PanelGraphicKey79;
  TPanel *PanelGraphicKey10;
  TPanel *PanelGraphicKey11;
  TPanel *PanelGraphicKey3;
  TPanel *PanelGraphicKey67;
  TPanel *PanelGraphicKey59;
  TPanel *PanelGraphicKey58;
  TPanel *PanelGraphicKey50;
  TPanel *PanelGraphicKey51;
  TPanel *PanelGraphicKey43;
  TPanel *PanelGraphicKey42;
  TPanel *PanelGraphicKey35;
  TPanel *PanelGraphicKey34;
  TPanel *PanelGraphicKey27;
  TPanel *PanelGraphicKey26;
  TPanel *PanelGraphicKey17;
  TPanel *PanelGraphicKey20;
  TPanel *PanelGraphicKey12;
  TPanel *PanelGraphicKey4;
  TPanel *PanelGraphicKey13;
  TPanel *PanelGraphicKey14;
  TPanel *PanelGraphicKey5;
  TPanel *PanelGraphicKey15;
  TPanel *PanelGraphicKey0;
  TImage *ImageGraphicKey0;
  TPanel *PanelGraphicKey7;
  TPanel *PanelGraphicKey8;
  TImage *ImageGraphicKey8;
  TPanel *PanelGraphicKey2;
  TImage *ImageGraphicKey2;
  TPanel *PanelGraphicKey1;
  TImage *ImageGraphicKey1;
  TPanel *PanelGraphicKey18;
  TPanel *PanelGraphicKey9;
  TPanel *PanelGraphicKey6;
  TPanel *PanelGraphicKey47;
  TPanel *PanelGraphicKey69;
  TPanel *PanelGraphicKey60;
  TPanel *PanelGraphicKey61;
  TPanel *PanelGraphicKey53;
  TPanel *PanelGraphicKey52;
  TPanel *PanelGraphicKey44;
  TPanel *PanelGraphicKey45;
  TPanel *PanelGraphicKey37;
  TPanel *PanelGraphicKey36;
  TPanel *PanelGraphicKey29;
  TPanel *PanelGraphicKey28;
  TPanel *PanelGraphicKey19;
  TPanel *PanelGraphicKey71;
  TPanel *PanelGraphicKey63;
  TPanel *PanelGraphicKey62;
  TPanel *PanelGraphicKey55;
  TPanel *PanelGraphicKey54;
  TPanel *PanelGraphicKey46;
  TPanel *PanelGraphicKey38;
  TPanel *PanelGraphicKey39;
  TPanel *PanelGraphicKey31;
  TPanel *PanelGraphicKey30;
  TPanel *PanelGraphicKey22;
  TPanel *PanelGraphicKey21R;
  TGroupBox *GroupBoxJoystick0;
  TPanel *PanelGraphicKey72;
  TImage *ImageGraphicKey72;
  TPanel *PanelGraphicKey74;
  TImage *ImageGraphicKey74;
  TPanel *PanelGraphicKey76;
  TPanel *PanelGraphicKey77;
  TPanel *PanelGraphicKey75;
  TImage *ImageGraphicKey75;
  TPanel *PanelGraphicKey73;
  TImage *ImageGraphicKey73;
  TGroupBox *GroupBoxGraphicHardwareMapping;
  TButton *ButtonGraphicHardwareSet;
  TPanel *PanelGraphicHardwareKey;
  TGroupBox *GroupBoxGraphicASCIIMapping;
  TPanel *PanelGraphicWindowsVK;
  TPanel *PanelFormatKeycode;
  TPanel *PanelGraphicASCIIKey;
  TButton *ButtonGraphicASCIISet;
  TGroupBox *GroupBoxLastWindowsKey;
  TButton *ButtonGraphicWindowsVKFind;
  TGroupBox *GroupBoxJoystickMapping;
  TButton *ButtonJoystickSet;
  TPanel *PanelJoystickKey;
  TLabel *LabelTips1;
  TLabel *LabelTips2;
  TLabel *LabelTips3;
  TLabel *LabelHowToUse;
  TPanel *PanelGraphicKey18End;
  TLabel *LabelTips2bis;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ButtonApplyClick(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall ButtonDefaultClick(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall TabControlGraphicLayoutChange(TObject *Sender);
  void __fastcall PanelGraphicKeyClick(TObject *Sender);
  void __fastcall ButtonGraphicWindowsVKFindClick(TObject *Sender);
  void __fastcall PanelGraphicJoystick0KeyClick(TObject *Sender);
  void __fastcall ButtonGraphicHardwareSetClick(TObject *Sender);
  void __fastcall ImageGraphicKeyClick(TObject *Sender);
  void __fastcall ImageGraphicKeyJoystick0Click(TObject *Sender);
  void __fastcall PanelFormatKeycodeClick(TObject *Sender);
  void __fastcall ButtonGraphicASCIISetClick(TObject *Sender);
  void __fastcall ButtonJoystickSetClick(TObject *Sender);
  void __fastcall TabControlGraphicLayoutDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  int mGetASCIIKeyIndex;
  int mGetHardwareKeyIndex;
  int mGetJoystickKeyIndex;
  TMessageEvent OldAppMessage;
  TList* mASCIIMappingP;
  TList* mHardwareMappingP;
  TList* mJoystickMappingP;
  TPanel* mCurrentGraphicKeyP;
  tUShort const * mKeyboardCPCP;
  int mLastWindowsKeyPressed;
  int mJoystickKey;
  int mHardwareKey;
  int mASCIIKey;
  //
  // Methods
  //
  void __fastcall AppMessage(tagMSG &Msg, bool &Handled);
  void __fastcall WMKeyDown(TWMKey &Message);
  inline void SetSettings(TSettings* Value) { mSettingsP = Value; };
  void __fastcall ShowGraphicLayout(const char ** Layout);
  inline void __fastcall SetKeyboardCPC(tUShort const * Value) { mKeyboardCPCP = Value; };
  void __fastcall LoadASCIIMapping(void);
  void __fastcall LoadHardwareMapping(void);
  void __fastcall LoadJoystickMapping(void);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property tUShort const * KeyboardCPC = { write=SetKeyboardCPC };
  //
  // API
  //
  __fastcall TFormKeyboardMapping(TComponent* Owner);

  tKeyboardMappingKey* __fastcall GetFirstASCIIKey(void);
  tKeyboardMappingKey* __fastcall GetNextASCIIKey(void);

  tKeyboardMappingKey* __fastcall GetFirstHardwareKey(void);
  tKeyboardMappingKey* __fastcall GetNextHardwareKey(void);

  tKeyboardMappingKey* __fastcall GetFirstJoystickKey(void);
  tKeyboardMappingKey* __fastcall GetNextJoystickKey(void);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_KEYDOWN, TWMKey, WMKeyDown);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKeyboardMapping *FormKeyboardMapping;
//---------------------------------------------------------------------------
#endif
