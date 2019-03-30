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


#ifndef TFormDebugFDCH
#define TFormDebugFDCH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
#include "..\Emulator\TEmulator.h"
//---------------------------------------------------------------------------
class TFormDebugFDC : public TForm
{
__published:	// Composants gérés par l'EDI
  TGroupBox *GroupBoxCurrentState;
  TGroupBox *GroupBoxHistory;
  TPanel *PanelHistoryControls;
  TButton *ButtonHistoryStartStop;
  TButton *ButtonHistoryClear;
  TListBox *ListBoxHistory;
  TCheckBox *CheckBoxShowReadStatus;
  TCheckBox *CheckBoxShowReadData;
  TCheckBox *CheckBoxShowWriteData;
  TMainMenu *MainMenu1;
  TMenuItem *HistoryMenu;
  TMenuItem *CopytoclipboardMenu;
  TPopupMenu *PopupMenuHistory;
  TMenuItem *Copytoclipboard1;
  TCheckBox *CheckBoxShowDiskIndex;
  TCheckBox *CheckBoxShowDiskSector;
  TCheckBox *CheckBoxShowDiskTrack;
  TCheckBox *CheckBoxAddTimestamp;
  TCheckBox *CheckBoxAddInterval;
  TLabel *LabelStateMotor;
  TLabel *LabelStateNbTracks;
  TLabel *LabelStateCurrentTrack;
  TLabel *LabelStateNbSectors;
  TPanel *PanelStateMotor;
  TPanel *PanelFormatStateNbTracks;
  TPanel *PanelStateNbTracks;
  TPanel *PanelFormatStateCurrentTrack;
  TPanel *PanelStateCurrentTrack;
  TPanel *PanelFormatStateNbSectors;
  TPanel *PanelStateNbSectors;
  TGroupBox *GroupBoxCurrentSector;
  TLabel *LabelStateCurrentSectorIndex;
  TPanel *PanelFormatStateCurrentSectorIndex;
  TPanel *PanelStateCurrentSectorIndex;
  TLabel *LabelStateCurrentSector;
  TPanel *PanelFormatStateCurrentSector;
  TPanel *PanelStateCurrentSector;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ButtonHistoryClearClick(TObject *Sender);
  void __fastcall ButtonHistoryStartStopClick(TObject *Sender);
  void __fastcall Copytoclipboard1Click(TObject *Sender);
  void __fastcall CopytoclipboardMenuClick(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall PanelFormatStateNbTracksClick(TObject *Sender);
  void __fastcall PanelFormatStateCurrentTrackClick(TObject *Sender);
  void __fastcall PanelFormatStateNbSectorsClick(TObject *Sender);
  void __fastcall PanelFormatStateCurrentSectorIndexClick(TObject *Sender);
  void __fastcall PanelFormatStateCurrentSectorClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  bool bHistoryEnabled;
  unsigned int iLastReadTimestamp;
  //
  // Methods
  //
  void __fastcall OnFDCEvent(tFDC* FDC);
  void __fastcall InitCurrentState(tNativeCPC* NativeCPC);
  void __fastcall WMMove(TMessage &Message);
  void __fastcall SetSettings(TSettings* Value);
  bool __fastcall OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  //
  // API
  //
  __fastcall TFormDebugFDC(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugFDC *FormDebugFDC;
//---------------------------------------------------------------------------
#endif
