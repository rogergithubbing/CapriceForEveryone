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


#ifndef TDialogButtonH
#define TDialogButtonH

#include "TDialogComponent.h"

typedef enum
{
  PushButton,
  PressOnlyButton,
  ToggleButton
} TDialogButtonType;


typedef void __fastcall (__closure *TDialogButtonEvent)(void);


class TDialogButton : public TDialogComponent
{
private:
  //
  // Members
  //
  TDialogButtonType mType;
	String mText;
	bool mActivated;
  TDialogButtonEvent mOnPressed;
  int mScreenTextLeft;
  int mScreenTextTop;
  SIZE mTextExtent;
  TColor mActiveColor;
  TColor mInactiveColor;
#ifdef _USE_OPENGL
  GLfloat mGLActiveColorRed;
  GLfloat mGLActiveColorGreen;
  GLfloat mGLActiveColorBlue;
  GLfloat mGLInactiveColorRed;
  GLfloat mGLInactiveColorGreen;
  GLfloat mGLInactiveColorBlue;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  D3DCOLOR mD3DActiveColor;
  D3DCOLOR mD3DInactiveColor;
  LPDIRECT3DVERTEXBUFFER9 mD3DBackgroundVertexBuffer;
#endif /* _USE_DIRECTX */
  TDialogDisplay mDialogDisplay;
  //
  // Methods
  //
  inline void __fastcall SetType(TDialogButtonType Value) { mType = Value; };
  void __fastcall SetText(String Value);
  inline bool __fastcall GetActivated(void) { return mActivated; };
  inline void __fastcall SetActivated(bool Value) { mActivated = Value; };
  inline void __fastcall SetOnPressed(TDialogButtonEvent Value) { mOnPressed = Value; };
  void __fastcall SetActiveColor(TColor Value);
  void __fastcall SetInactiveColor(TColor Value);
  void __fastcall UpdateTextOrigin(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayD3D(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGL(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGDI(TDrawingContext* DrawingContextP);

public:
  //
  // Properties
  //
  __property TDialogButtonType Type = {write=SetType};
  __property bool Activated = {read=GetActivated, write=SetActivated};
  __property String Text = {write=SetText};
  __property TDialogButtonEvent OnPressed = {write=SetOnPressed};
  __property TColor ActiveColor = {write=SetActiveColor};
  __property TColor InactiveColor = {write=SetInactiveColor};

  //
  // Public API
  //
  __fastcall TDialogButton(TDrawingContext* DrawingContextP);
  virtual __fastcall ~TDialogButton();

  bool __fastcall OnMouseClick(int X, int Y);
  void __fastcall OnResize(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplay(TDrawingContext* DrawingContextP);
};

//---------------------------------------------------------------------------
#endif

