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


#ifndef TDialogOpenCDTH
#define TDialogOpenCDTH

#include "TDialogForm.h"
#include "TDialogLabel.h"
#include "TDialogButton.h"
#include "TDialogListBox.h"

class TDialogOpenCDT : public TDialogForm
{
private:
  //
  // Members
  //
  TDialogLabel* LabelTitle;
  TDialogLabel* LabelExtensions;
  TDialogButton* ButtonCDT;
  TDialogButton* ButtonWAV;
  TDialogButton* ButtonCSW;
  TDialogButton* ButtonZIP;
  TDialogLabel* LabelPathTitle;
  TDialogLabel* LabelPath;
  TDialogListBox* ListBoxFiles;
  TDialogButton* ButtonLoad;
  TDialogButton* ButtonCancel;
  AnsiString mPath;
  AnsiString mDisplayedPath;
  TDialogDisplay mDialogDisplay;
#ifdef _USE_DIRECTX
  LPDIRECT3DVERTEXBUFFER9 mD3DBackgroundVertexBuffer;
#endif /* _USE_DIRECTX */
  //
  // Methods
  //
  void __fastcall SetInitialPath(AnsiString Value);
  inline AnsiString __fastcall GetCurrentPath(void) { return mPath; };
  void __fastcall ClipPath(TDrawingContext* DrawingContextP);
  AnsiString __fastcall GetSelectedFile(void);
  void __fastcall SetSelectedFile(AnsiString File);
  void __fastcall OnExtensionsPressed(void);
  void __fastcall OnLoadPressed(void);
  void __fastcall OnNewPressed(void);
  void __fastcall OnCancelPressed(void);
  void __fastcall OnDisplayD3D(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGL(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGDI(TDrawingContext* DrawingContextP);

public:
  //
  // Properties
  //
  __property AnsiString InitialPath = {write=SetInitialPath};
  __property AnsiString CurrentPath = {read=GetCurrentPath};
  __property AnsiString SelectedFile = {read=GetSelectedFile, write=SetSelectedFile};
  //
  // Public API
  //
  __fastcall TDialogOpenCDT(TDrawingContext* DrawingContextP);
  virtual __fastcall ~TDialogOpenCDT();

  bool __fastcall OnKeyDown(WORD &Key);
  void __fastcall OnResize(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplay(TDrawingContext* DrawingContextP);
};

//---------------------------------------------------------------------------
#endif
