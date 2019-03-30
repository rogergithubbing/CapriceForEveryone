/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2015 by Frédéric Coste

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


#ifndef TFormDebugBASICColorsH
#define TFormDebugBASICColorsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Settings\TSettings.h"
//---------------------------------------------------------------------------
class TFormDebugBASICColors : public TForm
{
__published:	// Composants gérés par l'EDI
  TPanel *PanelSelect0;
  TLabel *LabelColor0;
  TPanel *PanelColor0;
  TPanel *PanelName0;
  TPanel *PanelSelect9;
  TLabel *LabelColor9;
  TPanel *PanelColor9;
  TPanel *PanelName9;
  TPanel *PanelSelect1;
  TLabel *LabelColor1;
  TPanel *PanelColor1;
  TPanel *PanelName1;
  TPanel *PanelSelect18;
  TLabel *LabelColor18;
  TPanel *PanelColor18;
  TPanel *PanelName18;
  TPanel *PanelSelect2;
  TLabel *Label1;
  TPanel *PanelColor2;
  TPanel *PanelName2;
  TPanel *PanelSelect3;
  TLabel *Label2;
  TPanel *PanelColor3;
  TPanel *PanelName3;
  TPanel *PanelSelect4;
  TLabel *Label3;
  TPanel *PanelColor4;
  TPanel *PanelName4;
  TPanel *PanelSelect5;
  TLabel *Label4;
  TPanel *PanelColor5;
  TPanel *PanelName5;
  TPanel *PanelSelect6;
  TLabel *Label5;
  TPanel *PanelColor6;
  TPanel *PanelName6;
  TPanel *PanelSelect7;
  TLabel *Label6;
  TPanel *PanelColor7;
  TPanel *PanelName7;
  TPanel *PanelSelect8;
  TLabel *Label7;
  TPanel *PanelColor8;
  TPanel *PanelName8;
  TPanel *PanelSelect10;
  TLabel *LabelColor10;
  TPanel *PanelColor10;
  TPanel *PanelName10;
  TPanel *PanelSelect11;
  TLabel *LabelColor11;
  TPanel *PanelColor11;
  TPanel *PanelName11;
  TPanel *PanelSelect12;
  TLabel *LabelColor12;
  TPanel *PanelColor12;
  TPanel *PanelName12;
  TPanel *PanelSelect13;
  TLabel *LabelColor13;
  TPanel *PanelColor13;
  TPanel *PanelName13;
  TPanel *PanelSelect14;
  TLabel *LabelColor14;
  TPanel *PanelColor14;
  TPanel *PanelName14;
  TPanel *PanelSelect15;
  TLabel *LabelColor15;
  TPanel *PanelColor15;
  TPanel *PanelName15;
  TPanel *PanelSelect16;
  TLabel *LabelColor16;
  TPanel *PanelColor16;
  TPanel *PanelName16;
  TPanel *PanelSelect17;
  TLabel *LabelColor17;
  TPanel *PanelColor17;
  TPanel *PanelName17;
  TPanel *PanelSelect19;
  TLabel *LabelColor19;
  TPanel *PanelColor19;
  TPanel *PanelName19;
  TPanel *PanelSelect20;
  TLabel *LabelColor20;
  TPanel *PanelColor20;
  TPanel *PanelName20;
  TPanel *PanelSelect21;
  TLabel *LabelColor21;
  TPanel *PanelColor21;
  TPanel *PanelName21;
  TPanel *PanelSelect22;
  TLabel *LabelColor22;
  TPanel *PanelColor22;
  TPanel *PanelName22;
  TPanel *PanelSelect23;
  TLabel *LabelColor23;
  TPanel *PanelColor23;
  TPanel *PanelName23;
  TPanel *PanelSelect24;
  TLabel *LabelColor24;
  TPanel *PanelColor24;
  TPanel *PanelName24;
  TPanel *PanelSelect25;
  TLabel *LabelColor25;
  TPanel *PanelColor25;
  TPanel *PanelName25;
  TPanel *PanelSelect26;
  TLabel *LabelColor26;
  TPanel *PanelColor26;
  TPanel *PanelName26;
  TButton *ButtonCancel;
  void __fastcall PanelSelectClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
private:	// Déclarations utilisateur
  //
  // Members
  //
  TSettings* mSettingsP;
  int mColor;
  TPanel* mPanelSelect[27];

  //
  // Methods
  //
  inline void SetSettings(TSettings* Value) { mSettingsP = Value; };
  inline int __fastcall GetColor(void) { return mColor; };
  void __fastcall SetColor(int Value);
  
public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* Settings = { write=SetSettings };
  __property int SelectedColor = { read=GetColor, write=SetColor };

  //
  // API
  //
  __fastcall TFormDebugBASICColors(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugBASICColors *FormDebugBASICColors;
//---------------------------------------------------------------------------
#endif
