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
//---------------------------------------------------------------------------

#include "colours_green.h"


/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


#define INDEX_RED            0
#define INDEX_GREEN          1
#define INDEX_BLUE           2
#define INDEX_INTENSITY      3
#define INDEX_SHADOW         4


static const tDouble colours_green[NB_COLORS] =
{
   0.5647, //  0
   0.5647, //  1 
   0.7529, //  2
   0.9412, //  3
   0.1882, //  4
   0.3765, //  5
   0.4706, //  6
   0.6588, //  7
   0.3765, //  8
   0.9412, //  9
   0.9098, // 10
   0.9725, // 11
   0.3451, // 12
   0.4078, // 13
   0.6275, // 14
   0.6902, // 15
   0.1882, // 16
   0.7529, // 17
   0.7216, // 18
   0.7843, // 19
   0.1569, // 20
   0.2196, // 21
   0.4392, // 22
   0.5020, // 23
   0.2824, // 24
   0.8471, // 25
   0.8157, // 26
   0.8784, // 27
   0.2510, // 28
   0.3137, // 29
   0.5333, // 30
   0.5961  // 31
};



colours_rgb_entry green_rgb[NB_COLORS] =
{
  //   Red, Green, Blue, Intensity, Shadow_index
   {   0,   0,     0,    0,          0 }, //  0
   {   0,   0,     0,    0,          1 }, //  1
   {   0,   0,     0,    0,          2 }, //  2
   {   0,   0,     0,    0,          3 }, //  3
   {   0,   0,     0,    0,          4 }, //  4
   {   0,   0,     0,    0,          5 }, //  5
   {   0,   0,     0,    0,          6 }, //  6
   {   0,   0,     0,    0,          7 }, //  7
   {   0,   0,     0,    0,          8 }, //  8
   {   0,   0,     0,    0,          9 }, //  9
   {   0,   0,     0,    0,         10 }, //  10
   {   0,   0,     0,    0,         11 }, //  11
   {   0,   0,     0,    0,         12 }, //  12
   {   0,   0,     0,    0,         13 }, //  13
   {   0,   0,     0,    0,         14 }, //  14
   {   0,   0,     0,    0,         15 }, //  15
   {   0,   0,     0,    0,         16 }, //  16
   {   0,   0,     0,    0,         17 }, //  17
   {   0,   0,     0,    0,         18 }, //  18
   {   0,   0,     0,    0,         19 }, //  19
   {   0,   0,     0,    0,         20 }, //  20
   {   0,   0,     0,    0,         21 }, //  21
   {   0,   0,     0,    0,         22 }, //  22
   {   0,   0,     0,    0,         23 }, //  23
   {   0,   0,     0,    0,         24 }, //  24
   {   0,   0,     0,    0,         25 }, //  25
   {   0,   0,     0,    0,         26 }, //  26
   {   0,   0,     0,    0,         27 }, //  27
   {   0,   0,     0,    0,         28 }, //  28
   {   0,   0,     0,    0,         29 }, //  29
   {   0,   0,     0,    0,         30 }, //  30
   {   0,   0,     0,    0,         31 }, //  31
};
//---------------------------------------------------------------------------



void PrepareGreenColoursTable(void)
{
  tLong green;
  tLong diff;
  tLong bestdiff;
	tUChar loop;
	tUChar index;
  tUChar nearestindex;
  
  // Prepare green RGB table
  for (loop=0; loop<NB_COLORS; loop++)
  {
    green = colours_green[loop] * 255;
    
    if (green > 255)
    {
      green = 255;
    }

    green_rgb[loop].colour_g = (tUChar)green; // Green
    green_rgb[loop].intensity = (tUChar)green; // Intensity
    

    // Retreive nearest shadow index
    nearestindex = 0;
    bestdiff = 255;
    for (index=0; index < NB_COLORS; index++)
    {
      diff = (green/2) - green_rgb[index].colour_g;
      if (diff < bestdiff)
      {
        nearestindex = index;
        bestdiff = diff;
      }
    }
    
    green_rgb[loop].shadow_index = nearestindex; // nearest shadow
  }
}
//---------------------------------------------------------------------------


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
