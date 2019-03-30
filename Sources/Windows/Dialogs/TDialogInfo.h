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


#ifndef TDialogInfoH
#define TDialogInfoH

#include "TDialogForm.h"
#include "TDialogInfoLabel.h"
#include "..\Drawing\TDrawingContext.h"



class TDialogInfo : public TDialogForm
{
private:
  //
  // Members
  //
  float mFirstLabelTop;
  float mLabelStep;
  bool mResizeRequested;
  bool mShowInfo;
  TDialogInfoLabel* mLabelPauseMode;
  TDialogInfoLabel* mLabelMachineType;
  TDialogInfoLabel* mLabelDriveAFilename;
  TDialogInfoLabel* mLabelDriveBFilename;
  TDialogInfoLabel* mLabelTapeFilename;
  TDialogInfoLabel* mLabelSnapshotFilename;
  TDialogInfoLabel* mLabelControllerProfile;
  TDialogInfoLabel* mLabelDisplaySize;
  TDialogDisplay mDialogDisplay;
  //
  // Methods
  //
  void __fastcall SetMachineType(AnsiString Value);
  void __fastcall SetDriveAFilename(AnsiString Value);
  void __fastcall SetDriveBFilename(AnsiString Value);
  void __fastcall SetTapeFilename(AnsiString Value);
  void __fastcall SetSnapshot(AnsiString Value);
  void __fastcall SetControllerProfile(AnsiString Value);
  void __fastcall SetFullscreen(bool Value);
  void __fastcall UpdateLabelsTop(void);
  inline void __fastcall SetShowInfo(bool Value) { mShowInfo = Value; };
  void __fastcall OnDisplayD3D(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGL(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGDI(TDrawingContext* DrawingContextP);

public:
  //
  // Properties
  //
  __property AnsiString MachineType = { write=SetMachineType };
  __property AnsiString DriveAFilename = { write=SetDriveAFilename };
  __property AnsiString DriveBFilename = { write=SetDriveBFilename };
  __property AnsiString TapeFilename = { write=SetTapeFilename };
  __property AnsiString Snapshot = { write=SetSnapshot };
  __property AnsiString ControllerProfile = { write=SetControllerProfile };
  __property bool Fullscreen = { write=SetFullscreen };
  __property bool ShowInfo = { write=SetShowInfo };

  //
  // Public API
  //
  __fastcall TDialogInfo(TDrawingContext* DrawingContextP, bool ShowInfo);
  virtual __fastcall ~TDialogInfo();

  void __fastcall OnResize(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplay(TDrawingContext* DrawingContextP);
};

//---------------------------------------------------------------------------
#endif
