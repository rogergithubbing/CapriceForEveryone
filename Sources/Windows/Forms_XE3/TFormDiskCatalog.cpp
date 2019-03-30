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

#include "TFormDiskCatalog.h"
#include "..\Emulator\TEmulator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDiskCatalog *FormDiskCatalog;
//---------------------------------------------------------------------------
__fastcall TFormDiskCatalog::TFormDiskCatalog(TComponent* Owner)
  : TForm(Owner)
{
  mNbMinEntries = ListBoxCatalogEntries->Items->Count;
}
//---------------------------------------------------------------------------
void __fastcall TFormDiskCatalog::FormShow(TObject *Sender)
{
  mRUNRequested = false;
  mLOADRequested = false;
  mCPMRequested = false;

  if (Tag == 0)
  {
    Close();
    return;
  }

  TEmulator* EmulatorP = (TEmulator*)Tag;

  ListBoxCatalogEntries->Clear();
  EmulatorP->ReadDiskCatalog(DriveA);
  AnsiString AutoStartCommand = EmulatorP->DetectAutoStart(DriveA);
  AnsiString AutoStartEntry = "";
  for (int Loop=0; Loop < EmulatorP->NbCatalogEntries; Loop++)
  {
    ListBoxCatalogEntries->Items->Add(EmulatorP->CatalogEntry[Loop]);

    // Autostart entry ?
    if (AutoStartCommand.Pos(EmulatorP->CatalogEntry[Loop]) > 0)
    {
      AutoStartEntry = EmulatorP->CatalogEntry[Loop];
    }
  }

  // Select Autostart entry
  if (AutoStartEntry.Length() > 0)
  {
    ListBoxCatalogEntries->ItemIndex = ListBoxCatalogEntries->Items->IndexOf(AutoStartEntry);
  }

  // Adapt list size
  int NbEntries = ListBoxCatalogEntries->Items->Count;
  if (NbEntries > 16) NbEntries = 16;
  if (NbEntries < mNbMinEntries) NbEntries = mNbMinEntries;
  //int NbVisibleEntries = ListBoxCatalogEntries->Height / ListBoxCatalogEntries->ItemHeight;
  //Height += (NbEntries - NbVisibleEntries) * ListBoxCatalogEntries->ItemHeight;
  ClientHeight = NbEntries * ListBoxCatalogEntries->ItemHeight;

  if (ListBoxCatalogEntries->Items->Count)
  {
    if (ListBoxCatalogEntries->ItemIndex == -1)
    {
      ListBoxCatalogEntries->ItemIndex = 0;
    }

    ButtonRUN->Enabled = true;
    ButtonRUN->Default = true;
    ButtonLOAD->Enabled = true;
    ButtonCPM->Default = false;

    ActiveControl = ListBoxCatalogEntries;
  }
  else
  {
    ButtonRUN->Enabled = false;
    ButtonRUN->Default = false;
    ButtonLOAD->Enabled = false;
    ButtonCPM->Default = true;

    ActiveControl = ButtonCPM;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDiskCatalog::GetSelectedFilename(void)
{
  return ListBoxCatalogEntries->Items->Strings[ListBoxCatalogEntries->ItemIndex];
}
//---------------------------------------------------------------------------


void __fastcall TFormDiskCatalog::ButtonRUNClick(TObject *Sender)
{
	mRUNRequested = true;
  ModalResult = mrOk;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDiskCatalog::ButtonLOADClick(TObject *Sender)
{
	mLOADRequested = true;
  ModalResult = mrOk;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDiskCatalog::ButtonCPMClick(TObject *Sender)
{
  mCPMRequested = true;
  ModalResult = mrOk;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDiskCatalog::ListBoxCatalogEntriesDblClick(
      TObject *Sender)
{
  mRUNRequested = true;
  ModalResult = mrOk;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormDiskCatalog::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_F1)
  {
    ButtonRUNClick(Sender);
  }
  else if (Key == VK_F2)
  {
    ButtonLOADClick(Sender);
  }
  else if (Key == VK_F3)
  {
    ButtonCPMClick(Sender);
  }

  NOT_USED(Shift);
}
//---------------------------------------------------------------------------

