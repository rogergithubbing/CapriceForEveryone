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


#ifndef TDialogLabelH
#define TDialogLabelH

#include "TDialogComponent.h"


class TDialogLabel : public TDialogComponent
{
private:
  //
  // Members
	//
	String mText;
	SIZE mTextExtent;
  TColor mColor;
#ifdef _USE_OPENGL
  GLfloat mGLColorRed;
  GLfloat mGLColorGreen;
  GLfloat mGLColorBlue;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
	float mD3DColorRed;
	float mD3DColorGreen;
	float mD3DColorBlue;
#endif /* _USE_DIRECTX */
  TDialogDisplay mDialogDisplay;
  //
  // Methods
  //
	void __fastcall SetText(String Value);
  void __fastcall SetColor(TColor Value);
  void __fastcall OnDisplayD3D(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGL(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplayGDI(TDrawingContext* DrawingContextP);

public:
  //
  // Properties
  //
  __property String Text = {write=SetText};
  __property TColor Color = {write=SetColor};

  //
  // Public API
  //
  __fastcall TDialogLabel(TDrawingContext* DrawingContextP);
  virtual __fastcall ~TDialogLabel();

  void __fastcall OnResize(TDrawingContext* DrawingContextP);
  void __fastcall OnDisplay(TDrawingContext* DrawingContextP);
};

//---------------------------------------------------------------------------
#endif

