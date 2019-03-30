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


#ifndef TFormDiskCatalogH
#define TFormDiskCatalogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDiskCatalog : public TForm
{
__published:	// Composants gérés par l'EDI
  TListBox *ListBoxCatalogEntries;
  TPanel *PanelButtons;
  TButton *ButtonCancel;
  TButton *ButtonLOAD;
  TButton *ButtonRUN;
  TButton *ButtonCPM;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ButtonRUNClick(TObject *Sender);
  void __fastcall ButtonLOADClick(TObject *Sender);
  void __fastcall ListBoxCatalogEntriesDblClick(TObject *Sender);
  void __fastcall ButtonCPMClick(TObject *Sender);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  
private:	// Déclarations utilisateur
  //
  // Members
  //
  bool mRUNRequested;
  bool mLOADRequested;
  bool mCPMRequested;
  int mNbMinEntries;

  //
  // Methods
  //
  AnsiString __fastcall GetSelectedFilename(void);
  inline bool __fastcall GetRUNRequested(void) { return mRUNRequested; };
  inline bool __fastcall GetLOADRequested(void) { return mLOADRequested; };
  inline bool __fastcall GetCPMRequested(void) { return mCPMRequested; };
  inline void __fastcall SetLOADVisible(bool Value) { ButtonLOAD->Visible = Value; };

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property AnsiString SelectedFilename = { read=GetSelectedFilename };
  __property bool RUNRequested = { read=GetRUNRequested };
  __property bool LOADRequested = { read=GetLOADRequested };
  __property bool CPMRequested = { read=GetCPMRequested };
  __property bool LOADVisible = { write = SetLOADVisible };

  //
  // API
  //
  __fastcall TFormDiskCatalog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDiskCatalog *FormDiskCatalog;
//---------------------------------------------------------------------------
#endif
