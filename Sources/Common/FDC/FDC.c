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

#include "..\FDC.h"


/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


const tUChar fdc_iam[] =
{ FDC_BYTE_IAM_1_3, FDC_BYTE_IAM_1_3, FDC_BYTE_IAM_1_3, FDC_BYTE_IAM_4 };
const tUChar fdc_idam[] =
{ FDC_BYTE_IDAM_1_3, FDC_BYTE_IDAM_1_3, FDC_BYTE_IDAM_1_3, FDC_BYTE_IDAM_4 };
const tUChar fdc_dataam[] =
{ FDC_BYTE_DATAAM_1_3, FDC_BYTE_DATAAM_1_3, FDC_BYTE_DATAAM_1_3, FDC_BYTE_DATAAM_4 };
const tUChar fdc_dataam_deleted[] =
{ FDC_BYTE_DATAAM_1_3, FDC_BYTE_DATAAM_1_3, FDC_BYTE_DATAAM_1_3, FDC_BYTE_DATAAM_4_DELETED };

const tUChar fdc_sync_iam[] =
{ FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC,
  FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC,
  FDC_BYTE_IAM_1_3, FDC_BYTE_IAM_1_3, FDC_BYTE_IAM_1_3, FDC_BYTE_IAM_4 };

const tUChar fdc_sync_idam[] =
{ FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC,
  FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC, FDC_BYTE_SYNC,
  FDC_BYTE_IDAM_1_3, FDC_BYTE_IDAM_1_3, FDC_BYTE_IDAM_1_3, FDC_BYTE_IDAM_4 };


tUChar* DetectIAM(tUChar* TrackData, tULong Length)
/***********************************************************************
 *
 *  DetectIAM
 *
 ***********************************************************************/
{
tUChar* IAMP = cNull;
tUChar IAMSize = sizeof(fdc_sync_iam);
const tUChar* IAMString = fdc_sync_iam;
tULong Loop;

  for (Loop=0; Loop < Length; Loop++)
  {
    if (*TrackData == *IAMString)
    {
      IAMString++;
      if (IAMP == cNull)
      {
        IAMP = TrackData;
      }
      // End of string
      IAMSize--;
      if (IAMSize == 0)
      {
        break;
      }
    }
    else if (IAMP != cNull)
    {
      TrackData = IAMP;
      IAMP = cNull;
      IAMString = fdc_sync_iam;
      IAMSize = sizeof(fdc_sync_iam);
    }

    TrackData++;
  }

  if (IAMSize != 0)
  {
    IAMP = cNull;
  }

  return (IAMP);
}
//---------------------------------------------------------------------------


tUChar* DetectIDAM(tUChar* TrackData, tULong Length)
/***********************************************************************
 *
 *  DetectIDAM
 *
 ***********************************************************************/
{
tUChar* IDAMP = cNull;
tUChar IDAMSize = sizeof(fdc_sync_idam);
const tUChar* IDAMString = fdc_sync_idam;
tULong Loop;

  for (Loop=0; Loop < Length; Loop++)
  {
    if (*TrackData == *IDAMString)
    {
      IDAMString++;
      if (IDAMP == cNull)
      {
        IDAMP = TrackData;
      }
      // End of string
      IDAMSize--;
      if (IDAMSize == 0)
      {
        break;
      }
    }
    else if (IDAMP != cNull)
    {
      TrackData = IDAMP;
      IDAMP = cNull;
      IDAMString = fdc_sync_idam;
      IDAMSize = sizeof(fdc_sync_idam);
    }

    TrackData++;
  }

  if (IDAMSize != 0)
  {
    IDAMP = cNull;
  }

  return (IDAMP);
}
//---------------------------------------------------------------------------


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif
