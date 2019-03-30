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
#include "glew.h"
#include "TDialogInfo.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TDialogInfo::TDialogInfo(TDrawingContext* DrawingContextP, bool ShowInfo) : TDialogForm()
//
// Constructor
//
{
  // Prepare size
  mLeft = 0.0f;
  mRight = 1.0f;
  mTop = 0.0f;
  mBottom = 1.0f;

  mFirstLabelTop = 0.08f;
  mLabelStep = 0.04f;

  // Create components
  mLabelPauseMode = new TDialogInfoLabel(DrawingContextP);
  mLabelPauseMode->Top = 0.00f;
  mLabelPauseMode->Left = 0.01f;
  mLabelPauseMode->Color = clRed; // (TColor)0x000000ff;
  mLabelMachineType = new TDialogInfoLabel(DrawingContextP);
  mLabelMachineType->Left = 0.01f;
  mLabelMachineType->Color = clRed; // (TColor)0x000000ff;
  mLabelDriveAFilename = new TDialogInfoLabel(DrawingContextP);
  mLabelDriveAFilename->Left = 0.01f;
  mLabelDriveAFilename->Color = clRed; // (TColor)0x000000ff;
  mLabelDriveBFilename = new TDialogInfoLabel(DrawingContextP);
  mLabelDriveBFilename->Left = 0.01f;
  mLabelDriveBFilename->Color = clRed; // (TColor)0x000000ff;
  mLabelTapeFilename = new TDialogInfoLabel(DrawingContextP);
  mLabelTapeFilename->Left = 0.01f;
  mLabelTapeFilename->Color = clRed; // (TColor)0x000000ff;
  mLabelSnapshotFilename = new TDialogInfoLabel(DrawingContextP);
  mLabelSnapshotFilename->Left = 0.01f;
  mLabelSnapshotFilename->Color = clRed; // (TColor)0x000000ff;
  mLabelControllerProfile = new TDialogInfoLabel(DrawingContextP);
  mLabelControllerProfile->Left = 0.01f;
  mLabelControllerProfile->Color = clRed; // (TColor)0x000000ff;
  mLabelDisplaySize = new TDialogInfoLabel(DrawingContextP);
  mLabelDisplaySize->Left = 0.01f;
  mLabelDisplaySize->Color = clRed; // (TColor)0x000000ff;

  UpdateLabelsTop();
  mResizeRequested = false;
  SetFullscreen(false);
  mShowInfo = ShowInfo;

  // Add components in order of display (first in the list, first displayed)
  AddComponent(mLabelPauseMode);
  AddComponent(mLabelMachineType);
  AddComponent(mLabelDisplaySize);
  AddComponent(mLabelDriveAFilename);
  AddComponent(mLabelDriveBFilename);
  AddComponent(mLabelTapeFilename);
  AddComponent(mLabelSnapshotFilename);
  AddComponent(mLabelControllerProfile);

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


 __fastcall TDialogInfo::~TDialogInfo()
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


//
// Write access
//
void __fastcall TDialogInfo::SetMachineType(AnsiString Value)
{
  if (mShowInfo == true)
  {
    mLabelMachineType->Text = Value;
    mResizeRequested = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogInfo::SetDriveAFilename(AnsiString Value)
{
  if (mShowInfo == true)
  {
    mLabelDriveAFilename->Text = Value;
    mResizeRequested = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogInfo::SetDriveBFilename(AnsiString Value)
{
  if (mShowInfo == true)
  {
    mLabelDriveBFilename->Text = Value;
    mResizeRequested = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogInfo::SetTapeFilename(AnsiString Value)
{
  if (mShowInfo == true)
  {
    mLabelTapeFilename->Text = Value;
    mResizeRequested = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogInfo::SetSnapshot(AnsiString Value)
{
  if (mShowInfo == true)
  {
    mLabelSnapshotFilename->Text = Value;
    mResizeRequested = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogInfo::SetControllerProfile(AnsiString Value)
{
  if (mShowInfo == true)
  {
    mLabelControllerProfile->Text = Value;
    mResizeRequested = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDialogInfo::SetFullscreen(bool Value)
{
  if (Value == false)
  {
    mLabelPauseMode->Text = "PAUSE (Press F1 to resume, Alt-F4 to quit)";
  }
  else
  {
    mLabelPauseMode->Text = "PAUSE (Press F1 to resume, F11 for windowed, Alt-F4 to quit)";
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogInfo::UpdateLabelsTop(void)
{
  float CurrentTop = mFirstLabelTop;
  
  // Machine
  if (mLabelMachineType->Text.IsEmpty() == false)
  {
    mLabelMachineType->Top = CurrentTop;
    CurrentTop += mLabelStep;
  }
  // Drive A Filename
  if (mLabelDriveAFilename->Text.IsEmpty() == false)
  {
    mLabelDriveAFilename->Top = CurrentTop;
    CurrentTop += mLabelStep;
  }
  // Drive B Filename
  if (mLabelDriveBFilename->Text.IsEmpty() == false)
  {
    mLabelDriveBFilename->Top = CurrentTop;
    CurrentTop += mLabelStep;
  }
  // Tape Filename
  if (mLabelTapeFilename->Text.IsEmpty() == false)
  {
    mLabelTapeFilename->Top = CurrentTop;
    CurrentTop += mLabelStep;
  }
  // Snapshot Filename
  if (mLabelSnapshotFilename->Text.IsEmpty() == false)
  {
    mLabelSnapshotFilename->Top = CurrentTop;
    CurrentTop += mLabelStep;
  }
  // Controller Profile
  if (mLabelControllerProfile->Text.IsEmpty() == false)
  {
    mLabelControllerProfile->Top = CurrentTop;
    CurrentTop += mLabelStep;
  }
  // Display size
  if (mLabelDisplaySize->Text.IsEmpty() == false)
  {
    mLabelDisplaySize->Top = CurrentTop;
    //CurrentTop += mLabelStep;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogInfo::OnResize(TDrawingContext* DrawingContextP)
{
  TDialogForm::OnResize(DrawingContextP);

  if (mShowInfo == true)
  {
    // Update display size label
    AnsiString DisplayLabel = "Display=";
    DisplayLabel += AnsiString(DrawingContextP->DisplayWidth);
    DisplayLabel += "x";
    DisplayLabel += AnsiString(DrawingContextP->DisplayHeight);
    mLabelDisplaySize->Text = DisplayLabel;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDialogInfo::OnDisplay(TDrawingContext* DrawingContextP)
{
  // Resize requested ?
  if (mResizeRequested == true)
  {
    mResizeRequested = false;
    UpdateLabelsTop();
    OnResize(DrawingContextP);
  }

  mDialogDisplay(DrawingContextP);
}
//---------------------------------------------------------------------------

void __fastcall TDialogInfo::OnDisplayD3D(TDrawingContext* DrawingContextP)
{
  // Display all components
  TDialogForm::OnDisplay(DrawingContextP);
}
//---------------------------------------------------------------------------

void __fastcall TDialogInfo::OnDisplayGL(TDrawingContext* DrawingContextP)
{
  // Push emulator texture palette parameters
  glPushAttrib(GL_CURRENT_BIT);

  // Text area clipping
  glEnable(GL_SCISSOR_TEST);
  glScissor(mScreenLeft+2,
            DrawingContextP->EmulatorScreenBottom - mScreenBottom + 2,
            mScreenRight - mScreenLeft - 4,
            mScreenBottom - mScreenTop - 4);

  // Display all components
  TDialogForm::OnDisplay(DrawingContextP);

  glDisable(GL_SCISSOR_TEST);

  glPopAttrib();
}
//---------------------------------------------------------------------------

void __fastcall TDialogInfo::OnDisplayGDI(TDrawingContext* DrawingContextP)
{
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
                                      DrawingContextP->InfoFont);

  // Display all components
  TDialogForm::OnDisplay(DrawingContextP);

  SelectObject(DrawingContextP->OnscreenHDC,
               oldFont);

  // Restore previous clipping area
  SelectClipRgn(DrawingContextP->OnscreenHDC, restoreRegion);
  if (restoreRegion != NULL)
  {
    DeleteObject(restoreRegion);
  }
  DeleteObject(ClipRegion);
}
//---------------------------------------------------------------------------

