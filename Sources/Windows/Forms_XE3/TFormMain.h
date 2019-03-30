/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2018 by Frédéric Coste

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


#ifndef TFormMainH
#define TFormMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include "..\Settings\TSettings.h"
#include "..\Emulator\TEmulator.h"
#include "..\Drawing\TDrawingContext.h"
#include "..\Audio\TAudioContext.h"
#include "..\Dialogs\TDialogInfo.h"
#include "..\Inputs\TInputContext.h"
#include "..\Time\TBaseTimer.h"
//---------------------------------------------------------------------------

class TFormMain : public TForm
{
__published:	// Composants gérés par l'EDI
  TPanel *PanelCPCScreen;
  TOpenDialog *OpenDialog;
  TMainMenu *MainMenu;
  TMenuItem *DrivesMenu;
  TMenuItem *LoadimageintoDriveAMenu;
  TMenuItem *LoadimageintoDriveBMenu;
  TMenuItem *SwapDrivesMenu;
  TMenuItem *EjectbothMenu;
  TMenuItem *EmulatorMenu;
  TMenuItem *ExitMenu;
  TMenuItem *N1;
  TMenuItem *ToggleCPUSpeedMenu;
  TMenuItem *ResetMenu;
  TMenuItem *HelpMenu;
  TMenuItem *DisplayMenu;
  TMenuItem *EnlargeDisplayMenu;
  TMenuItem *ReduceDisplayMenu;
  TMenuItem *ExtraMenu;
  TMenuItem *N3;
  TMenuItem *EmulatorSettingsMenu;
  TMenuItem *N4;
  TMenuItem *NightmodeMenu;
  TMenuItem *ControlPanelMenu;
  TMenuItem *N5;
  TMenuItem *CRTCRegistersMenu;
  TMenuItem *N6;
  TMenuItem *PauseMenu;
  TMenuItem *MuteMenu;
  TMenuItem *FDCOperationsMenu;
  TSaveDialog *SaveDialog;
  TMenuItem *InputsMenu;
  TMenuItem *ToggleColorMonochromeMenu;
  TMenuItem *N8;
  TMenuItem *MouseAsLightpenMenu;
  TMenuItem *EnableAutoStartMenu;
  TMenuItem *MouseAsMagnumLightPhaserMenu;
  TMenuItem *MouseAsGunStickMenu;
  TMenuItem *FullscreenMenu;
  TPanel *PanelControl;
  TMenuItem *GameControllerSeparator;
  TMenuItem *GameControllersSettingsMenu;
  TMenuItem *LoadDefaultProfileMenu;
  TMenuItem *LoadDriveAProfile;
  TMenuItem *N7;
  TMenuItem *SmallScreenshotMenu;
  TMenuItem *ScreenshotMenu;
  TMenuItem *N11;
  TMenuItem *SaveSnapshotMenu;
  TMenuItem *UpdateSnapshotMenu;
  TMenuItem *LoadSnapshotMenu;
  TMenuItem *ReloadSnapshotMenu;
  TMenuItem *N12;
  TMenuItem *KeyboardMappingMenu;
  TMenuItem *MouseAsWestPhaserMenu;
  TMenuItem *N2;
  TMenuItem *AboutMenu;
  TMenuItem *N13;
  TMenuItem *Hints1;
  TMenuItem *GateArrayRegistersMenu;
  TMenuItem *DriveACatalogMenu;
  TMenuItem *N14;
  TMenuItem *Z80EditorMenu;
  TMenuItem *MemoryEditorMenu;
  TMenuItem *TapeMenu;
  TMenuItem *PlayTapeMenu;
  TMenuItem *RewindTapeMenu;
  TMenuItem *N15;
  TMenuItem *InsertTapeMenu;
  TMenuItem *EjectTapeMenu;
  TMenuItem *PSGOperationsMenu;
  TMenuItem *ResetCounterTo000Menu;
  TMenuItem *RewindToCounter000Menu;
  TMenuItem *N17;
  TMenuItem *GoToPreviousTagMenu;
  TMenuItem *GoToNextTagMenu;
  TMenuItem *TextCaptureMenu;
  TMenuItem *TapeAudioWorkshopMenu;
  TMenuItem *EnableAutoPlayAtInsertMenu;
  TMenuItem *N18;
  TPanel *PanelFloattingControl;
  TSpeedButton *SpeedButtonControlReset;
  TSpeedButton *SpeedButtonControlMute;
  TPanel *PanelControlTape;
  TSpeedButton *SpeedButtonControlTape;
  TSpeedButton *SpeedButtonControlTapeRewind;
  TSpeedButton *SpeedButtonControlTapePrevious;
  TSpeedButton *SpeedButtonControlTapePlayStop;
  TSpeedButton *SpeedButtonControlTapeNext;
  TPanel *PanelControlDrives;
  TSpeedButton *SpeedButtonControlDriveA;
  TPanel *PanelControlDriveAPosition;
  TSpeedButton *SpeedButtonControlSwapDrives;
  TSpeedButton *SpeedButtonControlDriveB;
  TPanel *PanelControlDriveBPosition;
  TMenuItem *N19;
  TMenuItem *AutoTypeFromClipboardMenu;
  TMenuItem *MEA8000Menu;
  TMenuItem *VolumeUpMenu;
  TMenuItem *VolumeDownMenu;
  TMenuItem *PrinterMenu;
  TMenuItem *UnloadSnapshotMenu;
  TMenuItem *OpenScreenshotsFolderMenu;
  TMenuItem *Maximumspeed1;
  TMenuItem *FullSpeedMenu;
  TMenuItem *Speed200Menu;
  TMenuItem *Speed300Menu;
  TMenuItem *CustomSpeedMenu;
  TMenuItem *N9;
  TMenuItem *SetCustomSpeedMenu;
  TMenuItem *AudioSettingsMenu;
        TMenuItem *DebuggerMenu;
  TMenuItem *DisplaySettingsMenu;
  TMenuItem *DriveAMenu;
  TMenuItem *DriveAEjectMenu;
  TMenuItem *DriveBMenu;
  TMenuItem *DriveBEjectMenu;
  TMenuItem *DriveAWriteProtectionMenu;
  TMenuItem *DriveBWriteProtectionMenu;
  TMenuItem *N10;
  TMenuItem *DriveACreateDATADiskMenu;
  TMenuItem *DriveBCreateDATADiskMenu;
  TMenuItem *N20;
  TMenuItem *N21;
  TMenuItem *ConfirmDiskImagesUpdatesMenu;
  TMenuItem *N22;
  TMenuItem *N23;
  TMenuItem *UseHardwareKeyboardMappingMenu;
  TMenuItem *SaveContextMenu;
  TMenuItem *DriveAFlipSidesMenu;
  TMenuItem *N24;
  TMenuItem *DriveBFlipSidesMenu;
  TMenuItem *DriveAUpdateDiskImageMenu;
  TMenuItem *DriveBUpdateDiskImageMenu;
  TPopupMenu *DrivePopupMenu;
  TMenuItem *PopupDriveFlipSidesMenu;
  TMenuItem *PopupDriveCreateDATADiskMenu;
  TMenuItem *N16;
  TMenuItem *PopupDriveUpdateDiskImageMenu;
  TMenuItem *PopupDriveReloadDiskImageMenu;
  TMenuItem *N25;
  TMenuItem *DriveAReloadDiskImageMenu;
  TMenuItem *DriveBReloadDiskImageMenu;
  TMenuItem *N26;
  TMenuItem *PlaybackSessionMenu;
  TMenuItem *N27;
  TMenuItem *N28;
  TMenuItem *DiskViewerMenu2;
  TMenuItem *N29;
  TMenuItem *TapeViewerMenu2;
  TMenuItem *AutoPlayTapeMenu;
  TMenuItem *CheatScriptsMenu;
  TMenuItem *OpenCapriceFolderMenu;
  TMenuItem *DriveALoadRecentImageMenu;
  TMenuItem *DriveALoadFavouriteImageMenu;
  TMenuItem *DriveAAddToFavouriteMenu;
  TMenuItem *PopupDriveAddToFavouriteMenu;
  TMenuItem *DriveBAddToFavouriteMenu;
  TMenuItem *N30;
  TMenuItem *N31;
  TMenuItem *N32;
  TMenuItem *NeverUpdateDiskImagesMenu;
  TMenuItem *MouseAsAMXMouseMenu;
  TMenuItem *N33;
  TMenuItem *AMXMouseSpeedMenu;
  TMenuItem *AMXMouseSpeed100Menu;
  TMenuItem *AMXMouseSpeed200Menu;
  TPopupMenu *PopupMenuScreen;
  TMenuItem *AutotypefromClipboard1;
  TMenuItem *ShowHintsMenu;
  TMenuItem *GraphicsExplorerMenu;
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ExitMenuClick(TObject *Sender);
  void __fastcall LoadimageintoDriveAMenuClick(TObject *Sender);
  void __fastcall LoadimageintoDriveBMenuClick(TObject *Sender);
  void __fastcall SwapDrivesMenuClick(TObject *Sender);
  void __fastcall ToggleCPUSpeedMenuClick(TObject *Sender);
  void __fastcall EjectbothMenuClick(TObject *Sender);
  void __fastcall ResetMenuClick(TObject *Sender);
  void __fastcall EnlargeDisplayMenuClick(TObject *Sender);
  void __fastcall ReduceDisplayMenuClick(TObject *Sender);
  void __fastcall NightmodeMenuClick(TObject *Sender);
  void __fastcall ControlPanelMenuClick(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall CRTCRegistersMenuClick(TObject *Sender);
  void __fastcall PauseMenuClick(TObject *Sender);
  void __fastcall MuteMenuClick(TObject *Sender);
  void __fastcall FDCOperationsMenuClick(TObject *Sender);
  void __fastcall ToggleColorMonochromeMenuClick(TObject *Sender);
  void __fastcall PanelCPCScreenMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
  void __fastcall EnableAutoStartMenuClick(TObject *Sender);
  void __fastcall PanelCPCScreenMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall PanelCPCScreenMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall MouseAsMagnumLightPhaserMenuClick(TObject *Sender);
  void __fastcall MouseAsGunStickMenuClick(TObject *Sender);
  void __fastcall FullscreenMenuClick(TObject *Sender);
  void __fastcall AboutMenuClick(TObject *Sender);
  void __fastcall GameControllersSettingsMenuClick(TObject *Sender);
  void __fastcall LoadDefaultProfileMenuClick(TObject *Sender);
  void __fastcall LoadDriveAProfileClick(TObject *Sender);
  void __fastcall SmallScreenshotMenuClick(TObject *Sender);
  void __fastcall ScreenshotMenuClick(TObject *Sender);
  void __fastcall SaveSnapshotMenuClick(TObject *Sender);
  void __fastcall UpdateSnapshotMenuClick(TObject *Sender);
  void __fastcall LoadSnapshotMenuClick(TObject *Sender);
  void __fastcall ReloadSnapshotMenuClick(TObject *Sender);
  void __fastcall KeyboardMappingMenuClick(TObject *Sender);
  void __fastcall PanelCPCScreenResize(TObject *Sender);
  void __fastcall EmulatorSchedulingMenuClick(TObject *Sender);
  void __fastcall MouseAsLightpenMenuClick(TObject *Sender);
//  void __fastcall KeyboardAsJoystickMenuClick(TObject *Sender);
  void __fastcall MouseAsWestPhaserMenuClick(TObject *Sender);
  void __fastcall LightgunMenuClick(TObject *Sender);
  void __fastcall DSKViewerMenuClick(TObject *Sender);
  void __fastcall HintsMenuClick(TObject *Sender);
  void __fastcall GateArrayRegistersMenuClick(TObject *Sender);
  void __fastcall DriveACatalogMenuClick(TObject *Sender);
  void __fastcall Z80EditorMenuClick(TObject *Sender);
  void __fastcall MemoryEditorMenuClick(TObject *Sender);
  void __fastcall RewindTapeMenuClick(TObject *Sender);
  void __fastcall EjectTapeMenuClick(TObject *Sender);
  void __fastcall InsertTapeMenuClick(TObject *Sender);
  void __fastcall PlayTapeMenuClick(TObject *Sender);
  void __fastcall PSGOperationsMenuClick(TObject *Sender);
  void __fastcall CDTViewerMenuClick(TObject *Sender);
  void __fastcall ResetCounterTo000MenuClick(TObject *Sender);
  void __fastcall RewindToCounter000MenuClick(TObject *Sender);
  void __fastcall GoToPreviousTagMenuClick(TObject *Sender);
  void __fastcall GoToNextTagMenuClick(TObject *Sender);
  void __fastcall TextCaptureMenuClick(TObject *Sender);
  void __fastcall TapeAudioWorkshopMenuClick(TObject *Sender);
  void __fastcall EnableAutoPlayAtInsertMenuClick(TObject *Sender);
//  void __fastcall DecathlonModeMenuClick(TObject *Sender);
  void __fastcall PanelControlResize(TObject *Sender);
  void __fastcall SpeedButtonControlDriveAClick(TObject *Sender);
  void __fastcall SpeedButtonControlDriveBClick(TObject *Sender);
  void __fastcall SpeedButtonControlSwapDrivesClick(TObject *Sender);
  void __fastcall SpeedButtonControlResetClick(TObject *Sender);
  void __fastcall SpeedButtonControlTapeClick(TObject *Sender);
  void __fastcall SpeedButtonControlTapeRewindClick(TObject *Sender);
  void __fastcall SpeedButtonControlTapePreviousClick(TObject *Sender);
  void __fastcall SpeedButtonControlTapePlayStopClick(TObject *Sender);
  void __fastcall SpeedButtonControlTapeNextClick(TObject *Sender);
  void __fastcall SpeedButtonControlMuteClick(TObject *Sender);
  void __fastcall AutoTypeFromClipboardMenuClick(TObject *Sender);
  void __fastcall MEA8000MenuClick(TObject *Sender);
  void __fastcall VolumeUpMenuClick(TObject *Sender);
  void __fastcall VolumeDownMenuClick(TObject *Sender);
  void __fastcall PrinterMenuClick(TObject *Sender);
  void __fastcall EmulatorSettingsMenuClick(TObject *Sender);
  void __fastcall UnloadSnapshotMenuClick(TObject *Sender);
  void __fastcall PanelCPCScreenMouseClick(TObject *Sender);
  void __fastcall PanelCPCScreenMouseDblClick(TObject *Sender);
  void __fastcall OpenScreenshotsFolderMenuClick(TObject *Sender);
  void __fastcall FileEditorMenuClick(TObject *Sender);
  void __fastcall FullSpeedMenuClick(TObject *Sender);
  void __fastcall Speed200MenuClick(TObject *Sender);
  void __fastcall Speed300MenuClick(TObject *Sender);
  void __fastcall CustomSpeedMenuClick(TObject *Sender);
  void __fastcall SetCustomSpeedMenuClick(TObject *Sender);
  void __fastcall AudioSettingsMenuClick(TObject *Sender);
//  void __fastcall CombatSchoolModeMenuClick(TObject *Sender);
  void __fastcall DebuggerMenuClick(TObject *Sender);
  void __fastcall DisplaySettingsMenuClick(TObject *Sender);
  void __fastcall DriveAEjectMenuClick(TObject *Sender);
  void __fastcall DriveBEjectMenuClick(TObject *Sender);
  void __fastcall DriveAWriteProtectionMenuClick(TObject *Sender);
  void __fastcall DriveBWriteProtectionMenuClick(TObject *Sender);
  void __fastcall PanelControlDriveAPositionClick(TObject *Sender);
  void __fastcall PanelControlDriveBPositionClick(TObject *Sender);
  void __fastcall DriveACreateDATADiskMenuClick(TObject *Sender);
  void __fastcall DriveBCreateDATADiskMenuClick(TObject *Sender);
  void __fastcall ConfirmDiskImagesUpdatesMenuClick(TObject *Sender);
  void __fastcall FormCanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize);
  void __fastcall UseHardwareKeyboardMappingMenuClick(TObject *Sender);
  void __fastcall SaveContextMenuClick(TObject *Sender);
  void __fastcall EngineDataMenuClick(TObject *Sender);
  void __fastcall DriveAFlipSidesMenuClick(TObject *Sender);
  void __fastcall DriveBFlipSidesMenuClick(TObject *Sender);
  void __fastcall DriveAUpdateDiskImageMenuClick(TObject *Sender);
  void __fastcall DriveBUpdateDiskImageMenuClick(TObject *Sender);
  void __fastcall DrivePopupMenuPopup(TObject *Sender);
  void __fastcall DriveAReloadDiskImageMenuClick(TObject *Sender);
  void __fastcall DriveBReloadDiskImageMenuClick(TObject *Sender);
  void __fastcall PlaybackSessionMenuClick(TObject *Sender);
  void __fastcall AutoPlayTapeMenuClick(TObject *Sender);
  void __fastcall CheatScriptsMenuClick(TObject *Sender);
  void __fastcall OpenCapriceFolderMenuClick(TObject *Sender);
  void __fastcall DriveAAddToFavouriteMenuClick(TObject *Sender);
  void __fastcall DriveBAddToFavouriteMenuClick(TObject *Sender);
  void __fastcall NeverUpdateDiskImagesMenuClick(TObject *Sender);
  void __fastcall MouseAsAMXMouseMenuClick(TObject *Sender);
  void __fastcall AMXMouseSpeed100MenuClick(TObject *Sender);
  void __fastcall AMXMouseSpeed200MenuClick(TObject *Sender);
  void __fastcall ShowHintsMenuClick(TObject *Sender);
  void __fastcall GraphicsExplorerMenuClick(TObject *Sender);
  void __fastcall PanelCPCScreenMouseEnter(TObject *Sender);
  void __fastcall ROMOperationsMenuClick(TObject *Sender);

private:	// Déclarations utilisateur
  //
  // Members
  //
  TBaseTimer* mBaseTimerP;
  TEmulator* mEmulatorP;
  TSettings* mSettingsP;
  TDialogInfo* mInfoFormP;
  TDrawingContext* mDrawingContextP;
  TAudioContext* mAudioContextP;
  TInputContext* mInputContextP;
  tULong mExecuteCondition;
  bool mMouseNecessary;
  AnsiString mApplicationTitle;
  bool mScreenSaverActive;
  LONG mFullScreenOldStyle;
  LONG mFullScreenOldExStyle;
  int mFullScreenOldTop;
  int mFullScreenOldLeft;
  int mFullScreenOldWidth;
  int mFullScreenOldHeight;
  int mFullScreenControlPanelVisible;
  int mEmulatorPerformance;
  int mCompteurCycleSecond;
  bool mFullSpeed;
  TCursor mOldCursor;
  TList* mEmulatorMessages;
  bool mRunTillNextLightgunTrigger;
  TShiftState mShiftState;
  bool mDoNotSaveSettings;
  bool mShowOverridenSettings;
  AnsiString mStartSnapshotFilename;
  AnsiString mStartROMFilename;
  AnsiString mStartAutoTypeCommand;
  AnsiString mStartScriptFilename;
  bool mFDCMotorState;
  bool mTapeMotorState;
  TDialogForm* mFullscreenDialogForm;
  bool mUpdateControlPanel;
  int mFPSCounter;
  int mFPSDisplayed;
  int mFormFrameWidth;
  int mFormFrameHeight;
  bool mCheckAutoRunAtStartup;
  bool mCheckAutoPlayAtStartup;
  bool mKeyboardAsJoystickAtStartup;
  bool mFullscreenAtStartup;
  bool mWindowedAtStartup;
  bool mTurboAtStartup;
  TStringList* mRecentDiskFilenames;
  TStringList* mFavouriteDiskFilenames;
  bool mQuietMode;
  // Events
  TList* mOnEmulatorCPCEventsList;
  TList* mOnEmulatorFDCEventList;
  TList* mOnEmulatorDiskEventList;
  TList* mOnEmulatorTapeEventList;
  TEmulatorMouseDownEvent OnEmulatorMouseDown;
  TEmulatorMouseUpEvent OnEmulatorMouseUp;
  TEmulatorMouseMoveEvent OnEmulatorMouseMove;
  TIdleEvent mOldIdleEvent;
  TIdleEvent mIdleEventBeforePause;
  //
  // Methods
  //
  void __fastcall OnApplicationDeactivate(TObject* Sender);
  void __fastcall AppMessage(tagMSG &Msg, bool &Handled);
  void __fastcall WMMove(TMessage &Message);
  void __fastcall WMKeyDown(TWMKey &Message);
  void __fastcall WMKeyUp(TWMKey &Message);

  bool __fastcall InitInputs(void);
  void __fastcall OnIdleNormalSpeed(TObject *Sender, bool &Done);
  void __fastcall OnIdleFullSpeed(TObject *Sender, bool &Done);
  void __fastcall OnIdlePaused(TObject *Sender, bool &Done);

  bool __fastcall ExecuteSoundCondition(void);
  void __fastcall UpdateSoundBuffers(void);
  inline void __fastcall ExecuteVDUCondition(void);
  inline void __fastcall ExecutePSGCondition(void);

  void __fastcall DropFile(TWMDropFiles &Message); // drag & drop handler

  void __fastcall SetAudioSettings();

  inline TSettings* __fastcall GetSettings(void) { return mSettingsP; };
  inline TEmulator* __fastcall GetEmulator(void) { return mEmulatorP; };
  inline tNativeCPC* __fastcall GetNativeCPC(void) { return mEmulatorP->NativeCPC; };
  inline TDrawingContext* __fastcall GetDrawingContext(void) { return mDrawingContextP; };

  void __fastcall AudioRendererMenuClick(TObject *Sender);
  void __fastcall OnRemoveGameController(AnsiString Name);

  bool __fastcall OnWINInputKeyDown(WORD &Key, TShiftState Shift);
  bool __fastcall OnEmulatorKeyDown(WORD &Key);
  bool __fastcall OnEmulatorKeyDownWithAlt(WORD &Key);
  bool __fastcall OnEmulatorKeyDownWithCtrl(WORD &Key);
  bool __fastcall OnEmulatorKeyDownWithShift(WORD &Key);
  bool __fastcall OnEmulatorKeyDownWithCtrlAndShift(WORD &Key);
  bool __fastcall OnWINInputKeyUp(WORD &Key, TShiftState Shift);
  void __fastcall OnCPCInputKeyDown(tUChar Key);
  void __fastcall OnCPCInputKeyUp(tUChar Key);

  void __fastcall OnInputMouseDown(TMouseButton Button, int X, int Y);
  void __fastcall OnInputMouseUp(TMouseButton Button, int X, int Y);
  void __fastcall OnInputMouseMove(int X, int Y);
  void __fastcall OnInputMouseClick(int X, int Y);
  void __fastcall OnInputMouseDblClick(int X, int Y);
  void __fastcall OnLightpenMove(int X, int Y);
  void __fastcall OnMagnumLightPhaserPressed(TMouseButton Button, int X, int Y);
  void __fastcall OnMagnumLightPhaserReleased(TMouseButton Button);
  void __fastcall OnMagnumLightPhaserMove(int X, int Y);
  void __fastcall OnGunStickPressed(TMouseButton Button, int X, int Y);
  void __fastcall OnGunStickReleased(TMouseButton Button);
  void __fastcall OnGunStickMove(int X, int Y);
  void __fastcall OnWestPhaserPressed(TMouseButton Button, int X, int Y);
  void __fastcall OnWestPhaserReleased(TMouseButton Button);
  void __fastcall OnWestPhaserMove(int X, int Y);
  void __fastcall OnAMXMousePressed(TMouseButton Button, int X, int Y);
  void __fastcall OnAMXMouseReleased(TMouseButton Button);
  void __fastcall OnAMXMouseMove(int X, int Y);

  void __fastcall SetFullscreen(void);
  void __fastcall SetWindowed(void);

  void __fastcall AddEmulatorMessage(String NewMessage);
  void __fastcall AddEmulatorMessage(String NewMessage, int Duration);
  void __fastcall RemoveEmulatorMessage(int IndexToRemove);
  void __fastcall DisplayEmulatorMessage(void);

  void __fastcall OnFullscreenOpenDSKDialogEnd(TDialogFormEndCause EndCause);
  void __fastcall OnFullscreenOpenSNADialogEnd(TDialogFormEndCause EndCause);
  void __fastcall OnFullscreenOpenCDTDialogEnd(TDialogFormEndCause EndCause);
  void __fastcall OnFullscreenDiskCatalogDialogEnd(TDialogFormEndCause EndCause);

  void __fastcall OnEmulatorNextLightgunTrigger(void);

  void __fastcall MaximizeEmulatorEndEventPriority(TEmulatorCPCEvent Callback);

  void __fastcall OnDiskEvent(tDrive* DriveP);
  void __fastcall OnFDCEvent(tFDC* FDC);

  void __fastcall ResetEmulator(void);

  void __fastcall SetRealCPCSpeed(void);
  void __fastcall SetFullSpeed(void);
  void __fastcall SetSpeed300Percent(void);
  void __fastcall SetSpeed200Percent(void);
  void __fastcall SetSpeedCustom(void);

  int __fastcall GetCommandParamList(TStringList* ParamList);

  void __fastcall AddRecentDisk(AnsiString Filename);
  void __fastcall AddFavouriteDisk(AnsiString Filename);
  void __fastcall LoadDriveAStoredImageMenuClick(TObject *Sender);

  void __fastcall UpdateDriveAMenu(void);
  void __fastcall UpdateDriveBMenu(void);

  void __fastcall UpdateShowHints(void);

public:		// Déclarations utilisateur
  //
  // Properties
  //
  __property TSettings* SettingsP = {read=GetSettings};
  __property TEmulator* EmulatorP = {read=GetEmulator};
  __property tNativeCPC* NativeCPC = {read=GetNativeCPC};
  __property TDrawingContext* DrawingContextP = {read=GetDrawingContext};

  //
  // API
  //
  __fastcall TFormMain(TComponent* Owner);

  void __fastcall SwapDrives(void);
  void __fastcall FlipSidesDriveA(void);
  void __fastcall EjectDriveA(void);
  void __fastcall EjectDriveB(void);
  void __fastcall LoadDriveA(AnsiString Filename, bool CheckAutoStart);
  void __fastcall LoadDriveB(AnsiString Filename);
  void __fastcall CreateDriveA(AnsiString Filename);
  void __fastcall CreateDriveB(AnsiString Filename);

  void __fastcall InsertTape(AnsiString Filename, bool CheckAutoPlay);
  void __fastcall PlayTape(void);
  void __fastcall StopTape(void);

  void __fastcall SubscribeToCPCEvents(TEmulatorCPCEvent Callback);
  void __fastcall UnsubscribeToCPCEvents(TEmulatorCPCEvent Callback);
  void __fastcall SubscribeToFDCEvents(TEmulatorFDCEvent Callback);
  void __fastcall UnsubscribeToFDCEvents(TEmulatorFDCEvent Callback);
  void __fastcall SubscribeToDiskEvents(TEmulatorDiskEvent Callback);
  void __fastcall UnsubscribeToDiskEvents(TEmulatorDiskEvent Callback);
  void __fastcall SubscribeToTapeEvents(TEmulatorTapeEvent Callback);
  void __fastcall UnsubscribeToTapeEvents(TEmulatorTapeEvent Callback);

  void __fastcall Pause(bool Paused);
  void __fastcall ResumeStepByStep(TEmulatorCPCEvent FirstCallback);
  void __fastcall StopStepByStep(bool Paused);


#pragma option push -vi-
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, DropFile);
  MESSAGE_HANDLER(WM_MOVE, TMessage, WMMove);
  MESSAGE_HANDLER(WM_KEYDOWN, TWMKey, WMKeyDown);
  MESSAGE_HANDLER(WM_KEYUP, TWMKey, WMKeyUp);
END_MESSAGE_MAP(TForm)
#pragma option pop
};

//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
