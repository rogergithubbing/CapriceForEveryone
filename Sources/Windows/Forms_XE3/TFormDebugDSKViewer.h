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


#ifndef TFormDebugDSKViewerH
#define TFormDebugDSKViewerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Emulator\TEmulator.h"
#include "..\Settings\TSettings.h"


//---------------------------------------------------------------------------
class TFormDebugDSKViewer : public TForm
{
__published:	// Composants gérés par l'EDI
  TPanel *PanelSectorData;
  TPanel *PanelDrive;
  TComboBox *ComboBoxDrive;
  TButton *ButtonMonitoring;
  TStringGrid *StringGridSectors;
  TPanel *PanelInfos;
  TPanel *PanelSectorState;
  TComboBox *ComboBoxSides;
  TPageControl *PageControlData;
  TTabSheet *TabSheetDataSector;
  TMemo *MemoDataSector;
  TTabSheet *TabSheetDataGAP3;
  TTabSheet *TabSheetDataDeclared;
  TMemo *MemoDataGAP3;
  TMemo *MemoDataDeclared;
  TPopupMenu *PopupMenuClipboard;
  TMenuItem *CopyToClipboardMenu;
  TTabSheet *TabSheetSector;
  TMemo *MemoSectorInfo;
  TGroupBox *GroupBoxTrack;
  TMemo *MemoTrackInfo;
  TGroupBox *GroupBoxDisk;
  TMemo *MemoDiskInfo;
  TPopupMenu *PopupMenuData;
  TMenuItem *MenuItem1;
  TMenuItem *N1;
  TMenuItem *ASCII7bitsMenu;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormHide(TObject *Sender);
  void __fastcall ComboBoxDriveChange(TObject *Sender);
  void __fastcall StringGridSectorsDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
  void __fastcall StringGridSectorsSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
  void __fastcall ButtonMonitoringClick(TObject *Sender);
  void __fastcall ComboBoxSidesChange(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall CopyToClipboardMenuClick(TObject *Sender);
  void __fastcall PageControlDataChange(TObject *Sender);
  void __fastcall ASCII7bitsMenuClick(TObject *Sender);
  void __fastcall ComboBoxDriveDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall ComboBoxSidesDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
  void __fastcall PageControlDataDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TEmulator* mEmulatorP;
  TSettings* mSettingsP;
  bool mMonitoringInProgress;
  int mCurrentSide;
  bool mDeclaredDataPrepared;
  bool mDataASCII7Bits;
  tDrive* mStartDriveP;
  TColor mCellFrameColor;
  //
  // Methods
  //
  AnsiString __fastcall GetTitle(void);
  void __fastcall WMMove(TMessage &Message);
  inline void __fastcall SetEmulator(TEmulator* Value) { mEmulatorP=Value; };
  void __fastcall SetSettings(TSettings* Value);
  inline void __fastcall SetStartDrive(tDrive* Value) { mStartDriveP=Value; };
  void __fastcall OnDiskEvent(tDrive* DriveP);
  void __fastcall ShowDriveContent(int TrackSelected);
  void __fastcall ShowSideContent(int TrackSelected);
  void __fastcall ShowTrackInfo(int TrackNumber);
  void __fastcall ShowSectorInfo(int TrackNumber, int SectorNumber);
  void __fastcall ShowData(int TrackNumber, int SectorNumber);
  void __fastcall ShowDeclaredData(int TrackNumber, int SectorNumber);

  //
  // Private Properties
  //
  __property AnsiString Title = { read=GetTitle };

public:		// Déclarations utilisateur
  //
  // Public Properties
  //
  __property TEmulator* Emulator = { write=SetEmulator };
  __property TSettings* Settings = { write=SetSettings };
  __property tDrive* StartDrive = { write=SetStartDrive };

  //
  // API
  //
  __fastcall TFormDebugDSKViewer(TComponent* Owner);

#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
END_MESSAGE_MAP(TForm)
#pragma option pop
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugDSKViewer *FormDebugDSKViewer;
//---------------------------------------------------------------------------
#endif
