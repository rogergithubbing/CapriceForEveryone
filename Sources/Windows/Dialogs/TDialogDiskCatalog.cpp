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
#include "TDialogDiskCatalog.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TDialogDiskCatalog::TDialogDiskCatalog(TDrawingContext* DrawingContextP) : TDialogForm()
//
// Constructor
//
{
TColor ActiveColor = (TColor)0x00ffff00; // ABGR
TColor InactiveColor = (TColor)0x00000000; // ABGR

  // Prepare size
  mLeft = 0.02f;
  mRight = 0.98f;
  mTop = 0.02f;
  mBottom = 0.98f;

  // Init members
  mEmulatorP = NULL;

  //
  // Create components
  //
  LabelTitle = new TDialogLabel(DrawingContextP);
  LabelTitle->Top = 0.02f;
  LabelTitle->Left = 0.03f;
  LabelTitle->Color = ActiveColor;
  LabelTitle->Text = "Drive A disk catalog";
  ListBoxFiles = new TDialogListBox(DrawingContextP);
  ListBoxFiles->ActiveColor = ActiveColor;
  ListBoxFiles->InactiveColor = InactiveColor;
  ListBoxFiles->Left = 0.15f;
  ListBoxFiles->Right = 0.50f;
  ListBoxFiles->Top = 0.20f;
  ListBoxFiles->Bottom = 0.91f;
  ButtonRUN = new TDialogButton(DrawingContextP);
  ButtonRUN->ActiveColor = ActiveColor;
  ButtonRUN->InactiveColor = InactiveColor;
  ButtonRUN->Left = 0.60f;
  ButtonRUN->Right = 0.85f;
  ButtonRUN->Top = 0.20f;
  ButtonRUN->Bottom = 0.30f;
  ButtonRUN->Text = "RUN";
  ButtonRUN->Type = PushButton;
  ButtonRUN->OnPressed = OnRUNPressed;
  ButtonLOAD = new TDialogButton(DrawingContextP);
  ButtonLOAD->ActiveColor = ActiveColor;
  ButtonLOAD->InactiveColor = InactiveColor;
  ButtonLOAD->Left = 0.60f;
  ButtonLOAD->Right = 0.85f;
  ButtonLOAD->Top = 0.35f;
  ButtonLOAD->Bottom = 0.45f;
  ButtonLOAD->Text = "LOAD";
  ButtonLOAD->Type = PushButton;
  ButtonLOAD->OnPressed = OnLOADPressed;
  ButtonCPM = new TDialogButton(DrawingContextP);
  ButtonCPM->ActiveColor = ActiveColor;
  ButtonCPM->InactiveColor = InactiveColor;
  ButtonCPM->Left = 0.60f;
  ButtonCPM->Right = 0.85f;
  ButtonCPM->Top = 0.50f;
  ButtonCPM->Bottom = 0.60f;
  ButtonCPM->Text = "|CPM";
  ButtonCPM->Type = PushButton;
  ButtonCPM->OnPressed = OnCPMPressed;
  ButtonCancel = new TDialogButton(DrawingContextP);
  ButtonCancel->ActiveColor = ActiveColor;
  ButtonCancel->InactiveColor = InactiveColor;
  ButtonCancel->Left = 0.60f;
  ButtonCancel->Right = 0.85f;
  ButtonCancel->Top = 0.86f;
  ButtonCancel->Bottom = 0.91f;
  ButtonCancel->Text = "Cancel";
  ButtonCancel->Type = PushButton;
  ButtonCancel->OnPressed = OnCancelPressed;

  //
  // Add components in order of display (first in the list, first displayed)
  //
  AddComponent(LabelTitle);
  AddComponent(ListBoxFiles);
  AddComponent(ButtonRUN);
  AddComponent(ButtonLOAD);
  AddComponent(ButtonCPM);
  AddComponent(ButtonCancel);

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
      Application->MessageBox(msg.c_str(), _T("TDialogOpenDSK::TDialogOpenDSK"), MB_OK);
#else /* _DEBUG */
      NOT_USED(Result);
#endif /* _DEBUG */
    }
  }
#endif /* _USE_DIRECTX */

  // Prepare components size
  OnResize(DrawingContextP);

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


 __fastcall TDialogDiskCatalog::~TDialogDiskCatalog()
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


bool __fastcall TDialogDiskCatalog::OnKeyDown(WORD &Key)
{
bool treated = true;

  switch (Key)
  {
    case VK_UP:
    {
      ListBoxFiles->SelectedIndex--;
    }
    break;

    case VK_DOWN:
    {
      ListBoxFiles->SelectedIndex++;
    }
    break;

    case VK_PRIOR:
    {
      ListBoxFiles->SelectedIndex -= ListBoxFiles->DisplayedItemCount;
    }
    break;

    case VK_NEXT:
    {
      ListBoxFiles->SelectedIndex += ListBoxFiles->DisplayedItemCount;
    }
    break;

    case VK_HOME:
    {
      ListBoxFiles->SelectedIndex = 0;
    }
    break;

    case VK_END:
    {
      ListBoxFiles->SelectedIndex = ListBoxFiles->ItemCount;
    }
    break;

    case VK_RETURN:
    {
      if (ButtonRUN->Visible == true)
      {
        OnRUNPressed();
      }
      else
      {
        OnCPMPressed();
      }
    }
    break;

    case VK_ESCAPE:
    {
      OnCancelPressed();
    }
    break;

    default:
    {
      // Numeric key pressed
      if ( (Key >= 0x30) && (Key <= 0x39) )
      {
        AnsiString KeyString = (char)Key;
        ListBoxFiles->FindNext(KeyString);
      }
      // Alphanumeric key pressed
      else if ( (Key >= 0x41) && (Key <= 0x5a) )
      {
        AnsiString KeyString = (char)Key;
        ListBoxFiles->FindNext(KeyString);
      }
      else
      {
        treated = false;
      }
    }
  }

  return treated;
}
//---------------------------------------------------------------------------


void __fastcall TDialogDiskCatalog::ShowCatalog(TEmulator* EmulatorP)
{
  mEmulatorP = EmulatorP;

  // Fill listbox with catalog entries
  ListBoxFiles->Clear();
  mEmulatorP->ReadDiskCatalog(DriveA);
  for (int Loop=0; Loop < mEmulatorP->NbCatalogEntries; Loop++)
  {
    ListBoxFiles->Add(mEmulatorP->CatalogEntry[Loop]);
  }

  ButtonRUN->Visible = ListBoxFiles->ItemCount ? true : false;
  ButtonLOAD->Visible = ListBoxFiles->ItemCount ? true : false;
}
//---------------------------------------------------------------------------


AnsiString __fastcall TDialogDiskCatalog::GetSelectedFile(void)
{
  return ListBoxFiles->SelectedString;
}
//---------------------------------------------------------------------------


void __fastcall TDialogDiskCatalog::OnCancelPressed(void)
{
  if (OnEndEvent) OnEndEvent(Cancel);
}
//---------------------------------------------------------------------------
void __fastcall TDialogDiskCatalog::OnRUNPressed(void)
{
  // File is selected
  if (OnEndEvent) OnEndEvent(Button1);
}
//---------------------------------------------------------------------------
void __fastcall TDialogDiskCatalog::OnLOADPressed(void)
{
  // File is selected
  if (OnEndEvent) OnEndEvent(Button2);
}
//---------------------------------------------------------------------------
void __fastcall TDialogDiskCatalog::OnCPMPressed(void)
{
  // File is selected
  if (OnEndEvent) OnEndEvent(Button3);
}
//---------------------------------------------------------------------------


void __fastcall TDialogDiskCatalog::OnMouseDblClick(int X, int Y)
{
  if (ListBoxFiles->OnMouseDblClick(X,Y) == true)
  {
    if (ButtonRUN->Visible == true)
    {
      OnRUNPressed();
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogDiskCatalog::OnResize(TDrawingContext* DrawingContextP)
{
  TDialogForm::OnResize(DrawingContextP);

#ifdef _USE_DIRECTX
  if (DrawingContextP->UseDirectX == true)
  {
    // Dialog background vertices
    float Left = (float)mScreenLeft;
    float Top  = (float)mScreenTop;
    float Right = (float)mScreenRight;
    float Bottom = (float)mScreenBottom;
    D3DCOLOR Color = D3DCOLOR_ARGB(178, 0, 0, 0); // Black 30% transparent
    D3DDialogVertex BackgroundVertices[] =
    {
      //X,     Y,      Z,    RHW,   DIFFUSE
      { Left,  Top,    0.0f, 1.0f,  Color },
      { Right, Top,    0.0f, 1.0f,  Color },
      { Left,  Bottom, 0.0f, 1.0f,  Color },
      { Right, Bottom, 0.0f, 1.0f,  Color },
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


void __fastcall TDialogDiskCatalog::OnDisplay(TDrawingContext* DrawingContextP)
{
  mDialogDisplay(DrawingContextP);
}
//---------------------------------------------------------------------------

void __fastcall TDialogDiskCatalog::OnDisplayD3D(TDrawingContext* DrawingContextP)
{
#ifdef _USE_DIRECTX

  // Draw dialog background
  DrawingContextP->D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DrawingContextP->D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  DrawingContextP->D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  DrawingContextP->D3DDevice->SetStreamSource(0,
                                              mD3DBackgroundVertexBuffer,
                                              0,
                                              sizeof(D3DDialogVertex) );
  DrawingContextP->D3DDevice->SetFVF(D3DFVF_DIALOGVERTEX);
  DrawingContextP->D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  DrawingContextP->D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

  // Draw dialog boundaries
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
  DrawingContextP->D3DLine->Draw(lines, 5, D3DCOLOR_XRGB(0, 255, 255)); // Cyan
  DrawingContextP->D3DLine->End();

  // Display all components
  TDialogForm::OnDisplay(DrawingContextP);

#else /* _USE_DIRECTX */

  NOT_USED(DrawingContextP);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDialogDiskCatalog::OnDisplayGL(TDrawingContext* DrawingContextP)
{
#ifdef _USE_OPENGL

  // Push emulator texture palette parameters
  glPushAttrib(GL_CURRENT_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Draw dialog background
  glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
  glBegin(GL_QUADS);
    glVertex2i(mScreenLeft, mScreenTop);
    glVertex2i(mScreenRight, mScreenTop);
    glVertex2i(mScreenRight, mScreenBottom);
    glVertex2i(mScreenLeft, mScreenBottom);
  glEnd();

  // Draw dialog border
  glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINE_LOOP);
    glVertex2i(mScreenLeft, mScreenTop);
    glVertex2i(mScreenRight, mScreenTop);
    glVertex2i(mScreenRight, mScreenBottom);
    glVertex2i(mScreenLeft, mScreenBottom);
  glEnd();

  // Display all components
  TDialogForm::OnDisplay(DrawingContextP);

  glDisable(GL_BLEND);
  glPopAttrib();

#else /* _USE_OPENGL */

  NOT_USED(DrawingContextP);

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDialogDiskCatalog::OnDisplayGDI(TDrawingContext* DrawingContextP)
{
  // Draw Dialog background
  RECT BackgroundRect;
  SetRect(&BackgroundRect, mScreenLeft, mScreenTop, mScreenRight, mScreenBottom);
  HBRUSH BackgroundHBrush = CreateSolidBrush(RGB(0,0,0));
  HBRUSH oldBrush = (HBRUSH)SelectObject(DrawingContextP->OnscreenHDC, BackgroundHBrush);
  FillRect(DrawingContextP->OnscreenHDC, &BackgroundRect, BackgroundHBrush);
  SelectObject(DrawingContextP->OnscreenHDC, oldBrush);
  DeleteObject(BackgroundHBrush);

  // Draw dialog border
  HPEN BorderHPen = CreatePen(PS_SOLID, 2, RGB(0,255,255));
  HPEN oldPen = (HPEN)SelectObject(DrawingContextP->OnscreenHDC, BorderHPen);
  MoveToEx(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenTop, NULL);
  LineTo(DrawingContextP->OnscreenHDC, mScreenRight, mScreenTop);
  LineTo(DrawingContextP->OnscreenHDC, mScreenRight, mScreenBottom);
  LineTo(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenBottom);
  LineTo(DrawingContextP->OnscreenHDC, mScreenLeft, mScreenTop);
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
                                  mScreenRight - 2,
                                  mScreenBottom - 2);
  SelectClipRgn(DrawingContextP->OnscreenHDC, ClipRegion);

  HFONT oldFont = (HFONT)SelectObject(DrawingContextP->OnscreenHDC,
                                      DrawingContextP->DialogFont);

  // Display all components
  TDialogForm::OnDisplay(DrawingContextP);

  SelectObject(DrawingContextP->OnscreenHDC, oldFont);

  // Restore previous clipping area
  SelectClipRgn(DrawingContextP->OnscreenHDC, restoreRegion);
  if (restoreRegion != NULL)
  {
    DeleteObject(restoreRegion);
  }
  DeleteObject(ClipRegion);
}
//---------------------------------------------------------------------------

