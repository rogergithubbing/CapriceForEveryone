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
#include "TCheatActionEqual.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


__fastcall TCheatActionEqual::TCheatActionEqual(AnsiString Command) : TCheatAction()
//
// Constructor
//
{
  mBranchToLabel = false;

  int ParamPos = Command.Pos("=");
  int ParamLength = Command.Length() - ParamPos;
  int CommentPos = Command.Pos(";");

  if (CommentPos)
  {
    ParamLength = CommentPos - ParamPos - 1;
  }
  else
  {
    CommentPos = Command.Length();
  }

  if (ParamPos < CommentPos)
  {
    AnsiString Param = Command.SubString(ParamPos+1, ParamLength).Trim();
    int SeparatorPos = Param.Pos(",");

    // Address
    AnsiString StringAddress = Param.SubString(1, SeparatorPos-1).Trim();

    // Value
    Param = Param.SubString(SeparatorPos+1, Param.Length()).Trim();
    SeparatorPos = Param.Pos(",");
    AnsiString StringValue = Param.SubString(1, SeparatorPos-1).Trim();

    // Mask
    Param = Param.SubString(SeparatorPos+1, Param.Length()).Trim();
    SeparatorPos = Param.Pos(":");
    AnsiString StringMask = Param.SubString(1, SeparatorPos-1).Trim();

    // Branch label
    mBranchLabel = Param.SubString(SeparatorPos+1, Param.Length()).Trim();

    try
    {
      if (StringAddress.Pos("&") == 1)
      {
        mAddress = StrToInt("0x" + StringAddress.SubString(2, StringAddress.Length()).Trim());
      }
      else
      {
        mAddress = StringAddress.ToInt();
      }
    }
    catch(...)
    {
      mAddress = -1;
    }

    try
    {
      if (StringValue.Pos("&") == 1)
      {
        mExpectedValue = StrToInt("0x" + StringValue.SubString(2, StringValue.Length()).Trim());
      }
      else
      {
        mExpectedValue = StringValue.ToInt();
      }
    }
    catch(...)
    {
      mExpectedValue = -1;
    }

    try
    {
      if (StringMask.Pos("&") == 1)
      {
        mMask = StrToInt("0x" + StringMask.SubString(2, StringValue.Length()).Trim());
      }
      else
      {
        mMask = StringMask.ToInt();
      }
    }
    catch(...)
    {
      mMask = -1;
    }

    // Check validity
    if ((mAddress < 0x10000) && (mExpectedValue < 0x100) && (mMask < 0x100))
    {
      mValid = true;
    }
  }
}
//---------------------------------------------------------------------------


__fastcall TCheatActionEqual::~TCheatActionEqual(void)
//
// Destructor
//
{
}
//---------------------------------------------------------------------------


void __fastcall TCheatActionEqual::Execute(tNativeCPC* NativeCPC)
{
  //tUChar Value = Engine_ReadMem(NativeCPC, mAddress);
  tUChar Value = *(NativeCPC->pbRAM + mAddress);

  // read value is equal ?
  if ((Value & mMask) == mExpectedValue)
  {
    mBranchToLabel = true;
  }
}
//---------------------------------------------------------------------------


AnsiString __fastcall TCheatActionEqual::NextAction(void)
{
  return (mBranchToLabel == true ? mBranchLabel : (AnsiString)NULL);
}
//---------------------------------------------------------------------------


