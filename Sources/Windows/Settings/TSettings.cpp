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


#include <IniFiles.hpp>
#include <FileCtrl.hpp>
#pragma hdrstop
#include "TSettings.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

const char* Devices[] =
{
  "CPC_464_UK",
  "CPC_664_UK",
  "CPC_6128_UK",
  "CPC_464_FR",
  "CPC_664_FR",
  "CPC_6128_FR",
  "CPC_464_SP",
  "CPC_6128_SP",
};
#define NB_DEVICES                              NUMBER_OF_ITEMS(Devices)

const char* Brands[] =
{
  "Amstrad",
  "Orion",
  "Schneider",
  "Awa",
  "Solavox",
  "Saisho",
  "Triumph",
  "Isp",
};
#define NB_BRANDS                               NUMBER_OF_ITEMS(Brands)


#define DEFAULT_PREFIX                      "Default"
const char* const EmptyRomName              = "Empty";
const char* const DefaultPrefix             = DEFAULT_PREFIX;
const char* const DefaultOSRomName          = DEFAULT_PREFIX " OS";
const char* const DefaultBASICRomName       = DEFAULT_PREFIX " BASIC";
const char* const DefaultAMSDOSRomName      = DEFAULT_PREFIX " AMSDOS";
const char* const DefaultSiliconDiscRomName = DEFAULT_PREFIX " Silicon Disc 1.3";


//
// Sections
//
#define STRING_MAINFORM                         "MainForm"
#define STRING_DRIVES                           "Drives"
#define STRING_TAPE                             "Tape"
#define STRING_EMULATOR                         "Emulator"
#define STRING_INPUTS                           "Inputs"
#define STRING_AUDIO                            "Audio"
#define STRING_ASCIIKEYBOARD                    "ASCII Keyboard"
#define STRING_HARDWAREKEYBOARD                 "Hardware Keyboard"
#define STRING_JOYSTICKKEYBOARD                 "Joystick Keyboard"
#define STRING_ROMS                             "Roms"
#define STRING_WINDOWS                          "Windows"
#define STRING_SNAPSHOT                         "Snapshot"
#define STRING_SCREENSHOT                       "Screenshot"
#define STRING_PRINTER                          "Printer"
#define STRING_CHEATSCRIPTS                     "CheatScripts"

// Section MAINFORM
#define STRING_MAINFORM_LEFT                    "Left"
#define DEFAULT_MAINFORM_LEFT                   0
#define STRING_MAINFORM_TOP                     "Top"
#define DEFAULT_MAINFORM_TOP                    0
#define STRING_MAINFORM_WIDTH                   "PanelCPCWidth"
#define DEFAULT_MAINFORM_WIDTH                  768
#define STRING_MAINFORM_HEIGHT                  "PanelCPCHeight"
#define DEFAULT_MAINFORM_HEIGHT                 544
#define STRING_MAINFORM_USEOPENGL               "UseOpenGL"
#define DEFAULT_MAINFORM_USEOPENGL              false
#define STRING_MAINFORM_FORCEGDI                "ForceGDI"
#define DEFAULT_MAINFORM_FORCEGDI               false
#define STRING_MAINFORM_ONSCREENDISPLAY         "OnscreenDisplay"
#define DEFAULT_MAINFORM_ONSCREENDISPLAY        (OSD_DISK | OSD_TAPE | OSD_ICONS | OSD_MESSAGES)
#define STRING_MAINFORM_FULLSCREEN              "Fullscreen"
#define DEFAULT_MAINFORM_FULLSCREEN             false
#define STRING_MAINFORM_KEEPFULLSCREEN          "KeepFullscreen"
#define DEFAULT_MAINFORM_KEEPFULLSCREEN         false
#define STRING_MAINFORM_WINDOWSTYLE             "WindowStyle"
#define DEFAULT_MAINFORM_WINDOWSTYLE            0
#define STRING_MAINFORM_WINDOWEXSTYLE           "WindowExStyle"
#define DEFAULT_MAINFORM_WINDOWEXSTYLE          0
#define STRING_MAINFORM_SCANLINES               "DrawScanlines"
#define DEFAULT_MAINFORM_SCANLINES              true
#define STRING_MAINFORM_SCANLINES_DIRECTORY     "ScanlinesDirectory"
#define DEFAULT_MAINFORM_SCANLINES_DIRECTORY    "Scanlines\\"
#define STRING_MAINFORM_CONTROL_PANEL_VISIBLE   "ControlPanelVisible"
#define DEFAULT_MAINFORM_CONTROL_PANEL_VISIBLE  true
#define STRING_MAINFORM_FULLSCREEN_BRIGHTNESS   "FullscreenBrightness"
#define DEFAULT_MAINFORM_FULLSCREEN_BRIGHTNESS  100
#define STRING_MAINFORM_FULLSCREEN_MENU         "FullscreenMenu"
#define DEFAULT_MAINFORM_FULLSCREEN_MENU        false
#define STRING_MAINFORM_KEEPASPECTRATIO         "KeepAspectRatio"
#define DEFAULT_MAINFORM_KEEPASPECTRATIO        true
#define STRING_MAINFORM_SHOWPAUSEINFO           "ShowPauseInfo"
#define DEFAULT_MAINFORM_SHOWPAUSEINFO          true

// Section DRIVES
#define STRING_DRIVEA_DISKFILENAME              "DriveADiskFilename"
#define DEFAULT_DRIVEA_DISKFILENAME             ""
#define STRING_DRIVEB_DISKFILENAME              "DriveBDiskFilename"
#define DEFAULT_DRIVEB_DISKFILENAME             ""
#define STRING_AUTOSTART_ENABLE                 "AutoStartEnable"
#define DEFAULT_AUTOSTART_ENABLE                false
#define STRING_DRIVE_ICON_SIZE_FACTOR           "DriveIconSizeFactor"
#define DEFAULT_DRIVE_ICON_SIZE_FACTOR          75
#define STRING_DISC_DIRECTORY                   "DiscDirectory"
#define DEFAULT_DISC_DIRECTORY                  ""
#define STRING_DISC_CONFIRMDISKIMAGEUPDATES     "ConfirmDiskImageUpdates"
#define DEFAULT_DISC_CONFIRMDISKIMAGEUPDATES    false
#define STRING_DISC_NEVERUPDATEDISKIMAGES       "NeverUpdateDiskImages"
#define DEFAULT_DISC_NEVERUPDATEDISKIMAGES      false
#define STRING_SHOW_STATIC_DRIVE_ICON           "ShowStaticDriveIcon"
#define DEFAULT_SHOW_STATIC_DRIVE_ICON          false

// Section TAPE
#define STRING_TAPE_FILENAME                    "TapeFilename"
#define DEFAULT_TAPE_FILENAME                   ""
#define STRING_AUTOPLAY_ENABLE                  "AutoPlayEnable"
#define DEFAULT_AUTOPLAY_ENABLE                 false
#define STRING_TAPE_DIRECTORY                   "TapeDirectory"
#define DEFAULT_TAPE_DIRECTORY                  ""

// Section EMULATOR
#define STRING_EMULATOR_DEVICE                  "Device"
#define DEFAULT_EMULATOR_DEVICE                 Devices[2]
#define STRING_EMULATOR_BRAND                   "Brand"
#define DEFAULT_EMULATOR_BRAND                  Brands[0]
#define STRING_EMULATOR_CRTC_TYPE               "CRTCType"
#define DEFAULT_EMULATOR_CRTC_TYPE              0
#define STRING_EMULATOR_FLOPPYDRIVE             "FloppyDrive"
#define DEFAULT_EMULATOR_FLOPPYDRIVE            true
#define STRING_EMULATOR_FASTFDC                 "FastFDC"
#define DEFAULT_EMULATOR_FASTFDC                false
#define STRING_EMULATOR_64KMEMEXT               "64kMemoryExtension"
#define DEFAULT_EMULATOR_64KMEMEXT              false
#define STRING_EMULATOR_256KMEMEXT              "256kMemoryExtension"
#define DEFAULT_EMULATOR_256KMEMEXT             false
#define STRING_EMULATOR_256KSILICONDISC         "256kSiliconDisc"
#define DEFAULT_EMULATOR_256KSILICONDISC        false
#define STRING_EMULATOR_TECHNIMUSIQUE           "TechniMusique"
#define DEFAULT_EMULATOR_TECHNIMUSIQUE          false
#define STRING_EMULATOR_PLAYCITY                "PlayCity"
#define DEFAULT_EMULATOR_PLAYCITY               false
#define STRING_EMULATOR_MODE2ALIASING           "Mode2Antialiasing"
#define DEFAULT_EMULATOR_MODE2ALIASING          false
#define STRING_EMULATOR_COLORMONITOR            "ColorMonitor"
#define DEFAULT_EMULATOR_COLORMONITOR           true
#define STRING_EMULATOR_PREFKEYBLAYOUT          "PreferredKeyboardLayout"
#define DEFAULT_EMULATOR_PREFKEYBLAYOUT         0
#define STRING_EMULATOR_MAXSPEEDINDEX           "MaxSpeedIndex"
#define DEFAULT_EMULATOR_MAXSPEEDINDEX          0
#define STRING_EMULATOR_MAXCUSTOMSPEED          "MaxCustomSpeed"
#define DEFAULT_EMULATOR_MAXCUSTOMSPEED         150
#define STRING_EMULATOR_WINDOWSPRIORITY         "WindowsPriority"
#define DEFAULT_EMULATOR_WINDOWSPRIORITY        0
#define STRING_EMULATOR_COLDRESETATREALSPEED    "ColdResetAtRealSpeed"
#define DEFAULT_EMULATOR_COLDRESETATREALSPEED   false
#define STRING_EMULATOR_SMALLSCREENSHOTSHORTCUT    "SmallScreenshotShortcut"
#define DEFAULT_EMULATOR_SMALLSCREENSHOTSHORTCUT   false
#define STRING_EMULATOR_THEME_INDEX             "ThemeIndex"
#define DEFAULT_EMULATOR_THEME_INDEX            1
#define STRING_EMULATOR_THEME_BACKGROUND        "ThemeBackground"
#define DEFAULT_EMULATOR_THEME_BACKGROUND       0x00800000
#define STRING_EMULATOR_THEME_TEXT1             "ThemeText1"
#define DEFAULT_EMULATOR_THEME_TEXT1            0x00FFFF
#define STRING_EMULATOR_THEME_TEXT2             "ThemeText2"
#define DEFAULT_EMULATOR_THEME_TEXT2            0xFFFF00
#define STRING_EMULATOR_SHOWHINTS               "ShowHints"
#define DEFAULT_EMULATOR_SHOWHINTS              true
#define STRING_EMULATOR_VHOLD_ADJUST            "VHoldAdjust"
#define DEFAULT_EMULATOR_VHOLD_ADJUST           250

// Section INPUTS
#define STRING_PROFILE_DIRECTORY                "ProfileDirectory"
#define DEFAULT_PROFILE_DIRECTORY               "Profiles\\"
#define STRING_PROFILE_DEFAULT                  "DefaultProfileFilename"
#define DEFAULT_PROFILE_DEFAULT                 "Default.prfl"
#define STRING_USEHARDWAREKEYBOARDMAP           "UseHardwareKeyboardMapping"
#define DEFAULT_USEHARDWAREKEYBOARDMAP          false
#define STRING_AMXMOUSE_FACTORX                 "AMXMouseFactorX"
#define DEFAULT_AMXMOUSE_FACTORX                150
#define STRING_AMXMOUSE_FACTORY                 "AMXMouseFactorY"
#define DEFAULT_AMXMOUSE_FACTORY                100

// Section AUDIO
#define STRING_AUDIO_MUTE                       "Mute"
#define DEFAULT_AUDIO_MUTE                      false
#define STRING_SPEAKER_STEREO                   "SpeakerStereo"
#define DEFAULT_SPEAKER_STEREO                  true
#define STRING_PLAYCITY_STEREO                  "PlayCityStereo"
#define DEFAULT_PLAYCITY_STEREO                 true
#define STRING_PLAYCITY_INVERT                  "PlayCityInvert"
#define DEFAULT_PLAYCITY_INVERT                 true
#define STRING_GAIN_SPEAKER                     "GainSpeaker"
#define DEFAULT_GAIN_SPEAKER                    100
#define STRING_GAIN_FLOPPY                      "GainFloppy"
#define DEFAULT_GAIN_FLOPPY                     30
#define STRING_GAIN_TAPE                        "GainTape"
#define DEFAULT_GAIN_TAPE                       30
#define STRING_GAIN_TMPI                        "GainTMPI"
#define DEFAULT_GAIN_TMPI                       100
#define STRING_GAIN_PLAYCITY                    "GainPlayCity"
#define DEFAULT_GAIN_PLAYCITY                   100
#define STRING_VOLUME                           "Volume"
#define DEFAULT_VOLUME                          100
#define STRING_AUDIO_RENDERER                   "AudioRenderer"
#define DEFAULT_AUDIO_RENDERER                  ""
#define STRING_TAPE_AUDIO_USEDEFAULT            "TapeAudioUseDefault"
#define DEFAULT_TAPE_AUDIO_USEDEFAULT           true
#define STRING_TAPE_PPITHRESHOLD8               "TapePPIThreshold8"
#define DEFAULT_TAPE_PPITHRESHOLD8              132
#define STRING_TAPE_PPITHRESHOLD16              "TapePPIThreshold16"
#define DEFAULT_TAPE_PPITHRESHOLD16             1024
#define STRING_TAPE_AUTOGAIN_ACTIVE             "TapeAutoGainActive"
#define DEFAULT_TAPE_AUTOGAIN_ACTIVE            true
#define STRING_TAPE_LPF1_ACTIVE                 "TapeLPF1Active"
#define DEFAULT_TAPE_LPF1_ACTIVE                true
#define STRING_TAPE_LPF1_A                      "TapeLPF1_A"
#define DEFAULT_TAPE_LPF1_A                     "0.4"
#define STRING_TAPE_LPF2_ACTIVE                 "TapeLPF2Active"
#define DEFAULT_TAPE_LPF2_ACTIVE                false
#define STRING_TAPE_LPF2_ORDER                  "TapeLPF2Order"
#define DEFAULT_TAPE_LPF2_ORDER                 2
#define STRING_TAPE_LPF2_FREQ                   "TapeLPF2Freq"
#define DEFAULT_TAPE_LPF2_FREQ                  4000
#define STRING_TAPE_HPF2_ACTIVE                 "TapeHPF2Active"
#define DEFAULT_TAPE_HPF2_ACTIVE                false
#define STRING_TAPE_HPF2_ORDER                  "TapeHPF2Order"
#define DEFAULT_TAPE_HPF2_ORDER                 2
#define STRING_TAPE_HPF2_FREQ                   "TapeHPF2Freq"
#define DEFAULT_TAPE_HPF2_FREQ                  500
#define STRING_TAPE_CREATE_CSW                  "TapeCreateCSW"
#define DEFAULT_TAPE_CREATE_CSW                 false

// Section ROMS
#define STRING_ROMS_DIRECTORY                   "RomsDirectory"
#define DEFAULT_ROMS_DIRECTORY                  "Roms\\"
#define STRING_ROMS_LOWERROM                    "LowerROM"
#define STRING_ROMS_UPPERROM                    "UpperROM"
#define DEFAULT_ROMS_FILENAME                   ""

// Sections Windows
#define STRING_DSKVIEWER_LEFT                   "DSKViewerFormLeft"
#define DEFAULT_DSKVIEWER_LEFT                  0
#define STRING_DSKVIEWER_TOP                    "DSKViewerFormTop"
#define DEFAULT_DSKVIEWER_TOP                   0
#define STRING_DSKVIEWER_WIDTH                  "DSKViewerFormWidth"
#define DEFAULT_DSKVIEWER_WIDTH                 0
#define STRING_DSKVIEWER_HEIGHT                 "DSKViewerFormHeight"
#define DEFAULT_DSKVIEWER_HEIGHT                0
#define STRING_CRTC_LEFT                        "CRTCFormLeft"
#define DEFAULT_CRTC_LEFT                       0
#define STRING_CRTC_TOP                         "CRTCFormTop"
#define DEFAULT_CRTC_TOP                        0
#define STRING_CRTC_WIDTH                       "CRTCFormWidth"
#define DEFAULT_CRTC_WIDTH                      0
#define STRING_CRTC_HEIGHT                      "CRTCFormHeight"
#define DEFAULT_CRTC_HEIGHT                     0
#define STRING_GATEARRAY_LEFT                   "GateArrayFormLeft"
#define DEFAULT_GATEARRAY_LEFT                  0
#define STRING_GATEARRAY_TOP                    "GateArrayFormTop"
#define DEFAULT_GATEARRAY_TOP                   0
#define STRING_GATEARRAY_WIDTH                  "GateArrayFormWidth"
#define DEFAULT_GATEARRAY_WIDTH                 0
#define STRING_GATEARRAY_HEIGHT                 "GateArrayFormHeight"
#define DEFAULT_GATEARRAY_HEIGHT                0
#define STRING_FDC_LEFT                         "FDCFormLeft"
#define DEFAULT_FDC_LEFT                        0
#define STRING_FDC_TOP                          "FDCFormTop"
#define DEFAULT_FDC_TOP                         0
#define STRING_FDC_WIDTH                        "FDCFormWidth"
#define DEFAULT_FDC_WIDTH                       0
#define STRING_FDC_HEIGHT                       "FDCFormHeight"
#define DEFAULT_FDC_HEIGHT                      0
#define STRING_Z80_LEFT                         "Z80FormLeft"
#define DEFAULT_Z80_LEFT                        0
#define STRING_Z80_TOP                          "Z80FormTop"
#define DEFAULT_Z80_TOP                         0
#define STRING_Z80_WIDTH                        "Z80FormWidth"
#define DEFAULT_Z80_WIDTH                       0
#define STRING_Z80_HEIGHT                       "Z80FormHeight"
#define DEFAULT_Z80_HEIGHT                      0
#define STRING_MEMORY_LEFT                      "MemoryFormLeft"
#define DEFAULT_MEMORY_LEFT                     0
#define STRING_MEMORY_TOP                       "MemoryFormTop"
#define DEFAULT_MEMORY_TOP                      0
#define STRING_MEMORY_WIDTH                     "MemoryFormWidth"
#define DEFAULT_MEMORY_WIDTH                    0
#define STRING_MEMORY_HEIGHT                    "MemoryFormHeight"
#define DEFAULT_MEMORY_HEIGHT                   0
#define STRING_PSG_LEFT                         "PSGFormLeft"
#define DEFAULT_PSG_LEFT                        0
#define STRING_PSG_TOP                          "PSGFormTop"
#define DEFAULT_PSG_TOP                         0
#define STRING_PSG_WIDTH                        "PSGFormWidth"
#define DEFAULT_PSG_WIDTH                       0
#define STRING_PSG_HEIGHT                       "PSGFormHeight"
#define DEFAULT_PSG_HEIGHT                      0
#define STRING_CDTVIEWER_LEFT                   "CDTViewerFormLeft"
#define DEFAULT_CDTVIEWER_LEFT                  0
#define STRING_CDTVIEWER_TOP                    "CDTViewerFormTop"
#define DEFAULT_CDTVIEWER_TOP                   0
#define STRING_CDTVIEWER_WIDTH                  "CDTViewerFormWidth"
#define DEFAULT_CDTVIEWER_WIDTH                 0
#define STRING_CDTVIEWER_HEIGHT                 "CDTViewerFormHeight"
#define DEFAULT_CDTVIEWER_HEIGHT                0
#define STRING_TAPEAUDIOWORKSHOP_LEFT           "TapeAudioWorkshopFormLeft"
#define DEFAULT_TAPEAUDIOWORKSHOP_LEFT          0
#define STRING_TAPEAUDIOWORKSHOP_TOP            "TapeAudioWorkshopFormTop"
#define DEFAULT_TAPEAUDIOWORKSHOP_TOP           0
#define STRING_MEA8000_LEFT                     "MEA8000FormLeft"
#define DEFAULT_MEA8000_LEFT                    0
#define STRING_MEA8000_TOP                      "MEA8000FormTop"
#define DEFAULT_MEA8000_TOP                     0
#define STRING_MEA8000_WIDTH                    "MEA8000FormWidth"
#define DEFAULT_MEA8000_WIDTH                   0
#define STRING_MEA8000_HEIGHT                   "MEA8000FormHeight"
#define DEFAULT_MEA8000_HEIGHT                  0
#define STRING_DEBUGGER_LEFT                    "DebuggerFormLeft"
#define DEFAULT_DEBUGGER_LEFT                   0
#define STRING_DEBUGGER_TOP                     "DebuggerFormTop"
#define DEFAULT_DEBUGGER_TOP                    0
#define STRING_DEBUGGER_WIDTH                   "DebuggerFormWidth"
#define DEFAULT_DEBUGGER_WIDTH                  0
#define STRING_DEBUGGER_HEIGHT                  "DebuggerFormHeight"
#define DEFAULT_DEBUGGER_HEIGHT                 0
#define STRING_PRINTER_LEFT                     "PrinterFormLeft"
#define DEFAULT_PRINTER_LEFT                    0
#define STRING_PRINTER_TOP                      "PrinterFormTop"
#define DEFAULT_PRINTER_TOP                     0
#define STRING_PRINTER_WIDTH                    "PrinterFormWidth"
#define DEFAULT_PRINTER_WIDTH                   0
#define STRING_PRINTER_HEIGHT                   "PrinterFormHeight"
#define DEFAULT_PRINTER_HEIGHT                  0
#define STRING_GRAPHICSEXPLORER_LEFT            "GraphicsExplorerFormLeft"
#define DEFAULT_GRAPHICSEXPLORER_LEFT           0
#define STRING_GRAPHICSEXPLORER_TOP             "GraphicsExplorerFormTop"
#define DEFAULT_GRAPHICSEXPLORER_TOP            0
#define STRING_GRAPHICSEXPLORER_WIDTH           "GraphicsExplorerFormWidth"
#define DEFAULT_GRAPHICSEXPLORER_WIDTH          0
#define STRING_GRAPHICSEXPLORER_HEIGHT          "GraphicsExplorerFormHeight"
#define DEFAULT_GRAPHICSEXPLORER_HEIGHT         0

// Section Snapshot
#define STRING_SNAPSHOT_DIRECTORY               "SnapshotDirectory"
#define DEFAULT_SNAPSHOT_DIRECTORY              "Snapshots\\"

// Section Screenshot
#define STRING_SCREENSHOT_DIRECTORY             "ScreenshotDirectory"
#define DEFAULT_SCREENSHOT_DIRECTORY            "Screenshots\\"

// Section Cheat scripts
#define STRING_CHEATSCRIPTS_DIRECTORY           "CheatScriptsDirectory"
#define DEFAULT_CHEATSCRIPTS_DIRECTORY          "Scripts\\"

// Section ASCII Keyboard
#define STRING_ASCIIKEY                         "ASCIIKey"

// Section Hardware Keyboard
#define STRING_HARDWAREKEY                      "HardwareKey"

// Section Joystick Keyboard
#define STRING_JOYSTICKKEY                      "JoystickKey"

// Section Printer
#define STRING_COUNTRY_CODE                     "CountryCode"
#define DEFAULT_COUNTRY_CODE                    0
#define STRING_CRLF                             "CRLF"
#define DEFAULT_CRLF                            0



int __fastcall TSettings::GetCPCDevice(void)
{
int Value = -1;

	for (int Loop=0; Loop < (int)NB_DEVICES; Loop++)
  {
    if (m_CPCDevice == Devices[Loop])
    {
      Value = Loop;
      break;
    }
  }

  return Value;
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetCPCDevice(const int Value)
{
  if (m_CPCDevice != Devices[Value])
  {
    m_Updated = true;
    m_CPCDevice = Devices[Value];
  }
}
//------------------------------------------------------------------------------
int __fastcall TSettings::GetCPCBrand(void)
{
int Value = -1;

  for (int Loop=0; Loop < (int)NB_BRANDS; Loop++)
  {
    if (m_CPCBrand == Brands[Loop])
    {
      Value = Loop;
      break;
    }
  }

  return Value;
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetCPCBrand(const int Value)
{
  if (m_CPCBrand != Brands[Value])
  {
    m_Updated = true;
    m_CPCBrand = Brands[Value];
  }
}
//------------------------------------------------------------------------------
AnsiString __fastcall TSettings::GetUpperROMFilename(int Index)
{
AnsiString Filename = DEFAULT_ROMS_FILENAME;

  if (Index < m_UpperROMFilenames->Count)
  {
    Filename = m_UpperROMFilenames->Strings[Index];
  }

  return Filename;
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetUpperROMFilename(int Index, const AnsiString Value)
{
  if (Index < m_UpperROMFilenames->Count)
  {
    if (Value != m_UpperROMFilenames->Strings[Index])
    {
      m_Updated = true;
      m_UpperROMFilenames->Strings[Index] = Value;
    }
  }
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetRomsDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
    m_INIRomsDirectory = Value;
    m_RomsDirectory = Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSettings::SetDiscDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
		m_INIDiscDirectory = Value;
    m_DiscDirectory = Value;
	}
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetProfileDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
		m_INIProfileDirectory = Value;
    m_ProfileDirectory = Value;
	}
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetTapeDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
		m_INITapeDirectory = Value;
    m_TapeDirectory = Value;
	}
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetSnapshotDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
		m_INISnapshotDirectory = Value;
    m_SnapshotDirectory = Value;
	}
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetScreenshotDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
		m_INIScreenshotDirectory = Value;
    m_ScreenshotDirectory = Value;
	}
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetCheatScriptsDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
		m_INICheatScriptsDirectory = Value;
    m_CheatScriptsDirectory = Value;
	}
}
//------------------------------------------------------------------------------
void __fastcall TSettings::SetScanlinesDirectory(const AnsiString Value)
{
	if (Value != NULL)
	{
    m_Updated = true;
		m_INIScanlinesDirectory = Value;
    m_ScanlinesDirectory = Value;
	}
}
//------------------------------------------------------------------------------


__fastcall TSettings::TSettings(AnsiString Filename)
{
  m_Updated = false;
  m_Filename = Filename;

  // Filenames and folders
  m_DriveADiskFilename = DEFAULT_DRIVEA_DISKFILENAME;
  m_DriveBDiskFilename = DEFAULT_DRIVEB_DISKFILENAME;
  m_TapeFilename = DEFAULT_TAPE_FILENAME;
  m_INIProfileDirectory = DEFAULT_PROFILE_DIRECTORY;
  m_DefaultProfileFilename = DEFAULT_PROFILE_DEFAULT;
  m_INIRomsDirectory = DEFAULT_ROMS_DIRECTORY;
  m_LowerROMFilename = DEFAULT_ROMS_FILENAME;
  m_UpperROMFilenames = new TStringList;
  for (int loop=0; loop < NB_UPPERROMS; loop++)
  {
    m_UpperROMFilenames->Add(DEFAULT_ROMS_FILENAME);
  }
  m_INIDiscDirectory = DEFAULT_DISC_DIRECTORY;
  m_INITapeDirectory = DEFAULT_TAPE_DIRECTORY;
  m_INISnapshotDirectory = DEFAULT_SNAPSHOT_DIRECTORY;
  m_INIScreenshotDirectory = DEFAULT_SCREENSHOT_DIRECTORY;
  m_INICheatScriptsDirectory = DEFAULT_CHEATSCRIPTS_DIRECTORY;
  m_INIScanlinesDirectory = DEFAULT_MAINFORM_SCANLINES_DIRECTORY;
  m_ConfirmDiskImageUpdates = DEFAULT_DISC_CONFIRMDISKIMAGEUPDATES;
  m_NeverUpdateDiskImages = DEFAULT_DISC_NEVERUPDATEDISKIMAGES;
  // Graphics
  m_UseOpenGL = DEFAULT_MAINFORM_USEOPENGL;
  m_ForceGDI = DEFAULT_MAINFORM_FORCEGDI;
  m_OnscreenDisplay = DEFAULT_MAINFORM_ONSCREENDISPLAY;
  m_Fullscreen = DEFAULT_MAINFORM_FULLSCREEN;
  m_KeepFullscreen = DEFAULT_MAINFORM_KEEPFULLSCREEN;
  m_WindowStyle = DEFAULT_MAINFORM_WINDOWSTYLE;
  m_WindowExStyle = DEFAULT_MAINFORM_WINDOWEXSTYLE;
  m_DrawScanlines = DEFAULT_MAINFORM_SCANLINES;
  m_DriveIconSizeFactor = DEFAULT_DRIVE_ICON_SIZE_FACTOR;
  m_ShowStaticDriveIcon = DEFAULT_SHOW_STATIC_DRIVE_ICON;
  m_ControlPanelVisible = DEFAULT_MAINFORM_CONTROL_PANEL_VISIBLE;
  m_ColorMonitor = DEFAULT_EMULATOR_COLORMONITOR;
  m_Mode2Antialiasing = DEFAULT_EMULATOR_MODE2ALIASING;
  m_FullscreenBrightness = DEFAULT_MAINFORM_FULLSCREEN_BRIGHTNESS;
  m_FullscreenMenu = DEFAULT_MAINFORM_FULLSCREEN_MENU;
  m_KeepAspectRatio = DEFAULT_MAINFORM_KEEPASPECTRATIO;
  m_ShowPauseInfo = DEFAULT_MAINFORM_SHOWPAUSEINFO;
  // Emulator
  m_AutoStartEnable = DEFAULT_AUTOSTART_ENABLE;
  m_AutoPlayEnable = DEFAULT_AUTOPLAY_ENABLE;
  m_CPCDevice = DEFAULT_EMULATOR_DEVICE;
  m_CPCBrand = DEFAULT_EMULATOR_BRAND;
  m_CRTCType = DEFAULT_EMULATOR_CRTC_TYPE;
  m_PreferredKeyboardLayout = DEFAULT_EMULATOR_PREFKEYBLAYOUT;
  m_UseFloppyDrive = DEFAULT_EMULATOR_FLOPPYDRIVE;
  m_UseFastFDC = DEFAULT_EMULATOR_FASTFDC;
  m_Use64kMemoryExtension = DEFAULT_EMULATOR_64KMEMEXT;
  m_Use256kMemoryExtension = DEFAULT_EMULATOR_256KMEMEXT;
  m_Use256kSiliconDisc = DEFAULT_EMULATOR_256KSILICONDISC;
  m_UseTechniMusique = DEFAULT_EMULATOR_TECHNIMUSIQUE;
  m_UsePlayCity = DEFAULT_EMULATOR_PLAYCITY;
  m_MaxSpeedIndex = DEFAULT_EMULATOR_MAXSPEEDINDEX;
  m_MaxCustomSpeed = DEFAULT_EMULATOR_MAXCUSTOMSPEED;
  m_WindowsPriority = DEFAULT_EMULATOR_WINDOWSPRIORITY;
  m_ColdResetAtRealSpeed = DEFAULT_EMULATOR_COLDRESETATREALSPEED;
  m_SmallScreenshotShortcut = DEFAULT_EMULATOR_SMALLSCREENSHOTSHORTCUT;
  m_VHoldAdjust = DEFAULT_EMULATOR_VHOLD_ADJUST;
  // Inputs
  m_UseHardwareKeyboardMapping = DEFAULT_USEHARDWAREKEYBOARDMAP;
  m_NonASCIIKeysList = new TList;
  RestoreDefaultNonASCIIKeys();
  m_HardwareKeysList = new TList;
  RestoreDefaultHardwareKeys();
  m_JoystickKeysList = new TList;
  RestoreDefaultJoystickKeys();
  m_AMXMouseFactorX = DEFAULT_AMXMOUSE_FACTORX;
  m_AMXMouseFactorY = DEFAULT_AMXMOUSE_FACTORY;
  // Audio
  m_Mute = DEFAULT_AUDIO_MUTE;
  m_AudioGainSpeaker = DEFAULT_GAIN_SPEAKER;
  m_AudioGainFloppy = DEFAULT_GAIN_FLOPPY;
  m_AudioGainTape = DEFAULT_GAIN_TAPE;
  m_AudioGainTMPI = DEFAULT_GAIN_TMPI;
  m_AudioGainPlayCity = DEFAULT_GAIN_PLAYCITY;
  m_AudioVolume = DEFAULT_VOLUME;
  m_AudioRenderer = DEFAULT_AUDIO_RENDERER;
  m_AudioSpeakerFreq = SND_FREQ;
  m_AudioSpeakerStereo = DEFAULT_SPEAKER_STEREO;
  m_AudioSpeaker16Bits = SND_16BITS;
  m_AudioTMPIFreq = SND_FREQ;
  m_AudioPlayCityFreq = SND_FREQ;
  m_AudioPlayCityStereo = DEFAULT_PLAYCITY_STEREO;
  m_AudioPlayCity16Bits = SND_16BITS;
  m_AudioPlayCityInvert = DEFAULT_PLAYCITY_INVERT;
  // Forms position
  m_MainFormLeft = DEFAULT_MAINFORM_LEFT;
  m_MainFormTop = DEFAULT_MAINFORM_TOP;
  m_MainFormWidth = DEFAULT_MAINFORM_WIDTH;
  m_MainFormHeight = DEFAULT_MAINFORM_HEIGHT;
  m_DSKViewerFormLeft = DEFAULT_DSKVIEWER_LEFT;
  m_DSKViewerFormTop = DEFAULT_DSKVIEWER_TOP;
  m_DSKViewerFormWidth = DEFAULT_DSKVIEWER_WIDTH;
  m_DSKViewerFormHeight = DEFAULT_DSKVIEWER_HEIGHT;
  m_CRTCFormLeft = DEFAULT_CRTC_LEFT;
  m_CRTCFormTop = DEFAULT_CRTC_TOP;
  m_CRTCFormWidth = DEFAULT_CRTC_WIDTH;
  m_CRTCFormHeight = DEFAULT_CRTC_HEIGHT;
  m_GateArrayFormLeft = DEFAULT_GATEARRAY_LEFT;
  m_GateArrayFormTop = DEFAULT_GATEARRAY_TOP;
  m_GateArrayFormWidth = DEFAULT_GATEARRAY_WIDTH;
  m_GateArrayFormHeight = DEFAULT_GATEARRAY_HEIGHT;
  m_FDCFormLeft = DEFAULT_FDC_LEFT;
  m_FDCFormTop = DEFAULT_FDC_TOP;
  m_FDCFormWidth = DEFAULT_FDC_WIDTH;
  m_FDCFormHeight = DEFAULT_FDC_HEIGHT;
  m_Z80FormLeft = DEFAULT_Z80_LEFT;
  m_Z80FormTop = DEFAULT_Z80_TOP;
  m_Z80FormWidth = DEFAULT_Z80_WIDTH;
  m_Z80FormHeight = DEFAULT_Z80_HEIGHT;
  m_MemoryFormLeft = DEFAULT_MEMORY_LEFT;
  m_MemoryFormTop = DEFAULT_MEMORY_TOP;
  m_MemoryFormWidth = DEFAULT_MEMORY_WIDTH;
  m_MemoryFormHeight = DEFAULT_MEMORY_HEIGHT;
  m_PSGFormLeft = DEFAULT_PSG_LEFT;
  m_PSGFormTop = DEFAULT_PSG_TOP;
  m_PSGFormWidth = DEFAULT_PSG_WIDTH;
  m_PSGFormHeight = DEFAULT_PSG_HEIGHT;
  m_CDTViewerFormLeft = DEFAULT_CDTVIEWER_LEFT;
  m_CDTViewerFormTop = DEFAULT_CDTVIEWER_TOP;
  m_CDTViewerFormWidth = DEFAULT_CDTVIEWER_WIDTH;
  m_CDTViewerFormHeight = DEFAULT_CDTVIEWER_HEIGHT;
  m_TapeAudioWorkshopFormLeft = DEFAULT_TAPEAUDIOWORKSHOP_LEFT;
  m_TapeAudioWorkshopFormTop = DEFAULT_TAPEAUDIOWORKSHOP_TOP;
  m_MEA8000FormLeft = DEFAULT_MEA8000_LEFT;
  m_MEA8000FormTop = DEFAULT_MEA8000_TOP;
  m_MEA8000FormWidth = DEFAULT_MEA8000_WIDTH;
  m_MEA8000FormHeight = DEFAULT_MEA8000_HEIGHT;
  m_DebuggerFormLeft = DEFAULT_DEBUGGER_LEFT;
  m_DebuggerFormTop = DEFAULT_DEBUGGER_TOP;
  m_DebuggerFormWidth = DEFAULT_DEBUGGER_WIDTH;
  m_DebuggerFormHeight = DEFAULT_DEBUGGER_HEIGHT;
  m_PrinterFormLeft = DEFAULT_PRINTER_LEFT;
  m_PrinterFormTop = DEFAULT_PRINTER_TOP;
  m_PrinterFormWidth = DEFAULT_PRINTER_WIDTH;
  m_PrinterFormHeight = DEFAULT_PRINTER_HEIGHT;
  m_GraphicsExplorerFormLeft = DEFAULT_GRAPHICSEXPLORER_LEFT;
  m_GraphicsExplorerFormTop = DEFAULT_GRAPHICSEXPLORER_TOP;
  m_GraphicsExplorerFormWidth = DEFAULT_GRAPHICSEXPLORER_WIDTH;
  m_GraphicsExplorerFormHeight = DEFAULT_GRAPHICSEXPLORER_HEIGHT;
  // Tape audio workshop
  m_TapeAudioUseDefault = DEFAULT_TAPE_AUDIO_USEDEFAULT;
  m_TapeAudioPPIThreshold8 = DEFAULT_TAPE_PPITHRESHOLD8;
  m_TapeAudioPPIThreshold16 = DEFAULT_TAPE_PPITHRESHOLD16;
  m_TapeAudioAutoGainActive = DEFAULT_TAPE_AUTOGAIN_ACTIVE;
  m_TapeAudioLPF1Active = DEFAULT_TAPE_LPF1_ACTIVE; 
  m_TapeAudioLPF1_A = DEFAULT_TAPE_LPF1_A;
  m_TapeAudioLPF2Active = DEFAULT_TAPE_LPF2_ACTIVE;
  m_TapeAudioLPF2Order = DEFAULT_TAPE_LPF2_ORDER;
  m_TapeAudioLPF2Freq = DEFAULT_TAPE_LPF2_FREQ;
  m_TapeAudioHPF2Active = DEFAULT_TAPE_HPF2_ACTIVE;
  m_TapeAudioHPF2Order = DEFAULT_TAPE_HPF2_ORDER;
  m_TapeAudioHPF2Freq = DEFAULT_TAPE_HPF2_FREQ;
  m_TapeAudioCreateCSW = DEFAULT_TAPE_CREATE_CSW;
  // Printer
  m_PrinterCountryCode = DEFAULT_COUNTRY_CODE;
  m_PrinterCRLF = DEFAULT_CRLF;

  // Read settings from file.
  Read(m_Filename);

  // First settings read
  m_Updated = false;
}
//---------------------------------------------------------------------------


__fastcall TSettings::~TSettings(void)
{
  ClearNonASCIIKeys();
  delete m_NonASCIIKeysList;
  ClearHardwareKeys();
  delete m_HardwareKeysList;
  ClearJoystickKeys();
  delete m_JoystickKeysList;

  delete m_UpperROMFilenames;
}
//---------------------------------------------------------------------------


TStringList* __fastcall TSettings::GetInfo(void)
{
  TStringList* InfoStrings = new TStringList;

  if (FileExists(m_Filename) == true)
  {
    // Add "Caprice.ini" content
    InfoStrings->LoadFromFile(m_Filename);
  }
  else
  {
    InfoStrings->Add("No settings file !!");
  }

  InfoStrings->Insert(0, "========");
  InfoStrings->Insert(0, "Settings");

  // Add folders
  InfoStrings->Add("");
  InfoStrings->Add("Directories");
  InfoStrings->Add("===========");
  InfoStrings->Add("Profile= " + m_ProfileDirectory);
  InfoStrings->Add("Roms= " + m_RomsDirectory);
  InfoStrings->Add("Disc= " + m_DiscDirectory);
  InfoStrings->Add("Tape= " + m_TapeDirectory);
  InfoStrings->Add("Snapshot= " + m_SnapshotDirectory);
  InfoStrings->Add("Screenshot= " + m_ScreenshotDirectory);
  InfoStrings->Add("CheatScripts= " + m_CheatScriptsDirectory);
  InfoStrings->Add("Scanlines= " + m_ScanlinesDirectory);

  return InfoStrings;
}
//---------------------------------------------------------------------------


void __fastcall TSettings::Read(AnsiString Filename)
{
  if (FileExists(Filename) == true)
  {
    TIniFile *FileP = new TIniFile(Filename);

    // Read main form settings
    m_MainFormLeft = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_LEFT, m_MainFormLeft);
    m_MainFormTop = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_TOP, m_MainFormTop);
    m_MainFormWidth = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_WIDTH, m_MainFormWidth);
    m_MainFormHeight = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_HEIGHT, m_MainFormHeight);
    m_UseOpenGL = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_USEOPENGL, m_UseOpenGL);
    m_ForceGDI = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_FORCEGDI, m_ForceGDI);
    m_OnscreenDisplay = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_ONSCREENDISPLAY, m_OnscreenDisplay);
    m_Fullscreen = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_FULLSCREEN, m_Fullscreen);
    m_KeepFullscreen = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_KEEPFULLSCREEN, m_KeepFullscreen);
    m_WindowStyle = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_WINDOWSTYLE, m_WindowStyle);
    m_WindowExStyle = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_WINDOWEXSTYLE, m_WindowExStyle);
    m_DrawScanlines = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_SCANLINES, m_DrawScanlines);
    m_INIDiscDirectory = FileP->ReadString(STRING_MAINFORM, STRING_DISC_DIRECTORY, m_INIDiscDirectory);
    m_INIScanlinesDirectory = FileP->ReadString(STRING_MAINFORM, STRING_MAINFORM_SCANLINES_DIRECTORY, m_INIScanlinesDirectory);
    m_ControlPanelVisible = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_CONTROL_PANEL_VISIBLE, m_ControlPanelVisible);
    m_FullscreenBrightness = FileP->ReadInteger(STRING_MAINFORM, STRING_MAINFORM_FULLSCREEN_BRIGHTNESS, m_FullscreenBrightness);
    m_FullscreenMenu = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_FULLSCREEN_MENU, m_FullscreenMenu);
    m_KeepAspectRatio = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_KEEPASPECTRATIO, m_KeepAspectRatio);
    m_ShowPauseInfo = FileP->ReadBool(STRING_MAINFORM, STRING_MAINFORM_SHOWPAUSEINFO, m_ShowPauseInfo);
    // Read drive disks
    m_DriveADiskFilename = FileP->ReadString(STRING_DRIVES, STRING_DRIVEA_DISKFILENAME, m_DriveADiskFilename);
    m_DriveBDiskFilename = FileP->ReadString(STRING_DRIVES, STRING_DRIVEB_DISKFILENAME, m_DriveBDiskFilename);
    m_AutoStartEnable = FileP->ReadBool(STRING_DRIVES, STRING_AUTOSTART_ENABLE, m_AutoStartEnable);
    m_DriveIconSizeFactor = FileP->ReadInteger(STRING_DRIVES, STRING_DRIVE_ICON_SIZE_FACTOR, m_DriveIconSizeFactor);
    m_ShowStaticDriveIcon = FileP->ReadBool(STRING_DRIVES, STRING_SHOW_STATIC_DRIVE_ICON, m_ShowStaticDriveIcon);
    m_INIDiscDirectory = FileP->ReadString(STRING_DRIVES, STRING_DISC_DIRECTORY, m_INIDiscDirectory);
    m_ConfirmDiskImageUpdates = FileP->ReadBool(STRING_DRIVES, STRING_DISC_CONFIRMDISKIMAGEUPDATES, m_ConfirmDiskImageUpdates);
    m_NeverUpdateDiskImages = FileP->ReadBool(STRING_DRIVES, STRING_DISC_NEVERUPDATEDISKIMAGES, m_NeverUpdateDiskImages);
    // Tape
    m_TapeFilename = FileP->ReadString(STRING_TAPE, STRING_TAPE_FILENAME, m_TapeFilename);
    m_AutoPlayEnable = FileP->ReadBool(STRING_TAPE, STRING_AUTOPLAY_ENABLE, m_AutoPlayEnable);
    m_INITapeDirectory = FileP->ReadString(STRING_TAPE, STRING_TAPE_DIRECTORY, m_INITapeDirectory);
    // Emulator
    m_CPCDevice = FileP->ReadString(STRING_EMULATOR, STRING_EMULATOR_DEVICE, m_CPCDevice);
    m_CPCBrand = FileP->ReadString(STRING_EMULATOR, STRING_EMULATOR_BRAND, m_CPCBrand);
    m_CRTCType = FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_CRTC_TYPE, m_CRTCType);
    m_PreferredKeyboardLayout = FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_PREFKEYBLAYOUT, m_PreferredKeyboardLayout);
    m_UseFloppyDrive = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_FLOPPYDRIVE, m_UseFloppyDrive);
    m_UseFastFDC = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_FASTFDC, m_UseFastFDC);
    m_Use64kMemoryExtension = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_64KMEMEXT, m_Use64kMemoryExtension);
    m_Use256kMemoryExtension = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_256KMEMEXT, m_Use256kMemoryExtension);
    m_Use256kSiliconDisc = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_256KSILICONDISC, m_Use256kSiliconDisc);
    m_UseTechniMusique = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_TECHNIMUSIQUE, m_UseTechniMusique);
    m_UsePlayCity = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_PLAYCITY, m_UsePlayCity);
    m_Mode2Antialiasing = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_MODE2ALIASING, m_Mode2Antialiasing);
    m_ColorMonitor = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_COLORMONITOR, m_ColorMonitor);
    m_MaxSpeedIndex = FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_MAXSPEEDINDEX, m_MaxSpeedIndex);
    m_MaxCustomSpeed = FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_MAXCUSTOMSPEED, m_MaxCustomSpeed);
    m_WindowsPriority = FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_WINDOWSPRIORITY, m_WindowsPriority);
    m_ColdResetAtRealSpeed = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_COLDRESETATREALSPEED, m_ColdResetAtRealSpeed);
    m_SmallScreenshotShortcut = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_SMALLSCREENSHOTSHORTCUT, m_SmallScreenshotShortcut);
    m_ThemeIndex = FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_INDEX, m_ThemeIndex);
    m_ThemeBackgroundColor = (TColor)FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_BACKGROUND, m_ThemeBackgroundColor);
    m_ThemeText1Color = (TColor)FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_TEXT1, m_ThemeText1Color);
    m_ThemeText2Color = (TColor)FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_TEXT2, m_ThemeText2Color);
    m_ShowHints = FileP->ReadBool(STRING_EMULATOR, STRING_EMULATOR_SHOWHINTS, m_ShowHints);
    m_VHoldAdjust = FileP->ReadInteger(STRING_EMULATOR, STRING_EMULATOR_VHOLD_ADJUST, m_VHoldAdjust);
    // Inputs
    m_INIProfileDirectory = FileP->ReadString(STRING_INPUTS, STRING_PROFILE_DIRECTORY, m_INIProfileDirectory);
    m_DefaultProfileFilename = FileP->ReadString(STRING_INPUTS, STRING_PROFILE_DEFAULT, m_DefaultProfileFilename);
    m_UseHardwareKeyboardMapping = FileP->ReadBool(STRING_INPUTS, STRING_USEHARDWAREKEYBOARDMAP, m_UseHardwareKeyboardMapping);
    m_AMXMouseFactorX = FileP->ReadInteger(STRING_INPUTS, STRING_AMXMOUSE_FACTORX, m_AMXMouseFactorX);
    m_AMXMouseFactorY = FileP->ReadInteger(STRING_INPUTS, STRING_AMXMOUSE_FACTORY, m_AMXMouseFactorY);
    // Snapshot
    m_INISnapshotDirectory = FileP->ReadString(STRING_SNAPSHOT, STRING_SNAPSHOT_DIRECTORY, m_INISnapshotDirectory);
    // Screenshot
    m_INIScreenshotDirectory = FileP->ReadString(STRING_SCREENSHOT, STRING_SCREENSHOT_DIRECTORY, m_INIScreenshotDirectory);
    // Cheat scripts
    m_INICheatScriptsDirectory = FileP->ReadString(STRING_CHEATSCRIPTS, STRING_CHEATSCRIPTS_DIRECTORY, m_INICheatScriptsDirectory);
    // Audio
    m_Mute = FileP->ReadBool(STRING_AUDIO, STRING_AUDIO_MUTE, m_Mute);
    m_AudioGainSpeaker = FileP->ReadInteger(STRING_AUDIO, STRING_GAIN_SPEAKER, m_AudioGainSpeaker);
    m_AudioGainFloppy = FileP->ReadInteger(STRING_AUDIO, STRING_GAIN_FLOPPY, m_AudioGainFloppy);
    m_AudioGainTape = FileP->ReadInteger(STRING_AUDIO, STRING_GAIN_TAPE, m_AudioGainTape);
    m_AudioGainTMPI = FileP->ReadInteger(STRING_AUDIO, STRING_GAIN_TMPI, m_AudioGainTMPI);
    m_AudioGainPlayCity = FileP->ReadInteger(STRING_AUDIO, STRING_GAIN_PLAYCITY, m_AudioGainPlayCity);
    m_AudioVolume = FileP->ReadInteger(STRING_AUDIO, STRING_VOLUME, m_AudioVolume);
    m_AudioRenderer = FileP->ReadString(STRING_AUDIO, STRING_AUDIO_RENDERER, m_AudioRenderer);
    m_AudioSpeakerStereo = FileP->ReadBool(STRING_AUDIO, STRING_SPEAKER_STEREO, m_AudioSpeakerStereo);
    m_AudioPlayCityStereo = FileP->ReadBool(STRING_AUDIO, STRING_PLAYCITY_STEREO, m_AudioPlayCityStereo);
    m_AudioPlayCityInvert = FileP->ReadBool(STRING_AUDIO, STRING_PLAYCITY_INVERT, m_AudioPlayCityInvert);
    m_TapeAudioUseDefault = FileP->ReadBool(STRING_AUDIO, STRING_TAPE_AUDIO_USEDEFAULT, m_TapeAudioUseDefault);
    m_TapeAudioPPIThreshold8 = (tUChar)FileP->ReadInteger(STRING_AUDIO, STRING_TAPE_PPITHRESHOLD8, m_TapeAudioPPIThreshold8);
    m_TapeAudioPPIThreshold16 = (tShort)FileP->ReadInteger(STRING_AUDIO, STRING_TAPE_PPITHRESHOLD16, m_TapeAudioPPIThreshold16);
    m_TapeAudioAutoGainActive = FileP->ReadBool(STRING_AUDIO, STRING_TAPE_AUTOGAIN_ACTIVE, m_TapeAudioAutoGainActive);
    m_TapeAudioLPF1Active = FileP->ReadBool(STRING_AUDIO, STRING_TAPE_LPF1_ACTIVE, m_TapeAudioLPF1Active);
    m_TapeAudioLPF1_A = FileP->ReadString(STRING_AUDIO, STRING_TAPE_LPF1_A, m_TapeAudioLPF1_A);
    m_TapeAudioLPF2Active = FileP->ReadBool(STRING_AUDIO, STRING_TAPE_LPF2_ACTIVE, m_TapeAudioLPF2Active);
    m_TapeAudioLPF2Order = (tUChar)FileP->ReadInteger(STRING_AUDIO, STRING_TAPE_LPF2_ORDER, m_TapeAudioLPF2Order);
    m_TapeAudioLPF2Freq = (tUShort)FileP->ReadInteger(STRING_AUDIO, STRING_TAPE_LPF2_FREQ, m_TapeAudioLPF2Freq);
    m_TapeAudioHPF2Active = FileP->ReadBool(STRING_AUDIO, STRING_TAPE_HPF2_ACTIVE, m_TapeAudioHPF2Active);
    m_TapeAudioHPF2Order = (tUChar)FileP->ReadInteger(STRING_AUDIO, STRING_TAPE_HPF2_ORDER, m_TapeAudioHPF2Order);
    m_TapeAudioHPF2Freq = (tUShort)FileP->ReadInteger(STRING_AUDIO, STRING_TAPE_HPF2_FREQ, m_TapeAudioHPF2Freq);
    m_TapeAudioCreateCSW = FileP->ReadBool(STRING_AUDIO, STRING_TAPE_CREATE_CSW, m_TapeAudioCreateCSW);
    // Windows
    m_DSKViewerFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_DSKVIEWER_LEFT, m_DSKViewerFormLeft);
    m_DSKViewerFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_DSKVIEWER_TOP, m_DSKViewerFormTop);
    m_DSKViewerFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_DSKVIEWER_WIDTH, m_DSKViewerFormWidth);
    m_DSKViewerFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_DSKVIEWER_HEIGHT, m_DSKViewerFormHeight);
    m_CRTCFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_CRTC_LEFT, m_CRTCFormLeft);
    m_CRTCFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_CRTC_TOP, m_CRTCFormTop);
    m_CRTCFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_CRTC_WIDTH, m_CRTCFormWidth);
    m_CRTCFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_CRTC_HEIGHT, m_CRTCFormHeight);
    m_GateArrayFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_GATEARRAY_LEFT, m_GateArrayFormLeft);
    m_GateArrayFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_GATEARRAY_TOP, m_GateArrayFormTop);
    m_GateArrayFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_GATEARRAY_WIDTH, m_GateArrayFormWidth);
    m_GateArrayFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_GATEARRAY_HEIGHT, m_GateArrayFormHeight);
    m_FDCFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_FDC_LEFT, m_FDCFormLeft);
    m_FDCFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_FDC_TOP, m_FDCFormTop);
    m_FDCFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_FDC_WIDTH, m_FDCFormWidth);
    m_FDCFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_FDC_HEIGHT, m_FDCFormHeight);
    m_Z80FormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_Z80_LEFT, m_Z80FormLeft);
    m_Z80FormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_Z80_TOP, m_Z80FormTop);
    m_Z80FormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_Z80_WIDTH, m_Z80FormWidth);
    m_Z80FormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_Z80_HEIGHT, m_Z80FormHeight);
    m_MemoryFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_MEMORY_LEFT, m_MemoryFormLeft);
    m_MemoryFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_MEMORY_TOP, m_MemoryFormTop);
    m_MemoryFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_MEMORY_WIDTH, m_MemoryFormWidth);
    m_MemoryFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_MEMORY_HEIGHT, m_MemoryFormHeight);
    m_PSGFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_PSG_LEFT, m_PSGFormLeft);
    m_PSGFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_PSG_TOP, m_PSGFormTop);
    m_PSGFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_PSG_WIDTH, m_PSGFormWidth);
    m_PSGFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_PSG_HEIGHT, m_PSGFormHeight);
    m_CDTViewerFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_CDTVIEWER_LEFT, m_CDTViewerFormLeft);
    m_CDTViewerFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_CDTVIEWER_TOP, m_CDTViewerFormTop);
    m_CDTViewerFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_CDTVIEWER_WIDTH, m_CDTViewerFormWidth);
    m_CDTViewerFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_CDTVIEWER_HEIGHT, m_CDTViewerFormHeight);
    m_TapeAudioWorkshopFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_TAPEAUDIOWORKSHOP_LEFT, m_TapeAudioWorkshopFormLeft);
    m_TapeAudioWorkshopFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_TAPEAUDIOWORKSHOP_TOP, m_TapeAudioWorkshopFormTop);
    m_MEA8000FormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_MEA8000_LEFT, m_MEA8000FormLeft);
    m_MEA8000FormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_MEA8000_TOP, m_MEA8000FormTop);
    m_MEA8000FormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_MEA8000_WIDTH, m_MEA8000FormWidth);
    m_MEA8000FormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_MEA8000_HEIGHT, m_MEA8000FormHeight);
    m_DebuggerFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_DEBUGGER_LEFT, m_DebuggerFormLeft);
    m_DebuggerFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_DEBUGGER_TOP, m_DebuggerFormTop);
    m_DebuggerFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_DEBUGGER_WIDTH, m_DebuggerFormWidth);
    m_DebuggerFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_DEBUGGER_HEIGHT, m_DebuggerFormHeight);
    m_PrinterFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_PRINTER_LEFT, m_PrinterFormLeft);
    m_PrinterFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_PRINTER_TOP, m_PrinterFormTop);
    m_PrinterFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_PRINTER_WIDTH, m_PrinterFormWidth);
    m_PrinterFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_PRINTER_HEIGHT, m_PrinterFormHeight);
    m_GraphicsExplorerFormLeft = FileP->ReadInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_LEFT, m_GraphicsExplorerFormLeft);
    m_GraphicsExplorerFormTop = FileP->ReadInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_TOP, m_GraphicsExplorerFormTop);
    m_GraphicsExplorerFormWidth = FileP->ReadInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_WIDTH, m_GraphicsExplorerFormWidth);
    m_GraphicsExplorerFormHeight = FileP->ReadInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_HEIGHT, m_GraphicsExplorerFormHeight);
    // Printer
    m_PrinterCountryCode = FileP->ReadInteger(STRING_PRINTER, STRING_COUNTRY_CODE, m_PrinterCountryCode);
    m_PrinterCRLF = FileP->ReadInteger(STRING_PRINTER, STRING_CRLF, m_PrinterCRLF);

    //
    // ROM Mapping
    //
    m_INIRomsDirectory = FileP->ReadString(STRING_ROMS, STRING_ROMS_DIRECTORY, m_INIRomsDirectory);
    if (FileP->SectionExists(STRING_ROMS) == true)
    {
      TStringList* Keys = new TStringList;
      FileP->ReadSectionValues(STRING_ROMS, Keys);

      AnsiString KeyUpperROM = STRING_ROMS_UPPERROM;
      for (int loop=0; loop < Keys->Count; loop++)
      {
        AnsiString Key = Keys->Strings[loop];

        // Lower ROM
        if (Key.Pos(STRING_ROMS_LOWERROM) == 1)
        {
          m_LowerROMFilename = Key.SubString(Key.Pos("=")+1, Key.Length());
        }
        // Upper ROM
        else if (Key.Pos(KeyUpperROM) == 1)
        {
          int Index = Key.SubString(KeyUpperROM.Length()+1,
                                    Key.Pos("=") - (KeyUpperROM.Length()+1)).ToInt();
          m_UpperROMFilenames->Strings[Index] = Key.SubString(Key.Pos("=")+1, Key.Length());
        }
      }

      delete Keys;
    }
    // ASCII Keyboard
    if (FileP->SectionExists(STRING_ASCIIKEYBOARD) == true)
    {
      TStringList* Keys = new TStringList;
      FileP->ReadSectionValues(STRING_ASCIIKEYBOARD, Keys);

      AnsiString KeyID = STRING_ASCIIKEY;
      for (int loop=0; loop < Keys->Count; loop++)
      {
        if (Keys->Strings[loop].Pos(KeyID) > 0)
        {
          tKeyboardMappingKey* KeyP = new tKeyboardMappingKey;
          KeyP->WindowsKey = (tUShort)Keys->Strings[loop].SubString(KeyID.Length() + 1,
                                                                    Keys->Strings[loop].Pos("=") - (KeyID.Length()+1)).ToInt();
          KeyP->CPCKey = (tUChar)Keys->Strings[loop].SubString(Keys->Strings[loop].Pos("=")+1, Keys->Strings[loop].Length()).ToInt();
          AddNonASCIIKey(KeyP);
        }
      }

      delete Keys;
    }
    // Hardware Keyboard
    if (FileP->SectionExists(STRING_HARDWAREKEYBOARD) == true)
    {
      TStringList* Keys = new TStringList;
      FileP->ReadSectionValues(STRING_HARDWAREKEYBOARD, Keys);

      AnsiString KeyID = STRING_HARDWAREKEY;
      for (int loop=0; loop < Keys->Count; loop++)
      {
        if (Keys->Strings[loop].Pos(KeyID) > 0)
        {
          tKeyboardMappingKey* KeyP = new tKeyboardMappingKey;
          KeyP->CPCKey = (tUChar)Keys->Strings[loop].SubString(KeyID.Length()+1,
                                                               Keys->Strings[loop].Pos("=") - (KeyID.Length()+1)).ToInt();
          KeyP->CPCKey = USERMANUAL_TO_CPC_KBD(KeyP->CPCKey);
          KeyP->WindowsKey = (tUShort)Keys->Strings[loop].SubString(Keys->Strings[loop].Pos("=")+1, Keys->Strings[loop].Length()).ToInt();
          AddHardwareKey(KeyP);
        }
      }

      delete Keys;
    }
    // Joystick Keyboard
    if (FileP->SectionExists(STRING_JOYSTICKKEYBOARD) == true)
    {
      TStringList* Keys = new TStringList;
      FileP->ReadSectionValues(STRING_JOYSTICKKEYBOARD, Keys);

      AnsiString KeyID = STRING_JOYSTICKKEY;
      for (int loop=0; loop < Keys->Count; loop++)
      {
        if (Keys->Strings[loop].Pos(KeyID) > 0)
        {
          tKeyboardMappingKey* KeyP = new tKeyboardMappingKey;
          KeyP->CPCKey = (tUChar)Keys->Strings[loop].SubString(KeyID.Length()+1,
                                                                Keys->Strings[loop].Pos("=") - (KeyID.Length()+1)).ToInt();
          KeyP->WindowsKey = (tUShort)Keys->Strings[loop].SubString(Keys->Strings[loop].Pos("=")+1, Keys->Strings[loop].Length()).ToInt();
          AddJoystickKey(KeyP);
        }
      }

      delete Keys;
    }

    m_Updated = true;

    delete FileP;
  }

  //
  // Valid Folders
  //
  AnsiString ApplicationPath = ExtractFilePath(m_Filename);
  m_ProfileDirectory = ExpandFileName(m_INIProfileDirectory);
  if (DirectoryExists(m_ProfileDirectory) == false)
  {
    m_ProfileDirectory = ApplicationPath + DEFAULT_PROFILE_DIRECTORY;
  }
  m_RomsDirectory = ExpandFileName(m_INIRomsDirectory);
  if (DirectoryExists(m_RomsDirectory) == false)
  {
    m_RomsDirectory = ApplicationPath + DEFAULT_ROMS_DIRECTORY;
  }
  m_DiscDirectory = ExpandFileName(m_INIDiscDirectory);
  if (DirectoryExists(m_DiscDirectory) == false)
  {
    m_DiscDirectory = ApplicationPath + DEFAULT_DISC_DIRECTORY;
  }
  m_TapeDirectory = ExpandFileName(m_INITapeDirectory);
  if (DirectoryExists(m_TapeDirectory) == false)
  {
    m_TapeDirectory = ApplicationPath + DEFAULT_TAPE_DIRECTORY;
  }
  m_SnapshotDirectory = ExpandFileName(m_INISnapshotDirectory);
  if (DirectoryExists(m_SnapshotDirectory) == false)
  {
    m_SnapshotDirectory = ApplicationPath + DEFAULT_SNAPSHOT_DIRECTORY;
  }
  m_ScreenshotDirectory = ExpandFileName(m_INIScreenshotDirectory);
  if (DirectoryExists(m_ScreenshotDirectory) == false)
  {
    m_ScreenshotDirectory = ApplicationPath + DEFAULT_SCREENSHOT_DIRECTORY;
  }
  m_CheatScriptsDirectory = ExpandFileName(m_INICheatScriptsDirectory);
  if (DirectoryExists(m_CheatScriptsDirectory) == false)
  {
    m_CheatScriptsDirectory = ApplicationPath + DEFAULT_CHEATSCRIPTS_DIRECTORY;
  }
  m_ScanlinesDirectory = ExpandFileName(m_INIScanlinesDirectory);
  if (DirectoryExists(m_ScanlinesDirectory) == false)
  {
    m_ScanlinesDirectory = ApplicationPath + DEFAULT_MAINFORM_SCANLINES_DIRECTORY;
  }
}
//---------------------------------------------------------------------------


bool __fastcall TSettings::Update(void)
{
bool ToCreate = false;

  if (FileExists(m_Filename) == true)
  {
    if (m_Updated == true)
    {
      // Vérifier l'attribut "lecture seule"
      int Attributes = FileGetAttr(m_Filename);
      if ((Attributes & faReadOnly) == 0)
      {
        ToCreate = true;
      }
    }
  }
  else
  {
    ToCreate = true;
  }

  if (ToCreate == true)
  {
    TIniFile *FileP = new TIniFile(m_Filename);

    // Write main form settings
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_LEFT,                  m_MainFormLeft);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_TOP,                   m_MainFormTop);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_WIDTH,                 m_MainFormWidth);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_HEIGHT,                m_MainFormHeight);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_USEOPENGL,             m_UseOpenGL == true ? 1 : 0);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_FORCEGDI,              m_ForceGDI == true ? 1 : 0);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_ONSCREENDISPLAY,       m_OnscreenDisplay);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_SCANLINES,             m_DrawScanlines == true ? 1 : 0);
    FileP->WriteString(STRING_MAINFORM,  STRING_MAINFORM_SCANLINES_DIRECTORY,   m_INIScanlinesDirectory);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_FULLSCREEN,            m_Fullscreen == true ? 1 : 0);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_KEEPFULLSCREEN,        m_KeepFullscreen == true ? 1 : 0);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_WINDOWSTYLE,           m_WindowStyle);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_WINDOWEXSTYLE,         m_WindowExStyle);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_CONTROL_PANEL_VISIBLE, m_ControlPanelVisible == true ? 1 : 0);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_FULLSCREEN_BRIGHTNESS, m_FullscreenBrightness);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_FULLSCREEN_MENU,       m_FullscreenMenu == true ? 1 : 0);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_KEEPASPECTRATIO,       m_KeepAspectRatio == true ? 1 : 0);
    FileP->WriteInteger(STRING_MAINFORM, STRING_MAINFORM_SHOWPAUSEINFO,         m_ShowPauseInfo == true ? 1 : 0);
    // Write drive disks
    FileP->WriteString(STRING_DRIVES, STRING_DRIVEA_DISKFILENAME,           m_DriveADiskFilename);
    FileP->WriteString(STRING_DRIVES, STRING_DRIVEB_DISKFILENAME,           m_DriveBDiskFilename);
    FileP->WriteInteger(STRING_DRIVES, STRING_AUTOSTART_ENABLE,             m_AutoStartEnable == true ? 1 : 0);
    FileP->WriteInteger(STRING_DRIVES, STRING_DRIVE_ICON_SIZE_FACTOR,       m_DriveIconSizeFactor);
    FileP->WriteInteger(STRING_DRIVES, STRING_SHOW_STATIC_DRIVE_ICON,       m_ShowStaticDriveIcon == true ? 1 : 0);
    FileP->WriteString(STRING_DRIVES, STRING_DISC_DIRECTORY,                m_INIDiscDirectory);
    FileP->WriteInteger(STRING_DRIVES, STRING_DISC_CONFIRMDISKIMAGEUPDATES, m_ConfirmDiskImageUpdates == true ? 1 : 0);
    FileP->WriteInteger(STRING_DRIVES, STRING_DISC_NEVERUPDATEDISKIMAGES,   m_NeverUpdateDiskImages == true ? 1 : 0);
    // Tape
    FileP->WriteString(STRING_TAPE, STRING_TAPE_FILENAME,         m_TapeFilename);
    FileP->WriteInteger(STRING_TAPE, STRING_AUTOPLAY_ENABLE,      m_AutoPlayEnable == true ? 1 : 0);
    FileP->WriteString(STRING_TAPE, STRING_TAPE_DIRECTORY,        m_INITapeDirectory);
    // Emulator
    FileP->WriteString(STRING_EMULATOR, STRING_EMULATOR_DEVICE,                m_CPCDevice);
    FileP->WriteString(STRING_EMULATOR, STRING_EMULATOR_BRAND,                 m_CPCBrand);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_CRTC_TYPE,            m_CRTCType);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_PREFKEYBLAYOUT,       m_PreferredKeyboardLayout);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_FLOPPYDRIVE,          m_UseFloppyDrive == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_FASTFDC,              m_UseFastFDC == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_64KMEMEXT,            m_Use64kMemoryExtension == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_256KMEMEXT,           m_Use256kMemoryExtension == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_256KSILICONDISC,      m_Use256kSiliconDisc == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_TECHNIMUSIQUE,        m_UseTechniMusique == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_PLAYCITY,             m_UsePlayCity == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_MODE2ALIASING,        m_Mode2Antialiasing == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_COLORMONITOR,         m_ColorMonitor == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_MAXSPEEDINDEX,        m_MaxSpeedIndex);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_MAXCUSTOMSPEED,       m_MaxCustomSpeed);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_WINDOWSPRIORITY,      m_WindowsPriority);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_COLDRESETATREALSPEED, m_ColdResetAtRealSpeed == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_SMALLSCREENSHOTSHORTCUT, m_SmallScreenshotShortcut == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_INDEX,          m_ThemeIndex);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_BACKGROUND,     (int)m_ThemeBackgroundColor);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_TEXT1,          (int)m_ThemeText1Color);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_THEME_TEXT2,          (int)m_ThemeText2Color);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_SHOWHINTS,            m_ShowHints == true ? 1 : 0);
    FileP->WriteInteger(STRING_EMULATOR, STRING_EMULATOR_VHOLD_ADJUST,         m_VHoldAdjust);
    // Inputs
    FileP->WriteString(STRING_INPUTS, STRING_PROFILE_DIRECTORY,       m_INIProfileDirectory);
    FileP->WriteString(STRING_INPUTS, STRING_PROFILE_DEFAULT,         m_DefaultProfileFilename);
    FileP->WriteInteger(STRING_INPUTS, STRING_USEHARDWAREKEYBOARDMAP, m_UseHardwareKeyboardMapping == true ? 1 : 0);
    FileP->WriteInteger(STRING_INPUTS, STRING_AMXMOUSE_FACTORX,       m_AMXMouseFactorX);
    FileP->WriteInteger(STRING_INPUTS, STRING_AMXMOUSE_FACTORY,       m_AMXMouseFactorY);
    // Snapshot
    FileP->WriteString(STRING_SNAPSHOT, STRING_SNAPSHOT_DIRECTORY, m_INISnapshotDirectory);
    // Screenshot
    FileP->WriteString(STRING_SCREENSHOT, STRING_SCREENSHOT_DIRECTORY, m_INIScreenshotDirectory);
    // Cheat scripts
    FileP->WriteString(STRING_CHEATSCRIPTS, STRING_CHEATSCRIPTS_DIRECTORY, m_INICheatScriptsDirectory);
    // Audio
    FileP->WriteInteger(STRING_AUDIO, STRING_AUDIO_MUTE,            m_Mute == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_GAIN_SPEAKER,          m_AudioGainSpeaker);
    FileP->WriteInteger(STRING_AUDIO, STRING_GAIN_FLOPPY,           m_AudioGainFloppy);
    FileP->WriteInteger(STRING_AUDIO, STRING_GAIN_TAPE,             m_AudioGainTape);
    FileP->WriteInteger(STRING_AUDIO, STRING_GAIN_TMPI,             m_AudioGainTMPI);
    FileP->WriteInteger(STRING_AUDIO, STRING_GAIN_PLAYCITY,         m_AudioGainPlayCity);
    FileP->WriteInteger(STRING_AUDIO, STRING_VOLUME,                m_AudioVolume);
    FileP->WriteString(STRING_AUDIO, STRING_AUDIO_RENDERER,         m_AudioRenderer);
    FileP->WriteInteger(STRING_AUDIO, STRING_SPEAKER_STEREO,        m_AudioSpeakerStereo == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_PLAYCITY_STEREO,       m_AudioPlayCityStereo == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_PLAYCITY_INVERT,       m_AudioPlayCityInvert == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_AUDIO_USEDEFAULT, m_TapeAudioUseDefault == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_PPITHRESHOLD8,    m_TapeAudioPPIThreshold8);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_PPITHRESHOLD16,   m_TapeAudioPPIThreshold16);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_AUTOGAIN_ACTIVE,  m_TapeAudioAutoGainActive == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_LPF1_ACTIVE,      m_TapeAudioLPF1Active == true ? 1 : 0);
    FileP->WriteString(STRING_AUDIO, STRING_TAPE_LPF1_A,            m_TapeAudioLPF1_A);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_LPF2_ACTIVE,      m_TapeAudioLPF2Active == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_LPF2_ORDER,       m_TapeAudioLPF2Order);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_LPF2_FREQ,        m_TapeAudioLPF2Freq);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_HPF2_ACTIVE,      m_TapeAudioHPF2Active == true ? 1 : 0);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_HPF2_ORDER,       m_TapeAudioHPF2Order);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_HPF2_FREQ,        m_TapeAudioHPF2Freq);
    FileP->WriteInteger(STRING_AUDIO, STRING_TAPE_CREATE_CSW,       m_TapeAudioCreateCSW == true ? 1 : 0);
    // Windows
    FileP->WriteInteger(STRING_WINDOWS, STRING_DSKVIEWER_LEFT,   m_DSKViewerFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_DSKVIEWER_TOP,    m_DSKViewerFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_DSKVIEWER_WIDTH,  m_DSKViewerFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_DSKVIEWER_HEIGHT, m_DSKViewerFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CRTC_LEFT,        m_CRTCFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CRTC_TOP,         m_CRTCFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CRTC_WIDTH,       m_CRTCFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CRTC_HEIGHT,      m_CRTCFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GATEARRAY_LEFT,   m_GateArrayFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GATEARRAY_TOP,    m_GateArrayFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GATEARRAY_WIDTH,  m_GateArrayFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GATEARRAY_HEIGHT, m_GateArrayFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_FDC_LEFT,         m_FDCFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_FDC_TOP,          m_FDCFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_FDC_WIDTH,        m_FDCFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_FDC_HEIGHT,       m_FDCFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_Z80_LEFT,         m_Z80FormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_Z80_TOP,          m_Z80FormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_Z80_WIDTH,        m_Z80FormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_Z80_HEIGHT,       m_Z80FormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEMORY_LEFT,      m_MemoryFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEMORY_TOP,       m_MemoryFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEMORY_WIDTH,     m_MemoryFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEMORY_HEIGHT,    m_MemoryFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PSG_LEFT,         m_PSGFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PSG_TOP,          m_PSGFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PSG_WIDTH,        m_PSGFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PSG_HEIGHT,       m_PSGFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CDTVIEWER_LEFT,   m_CDTViewerFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CDTVIEWER_TOP,    m_CDTViewerFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CDTVIEWER_WIDTH,  m_CDTViewerFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_CDTVIEWER_HEIGHT, m_CDTViewerFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_TAPEAUDIOWORKSHOP_LEFT,   m_TapeAudioWorkshopFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_TAPEAUDIOWORKSHOP_TOP,    m_TapeAudioWorkshopFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEA8000_LEFT,     m_MEA8000FormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEA8000_TOP,      m_MEA8000FormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEA8000_WIDTH,    m_MEA8000FormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_MEA8000_HEIGHT,   m_MEA8000FormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_DEBUGGER_LEFT,    m_DebuggerFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_DEBUGGER_TOP,     m_DebuggerFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_DEBUGGER_WIDTH,   m_DebuggerFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_DEBUGGER_HEIGHT,  m_DebuggerFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PRINTER_LEFT,     m_PrinterFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PRINTER_TOP,      m_PrinterFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PRINTER_WIDTH,    m_PrinterFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_PRINTER_HEIGHT,   m_PrinterFormHeight);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_LEFT,     m_GraphicsExplorerFormLeft);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_TOP,      m_GraphicsExplorerFormTop);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_WIDTH,    m_GraphicsExplorerFormWidth);
    FileP->WriteInteger(STRING_WINDOWS, STRING_GRAPHICSEXPLORER_HEIGHT,   m_GraphicsExplorerFormHeight);
    // Printer
    FileP->WriteInteger(STRING_PRINTER, STRING_COUNTRY_CODE,     m_PrinterCountryCode);
    FileP->WriteInteger(STRING_PRINTER, STRING_CRLF,             m_PrinterCRLF);

    //
    // Roms
    //
    FileP->EraseSection(STRING_ROMS);
    FileP->WriteString(STRING_ROMS, STRING_ROMS_DIRECTORY, m_INIRomsDirectory);
    if (m_LowerROMFilename.IsEmpty() == false)
    {
      FileP->WriteString(STRING_ROMS, STRING_ROMS_LOWERROM, m_LowerROMFilename);
    }
    for (int Loop=0; Loop < NB_UPPERROMS; Loop++)
    {
      if (m_UpperROMFilenames->Strings[Loop].IsEmpty() == false)
      {
        AnsiString String = STRING_ROMS_UPPERROM + AnsiString(Loop);
        FileP->WriteString(STRING_ROMS, String, m_UpperROMFilenames->Strings[Loop]);
      }
    }
    // ASCII Keyboard
    FileP->EraseSection(STRING_ASCIIKEYBOARD);
    for (int loop=0; loop < m_NonASCIIKeysList->Count; loop++)
    {
      tKeyboardMappingKey* KeyP = (tKeyboardMappingKey*)m_NonASCIIKeysList->Items[loop];
      AnsiString KeyString = STRING_ASCIIKEY + AnsiString(KeyP->WindowsKey);
      FileP->WriteInteger(STRING_ASCIIKEYBOARD, KeyString, KeyP->CPCKey);
    }
    // Hardware Keyboard
    FileP->EraseSection(STRING_HARDWAREKEYBOARD);
    for (int loop=0; loop < m_HardwareKeysList->Count; loop++)
    {
      tKeyboardMappingKey* KeyP = (tKeyboardMappingKey*)m_HardwareKeysList->Items[loop];
      AnsiString KeyString = STRING_HARDWAREKEY + AnsiString(CPC_KBD_TO_USERMANUAL(KeyP->CPCKey));
      FileP->WriteInteger(STRING_HARDWAREKEYBOARD, KeyString, KeyP->WindowsKey);
    }
    // Joystick Keyboard
    FileP->EraseSection(STRING_JOYSTICKKEYBOARD);
    for (int loop=0; loop < m_JoystickKeysList->Count; loop++)
    {
      tKeyboardMappingKey* KeyP = (tKeyboardMappingKey*)m_JoystickKeysList->Items[loop];
      AnsiString KeyString = STRING_JOYSTICKKEY + AnsiString(KeyP->CPCKey);
      FileP->WriteInteger(STRING_JOYSTICKKEYBOARD, KeyString, KeyP->WindowsKey);
    }

    delete FileP;

    m_Updated = false;
  }

  return (ToCreate);
}
//---------------------------------------------------------------------------


const tKeyboardMappingKey* __fastcall TSettings::GetFirstNonASCIIKey(void)
{
  m_SpecialASCIIKeyIndex = -1;

  return GetNextNonASCIIKey();
}
//---------------------------------------------------------------------------

const tKeyboardMappingKey* __fastcall TSettings::GetNextNonASCIIKey(void)
{
  m_SpecialASCIIKeyIndex++;

  if (m_SpecialASCIIKeyIndex < m_NonASCIIKeysList->Count)
  {
    return ((tKeyboardMappingKey*)m_NonASCIIKeysList->Items[m_SpecialASCIIKeyIndex]);
  }

  return NULL;
}
//---------------------------------------------------------------------------

void __fastcall TSettings::ClearNonASCIIKeys(void)
{
  m_Updated = true;

  // Clear current keylist
  while (m_NonASCIIKeysList->Count)
  {
    delete (tKeyboardMappingKey*)m_NonASCIIKeysList->Items[0];
    m_NonASCIIKeysList->Delete(0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettings::AddNonASCIIKey(tKeyboardMappingKey* KeyToAdd)
{
  m_Updated = true;

  // Check if Windows Key already exists
  int loop;
  for (loop=0; loop < m_NonASCIIKeysList->Count; loop++)
  {
    tKeyboardMappingKey* Key = (tKeyboardMappingKey*)m_NonASCIIKeysList->Items[loop];
    if (Key->WindowsKey == KeyToAdd->WindowsKey)
    {
      // Update current key
      Key->CPCKey = KeyToAdd->CPCKey;
      delete KeyToAdd;
      break;
    }
  }

  // Add new key
  if (loop == m_NonASCIIKeysList->Count)
  {
    m_NonASCIIKeysList->Add(KeyToAdd);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettings::RestoreDefaultNonASCIIKeys(void)
{
  m_Updated = true;

  // Clear current keylist
  ClearNonASCIIKeys();

  // Restore default keys
  m_SpecialASCIIKeyIndex = 0;
  for (tKeyboardMappingKey* DefaultKey = (tKeyboardMappingKey*)default_non_ascii_keys;
       DefaultKey->CPCKey != CPC_KEY_INVALID;
       DefaultKey++)
  {
    tKeyboardMappingKey* Key = new tKeyboardMappingKey;
    Key->WindowsKey = DefaultKey->WindowsKey;
    Key->CPCKey = DefaultKey->CPCKey;
    AddNonASCIIKey(Key);
  }
}
//---------------------------------------------------------------------------


const tKeyboardMappingKey* __fastcall TSettings::GetFirstHardwareKey(void)
{
  m_HardwareKeyIndex = -1;

  return GetNextHardwareKey();
}
//---------------------------------------------------------------------------

const tKeyboardMappingKey* __fastcall TSettings::GetNextHardwareKey(void)
{
  m_HardwareKeyIndex++;

  if (m_HardwareKeyIndex < m_HardwareKeysList->Count)
  {
    return ((tKeyboardMappingKey*)m_HardwareKeysList->Items[m_HardwareKeyIndex]);
  }

  return NULL;
}
//---------------------------------------------------------------------------

void __fastcall TSettings::ClearHardwareKeys(void)
{
  m_Updated = true;

  // Clear current keylist
  while (m_HardwareKeysList->Count)
  {
    delete (tKeyboardMappingKey*)m_HardwareKeysList->Items[0];
    m_HardwareKeysList->Delete(0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettings::AddHardwareKey(tKeyboardMappingKey* KeyToAdd)
{
  m_Updated = true;

  // Check if Windows Key already exists
  int loop;
  for (loop=0; loop < m_HardwareKeysList->Count; loop++)
  {
    tKeyboardMappingKey* Key = (tKeyboardMappingKey*)m_HardwareKeysList->Items[loop];
    if (Key->WindowsKey == KeyToAdd->WindowsKey)
    {
      // Update current key
      Key->CPCKey = KeyToAdd->CPCKey;
      delete KeyToAdd;
      break;
    }
  }

  // Add new key
  if (loop == m_HardwareKeysList->Count)
  {
    m_HardwareKeysList->Add(KeyToAdd);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettings::RestoreDefaultHardwareKeys(void)
{
  m_Updated = true;

  // Clear current keylist
  ClearHardwareKeys();

  // Restore default keys
  for (m_HardwareKeyIndex=0; m_HardwareKeyIndex < NB_KEYBOARD_KEYS; m_HardwareKeyIndex++)
  {
    tUChar CPCKeycode = USERMANUAL_TO_CPC_KBD(m_HardwareKeyIndex);

    for (tUShort loop=0; loop < NB_KEYBOARD_KEYS; loop++)
    {
      if (default_hardware_virtual_to_cpc_kbdmatrix[loop] == CPCKeycode)
      {
        // Create Key mapping
        tKeyboardMappingKey* KeyP = new tKeyboardMappingKey;
        KeyP->WindowsKey = loop;
        KeyP->CPCKey = CPCKeycode;
        AddHardwareKey(KeyP);
        break;
      }
    }
  }
}
//---------------------------------------------------------------------------

const tKeyboardMappingKey* __fastcall TSettings::GetFirstJoystickKey(void)
{
  m_JoystickKeyIndex = -1;

  return GetNextJoystickKey();
}
//---------------------------------------------------------------------------

const tKeyboardMappingKey* __fastcall TSettings::GetNextJoystickKey(void)
{
  m_JoystickKeyIndex++;

  if (m_JoystickKeyIndex < m_JoystickKeysList->Count)
  {
    return ((tKeyboardMappingKey*)m_JoystickKeysList->Items[m_JoystickKeyIndex]);
  }

  return NULL;
}
//---------------------------------------------------------------------------

void __fastcall TSettings::ClearJoystickKeys(void)
{
  m_Updated = true;

  // Clear current keylist
  while (m_JoystickKeysList->Count)
  {
    delete (tKeyboardMappingKey*)m_JoystickKeysList->Items[0];
    m_JoystickKeysList->Delete(0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettings::AddJoystickKey(tKeyboardMappingKey* KeyToAdd)
{
  m_Updated = true;

  // Check if Windows Key already exists
  int loop;
  for (loop=0; loop < m_JoystickKeysList->Count; loop++)
  {
    tKeyboardMappingKey* Key = (tKeyboardMappingKey*)m_JoystickKeysList->Items[loop];
    if (Key->CPCKey == KeyToAdd->CPCKey)
    {
      Key->WindowsKey = KeyToAdd->WindowsKey;
      delete KeyToAdd;
      break;
    }
  }

  // Add new key
  if (loop == m_JoystickKeysList->Count)
  {
    m_JoystickKeysList->Add(KeyToAdd);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSettings::RestoreDefaultJoystickKeys(void)
{
  m_Updated = true;

  // Clear current keylist
  ClearJoystickKeys();

  // Restore default keys
  const tKeyboardMappingKey* DefaultKey = (const tKeyboardMappingKey*)default_joystick_keys;
  while (DefaultKey->WindowsKey != 0)
  {
    tKeyboardMappingKey* Key = new tKeyboardMappingKey;
    Key->WindowsKey = DefaultKey->WindowsKey;
    Key->CPCKey = DefaultKey->CPCKey;
    AddJoystickKey(Key);

    DefaultKey++;
  }
}
//---------------------------------------------------------------------------

