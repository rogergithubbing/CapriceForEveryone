/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2012 by Frédéric Coste

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

#ifndef DIRECT_DEBUG_H
#define DIRECT_DEBUG_H

// List of constants
enum
{
  // 0
  DD_CONSTANT_Ticks_Per_Second,

  // MUST be the last one
  DD_CONSTANT_NB_ITEMS
};


#if defined(__WIN32__)
/*const char DD_Constants_Label[][50] =
{
  // 0
  "Ticks_Per_Second",
};*/
#endif /* __WIN32__ */

#endif /* DIRECT_DEBUG_H */

