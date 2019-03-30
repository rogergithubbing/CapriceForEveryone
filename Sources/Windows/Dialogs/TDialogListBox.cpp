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
#include "TDialogListBox.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TDialogListBox::TDialogListBox(TDrawingContext* DrawingContextP) : TDialogComponent()
//
// Constructor
//
{
	mStringList = new TStringList();
  mAlphaSorted = true;
  mItemHeight = 0.05f;
  mScrollbarWidth = 0.03f;
  mResizeRequested = true;
  mTopIndex = 0;
  mOnSelectionChange = NULL;

#ifdef _USE_DIRECTX
  mD3DSelectedItemVertexBuffer = NULL;
  if (DrawingContextP->UseDirectX == true)
  {
    HRESULT Result;

    // Background vertices
    if (FAILED(Result = DrawingContextP->D3DDevice->CreateVertexBuffer(4 * sizeof(D3DDialogVertex),
                                                                       D3DUSAGE_WRITEONLY,
                                                                       D3DFVF_DIALOGVERTEX,
                                                                       D3DPOOL_MANAGED,
                                                                       &mD3DSelectedItemVertexBuffer,
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


__fastcall TDialogListBox::~TDialogListBox(void)
//
// Destructor
//
{
	delete mStringList;

#ifdef _USE_DIRECTX
  if (mD3DSelectedItemVertexBuffer != NULL)
  {
    mD3DSelectedItemVertexBuffer->Release();
  }
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::SetActiveColor(TColor Value)
{
  mActiveColor = Value;
#ifdef _USE_OPENGL
  mActiveColorRed = (GLfloat)GetRValue(Value) / 255.0f;
  mActiveColorGreen = (GLfloat)GetGValue(Value) / 255.0f;
  mActiveColorBlue = (GLfloat)GetBValue(Value) / 255.0f;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  mD3DActiveColor = D3DCOLOR_ARGB(255,
                                  GetRValue(Value),
                                  GetGValue(Value),
                                  GetBValue(Value));
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::SetInactiveColor(TColor Value)
{
  mInactiveColor = Value;
#ifdef _USE_OPENGL
  mInactiveColorRed = (GLfloat)GetRValue(Value) / 255.0f;
  mInactiveColorGreen = (GLfloat)GetGValue(Value) / 255.0f;
  mInactiveColorBlue = (GLfloat)GetBValue(Value) / 255.0f;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  mD3DInactiveColor = D3DCOLOR_ARGB(255,
                                    GetRValue(Value),
                                    GetGValue(Value),
                                    GetBValue(Value));
#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::SetItemHeight(float Value)
{
  mItemHeight = Value;
  mResizeRequested = true;
};
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::SetScrollbarWidth(float Value)
{
  mScrollbarWidth = Value;
  mResizeRequested = true;
};
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::Clear(void)
{
	mStringList->Clear();

  mTopIndex = 0;
  mSelectedIndex = 0;
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::Add(AnsiString NewItem)
{
	// Add new string
	mStringList->Add(NewItem);

	if (mAlphaSorted == true) mStringList->Sort();
}
//---------------------------------------------------------------------------


AnsiString __fastcall TDialogListBox::GetSelectedString(void)
{
	return mStringList->Strings[mSelectedIndex];
};
//---------------------------------------------------------------------------


int __fastcall TDialogListBox::GetItemCount(void)
{
	return mStringList->Count;
};
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::SetSelectedIndex(int Value)
{
	if (mStringList->Count == 0) return;

  mSelectedIndex = Value;

  if (mSelectedIndex < 0)
  {
    mSelectedIndex = 0;
  }
	else if (mSelectedIndex >= (mStringList->Count - 1))
	{
		mSelectedIndex = mStringList->Count - 1;
	}

  if (mSelectedIndex < mTopIndex)
  {
    mTopIndex = mSelectedIndex;
  }
  else if (mSelectedIndex >= (mTopIndex+mDisplayedItemCount))
  {
    mTopIndex = mSelectedIndex - mDisplayedItemCount + 1;
  }

  // Signal event
  if (mOnSelectionChange) mOnSelectionChange();
};
//---------------------------------------------------------------------------


int __fastcall TDialogListBox::Select(AnsiString NewItem)
{
	int Index = mStringList->IndexOf(NewItem);

	mSelectedIndex = Index;
	UpdateTopIndex();

	return Index;
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::OnResize(TDrawingContext* DrawingContextP)
{
  TDialogComponent::OnResize(DrawingContextP);
  mResizeRequested = false;

  // Update screen size
  mScreenScrollbarLeft = mScreenRight - (int)((float)DrawingContextP->EmulatorScreenWidth * mScrollbarWidth);
  mScreenItemHeight = (int)((float)DrawingContextP->EmulatorScreenHeight * mItemHeight);

  mDisplayedItemCount = (mScreenBottom - mScreenTop) / mScreenItemHeight;

  SIZE TextExtent = DrawingContextP->GetDialogExtent("A"); // Get font height

  if (DrawingContextP->UseOpenGL == true)
  {
    mItemTextOffset = (mScreenItemHeight - TextExtent.cy) / 2 + DrawingContextP->DialogFontLeading;
  }
  else
  {
    mItemTextOffset = (mScreenItemHeight - TextExtent.cy) / 2;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::UpdateTopIndex(void)
{
  if (mSelectedIndex < mDisplayedItemCount)
  {
    mTopIndex = 0;
  }
	else if ((mStringList->Count-mSelectedIndex) < mDisplayedItemCount)
	{
		mTopIndex = mStringList->Count - mDisplayedItemCount;
	}
  else
  {
    mTopIndex = mSelectedIndex - (mDisplayedItemCount/2);
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::FindNext(AnsiString Key)
{
  int CurrentIndex = mSelectedIndex;
  int FoundIndex = -1;

  Key = Key.UpperCase();

  do
  {
    // Next string
    CurrentIndex++;
		if (CurrentIndex >= mStringList->Count)
		{
			CurrentIndex = 0;
		}

		// Check key string
		if (mStringList->Strings[CurrentIndex].UpperCase().Pos(Key) == 1)
		{
      FoundIndex = CurrentIndex;
    }
  }
  while ( (FoundIndex == -1) && (CurrentIndex != mSelectedIndex) );

  // Select new item
  if (FoundIndex != -1)
  {
    mSelectedIndex = FoundIndex;
    UpdateTopIndex();
  }
}
//---------------------------------------------------------------------------


bool __fastcall TDialogListBox::OnMouseClick(int X, int Y)
{
  bool Inside = TDialogComponent::OnMouseClick(X, Y);
  if (Inside == true)
  {
    // Scrollbar check
    if (X > mScreenScrollbarLeft)
    {
      if (Y < (mScreenTop + ((mScreenBottom - mScreenTop) / 2)))
      {
        SetSelectedIndex(mSelectedIndex - mDisplayedItemCount);
      }
      else
      {
        SetSelectedIndex(mSelectedIndex + mDisplayedItemCount);
      }
    }
    else // Item selection
    {
      int Item = (Y - mScreenTop) / mScreenItemHeight;
      SetSelectedIndex(mTopIndex + Item);
    }
  }

  return Inside;
}
//---------------------------------------------------------------------------


void __fastcall TDialogListBox::OnDisplay(TDrawingContext* DrawingContextP)
{
  if (mResizeRequested == true)
  {
    OnResize(DrawingContextP);
  }

  if (mVisible == true)
  {
    // Component display
    mDialogDisplay(DrawingContextP);
  }
}
//---------------------------------------------------------------------------

void __fastcall TDialogListBox::OnDisplayD3D(TDrawingContext* DrawingContextP)
{
#ifdef _USE_DIRECTX

  // Draw dialog border
  D3DXVECTOR2 BorderLines[] =
  {
    D3DXVECTOR2(mScreenLeft, mScreenTop),
    D3DXVECTOR2(mScreenRight, mScreenTop),
    D3DXVECTOR2(mScreenRight, mScreenBottom),
    D3DXVECTOR2(mScreenLeft, mScreenBottom),
    D3DXVECTOR2(mScreenLeft, mScreenTop),
  };
  DrawingContextP->D3DLine->SetWidth(2.0f);
  DrawingContextP->D3DLine->Begin();
  DrawingContextP->D3DLine->Draw(BorderLines, 5, mD3DActiveColor);
  DrawingContextP->D3DLine->End();

  // Draw scrollbar
  int ScreenMiddle = mScreenTop + ((mScreenBottom - mScreenTop) / 2);
  D3DXVECTOR2 ScrollbarVertLines[] =
  {
    D3DXVECTOR2(mScreenScrollbarLeft, mScreenTop),
    D3DXVECTOR2(mScreenScrollbarLeft, mScreenBottom),
  };
  D3DXVECTOR2 ScrollbarMidLines[] =
  {
    D3DXVECTOR2(mScreenScrollbarLeft, ScreenMiddle),
    D3DXVECTOR2(mScreenRight, ScreenMiddle),
  };
  DrawingContextP->D3DLine->SetWidth(2.0f);
  DrawingContextP->D3DLine->Begin();
  DrawingContextP->D3DLine->Draw(ScrollbarVertLines, 2, mD3DActiveColor);
  DrawingContextP->D3DLine->Draw(ScrollbarMidLines, 2, mD3DActiveColor);
  DrawingContextP->D3DLine->End();

  // Display items
  if (mStringList->Count)
  {
    int itemloop = mTopIndex;
    for (int itemtop = mScreenTop;
         (itemtop+mScreenItemHeight) < mScreenBottom;
         itemtop += mScreenItemHeight, itemloop++)
    {
      if (itemloop < mStringList->Count)
      {
        if (itemloop == mSelectedIndex)
        {
          float Left = (float)mScreenLeft;
          float Top  = (float)itemtop;
          float Right = (float)mScreenScrollbarLeft;
          float Bottom = (float)itemtop+mScreenItemHeight;
          D3DDialogVertex SelectedItemVertices[] =
          {
            //X,     Y,      Z,    RHW,   DIFFUSE
            { Left,  Top,    0.0f, 1.0f,  mD3DActiveColor },
            { Right, Top,    0.0f, 1.0f,  mD3DActiveColor },
            { Left,  Bottom, 0.0f, 1.0f,  mD3DActiveColor },
            { Right, Bottom, 0.0f, 1.0f,  mD3DActiveColor },
          };
          void *pVertices = NULL;
          mD3DSelectedItemVertexBuffer->Lock(0, sizeof(SelectedItemVertices), (void**)&pVertices, 0);
          if (pVertices != NULL)
          {
            memcpy(pVertices, SelectedItemVertices, sizeof(SelectedItemVertices) );
            mD3DSelectedItemVertexBuffer->Unlock();
          }

          // Fill area with active color
          DrawingContextP->D3DDevice->SetStreamSource(0,
                                                      mD3DSelectedItemVertexBuffer,
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
                mScreenLeft+5,
                itemtop+mItemTextOffset,
                mScreenScrollbarLeft - 4,
                itemtop+mScreenItemHeight);

        // Text in plain color
        D3DCOLOR TextColor = (itemloop == mSelectedIndex) ? mD3DInactiveColor : mD3DActiveColor;
        DrawingContextP->D3DDialogFont->DrawText(NULL,
                                                 String(mStringList->Strings[itemloop]).c_str(),
                                                 -1,
                                                 &textRect,
                                                 DT_SINGLELINE,
                                                 TextColor);
      }
    }
  }

#else /* _USE_DIRECTX */

  NOT_USED(DrawingContextP);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDialogListBox::OnDisplayGL(TDrawingContext* DrawingContextP)
{
#ifdef _USE_OPENGL

  // Draw dialog border
  glColor4f(mActiveColorRed, mActiveColorGreen, mActiveColorBlue, 1.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINE_LOOP);
    glVertex2i(mScreenLeft, mScreenTop);
    glVertex2i(mScreenRight, mScreenTop);
    glVertex2i(mScreenRight, mScreenBottom);
    glVertex2i(mScreenLeft, mScreenBottom);
  glEnd();

  // Draw scrollbar
  int ScreenMiddle = mScreenTop + ((mScreenBottom - mScreenTop) / 2);
  glBegin(GL_LINES);
    glVertex2i(mScreenScrollbarLeft, mScreenTop);
    glVertex2i(mScreenScrollbarLeft, mScreenBottom);
    glVertex2i(mScreenScrollbarLeft, ScreenMiddle);
    glVertex2i(mScreenRight, ScreenMiddle);
  glEnd();

  // Text area clipping
  glEnable(GL_SCISSOR_TEST);
  glScissor(mScreenLeft+2,
            DrawingContextP->EmulatorScreenBottom - mScreenBottom + 2,
            mScreenScrollbarLeft - mScreenLeft - 4,
            mScreenBottom - mScreenTop - 4);

  // Push the list base value
  glPushAttrib(GL_LIST_BIT);
  // Set a new list base value.
  glListBase(DrawingContextP->GLDialogFontBase - 32);

  // Display items
  if (mStringList->Count)
  {
    int itemloop = mTopIndex;
    for (int itemtop = mScreenTop;
         (itemtop+mScreenItemHeight) < mScreenBottom;
         itemtop += mScreenItemHeight, itemloop++)
    {
      if (itemloop < mStringList->Count)
      {
        if (itemloop == mSelectedIndex)
        {
          // Fill area with active color
          glColor4f(mActiveColorRed, mActiveColorGreen, mActiveColorBlue, 1.0f);
          glBegin(GL_QUADS);
            glVertex2i(mScreenLeft, itemtop);
            glVertex2i(mScreenScrollbarLeft, itemtop);
            glVertex2i(mScreenScrollbarLeft, itemtop+mScreenItemHeight);
            glVertex2i(mScreenLeft, itemtop+mScreenItemHeight);
          glEnd();

          // Set text color
          glColor4f(mInactiveColorRed, mInactiveColorGreen, mInactiveColorBlue, 1.0f);
        }
        else
        {
          // Set text color
          glColor4f(mActiveColorRed, mActiveColorGreen, mActiveColorBlue, 1.0f);
        }

        // Draw item string
        glRasterPos2i(mScreenLeft+5, itemtop+mScreenItemHeight-mItemTextOffset);

        // Call the lists to draw the text.
        glCallLists((GLsizei)mStringList->Strings[itemloop].Length(),
                    GL_UNSIGNED_BYTE,
                    (GLvoid *)AnsiString(mStringList->Strings[itemloop]).c_str());
      }
    }
  }

  // Restore list
  glPopAttrib();
  // Stop area clipping
  glDisable(GL_SCISSOR_TEST);

#else /* _USE_OPENGL */

  NOT_USED(DrawingContextP);

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDialogListBox::OnDisplayGDI(TDrawingContext* DrawingContextP)
{
  HPEN BorderHPen = CreatePen(PS_SOLID, 2, mActiveColor);
  HPEN oldPen = (HPEN)SelectObject(DrawingContextP->OnscreenHDC, BorderHPen);

  // Draw dialog border
  MoveToEx(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenTop, NULL);
  LineTo(DrawingContextP->OnscreenHDC, mScreenRight, mScreenTop);
  LineTo(DrawingContextP->OnscreenHDC, mScreenRight, mScreenBottom);
  LineTo(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenBottom);
  LineTo(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenTop);

  // Draw scrollbar
  int ScreenMiddle = mScreenTop + ((mScreenBottom - mScreenTop) / 2);
  MoveToEx(DrawingContextP->OnscreenHDC, mScreenScrollbarLeft, mScreenTop, NULL);
  LineTo(DrawingContextP->OnscreenHDC, mScreenScrollbarLeft, mScreenBottom);
  MoveToEx(DrawingContextP->OnscreenHDC, mScreenScrollbarLeft, ScreenMiddle, NULL);
  LineTo(DrawingContextP->OnscreenHDC, mScreenRight, ScreenMiddle);

  SelectObject(DrawingContextP->OnscreenHDC, oldPen);
  DeleteObject(BorderHPen);

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
                                  mScreenScrollbarLeft - 2,
                                  mScreenBottom - 2);
  SelectClipRgn(DrawingContextP->OnscreenHDC, ClipRegion);

  int OldBkMode = SetBkMode(DrawingContextP->OnscreenHDC, TRANSPARENT);

  // Display items
  if (mStringList->Count)
  {
    int itemloop = mTopIndex;
    for (int itemtop = mScreenTop;
         (itemtop+mScreenItemHeight) < mScreenBottom;
         itemtop += mScreenItemHeight, itemloop++)
    {
      if (itemloop == mSelectedIndex)
      {
        // Fill area with active color
        RECT BackgroundRect;
        SetRect(&BackgroundRect,
                mScreenLeft,
                itemtop,
                mScreenScrollbarLeft,
                itemtop+mScreenItemHeight);
        HBRUSH BackgroundHBrush = CreateSolidBrush(mActiveColor);
        HBRUSH oldBrush = (HBRUSH)SelectObject(DrawingContextP->OnscreenHDC, BackgroundHBrush);
        FillRect(DrawingContextP->OnscreenHDC, &BackgroundRect, BackgroundHBrush);
        SelectObject(DrawingContextP->OnscreenHDC, oldBrush);
        DeleteObject(BackgroundHBrush);

        // Set text color
        SetTextColor(DrawingContextP->OnscreenHDC, mInactiveColor);
      }
      else
      {
        // Set text color
        SetTextColor(DrawingContextP->OnscreenHDC, mActiveColor);
      }

      if (itemloop < mStringList->Count)
      {
        TextOut(DrawingContextP->OnscreenHDC,
            mScreenLeft+5,
            itemtop+mItemTextOffset,
            mStringList->Strings[itemloop].c_str(),
            mStringList->Strings[itemloop].Length());
      }
    }
  }

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

