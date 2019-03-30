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


#ifndef TBaseTimerH
#define TBaseTimerH


//=================================
// Options
#undef USE_PRECISION_TIMER
//#define USE_PRECISION_TIMER
//=================================


#ifndef USE_PRECISION_TIMER
#define TBaseTime   LARGE_INTEGER
#else /* USE_PRECISION_TIMER */
#define TBaseTime   DWORD
#endif /* USE_PRECISION_TIMER */


class TBaseTimer
{
private:
  //
  // Members
  //
  TBaseTime mTimerFrequency;
  TBaseTime mNextCycle;
  TBaseTime mNextSecond;
  TBaseTime mNextIdle;
  TBaseTime mCycleDuration;
  TBaseTime mSecondDuration;
  int mPrecisionTimerID;
  DWORD mPrecisionTimerCounts;
  //
  // Methods
  //
  int __fastcall  GetPrecisionTimerResolution(int target);
  int __fastcall  StartPrecisionTimer(int Interval, int Resolution);
  void __fastcall StopPrecisionTimer(int TimerID);

public:
  //
  // Properties
  //
  //
  // API
  //
  __fastcall TBaseTimer(int CyclesPerSecond);
  virtual __fastcall ~TBaseTimer();

  void __fastcall Reset(TBaseTime* Time);
  bool __fastcall IsStarted(TBaseTime* Time);

  void __fastcall GetNow(TBaseTime* Time);
  int __fastcall  GetElapsedMS(TBaseTime* StartTime);
  int __fastcall  GetElapsedUS(TBaseTime* StartTime);

  void __fastcall SetNextOccurenceS(TBaseTime* Time, int Seconds);
  bool __fastcall IsNextOccurenceElapsed(TBaseTime* Time);

  void __fastcall StartCycleTimer(void);
  bool __fastcall IsCycleTimerElapsed(void);

  void __fastcall StartSecondTimer(void);
  bool __fastcall IsSecondTimerElapsed(void);

  void __fastcall UpdateNextIdleTimer(void);
  bool __fastcall IsNextIdleTimerElapsed(void);
};

//---------------------------------------------------------------------------
#endif

