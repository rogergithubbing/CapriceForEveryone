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
#include "TDialogOpenDSK.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


#define DSK_EXT                     "*.dsk"
#define IPF_EXT                     "*.ipf"
#define ZIP_EXT                     "*.zip"



__fastcall TDialogOpenDSK::TDialogOpenDSK(TDrawingContext* DrawingContextP) : TDialogForm()
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

  //
  // Create components
  //
  LabelTitle = new TDialogLabel(DrawingContextP);
  LabelTitle->Top = 0.02f;
  LabelTitle->Left = 0.03f;
  LabelTitle->Color = ActiveColor;
  LabelTitle->Text = "Select disk image file";
  LabelLoadOnDrive = new TDialogLabel(DrawingContextP);
  LabelLoadOnDrive->Top = 0.08f;
  LabelLoadOnDrive->Left = 0.05f;
  LabelLoadOnDrive->Color = ActiveColor;
  LabelLoadOnDrive->Text = "Load on";
  LabelPathTitle = new TDialogLabel(DrawingContextP);
  LabelPathTitle->Top = 0.20f;
  LabelPathTitle->Left = 0.05f;
  LabelPathTitle->Color = ActiveColor;
  LabelPathTitle->Text = "Path= ";
  LabelPath = new TDialogLabel(DrawingContextP);
  LabelPath->Top = 0.20f;
  LabelPath->Left = 0.11f;
  LabelPath->Color = ActiveColor;
  LabelExtensions = new TDialogLabel(DrawingContextP);
  LabelExtensions->Top = 0.14f;
  LabelExtensions->Left = 0.05f;
  LabelExtensions->Color = ActiveColor;
  LabelExtensions->Text = "Extensions";
  ButtonDSK = new TDialogButton(DrawingContextP);
  ButtonDSK->ActiveColor = ActiveColor;
  ButtonDSK->InactiveColor = InactiveColor;
  ButtonDSK->Left = 0.25f;
  ButtonDSK->Right = 0.35f;
  ButtonDSK->Top = 0.14f;
  ButtonDSK->Bottom = 0.19f;
  ButtonDSK->Text = "DSK";
  ButtonDSK->Type = ToggleButton;
  ButtonDSK->Activated = true;
  ButtonDSK->OnPressed = OnExtensionsPressed;
  ButtonIPF = new TDialogButton(DrawingContextP);
  ButtonIPF->ActiveColor = ActiveColor;
  ButtonIPF->InactiveColor = InactiveColor;
  ButtonIPF->Left = 0.36f;
  ButtonIPF->Right = 0.46f;
  ButtonIPF->Top = 0.14f;
  ButtonIPF->Bottom = 0.19f;
  ButtonIPF->Text = "IPF";
  ButtonIPF->Type = ToggleButton;
  ButtonIPF->Activated = true;
  ButtonIPF->OnPressed = OnExtensionsPressed;
  ButtonZIP = new TDialogButton(DrawingContextP);
  ButtonZIP->ActiveColor = ActiveColor;
  ButtonZIP->InactiveColor = InactiveColor;
  ButtonZIP->Left = 0.47f;
  ButtonZIP->Right = 0.57f;
  ButtonZIP->Top = 0.14f;
  ButtonZIP->Bottom = 0.19f;
  ButtonZIP->Text = "ZIP";
  ButtonZIP->Type = ToggleButton;
  ButtonZIP->Activated = true;
  ButtonZIP->OnPressed = OnExtensionsPressed;
  ButtonDriveA = new TDialogButton(DrawingContextP);
  ButtonDriveA->ActiveColor = ActiveColor;
  ButtonDriveA->InactiveColor = InactiveColor;
  ButtonDriveA->Left = 0.25f;
  ButtonDriveA->Right = 0.55f;
  ButtonDriveA->Top = 0.08f;
  ButtonDriveA->Bottom = 0.13f;
  ButtonDriveA->Text = "Drive A";
  ButtonDriveA->Type = PressOnlyButton;
  ButtonDriveA->OnPressed = OnDriveAPressed;
  ButtonDriveB = new TDialogButton(DrawingContextP);
  ButtonDriveB->ActiveColor = ActiveColor;
  ButtonDriveB->InactiveColor = InactiveColor;
  ButtonDriveB->Left = 0.60f;
  ButtonDriveB->Right = 0.90f;
  ButtonDriveB->Top = 0.08f;
  ButtonDriveB->Bottom = 0.13f;
  ButtonDriveB->Text = "Drive B";
  ButtonDriveB->Type = PressOnlyButton;
  ButtonDriveB->OnPressed = OnDriveBPressed;
  ListBoxFiles = new TDialogListBox(DrawingContextP);
  ListBoxFiles->ActiveColor = ActiveColor;
  ListBoxFiles->InactiveColor = InactiveColor;
  ListBoxFiles->Left = 0.10f;
  ListBoxFiles->Right = 0.90f;
  ListBoxFiles->Top = 0.25f;
  ListBoxFiles->Bottom = 0.86f;
  ButtonLoad = new TDialogButton(DrawingContextP);
  ButtonLoad->ActiveColor = ActiveColor;
  ButtonLoad->InactiveColor = InactiveColor;
  ButtonLoad->Left = 0.05f;
  ButtonLoad->Right = 0.30f;
  ButtonLoad->Top = 0.90f;
  ButtonLoad->Bottom = 0.95f;
  ButtonLoad->Text = "Load";
  ButtonLoad->Type = PushButton;
  ButtonLoad->OnPressed = OnLoadPressed;
  ButtonNew = new TDialogButton(DrawingContextP);
  ButtonNew->ActiveColor = ActiveColor;
  ButtonNew->InactiveColor = InactiveColor;
  ButtonNew->Left = 0.37f;
  ButtonNew->Right = 0.62f;
  ButtonNew->Top = 0.90f;
  ButtonNew->Bottom = 0.95f;
  ButtonNew->Text = "New";
  ButtonNew->Type = PushButton;
  ButtonNew->OnPressed = OnNewPressed;
  ButtonCancel = new TDialogButton(DrawingContextP);
  ButtonCancel->ActiveColor = ActiveColor;
  ButtonCancel->InactiveColor = InactiveColor;
  ButtonCancel->Left = 0.70f;
  ButtonCancel->Right = 0.95f;
  ButtonCancel->Top = 0.90f;
  ButtonCancel->Bottom = 0.95f;
  ButtonCancel->Text = "Cancel";
  ButtonCancel->Type = PushButton;
  ButtonCancel->OnPressed = OnCancelPressed;

  //
  // Add components in order of display (first in the list, first displayed)
  //
  AddComponent(LabelTitle);
  AddComponent(LabelLoadOnDrive);
  AddComponent(ButtonDriveA);
  AddComponent(ButtonDriveB);
  AddComponent(LabelPathTitle);
  AddComponent(LabelPath);
  AddComponent(LabelExtensions);
  AddComponent(ButtonDSK);
  AddComponent(ButtonIPF);
  AddComponent(ButtonZIP);
  AddComponent(ListBoxFiles);
  AddComponent(ButtonLoad);
  AddComponent(ButtonNew);
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


 __fastcall TDialogOpenDSK::~TDialogOpenDSK()
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


bool __fastcall TDialogOpenDSK::OnKeyDown(WORD &Key)
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

    case VK_LEFT:
    {
      // Select Drive A
      DriveAActive = true;
    }
    break;

    case VK_RIGHT:
    {
      // Select Drive B
      DriveBActive = true;
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
      OnLoadPressed();
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


void __fastcall TDialogOpenDSK::SetDriveAActive(bool Value)
{
  ButtonDriveA->Activated = Value;

  if (Value == true)
  {
    // Deactivate other drive
    ButtonDriveB->Activated = false;
  }
};
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::SetDriveBActive(bool Value)
{
  ButtonDriveB->Activated = Value;

  if (Value == true)
  {
    // Deactivate other drive
    ButtonDriveA->Activated = false;
  }
};
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::OnDriveAPressed(void)
{
  // Deactivate Drive B
  ButtonDriveB->Activated = false;
}
//---------------------------------------------------------------------------
void __fastcall TDialogOpenDSK::OnDriveBPressed(void)
{
  // Deactivate Drive A
  ButtonDriveA->Activated = false;
}
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::OnExtensionsPressed(void)
{
  SetInitialPath(mPath);
}
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::SetInitialPath(AnsiString Value)
{
  mPath = Value;
  mDisplayedPath = "";

  // Clear list
  ListBoxFiles->Clear();
  // First add folders
  TSearchRec sr;
  if (FindFirst(mPath + "*.*", faDirectory, sr) == 0)
  {
    do
    {
      if (sr.Attr & faDirectory)
      {
        if (sr.Name != ".")
        {
          ListBoxFiles->Add("\\" + sr.Name);
        }
      }
    }
    while (FindNext(sr) == 0);
    FindClose(sr);
  }
  //
  // Add all supported disk image files
  //
  // DSK
  if (ButtonDSK->Activated == true)
  {
    if (FindFirst(mPath + DSK_EXT, faAnyFile, sr) == 0)
    {
      do
      {
        ListBoxFiles->Add(sr.Name);
      }
      while (FindNext(sr) == 0);
      FindClose(sr);
    }
  }
  // IPF
  if (ButtonIPF->Activated == true)
  {
    if (FindFirst(mPath + IPF_EXT, faAnyFile, sr) == 0)
    {
      do
      {
        ListBoxFiles->Add(sr.Name);
      }
      while (FindNext(sr) == 0);
      FindClose(sr);
    }
  }
  // ZIP
  if (ButtonZIP->Activated == true)
  {
    if (FindFirst(mPath + ZIP_EXT, faAnyFile, sr) == 0)
    {
      do
      {
        ListBoxFiles->Add(sr.Name);
      }
      while (FindNext(sr) == 0);
      FindClose(sr);
    }
  }
};
//---------------------------------------------------------------------------


AnsiString __fastcall TDialogOpenDSK::GetSelectedFile(void)
{
  return (mPath + ListBoxFiles->SelectedString);
}
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::SetSelectedFile(AnsiString File)
{
  ListBoxFiles->Select(File);
}
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::ClipPath(TDrawingContext* DrawingContextP)
{
  // Get max path extent
  int MaxWidth = mScreenRight - LabelPath->ScreenLeft - 10; // Add right margin

  // Clip pathname
  mDisplayedPath = mPath;
  SIZE PathExtent = DrawingContextP->GetDialogExtent(mDisplayedPath);
  if (PathExtent.cx > MaxWidth)
  {
    int Pos = mDisplayedPath.Pos("\\");
    AnsiString Root = mDisplayedPath.SubString(1, Pos);
    AnsiString RemainPath = mDisplayedPath.SubString(Pos+1, mDisplayedPath.Length());
    
    while (PathExtent.cx > MaxWidth)
    {
      // Reduce path from root
      int NewFolderPos = RemainPath.Pos("\\") + 1;
      RemainPath = RemainPath.SubString(NewFolderPos, RemainPath.Length());

      AnsiString Temp = Root + "...\\" + RemainPath;
      PathExtent = DrawingContextP->GetDialogExtent(Temp);
    }

    mDisplayedPath = Root + "...\\" + RemainPath;
  }

  LabelPath->Text = mDisplayedPath;
};
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::OnLoadPressed(void)
{
  AnsiString SelectedFile = ListBoxFiles->SelectedString;
  // Folder is selected
  if (SelectedFile.IsPathDelimiter(1) == true)
  {
    if (SelectedFile == "\\..")
    {
      // Go to parent folder
      AnsiString CurrentPath = ExpandFileName(mPath + "..\\");
      SetInitialPath(CurrentPath);
    }
    else
    {
      AnsiString Folder = SelectedFile.SubString(2, SelectedFile.Length());
      SetInitialPath(CurrentPath + Folder + "\\");
    }
  }
  else
  {
    // File is selected
    if (OnEndEvent) OnEndEvent(Button1);
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::OnNewPressed(void)
{
  if (OnEndEvent) OnEndEvent(Button2);
}
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::OnCancelPressed(void)
{
  if (OnEndEvent) OnEndEvent(Cancel);
}
//---------------------------------------------------------------------------


void __fastcall TDialogOpenDSK::OnResize(TDrawingContext* DrawingContextP)
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


void __fastcall TDialogOpenDSK::OnDisplay(TDrawingContext* DrawingContextP)
{
  if (mDisplayedPath.IsEmpty() == true)
  {
    ClipPath(DrawingContextP);
  }

  mDialogDisplay(DrawingContextP);
}
//---------------------------------------------------------------------------

void __fastcall TDialogOpenDSK::OnDisplayD3D(TDrawingContext* DrawingContextP)
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

void __fastcall TDialogOpenDSK::OnDisplayGL(TDrawingContext* DrawingContextP)
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

void __fastcall TDialogOpenDSK::OnDisplayGDI(TDrawingContext* DrawingContextP)
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

