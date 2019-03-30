/*
    CaPriCe for Palm OS - Amstrad CPC 464/664/6128 emulator for Palm devices
    Copyright (C) 2006-2011 by Frédéric Coste
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2016 by Frédéric Coste

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

// roger to include NULL
#include<cstddef>

#ifndef TYPES_H
#define TYPES_H

// Types
typedef void                    tVoid;
typedef unsigned char           tBool;
typedef signed char             tChar;
typedef unsigned char           tUChar;
typedef signed short            tShort;
typedef unsigned short          tUShort;
typedef signed long             tLong;
typedef unsigned long           tULong;
typedef float                   tFloat;
typedef double                  tDouble;

#if defined(__WIN32__)
typedef tUShort DmResID;
typedef void*   MemHandle;
typedef tUShort Err;
#endif /* __WIN32__ */


//
// Endianness
//
#define BIG_ENDIAN    1
#define LITTLE_ENDIAN 2

#if defined(__PALMOS__)
  #define GUI_ENDIANNESS     BIG_ENDIAN
  #define ENGINE_ENDIANNESS  LITTLE_ENDIAN
#elif defined(__WIN32__)
  #define GUI_ENDIANNESS     LITTLE_ENDIAN
  #define ENGINE_ENDIANNESS  LITTLE_ENDIAN
#endif


#define ENDIAN_SWAP_32(Value) \
( \
    ((Value & 0xff) << 24) \
  | ((Value & 0xff00) << 8) \
  | ((Value >> 8) & 0xff00) \
  | ((Value >> 24) & 0xff) \
)


//
// Aligned arrays
//
typedef union
{
// roger
  //tUChar Buffer[];
  tULong Dummy;
}tULongAlignedArray;


//
// ULong field
//
typedef union
{
  tULong ULong;
  struct { tUShort usH, usL; } UShort;
  struct { tUChar ucHH, ucHL, ucLH, ucLL; } UChar;
} tBigEndianULongField;
typedef union
{
  tULong ULong;
  struct { tUShort usL, usH; } UShort;
  struct { tUChar ucLL, ucLH, ucHL, ucHH; } UChar;
} tLittleEndianULongField;
#if GUI_ENDIANNESS == BIG_ENDIAN
  #define tFieldULong           tBigEndianULongField
#else
  #define tFieldULong           tLittleEndianULongField
#endif
#if ENGINE_ENDIANNESS == BIG_ENDIAN
  #define tNativeULongField     tBigEndianULongField
#else
  #define tNativeULongField     tLittleEndianULongField
#endif


//
// UShort field
//
typedef union
{
  tUShort UShort;
  struct { tUChar ucH, ucL; } UChar;
} tBigEndianUShortField;
typedef union
{
  tUShort UShort;
  struct { tUChar ucL, ucH; } UChar;
} tLittleEndianUShortField;
#if GUI_ENDIANNESS == BIG_ENDIAN
  #define tFieldUShort          tBigEndianUShortField
#else
  #define tFieldUShort          tLittleEndianUShortField
#endif
#if ENGINE_ENDIANNESS == BIG_ENDIAN
  #define tNativeUShortField    tBigEndianUShortField
#else
  #define tNativeUShortField    tLittleEndianUShortField
#endif


//
// Register
//
typedef union
{
  struct { tUChar l, h, h2, h3; } b;
  struct { tUShort l, h; } w;
  tULong d;
} tLittleEndianRegister;
typedef union
{
  struct { tUChar h3, h2, h, l; } b;
  struct { tUShort h, l; } w;
  tULong d;
} tBigEndianRegister;
#if ENGINE_ENDIANNESS == BIG_ENDIAN
  #define tRegister          tBigEndianRegister
#else
  #define tRegister          tLittleEndianRegister
#endif


typedef enum
{
  ResultSucceed,
  ResultFailed
} tResult;


//roger: changed to NULL BECAUSE OF CONFLICT IN fdc_command.h
//#define cNull                   ((tVoid*)0)
#define cNull                   NULL
#define cFalse                  (0)
#define cTrue                   (!cFalse)


//
// MACRO TO AVOID WARNINGS FOR UNUSED PARAMETER OR VARIABLE
//
#ifndef NOT_USED
#define NOT_USED(p) (tVoid)(p);
#endif /* ! NOT_USED */


// Language definitions
#if defined(__PALMOS__)
  #define INLINE inline
#elif defined(__WIN32__)
  #define INLINE
#else
  #define INLINE
#endif


//
// UNICODE check
//
	//#ifdef __BORLANDC__
	#if 0

	#if (__BORLANDC__ == 0x551) // Borland C++ Builder 5 Update 1
	#undef UNICODE
	#elif (__BORLANDC__ == 0x650) && defined(_WIN64) // RAD Studio XE3 Update 2 Win64
	#define UNICODE
	#elif (__BORLANDC__ == 0x651) // RAD Studio XE3 Update 2 Win32
	#define UNICODE
	#else /* __BORLANDC__ */
	//#pragma message ("You are compiling using Borland C++ version " __BORLANDC__)
	#error invalid __BORLANDC__
	#endif /* __BORLANDC__ */

	#else  /* __BORLANDC__ */

	#undef UNICODE

	#endif /* __BORLANDC__ */


#endif /* TYPES_H */

