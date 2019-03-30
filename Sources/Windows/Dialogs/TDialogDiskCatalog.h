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


#ifndef TDialogDiskCatalogH
#define TDialogDiskCatalogH

#include "TDialogForm.h"
#include "TDialogLabel.h"
#include "TDialogButton.h"
#include "TDialogListBox.h"
#include "..\Emulator\TEmulator.h"

class TDialogDiskCatalog : public TDialogForm
{
private:
  //
  // Members
  //
  TDialogLabel* LabelTitle;
  TDialogListBox* ListBoxFiles;
  TDialogButton* ButtonCancel;
  TDialogButton* ButtonRUN;
  TDialogButton* ButtonLOAD;
  TDialogButton* ButtonCPM;
  TEmulator* mEmulatorP;
  TDialogDisplay mDialogDisplay;
#ifdef _USE_DIRECTX
  LPDIRECT3DVERTEXBUFFER9 mD3DBackgroundVertexBuffer;
#endif /* _USE_DIRECTX */

  //
  // Methods
  //
  void __fastcall OnCancelPressed(void);
  inline void __fastcall SetEmulator(TEmulator* value) { mEmulatorP = value; };
  AnsiString __fastcall GetSelectedFile(void);
  void __fastcall OnRUNPressed(void);
  void __fastcall OnLOADPressed(void);
  void __fastcall OnCPMPressed(void);
  void __fastcall OnDisplayD3D(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGL(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGDI(TDrawingContext* DrawingContextP);

public:
  //
  // Properties
  //
  __property TEmulator* EmulatorP = {write=SetEmulator};
  __property AnsiString SelectedFile = {read=GetSelectedFile};

  //
  // Public API
  //
  __fastcall TDialogDiskCatalog(TDrawingContext* DrawingContextP);
  virtual __fastcall ~TDialogDiskCatalog();

  bool __fastcall OnKeyDown(WORD &Key);
  void __fastcall OnResize(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplay(TDrawingContext* DrawingContextP);
  void __fastcall OnMouseDblClick(int X, int Y);

  void __fastcall ShowCatalog(TEmulator* EmulatorP);
};

//---------------------------------------------------------------------------
#endif
