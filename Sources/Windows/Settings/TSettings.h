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


#ifndef TSettingsH
#define TSettingsH

#include "..\..\..\Sources\Engine\Native_CPC.h"
#include "..\Inputs\Keyboard.h"


#define NB_UPPERROMS                            16


// OnScreen display
#define OSD_NONE                                0x00
#define OSD_MESSAGES                            0x01
#define OSD_ICONS                               0x02
#define OSD_TAPE                                0x04
#define OSD_DISK                                0x08
#define OSD_DEBUG                               0x80


extern const char* const EmptyRomName;
extern const char* const DefaultPrefix;
extern const char* const DefaultOSRomName;
extern const char* const DefaultBASICRomName;
extern const char* const DefaultAMSDOSRomName;
extern const char* const DefaultSiliconDiscRomName;


#pragma warn -8026

class TSettings
{
private:
  //
  // Members
  //
  bool m_Updated;
  AnsiString m_Filename;
  // Filenames and folders
  AnsiString m_DriveADiskFilename;
  AnsiString m_DriveBDiskFilename;
  AnsiString m_TapeFilename;
  AnsiString m_DefaultProfileFilename;
  AnsiString m_LowerROMFilename;
  TStringList* m_UpperROMFilenames;
  AnsiString m_INIProfileDirectory;
  AnsiString m_ProfileDirectory;
  AnsiString m_INIRomsDirectory;
  AnsiString m_RomsDirectory;
  AnsiString m_INIDiscDirectory;
  AnsiString m_DiscDirectory;
  AnsiString m_INITapeDirectory;
  AnsiString m_TapeDirectory;
  AnsiString m_INISnapshotDirectory;
  AnsiString m_SnapshotDirectory;
  AnsiString m_INIScreenshotDirectory;
  AnsiString m_ScreenshotDirectory;
  AnsiString m_INICheatScriptsDirectory;
  AnsiString m_CheatScriptsDirectory;
  AnsiString m_INIScanlinesDirectory;
  AnsiString m_ScanlinesDirectory;
  bool m_ConfirmDiskImageUpdates;
  bool m_NeverUpdateDiskImages;
  // Graphics
  bool m_Fullscreen;
  bool m_KeepFullscreen;
  bool m_DrawScanlines;
  bool m_Mode2Antialiasing;
  bool m_ColorMonitor;
  bool m_ControlPanelVisible;
  bool m_UseOpenGL;
  bool m_ForceGDI;
  int m_OnscreenDisplay;
  LONG m_WindowStyle;
  LONG m_WindowExStyle;
  int m_DriveIconSizeFactor;
  bool m_ShowStaticDriveIcon;
  int m_FullscreenBrightness;
  bool m_FullscreenMenu;
  bool m_KeepAspectRatio;
  bool m_ShowPauseInfo;
  int m_ThemeIndex;
  TColor m_ThemeBackgroundColor;
  TColor m_ThemeText1Color;
  TColor m_ThemeText2Color;
  int m_VHoldAdjust;
  // Emulator
  bool m_AutoStartEnable;
  bool m_AutoPlayEnable;
  AnsiString m_CPCDevice;
  AnsiString m_CPCBrand;
  int m_CRTCType;
  bool m_UseFloppyDrive;
  bool m_UseFastFDC;
  bool m_Use64kMemoryExtension;
  bool m_Use256kMemoryExtension;
  bool m_Use256kSiliconDisc;
  bool m_UseTechniMusique;
  bool m_UsePlayCity;
  int m_MaxSpeedIndex;
  int m_MaxCustomSpeed;
  int m_WindowsPriority;
  bool m_ColdResetAtRealSpeed;
  bool m_SmallScreenshotShortcut;
  bool m_ShowHints;
  // Inputs
  int m_PreferredKeyboardLayout;
  bool m_UseHardwareKeyboardMapping;
  TList* m_NonASCIIKeysList;
  int m_SpecialASCIIKeyIndex;
  TList* m_HardwareKeysList;
  int m_HardwareKeyIndex;
  TList* m_JoystickKeysList;
  int m_JoystickKeyIndex;
  // Audio
  bool m_Mute;
  int m_AudioGainSpeaker;
  int m_AudioGainFloppy;
  int m_AudioGainTape;
  int m_AudioGainTMPI;
  int m_AudioGainPlayCity;
  int m_AudioVolume;
  AnsiString m_AudioRenderer;
  bool m_AudioSpeakerStereo;
  bool m_AudioSpeaker16Bits;
  int m_AudioSpeakerFreq;
  int m_AudioTMPIFreq;
  int m_AudioPlayCityFreq;
  bool m_AudioPlayCityStereo;
  bool m_AudioPlayCity16Bits;
  bool m_AudioPlayCityInvert;
  // Forms position
  int m_MainFormLeft;
  int m_MainFormTop;
  int m_MainFormWidth;
  int m_MainFormHeight;
  int m_DSKViewerFormLeft;
  int m_DSKViewerFormTop;
  int m_DSKViewerFormWidth;
  int m_DSKViewerFormHeight;
  int m_CRTCFormLeft;
  int m_CRTCFormTop;
  int m_CRTCFormWidth;
  int m_CRTCFormHeight;
  int m_GateArrayFormLeft;
  int m_GateArrayFormTop;
  int m_GateArrayFormWidth;
  int m_GateArrayFormHeight;
  int m_FDCFormLeft;
  int m_FDCFormTop;
  int m_FDCFormWidth;
  int m_FDCFormHeight;
  int m_Z80FormLeft;
  int m_Z80FormTop;
  int m_Z80FormWidth;
  int m_Z80FormHeight;
  int m_MemoryFormLeft;
  int m_MemoryFormTop;
  int m_MemoryFormWidth;
  int m_MemoryFormHeight;
  int m_PSGFormLeft;
  int m_PSGFormTop;
  int m_PSGFormWidth;
  int m_PSGFormHeight;
  int m_CDTViewerFormLeft;
  int m_CDTViewerFormTop;
  int m_CDTViewerFormWidth;
  int m_CDTViewerFormHeight;
  int m_TapeAudioWorkshopFormLeft;
  int m_TapeAudioWorkshopFormTop;
  int m_MEA8000FormLeft;
  int m_MEA8000FormTop;
  int m_MEA8000FormWidth;
  int m_MEA8000FormHeight;
  int m_DebuggerFormLeft;
  int m_DebuggerFormTop;
  int m_DebuggerFormWidth;
  int m_DebuggerFormHeight;
  int m_PrinterFormLeft;
  int m_PrinterFormTop;
  int m_PrinterFormWidth;
  int m_PrinterFormHeight;
  int m_GraphicsExplorerFormLeft;
  int m_GraphicsExplorerFormTop;
  int m_GraphicsExplorerFormWidth;
  int m_GraphicsExplorerFormHeight;
  // Tape audio workshop
  bool m_TapeAudioUseDefault;
  tUChar m_TapeAudioPPIThreshold8;
  tShort m_TapeAudioPPIThreshold16;
  bool m_TapeAudioAutoGainActive;
  bool m_TapeAudioLPF1Active;
  AnsiString m_TapeAudioLPF1_A;
  bool m_TapeAudioLPF2Active;
  tUChar m_TapeAudioLPF2Order;
  tUShort m_TapeAudioLPF2Freq;
  bool m_TapeAudioHPF2Active;
  tUChar m_TapeAudioHPF2Order;
  tUShort m_TapeAudioHPF2Freq;
  bool m_TapeAudioCreateCSW;
  // Printer
  int m_PrinterCountryCode;
  int m_PrinterCRLF;
  // AMX Mouse
  int m_AMXMouseFactorX;
  int m_AMXMouseFactorY;
  //
  // Methods
  //
  template<class T> void __fastcall SetValue(const T& NewValue, T& LastValue);
  // Filenames and folders
  inline AnsiString __fastcall GetDriveADiskFilename(void) const { return m_DriveADiskFilename; };
  inline void __fastcall SetDriveADiskFilename(const AnsiString Value) { SetValue<AnsiString>(Value, m_DriveADiskFilename); }
  inline AnsiString __fastcall GetDriveBDiskFilename(void) const { return m_DriveBDiskFilename; };
  inline void __fastcall SetDriveBDiskFilename(const AnsiString Value) { SetValue<AnsiString>(Value, m_DriveBDiskFilename); }
  inline AnsiString __fastcall GetTapeFilename(void) const { return m_TapeFilename; };
  inline void __fastcall SetTapeFilename(const AnsiString Value) { SetValue<AnsiString>(Value, m_TapeFilename); }
  inline AnsiString __fastcall GetProfileDirectory(void) const { return m_ProfileDirectory; };
  void __fastcall SetProfileDirectory(const AnsiString Value);
  inline AnsiString __fastcall GetDefaultProfileFilename(void) const { return m_DefaultProfileFilename; };
  inline void __fastcall SetDefaultProfileFilename(const AnsiString Value) { SetValue<AnsiString>(Value, m_DefaultProfileFilename); }
  inline AnsiString __fastcall GetRomsDirectory(void) const { return m_RomsDirectory; };
  void __fastcall SetRomsDirectory(const AnsiString Value);
  inline AnsiString __fastcall GetLowerROMFilename(void) const { return m_LowerROMFilename; };
  inline void __fastcall SetLowerROMFilename(const AnsiString Value) { SetValue<AnsiString>(Value, m_LowerROMFilename); }
  AnsiString __fastcall GetUpperROMFilename(int Index);
  void __fastcall SetUpperROMFilename(int Index, const AnsiString Value);
  inline AnsiString __fastcall GetDiscDirectory(void) const { return m_DiscDirectory; };
  void __fastcall SetDiscDirectory(const AnsiString Value);
	inline AnsiString __fastcall GetTapeDirectory(void) const { return m_TapeDirectory; };
  void __fastcall SetTapeDirectory(const AnsiString Value);
  inline AnsiString __fastcall GetSnapshotDirectory(void) const { return m_SnapshotDirectory; };
  void __fastcall SetSnapshotDirectory(const AnsiString Value);
  inline AnsiString __fastcall GetScreenshotDirectory(void) const { return m_ScreenshotDirectory; };
  void __fastcall SetScreenshotDirectory(const AnsiString Value);
  inline AnsiString __fastcall GetCheatScriptsDirectory(void) const { return m_CheatScriptsDirectory; };
  void __fastcall SetCheatScriptsDirectory(const AnsiString Value);
  inline AnsiString __fastcall GetScanlinesDirectory(void) const { return m_ScanlinesDirectory; };
  void __fastcall SetScanlinesDirectory(const AnsiString Value);
  inline bool __fastcall GetConfirmDiskImageUpdates(void) const { return m_ConfirmDiskImageUpdates; };
  inline void __fastcall SetConfirmDiskImageUpdates(const bool Value) { SetValue<bool>(Value, m_ConfirmDiskImageUpdates); }
  inline bool __fastcall GetNeverUpdateDiskImages(void) const { return m_NeverUpdateDiskImages; };
  inline void __fastcall SetNeverUpdateDiskImages(const bool Value) { SetValue<bool>(Value, m_NeverUpdateDiskImages); }
  // Graphics
  inline bool __fastcall GetFullscreen(void) const { return m_Fullscreen; };
  inline void __fastcall SetFullscreen(const bool Value) { SetValue<bool>(Value, m_Fullscreen); }
  inline bool __fastcall GetKeepFullscreen(void) const { return m_KeepFullscreen; };
  inline void __fastcall SetKeepFullscreen(const bool Value) { SetValue<bool>(Value, m_KeepFullscreen); }
  inline bool __fastcall GetDrawScanlines(void) const { return m_DrawScanlines; };
  inline void __fastcall SetDrawScanlines(const bool Value) { SetValue<bool>(Value, m_DrawScanlines); }
  inline bool __fastcall GetMode2Antialiasing(void) const { return m_Mode2Antialiasing; };
  inline void __fastcall SetMode2Antialiasing(const bool Value) { SetValue<bool>(Value, m_Mode2Antialiasing); }
  inline bool __fastcall GetColorMonitor(void) const { return m_ColorMonitor; };
  inline void __fastcall SetColorMonitor(const bool Value) { SetValue<bool>(Value, m_ColorMonitor); }
  inline bool __fastcall GetControlPanelVisible(void) const { return m_ControlPanelVisible; };
  inline void __fastcall SetControlPanelVisible(const bool Value) { SetValue<bool>(Value, m_ControlPanelVisible); }
  inline bool __fastcall GetUseOpenGL(void) const { return m_UseOpenGL; };
  inline void __fastcall SetUseOpenGL(const bool Value) { SetValue<bool>(Value, m_UseOpenGL); }
  inline bool __fastcall GetForceGDI(void) const { return m_ForceGDI; };
  inline void __fastcall SetForceGDI(const bool Value) { SetValue<bool>(Value, m_ForceGDI); }
  inline int __fastcall GetOnScreenDisplay(void) const { return m_OnscreenDisplay; };
  inline void __fastcall SetOnScreenDisplay(const int Value) { SetValue<int>(Value, m_OnscreenDisplay); }
  inline LONG __fastcall GetWindowStyle(void) const { return m_WindowStyle; };
  inline void __fastcall SetWindowStyle(const LONG Value) { SetValue<LONG>(Value, m_WindowStyle); }
  inline LONG __fastcall GetWindowExStyle(void) const { return m_WindowExStyle; };
  inline void __fastcall SetWindowExStyle(const LONG Value) { SetValue<LONG>(Value, m_WindowExStyle); }
  inline int __fastcall GetDriveIconSizeFactor(void) const { return m_DriveIconSizeFactor; };
  inline bool __fastcall GetShowStaticDriveIcon(void) const { return m_ShowStaticDriveIcon; };
  inline int __fastcall GetFullscreenBrightness(void) const { return m_FullscreenBrightness; };
  inline void __fastcall SetFullscreenBrightness(const int Value) { SetValue<int>(Value, m_FullscreenBrightness); }
  inline bool __fastcall GetFullscreenMenu(void) const { return m_FullscreenMenu; };
  inline void __fastcall SetFullscreenMenu(const bool Value) { SetValue<bool>(Value, m_FullscreenMenu); }
  inline bool __fastcall GetKeepAspectRatio(void) const { return m_KeepAspectRatio; };
  inline void __fastcall SetKeepAspectRatio(const bool Value) { SetValue<bool>(Value, m_KeepAspectRatio); }
  inline bool __fastcall GetShowPauseInfo(void) const { return m_ShowPauseInfo; };
  inline void __fastcall SetShowPauseInfo(const bool Value) { SetValue<bool>(Value, m_ShowPauseInfo); }
  inline int __fastcall GetThemeIndex(void) const { return m_ThemeIndex; };
  inline void __fastcall SetThemeIndex(const int Value) { SetValue<int>(Value, m_ThemeIndex); }
  inline TColor __fastcall GetThemeBackgroundColor(void) const { return m_ThemeBackgroundColor; };
  inline void __fastcall SetThemeBackgroundColor(const TColor Value) { SetValue<TColor>(Value, m_ThemeBackgroundColor); }
  inline TColor __fastcall GetThemeText1Color(void) const { return m_ThemeText1Color; };
  inline void __fastcall SetThemeText1Color(const TColor Value) { SetValue<TColor>(Value, m_ThemeText1Color); }
  inline TColor __fastcall GetThemeText2Color(void) const { return m_ThemeText2Color; };
  inline void __fastcall SetThemeText2Color(const TColor Value) { SetValue<TColor>(Value, m_ThemeText2Color); }
  inline int __fastcall GetVHoldAdjust(void) const { return m_VHoldAdjust; };
  inline void __fastcall SetVHoldAdjust(const int Value) { SetValue<int>(Value, m_VHoldAdjust); }
  // Emulator
  inline bool __fastcall GetAutoStartEnable(void) const { return m_AutoStartEnable; };
  inline void __fastcall SetAutoStartEnable(const bool Value) { SetValue<bool>(Value, m_AutoStartEnable); }
  inline bool __fastcall GetAutoPlayEnable(void) const { return m_AutoPlayEnable; };
  inline void __fastcall SetAutoPlayEnable(const bool Value) { SetValue<bool>(Value, m_AutoPlayEnable); }
  int __fastcall GetCPCDevice(void);
  void __fastcall SetCPCDevice(const int Value);
  int __fastcall GetCPCBrand(void);
  void __fastcall SetCPCBrand(const int Value);
  inline int __fastcall GetCRTCType(void) const { return m_CRTCType; };
  inline void __fastcall SetCRTCType(const int Value) { SetValue<int>(Value, m_CRTCType); }
  inline bool __fastcall GetUseFloppyDrive(void) const { return m_UseFloppyDrive; };
  inline void __fastcall SetUseFloppyDrive(const bool Value) { SetValue<bool>(Value, m_UseFloppyDrive); }
  inline bool __fastcall GetUseFastFDC(void) const { return m_UseFastFDC; };
  inline void __fastcall SetUseFastFDC(const bool Value) { SetValue<bool>(Value, m_UseFastFDC); }
  inline bool __fastcall GetUse64kMemoryExtension(void) const { return m_Use64kMemoryExtension; };
  inline void __fastcall SetUse64kMemoryExtension(const bool Value) { SetValue<bool>(Value, m_Use64kMemoryExtension); }
  inline bool __fastcall GetUse256kMemoryExtension(void) const { return m_Use256kMemoryExtension; };
  inline void __fastcall SetUse256kMemoryExtension(const bool Value) { SetValue<bool>(Value, m_Use256kMemoryExtension); }
  inline bool __fastcall GetUse256kSiliconDisc(void) const { return m_Use256kSiliconDisc; };
  inline void __fastcall SetUse256kSiliconDisc(const bool Value) { SetValue<bool>(Value, m_Use256kSiliconDisc); }
  inline bool __fastcall GetUseTechniMusique(void) const { return m_UseTechniMusique; };
  inline void __fastcall SetUseTechniMusique(const bool Value) { SetValue<bool>(Value, m_UseTechniMusique); }
  inline bool __fastcall GetUsePlayCity(void) const { return m_UsePlayCity; };
  inline void __fastcall SetUsePlayCity(const bool Value) { SetValue<bool>(Value, m_UsePlayCity); }
  inline int __fastcall GetMaxSpeedIndex(void) const { return m_MaxSpeedIndex; };
  inline void __fastcall SetMaxSpeedIndex(const int Value) { SetValue<int>(Value, m_MaxSpeedIndex); }
  inline int __fastcall GetMaxCustomSpeed(void) const { return m_MaxCustomSpeed; };
  inline void __fastcall SetMaxCustomSpeed(const int Value) { SetValue<int>(Value, m_MaxCustomSpeed); }
  inline int __fastcall GetWindowsPriority(void) const { return m_WindowsPriority; };
  inline bool __fastcall GetColdResetAtRealSpeed(void) const { return m_ColdResetAtRealSpeed; };
  inline void __fastcall SetColdResetAtRealSpeed(const bool Value) { SetValue<bool>(Value, m_ColdResetAtRealSpeed); }
  inline bool __fastcall GetSmallScreenshotShortcut(void) const { return m_SmallScreenshotShortcut; };
  inline void __fastcall SetSmallScreenshotShortcut(const bool Value) { SetValue<bool>(Value, m_SmallScreenshotShortcut); }
  inline bool __fastcall GetShowHints(void) const { return m_ShowHints; };
  inline void __fastcall SetShowHints(const bool Value) { SetValue<bool>(Value, m_ShowHints); }
  // Inputs
  inline int __fastcall GetPreferredKeyboardLayout(void) const { return m_PreferredKeyboardLayout; };
  inline void __fastcall SetPreferredKeyboardLayout(const int Value) { SetValue<int>(Value, m_PreferredKeyboardLayout); }
  inline bool __fastcall GetUseHardwareKeyboardMapping(void) const { return m_UseHardwareKeyboardMapping; };
  inline void __fastcall SetUseHardwareKeyboardMapping(const bool Value) { SetValue<bool>(Value, m_UseHardwareKeyboardMapping); }
  // Audio
  inline bool __fastcall GetMute(void) const { return m_Mute; };
  inline void __fastcall SetMute(const bool Value) { SetValue<bool>(Value, m_Mute); }
  inline int __fastcall GetAudioGainSpeaker(void) const { return m_AudioGainSpeaker; };
  inline void __fastcall SetAudioGainSpeaker(const int Value) { SetValue<int>(Value, m_AudioGainSpeaker); }
  inline int __fastcall GetAudioGainFloppy(void) const { return m_AudioGainFloppy; };
  inline void __fastcall SetAudioGainFloppy(const int Value) { SetValue<int>(Value, m_AudioGainFloppy); }
  inline int __fastcall GetAudioGainTape(void) const { return m_AudioGainTape; };
  inline void __fastcall SetAudioGainTape(const int Value) { SetValue<int>(Value, m_AudioGainTape); }
  inline int __fastcall GetAudioGainTMPI(void) const { return m_AudioGainTMPI; };
  inline void __fastcall SetAudioGainTMPI(const int Value) { SetValue<int>(Value, m_AudioGainTMPI); }
  inline int __fastcall GetAudioGainPlayCity(void) const { return m_AudioGainPlayCity; };
  inline void __fastcall SetAudioGainPlayCity(const int Value) { SetValue<int>(Value, m_AudioGainPlayCity); }
  inline int __fastcall GetAudioVolume(void) const { return m_AudioVolume; };
  inline void __fastcall SetAudioVolume(const int Value) { SetValue<int>(Value, m_AudioVolume); }
  inline AnsiString __fastcall GetAudioRenderer(void) const { return m_AudioRenderer; };
  inline void __fastcall SetAudioRenderer(const AnsiString Value) { SetValue<AnsiString>(Value, m_AudioRenderer); }
  inline bool __fastcall GetAudioSpeakerStereo(void) const { return m_AudioSpeakerStereo; };
  inline void __fastcall SetAudioSpeakerStereo(const bool Value) { SetValue<bool>(Value, m_AudioSpeakerStereo); }
  inline bool __fastcall GetAudioSpeaker16Bits(void) const { return m_AudioSpeaker16Bits; };
  inline int __fastcall GetAudioSpeakerFreq(void) const { return m_AudioSpeakerFreq; };
  inline int __fastcall GetAudioTMPIFreq(void) const { return m_AudioTMPIFreq; };
  inline int __fastcall GetAudioPlayCityFreq(void) const { return m_AudioPlayCityFreq; };
  inline bool __fastcall GetAudioPlayCityStereo(void) const { return m_AudioPlayCityStereo; };
  inline void __fastcall SetAudioPlayCityStereo(const bool Value) { SetValue<bool>(Value, m_AudioPlayCityStereo); }
  inline bool __fastcall GetAudioPlayCity16Bits(void) const { return m_AudioPlayCity16Bits; };
  inline bool __fastcall GetAudioPlayCityInvert(void) const { return m_AudioPlayCityInvert; };
  inline void __fastcall SetAudioPlayCityInvert(const bool Value) { SetValue<bool>(Value, m_AudioPlayCityInvert); }
  // Forms position
  inline int __fastcall GetMainFormLeft(void) const { return m_MainFormLeft; };
  inline void __fastcall SetMainFormLeft(const int Value) { SetValue<int>(Value, m_MainFormLeft); }
  inline int __fastcall GetMainFormTop(void) const { return m_MainFormTop; };
  inline void __fastcall SetMainFormTop(const int Value) { SetValue<int>(Value, m_MainFormTop); }
  inline int __fastcall GetMainFormWidth(void) const { return m_MainFormWidth; };
  inline void __fastcall SetMainFormWidth(const int Value) { SetValue<int>(Value, m_MainFormWidth); }
  inline int __fastcall GetMainFormHeight(void) const { return m_MainFormHeight; };
  inline void __fastcall SetMainFormHeight(const int Value) { SetValue<int>(Value, m_MainFormHeight); }
  inline int __fastcall GetDSKViewerFormLeft(void) const { return m_DSKViewerFormLeft; };
  inline void __fastcall SetDSKViewerFormLeft(const int Value) { SetValue<int>(Value, m_DSKViewerFormLeft); }
  inline int __fastcall GetDSKViewerFormTop(void) const { return m_DSKViewerFormTop; };
  inline void __fastcall SetDSKViewerFormTop(const int Value) { SetValue<int>(Value, m_DSKViewerFormTop); }
  inline int __fastcall GetDSKViewerFormWidth(void) const { return m_DSKViewerFormWidth; };
  inline void __fastcall SetDSKViewerFormWidth(const int Value) { SetValue<int>(Value, m_DSKViewerFormWidth); }
  inline int __fastcall GetDSKViewerFormHeight(void) const { return m_DSKViewerFormHeight; };
  inline void __fastcall SetDSKViewerFormHeight(const int Value) { SetValue<int>(Value, m_DSKViewerFormHeight); }
  inline int __fastcall GetCRTCFormLeft(void) const { return m_CRTCFormLeft; };
  inline void __fastcall SetCRTCFormLeft(const int Value) { SetValue<int>(Value, m_CRTCFormLeft); }
  inline int __fastcall GetCRTCFormTop(void) const { return m_CRTCFormTop; };
  inline void __fastcall SetCRTCFormTop(const int Value) { SetValue<int>(Value, m_CRTCFormTop); }
  inline int __fastcall GetCRTCFormWidth(void) const { return m_CRTCFormWidth; };
  inline void __fastcall SetCRTCFormWidth(const int Value) { SetValue<int>(Value, m_CRTCFormWidth); }
  inline int __fastcall GetCRTCFormHeight(void) const { return m_CRTCFormHeight; };
  inline void __fastcall SetCRTCFormHeight(const int Value) { SetValue<int>(Value, m_CRTCFormHeight); }
  inline int __fastcall GetGateArrayFormLeft(void) const { return m_GateArrayFormLeft; };
  inline void __fastcall SetGateArrayFormLeft(const int Value) { SetValue<int>(Value, m_GateArrayFormLeft); }
  inline int __fastcall GetGateArrayFormTop(void) const { return m_GateArrayFormTop; };
  inline void __fastcall SetGateArrayFormTop(const int Value) { SetValue<int>(Value, m_GateArrayFormTop); }
  inline int __fastcall GetGateArrayFormWidth(void) const { return m_GateArrayFormWidth; };
  inline void __fastcall SetGateArrayFormWidth(const int Value) { SetValue<int>(Value, m_GateArrayFormWidth); }
  inline int __fastcall GetGateArrayFormHeight(void) const { return m_GateArrayFormHeight; };
  inline void __fastcall SetGateArrayFormHeight(const int Value) { SetValue<int>(Value, m_GateArrayFormHeight); }
  inline int __fastcall GetFDCFormLeft(void) const { return m_FDCFormLeft; };
  inline void __fastcall SetFDCFormLeft(const int Value) { SetValue<int>(Value, m_FDCFormLeft); }
  inline int __fastcall GetFDCFormTop(void) const { return m_FDCFormTop; };
  inline void __fastcall SetFDCFormTop(const int Value) { SetValue<int>(Value, m_FDCFormTop); }
  inline int __fastcall GetFDCFormWidth(void) const { return m_FDCFormWidth; };
  inline void __fastcall SetFDCFormWidth(const int Value) { SetValue<int>(Value, m_FDCFormWidth); }
  inline int __fastcall GetFDCFormHeight(void) const { return m_FDCFormHeight; };
  inline void __fastcall SetFDCFormHeight(const int Value) { SetValue<int>(Value, m_FDCFormHeight); }
  inline int __fastcall GetZ80FormLeft(void) const { return m_Z80FormLeft; };
  inline void __fastcall SetZ80FormLeft(const int Value) { SetValue<int>(Value, m_Z80FormLeft); }
  inline int __fastcall GetZ80FormTop(void) const { return m_Z80FormTop; };
  inline void __fastcall SetZ80FormTop(const int Value) { SetValue<int>(Value, m_Z80FormTop); }
  inline int __fastcall GetZ80FormWidth(void) const { return m_Z80FormWidth; };
  inline void __fastcall SetZ80FormWidth(const int Value) { SetValue<int>(Value, m_Z80FormWidth); }
  inline int __fastcall GetZ80FormHeight(void) const { return m_Z80FormHeight; };
  inline void __fastcall SetZ80FormHeight(const int Value) { SetValue<int>(Value, m_Z80FormHeight); }
  inline int __fastcall GetMemoryFormLeft(void) const { return m_MemoryFormLeft; };
  inline void __fastcall SetMemoryFormLeft(const int Value) { SetValue<int>(Value, m_MemoryFormLeft); }
  inline int __fastcall GetMemoryFormTop(void) const { return m_MemoryFormTop; };
  inline void __fastcall SetMemoryFormTop(const int Value) { SetValue<int>(Value, m_MemoryFormTop); }
  inline int __fastcall GetMemoryFormWidth(void) const { return m_MemoryFormWidth; };
  inline void __fastcall SetMemoryFormWidth(const int Value) { SetValue<int>(Value, m_MemoryFormWidth); }
  inline int __fastcall GetMemoryFormHeight(void) const { return m_MemoryFormHeight; };
  inline void __fastcall SetMemoryFormHeight(const int Value) { SetValue<int>(Value, m_MemoryFormHeight); }
  inline int __fastcall GetPSGFormLeft(void) const { return m_PSGFormLeft; };
  inline void __fastcall SetPSGFormLeft(const int Value) { SetValue<int>(Value, m_PSGFormLeft); }
  inline int __fastcall GetPSGFormTop(void) const { return m_PSGFormTop; };
  inline void __fastcall SetPSGFormTop(const int Value) { SetValue<int>(Value, m_PSGFormTop); }
  inline int __fastcall GetPSGFormWidth(void) const { return m_PSGFormWidth; };
  inline void __fastcall SetPSGFormWidth(const int Value) { SetValue<int>(Value, m_PSGFormWidth); }
  inline int __fastcall GetPSGFormHeight(void) const { return m_PSGFormHeight; };
  inline void __fastcall SetPSGFormHeight(const int Value) { SetValue<int>(Value, m_PSGFormHeight); }
  inline int __fastcall GetCDTViewerFormLeft(void) const { return m_CDTViewerFormLeft; };
  inline void __fastcall SetCDTViewerFormLeft(const int Value) { SetValue<int>(Value, m_CDTViewerFormLeft); }
  inline int __fastcall GetCDTViewerFormTop(void) const { return m_CDTViewerFormTop; };
  inline void __fastcall SetCDTViewerFormTop(const int Value) { SetValue<int>(Value, m_CDTViewerFormTop); }
  inline int __fastcall GetCDTViewerFormWidth(void) const { return m_CDTViewerFormWidth; };
  inline void __fastcall SetCDTViewerFormWidth(const int Value) { SetValue<int>(Value, m_CDTViewerFormWidth); }
  inline int __fastcall GetCDTViewerFormHeight(void) const { return m_CDTViewerFormHeight; };
  inline void __fastcall SetCDTViewerFormHeight(const int Value) { SetValue<int>(Value, m_CDTViewerFormHeight); }
  inline int __fastcall GetTapeAudioWorkshopFormLeft(void) const { return m_TapeAudioWorkshopFormLeft; };
  inline void __fastcall SetTapeAudioWorkshopFormLeft(const int Value) { SetValue<int>(Value, m_TapeAudioWorkshopFormLeft); }
  inline int __fastcall GetTapeAudioWorkshopFormTop(void) const { return m_TapeAudioWorkshopFormTop; };
  inline void __fastcall SetTapeAudioWorkshopFormTop(const int Value) { SetValue<int>(Value, m_TapeAudioWorkshopFormTop); }
  inline int __fastcall GetMEA8000FormLeft(void) const { return m_MEA8000FormLeft; };
  inline void __fastcall SetMEA8000FormLeft(const int Value) { SetValue<int>(Value, m_MEA8000FormLeft); }
  inline int __fastcall GetMEA8000FormTop(void) const { return m_MEA8000FormTop; };
  inline void __fastcall SetMEA8000FormTop(const int Value) { SetValue<int>(Value, m_MEA8000FormTop); }
  inline int __fastcall GetMEA8000FormWidth(void) const { return m_MEA8000FormWidth; };
  inline void __fastcall SetMEA8000FormWidth(const int Value) { SetValue<int>(Value, m_MEA8000FormWidth); }
  inline int __fastcall GetMEA8000FormHeight(void) const { return m_MEA8000FormHeight; };
  inline void __fastcall SetMEA8000FormHeight(const int Value) { SetValue<int>(Value, m_MEA8000FormHeight); }
  inline int __fastcall GetDebuggerFormLeft(void) const { return m_DebuggerFormLeft; };
  inline void __fastcall SetDebuggerFormLeft(const int Value) { SetValue<int>(Value, m_DebuggerFormLeft); }
  inline int __fastcall GetDebuggerFormTop(void) const { return m_DebuggerFormTop; };
  inline void __fastcall SetDebuggerFormTop(const int Value) { SetValue<int>(Value, m_DebuggerFormTop); }
  inline int __fastcall GetDebuggerFormWidth(void) const { return m_DebuggerFormWidth; };
  inline void __fastcall SetDebuggerFormWidth(const int Value) { SetValue<int>(Value, m_DebuggerFormWidth); }
  inline int __fastcall GetDebuggerFormHeight(void) const { return m_DebuggerFormHeight; };
  inline void __fastcall SetDebuggerFormHeight(const int Value) { SetValue<int>(Value, m_DebuggerFormHeight); }
  inline int __fastcall GetPrinterFormLeft(void) const { return m_PrinterFormLeft; };
  inline void __fastcall SetPrinterFormLeft(const int Value) { SetValue<int>(Value, m_PrinterFormLeft); }
  inline int __fastcall GetPrinterFormTop(void) const { return m_PrinterFormTop; };
  inline void __fastcall SetPrinterFormTop(const int Value) { SetValue<int>(Value, m_PrinterFormTop); }
  inline int __fastcall GetPrinterFormWidth(void) const { return m_PrinterFormWidth; };
  inline void __fastcall SetPrinterFormWidth(const int Value) { SetValue<int>(Value, m_PrinterFormWidth); }
  inline int __fastcall GetPrinterFormHeight(void) const { return m_PrinterFormHeight; };
  inline void __fastcall SetPrinterFormHeight(const int Value) { SetValue<int>(Value, m_PrinterFormHeight); }
  inline int __fastcall GetGraphicsExplorerFormLeft(void) const { return m_GraphicsExplorerFormLeft; };
  inline void __fastcall SetGraphicsExplorerFormLeft(const int Value) { SetValue<int>(Value, m_GraphicsExplorerFormLeft); }
  inline int __fastcall GetGraphicsExplorerFormTop(void) const { return m_GraphicsExplorerFormTop; };
  inline void __fastcall SetGraphicsExplorerFormTop(const int Value) { SetValue<int>(Value, m_GraphicsExplorerFormTop); }
  inline int __fastcall GetGraphicsExplorerFormWidth(void) const { return m_GraphicsExplorerFormWidth; };
  inline void __fastcall SetGraphicsExplorerFormWidth(const int Value) { SetValue<int>(Value, m_GraphicsExplorerFormWidth); }
  inline int __fastcall GetGraphicsExplorerFormHeight(void) const { return m_GraphicsExplorerFormHeight; };
  inline void __fastcall SetGraphicsExplorerFormHeight(const int Value) { SetValue<int>(Value, m_GraphicsExplorerFormHeight); }
  // Tape audio workshop
  inline bool __fastcall GetTapeAudioUseDefault(void) const { return m_TapeAudioUseDefault; };
  inline void __fastcall SetTapeAudioUseDefault(const bool Value) { SetValue<bool>(Value, m_TapeAudioUseDefault); }
  inline tUChar __fastcall GetTapeAudioPPIThreshold8(void) const { return m_TapeAudioPPIThreshold8; };
  inline void __fastcall SetTapeAudioPPIThreshold8(const tUChar Value) { SetValue<tUChar>(Value, m_TapeAudioPPIThreshold8); }
  inline tShort __fastcall GetTapeAudioPPIThreshold16(void) const { return m_TapeAudioPPIThreshold16; };
  inline void __fastcall SetTapeAudioPPIThreshold16(const tShort Value) { SetValue<tShort>(Value, m_TapeAudioPPIThreshold16); }
  inline bool __fastcall GetTapeAudioAutoGainActive(void) const { return m_TapeAudioAutoGainActive; };
  inline void __fastcall SetTapeAudioAutoGainActive(const bool Value) { SetValue<bool>(Value, m_TapeAudioAutoGainActive); }
  inline bool __fastcall GetTapeAudioLPF1Active(void) const { return m_TapeAudioLPF1Active; };
  inline void __fastcall SetTapeAudioLPF1Active(const bool Value) { SetValue<bool>(Value, m_TapeAudioLPF1Active); }
  inline AnsiString __fastcall GetTapeAudioLPF1_A(void) const { return m_TapeAudioLPF1_A; };
  inline void __fastcall SetTapeAudioLPF1_A(const AnsiString Value) { SetValue<AnsiString>(Value, m_TapeAudioLPF1_A); }
  inline bool __fastcall GetTapeAudioLPF2Active(void) const { return m_TapeAudioLPF2Active; };
  inline void __fastcall SetTapeAudioLPF2Active(const bool Value) { SetValue<bool>(Value, m_TapeAudioLPF2Active); }
  inline tUChar __fastcall GetTapeAudioLPF2Order(void) const { return m_TapeAudioLPF2Order; };
  inline void __fastcall SetTapeAudioLPF2Order(const tUChar Value) { SetValue<tUChar>(Value, m_TapeAudioLPF2Order); }
  inline tUShort __fastcall GetTapeAudioLPF2Freq(void) const { return m_TapeAudioLPF2Freq; };
  inline void __fastcall SetTapeAudioLPF2Freq(const tUShort Value) { SetValue<tUShort>(Value, m_TapeAudioLPF2Freq); }
  inline bool __fastcall GetTapeAudioHPF2Active(void) const { return m_TapeAudioHPF2Active; };
  inline void __fastcall SetTapeAudioHPF2Active(const bool Value) { SetValue<bool>(Value, m_TapeAudioHPF2Active); }
  inline tUChar __fastcall GetTapeAudioHPF2Order(void) const { return m_TapeAudioHPF2Order; };
  inline void __fastcall SetTapeAudioHPF2Order(const tUChar Value) { SetValue<tUChar>(Value, m_TapeAudioHPF2Order); }
  inline tUShort __fastcall GetTapeAudioHPF2Freq(void) const { return m_TapeAudioHPF2Freq; };
  inline void __fastcall SetTapeAudioHPF2Freq(const tUShort Value) { SetValue<tUShort>(Value, m_TapeAudioHPF2Freq); }
  inline bool __fastcall GetTapeAudioCreateCSW(void) const { return m_TapeAudioCreateCSW; };
  inline void __fastcall SetTapeAudioCreateCSW(const bool Value) { SetValue<bool>(Value, m_TapeAudioCreateCSW); }
  // Printer
  inline int __fastcall GetPrinterCountryCode(void) const { return m_PrinterCountryCode; };
  inline void __fastcall SetPrinterCountryCode(const int Value) { SetValue<int>(Value, m_PrinterCountryCode); }
  inline int __fastcall GetPrinterCRLF(void) const { return m_PrinterCRLF; };
  inline void __fastcall SetPrinterCRLF(const int Value) { SetValue<int>(Value, m_PrinterCRLF); }
  // AMX Mouse
  inline int __fastcall GetAMXMouseFactorX(void) const { return m_AMXMouseFactorX; };
  inline void __fastcall SetAMXMouseFactorX(const int Value) { SetValue<int>(Value, m_AMXMouseFactorX); }
  inline int __fastcall GetAMXMouseFactorY(void) const { return m_AMXMouseFactorY; };
  inline void __fastcall SetAMXMouseFactorY(const int Value) { SetValue<int>(Value, m_AMXMouseFactorY); }

public:
  //
  // Properties
  //
  // Filenames and folders
  __property AnsiString DriveADiskFilename = {read=GetDriveADiskFilename, write=SetDriveADiskFilename};
  __property AnsiString DriveBDiskFilename = {read=GetDriveBDiskFilename, write=SetDriveBDiskFilename};
  __property AnsiString TapeFilename = {read=GetTapeFilename, write=SetTapeFilename};
  __property AnsiString ProfileDirectory = {read=GetProfileDirectory, write=SetProfileDirectory};
  __property AnsiString DefaultProfileFilename = {read=GetDefaultProfileFilename, write=SetDefaultProfileFilename};
  __property AnsiString RomsDirectory = {read=GetRomsDirectory, write=SetRomsDirectory};
  __property AnsiString LowerROMFilename = {read=GetLowerROMFilename, write=SetLowerROMFilename};
  __property AnsiString UpperROMFilename[int Index] = {read=GetUpperROMFilename, write=SetUpperROMFilename};
  __property AnsiString DiscDirectory = {read=GetDiscDirectory, write=SetDiscDirectory};
  __property AnsiString TapeDirectory = {read=GetTapeDirectory, write=SetTapeDirectory};
  __property AnsiString SnapshotDirectory = {read=GetSnapshotDirectory, write=SetSnapshotDirectory};
  __property AnsiString ScreenshotDirectory = {read=GetScreenshotDirectory, write=SetScreenshotDirectory};
  __property AnsiString CheatScriptsDirectory = {read=GetCheatScriptsDirectory, write=SetCheatScriptsDirectory};
  __property AnsiString ScanlinesDirectory = {read=GetScanlinesDirectory, write=SetScanlinesDirectory};
  __property bool ConfirmDiskImageUpdates = {read=GetConfirmDiskImageUpdates, write=SetConfirmDiskImageUpdates};
  __property bool NeverUpdateDiskImages = {read=GetNeverUpdateDiskImages, write=SetNeverUpdateDiskImages};
  // Graphics
  __property bool Fullscreen = {read=GetFullscreen, write=SetFullscreen};
  __property bool KeepFullscreen = {read=GetKeepFullscreen, write=SetKeepFullscreen};
  __property bool DrawScanlines = {read=GetDrawScanlines, write=SetDrawScanlines};
  __property bool ColorMonitor = {read=GetColorMonitor, write=SetColorMonitor};
  __property bool ControlPanelVisible = {read=GetControlPanelVisible, write=SetControlPanelVisible};
  __property bool UseOpenGL = {read=GetUseOpenGL, write=SetUseOpenGL};
  __property bool ForceGDI = {read=GetForceGDI, write=SetForceGDI};
  __property int OnscreenDisplay = {read=GetOnScreenDisplay, write=SetOnScreenDisplay};
  __property LONG WindowStyle = {read=GetWindowStyle, write=SetWindowStyle};
  __property LONG WindowExStyle = {read=GetWindowExStyle, write=SetWindowExStyle};
  __property int DriveIconSizeFactor = {read=GetDriveIconSizeFactor};
  __property bool ShowStaticDriveIcon = {read=m_ShowStaticDriveIcon};
  __property int FullscreenBrightness = {read=GetFullscreenBrightness, write=SetFullscreenBrightness};
  __property bool FullscreenMenu = {read=GetFullscreenMenu, write=SetFullscreenMenu};
  __property bool KeepAspectRatio = {read=GetKeepAspectRatio, write=SetKeepAspectRatio};
  __property bool ShowPauseInfo = {read=GetShowPauseInfo, write=SetShowPauseInfo};
  __property int ThemeIndex = {read=GetThemeIndex, write=SetThemeIndex};
  __property TColor ThemeBackgroundColor = {read=GetThemeBackgroundColor, write=SetThemeBackgroundColor};
  __property TColor ThemeText1Color = {read=GetThemeText1Color, write=SetThemeText1Color};
  __property TColor ThemeText2Color = {read=GetThemeText2Color, write=SetThemeText2Color};
  __property int VHoldAdjust = {read=GetVHoldAdjust, write=SetVHoldAdjust};
  // Emulator
  __property bool AutoStartEnable = {read=GetAutoStartEnable, write=SetAutoStartEnable};
  __property bool AutoPlayEnable = {read=GetAutoPlayEnable, write=SetAutoPlayEnable};
  __property int CPCDevice = {read=GetCPCDevice, write=SetCPCDevice};
  __property int CPCBrand = {read=GetCPCBrand, write=SetCPCBrand};
  __property int CRTCType = {read=GetCRTCType, write=SetCRTCType};
  __property bool UseFloppyDrive = {read=GetUseFloppyDrive, write=SetUseFloppyDrive};
  __property bool UseFastFDC = {read=GetUseFastFDC, write=SetUseFastFDC};
  __property bool Use64kMemoryExtension = {read=GetUse64kMemoryExtension, write=SetUse64kMemoryExtension};
  __property bool Use256kMemoryExtension = {read=GetUse256kMemoryExtension, write=SetUse256kMemoryExtension};
  __property bool Use256kSiliconDisc = {read=GetUse256kSiliconDisc, write=SetUse256kSiliconDisc};
  __property bool UseTechniMusique = {read=GetUseTechniMusique, write=SetUseTechniMusique};
  __property bool UsePlayCity = {read=GetUsePlayCity, write=SetUsePlayCity};
  __property int MaxSpeedIndex = {read=GetMaxSpeedIndex, write=SetMaxSpeedIndex};
  __property int MaxCustomSpeed = {read=GetMaxCustomSpeed, write=SetMaxCustomSpeed};
  __property int WindowsPriority = {read=GetWindowsPriority};
  __property bool ColdResetAtRealSpeed = {read=GetColdResetAtRealSpeed, write=SetColdResetAtRealSpeed};
  __property bool SmallScreenshotShortcut = {read=GetSmallScreenshotShortcut, write=SetSmallScreenshotShortcut};
  __property bool ShowHints = {read=GetShowHints, write=SetShowHints};
  // Inputs
  __property int PreferredKeyboardLayout = {read=GetPreferredKeyboardLayout, write=SetPreferredKeyboardLayout};
  __property bool UseHardwareKeyboardMapping = {read=GetUseHardwareKeyboardMapping, write=SetUseHardwareKeyboardMapping};
  // Audio
  __property bool Mute = {read=GetMute, write=SetMute};
  __property int AudioGainSpeaker = {read=GetAudioGainSpeaker, write=SetAudioGainSpeaker};
  __property int AudioGainFloppy = {read=GetAudioGainFloppy, write=SetAudioGainFloppy};
  __property int AudioGainTape = {read=GetAudioGainTape, write=SetAudioGainTape};
  __property int AudioGainTMPI = {read=GetAudioGainTMPI, write=SetAudioGainTMPI};
  __property int AudioGainPlayCity = {read=GetAudioGainPlayCity, write=SetAudioGainPlayCity};
  __property int AudioVolume = {read=GetAudioVolume, write=SetAudioVolume};
  __property AnsiString AudioRenderer = {read=GetAudioRenderer, write=SetAudioRenderer};
  __property bool AudioSpeakerStereo = {read=GetAudioSpeakerStereo, write=SetAudioSpeakerStereo};
  __property bool AudioSpeaker16Bits = {read=GetAudioSpeaker16Bits};
  __property int AudioSpeakerFreq = {read=GetAudioSpeakerFreq};
  __property int AudioTMPIFreq = {read=GetAudioTMPIFreq};
  __property int AudioPlayCityFreq = {read=GetAudioPlayCityFreq};
  __property bool AudioPlayCityStereo = {read=GetAudioPlayCityStereo, write=SetAudioPlayCityStereo};
  __property bool AudioPlayCity16Bits = {read=GetAudioPlayCity16Bits};
  __property bool AudioPlayCityInvert = {read=GetAudioPlayCityInvert, write=SetAudioPlayCityInvert};
  // Forms position
  __property int MainFormLeft = {read=GetMainFormLeft, write=SetMainFormLeft};
  __property int MainFormTop = {read=GetMainFormTop, write=SetMainFormTop};
  __property int MainFormWidth = {read=GetMainFormWidth, write=SetMainFormWidth};
  __property int MainFormHeight = {read=GetMainFormHeight, write=SetMainFormHeight};
  __property int DSKViewerFormLeft = {read=GetDSKViewerFormLeft, write=SetDSKViewerFormLeft};
  __property int DSKViewerFormTop = {read=GetDSKViewerFormTop, write=SetDSKViewerFormTop};
  __property int DSKViewerFormWidth = {read=GetDSKViewerFormWidth, write=SetDSKViewerFormWidth};
  __property int DSKViewerFormHeight = {read=GetDSKViewerFormHeight, write=SetDSKViewerFormHeight};
  __property int CRTCFormLeft = {read=GetCRTCFormLeft, write=SetCRTCFormLeft};
  __property int CRTCFormTop = {read=GetCRTCFormTop, write=SetCRTCFormTop};
  __property int CRTCFormWidth = {read=GetCRTCFormWidth, write=SetCRTCFormWidth};
  __property int CRTCFormHeight = {read=GetCRTCFormHeight, write=SetCRTCFormHeight};
  __property int GateArrayFormLeft = {read=GetGateArrayFormLeft, write=SetGateArrayFormLeft};
  __property int GateArrayFormTop = {read=GetGateArrayFormTop, write=SetGateArrayFormTop};
  __property int GateArrayFormWidth = {read=GetGateArrayFormWidth, write=SetGateArrayFormWidth};
  __property int GateArrayFormHeight = {read=GetGateArrayFormHeight, write=SetGateArrayFormHeight};
  __property int FDCFormLeft = {read=GetFDCFormLeft, write=SetFDCFormLeft};
  __property int FDCFormTop = {read=GetFDCFormTop, write=SetFDCFormTop};
  __property int FDCFormWidth = {read=GetFDCFormWidth, write=SetFDCFormWidth};
  __property int FDCFormHeight = {read=GetFDCFormHeight, write=SetFDCFormHeight};
  __property int Z80FormLeft = {read=GetZ80FormLeft, write=SetZ80FormLeft};
  __property int Z80FormTop = {read=GetZ80FormTop, write=SetZ80FormTop};
  __property int Z80FormWidth = {read=GetZ80FormWidth, write=SetZ80FormWidth};
  __property int Z80FormHeight = {read=GetZ80FormHeight, write=SetZ80FormHeight};
  __property int MemoryFormLeft = {read=GetMemoryFormLeft, write=SetMemoryFormLeft};
  __property int MemoryFormTop = {read=GetMemoryFormTop, write=SetMemoryFormTop};
  __property int MemoryFormWidth = {read=GetMemoryFormWidth, write=SetMemoryFormWidth};
  __property int MemoryFormHeight = {read=GetMemoryFormHeight, write=SetMemoryFormHeight};
  __property int PSGFormLeft = {read=GetPSGFormLeft, write=SetPSGFormLeft};
  __property int PSGFormTop = {read=GetPSGFormTop, write=SetPSGFormTop};
  __property int PSGFormWidth = {read=GetPSGFormWidth, write=SetPSGFormWidth};
  __property int PSGFormHeight = {read=GetPSGFormHeight, write=SetPSGFormHeight};
  __property int CDTViewerFormLeft = {read=GetCDTViewerFormLeft, write=SetCDTViewerFormLeft};
  __property int CDTViewerFormTop = {read=GetCDTViewerFormTop, write=SetCDTViewerFormTop};
  __property int CDTViewerFormWidth = {read=GetCDTViewerFormWidth, write=SetCDTViewerFormWidth};
  __property int CDTViewerFormHeight = {read=GetCDTViewerFormHeight, write=SetCDTViewerFormHeight};
  __property int TapeAudioWorkshopFormLeft = {read=GetTapeAudioWorkshopFormLeft, write=SetTapeAudioWorkshopFormLeft};
  __property int TapeAudioWorkshopFormTop = {read=GetTapeAudioWorkshopFormTop, write=SetTapeAudioWorkshopFormTop};
  __property int MEA8000FormLeft = {read=GetMEA8000FormLeft, write=SetMEA8000FormLeft};
  __property int MEA8000FormTop = {read=GetMEA8000FormTop, write=SetMEA8000FormTop};
  __property int MEA8000FormWidth = {read=GetMEA8000FormWidth, write=SetMEA8000FormWidth};
  __property int MEA8000FormHeight = {read=GetMEA8000FormHeight, write=SetMEA8000FormHeight};
  __property int DebuggerFormLeft = {read=GetDebuggerFormLeft, write=SetDebuggerFormLeft};
  __property int DebuggerFormTop = {read=GetDebuggerFormTop, write=SetDebuggerFormTop};
  __property int DebuggerFormWidth = {read=GetDebuggerFormWidth, write=SetDebuggerFormWidth};
  __property int DebuggerFormHeight = {read=GetDebuggerFormHeight, write=SetDebuggerFormHeight};
  __property int PrinterFormLeft = {read=GetPrinterFormLeft, write=SetPrinterFormLeft};
  __property int PrinterFormTop = {read=GetPrinterFormTop, write=SetPrinterFormTop};
  __property int PrinterFormWidth = {read=GetPrinterFormWidth, write=SetPrinterFormWidth};
  __property int PrinterFormHeight = {read=GetPrinterFormHeight, write=SetPrinterFormHeight};
  __property int GraphicsExplorerFormLeft = {read=GetGraphicsExplorerFormLeft, write=SetGraphicsExplorerFormLeft};
  __property int GraphicsExplorerFormTop = {read=GetGraphicsExplorerFormTop, write=SetGraphicsExplorerFormTop};
  __property int GraphicsExplorerFormWidth = {read=GetGraphicsExplorerFormWidth, write=SetGraphicsExplorerFormWidth};
  __property int GraphicsExplorerFormHeight = {read=GetGraphicsExplorerFormHeight, write=SetGraphicsExplorerFormHeight};
  // Tape audio workshop
  __property bool TapeAudioUseDefault = {read=GetTapeAudioUseDefault, write=SetTapeAudioUseDefault};
  __property tUChar TapeAudioPPIThreshold8 = {read=GetTapeAudioPPIThreshold8, write=SetTapeAudioPPIThreshold8};
  __property tShort TapeAudioPPIThreshold16 = {read=GetTapeAudioPPIThreshold16, write=SetTapeAudioPPIThreshold16};
  __property bool TapeAudioAutoGainActive = {read=GetTapeAudioAutoGainActive, write=SetTapeAudioAutoGainActive};
  __property bool TapeAudioLPF1Active = {read=GetTapeAudioLPF1Active, write=SetTapeAudioLPF1Active};
  __property AnsiString TapeAudioLPF1_A = {read=GetTapeAudioLPF1_A, write=SetTapeAudioLPF1_A};
  __property bool TapeAudioLPF2Active = {read=GetTapeAudioLPF2Active, write=SetTapeAudioLPF2Active};
  __property tUChar TapeAudioLPF2Order = {read=GetTapeAudioLPF2Order, write=SetTapeAudioLPF2Order};
  __property tUShort TapeAudioLPF2Freq = {read=GetTapeAudioLPF2Freq, write=SetTapeAudioLPF2Freq};
  __property bool TapeAudioHPF2Active = {read=GetTapeAudioHPF2Active, write=SetTapeAudioHPF2Active};
  __property tUChar TapeAudioHPF2Order = {read=GetTapeAudioHPF2Order, write=SetTapeAudioHPF2Order};
  __property tUShort TapeAudioHPF2Freq = {read=GetTapeAudioHPF2Freq, write=SetTapeAudioHPF2Freq};
  __property bool TapeAudioCreateCSW = {read=GetTapeAudioCreateCSW, write=SetTapeAudioCreateCSW};
  // Printer
  __property int PrinterCountryCode = {read=GetPrinterCountryCode, write=SetPrinterCountryCode};
  __property int PrinterCRLF = {read=GetPrinterCRLF, write=SetPrinterCRLF};
  // AMX Mouse
  __property int AMXMouseFactorX = {read=GetAMXMouseFactorX, write=SetAMXMouseFactorX};
  __property int AMXMouseFactorY = {read=GetAMXMouseFactorY, write=SetAMXMouseFactorY};
  //
  // Methods
  //
  __fastcall TSettings(AnsiString Filename);
  __fastcall ~TSettings();

  TStringList* __fastcall GetInfo(void);
  
  void __fastcall Read(AnsiString Filename);
  bool __fastcall Update(void);

  const tKeyboardMappingKey* __fastcall GetFirstNonASCIIKey(void);
  const tKeyboardMappingKey* __fastcall GetNextNonASCIIKey(void);
  void __fastcall ClearNonASCIIKeys(void);
  void __fastcall AddNonASCIIKey(tKeyboardMappingKey* KeyToAdd);
  void __fastcall RestoreDefaultNonASCIIKeys(void);

  const tKeyboardMappingKey* __fastcall GetFirstHardwareKey(void);
  const tKeyboardMappingKey* __fastcall GetNextHardwareKey(void);
  void __fastcall ClearHardwareKeys(void);
  void __fastcall AddHardwareKey(tKeyboardMappingKey* KeyToAdd);
  void __fastcall RestoreDefaultHardwareKeys(void);

  const tKeyboardMappingKey* __fastcall GetFirstJoystickKey(void);
  const tKeyboardMappingKey* __fastcall GetNextJoystickKey(void);
  void __fastcall ClearJoystickKeys(void);
  void __fastcall AddJoystickKey(tKeyboardMappingKey* KeyToAdd);
  void __fastcall RestoreDefaultJoystickKeys(void);
};


template<class T> void __fastcall TSettings::SetValue(const T& NewValue, T& LastValue)
{
  if (NewValue != LastValue)
  {
    m_Updated = true;
    LastValue = NewValue;
  }
}


#pragma warn +8026

//---------------------------------------------------------------------------
#endif
