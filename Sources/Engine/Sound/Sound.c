/*
	Caprice32 - Amstrad CPC Emulator
	(c) Copyright 1997-2005 Ulrich Doewich

	CaPriCe for Palm OS - Amstrad CPC 464/664/6128 emulator for Palm devices
	Copyright (C) 2006-2011 by Frédéric Coste
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

//roger
#define M_LOG_2			3.01029995664e-01

#include "..\Native_CPC.h"
//#include "..\..\Common\Math.h"
#include "Math.h"

//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#undef PATCH_1
#define PATCH_1

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


#ifdef __cplusplus
extern "C" {
#endif


// Amplitude table (c)Hacker KAY
static const tUShort Amplitudes_AY[16] =
{
   0, 836, 1212, 1773, 2619, 3875, 5397, 8823,
   10392, 16706, 23339, 29292, 36969, 46421, 55195, 65535
};


#if defined(__PALMOS__)
tVoid Sound_Calculate_Level_Tables(tVoid* param)
#elif defined (__WIN32__)
tVoid Sound_Calculate_Level_Tables(tPSG* PSG, tUChar AudioStereo, tUChar Audio16Bits)
#endif
/***********************************************************************
 *
 *  Sound_Calculate_Level_Tables
 *
 ***********************************************************************/
{
tDouble k;
tDouble Log2 = M_LOG_2;
#if defined(__PALMOS__)
tNativeCPC* NativeCPC;
tPSG* PSG;
#endif /* __PALMOS__ */
tLong i;
tLong b;
tLong l;
tLong r;
tLong Index_A;
tLong Index_B;
tLong Index_C;
tLong Index_AR;
tLong Index_BR;
tLong Index_CR;
tLong* Level_AL;
tLong* Level_AR;
tLong* Level_BL;
tLong* Level_BR;
tLong* Level_CL;
tLong* Level_CR;
tLong* Level_PP;
tLong* Level_PLAYCITY;

  Index_A  = AUDIO_INDEX_AL;
  Index_B  = AUDIO_INDEX_BL;
  Index_C  = AUDIO_INDEX_CL;
  Index_AR = AUDIO_INDEX_AR;
  Index_BR = AUDIO_INDEX_BR;
  Index_CR = AUDIO_INDEX_CR;

#if defined(__PALMOS__)
  NativeCPC = (tNativeCPC*)param;
  PSG = NativeCPC->PSG;
#endif /* __PALMOS__ */

  Level_AR = PSG->Level_AR;
  Level_AL = PSG->Level_AL;
  Level_BR = PSG->Level_BR;
  Level_BL = PSG->Level_BL;
  Level_CR = PSG->Level_CR;
  Level_CL = PSG->Level_CL;
  Level_PP = PSG->Level_PP;
  Level_PLAYCITY = PSG->Level_PLAYCITY;

  l = Index_A + Index_B + Index_C;
  r = Index_AR + Index_BR + Index_CR;

  if (AudioStereo)
  {
    if (l < r)
    {
      l = r;
    }
  }
  else
  {
#ifndef PATCH_1
    l += r;
    Index_A += Index_AR;
    Index_B += Index_BR;
    Index_C += Index_CR;
#else /* !PATCH_1 */
    Index_A = AUDIO_INDEX_MONO;
    Index_B = AUDIO_INDEX_MONO;
    Index_C = AUDIO_INDEX_MONO;
    l = Index_A + Index_B + Index_C;
#endif /* PATCH_1 */
  }

  if (Audio16Bits)
  {
    r = 32767;
  }
  else
  {
    r = 127;
  }

  if (l == 0) l++; // Avoid division by 0
  l = 255 * r / l;

  for (i = 0; i < 16; i++)
  {
    b = (tLong)floor(Index_A / 255.0 * Amplitudes_AY[i]);
    b = (tLong)floor(b / 65535.0 * l);
    Level_AL[i * 2] = b;
    Level_AL[(i * 2) + 1] = b;
    b = (tLong)floor(Index_AR / 255.0 * Amplitudes_AY[i]);
    b = (tLong)floor(b / 65535.0 * l);
    Level_AR[i * 2] = b;
    Level_AR[(i * 2) + 1] = b;
    b = (tLong)floor(Index_B / 255.0 * Amplitudes_AY[i]);
    b = (tLong)floor(b / 65535.0 * l);
    Level_BL[i * 2] = b;
    Level_BL[(i * 2) + 1] = b;
    b = (tLong)floor(Index_BR / 255.0 * Amplitudes_AY[i]);
    b = (tLong)floor(b / 65535.0 * l);
    Level_BR[i * 2] = b;
    Level_BR[(i * 2) + 1] = b;
    b = (tLong)floor(Index_C / 255.0 * Amplitudes_AY[i]);
    b = (tLong)floor(b / 65535.0 * l);
    Level_CL[i * 2] = b;
    Level_CL[(i * 2) + 1] = b;
    b = (tLong)floor(Index_CR / 255.0 * Amplitudes_AY[i]);
    b = (tLong)floor(b / 65535.0 * l);
    Level_CR[i * 2] = b;
    Level_CR[(i * 2) + 1] = b;
  }

  k = exp(AUDIO_SPEAKER_VOLUME * Log2 / AUDIO_SPEAKER_PREAMP_MAX) - 1;

  for (i = 0; i < 32; i++)
  {
    Level_AL[i] = (tLong)floor(Level_AL[i] * k);
    Level_AR[i] = (tLong)floor(Level_AR[i] * k);
    Level_BL[i] = (tLong)floor(Level_BL[i] * k);
    Level_BR[i] = (tLong)floor(Level_BR[i] * k);
    Level_CL[i] = (tLong)floor(Level_CL[i] * k);
    Level_CR[i] = (tLong)floor(Level_CR[i] * k);
  }

  if (Audio16Bits)
  {
    PSG->LevelTape = (tLong)floor((TAPE_VOLUME * 128) * k);
  }
  else
  {
    PSG->LevelTape = (tLong)floor((TAPE_VOLUME / 2) * k);
  }

  for (i = 0, b = 255; i < 256; i++) // calculate the 256 levels of the Digiblaster/Soundplayer
  {
    Level_PP[i] = (tLong)(-floor(((b << 8) / 65535.0 * l) * k));
    b--;
  }

  //
  // PLAYCITY Levels
  //
#ifndef PATCH_1
  l += r;
  Index_A += Index_AR;
  Index_B += Index_BR;
  Index_C += Index_CR;
#else /* !PATCH_1 */
  Index_A = AUDIO_INDEX_MONO;
  Index_B = AUDIO_INDEX_MONO;
  Index_C = AUDIO_INDEX_MONO;
  l = Index_A + Index_B + Index_C;
#endif /* PATCH_1 */

  if (Audio16Bits)
  {
    r = 32767;
  }
  else
  {
    r = 127;
  }

  if (l == 0) l++; // Avoid division by 0
  l = 255 * r / l;

  for (i = 0; i < 16; i++)
  {
    b = (tLong)floor(AUDIO_INDEX_MONO / 255.0 * Amplitudes_AY[i]);
    b = (tLong)floor(b / 65535.0 * l);
    Level_PLAYCITY[i * 2] = b;
    Level_PLAYCITY[(i * 2) + 1] = b;
  }

  k = exp(AUDIO_PLAYCITY_VOLUME * Log2 / AUDIO_PLAYCITY_PREAMP_MAX) - 1;

  for (i = 0; i < 32; i++)
  {
	Level_PLAYCITY[i] = (tLong)floor(Level_PLAYCITY[i] * k);
  }
}
/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

