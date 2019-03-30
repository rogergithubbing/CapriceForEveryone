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

#ifndef __WIN32__
#define __WIN32__
#endif

//__WIN32__
#ifndef NATIVE_CPC_H
#define NATIVE_CPC_H

#ifdef __PALMOS__
#include <PceNativeCall.h>
#endif /* __PALMOS__ */

#include "types.h"

#ifdef __PALMOS__
//
// !! CPC native engine should be rebuilt following to any modification of thie file !!
//
#endif /* __PALMOS__ */


/***********************************************************************
 *
 *  Application Defines
 *
 ***********************************************************************/

#undef ENABLE_TAPE
#define ENABLE_TAPE

#undef TAPE_CYCLE_COUNT_FLOAT
//#define TAPE_CYCLE_COUNT_FLOAT
#undef ENABLE_TAPE_FORCE_PAUSES
//#define ENABLE_TAPE_FORCE_PAUSES
#undef ENABLE_TAPE_INSERT_PAUSE
#define ENABLE_TAPE_INSERT_PAUSE
#undef USE_TAPE_SPEED_ADJUST
//#define USE_TAPE_SPEED_ADJUST

#undef ENABLE_PRINTER
#define ENABLE_PRINTER

#undef ENABLE_TEXT_CAPTURE
#define ENABLE_TEXT_CAPTURE

#undef ENABLE_PLAYCITY
#define ENABLE_PLAYCITY

#undef ENABLE_AMX_MOUSE
#define ENABLE_AMX_MOUSE

#undef CRTC_ENGINE_V2
#define CRTC_ENGINE_V2

#undef USE_AUDIO_FILLED_BUFFER
#define USE_AUDIO_FILLED_BUFFER

#undef CAPRICE_4_2_ENGINE
//#define CAPRICE_4_2_ENGINE

#ifdef CAPRICE_4_2_ENGINE
  #ifndef CRTC_ENGINE_V2
  #define CRTC_ENGINE_V2
  #endif /* CRTC_ENGINE_V2 */
  #define PSG_AY_3_8910
#endif /* CAPRICE_4_2_ENGINE */

#undef ENABLE_TMPI_SPEECH
#define ENABLE_TMPI_SPEECH
#undef TMPI_SPEECH_FLOAT_MODE
//#define TMPI_SPEECH_FLOAT_MODE

#ifdef __PALMOS__
  // CAUTION: Palm TE doesn't accept native soundstream
  #undef SNDSTREAM_NATIVE
  //#define SNDSTREAM_NATIVE
  // Simulator doesn't accept native soundstream
  #ifdef __SIMU__
    #undef SNDSTREAM_NATIVE
  #endif /* __SIMU__ */
#endif /* __PALMOS__ */

#ifdef __BORLANDC__
#define ENABLE_METROLOGY
#endif /* __BORLANDC__ */


/***********************************************************************
 *
 *  Common macros
 *
 ***********************************************************************/

#define NUMBER_OF_ITEMS(array)             (sizeof(array) / sizeof(array[0]))
#define IS_POWER_OF_2(x)                   ((x) == ((x) & (~(x) + 1)))
#define ROUND_UPPER_4(x)                   ((x+3) & ~3)
#define ROUND_UPPER_8(x)                   ((x+7) & ~7)
#define ROUND_UPPER_16(x)                  ((x+15) & ~15)
#define ROUND_UPPER_256(x)                 ((x+255) & ~255)
#define OFFSET_OF(STRUCTURE,FIELD)         ((long)((char*)&((STRUCTURE*)0)->FIELD))

#define DWORD_UPPER_ALIGN(x)               ROUND_UPPER_4(x)

#define ROUND_UPPER_POWER_OF_2(val,trig)   { val=1; while (val<trig) val<<=1; }



/***********************************************************************
 *
 *  Internal Constants
 *
 ***********************************************************************/

//
// Execute return code
//
#define EC_BREAKPOINT               0x00000001
#define EC_FRAME_COMPLETE           0x00000002
#define EC_CYCLE_COUNT              0x00000004
#define EC_SOUND_BUFFER             0x00000008
#define EC_END_INSTRUCTION          0x00000010
#define EC_RESET                    0x00000020
#define EC_TEXT_CAPTURE             0x00000040
#define EC_FDC_EVENT                0x00000080
#define EC_ROM_INOUT_EVENT          0x00000100
#define EC_ERROR_MASK               0xFF000000
#ifdef _DEBUG
  #define EC_DEBUG_VDU_START        0x00010000
  #define EC_DEBUG_VDU_END          0x00020000
  #define EC_DEBUG_IDLE_ELAPSED     0x00040000
#endif /* _DEBUG */


#ifdef __PALMOS__
  #define MAX_RAM_SIZE              128   // 64 + 64K Expansion
  // Do not use silicon disc
  #undef USE_SILICON_DISC
#else /* __PALMOS__ */
  #define MAX_RAM_SIZE              576   // 64 + 256K Expansion + 256K Silicon disc
  #define USE_SILICON_DISC
#endif /* __PALMOS__ */


#define CLOCK_Z80                 4000000U // 4MHz
#define CYCLES_PER_SECOND         50U
#define CYCLES_MS                 (1000 / CYCLES_PER_SECOND)
#define CYCLE_COUNT_INIT          (CLOCK_Z80 / CYCLES_PER_SECOND) // 4MHz divided by 50Hz = number of CPU cycles per frame
//#define CLOCK_PER_MS(MS)          (CLOCK_Z80 * MS / 1000)
#define CLOCK_PER_MS(MS)          ((CLOCK_Z80 / 1000U) * MS)
#define CLOCK_PER_US(US)          ((CLOCK_Z80 / 1000000U) * US)

#define CLOCK_AY_INPUT            1000000U  // 1MHz
#define CLOCK_AY                  (CLOCK_AY_INPUT / 8) // 125000 Hz

#define SND_FREQ                  44100U
#define SND_SAMPLES               (SND_FREQ / CYCLES_PER_SECOND) // Nb Samples per cycles
#define SND_16BITS                1 /* 0=8bits, 1=16bits */
#define SND_STEREO                1 /* 0=Mono, 1=Stereo */

// Number of CPU cycles per sample
#define SND_CYCLE_SOUND_SHIFT          10U
#define SND_CYCLE_SOUND_INIT           ((CLOCK_Z80 << SND_CYCLE_SOUND_SHIFT) / SND_FREQ)
// Number of AY cycles per sample (Most significant short)
#define SND_LOOP_COUNT_SHIFT           15U
#define SND_LOOP_COUNT_INCREMENT       (1 << SND_LOOP_COUNT_SHIFT)
#define SND_LOOP_COUNT_INIT            ((CLOCK_AY << SND_LOOP_COUNT_SHIFT) / SND_FREQ)
// Number of YMZ cycles per sample (Most significant short)
#define PLAYCITY_LOOP_COUNT_SHIFT      14U
#define PLAYCITY_LOOP_COUNT_INCREMENT  (1 << PLAYCITY_LOOP_COUNT_SHIFT)

#if defined(__PALMOS__)
  // Samples requested par SndStreamCreate callback
  // 8bits=2048, 16bits=1024, Mono=4096
  #if SND_SAMPLES < 256
    #define SND_BUFFER_SAMPLES      512
  #elif SND_SAMPLES < 512
    #define SND_BUFFER_SAMPLES      1024
  #else
    #define SND_BUFFER_SAMPLES      2048
  #endif
#else /* __PALMOS__ */
  #define SND_BUFFER_SAMPLES        SND_SAMPLES
#endif /* __PALMOS__ */


// Sample size (in bytes)
#define SND_SAMPLE_SIZE           ( (SND_16BITS + 1) * (SND_STEREO + 1) )

// Size of sound buffer (in bytes)
// Used to generate EC_SOUND_BUFFER exit condition
#define SND_BUFFER_SIZE           (SND_BUFFER_SAMPLES * SND_SAMPLE_SIZE)

// BufferSize (in frames) parameter for SndStreamCreate and SndStreamCreateExtended
#define SND_STREAM_SAMPLES        SND_SAMPLES


#define SND_SAMPLE_OFFSET_16BIT   0
#define SND_SAMPLE_OFFSET_8BIT    128

#if SND_STEREO == 1
  #if SND_16BITS == 0
    #define SND_SAMPLE_ORIGIN     SND_SAMPLE_OFFSET_8BIT
  #else /* SND_16BITS == 0 */
    #define SND_SAMPLE_ORIGIN     SND_SAMPLE_OFFSET_16BIT
  #endif /* SND_16BITS == 0 */
#else /* SND_STEREO == 1 */
  #define SND_SAMPLE_ORIGIN       SND_SAMPLE_OFFSET_8BIT
#endif /* SND_STEREO == 1 */


//
// TMPI Speech synthesis
//
#define TMPI_F0                   8000 /*8096*/ // Digital filters work at 8KHz
#define TMPI_SUPERSAMPLING        8
#define TMPI_QUANT                512     // Table amplitude [-QUANT,QUANT]
#define TMPI_TABLE_LEN            3600    // Filter coefficients from frequencies
#define TMPI_NOISE_LEN            8192    // Noise generator table
// Number of CPU cycles per TMPI DAC sample
#define TMPI_CYCLE_INIT_SHIFT     10
#define TMPI_CYCLE_INIT           ((CLOCK_Z80 << TMPI_CYCLE_INIT_SHIFT) / TMPI_F0)
// Stream buffer
#define TMPI_SAMPLE_SIZE          (sizeof(tUShort)) // 16 bits mono
#define TMPI_BUFFER_SAMPLES       (SND_FREQ / CYCLES_PER_SECOND)


//
// PlayCity
//
#define PLAYCITY_SAMPLE_SIZE          ( (SND_16BITS + 1) * (SND_STEREO + 1) )
#define PLAYCITY_BUFFER_SAMPLES       SND_SAMPLES
#define PLAYCITY_BUFFER_SIZE          (PLAYCITY_BUFFER_SAMPLES * PLAYCITY_SAMPLE_SIZE)

#define CTC_CONTROL_INT               0x80
#define CTC_CONTROL_MODE              0x40
#define CTC_CONTROL_PRESCALER         0x20
#define CTC_CONTROL_EDGE              0x10
#define CTC_CONTROL_TRIGGER           0x08
#define CTC_CONTROL_TIMECONST         0x04
#define CTC_CONTROL_RESET             0x02
#define CTC_CONTROL_WORD              0x01


#define CPC_VISIBLE_SCR_WIDTH     384   // Visible width = 4+40+4 * 8
#define CPC_VISIBLE_SCR_HEIGHT    272   // Visible height = 5+25+4 * 8 (Initial = 270)
#ifndef CRTC_ENGINE_V2
  #define CPC_SCR_WIDTH             384   // Visible width = 4+40+4 * 8
  #define CPC_SCR_HEIGHT            312
  #define OFFSCREEN_VISIBLE_Y       1
#else /* !CRTC_ENGINE_V2 */
  #define CPC_SCR_WIDTH             768   // max width (Initial = 1024)
  #define CPC_SCR_HEIGHT            312   // max height
  #define OFFSCREEN_VISIBLE_Y       0
#ifdef CAPRICE_4_2_ENGINE
  #define MAX_DRAWN                 270   // Max displayed scan line (+1) (Initial = 270)
#else /* CAPRICE_4_2_ENGINE */
  #define MAX_DRAWN                 272   // Max displayed scan line (+1) (Initial = 270)
#endif /* CAPRICE_4_2_ENGINE */
  #define MAX_SYNC_DEC              80   // (Initial = 80) (Best = 80)
  #define MAX_SYNC_INC              80   // (Initial = 80) (Best = 80)
  #define MIN_VHOLD                 250   // (Initial = 250) (Best = 250) // 254 for iMPDraw full display
  #define MAX_VHOLD                 380   // (Initial = 380) (Best = 380)
  #define MID_VHOLD                 280   // (Initial = 295) (Best = 280)
  #define MIN_VHOLD_RANGE           55    // (Initial = 46) (Best = 55)
  #define MAX_VHOLD_RANGE           65    // (Initial = 74) (Best = 65)
#endif /* !CRTC_ENGINE_V2 */

#define CPC_MODEL_464_UK          0
#define CPC_MODEL_664_UK          1
#define CPC_MODEL_6128_UK         2
#define CPC_MODEL_464_FR          3
#define CPC_MODEL_664_FR          4
#define CPC_MODEL_6128_FR         5
#define CPC_MODEL_464_SP          6
#define CPC_MODEL_6128_SP         7

#if defined(__PALMOS__)
#define ROM_ID_TYPE               'tbin'
#define ROM_ID_464_UK             1001
#define ROM_ID_664_UK             1002
#define ROM_ID_6128_UK            1003
#define ROM_ID_6128_FR            1004
#define ROM_ID_464_FR             1005
#define ROM_ID_AMSDOS             2000
#define ROM_ID_PARADOS            2001
#define ROM_ID_SIZE_CPC           32768
#define ROM_ID_SIZE_AMSDOS        16384
#define ROM_ID_SIZE_PARADOS       16384
#endif /* __PALMOS__ */

#define PATHNAME_MAXSIZE          80

#define FONT_CPC_WIDTH            8
#define FONT_CPC_HEIGHT           8

#define MEMBANK_NUMBER            4


// CPC Displayed Manufacturer Name
#define CPC_BRAND_MASK            0x0E
#define CPC_JUMPER_AMSTRAD        0x0E
#define CPC_JUMPER_ORION          0x0C
#define CPC_JUMPER_SCHNEIDER      0x0A
#define CPC_JUMPER_AWA            0x08
#define CPC_JUMPER_SOLAVOX        0x06
#define CPC_JUMPER_SAISHO         0x04
#define CPC_JUMPER_TRIUMPH        0x02
#define CPC_JUMPER_ISP            0x00
// Settings
#define CPC_JUMPER_50HZ           0x10
//
#define CPC_DEFAULT_JUMPERS       (CPC_JUMPER_AMSTRAD | CPC_JUMPER_50HZ)


#define CRTC_TYPE_0               0
#define CRTC_TYPE_1               1


#define Sflag                     0x80 // sign flag
#define Zflag                     0x40 // zero flag
#define Yflag                     0x20
#define Hflag                     0x10 // halfcarry flag
#define Xflag                     0x08
#define Pflag                     0x04 // parity flag
#define Vflag                     0x04 // overflow flag
#define Nflag                     0x02 // negative flag
#define Cflag                     0x01 // carry flag
#define Xflags                    (Xflag | Yflag)


// CRTC flags
#define VS_flag                   (1 << 0)  // 0x00000001=1
#define HS_flag                   (1 << 1)  // 0x00000002=2
#define HDT_flag                  (1 << 2)  // 0x00000004=4
#define VDT_flag                  (1 << 3)  // 0x00000008=8
#define HT_flag                   (1 << 4)  // 0x00000010=16
#define VT_flag                   (1 << 5)  // 0x00000020=32
#define MR_flag                   (1 << 6)  // 0x00000040=64
#define VSf_flag                  (1 << 7)  // 0x00000080=128
#define VSreached_flag            (1 << 8)  // 0x00000100=256

//
// FDC
//
#define EXEC_PHASE                0
#define CMD_PHASE                 1
#define RESULT_PHASE              2

#define FDC_TO_CPU                0
#define CPU_TO_FDC                1

// Recalib
#define FDC_RECALIB_MAX_STEPS     77

// Motor state
#define FDC_MOTOR_STOPPED         0
#define FDC_MOTOR_STARTING        1
#define FDC_MOTOR_READY           2
#define FDC_MOTOR_STOPPING        3

// Main Status Register
#define MSR_RQM                   0x80
#define MSR_DIO                   0x40
#define MSR_EXM                   0x20
#define MSR_CB                    0x10
#define MSR_D3B                   0x08
#define MSR_D2B                   0x04
#define MSR_D1B                   0x02
#define MSR_D0B                   0x01

// ST0 Register
#define ST0_NT_MASK               0xC0 // Termination mask
#define ST0_NT                    0x00 // Normal Termination
#define ST0_AT                    0x40 // Abnormal Termination of command
#define ST0_IC                    0x80 // Invalid Command issue
#define ST0_ATR                   0xC0 // Abnormal termination due to ready change
#define ST0_SE                    0x20 // Seek End
#define ST0_EC                    0x10 // Equipement check
#define ST0_NR                    0x08 // Not ready
#define ST0_HD                    0x04 // Head address
#define ST0_US1                   0x02 // Unit Select 1
#define ST0_US0                   0x01 // Unit select 0

// ST1 Register
#define ST1_EN                    0x80 // End of Cylinder
#define ST1_DE                    0x20 // Data Error
#define ST1_OR                    0x10 // Overrrun
#define ST1_ND                    0x04 // No Data
#define ST1_NW                    0x02 // Not Writable
#define ST1_MA                    0x01 // Missing Address Mark

// ST2 Register
#define ST2_CM                    0x40 // Control Mark
#define ST2_DD                    0x20 // Data Error in Data field
#define ST2_WC                    0x10 // Wrong Cylinder
#define ST2_SH                    0x08 // Scan Equal Hit
#define ST2_SN                    0x04 // Scan Not Satisfied
#define ST2_BC                    0x02 // Bad Cylinder
#define ST2_MD                    0x01 // Missing Address Mark in Data Field

// ST3 Register
#define ST3_FT                    0x80 // Fault
#define ST3_WP                    0x40 // Write Protected
#define ST3_RY                    0x20 // Ready
#define ST3_T0                    0x10 // Track 0
#define ST3_TS                    0x08 // Two sided
#define ST3_HD                    0x04 // Head Address
#define ST3_US1                   0x02 // Unit Select 1
#define ST3_US0                   0x01 // Unit Select 0

// FDC flags
#define SKIP_flag                 0x0001 // skip sectors with DDAM/DAM
#define SEEKDRVA_flag             0x0002 // seek operation has finished for drive A
#define SEEKDRVB_flag             0x0004 // seek operation has finished for drive B
#define RNDDE_flag                0x0008 // simulate random DE sectors
#define OVERRUN_flag              0x0010 // data transfer timed out
#define SCAN_flag                 0x0020 // one of the three scan commands is active
#define SCANFAILED_flag           0x0040 // memory and sector data does not match
#define STATUSDRVA_flag           0x0080 // status change of drive A
#define STATUSDRVB_flag           0x0100 // status change of drive B
#define SEEKMAX_flag              0x0200 // Seek max steps
#define DELETED_flag              0x0400 // Read/Write deleted data
#define MULTITRACK_flag           0x0800 // Multitrack operation

// COMMAND codes
#define CMD_MT                    0x80 // Multitrack
#define CMD_MF                    0x40 // FM or MFM Mode (MFM for Amstrad, should be always 1)
#define CMD_SK                    0x20 // Skip deleted data address mark
// Operations
#define CMD_OP_MASK               0x1F
#define CMD_OP_READ               0x06
#define CMD_OP_READ_DELETED       0x0C
#define CMD_OP_WRITE              0x05
#define CMD_OP_WRITE_DELETED      0x09
#define CMD_OP_READ_TRACK         0x02
#define CMD_OP_READ_ID            0x0A
#define CMD_OP_FORMAT             0x0D
#define CMD_OP_SCAN_EQ            0x11
#define CMD_OP_SCAN_LOW           0x19
#define CMD_OP_SCAN_HIGH          0x1D
#define CMD_OP_RECALIB            0x07
#define CMD_OP_INTSTAT            0x08
#define CMD_OP_SPECIFY            0x03
#define CMD_OP_DRIVESTAT          0x04
#define CMD_OP_SEEK               0x0F

// COMMAND bytes indexes
#define CMD_CODE                  0
#define CMD_UNIT                  1
#define CMD_C                     2
#define CMD_H                     3
#define CMD_R                     4
#define CMD_N                     5
#define CMD_EOT                   6
#define CMD_GPL                   7
#define CMD_DTL                   8
#define CMD_STP                   8

// RESULT bytes indexes
#define RES_ST0                   0
#define RES_ST1                   1
#define RES_ST2                   2
#define RES_C                     3
#define RES_H                     4
#define RES_R                     5
#define RES_N                     6

// FDC_ENGINE_V1
#define OVERRUN_TIMEOUT                       CLOCK_PER_US(128)
// Fix for Orion Prime Disk accesses
#define INITIAL_TIMEOUT                       (OVERRUN_TIMEOUT * 16 /* Original=4 */) // ??
#define READ_STATUS_DELAY                     1

// DSK format
#define DSK_HEADER_SIZE               256
#define DSK_TRACKHEADER_SIZE          256
#define DSK_SECTORID_SIZE             8
#define DSK_NBSECTORPERHEADER         (DSK_TRACKHEADER_SIZE / DSK_SECTORID_SIZE)
//
#define DSK_TRACKMAX                  102   // max amount that fits in a DSK header
#define DSK_SIDEMAX                   2
#define DSK_SECTORMAX                 29    // max amount that fits in a DSK Track header
#define DSK_MIN_GAP3_LENGTH           10
#define EDSK_SECTORMAX                64    // Unlimited number of sectors (2112 AD got 64 sectors).
#define EDSK_OFFSETINFOHEADERSIZE     15
#define EDSK_GAP3ADJUSTTHRESHOLD      10

#define MAX_CMD_COUNT                 15

// Low level data format (MFM Mode)
#define FDC_BYTES_GAP4A               80
#define FDC_BYTES_SYNC                12
#define FDC_BYTES_IAM                 4
#define FDC_BYTES_GAP1                50
#define FDC_BYTES_IDAM                4
#define FDC_BYTES_CHRN                4
#define FDC_BYTES_CRC                 2
#define FDC_BYTES_GAP2                22
#define FDC_BYTES_DATAAM              4

#define FDC_BYTE_GAP4A                0x4E
#define FDC_BYTE_GAP1                 0x4E
#define FDC_BYTE_SYNC                 0x00
#define FDC_BYTE_IAM_1_3              0xC2
#define FDC_BYTE_IAM_4                0xFC
#define FDC_BYTE_IDAM_1_3             0xA1
#define FDC_BYTE_IDAM_4               0xFE
#define FDC_BYTE_GAP2                 0x4E
#define FDC_BYTE_DATAAM_1_3           0xA1
#define FDC_BYTE_DATAAM_4             0xFB
#define FDC_BYTE_DATAAM_4_DELETED     0xF8
#define FDC_BYTE_GAP3                 0x4E
#define FDC_BYTE_GAP4B                0x4E
#define FDC_BYTE_FORMAT               0xE5

// Disk Timings
#define DISC_REVOLUTION                       (CLOCK_PER_MS(200))   // 200ms = 300rpm
#define FDC_MOTOR_START_DELAY                 (CLOCK_PER_MS(200))   // 200ms delay between motor start command and motor ready
//#define FDC_MOTOR_STOP_DELAY                  (CLOCK_PER_MS(200))   // 200ms delay between motor stop command and motor not ready
#define FDC_MOTOR_STOP_DELAY                  (CLOCK_PER_MS(100))   // 100ms delay between motor stop command and motor not ready
#define FDC_DRQ_CYCLE_TIME                    (CLOCK_PER_US(32))    // 500kHz => 2us per pulse, MFM => 2 pulses per bit, 16 pulses per byte = 32us
#define FDC_BYTES_FROM_INDEX_TO_GAP1          (   FDC_BYTES_GAP4A \
                                                + FDC_BYTES_SYNC  \
                                                + FDC_BYTES_IAM   \
                                                + FDC_BYTES_GAP1 )
#define FDC_BYTES_FROM_GAP1_TO_CHRN           (   FDC_BYTES_SYNC  \
                                                + FDC_BYTES_IDAM )
#define FDC_BYTES_FROM_INDEX_TO_CHRN          (   FDC_BYTES_FROM_INDEX_TO_GAP1 \
                                                + FDC_BYTES_FROM_GAP1_TO_CHRN )
#define FDC_BYTES_FROM_CHRN_TO_DATA           (   FDC_BYTES_CHRN \
                                                + FDC_BYTES_CRC \
                                                + FDC_BYTES_GAP2  \
                                                + FDC_BYTES_SYNC  \
                                                + FDC_BYTES_DATAAM )
#define FDC_DELAY_FROM_INDEX_TO_CHRN          (FDC_BYTES_FROM_INDEX_TO_CHRN * FDC_DRQ_CYCLE_TIME)
#define FDC_DELAY_FROM_CHRN_TO_DATA           (FDC_BYTES_FROM_CHRN_TO_DATA * FDC_DRQ_CYCLE_TIME)
#define FDC_DELAY_CHRN                        ((FDC_BYTES_CHRN + FDC_BYTES_CRC) * FDC_DRQ_CYCLE_TIME)
#define FDC_MAX_BYTES_PER_TRACK               6250                  // 200ms / 32us
#define FDC_MAX_BYTES_PER_SECTOR              6144


//
// DISK
//
#define MAX_DISK_FORMAT                       2
#define FIRST_CUSTOM_DISK_FORMAT              2

#define DISK_FORMAT_DATA                      0
#define DISK_FORMAT_VENDOR                    1

#define DEFAULT_DISK_FORMAT                   DISK_FORMAT_DATA

//
// TAPE
//
#define TAPE_CDT_HEADER_SIZE                10
#define TAPE_CSW_V1_HEADER_SIZE             0x20

#define TAPE_PILOT_STAGE                    1
#define TAPE_SYNC_STAGE                     2
#define TAPE_DATA_STAGE                     3
#define TAPE_SAMPLE_DATA_STAGE              4
#define TAPE_PAUSE_STAGE                    5
#define TAPE_END                            6

#define TAPE_VOLUME                         32

#ifndef TAPE_CYCLE_COUNT_FLOAT
// ZX Ticks from 700 to 2500Hz => Maximum: 3500000/700=5000 (13 bits data)
// Maximum range shift is 32bits full range - 13bits max data = 19 bits fixed-point arithmetic.
#define CYCLE_SCALE_SHIFT                   19U
#define CYCLE_SCALE                         ((((tULong)40) << CYCLE_SCALE_SHIFT) / 35)
#define CYCLE_ADJUST(p)                     ((((tULong)p) * CYCLE_SCALE) >> CYCLE_SCALE_SHIFT)
#else /* !TAPE_CYCLE_COUNT_FLOAT */
#define CYCLE_ADJUST(p)                     ((tULong)((((double)p) * 40.f) / 35.f))
#endif /* !TAPE_CYCLE_COUNT_FLOAT */
#define MS_TO_CYCLES(p)                     ((tULong)(p) * 4000)

#define TAPE_LEVEL_LOW                      0
#define TAPE_LEVEL_HIGH                     0x80

#define TAPE_PAUSE_DELAY                    MS_TO_CYCLES(1) // 1ms

// OK "The Last Mission"
#define TAPE_DELAY_ACTIVATION               (CLOCK_PER_MS(50)) // min activation duration
#define TAPE_DELAY_DEACTIVATION             (CLOCK_PER_MS(50))

#if defined(ENABLE_TAPE_FORCE_PAUSES) || defined(ENABLE_TAPE_INSERT_PAUSE)
  #define TAPE_PLAY_START_PAUSE             0
#else
  #define TAPE_PLAY_START_PAUSE             MS_TO_CYCLES(2000) // 2 seconds pause
#endif

#define TAPE_MAX_DURATION                   ((99 * 60) + 99) // 99:99

// Number of CPU cycles per CSW sample
#define TAPE_CSW_CYCLE_SHIFT                10
#define TAPE_CSW_SAMPLE_CYCLES(SampleRate)  ((CLOCK_Z80 << TAPE_CSW_CYCLE_SHIFT) / SampleRate)


//
// Audio
//
#define AUDIO_INDEX_AL            255
#define AUDIO_INDEX_BL            170
#define AUDIO_INDEX_CL            13
#define AUDIO_INDEX_AR            13
#define AUDIO_INDEX_BR            170
#define AUDIO_INDEX_CR            255
//
#define AUDIO_INDEX_MONO          255
//
// 50 minimum should be used to get strong enough signal to be able to hear weak sounds (Sorcery Channel B)
#define AUDIO_SPEAKER_VOLUME          120
#define AUDIO_SPEAKER_PREAMP_MAX      100
#define AUDIO_PLAYCITY_VOLUME         120
#define AUDIO_PLAYCITY_PREAMP_MAX     100


//
// Keyboard matrix
//
#define IS_KEY_PRESSED(key)                   ((NativeCPC->keyboard_matrix[((tUChar)key) >> 4] & (tUChar)(1 << (((tUChar)key) & 7))) == 0)
#define PRESS_KEY(key)                        (NativeCPC->keyboard_matrix[((tUChar)key) >> 4] &= (tUChar)(~(1 << (((tUChar)key) & 7))))
#define RELEASE_KEY(key)                      (NativeCPC->keyboard_matrix[((tUChar)key) >> 4] |= (tUChar)(1 << (((tUChar)key) & 7)))

#define USERMANUAL_TO_CPC_KBD(keycode)        (((((tUChar)keycode) >> 3) << 4) | (((tUChar)keycode) & 7))
#define CPC_KBD_TO_USERMANUAL(keycode)        (((((tUChar)keycode) >> 4) << 3) | (((tUChar)keycode) & 7))

#define LINE_FROM_KEYCODE(Key)                ((tUChar)(Key >> 4))
#define ROW_FROM_KEYCODE(Key)                 ((tUChar)(Key & 7))
#define ROWMASK_FROM_KEYCODE(Key)             ((tUChar)(~(1 << ROW_FROM_KEYCODE(Key))))


// RAD Studio XE3 Update 2
#if (__BORLANDC__ == 0x651)
#undef _D
#endif /* __BORLANDC__ */
// RAD Studio XE6 Update 1
#if (__BORLANDC__ == 0x680)
#undef _D
#endif /* __BORLANDC__ */


//
// Z80 registers access macros
//
#define _A        Z80->Regs.AF.b.h
#define _F        Z80->Regs.AF.b.l
#define _AF       Z80->Regs.AF.w.l
#define _AFdword  Z80->Regs.AF.d
#define _B        Z80->Regs.BC.b.h
#define _C        Z80->Regs.BC.b.l
#define _BC       Z80->Regs.BC.w.l
#define _BCdword  Z80->Regs.BC.d
#define _D        Z80->Regs.DE.b.h
#define _E        Z80->Regs.DE.b.l
#define _DE       Z80->Regs.DE.w.l
#define _DEdword  Z80->Regs.DE.d
#define _H        Z80->Regs.HL.b.h
#define _L        Z80->Regs.HL.b.l
#define _HL       Z80->Regs.HL.w.l
#define _HLdword  Z80->Regs.HL.d
#define _PC       Z80->Regs.PC.w.l
#define _PCdword  Z80->Regs.PC.d
#define _SP       Z80->Regs.SP.w.l
#define _SPdword  Z80->Regs.SP.d
//
#define _IXh      Z80->Regs.IX.b.h
#define _IXl      Z80->Regs.IX.b.l
#define _IX       Z80->Regs.IX.w.l
#define _IXdword  Z80->Regs.IX.d
#define _IYh      Z80->Regs.IY.b.h
#define _IYl      Z80->Regs.IY.b.l
#define _IY       Z80->Regs.IY.w.l
#define _IYdword  Z80->Regs.IY.d
//
#define _I        Z80->Regs.I
#define _R        Z80->Regs.R
#define _Rb7      Z80->Regs.Rb7
#define _IFF1     Z80->Regs.IFF1
#define _IFF2     Z80->Regs.IFF2
#define _IM       Z80->Regs.IM
#define _HALT     Z80->Regs.HALT
//
#define _WZh      Z80->Regs.WZ.b.h
#define _WZl      Z80->Regs.WZ.b.l
#define _WZ       Z80->Regs.WZ.w.l
#define _WZdword  Z80->Regs.WZ.d


//
// Function pointers
//
struct structNativeCPC;
struct structPSG;
struct structVDU;
struct structCRTC;
struct structFDC;

typedef tVoid  (*tNativeCPCPtr)(struct structNativeCPC*);
typedef tVoid  (*tCaseEnvType)(struct structPSG*);
typedef tVoid  (*tDrawHandlerPtr)(struct structVDU*, tULong);
typedef tVoid  (*tSynthetizerPtr)(struct structNativeCPC*);
typedef tVoid  (*tSetAYRegisterPtr)(struct structPSG*, tULong, tULong);
typedef tVoid  (*tZ80OUTHandlerPtr)(struct structNativeCPC*, tRegister, tULong);
typedef tVoid  (*tSetCRTCRegisterPtr)(struct structNativeCPC*, tULong, tULong);
typedef tULong (*tGetCRTCRegisterPtr)(struct structNativeCPC*, tULong);
typedef tULong (*tGetCRTCStatusPtr)(struct structNativeCPC*);
typedef tVoid  (*tVideoEnginePtr)(struct structNativeCPC*, tULong);
typedef tVoid  (*tCRTCPtr)(struct structCRTC*);
typedef tVoid  (*tFDCPtr)(struct structFDC*);
typedef tVoid  (*tFDCCmdHandlerPtr)(struct structFDC*);
typedef tVoid  (*tFDCZ80WaitStatePtr)(struct structFDC*, tULong);
typedef tBool  (*tFDCCmdMotorPtr)(struct structFDC*, tULong);
typedef tULong (*tFDCReadStatusPtr)(struct structFDC*);
typedef tULong (*tFDCReadDataPtr)(struct structFDC*);
typedef tVoid  (*tFDCWriteDataPtr)(struct structFDC*, tULong);
#if defined(__PALMOS__)
typedef tVoid  (*tSoundCalculateLevelTablesPtr)(tVoid*);
#endif /* __PALMOS__ */
#if defined(__WIN32__)
typedef tVoid* (*tMemPtrNewFct)(tULong);
typedef tVoid  (*tMemPtrDeleteFct)(tVoid*);
#endif /* __WIN32__ */


// =============================================================================
// ATTENTION : 4-bytes data should be 4-bytes aligned
//
// 4-bytes data first
// then 2-bytes data
// and 1-byte data at the end
// =============================================================================


// ============================================================================
//
// colours_rgb_entry
//
typedef struct
{
  tUChar colour_r;
  tUChar colour_g;
  tUChar colour_b;
  tUChar intensity;
  tUChar shadow_index;
} colours_rgb_entry;
// ============================================================================


// ============================================================================
//
// tAutoStartReturnCode
//
typedef enum
{
  AutoStartOK = 0,
  DiskFormatUnknown,
  NoValidFileEntry,
  TooManyFileEntries
} tAutoStartReturnCode;
// ============================================================================


// ============================================================================
//
// tNoise
//
typedef union
{
  struct
  {
    tUShort Low;
    tUShort Val;
  } s;
  tULong Seed;
} tNoise;
// ============================================================================


// ============================================================================
//
// Regs
//
typedef struct
{
  tRegister AF, BC, DE, HL, PC, SP, AFx, BCx, DEx, HLx, IX, IY;
  tRegister WZ; // The internal "work" register used by the Z80 (from Caprice Reloaded r598)
  tULong I, R, Rb7, IFF1, IFF2, IM, HALT, EI_issued, int_pending, nmi_pending;
  tULong breakpoint;
  tULong flags2, flags1; // (for SCF,CCF)
} tZ80Regs;
// ============================================================================


// ============================================================================
//
// tZ80
//
#define Z80_MAGIC                   0xb45e87cc // !! DO NOT CHANGE !!

typedef struct
{
  //
  // Static pointers first (not affected by restoration)
  //
  tUChar* SZ;
  tUChar* SZ_BIT;
  tUChar* SZP;
  tUChar* SZHV_inc;
  tUChar* SZHV_dec;
  tUChar* SZHVC_add;
  tUChar* SZHVC_sub;

  //
  // 32bits, 16 bits then 8 bits variables
  //
  tULong iCycleCount;
  tULong iWSAdjust;

  tULong magic; // Identification tag

  tZ80Regs Regs;
} tZ80;

#ifdef __PALMOS__
#define STATIC_SIZE_Z80       ( sizeof(tUChar*) + /* SZ */ \
                                sizeof(tUChar*) + /* SZ_BIT */ \
                                sizeof(tUChar*) + /* SZP */ \
                                sizeof(tUChar*) + /* SZHV_inc */ \
                                sizeof(tUChar*)   /* SZHV_dec */ )
#define DYNAMIC_SIZE_Z80      (0)
#endif /* __PALMOS__ */
// ============================================================================


// ============================================================================
//
// fdc_cmd_table_def
//
typedef struct
{
  //
  // 32bits, 16 bits then 8 bits variables
  //
  tFDCCmdHandlerPtr cmd_handler;
  tULong cmd;
  tULong cmd_length;
  tULong res_length;
  tULong cmd_direction;
#ifdef ENABLE_METROLOGY
  tUChar label[20];
#endif /* ENABLE_METROLOGY */
} fdc_cmd_table_def;
// ============================================================================


// ============================================================================
//
// tSector
//
#define SECTOR_FLAGS_NBELEMENT          4

#define SECTOR_CRC_NBELEMENT            2
#define SECTOR_CRC_MSB                  0
#define SECTOR_CRC_LSB                  1

#define SECTOR_CHRN_NBELEMENT           6
#define SECTOR_CHRN_C                   0
#define SECTOR_CHRN_H                   1
#define SECTOR_CHRN_R                   2
#define SECTOR_CHRN_N                   3
#define SECTOR_CHRN_CRC_MSB             4
#define SECTOR_CHRN_CRC_LSB             5

typedef struct
{
  //
  // 32bits, 16 bits then 8 bits variables
  //
  tUChar* data; // pointer to sector data
  tUChar* declared_data; // pointer to declared sized sector data
  tUChar* gap2; // pointer to sector GAP#2
  tUChar* gap3; // pointer to sector GAP#3
  tULong  size; // sector size in bytes
  tULong  declared_size; // most of the time equals size, except for "weak" sectors
  tULong  data_size; // effective size of data
  tULong  weak_count; // WEAK SECTORS
  tULong  data_gap3_length; // GAP#3 length included into data
  tULong  info_gap3_length; // GAP#3 length given by track info
  tULong  gap2_length; // GAP#2 length
  tULong  gap3_length; // GAP#3 length
  tULong  track_offset; // Offset from beginning of track
  tULong  offset_info; // Offset given by "Offset-info" section
  tUChar  CHRN[SECTOR_CHRN_NBELEMENT]; // the CHRN for this sector
  tUChar  data_crc[SECTOR_CRC_NBELEMENT]; // CRC CCITT of DATA
  tUChar  flags[SECTOR_FLAGS_NBELEMENT]; // ST1 and ST2 - reflects any possible error conditions
  tBool   empty;
} tSector;
// ============================================================================


// ============================================================================
//
// tTrack
//
typedef struct
{
  //
  // 32bits, 16 bits then 8 bits variables
  //
  tUChar* data; // pointer to track data
  tUChar* original_data; // pointer to original track data
  tUChar* new_data; // pointer to new track data
  tULong  sectors; // sector count for this track
  tULong  formatted_sectors; // number of formatted sectors
  tULong  first_sector_overwritten; // do not take first sector into account 
  tULong  size; // track size in bytes
  tULong  original_size; // original track size in bytes
  tULong  new_size; // track new size in bytes
  tULong  gap3_length; // GAP#3 size
  tULong  real_size; // Track size including ID, GAPS, SYNC...
  tULong  offset_info_size; // Size given by offset info
  tULong  filler; // format filler
  tSector sector[EDSK_SECTORMAX]; // array of sector information structures
} tTrack;
// ============================================================================


// ============================================================================
//
// tDrive
//
#define DRIVE_MAGIC                     0xea56cc32 // !! DO NOT CHANGE !!

#define MAX_SIZETAB_FILENAME            100
#define SIZETAB_FILENAME                (DWORD_UPPER_ALIGN((tULong)sizeof(tChar) * MAX_SIZETAB_FILENAME))

typedef struct
{
  //
  // 32bits, 16 bits then 8 bits variables
  //
  tUChar* dataP;
  tUChar* new_dataP;    // Memory allocation including formatted tracks
  tULong data_size;
  tULong new_data_size; // New data size after tracks format

  tULong tracks; // total number of tracks
  tULong current_track; // PCN (Present Cylinder Number)
  tULong dest_track; // NCN (New Cylinder Number)
  tULong sides; // total number of sides
  tULong current_side; // side being accessed
  tULong current_sector; // sector being accessed
  tULong random_DEs; // sectors with Data Errors return random data?
  tULong flipped; // reverse the side to access?
  tULong write_protected; // is the image write protected?
  tULong read_only; // is the image file read_only ?
  tULong altered; // has the image been modified?
  tULong offset_info_available; // Offset-Info section available
  
  tULong magic; // Identification tag

#ifdef ENABLE_METROLOGY
  tULong signal_updated;
  tULong signal_content_change;
  tULong signal_new_position;
  tULong signal_track_format;
  tULong signal_sector_write;
#endif /* ENABLE_METROLOGY */

  tTrack track[DSK_TRACKMAX][DSK_SIDEMAX]; // array of track information structures

  tChar filename[SIZETAB_FILENAME];
} tDrive;
// ============================================================================


// ============================================================================
//
// tDiskFormat
//
typedef struct 
{
   tUChar label[40]; // label to display in options dialog
   tULong tracks; // number of tracks
   tULong sides; // number of sides
   tULong sectors; // sectors per track
   tULong sector_size; // sector size as N value
   tULong gap3_length; // GAP#3 size
   tUChar filler_byte; // default byte to use
   tUChar sector_ids[2][16]; // sector IDs
} tDiskFormat;
// ============================================================================


// ============================================================================
//
// tDSKHeader (256 bytes size)
//
typedef struct
{
   tChar id[34];
   tChar creator[14];
   tUChar tracks;
   tUChar sides;
   tUChar track_size_LSB; // Standard disk image
   tUChar track_size_MSB; // Standard disk image
   tUChar track_size[DSK_TRACKMAX * DSK_SIDEMAX];
} tDSKHeader;
// ============================================================================


// ============================================================================
//
// tTrackHeader (first 256 bytes)
//
typedef struct {
   tChar id[12];
   tChar unused1[4];
   tUChar track;
   tUChar side;
   tUChar unused2[2];
   tUChar bps;
   tUChar sectors;
   tUChar gap3_length;
   tUChar filler;
   tUChar sector[DSK_SECTORMAX][8];
} tTrackHeader;
// ============================================================================


// ============================================================================
//
// tTrackSector
//
typedef struct {
   tUChar track_C;
   tUChar side_H;
   tUChar sectorID_R;
   tUChar size_N;
   tUChar flags_ST1;
   tUChar flags_ST2;
   tUChar size_LSB;
   tUChar size_MSB;
} tSectorInfo;
// ============================================================================


// ============================================================================
//
// tCRTC
//
#define CRTC_MAGIC                          0xfd58aa43 // !! DO NOT CHANGE !!

#define CRTC_DRAWHANDLER_NBELEMENT          4
#define CRTC_MODEMAPS_NBELEMENT             4
#define CRTC_REGISTERS_NBELEMENT            18

// The next 4 bytes must remain together
typedef union
{
  tULong combined;
  struct
  {
    tUChar monVSYNC;
    tUChar inHSYNC;
    union
    {
      tUShort combined;
      struct
      {
        tUChar DISPTIMG;
        tUChar HDSPTIMG;
      };
    } dt;
  };
} tCRTCflags1;

// The next two bytes must remain together
typedef union
{
  tUShort combined;
  struct
  {
    tUChar NewDISPTIMG;
    tUChar NewHDSPTIMG;
  };
} tCRTCnew_dt;

typedef struct structCRTC
{
  //
  // Static pointers first (not affected by restoration)
  //
  // CRTC_ENGINE_V1
  tDrawHandlerPtr DrawHandler[CRTC_DRAWHANDLER_NBELEMENT];
  tDrawHandlerPtr CurrentHandler;
  tDrawHandlerPtr RequestedHandler;

  // CRTC_ENGINE_V2
  tCRTCPtr CharInstMR;
  tCRTCPtr CharInstSL;
  tNativeCPCPtr PreRender;
  tNativeCPCPtr Render;
  const tULong* ModeMaps[CRTC_MODEMAPS_NBELEMENT];
  const tULong* ModeMap;
  // Start values
  tCRTCPtr StartCharInstMR;
  tCRTCPtr StartCharInstSL;
  const tULong* StartModeMap;

  //
  // 32bits, 16 bits then 8 bits variables
  //
  // CRTC_ENGINE_V1
  tULong flags;

  tULong requested_addr;
  tULong addr;

  tULong char_count;
  tULong line_count;
  tULong raster_count;
  tULong vt_adjust_count;

  // CRTC_ENGINE_V1
  tULong skew;
  tULong max_raster;
  tULong last_hdisp;

  tULong cursor_start_ras;
  tULong cursor_end_ras;
  tULong cursor_addr;
  tULong cursor_state;
  tULong cursor_blink_count;

  tULong reg_select;
  tULong registers[CRTC_REGISTERS_NBELEMENT];
  tULong start_addr;
  tULong lpen_offset;
  tULong lpen_strobe;

  // CRTC_ENGINE_V1
  tULong requested_mode;
  tULong current_mode;
  tULong stop_rendering;

  tULong magic; // Identification tag

  // CRTC_ENGINE_V2
  tCRTCflags1 flags1;
  tULong next_addr;
  tULong next_address;
  tULong scr_base;
  tULong hstart;
  tULong hend;
  tULong last_hend;
  tULong reg5;
  tULong r7match;
  tULong r9match;
  tULong flag_startvta;
  tULong flag_resnext;
  tULong flag_invta;
  tULong flag_resvsync;
  tULong flag_invsync;
  tULong flag_resscan;
  tULong flag_reschar;
  tULong flag_newscan;
  tULong flag_resframe;
  tULong flag_hadhsync;
  tULong flag_inmonhsync;
  tULong LastPreRend;
  tULong* RendStart;
  tULong* RendPos;
  tUChar* RendWid;
  tUChar* RendOut;

  tULong sync_dec_max;
  tULong sync_inc_max;

  tLong hsw;
  tLong hsw_active;
  tLong hsw_count;
  tLong vsw;
  tLong vsw_count;

  // CRTC_ENGINE_V2
  tLong iMonHSPeakPos;
  tLong iMonHSStartPos;
  tLong iMonHSEndPos;
  tLong iMonHSPeakToStart;
  tLong iMonHSStartToPeak;
  tLong iMonHSEndToPeak;
  tLong iMonHSPeakToEnd;
  tLong HorzPos;
  tLong MonHSYNC;
  tLong MonFreeSync;
  tLong HSyncDuration;
  tLong MinHSync;
  tLong MaxHSync;
  tLong HadP;

#ifdef ENABLE_METROLOGY  
  tULong signal_register_update;
#endif /* ENABLE_METROLOGY */

  // CRTC_ENGINE_V2
  tCRTCnew_dt new_dt;
  tUShort MinVSync;
  tUShort MaxVSync;

  // CRTC_ENGINE_V2
  tUChar HorzChar;
  tUChar HorzMax;
  tUChar HorzPix[49];
  tUChar RendBuff[800];
} tCRTC;

#ifdef __PALMOS__
#define STATIC_SIZE_CRTC      ( (sizeof(tDrawHandlerPtr) * CRTC_DRAWHANDLER_NBELEMENT) + /* DrawHandler */ \
                                sizeof(tDrawHandlerPtr) + /* CurrentHandler */ \
                                sizeof(tDrawHandlerPtr) + /* CurrentHandler */ ) \
                                sizeof(tCRTCPtr)        + /* CharInstMR */ \
                                sizeof(tCRTCPtr)        + /* CharInstSL */ \
                                sizeof(tCRTCPtr)        + /* PreRender */ \
                                sizeof(tCRTCPtr)        + /* Render */ \
                                (sizeof(tULong*) * CRTC_MODEMAPS_NBELEMENT) + /* ModeMaps */ \
                                sizeof(tULong*)         + /* ModeMap */ \
                                sizeof(tCRTCPtr)        + /* StartCharInstMR */ \
                                sizeof(tCRTCPtr)        + /* StartCharInstSL */ \
                                sizeof(tCRTCPtr)        + /* StartRender */ \
                                sizeof(tULong*)           /* StartModeMap */ )
#define DYNAMIC_SIZE_CRTC     (0)
#endif /* __PALMOS__ */
// ============================================================================


// ============================================================================
//
// tVDU
//
#define VDU_MAGIC                 0xa312ccab // !! DO NOT CHANGE !!

#ifndef CRTC_ENGINE_V2
#define VDU_SCANLINE_MIN          CPC_VISIBLE_SCR_HEIGHT
#define VDU_HSTART                7
#define VDU_VSTART                27
#define VDU_HWIDTH                (CPC_SCR_WIDTH / 8)
#define VDU_VHEIGHT               CPC_VISIBLE_SCR_HEIGHT
#endif /* CRTC_ENGINE_V2 */

typedef struct structVDU
{
  //
  // Static pointers first (not affected by restoration)
  //
  tULong* scr_base;
  // CRTC_ENGINE_V2
  tULong* scr_pos;
  tUShort* MAXlate;
  // CRTC_ENGINE_V1
  tULong* scr_current;
  tUChar* mode0_table;
  tUChar* mode1_table;
  tUChar* pbRAM;
  tULong* palette;

  //
  // 32bits, 16 bits then 8 bits variables replaced by Restoration
  //
  tULong vcount;
  tULong scr_line;
  tULong scr_line_offs; // Number of ULong for a horizontal line
  tULong hdelay;
  tULong vdelay;
  tULong frame_completed;
  tULong char_count;
  tULong hcount;
  tULong ram_addr;
  tULong border;
  tULong raster_length;
  tULong flag_drawing;
  tULong antialiased_color;

  tULong vhold_min;
  tULong vhold_max;
  tULong vhold_mid;
  tULong vhold_range_min;
  tULong vhold_range_max;

  tULong magic; // Identification tag

  tLong scrln;
  tLong scanline;
  tLong hsw;
  tLong hsw_active;
  tLong hsw_count;
  tLong vsw_count;
} tVDU;

#ifdef __PALMOS__
#define STATIC_SIZE_VDU       ( sizeof(tULong*) + /* scr_base */ \
                                sizeof(tUChar*) + /* scr_pos */ \
                                sizeof(tUShort*) + /* MAXlate */ \
                                sizeof(tUChar*) + /* scr_current */ \
                                sizeof(tUChar*) + /* mode0_table */ \
                                sizeof(tUChar*) + /* mode1_table */ \
                                sizeof(tUChar*) + /* pbRAM */ \
                                sizeof(tULong*)   /* palette */ )
#define DYNAMIC_SIZE_VDU      (0)
#endif /* __PALMOS__ */
// ============================================================================


// ============================================================================
//
// tGateArray
//
#define GA_MAGIC                          0xe03ab1dd // !! DO NOT CHANGE !!

#define GA_INK_VALUES_NBELEMENT           17
#define GA_PALETTE_NBELEMENT              19

typedef struct
{
  //
  // 32bits, 16 bits then 8 bits variables
  //
  tULong sl_count;

  tULong ROM_config;
  tULong RAM_bank;
  tULong RAM_config;
  tULong upper_ROM;
  tULong pen;

  tULong int_delay;

  tULong ink_values[GA_INK_VALUES_NBELEMENT];
  tULong palette[GA_PALETTE_NBELEMENT];

  const tUChar* BasicColorsP;
  const tUChar* GateArrayColorsP;

  tULong magic; // Identification tag

  // CRTC_ENGINE_V2
  tULong hs_count;
  tULong requested_scr_mode;
  tULong scr_mode;

#ifdef ENABLE_METROLOGY  
  tULong signal_palette_update;
#endif /* ENABLE_METROLOGY */
} tGateArray;
// ============================================================================


// ============================================================================
//
// tFDC
//
#define FDC_MAGIC                               0x55fd12ab // !! DO NOT CHANGE !!

#define FDC_MAX_TRACK_SIZE                      (6144 - 154)

#define FDC_COMMAND_NBELEMENT                   12
#define FDC_RESULT_NBELEMENT                    8

typedef struct structFDC
{
  //
  // Static pointers first
  //
  fdc_cmd_table_def* FDCCommandTable;

  //
  // Dynamic pointers
  //
  tDrive* active_drive; // reference to the currently selected drive
  tTrack* active_track; // reference to the currently selected track, of the active_drive
  tSector* active_sector; // reference to the currently selected sector, of the active_track

  // Parent interface
  tDrive* DriveA;
  tDrive* DriveB;
  tUChar* pbGPBuffer;  // Fast FDC ONLY
  tMemPtrNewFct NewMemFct;
  tMemPtrDeleteFct DeleteMemFct;

  tUChar* buffer_ptr;
  tUChar* buffer_endptr;

  // Real FDC
  tFDCPtr on_index_ptr;
  tFDCPtr on_index_end_operation_ptr;
  tFDCPtr on_sector_ptr;
  tFDCPtr on_sector_chrn_ptr;
  tFDCPtr on_sector_data_ptr;
  tFDCPtr on_end_of_transfer_ptr;
  tFDCPtr on_data_register_ptr;
  tFDCPtr on_abort_ptr;
  tFDCPtr head_on_track_ptr;
  tFDCPtr head_step_ptr;
  
  //
  // 32bits, 16 bits then 8 bits variables
  //
  tFDCCmdHandlerPtr cmd_handler;
  tFDCZ80WaitStatePtr z80_wait_state_fct;

  tFDCCmdMotorPtr cmd_motor_fct;
  tFDCReadStatusPtr read_status_fct;
  tFDCReadDataPtr read_data_fct;
  tFDCWriteDataPtr write_data_fct;

  tULong headsteps;
  tULong recalibsteps;

  tULong led;
  tULong motor;
  tULong phase;
  tULong cmd_index;
  tULong cmd_length;
  tULong res_length;
  tULong cmd_direction;
  tULong byte_count;
  tULong buffer_count;

  tULong flags;
  tULong read_status_delay;

  tULong command[FDC_COMMAND_NBELEMENT];
  tULong result[FDC_RESULT_NBELEMENT];

  tULong weak_counter;

  tULong magic; // Identification tag

  // Real FDC
  tULong main_status_register;
  tULong ST0_register;
  tULong ST1_register;
  tULong ST2_register;
  tULong ST3_register;
  tULong data_register;
  tULong step_rate_time;
  tULong motor_state;
  tULong data_transfer_ack; // Data transfer acknowledge
  tULong index_count;
  tULong requested_sector_size;
  tULong requested_read_count;
  tULong sector_read_count;
  tULong current_track_count;
  tULong last_track_data_count;
  tULong next_track_count;
  tULong last_read_CHRN[SECTOR_CHRN_NBELEMENT];
  tULong end_of_track_detected;
  tULong delayed_intstat;

  tULong temp_crc;
  tULong idam_crc;
  tULong dataam_crc;

  tLong timeout;  // Fast FDC

  // Real FDC
  tLong motor_start_stop_timer;
  tLong index_timer;
  tLong sector_timer;
  tLong head_on_track_timer;
  tLong head_step_timer;

#ifdef ENABLE_METROLOGY
  tULong signal_updated;
  tULong signal_new_position;
  tULong signal_read_status;
  tULong signal_new_command;
  tULong signal_command_result;
  tULong signal_read_data;
  tULong signal_write_data;
  tULong signal_motor_change;
  tULong signal_data;
  tULong signal_phase;
  tULong signal_index;
  tULong signal_track;
  tULong signal_sector;
  tULong read_status_value;
  tULong motor_on_time;
#endif /* ENABLE_METROLOGY */
} tFDC;
// ============================================================================


// ============================================================================
//
// tPrinter
//
// DMP2000
// Print speed: 105 CPS (Characters per second) / 52 CPS min
// Standard: 10 CPI / 80 CPL
// Condensed: 17 CPI / 137 CPL
// Line feed speed: 200ms (1/6 inch)

// DMP 2160
// Print speed: 160 CPS

// EPSON FX-80
// Print speed: 160 CPS

#define PRINTER_MAGIC                           0xdfc5291a // !! DO NOT CHANGE !!

#define PRINTER_DATA_BUFFER_SIZE                2048

#define PRINTER_BUSY_PRINT_CHAR_TIME            (CLOCK_PER_MS(10))

typedef struct
{
  tLong  busy_counter;
  tULong data_counter;

  tULong online;
  tULong busy;

  tULong magic;

  tUChar data_buffer[PRINTER_DATA_BUFFER_SIZE];
} tPrinter;
// ============================================================================


// ============================================================================
//
// tTapeBlock
//
typedef struct strTapeBlock
{
  tUChar* pbImageData;
  tULong length;
  tULong duration_ms; // in milliseconds
  tULong time_offset_ms; // time offset from start  
  tUChar block_id;
} tTapeBlock;
// ============================================================================


// ============================================================================
//
// tTape
//
#define TAPE_WCYCLETABLE_NBELEMENT              2

#define TAPE_MAGIC                              0xfe45aa3c // !! DO NOT CHANGE !!

typedef struct
{
  //
  // Static pointers first (not affected by restoration)
  //
  tTapeBlock* blocks;
  tUChar* onebit_table;

  //
  // Dynamic pointers (Reindexed by Restoration)
  //
  tUShort* pwTapePulseTable;
  tUShort* pwTapePulseTableEnd;
  tUShort* pwTapePulseTablePtr;

  tUChar* pbTapeBlock;
  tUChar* pbTapeBlockData;

  tUChar* pbTapeImage;
  tUChar* pbTapeImageEnd;

  tUChar* pbMessagesStart;
  tUChar* pbMessagesEnd;

  tMemPtrNewFct MemPtrNewPtr;
  tMemPtrDeleteFct MemPtrDeletePtr;

  tUChar* pbCSWBuffer;
  tUChar* pbCSWData;
  tUChar* pbCSWEnd;

  //
  // 32bits, 16 bits then 8 bits variables
  //
  tULong dwTapePulseCycles;
  tULong dwTapeZeroPulseCycles;
  tULong dwTapeOnePulseCycles;
  tULong dwTapeStage;
  tULong dwTapePulseCount;
  tULong dwTapeDataCount;
  tULong dwTapeBitsToShift;

  tULong dwTapeSpeedAdjust;

  tULong tape_motor;
  tULong tape_play_button;
  tULong relay_activations;
  tULong timer_motor_activation;
  tULong timer_motor_deactivation;

  tULong csw_cycles_per_sample;
  tULong csw_nbsamples;
  tULong csw_sample_rate;

  tULong nb_blocks;
  tULong current_block;
  tULong counter000_block;

  tULong timer_from_play;
  tULong timer_play_second;
  tULong total_duration_ms;
  tULong total_duration_s;

  tULong magic; // Identification tag

  tLong iTapeCycleCount;

  tUShort wCycleTable[TAPE_WCYCLETABLE_NBELEMENT];

  tUChar bTapeData;
  tUChar bTapeLevel;
} tTape;
// ============================================================================


// ============================================================================
//
// tPPI
//
#define PPI_MAGIC              0x65bbc94a // !! DO NOT CHANGE !!

typedef struct
{
  //
  // 32bits, 16 bits then 8 bits variables
  //
  tULong control;
  tULong portA;
  tULong portB;
  tULong portC;

  tULong magic; // Identification tag
} tPPI;
// ============================================================================


// ============================================================================
//
// tPSG
//
#define PSG_MAGIC                       0xf3e259a2 // !! DO NOT CHANGE !!

#define PSG_REGISTERS_NBELEMENT         16

typedef struct structPSG
{
  //
  // Static pointers first (not affected by restoration)
  //
  tLong* Level_PP;
  tLong* Level_AR;
  tLong* Level_AL;
  tLong* Level_BR;
  tLong* Level_BL;
  tLong* Level_CR;
  tLong* Level_CL;
  tLong* Level_PLAYCITY;

  tUChar* pbSndBuffer;
  tUChar* pbSndBufferEnd;

  //
  // Dynamic pointers (Reindexed by Restoration)
  //
  tUChar* snd_bufferptr;

  //
  // 32bits, 16 bits then 8 bits variables
  //
  tCaseEnvType Case_Env;

  tULong cycle_count;
  tULong LoopCount;

  tNoise Noise;

  tULong FilledBufferSize;
  tULong SndBufferSize;

  tULong Ton_Counter_A;
  tULong Ton_Counter_B;
  tULong Ton_Counter_C;

  tULong Envelope_Counter;
  tULong Noise_Counter;

  tULong Channel_EnA;
  tULong Channel_EnB;
  tULong Channel_EnC;
  tULong Ton_EnA;
  tULong Ton_EnB;
  tULong Ton_EnC;
  tULong Noise_EnA;
  tULong Noise_EnB;
  tULong Noise_EnC;
  tULong Envelope_EnA;
  tULong Envelope_EnB;
  tULong Envelope_EnC;

  tULong Ton_A;
  tULong Ton_B;
  tULong Ton_C;

  tULong buffer_full;

  tULong snd_volume;
  tULong snd_pp_device;

  tULong FirstPeriod;
  tULong control;
  tULong reg_select;

  tULong snd_enabled;

  tULong magic; // Identification tag

#ifdef ENABLE_METROLOGY
  tULong signal_Left_Chan_Ready;
  tULong signal_Right_Chan_Ready;
  tLong Left_Chan_Sample;
  tLong Right_Chan_Sample;
#endif /* ENABLE_METROLOGY */

  tLong AmplitudeEnv;

  tLong Left_Chan;
  tLong Right_Chan;

  tLong LevelTape;

  union
  {
    tUChar Index[PSG_REGISTERS_NBELEMENT];
    struct
    {
      tUChar TonALo;        // R0  = Channel A Fine Tone Period Register
      tUChar TonAHi;        // R1  = Channel A Coarse Tone Period Register
      tUChar TonBLo;        // R2  = Channel B Fine Tone Period Register
      tUChar TonBHi;        // R3  = Channel B Coarse Tone Period Register
      tUChar TonCLo;        // R4  = Channel C Fine Tone Period Register
      tUChar TonCHi;        // R5  = Channel C Coarse Tone Period Register
      tUChar Noise;         // R6  = Noise Period Control
      tUChar Mixer;         // R7  = Mixer Control-I/O Enable
      tUChar AmplitudeA;    // R10 = Channel A Amplitude Control
      tUChar AmplitudeB;    // R11 = Channel B Amplitude Control
      tUChar AmplitudeC;    // R12 = Channel C Amplitude Control
      tUChar EnvelopeLo;    // R13 = Envelope Fine Period Register
      tUChar EnvelopeHi;    // R14 = Envelope Coarse Period Register
      tUChar EnvType;       // R15 = Envelope Shape/Cycle Control
      tUChar PortA;         // R16 = I/O Port Data
      tUChar PortB;         // R17 = I/O Port Data
    } UChar;
  } RegisterAY;
} tPSG;
// ============================================================================


// ============================================================================
//
// tTMPISpeech
//
#define TMPI_SPEECH_MAGIC               0x7d65e30c // !! DO NOT CHANGE !!

/* finite machine state controling frames */
typedef enum
{
	MEA8000_STOPPED = 0,    /* nothing to do, timer disabled */
	MEA8000_WAIT_FIRST, /* received pitch, wait for first full trame, timer disabled */
	MEA8000_STARTED,    /* playing a frame, timer on */
	MEA8000_SLOWING,    /* repating last frame with decreasing amplitude, timer on */
} tTMPISpeechState;


typedef struct 
{
#ifdef TMPI_SPEECH_FLOAT_MODE
	tDouble fm, last_fm;         /* frequency, in Hz */
	tDouble bw, last_bw;         /* band-width, in Hz */
	tDouble output, last_output; /* filter state */
#else /* TMPI_SPEECH_FLOAT_MODE */
	tUShort fm, last_fm;
	tUShort bw, last_bw;
	tLong   output, last_output;
#endif /* TMPI_SPEECH_FLOAT_MODE */
} tTMPISpeechFilter;


typedef struct
{
  //
  // Static pointers first (not affected by restoration)
  //
  tUChar* pbBufferStart;
  tUChar* pbBufferEnd;

  tLong* cos_table; /* fm => cos coefficient */
  tLong* exp_table; /* bw => exp coefficient */
  tLong* exp2_table; /* bw => 2*exp coefficient */
  tLong* noise_table;

  //
  // Dynamic pointers (Reindexed by Restoration)
  //
  tShort* bufferptr;

  //
  // 32bits, 16 bits then 8 bits variables
  //
	tTMPISpeechFilter f[4]; /* filters */

	tULong phi; /* absolute phase for frequency / noise generator */

  tULong magic; // Identification tag

  tULong FilledBufferSize;
  tULong BufferSize;

#ifdef ENABLE_METROLOGY
  tULong signal_Sample_Ready;
  tULong signal_Write_Command;
  tULong signal_Write_Command_Value;
  tULong signal_Write_Pitch;
  tULong signal_Write_Pitch_Value;  
  tULong signal_Write_Frame;
  tULong signal_Write_Overflow;
  tULong signal_Read_Status;
  tULong signal_Read_Status_Value;
#endif /* ENABLE_METROLOGY */

  tLong cycle_count; /* timer */

  tTMPISpeechState state;

	tUShort framelength;  /* in samples */
	tUShort framepos;     /* in samples */
	tUShort framelog;     /* log2 of framelength */

	tUShort last_ampl, ampl;    /* amplitude * 1000 */
	tUShort last_pitch, pitch;  /* pitch of sawtooth signal, in Hz */

	tShort lastsample, sample; /* output samples are interpolated */

	tUChar buf[4]; /* store 4 consecutive data to form a frame info */
	tUChar bufpos; /* new byte to write in frame info buffer */

	tUChar cont; /* if no data 0=stop 1=repeat last frame */
	tUChar roe;  /* enable req output, now unimplemented */
	tUChar noise;
  
  tUChar enabled;
} tTMPISpeech;

// ============================================================================


// ============================================================================
//
// tPlayCity
//
#define PLAYCITY_MAGIC              0xf269b8dd // !! DO NOT CHANGE !!

#define CTC_NB_CHANNELS             4
#define CTC_CHANNEL0                0
#define CTC_CHANNEL1                1
#define CTC_CHANNEL2                2
#define CTC_CHANNEL3                3

typedef struct
{
  //
  // Static pointers first (not affected by restoration)
  //
  tPSG* PSG_Left;
  tPSG* PSG_Right;
  tUChar* pbSndBuffer;
  tUChar* pbSndBufferEnd;

  //
  // Dynamic pointers (Reindexed by Restoration)
  //
  tUChar* snd_bufferptr;

  //
  // 32bits, 16 bits then 8 bits variables
  //
  tULong Freq;

  tULong FilledBufferSize;
  tULong SndBufferSize;

  tULong LoopCount;
  tULong LoopCountInit;

  tULong buffer_full;

  tULong snd_enabled;
  tULong snd_volume;

  tULong enabled;

  tULong magic; // Identification tag

  tLong Left_Chan;
  tLong Right_Chan;

  tLong CTC_timer[CTC_NB_CHANNELS];
  tLong CTC_timer_init[CTC_NB_CHANNELS];
  
  tULong CTC_counter[CTC_NB_CHANNELS];
  
  tULong CTC_control_word[CTC_NB_CHANNELS];
  tULong CTC_time_constant[CTC_NB_CHANNELS];

} tPlayCity;


// ============================================================================
//
// tPreferences
//
//
// !!! CAUTION !!!
// Published version dependant structure size.
//
// DO NOT DELETE ITEMS. DO NOT RESIZE ITEMS. 8 Bits variables ONLY
//
#if defined(__PALMOS__)

typedef struct
{
  // Version 1
  tUChar CPCModel;                // CPC_MODEL_464_UK, CPC_MODEL_664_UK ou CPC_MODEL_6128_UK
  tUChar CPCJumpers;              // Settings jumpers
  tUChar ScreenType;              // 0 = Color, 1 = Monochrome
  tUChar CPCTrueSpeed;            // 0 = Full Speed, 1 = CPC Speed
  tChar  FilesPathname[80];
  // Version 2
  tUChar ScreenIntensity;
  tUChar SoundEnabled;            // 0 = No sound, 1 = Sound played
  tUChar SoundVolume;             // Sound volume when played
  tUChar SoundSystemVolume;       // 0 = Own volume, 1 = System Games volume
  // Version 3
  tUChar Display320x480;          // Last display mode
  tUChar AutomaticContextSave;    // 0 = Manual, 1 = Automatic
  tUChar CustomColorUse;          // Not used : 0 = Do not use custom colors, 1 = Use custom colors
  tUChar CustomColorIndexes[32];  // Not used
  tUChar Mode2AntiAliasing;       // 0 = Mode 2 Aliased, 1 = Mode 2 Anti-aliased
  // Version 4
  tUChar OnScreenDisplayActive;   // 0 = Inactive, 1 = Active
  tUChar OnScreenRockerActive;    // 0 = Inactive, 1 = Active
  tUChar RockerCenterKeyIndex;
  tUChar JoystickFireHardKey;     // No longer used
  tUChar PreferencesChanged;
  // Version 5
  tUChar AutoOffDisable;
  tUChar ScreenDisplayOffsetX;
  tUChar ScreenDisplayOffsetY;
  tUChar HardKey1Index;
  tUChar HardKey2Index;
  tUChar HardKey3Index;
  tUChar HardKey4Index;
  // Version 6
  tUChar UseParados;              // 0 = AMSDOS, 1 = PARADOS
  tUChar Use64kMemoryExtension;   // 0 = Not used, 1 = Used
  tUChar NightModeActive;
  // Version 7
  tUChar CPCKeycodeA;
  tUChar Rendering;
  // Version 8
  tUChar CPCKeycodeB;
  tUChar CPCKeycodeC;
  tUChar CPCKeycodeD;
  tUChar AutoStartEnable;
  // Version 9
  tUChar OnScreenLightPenActive;   // 0 = Inactive, 1 = Active
  tUChar OnScreenMagnumGunActive;  // 0 = Inactive, 1 = Active
  tUChar OnScreenGunstickActive;   // 0 = Inactive, 1 = Active
  tUChar OnScreenWestPhaserActive; // 0 = Inactive, 1 = Active
  // Version 10
  tUChar OnScreenAMXMouseActive;   // 0 = Inactive, 1 = Active
  //
} tPreferences;

#elif defined(__WIN32__)

typedef struct
{
  tUChar  CPCModel;                // CPC_MODEL_464_UK, CPC_MODEL_664_UK ou CPC_MODEL_6128_UK
  tUChar  CPCJumpers;              // Settings jumpers
  tUChar  Use64kMemoryExtension;   // 0 = Not used, 1 = Used
  tUChar  Use256kMemoryExtension;  // 0 = Not used, 1 = Used
  tUChar  Use256kSiliconDisc;      // 0 = Not used, 1 = Used
  tUChar  ScreenType;              // 0 = Color, 1 = Monochrome
  tUChar  ScreenIntensity;
  tUChar  NightModeActive;
  tUChar  SoundVolume;             // Sound volume when played
  tUChar  Mode2AntiAliasing;       // 0 = Mode 2 Aliased, 1 = Mode 2 Anti-aliased
  tUChar  SpeakerStereo;           // 0 = Mono, 1 = Stereo
  tUChar  Speaker16Bits;           // 0 = 8 bits, 1 = 16 bits
  tUShort SpeakerFreq;
  tUChar  PlayCityStereo;          // 0 = Mono, 1 = Stereo
  tUChar  PlayCity16Bits;          // 0 = 8 bits, 1 = 16 bits
  tUShort PlayCityFreq;
  tUShort PlayCityInvert;          // 0 = Not inverted, 1 = Inverted
} tPreferences;

#endif
// ============================================================================


// ============================================================================
//
// tNativeCPC
//
#define NATIVECPC_MAGIC                               0x18da76bb // !! DO NOT CHANGE !!

#define NATIVECPC_MEMBANK_CONFIG_NBELEMENT            8
#define NATIVECPC_WCYCLETABLE_NBELEMENT               2
#define NATIVECPC_KEYBOARD_MATRIX_NBELEMENT           16
#define NATIVECPC_COLOURS_NBELEMENT                   32
#define NATIVECPC_PALM_PALETTE_NBELEMENT              256

typedef struct structNativeCPC
{
  //
  // Static pointers first (not affected by restoration)
  //
  tZ80* Z80;

  tCRTC* CRTC;
  tUChar* mode0_table;
  tUChar* mode1_table;
  tUShort* MAXlate;

  tGateArray* GateArray;

  tFDC* FDC;
  fdc_cmd_table_def* FDCCommandTable; // FDC

  tTape* Tape;

  tPPI* PPI;

  tPSG* PSG;

  tVDU* VDU;

  tPrinter* Printer;

  tTMPISpeech* TMPISpeech;

  tPlayCity* PlayCity;

  tPreferences* prefP;

  tUChar* contextP;

  tUChar* pbGPBuffer;  // FDC V1 ONLY

  tUChar* pbRAM;
  tUChar** memmap_ROM;

#if defined(__PALMOS__)
  tUShort* DAATable; // Z80
#else /* __PALMOS__ */
  const tUShort* DAATable; // Z80
#endif /* __PALMOS__ */

  colours_rgb_entry* colours_rgb; // Video
  colours_rgb_entry* colours_green; // Video
  colours_rgb_entry* active_colours; // Video

  tVoid* BmpOffScreenBits; // Video

#if defined(__PALMOS__)
  const tVoid* emulStateP;
  Call68KFuncType* call68KFuncP;
#else /* __PALMOS__ */
  tVoid* emulStateP;
  tVoid* call68KFuncP;
#endif /* __PALMOS__ */

#if defined(__WIN32__)
  tZ80OUTHandlerPtr Z80OUTHandlerPtr;
  tSetAYRegisterPtr SetAYRegisterPtr;
  tSetCRTCRegisterPtr SetCRTCRegisterPtr;
  tGetCRTCRegisterPtr GetCRTCRegisterPtr;
  tGetCRTCStatusPtr GetCRTCStatusPtr;
  tVideoEnginePtr VideoEnginePtr;
  tSynthetizerPtr SpeakerSynthetizerPtr;
  tSynthetizerPtr PlayCitySynthetizerPtr;
#endif /* __WIN32__ */

#if defined(__WIN32__)
  tSynthetizerPtr SpeakerSynthetizerStereo16Ptr;
  tSynthetizerPtr SpeakerSynthetizerMono16Ptr;
  tSynthetizerPtr PlayCitySynthetizerStereo16Ptr;
  tSynthetizerPtr PlayCitySynthetizerStereo16InvertPtr;
  tSynthetizerPtr PlayCitySynthetizerMono16Ptr;
#endif /* __WIN32__ */

  tMemPtrNewFct MemPtrNewPtr;
  tMemPtrDeleteFct MemPtrDeletePtr;

#if defined(__WIN32__)
  colours_rgb_entry* WinPalettePtr;
#else /* __WIN32__ */
  tULong WinPalettePtr;
#endif /* __WIN32__ */

#if defined(__PALMOS__)
  tULong SoundCalculateLevelTablesPtr;
#endif /* __PALMOS__ */

  tVoid*  hMemROMlo;
  tVoid*  hMemAMSDOS;
  tUChar* pbROMlo;
  tUChar* pbROMhi;
  tUChar* pbExpansionROM;

  tULong FirstInitToPerform;
  tULong RestorationPerformed;

  // Screen copy in landscape mode
  tULong* OffscreenStartBits;
  tULong  OffscreenCopyHeight;
  tULong  OffscreenCopyWidth;
  tULong  OffscreenAlignGap;
  tULong* OnscreenStartBits;
  tULong  OnscreenPixelGap;
  tULong  OnscreenAlignGap;

  //
  // Dynamic pointers (Reindexed by Restoration)
  //
  tUChar* membank_config[NATIVECPC_MEMBANK_CONFIG_NBELEMENT][MEMBANK_NUMBER];
  tUChar* membank_read[MEMBANK_NUMBER];
  tUChar* membank_write[MEMBANK_NUMBER];

#ifdef ENABLE_METROLOGY
  tULong enable_ROM_operations;
  tULong ROM_operations_upper_ROM; 
  tULong ROM_operations_direction; // 0=IN, 1=OUT
  tULong ROM_operations_port;
  tULong ROM_operations_data;
  tULong signal_ROM_external_operation;
#endif /* ENABLE_METROLOGY */
  //
  // 32bits, 16 bits then 8 bits variables replaced by Restoration
  //
  tULong ram_size; // CPC
  tULong rambank_used; // CPC

  tULong magic; // Identification tag

  tULong colours[NATIVECPC_COLOURS_NBELEMENT]; // Video
  tULong BmpOffScreenSizeInBytes; // Video
  tULong BitsPerPixel; // Video

  tULong jumpers;
  tULong keyboard_line; // keyboard matrix index
  tULong printer_port;
  tULong scr_tube;
  tULong scr_intensity;
  tULong crtc_type;
  tULong floppy_drive;
  tULong fast_fdc;
  tULong vhold_adjust;

  tULong text_capture;
  tULong fdc_event_capture;
  tULong printer_event_capture;

  tULong timer_from_reset;
  
  // Lightgun emulation
  tULong lightgun_x_pos;
  tULong lightgun_y_pos;
  tULong lightgun_y_radius;
  tULong lightgun_beam_key_mask;
  tULong lightgun_sensitivity;
  tULong lightgun_beam_detect;
  tULong lightgun_beam_state;
  tULong lightgun_beam_duration;
  tULong lightgun_counter;
  tULong lightgun_trigger_delay;
  tULong lightgun_trigger_duration;
  tULong lightgun_random_crtc; // 0=inactive; 1=active
  tULong lightgun_trigger_time;

#ifdef ENABLE_METROLOGY
  tULong signal_lightgun_trigger;
  tULong signal_lightgun_beam_detected;
  tULong signal_lightgun_trigger_end;
  tULong signal_lightgun_read_joystick;
#endif /* ENABLE_METROLOGY */

  // AMX mouse emulation
  tULong amx_mouse_active;
  tULong amx_mouse_reset;
  tLong amx_mouse_last_pos_X;
  tLong amx_mouse_last_pos_Y;
  tLong amx_mouse_X_pulses;
  tLong amx_mouse_Y_pulses;

  tLong cycle_count;

#ifdef _PROFILE
  tULong profileCounter[PROFILE_NB_ITEMS];
#endif /* _PROFILE */

  tUShort BmpOffScreenBytesRow; // Video

  tUChar keyboard_matrix[NATIVECPC_KEYBOARD_MATRIX_NBELEMENT];
  tUChar keyboard_clash_type;

  tUChar paused;

#if defined(__PALMOS__)
  tUChar PalmToCPCPalette[NATIVECPC_PALM_PALETTE_NBELEMENT];
  tUChar PalmPaletteIntensity[NATIVECPC_PALM_PALETTE_NBELEMENT];
#endif /* __PALMOS__ */

  tUChar night_mode;

#if defined(__WIN32__)
  tUChar step_by_step;
#endif /* __WIN32__ */

#if defined(__PALMOS__)
  // CPC key code for hard keys. Session dependant.
  tUChar HardKeyCPCKeyCodeA;
  tUChar HardKeyCPCKeyCodeB;
  tUChar HardKeyCPCKeyCodeC;
  tUChar HardKeyCPCKeyCodeD;
#endif /* __PALMOS__ */

} tNativeCPC;
// ============================================================================


// ============================================================================
//
// tDiskOperation
//
#define DISKOPERATION_MAX_CATALOG_ENTRIES       64
#define DISKOPERATION_CATALOG_ENTRY_SIZE        12
typedef struct
{
  tNativeCPC* NativeCPC;
  tDrive* Drive;
  const tDiskFormat* DiskFormatTableP;
  tUChar* DiskContentP;
  tVoid* Param;
  tULong disk_size;
  tULong NbCatalogEntries;
  tUChar FormatType;
  tUChar CatalogEntries[DISKOPERATION_MAX_CATALOG_ENTRIES][DISKOPERATION_CATALOG_ENTRY_SIZE];
} tDiskOperation;
// ============================================================================


// ============================================================================
//
// tSoundCallbackParam
//
typedef struct
{
  //
  // Static pointers first (not affected by restoration)
  //
  tUChar* SoundBufferStartP;
  tUChar* SoundBufferEndP;
  tUChar** CurrentPosPP;
  tULong* CurrentSizeP;
  tUChar* LastPosP;
  tULong BufferRead;

  tULong StreamRef;

  //
  // 32bits, 16 bits then 8 bits variables
  //
} tSoundCallbackParam;
// ============================================================================



// ============================================================================
//
// Memory Allocation tables sizes
//
#define MEMMAP_ROM_COUNT                256

#define SIZETAB_RAM                     ((tULong)(sizeof(tUChar) * MAX_RAM_SIZE * 1024))
#define SIZETAB_GPBUFFER                ((tULong)(sizeof(tUChar) * 4 * 1024))    // General Purpose RAM
#define SIZETAB_ROMLO                   ((tULong)(sizeof(tUChar) * 32 * 1024))
#define SIZETAB_MEMMAP_ROM              ((tULong)(sizeof(tUChar*) * MEMMAP_ROM_COUNT))
#define SIZETAB_Z80TABLE                ((tULong)(sizeof(tUChar) * 256))
#define SIZETAB_Z80_SZHVC_TABLE         ((tULong)(sizeof(tUChar) * 2 * 256 * 256))
#define SIZETAB_BIT1                    ((tULong)(sizeof(tUChar) * 256))
#define SIZETAB_MODE0                   ((tULong)(sizeof(tUChar) * 512))
#define SIZETAB_MODE1                   ((tULong)(sizeof(tUChar) * 1024))
#define SIZETAB_MAXLATE                 ((tULong)(sizeof(tUShort) * 0x7400))
#define SIZETAB_SND_BUFFER              ((tULong)(sizeof(tUChar) * SND_BUFFER_SIZE))
#define SIZETAB_LEVEL_PP                ((tULong)(sizeof(tLong) * 256))
#define SIZETAB_LEVEL_AR                ((tULong)(sizeof(tLong) * 32))
#define SIZETAB_LEVEL_AL                ((tULong)(sizeof(tLong) * 32))
#define SIZETAB_LEVEL_BR                ((tULong)(sizeof(tLong) * 32))
#define SIZETAB_LEVEL_BL                ((tULong)(sizeof(tLong) * 32))
#define SIZETAB_LEVEL_CR                ((tULong)(sizeof(tLong) * 32))
#define SIZETAB_LEVEL_CL                ((tULong)(sizeof(tLong) * 32))
#define SIZETAB_LEVEL_PLAYCITY          ((tULong)(sizeof(tLong) * 32))
#define SIZETAB_FDCCOMMAND              ((tULong)(sizeof(fdc_cmd_table_def) * MAX_CMD_COUNT))
#define SIZETAB_VERSION                 ((tULong)(sizeof(tChar) * 20))
#define SIZETAB_TMPI_BUFFER             ((tULong)(TMPI_SAMPLE_SIZE * TMPI_BUFFER_SAMPLES))
#define SIZETAB_PLAYCITY_BUFFER         ((tULong)(sizeof(tUChar) * PLAYCITY_BUFFER_SIZE))
#define SIZETAB_TMPI_COS_TABLE          ((tULong)(sizeof(tLong) * TMPI_TABLE_LEN))
#define SIZETAB_TMPI_EXP_TABLE          ((tULong)(sizeof(tLong) * TMPI_TABLE_LEN))
#define SIZETAB_TMPI_EXP2_TABLE         ((tULong)(sizeof(tLong) * TMPI_TABLE_LEN))
#define SIZETAB_TMPI_NOISE_TABLE        ((tULong)(sizeof(tLong) * TMPI_NOISE_LEN))
// ============================================================================



// ============================================================================
//
// tContextHeader
//
#define CONTEXT_FLAG_PROFILE            0x00000001
#define CONTEXT_FLAG_DEBUG              0x00000002
#define CONTEXT_FLAG_TESTU              0x00000004
#define CONTEXT_FLAG_TRACE              0x00000008

typedef struct
{
  tLong Flags;
  tChar version[SIZETAB_VERSION];
  tUChar CPCModel;
  tUChar ScreenType;
  tUChar UseParados;
  tUChar Use64kMemoryExtension;
  tUChar EmuKeyFxState;
  tUChar EmuKeyJoystickState;
  tUChar EmuKeyAutoToggleState;
  tUChar Mode2AntiAliasing;
} tContextHeader;
// ============================================================================


// ============================================================================
//
// tContextResources
//
typedef struct
{
  tVoid* BmpOffScreenBits; // WinCreateOffscreenWindow
  colours_rgb_entry* colours_rgb;   // (colours_rgb_entry*)MemHandleLock(colours_rgbH)
  colours_rgb_entry* colours_green; // (colours_rgb_entry*)MemHandleLock(colours_greenH)
  const tUShort* DAATable; // (tUShort*)MemHandleLock(DAATableH)
  tVoid* PNOCPCExecuteP; // DmCreateLargeResource
  fdc_cmd_table_def* FDCCommandTableP;

  // Memory handles
  tVoid* colours_rgbH;
  tVoid* colours_greenH;
  tVoid* DAATableH;

  tULong BmpOffScreenSizeInBytes; // BmpGetSizes
  tULong BmpOffScreenBytesRow; // BmpGetDimensions
  tULong BmpBitsPerPixel;
} tContextResources;
// ============================================================================


// ============================================================================
//
// tEngineGraphicFormat
//
typedef struct
{
  tULong VisibleScreenWidth;
  tULong VisibleScreenHeight;
  tULong ScreenWidth;
  tULong ScreenHeight;
  tULong OffscreenVisibleY;
  tULong MaxBitPerPixel;
} tEngineGraphicFormat;
// ============================================================================


// ============================================================================
//
// Context sections definition
//
#define CONTEXT_OFFSET_HEADER             (0L)
#define CONTEXT_SIZE_HEADER               (DWORD_UPPER_ALIGN(sizeof(tContextHeader)))
//
#define CONTEXT_OFFSET_RESOURCES          (CONTEXT_OFFSET_HEADER + CONTEXT_SIZE_HEADER)
#define CONTEXT_SIZE_RESOURCES            (DWORD_UPPER_ALIGN(sizeof(tContextResources)))
//
#define CONTEXT_OFFSET_DISKOPERATION      (CONTEXT_OFFSET_RESOURCES + CONTEXT_SIZE_RESOURCES)
#define CONTEXT_SIZE_DISKOPERATION        (DWORD_UPPER_ALIGN(sizeof(tDiskOperation)))
//
#define CONTEXT_OFFSET_NATIVECPC          (CONTEXT_OFFSET_DISKOPERATION + CONTEXT_SIZE_DISKOPERATION)
#define CONTEXT_SIZE_NATIVECPC            (DWORD_UPPER_ALIGN(sizeof(tNativeCPC)))
//
#define CONTEXT_OFFSET_Z80                (CONTEXT_OFFSET_NATIVECPC + CONTEXT_SIZE_NATIVECPC)
#define CONTEXT_SIZE_Z80                  (DWORD_UPPER_ALIGN(sizeof(tZ80)))
//
#define CONTEXT_OFFSET_PSG                (CONTEXT_OFFSET_Z80 + CONTEXT_SIZE_Z80)
#define CONTEXT_SIZE_PSG                  (DWORD_UPPER_ALIGN(sizeof(tPSG)))
//
#define CONTEXT_OFFSET_FDC                (CONTEXT_OFFSET_PSG + CONTEXT_SIZE_PSG)
#define CONTEXT_SIZE_FDC                  (DWORD_UPPER_ALIGN(sizeof(tFDC)))
//
#define CONTEXT_OFFSET_TAPE               (CONTEXT_OFFSET_FDC + CONTEXT_SIZE_FDC)
#define CONTEXT_SIZE_TAPE                 (DWORD_UPPER_ALIGN(sizeof(tTape)))
//
#define CONTEXT_OFFSET_VDU                (CONTEXT_OFFSET_TAPE + CONTEXT_SIZE_TAPE)
#define CONTEXT_SIZE_VDU                  (DWORD_UPPER_ALIGN(sizeof(tVDU)))
//
#define CONTEXT_OFFSET_CRTC               (CONTEXT_OFFSET_VDU + CONTEXT_SIZE_VDU)
#define CONTEXT_SIZE_CRTC                 (DWORD_UPPER_ALIGN(sizeof(tCRTC)))
//
#define CONTEXT_OFFSET_TMPI_SPEECH        (CONTEXT_OFFSET_CRTC + CONTEXT_SIZE_CRTC)
#define CONTEXT_SIZE_TMPI_SPEECH          (DWORD_UPPER_ALIGN(sizeof(tTMPISpeech)))
//
#define CONTEXT_OFFSET_PLAYCITY           (CONTEXT_OFFSET_TMPI_SPEECH + CONTEXT_SIZE_TMPI_SPEECH)
#define CONTEXT_SIZE_PLAYCITY             (DWORD_UPPER_ALIGN(sizeof(tPlayCity)))
//
#define CONTEXT_OFFSET_PLAYCITY_LEFT      (CONTEXT_OFFSET_PLAYCITY + CONTEXT_SIZE_PLAYCITY)
#define CONTEXT_SIZE_PLAYCITY_LEFT        (DWORD_UPPER_ALIGN(sizeof(tPSG)))
//
#define CONTEXT_OFFSET_PLAYCITY_RIGHT     (CONTEXT_OFFSET_PLAYCITY_LEFT + CONTEXT_SIZE_PLAYCITY_LEFT)
#define CONTEXT_SIZE_PLAYCITY_RIGHT       (DWORD_UPPER_ALIGN(sizeof(tPSG)))
//
#define CONTEXT_OFFSET_Z80_SZ             (CONTEXT_OFFSET_PLAYCITY_RIGHT + CONTEXT_SIZE_PLAYCITY_RIGHT)
#define CONTEXT_SIZE_Z80_SZ               (DWORD_UPPER_ALIGN(SIZETAB_Z80TABLE))
//
#define CONTEXT_OFFSET_Z80_SZ_BIT         (CONTEXT_OFFSET_Z80_SZ + CONTEXT_SIZE_Z80_SZ)
#define CONTEXT_SIZE_Z80_SZ_BIT           (DWORD_UPPER_ALIGN(SIZETAB_Z80TABLE))
//
#define CONTEXT_OFFSET_Z80_SZP            (CONTEXT_OFFSET_Z80_SZ_BIT + CONTEXT_SIZE_Z80_SZ_BIT)
#define CONTEXT_SIZE_Z80_SZP              (DWORD_UPPER_ALIGN(SIZETAB_Z80TABLE))
//
#define CONTEXT_OFFSET_Z80_SZHV_inc       (CONTEXT_OFFSET_Z80_SZP + CONTEXT_SIZE_Z80_SZP)
#define CONTEXT_SIZE_Z80_SZHV_inc         (DWORD_UPPER_ALIGN(SIZETAB_Z80TABLE))
//
#define CONTEXT_OFFSET_Z80_SZHV_dec       (CONTEXT_OFFSET_Z80_SZHV_inc + CONTEXT_SIZE_Z80_SZHV_inc)
#define CONTEXT_SIZE_Z80_SZHV_dec         (DWORD_UPPER_ALIGN(SIZETAB_Z80TABLE))
//
#define CONTEXT_OFFSET_Z80_SZHVC_add      (CONTEXT_OFFSET_Z80_SZHV_dec + CONTEXT_SIZE_Z80_SZHV_dec)
#define CONTEXT_SIZE_Z80_SZHVC_add        (DWORD_UPPER_ALIGN(SIZETAB_Z80_SZHVC_TABLE))
//
#define CONTEXT_OFFSET_Z80_SZHVC_sub      (CONTEXT_OFFSET_Z80_SZHVC_add + CONTEXT_SIZE_Z80_SZHVC_add)
#define CONTEXT_SIZE_Z80_SZHVC_sub        (DWORD_UPPER_ALIGN(SIZETAB_Z80_SZHVC_TABLE))
//
#define CONTEXT_OFFSET_BIT1_TABLE         (CONTEXT_OFFSET_Z80_SZHVC_sub + CONTEXT_SIZE_Z80_SZHVC_sub)
#define CONTEXT_SIZE_BIT1_TABLE           (DWORD_UPPER_ALIGN(SIZETAB_BIT1))
//
#define CONTEXT_OFFSET_MODE0_TABLE        (CONTEXT_OFFSET_BIT1_TABLE + CONTEXT_SIZE_BIT1_TABLE)
#define CONTEXT_SIZE_MODE0_TABLE          (DWORD_UPPER_ALIGN(SIZETAB_MODE0))
//
#define CONTEXT_OFFSET_MODE1_TABLE        (CONTEXT_OFFSET_MODE0_TABLE + CONTEXT_SIZE_MODE0_TABLE)
#define CONTEXT_SIZE_MODE1_TABLE          (DWORD_UPPER_ALIGN(SIZETAB_MODE1))
//
#define CONTEXT_OFFSET_MAXLATE            (CONTEXT_OFFSET_MODE1_TABLE + CONTEXT_SIZE_MODE1_TABLE)
#define CONTEXT_SIZE_MAXLATE              (DWORD_UPPER_ALIGN(SIZETAB_MAXLATE))
//
#define CONTEXT_OFFSET_GATEARRAY          (CONTEXT_OFFSET_MAXLATE + CONTEXT_SIZE_MAXLATE)
#define CONTEXT_SIZE_GATEARRAY            (DWORD_UPPER_ALIGN(sizeof(tGateArray)))
//
#define CONTEXT_OFFSET_PPI                (CONTEXT_OFFSET_GATEARRAY + CONTEXT_SIZE_GATEARRAY)
#define CONTEXT_SIZE_PPI                  (DWORD_UPPER_ALIGN(sizeof(tPPI)))
//
#define CONTEXT_OFFSET_PRINTER            (CONTEXT_OFFSET_PPI + CONTEXT_SIZE_PPI)
#define CONTEXT_SIZE_PRINTER              (DWORD_UPPER_ALIGN(sizeof(tPrinter)))
//
#define CONTEXT_OFFSET_PSG_LEVEL_PP       (CONTEXT_OFFSET_PRINTER + CONTEXT_SIZE_PRINTER)
#define CONTEXT_SIZE_PSG_LEVEL_PP         (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_PP))
//
#define CONTEXT_OFFSET_PSG_LEVEL_AR       (CONTEXT_OFFSET_PSG_LEVEL_PP + CONTEXT_SIZE_PSG_LEVEL_PP)
#define CONTEXT_SIZE_PSG_LEVEL_AR         (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_AR))
//
#define CONTEXT_OFFSET_PSG_LEVEL_AL       (CONTEXT_OFFSET_PSG_LEVEL_AR + CONTEXT_SIZE_PSG_LEVEL_AR)
#define CONTEXT_SIZE_PSG_LEVEL_AL         (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_AL))
//
#define CONTEXT_OFFSET_PSG_LEVEL_BR       (CONTEXT_OFFSET_PSG_LEVEL_AL + CONTEXT_SIZE_PSG_LEVEL_AL)
#define CONTEXT_SIZE_PSG_LEVEL_BR         (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_BR))
//
#define CONTEXT_OFFSET_PSG_LEVEL_BL       (CONTEXT_OFFSET_PSG_LEVEL_BR + CONTEXT_SIZE_PSG_LEVEL_BR)
#define CONTEXT_SIZE_PSG_LEVEL_BL         (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_BL))
//
#define CONTEXT_OFFSET_PSG_LEVEL_CR       (CONTEXT_OFFSET_PSG_LEVEL_BL + CONTEXT_SIZE_PSG_LEVEL_BL)
#define CONTEXT_SIZE_PSG_LEVEL_CR         (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_CR))
//
#define CONTEXT_OFFSET_PSG_LEVEL_CL       (CONTEXT_OFFSET_PSG_LEVEL_CR + CONTEXT_SIZE_PSG_LEVEL_CR)
#define CONTEXT_SIZE_PSG_LEVEL_CL         (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_CL))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL     (CONTEXT_OFFSET_PSG_LEVEL_CL + CONTEXT_SIZE_PSG_LEVEL_CL)
#define CONTEXT_SIZE_PLAYCITY_LEVEL       (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_PLAYCITY))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL_PP  (CONTEXT_OFFSET_PLAYCITY_LEVEL + CONTEXT_SIZE_PLAYCITY_LEVEL)
#define CONTEXT_SIZE_PLAYCITY_LEVEL_PP    (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_PP))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL_AR  (CONTEXT_OFFSET_PLAYCITY_LEVEL_PP + CONTEXT_SIZE_PLAYCITY_LEVEL_PP)
#define CONTEXT_SIZE_PLAYCITY_LEVEL_AR    (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_AR))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL_AL  (CONTEXT_OFFSET_PLAYCITY_LEVEL_AR + CONTEXT_SIZE_PLAYCITY_LEVEL_AR)
#define CONTEXT_SIZE_PLAYCITY_LEVEL_AL    (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_AL))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL_BR  (CONTEXT_OFFSET_PLAYCITY_LEVEL_AL + CONTEXT_SIZE_PLAYCITY_LEVEL_AL)
#define CONTEXT_SIZE_PLAYCITY_LEVEL_BR    (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_BR))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL_BL  (CONTEXT_OFFSET_PLAYCITY_LEVEL_BR + CONTEXT_SIZE_PLAYCITY_LEVEL_BR)
#define CONTEXT_SIZE_PLAYCITY_LEVEL_BL    (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_BL))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL_CR  (CONTEXT_OFFSET_PLAYCITY_LEVEL_BL + CONTEXT_SIZE_PLAYCITY_LEVEL_BL)
#define CONTEXT_SIZE_PLAYCITY_LEVEL_CR    (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_CR))
//
#define CONTEXT_OFFSET_PLAYCITY_LEVEL_CL  (CONTEXT_OFFSET_PLAYCITY_LEVEL_CR + CONTEXT_SIZE_PLAYCITY_LEVEL_CR)
#define CONTEXT_SIZE_PLAYCITY_LEVEL_CL    (DWORD_UPPER_ALIGN(SIZETAB_LEVEL_CL))
//
#define CONTEXT_OFFSET_GPBUFFER           (CONTEXT_OFFSET_PLAYCITY_LEVEL_CL + CONTEXT_SIZE_PLAYCITY_LEVEL_CL)
#define CONTEXT_SIZE_GPBUFFER             (DWORD_UPPER_ALIGN(SIZETAB_GPBUFFER))
//
#define CONTEXT_OFFSET_RAM                (CONTEXT_OFFSET_GPBUFFER + CONTEXT_SIZE_GPBUFFER)
#define CONTEXT_SIZE_RAM                  (DWORD_UPPER_ALIGN(SIZETAB_RAM))
//
#define CONTEXT_OFFSET_SND_BUFFER         (CONTEXT_OFFSET_RAM + CONTEXT_SIZE_RAM)
#define CONTEXT_SIZE_SND_BUFFER           (DWORD_UPPER_ALIGN(SIZETAB_SND_BUFFER))
//
#define CONTEXT_OFFSET_TMPI_BUFFER        (CONTEXT_OFFSET_SND_BUFFER + CONTEXT_SIZE_SND_BUFFER)
#define CONTEXT_SIZE_TMPI_BUFFER          (DWORD_UPPER_ALIGN(SIZETAB_TMPI_BUFFER))
//
#define CONTEXT_OFFSET_PLAYCITY_BUFFER    (CONTEXT_OFFSET_TMPI_BUFFER + CONTEXT_SIZE_TMPI_BUFFER)
#define CONTEXT_SIZE_PLAYCITY_BUFFER      (DWORD_UPPER_ALIGN(SIZETAB_PLAYCITY_BUFFER))
//
#define CONTEXT_OFFSET_TMPI_COS_TABLE     (CONTEXT_OFFSET_PLAYCITY_BUFFER + CONTEXT_SIZE_PLAYCITY_BUFFER)
#define CONTEXT_SIZE_TMPI_COS_TABLE       (DWORD_UPPER_ALIGN(SIZETAB_TMPI_COS_TABLE))
//
#define CONTEXT_OFFSET_TMPI_EXP_TABLE     (CONTEXT_OFFSET_TMPI_COS_TABLE + CONTEXT_SIZE_TMPI_COS_TABLE)
#define CONTEXT_SIZE_TMPI_EXP_TABLE       (DWORD_UPPER_ALIGN(SIZETAB_TMPI_EXP_TABLE))
//
#define CONTEXT_OFFSET_TMPI_EXP2_TABLE    (CONTEXT_OFFSET_TMPI_EXP_TABLE + CONTEXT_SIZE_TMPI_EXP_TABLE)
#define CONTEXT_SIZE_TMPI_EXP2_TABLE      (DWORD_UPPER_ALIGN(SIZETAB_TMPI_EXP2_TABLE))
//
#define CONTEXT_OFFSET_TMPI_NOISE_TABLE   (CONTEXT_OFFSET_TMPI_EXP2_TABLE + CONTEXT_SIZE_TMPI_EXP2_TABLE)
#define CONTEXT_SIZE_TMPI_NOISE_TABLE     (DWORD_UPPER_ALIGN(SIZETAB_TMPI_NOISE_TABLE))
//
#define CONTEXT_OFFSET_SESSIONFILENAME    (CONTEXT_OFFSET_TMPI_NOISE_TABLE + CONTEXT_SIZE_TMPI_NOISE_TABLE)
#define CONTEXT_SIZE_SESSIONFILENAME      (DWORD_UPPER_ALIGN(SIZETAB_FILENAME))
//
#define CONTEXT_OFFSET_SOUND_CB_PARAM     (CONTEXT_OFFSET_SESSIONFILENAME + CONTEXT_SIZE_SESSIONFILENAME)
#define CONTEXT_SIZE_SOUND_CB_PARAM       (DWORD_UPPER_ALIGN(sizeof(tSoundCallbackParam)))
//
#define CONTEXT_OFFSET_DRIVE_A            (CONTEXT_OFFSET_SOUND_CB_PARAM + CONTEXT_SIZE_SOUND_CB_PARAM)
#define CONTEXT_SIZE_DRIVE                (DWORD_UPPER_ALIGN(sizeof(tDrive)))
#define CONTEXT_SIZE_DRIVE_A              CONTEXT_SIZE_DRIVE
//
#define CONTEXT_OFFSET_DRIVE_B            (CONTEXT_OFFSET_DRIVE_A + CONTEXT_SIZE_DRIVE_A)
#define CONTEXT_SIZE_DRIVE_B              CONTEXT_SIZE_DRIVE
//
#define CONTEXT_OFFSET_MEMMAP_ROM         (CONTEXT_OFFSET_DRIVE_B + CONTEXT_SIZE_DRIVE_B)
#define CONTEXT_SIZE_MEMMAP_ROM           (DWORD_UPPER_ALIGN(SIZETAB_MEMMAP_ROM))
//
#define SIZETAB_CONTEXT                   (CONTEXT_OFFSET_MEMMAP_ROM + CONTEXT_SIZE_MEMMAP_ROM)
//
// ============================================================================



//
// Errors
//
// copied from ErrorBase.h
#define errNone                           0x0000  // No error
#define memErrorClass                     0x0100  // Memory Manager
#define appErrorClass                     0x8000  // Application-defined errors
//
#define testUErrorClass                   appErrorClass

// copied from MemoryMgr.h
#define memErrNotEnoughSpace              (memErrorClass | 2)
#define memErrInvalidParam                (memErrorClass | 3)   /* invalid param or requested size is too big */

// copied from VFSMgr.h
#define vfsErrorClass                     0x2A00    // Post-3.5 this is defined in ErrorBase.h
#define vfsErrBadData                     (vfsErrorClass | 12)  // operation could not be completed because of invalid data (i.e., import DB from .PRC file)

#if defined(__WIN32__)
  #define vfsErrDSKNoData                 (vfsErrorClass + 1)
  #define vfsErrDSKTooManySides           (vfsErrorClass + 2)
  #define vfsErrDSKNoTrackInfo            (vfsErrorClass + 3)
  #define vfsErrDSKBadSignature           (vfsErrorClass + 4)
  #define vfsErrTapeBadSignature          (vfsErrorClass + 5)
  #define vfsErrTapeInvalid               (vfsErrorClass + 6)
  #define vfsErrTapeUnsupported           (vfsErrorClass + 7)
#elif defined(__PALMOS__)
  #define vfsErrDSKTooManySides           vfsErrBadData
  #define vfsErrDSKNoTrackInfo            vfsErrBadData
  #define vfsErrDSKBadSignature           vfsErrBadData
#endif


//
// Trace breakpoints
//
enum
{
  /*  0 */ TRACE_BP_read_mem = 0,
  /*  1 */ TRACE_BP_signed_read_mem,
  /*  2 */ TRACE_BP_write_mem,
  /*  3 */ TRACE_BP_PNOMain_registers,
  /*  4 */ TRACE_BP_TestU,
  /*  5 */ TRACE_BP_get_byte_address,
  // Must be the last
  NUMBER_OF_TRACE_BP
};


//
// Prototypes declarations
//

#if defined(__WIN32__)

extern tVoid CreateDeclaredSizeSector(tTrack* trackP, tULong CurrentSector);
extern tVoid OptimizeTrackSize(tNativeCPC* NativeCPC, tTrack* trackP);

#ifdef __cplusplus
extern "C" {
#endif
extern __declspec(dllexport) tULong Engine_CPCExecute(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_CPCReset(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_CPCSetColor(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_CPCStart(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_CPCStop(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_DiskEject(tDiskOperation* DiskOperation);
extern __declspec(dllexport) tULong Engine_DiskFormat(tDiskOperation* DiskOperation);
extern __declspec(dllexport) tULong Engine_DiskLoad(tDiskOperation* DiskOperation);
extern __declspec(dllexport) tULong Engine_DiskSave(tDiskOperation* DiskOperation);
extern __declspec(dllexport) tULong Engine_DiskOptimize(tDiskOperation* DiskOperation);
extern __declspec(dllexport) tULong Engine_AutoStart(tDiskOperation* DiskOperation);
extern __declspec(dllexport) tULong Engine_GetCatalog(tDiskOperation* DiskOperation);
extern __declspec(dllexport) tVoid  Sound_Calculate_Level_Tables(tPSG* PSG, tUChar Stereo, tUChar Audio16Bits);
extern __declspec(dllexport) tVoid  Engine_GetGraphicFormat(tEngineGraphicFormat* GraphicFormat);
extern __declspec(dllexport) tBool  Engine_IsVideoVBL(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tBool  Engine_IsVideoHBL(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_GetVideoCharCount(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_GetVideoLineCount(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tULong Engine_GetVideoRasterCount(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tLong  Engine_GetVideoBeamPosX(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tLong  Engine_GetVideoBeamPosY(tNativeCPC* NativeCPC);
extern __declspec(dllexport) tUChar Engine_ReadMem(tNativeCPC* NativeCPC, tULong addr);
extern __declspec(dllexport) tVoid  Engine_WriteMem(tNativeCPC* NativeCPC, tULong addr, tULong val);
extern __declspec(dllexport) tVoid  Engine_TapeEject(tTape* Tape);
extern __declspec(dllexport) tULong Engine_TapeInsert(tTape* Tape, tUChar* TapeDataP, tULong Length);
extern __declspec(dllexport) tVoid  Engine_TapeRewind(tTape* Tape);
extern __declspec(dllexport) tLong  Engine_TapeGetNextBlock(tTape* Tape);
extern __declspec(dllexport) tVoid  Engine_TapeResetCounter(tTape* Tape);
extern __declspec(dllexport) tLong  Engine_TapeGetPrevMessageBlock(tTape* Tape);
extern __declspec(dllexport) tLong  Engine_TapeGetNextMessageBlock(tTape* Tape);
extern __declspec(dllexport) tVoid  Engine_PrinterSetOnline(tPrinter* Printer);
extern __declspec(dllexport) tVoid  Engine_PrinterSetOffline(tPrinter* Printer);
#ifdef __cplusplus
}  /* End of the 'extern "C"' block */
#endif

#endif /* __WIN32__ */

#endif /* ! NATIVE_CPC_H */

