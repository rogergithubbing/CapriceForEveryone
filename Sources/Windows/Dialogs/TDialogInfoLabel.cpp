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
#include "TDialogInfoLabel.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TDialogInfoLabel::TDialogInfoLabel(TDrawingContext* DrawingContextP) : TDialogComponent()
//
// Constructor
//
{
  mText = "";

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


__fastcall TDialogInfoLabel::~TDialogInfoLabel(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TDialogInfoLabel::SetText(String Value)
{
  mText = Value;
  mTextExtent.cx = 0;
}
//---------------------------------------------------------------------------


void __fastcall TDialogInfoLabel::SetColor(TColor Value)
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


void __fastcall TDialogInfoLabel::OnResize(TDrawingContext* DrawingContextP)
{
  TDialogComponent::OnResize(DrawingContextP);

  mTextExtent = DrawingContextP->GetInfoExtent(mText);
}
//---------------------------------------------------------------------------


void __fastcall TDialogInfoLabel::OnDisplay(TDrawingContext* DrawingContextP)
{
  if (mTextExtent.cx == 0)
  {
    mTextExtent = DrawingContextP->GetInfoExtent(mText);
  }

  if (mVisible == false) return;

  mDialogDisplay(DrawingContextP);
}
//---------------------------------------------------------------------------

void __fastcall TDialogInfoLabel::OnDisplayD3D(TDrawingContext* DrawingContextP)
{
#ifdef _USE_DIRECTX

  RECT textRect;
  SetRect(&textRect,
          mScreenLeft+1,
          mScreenTop+1,
          DrawingContextP->EmulatorScreenRight,
          DrawingContextP->EmulatorScreenBottom);

  // Text outline in plain white
  DrawingContextP->D3DInfoFont->DrawText(NULL,
                                         String(mText).c_str(),
                                         -1,
                                         &textRect,
                                         DT_SINGLELINE,
                                         D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

  // Text in plain color
  textRect.left--;
  textRect.top--;
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

void __fastcall TDialogInfoLabel::OnDisplayGL(TDrawingContext* DrawingContextP)
{
#ifdef _USE_OPENGL

  char* TextChar = AnsiString(mText).c_str();
  int TextLength = mText.Length();

  // Push the list base value
  glPushAttrib(GL_LIST_BIT);
  // Set a new list base value.
  glListBase(DrawingContextP->GLInfoFontBase - 32);

  // Text outline
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glRasterPos2i(mScreenLeft + 1,
                mScreenTop + mTextExtent.cy + 1);
  // Call the lists to draw the text.
  glCallLists((GLsizei)TextLength,
              GL_UNSIGNED_BYTE,
              (GLvoid *)TextChar);

  // Text in defined color
  glColor4f(mGLColorRed, mGLColorGreen, mGLColorBlue, 1.0f);
  glRasterPos2i(mScreenLeft,
                mScreenTop + mTextExtent.cy);
  // Call the lists to draw the text.
  glCallLists((GLsizei)TextLength,
              GL_UNSIGNED_BYTE,
              (GLvoid *)TextChar);

  // Restore list
  glPopAttrib();

#else /* _USE_OPENGL */

  NOT_USED(DrawingContextP);

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDialogInfoLabel::OnDisplayGDI(TDrawingContext* DrawingContextP)
{
  int OldBkMode = SetBkMode(DrawingContextP->OnscreenHDC, TRANSPARENT);

  // Draw text shadow
  SetTextColor(DrawingContextP->OnscreenHDC, RGB(255,255,255));
  TextOut(DrawingContextP->OnscreenHDC,
          mScreenLeft + 2,
          mScreenTop + 1,
          mText.c_str(),
          mText.Length());

  // Draw text
  SetTextColor(DrawingContextP->OnscreenHDC, mColor);
  TextOut(DrawingContextP->OnscreenHDC,
          mScreenLeft + 1,
          mScreenTop,
          mText.c_str(),
          mText.Length());

  SetBkMode(DrawingContextP->OnscreenHDC, OldBkMode);
}
//---------------------------------------------------------------------------

