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
#include <tchar.h>
#pragma hdrstop
#include "TDialogButton.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



__fastcall TDialogButton::TDialogButton(TDrawingContext* DrawingContextP) : TDialogComponent()
//
// Constructor
//
{
  OnPressed = NULL;
  mActivated = false;
  mText = "";

#ifdef _USE_DIRECTX
  mD3DBackgroundVertexBuffer = NULL;
  if (DrawingContextP->UseDirectX == true)
  {
    HRESULT Result;

    // Background vertices
    if (FAILED(Result = DrawingContextP->D3DDevice->CreateVertexBuffer(4 * sizeof(D3DDialogVertex),
                                                                       D3DUSAGE_WRITEONLY,
                                                                       D3DFVF_DIALOGVERTEX,
                                                                       D3DPOOL_MANAGED,
                                                                       &mD3DBackgroundVertexBuffer,
                                                                       NULL)))
    {
#ifdef _DEBUG
      String msg = _T("CreateVertexBuffer Result=") + String::IntToHex(Result,8);
      Application->MessageBox(msg.c_str(), _T("TDialogButton::TDialogButton"), MB_OK);
#else /* _DEBUG */
      NOT_USED(Result);
#endif /* _DEBUG */
    }
  }
#endif /* _USE_DIRECTX */

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


__fastcall TDialogButton::~TDialogButton(void)
//
// Destructor
//
{
#ifdef _USE_DIRECTX
  if (mD3DBackgroundVertexBuffer != NULL)
  {
    mD3DBackgroundVertexBuffer->Release();
  }
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogButton::SetText(String Value)
{
  mText = Value;
  mTextExtent.cx = 0;
}
//---------------------------------------------------------------------------


void __fastcall TDialogButton::SetActiveColor(TColor Value)
{
  mActiveColor = Value;
#ifdef _USE_OPENGL
  mGLActiveColorRed = (GLfloat)GetRValue(Value) / 255.0f;
  mGLActiveColorGreen = (GLfloat)GetGValue(Value) / 255.0f;
  mGLActiveColorBlue = (GLfloat)GetBValue(Value) / 255.0f;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  mD3DActiveColor = D3DCOLOR_ARGB(255,
                                  GetRValue(Value),
                                  GetGValue(Value),
                                  GetBValue(Value));
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogButton::SetInactiveColor(TColor Value)
{
  mInactiveColor = Value;
#ifdef _USE_OPENGL
  mGLInactiveColorRed = (GLfloat)GetRValue(Value) / 255.0f;
  mGLInactiveColorGreen = (GLfloat)GetGValue(Value) / 255.0f;
  mGLInactiveColorBlue = (GLfloat)GetBValue(Value) / 255.0f;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  mD3DInactiveColor = D3DCOLOR_ARGB(255,
                                    GetRValue(Value),
                                    GetGValue(Value),
                                    GetBValue(Value));
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogButton::UpdateTextOrigin(TDrawingContext* DrawingContextP)
{
  mTextExtent = DrawingContextP->GetDialogExtent(mText);

  if (DrawingContextP->UseOpenGL == true)
  {
    mScreenTextLeft = (((mScreenRight - mScreenLeft) - mTextExtent.cx) / 2)
                      + mScreenLeft;
    mScreenTextTop = (((mScreenBottom - mScreenTop) - mTextExtent.cy) / 2)
                      + mScreenTop + mTextExtent.cy - DrawingContextP->DialogFontLeading;
  }
  else
  {
    mScreenTextLeft = (((mScreenRight - mScreenLeft) - mTextExtent.cx) / 2)
                      + mScreenLeft;
    mScreenTextTop = (((mScreenBottom - mScreenTop) - mTextExtent.cy) / 2)
                      + mScreenTop;
  }
}
//---------------------------------------------------------------------------


bool __fastcall TDialogButton::OnMouseClick(int X, int Y)
{
bool Inside = TDialogComponent::OnMouseClick(X, Y);

  if (Inside == true)
  {
    if (mType == PressOnlyButton)
    {
      mActivated = true;
    }
    else if (mType == ToggleButton)
    {
      mActivated = !mActivated;
    }

    // Signal event
    if (mOnPressed)
    {
      mOnPressed();
    }
  }

  return Inside;
}
//---------------------------------------------------------------------------


void __fastcall TDialogButton::OnResize(TDrawingContext* DrawingContextP)
{
  TDialogComponent::OnResize(DrawingContextP);
  UpdateTextOrigin(DrawingContextP);

#ifdef _USE_DIRECTX
  if (DrawingContextP->UseDirectX == true)
  {
    // Dialog background vertices
    float Left = (float)mScreenLeft;
    float Top  = (float)mScreenTop;
    float Right = (float)mScreenRight;
    float Bottom = (float)mScreenBottom;
    D3DDialogVertex BackgroundVertices[] =
    {
      //X,     Y,      Z,    RHW,   DIFFUSE
      { Left,  Top,    0.0f, 1.0f,  mD3DActiveColor },
      { Right, Top,    0.0f, 1.0f,  mD3DActiveColor },
      { Left,  Bottom, 0.0f, 1.0f,  mD3DActiveColor },
      { Right, Bottom, 0.0f, 1.0f,  mD3DActiveColor },
    };
    void *pVertices = NULL;
    mD3DBackgroundVertexBuffer->Lock(0, sizeof(BackgroundVertices), (void**)&pVertices, 0);
    if (pVertices != NULL)
    {
      memcpy(pVertices, BackgroundVertices, sizeof(BackgroundVertices) );
      mD3DBackgroundVertexBuffer->Unlock();
    }
  }
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogButton::OnDisplay(TDrawingContext* DrawingContextP)
{
  if (mTextExtent.cx == 0)
  {
    UpdateTextOrigin(DrawingContextP);
  }

  if (mVisible == true)
  {
    // Component display
    mDialogDisplay(DrawingContextP);
  }
}
//---------------------------------------------------------------------------

void __fastcall TDialogButton::OnDisplayD3D(TDrawingContext* DrawingContextP)
{
#ifdef _USE_DIRECTX

  if (mActivated == false)
  {
    // Draw dialog border
    D3DXVECTOR2 lines[] =
    {
      D3DXVECTOR2(mScreenLeft, mScreenTop),
      D3DXVECTOR2(mScreenRight, mScreenTop),
      D3DXVECTOR2(mScreenRight, mScreenBottom),
      D3DXVECTOR2(mScreenLeft, mScreenBottom),
      D3DXVECTOR2(mScreenLeft, mScreenTop),
    };
    DrawingContextP->D3DLine->SetWidth(2.0f);
    DrawingContextP->D3DLine->Begin();
    DrawingContextP->D3DLine->Draw(lines, 5, mD3DActiveColor);
    DrawingContextP->D3DLine->End();
  }
  else
  {
    // Draw filled button
    DrawingContextP->D3DDevice->SetStreamSource(0,
                                                mD3DBackgroundVertexBuffer,
                                                0,
                                                sizeof(D3DDialogVertex) );
    DrawingContextP->D3DDevice->SetFVF(D3DFVF_DIALOGVERTEX);
    DrawingContextP->D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  }

  //
  // Draw text
  //
  RECT textRect;
  SetRect(&textRect,
          mScreenTextLeft,
          mScreenTextTop,
          mScreenRight,
          mScreenBottom);

  // Text in plain color
  D3DCOLOR TextColor = (mActivated == false) ? mD3DActiveColor : mD3DInactiveColor;
  DrawingContextP->D3DDialogFont->DrawText(NULL,
                                           String(mText).c_str(),
                                           -1,
                                           &textRect,
                                           DT_SINGLELINE,
                                           TextColor);

#else /* _USE_DIRECTX */

  NOT_USED(DrawingContextP);

#endif /*_USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDialogButton::OnDisplayGL(TDrawingContext* DrawingContextP)
{
#ifdef _USE_OPENGL

  if (mActivated == false)
  {
    // Draw dialog border
    glColor4f(mGLActiveColorRed, mGLActiveColorGreen, mGLActiveColorBlue, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
      glVertex2i(mScreenLeft, mScreenTop);
      glVertex2i(mScreenRight, mScreenTop);
      glVertex2i(mScreenRight, mScreenBottom);
      glVertex2i(mScreenLeft, mScreenBottom);
    glEnd();
  }
  else
  {
    // Draw filled button
    glColor4f(mGLActiveColorRed, mGLActiveColorGreen, mGLActiveColorBlue, 1.0f);
    glBegin(GL_QUADS);
      glVertex2i(mScreenLeft, mScreenTop);
      glVertex2i(mScreenRight, mScreenTop);
      glVertex2i(mScreenRight, mScreenBottom);
      glVertex2i(mScreenLeft, mScreenBottom);
    glEnd();
  }

  //
  // Draw text
  //
  if (mActivated == false)
  {
    glColor4f(mGLActiveColorRed, mGLActiveColorGreen, mGLActiveColorBlue, 1.0f);
  }
  else
  {
    glColor4f(mGLInactiveColorRed, mGLInactiveColorGreen, mGLInactiveColorBlue, 1.0f);
  }
  glRasterPos2i(mScreenTextLeft, mScreenTextTop);

  // Text area clipping
  glEnable(GL_SCISSOR_TEST);
  glScissor(mScreenLeft+2,
            DrawingContextP->EmulatorScreenBottom - mScreenBottom + 2,
            mScreenRight - mScreenLeft - 4,
            mScreenBottom - mScreenTop - 4);

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

  glDisable(GL_SCISSOR_TEST);

#else /* _USE_OPENGL */

  NOT_USED(DrawingContextP);

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDialogButton::OnDisplayGDI(TDrawingContext* DrawingContextP)
{
  if (mActivated == false)
  {
    // Draw button border
    HPEN BorderHPen = CreatePen(PS_SOLID, 2, mActiveColor);
    HPEN oldPen = (HPEN)SelectObject(DrawingContextP->OnscreenHDC, BorderHPen);
    MoveToEx(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenTop, NULL);
    LineTo(DrawingContextP->OnscreenHDC, mScreenRight, mScreenTop);
    LineTo(DrawingContextP->OnscreenHDC, mScreenRight, mScreenBottom);
    LineTo(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenBottom);
    LineTo(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenTop);
    SelectObject(DrawingContextP->OnscreenHDC, oldPen);
    DeleteObject(BorderHPen);
  }
  else
  {
    // Fill Button
    RECT BackgroundRect;
    SetRect(&BackgroundRect, mScreenLeft, mScreenTop, mScreenRight, mScreenBottom);
    HBRUSH BackgroundHBrush = CreateSolidBrush(mActiveColor);
    HBRUSH oldBrush = (HBRUSH)SelectObject(DrawingContextP->OnscreenHDC, BackgroundHBrush);
    FillRect(DrawingContextP->OnscreenHDC, &BackgroundRect, BackgroundHBrush);
    SelectObject(DrawingContextP->OnscreenHDC, oldBrush);
    DeleteObject(BackgroundHBrush);
  }

  // Get current clipping area
  HRGN restoreRegion = CreateRectRgn( 0, 0, 0, 0 );
  if (GetClipRgn(DrawingContextP->OnscreenHDC, restoreRegion) != 1)
  {
    DeleteObject(restoreRegion);
    restoreRegion = NULL;
  }
  // Set clipping area
  HRGN ClipRegion = CreateRectRgn(mScreenLeft + 2,
                                  mScreenTop + 2,
                                  mScreenRight - 2,
                                  mScreenBottom - 2);
  SelectClipRgn(DrawingContextP->OnscreenHDC, ClipRegion);

  int OldBkMode = SetBkMode(DrawingContextP->OnscreenHDC, TRANSPARENT);

  if (mActivated == false)
  {
    SetTextColor(DrawingContextP->OnscreenHDC, mActiveColor);
  }
  else
  {
    SetTextColor(DrawingContextP->OnscreenHDC, mInactiveColor);
  }

  // Draw Text
  TextOut(DrawingContextP->OnscreenHDC,
          mScreenTextLeft,
          mScreenTextTop,
          mText.c_str(),
          mText.Length());

  SetBkMode(DrawingContextP->OnscreenHDC, OldBkMode);

  // Restore previous clipping area
  SelectClipRgn(DrawingContextP->OnscreenHDC, restoreRegion);
  if (restoreRegion != NULL)
  {
    DeleteObject(restoreRegion);
  }
  DeleteObject(ClipRegion);
}
//---------------------------------------------------------------------------

