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


#include <vcl.h>
#pragma hdrstop
#include "TDialogLabel.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TDialogLabel::TDialogLabel(TDrawingContext* DrawingContextP) : TDialogComponent()
//
// Constructor
//
{
#ifdef _USE_DIRECTX
  if (DrawingContextP->UseDirectX == true)
  {
    mDialogDisplay = OnDisplayD3D;
  }
  else
#endif /* _USE_DIRECTX */
#ifdef _USE_OPENGL
  if (DrawingContextP->UseOpenGL == true)
  {
    mDialogDisplay = OnDisplayGL;
  }
  else
#endif /* _USE_OPENGL */
  {
    mDialogDisplay = OnDisplayGDI;
  }
}
//---------------------------------------------------------------------------


__fastcall TDialogLabel::~TDialogLabel(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TDialogLabel::SetText(String Value)
{
  mText = Value;
  mTextExtent.cx = 0;
}
//---------------------------------------------------------------------------


void __fastcall TDialogLabel::SetColor(TColor Value)
{
  mColor = Value;
#ifdef _USE_OPENGL
  mGLColorRed = (GLfloat)GetRValue(Value) / 255.0f;
  mGLColorGreen = (GLfloat)GetGValue(Value) / 255.0f;
  mGLColorBlue = (GLfloat)GetBValue(Value) / 255.0f;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  mD3DColorRed = (float)GetRValue(Value) / 255.0f;
  mD3DColorGreen = (float)GetGValue(Value) / 255.0f;
  mD3DColorBlue = (float)GetBValue(Value) / 255.0f;
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogLabel::OnResize(TDrawingContext* DrawingContextP)
{
  TDialogComponent::OnResize(DrawingContextP);
  mTextExtent = DrawingContextP->GetDialogExtent(mText);
}
//---------------------------------------------------------------------------


void __fastcall TDialogLabel::OnDisplay(TDrawingContext* DrawingContextP)
{
  if (mTextExtent.cx == 0)
  {
    mTextExtent = DrawingContextP->GetDialogExtent(mText);
  }

  if (mVisible == true)
  {
    // Component display
    mDialogDisplay(DrawingContextP);
  }
}
//---------------------------------------------------------------------------

void __fastcall TDialogLabel::OnDisplayD3D(TDrawingContext* DrawingContextP)
{
#ifdef _USE_DIRECTX

  RECT textRect;
  SetRect(&textRect,
          mScreenLeft,
          mScreenTop,
          DrawingContextP->EmulatorScreenRight,
          DrawingContextP->EmulatorScreenBottom);

  // Text in plain color
  DrawingContextP->D3DInfoFont->DrawText(NULL,
                                         String(mText).c_str(),
                                         -1,
                                         &textRect,
                                         DT_SINGLELINE,
                                         D3DXCOLOR(mD3DColorRed, mD3DColorGreen, mD3DColorBlue, 1.0f));

#else /* _USE_DIRECTX */

  NOT_USED(DrawingContextP);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDialogLabel::OnDisplayGL(TDrawingContext* DrawingContextP)
{
#ifdef _USE_OPENGL

  // Text
  glColor4f(mGLColorRed, mGLColorGreen, mGLColorBlue, 1.0f);
  glRasterPos2i(mScreenLeft, mScreenTop+mTextExtent.cy);

  // Push the list base value
  glPushAttrib(GL_LIST_BIT);
  // Set a new list base value.
  glListBase(DrawingContextP->GLDialogFontBase - 32);
  // Call the lists to draw the text.
  glCallLists((GLsizei)mText.Length(),
              GL_UNSIGNED_BYTE,
              (GLvoid *)AnsiString(mText).c_str());

  // Restore list
  glPopAttrib();

#else /* _USE_OPENGL */

  NOT_USED(DrawingContextP);

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDialogLabel::OnDisplayGDI(TDrawingContext* DrawingContextP)
{
  // Draw Text
  int OldBkMode = SetBkMode(DrawingContextP->OnscreenHDC, TRANSPARENT);
  SetTextColor(DrawingContextP->OnscreenHDC, mColor);
  TextOut(DrawingContextP->OnscreenHDC,
          mScreenLeft,
          mScreenTop,
          mText.c_str(),
          mText.Length());
  SetBkMode(DrawingContextP->OnscreenHDC, OldBkMode);
}
//---------------------------------------------------------------------------

