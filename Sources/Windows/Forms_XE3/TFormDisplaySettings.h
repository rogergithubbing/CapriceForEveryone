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


#ifndef TFormDisplaySettingsH
#define TFormDisplaySettingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
#include "..\Drawing\TDrawingContext.h"
#include "..\..\..\Sources\Engine\Native_CPC.h"
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDisplaySettings : public TForm
{
__published:	// Composants gérés par l'EDI
  TButton *ButtonCancel;
  TButton *ButtonOK;
  TCheckBox *CheckBoxFullscreenWithMenu;
  TCheckBox *CheckBoxDrawScanlines;
  TCheckBox *CheckBoxKeepAspectRatio;
  TGroupBox *GroupBoxOSD;
  TCheckBox *CheckBoxOSDIcons;
  TCheckBox *CheckBoxOSDDisk;
  TCheckBox *CheckBoxOSDTape;
  TCheckBox *CheckBoxOSDMessages;
  TCheckBox *CheckBoxKeepFullscreen;
  TCheckBox *CheckBoxShowInfoDuringPause;
  TGroupBox *GroupBoxTheme;
  TComboBox *ComboBoxTheme;
  TPanel *PanelThemeBackground;
  TLabel *LabelThemeBackground;
  TLabel *LabelThemeText1;
  TPanel *PanelThemeText1;
  TLabel *LabelThemeText2;
  TPanel *PanelThemeText2;
  TGroupBox *GroupBoxVHoldAdjust;
  TTrackBar *TrackBarVHOLDAdjust;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ButtonOKClick(TObject *Sender);
  void __fastcall ButtonCancelClick(TObject *Sender);
  void __fastcall CheckBoxDrawScanlinesClick(TObject *Sender);
  void __fastcall ComboBoxThemeDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxThemeChange(TObject *Sender);
  void __fastcall PanelThemeBackgroundClick(TObject *Sender);
  void __fastcall PanelThemeText1Click(TObject *Sender);
  void __fastcall PanelThemeText2Click(TObject *Sender);
  void __fastcall TrackBarVHOLDAdjustChange(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  TDrawingContext* mDrawingContextP;
  tNativeCPC* mNativeCPCP;

  //
  // Methods
  //
  inline void SetSettings(TSettings* Value) { mSettingsP = Value; };
  inline void SetDrawingContext(TDrawingContext* Value) { mDrawingContextP = Value; };
  inline void SetNativeCPC(tNativeCPC* Value) { mNativeCPCP = Value; };
  void __fastcall ApplyTheme(void);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property TDrawingContext* DrawingContext = { write=SetDrawingContext };
  __property tNativeCPC* NativeCPC = { write=SetNativeCPC };

  //
  // API
  //
  __fastcall TFormDisplaySettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDisplaySettings *FormDisplaySettings;
//---------------------------------------------------------------------------
#endif
