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

#ifndef MFM_H
#define MFM_H


#include "..\Engine\types.h"
#include "..\..\Engine\Native_CPC.h"


#ifdef __cplusplus
extern "C" {
#endif

extern tVoid DetectMFMShift(tUChar Expected,
                            tUChar Source,
                            tUChar* BitShift,
                            tUChar* PulseShift);
extern tVoid MFMShift(tUChar* dataP,
                      tULong numBytes,
                      tUChar Shift,
                      tUChar PulseShift);
extern tVoid MFMShiftWithCopy(tUChar* destP,
                              tUChar* sourceP,
                              tULong numBytes,
                              tUChar Shift,
                              tUChar PulseShift);

extern tUChar GetByteFromPulses(tUChar* Bits);
extern tUChar GetHalfByteFromPulses(tUChar* Bits);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* ! MFM_H */
