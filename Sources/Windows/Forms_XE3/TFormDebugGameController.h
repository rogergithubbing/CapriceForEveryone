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

#define _USE_DIRECTINPUT8

#ifndef TFormDebugGameControllerH
#define TFormDebugGameControllerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>

#ifdef _USE_DIRECTINPUT8
#include <initguid.h>
#include "..\DirectInput\dinput.h"
#include "..\DirectInput\dinputd.h"
#endif /* _USE_DIRECTINPUT8 */


//---------------------------------------------------------------------------
class TFormDebugGameController : public TForm
{
__published:	// Composants gérés par l'EDI
  TPageControl *PageControl1;
  TTabSheet *TabSheetButtons;
  TSpeedButton *SpeedButtonButton1;
  TSpeedButton *SpeedButtonButton2;
  TSpeedButton *SpeedButtonButton3;
  TSpeedButton *SpeedButtonButton4;
  TTabSheet *TabSheetPOV;
  TGroupBox *GroupBoxPOV1;
  TSpeedButton *SpeedButtonPOV1Up;
  TSpeedButton *SpeedButtonPOV1Right;
  TSpeedButton *SpeedButtonPOV1Down;
  TSpeedButton *SpeedButtonPOV1Left;
  TTabSheet *TabSheetAnalog;
  TTrackBar *TrackBarXAxis;
  TLabel *LabelXAxis;
  TLabel *LabelYAxis;
  TTrackBar *TrackBarYAxis;
  TTrackBar *TrackBarZAxis;
  TTrackBar *TrackBarRxAxis;
  TLabel *LabelZAxis;
  TLabel *LabelRxAxis;

private:	// Déclarations utilisateur
  inline int __fastcall GetNbButtons(void) { return 4; };
  inline int __fastcall GetNbPOV(void) { return 1; };
  inline bool __fastcall GetXAxisEnable(void) { return true; };
  inline bool __fastcall GetYAxisEnable(void) { return true; };
  inline bool __fastcall GetZAxisEnable(void) { return true; };
  inline bool __fastcall GetRxAxisEnable(void) { return true; };

public:		// Déclarations utilisateur
  __property int NbButtons = { read=GetNbButtons };
  __property int NbPOV = { read=GetNbPOV };
  __property bool XAxisEnable = { read=GetXAxisEnable };
  __property bool YAxisEnable = { read=GetYAxisEnable };
  __property bool ZAxisEnable = { read=GetZAxisEnable };
  __property bool RxAxisEnable = { read=GetRxAxisEnable };
  __fastcall TFormDebugGameController(TComponent* Owner);

  void __fastcall GetState(DIJOYSTATE* StateP);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormDebugGameController *FormDebugGameController;
//---------------------------------------------------------------------------
#endif
