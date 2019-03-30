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
#include <stdlib.h>
#include <vcl.h>
#include <mmsystem.h>
#if (__BORLANDC__ == 0x651) || (__BORLANDC__ == 0x680)
#include <System.Math.hpp>
#endif /* __BORLANDC__ */
#pragma hdrstop
#include "..\..\Engine\types.h"
#include "TBaseTimer.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)



static void CALLBACK PrecisionTimerProc(unsigned int uID,
                                        unsigned int uMsg,
                                        DWORD dwUser,
                                        DWORD dw1,
                                        DWORD dw2);


__fastcall TBaseTimer::TBaseTimer(int CyclesPerSecond)
//
// Constructor
//
{
  // Prevent divison by 0
  if (CyclesPerSecond == 0) CyclesPerSecond = 1;

#ifndef USE_PRECISION_TIMER

  // Get timer frequency
  QueryPerformanceFrequency(&mTimerFrequency);

  // Init timers
  mNextCycle.QuadPart = 0;
  mNextSecond.QuadPart = 0;

  // Init durations
  mCycleDuration.QuadPart = mTimerFrequency.QuadPart / CyclesPerSecond;
  mSecondDuration.QuadPart = mTimerFrequency.QuadPart;

#else /* USE_PRECISION_TIMER */

  // Get timer frequency
  mPrecisionTimerCounts = 0;
  //int Resolution = GetPrecisionTimerResolution(1);
  //mPrecisionTimerID = StartPrecisionTimer(1, Resolution);
  mPrecisionTimerID = StartPrecisionTimer(1, 0);

  // Init timers
  mNextCycle = 0;
  mNextSecond = 0;

  // Init durations
  mCycleDuration = 1000 / CyclesPerSecond;
  mSecondDuration = 1000;

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


__fastcall TBaseTimer::~TBaseTimer()
//
// Destructor
//
{
#ifdef USE_PRECISION_TIMER

  // Stop Precision timer
  StopPrecisionTimer(mPrecisionTimerID);

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


void __fastcall TBaseTimer::Reset(TBaseTime* Time)
{
#ifndef USE_PRECISION_TIMER

  Time->QuadPart = 0;

#else /* USE_PRECISION_TIMER */

  *Time = 0;

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


bool __fastcall TBaseTimer::IsStarted(TBaseTime* Time)
{
#ifndef USE_PRECISION_TIMER

  return (Time->QuadPart == 0) ? false : true;

#else /* USE_PRECISION_TIMER */

  return (*Time == 0) ? false : true;

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


void __fastcall TBaseTimer::GetNow(TBaseTime* Time)
{
#ifndef USE_PRECISION_TIMER

  // Récupèrer le temps système
  QueryPerformanceCounter(Time);

#else /* USE_PRECISION_TIMER */

  *Time = mPrecisionTimerCounts;

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


int __fastcall TBaseTimer::GetElapsedMS(TBaseTime* StartTime)
{
#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;
LARGE_INTEGER Elapsed;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  Counter.QuadPart -= StartTime->QuadPart;
  Elapsed.QuadPart = 1000L * Counter.QuadPart / mTimerFrequency.QuadPart;

  return ((int)Elapsed.QuadPart);

#else /* USE_PRECISION_TIMER */

  return (mPrecisionTimerCounts - *StartTime);

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------
int __fastcall TBaseTimer::GetElapsedUS(TBaseTime* StartTime)
{
#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;
LARGE_INTEGER Elapsed;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  Counter.QuadPart -= StartTime->QuadPart;
  Elapsed.QuadPart = 1000000L * Counter.QuadPart / mTimerFrequency.QuadPart;

  return ((int)Elapsed.QuadPart);

#else /* USE_PRECISION_TIMER */

  return ((mPrecisionTimerCounts - *StartTime) * 1000);

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


void __fastcall TBaseTimer::SetNextOccurenceS(TBaseTime* Time, int Seconds)
{
#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  Time->QuadPart = Counter.QuadPart + (Seconds * mSecondDuration.QuadPart);

#else /* USE_PRECISION_TIMER */

  *Time = mPrecisionTimerCounts + (Seconds * mSecondDuration);

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


bool __fastcall TBaseTimer::IsNextOccurenceElapsed(TBaseTime* Time)
{
bool Elapsed = false;

#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  if (Counter.QuadPart >= Time->QuadPart)
  {
    Elapsed = true;
  }

#else /* USE_PRECISION_TIMER */

  if (mPrecisionTimerCounts >= *Time)
  {
    Elapsed = true;
  }

#endif /* USE_PRECISION_TIMER */

  return (Elapsed);
}
//---------------------------------------------------------------------------


void __fastcall TBaseTimer::StartCycleTimer(void)
{
#ifndef USE_PRECISION_TIMER

  mNextCycle.QuadPart = 0;

#else /* USE_PRECISION_TIMER */

  mNextCycle = 0;

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


bool __fastcall TBaseTimer::IsCycleTimerElapsed(void)
{
bool Elapsed = false;

#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  if (mNextCycle.QuadPart == 0)
  {
    Elapsed = true;
    // Update to next occurence
    mNextCycle.QuadPart = Counter.QuadPart + mCycleDuration.QuadPart;
  }
  else if (Counter.QuadPart >= mNextCycle.QuadPart)
  {
    Elapsed = true;
    // Update to next occurence
    mNextCycle.QuadPart += mCycleDuration.QuadPart;
  }

#else /* USE_PRECISION_TIMER */

  if (mNextCycle == 0)
  {
    mNextCycle = mPrecisionTimerCounts;
  }
  else if (mPrecisionTimerCounts >= mNextCycle)
  {
    Elapsed = true;
    // Update to next occurence
    mNextCycle += mCycleDuration;
  }

#endif /* USE_PRECISION_TIMER */
  
  return (Elapsed);
}
//---------------------------------------------------------------------------


void __fastcall TBaseTimer::StartSecondTimer(void)
{
#ifndef USE_PRECISION_TIMER

  mNextSecond.QuadPart = 0;

#else /* USE_PRECISION_TIMER */

  mNextSecond = 0;

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


bool __fastcall TBaseTimer::IsSecondTimerElapsed(void)
{
bool Elapsed = false;

#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  if (mNextSecond.QuadPart == 0)
  {
    mNextSecond.QuadPart = Counter.QuadPart + mSecondDuration.QuadPart;
  }
  else if (Counter.QuadPart >= mNextSecond.QuadPart)
  {
    Elapsed = true;
    // Update to next occurence
    mNextSecond.QuadPart += mSecondDuration.QuadPart;
  }

#else /* USE_PRECISION_TIMER */

  if (mNextSecond == 0)
  {
    mNextSecond = mPrecisionTimerCounts;
  }
  else if (mPrecisionTimerCounts >= mNextSecond)
  {
    Elapsed = true;
    // Update to next occurence
    mNextSecond += mSecondDuration;
  }

#endif /* USE_PRECISION_TIMER */
  
  return (Elapsed);
}
//---------------------------------------------------------------------------


int __fastcall TBaseTimer::GetPrecisionTimerResolution(int target)
{
TIMECAPS tc;

	timeGetDevCaps(&tc, sizeof(TIMECAPS));

//roger
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#if (__BORLANDC__ == 0x551)
	unsigned int uiTimerRes = min(max((int)tc.wPeriodMin, target), (int)tc.wPeriodMax);
#else /* __BORLANDC__ */
	unsigned int uiTimerRes = min(max((int)tc.wPeriodMin, target), (int)tc.wPeriodMax);
#endif /* __BORLANDC__ */

	timeBeginPeriod(uiTimerRes);
  
  return uiTimerRes;
}
//---------------------------------------------------------------------------


void __fastcall TBaseTimer::UpdateNextIdleTimer(void)
{
#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  mNextIdle.QuadPart = Counter.QuadPart + mCycleDuration.QuadPart;

#else /* USE_PRECISION_TIMER */

  mNextIdle = mPrecisionTimerCounts + mCycleDuration;

#endif /* USE_PRECISION_TIMER */
}
//---------------------------------------------------------------------------


bool __fastcall TBaseTimer::IsNextIdleTimerElapsed(void)
{
bool Elapsed = false;

#ifndef USE_PRECISION_TIMER

LARGE_INTEGER Counter;

  // Récupèrer le temps système
  QueryPerformanceCounter(&Counter);

  if (Counter.QuadPart >= mNextIdle.QuadPart)
  {
    Elapsed = true;
  }

#else /* USE_PRECISION_TIMER */

  if (mNextIdle == 0)
  {
    mNextIdle = mPrecisionTimerCounts;
  }
  else if (mPrecisionTimerCounts >= mNextIdle)
  {
    Elapsed = true;
  }

#endif /* USE_PRECISION_TIMER */
  
  return (Elapsed);
}
//---------------------------------------------------------------------------


int __fastcall TBaseTimer::StartPrecisionTimer(int Interval, int Resolution)
{
  return timeSetEvent(Interval,
                      Resolution,
                      PrecisionTimerProc,
                      (DWORD)&mPrecisionTimerCounts,
                      TIME_PERIODIC);
}
//---------------------------------------------------------------------------


void __fastcall TBaseTimer::StopPrecisionTimer(int TimerID)
{
  timeKillEvent(TimerID);
}
//---------------------------------------------------------------------------


static void CALLBACK PrecisionTimerProc(unsigned int uID,
																				unsigned int uMsg,
                                        DWORD dwUser,
																				DWORD dw1,
																				DWORD dw2)
{
  DWORD* Counter = (DWORD*)dwUser;

  // Increment milliseconds
  (*Counter)++;

	NOT_USED(uID);
	NOT_USED(uMsg);
	NOT_USED(dw1);
	NOT_USED(dw2);
}
//---------------------------------------------------------------------------

