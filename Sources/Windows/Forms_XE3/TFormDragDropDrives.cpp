/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2018 by Frédéric Coste

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
#include "TFormMain.h"

#include "TFormDragDropDrives.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDragDropDrives *FormDragDropDrives;
//---------------------------------------------------------------------------
__fastcall TFormDragDropDrives::TFormDragDropDrives(TComponent* Owner)
  : TForm(Owner)
{
  // Apply Theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;

  mFontRatio = ClientHeight / 12;
  PanelDriveA->Font->Color = clBlack;
  PanelDriveA->Font->Size = 12;
  PanelDriveA->Color = clBtnShadow;
  PanelDriveB->Font->Color = clBlack;
  PanelDriveB->Font->Size = 12;
  PanelDriveB->Color = clBtnShadow;
}
//---------------------------------------------------------------------------


void __fastcall TFormDragDropDrives::PanelDriveAMouseEnter(TObject *Sender)
{
  PanelDriveA->Font->Color = clRed;
  PanelDriveA->Color = clBtnFace;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDragDropDrives::PanelDriveAMouseLeave(TObject *Sender)
{
  PanelDriveA->Font->Color = clBlack;
  PanelDriveA->Color = clBtnShadow;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDragDropDrives::PanelDriveBMouseEnter(TObject *Sender)
{
  PanelDriveB->Font->Color = clRed;
  PanelDriveB->Color = clBtnFace;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDragDropDrives::PanelDriveBMouseLeave(TObject *Sender)
{
  PanelDriveB->Font->Color = clBlack;
  PanelDriveB->Color = clBtnShadow;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDragDropDrives::PanelDriveAClick(TObject *Sender)
{
  ModalResult = mrYes;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDragDropDrives::PanelDriveBClick(TObject *Sender)
{
  ModalResult = mrNo;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDragDropDrives::FormShow(TObject *Sender)
{
  Width = ParentWidth;
  Height = ParentHeight;
  PanelDriveA->Width = ClientWidth / 2;
  PanelDriveB->Width = ClientWidth / 2;

  PanelDriveA->Font->Size = ClientHeight / mFontRatio;
  PanelDriveB->Font->Size = ClientHeight / mFontRatio;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDragDropDrives::FormKeyPress(TObject *Sender, System::WideChar &Key)

{
  if (Key == VK_ESCAPE)
  {
    ModalResult = mrCancel;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

