/*
    Caprice32 - Amstrad CPC Emulator
    (c) Copyright 1997-2005 Ulrich Doewich

    CaPriCe for Palm OS - Amstrad CPC 464/664/6128 emulator for Palm devices
    Copyright (C) 2006-2011 by Frédéric Coste
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2017 by Frédéric Coste

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

#if defined(__PALMOS__)
#include <PceNativeCall.h>
#include <ByteOrderUtils.h>
#endif /* __PALMOS__ */

#include "..\Native_CPC.h"
#include "..\..\Common\Tables\colours_green.h"
#include "..\..\Common\Tables\colours_rgb.h"
#include <Math.h>
#include "..\..\Common\CRC_CCITT.h"
#include "..\..\Common\FDC.h"


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif


#if defined(__PALMOS__)
tULong PNOMain(const tVoid*,
               tVoid*,
               Call68KFuncType*);
#endif /* __PALMOS__ */

#if defined(__GNUC__)
// This seemingly bogus function satisfies the Windows LoadLibrary() call,
// (and must be the first function in our PNO .dll).
int /*WINAPI*/ my_bogus_cygwin_dll_entry(unsigned long /*HANDLE*/ h,
                                         unsigned long /*DWORD*/ reason,
                                         void* p)
{
  return 1;
}
#endif /* __GNUC__ */


#if defined(__PALMOS__)

// copied from CoreTraps.h
#define sysTrapMemHandleLock          0xA021
#define sysTrapDmGetResource          0xA05F

// copied from DataMgr.h
typedef tULong  DmResType;
typedef tUShort DmResID;

#endif /* __PALMOS__ */


static tVoid CPCInit(tNativeCPC* NativeCPC);
static tVoid Z80Init(tNativeCPC* NativeCPC);
static tVoid CRTCInit(tNativeCPC* NativeCPC);
static tVoid FDCInit(tNativeCPC* NativeCPC);
static tVoid PrinterInit(tNativeCPC* NativeCPC);
static tVoid ga_init_banking(tNativeCPC* NativeCPC);
static tVoid audio_init(tNativeCPC* NativeCPC);
static tVoid TapeInit(tTape* Tape);
static tVoid TMPISpeechInit(tTMPISpeech* TMPISpeech);


//
// Routines
//
static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value);


//
// External routines
//
#if defined(__PALMOS__)
static tVoid* GetResourcePtr(DmResID resID,
                             const tVoid* emulStateP,
                             Call68KFuncType* call68KFuncP);

static tVoid* MemHandleLock(tVoid* h,
                            const tVoid* emulStateP,
                            Call68KFuncType* call68KFuncP);
#endif /* __PALMOS__ */

//
// TestU
//
#ifdef _TESTU
static tUShort PerformTestU(tNativeCPC* NativeCPC);
#endif /* _TESTU */


/***********************************************************************
 *
 *  Entry Points
 *
 ***********************************************************************/
#if defined(__PALMOS__)
tULong PNOMain(const tVoid* emulStateP,
               tVoid* userData68KP,
               Call68KFuncType* call68KFuncP)
#elif defined(__WIN32__)
tULong Engine_CPCStart(tNativeCPC* NativeCPC)
#endif /* __PALMOS__ || __WIN32__ */
/***********************************************************************
 *
 *  PNOMain (PALM OS)
 *  Engine_CPCStart (WIN32)
 *
 ***********************************************************************/
{
#if defined(__PALMOS__)
tNativeCPC* NativeCPC;
#endif /* __PALMOS__ || __WIN32__ */
DmResID resID;

#if defined(__PALMOS__)
  NativeCPC = (tNativeCPC*)userData68KP;

  // Palm OS 68K interface
  NativeCPC->emulStateP = emulStateP;
  NativeCPC->call68KFuncP = call68KFuncP;
  NativeCPC->prefP = (tPreferences*)EndianSwap32(NativeCPC->prefP);
  NativeCPC->EmulatorSettingsP = (tPreferences*)EndianSwap32(NativeCPC->EmulatorSettingsP);
  NativeCPC->contextP = (tUChar*)EndianSwap32(NativeCPC->contextP);
  NativeCPC->BmpOffScreenBits = (tVoid*)EndianSwap32(NativeCPC->BmpOffScreenBits);
  NativeCPC->BmpOffScreenSizeInBytes = EndianSwap32(NativeCPC->BmpOffScreenSizeInBytes);
  NativeCPC->BmpOffScreenBytesRow = EndianSwap16(NativeCPC->BmpOffScreenBytesRow);
  NativeCPC->TraceAlertPtr = EndianSwap32(NativeCPC->TraceAlertPtr);
  NativeCPC->MemPtrNewPtr = EndianSwap32(NativeCPC->MemPtrNewPtr);
  NativeCPC->WinPalettePtr = EndianSwap32(NativeCPC->WinPalettePtr);
  NativeCPC->SoundCalculateLevelTablesPtr = EndianSwap32(NativeCPC->SoundCalculateLevelTablesPtr);
  NativeCPC->colours_rgb = (colours_rgb_entry*)EndianSwap32(NativeCPC->colours_rgb);
  NativeCPC->colours_green = (colours_rgb_entry*)EndianSwap32(NativeCPC->colours_green);
  NativeCPC->FDCCommandTable = (fdc_cmd_table_def*)EndianSwap32(NativeCPC->FDCCommandTable);
  NativeCPC->DAATable = (tUShort*)EndianSwap32(NativeCPC->DAATable);
#endif /* __PALMOS__ */

  NativeCPC->paused = 0;
  NativeCPC->FirstInitToPerform = 1;
#if defined(__WIN32__)
  NativeCPC->step_by_step = 0;
#endif /* __WIN32__ */

  //
  // CPC structure
  //
  CPCInit(NativeCPC);

  //
  // Z80 structure
  //
  NativeCPC->Z80 = (tZ80*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80);
  NativeCPC->Z80->SZ = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80_SZ);
  NativeCPC->Z80->SZ_BIT = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80_SZ_BIT);
  NativeCPC->Z80->SZP = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80_SZP);
  NativeCPC->Z80->SZHV_inc = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80_SZHV_inc);
  NativeCPC->Z80->SZHV_dec = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80_SZHV_dec);
  NativeCPC->Z80->SZHVC_add = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80_SZHVC_add);
  NativeCPC->Z80->SZHVC_sub = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_Z80_SZHVC_sub);
  Z80Init(NativeCPC);

  //
  // CRTC structure
  //
  NativeCPC->CRTC = (tCRTC*)(NativeCPC->contextP + CONTEXT_OFFSET_CRTC);
  NativeCPC->mode0_table = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_MODE0_TABLE);
  NativeCPC->mode1_table = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_MODE1_TABLE);
#ifdef CRTC_ENGINE_V2
  NativeCPC->MAXlate = (tUShort*)(NativeCPC->contextP + CONTEXT_OFFSET_MAXLATE);
#endif /* CRTC_ENGINE_V2 */
  CRTCInit(NativeCPC);

  //
  // FDC structure
  //
  NativeCPC->FDC = (tFDC*)(NativeCPC->contextP + CONTEXT_OFFSET_FDC);
  NativeCPC->FDC->DriveA = (tDrive*)(NativeCPC->contextP + CONTEXT_OFFSET_DRIVE_A);
  NativeCPC->FDC->DriveB = (tDrive*)(NativeCPC->contextP + CONTEXT_OFFSET_DRIVE_B);
  FDCInit(NativeCPC);

  //
  // Tape structure
  //
  NativeCPC->Tape = (tTape*)(NativeCPC->contextP + CONTEXT_OFFSET_TAPE);
  NativeCPC->Tape->onebit_table = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_BIT1_TABLE);
  NativeCPC->Tape->MemPtrNewPtr = NativeCPC->MemPtrNewPtr;
  NativeCPC->Tape->MemPtrDeletePtr = NativeCPC->MemPtrDeletePtr;
  TapeInit(NativeCPC->Tape);

  //
  // PPI structure
  //
  NativeCPC->PPI = (tPPI*)(NativeCPC->contextP + CONTEXT_OFFSET_PPI);

  //
  // Printer structure
  //
  NativeCPC->Printer = (tPrinter*)(NativeCPC->contextP + CONTEXT_OFFSET_PRINTER);
#ifdef ENABLE_PRINTER
  PrinterInit(NativeCPC);
#endif /* ENABLE_PRINTER */

  //
  // Audio
  //
  audio_init(NativeCPC);

  //
  // VDU structure
  //
  NativeCPC->VDU = (tVDU*)(NativeCPC->contextP + CONTEXT_OFFSET_VDU);

  //
  // TMPISpeech structure
  //
  NativeCPC->TMPISpeech = (tTMPISpeech*)(NativeCPC->contextP + CONTEXT_OFFSET_TMPI_SPEECH);
  NativeCPC->TMPISpeech->pbBufferStart = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_TMPI_BUFFER);
  NativeCPC->TMPISpeech->cos_table = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_TMPI_COS_TABLE);
  NativeCPC->TMPISpeech->exp_table = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_TMPI_EXP_TABLE);
  NativeCPC->TMPISpeech->exp2_table = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_TMPI_EXP2_TABLE);
  NativeCPC->TMPISpeech->noise_table = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_TMPI_NOISE_TABLE);
#ifdef ENABLE_TMPI_SPEECH
  TMPISpeechInit(NativeCPC->TMPISpeech);
#endif /* ENABLE_TMPI_SPEECH */

  //
  // Allocation de la mémoire
  //
  NativeCPC->pbGPBuffer = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_GPBUFFER);
  NativeCPC->pbRAM = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_RAM);
  NativeCPC->memmap_ROM = (tUChar**)(NativeCPC->contextP + CONTEXT_OFFSET_MEMMAP_ROM);

  //
  // GateArray structure
  //
  NativeCPC->GateArray = (tGateArray*)(NativeCPC->contextP + CONTEXT_OFFSET_GATEARRAY);

  //
  // Mapping mémoire
  //
  // init the CPC memory banking map
  ga_init_banking(NativeCPC);

  //
  // Charger les ROMs
  //
#if defined(__PALMOS__)
  // CPC ROM en ROMlo
  if (NativeCPC->prefP->CPCModel == CPC_MODEL_464_UK)
  {
    resID = ROM_ID_464_UK;
  }
  else if (NativeCPC->prefP->CPCModel == CPC_MODEL_464_FR)
  {
    resID = ROM_ID_464_FR;
  }
  else if (NativeCPC->prefP->CPCModel == CPC_MODEL_664_UK)
  {
    resID = ROM_ID_664_UK;
  }
  else if (NativeCPC->prefP->CPCModel == CPC_MODEL_6128_FR)
  {
    resID = ROM_ID_6128_FR;
  }
  else /* Default is CPC_MODEL_6128_UK */
  {
    resID = ROM_ID_6128_UK;
  }
  NativeCPC->hMemROMlo = GetResourcePtr(resID,
                                        emulStateP,
                                        call68KFuncP);
  NativeCPC->pbROMlo = (tUChar*)MemHandleLock(NativeCPC->hMemROMlo,
                                              emulStateP,
                                              call68KFuncP);
											  
  NativeCPC->pbROMhi = NativeCPC->pbExpansionROM = NativeCPC->pbROMlo + 16384;
#endif /* __PALMOS__ */

    
#if defined(__PALMOS__)
  // AMSDOS à l'emplacement 7
  if (NativeCPC->prefP->UseParados == 0)
  {
    resID = ROM_ID_AMSDOS;
  }
  else // Use PARADOS
  {
    resID = ROM_ID_PARADOS;
  }
  NativeCPC->hMemAMSDOS = GetResourcePtr(resID,
                                         emulStateP,
                                         call68KFuncP
                                        );
  NativeCPC->memmap_ROM[7] = (tUChar*)MemHandleLock(NativeCPC->hMemAMSDOS,
                                                    emulStateP,
                                                    call68KFuncP);
#endif /* __PALMOS__ */

#ifdef _TESTU
  return PerformTestU(NativeCPC);
#else /* _TESTU */
  return errNone;
#endif /* _TESTU */
}
/*----------------------------------------------------------------------------*/



static tVoid CPCInit(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  CPCInit
 *
 ***********************************************************************/
{
tPreferences* prefP = NativeCPC->prefP;

  NativeCPC->jumpers = prefP->CPCJumpers;

  // Clear RAM banks usage
  NativeCPC->rambank_used = 0;

#ifdef __WIN32__
  if (prefP->Use256kMemoryExtension)
  {
    NativeCPC->ram_size = 320; // 64k + 256k

    // 256K expansion memory uses RAM banks 0 to 4.
    NativeCPC->rambank_used |= 0x0F;
  }
  else
#endif /* __WIN32__ */
  if ( (prefP->CPCModel == CPC_MODEL_6128_UK)
       || (prefP->CPCModel == CPC_MODEL_6128_FR)
       || (prefP->Use64kMemoryExtension) )
  {
    NativeCPC->ram_size = 128;

    // 64Ko expansion (internal or external) uses RAM bank 0.
    NativeCPC->rambank_used |= 0x01;
  }
  else
  {
    NativeCPC->ram_size = 64;
  }

#ifdef USE_SILICON_DISC
  if (prefP->Use256kSiliconDisc)
  {
    NativeCPC->ram_size = MAX_RAM_SIZE;
    // Silicon disc uses RAM banks 4 to 7.
    NativeCPC->rambank_used |= 0xF0;
  }
#endif /* USE_SILICON_DISC */

  // Prepare color tables
  PrepareGreenColoursTable();
  NativeCPC->colours_rgb = colours_rgb;
  NativeCPC->colours_green = green_rgb;

  NativeCPC->scr_tube = prefP->ScreenType; // 0 = Color, 1 = Green
  NativeCPC->scr_intensity = prefP->ScreenIntensity;

  NativeCPC->night_mode = prefP->NightModeActive;

  // Keyboard clash type
  if (prefP->CPCModel == CPC_MODEL_664_UK)
  {
    NativeCPC->keyboard_clash_type = 2;
  }
  else
  {
    NativeCPC->keyboard_clash_type = 1;
  }
}
/*----------------------------------------------------------------------------*/


static tVoid Z80Init(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  Z80Init
 *
 ***********************************************************************/
{
tZ80* Z80 = NativeCPC->Z80;
tUShort i, p;
tShort oldval, newval, val;
tUChar* padd;
tUChar* padc;
tUChar* psub;
tUChar* psbc;

  Z80->Regs.breakpoint = 0xffffffff; // clear break point

  for (i = 0; i < 256; i++)
  {
    p = 0;

    if (i & 0x01) ++p;
    if (i & 0x02) ++p;
    if (i & 0x04) ++p;
    if (i & 0x08) ++p;
    if (i & 0x10) ++p;
    if (i & 0x20) ++p;
    if (i & 0x40) ++p;
    if (i & 0x80) ++p;
      
    Z80->SZ[i] = i ? i & Sflag : Zflag;
    Z80->SZ[i] |= (i & Xflags);
    Z80->SZ_BIT[i] = i ? i & Sflag : Zflag | Pflag;
    Z80->SZ_BIT[i] |= (i & Xflags);
    Z80->SZP[i] = Z80->SZ[i] | ((p & 1) ? 0 : Pflag);

    Z80->SZHV_inc[i] = Z80->SZ[i];
    
    if (i == 0x80)
    {
      Z80->SZHV_inc[i] |= Vflag;
    }
    if ((i & 0x0f) == 0x00)
    {
      Z80->SZHV_inc[i] |= Hflag;
    }
      
    Z80->SZHV_dec[i] = Z80->SZ[i] | Nflag;

    if (i == 0x7f)
    {
      Z80->SZHV_dec[i] |= Vflag;
    }
    if ((i & 0x0f) == 0x0f)
    {
      Z80->SZHV_dec[i] |= Hflag;
    }
  }

  // From MAME
  padd = Z80->SZHVC_add;
  padc = Z80->SZHVC_add + 65536;
  psub = Z80->SZHVC_sub;
  psbc = Z80->SZHVC_sub + 65536;
	for (oldval = 0; oldval < 256; oldval++)
	{
		for (newval = 0; newval < 256; newval++)
		{
			/* add or adc w/o carry set */
			val = newval - oldval;
			*padd = (newval) ? ((newval & 0x80) ? Sflag : 0) : Zflag;
			*padd |= (newval & Xflags);  /* undocumented flag bits 5+3 */
			if( (newval & 0x0f) < (oldval & 0x0f) ) *padd |= Hflag;
			if( newval < oldval ) *padd |= Cflag;
			if( (val^oldval^0x80) & (val^newval) & 0x80 ) *padd |= Vflag;
			padd++;

			/* adc with carry set */
			val = newval - oldval - 1;
			*padc = (newval) ? ((newval & 0x80) ? Sflag : 0) : Zflag;
			*padc |= (newval & Xflags);  /* undocumented flag bits 5+3 */
			if( (newval & 0x0f) <= (oldval & 0x0f) ) *padc |= Hflag;
			if( newval <= oldval ) *padc |= Cflag;
			if( (val^oldval^0x80) & (val^newval) & 0x80 ) *padc |= Vflag;
			padc++;

			/* cp, sub or sbc w/o carry set */
			val = oldval - newval;
			*psub = Nflag | ((newval) ? ((newval & 0x80) ? Sflag : 0) : Zflag);
			*psub |= (newval & Xflags);  /* undocumented flag bits 5+3 */
			if( (newval & 0x0f) > (oldval & 0x0f) ) *psub |= Hflag;
			if( newval > oldval ) *psub |= Cflag;
			if( (val^oldval) & (oldval^newval) & 0x80 ) *psub |= Vflag;
			psub++;

			/* sbc with carry set */
			val = oldval - newval - 1;
			*psbc = Nflag | ((newval) ? ((newval & 0x80) ? Sflag : 0) : Zflag);
			*psbc |= (newval & Xflags);  /* undocumented flag bits 5+3 */
			if( (newval & 0x0f) >= (oldval & 0x0f) ) *psbc |= Hflag;
			if( newval >= oldval ) *psbc |= Cflag;
			if( (val^oldval) & (oldval^newval) & 0x80 ) *psbc |= Vflag;
			psbc++;
		}
	}
}
/*----------------------------------------------------------------------------*/


#ifndef CRTC_ENGINE_V2

static tVoid CRTCInit(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  CRTCInit
 *
 ***********************************************************************/
{
tUShort idx, n;
tUChar p1, p2, p3, p4;

  // Tables initialisation
  idx = 0;
  for (n = 0; n < 256; n++) // calculate mode0 byte-to-pixel translation table
  {
    p1 = ((n & 0x80) >> 7) + ((n & 0x08) >> 2) + ((n & 0x20) >> 3) + ((n & 0x02) << 2);
    p2 = ((n & 0x40) >> 6) + ((n & 0x04) >> 1) + ((n & 0x10) >> 2) + ((n & 0x01) << 3);
    NativeCPC->mode0_table[idx++] = p1;
    NativeCPC->mode0_table[idx++] = p2;
  }

  idx = 0;
  for (n = 0; n < 256; n++) // calculate mode1 byte-to-pixel translation table
  {
    p1 = ((n & 0x80) >> 7) + ((n & 0x08) >> 2);
    p2 = ((n & 0x40) >> 6) + ((n & 0x04) >> 1);
    p3 = ((n & 0x20) >> 5) +  (n & 0x02);
    p4 = ((n & 0x10) >> 4) + ((n & 0x01) << 1);

    NativeCPC->mode1_table[idx++] = p1;
    NativeCPC->mode1_table[idx++] = p2;
    NativeCPC->mode1_table[idx++] = p3;
    NativeCPC->mode1_table[idx++] = p4;
  }
}
/*----------------------------------------------------------------------------*/

#else /* CRTC_ENGINE_V2 */

static tVoid CRTCInit(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  CRTCInit
 *
 ***********************************************************************/
{
tUShort l;

  for (l = 0; l < 0x7400; l++)
  {
    tUShort j = l << 1; // actual address
    NativeCPC->MAXlate[l] = (j & 0x7FE) | ((j & 0x6000) << 1);
  }
}
/*----------------------------------------------------------------------------*/

#endif /* CRTC_ENGINE_V2 */


static tVoid FDCInit(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  FDCInit
 *
 ***********************************************************************/
{
tFDC* FDC = NativeCPC->FDC;

  // Set marker
  FDC->magic = FDC_MAGIC;

  FDC->FDCCommandTable = NativeCPC->FDCCommandTable;

  FDC->idam_crc = CRC_CCITT(fdc_idam, 4);
  FDC->dataam_crc = CRC_CCITT(fdc_dataam, 4);

#ifdef REMOVE_NATIVECPC_DRIVES
  FDC->DriveA->magic = DRIVE_MAGIC;
  FDC->DriveB->magic = DRIVE_MAGIC;
#endif /* REMOVE_NATIVECPC_DRIVES */
}
/*----------------------------------------------------------------------------*/


static tVoid ga_init_banking(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  ga_init_banking
 *
 ***********************************************************************/
{
tUChar *romb0, *romb1, *romb2, *romb3, *romb4, *romb5, *romb6, *romb7;
tUChar *pbRAMbank;

  romb0 = NativeCPC->pbRAM;
  romb1 = NativeCPC->pbRAM + ((tULong)1*16384);
  romb2 = NativeCPC->pbRAM + ((tULong)2*16384);
  romb3 = NativeCPC->pbRAM + ((tULong)3*16384);

  pbRAMbank = NativeCPC->pbRAM + ((NativeCPC->GateArray->RAM_bank + 1) * 65536);
  romb4 = pbRAMbank;
  romb5 = pbRAMbank + ((tULong)1*16384);
  romb6 = pbRAMbank + ((tULong)2*16384);
  romb7 = pbRAMbank + ((tULong)3*16384);

  NativeCPC->membank_config[0][0] = romb0;
  NativeCPC->membank_config[0][1] = romb1;
  NativeCPC->membank_config[0][2] = romb2;
  NativeCPC->membank_config[0][3] = romb3;

  NativeCPC->membank_config[1][0] = romb0;
  NativeCPC->membank_config[1][1] = romb1;
  NativeCPC->membank_config[1][2] = romb2;
  NativeCPC->membank_config[1][3] = romb7;

  NativeCPC->membank_config[2][0] = romb4;
  NativeCPC->membank_config[2][1] = romb5;
  NativeCPC->membank_config[2][2] = romb6;
  NativeCPC->membank_config[2][3] = romb7;

  NativeCPC->membank_config[3][0] = romb0;
  NativeCPC->membank_config[3][1] = romb3;
  NativeCPC->membank_config[3][2] = romb2;
  NativeCPC->membank_config[3][3] = romb7;

  NativeCPC->membank_config[4][0] = romb0;
  NativeCPC->membank_config[4][1] = romb4;
  NativeCPC->membank_config[4][2] = romb2;
  NativeCPC->membank_config[4][3] = romb3;

  NativeCPC->membank_config[5][0] = romb0;
  NativeCPC->membank_config[5][1] = romb5;
  NativeCPC->membank_config[5][2] = romb2;
  NativeCPC->membank_config[5][3] = romb3;

  NativeCPC->membank_config[6][0] = romb0;
  NativeCPC->membank_config[6][1] = romb6;
  NativeCPC->membank_config[6][2] = romb2;
  NativeCPC->membank_config[6][3] = romb3;

  NativeCPC->membank_config[7][0] = romb0;
  NativeCPC->membank_config[7][1] = romb7;
  NativeCPC->membank_config[7][2] = romb2;
  NativeCPC->membank_config[7][3] = romb3;
}
/*----------------------------------------------------------------------------*/


static tVoid audio_init(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  audio_init
 *
 ***********************************************************************/
{
tPSG* PSG = (tPSG*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG);
#ifdef ENABLE_PLAYCITY
tPlayCity* PlayCity = (tPlayCity*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY);
#endif /* ENABLE_PLAYCITY */
tPreferences* prefP = NativeCPC->prefP;
#if defined(__PALMOS__)
tUChar args[4];
#endif /* __PALMOS__ */

  NativeCPC->PSG = PSG;
  
  PSG->Level_PP = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG_LEVEL_PP);
  PSG->Level_AR = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG_LEVEL_AR);
  PSG->Level_AL = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG_LEVEL_AL);
  PSG->Level_BR = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG_LEVEL_BR);
  PSG->Level_BL = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG_LEVEL_BL);
  PSG->Level_CR = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG_LEVEL_CR);
  PSG->Level_CL = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PSG_LEVEL_CL);
  PSG->Level_PLAYCITY = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL);

  PSG->snd_enabled = 0; // Always disabled at start.
  PSG->snd_volume = prefP->SoundVolume;
  PSG->snd_pp_device = 0;

  // Initializes buffer pointers
  PSG->pbSndBuffer = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_SND_BUFFER);
  PSG->pbSndBufferEnd = PSG->pbSndBuffer + SND_BUFFER_SIZE;
  PSG->snd_bufferptr = PSG->pbSndBuffer; // init write cursor
  PSG->FilledBufferSize = 0;

  //
  // Init Levels
  //
#if defined(__PALMOS__)
  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(NativeCPC);
  
  // Calculate_Level_Tables();
  NativeCPC->call68KFuncP(NativeCPC->emulStateP,
                          NativeCPC->SoundCalculateLevelTablesPtr,
                          &args,
                          4);
#elif defined (__WIN32__)
  //
  // Calculate_Level_Tables();
  Sound_Calculate_Level_Tables(PSG, prefP->SpeakerStereo, prefP->Speaker16Bits);
#endif

#ifdef ENABLE_PLAYCITY
  //
  // Init PlayCity
  //
  NativeCPC->PlayCity = PlayCity;

  PlayCity->PSG_Left = (tPSG*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEFT);
  PlayCity->PSG_Right = (tPSG*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_RIGHT);

  // Initializes buffer pointers
  PlayCity->pbSndBuffer = (tUChar*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_BUFFER);
  PlayCity->pbSndBufferEnd = PlayCity->pbSndBuffer + PLAYCITY_BUFFER_SIZE;
  PlayCity->snd_bufferptr = PlayCity->pbSndBuffer; // init write cursor
  PlayCity->FilledBufferSize = 0;

  PlayCity->enabled = 0; // Always disabled at start.
  PlayCity->snd_enabled = 0; // Always disabled at start.
  PlayCity->snd_volume = prefP->SoundVolume;

  // Set levels
  PlayCity->PSG_Left->Level_PP = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL_PP);
  PlayCity->PSG_Left->Level_AR = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL_AR);
  PlayCity->PSG_Left->Level_AL = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL_AL);
  PlayCity->PSG_Left->Level_BR = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL_BR);
  PlayCity->PSG_Left->Level_BL = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL_BL);
  PlayCity->PSG_Left->Level_CR = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL_CR);
  PlayCity->PSG_Left->Level_CL = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL_CL);
  PlayCity->PSG_Left->Level_PLAYCITY = (tLong*)(NativeCPC->contextP + CONTEXT_OFFSET_PLAYCITY_LEVEL);
  PlayCity->PSG_Right->Level_PP = PlayCity->PSG_Left->Level_PP;
  PlayCity->PSG_Right->Level_AR = PlayCity->PSG_Left->Level_AR;
  PlayCity->PSG_Right->Level_AL = PlayCity->PSG_Left->Level_AL;
  PlayCity->PSG_Right->Level_BR = PlayCity->PSG_Left->Level_BR;
  PlayCity->PSG_Right->Level_BL = PlayCity->PSG_Left->Level_BL;
  PlayCity->PSG_Right->Level_CR = PlayCity->PSG_Left->Level_CR;
  PlayCity->PSG_Right->Level_CL = PlayCity->PSG_Left->Level_CL;
  PlayCity->PSG_Right->Level_PLAYCITY = PlayCity->PSG_Left->Level_PLAYCITY;
  Sound_Calculate_Level_Tables(PlayCity->PSG_Left, prefP->PlayCityStereo, prefP->PlayCity16Bits);
#endif /* ENABLE_PLAYCITY */
}
/*----------------------------------------------------------------------------*/


static tVoid TapeInit(tTape* Tape)
/***********************************************************************
 *
 *  InitTape
 *
 ***********************************************************************/
{
tUChar* table_ptr;
tULong byte_loop;
tUChar bit_loop;
tUChar temp;

  Tape->dwTapeStage = TAPE_END;

  Tape->pbTapeImage = cNull;
  Tape->pbTapeImageEnd = cNull;

  Tape->magic = TAPE_MAGIC;

  Tape->dwTapeSpeedAdjust = 100;

  //
  // Create bit1 table
  //
  table_ptr = Tape->onebit_table;
  for (byte_loop=0; byte_loop < 256; byte_loop++)
  {
    *table_ptr = 0;
    temp = byte_loop;
    for (bit_loop=0; bit_loop < 8; bit_loop++)
    {
      if (temp & 0x01)
      {
        (*table_ptr)++;
      }
      temp >>= 1;
    }
    table_ptr++;
  }
}
/*----------------------------------------------------------------------------*/


#ifdef ENABLE_TMPI_SPEECH
static tVoid TMPISpeechInit(tTMPISpeech* TMPISpeech)
/***********************************************************************
 *
 *  TMPISpeechInit
 *
 ***********************************************************************/
{
tLong i;
tULong seed;

  TMPISpeech->magic = TMPI_SPEECH_MAGIC;

	for (i=0; i < TMPI_TABLE_LEN; i++)
	{
		tDouble f = (tDouble)i / TMPI_F0;
		TMPISpeech->cos_table[i]  = (tLong)(2. * cos(2.*M_PI*f) * TMPI_QUANT);
		TMPISpeech->exp_table[i]  = (tLong)(exp(-M_PI*f) * TMPI_QUANT);
		TMPISpeech->exp2_table[i] = (tLong)(exp(-2*M_PI*f) * TMPI_QUANT);
	}

  seed = (tULong)TMPISpeech; // Start random generation using random pointer address
	for (i=0; i < TMPI_NOISE_LEN; i++)
  {
		TMPISpeech->noise_table[i] = ((tLong)(rand(&seed) * 2. * TMPI_QUANT)) - TMPI_QUANT;
    TMPISpeech->noise_table[i] /= 2; // Noise reduction
  }

  // Buffer pointers
  TMPISpeech->pbBufferEnd = TMPISpeech->pbBufferStart + SIZETAB_TMPI_BUFFER;
  TMPISpeech->bufferptr = (tShort*)TMPISpeech->pbBufferStart;
}
/*----------------------------------------------------------------------------*/
#endif /* ENABLE_TMPI_SPEECH */


#ifdef ENABLE_PRINTER
static tVoid PrinterInit(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PrinterInit
 *
 ***********************************************************************/
{
tPrinter* Printer = NativeCPC->Printer;

  Printer->magic = PRINTER_MAGIC;

  Printer->busy = 1;
}
/*----------------------------------------------------------------------------*/
#endif /* ENABLE_PRINTER */


//==============================================================================
//
// Routines
//
//==============================================================================
static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value)
/***********************************************************************
 *
 *  MemSet
 *
 ***********************************************************************/
{
  while (numBytes--)
  {
    *(destP++) = value;
  }
}
/*----------------------------------------------------------------------------*/


//==============================================================================
//
// Palm OS routines
//
//==============================================================================

#if defined(__PALMOS__)
static tVoid* GetResourcePtr(DmResID resID,
                             const tVoid* emulStateP,
                             Call68KFuncType* call68KFuncP)
/***********************************************************************
 *
 *  GetResourcePtr
 *
 ***********************************************************************/
{

tUChar args[6]; // Size should be multiple of 2

  // MemHandle DmGetResource(DmResType type, DmResID resID);

  // DmGetResource Parameters
  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(ROM_ID_TYPE);
  *((tUShort*)(&(args[4]))) = (tUShort)EndianSwap16(resID);
  
  return (tVoid*)call68KFuncP(emulStateP,
                              PceNativeTrapNo(sysTrapDmGetResource),
                              &args,
                              (sizeof(args)/sizeof(args[0])) | kPceNativeWantA0);
}
/*----------------------------------------------------------------------------*/


static tVoid* MemHandleLock(tVoid* h,
                            const tVoid* emulStateP,
                            Call68KFuncType* call68KFuncP)
/***********************************************************************
 *
 *  MemHandleLock
 *
 ***********************************************************************/
{
tUChar args[4]; // Size should be multiple of 2

  // MemPtr MemHandleLock(MemHandle h);

  // MemHandleLock Parameters
  *((tULong*)(&(args[0]))) = (tULong)EndianSwap32(h);
  
  return (tVoid*)call68KFuncP(emulStateP,
                              PceNativeTrapNo(sysTrapMemHandleLock),
                              &args, 
                              (sizeof(args)/sizeof(args[0])) | kPceNativeWantA0);
}
/*----------------------------------------------------------------------------*/
#endif /* __PALMOS__ */


//==============================================================================
//
// Unitary Tests
//
//==============================================================================
#ifdef _TESTU
// Prototypes of TestU fonctions
static tUShort TestU_DWORD_UPPER_ALIGN(tNativeCPC* NativeCPC,
                                       tUChar NoTest);
static tUShort TestU_MemoryAllocation(tNativeCPC* NativeCPC,
                                      tUChar NoTest);
#ifdef __PALMOS__
static tUShort TestU_MemoryPointers(tNativeCPC* NativeCPC,
                                    tUChar NoTest);
#endif /* __PALMOS__ */


static tUShort PerformTestU(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PerformTestU
 *
 ***********************************************************************/
{
tUShort Result = errNone;
tUChar NoTest = 1;

  /* 1 */ if (Result == errNone) Result = TestU_DWORD_UPPER_ALIGN(NativeCPC,
                                                                  NoTest++);
  /* 2 */ if (Result == errNone) Result = TestU_MemoryAllocation(NativeCPC,
                                                                 NoTest++);
#ifdef __PALMOS__
  /* 3 */ if (Result == errNone) Result = TestU_MemoryPointers(NativeCPC,
                                                               NoTest++);
#endif /* __PALMOS__ */
  
  return (Result);
}
/*----------------------------------------------------------------------------*/


static tUShort TestU_DWORD_UPPER_ALIGN(tNativeCPC* NativeCPC,
                                       tUChar NoTest)
/***********************************************************************
 *
 *  TestU_DWORD_UPPER_ALIGN
 *
 ***********************************************************************/
{
tUShort Result = errNone;

#if defined(__BORLANDC__)
#pragma warn -8008
#pragma warn -8066
#endif

	NOT_USED(NativeCPC);

  if (DWORD_UPPER_ALIGN(0) != 0)
  {
    Result = testUErrorClass + NoTest;
  }
  if ( (DWORD_UPPER_ALIGN(1) != 4) ||
       (DWORD_UPPER_ALIGN(3) != 4) ||
       (DWORD_UPPER_ALIGN(127) != 128) )
  {
    Result = testUErrorClass + NoTest;
  }
  if ( (DWORD_UPPER_ALIGN(64) != 64) ||
       (DWORD_UPPER_ALIGN(256) != 256) ||
       (DWORD_UPPER_ALIGN(65536) != 65536) )
  {
    Result = testUErrorClass + NoTest;
  }
  if ( (DWORD_UPPER_ALIGN(1457) != 1460) ||
       (DWORD_UPPER_ALIGN(75079) != 75080) )
  {
    Result = testUErrorClass + NoTest;
  }

#if defined(__BORLANDC__)
#pragma warn +8008
#pragma warn +8066
#endif
  
  return (Result);
}
/*----------------------------------------------------------------------------*/

static tUShort TestU_MemoryAllocation(tNativeCPC* NativeCPC,
                                      tUChar NoTest)
/***********************************************************************
 *
 *  TestU_MemoryAllocation
 *
 ***********************************************************************/
{
tUShort Result = errNone;
tUChar* contexteP = NativeCPC->contextP;

  if ( ((tULong)NativeCPC != ((tULong)contexteP + CONTEXT_OFFSET_NATIVECPC)) ||
       ((tULong)NativeCPC->Z80 != ((tULong)contexteP + CONTEXT_OFFSET_Z80)) ||
       ((tULong)NativeCPC->Z80->SZ != ((tULong)contexteP + CONTEXT_OFFSET_Z80_SZ)) ||
       ((tULong)NativeCPC->Z80->SZ_BIT != ((tULong)contexteP + CONTEXT_OFFSET_Z80_SZ_BIT)) ||
       ((tULong)NativeCPC->Z80->SZP != ((tULong)contexteP + CONTEXT_OFFSET_Z80_SZP)) ||
       ((tULong)NativeCPC->Z80->SZHV_inc != ((tULong)contexteP + CONTEXT_OFFSET_Z80_SZHV_inc)) ||
       ((tULong)NativeCPC->Z80->SZHV_dec != ((tULong)contexteP + CONTEXT_OFFSET_Z80_SZHV_dec)) ||
       ((tULong)NativeCPC->CRTC != ((tULong)contexteP + CONTEXT_OFFSET_CRTC)) ||
       ((tULong)NativeCPC->mode0_table != ((tULong)contexteP + CONTEXT_OFFSET_MODE0_TABLE)) ||
       ((tULong)NativeCPC->mode1_table != ((tULong)contexteP + CONTEXT_OFFSET_MODE1_TABLE)) ||
       ((tULong)NativeCPC->GateArray != ((tULong)contexteP + CONTEXT_OFFSET_GATEARRAY)) ||
       ((tULong)NativeCPC->FDC != ((tULong)contexteP + CONTEXT_OFFSET_FDC)) ||
       ((tULong)NativeCPC->FDC->DriveA != ((tULong)contexteP + CONTEXT_OFFSET_DRIVE_A)) ||
       ((tULong)NativeCPC->FDC->DriveB != ((tULong)contexteP + CONTEXT_OFFSET_DRIVE_B)) ||
       ((tULong)NativeCPC->PPI != ((tULong)contexteP + CONTEXT_OFFSET_PPI)) ||
       ((tULong)NativeCPC->PSG != ((tULong)contexteP + CONTEXT_OFFSET_PSG)) ||
       ((tULong)NativeCPC->PSG->Level_PP != ((tULong)contexteP + CONTEXT_OFFSET_PSG_LEVEL_PP)) ||
       ((tULong)NativeCPC->PSG->Level_AR != ((tULong)contexteP + CONTEXT_OFFSET_PSG_LEVEL_AR)) ||
       ((tULong)NativeCPC->PSG->Level_AL != ((tULong)contexteP + CONTEXT_OFFSET_PSG_LEVEL_AL)) ||
       ((tULong)NativeCPC->PSG->Level_BR != ((tULong)contexteP + CONTEXT_OFFSET_PSG_LEVEL_BR)) ||
       ((tULong)NativeCPC->PSG->Level_BL != ((tULong)contexteP + CONTEXT_OFFSET_PSG_LEVEL_BL)) ||
       ((tULong)NativeCPC->PSG->Level_CR != ((tULong)contexteP + CONTEXT_OFFSET_PSG_LEVEL_CR)) ||
       ((tULong)NativeCPC->PSG->Level_CL != ((tULong)contexteP + CONTEXT_OFFSET_PSG_LEVEL_CL)) ||
       ((tULong)NativeCPC->PSG->pbSndBuffer != ((tULong)contexteP + CONTEXT_OFFSET_SND_BUFFER)) ||
       ((tULong)NativeCPC->VDU != ((tULong)contexteP + CONTEXT_OFFSET_VDU)) ||
       ((tULong)NativeCPC->TMPISpeech != ((tULong)contexteP + CONTEXT_OFFSET_TMPI_SPEECH)) ||
       ((tULong)NativeCPC->pbGPBuffer != ((tULong)contexteP + CONTEXT_OFFSET_GPBUFFER)) ||
       ((tULong)NativeCPC->memmap_ROM != ((tULong)contexteP + CONTEXT_OFFSET_MEMMAP_ROM)) ||
       ((tULong)NativeCPC->pbRAM != ((tULong)contexteP + CONTEXT_OFFSET_RAM)) )
  {
    Result = testUErrorClass + NoTest;
  }

  return (Result);
}
/*----------------------------------------------------------------------------*/


#ifdef __PALMOS__

static tUShort TestU_MemoryPointers(tNativeCPC* NativeCPC,
                                    tUChar NoTest)
/***********************************************************************
 *
 *  TestU_MemoryPointers
 *
 ***********************************************************************/
{
tUShort Result = errNone;

  //
  // tNativeCPC
  //
  if ( ((tULong)NativeCPC + STATIC_SIZE_NATIVECPC) != (tULong)&NativeCPC->DriveA )
  {
    Result = testUErrorClass + NoTest;
  }
  if ( ((tULong)NativeCPC + STATIC_SIZE_NATIVECPC + DYNAMIC_SIZE_NATIVECPC) != 
       (tULong)&NativeCPC->dwTapePulseCycles )
  {
    Result = testUErrorClass + NoTest;
  }
  
  //
  // tZ80
  //
  if ( ((tULong)NativeCPC->Z80 + STATIC_SIZE_Z80) != (tULong)&NativeCPC->Z80->iCycleCount )
  {
    Result = testUErrorClass + NoTest;
  }

  //
  // tPSG
  //
  if ( ((tULong)NativeCPC->PSG + STATIC_SIZE_PSG) != (tULong)&NativeCPC->PSG->snd_bufferptr )
  {
    Result = testUErrorClass + NoTest;
  }
  if ( ((tULong)NativeCPC->PSG + STATIC_SIZE_PSG + DYNAMIC_SIZE_PSG) != (tULong)&NativeCPC->PSG->Case_Env )
  {
    Result = testUErrorClass + NoTest;
  }

  //
  // tFDC
  //
  if ( ((tULong)NativeCPC->FDC + STATIC_SIZE_FDC) != (tULong)&NativeCPC->FDC->active_drive )
  {
    Result = testUErrorClass + NoTest;
  }
  if ( ((tULong)NativeCPC->FDC + STATIC_SIZE_FDC + DYNAMIC_SIZE_FDC) != (tULong)&NativeCPC->FDC->cmd_handler )
  {
    Result = testUErrorClass + NoTest;
  }

  return (Result);
}
/*----------------------------------------------------------------------------*/

#endif /* __PALMOS__ */

#endif /* _TESTU */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

