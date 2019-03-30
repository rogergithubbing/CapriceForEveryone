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


#ifndef TFormEditKeycodeH
#define TFormEditKeycodeH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
#include "..\Inputs\TInputContext.h"
//---------------------------------------------------------------------------
class TFormEditKeycode : public TForm
{
__published:	// Composants gérés par l'EDI
  TPageControl *PageControlKeycode;
  TTabSheet *TabSheetKeyboard;
  TTabSheet *TabSheetMouse;
  TCheckBox *CheckBoxMouseButton;
  TGroupBox *GroupBoxModifiers;
  TCheckBox *CheckBoxModifierShift;
  TCheckBox *CheckBoxModifierCtrl;
  TComboBox *ComboBoxNewKeycode;
  TLabel *LabelCurrentKeycode;
  TPanel *PanelButtons;
  TButton *ButtonCancel;
  TButton *ButtonOK;
  TButton *ButtonNewKeycode;
  TLabel *LabelNewKeycode;
  TPanel *PanelCurrentKeycode;
  TPanel *PanelNewKeycode;
  TLabel *LabelFromList;
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ComboBoxNewKeycodeChange(TObject *Sender);
  void __fastcall ComboBoxNewKeycodeClick(TObject *Sender);
  void __fastcall CheckBoxMouseButtonClick(TObject *Sender);
  void __fastcall ButtonNewKeycodeClick(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ComboBoxNewKeycodeDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall CheckBoxModifierShiftClick(TObject *Sender);
  void __fastcall CheckBoxModifierCtrlClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  TInputContext* mInputContextP;
  WORD mKey;
  TShiftState mShiftState;
  bool mMouseButton;
  TLabel *LabelHitKey;
  TMessageEvent mLastAppMessage;
  //
  // Methods
  //
  inline void __fastcall SetSettings(TSettings* Value) { mSettingsP = Value; };
  inline void __fastcall SetInputContext(TInputContext* Value) { mInputContextP = Value; };
  inline WORD __fastcall GetKey(void) { return mKey; };
  inline void __fastcall SetKey(WORD Value) { mKey = Value; };
  inline TShiftState __fastcall GetShift(void) { return mShiftState; };
  inline void __fastcall SetShift(TShiftState Value) { mShiftState = Value; };
  inline bool __fastcall GetMouseButton(void) { return mMouseButton; };
  inline void __fastcall SetMouseButton(bool Value) { mMouseButton = Value; };
  void __fastcall SetMouseEnable(bool Value);
  void __fastcall AppMessage(tagMSG &Msg, bool &Handled);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property WORD Key = { read=GetKey, write=SetKey };
  __property TShiftState ShiftState = { read=GetShift, write=SetShift };
  __property TInputContext* InputContextP = { write=SetInputContext };
  __property bool MouseButton = { read=GetMouseButton, write=SetMouseButton };
  __property bool MouseEnable = { write=SetMouseEnable };
  //
  // API
  //
  __fastcall TFormEditKeycode(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEditKeycode *FormEditKeycode;
//---------------------------------------------------------------------------
#endif
