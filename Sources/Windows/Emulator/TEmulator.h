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


#ifndef TEmulatorH
#define TEmulatorH

#include "..\..\..\Sources\Engine\Native_CPC.h"
#include "..\Settings\TSettings.h"
#include "..\Drawing\TDrawingContext.h"


#define DISKIMAGE_DSK_EXT           ".dsk"
#define DISKIMAGE_IPF_EXT           ".ipf"
#define DISKIMAGE_RAW_EXT           ".raw"


// Additional errors
#define errFilePermissionDenied           0x1000
#define errFileBadWriteImage              0x1001
#define errFileDoesNotExist               0x1002
#define errBadMemoryAllocation            0x1003
#define errBadInflate                     0x1004
#define errIncorrectParameters            0x1005


// Events types
typedef bool __fastcall (__closure *TEmulatorCPCEvent)(tULong Condition, tNativeCPC* NativeCPC);
typedef void __fastcall (__closure *TEmulatorFDCEvent)(tFDC* FDCP);
typedef void __fastcall (__closure *TEmulatorDiskEvent)(tDrive* DriveP);
typedef void __fastcall (__closure *TEmulatorTapeEvent)(tTape* TapeP);
typedef void __fastcall (__closure *TEmulatorMouseDownEvent)(TMouseButton Button, int X, int Y);
typedef void __fastcall (__closure *TEmulatorMouseUpEvent)(TMouseButton Button);
typedef void __fastcall (__closure *TEmulatorMouseMoveEvent)(int X, int Y);
typedef bool __fastcall (__closure *TEmulatorWINKeyEvent)(WORD &Key, TShiftState Shift);
typedef void __fastcall (__closure *TEmulatorCPCKeyEvent)(tUChar Keycode);
typedef void __fastcall (__closure *TEmulatorLightgunPlugEvent)(void);
typedef void __fastcall (__closure *TEmulatorLightgunTriggerEndEvent)(void);


struct TPressedKey
{
  WORD VirtualKey;
  tUChar cpc_key;
};

enum TDiskDrive
{
  DriveA,
  DriveB
};


class TEmulatorContext
{
public:
  bool AudioEnable;
  
  String  DriveAFilename;
  tDrive  DriveA;

  String  DriveBFilename;
  tDrive  DriveB;

  String  TapeFilename;
  tTape   Tape;
};


class TEmulator
{
private:
  //
  // Members
  //
  TSettings* mSettingsP;
  tUChar* mContextP;
  tContextResources* mResourcesP;
  colours_rgb_entry* mPaletteP;
  tPreferences* mPrefP;
  tNativeCPC* mNativeCPC;
  TResourceStream* mStreamCPCP;
  TResourceStream* mStreamDOSP;
  TResourceStream* mSiliconDiscP;
  tUChar* mAudioSpeakerDataP;
  int mAudioSpeakerSize;
  tUChar* mAudioTMPIDataP;
  int mAudioTMPISize;
  tUChar* mAudioPlayCityDataP;
  int mAudioPlayCitySize;
  tUShort const * mKeyboardCPCP;
  TEmulatorWINKeyEvent mOnWINKeyPress;
  TEmulatorWINKeyEvent mOnWINKeyRelease;
  TEmulatorCPCKeyEvent mOnCPCKeyPress;
  TEmulatorCPCKeyEvent mOnCPCKeyRelease;
  TList* mPressedKeyListP;
  tUChar* mPressedKeyCounterP;
  String mDiskAFilename;
  String mDiskBFilename;
  String mSnapshotFilename;
  String mTapeFilename;
  String mCPCModel;
  bool mPaused;
  bool mUseKeyboardAsJoystick;
  tUChar* mASCIIKeyboardP;
  tUChar* mHardwareKeyboardP;
  tUChar* mJoystickKeyboardP;
  tUChar* mVirtualKeyboardAsJoystickP;
  tUChar* mHardwareKeyboardAsJoystickP;
  TEmulatorLightgunPlugEvent mLightgunPlugEvent;
  TEmulatorLightgunTriggerEndEvent mLightgunTriggerEndEvent;
  TStringList* mDiskCatalogEntries;
  TStringList* mSimpleMessages;
  bool mConfirmDiskImageUpdates;
  bool mNeverUpdateDiskImages;
  // AutoType
  bool mAutoTypeReady;
  char* mAutoTypeBufferP;
  char* mAutoTypeCurrentCharP;
  int mAutoTypeDuration;
  int mAutoTypeStatus;
  tUShort mAutoTypeSHIFTandCTRLState;
  bool mAutoPlayRequested;
  int mAutoPlayDuration;
  int mAutoPlayStatus;
  // Decathlon mode
  bool mDecathlonModeActive;
  int mDecathlonCycleCount;
  int mDecathlonCyclePeriod;
  tUChar mDecathlonCurrentKey;
  // Combat School mode
  bool mCombatSchoolModeActive;
  int mCombatSchoolCycleCount;
  int mCombatSchoolCyclePeriod;
  tUChar mCombatSchoolCurrentKey;
  // SNR playback
  tUChar* mSNRFileP;
  tUChar* mSNRStreamP;
  tUChar* mSNREndStreamP;
  tUChar* mSNRActionP;
  tUChar mSNRActionSize;
  tULong mSNRCount;
  // AMX Mouse
  float mAMXMouseFactorX;
  float mAMXMouseFactorY;


  //
  // Private Methods
  //
  void __fastcall SetSettings(TSettings* SettingsP);
  void __fastcall HandleAutoType(void);
  void __fastcall HandleAutoPlay(void);
  bool __fastcall OnASCIIKeyPress(WORD &VirtualKey, TShiftState Shift);
  bool __fastcall OnHardwareKeyPress(WORD &VirtualKey, TShiftState Shift);
  bool __fastcall OnASCIIKeyRelease(WORD &VirtualKey, TShiftState Shift);
  bool __fastcall OnHardwareKeyRelease(WORD &VirtualKey, TShiftState Shift);
  void __fastcall OnCPCKeycodeKeyPress(tUChar CPCKeycode);
  void __fastcall OnCPCKeycodeKeyRelease(tUChar CPCKeycode);
  tUShort __fastcall GetKeySHIFTandCTRLState(void);
  tUShort __fastcall GetKeySHIFTandCTRLState(tUShort usKeyState);
  void __fastcall SetKeySHIFTandCTRLState(tUShort usKeyState);
  void __fastcall SetASCIIKeySHIFTandCTRLState(void);
  tUShort __fastcall KeyboardGetAsciiSHIFTandCTRL(char key);
  void __fastcall KeyboardSetAsciiKeyDown(char key);
  void __fastcall KeyboardSetAsciiKeyUp(char key);
  void __fastcall KeyboardSetAsciiSHIFTandCTRL(char key);
  inline TEmulatorWINKeyEvent __fastcall GetOnWINKeyPress(void) { return mOnWINKeyPress; };
  inline TEmulatorWINKeyEvent __fastcall GetOnWINKeyRelease(void) { return mOnWINKeyRelease; };
  inline TEmulatorCPCKeyEvent __fastcall GetOnCPCKeyPress(void) { return mOnCPCKeyPress; };
  inline TEmulatorCPCKeyEvent __fastcall GetOnCPCKeyRelease(void) { return mOnCPCKeyRelease; };
  inline tNativeCPC* __fastcall GetNativeCPC(void) const { return mNativeCPC; };
  inline String __fastcall GetCPCModel(void) const { return mCPCModel; };
  inline String __fastcall GetDiskAFilename(void) const { return mDiskAFilename; };
  inline String __fastcall GetDiskBFilename(void) const { return mDiskBFilename; };
  inline String __fastcall GetSnapshotFilename(void) const { return mSnapshotFilename; };
  inline String __fastcall GetTapeFilename(void) const { return mTapeFilename; };
  inline bool __fastcall GetDriveLed(void) const { return mNativeCPC->FDC->led ? true : false; };
  inline bool __fastcall GetPaused(void) { return mPaused; };
  void __fastcall SetPaused(bool Value);
  void __fastcall OnGunStickTriggerEnd(void);
  void __fastcall OnWestPhaserTriggerEnd(void);
  inline bool __fastcall GetFDCModified(void) { return mNativeCPC->FDC->signal_updated ? true : false; };
  void __fastcall SetFDCModified(bool Value);
  inline bool __fastcall GetDriveAModified(void) { return mNativeCPC->FDC->DriveA->signal_updated ? true : false; };
  void __fastcall SetDriveAModified(bool Value);
  inline bool __fastcall GetDriveBModified(void) { return mNativeCPC->FDC->DriveB->signal_updated ? true : false; };
  void __fastcall SetDriveBModified(bool Value);
  inline bool __fastcall GetDriveAAltered(void) { return mNativeCPC->FDC->DriveA->altered ? true : false; };
  inline bool __fastcall GetDriveBAltered(void) { return mNativeCPC->FDC->DriveB->altered ? true : false; };
  inline bool __fastcall GetDriveADoubleSided(void) { return mNativeCPC->FDC->DriveA->sides ? true : false; };
  inline bool __fastcall GetDriveBDoubleSided(void) { return mNativeCPC->FDC->DriveB->sides ? true : false; };
  inline bool __fastcall GetDriveAFlipped(void) { return mNativeCPC->FDC->DriveA->flipped ? true : false; };
  void __fastcall SetDriveAFlipped(bool Value);
  inline bool __fastcall GetDriveBFlipped(void) { return mNativeCPC->FDC->DriveB->flipped ? true : false; };
  void __fastcall SetDriveBFlipped(bool Value);
  bool __fastcall GetDriveAWriteProtected(void);
  void __fastcall SetDriveAWriteProtected(bool WriteProtected);
  bool __fastcall GetDriveBWriteProtected(void);
  void __fastcall SetDriveBWriteProtected(bool WriteProtected);
  bool __fastcall IsStepByStepActive(void);
  inline int __fastcall GetNbCatalogEntries(void) { return mDiskCatalogEntries->Count; };
  AnsiString __fastcall GetCatalogEntry(int Index);
  inline int __fastcall GetNbSimpleMessages(void) { return mSimpleMessages->Count; };
  AnsiString __fastcall GetFirstSimpleMessage(void);
  void __fastcall CreateMessagesFromTape(tTape* TapeP);
  inline tUShort const * __fastcall GetKeyboardCPC(void) { return mKeyboardCPCP; };
  inline bool __fastcall GetTMPIEnabled(void) const { return mNativeCPC->TMPISpeech->enabled ? true : false; };
  inline void __fastcall SetTMPIEnabled(bool Value) const { mNativeCPC->TMPISpeech->enabled = (Value == true) ? 1 : 0; };
  inline bool __fastcall GetPlayCityEnabled(void) const { return mNativeCPC->PlayCity->enabled ? true : false; };
  inline void __fastcall SetPlayCityEnabled(bool Value) const { mNativeCPC->PlayCity->enabled = (Value == true) ? 1 : 0; };
  bool __fastcall GetAutoTypeInProgress(void);
  inline bool __fastcall GetConfirmDiskImageUpdates(void) { return mConfirmDiskImageUpdates; };
  inline void __fastcall SetConfirmDiskImageUpdates(bool Value) { mConfirmDiskImageUpdates = Value; };
  inline bool __fastcall GetNeverUpdateDiskImages(void) { return mNeverUpdateDiskImages; };
  inline void __fastcall SetNeverUpdateDiskImages(bool Value) { mNeverUpdateDiskImages = Value; };
  bool __fastcall ApplySnapshot(void* dataP);
  void __fastcall PlaySNRStream(void);
  bool __fastcall IsPlayingSNRStream(void);

public:
  //
  // Properties
  //
  __property tNativeCPC* NativeCPC = {read=GetNativeCPC};
  __property TSettings* Settings = {write=SetSettings};
  __property String CPCModel = {read=GetCPCModel};
  __property String DiskAFilename = {read=GetDiskAFilename};
  __property String DiskBFilename = {read=GetDiskBFilename};
  __property String SnapshotFilename = {read=GetSnapshotFilename};
  __property String TapeFilename = {read=GetTapeFilename};
  __property bool DriveLed = {read=GetDriveLed};
  __property bool Paused = {read=GetPaused, write=SetPaused};
  __property TEmulatorWINKeyEvent OnWINKeyPress = { read=GetOnWINKeyPress };
  __property TEmulatorWINKeyEvent OnWINKeyRelease = { read=GetOnWINKeyRelease };
  __property TEmulatorCPCKeyEvent OnCPCKeyPress = { read=GetOnCPCKeyPress };
  __property TEmulatorCPCKeyEvent OnCPCKeyRelease = { read=GetOnCPCKeyRelease };
  __property bool FDCModified = { read=GetFDCModified, write=SetFDCModified };
  __property bool DriveAModified = { read=GetDriveAModified, write=SetDriveAModified };
  __property bool DriveBModified = { read=GetDriveBModified, write=SetDriveBModified };
  __property bool DriveAAltered = { read=GetDriveAAltered };
  __property bool DriveBAltered = { read=GetDriveBAltered };
  __property bool DriveADoubleSided = { read=GetDriveADoubleSided };
  __property bool DriveBDoubleSided = { read=GetDriveBDoubleSided };
  __property bool DriveAFlipped = { read=GetDriveAFlipped, write=SetDriveAFlipped };
  __property bool DriveBFlipped = { read=GetDriveBFlipped, write=SetDriveBFlipped };
  __property bool DriveAWriteProtected = { read=GetDriveAWriteProtected, write=SetDriveAWriteProtected };
  __property bool DriveBWriteProtected = { read=GetDriveBWriteProtected, write=SetDriveBWriteProtected };
  __property bool StepByStepActive = { read=IsStepByStepActive };
  __property int NbCatalogEntries = { read=GetNbCatalogEntries };
  __property String CatalogEntry[int Index] = { read=GetCatalogEntry };
  __property int NbSimpleMessages = { read=GetNbSimpleMessages };
  __property String SimpleMessage = { read=GetFirstSimpleMessage };
  __property tUShort const * KeyboardCPC = { read=GetKeyboardCPC };
  __property bool TMPIEnabled = { read=GetTMPIEnabled, write=SetTMPIEnabled };
  __property bool PlayCityEnabled = { read=GetPlayCityEnabled, write=SetPlayCityEnabled };
  __property bool AutoTypeInProgress = { read=GetAutoTypeInProgress };
  __property bool ConfirmDiskImageUpdates = { read=GetConfirmDiskImageUpdates, write=SetConfirmDiskImageUpdates };
  __property bool NeverUpdateDiskImages = { read=GetNeverUpdateDiskImages, write=SetNeverUpdateDiskImages };
  __property bool PlayingSNRStream = { read=IsPlayingSNRStream };
  //
  // API
  //
  __fastcall TEmulator(TSettings* SettingsP, TDrawingContext* DrawingContextP);
  __fastcall ~TEmulator();
  
  int __fastcall GetPalette(TColor* PaletteP);
  Err __fastcall ToggleNightMode(void);
  Err __fastcall SetScreenType(bool ScreenType);

  Err __fastcall Start(void);
  Err __fastcall Stop(void);
  Err __fastcall Reset(void);
  Err __fastcall Restart(TSettings* SettingsP);
  Err __fastcall Execute(void);

  Err __fastcall Save(TEmulatorContext* ContextP);
  Err __fastcall Restore(const TEmulatorContext* ContextP);

  void __fastcall SwapDrives(void);
  Err  __fastcall LoadDisk(AnsiString Filename, TDiskDrive drive);
  Err  __fastcall CreateDisk(AnsiString Filename, TDiskDrive drive, tUChar FormatType);
  Err  __fastcall CreateVirtualDisk(AnsiString Filename, TDiskDrive drive, tUChar FormatType);
  Err  __fastcall SaveDisk(AnsiString Filename, TDiskDrive drive);
  Err  __fastcall EjectDisk(TDiskDrive drive);
  Err  __fastcall SaveAndEjectDisk(TDiskDrive drive);
  Err  __fastcall UpdateDisk(TDiskDrive drive);
  AnsiString  __fastcall DetectAutoStart(TDiskDrive drive);
  void __fastcall RUNFilename(AnsiString Filename);
  void __fastcall LOADFilename(AnsiString Filename);
  void __fastcall ExecuteCPM(void);
  int  __fastcall ReadDiskCatalog(TDiskDrive drive);
  void __fastcall ForceDiskAFilename(String Filename);
  void __fastcall ForceDiskBFilename(String Filename);

  const void* __fastcall GetAudioEmulatorData(void);
  int   __fastcall GetAudioEmulatorDataSize(void);
  const void* __fastcall GetAudioTMPIData(void);
  int   __fastcall GetAudioTMPIDataSize(void);
  const void* __fastcall GetAudioPlayCityData(void);
  int   __fastcall GetAudioPlayCityDataSize(void);

  void __fastcall AudioStart(void);
  void __fastcall AudioStop(void);
  bool __fastcall IsAudioActive(void);

  void __fastcall StartStepByStep(void);
  void __fastcall StopStepByStep(void);

  void __fastcall StartKeyboardAsJoystick(void);
  void __fastcall StopKeyboardAsJoystick(void);

  void __fastcall PlugMagnumLightPhaser(void);
  void __fastcall UnplugMagnumLightPhaser(void);
  void __fastcall OnMagnumLightPhaserPressed(void);
  void __fastcall OnMagnumLightPhaserReleased(void);
  void __fastcall PlugGunStick(void);
  void __fastcall UnplugGunStick(void);
  void __fastcall OnGunStickPressed(void);
  void __fastcall OnGunStickReleased(void);
  void __fastcall PlugWestPhaser(void);
  void __fastcall UnplugWestPhaser(void);
  void __fastcall OnWestPhaserPressed(void);
  void __fastcall OnWestPhaserReleased(void);
  void __fastcall PlugAMXMouse(void);
  void __fastcall UnplugAMXMouse(void);
  void __fastcall ResetAMXMouse(void);
  void __fastcall SetAMXMouseXFactor(float factor);
  void __fastcall SetAMXMouseYFactor(float factor);
  void __fastcall OnAMXMouseButtonLPressed(void);
  void __fastcall OnAMXMouseButtonLReleased(void);
  void __fastcall OnAMXMouseButtonMPressed(void);
  void __fastcall OnAMXMouseButtonMReleased(void);
  void __fastcall OnAMXMouseButtonRPressed(void);
  void __fastcall OnAMXMouseButtonRReleased(void);

  void __fastcall OnLightpenMove(int X, int Y);
  void __fastcall OnPhaserMove(int X, int Y);
  void __fastcall OnMouseMove(int X, int Y);

  bool __fastcall SaveSnapshot(String Filename);
  bool __fastcall UpdateSnapshot(void);
  bool __fastcall LoadSnapshot(String Filename);
  bool __fastcall PlaySNRSession(String Filename);
  bool __fastcall ReloadSnapshot(void);
  void __fastcall UnloadSnapshot(void);
  void __fastcall StopSNRSession(void);

  void __fastcall UpdateKeyboardMapping(void);

  tUChar* __fastcall LoadROM(String ROMFilename);
  int __fastcall AddROM(int Slot, String Filename);
  //void __fastcall RemoveROM(int Slot);
  //AnsiString __fastcall GetROMFilename(int Slot);

  void __fastcall EjectTape(void);
  void __fastcall RewindTape(void);
  Err  __fastcall InsertTape(String Filename);
  Err  __fastcall InsertTapeCDT(String Filename,
                                tUChar* BlockDataP,
                                tULong Length);
  Err  __fastcall InsertTapeCSW(String Filename,
                                tUChar* BlockDataP,
                                tULong Length,
                                tULong SampleRate,
                                tUChar StartPolarity);
  void __fastcall PlayTape(void);
  void __fastcall StopTape(void);
  bool __fastcall IsTapePlaying(void);
  void __fastcall TapeSetCurrentBlock(int NewBlock);
  void __fastcall ResetTapeCounterTo000(void);
  void __fastcall RewindToTapeCounter000(void);
  int  __fastcall GetTapePlayDuration(void);
  int  __fastcall GetTapeTotalDuration(void);
  int  __fastcall GoToPreviousMessage(void);
  int  __fastcall GoToNextMessage(void);
  bool __fastcall IsTapeNavigationAllowed(void);

  static bool __fastcall PrepareDirectCDTUseSettings(TSettings* SettingsP);
  static bool __fastcall PrepareDirectDSKUseSettings(TSettings* SettingsP);

  void __fastcall StartDecathlonMode(void);
  void __fastcall StopDecathlonMode(void);

  void __fastcall StartCombatSchoolMode(void);
  void __fastcall StopCombatSchoolMode(void);

  void __fastcall StartAutoType(String Text);
  void __fastcall StartAutoPlay(void);

  void __fastcall SaveContext(String Filename);

  void __fastcall SetPrinterOnline(void);
  void __fastcall SetPrinterOffline(void);
};

//---------------------------------------------------------------------------
#endif

