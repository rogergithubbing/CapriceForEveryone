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


#ifndef TFormEmulatorSettingsH
#define TFormEmulatorSettingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"

//---------------------------------------------------------------------------
class TFormEmulatorSettings : public TForm
{
__published:	// Composants gérés par l'EDI
  TGroupBox *GroupBoxMachine;
  TLabel *LabelDevice;
  TComboBox *ComboBoxDevices;
  TLabel *LabelBrand;
  TComboBox *ComboBoxBrands;
  TCheckBox *CheckBoxUse64kMemoryExtension;
  TCheckBox *CheckBoxUse256kMemoryExtension;
  TCheckBox *CheckBoxUse256kSiliconDisc;
  TPanel *PanelButtons;
  TButton *ButtonOK;
  TButton *ButtonCancel;
  TGroupBox *GroupBoxROMMapping;
  TStringGrid *StringGridROMMapping;
  TPanel *PanelROMSettings;
  TComboBox *ComboBoxAvailableROM;
  TLabel *LabelSelectROM;
  TButton *ButtonDefaultROMMapping;
  TCheckBox *CheckBoxUseTechniMusique;
  TLabel *LabelCRTCType;
  TComboBox *ComboBoxCRTCType;
  TCheckBox *CheckBoxUseFloppyDrive;
  TCheckBox *CheckBoxUseFastFDC;
  TCheckBox *CheckBoxUsePlayCity;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ButtonOKClick(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall StringGridROMMappingMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ComboBoxAvailableROMChange(TObject *Sender);
  void __fastcall StringGridROMMappingSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
  void __fastcall ButtonDefaultROMMappingClick(TObject *Sender);
  void __fastcall StringGridROMMappingKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall CheckBoxUseFloppyDriveClick(TObject *Sender);
  void __fastcall ComboBoxDevicesChange(TObject *Sender);
  void __fastcall ComboBoxCRTCTypeChange(TObject *Sender);
  void __fastcall CheckBoxUse256kSiliconDiscClick(TObject *Sender);
  void __fastcall StringGridROMMappingDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall ComboBoxCRTCTypeDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxDevicesDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxBrandsDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxAvailableROMDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
private:	// Déclarations utilisateur
  //
  // Members
  //
  TStringList* mAvailableROMList;
  TSettings* mSettingsP;

  //
  // Methods
  //
  void __fastcall FillROMMapping(void);
  void __fastcall UpdateAvailableROMList(int Index);
  inline void __fastcall SetSettings(TSettings* Value) { mSettingsP = Value; };

public:		// Déclarations utilisateur
  __property TSettings* Settings = { write=SetSettings };

  __fastcall TFormEmulatorSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmulatorSettings *FormEmulatorSettings;
//---------------------------------------------------------------------------
#endif
