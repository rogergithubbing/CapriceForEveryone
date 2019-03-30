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


#ifndef TDialogListBoxH
#define TDialogListBoxH

#include "TDialogComponent.h"


typedef void __fastcall (__closure *TDialogListBoxSelectionEvent)(void);


class TDialogListBox : public TDialogComponent
{
private:
  //
  // Members
	//
	TStringList* mStringList;
	bool mAlphaSorted;
  int mTopIndex;
  int mDisplayedItemCount;
  bool mResizeRequested;
  float mItemHeight;
  int mScreenItemHeight;
  float mScrollbarWidth;
  int mScreenScrollbarLeft;
  int mSelectedIndex;
  int mItemTextOffset;
  TDialogListBoxSelectionEvent mOnSelectionChange;
  TColor mActiveColor;
  TColor mInactiveColor;
#ifdef _USE_OPENGL
  GLfloat mActiveColorRed;
  GLfloat mActiveColorGreen;
  GLfloat mActiveColorBlue;
  GLfloat mInactiveColorRed;
  GLfloat mInactiveColorGreen;
  GLfloat mInactiveColorBlue;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  D3DCOLOR mD3DActiveColor;
  D3DCOLOR mD3DInactiveColor;
  LPDIRECT3DVERTEXBUFFER9 mD3DSelectedItemVertexBuffer;
#endif /* _USE_DIRECTX */
  TDialogDisplay mDialogDisplay;

  //
  // Methods
  //
  void __fastcall SetActiveColor(TColor Value);
  void __fastcall SetInactiveColor(TColor Value);
  void __fastcall SetItemHeight(float Value);
  void __fastcall SetScrollbarWidth(float Value);
  inline int __fastcall GetSelectedIndex(void) { return mSelectedIndex; };
  void __fastcall SetSelectedIndex(int Value);
  AnsiString __fastcall GetSelectedString(void);
  int __fastcall GetItemCount(void);
  inline int __fastcall GetDisplayedItemCount(void) { return mDisplayedItemCount; };
  inline void __fastcall SetOnSelectionChange(TDialogListBoxSelectionEvent Value) { mOnSelectionChange = Value; };
  void __fastcall UpdateTopIndex(void);
  inline bool __fastcall GetAlphaSorted(void) { return mAlphaSorted; };
  inline void __fastcall SetAlphaSorted(bool Value) { mAlphaSorted = Value; };
  void __fastcall OnDisplayD3D(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGL(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGDI(TDrawingContext* DrawingContextP);

public:
  //
  // Properties
  //
  __property TColor ActiveColor = {write=SetActiveColor};
  __property
   TColor InactiveColor = {write=SetInactiveColor};
  __property float ItemHeight = {write=SetItemHeight};
  __property float ScrollbarWidth = {write=SetScrollbarWidth};
  __property int SelectedIndex = {read=GetSelectedIndex, write=SetSelectedIndex};
  __property AnsiString SelectedString = {read=GetSelectedString};
  __property int ItemCount = {read=GetItemCount};
  __property int DisplayedItemCount = {read=GetDisplayedItemCount};
  __property TDialogListBoxSelectionEvent OnSelectionChange = {write=SetOnSelectionChange};
  __property bool AlphaSorted = {read=GetAlphaSorted, write= SetAlphaSorted};

  //
  // Public API
  //
  __fastcall TDialogListBox(TDrawingContext* DrawingContextP);
  virtual __fastcall ~TDialogListBox();

  void __fastcall Clear(void);
  void __fastcall Add(AnsiString NewItem);
  AnsiString __fastcall GetString(int Index);
  int __fastcall Select(AnsiString Item);

  void __fastcall OnResize(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplay(TDrawingContext* DrawingContextP);
  bool __fastcall OnMouseClick(int X, int Y);

  void __fastcall FindNext(AnsiString Key);
};

//---------------------------------------------------------------------------
#endif

