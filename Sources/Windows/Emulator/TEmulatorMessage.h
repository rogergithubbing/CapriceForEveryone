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


#ifndef TEmulatorMessageH
#define TEmulatorMessageH

#include "..\Time\TBaseTimer.h"


class TEmulatorMessage
{
private:
  //
  // Members
  //
	String mMessage;
	TBaseTime mTillEvent;
	float mTransparency;

	//
	// Methods
	//
	inline String __fastcall GetMessage(void) { return mMessage; };
	inline float __fastcall GetTransparency(void) { return mTransparency; };

public:
	//
	// Properties
	//
	__property String Message = { read=GetMessage };
	__property float Transparency = { read=GetTransparency };

	//
	// API
	//
	__fastcall TEmulatorMessage(String Message, TBaseTime TillEvent);
	virtual __fastcall ~TEmulatorMessage();

	bool __fastcall IsDurationElapsed(TBaseTimer* BaseTimerP);
	bool __fastcall DecreaseTransparency(void);
};

//---------------------------------------------------------------------------
#endif

