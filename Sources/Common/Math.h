/*
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

#ifndef MATH_H
#define MATH_H

#include "..\Engine\types.h"
#ifdef __PALMOS__
#include "..\..\Palm\Sections.h"
#endif /* __PALMOS__ */


#if defined (__PALMOS__)
// Big endian
#define __HI32(x) *((tULong*)&x)			// return first tULong
#define __LO32(x) *((tULong*)&x + 1)	// return second tULong
#elif defined(__WIN32__)
// Little endian
#define __HI32(x) *((tULong*)&x + 1)  // return second tULong
#define __LO32(x) *((tULong*)&x)			// return first tULong
#else
  // commented in order to be able to compile to x64 #error Need endianness definition
#endif 


/* Get the more significant 32 bit int from a double.  */
#define GET_HIGH_WORD(i,d)  (i) = __HI32(d);
/* Get the less significant 32 bit int from a double.  */
#define GET_LOW_WORD(i,d) (i) = __LO32(d);
/* Set the more significant 32 bits of a double from an int.  */
#define SET_HIGH_WORD(d,v) __HI32(d) = (v);
/* Set the less significant 32 bits of a double from an int.  */
#define SET_LOW_WORD(d,v) __LO32(d) = (v);


/* Get two 32 bit ints from a double.  */
#define EXTRACT_WORDS(ix0,ix1,d) \
			(ix0) = __HI32(d); \
			(ix1) = __LO32(d);

/* Set a double from two 32 bit ints.  */
#define INSERT_WORDS(d,ix0,ix1) \
			__HI32(d) = (ix0); \
			__LO32(d) = (ix1);


#if defined(__PALMOS__)

extern tDouble math_exp(tDouble x) SECTION_MATH;
extern tDouble math_rint(tDouble x) SECTION_MATH;

#endif /* __PALMOS__ */



#if defined(__WIN32__)

#ifdef __cplusplus
extern "C" {
#endif

extern tDouble math_rint(tDouble x);
extern tDouble math_ceil(tDouble x);
extern tDouble math_floor(tDouble x);
extern tDouble math_fabs(tDouble x);

extern tDouble math_exp(tDouble x);
extern tDouble math_log(tDouble x);

extern tDouble math_cos(tDouble x);
extern tDouble math_sin(tDouble x);
extern tDouble math_tan(tDouble x);
extern tDouble math_atan(tDouble x);

extern tDouble math_random(tULong* seed);

extern tDouble ieee754_sqrt(tDouble x);
extern tLong   ieee754_rem_pio2(tDouble x, tDouble *y);

extern tDouble kernel_copysign(tDouble x, tDouble y);
extern tDouble kernel_scalbn (tDouble x, tLong n);
extern tLong   kernel_rem_pio2(tDouble *x, tDouble *y, tLong e0, tLong nx, tLong prec, const tLong *ipio2);
extern tDouble kernel_cos(tDouble x, tDouble y);
extern tDouble kernel_sin(tDouble x, tDouble y, tLong iy);
extern tDouble kernel_tan(tDouble x, tDouble y, tLong iy);

#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif


//
// Problem when starting D3D => math_rint goes crazy !!
//
#ifdef __BORLANDC__
#define math_rint math_floor
#endif /* __BORLANDC__ */

#endif /* __WIN32__ */


#define M_LOG_2			3.01029995664e-01

/* Constants rounded for 21 decimals. */
#define M_E         2.71828182845904523536
#define M_LOG2E     1.44269504088896340736
#define M_LOG10E    0.434294481903251827651
#define M_LN2       0.693147180559945309417
#define M_LN10      2.30258509299404568402
#define M_PI        3.14159265358979323846
#define M_PI_2      1.57079632679489661923
#define M_PI_4      0.785398163397448309616
#define M_1_PI      0.318309886183790671538
#define M_2_PI      0.636619772367581343076
#define M_1_SQRTPI  0.564189583547756286948
#define M_2_SQRTPI  1.12837916709551257390
#define M_SQRT2     1.41421356237309504880
#define M_SQRT2_2   0.707106781186547524401


#if (__BORLANDC__ == 0x651)
//#include <math.h>
#define math_rint math_floor
#endif /* __BORLANDC__ */


#endif