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


#ifndef TFormDragDropDrivesH
#define TFormDragDropDrivesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormDragDropDrives : public TForm
{
__published:	// Composants gérés par l'EDI
  TPanel *PanelDriveB;
  TPanel *PanelDriveA;
  void __fastcall PanelDriveBMouseEnter(TObject *Sender);
  void __fastcall PanelDriveBMouseLeave(TObject *Sender);
  void __fastcall PanelDriveAMouseEnter(TObject *Sender);
  void __fastcall PanelDriveAMouseLeave(TObject *Sender);
  void __fastcall PanelDriveAClick(TObject *Sender);
  void __fastcall PanelDriveBClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, System::WideChar &Key);

private:	// Déclarations utilisateur
  //
  // Members
  //
  int mParentWidth;
  int mParentHeight;
  double mFontRatio;
  //
  // Methods
  //
  inline int __fastcall GetParentWidth(void) { return mParentWidth; };
  inline void __fastcall SetParentWidth(int Value) { mParentWidth = Value; };
  inline int __fastcall GetParentHeight(void) { return mParentHeight; };
  inline void __fastcall SetParentHeight(int Value) { mParentHeight = Value; };

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property int ParentWidth = {read=GetParentWidth,write=SetParentWidth};
  __property int ParentHeight = {read=GetParentHeight,write=SetParentHeight};

  //
  // API
  //
  __fastcall TFormDragDropDrives(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDragDropDrives *FormDragDropDrives;
//---------------------------------------------------------------------------
#endif
