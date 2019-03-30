/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2017 by Frédéric Coste

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
#include "TFormZIPCatalog.h"
#include "..\..\..\Sources\Engine\Native_CPC.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormZIPCatalog *FormZIPCatalog;
//---------------------------------------------------------------------------
__fastcall TFormZIPCatalog::TFormZIPCatalog(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TFormZIPCatalog::Clear(void)
{
  ListBoxFiles->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TFormZIPCatalog::AddFile(String File)
{
  ListBoxFiles->Items->Add(File);
}
//---------------------------------------------------------------------------


void __fastcall TFormZIPCatalog::FormShow(TObject *Sender)
{
  // Select first file
#if (__BORLANDC__ == 0x551) // Borland C++ Builder 5 Update 1
  if (ListBoxFiles->Items->Count > 0)
#else
  if (ListBoxFiles->Count > 0)
#endif 
  {
    ListBoxFiles->ItemIndex = 0;
  }

  ListBoxFiles->SetFocus();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


String __fastcall TFormZIPCatalog::GetSelectedFile(void)
{
  return ListBoxFiles->Items->Strings[ListBoxFiles->ItemIndex];
}
//---------------------------------------------------------------------------


