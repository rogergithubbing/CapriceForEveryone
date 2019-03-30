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

#ifndef FDC_H
#define FDC_H


#include "..\Engine\types.h"
#include "..\..\Engine\Native_CPC.h"

extern const tUChar fdc_iam[];
extern const tUChar fdc_idam[];
extern const tUChar fdc_dataam[];
extern const tUChar fdc_dataam_deleted[];

extern const tUChar fdc_sync_iam[];


#ifdef __cplusplus
extern "C" {
#endif

extern tUChar* DetectIAM(tUChar* TrackData, tULong Length);
extern tUChar* DetectIDAM(tUChar* TrackData, tULong Length);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif


#endif /* ! FDC_H */
