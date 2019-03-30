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

#include "..\Native_CPC.h"


#ifdef __cplusplus
extern "C" {
#endif


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#undef PATCH_BEAM_X
#define PATCH_BEAM_X

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


#if defined (__WIN32__)
tBool Engine_IsVideoVBL(tNativeCPC* NativeCPC)
#endif
/***********************************************************************
 *
 *  Engine_IsVideoVBL
 *
 ***********************************************************************/
{
#ifndef CRTC_ENGINE_V2
  return ((NativeCPC->CRTC->flags & VS_flag) ? cTrue : cFalse);
#else /* CRTC_ENGINE_V2 */
  return (NativeCPC->CRTC->flag_invsync ? cTrue : cFalse);
#endif /* CRTC_ENGINE_V2 */
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tBool Engine_IsVideoHBL(tNativeCPC* NativeCPC)
#endif
/***********************************************************************
 *
 *  Engine_IsVideoHBL
 *
 ***********************************************************************/
{
#ifndef CRTC_ENGINE_V2
  return ((NativeCPC->CRTC->flags & HS_flag) ? cTrue : cFalse);
#else /* CRTC_ENGINE_V2 */
  return (NativeCPC->CRTC->flags1.inHSYNC ? cTrue : cFalse);
#endif /* CRTC_ENGINE_V2 */
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tULong Engine_GetVideoCharCount(tNativeCPC* NativeCPC)
#endif
/***********************************************************************
 *
 *  Engine_GetVideoCharCount
 *
 ***********************************************************************/
{
  return NativeCPC->CRTC->char_count;
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tULong Engine_GetVideoLineCount(tNativeCPC* NativeCPC)
#endif
/***********************************************************************
 *
 *  Engine_GetVideoLineCount
 *
 ***********************************************************************/
{
  return NativeCPC->CRTC->line_count;
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tULong Engine_GetVideoRasterCount(tNativeCPC* NativeCPC)
#endif
/***********************************************************************
 *
 *  Engine_GetVideoRasterCount
 *
 ***********************************************************************/
{
  return NativeCPC->CRTC->raster_count;
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tLong Engine_GetVideoBeamPosX(tNativeCPC* NativeCPC)
#endif
/***********************************************************************
 *
 *  Engine_GetVideoBeamPosX
 *
 ***********************************************************************/
{
#ifndef CRTC_ENGINE_V2
  return (NativeCPC->CRTC->char_count * 8);
#else /* CRTC_ENGINE_V2 */

#ifndef PATCH_BEAM_X
  tUShort originX = (NativeCPC->CRTC->registers[0]+1)
                    - (NativeCPC->CRTC->registers[2] + NativeCPC->CRTC->hsw);
  return ((NativeCPC->CRTC->char_count + originX - 1) * 8); 
#else /* PATCH_BEAM_X */
  return ((NativeCPC->CRTC->HorzPos >> 5) - 8);
#endif /* PATCH_BEAM_X */

#endif /* CRTC_ENGINE_V2 */
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tLong Engine_GetVideoBeamPosY(tNativeCPC* NativeCPC)
#endif
/***********************************************************************
 *
 *  Engine_GetVideoBeamPosY
 *
 ***********************************************************************/
{
#ifndef CRTC_ENGINE_V2
  return (NativeCPC->VDU->scanline);
#else /* CRTC_ENGINE_V2 */
  return (NativeCPC->VDU->scrln);
#endif /* CRTC_ENGINE_V2 */
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tUChar Engine_ReadMem(tNativeCPC* NativeCPC,
                      tULong addr)
#endif
/***********************************************************************
 *
 *  Engine_ReadMem
 *
 ***********************************************************************/
{
  return (*(NativeCPC->membank_read[(addr >> 14) & 3] + (addr & 0x3fff))); // returns a byte from a 16KB memory bank
}
/*----------------------------------------------------------------------------*/


#if defined (__WIN32__)
tVoid Engine_WriteMem(tNativeCPC* NativeCPC,
                      tULong addr,
                      tULong val)
#endif
/***********************************************************************
 *
 *  Engine_WriteMem
 *
 ***********************************************************************/
{
  *(NativeCPC->membank_write[(addr >> 14) & 3] + (addr & 0x3fff)) = (tUChar)val; // writes a byte to a 16KB memory bank
}
/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

