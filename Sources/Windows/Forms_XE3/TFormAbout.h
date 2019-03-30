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


#ifndef TFormAboutH
#define TFormAboutH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdCtrls.hpp>
#include "..\Audio\TAudioContext.h"
#include "..\Drawing\TDrawingContext.h"
#include "..\Inputs\TInputContext.h"
#include "..\Settings\TSettings.h"

//---------------------------------------------------------------------------
class TFormAbout : public TForm
{
__published:	// Composants gérés par l'EDI
  TButton *OK;
  TLabel *LabelTitre;
  TImage *Image1;
  TMemo *MemoAuthor;
  TPopupMenu *PopupMenuInfo;
  TMenuItem *Copytoclipboard1;
  TPageControl *PageControl;
  TTabSheet *TabSheetVersions;
  TMemo *MemoVersions;
  TTabSheet *TabSheetDebugInfo;
  TMemo *MemoDebugInfo;
  TTabSheet *TabSheetCredits;
  TMemo *MemoCredits;
  TTabSheet *TabSheetHistory;
  TMemo *MemoHistory;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall Copytoclipboard1Click(TObject *Sender);
  void __fastcall MemoContextPopup(TObject *Sender,
          TPoint &MousePos, bool &Handled);
private:	// Déclarations utilisateur
  //
  // Members
  //
  TAudioContext* mAudioContextP;
  TDrawingContext* mDrawingContextP;
  TInputContext* mInputContextP;
  TSettings* mSettingsP;
  //
  // Methods
  //
  inline void __fastcall SetAudioContext(TAudioContext* Value) { mAudioContextP = Value; };
  inline void __fastcall SetDrawingContext(TDrawingContext* Value) { mDrawingContextP = Value; };
  inline void __fastcall SetInputContext(TInputContext* Value) { mInputContextP = Value; };
  inline void __fastcall SetSettings(TSettings* Value) { mSettingsP = Value; };
public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TAudioContext* AudioContext = { write=SetAudioContext };
  __property TDrawingContext* DrawingContext = { write=SetDrawingContext };
  __property TInputContext* InputContext = { write=SetInputContext };
  __property TSettings* Settings = { write=SetSettings };
  //
  // APIs
  //
  __fastcall TFormAbout(TComponent* Owner);
  static AnsiString __fastcall GetFileVersion(String Filename);
  static AnsiString __fastcall GetFileFullVersion(String Filename);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAbout *FormAbout;
//---------------------------------------------------------------------------
#endif
