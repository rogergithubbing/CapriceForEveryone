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


#ifndef TFormGameControllerSettingsH
#define TFormGameControllerSettingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Inputs\TInputContext.h"
#include "..\Settings\TSettings.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormGameControllerSettings : public TForm
{
__published:	// Composants gérés par l'EDI
  TComboBox *ComboBoxControllersList;
  TButton *ButtonProperties;
  TPageControl *PageControlActions;
  TTabSheet *TabSheetButtons;
  TStringGrid *StringGridButtons;
  TButton *ButtonApply;
  TTabSheet *TabSheetPOV;
  TComboBox *ComboBoxPOVId;
  TLabel *POVId;
  TStringGrid *StringGridPOV;
  TTabSheet *TabSheetAnalog;
  TStringGrid *StringGridAnalog;
  TComboBox *ComboBoxAnalogAxis;
  TLabel *LabelAxis;
  TTrackBar *TrackBarAnalogPressThreshold;
  TLabel *LabelAnalogPressThreshold;
  TButton *ButtonCancel;
  TGroupBox *GroupBoxProfile;
  TLabel *LabelFromProfile;
  TLabel *LabelSaveProfile;
  TComboBox *ComboBoxProfiles;
  TCheckBox *CheckBoxAnalogMouseX;
  TCheckBox *CheckBoxAnalogMouseY;
  TLabel *LabelAnalogDeadZone;
  TTrackBar *TrackBarAnalogDeadZone;
  TButton *ButtonReset;
  TPanel *PanelFromProfile;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ButtonPropertiesClick(TObject *Sender);
  void __fastcall ComboBoxControllersListChange(TObject *Sender);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall StringGridButtonsDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
  void __fastcall StringGridButtonsMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall ComboBoxProfilesChange(TObject *Sender);
  void __fastcall ComboBoxPOVIdChange(TObject *Sender);
  void __fastcall StringGridPOVDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
  void __fastcall StringGridPOVMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall ComboBoxAnalogAxisChange(TObject *Sender);
  void __fastcall StringGridAnalogDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
  void __fastcall StringGridAnalogMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall TrackBarAnalogPressThresholdChange(TObject *Sender);
  void __fastcall TabSheetButtonsShow(TObject *Sender);
  void __fastcall TabSheetPOVShow(TObject *Sender);
  void __fastcall TabSheetAnalogShow(TObject *Sender);
  void __fastcall CheckBoxAnalogMouseXClick(TObject *Sender);
  void __fastcall CheckBoxAnalogMouseYClick(TObject *Sender);
  void __fastcall TrackBarAnalogDeadZoneChange(TObject *Sender);
  void __fastcall ButtonResetClick(TObject *Sender);
  void __fastcall ComboBoxControllersListDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
  void __fastcall ComboBoxProfilesDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxPOVIdDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxAnalogAxisDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall PageControlActionsDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);
private:	// Déclarations utilisateur
  // Members
  TInputContext* mInputContextP;
  TInputDevice* mSelectedDeviceP;
  TSettings* mSettingsP;
  bool mModified;
  AnsiString mDefaultProfile;
  AnsiString mDriveAProfile;
  AnsiString mDriveBProfile;
  AnsiString mCurrentProfile;
  TInputActionAnalog* mSelectedAnalogP;
  int mSelectedAnalogIndex;
  // Methods
  inline void __fastcall SetInputContext(TInputContext* Value) { mInputContextP = Value; };
  inline void __fastcall SetSettings(TSettings* Value) { mSettingsP = Value; };
  void __fastcall OnIdle(TObject *Sender, bool &Done);
  inline bool __fastcall GetModified(void) { return mModified; };
  void __fastcall SetCurrentProfile(AnsiString Value);
  void __fastcall SetDefaultProfile(AnsiString Value);
  void __fastcall SetDriveAProfile(AnsiString Value);
  void __fastcall SetDriveBProfile(AnsiString Value);
  AnsiString __fastcall GetSelectedProfile(void);

public:		// Déclarations utilisateur
  // Properties
  __property TInputContext* InputContextP = { write=SetInputContext };
  __property TSettings* Settings = { write=SetSettings };
  __property bool Modified = { read=GetModified };
  __property AnsiString CurrentProfile = { write=SetCurrentProfile };
  __property AnsiString DefaultProfile = { write=SetDefaultProfile };
  __property AnsiString DriveAProfile = { write=SetDriveAProfile };
  __property AnsiString DriveBProfile = { write=SetDriveBProfile };
  __property AnsiString SelectedProfile = { read=GetSelectedProfile };
  // Methods
  __fastcall TFormGameControllerSettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGameControllerSettings *FormGameControllerSettings;
//---------------------------------------------------------------------------
#endif
