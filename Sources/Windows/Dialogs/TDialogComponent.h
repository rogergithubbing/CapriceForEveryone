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


#ifndef TDialogComponentH
#define TDialogComponentH


#ifdef _USE_OPENGL
#include "..\OpenGL\glew.h"
#include "..\OpenGL\wglew.h"
#include <gl\glu.h>
#endif /* _USE_OPENGL */

#include "..\Drawing\TDrawingContext.h"


typedef void __fastcall (__closure *TDialogDisplay)(TDrawingContext* DrawingContextP);


class TDialogComponent
{
private:
  //
  // Members
  //
  // Pourcent size 0.0 to 1.0 of screen width and height
  float mLeft;
  float mRight;
  float mTop;
  float mBottom;

protected:
  //
  // Members
  //
  int mScreenLeft;
  int mScreenRight;
  int mScreenTop;
  int mScreenBottom;
  bool mVisible;
  //
  // Methods
  //
  inline int __fastcall GetScreenLeft() { return mScreenLeft; };
  inline void __fastcall SetLeft(float Value) { mLeft = Value; };
  inline int __fastcall GetScreenRight() { return mScreenRight; };
  inline void __fastcall SetRight(float Value) { mRight = Value; };
  inline int __fastcall GetScreenTop() { return mScreenTop; };
  inline void __fastcall SetTop(float Value) { mTop = Value; };
  inline int __fastcall GetScreenBottom() { return mScreenBottom; };
  inline void __fastcall SetBottom(float Value) { mBottom = Value; };
  inline bool __fastcall GetVisible(void) { return mVisible; };
  inline void __fastcall SetVisible(bool Value) { mVisible = Value; };

public:
  //
  // Properties
  //
  __property float Left = { write=SetLeft };
  __property float Right = { write=SetRight };
  __property float Top = { write=SetTop };
  __property float Bottom = { write=SetBottom };
  __property int ScreenLeft = { read=GetScreenLeft };
  __property int ScreenRight = { read=GetScreenRight };
  __property int ScreenTop = { read=GetScreenTop };
  __property int ScreenBottom = { read=GetScreenBottom };
  __property bool Visible = { read=GetVisible, write=SetVisible };
  //
  // Public API
  //
  __fastcall TDialogComponent();
  __fastcall ~TDialogComponent();

  virtual void __fastcall OnMouseMove(int X, int Y) { };
  virtual bool __fastcall OnMouseDown(TMouseButton Button, int X, int Y);
  virtual bool __fastcall OnMouseClick(int X, int Y);
  virtual bool __fastcall OnMouseDblClick(int X, int Y);
  virtual void __fastcall OnResize(TDrawingContext* DrawingContextP);
  virtual void __fastcall OnDisplay(TDrawingContext* DrawingContextP) = 0;
};

//---------------------------------------------------------------------------
#endif
