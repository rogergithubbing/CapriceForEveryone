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

#ifdef __PALMOS__
#include <PceNativeCall.h>
#include <ByteOrderUtils.h>
#endif /* __PALMOS__ */

#include "..\Native_CPC.h"

//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


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


#if defined(__PALMOS__)

// copied from CoreTraps.h
#define sysTrapMemHandleUnlock        0xA022
#define sysTrapDmReleaseResource      0xA061

#endif /* __PALMOS__ */

//
// External routines
//
#if defined(__PALMOS__)
static tUShort MemHandleUnlock(tVoid* h,
                               const tVoid* emulStateP,
                               Call68KFuncType* call68KFuncP);

static tUShort DmReleaseResource(void* resourceH,
                                 const tVoid* emulStateP,
                                 Call68KFuncType* call68KFuncP);
#endif /* __PALMOS__ */


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
tULong Engine_CPCStop(tNativeCPC* NativeCPC)
#endif /* __PALMOS__ || __WIN32__ */
/***********************************************************************
 *
 *  PNOMain (PALM OS)
 *  Engine_CPCStop (WIN32)
 *
 ***********************************************************************/
{
#if defined(__PALMOS__)
tNativeCPC* NativeCPC = (tNativeCPC*)userData68KP;
#endif /* __PALMOS__ */

  // Désallocation de la mémoire
  if (NativeCPC->pbROMlo != cNull)
  {
#if defined(__PALMOS__)
    MemHandleUnlock(NativeCPC->hMemROMlo,
                    emulStateP,
                    call68KFuncP);
    DmReleaseResource(NativeCPC->hMemROMlo,
                      emulStateP,
                      call68KFuncP);
#endif /* __PALMOS__ */

    NativeCPC->pbROMlo = cNull;
    NativeCPC->hMemROMlo = cNull;
    NativeCPC->pbROMhi = cNull;
    NativeCPC->pbExpansionROM = cNull;
  }
  
  if (NativeCPC->memmap_ROM != cNull)
  {
    if (NativeCPC->memmap_ROM[7] != cNull)
    {
#if defined(__PALMOS__)
      MemHandleUnlock(NativeCPC->hMemAMSDOS,
                      emulStateP,
                      call68KFuncP);
      DmReleaseResource(NativeCPC->hMemAMSDOS,
                        emulStateP,
                        call68KFuncP);
#endif /* __PALMOS__ */

      NativeCPC->hMemAMSDOS = cNull;
      NativeCPC->memmap_ROM[7] = cNull;
    }
  }

  return (tULong)errNone;
}


//==============================================================================
//
// Palm OS routines
//
//==============================================================================

#if defined(__PALMOS__)
static tUShort MemHandleUnlock(tVoid* h,
                               const tVoid* emulStateP,
                               Call68KFuncType* call68KFuncP)
/***********************************************************************
 *
 *  MemHandleUnlock
 *
 ***********************************************************************/
{
tUChar args[4];

  // Err MemHandleUnlock(MemHandle h);
  // MemChunkFree Parameters
  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(h);

  return (tUShort)call68KFuncP(emulStateP,
                               PceNativeTrapNo(sysTrapMemHandleUnlock),
                               &args,
                               (sizeof(args)/sizeof(args[0])));
}
/*----------------------------------------------------------------------------*/


static tUShort DmReleaseResource(void* resourceH,
                                 const tVoid* emulStateP,
                                 Call68KFuncType* call68KFuncP)
/***********************************************************************
 *
 *  DmReleaseResource
 *
 ***********************************************************************/
{
tUChar args[4];

  // Err DmReleaseResource(MemHandle resourceH);
  // DmReleaseResource Parameters
  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(resourceH);

  return (tUShort)call68KFuncP(emulStateP,
                               PceNativeTrapNo(sysTrapDmReleaseResource),
                               &args,
                               (sizeof(args)/sizeof(args[0])));
}
/*----------------------------------------------------------------------------*/
#endif /* __PALMOS__ */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
