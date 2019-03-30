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


#if defined (__WIN32__)
tVoid Engine_GetGraphicFormat(tEngineGraphicFormat* GraphicFormat)
#endif
/***********************************************************************
 *
 *  Engine_GetGraphicFormat
 *
 ***********************************************************************/
{
  GraphicFormat->VisibleScreenWidth = CPC_VISIBLE_SCR_WIDTH;
  GraphicFormat->VisibleScreenHeight = CPC_VISIBLE_SCR_HEIGHT;
  GraphicFormat->ScreenWidth = CPC_SCR_WIDTH;
  GraphicFormat->ScreenHeight = CPC_SCR_HEIGHT;
  GraphicFormat->OffscreenVisibleY = OFFSCREEN_VISIBLE_Y;
#ifndef CRTC_ENGINE_V2
  GraphicFormat->MaxBitPerPixel = 8;
#else /* CRTC_ENGINE_V2 */
  GraphicFormat->MaxBitPerPixel = 32;
#endif /* CRTC_ENGINE_V2 */
}
/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

