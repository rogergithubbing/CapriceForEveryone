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


#ifndef TFormCustomSpeedH
#define TFormCustomSpeedH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormCustomSpeed : public TForm
{
__published:	// Composants gérés par l'EDI
  TButton *ButtonOK;
  TButton *ButtonCancel;
  TEdit *EditCustomSpeed;
  TLabel *LabelCustomSpeed;
  TLabel *LabelNote;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall EditCustomSpeedChange(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  int mCustomSpeed;

  //
  // Methods
  //
  void __fastcall SetSettings(TSettings* Value);
  inline int __fastcall GetCustomSpeed(void) { return mCustomSpeed; };
  inline void __fastcall SetCustomSpeed(int Value) { mCustomSpeed = Value; };

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property int CustomSpeed = { read=GetCustomSpeed, write=SetCustomSpeed };

  //
  // API
  //
  __fastcall TFormCustomSpeed(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCustomSpeed *FormCustomSpeed;
//---------------------------------------------------------------------------
#endif
