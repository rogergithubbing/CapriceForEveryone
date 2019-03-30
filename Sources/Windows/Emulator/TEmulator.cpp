/*
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


#include <vcl.h>
#include <tchar.h>
#pragma hdrstop
#include "TEmulator.h"
#include "..\Inputs\Keyboard.h"
#include "..\IPF\TIPFContainer.h"
#include "..\IPF\TRAWContainer.h"
#include "..\..\Engine\Native_CPC.h"
#include "..\..\Common\Tables\fdc_command.h"
#include "..\..\Common\Tables\z80daa.h"
#include "..\..\Common\Tables\keyboard_UK.h"
#include "..\..\Common\Tables\keyboard_FR.h"
#include "..\..\Common\Tables\keyboard_SP.h"
#include "..\..\Common\Tables\disk_format.h"
#include "..\..\Common\Native_RLE.h"
#include "..\ZLib\TZIPContainer.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


//=================================
// Options
#undef USE_DYNAMIC_AUDIO_BUFFERS
#define USE_DYNAMIC_AUDIO_BUFFERS

#undef DEBUG_CONVERT_IPF2DSK
//#define DEBUG_CONVERT_IPF2DSK
//=================================


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#undef PATCH_1
#define PATCH_1

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


#define AUTOTYPEPRESSTIME_MS              50    // Key pressed duration for autostart (ms)
#define AUTOTYPEPRESS_CYCLES              ((AUTOTYPEPRESSTIME_MS / CYCLES_MS) + 1)
#define AUTOTYPERELEASETIME_MS            70     // Key released duration for autostart (ms)
#define AUTOTYPERELEASE_CYCLES            ((AUTOTYPERELEASETIME_MS / CYCLES_MS) + 1)
#define AUTOTYPERESETTIME_MS              1500   // Time to wait from Reset before sending keys for autostart (ms)
#define AUTOTYPERESET_CYCLES              ((AUTOTYPERESETTIME_MS / CYCLES_MS) + 1)


// Default preferences value
#define DEFAULT_PREF_SCREENINTENSITY      10
#define DEFAULT_PREF_NIGHTMODE_ACTIVE     0
#define DEFAULT_PREF_SOUNDVOLUME          40


// SNA Errors
#define ERR_SNA_OK                        0
#define ERR_SNA_INVALID                   1
#define ERR_SNA_EMU_NOT_READY             2


// Decathlon mode
#define DECATHLON_PERIOD_START            8
#define DECATHLON_PERIOD_MIN              2
#define DECATHLON_PERIOD_MAX              15

// Combat school mode
#define COMBATSCHOOL_PERIOD_START         8
#define COMBATSCHOOL_PERIOD_MIN           1
#define COMBATSCHOOL_PERIOD_MAX           15


typedef struct
{
  char id[8];
  char unused1[8];
  unsigned char version;
  unsigned char AF[2];
  unsigned char BC[2];
  unsigned char DE[2];
  unsigned char HL[2];
  unsigned char R;
  unsigned char I;
  unsigned char IFF0;
  unsigned char IFF1;
  unsigned char IX[2];
  unsigned char IY[2];
  unsigned char SP[2];
  unsigned char PC[2];
  unsigned char IM;
  unsigned char AFx[2];
  unsigned char BCx[2];
  unsigned char DEx[2];
  unsigned char HLx[2];
  unsigned char ga_pen;
  unsigned char ga_ink_values[17];
  unsigned char ga_ROM_config;
  unsigned char ga_RAM_config;
  unsigned char crtc_reg_select;
  unsigned char crtc_registers[18];
  unsigned char upper_ROM;
  unsigned char ppi_A;
  unsigned char ppi_B;
  unsigned char ppi_C;
  unsigned char ppi_control;
	unsigned char psg_reg_select;
  unsigned char psg_registers[16];
  unsigned char ram_size[2];
  // version 2 info follows
  unsigned char cpc_model;
  unsigned char last_interrupt;
  unsigned char scr_modes[6];
  // version 3 info follows
  unsigned char drvA_DOSfilename[13];
  unsigned char drvB_DOSfilename[13];
  unsigned char cart_DOSfilename[13];
  unsigned char fdc_motor;
  unsigned char drvA_current_track;
  unsigned char drvB_current_track;
  unsigned char drvC_current_track;
  unsigned char drvD_current_track;
  unsigned char printer_data;
  unsigned char psg_env_step;
  unsigned char psg_env_direction;
  unsigned char crtc_type;
  unsigned char crtc_addr[2];
  unsigned char crtc_scanline[2];
  unsigned char crtc_char_count[2];
  unsigned char crtc_line_count;
  unsigned char crtc_raster_count;
  unsigned char crtc_vt_adjust_count;
  unsigned char crtc_hsw_count;
  unsigned char crtc_vsw_count;
  unsigned char crtc_flags[2];
  unsigned char ga_int_delay;
  unsigned char ga_sl_count;
  unsigned char z80_int_pending;
  unsigned char unused2[75];
} t_SNA_header;


__fastcall TEmulator::TEmulator(TSettings* SettingsP,
                                TDrawingContext* DrawingContextP)
{
  mSettingsP = NULL;
	mStreamCPCP = NULL;
  mStreamDOSP = NULL;
  mSiliconDiscP = NULL;

  mAudioSpeakerDataP = NULL;
  mAudioTMPIDataP = NULL;
  mAudioPlayCityDataP = NULL;

  mLightgunPlugEvent = NULL;
  mLightgunTriggerEndEvent = NULL;

  mPaused = false;

  mDiskAFilename = "";
  mDiskBFilename = "";
  mSnapshotFilename = "";
  mConfirmDiskImageUpdates = SettingsP->ConfirmDiskImageUpdates;
  mNeverUpdateDiskImages = SettingsP->NeverUpdateDiskImages;

  // AutoType
  mAutoTypeReady = false;
  mAutoTypeBufferP = NULL;
  mAutoTypeStatus = 0;
  mAutoTypeDuration = 0;
  mAutoPlayRequested = false;
  mAutoPlayDuration = 0;
  mAutoPlayStatus = 0;

  // SNR
  mSNRFileP = NULL;
  mSNRStreamP = NULL;

  // Decathlon mode
  mDecathlonModeActive = false;
  mDecathlonCyclePeriod = DECATHLON_PERIOD_START;

  // Combat school mode
  mCombatSchoolModeActive = false;
  mCombatSchoolCyclePeriod = COMBATSCHOOL_PERIOD_START;

  // AMX Mouse
  mAMXMouseFactorX = (float)SettingsP->AMXMouseFactorX / 100;
  mAMXMouseFactorY = (float)SettingsP->AMXMouseFactorY / 100;

  mContextP = (tUChar*)new unsigned char [SIZETAB_CONTEXT];
  memset(mContextP, 0, SIZETAB_CONTEXT);

  // Pointer affectation into context memory
  mResourcesP = (tContextResources*)(mContextP + CONTEXT_OFFSET_RESOURCES);

  // Créer le buffer d'écran
  mResourcesP->BmpOffScreenBits = (tVoid*)DrawingContextP->EmuScreenBITMAP.bmBits;
  mResourcesP->BmpOffScreenBytesRow = (tUShort)(DrawingContextP->EmuScreenBITMAP.bmWidthBytes);
  mResourcesP->BmpOffScreenSizeInBytes = CPC_SCR_WIDTH * CPC_SCR_HEIGHT;
  mResourcesP->BmpBitsPerPixel = (tULong)DrawingContextP->BitsPerPixel;

	// Initialise preferences
  mPrefP = new tPreferences;
  mPrefP->ScreenIntensity = DEFAULT_PREF_SCREENINTENSITY;
  mPrefP->NightModeActive = DEFAULT_PREF_NIGHTMODE_ACTIVE;
  mPrefP->SoundVolume = DEFAULT_PREF_SOUNDVOLUME;
  mPrefP->SpeakerStereo = SettingsP->AudioSpeakerStereo;
  mPrefP->Speaker16Bits = SettingsP->AudioSpeaker16Bits;
  mPrefP->SpeakerFreq = SettingsP->AudioSpeakerFreq;
  mPrefP->PlayCityFreq = SettingsP->AudioPlayCityFreq;
  mPrefP->PlayCityStereo = SettingsP->AudioPlayCityStereo;
  mPrefP->PlayCity16Bits = SettingsP->AudioPlayCity16Bits;
  mPrefP->PlayCityInvert = SettingsP->AudioPlayCityInvert;

  // Création de la palette
  mPaletteP = new colours_rgb_entry[NATIVECPC_COLOURS_NBELEMENT];

  // Keyboard mapping
  mUseKeyboardAsJoystick = false;
  mASCIIKeyboardP = new tUChar[NB_KEYBOARD_KEYS];
  mHardwareKeyboardP = new tUChar[NB_KEYBOARD_KEYS];
  mJoystickKeyboardP = new tUChar[NB_JOYSTICK_KEYS];
  mVirtualKeyboardAsJoystickP = new tUChar[NB_JOYSTICK_KEYS];
	mHardwareKeyboardAsJoystickP = new tUChar[NB_JOYSTICK_KEYS];

  // Création de la liste des touches appuyées
  mPressedKeyListP = new TList;
  mPressedKeyCounterP = new tUChar[NB_KEYBOARD_KEYS];

  // Création du catalog du disk
  mDiskCatalogEntries = new TStringList;

  // Create messages list
  mSimpleMessages = new TStringList;

  // Apply settings
  Settings = SettingsP;
}
//---------------------------------------------------------------------------


__fastcall TEmulator::~TEmulator(void)
{
  SaveAndEjectDisk(DriveA);
  SaveAndEjectDisk(DriveB);

  EjectTape();

  delete [] mASCIIKeyboardP;
  delete [] mHardwareKeyboardP;
  delete [] mJoystickKeyboardP;
  delete [] mVirtualKeyboardAsJoystickP;
  delete [] mHardwareKeyboardAsJoystickP;

  delete [] mContextP;
  delete [] mAutoTypeBufferP;
  delete [] mPaletteP;

  delete [] mAudioSpeakerDataP;
  delete [] mAudioTMPIDataP;
  delete [] mAudioPlayCityDataP;

  delete mPrefP;

  if (mPressedKeyListP != NULL)
  {
    while (mPressedKeyListP->Count)
    {
      delete mPressedKeyListP->Items[0];
      mPressedKeyListP->Delete(0);
    }

		delete mPressedKeyListP;
  }
  
  delete [] mPressedKeyCounterP;

  delete mStreamCPCP;
  delete mStreamDOSP;
  delete mSiliconDiscP;

  delete mDiskCatalogEntries;
  delete mSimpleMessages;

  delete [] mSNRFileP;
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::Start(void)
{
  // Pointer affectation into context memory
  mNativeCPC = (tNativeCPC*)(mContextP + CONTEXT_OFFSET_NATIVECPC);

  mNativeCPC->prefP = mPrefP;
  mNativeCPC->contextP = mContextP;
  mNativeCPC->BmpOffScreenBits = mResourcesP->BmpOffScreenBits;
  mNativeCPC->BmpOffScreenBytesRow = mResourcesP->BmpOffScreenBytesRow;
  mNativeCPC->BmpOffScreenSizeInBytes = mResourcesP->BmpOffScreenSizeInBytes;
  mNativeCPC->BitsPerPixel = mResourcesP->BmpBitsPerPixel;
  mNativeCPC->WinPalettePtr = mPaletteP;
  mNativeCPC->MemPtrNewPtr = (tMemPtrNewFct)malloc;
  mNativeCPC->MemPtrDeletePtr = (tMemPtrDeleteFct)free;

  // Lire les tables de constantes
  mNativeCPC->FDCCommandTable = FDCCommandTable;
  mNativeCPC->DAATable = DAATable;

  // At start, all keys are released.
  for (int loop=0; loop < NB_KEYBOARD_KEYS; loop++)
	{
    mPressedKeyCounterP[loop] = 0;
  } 

  // Prepare CPC Model
	if      (mSettingsP->CPCBrand == 1) mCPCModel = "Orion";
  else if (mSettingsP->CPCBrand == 2) mCPCModel = "Schneider";
  else if (mSettingsP->CPCBrand == 3) mCPCModel = "Awa";
  else if (mSettingsP->CPCBrand == 4) mCPCModel = "Solavox";
  else if (mSettingsP->CPCBrand == 5) mCPCModel = "Saisho";
  else if (mSettingsP->CPCBrand == 6) mCPCModel = "Triumph";
  else if (mSettingsP->CPCBrand == 7) mCPCModel = "Isp";
  else mCPCModel = "Amstrad";

  // Prepare CRTC Type
  mNativeCPC->crtc_type = mSettingsP->CRTCType;

  // Floppy Drive
  mNativeCPC->floppy_drive = mSettingsP->UseFloppyDrive;
  mNativeCPC->fast_fdc = mSettingsP->UseFastFDC;

  // VHold Adjust
  mNativeCPC->vhold_adjust = mSettingsP->VHoldAdjust;

  // Affecter les ressources du CPC
  switch (mSettingsP->CPCDevice)
  {
    case CPC_MODEL_464_FR:
		{
      mCPCModel += " CPC 464 FR";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC464_FR"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_fr;

			if (mSettingsP->Use64kMemoryExtension == true)
			{
				mCPCModel += " + 64Ko";
			}
		}
		break;

    case CPC_MODEL_464_SP:
		{
      mCPCModel += " CPC 464 SP";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC464_SP"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_sp;

			if (mSettingsP->Use64kMemoryExtension == true)
			{
				mCPCModel += " + 64Ko";
			}
		}
		break;

		case CPC_MODEL_664_UK:
		{
			mCPCModel += " CPC 664 UK";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC664_UK"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_uk;

			if (mSettingsP->Use64kMemoryExtension == true)
			{
				mCPCModel += " + 64Ko";
			}
		}
		break;

		case CPC_MODEL_664_FR:
		{
			mCPCModel += " CPC 664 FR";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC664_UK"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_uk;

			if (mSettingsP->Use64kMemoryExtension == true)
			{
				mCPCModel += " + 64Ko";
			}
		}
		break;

		case CPC_MODEL_6128_UK:
		{
			mCPCModel += " CPC 6128 UK";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC6128_UK"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_uk;
		}
		break;

		case CPC_MODEL_6128_FR:
		{
			mCPCModel += " CPC 6128 FR";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC6128_FR"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_fr;
		}
		break;

		case CPC_MODEL_6128_SP:
		{
			mCPCModel += " CPC 6128 SP";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC6128_SP"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_sp;
		}
		break;

		default:
		{
			mCPCModel += " CPC 464 UK";
			mStreamCPCP = new TResourceStream((int)HInstance, _T("ROMCPC464_UK"), RT_RCDATA);
			mKeyboardCPCP = cpc_kbd_uk;

			if (mSettingsP->Use64kMemoryExtension == true)
			{
				mCPCModel += " + 64Ko";
			}
		}
	}

	mStreamDOSP = new TResourceStream((int)HInstance, _T("ROMAmsdos"), RT_RCDATA);

#ifdef USE_SILICON_DISC
	mSiliconDiscP = new TResourceStream((int)HInstance, _T("ROMSiliconDisk13"), RT_RCDATA);
#endif /* USE_SILICON_DISC */

	Err Error;

	do
	{
		Error = (Err)Engine_CPCStart(mNativeCPC);
		if (Error != errNone)
		{
#ifdef _DEBUG
			Application->MessageBox(_T("[TEmulator] Engine_CPCStart ERROR"), _T("ERROR"), MB_OK);
#endif /* _DEBUG */
			continue;
		}

    //
    // ROM mapping
    //
    // Default mapping
    mNativeCPC->pbROMlo = (tUChar*)mStreamCPCP->Memory;
    mNativeCPC->pbROMhi = mNativeCPC->pbExpansionROM = mNativeCPC->pbROMlo + 16384;
    mNativeCPC->memmap_ROM[7] = (tUChar*)mStreamDOSP->Memory;

    // Audio
    mNativeCPC->TMPISpeech->enabled = mSettingsP->UseTechniMusique;
    mNativeCPC->PlayCity->enabled = mSettingsP->UsePlayCity;
#ifdef USE_DYNAMIC_AUDIO_BUFFERS
    // Overload audio buffers
    mNativeCPC->PSG->pbSndBuffer = mAudioSpeakerDataP;
    mNativeCPC->PSG->pbSndBufferEnd = mAudioSpeakerDataP + mAudioSpeakerSize;
    mNativeCPC->PSG->snd_bufferptr = mAudioSpeakerDataP;
    mNativeCPC->PSG->SndBufferSize = mAudioSpeakerSize;
    mNativeCPC->TMPISpeech->pbBufferStart = mAudioTMPIDataP;
    mNativeCPC->TMPISpeech->pbBufferEnd = mAudioTMPIDataP + mAudioTMPISize;
    mNativeCPC->TMPISpeech->bufferptr = (tShort*)mAudioTMPIDataP;
    mNativeCPC->TMPISpeech->BufferSize = mAudioTMPISize;
    mNativeCPC->PlayCity->pbSndBuffer = mAudioPlayCityDataP;
    mNativeCPC->PlayCity->pbSndBufferEnd = mAudioPlayCityDataP + mAudioPlayCitySize;
    mNativeCPC->PlayCity->snd_bufferptr = mAudioPlayCityDataP;
    mNativeCPC->PlayCity->SndBufferSize = mAudioPlayCitySize;
#endif /* USE_DYNAMIC_AUDIO_BUFFERS */

    //
    // User mapping
    //
    AnsiString ROMPath = ExpandFileName(mSettingsP->RomsDirectory);
    // Lower ROM
    if (mSettingsP->LowerROMFilename.IsEmpty() == false)
    {
      mNativeCPC->pbROMlo = LoadROM(ROMPath+mSettingsP->LowerROMFilename);
      if (mNativeCPC->pbROMlo == NULL)
      {
        mNativeCPC->pbROMlo = (tUChar*)mStreamCPCP->Memory;
      }

      // Update CPC Model
      mCPCModel = "Custom";
    }
    // Upper ROM 0
    if (mSettingsP->UpperROMFilename[0].IsEmpty() == false)
    {
      mNativeCPC->memmap_ROM[0] = LoadROM(ROMPath+mSettingsP->UpperROMFilename[0]);

      // Update CPC Model
      mCPCModel = "Custom";
    }
    // Upper ROMS
    for (int Loop=1; Loop < NB_UPPERROMS; Loop++)
    {
#ifdef USE_SILICON_DISC
      // Check Default Silicon Disc
      if (mSettingsP->UpperROMFilename[Loop] == DefaultSiliconDiscRomName)
      {
        mNativeCPC->memmap_ROM[Loop] = (tUChar*)mSiliconDiscP->Memory;
      }
      else
#endif /* USE_SILICON_DISC */
      if (mSettingsP->UpperROMFilename[Loop] == EmptyRomName)
      {
        mNativeCPC->memmap_ROM[Loop] = NULL;
      }
      else if (mSettingsP->UpperROMFilename[Loop] == DefaultAMSDOSRomName)
      {
        mNativeCPC->memmap_ROM[Loop] = (tUChar*)mStreamDOSP->Memory;
      }
      else if (mSettingsP->UpperROMFilename[Loop].IsEmpty() == false)
      {
        mNativeCPC->memmap_ROM[Loop] = LoadROM(ROMPath+mSettingsP->UpperROMFilename[Loop]);
      }
    }

    // Add CRTC Type
    mCPCModel += " (CRTC type " + AnsiString(mSettingsP->CRTCType) + ")";


    Error = (Err)Engine_CPCSetColor(mNativeCPC);
    if (Error != errNone)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("[TEmulator] Engine_CPCSetColor ERROR"), _T("ERROR"), MB_OK);
#endif /* _DEBUG */
			continue;
		}

		Error = (Err)Reset();
  }
  while (0);

  return (Error);
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SetPaused(bool Value)
{
  mPaused = Value;

  if (mNativeCPC)
  {
    mNativeCPC->paused = (Value == true) ? cTrue : cFalse;
  }
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::Stop(void)
{
  Err Error = (Err)Engine_CPCStop(mNativeCPC);
  if (Error != errNone)
  {
#ifdef _DEBUG
		Application->MessageBox(_T("[TEmulator] Engine_CPCStop ERROR"), _T("ERROR"), MB_OK);
#endif /* _DEBUG */
	}

  if (mNativeCPC->Tape->pbCSWBuffer)
  {
    free(mNativeCPC->Tape->pbCSWBuffer);
    mNativeCPC->Tape->pbCSWBuffer = NULL;
  }

  // Delete loaded User ROMs
  if (mNativeCPC->pbROMlo != (tUChar*)mStreamCPCP->Memory)
  {
    delete[] mNativeCPC->pbROMlo;
  }
  if (mNativeCPC->memmap_ROM[7] != (tUChar*)mStreamDOSP->Memory)
  {
    delete[] mNativeCPC->memmap_ROM[7];
  }
  for (int Loop=0; Loop < NB_UPPERROMS; Loop++)
  {
    // Pass AMSDOS
    if (Loop == 7) continue;

#ifdef USE_SILICON_DISC
    // Check Default Silicon Disc
    if (mNativeCPC->memmap_ROM[Loop] != (tUChar*)mSiliconDiscP->Memory)
#endif /* USE_SILICON_DISC */
    {
      delete[] mNativeCPC->memmap_ROM[Loop];
    }
  }

  delete mStreamCPCP;
  mStreamCPCP = NULL;
  delete mStreamDOSP;
  mStreamDOSP = NULL;
  delete mSiliconDiscP;
  mSiliconDiscP = NULL;

  // Stop Autostart
  delete [] mAutoTypeBufferP;
  mAutoTypeBufferP = NULL;
  mAutoTypeReady = false;
  mAutoTypeStatus = 0;

  return (Error);
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::Restart(TSettings* SettingsP)
{
  // Apply new settings
  Settings = SettingsP;

  //
  // Save context
  //
  tULong snd_enabled = mNativeCPC->PSG->snd_enabled;
  tUChar night_mode = mNativeCPC->night_mode;
  tULong text_capture = mNativeCPC->text_capture;
  tULong fdc_event_capture = mNativeCPC->fdc_event_capture;
  tULong printer_event_capture = mNativeCPC->printer_event_capture;
  tULong breakpoint = mNativeCPC->Z80->Regs.breakpoint;
  tULong channelAEnable = mNativeCPC->PSG->Channel_EnA;
  tULong channelBEnable = mNativeCPC->PSG->Channel_EnB;
  tULong channelCEnable = mNativeCPC->PSG->Channel_EnC;
  tDrive* oldDriveA = mNativeCPC->FDC->DriveA;
  tDrive* oldDriveB = mNativeCPC->FDC->DriveB;

  // Emulator restart
  Stop();
  Err Error = Start();

  //
  // Restore context
  //
  mNativeCPC->PSG->snd_enabled = snd_enabled;
  if (night_mode)
  {
    mNativeCPC->night_mode = night_mode;
    Engine_CPCSetColor(mNativeCPC);
  }
  mNativeCPC->text_capture = text_capture;
  mNativeCPC->fdc_event_capture = fdc_event_capture;
  mNativeCPC->printer_event_capture = printer_event_capture;
  mNativeCPC->Z80->Regs.breakpoint = breakpoint;
  mNativeCPC->PSG->Channel_EnA = channelAEnable;
  mNativeCPC->PSG->Channel_EnB = channelBEnable;
  mNativeCPC->PSG->Channel_EnC = channelCEnable;
  mNativeCPC->FDC->DriveA = oldDriveA;
  mNativeCPC->FDC->DriveB = oldDriveB;

  // Restore lightgun
  if (mLightgunPlugEvent)
  {
    mLightgunPlugEvent();
  }

  return (Error);
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::Reset(void)
{
  Err Error = (Err)Engine_CPCReset(mNativeCPC);
  if (Error != errNone)
  {
#ifdef _DEBUG
		Application->MessageBox(_T("[TEmulator] Engine_CPCReset ERROR"), _T("ERROR"), MB_OK);
#endif /* _DEBUG */
  }

  // Stop and Prepare for next Autostart
  delete [] mAutoTypeBufferP;
  mAutoTypeBufferP = NULL;
  mAutoTypeReady = false;
  mAutoTypeStatus = 0;

  // Stop Playback session
  StopSNRSession();

  return (Error);
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SetSettings(TSettings* SettingsP)
{
  mSettingsP = SettingsP;

  mConfirmDiskImageUpdates = mSettingsP->ConfirmDiskImageUpdates;
  mNeverUpdateDiskImages = mSettingsP->NeverUpdateDiskImages;

  //
	// Update preferences
  //
  mPrefP->CPCModel = (tUChar)mSettingsP->CPCDevice;
  mPrefP->ScreenType = (mSettingsP->ColorMonitor == true) ? 0 : 1;

  mPrefP->Use64kMemoryExtension = mSettingsP->Use64kMemoryExtension;
  mPrefP->Use256kMemoryExtension = mSettingsP->Use256kMemoryExtension;
#ifndef USE_SILICON_DISC
  mPrefP->Use256kSiliconDisc = false;
#else /* USE_SILICON_DISC */
  mPrefP->Use256kSiliconDisc = mSettingsP->Use256kSiliconDisc;
#endif /* USE_SILICON_DISC */

  // Prepare jumpers
  mPrefP->CPCJumpers = CPC_JUMPER_50HZ;
  if (mSettingsP->CPCBrand == 1) { mPrefP->CPCJumpers |= CPC_JUMPER_ORION; }
  else if (mSettingsP->CPCBrand == 2) { mPrefP->CPCJumpers |= CPC_JUMPER_SCHNEIDER; }
  else if (mSettingsP->CPCBrand == 3) { mPrefP->CPCJumpers |= CPC_JUMPER_AWA; }
  else if (mSettingsP->CPCBrand == 4) { mPrefP->CPCJumpers |= CPC_JUMPER_SOLAVOX; }
  else if (mSettingsP->CPCBrand == 5) { mPrefP->CPCJumpers |= CPC_JUMPER_SAISHO; }
  else if (mSettingsP->CPCBrand == 6) { mPrefP->CPCJumpers |= CPC_JUMPER_TRIUMPH; }
  else if (mSettingsP->CPCBrand == 7) { mPrefP->CPCJumpers |= CPC_JUMPER_ISP; }
  else { mPrefP->CPCJumpers |= CPC_JUMPER_AMSTRAD; }

  // Keyboard mapping
  UpdateKeyboardMapping();

  //
  // Create audio buffers
  //
  // Speaker
  mAudioSpeakerSize = mSettingsP->AudioSpeakerFreq / CYCLES_PER_SECOND;
  mAudioSpeakerSize *= mSettingsP->AudioSpeakerStereo == true ? 2 : 1;
  mAudioSpeakerSize *= mSettingsP->AudioSpeaker16Bits == true ? 2 : 1;
  delete [] mAudioSpeakerDataP;
  mAudioSpeakerDataP = new tUChar[mAudioSpeakerSize];
  // TMPI Speech
  mAudioTMPISize = mSettingsP->AudioTMPIFreq / CYCLES_PER_SECOND;
  mAudioTMPISize *= 2; // 16Bits mono
  delete [] mAudioTMPIDataP;
  mAudioTMPIDataP = new tUChar[mAudioTMPISize];
  // PlayCity
  mAudioPlayCitySize = mSettingsP->AudioPlayCityFreq / CYCLES_PER_SECOND;
  mAudioPlayCitySize *= mSettingsP->AudioPlayCityStereo == true ? 2 : 1;
  mAudioPlayCitySize *= mSettingsP->AudioPlayCity16Bits == true ? 2 : 1;
  delete [] mAudioPlayCityDataP;
  mAudioPlayCityDataP = new tUChar[mAudioPlayCitySize];
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::Execute(void)
{
  // Execute emulation
  Err Code = (Err)Engine_CPCExecute(mNativeCPC);

  if (Code & EC_FRAME_COMPLETE)
  {
    // Tape message to be displayed
    if (mNativeCPC->Tape->pbMessagesStart)
    {
      // Create emulator messages
      CreateMessagesFromTape(mNativeCPC->Tape);
      mNativeCPC->Tape->pbMessagesStart = 0;
    }

    if (mSNRStreamP)
    {
      PlaySNRStream();
    }
  }

#ifndef USE_AUDIO_FILLED_BUFFER
  if (Code & EC_SOUND_BUFFER)
  {
    mNativeCPC->PSG->FilledBufferSize = 0;
#ifdef ENABLE_TMPI_SPEECH
    mNativeCPC->TMPISpeech->FilledBufferSize = 0;
#endif /* ENABLE_TMPI_SPEECH */
#ifdef ENABLE_PLAYCITY
    mNativeCPC->PlayCity->FilledBufferSize = 0;
#endif /* ENABLE_PLAYCITY */
  }
#endif /* !USE_AUDIO_FILLED_BUFFER */

  if (Code & EC_ERROR_MASK)
  {
		String Message = _T("[TEmulator] Engine_CPCExecute ERROR=0x") + String::IntToHex(Code>>24, 4);
#ifdef _DEBUG
		Application->MessageBox(Message.c_str(), _T("ERROR"), MB_OK);
#endif /* _DEBUG */
  }

  //
  // Cycle End operations
  //
  if (Code & EC_CYCLE_COUNT)
  {
    // AutoType
    if (mAutoTypeReady == false)
    {
      mAutoTypeDuration++;
      if (mAutoTypeDuration > (int)AUTOTYPERESET_CYCLES)
      {
        mAutoTypeReady = true;
      }
    }
    else if (mAutoTypeBufferP)
    {
      HandleAutoType();
    }
    else if (mAutoPlayRequested == true)
    {
      HandleAutoPlay();
    }

    // Decathlon mode
    if (mDecathlonModeActive == true)
    {
      mDecathlonCycleCount--;
      if (!mDecathlonCycleCount)
      {
        mDecathlonCycleCount = mDecathlonCyclePeriod;
        // Release current key
        tUShort cpc_key = mKeyboardCPCP[mDecathlonCurrentKey];
        RELEASE_KEY(cpc_key);
        // Toggle key
        mDecathlonCurrentKey = mDecathlonCurrentKey == CPC_KEY_J0_LEFT ? CPC_KEY_J0_RIGHT : CPC_KEY_J0_LEFT;
        // Press key
        cpc_key = mKeyboardCPCP[mDecathlonCurrentKey];
        PRESS_KEY(cpc_key);
      }
    }
    // Combat school mode
    else if (mCombatSchoolModeActive == true)
    {
      mCombatSchoolCycleCount--;
      if (!mCombatSchoolCycleCount)
      {
        mCombatSchoolCycleCount = mCombatSchoolCyclePeriod;
        // Release current key
        tUShort cpc_key = mKeyboardCPCP[mCombatSchoolCurrentKey];
        RELEASE_KEY(cpc_key);
        // Get next key
        if (mCombatSchoolCurrentKey == CPC_KEY_J0_LEFT) mCombatSchoolCurrentKey = CPC_KEY_J0_UP;
        else if (mCombatSchoolCurrentKey == CPC_KEY_J0_UP) mCombatSchoolCurrentKey = CPC_KEY_J0_RIGHT;
        else if (mCombatSchoolCurrentKey == CPC_KEY_J0_RIGHT) mCombatSchoolCurrentKey = CPC_KEY_J0_DOWN;
        else mCombatSchoolCurrentKey = CPC_KEY_J0_LEFT;
        // Press key
        cpc_key = mKeyboardCPCP[mCombatSchoolCurrentKey];
        PRESS_KEY(cpc_key);
      }
    }

    // Lightgun trigger end event
    if (mLightgunTriggerEndEvent != NULL)
    {
      if (mNativeCPC->timer_from_reset > mNativeCPC->lightgun_trigger_duration)
      {
        mLightgunTriggerEndEvent();
        mLightgunTriggerEndEvent = NULL;
      }
    }
  }

  return (Code);
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::Save(TEmulatorContext* ContextP)
{
  if (ContextP == NULL)
    return errBadMemoryAllocation;

  // Audio
  ContextP->AudioEnable = IsAudioActive();

  // Save Drive A context
  ContextP->DriveAFilename = mDiskAFilename;
  memcpy(&ContextP->DriveA, mNativeCPC->FDC->DriveA, sizeof(tDrive));
  memset(mNativeCPC->FDC->DriveA, 0, sizeof(tDrive));

  // Save Drive B context
  ContextP->DriveBFilename = mDiskBFilename;
  memcpy(&ContextP->DriveB, mNativeCPC->FDC->DriveB, sizeof(tDrive));
  memset(mNativeCPC->FDC->DriveB, 0, sizeof(tDrive));

  // Save Tape context
  ContextP->TapeFilename = mTapeFilename;
  memcpy(&ContextP->Tape, mNativeCPC->Tape, sizeof(tTape));
  memset(mNativeCPC->Tape, 0, sizeof(tTape));

  return (errNone);
}
//---------------------------------------------------------------------------

Err __fastcall TEmulator::Restore(const TEmulatorContext* ContextP)
{
  if (ContextP == NULL)
    return errBadMemoryAllocation;

  // Audio
  if (ContextP->AudioEnable == true)
  {
    AudioStart();
  }

  // Restore Drive A context
  mDiskAFilename = ContextP->DriveAFilename;
  memcpy(mNativeCPC->FDC->DriveA, &ContextP->DriveA, sizeof(tDrive));

  // Restore Drive B context
  mDiskBFilename = ContextP->DriveBFilename;
  memcpy(mNativeCPC->FDC->DriveB, &ContextP->DriveB, sizeof(tDrive));

  // Restore Tape context
  mTapeFilename = ContextP->TapeFilename;
  memcpy(mNativeCPC->Tape, &ContextP->Tape, sizeof(tTape));

  return (errNone);
}
//---------------------------------------------------------------------------


int __fastcall TEmulator::GetPalette(TColor* PaletteP)
{
  for (int index = 0;
       index < NATIVECPC_COLOURS_NBELEMENT;
       index++)
  {
    PaletteP[index] = TColor(RGB(mPaletteP[index].colour_r,
                                 mPaletteP[index].colour_g,
                                 mPaletteP[index].colour_b));
  }

  return (NATIVECPC_COLOURS_NBELEMENT);
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::ToggleNightMode(void)
{
  // Toggle night mode
  mNativeCPC->night_mode ^= 0x01;
  
  // Prepare new palette
  Err Error = (Err)Engine_CPCSetColor(mNativeCPC);

  return (Error);
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::SetScreenType(bool ScreenType)
{
  mPrefP->ScreenType = (ScreenType == true) ? 0 : 1;
  mNativeCPC->scr_tube = mPrefP->ScreenType;

  // Prepare new palette
  Err Error = (Err)Engine_CPCSetColor(mNativeCPC);

  return (Error);
}
//---------------------------------------------------------------------------


bool __fastcall TEmulator::OnASCIIKeyPress(WORD &VirtualKey, TShiftState Shift)
{
bool KeyTreated = false;
tUChar cpc_key_index;

  cpc_key_index = mASCIIKeyboardP[(tUChar)VirtualKey];
  if (cpc_key_index == CPC_KEY_INVALID)
  {
    // Key CTRL released or key ALTGR pressed
    if ( (!Shift.Contains(ssCtrl))
         || (Shift.Contains(ssCtrl) && Shift.Contains(ssAlt)) )
    {
      TKeyboardState ks;
      GetKeyboardState(ks);
      if (Shift.Contains(ssShift))
      {
        ks[VK_SHIFT] |= 0x80;
      }
      WORD AsciiKey;
      int keycode = ToAscii(VirtualKey,
                            MapVirtualKey(VirtualKey, 0),
                            ks,
                            &AsciiKey,
                            0);
      if (keycode == 1)
      {
        cpc_key_index = ascii_to_cpc_keyindex[(tUChar)AsciiKey];
      }
    }
    else
    {
      if (!Shift.Contains(ssCtrl))
      {
        cpc_key_index = ascii_to_cpc_keyindex[(tUChar)VirtualKey];
      }
    }
  }

  // Check if key already pressed
  if (cpc_key_index != CPC_KEY_INVALID)
  {
    for (int Loop=0; Loop < mPressedKeyListP->Count; Loop++)
    {
      TPressedKey* PressedKeyP = (TPressedKey*)mPressedKeyListP->Items[Loop];

      if (PressedKeyP->VirtualKey == VirtualKey)
      {
        tUShort cpc_key = mKeyboardCPCP[PressedKeyP->cpc_key];
        mPressedKeyCounterP[(tUChar)cpc_key]++;

        // Key already pressed, do not take into account.
        cpc_key_index = CPC_KEY_INVALID;
        break;
      }
    }
  }

  if (cpc_key_index != CPC_KEY_INVALID)
  {
    //
    // Decathlon mode active
    //
    if (mDecathlonModeActive == true)
    {
      // Speed key
      if (cpc_key_index == CPC_KEY_J0_LEFT)
      {
        // Increase period to slow down
        if (mDecathlonCyclePeriod < DECATHLON_PERIOD_MAX)
        {
          mDecathlonCyclePeriod++;
        }

        // Key has been treated
        cpc_key_index = CPC_KEY_INVALID;
        KeyTreated = true;
      }
      else if (cpc_key_index == CPC_KEY_J0_RIGHT)
      {
        // Decrease period to speed up
        if (mDecathlonCyclePeriod > DECATHLON_PERIOD_MIN)
        {
          mDecathlonCyclePeriod--;
        }

        // Key has been treated
        cpc_key_index = CPC_KEY_INVALID;
        KeyTreated = true;
      }
    }
    //
    // Combat school mode
    //
    else if (mCombatSchoolModeActive == true)
    {
      // Speed key
      if (cpc_key_index == CPC_KEY_J0_LEFT)
      {
        // Increase period to slow down
        if (mCombatSchoolCyclePeriod < COMBATSCHOOL_PERIOD_MAX)
        {
          mCombatSchoolCyclePeriod++;
        }

        // Key has been treated
        cpc_key_index = CPC_KEY_INVALID;
        KeyTreated = true;
      }
      else if (cpc_key_index == CPC_KEY_J0_RIGHT)
      {
        // Decrease period to speed up
        if (mCombatSchoolCyclePeriod > COMBATSCHOOL_PERIOD_MIN)
        {
          mCombatSchoolCyclePeriod--;
        }

        // Key has been treated
        cpc_key_index = CPC_KEY_INVALID;
        KeyTreated = true;
      }
    }
  }

  if (cpc_key_index != CPC_KEY_INVALID)
  {
    tUShort cpc_key = mKeyboardCPCP[cpc_key_index];

    // Use press key counter to handle game controllers
    PRESS_KEY(cpc_key);
    mPressedKeyCounterP[(tUChar)cpc_key]++;

    // Add key pressed
    TPressedKey* PressedKeyP = new TPressedKey;
    PressedKeyP->VirtualKey = VirtualKey;
    PressedKeyP->cpc_key = cpc_key_index;
    mPressedKeyListP->Add(PressedKeyP);

    // Update SHIFT and CONTROL state depending on all pressed keys
    SetASCIIKeySHIFTandCTRLState();

    KeyTreated = true;
  }

  // Stop autotype
  if ( (KeyTreated == true) && (mAutoTypeBufferP != NULL) )
  {
    delete [] mAutoTypeBufferP;
    mAutoTypeBufferP = NULL;
    mAutoTypeStatus = 0;
  }

  return (KeyTreated);
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::OnHardwareKeyPress(WORD &VirtualKey, TShiftState Shift)
{
bool KeyTreated = false;

  tUChar cpc_key = mHardwareKeyboardP[VirtualKey];

  // Check if key already pressed
  if (cpc_key != CPC_KEY_INVALID)
  {
    for (int Loop=0; Loop < mPressedKeyListP->Count; Loop++)
    {
      TPressedKey* PressedKeyP = (TPressedKey*)mPressedKeyListP->Items[Loop];

      if (PressedKeyP->VirtualKey == VirtualKey)
      {
        mPressedKeyCounterP[cpc_key]++;

        // Key already pressed, do not take into account.
        cpc_key = CPC_KEY_INVALID;
        break;
      }
    }
  }

  if (cpc_key != CPC_KEY_INVALID)
  {
    //
    // Decathlon mode active
    //
    if (mDecathlonModeActive == true)
    {
      // Speed key
      if (cpc_key == mKeyboardCPCP[CPC_KEY_J0_LEFT])
      {
        // Increase period to slow down
        if (mDecathlonCyclePeriod < DECATHLON_PERIOD_MAX)
        {
          mDecathlonCyclePeriod++;
        }

        // Key has been treated
        cpc_key = CPC_KEY_INVALID;
        KeyTreated = true;
      }
      else if (cpc_key == mKeyboardCPCP[CPC_KEY_J0_RIGHT])
      {
        // Decrease period to speed up
        if (mDecathlonCyclePeriod > DECATHLON_PERIOD_MIN)
        {
          mDecathlonCyclePeriod--;
        }

        // Key has been treated
        cpc_key = CPC_KEY_INVALID;
        KeyTreated = true;
      }
    }
    //
    // Combat school mode
    //
    else if (mCombatSchoolModeActive == true)
    {
      // Speed key
      if (cpc_key == mKeyboardCPCP[CPC_KEY_J0_LEFT])
      {
        // Increase period to slow down
        if (mCombatSchoolCyclePeriod < COMBATSCHOOL_PERIOD_MAX)
        {
          mCombatSchoolCyclePeriod++;
        }

        // Key has been treated
        cpc_key = CPC_KEY_INVALID;
        KeyTreated = true;
      }
      else if (cpc_key == mKeyboardCPCP[CPC_KEY_J0_RIGHT])
      {
        // Decrease period to speed up
        if (mCombatSchoolCyclePeriod > COMBATSCHOOL_PERIOD_MIN)
        {
          mCombatSchoolCyclePeriod--;
        }

        // Key has been treated
        cpc_key = CPC_KEY_INVALID;
        KeyTreated = true;
      }
    }
  }

  if (cpc_key != CPC_KEY_INVALID)
  {
    // Use press key counter to handle game controllers
    PRESS_KEY(cpc_key);
    mPressedKeyCounterP[cpc_key]++;

    // Add key pressed
    TPressedKey* PressedKeyP = new TPressedKey;
    PressedKeyP->VirtualKey = VirtualKey;
    PressedKeyP->cpc_key = cpc_key;
    mPressedKeyListP->Add(PressedKeyP);

    KeyTreated = true;
  }

	// Stop autotype
	if ( (KeyTreated == true) && (mAutoTypeBufferP != NULL) )
	{
		delete [] mAutoTypeBufferP;
		mAutoTypeBufferP = NULL;
		mAutoTypeStatus = 0;
	}

  NOT_USED(Shift);

	return (KeyTreated);
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::OnASCIIKeyRelease(WORD &VirtualKey, TShiftState Shift)
{
bool KeyTreated = false;

  // Look into pressed key list
  for (int Loop=0; Loop < mPressedKeyListP->Count; Loop++)
  {
    TPressedKey* PressedKeyP = (TPressedKey*)mPressedKeyListP->Items[Loop];

    if (PressedKeyP->VirtualKey == VirtualKey)
    {
      // Use press key counter to handle game controllers
      tUShort cpc_key = mKeyboardCPCP[PressedKeyP->cpc_key];
      mPressedKeyCounterP[(tUChar)cpc_key]--;
      if (mPressedKeyCounterP[(tUChar)cpc_key] == 0)
      {
        RELEASE_KEY(cpc_key);

        // Delete pressed key
        mPressedKeyListP->Delete(Loop);
        delete PressedKeyP;

        // Update SHIFT and CONTROL state depending on remaining pressed keys
        SetASCIIKeySHIFTandCTRLState();
      }

      KeyTreated = true;

      break;
    }
  }

  NOT_USED(Shift);

  return (KeyTreated);
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::OnHardwareKeyRelease(WORD &VirtualKey, TShiftState Shift)
{
bool KeyTreated = false;

  // Look into pressed key list
  for (int Loop=0; Loop < mPressedKeyListP->Count; Loop++)
  {
    TPressedKey* PressedKeyP = (TPressedKey*)mPressedKeyListP->Items[Loop];

    if (PressedKeyP->VirtualKey == VirtualKey)
    {
      tUChar cpc_key = (tUChar)PressedKeyP->cpc_key;

      // Use press key counter to handle game controllers
      mPressedKeyCounterP[cpc_key]--;
      if (mPressedKeyCounterP[cpc_key] == 0)
      {
        RELEASE_KEY(cpc_key);

        // Delete pressed key
        mPressedKeyListP->Delete(Loop);
        delete PressedKeyP;
      }

      KeyTreated = true;

      break;
    }
  }

  NOT_USED(Shift);

  return (KeyTreated);
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::OnCPCKeycodeKeyPress(tUChar CPCKeycode)
{
  if (CPCKeycode != CPC_KEY_INVALID)
  {
    if (!mPressedKeyCounterP[CPCKeycode])
    {
      PRESS_KEY(CPCKeycode);
    }
    mPressedKeyCounterP[CPCKeycode]++;
  }
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::OnCPCKeycodeKeyRelease(tUChar CPCKeycode)
{
  if (CPCKeycode != CPC_KEY_INVALID)
  {
    if (mPressedKeyCounterP[CPCKeycode])
    {
      mPressedKeyCounterP[CPCKeycode]--;
      if (!mPressedKeyCounterP[CPCKeycode])
      {
        RELEASE_KEY(CPCKeycode);
      }
    }
  }
}
//---------------------------------------------------------------------------



tUShort __fastcall TEmulator::GetKeySHIFTandCTRLState(void)
{
tUShort usKeyStatus = 0;

  // Save SHIFT state
  if (IS_KEY_PRESSED(CPC_KBD_SHIFT))
  {
    usKeyStatus |= MOD_CPC_SHIFT;
  }

  // Save CTRL state
  if (IS_KEY_PRESSED(CPC_KBD_CTRL))
  {
    usKeyStatus |= MOD_CPC_CTRL;
  }

  return usKeyStatus;
}
//---------------------------------------------------------------------------
tUShort __fastcall TEmulator::GetKeySHIFTandCTRLState(tUShort usKeyState)
{
  return (usKeyState & (MOD_CPC_SHIFT | MOD_CPC_CTRL));
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::SetKeySHIFTandCTRLState(tUShort usKeyState)
{
  if (usKeyState & MOD_CPC_SHIFT) // CPC SHIFT key required?
  {
    PRESS_KEY(CPC_KBD_SHIFT); // key needs to be SHIFTed
  }
  else
  {
    RELEASE_KEY(CPC_KBD_SHIFT); // make sure key is unSHIFTed
  }

  if (usKeyState & MOD_CPC_CTRL) // CPC CONTROL key required?
  {
    PRESS_KEY(CPC_KBD_CTRL); // CONTROL key is held down
  }
  else
  {
    RELEASE_KEY(CPC_KBD_CTRL); // // make sure CONTROL key is released
  }
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SetASCIIKeySHIFTandCTRLState(void)
{
#ifndef PATCH_1

  RELEASE_KEY(CPC_KBD_SHIFT); // make sure key is unSHIFTed
  RELEASE_KEY(CPC_KBD_CTRL); // make sure CONTROL key is released

  // Check all pressed keys
  for (int Loop=0; Loop < mPressedKeyListP->Count; Loop++)
  {
    TPressedKey* PressedKeyP = (TPressedKey*)mPressedKeyListP->Items[Loop];
    tUShort cpc_key = mKeyboardCPCP[PressedKeyP->cpc_key];

    if (cpc_key & MOD_CPC_SHIFT) // CPC SHIFT key required?
    {
      PRESS_KEY(CPC_KBD_SHIFT); // key needs to be SHIFTed
    }

    if (cpc_key & MOD_CPC_CTRL) // CPC CONTROL key required?
    {
      PRESS_KEY(CPC_KBD_CTRL); // CONTROL key is held down
    }
  }

#else /* PATCH_1 */

  tUShort cpc_key = 0U;

  // Check all pressed keys
  for (int Loop=0; Loop < mPressedKeyListP->Count; Loop++)
  {
    TPressedKey* PressedKeyP = (TPressedKey*)mPressedKeyListP->Items[Loop];
    cpc_key |= mKeyboardCPCP[PressedKeyP->cpc_key];
  }

  SetKeySHIFTandCTRLState(cpc_key);

#endif /* PATCH_1 */
}
/*----------------------------------------------------------------------------*/


tUShort __fastcall TEmulator::KeyboardGetAsciiSHIFTandCTRL(char key)
{
tUShort SHIFTandCTRLState = 0;

  tUChar cpc_key_index = ascii_to_cpc_keyindex[key];
  if (cpc_key_index != CPC_KEY_INVALID)
  {
    tUShort cpc_key = mKeyboardCPCP[cpc_key_index];
    SHIFTandCTRLState = GetKeySHIFTandCTRLState(cpc_key);
  }

  return (SHIFTandCTRLState);
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::KeyboardSetAsciiKeyDown(char key)
{
  tUChar cpc_key_index = ascii_to_cpc_keyindex[key];
  if (cpc_key_index != CPC_KEY_INVALID)
  {
    tUShort cpc_key = mKeyboardCPCP[cpc_key_index];

    PRESS_KEY(cpc_key);

    SetKeySHIFTandCTRLState(cpc_key);
  }
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::KeyboardSetAsciiKeyUp(char key)
{
  tUChar cpc_key_index = ascii_to_cpc_keyindex[key];
  if (cpc_key_index != CPC_KEY_INVALID)
  {
    tUShort cpc_key = mKeyboardCPCP[cpc_key_index];

    RELEASE_KEY(cpc_key);

    if (cpc_key & MOD_CPC_SHIFT) // CPC SHIFT key required?
    {
      RELEASE_KEY(CPC_KBD_SHIFT); // make sure key is unSHIFTed
    }

    if (cpc_key & MOD_CPC_CTRL) // CPC CONTROL key required?
    {
      RELEASE_KEY(CPC_KBD_CTRL); // make sure CONTROL key is released
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::KeyboardSetAsciiSHIFTandCTRL(char key)
{
  tUChar cpc_key_index = ascii_to_cpc_keyindex[key];
  if (cpc_key_index != CPC_KEY_INVALID)
  {
    tUShort cpc_key = mKeyboardCPCP[cpc_key_index];

    SetKeySHIFTandCTRLState(cpc_key);
  }
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SwapDrives(void)
{
  // Save context
  tULong oldDurrentTrackDriveA = mNativeCPC->FDC->DriveA->current_track;
  tULong oldDurrentTrackDriveB = mNativeCPC->FDC->DriveB->current_track;

  // Swap Drives
  tDrive* driveTempP = mNativeCPC->FDC->DriveA;
  mNativeCPC->FDC->DriveA = mNativeCPC->FDC->DriveB;
  mNativeCPC->FDC->DriveB = driveTempP;

  // Restore context
  mNativeCPC->FDC->DriveA->current_track = oldDurrentTrackDriveA;
  mNativeCPC->FDC->DriveB->current_track = oldDurrentTrackDriveB;

  // Swap Disks filename
  AnsiString TempFilename = mDiskAFilename;
  mDiskAFilename = mDiskBFilename;
  mDiskBFilename = TempFilename;

  // Signal Drives content change
  mNativeCPC->FDC->DriveA->signal_updated++;
  mNativeCPC->FDC->DriveA->signal_content_change++;
  mNativeCPC->FDC->DriveB->signal_updated++;
  mNativeCPC->FDC->DriveB->signal_content_change++;
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::EjectDisk(TDiskDrive drive)
{
  // Prepare disk operation
  tDiskOperation* DiskOperationP = (tDiskOperation*)(NativeCPC->contextP + CONTEXT_OFFSET_DISKOPERATION);
  DiskOperationP->NativeCPC = mNativeCPC;
  DiskOperationP->Drive = (drive == DriveA) ? NativeCPC->FDC->DriveA : NativeCPC->FDC->DriveB;
  DiskOperationP->DiskContentP = cNull;

  Err Result = (Err)Engine_DiskEject(DiskOperationP);

  // Free disk content memory
  if (DiskOperationP->DiskContentP != cNull)
  {
    free(DiskOperationP->DiskContentP);
    DiskOperationP->DiskContentP = cNull;
  }

  if (drive == DriveA)
  {
    mDiskAFilename = "";
  }
  else
  {
    mDiskBFilename = "";
  }

  // Signal Drive content change
  DiskOperationP->Drive->signal_updated++;
  DiskOperationP->Drive->signal_content_change++;

  return (Result);
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::LoadDisk(AnsiString Filename,
                                   TDiskDrive drive)
{
  if (Filename.IsEmpty() == true)
    return errNone;

  // Archive Container
  AnsiString InArchiveFilename = "";
  if (Filename.LowerCase().Pos(CONTAINER_ZIP_EXT))
  {
    int ExtensionPos = Filename.LowerCase().Pos(CONTAINER_ZIP_EXT);
    InArchiveFilename = Filename.SubString(ExtensionPos+5, Filename.Length());
    Filename = Filename.SubString(1, ExtensionPos+3);
  }

  AnsiString FilePath = ExtractFilePath(Filename);
  if (FilePath.IsEmpty() == true)
  {
    Filename = mSettingsP->DiscDirectory + Filename;
  }

  if (FileExists(Filename) == false)
    return errFileDoesNotExist;

  bool WriteProtected = (FileGetAttr(Filename) & faReadOnly) ? true : false;

  EjectDisk(drive);

  // Prepare disk operation
  tDiskOperation* DiskOperationP = (tDiskOperation*)(mNativeCPC->contextP + CONTEXT_OFFSET_DISKOPERATION);
  DiskOperationP->NativeCPC = mNativeCPC;
  DiskOperationP->Drive = (drive == DriveA) ? NativeCPC->FDC->DriveA : NativeCPC->FDC->DriveB;

  Err Error = errNone;

  // Ouverture du fichier en lecture
  int iHandle = FileOpen(Filename, fmOpenRead);
  // Met le curseur à la fin du fichier pour récupérer la taille du fichier
  int iLength = FileSeek(iHandle, 0, 2);
  // Remet le curseur en début de fichier pour la prochaine lecture
  FileSeek(iHandle, 0, 0);
  DiskOperationP->disk_size = iLength;
  DiskOperationP->DiskContentP = (tUChar*)malloc(DiskOperationP->disk_size);

  // Read file
  FileRead(iHandle,
           DiskOperationP->DiskContentP,
           iLength);

  FileClose(iHandle);

  // Load file from archive container
  if (InArchiveFilename.IsEmpty() == false)
  {
    WriteProtected = false;

    TZIPContainer* ZIPContainer = new TZIPContainer(Filename);

    // Allocate memory
    int UncompressedSize = ZIPContainer->GetFileUncompressedSize(InArchiveFilename);
    tUChar* FileData = (tUChar*)malloc(UncompressedSize);

    // Read file from archive
    if (ZIPContainer->ReadFile(InArchiveFilename, FileData, UncompressedSize) != UNZ_OK)
    {
      Error = errBadInflate;
    }

    delete ZIPContainer;

    // Prepare disk operation
    free(DiskOperationP->DiskContentP);
    DiskOperationP->disk_size = UncompressedSize;
    DiskOperationP->DiskContentP = FileData;
    Filename = InArchiveFilename;
  }

  // IPF
  if (Error == errNone)
  {
    if (Filename.LowerCase().Pos(DISKIMAGE_IPF_EXT))
    {
      TIPFContainer* IPF = new TIPFContainer();
      Error = IPF->Load(DiskOperationP->DiskContentP,
                        DiskOperationP->disk_size);
      if (Error == errNone)
      {
        // Replace IPF by DSK
        free(DiskOperationP->DiskContentP);
        DiskOperationP->disk_size = IPF->DSKSize;
        DiskOperationP->DiskContentP = (tUChar*)malloc(DiskOperationP->disk_size);
        IPF->BuildDSKStructure(DiskOperationP->DiskContentP,
                               DiskOperationP->disk_size);
      }

      delete IPF;

#ifdef DEBUG_CONVERT_IPF2DSK
      // Change extension to DSK.
      AnsiString DebugFilename = ChangeFileExt(Filename, ".debug.dsk");
      // Création du nouveau fichier
      int iHandle = FileCreate(DebugFilename);
      FileWrite(iHandle, DiskOperationP->DiskContentP, DiskOperationP->disk_size);
      FileClose(iHandle);
#endif /* DEBUG_CONVERT_IPF2DSK */
    }

#ifdef _DEBUG
    if (Filename.LowerCase().Pos(DISKIMAGE_RAW_EXT))
    {
      TRAWContainer* RAW = new TRAWContainer();
      Error = RAW->Load(DiskOperationP->DiskContentP,
                        DiskOperationP->disk_size);
      /*if (Error == errNone)
      {
        // Replace RAW by DSK
        free(DiskOperationP->DiskContentP);
        DiskOperationP->disk_size = RAW->DSKSize;
        DiskOperationP->DiskContentP = (tUChar*)malloc(DiskOperationP->disk_size);
        RAW->BuildDSKStructure(DiskOperationP->DiskContentP,
                               DiskOperationP->disk_size);
      }*/

      delete RAW;
    }
#endif /* _DEBUG */
  }

  // Set write protected
  if (WriteProtected == true)
  {
    DiskOperationP->Drive->write_protected = ~0;
    DiskOperationP->Drive->read_only = 1;
  }

  // Signal drive content change
  DiskOperationP->Drive->signal_updated++;
  DiskOperationP->Drive->signal_content_change++;

  if (drive == DriveA)
  {
    mDiskAFilename = Filename;
  }
  else
  {
    mDiskBFilename = Filename;
  }

  //
  // Load entire disk image
  //
  if (Error == errNone)
  {
    Error = (Err)Engine_DiskLoad(DiskOperationP);
  }
  if (Error != errNone)
  {
    EjectDisk(drive);
  }

#ifdef DEBUG_CONVERT_IPF2DSK
  if (Filename.LowerCase().Pos(DISKIMAGE_IPF_EXT))
  {
    DiskOperationP->Drive->altered++;
  }
#endif /* DEBUG_CONVERT_IPF2DSK */

  if (Error != errNone)
  {
		String Message;

		// Prepare error
		switch (Error)
		{
			case vfsErrDSKNoData:
			{
				Message = _T("Missing data");
			}
			break;

			case vfsErrDSKTooManySides:
			{
				Message = _T("Too many sides");
			}
			break;

			case vfsErrDSKNoTrackInfo:
			{
				Message = _T("Missing TrackInfo signature");
			}
			break;

			case vfsErrDSKBadSignature:
			{
				Message = _T("Bad or missing DSK signature");
			}
			break;

			case errIPFBadStructure:
			case errIPFBadRecord:
			{
				Message = _T("Bad IPF/RAW Structure");
			}
			break;

			case errIPFBadPlatform:
			{
				Message = _T("Not an Amstrad CPC IPF image");
			}
			break;

			default:
			{
				Message = _T("Error=") + String::IntToHex(Error,4);
			}
		}

		// Show error
		Application->MessageBox(Message.c_str(), _T("Error loading Disk"), MB_OK);
	}

	return Error;
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::SaveDisk(AnsiString Filename,
                                   TDiskDrive drive)
{
  tDrive* DriveP = (drive == DriveA) ? mNativeCPC->FDC->DriveA : mNativeCPC->FDC->DriveB;

  // Si le disque n'a pas été modifié, ne rien faire
  if (DriveP->altered == 0)
    return errNone;

  if (mNeverUpdateDiskImages == true)
    return errNone;

  if (mConfirmDiskImageUpdates == true)
  {
    if (FileExists(Filename) == true)
    {
      if (Application->MessageBox(_T("Do you want to save disk image updates ?"),
                                  _T("Updated disk image"),
                                  MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2) == IDNO)
      {
        // User do not confirm modification saving.
        return errNone;
      }
    }
  }

  // IPF should not be written
  if (Filename.LowerCase().Pos(DISKIMAGE_IPF_EXT))
  {
    // Change extension to DSK.
    Filename = ChangeFileExt(Filename, DISKIMAGE_DSK_EXT);
  }

  if (FileExists(Filename) == true)
  {
    // Vérifier l'attribut "lecture seule"
    int Attributes = FileGetAttr(Filename);
    if (Attributes & faReadOnly)
    {
      return errFilePermissionDenied;
    }
  }

  // Prepare disk operation
  tDiskOperation* DiskOperationP = (tDiskOperation*)(NativeCPC->contextP + CONTEXT_OFFSET_DISKOPERATION);
  DiskOperationP->NativeCPC = mNativeCPC;
  DiskOperationP->Drive = DriveP;
  
  // Create new drive memory
  if (DriveP->new_data_size)
  {
    Engine_DiskOptimize(DiskOperationP);

    DriveP->new_dataP = (unsigned char*)malloc(DriveP->new_data_size);
    if (DriveP->new_dataP == NULL)
    {
      return errBadMemoryAllocation;
    }
  }

  Err Result = (Err)Engine_DiskSave(DiskOperationP);
  if (Result != errNone)
  {
    return errFileBadWriteImage;
  }

  if (drive == DriveA)
  {
    mDiskAFilename = Filename;
  }
  else
  {
    mDiskBFilename = Filename;
  }

  if (FileExists(Filename) == true)
  {
    DeleteFile(Filename);
  }

  // Création du nouveau fichier
  int iHandle = FileCreate(Filename);

  // Save entire disk file
  FileWrite(iHandle,
            DriveP->dataP,
            DriveP->data_size);

  FileClose(iHandle);

  return errNone;
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::CreateDisk(AnsiString Filename,
                                     TDiskDrive drive,
                                     tUChar FormatType)
{
  tDrive* DriveP = (drive == DriveA) ? mNativeCPC->FDC->DriveA : mNativeCPC->FDC->DriveB;

  // Prepare disk operation
  tDiskOperation* DiskOperationP = (tDiskOperation*)(NativeCPC->contextP + CONTEXT_OFFSET_DISKOPERATION);
  DiskOperationP->NativeCPC = mNativeCPC;
  DiskOperationP->Drive = DriveP;
  DiskOperationP->FormatType = FormatType;
  DiskOperationP->DiskFormatTableP = disk_format;

  // Signal drive content change
  DriveP->signal_updated++;
  DriveP->signal_content_change++;

  Err Result = (Err)Engine_DiskFormat(DiskOperationP);
#ifdef _TESTU
  if (Result >= appErrorClass)
  {
  }
#endif
  if (Result != errNone)
  {
    return errFileBadWriteImage;
  }

  // Save, eject and load New disk
  if (drive == DriveA)
  {
    mDiskAFilename = Filename;
  }
  else
  {
    mDiskBFilename = Filename;
	}

	if (Filename.IsEmpty() == false)
	{
		SaveAndEjectDisk(drive);

		LoadDisk(Filename,
						 drive);
	}

  return errNone;
}
//---------------------------------------------------------------------------

Err __fastcall TEmulator::CreateVirtualDisk(AnsiString Filename,
                                            TDiskDrive drive,
                                            tUChar FormatType)
{
  tDrive* DriveP = (drive == DriveA) ? mNativeCPC->FDC->DriveA : mNativeCPC->FDC->DriveB;

  // Prepare disk operation
  tDiskOperation* DiskOperationP = (tDiskOperation*)(NativeCPC->contextP + CONTEXT_OFFSET_DISKOPERATION);
  DiskOperationP->NativeCPC = mNativeCPC;
  DiskOperationP->Drive = DriveP;
  DiskOperationP->FormatType = FormatType;
  DiskOperationP->DiskFormatTableP = disk_format;

  Err Result = (Err)Engine_DiskFormat(DiskOperationP);
#ifdef _TESTU
  if (Result >= appErrorClass)
  {
  }
#endif
  if (Result != errNone)
  {
    return errFileBadWriteImage;
  }

  // Save virtual disk
  Result = (Err)Engine_DiskSave(DiskOperationP);
  if (Result != errNone)
  {
    return errFileBadWriteImage;
  }

  // Keep virtual disk
  DiskOperationP->DiskContentP = DriveP->dataP;
  DiskOperationP->disk_size = DriveP->data_size;
  DriveP->dataP = cNull;

  //
  // Load disk image
  //
  Engine_DiskEject(DiskOperationP);
  Result = (Err)Engine_DiskLoad(DiskOperationP);
  if (Result != errNone)
  {
    EjectDisk(drive);
  }
  else
  {
    if (drive == DriveA)
    {
      mDiskAFilename = Filename;
    }
    else
    {
      mDiskBFilename = Filename;
    }

    // Signal drive content have to be saved.
    DriveP->altered++;

    // Signal drive content change
    DriveP->signal_updated++;
    DriveP->signal_content_change++;
  }

  return errNone;
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::SaveAndEjectDisk(TDiskDrive drive)
{
  // Save disk if possible before eject
  AnsiString Filename = (drive == DriveA) ? mDiskAFilename : mDiskBFilename;
  if (Filename.IsEmpty() == false)
  {
    SaveDisk(Filename, drive);
  }

  return (EjectDisk(drive));
}
//---------------------------------------------------------------------------


Err __fastcall TEmulator::UpdateDisk(TDiskDrive drive)
{
Err Error = errFileDoesNotExist;

  // Save disk if possible before eject
  AnsiString Filename = (drive == DriveA) ? mDiskAFilename : mDiskBFilename;
  if (Filename.IsEmpty() == false)
  {
    Error = SaveDisk(Filename, drive);
  }

  return (Error);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TEmulator::DetectAutoStart(TDiskDrive drive)
{
  AnsiString AutoStartCommand = "";
  char* command = new char[20];
  memset(command, 0, 20);

  tDiskOperation* DiskOperationP = (tDiskOperation*)(mNativeCPC->contextP + CONTEXT_OFFSET_DISKOPERATION);
  memset(DiskOperationP, 0, sizeof(tDiskOperation));
  DiskOperationP->NativeCPC = mNativeCPC;
  DiskOperationP->Drive = (drive == DriveA) ? mNativeCPC->FDC->DriveA : mNativeCPC->FDC->DriveB;
  DiskOperationP->Param = (tVoid*)command;

  tAutoStartReturnCode ReturnCode = (tAutoStartReturnCode)Engine_AutoStart(DiskOperationP);

  if (ReturnCode == AutoStartOK)
  {
    AutoStartCommand = command;
  }

  delete [] command;

  return AutoStartCommand;
}
//---------------------------------------------------------------------------


bool __fastcall TEmulator::GetDriveAWriteProtected(void)
{
bool Ret = false;

  if (mNativeCPC->FDC->DriveA != NULL)
  {
    Ret = mNativeCPC->FDC->DriveA->write_protected ? true : false;
  }

  return Ret;
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::SetDriveAWriteProtected(bool WriteProtected)
{
  if (mNativeCPC->FDC->DriveA != NULL)
  {
    mNativeCPC->FDC->DriveA->write_protected = WriteProtected;
  }
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::GetDriveBWriteProtected(void)
{
bool Ret = false;

  if (mNativeCPC->FDC->DriveB != NULL)
  {
    Ret = mNativeCPC->FDC->DriveB->write_protected ? true : false;
  }

  return Ret;
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::SetDriveBWriteProtected(bool WriteProtected)
{
  if (mNativeCPC->FDC->DriveB != NULL)
  {
    mNativeCPC->FDC->DriveB->write_protected = WriteProtected;
  }
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::RUNFilename(AnsiString Filename)
{
  AnsiString Command = "RUN\"" + Filename + "\n";

  mAutoTypeBufferP = new char[Command.Length()+1];

  // Copy command
  char* CommandStr = Command.c_str();
  for (int loop=0; loop < Command.Length(); loop++)
  {
    mAutoTypeBufferP[loop] = CommandStr[loop];
  }
  mAutoTypeBufferP[Command.Length()] = '\0';
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::LOADFilename(AnsiString Filename)
{
  AnsiString Command = "LOAD\"" + Filename + "\n";

  mAutoTypeBufferP = new char[Command.Length()+1];

  // Copy command
  char* CommandStr = Command.c_str();
  for (int loop=0; loop < Command.Length(); loop++)
  {
    mAutoTypeBufferP[loop] = CommandStr[loop];
  }
  mAutoTypeBufferP[Command.Length()] = '\0';
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::ExecuteCPM(void)
{
  AnsiString Command = "|CPM\n";

  mAutoTypeBufferP = new char[Command.Length()+1];

  // Copy command
  char* CommandStr = Command.c_str();
  for (int loop=0; loop < Command.Length(); loop++)
  {
    mAutoTypeBufferP[loop] = CommandStr[loop];
  }
  mAutoTypeBufferP[Command.Length()] = '\0';
}
//---------------------------------------------------------------------------


int __fastcall TEmulator::ReadDiskCatalog(TDiskDrive drive)
{
  tDiskOperation* DiskOperationP = (tDiskOperation*)(mNativeCPC->contextP + CONTEXT_OFFSET_DISKOPERATION);
  memset(DiskOperationP, 0, sizeof(tDiskOperation));
  DiskOperationP->NativeCPC = mNativeCPC;
  DiskOperationP->Drive = (drive == DriveA) ? mNativeCPC->FDC->DriveA : mNativeCPC->FDC->DriveB;

  // Get Catalog
  int NbEntries = Engine_GetCatalog(DiskOperationP);

  // Add catalog entries
  mDiskCatalogEntries->Clear();
  for (int Loop=0; Loop < NbEntries; Loop++)
  {
    AnsiString Entry = (char*)DiskOperationP->CatalogEntries[Loop];
    AnsiString Filename = Entry.SubString(1, 8).Trim();
    if (Filename.Length() > 0)
    {
      AnsiString Extension = Entry.SubString(9, 3).Trim();
      if (Extension.Length() > 0)
      {
        mDiskCatalogEntries->Add(Filename + "." + Extension);
      }
      else
      {
        mDiskCatalogEntries->Add(Filename);
      }
    }
  }

  return NbEntries;
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::ForceDiskAFilename(String Filename)
{
  mDiskAFilename = Filename;
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::ForceDiskBFilename(String Filename)
{
  mDiskBFilename = Filename;
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::HandleAutoType(void)
{
  switch (mAutoTypeStatus)
  {
    case 0: // Start
    {
      mAutoTypeCurrentCharP = mAutoTypeBufferP;
      // Start at once
      mAutoTypeDuration = 1;
    }

    case 1: // Press SHIFT and CTRL is needed
    {
      if (!(--mAutoTypeDuration))
      {
        // key to press ?
        if (*mAutoTypeCurrentCharP)
        {
          // Press SHIFT and CTRL
          if (KeyboardGetAsciiSHIFTandCTRL(*mAutoTypeCurrentCharP))
          {
            // Prepare Press Duration
            mAutoTypeDuration = AUTOTYPEPRESS_CYCLES;
            KeyboardSetAsciiSHIFTandCTRL(*mAutoTypeCurrentCharP);
            mAutoTypeStatus = 2;
          }
          else
          {
            // Press Key at once
            mAutoTypeDuration = 1;
            mAutoTypeStatus = 2;
          }
        }
        else
        {
          // Stop autotyping
          delete [] mAutoTypeBufferP;
          mAutoTypeBufferP = NULL;
          mAutoTypeStatus = 0;
        }
      }
    }
    break;

    case 2: // Press key
    {
      if (!(--mAutoTypeDuration))
      {
        // Prepare Press Duration
        mAutoTypeDuration = AUTOTYPEPRESS_CYCLES;
        // Press key
        KeyboardSetAsciiKeyDown(*mAutoTypeCurrentCharP);
        mAutoTypeStatus = 3;
      }
    }
    break;


    case 3: // Release key
    {
      if (!(--mAutoTypeDuration))
      {
        // Release key
        KeyboardSetAsciiKeyUp(*(mAutoTypeCurrentCharP++));
        // Prepare Release Duration
        mAutoTypeDuration = AUTOTYPERELEASE_CYCLES;
        // Next Step
        mAutoTypeStatus = 1;
      }
    }
    break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::HandleAutoPlay(void)
{
  switch (mAutoPlayStatus)
  {
    case 0: // Press CTRL
    {
      PRESS_KEY(USERMANUAL_TO_CPC_KBD(23));
      mAutoPlayDuration = AUTOTYPEPRESS_CYCLES;
      mAutoPlayStatus = 1;
    }

    case 1: // Press ENTER
    {
      if (!(--mAutoPlayDuration))
      {
        PRESS_KEY(USERMANUAL_TO_CPC_KBD(6));
        mAutoPlayDuration = AUTOTYPEPRESS_CYCLES;
        mAutoPlayStatus = 2;
      }
    }
    break;

    case 2: // Release ENTER
    {
      if (!(--mAutoPlayDuration))
      {
        RELEASE_KEY(USERMANUAL_TO_CPC_KBD(6));
        mAutoPlayDuration = AUTOTYPEPRESS_CYCLES * 4;
        mAutoPlayStatus = 3;
      }
    }
    break;


    case 3: // Press ENTER
    {
      if (!(--mAutoPlayDuration))
      {
        PRESS_KEY(USERMANUAL_TO_CPC_KBD(6));
        mAutoPlayDuration = AUTOTYPEPRESS_CYCLES;
        mAutoPlayStatus = 4;
      }
    }
    break;


    case 4: // Release keys
    {
      if (!(--mAutoPlayDuration))
      {
        RELEASE_KEY(USERMANUAL_TO_CPC_KBD(6));
        RELEASE_KEY(USERMANUAL_TO_CPC_KBD(23));
        mAutoPlayRequested = false;
      }
    }
    break;
  }
}
//---------------------------------------------------------------------------


const void* __fastcall TEmulator::GetAudioEmulatorData(void)
{
#ifndef USE_DYNAMIC_AUDIO_BUFFERS
  return ((const void*)(mContextP + CONTEXT_OFFSET_SND_BUFFER));
#else /* !USE_DYNAMIC_AUDIO_BUFFERS */
  return ((const void*)mAudioSpeakerDataP);
#endif /* !USE_DYNAMIC_AUDIO_BUFFERS */
}
//---------------------------------------------------------------------------
int __fastcall TEmulator::GetAudioEmulatorDataSize(void)
{
#ifndef USE_DYNAMIC_AUDIO_BUFFERS
  return (SIZETAB_SND_BUFFER);
#else /* !USE_DYNAMIC_AUDIO_BUFFERS */
#ifndef USE_AUDIO_FILLED_BUFFER
  return (mAudioSpeakerSize);
#else /* !USE_AUDIO_FILLED_BUFFER */
  return (mNativeCPC->PSG->FilledBufferSize);
#endif /* !USE_AUDIO_FILLED_BUFFER */
#endif /* !USE_DYNAMIC_AUDIO_BUFFERS */
}
//---------------------------------------------------------------------------
const void* __fastcall TEmulator::GetAudioTMPIData(void)
{
#ifndef USE_DYNAMIC_AUDIO_BUFFERS
  return ((const void*)(mContextP + CONTEXT_OFFSET_TMPI_BUFFER));
#else /* !USE_DYNAMIC_AUDIO_BUFFERS */
  return ((const void*)mAudioTMPIDataP);
#endif /* !USE_DYNAMIC_AUDIO_BUFFERS */
}
//---------------------------------------------------------------------------
int __fastcall TEmulator::GetAudioTMPIDataSize(void)
{
#ifndef USE_DYNAMIC_AUDIO_BUFFERS
  return (SIZETAB_TMPI_BUFFER);
#else /* !USE_DYNAMIC_AUDIO_BUFFERS */
#ifndef USE_AUDIO_FILLED_BUFFER
  return (mAudioTMPISize);
#else /* !USE_AUDIO_FILLED_BUFFER */
  return (mNativeCPC->TMPISpeech->FilledBufferSize);
#endif /* !USE_AUDIO_FILLED_BUFFER */
#endif /* !USE_DYNAMIC_AUDIO_BUFFERS */
}
//---------------------------------------------------------------------------
const void* __fastcall TEmulator::GetAudioPlayCityData(void)
{
#ifndef USE_DYNAMIC_AUDIO_BUFFERS
  return ((const void*)(mContextP + CONTEXT_OFFSET_PLAYCITY_BUFFER));
#else /* !USE_DYNAMIC_AUDIO_BUFFERS */
  return ((const void*)mAudioPlayCityDataP);
#endif /* !USE_DYNAMIC_AUDIO_BUFFERS */
}
//---------------------------------------------------------------------------
int __fastcall TEmulator::GetAudioPlayCityDataSize(void)
{
#ifndef USE_DYNAMIC_AUDIO_BUFFERS
  return (SIZETAB_PLAYCITY_BUFFER);
#else /* !USE_DYNAMIC_AUDIO_BUFFERS */
#ifndef USE_AUDIO_FILLED_BUFFER
  return (mAudioPlayCitySize);
#else /* !USE_AUDIO_FILLED_BUFFER */
  return (mNativeCPC->PlayCity->FilledBufferSize);
#endif /* !USE_AUDIO_FILLED_BUFFER */
#endif /* !USE_DYNAMIC_AUDIO_BUFFERS */
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::AudioStart(void)
{
  // Enable Audio
  if (mNativeCPC != NULL)
  {
    mNativeCPC->PSG->snd_enabled = 1;

    if (mSettingsP->UsePlayCity == true)
    {
      mNativeCPC->PlayCity->snd_enabled = 1;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::AudioStop(void)
{
  // Disable Audio
  if (mNativeCPC != NULL)
  {
    mNativeCPC->PSG->snd_enabled = 0;
    mNativeCPC->PlayCity->snd_enabled = 0;
  }
}
//---------------------------------------------------------------------------
bool __fastcall TEmulator::IsAudioActive(void)
{
  return (mNativeCPC->PSG->snd_enabled ? true : false);
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::StartStepByStep(void)
{
  mNativeCPC->step_by_step = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::StopStepByStep(void)
{
  mNativeCPC->step_by_step = 0;
}
//---------------------------------------------------------------------------
bool __fastcall TEmulator::IsStepByStepActive(void)
{
  return (mNativeCPC->step_by_step ? true : false);
}
//---------------------------------------------------------------------------


//
// Key mapping
//
void __fastcall TEmulator::StartKeyboardAsJoystick(void)
{
  mUseKeyboardAsJoystick = true;

  // Save current mapping
  for (int loop=0; loop < NB_JOYSTICK_KEYS; loop++)
  {
    mVirtualKeyboardAsJoystickP[loop] = mASCIIKeyboardP[mJoystickKeyboardP[loop]];
    mHardwareKeyboardAsJoystickP[loop] = mHardwareKeyboardP[mJoystickKeyboardP[loop]];
  }

  // Set joystick 0 mapping
  mASCIIKeyboardP[mJoystickKeyboardP[KEY_JOY0_LEFT]]   = CPC_KEY_J0_LEFT;
  mASCIIKeyboardP[mJoystickKeyboardP[KEY_JOY0_UP]]     = CPC_KEY_J0_UP;
  mASCIIKeyboardP[mJoystickKeyboardP[KEY_JOY0_RIGHT]]  = CPC_KEY_J0_RIGHT;
  mASCIIKeyboardP[mJoystickKeyboardP[KEY_JOY0_DOWN]]   = CPC_KEY_J0_DOWN;
  mASCIIKeyboardP[mJoystickKeyboardP[KEY_JOY0_FIRE1]]  = CPC_KEY_J0_FIRE1;
  mASCIIKeyboardP[mJoystickKeyboardP[KEY_JOY0_FIRE2]]  = CPC_KEY_J0_FIRE2;
  mHardwareKeyboardP[mJoystickKeyboardP[KEY_JOY0_LEFT]]  = default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_LEFT];
  mHardwareKeyboardP[mJoystickKeyboardP[KEY_JOY0_UP]]    = default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_UP];
  mHardwareKeyboardP[mJoystickKeyboardP[KEY_JOY0_RIGHT]] = default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_RIGHT];
  mHardwareKeyboardP[mJoystickKeyboardP[KEY_JOY0_DOWN]]  = default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_DOWN];
  mHardwareKeyboardP[mJoystickKeyboardP[KEY_JOY0_FIRE1]] = default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_FIRE1];
  mHardwareKeyboardP[mJoystickKeyboardP[KEY_JOY0_FIRE2]] = default_hardware_virtual_to_cpc_kbdmatrix[VK_JOY0_FIRE2];
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::StopKeyboardAsJoystick(void)
{
  mUseKeyboardAsJoystick = false;

  // Restore current state
  for (int loop=0; loop < NB_JOYSTICK_KEYS; loop++)
  {
    mASCIIKeyboardP[mJoystickKeyboardP[loop]] = mVirtualKeyboardAsJoystickP[loop];
    mHardwareKeyboardP[mJoystickKeyboardP[loop]] = mHardwareKeyboardAsJoystickP[loop];
  }
}
//---------------------------------------------------------------------------


//
// Lightguns
//
void __fastcall TEmulator::PlugMagnumLightPhaser(void)
{
  NativeCPC->lightgun_random_crtc = 1;
  mLightgunPlugEvent = PlugMagnumLightPhaser;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::UnplugMagnumLightPhaser(void)
{
  NativeCPC->lightgun_random_crtc = 0;
  mLightgunPlugEvent = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnMagnumLightPhaserPressed(void)
{
  NativeCPC->lightgun_random_crtc = 0;
  NativeCPC->lightgun_trigger_time = NativeCPC->timer_from_reset;

  NativeCPC->signal_lightgun_trigger = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnMagnumLightPhaserReleased(void)
{
  NativeCPC->lightgun_random_crtc = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::PlugGunStick(void)
{
  mLightgunPlugEvent = PlugGunStick;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::UnplugGunStick(void)
{
  mLightgunPlugEvent = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnGunStickPressed(void)
{
  NativeCPC->lightgun_beam_detect = 1;
  NativeCPC->lightgun_beam_key_mask = ROWMASK_FROM_KEYCODE(GUNSTICK_BEAM_KEY);
  NativeCPC->lightgun_y_radius = GUNSTICK_Y_RADIUS;
  NativeCPC->lightgun_sensitivity = GUNSTICK_SENSITIVITY;
  NativeCPC->lightgun_trigger_duration = GUNSTICK_TRIG_DURATION;
  NativeCPC->lightgun_trigger_delay = NativeCPC->timer_from_reset + CLOCK_PER_MS(GUNSTICK_TRIG_DELAY);
  NativeCPC->lightgun_beam_duration = CLOCK_PER_US(GUNSTICK_BEAM_DURATION);
  NativeCPC->lightgun_trigger_time = NativeCPC->timer_from_reset;

#ifdef _DEBUG
  mLightgunTriggerEndEvent = OnGunStickTriggerEnd;
#endif /* _DEBUG */

  PRESS_KEY(GUNSTICK_TRIG_KEY);

  NativeCPC->signal_lightgun_trigger = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnGunStickTriggerEnd(void)
{
  NativeCPC->signal_lightgun_trigger_end = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnGunStickReleased(void)
{
  NativeCPC->lightgun_beam_detect = 0;

  RELEASE_KEY(GUNSTICK_TRIG_KEY);
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::PlugWestPhaser(void)
{
  NativeCPC->lightgun_beam_detect = 1;
  NativeCPC->lightgun_y_radius = WEST_PHASER_Y_RADIUS;
  NativeCPC->lightgun_sensitivity = WEST_PHASER_SENSITIVITY;
  NativeCPC->lightgun_beam_key_mask = ROWMASK_FROM_KEYCODE(WEST_PHASER_BEAM_KEY);
  NativeCPC->lightgun_trigger_delay = NativeCPC->timer_from_reset + CLOCK_PER_MS(WEST_PHASER_TRIG_DELAY);
  NativeCPC->lightgun_beam_duration = CLOCK_PER_US(WEST_PHASER_BEAM_DURATION);

  mLightgunPlugEvent = PlugWestPhaser;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::UnplugWestPhaser(void)
{
  NativeCPC->lightgun_beam_detect = 0;

  mLightgunPlugEvent = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnWestPhaserPressed(void)
{
  NativeCPC->lightgun_trigger_duration = NativeCPC->timer_from_reset + CLOCK_PER_MS(WEST_PHASER_TRIG_DURATION);

  mLightgunTriggerEndEvent = OnWestPhaserTriggerEnd;
  NativeCPC->lightgun_trigger_time = NativeCPC->timer_from_reset;

  PRESS_KEY(WEST_PHASER_TRIG_KEY);

  NativeCPC->signal_lightgun_trigger = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnWestPhaserTriggerEnd(void)
{
  RELEASE_KEY(WEST_PHASER_TRIG_KEY);

  NativeCPC->signal_lightgun_trigger_end = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnWestPhaserReleased(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::PlugAMXMouse(void)
{
  NativeCPC->amx_mouse_active = 1;
  ResetAMXMouse();
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::UnplugAMXMouse(void)
{
  NativeCPC->amx_mouse_active = 0;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::ResetAMXMouse(void)
{
  NativeCPC->amx_mouse_reset = 1;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::SetAMXMouseXFactor(float factor)
{
  mAMXMouseFactorX = factor * mSettingsP->AMXMouseFactorX / 100;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::SetAMXMouseYFactor(float factor)
{
  mAMXMouseFactorY = factor * mSettingsP->AMXMouseFactorY / 100;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnAMXMouseButtonLPressed(void)
{
  PRESS_KEY(AMX_MOUSE_BUTTON_L);
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnAMXMouseButtonLReleased(void)
{
  RELEASE_KEY(AMX_MOUSE_BUTTON_L);
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnAMXMouseButtonMPressed(void)
{
  PRESS_KEY(AMX_MOUSE_BUTTON_C);
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnAMXMouseButtonMReleased(void)
{
  RELEASE_KEY(AMX_MOUSE_BUTTON_C);
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnAMXMouseButtonRPressed(void)
{
  PRESS_KEY(AMX_MOUSE_BUTTON_R);
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::OnAMXMouseButtonRReleased(void)
{
  RELEASE_KEY(AMX_MOUSE_BUTTON_R);
}
//---------------------------------------------------------------------------


//
// Lightpen / Magnum Light Phaser
//
void __fastcall TEmulator::OnLightpenMove(int X, int Y)
{
tCRTC* CRTC;
tShort offscreenX;
tShort offscreenY;
tUShort addr_offset;
tUChar crtc_R0;
tUChar crtc_R1;
tUChar crtc_R2;
tUChar crtc_R4;
tUChar crtc_R7;
tUChar crtc_R9;
tUChar crtc_hsw;
tUShort originX;
tUShort originY;
tChar posY;

  offscreenX = (tShort)X;
  offscreenY = (tShort)Y;

  CRTC = mNativeCPC->CRTC;

  // Get registers
  crtc_R0  = CRTC->registers[0]; // Horizontal total character number - 1
  crtc_R1  = CRTC->registers[1]; // Horizontal displayed
  crtc_R2  = CRTC->registers[2]; // Horizontal Sync Pulse position
  crtc_R4  = CRTC->registers[4]; // Vertical total line character number - 1
  crtc_R7  = CRTC->registers[7]; // Vertical Sync position
  crtc_R9  = CRTC->registers[9]; // Maximum raster
  crtc_hsw = CRTC->hsw;          // Horizontal sync width

  // Compute display origins
  originX = (crtc_R0+1) - (crtc_R2 + crtc_hsw) - 4 /* ?? */;
  originY = (crtc_R4+1) - (crtc_R7 + 3 /* VSync = 3*/);

  // Compute LightPen address
  posY = (offscreenY / (crtc_R9+1)) - (originY - 1);
  addr_offset = ((tUChar)(posY >= 0 ? posY : (crtc_R4+1) - posY) * crtc_R1) +
                (offscreenX / 8) - originX;

  // Set LightPen offset 
  CRTC->lpen_offset = addr_offset;
}
//---------------------------------------------------------------------------


//
// GunStick / West Phaser
//
void __fastcall TEmulator::OnPhaserMove(int X, int Y)
{
tCRTC* CRTC;
tShort offscreenX;
tShort offscreenY;
tUChar crtc_R0;
tUChar crtc_R2;
#ifndef CRTC_ENGINE_V2
tUChar crtc_R4;
tUChar crtc_R7;
tUChar crtc_R9;
tUShort originY;
#endif /* CRTC_ENGINE_V2 */
tUChar crtc_hsw;
tUShort originX;

  offscreenX = (tShort)X;
  offscreenY = (tShort)Y;

  CRTC = mNativeCPC->CRTC;

  // Get registers
  crtc_R0  = CRTC->registers[0]; // Horizontal total character number - 1
  crtc_R2  = CRTC->registers[2]; // Horizontal Sync Pulse position
#ifndef CRTC_ENGINE_V2
  crtc_R4  = CRTC->registers[4]; // Vertical total line character number - 1
  crtc_R7  = CRTC->registers[7]; // Vertical Sync position
  crtc_R9  = CRTC->registers[9]; // Maximum raster
#endif /* CRTC_ENGINE_V2 */
  crtc_hsw = CRTC->hsw;          // Horizontal sync width

  // Compute display origins
  originX = (crtc_R0+1) - (crtc_R2 + crtc_hsw);
#ifndef CRTC_ENGINE_V2
  originY = (crtc_R4+1) - (crtc_R7 + 3 /* VSync = 3*/);
#endif /* CRTC_ENGINE_V2 */

  // Set Phaser coordinates
  NativeCPC->lightgun_x_pos = (offscreenX / 8) - originX;
#ifndef CRTC_ENGINE_V2
  NativeCPC->lightgun_y_pos = (offscreenY / (crtc_R9+1)) - (originY - 1);
#else /* CRTC_ENGINE_V2 */
  NativeCPC->lightgun_y_pos = offscreenY;
#endif /* CRTC_ENGINE_V2 */
}
//---------------------------------------------------------------------------


//
// AMX Mouse
//
void __fastcall TEmulator::OnMouseMove(int X, int Y)
{
  if (!NativeCPC->amx_mouse_reset)
  {
    tLong amx_mouse_X_move = (tLong)X - (tLong)NativeCPC->amx_mouse_last_pos_X;
    tLong amx_mouse_Y_move = (tLong)Y - (tLong)NativeCPC->amx_mouse_last_pos_Y;

    // Update number of pulses
    NativeCPC->amx_mouse_X_pulses += (tLong)((float)amx_mouse_X_move * mAMXMouseFactorX);
    NativeCPC->amx_mouse_Y_pulses += (tLong)((float)amx_mouse_Y_move * mAMXMouseFactorY);
  }
  else
  {
    NativeCPC->amx_mouse_reset = 0U;
  }

  // Keep last mouse position
  NativeCPC->amx_mouse_last_pos_X = (tLong)X;
  NativeCPC->amx_mouse_last_pos_Y = (tLong)Y;
}

//
// Snapshot
//
bool __fastcall TEmulator::SaveSnapshot(String Filename)
{
t_SNA_header sh;
bool SaveOK = false;

  memset(&sh, 0, sizeof(sh));
  strcpy(sh.id, "MV - SNA");
  sh.version = 3;

  // Z80
  tZ80* Z80 = mNativeCPC->Z80;
  sh.AF[1] = _A;
  sh.AF[0] = _F;
  sh.BC[1] = _B;
  sh.BC[0] = _C;
  sh.DE[1] = _D;
  sh.DE[0] = _E;
  sh.HL[1] = _H;
  sh.HL[0] = _L;
  sh.R = (_R & 0x7f) | (_Rb7 & 0x80);
  sh.I = _I;
  if (_IFF1)
    sh.IFF0 = 1;
  if (_IFF2)
    sh.IFF1 = 1;
  sh.IX[1] = _IXh;
  sh.IX[0] = _IXl;
  sh.IY[1] = _IYh;
  sh.IY[0] = _IYl;
  sh.SP[1] = Z80->Regs.SP.b.h;
  sh.SP[0] = Z80->Regs.SP.b.l;
  sh.PC[1] = Z80->Regs.PC.b.h;
  sh.PC[0] = Z80->Regs.PC.b.l;
  sh.IM = _IM;
  sh.AFx[1] = Z80->Regs.AFx.b.h;
  sh.AFx[0] = Z80->Regs.AFx.b.l;
  sh.BCx[1] = Z80->Regs.BCx.b.h;
  sh.BCx[0] = Z80->Regs.BCx.b.l;
  sh.DEx[1] = Z80->Regs.DEx.b.h;
  sh.DEx[0] = Z80->Regs.DEx.b.l;
  sh.HLx[1] = Z80->Regs.HLx.b.h;
  sh.HLx[0] = Z80->Regs.HLx.b.l;

  // Gate Array
  tGateArray* GateArray = mNativeCPC->GateArray;
  sh.ga_pen = GateArray->pen;
  for (int n = 0; n < 17; n++) // loop for all colours + border
  {
    sh.ga_ink_values[n] = GateArray->ink_values[n];
  }
  sh.ga_ROM_config = GateArray->ROM_config;
  sh.ga_RAM_config = GateArray->RAM_config;

  // CRTC
  tCRTC* CRTC = mNativeCPC->CRTC;
  sh.crtc_reg_select = CRTC->reg_select;
  for (int n = 0; n < 18; n++) // loop for all CRTC registers
  {
    sh.crtc_registers[n] = CRTC->registers[n];
  }

  // ROM select
  sh.upper_ROM = GateArray->upper_ROM;

  // PPI
  tPPI* PPI = mNativeCPC->PPI;
  sh.ppi_A = PPI->portA;
  sh.ppi_B = PPI->portB;
  sh.ppi_C = PPI->portC;
  sh.ppi_control = (tUChar)PPI->control;

  // PSG
  tPSG* PSG = mNativeCPC->PSG;
  sh.psg_reg_select = PSG->reg_select;
  for (int n = 0; n < 16; n++) // loop for all PSG registers
  {
    sh.psg_registers[n] = PSG->RegisterAY.Index[n];
  }

  sh.ram_size[0] = mNativeCPC->ram_size & 0xff;
  sh.ram_size[1] = (mNativeCPC->ram_size >> 8) & 0xff;

  // version 2 info
  if ( (mPrefP->CPCModel == CPC_MODEL_464_UK)
       || (mPrefP->CPCModel == CPC_MODEL_464_FR)
       || (mPrefP->CPCModel == CPC_MODEL_464_SP) )
  {
    sh.cpc_model = 0; // CPC 464
  }
  else if ( (mPrefP->CPCModel == CPC_MODEL_664_UK)
            || (mPrefP->CPCModel == CPC_MODEL_664_FR) )
  {
    sh.cpc_model = 1; // CPC 664
  }
  else
  {
    sh.cpc_model = 2; // CPC 6128
  }

  // version 3 info
  sh.fdc_motor = mNativeCPC->FDC->motor;
  sh.drvA_current_track = mNativeCPC->FDC->DriveA->current_track;
  sh.drvB_current_track = mNativeCPC->FDC->DriveB->current_track;
  sh.printer_data = mNativeCPC->printer_port ^ 0x80; // invert bit 7 again
  sh.psg_env_step = PSG->AmplitudeEnv >> 1; // divide by 2 to bring it into the 0 - 15 range
  if (PSG->FirstPeriod)
  {
    switch (PSG->RegisterAY.UChar.EnvType)
    {
      case 0:
      case 1:
      case 2:
      case 3:
      case 8:
      case 9:
      case 10:
      case 11:
      {
        sh.psg_env_direction = 0xff; // down
      }
      break;

      case 4:
      case 5:
      case 6:
      case 7:
      case 12:
      case 13:
      case 14:
      case 15:
      {
        sh.psg_env_direction = 0x01; // up
      }
      break;
    }
  }
  else
  {
    switch (PSG->RegisterAY.UChar.EnvType)
    {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 9:
      case 11:
      case 13:
      case 15:
      {
        sh.psg_env_direction = 0x00; // hold
      }
      break;

      case 8:
      case 14:
      {
        sh.psg_env_direction = 0xff; // down
      }
      break;

      case 10:
      case 12:
      {
        sh.psg_env_direction = 0x01; // up
      }
      break;
    }
  }
  sh.crtc_addr[0] = CRTC->addr & 0xff;
  sh.crtc_addr[1] = (CRTC->addr >> 8) & 0xff;
  sh.crtc_scanline[0] = mNativeCPC->VDU->scanline & 0xff;
  sh.crtc_scanline[1] = (mNativeCPC->VDU->scanline >> 8) & 0xff;
  sh.crtc_char_count[0] = CRTC->char_count;
  sh.crtc_line_count = CRTC->line_count;
  sh.crtc_raster_count = CRTC->raster_count;
  sh.crtc_hsw_count = CRTC->hsw_count;
  sh.crtc_vsw_count = CRTC->vsw_count;
  long dwFlags = 0;
  if (CRTC->flag_invsync) // vsync active?
  {
    dwFlags |= 1;
  }
  if (CRTC->flags1.inHSYNC) // hsync active?
  {
    dwFlags |= 2;
  }
  if (CRTC->flag_invta) // in vertical total adjust?
  {
    dwFlags |= 0x80;
  }
  sh.crtc_flags[0] = dwFlags & 0xff;
  sh.crtc_flags[1] = (dwFlags >> 8) & 0xff;
  sh.ga_int_delay = GateArray->hs_count;
  sh.ga_sl_count = (tUChar)GateArray->sl_count;
  sh.z80_int_pending = Z80->Regs.int_pending;

  int FileHandle = FileCreate(Filename);
  if (FileHandle > 0)
  {
    // Write snapshot header
    FileWrite(FileHandle, &sh, sizeof(t_SNA_header));
    // Write RAM content
    FileWrite(FileHandle, mNativeCPC->pbRAM, (mNativeCPC->ram_size * 1024));
    FileClose(FileHandle);

    mSnapshotFilename = Filename;
    SaveOK = true;
  }

  return (SaveOK);
}
//---------------------------------------------------------------------------
bool __fastcall TEmulator::UpdateSnapshot(void)
{
bool SaveOK = false;

  if (mSnapshotFilename.IsEmpty() == false)
  {
    SaveOK = SaveSnapshot(mSnapshotFilename);
  }

  return (SaveOK);
}
//---------------------------------------------------------------------------


bool __fastcall TEmulator::LoadSnapshot(String Filename)
{
t_SNA_header sh;
int Error = ERR_SNA_OK;
tRegister port;
tUChar val;

  memset(&sh, 0, sizeof(sh));

  int FileHandle = FileOpen(Filename, fmOpenRead);
  if (FileHandle > 0)
  {
    do
    {
      // Read Snapshot header
      if (FileRead(FileHandle, &sh, sizeof(t_SNA_header)) != (int)sizeof(t_SNA_header))
      {
        Error = ERR_SNA_INVALID;
        continue;
      }

      // Snapshot invalid ?
      if (memcmp(sh.id, "MV - SNA", 8) != 0)
      {
        Error = ERR_SNA_INVALID;
        continue;
      }

      tULong dwSnapSize = sh.ram_size[0] + (sh.ram_size[1] * 256); // memory dump size
      dwSnapSize &= ~0x3f; // limit to multiples of 64
      if (!dwSnapSize || (dwSnapSize > MAX_RAM_SIZE))
      {
        Error = ERR_SNA_INVALID;
        continue;
      }

      if (dwSnapSize != mNativeCPC->ram_size) // memory dump size differs from current RAM size?
      {
        // Init memory extension settings
        mSettingsP->Use64kMemoryExtension = false;
        mSettingsP->Use256kMemoryExtension = false;
        mSettingsP->Use256kSiliconDisc = false;
        mPrefP->Use64kMemoryExtension = false;
        mPrefP->Use256kMemoryExtension = false;
        mPrefP->Use256kSiliconDisc = false;

        // Change settings to adapt memory size
        if (dwSnapSize == 576)
        {
          mSettingsP->Use256kMemoryExtension = true;
          mSettingsP->Use256kSiliconDisc = true;
          mPrefP->Use256kMemoryExtension = true;
          mPrefP->Use256kSiliconDisc = true;
        }
        else if (dwSnapSize == 320)
        {
          mSettingsP->Use256kMemoryExtension = true;
          mPrefP->Use256kMemoryExtension = true;
        }
        else if (dwSnapSize == 128)
        {
          mSettingsP->Use64kMemoryExtension = true;
          mPrefP->Use64kMemoryExtension = true;
        }
      }

      // Check CPC model
      if (sh.version > 1)
      {
        // Get current CPC model
        tLong Model;
        if ( (mPrefP->CPCModel == CPC_MODEL_464_UK)
             || (mPrefP->CPCModel == CPC_MODEL_464_FR)
             || (mPrefP->CPCModel == CPC_MODEL_464_SP) )
        {
          Model = 0; // CPC 464
        }
        else if ( (mPrefP->CPCModel == CPC_MODEL_664_UK)
                  || (mPrefP->CPCModel == CPC_MODEL_664_FR) )
        {
          Model = 1; // CPC 664
        }
        else
        {
          Model = 2; // CPC 6128
        }

        // Different CPC model
        if (sh.cpc_model != Model)
        {
          // Change settings
          if (sh.cpc_model == 0)
          {
            mPrefP->CPCModel = CPC_MODEL_464_UK;
            mSettingsP->CPCDevice = CPC_MODEL_464_UK;
          }
          else if (sh.cpc_model == 1)
          {
            mPrefP->CPCModel = CPC_MODEL_664_UK;
            mSettingsP->CPCDevice = CPC_MODEL_664_UK;
          }
          else
          {
            mPrefP->CPCModel = CPC_MODEL_6128_UK;
            mSettingsP->CPCDevice = CPC_MODEL_6128_UK;
          }
        }
      }

      // Emulator restart
      if (Restart(mSettingsP) != errNone)
      {
        Error = ERR_SNA_EMU_NOT_READY;
        continue;
      }

      // Perform engine initialisation before applying snapshot
      Execute();

      // read memory dump into CPC RAM
      tLong RAMSize = dwSnapSize * 1024;
      if (FileRead(FileHandle, mNativeCPC->pbRAM, RAMSize) != RAMSize)
      {
        continue;
      }

      // Check direct accesses
      if ( (mNativeCPC->Z80OUTHandlerPtr == NULL)
           || (mNativeCPC->SetAYRegisterPtr == NULL) )
      {
        Error = ERR_SNA_EMU_NOT_READY;
        continue;
      }

      // Apply snapshot
      if (ApplySnapshot(&sh) == false)
      {
        Error = ERR_SNA_INVALID;
        continue;
      }
    }
    while (0);

    FileClose(FileHandle);
  }

  bool LoadOK = true;
  if (Error != ERR_SNA_OK)
  {
    // Signal error
		String ErrorMessage;
		if (Error == ERR_SNA_INVALID)
		{
			ErrorMessage = _T("Invalid SNA file");
		}
		else
		{
			ErrorMessage = _T("Emulator not ready");
		}

		Application->MessageBox(ErrorMessage.c_str(), _T("Load snapshot"), MB_OK);

    LoadOK = false;
    // Emulator reset
    Reset();
  }
  else
  {
    mSnapshotFilename = Filename;
  }

  return (LoadOK);
}
//---------------------------------------------------------------------------


bool __fastcall TEmulator::PlaySNRSession(String Filename)
{
int Error = ERR_SNA_OK;

  // Ouverture du fichier en lecture
  int iHandle = FileOpen(Filename, fmOpenRead);
  if (iHandle < 0)
    return false;
  // Met le curseur à la fin du fichier pour récupérer la taille du fichier
  int SNRFileSize = FileSeek(iHandle, 0, 2);
  // Remet le curseur en début de fichier pour la prochaine lecture
  FileSeek(iHandle, 0, 0);
  // Allocate and initialise memory
  delete [] mSNRFileP;
  mSNRFileP = (tUChar*) new tUChar[SNRFileSize+1]; // Add trailing 0 for end of chunk detection
  memset(mSNRFileP, 0, SNRFileSize+1);
  // Read file
  FileRead(iHandle,
           mSNRFileP,
           SNRFileSize);
  // Fermeture du fichier
  FileClose(iHandle);

  // Extract path
  AnsiString FilePath = ExtractFilePath(Filename);
  tUChar Version = 0;

  tUChar* DataP = mSNRFileP;
  while (*DataP)
  {
    //
    // Snapshot data
    //
    if (DataP == mSNRFileP)
    {
      t_SNA_header* shP = (t_SNA_header*)DataP;

      // Snapshot invalid ?
      if (memcmp(shP->id, "RW - SNR", 8) != 0)
      {
        Error = ERR_SNA_INVALID;
        break;
      }

      // No Version 3 snapshot
      if (shP->version < 3)
      {
        Error = ERR_SNA_INVALID;
        break;
      }

      //
      // Get current CPC model
      //
      tLong Model;
      if ( (mPrefP->CPCModel == CPC_MODEL_464_UK)
           || (mPrefP->CPCModel == CPC_MODEL_464_FR)
           || (mPrefP->CPCModel == CPC_MODEL_464_SP) )
      {
        Model = 0; // CPC 464
      }
      else if ( (mPrefP->CPCModel == CPC_MODEL_664_UK)
                || (mPrefP->CPCModel == CPC_MODEL_664_FR) )
      {
        Model = 1; // CPC 664
      }
      else
      {
        Model = 2; // CPC 6128
      }

      // Different CPC model
      if (shP->cpc_model != Model)
      {
        // Change settings
        if (shP->cpc_model == 0)
        {
          mPrefP->CPCModel = CPC_MODEL_464_UK;
          mSettingsP->CPCDevice = CPC_MODEL_464_UK;
        }
        else if (shP->cpc_model == 1)
        {
          mPrefP->CPCModel = CPC_MODEL_664_UK;
          mSettingsP->CPCDevice = CPC_MODEL_664_UK;
        }
        else
        {
          mPrefP->CPCModel = CPC_MODEL_6128_UK;
          mSettingsP->CPCDevice = CPC_MODEL_6128_UK;
        }
      }

      // Add peripherals
      mSettingsP->UseFloppyDrive = true;
      if (mSettingsP->UpperROMFilename[7] == EmptyRomName)
      {
        mSettingsP->UpperROMFilename[7] = DefaultAMSDOSRomName;
      }

      // Emulator restart
      if (Restart(mSettingsP) != errNone)
      {
        Error = ERR_SNA_EMU_NOT_READY;
        break;
      }

      // Perform engine initialisation before applying snapshot
      Execute();

      // read memory dump into CPC RAM
      DataP += sizeof(t_SNA_header);
      tULong dwSnapSize = shP->ram_size[0] + (shP->ram_size[1] * 256); // memory dump size
      dwSnapSize &= ~0x3f; // limit to multiples of 64
      tULong RAMSize = dwSnapSize * 1024;
      if (RAMSize)
      {
        memcpy(mNativeCPC->pbRAM, DataP, RAMSize);
        DataP += RAMSize;
      }
    }
    //
    // Memory chunk
    //
    else if (!strncmp((char*)DataP, "MEM0", 4))
    {
      tULong Size =   ((*(DataP+0x04)) << 0)
                    + ((*(DataP+0x05)) << 8)
                    + ((*(DataP+0x06)) << 16)
                    + ((*(DataP+0x07)) << 24);
      // Copy memory
      DecodeRLE(mNativeCPC->pbRAM, DataP+8, Size, 0, 0xE5);
      // Next chunk
      DataP += Size + 8; // Add chunk header
    }
    //
    // ROMS chunk
    //
    else if (!strncmp((char*)DataP, "ROMS", 4))
    {
      tULong Size =   ((*(DataP+0x04)) << 0)
                    + ((*(DataP+0x05)) << 8)
                    + ((*(DataP+0x06)) << 16)
                    + ((*(DataP+0x07)) << 24);
      // Next chunk
      DataP += Size + 8; // Add chunk header
    }
    //
    // DISK A chunk
    //
    else if (!strncmp((char*)DataP, "DSCA", 4))
    {
      tULong Size =   ((*(DataP+0x04)) << 0)
                    + ((*(DataP+0x05)) << 8)
                    + ((*(DataP+0x06)) << 16)
                    + ((*(DataP+0x07)) << 24);
      // Get Filename
      if (Size)
      {
        AnsiString DiskFilename = (char*)(DataP + 8);
        DiskFilename = DiskFilename.SubString(1, Size);
        DiskFilename = FilePath + DiskFilename;
        if (FileExists(DiskFilename) == true)
        {
          LoadDisk(DiskFilename, DriveA);
        }
        else
        {
          Error = ERR_SNA_INVALID;
          break;
        }
      }
      // Next chunk
      DataP += Size + 8; // Add chunk header
    }
    //
    // DISK B chunk
    //
    else if (!strncmp((char*)DataP, "DSCB", 4))
    {
      tULong Size =   ((*(DataP+0x04)) << 0)
                    + ((*(DataP+0x05)) << 8)
                    + ((*(DataP+0x06)) << 16)
                    + ((*(DataP+0x07)) << 24);
      // Get Filename
      if (Size)
      {
        AnsiString DiskFilename = (char*)(DataP + 8);
        DiskFilename = DiskFilename.SubString(1, Size);
        DiskFilename = FilePath + DiskFilename;
        if (FileExists(DiskFilename) == true)
        {
          LoadDisk(DiskFilename, DriveB);
        }
        else
        {
          Error = ERR_SNA_INVALID;
          break;
        }
      }
      // Next chunk
      DataP += Size + 8; // Add chunk header
    }
    //
    // Version chunk
    //
    else if (!strncmp((char*)DataP, "SNRV", 4))
    {
      tULong Size =   ((*(DataP+0x04)) << 0)
                    + ((*(DataP+0x05)) << 8)
                    + ((*(DataP+0x06)) << 16)
                    + ((*(DataP+0x07)) << 24);
      // Keep version
      Version = *(DataP + 0x08);
      // Next chunk
      DataP += Size + 8; // Add chunk header
    }
    //
    // SNR Data chunk
    //
    else if (!strncmp((char*)DataP, "SNR ", 4))
    {
      // Apply snapshot
      t_SNA_header* shP = (t_SNA_header*)mSNRFileP;
      if (ApplySnapshot(shP) == false)
      {
        Error = ERR_SNA_INVALID;
      }

      // Keep address
      if (Version == 0)
      {
        mSNRStreamP = DataP
                    + 4   // ID
                    + 10  // Keyboard settings
                    + 51  // Clock
                    + 8;  // Total cycles
      }
      else if (Version == 1)
      {
        mSNRStreamP = DataP
                    + 4   // ID
                    + 10  // Keyboard settings
                    + 64  // Clock
                    + 8;  // Total cycles
      }
      else
      {
        // Invalid version
        Error = ERR_SNA_INVALID;
        break;
      }

      mSNREndStreamP = mSNRFileP + SNRFileSize;

      // Set keyboard matrix
      memcpy(mNativeCPC->keyboard_matrix, DataP+4, 10);

      // Apply first frame
      if (*mSNRStreamP == 0)
      {
        mSNRCount  = *(mSNRStreamP+1);
        mSNRCount += *(mSNRStreamP+2) << 8;
        mSNRStreamP += 3;
      }
      else
      {
        mSNRCount = *mSNRStreamP;
        mSNRStreamP++;
      }

      break;
    }
    //
    // Unexpected chunk
    //
    else
    {
      Error = ERR_SNA_INVALID;
      break;
    }
  }

  bool LoadOK = true;
  if (Error != ERR_SNA_OK)
  {
    // Signal error
		String ErrorMessage;
		if (Error == ERR_SNA_INVALID)
		{
			ErrorMessage = _T("Invalid SNR file");
		}
		else
		{
			ErrorMessage = _T("Emulator not ready");
		}

		Application->MessageBox(ErrorMessage.c_str(), _T("Load SNR session"), MB_OK);

    LoadOK = false;
    // Emulator reset
    Reset();
  }
  else
  {
    mSnapshotFilename = Filename;
  }

  return (LoadOK);
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::ApplySnapshot(void* dataP)
{
t_SNA_header* shP = (t_SNA_header*)dataP;
tRegister port;
tUChar val;

  bool ApplyOK = false;

  do
  {
    if (shP == NULL)
      continue;

    // Check direct accesses
    if ( (mNativeCPC->Z80OUTHandlerPtr == NULL)
         || (mNativeCPC->SetAYRegisterPtr == NULL) )
    {
      continue;
    }

    // Z80
    tZ80* Z80 = mNativeCPC->Z80;
    _A = shP->AF[1];
    _F = shP->AF[0];
    _B = shP->BC[1];
    _C = shP->BC[0];
    _D = shP->DE[1];
    _E = shP->DE[0];
    _H = shP->HL[1];
    _L = shP->HL[0];
    _R = shP->R & 0x7f;
    _Rb7 = shP->R & 0x80; // bit 7 of R
    _I = shP->I;
    if (shP->IFF0)
       _IFF1 = Pflag;
    if (shP->IFF1)
       _IFF2 = Pflag;
    _IXh = shP->IX[1];
    _IXl = shP->IX[0];
    _IYh = shP->IY[1];
    _IYl = shP->IY[0];
    Z80->Regs.SP.b.h = shP->SP[1];
    Z80->Regs.SP.b.l = shP->SP[0];
    Z80->Regs.PC.b.h = shP->PC[1];
    Z80->Regs.PC.b.l = shP->PC[0];
    _IM = shP->IM; // interrupt mode
    Z80->Regs.AFx.b.h = shP->AFx[1];
    Z80->Regs.AFx.b.l = shP->AFx[0];
    Z80->Regs.BCx.b.h = shP->BCx[1];
    Z80->Regs.BCx.b.l = shP->BCx[0];
    Z80->Regs.DEx.b.h = shP->DEx[1];
    Z80->Regs.DEx.b.l = shP->DEx[0];
    Z80->Regs.HLx.b.h = shP->HLx[1];
    Z80->Regs.HLx.b.l = shP->HLx[0];

    // Gate Array
    tGateArray* GateArray = mNativeCPC->GateArray;
    port.b.h = 0x7f;
    for (int n = 0; n < 17; n++) // loop for all colours + border
    {
      GateArray->pen = n;
      val = shP->ga_ink_values[n]; // GA palette entry
      mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, val | (1<<6));
    }
    val = shP->ga_pen; // GA pen
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, (val & 0x3f));
    val = shP->ga_ROM_config; // GA ROM configuration
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, (val & 0x3f) | (2<<6));
    val = shP->ga_RAM_config; // GA RAM configuration
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, (val & 0x3f) | (3<<6));

    // CRTC
    port.b.h = 0xbd;
    for (int n = 0; n < 18; n++) // loop for all CRTC registers
    {
      val = shP->crtc_registers[n];
      mNativeCPC->CRTC->reg_select = n;
      mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, val);
    }
    port.b.h = 0xbc;
    val = shP->crtc_reg_select; // CRTC register select
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, val);

    // ROM select
    port.b.h = 0xdf;
    val = shP->upper_ROM; // upper ROM number
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, val);

    // PPI
    port.b.h = 0xf4; // port A
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, shP->ppi_A);
    port.b.h = 0xf5; // port B
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, shP->ppi_B);
    port.b.h = 0xf6; // port C
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, shP->ppi_C);
    port.b.h = 0xf7; // control
    mNativeCPC->Z80OUTHandlerPtr(mNativeCPC, port, shP->ppi_control);

    // PSG
    tPSG* PSG = mNativeCPC->PSG;
    PSG->control = mNativeCPC->PPI->portC;
    PSG->reg_select = shP->psg_reg_select;
    for (int n = 0; n < 16; n++) // loop for all PSG registers
    {
      mNativeCPC->SetAYRegisterPtr(mNativeCPC->PSG, n, shP->psg_registers[n]);
    }

    // Version 1 snapshot
    if (shP->version == 1)
    {
      ApplyOK = true;
      continue;
    }

    // Version 2 snapshot
    if (shP->version == 2)
    {
      ApplyOK = true;
      continue;
    }

    mNativeCPC->FDC->motor = shP->fdc_motor;
    mNativeCPC->FDC->DriveA->current_track = shP->drvA_current_track;
    mNativeCPC->FDC->DriveB->current_track = shP->drvB_current_track;
    mNativeCPC->printer_port = shP->printer_data ^ 0x80; // invert bit 7 again
    mNativeCPC->PSG->AmplitudeEnv = shP->psg_env_step << 1; // multiply by 2 to bring it into the 0 - 30 range

    mNativeCPC->PSG->FirstPeriod = false;
    if (shP->psg_env_direction == 0x01) // up
    {
      switch (mNativeCPC->PSG->RegisterAY.UChar.EnvType)
      {
        case 4:
        case 5:
        case 6:
        case 7:
        case 13:
        case 14:
        case 15:
        {
          mNativeCPC->PSG->FirstPeriod = true;
        }
        break;
      }
    }
    else if (shP->psg_env_direction == 0xff) // down
    {
      switch (mNativeCPC->PSG->RegisterAY.UChar.EnvType)
      {
        case 0:
        case 1:
        case 2:
        case 3:
        case 9:
        case 10:
        case 11:
        {
          mNativeCPC->PSG->FirstPeriod = true;
        }
        break;
      }
    }

    tCRTC* CRTC = mNativeCPC->CRTC;
    tVDU* VDU = mNativeCPC->VDU;

    CRTC->addr = shP->crtc_addr[0] + (shP->crtc_addr[1] * 256);
    VDU->scanline = shP->crtc_scanline[0] + (shP->crtc_scanline[1] * 256);
    if (VDU->scanline > CRTC->MaxVSync)
    {
      VDU->scanline = CRTC->MaxVSync; // limit to max value
    }
    CRTC->char_count = shP->crtc_char_count[0];
    CRTC->line_count = shP->crtc_line_count & 127;
    CRTC->raster_count = shP->crtc_raster_count & 31;
    CRTC->hsw_count = shP->crtc_hsw_count & 15;
    CRTC->vsw_count = shP->crtc_vsw_count & 15;
    tULong dwFlags = shP->crtc_flags[0] + (shP->crtc_flags[1] * 256);
    CRTC->flag_invsync = dwFlags & 1 ? 1 : 0; // vsync active?
    if (dwFlags & 2) // hsync active?
    {
      CRTC->flags1.inHSYNC = 0xff;
      CRTC->flag_hadhsync = 1;
      if ((CRTC->hsw_count >= 3) && (CRTC->hsw_count < 7))
      {
        CRTC->flag_inmonhsync = 1;
      }
    }
    CRTC->flag_invta = dwFlags & 0x80 ? 1 : 0; // in vertical total adjust?
    GateArray->hs_count = shP->ga_int_delay & 3;
    GateArray->sl_count = shP->ga_sl_count;
    Z80->Regs.int_pending = shP->z80_int_pending;

    ApplyOK = true;
  }
  while (0);

  return (ApplyOK);
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::ReloadSnapshot(void)
{
bool LoadOK = false;

  if (mSnapshotFilename.IsEmpty() == false)
  {
    LoadOK = LoadSnapshot(mSnapshotFilename);
  }

  return (LoadOK);
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::UnloadSnapshot(void)
{
  mSnapshotFilename = "";
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::UpdateKeyboardMapping(void)
{
  if (mSettingsP == NULL)
    return;

  // ASCII keys mapping
  memset(mASCIIKeyboardP, CPC_KEY_INVALID, NB_KEYBOARD_KEYS);
  const tKeyboardMappingKey* KeyP = mSettingsP->GetFirstNonASCIIKey();
  while (KeyP != NULL)
  {
    if (KeyP->WindowsKey < NB_KEYBOARD_KEYS)
    {
      // Update keyboard key
      mASCIIKeyboardP[KeyP->WindowsKey] = KeyP->CPCKey;
    }
    KeyP = mSettingsP->GetNextNonASCIIKey();
  }

  // Hardware keyboard mapping
  memcpy(mHardwareKeyboardP, default_hardware_virtual_to_cpc_kbdmatrix, NB_KEYBOARD_KEYS);
  KeyP = mSettingsP->GetFirstHardwareKey();
  while (KeyP != NULL)
  {
    if (KeyP->WindowsKey < NB_KEYBOARD_KEYS)
    {
      // Update keyboard key
      mHardwareKeyboardP[KeyP->WindowsKey] = KeyP->CPCKey;
    }
    KeyP = mSettingsP->GetNextHardwareKey();
  }

  // Joystick keyboard mapping
  KeyP = mSettingsP->GetFirstJoystickKey();
  while (KeyP != NULL)
  {
    if ( (KeyP->CPCKey >= CPC_KEY_J0_UP)
         && ((KeyP->CPCKey - CPC_KEY_J0_UP) < NB_JOYSTICK_KEYS) )
    {
      // Update joystick key
      mJoystickKeyboardP[KeyP->CPCKey - CPC_KEY_J0_UP] = (tUChar)KeyP->WindowsKey;
    }
    KeyP = mSettingsP->GetNextJoystickKey();
  }

  // Select keyboard
  if (mSettingsP->UseHardwareKeyboardMapping == false)
  {
    mOnWINKeyPress = OnASCIIKeyPress;
    mOnWINKeyRelease = OnASCIIKeyRelease;
  }
  else
  {
    mOnWINKeyPress = OnHardwareKeyPress;
    mOnWINKeyRelease = OnHardwareKeyRelease;
  }
  mOnCPCKeyPress = OnCPCKeycodeKeyPress;
  mOnCPCKeyRelease = OnCPCKeycodeKeyRelease;

  // Apply joystick mapping
  if (mUseKeyboardAsJoystick == true)
  {
    StartKeyboardAsJoystick();
  }
}
//---------------------------------------------------------------------------


tUChar* __fastcall TEmulator::LoadROM(String ROMFilename)
{
tUChar* ROMDataP = NULL;

  if (FileExists(ROMFilename) == true)
  {
    int iFileHandle = FileOpen(ROMFilename, fmOpenRead);
    if (iFileHandle != -1)
    {
      // Get file size
      int Size = FileSeek(iFileHandle, 0, 2);
      FileSeek(iFileHandle, 0, 0); // Back to beginning of the file.

      // Allocate memory and read entire file.
      ROMDataP = new tUChar[Size];
      FileRead(iFileHandle, ROMDataP, Size);
      FileClose(iFileHandle);
    }
  }

  return ROMDataP;
}
//---------------------------------------------------------------------------

int __fastcall TEmulator::AddROM(int Slot, String Filename)
{
  int FoundSlot = -1;
  for (int Loop=Slot; Loop < NB_UPPERROMS; Loop++)
  {
    // Empty Slot ?
    if (mNativeCPC->memmap_ROM[Loop] == NULL)
    {
      mSettingsP->UpperROMFilename[Loop] = ExtractFileName(Filename);
      mNativeCPC->memmap_ROM[Loop] = LoadROM(Filename);
      FoundSlot = Loop;
      break;
    }
  }

  return FoundSlot;
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SetFDCModified(bool Value)
{
  if (Value == false)
  {
    // Reset notifications
    mNativeCPC->FDC->signal_updated = 0;
    mNativeCPC->FDC->signal_read_status = 0;
    mNativeCPC->FDC->signal_read_data = 0;
    mNativeCPC->FDC->signal_write_data = 0;
    mNativeCPC->FDC->signal_new_command = 0;
    mNativeCPC->FDC->signal_command_result = 0;
    mNativeCPC->FDC->signal_motor_change = 0;
    mNativeCPC->FDC->signal_index = 0;
    mNativeCPC->FDC->signal_track = 0;
    mNativeCPC->FDC->signal_sector = 0;
  }
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SetDriveAModified(bool Value)
{
  tDrive* DriveA = mNativeCPC->FDC->DriveA;

  if (Value == false)
  {
    // Reset notifications
    DriveA->signal_updated = 0;
    DriveA->signal_content_change = 0;
    DriveA->signal_new_position = 0;
    DriveA->signal_track_format = 0;
    DriveA->signal_sector_write = 0;
  }
  else
  {
    DriveA->signal_updated = 1;
    DriveA->signal_content_change = 1;
  }
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::SetDriveBModified(bool Value)
{
  tDrive* DriveB = mNativeCPC->FDC->DriveB;

  if (Value == false)
  {
    // Reset notifications
    DriveB->signal_updated = 0;
    DriveB->signal_content_change = 0;
    DriveB->signal_new_position = 0;
    DriveB->signal_track_format = 0;
    DriveB->signal_sector_write = 0;
  }
  else
  {
    DriveB->signal_updated = 1;
    DriveB->signal_content_change = 1;
  }
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SetDriveAFlipped(bool Value)
{
  tDrive* DriveA = mNativeCPC->FDC->DriveA;

  DriveA->flipped = (Value == false) ? 0 : 1;

  // Signal Drive content change
  DriveA->signal_updated++;
  DriveA->signal_content_change++;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::SetDriveBFlipped(bool Value)
{
  tDrive* DriveB = mNativeCPC->FDC->DriveB;

  DriveB->flipped = (Value == false) ? 0 : 1;

  // Signal Drive content change
  DriveB->signal_updated++;
  DriveB->signal_content_change++;
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::EjectTape(void)
{
#ifdef ENABLE_TAPE
  Engine_TapeEject(mNativeCPC->Tape);
  mTapeFilename = "";
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::RewindTape(void)
{
#ifdef ENABLE_TAPE

  tTape* Tape = mNativeCPC->Tape;

  Engine_TapeRewind(Tape);

  // CDT
  if (Tape->blocks)
  {
    Tape->timer_from_play = Tape->blocks[Tape->current_block].time_offset_ms / 1000;
  }
  {
    Tape->timer_from_play = 0;
  }

  Tape->timer_play_second = 0;

#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

Err __fastcall TEmulator::InsertTape(String Filename)
{
Err Result = errNone;

#ifdef ENABLE_TAPE

  if (Filename.IsEmpty() == true)
    return errNone;

  // Archive Container
  AnsiString InArchiveFilename = "";
  if (Filename.LowerCase().Pos(CONTAINER_ZIP_EXT))
  {
    int ExtensionPos = Filename.LowerCase().Pos(CONTAINER_ZIP_EXT);
    InArchiveFilename = Filename.SubString(ExtensionPos+5, Filename.Length());
    Filename = Filename.SubString(1, ExtensionPos+3);
  }

  if (FileExists(Filename) == false)
    return errFileDoesNotExist;

  // Ouverture du fichier en lecture
  int iHandle = FileOpen(Filename, fmOpenRead);
  // Met le curseur à la fin du fichier pour récupérer la taille du fichier
  int iLength = FileSeek(iHandle, 0, 2);
  // Remet le curseur en début de fichier pour la prochaine lecture
  FileSeek(iHandle, 0, 0);
  // Prepare tape image read
  tUChar* TapeDataP = (tUChar*)malloc(iLength);

  // Load entire tape image
  FileRead(iHandle,
           TapeDataP,
           iLength);

  FileClose(iHandle);


  // Load file from archive container
  if (InArchiveFilename.IsEmpty() == false)
  {
    TZIPContainer* ZIPContainer = new TZIPContainer(Filename);

    // Allocate memory
    int UncompressedSize = ZIPContainer->GetFileUncompressedSize(InArchiveFilename);
    tUChar* FileData = (tUChar*)malloc(UncompressedSize);

    // Read file from archive
    if (ZIPContainer->ReadFile(InArchiveFilename, FileData, UncompressedSize) != UNZ_OK)
    {
      Result = errBadInflate;
    }

    delete ZIPContainer;

    // Prepare disk operation
    free(TapeDataP);
    iLength = UncompressedSize;
    TapeDataP = FileData;
    Filename = InArchiveFilename;
  }

  if (Result == errNone)
  {
    Result = (Err)Engine_TapeInsert(mNativeCPC->Tape,
                                    TapeDataP,
                                    iLength);
  }

  if (Result != errNone)
  {
    EjectTape();
  }
  else
  {
    mTapeFilename = Filename;
  }

  free(TapeDataP);

#endif /* ENABLE_TAPE */

  return (Result);
}
//---------------------------------------------------------------------------
Err __fastcall TEmulator::InsertTapeCDT(String Filename,
                                        tUChar* BlockDataP,
                                        tULong Length)
{
#ifdef ENABLE_TAPE

  if ( (BlockDataP == NULL) || (Length == 0) )
  {
    EjectTape();
    return errIncorrectParameters;
  }

  Err Result = (Err)Engine_TapeInsert(mNativeCPC->Tape,
                                      BlockDataP,
                                      Length);
  if (Result != errNone)
  {
    EjectTape();
  }
  else
  {
    mTapeFilename = Filename;
  }

  return (Result);

#else /* ENABLE_TAPE */

  return (errNone);

#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------
Err __fastcall TEmulator::InsertTapeCSW(String Filename,
                                        tUChar* DataP,
                                        tULong Length,
                                        tULong SampleRate,
                                        tUChar StartPolarity)
{
Err Result = errNone;

#ifdef ENABLE_TAPE

  if ( (DataP == NULL) || (Length == 0) )
  {
    EjectTape();
    return errIncorrectParameters;
  }

  //
  // Prepare CSW Read
  //
  tTape* Tape = mNativeCPC->Tape;

  Tape->bTapeLevel = StartPolarity ? TAPE_LEVEL_HIGH : TAPE_LEVEL_LOW;

  Tape->csw_sample_rate = SampleRate;
  Tape->csw_cycles_per_sample = TAPE_CSW_SAMPLE_CYCLES(SampleRate);
  Tape->iTapeCycleCount = (tLong)Tape->csw_cycles_per_sample;

  Tape->pbCSWBuffer = DataP;
  Tape->pbCSWData = DataP;
  Tape->pbCSWEnd = Tape->pbCSWBuffer + Length;

  //
  // Compute CSW duration
  //
  Tape->total_duration_s = 0;
  tUChar* BufferP = Tape->pbCSWBuffer;
  tULong Count = 0;
  while (BufferP < Tape->pbCSWEnd)
  {
    tULong NbPulses = *BufferP++;
    if (NbPulses)
    {
      Count += NbPulses;
    }
    else if ((BufferP+3) < Tape->pbCSWEnd)
    {
      Count +=   ((*(BufferP+0)) << 0)
               + ((*(BufferP+1)) << 8)
               + ((*(BufferP+2)) << 16)
               + ((*(BufferP+3)) << 24);
      BufferP += 4;
    }

    while (Count >= SampleRate)
    {
      Tape->total_duration_s++;
      Count -= SampleRate;
    }
  }

  // Set first Pulse
  Tape->csw_nbsamples = *Tape->pbCSWData++;
  if (!Tape->csw_nbsamples)
  {
    if ((Tape->pbCSWEnd - Tape->pbCSWData) >= 4)
    {
      Tape->csw_nbsamples =   ((*(Tape->pbCSWData++)) << 0)
                            + ((*(Tape->pbCSWData++)) << 8)
                            + ((*(Tape->pbCSWData++)) << 16)
                            + ((*(Tape->pbCSWData++)) << 24);
    }
  }

  mTapeFilename = Filename;

#endif /* ENABLE_TAPE */

  return (Result);
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::PlayTape(void)
{
#ifdef ENABLE_TAPE

tTape* Tape = mNativeCPC->Tape;

  Tape->tape_play_button = 1;

  Tape->bTapeLevel = TAPE_LEVEL_LOW;
  Tape->iTapeCycleCount = TAPE_PLAY_START_PAUSE;

#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::StopTape(void)
{
#ifdef ENABLE_TAPE
tTape* Tape = mNativeCPC->Tape;

  Tape->tape_play_button = 0;
  
  Tape->bTapeLevel = TAPE_LEVEL_LOW;
  Tape->iTapeCycleCount = TAPE_PLAY_START_PAUSE;
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::IsTapePlaying(void)
{
#ifdef ENABLE_TAPE
  return (mNativeCPC->Tape->tape_play_button ? true : false);
#else
  return (false);
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::TapeSetCurrentBlock(int NewBlock)
{
#ifdef ENABLE_TAPE
tTape* Tape = mNativeCPC->Tape;

  if (Tape->blocks)
  {
    tTapeBlock* TapeBlock = &Tape->blocks[NewBlock];

    Tape->pbTapeBlock = TapeBlock->pbImageData;
    Tape->current_block = NewBlock;

    Tape->bTapeLevel = TAPE_LEVEL_LOW;
    Tape->iTapeCycleCount = TAPE_PLAY_START_PAUSE;

    Engine_TapeGetNextBlock(Tape);

    Tape->timer_from_play = Tape->blocks[Tape->current_block].time_offset_ms / 1000;
    Tape->timer_play_second = 0;
  }
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::ResetTapeCounterTo000(void)
{
#ifdef ENABLE_TAPE
  Engine_TapeResetCounter(mNativeCPC->Tape);
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

void __fastcall TEmulator::RewindToTapeCounter000(void)
{
#ifdef ENABLE_TAPE
tTape* Tape = mNativeCPC->Tape;

  if (Tape->blocks)
  {
    tTapeBlock* TapeBlock = &Tape->blocks[Tape->counter000_block];

    Tape->pbTapeBlock = TapeBlock->pbImageData;
    Tape->current_block = Tape->counter000_block;

    Tape->bTapeLevel = TAPE_LEVEL_LOW;
    Tape->iTapeCycleCount = TAPE_PLAY_START_PAUSE;

    Engine_TapeGetNextBlock(Tape);

    Tape->timer_from_play = Tape->blocks[Tape->current_block].time_offset_ms / 1000;
    Tape->timer_play_second = 0;
  }
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

int  __fastcall TEmulator::GetTapePlayDuration(void)
{
#ifdef ENABLE_TAPE
  return (mNativeCPC->Tape->timer_from_play);
#else /* ENABLE_TAPE */
  return 0;
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

int  __fastcall TEmulator::GetTapeTotalDuration(void)
{
#ifdef ENABLE_TAPE
  return (mNativeCPC->Tape->total_duration_s);
#else /* ENABLE_TAPE */
  return 0;
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

int  __fastcall TEmulator::GoToPreviousMessage(void)
{
#ifdef ENABLE_TAPE
  tTape* Tape = mNativeCPC->Tape;
  int Ret = Engine_TapeGetPrevMessageBlock(Tape);

  if (Tape->blocks)
  {
    Tape->timer_from_play = Tape->blocks[Tape->current_block].time_offset_ms / 1000;
    Tape->timer_play_second = 0;
  }

  return Ret;
#else /* ENABLE_TAPE */
  return 0;
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------

int  __fastcall TEmulator::GoToNextMessage(void)
{
#ifdef ENABLE_TAPE
  tTape* Tape = mNativeCPC->Tape;
  int Ret = Engine_TapeGetNextMessageBlock(Tape);

  if (Tape->blocks)
  {
    Tape->timer_from_play = Tape->blocks[Tape->current_block].time_offset_ms / 1000;
    Tape->timer_play_second = 0;
  }

  return Ret;
#else /* ENABLE_TAPE */
  return 0;
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------


bool __fastcall TEmulator::IsTapeNavigationAllowed(void)
{
#ifdef ENABLE_TAPE
  tTape* Tape = mNativeCPC->Tape;
  bool Allowed = false;

  if (Tape->blocks)
  {
    for (tULong Block=0; Block < Tape->nb_blocks; Block++)
    {
      if ( (Tape->blocks[Block].block_id == 0x30)
           || (Tape->blocks[Block].block_id == 0x31) )
      {
        Allowed = true;
        break;
      }
    }
  }

  return Allowed;
#else /* ENABLE_TAPE */
  return false;
#endif /* ENABLE_TAPE */
}
//---------------------------------------------------------------------------


bool __fastcall TEmulator::PrepareDirectCDTUseSettings(TSettings* SettingsP)
{
bool SettingsChanged = false;

  // Check machine
  if ( (SettingsP->CPCDevice != CPC_MODEL_464_UK)
       && (SettingsP->CPCDevice != CPC_MODEL_464_FR) )
  {
    SettingsChanged = true;
    if (SettingsP->PreferredKeyboardLayout == 1)
    {
      SettingsP->CPCDevice = CPC_MODEL_464_FR;
    }
    else
    {
      SettingsP->CPCDevice = CPC_MODEL_464_UK;
    }
  }

  // Remove additional memory
  if (SettingsP->Use256kSiliconDisc == true)
  {
    SettingsChanged = true;
    SettingsP->Use256kSiliconDisc = false;
  }

  // Remove floppy drive
  if (SettingsP->UseFloppyDrive == true)
  {
    SettingsChanged = true;
    SettingsP->UseFloppyDrive = false;
  }

  // Remove all ROMS
  for (int Loop=1; Loop < NB_UPPERROMS; Loop++)
  {
    if (Loop == 7)
    {
      // Remove AMSDOS
      if (SettingsP->UpperROMFilename[Loop] != EmptyRomName)
      {
        SettingsChanged = true;
        SettingsP->UpperROMFilename[Loop] = EmptyRomName;
      }
    }
    else if (SettingsP->UpperROMFilename[Loop].IsEmpty() == false)
    {
      SettingsChanged = true;
      SettingsP->UpperROMFilename[Loop] = "";
    }
  }

  return SettingsChanged;
}
//---------------------------------------------------------------------------
bool __fastcall TEmulator::PrepareDirectDSKUseSettings(TSettings* SettingsP)
{
bool SettingsChanged = false;

  // Check machine
  if ( (SettingsP->CPCDevice != CPC_MODEL_6128_UK)
       && (SettingsP->CPCDevice != CPC_MODEL_6128_FR) )
  {
    SettingsChanged = true;
    if (SettingsP->PreferredKeyboardLayout == 1)
    {
      SettingsP->CPCDevice = CPC_MODEL_6128_FR;
    }
    else
    {
      SettingsP->CPCDevice = CPC_MODEL_6128_UK;
    }
  }

  // Add floppy drive
  if (SettingsP->UseFloppyDrive == false)
  {
    SettingsChanged = true;
    SettingsP->UseFloppyDrive = true;
  }

  // Remove all ROMS
  for (int Loop=1; Loop < NB_UPPERROMS; Loop++)
  {
    if (Loop == 7)
    {
      // Restore AMSDOS
      if (SettingsP->UpperROMFilename[Loop] != DefaultAMSDOSRomName)
      {
        SettingsChanged = true;
        SettingsP->UpperROMFilename[Loop] = DefaultAMSDOSRomName;
      }
    }
    else if (SettingsP->UpperROMFilename[Loop].IsEmpty() == false)
    {
      SettingsChanged = true;
      SettingsP->UpperROMFilename[Loop] = "";
    }
  }

  return SettingsChanged;
}
//---------------------------------------------------------------------------


AnsiString __fastcall TEmulator::GetFirstSimpleMessage(void)
{
AnsiString Message = "";

  if (mSimpleMessages->Count)
  {
    Message = mSimpleMessages->Strings[0];
    mSimpleMessages->Delete(0);
  }

  return Message;
};
//---------------------------------------------------------------------------

void __fastcall TEmulator::CreateMessagesFromTape(tTape* TapeP)
{
int block_index = -1;

  // Find block descriptor
  tTapeBlock* TapeblockP = TapeP->blocks;
  for (int Loop = 0; Loop < (int)TapeP->nb_blocks; Loop++)
  {
    if (TapeblockP->pbImageData == TapeP->pbMessagesStart)
    {
      block_index = Loop;
      break;
    }
    TapeblockP++;
  }

  // Block found ?
  if (block_index == -1) return;

  tUChar* BlockP = TapeP->pbMessagesStart;
  while (BlockP < TapeP->pbMessagesEnd)
  {
    // Tape BLOCK TEXT DESCRIPTION: 0x30
    if (*BlockP == 0x30)
    {
      AnsiString Description = AnsiString((char*)BlockP+2);
      mSimpleMessages->Add("Tape: " + Description.SubString(1, *(BlockP+1)));
    }

    // Tape BLOCK MESSAGE BLOCK: 0x31
    else if (*BlockP == 0x31)
    {
    }

    BlockP += TapeblockP->length;
    TapeblockP++;
  }
}
//---------------------------------------------------------------------------


AnsiString __fastcall TEmulator::GetCatalogEntry(int Index)
{
  return mDiskCatalogEntries->Strings[Index];
};
//---------------------------------------------------------------------------


void __fastcall TEmulator::StartDecathlonMode(void)
{
  mDecathlonModeActive = true;
  mDecathlonCycleCount = 1; // Start at once
  mDecathlonCurrentKey = CPC_KEY_J0_LEFT;
};
//---------------------------------------------------------------------------
void __fastcall TEmulator::StopDecathlonMode(void)
{
  mDecathlonModeActive = false;
  // Release current key
  tUShort cpc_key = mKeyboardCPCP[mDecathlonCurrentKey];
  RELEASE_KEY(cpc_key);
};
//---------------------------------------------------------------------------


void __fastcall TEmulator::StartCombatSchoolMode(void)
{
  mCombatSchoolModeActive = true;
  mCombatSchoolCycleCount = 1; // Start at once
  mCombatSchoolCurrentKey = CPC_KEY_J0_LEFT;
};
//---------------------------------------------------------------------------
void __fastcall TEmulator::StopCombatSchoolMode(void)
{
  mCombatSchoolModeActive = false;
  // Release current key
  tUShort cpc_key = mKeyboardCPCP[mCombatSchoolCurrentKey];
  RELEASE_KEY(cpc_key);
};
//---------------------------------------------------------------------------


void __fastcall TEmulator::StartAutoType(String Text)
{
  if (Text.Length() == 0) return;

  // Allocate autotype buffer
  mAutoTypeBufferP = new char[Text.Length()+1];
  memset(mAutoTypeBufferP, 0, Text.Length()+1);

  // Copy text buffer
  _TCHAR* TextStr = Text.c_str();
  char* AutoTypeStr = mAutoTypeBufferP;
  for (int loop=0; loop < Text.Length(); loop++)
  {
    *(AutoTypeStr++) = TextStr[loop];
  }
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::StartAutoPlay(void)
{
  mAutoPlayRequested = true;
  mAutoPlayDuration = 0;
  mAutoPlayStatus = 0;
}
//---------------------------------------------------------------------------

bool __fastcall TEmulator::GetAutoTypeInProgress(void)
{
  return (mAutoTypeBufferP != NULL ? true : false);
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SaveContext(String Filename)
{
#ifdef _DEBUG

  int FileHandle = FileCreate(Filename);
  if (FileHandle > 0)
  {
    // Write entire context content
    FileWrite(FileHandle, (tUChar*)mContextP, SIZETAB_CONTEXT);
    FileClose(FileHandle);
  }

#endif /* _DEBUG */
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::PlaySNRStream(void)
{
  // Decrement delay
  mSNRCount--;

  // Delay elapsed ?
  if (!mSNRCount)
  {
    // Get size of actions
    tUChar ActionsSize = *mSNRStreamP++;

    // Synchronisation ?
    if (ActionsSize & 0x80)
    {
      // Pass synchro
      ActionsSize &= 0x7f; // Remove synchro flag
      mSNRStreamP += 4;
    }

    // Something to do ?
    if (ActionsSize)
    {
      // New action
      if (ActionsSize < 0x7f)
      {
        // Save last action
        mSNRActionP = mSNRStreamP;
        mSNRActionSize = ActionsSize;

        // Go to next action
        mSNRStreamP += ActionsSize;
      }

      // Perform actions
      tUChar* ActionP = mSNRActionP;
      tUChar* ActionEndP = ActionP + mSNRActionSize;
      while (ActionP < ActionEndP)
      {
        tUChar Action = *ActionP;

        // RTC
        if ( (Action & 0xE0) == 0xE0)
        {
          int ActionSize = Action & 0x1F;
          ActionP += ActionSize + 1;
        }
        // MOUSE
        else if (Action & 0x80)
        {
          // SNR_MB
          if (Action & 0x01)
          {
            ActionP++;
          }
          // SNR_MX
          if (Action & 0x60)
          {
            ActionP++;
            // SNR_MX_W
            if (Action & 0x40)
            {
              ActionP++;
            }
          }
          // SNR_MY
          if (Action & 0x18)
          {
            ActionP++;
            // SNR_MY_W
            if (Action & 0x10)
            {
              ActionP++;
            }
          }
          // SNR_MW
          if (Action & 0x06)
          {
            ActionP++;
            // SNR_MW_W
            if (Action & 0x04)
            {
              ActionP++;
            }
          }
        }
        else
        {
          // Toggle key
          mNativeCPC->keyboard_matrix[Action >> 3] ^= (1 << (Action & 0x07));
          ActionP++;
        }
      }
    }

    // Next frame
    if (*mSNRStreamP == 0)
    {
      mSNRCount  = *(mSNRStreamP+1);
      mSNRCount += *(mSNRStreamP+2) << 8;
      mSNRStreamP += 3;
    }
    else
    {
      mSNRCount = *mSNRStreamP;
      mSNRStreamP++;
    }

    // End of stream ?
    if (mSNRStreamP >= mSNREndStreamP)
    {
      StopSNRSession();
    }
  }
}
//---------------------------------------------------------------------------
bool __fastcall TEmulator::IsPlayingSNRStream(void)
{
  return mSNRStreamP ? cTrue : cFalse;
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::StopSNRSession(void)
{
  mSNRStreamP = NULL;

  // Reset keyboard matrix
  memset(mNativeCPC->keyboard_matrix, 0xff, NATIVECPC_KEYBOARD_MATRIX_NBELEMENT);
}
//---------------------------------------------------------------------------


void __fastcall TEmulator::SetPrinterOnline(void)
{
#ifdef ENABLE_PRINTER
  Engine_PrinterSetOnline(mNativeCPC->Printer);
#endif /* ENABLE_PRINTER */
}
//---------------------------------------------------------------------------
void __fastcall TEmulator::SetPrinterOffline(void)
{
#ifdef ENABLE_PRINTER
  Engine_PrinterSetOffline(mNativeCPC->Printer);
#endif /* ENABLE_PRINTER */
}
//---------------------------------------------------------------------------

