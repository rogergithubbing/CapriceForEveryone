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


#ifndef TInputActionPOVH
#define TInputActionPOVH

#include "TInputAction.h"

#define POV_NBBUTTONS             4
#define POV_INDEXBUTTON_UP        0
#define POV_INDEXBUTTON_DOWN      1
#define POV_INDEXBUTTON_LEFT      2
#define POV_INDEXBUTTON_RIGHT     3


class TInputActionPOV : public TInputAction
{
private:
  // Methods
  inline int GetNbButtons(void) { return POV_NBBUTTONS; };
  inline TInputKey* __fastcall GetButtonsKey(int Index) { return (TInputKey*)Items[Index]; };
  void __fastcall SetButtonsKey(int Index, TInputKey* ButtonKeyP);
  inline TInputKey* __fastcall GetButtonUpKey(void) { return (TInputKey*)Items[POV_INDEXBUTTON_UP]; };
  void __fastcall SetButtonUpKey(TInputKey* ButtonKeyP);
  inline TInputKey* __fastcall GetButtonDownKey(void) { return (TInputKey*)Items[POV_INDEXBUTTON_DOWN]; };
  void __fastcall SetButtonDownKey(TInputKey* ButtonKeyP);
  inline TInputKey* __fastcall GetButtonLeftKey(void) { return (TInputKey*)Items[POV_INDEXBUTTON_LEFT]; };
  void __fastcall SetButtonLeftKey(TInputKey* ButtonKeyP);
  inline TInputKey* __fastcall GetButtonRightKey(void) { return (TInputKey*)Items[POV_INDEXBUTTON_RIGHT]; };
  void __fastcall SetButtonRightKey(TInputKey* ButtonKeyP);

protected:

public:
  // Properties
  __property int NbButtons = { read=GetNbButtons };
  __property TInputKey* Buttons[int Index] = { read=GetButtonsKey, write=SetButtonsKey };
  __property TInputKey* ButtonUpKey = { read=GetButtonUpKey, write=SetButtonUpKey };
  __property TInputKey* ButtonDownKey = { read=GetButtonDownKey, write=SetButtonDownKey };
  __property TInputKey* ButtonLeftKey = { read=GetButtonLeftKey, write=SetButtonLeftKey };
  __property TInputKey* ButtonRightKey = { read=GetButtonRightKey, write=SetButtonRightKey };
  // Public API
  __fastcall TInputActionPOV();
  virtual __fastcall ~TInputActionPOV();

  void __fastcall OnChange(long NewValue);
};

//---------------------------------------------------------------------------
#endif
