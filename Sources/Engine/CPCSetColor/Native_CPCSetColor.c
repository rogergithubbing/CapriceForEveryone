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

#if defined(__PALMOS__)
#include <PceNativeCall.h>
#include <ByteOrderUtils.h>
#endif /* __PALMOS__ */


#include "..\Native_CPC.h"


/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


#if defined(__PALMOS__)

// Copied from PalmTypes.h
typedef tUShort       Err;

// Copied from Window.h
typedef tUChar IndexedColorType;      // 1, 2, 4, or 8 bit index

#define winPaletteSet         1

// copied from CoreTraps.h
#define sysTrapWinRGBToIndex          0xA39E

#endif /* __PALMOS__ */


#if defined(__PALMOS__)
// Copied from Bitmap.h
typedef struct
{
  tUChar           index;       // index of color or best match to cur CLUT or unused.
  tUChar           r;           // amount of red, 0->255
  tUChar           g;           // amount of green, 0->255
  tUChar           b;           // amount of blue, 0->255
} RGBColorType;
#endif /* __PALMOS__ */


#include "..\Native_CPC.h"


#if defined(__PALMOS__)
tULong PNOMain(const tVoid*,
               tVoid*,
               Call68KFuncType*);
#endif /* __PALMOS__ */


#if defined(__GNUC__)
// This seemingly bogus function satisfies the Windows LoadLibrary() call,
// (and must be the first function in our PNO .dll).
int /*WINAPI*/ my_bogus_cygwin_dll_entry(unsigned long /*HANDLE*/ h,
                                         unsigned long /*DWORD*/ reason,
                                         void* p)
{
  return 1;
}
#endif /* __GNUC__ */


typedef struct
{
  tULong routineP;
  tULong tableP;
  tULong startIndex;
  tULong paletteEntries;
  tULong operation;
} WinPaletteParametersT;


#if defined(__PALMOS__)
//
// Palm OS routines
//
static IndexedColorType WinRGBToIndex(const RGBColorType* rgbP,
                                      const tVoid* emulStateP,
                                      Call68KFuncType* call68KFuncP);
static Err WinPalette(WinPaletteParametersT param,
                      const tVoid* emulStateP,
                      Call68KFuncType* call68KFuncP);
#endif /* __PALMOS__ */


//
// TestU
//
#ifdef _TESTU
static tUShort PerformTestU(tNativeCPC* NativeCPC);
#endif /* _TESTU */


/***********************************************************************
 *
 *  Entry Points
 *
 ***********************************************************************/
#if defined(__PALMOS__)
tULong PNOMain(const tVoid* emulStateP,
               tVoid* userData68KP,
               Call68KFuncType* call68KFuncP)
#elif defined(__WIN32__)
tULong Engine_CPCSetColor(tNativeCPC* NativeCPC)
#endif /* __PALMOS__ || __WIN32__ */
/***********************************************************************
 *
 *  PNOMain
 *
 ***********************************************************************/
{
#if defined(__PALMOS__)
tNativeCPC* NativeCPC;
#endif /* __PALMOS__ */
colours_rgb_entry* colours_rgb;
colours_rgb_entry* colours_green;
#if defined(__PALMOS__)
RGBColorType RGBColor;
WinPaletteParametersT winPaletteParam;
IndexedColorType PaletteIndex;
#endif /* __PALMOS__ */
tULong red, green, blue;
tUShort n;

#if defined(__PALMOS__)
  NativeCPC = (tNativeCPC*)userData68KP;
#endif /* __PALMOS__ */

#if defined(__PALMOS__)
  winPaletteParam.routineP = NativeCPC->WinPalettePtr;
  winPaletteParam.operation = winPaletteSet;
  winPaletteParam.paletteEntries = 1;
  winPaletteParam.tableP = (tULong)&RGBColor;
#endif /* __PALMOS__ */

  if ( (!NativeCPC->scr_tube) && (!NativeCPC->night_mode) )
  {
    colours_rgb = NativeCPC->colours_rgb;
    colours_green = NativeCPC->colours_green;
    NativeCPC->active_colours = NativeCPC->colours_rgb;

    for (n = 0; n < NATIVECPC_COLOURS_NBELEMENT; n++)
    {
      red = (tULong)(colours_rgb[n].colour_r * NativeCPC->scr_intensity / 10.0);
      if (red > 255) // limit to the maximum
      {
        red = 255;
      }
      green = (tULong)(colours_rgb[n].colour_g * NativeCPC->scr_intensity / 10.0);
      if (green > 255) // limit to the maximum
      {
        green = 255;
      }
      blue = (tULong)(colours_rgb[n].colour_b * NativeCPC->scr_intensity / 10.0);
      if (blue > 255) // limit to the maximum
      {
        blue = 255;
      }
      
#if defined(__PALMOS__)
      RGBColor.r = red;
      RGBColor.g = green;
      RGBColor.b = blue;

      PaletteIndex = WinRGBToIndex(&RGBColor,
                                   NativeCPC->emulStateP,
                                   NativeCPC->call68KFuncP);

      NativeCPC->colours[n] = PaletteIndex;
      NativeCPC->PalmToCPCPalette[PaletteIndex] = n;
      NativeCPC->PalmPaletteIntensity[PaletteIndex] = colours_green[n].intensity;

      // Set Palm palette with CPC color values
      winPaletteParam.startIndex = NativeCPC->colours[n];
      WinPalette(winPaletteParam,
                 NativeCPC->emulStateP,
                 NativeCPC->call68KFuncP);
#else /* __PALMOS__ */
      NativeCPC->WinPalettePtr[n].colour_r = red;
      NativeCPC->WinPalettePtr[n].colour_g = green;
      NativeCPC->WinPalettePtr[n].colour_b = blue;
      NativeCPC->WinPalettePtr[n].intensity = colours_green[n].intensity;
      NativeCPC->WinPalettePtr[n].shadow_index = colours_rgb[n].shadow_index;


      if (NativeCPC->BitsPerPixel == 32)
      {
        NativeCPC->colours[n] = (tULong)(255 << 24)
                                + ((tULong)(NativeCPC->WinPalettePtr[n].colour_r << 16))
                                + ((tULong)(NativeCPC->WinPalettePtr[n].colour_g << 8))
                                + (tULong)NativeCPC->WinPalettePtr[n].colour_b;
      }
      else if (NativeCPC->BitsPerPixel == 24)
      {
        NativeCPC->colours[n] = ((tULong)(NativeCPC->WinPalettePtr[n].colour_r << 16))
                                + ((tULong)(NativeCPC->WinPalettePtr[n].colour_g << 8))
                                + (tULong)NativeCPC->WinPalettePtr[n].colour_b;
      }
      else
      {
        NativeCPC->colours[n] = n;
      }

#endif /* __PALMOS__ */
    }
  }
  else
  {
    colours_green = NativeCPC->colours_green;
    NativeCPC->active_colours = NativeCPC->colours_green;
    
    for (n = 0; n < NATIVECPC_COLOURS_NBELEMENT; n++)
    {
      green = (tULong)(colours_green[n].colour_g * NativeCPC->scr_intensity / 10.0);
      if (green > 255) // limit to the maximum
      {
        green = 255;
      }
      
#if defined(__PALMOS__)
      // Invert red and green for night mode
      RGBColor.r = !NativeCPC->night_mode ? 0 : green;
      RGBColor.g = !NativeCPC->night_mode ? green : 0;
      RGBColor.b = 0;

      PaletteIndex = WinRGBToIndex(&RGBColor,
                                   NativeCPC->emulStateP,
                                   NativeCPC->call68KFuncP);

      NativeCPC->colours[n] = PaletteIndex;
      NativeCPC->PalmToCPCPalette[PaletteIndex] = n;
      NativeCPC->PalmPaletteIntensity[PaletteIndex] = colours_green[n].intensity;

      // Set Palm palette with CPC color values
      winPaletteParam.startIndex = NativeCPC->colours[n];
      WinPalette(winPaletteParam,
                 NativeCPC->emulStateP,
                 NativeCPC->call68KFuncP);
#else /* __PALMOS__ */
#ifndef _DEBUG
      NativeCPC->WinPalettePtr[n].colour_r = !NativeCPC->night_mode ? 0 : green;
      NativeCPC->WinPalettePtr[n].colour_g = !NativeCPC->night_mode ? green : 0;
      NativeCPC->WinPalettePtr[n].colour_b = 0;
#else /* _DEBUG */
      NativeCPC->WinPalettePtr[n].colour_r = !NativeCPC->night_mode ? (0xff - green) : green;
      NativeCPC->WinPalettePtr[n].colour_g = !NativeCPC->night_mode ? (0xff - green) : 0;
      NativeCPC->WinPalettePtr[n].colour_b = !NativeCPC->night_mode ? (0xff - green) : 0;
#endif /* _DEBUG */
      NativeCPC->WinPalettePtr[n].intensity = colours_green[n].intensity;
      NativeCPC->WinPalettePtr[n].shadow_index = colours_green[n].shadow_index;

      if (NativeCPC->BitsPerPixel == 32)
      {
        NativeCPC->colours[n] = (tULong)(255 << 24)
                                + ((tULong)(NativeCPC->WinPalettePtr[n].colour_r << 16))
                                + ((tULong)(NativeCPC->WinPalettePtr[n].colour_g << 8))
                                + (tULong)NativeCPC->WinPalettePtr[n].colour_b;
      }
      else if (NativeCPC->BitsPerPixel == 24)
      {
        NativeCPC->colours[n] = ((tULong)(NativeCPC->WinPalettePtr[n].colour_r << 16))
                                + ((tULong)(NativeCPC->WinPalettePtr[n].colour_g << 8))
                                + (tULong)NativeCPC->WinPalettePtr[n].colour_b;
      }
      else
      {
        NativeCPC->colours[n] = n;
      }

#endif /* __PALMOS__ */
    }
  }

  // Update palette
  for (n = 0; n < GA_INK_VALUES_NBELEMENT; n++)
  { // loop for all colours + border
		NativeCPC->GateArray->palette[n] = NativeCPC->colours[NativeCPC->GateArray->ink_values[n]];
  }

#ifdef _TESTU
  return PerformTestU(NativeCPC);
#else /* _TESTU */
  return errNone;
#endif /* _TESTU */

}
/*----------------------------------------------------------------------------*/



#if defined(__PALMOS__)

//==============================================================================
//
// Palm OS routines
//
//==============================================================================

static IndexedColorType WinRGBToIndex(const RGBColorType* rgbP,
                                      const void *emulStateP,
                                      Call68KFuncType *call68KFuncP)
/***********************************************************************
 *
 *  WinRGBToIndex
 *
 ***********************************************************************/
{
tUChar args[4]; // Size should be multiple of 2

  // IndexedColorType WinRGBToIndex(const RGBColorType* rgbP);

  // WinRGBToIndex Parameters
  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(rgbP);
  
  return (IndexedColorType)call68KFuncP(emulStateP,
                                        PceNativeTrapNo(sysTrapWinRGBToIndex),
                                        &args,
                                        sizeof(args)/sizeof(args[0]));
}
/*----------------------------------------------------------------------------*/


static Err WinPalette(WinPaletteParametersT param,
                      const void *emulStateP,
                      Call68KFuncType *call68KFuncP)
/***********************************************************************
 *
 *  WinPalette
 *
 ***********************************************************************/
{
tUChar args[12]; // Size should be multiple of 2

  // Err WinPalette(UInt8 operation, Int16 startIndex, UInt16 paletteEntries, RGBColorType* tableP);

  // WinPalette Parameters
  *((tULong*)(&(args[0]))) = EndianSwap32(param.tableP); // RGBColorType* tableP
  *((tUShort*)(&(args[4]))) = EndianSwap16((tUShort)param.startIndex); // Int16 startIndex
  *((tUShort*)(&(args[6]))) = EndianSwap16((tUShort)param.paletteEntries); // UInt16 paletteEntries
  args[8] = (tUChar)param.operation; // UInt8 operation

  return (Err)call68KFuncP(emulStateP,
                           param.routineP,
                           &args,
                           sizeof(args)/sizeof(args[0]));
}
/*----------------------------------------------------------------------------*/

#endif /* __PALMOS__ */


//==============================================================================
//
// Unitary Tests
//
//==============================================================================
#ifdef _TESTU
// Prototypes of TestU fonctions

static tUShort PerformTestU(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PerformTestU
 *
 ***********************************************************************/
{
tUShort Result = errNone;

	NOT_USED(NativeCPC);

  return (Result);
}
/*----------------------------------------------------------------------------*/

#endif /* _TESTU */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
