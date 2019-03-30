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

#if defined(__PALMOS__)
#include <PceNativeCall.h>
#include <ByteOrderUtils.h>
#endif /* __PALMOS__ */

#include "..\Native_CPC.h"
#include <Math.h>


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


static tVoid Z80Reset(tNativeCPC* NativeCPC);
static tVoid CPCReset(tNativeCPC* NativeCPC);
static tVoid VDUReset(tNativeCPC* NativeCPC);
static tVoid CRTCReset(tNativeCPC* NativeCPC);
static tVoid GateArrayReset(tNativeCPC* NativeCPC);
static tVoid ga_init_banking(tNativeCPC* NativeCPC);
static tVoid PPIReset(tNativeCPC* NativeCPC);
static tVoid PSGReset(tPSG* PSG);
tVoid PlayCity_reset(tPlayCity* PlayCity);
static tVoid FDCReset(tNativeCPC* NativeCPC);
static tVoid TapeReset(tTape* Tape);
static tVoid TMPISpeechReset(tTMPISpeech* TMPISpeech);
static tVoid PrinterReset(tNativeCPC* NativeCPC);

//
// Routines
//
static tVoid MemSet(tUChar* destP,
                    tULong numBytes,
                    tUChar value);
//
// TestU
//
#ifdef _TESTU
static tUShort PerformTestU(tNativeCPC* NativeCPC);
#endif /* _TESTU */


// CRTC Reset values
static const tUChar CRTC_values[2][14] =
{
   /*            R0,   R1,   R2,   R3,   R4,   R5,   R6,   R7,   R8,   R9,  R10,  R11,  R12,  R13 */
   /* 60Hz */ {0x3f, 0x28, 0x2e, 0x8e, 0x1f, 0x06, 0x19, 0x1b, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00},
   /* 50Hz */ {0x3f, 0x28, 0x2e, 0x8e, 0x26, 0x00, 0x19, 0x1e, 0x00, 0x07, 0x00, 0x00, 0x30, 0x00}
};


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
tULong Engine_CPCReset(tNativeCPC* NativeCPC)
#endif /* __PALMOS__ || __WIN32__ */
/***********************************************************************
 *
 *  PNOMain
 *
 ***********************************************************************/
{
#if defined(__PALMOS__)
tNativeCPC* NativeCPC = (tNativeCPC*)userData68KP;
#endif /* __PALMOS__ || __WIN32__ */
tUChar n;

#if defined(__PALMOS__)
  NOT_USED(emulStateP);
  NOT_USED(call68KFuncP);
#endif /* __PALMOS__ */

  Z80Reset(NativeCPC);
  CPCReset(NativeCPC);
  VDUReset(NativeCPC);
  CRTCReset(NativeCPC);
  GateArrayReset(NativeCPC);
  PPIReset(NativeCPC);
  PSGReset(NativeCPC->PSG);
  PlayCity_reset(NativeCPC->PlayCity);
  FDCReset(NativeCPC);
  TapeReset(NativeCPC->Tape);
#ifdef ENABLE_TMPI_SPEECH
  TMPISpeechReset(NativeCPC->TMPISpeech);
#endif /* ENABLE_TMPI_SPEECH */
#ifdef ENABLE_PRINTER
  PrinterReset(NativeCPC);
#endif /* ENABLE_PRINTER */

#ifndef CRTC_ENGINE_V2
  // Prepare video rendering.
  NativeCPC->VDU->ram_addr = NativeCPC->CRTC->addr;
  if (NativeCPC->VDU->ram_addr & 0x2000) // 32K screen?
  {
    NativeCPC->VDU->ram_addr += 0x4000; // advance to next 16K segment
  }
  NativeCPC->VDU->ram_addr &= 0xC7FF; // apply 11000111 mask to MSB (bits 15-8)
  NativeCPC->VDU->ram_addr += (tULong)NativeCPC->pbRAM;
#endif /* CRTC_ENGINE_V2 */

  // clear all memory used for CPC RAM
  MemSet((tUChar*)NativeCPC->pbRAM,
         ((tULong)NativeCPC->ram_size*1024),
         0);

#ifdef USE_SILICON_DISC
  if (NativeCPC->prefP->Use256kSiliconDisc)
  {
    // clear all memory used for CPC RAM
    MemSet((tUChar*)NativeCPC->pbRAM + (320*1024),
           ((tULong)256*1024),
           0);
  }
#endif /* USE_SILICON_DISC */

  for (n = 0; n < 4; n++) // initialize active read/write bank configuration
  {
    NativeCPC->membank_read[n] = NativeCPC->membank_config[0][n];
    NativeCPC->membank_write[n] = NativeCPC->membank_config[0][n];
  }
  NativeCPC->membank_read[0] = NativeCPC->pbROMlo; // 'page in' lower ROM
  NativeCPC->membank_read[3] = NativeCPC->pbROMhi; // 'page in' upper ROM

  NativeCPC->cycle_count = CYCLE_COUNT_INIT;

  NativeCPC->lightgun_beam_detect = 0;
  NativeCPC->lightgun_beam_state = 0;
  
#ifdef ENABLE_METROLOGY
  NativeCPC->timer_from_reset = 0;
#endif /* ENABLE_METROLOGY */

#ifdef _TESTU
  return PerformTestU(NativeCPC);
#else /* _TESTU */
  return errNone;
#endif /* _TESTU */
}
/*----------------------------------------------------------------------------*/


static tVoid Z80Reset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  Z80Reset
 *
 ***********************************************************************/
{
tZ80* Z80 = NativeCPC->Z80;
tULong old_breakpoint;

  // Save breakpoint
  old_breakpoint = Z80->Regs.breakpoint;

  MemSet((tUChar*)&Z80->Regs,
         sizeof(tZ80Regs),
         0); // clear result codes buffer
         
  // Set marker
  Z80->magic = Z80_MAGIC; 

  _IX = _IY = 0xffff; // IX and IY are FFFF after a reset!
  _F = Zflag; // set zero flag
  Z80->Regs.breakpoint = old_breakpoint; // restore breakpoint
}
/*----------------------------------------------------------------------------*/


static tVoid CPCReset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  CPCReset
 *
 ***********************************************************************/
{
  // clear CPC keyboard matrix
  MemSet((tUChar*)NativeCPC->keyboard_matrix,
         sizeof(NativeCPC->keyboard_matrix),
         0xff);
  
  NativeCPC->printer_port = 0xff;

  // Set marker
  NativeCPC->magic = NATIVECPC_MAGIC;
}
/*----------------------------------------------------------------------------*/


static tVoid VDUReset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  VDUReset
 *
 ***********************************************************************/
{
tVDU* VDU = NativeCPC->VDU;

  // clear VDU data structure 
  MemSet((tUChar*)VDU,
         sizeof(tVDU),
         0);
  
  VDU->scr_base = (tULong*)NativeCPC->BmpOffScreenBits; // reset to surface start
  VDU->scr_line_offs = NativeCPC->BmpOffScreenBytesRow / 4;

#ifndef CRTC_ENGINE_V2
  VDU->hsw = VDU->hsw_active = 4;
  VDU->scr_current = VDU->scr_base;
  VDU->palette = NativeCPC->GateArray->palette;
  VDU->pbRAM = NativeCPC->pbRAM;
  VDU->mode0_table = NativeCPC->mode0_table;
  VDU->mode1_table = NativeCPC->mode1_table;
#else /* CRTC_ENGINE_V2 */
  VDU->scr_pos = VDU->scr_base;
  VDU->MAXlate = NativeCPC->MAXlate;
  VDU->flag_drawing = 1;
#endif /* CRTC_ENGINE_V2 */

  //VDU->vhold_min = MIN_VHOLD;
  VDU->vhold_min = NativeCPC->vhold_adjust;
  VDU->vhold_max = MAX_VHOLD;
  VDU->vhold_mid = MID_VHOLD;
  VDU->vhold_range_min = MIN_VHOLD_RANGE;
  VDU->vhold_range_max = MAX_VHOLD_RANGE;

  // Set marker
  VDU->magic = VDU_MAGIC;
}
/*----------------------------------------------------------------------------*/


static tVoid CRTCReset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  CRTCReset
 *
 ***********************************************************************/
{
tCRTC* CRTC = NativeCPC->CRTC;
#ifndef CRTC_ENGINE_V2
tDrawHandlerPtr oldDrawHandler[CRTC_DRAWHANDLER_NBELEMENT];
#else /* CRTC_ENGINE_V2 */
tCRTCPtr OldCharInstMR;
tCRTCPtr OldCharInstSL;
tNativeCPCPtr OldRender;
const tULong* OldModeMaps[CRTC_MODEMAPS_NBELEMENT];
const tULong* OldModeMap;
#endif /* CRTC_ENGINE_V2 */
tUChar n, val1, val2;
#ifdef CRTC_ENGINE_V2
tLong Wid;
tULong i;
#endif /* CRTC_ENGINE_V2 */

  //
  // Save Static variables
  //
#ifndef CRTC_ENGINE_V2
  for (n = 0; n < CRTC_DRAWHANDLER_NBELEMENT; n++)
  {
    oldDrawHandler[n] = CRTC->DrawHandler[n];
  }
#else /* CRTC_ENGINE_V2 */
  OldCharInstMR = CRTC->StartCharInstMR;
  OldCharInstSL = CRTC->StartCharInstSL;
  OldRender = CRTC->Render;
  for (n = 0; n < CRTC_MODEMAPS_NBELEMENT; n++)
  {
    OldModeMaps[n] = CRTC->ModeMaps[n];
  }
  OldModeMap = CRTC->StartModeMap;
#endif /* CRTC_ENGINE_V2 */

  // Init CRTC
  MemSet((tUChar*)CRTC,
         sizeof(tCRTC),
         0); // clear CRTC data structure

  // program CRTC with 'valid' data
  for (n = 0; n < 14; n++)
  {
    CRTC->registers[n] = CRTC_values[(NativeCPC->jumpers & 0x10) >> 4][n];
  }

#ifndef CRTC_ENGINE_V2

  CRTC->flags = HDT_flag | VDT_flag;
  CRTC->hsw = CRTC->hsw_active = CRTC->registers[3] & 0x0f;
  CRTC->vsw = CRTC->registers[3] >> 4;
  //CRTC->vt_adjust = CRTC->registers[5] & 0x1f;
  CRTC->skew = (CRTC->registers[8] >> 4) & 3;
  if (CRTC->skew == 3) // no output?
  {
    CRTC->skew = 0xff;
  }
  CRTC->max_raster = CRTC->registers[9] << 3;
  val1 = CRTC->registers[12] & 0x3f;
  val2 = val1 & 0x0f; // isolate screen size
  val1 = (val1 << 1) & 0x60; // isolate CPC RAM bank
  val2 |= val1; // combine
  CRTC->addr = CRTC->requested_addr = (CRTC->registers[13] + (val2 << 8)) << 1;
  CRTC->last_hdisp = 0x28;
  
  CRTC->current_mode = CRTC->requested_mode = 1; // Start in Mode 1

  // Restore static variables
  for (n = 0; n < CRTC_DRAWHANDLER_NBELEMENT; n++) 
  {
    CRTC->DrawHandler[n] = oldDrawHandler[n];
  }

#else /* CRTC_ENGINE_V2 */

  Wid = 16;
  //CRTC->PosShift = 4;

  for (i = 0; i < 48; i++)
  {
    CRTC->HorzPix[i] = Wid;
  }
  CRTC->HorzPix[48] = 0;
  CRTC->RendStart = (tULong *)&CRTC->RendBuff[Wid];
  CRTC->RendPos = (tULong *)&CRTC->RendBuff[0];
  CRTC->RendOut = (tUChar *)CRTC->RendStart;
  CRTC->RendWid = &CRTC->HorzPix[0];

  CRTC->HorzPos = 0x500;
  CRTC->HorzChar = 0x04;
  CRTC->HorzMax = 48;
  CRTC->HSyncDuration = 0xA00;
  CRTC->MinHSync = 0x4000 - CRTC->HSyncDuration - 257;
  CRTC->MaxHSync = 0x4000 - CRTC->HSyncDuration + 257;
  CRTC->MonHSYNC = 0x4000 - CRTC->HSyncDuration;
  CRTC->MonFreeSync = CRTC->MonHSYNC;
  CRTC->flags1.monVSYNC = 0;
  CRTC->flags1.dt.DISPTIMG = 0xff;
  CRTC->flags1.dt.HDSPTIMG = 0x03;
  CRTC->new_dt.NewDISPTIMG = 0xff;
  CRTC->new_dt.NewHDSPTIMG = 0x03;

  CRTC->MinVSync = MID_VHOLD;
  CRTC->MaxVSync = CRTC->MinVSync + MIN_VHOLD_RANGE +
     (int)ceil((float)((CRTC->MinVSync - MIN_VHOLD) * (MAX_VHOLD_RANGE - MIN_VHOLD_RANGE) / (MAX_VHOLD - MIN_VHOLD)));

  // Restore static variables
  CRTC->StartCharInstMR = CRTC->CharInstMR = OldCharInstMR;
  CRTC->StartCharInstSL = CRTC->CharInstSL = OldCharInstSL;
  CRTC->Render = OldRender;
  for (n = 0; n < CRTC_MODEMAPS_NBELEMENT; n++)
  {
    CRTC->ModeMaps[n] = OldModeMaps[n];
  }
  CRTC->StartModeMap = CRTC->ModeMap = OldModeMap;

  //CRTC->lpen_strobe = 1;

  CRTC->sync_dec_max = MAX_SYNC_DEC;
  CRTC->sync_inc_max = MAX_SYNC_INC;

#endif /* CRTC_ENGINE_V2 */

  // Set marker
  CRTC->magic = CRTC_MAGIC;

  // Cursor
  CRTC->cursor_start_ras = 0;
  CRTC->cursor_end_ras = 0;
  CRTC->cursor_addr = 0;
  CRTC->cursor_state = 0;
  CRTC->cursor_blink_count = 0;
}
/*----------------------------------------------------------------------------*/


static tVoid GateArrayReset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  GateArrayReset
 *
 ***********************************************************************/
{
tGateArray* GateArray = NativeCPC->GateArray;
const tUChar* BasicColorsP;
const tUChar* GateArrayColorsP;

  BasicColorsP = GateArray->BasicColorsP;
  GateArrayColorsP = GateArray->GateArrayColorsP;

  // clear GA data structure
  MemSet((tUChar*)GateArray,
         sizeof(tGateArray),
         0);

  GateArray->BasicColorsP = BasicColorsP;
  GateArray->GateArrayColorsP = GateArrayColorsP;

  ga_init_banking(NativeCPC);

#ifdef CRTC_ENGINE_V2
  GateArray->scr_mode = GateArray->requested_scr_mode = 1; // set to mode 1
#endif /* CRTC_ENGINE_V2 */

  // Set marker
  GateArray->magic = GA_MAGIC;
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


static tVoid PPIReset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PPIReset
 *
 ***********************************************************************/
{
  // clear PPI data structure
  MemSet((tUChar*)NativeCPC->PPI,
         sizeof(tPPI),
         0);

  // Set marker
  NativeCPC->PPI->magic = PPI_MAGIC;
}
/*----------------------------------------------------------------------------*/


static tVoid PSGReset(tPSG* PSG)
/***********************************************************************
 *
 *  PSGReset
 *
 ***********************************************************************/
{
tULong Loop;

  PSG->control = 0;
  
  PSG->Ton_Counter_A = 0;
  PSG->Ton_Counter_B = 0;
  PSG->Ton_Counter_C = 0;
  
  PSG->Noise_Counter = 0;
  
  PSG->Envelope_Counter = 0;
  
  PSG->Channel_EnA = 1;
  PSG->Channel_EnB = 1;
  PSG->Channel_EnC = 1;

  PSG->Ton_A = 0;
  PSG->Ton_B = 0;
  PSG->Ton_C = 0;

  PSG->Left_Chan = 0;
  PSG->Right_Chan = 0;

  PSG->AmplitudeEnv = 32;

  PSG->Noise.Seed = 0xffff;
  
  PSG->cycle_count = 0;
  PSG->LoopCount = SND_LOOP_COUNT_INIT;

  // Set marker
  PSG->magic = PSG_MAGIC;

  // Init registers
  for (Loop=0; Loop < PSG_REGISTERS_NBELEMENT; Loop++)
  {
    PSG->RegisterAY.Index[Loop] = 0;
  }
}
/*----------------------------------------------------------------------------*/


tVoid PlayCity_reset(tPlayCity* PlayCity)
/***********************************************************************
 *
 *  PlayCity_reset
 *
 ***********************************************************************/
{
tUChar Loop;

  // Set marker
  PlayCity->magic = PLAYCITY_MAGIC;

  PlayCity->Freq = 4000000;
  PlayCity->LoopCountInit = (((PlayCity->Freq / 2) / 8) << PLAYCITY_LOOP_COUNT_SHIFT) / SND_FREQ;
  PlayCity->LoopCount = PlayCity->LoopCountInit;

  PlayCity->Left_Chan = 0;
  PlayCity->Right_Chan = 0;

  PlayCity->buffer_full = 0;

  // Reset CTC registers
  for (Loop=0; Loop < CTC_NB_CHANNELS; Loop++)
  {
    PlayCity->CTC_control_word[Loop] = 0;
    PlayCity->CTC_time_constant[Loop] = 0;
    PlayCity->CTC_counter[Loop] = 0;
    PlayCity->CTC_timer[Loop] = 0;
  }

  PSGReset(PlayCity->PSG_Left);
  PSGReset(PlayCity->PSG_Right);
}
/*----------------------------------------------------------------------------*/


static tVoid FDCReset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  FDCReset
 *
 ***********************************************************************/
{
tFDC* FDC = NativeCPC->FDC;

  FDC->phase = CMD_PHASE;
  FDC->flags = STATUSDRVA_flag | STATUSDRVB_flag;

  FDC->main_status_register = MSR_RQM;
  FDC->motor_state = FDC_MOTOR_STOPPED;
  FDC->motor = 0;
  FDC->led = 0;
  FDC->phase = CMD_PHASE;
  FDC->byte_count = 0;
  FDC->buffer_count = 0;

  FDC->on_index_ptr = 0;
  FDC->on_index_end_operation_ptr = 0;
  FDC->on_sector_ptr = 0;
  FDC->on_sector_chrn_ptr = 0;
  FDC->on_sector_data_ptr = 0;
  FDC->on_end_of_transfer_ptr = 0;
  FDC->on_data_register_ptr = 0;
  FDC->on_abort_ptr = 0;
  FDC->head_on_track_ptr = 0;
  FDC->head_step_ptr = 0;

  FDC->motor_start_stop_timer = 0;
  FDC->index_timer = 0;
  FDC->sector_timer = 0;
  FDC->head_on_track_timer = 0;
  FDC->head_step_timer = 0;
}
/*----------------------------------------------------------------------------*/


static tVoid TapeReset(tTape* Tape)
/***********************************************************************
 *
 *  TapeReset
 *
 ***********************************************************************/
{
  Tape->tape_motor = 0;
  Tape->relay_activations = 0;
}
/*----------------------------------------------------------------------------*/


#ifdef ENABLE_TMPI_SPEECH
static tVoid TMPISpeechReset(tTMPISpeech* TMPISpeech)
/***********************************************************************
 *
 *  TMPISpeechReset
 *
 ***********************************************************************/
{
tUChar i;

	TMPISpeech->phi = 0;
	TMPISpeech->cont = 0;
	TMPISpeech->roe = 0;
	TMPISpeech->state = MEA8000_STOPPED;
  TMPISpeech->sample = 0;
  TMPISpeech->lastsample = 0;
  TMPISpeech->framelength = 0;
  TMPISpeech->cycle_count = TMPI_CYCLE_INIT;

	for (i=0; i<4; i++)
	{
		TMPISpeech->f[i].last_output = 0;
		TMPISpeech->f[i].output = 0;
	}

  TMPISpeech->bufferptr = (tShort*)TMPISpeech->pbBufferStart;
  TMPISpeech->FilledBufferSize = 0;
}
/*----------------------------------------------------------------------------*/
#endif /* ENABLE_TMPI_SPEECH */


#ifdef ENABLE_PRINTER
static tVoid PrinterReset(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PrinterReset
 *
 ***********************************************************************/
{
tPrinter* Printer = NativeCPC->Printer;

  Printer->data_counter = 0;
  Printer->busy_counter = 0;

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
// Unitary Tests
//
//==============================================================================
#ifdef _TESTU
// Prototypes of TestU fonctions
static tUShort TestU_MemSet_1(tNativeCPC* NativeCPC,
                              tUChar NoTest);


static tUShort PerformTestU(tNativeCPC* NativeCPC)
/***********************************************************************
 *
 *  PerformTestU
 *
 ***********************************************************************/
{
tUShort Result = errNone;
tUChar NoTest = 1;

  if (Result == errNone) Result = TestU_MemSet_1(NativeCPC,
                                                 NoTest++);
  
  return (Result);
}
/*----------------------------------------------------------------------------*/


static tUShort TestU_MemSet_1(tNativeCPC* NativeCPC,
                              tUChar NoTest)
/***********************************************************************
 *
 *  TestU_MemSet_1
 *
 ***********************************************************************/
{
tUChar ArrayA[100];
tUChar Loop;
tUChar Result = errNone;

	NOT_USED(NativeCPC);

  // Prepare conditions
  for (Loop=0; Loop<100; Loop++)
    ArrayA[Loop] = 0x55;
  // Perform operation
  MemSet(ArrayA, 95, 0xAA);
  // Check Result
  for (Loop=0; Loop<95; Loop++)
  {
    if (ArrayA[Loop] != 0xAA)
      Result=testUErrorClass+NoTest;
  }
  for (Loop=95; Loop<100; Loop++)
  {
    if (ArrayA[Loop] != 0x55)
      Result=testUErrorClass+NoTest;
  }
  
  return (Result);
}
/*----------------------------------------------------------------------------*/

#endif /* _TESTU */


#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

