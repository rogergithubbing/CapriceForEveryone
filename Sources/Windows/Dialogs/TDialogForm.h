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


#ifndef TDialogFormH
#define TDialogFormH

#ifdef _USE_OPENGL
#include "..\OpenGL\glew.h"
#include "..\OpenGL\wglew.h"
#include <gl\glu.h>
#endif /* _USE_OPENGL */

#include "..\Drawing\TDrawingContext.h"
#include "TDialogComponent.h"


typedef enum
{
  Cancel,
  Button1,
  Button2,
  Button3
} TDialogFormEndCause;


typedef void __fastcall (__closure *TDialogFormEndEvent)(TDialogFormEndCause EndCause);


class TDialogForm : public TList
{
private:
  // Members
  TDialogFormEndEvent mOnEndEvent;
  // Methods
  TDialogComponent* __fastcall GetComponent(int iIndex);
  inline TDialogFormEndEvent __fastcall GetEndEvent(void) { return mOnEndEvent; };
  inline void __fastcall SetEndEvent(TDialogFormEndEvent Event) { mOnEndEvent = Event; };

protected:
  // Members
  float mLeft;
  float mRight;
  float mTop;
  float mBottom;
  int mScreenLeft;
  int mScreenRight;
  int mScreenTop;
  int mScreenBottom;

  // Methods
  void AddComponent(TDialogComponent* NewComponent);

public:
  // Properties
  __property TDialogComponent* Components[int iIndex] = {read=GetComponent};
  __property TDialogFormEndEvent OnEndEvent = { read=GetEndEvent, write=SetEndEvent };

  // Public API
  __fastcall TDialogForm();
  virtual __fastcall ~TDialogForm();

  virtual bool __fastcall OnKeyDown(WORD &Key) { return false; };
  virtual void __fastcall OnMouseMove(int X, int Y);
  virtual void __fastcall OnMouseDown(TMouseButton Button, int X, int Y);
  virtual void __fastcall OnMouseClick(int X, int Y);
  virtual void __fastcall OnMouseDblClick(int X, int Y);
  virtual void __fastcall OnResize(TDrawingContext* DrawingContextP);
  virtual void __fastcall OnDisplay(TDrawingContext* DrawingContextP);
};

//---------------------------------------------------------------------------
#endif
