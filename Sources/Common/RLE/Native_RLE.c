/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2016 by Frédéric Coste

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

#include "..\Native_RLE.h"
#define __WIN32__

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 *
 *  Entry Points
 *
 ***********************************************************************/

 
#if defined(__WIN32__)
tULong DecodeRLE(tUChar* Dest,
                 const tUChar* Source,
                 tULong SourceSize,
                 tULong MaxDestSize,
                 tUChar Tag)
#endif /* __WIN32__ */
/***********************************************************************
 *
 *  PNOMain
 *
 ***********************************************************************/
{
tULong DestSize = 0;
tUChar Loop;
tUChar Data;

  if (!Dest) return 0;
  if (!Source) return 0;

  while (SourceSize--)
  {
    // Tag ?
    if (*Source == Tag)
    {
      Source++;
      SourceSize--;

      Loop = *Source;

      // No loop ?
      if (Loop == 0)
      {
        // Copy Tag
        *(Dest++) = Tag;
        DestSize++;

        // Check max size
        if (MaxDestSize)
        {
          if (DestSize > MaxDestSize)
            return 0;
        }

        // Next source
        Source++;
      }
      else
      {
        Source++;
        SourceSize--;

        Data = *Source;

        while (Loop--)
        {
          *(Dest++) = Data;
          DestSize++;

          // Check max size
          if (MaxDestSize)
          {
            if (DestSize > MaxDestSize)
              return 0;
          }
        }

        // Next source byte
        Source++;
      }
    }
    else
    {
      // Direct copy
      *(Dest++) = *(Source++);
      DestSize++;

      // Check max size
      if (MaxDestSize)
      {
        if (DestSize > MaxDestSize)
          return 0;
      }
    }
  }

  return DestSize;
}
/*----------------------------------------------------------------------------*/


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
