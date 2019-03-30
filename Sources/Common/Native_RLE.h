/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2016 by Fr�d�ric Coste

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

#ifndef NATIVE_RLE_H
#define NATIVE_RLE_H


#include "..\Engine\types.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef __WIN32__
extern tULong DecodeRLE(tUChar* Dest,
                        const tUChar* Source,
                        tULong SourceSize,
                        tULong MaxDestSize,
                        tUChar Tag);
#endif /* __WIN32__ */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* ! NATIVE_RLE_H */
