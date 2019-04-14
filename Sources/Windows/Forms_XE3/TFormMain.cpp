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


#include <vcl.h>
#include <FileCtrl.hpp>
#include <mmsystem.h>
#include <tchar.h>
#pragma hdrstop
#include "TFormMain.h"
#include "TFormEmulatorSettings.h"
#include "TFormHints.h"
#include "TFormAbout.h"
#include "TFormGameControllerSettings.h"
#include "TFormKeyboardMapping.h"
#include "TFormDiskCatalog.h"
#include "TFormCustomSpeed.h"
#include "TFormAudioSettings.h"
#include "TFormDisplaySettings.h"
#include "TFormCheatScripts.h"
#include "TFormPrinter.h"
#include "TFormZIPCatalog.h"
#include "TFormDebugger.h"
#include "TFormDebugCRTC.h"
#include "TFormDebugGateArray.h"
#include "TFormDebugFDC.h"
//#include "TFormDebugEmulatorScheduling.h"
//#include "TFormDebugLightgun.h"
#include "TFormDebugDSKViewer.h"
#include "TFormDebugCDTViewer.h"
#include "TFormDebugZ80Editor.h"
#include "TFormDebugMemoryEditor.h"
#include "TFormDebugTextCapture.h"
//#include "TFormDebugFileEditor.h"
#include "TFormDebugPSG.h"
#include "TFormDebugTapeAudioWorkshop.h"
#include "TFormDebugMEA8000.h"
//#include "TFormDebugEngine.h"
#include "TFormDragDropDrives.h"
#if (__BORLANDC__ != 0x551)
#include "TFormDebugGraphicsExplorer.h"
#endif
//#include "TFormDebugROMOperations.h"
#include "..\Emulator\TEmulatorMessage.h"
#include "..\Dialogs\TDialogOpenDSK.h"
#include "..\Dialogs\TDialogOpenSNA.h"
#include "..\Dialogs\TDialogOpenCDT.h"
#include "..\Dialogs\TDialogDiskCatalog.h"
#include "..\ZLib\TZIPContainer.h"

//roger commented for c++ builder 103 to work
//
// Compiler check
//
//#if (__BORLANDC__ == 0x551) // Borland C++ Builder 5 Update 1
//#elif (__BORLANDC__ == 0x651) // RAD Studio XE3 Update 2 Win32
//#elif (__BORLANDC__ == 0x680) // RAD Studio XE6 Update 1
//#else /* __BORLANDC__ */
//// Project properties, check "Show general messages".
//#pragma message You are compiling using Borland C++ version __BORLANDC__
//#error invalid __BORLANDC__
//#endif /* __BORLANDC__ */


//=================================
// Options

#undef USE_VK_SNAPSHOT
//#define USE_VK_SNAPSHOT

#undef SHOW_FPS
//#define SHOW_FPS

#undef AUTORUN_REMOVE_FILE_EXTENSION
#define AUTORUN_REMOVE_FILE_EXTENSION
//=================================

//=================================
// DEBUG Options
#undef SHOW_PARAMLIST
#undef SHOW_ALL_OSD_ICONS
#ifdef _DEBUG
//#define SHOW_PARAMLIST
//#define SHOW_ALL_OSD_ICONS
#endif /* _DEBUG */
//=================================

//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


#define SETTINGS_FILENAME           "Caprice.ini"
#define NEW_DISK_FILENAME           "New Disk.dsk"
#define RECENTDISKS_FILENAME        "RecentDisks.list"
#define FAVOURITEDISKS_FILENAME     "FavouriteDisks.list"

// LowerCase
#define SETTINGS_EXT                ".ini"
#define PROFILE_EXT                 ".prfl"
#define BITMAP_EXT                  ".bmp"
#define TAPEIMAGE_EXT               ".cdt"
#define WAVTAPE_EXT                 ".wav"
#define CSWTAPE_EXT                 ".csw"
#define SNAPSHOT_EXT                ".sna"
#define PLAYBACK_EXT                ".snr"
#define ZIP_EXT                     ".zip"
#define ROM_EXT                     ".rom"

// LowerCase
#define PARAM_DRIVEA                "/drivea="
#define PARAM_DRIVEB                "/driveb="
#define PARAM_TAPE                  "/tape="
#define PARAM_SNA                   "/sna="
#define PARAM_SETTINGS              "/settings="
#define PARAM_COMMAND               "/command="
#define PARAM_FULLSCREEN            "/fullscreen"
#define PARAM_WINDOWED              "/windowed"
#define PARAM_KEYBOARDASJOYSTICK    "/keyboardasjoystick"
#define PARAM_TURBO                 "/turbo"
#define PARAM_QUIET                 "/quiet"
#define PARAM_SCRIPT                "/script="

#ifndef _DEBUG
#define DISKIMAGE_FILTER_OPEN       "All supported|*.dsk;*.ipf;*.zip|"\
                                    "CPC Disk image (.dsk)|*.dsk|"\
                                    "Interchangeable Preservation Format (.ipf)|*.ipf|"\
                                    "Compressed archive (.zip)|*.zip|"\
                                    "All files|*.*"
#else /* _DEBUG */
#define DISKIMAGE_FILTER_OPEN       "All supported|*.dsk;*.raw;*.ipf;*.zip;|"\
                                    "CPC Disk image (.dsk)|*.dsk|"\
                                    "CT-Raw (*.raw)|*.raw|"\
                                    "Interchangeable Preservation Format (.ipf)|*.ipf|"\
                                    "Compressed archive (.zip)|*.zip|"\
                                    "All files|*.*"
#endif /* _DEBUG */

#define DISKIMAGE_FILTER_SAVE       "CPC Disk image (.dsk)|*.dsk|"

#define TAPEIMAGE_FILTER            "All supported|*.cdt;*.wav;*.csw;*.zip|"\
                                    "CPC Tape image (.cdt)|*.cdt|"\
                                    "WAV Audio Tape (.wav)|*.wav|"\
                                    "Compressed Square Wave (.csw)|*.csw|"\
                                    "Compressed archive (.zip)|*.zip|"\
                                    "All files|*.*"

#define SNAPSHOT_FILTER             "CPC Snapshot (.sna)|*.sna|"\
                                    "All files|*.*"
#define PLAYBACK_FILTER             "CPC Sessions (.snr)|*.snr|"\
                                    "All files|*.*"


#define MESSAGE_EMULATOR_DURATION               3     // seconds
#define MESSAGE_OVERRIDENSETTINGS_DURATION      10    // seconds

#define DECATHLON_PERIOD_START                  10

#define CPU_SPEED_FULL                          0
#define CPU_SPEED_300                           1
#define CPU_SPEED_200                           2
#define CPU_SPEED_CUSTOM                        3

#define NB_MAX_RECENT_FILES                     10
#define NB_MAX_FAVOURITE_FILES                  10


static const TCursor crFullscreenCursor = 5;

static const AnsiString DefaultPanelControlDriveHint =
  "Show last FDC operation on drive \"Cylinder:Sector ID\"."
  " Click to Set/Remove write protection."
  " Right-Click for Popup menu.";

static const AnsiString DriveAString = "Drive A";
static const AnsiString DriveBString = "Drive B";


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------


__fastcall TFormMain::TFormMain(TComponent* Owner)
        : TForm(Owner)
{
  Application->OnDeactivate = OnApplicationDeactivate;
  Application->OnMessage = AppMessage;
  Application->HintColor = (TColor)0x00FFECC1;

  // Settings
  mDoNotSaveSettings = false;
  mShowOverridenSettings = false;
  mStartSnapshotFilename = "";
  mStartAutoTypeCommand = "";
  mStartROMFilename = "";
  mStartScriptFilename = "";
  mCheckAutoRunAtStartup = false;
  mCheckAutoPlayAtStartup = false;
  mKeyboardAsJoystickAtStartup = false;
  mFullscreenAtStartup = false;
  mWindowedAtStartup = false;
  mTurboAtStartup = false;
  mQuietMode = false;

  AnsiString SettingsFilename = ExtractFilePath(Application->ExeName) + SETTINGS_FILENAME;
  mSettingsP = new TSettings(SettingsFilename);

  TStringList* ParamList = new TStringList;
  int ParamCount = GetCommandParamList(ParamList);
  if (ParamCount > 1)
  {
#ifdef SHOW_PARAMLIST
	// Display parameters list
	String Message = String(CmdLine) + "\r\n\r\n";

	for (int Loop=1; Loop < Count; Loop++)
	{
	  Message += AnsiString(Loop) + " " + ParamList->Strings[Loop] + "\r\n";
	}
	Application->MessageBox(Message.c_str(), _T("Parameters"), MB_OK);
#endif /* SHOW_PARAMLIST */

	// First ParamStr [0] is application full filename
	for (int loop=1; loop < ParamCount; loop++)
	{
	  AnsiString Argument = ParamList->Strings[loop].Trim();
	  AnsiString LowerArgument = Argument.LowerCase();

	  // Config filename
	  if (LowerArgument.Pos(PARAM_SETTINGS) == 1)
	  {
		int StartPos = Argument.Pos("=") + 1;
		mSettingsP->Read(Argument.SubString(StartPos, Argument.Length()));
        mShowOverridenSettings = true;
        mDoNotSaveSettings = true;
      }
    }
  }


#if (__BORLANDC__ == 0x551)
	DecimalSeparator = '.';
#elif (__BORLANDC__ == 0x651) || (__BORLANDC__ == 0x680)
	FormatSettings.DecimalSeparator = '.';
#endif /* __BORLANDC__ */

  mFullSpeed = false;

  // Recent disks
  mRecentDiskFilenames = new TStringList();
  AnsiString RecentDisksFilename = ExtractFilePath(Application->ExeName) + RECENTDISKS_FILENAME;
  if (FileExists(RecentDisksFilename) == true)
  {
    mRecentDiskFilenames->LoadFromFile(RecentDisksFilename);
    
    // Remove not existing files
    for (int Loop=0; Loop < mRecentDiskFilenames->Count; Loop++)
    {
      if (FileExists(mRecentDiskFilenames->Strings[Loop]) == false)
      {
        mRecentDiskFilenames->Delete(Loop);
        Loop--; // Index synchro
      }
    }

    // Create menu
    for (int Loop=0; Loop < mRecentDiskFilenames->Count; Loop++)
    {
      TMenuItem* NewItem = new TMenuItem(NULL);
      NewItem->Caption = mRecentDiskFilenames->Strings[Loop];
      NewItem->OnClick = LoadDriveAStoredImageMenuClick;

      DriveALoadRecentImageMenu->Add(NewItem);
      DriveALoadRecentImageMenu->Enabled = true;
    }
  }

  // Favourites disks
  mFavouriteDiskFilenames = new TStringList();
  AnsiString FavouriteDisksFilename = ExtractFilePath(Application->ExeName) + FAVOURITEDISKS_FILENAME;
  if (FileExists(FavouriteDisksFilename) == true)
  {
    mFavouriteDiskFilenames->LoadFromFile(FavouriteDisksFilename);

    // Remove not existing files
    for (int Loop=0; Loop < mFavouriteDiskFilenames->Count; Loop++)
    {
      if (FileExists(mFavouriteDiskFilenames->Strings[Loop]) == false)
      {
        mFavouriteDiskFilenames->Delete(Loop);
        Loop--; // Index synchro
      }
    }

    // Create menu
    for (int Loop=0; Loop < mFavouriteDiskFilenames->Count; Loop++)
    {
      TMenuItem* NewItem = new TMenuItem(NULL);
      NewItem->Caption = mFavouriteDiskFilenames->Strings[Loop];
      NewItem->OnClick = LoadDriveAStoredImageMenuClick;

      DriveALoadFavouriteImageMenu->Add(NewItem);
      DriveALoadFavouriteImageMenu->Enabled = true;
    }
  }

  mBaseTimerP = new TBaseTimer(CYCLES_PER_SECOND);

  mDrawingContextP = new TDrawingContext(mSettingsP);

  mAudioContextP = new TAudioContext(mSettingsP);

  mInputContextP = new TInputContext;
  mInputContextP->OnWINKeyDown = OnWINInputKeyDown;
  mInputContextP->OnWINKeyUp = OnWINInputKeyUp;
  mInputContextP->OnCPCKeyDown = OnCPCInputKeyDown;
  mInputContextP->OnCPCKeyUp = OnCPCInputKeyUp;
  mInputContextP->OnRemoveDevice = OnRemoveGameController;
  mInputContextP->OnMouseDown = OnInputMouseDown;
  mInputContextP->OnMouseUp = OnInputMouseUp;
  mInputContextP->OnMouseMove = OnInputMouseMove;
  mInputContextP->OnMouseClick = OnInputMouseClick;
  mInputContextP->OnMouseDblClick = OnInputMouseDblClick;

  mInfoFormP = NULL;
  mEmulatorP = NULL;
  mMouseNecessary = false;
  mFPSCounter = 0;
  mFPSDisplayed = -1;

  mApplicationTitle = "CaPriCe Forever " + TFormAbout::GetFileVersion(Application->ExeName);
#ifdef _DEBUG
  mApplicationTitle += " (DEBUG)";
#endif /* _DEBUG */
  Caption = mApplicationTitle;

#ifndef _DEBUG
  ExtraMenu->Remove(SaveContextMenu);
  ExtraMenu->Remove(EngineDataMenu);
  ExtraMenu->Remove(FileEditorMenu);
  ExtraMenu->Remove(EmulatorSchedulingMenu);
  ExtraMenu->Remove(ROMOperationsMenu);
  ExtraMenu->Remove(LightgunMenu);
#endif /* !_DEBUG */
#ifndef ENABLE_TAPE
  // Remove "Tape" Menu
  MainMenu->Items->Remove(TapeMenu);
#endif /* !ENABLE_TAPE */
#ifndef ENABLE_PRINTER
  ExtraMenu->Remove(PrinterMenu);
#endif /* !ENABLE_PRINTER */
#ifndef ENABLE_AMX_MOUSE
  InputsMenu->Remove(MouseAsAMXMouseMenu);
  InputsMenu->Remove(N33);
  InputsMenu->Remove(AMXMouseSpeedMenu);
#endif /* ENABLE_AMX_MOUSE */

  PanelControlDriveAPosition->Hint = DefaultPanelControlDriveHint;
  PanelControlDriveBPosition->Hint = DefaultPanelControlDriveHint;

  //
  // Apply settings
  //
  Left = mSettingsP->MainFormLeft;
  Top = mSettingsP->MainFormTop;
  // Show control panel
  PanelControl->Visible = mSettingsP->ControlPanelVisible;
  ControlPanelMenu->Checked = PanelControl->Visible;
  ClientWidth = mSettingsP->MainFormWidth;
  ClientHeight = mSettingsP->MainFormHeight;
  if ( (mSettingsP->ControlPanelVisible == true)
       && (ClientHeight == mSettingsP->MainFormHeight) )
  {
    ClientHeight += PanelControl->Height;
  }
  // Apply aspect ratio check
  OnCanResize = FormCanResize;

  EnableAutoStartMenu->Checked = mSettingsP->AutoStartEnable;
  EnableAutoPlayAtInsertMenu->Checked = mSettingsP->AutoPlayEnable;
  ConfirmDiskImagesUpdatesMenu->Checked = mSettingsP->ConfirmDiskImageUpdates;
  NeverUpdateDiskImagesMenu->Checked = mSettingsP->NeverUpdateDiskImages;
  if (NeverUpdateDiskImagesMenu->Checked == true)
  {
    ConfirmDiskImagesUpdatesMenu->Enabled = false;
  }
  UseHardwareKeyboardMappingMenu->Checked = mSettingsP->UseHardwareKeyboardMapping;

  // Max speed
  switch(mSettingsP->MaxSpeedIndex)
  {
    case CPU_SPEED_300: Speed300Menu->Checked = true; break;
    case CPU_SPEED_200: Speed200Menu->Checked = true; break;
    case CPU_SPEED_CUSTOM: CustomSpeedMenu->Checked = true; break;
    default: FullSpeedMenu->Checked = true; break;
  }

#ifdef ENABLE_AMX_MOUSE
  // AMX Mouse speed
  AMXMouseSpeed100Menu->Checked = true;
#endif /* ENABLE_AMX_MOUSE */

#ifdef _DEBUG
  // OSD DEBUG
  mSettingsP->OnscreenDisplay |= OSD_DEBUG;
#endif /* _DEBUG */

  // Small screenshot shortcut
  if (mSettingsP->SmallScreenshotShortcut == true)
  {
    SmallScreenshotMenu->ShortCut = ScreenshotMenu->ShortCut;
    ScreenshotMenu->ShortCut = 0;
  }

  // Current directory is executable directory
  SetCurrentDir(ExtractFilePath(Application->ExeName));

  //
  // Check directories
  //
  // Check profiles directory
  String Path = ExpandFileName(mSettingsP->ProfileDirectory);
  if (DirectoryExists(Path) == false)
  {
    CreateDir(Path);
  }
  // Check roms directory
  Path = ExpandFileName(mSettingsP->RomsDirectory);
  if (DirectoryExists(Path) == false)
  {
    CreateDir(Path);
  }
  // Check snapshots directory
  Path = ExpandFileName(mSettingsP->SnapshotDirectory);
  if (DirectoryExists(Path) == false)
  {
    CreateDir(Path);
  }
  // Check screenshots directory
  Path = ExpandFileName(mSettingsP->ScreenshotDirectory);
  if (DirectoryExists(Path) == false)
  {
    CreateDir(Path);
  }
  // Check Disks directory
  Path = ExpandFileName(mSettingsP->DiscDirectory);
  if ( (DirectoryExists(Path) == false) || (mSettingsP->DiscDirectory.IsEmpty() == true) )
  {
    mSettingsP->DiscDirectory = ExtractFilePath(Application->ExeName);
  }
  // Check Tape directory
  Path = ExpandFileName(mSettingsP->TapeDirectory);
  if ( (DirectoryExists(Path) == false) || (mSettingsP->TapeDirectory.IsEmpty() == true) )
  {
    mSettingsP->TapeDirectory = ExtractFilePath(Application->ExeName);
  }
  // Check Cheat scripts directory
  Path = ExpandFileName(mSettingsP->CheatScriptsDirectory);
  if (DirectoryExists(Path) == false)
  {
    CreateDir(Path);
  }

  // Allow DSK files Drag and Drop
  DragAcceptFiles(Handle, true);

  //
  // Check Command line parameters
  //
  if (ParamCount > 1)
  {
    TStringList* Files = new TStringList;

    // First ParamStr [0] is application full filename
    for (int loop=1; loop < ParamCount; loop++)
    {
      AnsiString Argument = ParamList->Strings[loop].Trim();
      AnsiString LowerArgument = Argument.LowerCase();

      // DriveA filename
      if (LowerArgument.Pos(PARAM_DRIVEA) == 1)
      {
        int StartPos = Argument.Pos("=") + 1;
        mSettingsP->DriveADiskFilename = Argument.SubString(StartPos, Argument.Length());
        mCheckAutoRunAtStartup = true;
      }
      // DriveB filename
      else if (LowerArgument.Pos(PARAM_DRIVEB) == 1)
      {
        int StartPos = Argument.Pos("=") + 1;
        mSettingsP->DriveBDiskFilename = Argument.SubString(StartPos, Argument.Length());
        mCheckAutoRunAtStartup = true;
      }
      // Tape filename
      else if (LowerArgument.Pos(PARAM_TAPE) == 1)
      {
        int StartPos = Argument.Pos("=") + 1;
        mSettingsP->TapeFilename = Argument.SubString(StartPos, Argument.Length());
        mCheckAutoPlayAtStartup = true;
      }
      // SNA filename
      else if (LowerArgument.Pos(PARAM_SNA) == 1)
      {
        int StartPos = Argument.Pos("=") + 1;
        mStartSnapshotFilename = Argument.SubString(StartPos, Argument.Length());
      }
      // Start command
      else if (LowerArgument.Pos(PARAM_COMMAND) == 1)
      {
        int StartPos = Argument.Pos("=") + 1;
        mStartAutoTypeCommand = Argument.SubString(StartPos, Argument.Length());
      }
      // Fulscreen
      else if (LowerArgument.Pos(PARAM_FULLSCREEN) == 1)
      {
        mFullscreenAtStartup = true;
      }
      // Windowed
      else if (LowerArgument.Pos(PARAM_WINDOWED) == 1)
      {
        mWindowedAtStartup = true;
      }
      // Keyboard as Joystick
      else if (LowerArgument.Pos(PARAM_KEYBOARDASJOYSTICK) == 1)
      {
        mKeyboardAsJoystickAtStartup = true;
      }
      // Turbo
      else if (LowerArgument.Pos(PARAM_TURBO) == 1)
      {
        mTurboAtStartup = true;
      }
      // Quiet Mode
      else if (LowerArgument.Pos(PARAM_QUIET) == 1)
      {
        mQuietMode = true;
      }
      // Start script
      else if (LowerArgument.Pos(PARAM_SCRIPT) == 1)
      {
        int StartPos = Argument.Pos("=") + 1;
        mStartScriptFilename = Argument.SubString(StartPos, Argument.Length());
      }
      // Consider filenames
      else
      {
        Files->Add(Argument);
      }
    }

    //
    // Check filenames given as argument
    //
    if (Files->Count > 0)
    {
      // Only ONE archive file
      if ( (Files->Count == 1)
           && (Files->Strings[0].LowerCase().Pos(ZIP_EXT) != 0) )
      {
        TZIPContainer* ZIPContainer = new TZIPContainer(Files->Strings[0]);
        AnsiString ContainerFilename = Files->Strings[0] + "\\";

        // Archive contains files
        if (ZIPContainer->NbFiles)
        {
          Files->Clear();

          for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
          {
            AnsiString FileName = ZIPContainer->Files[Loop];
            AnsiString LowerFilename = FileName.LowerCase();

            // Add any supported files
            if ( (LowerFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
                 || (LowerFilename.Pos(DISKIMAGE_IPF_EXT) != 0) )
            {
              Files->Add(ContainerFilename + FileName);
            }
            else if ( (LowerFilename.Pos(TAPEIMAGE_EXT) != 0)
                      || (LowerFilename.Pos(WAVTAPE_EXT) != 0)
                      || (LowerFilename.Pos(CSWTAPE_EXT) != 0)
                    )
            {
              Files->Add(ContainerFilename + FileName);
            }
            /*else if (LowerFilename.Pos(SNAPSHOT_EXT) != 0)
            {
              Files->Add(ContainerFilename + FileName);
            }
            else if (LowerFilename.Pos(SETTINGS_EXT) != 0)
            {
              Files->Add(ContainerFilename + FileName);
            }*/
          }
        }

        delete ZIPContainer;
      }

      int NbDiskFiles = 0;
      int NbTapeFiles = 0;
      int NbSnapshotFiles = 0;
      for (int loop=0; loop < Files->Count; loop++)
      {
        AnsiString Filename = Files->Strings[loop];
        AnsiString LowerFilename = Filename.LowerCase();

        // Disk image file
        if  ( (LowerFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
              || (LowerFilename.Pos(DISKIMAGE_IPF_EXT) != 0) )
        {
          NbDiskFiles++;

          if (TEmulator::PrepareDirectDSKUseSettings(mSettingsP) == true)
          {
            mShowOverridenSettings = true;
            mDoNotSaveSettings = true;
          }

          if (NbDiskFiles == 1)
          {
            mSettingsP->DriveADiskFilename = Filename;
          }
          else if (NbDiskFiles == 2)
          {
            mSettingsP->DriveBDiskFilename = Filename;
          }
          else
          {
            // Ignore others
          }

          mCheckAutoRunAtStartup = true;
        }
        // Tape images
        else if ( (LowerFilename.Pos(TAPEIMAGE_EXT) != 0)
                  || (LowerFilename.Pos(WAVTAPE_EXT) != 0)
                  || (LowerFilename.Pos(CSWTAPE_EXT) != 0)
                )
        {
          NbTapeFiles++;

          if (NbTapeFiles == 1)
          {
            if (TEmulator::PrepareDirectCDTUseSettings(mSettingsP) == true)
            {
              mShowOverridenSettings = true;
              mDoNotSaveSettings = true;
            }

            mSettingsP->TapeFilename = Filename;
            mCheckAutoPlayAtStartup = true;
          }
          else
          {
            // Ignore others
          }
        }
        // Snapshot file
        else if (LowerFilename.Pos(SNAPSHOT_EXT) != 0)
        {
          NbSnapshotFiles++;

          if (NbSnapshotFiles == 1)
          {
            mStartSnapshotFilename = Filename;
          }
          else
          {
            // Ignore others
          }
        }
        // ROM file
        else if (LowerFilename.Pos(ROM_EXT) != 0)
        {
          mStartROMFilename = Filename;
        }
        // Ignore others
        else
        {
        }
      }
    }

    delete Files;
  }

  delete ParamList;

  // Création de la liste des callbacks
  mOnEmulatorCPCEventsList = new TList;
  mOnEmulatorFDCEventList = new TList;
  mOnEmulatorDiskEventList = new TList;
  mOnEmulatorTapeEventList = new TList;

  mRunTillNextLightgunTrigger = false;

  // Emulator messages
  mEmulatorMessages = new TList;

  mFullscreenDialogForm = NULL;
  mIdleEventBeforePause = NULL;

  // Fullscreen cursor
  Screen->Cursors[crFullscreenCursor] = LoadCursor(HInstance, _T("CURSOR_FS"));

  mFDCMotorState = false;
  mTapeMotorState = false;

  // Windows Priority
  if (mSettingsP->WindowsPriority == 1)
  {
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
  }
  else if (mSettingsP->WindowsPriority == 2)
  {
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
  }

  mFormFrameWidth = Width - ClientWidth;
  mFormFrameHeight = Height - ClientHeight;
  if (PanelControl->Visible == true)
  {
    mFormFrameHeight += PanelControl->Height;
  }

  // Cold Reset At Real speed
  if (mSettingsP->ColdResetAtRealSpeed == true)
  {
    AnsiString NewCaption = "Cold Reset at Real Speed";
    ResetMenu->Caption = NewCaption;
    SpeedButtonControlReset->Hint = NewCaption;
  }

  // Apply quiet mode
  mAudioContextP->QuietMode = mQuietMode;

  // Show Hints
  UpdateShowHints();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormShow(TObject *Sender)
{
  do
  {
    // Création du buffer d'écran de l'émulateur
    if (mDrawingContextP->CreateEmulatorOffscreenBitmap() == false)
    {
			Application->MessageBox(_T("Unable to create emulator bitmap."), _T("FormShow"), MB_OK);
			continue;
		}

		// Create emulator after emulator screen
		mEmulatorP = new TEmulator(mSettingsP, mDrawingContextP);
		if (mEmulatorP == NULL)
		{
			Application->MessageBox(_T("Unable to create emulator."), _T("FormShow"), MB_OK);
			continue;
		}

    // Start emulator
    Err EmuError = mEmulatorP->Start();
    if (EmuError != errNone)
    {
			String msg = "Unable to start emulator. Error=" + String::IntToHex(EmuError,8);
			Application->MessageBox(msg.c_str(), _T("FormShow"), MB_OK);
			Pause(true);
			continue;
		}

    // Preload ROM
    if (mStartROMFilename.IsEmpty() == false)
    {
      if (mEmulatorP->AddROM(1, mStartROMFilename) != -1)
      {
        mEmulatorP->Reset();
        AddEmulatorMessage("ROM mapping overriden !!", MESSAGE_OVERRIDENSETTINGS_DURATION);
        AddEmulatorMessage("At exit, emulator settings will not be saved.", MESSAGE_OVERRIDENSETTINGS_DURATION);
        // Do not save settings.
        mDoNotSaveSettings = true;
      }
      else
      {
        AddEmulatorMessage("No slot found for this ROM !!");
      }
    }

    // Show overriden settings
    if (mShowOverridenSettings == true)
    {
      AddEmulatorMessage("Settings overriden !!", MESSAGE_OVERRIDENSETTINGS_DURATION);
      AddEmulatorMessage("At exit, emulator settings will not be saved.", MESSAGE_OVERRIDENSETTINGS_DURATION);
    }

    // Preload drives
    if (mSettingsP->DriveADiskFilename.Length() > 0)
    {
      LoadDriveA(mSettingsP->DriveADiskFilename, mCheckAutoRunAtStartup);
    }
    if (mSettingsP->DriveBDiskFilename.Length() > 0)
    {
      LoadDriveB(mSettingsP->DriveBDiskFilename);
    }

    // Preload tape
    if (mSettingsP->TapeFilename.Length() > 0)
    {
      InsertTape(mSettingsP->TapeFilename, mCheckAutoPlayAtStartup);
    }

    // Init Palette
    if (mDrawingContextP->BitsPerPixel == 8)
    {
      mEmulatorP->GetPalette(mDrawingContextP->Palette);
      mDrawingContextP->setupPaletteGDI();
    }

    // Init graphics
    if (mDrawingContextP->Init(PanelCPCScreen->Handle,
                               mSettingsP->UseOpenGL,
                               mSettingsP->ForceGDI) == false)
    {
      Pause(true);
      continue;
    }

    // Init emulator audio
    if (mAudioContextP->Start() == false)
    {
      mSettingsP->Mute = true;
      continue;
    }
    SetAudioSettings();
    if (mSettingsP->Mute == true)
    {
      AddEmulatorMessage("Audio mute On");
    }

    // Init Inputs
    if (mInputContextP->Init() > 0)
    {
      // Enable game controler settings access
      GameControllerSeparator->Visible = true;
      GameControllersSettingsMenu->Visible = true;
      LoadDefaultProfileMenu->Visible = true;
      LoadDriveAProfile->Visible = true;

      // Load driveA or default profile
      //LoadDefaultProfileMenuClick(Sender);
      LoadDriveAProfileClick(Sender);

      // Start Game controller acquisition
      mInputContextP->Start(Handle);
    }

    // Start emulator in background
    mExecuteCondition = EC_FRAME_COMPLETE;
    Application->OnIdle = OnIdleNormalSpeed;
  }
  while (0);

  // Init CPC screen size
  mDrawingContextP->OnDisplayResize(PanelCPCScreen->Width,
                                    PanelCPCScreen->Height);
  mInputContextP->OnDisplayResize(PanelCPCScreen->Width,
                                  PanelCPCScreen->Height);

#ifndef _DEBUG
  // Prevent screensaver to be executed
  SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, (LPVOID)&mScreenSaverActive, 0);
  if (mScreenSaverActive == TRUE)
  {
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, FALSE, 0, 0);
  }
  // Prevent monitor to enter powersave mode
  SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
#endif /* _DEBUG */

  if (mSettingsP->Fullscreen == true)
  {
    SetFullscreen();

    // Restore last window style
    mFullScreenOldStyle = mSettingsP->WindowStyle;
    mFullScreenOldExStyle = mSettingsP->WindowExStyle;
  }

  // Windows settings
  if (FormDebugDSKViewer) FormDebugDSKViewer->Settings = mSettingsP;
  if (FormDebugCRTC) FormDebugCRTC->Settings = mSettingsP;
  if (FormDebugGateArray) FormDebugGateArray->Settings = mSettingsP;
  if (FormDebugFDC) FormDebugFDC->Settings = mSettingsP;
  if (FormDebugZ80Editor) FormDebugZ80Editor->Settings = mSettingsP;
  if (FormDebugMemoryEditor) FormDebugMemoryEditor->Settings = mSettingsP;
  if (FormDebugPSG) FormDebugPSG->Settings = mSettingsP;
  if (FormDebugCDTViewer) FormDebugCDTViewer->Settings = mSettingsP;
  if (FormDebugTapeAudioWorkshop) FormDebugTapeAudioWorkshop->Settings = mSettingsP;
  if (FormDebugMEA8000) FormDebugMEA8000->Settings = mSettingsP;
  if (FormDebugTextCapture) FormDebugTextCapture->Settings = mSettingsP;
  if (FormDebugger) FormDebugger->Settings = mSettingsP;
  if (FormPrinter) FormPrinter->Settings = mSettingsP;
  if (FormCheatScripts) FormCheatScripts->Settings = mSettingsP;
#if (__BORLANDC__ != 0x551)
  if (FormGraphicsExplorer) FormGraphicsExplorer->Settings = mSettingsP;
#endif
//roger
//  if (FormDebugROMOperations) FormDebugROMOperations->Settings = mSettingsP;

  // Start snapshot
  if (mStartSnapshotFilename.IsEmpty() == false)
  {
    if (mEmulatorP->LoadSnapshot(mStartSnapshotFilename) == true)
    {
      ReloadSnapshotMenu->Enabled = true;
      UpdateSnapshotMenu->Enabled = true;
      mSettingsP->SnapshotDirectory = ExtractFilePath(mStartSnapshotFilename);
    }
  }

  // AutoType command
  if (mStartAutoTypeCommand.IsEmpty() == false)
  {
    mEmulatorP->StartAutoType(mStartAutoTypeCommand + "\n");
  }

  // Control panel subscription
  if (PanelControl->Visible == true)
  {
    SubscribeToDiskEvents(OnDiskEvent);
    SubscribeToFDCEvents(OnFDCEvent);
  }

  // Keyboard as Joystick
//  if (mKeyboardAsJoystickAtStartup == true)
//  {
//    // Activate
//    KeyboardAsJoystickMenuClick(Sender);
//  }

  // Fullscreen requested
  if (mFullscreenAtStartup == true)
  {
    // Not already in fullscreen
    if (mSettingsP->Fullscreen == false)
    {
	    SetFullscreen();
    }
  }
  // Windowed requested
  else if (mWindowedAtStartup == true)
  {
    // Already in fullscreen
    if (mSettingsP->Fullscreen == true)
    {
	    SetWindowed();
    }
  }

  // Turbo requested
  if (mTurboAtStartup == true)
  {
    // Activate
    ToggleCPUSpeedMenuClick(Sender);
  }

  // Execute script requested
  if (mStartScriptFilename.IsEmpty() == false)
  {
    FormCheatScripts->ExecuteScript(mStartScriptFilename);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	// Store window style
	mSettingsP->WindowStyle = mFullScreenOldStyle;
	mSettingsP->WindowExStyle = mFullScreenOldExStyle;

  //
  // Save virtual disk before saving settings
  //
  if ( (mEmulatorP->DiskAFilename.IsEmpty() == false)
       && (mEmulatorP->DriveAAltered == true)
       && (mSettingsP->NeverUpdateDiskImages == false)
       && (ExtractFilePath(mEmulatorP->DiskAFilename).Length() == 0) )
  {
    // Request filename
    if (mDrawingContextP->Fullscreen == false)
    {
      SaveDialog->Title = DriveAString + " - " + mEmulatorP->DiskAFilename;
      SaveDialog->FileName = mEmulatorP->DiskAFilename;
      SaveDialog->Filter = DISKIMAGE_FILTER_SAVE;
      SaveDialog->DefaultExt = DISKIMAGE_DSK_EXT;
      if (SaveDialog->Execute() == true)
      {
        mEmulatorP->ForceDiskAFilename(SaveDialog->FileName);
      }
      else
      {
        mEmulatorP->EjectDisk(DriveA);
      }
    }
  }
  if ( (mEmulatorP->DiskBFilename.IsEmpty() == false)
       && (mEmulatorP->DriveBAltered == true)
       && (mSettingsP->NeverUpdateDiskImages == false)
       && (ExtractFilePath(mEmulatorP->DiskBFilename).Length() == 0) )
  {
    // Request filename
    if (mDrawingContextP->Fullscreen == false)
    {
      SaveDialog->Title = DriveBString + " - " + mEmulatorP->DiskBFilename;
      SaveDialog->FileName = mEmulatorP->DiskBFilename;
      SaveDialog->Filter = DISKIMAGE_FILTER_SAVE;
      SaveDialog->DefaultExt = DISKIMAGE_DSK_EXT;
      if (SaveDialog->Execute() == true)
      {
        mEmulatorP->ForceDiskBFilename(SaveDialog->FileName);
      }
      else
      {
        mEmulatorP->EjectDisk(DriveB);
      }
    }
  }

	if (mSettingsP != NULL)
	{
		// Save settings
		if (mDoNotSaveSettings == false)
		{
			if (mDrawingContextP->Fullscreen == false)
			{
				mSettingsP->MainFormLeft = Left;
				mSettingsP->MainFormTop = Top;
			}

			mSettingsP->DriveADiskFilename = mEmulatorP->DiskAFilename;
			mSettingsP->DriveBDiskFilename = mEmulatorP->DiskBFilename;
			mSettingsP->TapeFilename = mEmulatorP->TapeFilename;

			mSettingsP->Update();
		}
	}

	NOT_USED(Sender);
  NOT_USED(CanClose);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormDestroy(TObject *Sender)
{
#ifndef _DEBUG
	// Allow screensaver to be executed again
	if (mScreenSaverActive == TRUE)
	{
		SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, TRUE, 0, 0);
	}
	// allow monitor to enter powersave mode
	SetThreadExecutionState(ES_CONTINUOUS);
#endif /* _DEBUG */

  DragAcceptFiles(Handle, false);

	if (mInputContextP != NULL)
	{
		// Stop Game controller acquisition
		mInputContextP->Stop();
		delete mInputContextP;
		mInputContextP = NULL;
	}

	if (mEmulatorP != NULL)
	{
		mEmulatorP->Stop();
		delete mEmulatorP;
		mEmulatorP = NULL;
	}

	delete mSettingsP;
	mSettingsP = NULL;

	delete mAudioContextP;

	//
	// Emulator events subscribers lists
	//
	while (mOnEmulatorCPCEventsList->Count > 0)
	{
		TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[0];
		delete Event;
		mOnEmulatorCPCEventsList->Delete(0);
	}
	delete mOnEmulatorCPCEventsList;

	while (mOnEmulatorFDCEventList->Count > 0)
	{
		TEmulatorFDCEvent* Event = (TEmulatorFDCEvent*)mOnEmulatorFDCEventList->Items[0];
		delete Event;
		mOnEmulatorFDCEventList->Delete(0);
	}
	delete mOnEmulatorFDCEventList;

	while (mOnEmulatorDiskEventList->Count > 0)
	{
		TEmulatorDiskEvent* Event = (TEmulatorDiskEvent*)mOnEmulatorDiskEventList->Items[0];
		delete Event;
		mOnEmulatorDiskEventList->Delete(0);
	}
	delete mOnEmulatorDiskEventList;

	while (mOnEmulatorTapeEventList->Count > 0)
	{
		TEmulatorTapeEvent* Event = (TEmulatorTapeEvent*)mOnEmulatorTapeEventList->Items[0];
		delete Event;
		mOnEmulatorTapeEventList->Delete(0);
	}
	delete mOnEmulatorTapeEventList;


	// Emulator messages
	while (mEmulatorMessages->Count > 0)
	{
		TEmulatorMessage* Message = (TEmulatorMessage*)mEmulatorMessages->Items[0];
		delete Message;
		mEmulatorMessages->Delete(0);
	}
	delete mEmulatorMessages;

	delete mInfoFormP;
	delete mDrawingContextP;

	delete mBaseTimerP;

  // Save recent files
  mRecentDiskFilenames->SaveToFile(ExtractFilePath(Application->ExeName) + RECENTDISKS_FILENAME);
  delete mRecentDiskFilenames;

  // Save favourite files
  mFavouriteDiskFilenames->SaveToFile(ExtractFilePath(Application->ExeName) + FAVOURITEDISKS_FILENAME);
  delete mFavouriteDiskFilenames;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::OnApplicationDeactivate(TObject* Sender)
{
	// Simultate ALT key release to avoid problems with ALT+Tab Application switching
	Perform(WM_KEYUP, VK_ALT, (int)0L);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::AppMessage(tagMSG &Msg, bool &Handled)
{
  // Capture WM_SYSKEYDOWN and WM_SYSKEYUP event to get VK_ALT
  if (Msg.message == WM_SYSKEYDOWN)
  {
    // Prevent repeat
    if (!(Msg.lParam & 0x40000000))
    {
      Perform(WM_KEYDOWN, Msg.wParam, Msg.lParam);
    }
    Handled = true;
  }
  else if (Msg.message == WM_SYSKEYUP)
  {
    Perform(WM_KEYUP, Msg.wParam, Msg.lParam);
    Handled = true;
  }
  else if (this->Focused() == true)
	{
		// Capture WM_KEYDOWN event to get VK_TAB
		if (Msg.message == WM_KEYDOWN)
		{
      // Prevent repeat
      if (!(Msg.lParam & 0x40000000))
      {
			  Perform(WM_KEYDOWN, Msg.wParam, Msg.lParam);
      }
			Handled = true;
		}
		else if (Msg.message == WM_KEYUP)
		{
			Perform(WM_KEYUP, Msg.wParam, Msg.lParam);
			Handled = true;
		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::WMMove(TMessage &Message)
{
	if ( (Visible == true) && (mSettingsP != NULL) )
	{
		if (mDrawingContextP->Fullscreen == false)
		{
			mSettingsP->MainFormLeft = Left;
			mSettingsP->MainFormTop = Top;
		}
	}

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::WMKeyDown(TWMKey &Message)
{
  if (Message.CharCode == VK_RETURN)
  {
    if (Message.KeyData & 0x01000000)
    {
      Message.CharCode = VK_ENTER;
    }
  }
  else if (Message.CharCode == VK_SHIFT)
	{
    mShiftState = mShiftState << ssShift;
  }
  else if (Message.CharCode == VK_CONTROL)
  {
    mShiftState = mShiftState << ssCtrl;
  }
  else if (Message.CharCode == VK_ALT)
  {
    mShiftState = mShiftState << ssAlt;
  }

  if (mInputContextP->OnWINKeyDown(Message.CharCode, mShiftState) == true)
  {
    Message.CharCode = 0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::WMKeyUp(TWMKey &Message)
{
  if (Message.CharCode == VK_RETURN)
  {
    if (Message.KeyData & 0x01000000)
    {
      Message.CharCode = VK_ENTER;
    }
  }
  else if (Message.CharCode == VK_SHIFT)
  {
    mShiftState = mShiftState >> ssShift;
  }
  else if (Message.CharCode == VK_CONTROL)
  {
    mShiftState = mShiftState >> ssCtrl;
  }
  else if (Message.CharCode == VK_ALT)
  {
    mShiftState = mShiftState >> ssAlt;

    // CTRL key not pressed => AltGr pressed
    if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000))
    {
      mShiftState = mShiftState >> ssCtrl;
		}
  }

  // Deactivation message is sent while destroying application.
  // Deactivation is sending VK_UP message to handle ALT+TAB problem.
  if (mInputContextP)
  {
    mInputContextP->OnWINKeyUp(Message.CharCode, mShiftState);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DropFile(TWMDropFiles &Message)
{
  HDROP hDrop = (HDROP)Message.Drop;
  int FileCount = DragQueryFile(hDrop, -1, NULL, 0 );

  TStringList* Files = new TStringList;
  TStringList* IgnoreFiles = new TStringList;

  // Create file list from Drop message
  for (int file = 0; file < FileCount; file++)
  {
    int CharacterCount = DragQueryFile(hDrop, file, NULL, 0) + 1; // account for NULL terminator

    _TCHAR* FileName = new _TCHAR[CharacterCount];
    DragQueryFile(hDrop, file, FileName, CharacterCount);

    Files->Add(AnsiString(FileName));
  }

  // Only ONE archive file
  if ( (Files->Count == 1)
       && (Files->Strings[0].LowerCase().Pos(ZIP_EXT) != 0) )
  {
    TZIPContainer* ZIPContainer = new TZIPContainer(Files->Strings[0]);
    AnsiString ContainerFilename = Files->Strings[0] + "\\";

    // Archive contains files
    if (ZIPContainer->NbFiles)
    {
      Files->Clear();

      for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
      {
        AnsiString FileName = ZIPContainer->Files[Loop];
        AnsiString LowerFilename = FileName.LowerCase();

        // Add any supported files
        if ( (LowerFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
             || (LowerFilename.Pos(DISKIMAGE_IPF_EXT) != 0) )
        {
          Files->Add(ContainerFilename + FileName);
        }
        else if ( (LowerFilename.Pos(TAPEIMAGE_EXT) != 0)
                  || (LowerFilename.Pos(WAVTAPE_EXT) != 0)
                  || (LowerFilename.Pos(CSWTAPE_EXT) != 0)
                )
        {
          Files->Add(ContainerFilename + FileName);
        }
        /*else if (LowerFilename.Pos(SNAPSHOT_EXT) != 0)
        {
          Files->Add(ContainerFilename + FileName);
        }
        else if (LowerFilename.Pos(SETTINGS_EXT) != 0)
        {
          Files->Add(ContainerFilename + FileName);
        }*/
      }
    }

    delete ZIPContainer;
  }

  int NbDiskFiles = 0;
  int NbTapeFiles = 0;
  int NbSnapshotFiles = 0;
  int NbSettingsFiles = 0;
  for (int file = 0; file < Files->Count; file++)
  {
    AnsiString FileName = Files->Strings[file];
    AnsiString LowerFilename = FileName.LowerCase();

    // Disk image file
    if ( (LowerFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
         || (LowerFilename.Pos(DISKIMAGE_IPF_EXT) != 0) )
    {
      NbDiskFiles++;

      if (Files->Count == 1)
      {
        TFormDragDropDrives *DrivesForm = new TFormDragDropDrives(this);
        DrivesForm->ParentWidth = ClientWidth;
        DrivesForm->ParentHeight = ClientHeight;

        switch (DrivesForm->ShowModal())
        {
          case mrYes:
          {
            LoadDriveA(FileName, true);
          }
          break;

          case mrNo:
          {
            LoadDriveB(FileName);
          }
          break;

          default:
          {
            // Ignore file
          }
        }

        delete DrivesForm;
      }
      else
      {
        // Load disk image on drive A
        if (NbDiskFiles == 1)
        {
          LoadDriveA(FileName, true);
        }
        // Load disk image on drive B
        else if (NbDiskFiles == 2)
        {
          LoadDriveB(FileName);
        }
        else
        {
          // Ignore others
          IgnoreFiles->Add(FileName);
        }
      }
		}
    // Tape image
    else if ( (LowerFilename.Pos(TAPEIMAGE_EXT) != 0)
              || (LowerFilename.Pos(WAVTAPE_EXT) != 0)
              || (LowerFilename.Pos(CSWTAPE_EXT) != 0) )
    {
      NbTapeFiles++;

      // Load tape image
      if (NbTapeFiles == 1)
      {
        InsertTape(FileName, true);
      }
      else
      {
        // Ignore others
        IgnoreFiles->Add(FileName);
      }
    }
    // Snapshot file
    else if (LowerFilename.Pos(SNAPSHOT_EXT) != 0)
    {
      NbSnapshotFiles++;

      if (NbSnapshotFiles == 1)
      {
        if (mEmulatorP->LoadSnapshot(FileName) == true)
        {
          ReloadSnapshotMenu->Enabled = true;
          UpdateSnapshotMenu->Enabled = true;
          mSettingsP->SnapshotDirectory = ExtractFilePath(mEmulatorP->SnapshotFilename);

          // Stop playing tape
          if (PlayTapeMenu->Checked == true)
          {
            StopTape();
          }
        }
      }
      else
      {
        // Ignore others
				IgnoreFiles->Add(FileName);
      }
    }
    // Settings file
    else if (LowerFilename.Pos(SETTINGS_EXT) != 0)
    {
      NbSettingsFiles++;

      if (NbSettingsFiles == 1)
      {
        mSettingsP->Read(FileName);
        Pause(false);
        ResetEmulator();
        AddEmulatorMessage("Settings overriden !!", MESSAGE_OVERRIDENSETTINGS_DURATION);
        AddEmulatorMessage("At exit, emulator settings will not be saved.", MESSAGE_OVERRIDENSETTINGS_DURATION);
        // Do not save settings.
        mDoNotSaveSettings = true;
      }
      else
      {
        // Ignore others
        IgnoreFiles->Add(FileName);
      }
    }
    // ROM file
    else if (LowerFilename.Pos(ROM_EXT) != 0)
    {
      Pause(false);
      ResetEmulator();
      if (mEmulatorP->AddROM(1, FileName) != -1)
      {
        mEmulatorP->Reset();
        AddEmulatorMessage("ROM mapping overriden !!", MESSAGE_OVERRIDENSETTINGS_DURATION);
        AddEmulatorMessage("At exit, emulator settings will not be saved.", MESSAGE_OVERRIDENSETTINGS_DURATION);
        // Do not save settings.
        mDoNotSaveSettings = true;
      }
      else
      {
        AddEmulatorMessage("No slot found for this ROM !!");
      }
    }
    else
    {
      // Ignore file
      IgnoreFiles->Add(FileName);
    }
  }

  // Show ignore files
	if (IgnoreFiles->Count)
	{
		String Message = _T("Following files have been ignored:\r\n");
		for (int Loop=0; Loop < IgnoreFiles->Count; Loop++)
		{
			Message += _T(" - \"") + IgnoreFiles->Strings[Loop] + _T("\"\r\n");
		}
		Application->MessageBox(Message.c_str(), _T("Drag & Drop"), MB_OK);
	}

  delete IgnoreFiles;
  delete Files;

  DragFinish(hDrop);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight,
          bool &Resize)
{
  if (mSettingsP->KeepAspectRatio == true)
  {
    if ( (Visible == true) && (mDrawingContextP->Fullscreen == false) )
    {
      if (NewWidth != Width)
      {
        int NewScreenWidth = NewWidth - mFormFrameWidth;
        int NewScreenHeight = (int)((double)NewScreenWidth / mDrawingContextP->EmulatorAspectRatio);

        NewWidth = NewScreenWidth + mFormFrameWidth;
        NewHeight = NewScreenHeight + mFormFrameHeight;
      }
      else
      {
        Resize = false;
      }
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::PanelCPCScreenResize(TObject *Sender)
{
	// Update settings
  if (Visible == true)
  {
    mDrawingContextP->OnDisplayResize(PanelCPCScreen->Width,
                                      PanelCPCScreen->Height);
    mInputContextP->OnDisplayResize(PanelCPCScreen->Width,
                                    PanelCPCScreen->Height);

    if (mDrawingContextP->Fullscreen == false)
    {
      mSettingsP->MainFormWidth = PanelCPCScreen->Width;
      mSettingsP->MainFormHeight = PanelCPCScreen->Height;
    }

    if (mInfoFormP)
    {
      mInfoFormP->OnResize(mDrawingContextP);
    }

    if (mFullscreenDialogForm)
    {
      mFullscreenDialogForm->OnResize(mDrawingContextP);
    }
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PanelControlResize(TObject *Sender)
{
	PanelFloattingControl->Left = (PanelControl->Width - PanelFloattingControl->Width) / 2;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Emulator Menu
//
void __fastcall TFormMain::ResetMenuClick(TObject *Sender)
{
	Err Error = mEmulatorP->Reset();
	if (Error != errNone)
	{
		// Stop background emulation
		Application->OnIdle = NULL;

		String msg = _T("Unable to reset emulator. Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("ResetMenu"), MB_OK);
	}

  mExecuteCondition = EC_FRAME_COMPLETE;

  // Set Real Speed
  if (mSettingsP->ColdResetAtRealSpeed == true)
  {
    // Turbo activated ?
	  if (ToggleCPUSpeedMenu->Checked == true)
	  {
		  // Back to real CPC speed
		  ToggleCPUSpeedMenu->Checked = false;
		  SetRealCPCSpeed();
	  }
  }

	// Signal RESET to all subscribers
	for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
	{
		TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
		if (((TEmulatorCPCEvent)*Event)(EC_RESET, mEmulatorP->NativeCPC) == true)
    {
      // Pause emulation
      if (mEmulatorP->Paused == false)
      {
        Pause(true);
      }
    }
	}

  // Stop current script
  FormCheatScripts->StopCurrentScript();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PauseMenuClick(TObject *Sender)
{
	// Invert Pause
	Pause(PauseMenu->Checked ? false : true);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Pause(bool Paused)
{
  // Signal pause
  mEmulatorP->Paused = Paused;
  PauseMenu->Checked = Paused;

  if (EmulatorP->StepByStepActive == true)
  {
    // Deactivate Pause display during Step by step
    delete mInfoFormP;
    mInfoFormP = NULL;

    if (Paused == true)
    {
      if (mIdleEventBeforePause == NULL)
      {
        mIdleEventBeforePause = Application->OnIdle;
        Application->OnIdle = OnIdlePaused;
      }

      // Signal pause to all subscribers
      for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
      {
        TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
        ((TEmulatorCPCEvent)*Event)(EC_END_INSTRUCTION, mEmulatorP->NativeCPC);
      }
    }
    else
    {
      if (mIdleEventBeforePause)
      {
        Application->OnIdle = mIdleEventBeforePause;
        mIdleEventBeforePause = NULL;
      }
    }
  }
  else
  {
    // Delete Pause display
    delete mInfoFormP;
    mInfoFormP = NULL;

    if (Paused == true)
    {
      mAudioContextP->Pause();

      if (mIdleEventBeforePause == NULL)
      {
        mIdleEventBeforePause = Application->OnIdle;
        Application->OnIdle = OnIdlePaused;
      }

      // Signal pause to all subscribers
      for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
      {
        TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
        ((TEmulatorCPCEvent)*Event)(EC_END_INSTRUCTION, mEmulatorP->NativeCPC);
      }

      //
      // Prepare pause display
      //
      mInfoFormP = new TDialogInfo(mDrawingContextP, mSettingsP->ShowPauseInfo);
      mInfoFormP->MachineType = "Machine=" + mEmulatorP->CPCModel;
      if (EmulatorP->DiskAFilename.IsEmpty() == false)
      {
        AnsiString ReadOnly = (mEmulatorP->DriveAWriteProtected == true) ? "[READ ONLY] " : "";
        mInfoFormP->DriveAFilename = ReadOnly + "DriveA=" + ExtractFileName(EmulatorP->DiskAFilename);
      }
      if (EmulatorP->DiskBFilename.IsEmpty() == false)
      {
        AnsiString ReadOnly = (mEmulatorP->DriveBWriteProtected == true) ? "[READ ONLY] " : "";
        mInfoFormP->DriveBFilename = ReadOnly + "DriveB=" + ExtractFileName(EmulatorP->DiskBFilename);
      }
      if (EmulatorP->TapeFilename.IsEmpty() == false)
      {
        mInfoFormP->TapeFilename = "Tape=" + ExtractFileName(EmulatorP->TapeFilename);
      }
      if (EmulatorP->SnapshotFilename.IsEmpty() == false)
      {
        mInfoFormP->Snapshot = "Snapshot=" + ExtractFileName(EmulatorP->SnapshotFilename);
      }
      if (mInputContextP->ProfileFilename.IsEmpty() == false)
      {
        mInfoFormP->ControllerProfile = "ControllersProfile=" + mInputContextP->ProfileFilename;
      }
      mInfoFormP->Fullscreen = mDrawingContextP->Fullscreen;
    }
    else
    {
      //
      // Resume
      //
      mAudioContextP->Resume();
      mRunTillNextLightgunTrigger = false;

      if (mIdleEventBeforePause)
      {
        Application->OnIdle = mIdleEventBeforePause;
        mIdleEventBeforePause = NULL;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToggleCPUSpeedMenuClick(TObject *Sender)
{
	if (ToggleCPUSpeedMenu->Checked == false)
	{
		// Set max speed
		ToggleCPUSpeedMenu->Checked = true;

		switch(mSettingsP->MaxSpeedIndex)
		{
			case CPU_SPEED_300: SetSpeed300Percent(); break;
			case CPU_SPEED_200: SetSpeed200Percent(); break;
			case CPU_SPEED_CUSTOM: SetSpeedCustom(); break;
			default: SetFullSpeed();
		}
	}
	else
	{
		// Back to real CPC speed
		ToggleCPUSpeedMenu->Checked = false;
		SetRealCPCSpeed();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FullSpeedMenuClick(TObject *Sender)
{
	FullSpeedMenu->Checked = true;
	mSettingsP->MaxSpeedIndex = CPU_SPEED_FULL;

	// Back to real CPC speed
	if (ToggleCPUSpeedMenu->Checked == true)
	{
		SetFullSpeed();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Speed300MenuClick(TObject *Sender)
{
	Speed300Menu->Checked = true;
	mSettingsP->MaxSpeedIndex = CPU_SPEED_300;

	// Update CPC speed
	if (ToggleCPUSpeedMenu->Checked == true)
	{
		SetSpeed300Percent();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Speed200MenuClick(TObject *Sender)
{
	Speed200Menu->Checked = true;
	mSettingsP->MaxSpeedIndex = CPU_SPEED_200;

	// Update CPC speed
	if (ToggleCPUSpeedMenu->Checked == true)
	{
		SetSpeed200Percent();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::CustomSpeedMenuClick(TObject *Sender)
{
	CustomSpeedMenu->Checked = true;
	mSettingsP->MaxSpeedIndex = CPU_SPEED_CUSTOM;

	// Update CPC speed
	if (ToggleCPUSpeedMenu->Checked == true)
	{
		SetSpeedCustom();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SetCustomSpeedMenuClick(TObject *Sender)
{
	TFormCustomSpeed* CustomSpeedForm = new TFormCustomSpeed(this);
	CustomSpeedForm->Settings = mSettingsP;
	CustomSpeedForm->CustomSpeed = mSettingsP->MaxCustomSpeed;
	if (CustomSpeedForm->ShowModal() == mrOk)
	{
		mSettingsP->MaxCustomSpeed = CustomSpeedForm->CustomSpeed;

    if (mSettingsP->MaxSpeedIndex == CPU_SPEED_CUSTOM)
    {
		  SetSpeedCustom();
    }
	}
	delete CustomSpeedForm;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::VolumeUpMenuClick(TObject *Sender)
{
	if (mSettingsP->AudioVolume < AUDIO_VOLUME_MAX)
	{
		mSettingsP->AudioVolume += 10;
		if (mSettingsP->AudioVolume > AUDIO_VOLUME_MAX)
		{
			mSettingsP->AudioVolume = AUDIO_VOLUME_MAX;
		}
		mAudioContextP->Volume = mSettingsP->AudioVolume;
		AnsiString Message = AnsiString("Volume UP (") + AnsiString(mSettingsP->AudioVolume) + AnsiString(")");
		AddEmulatorMessage(Message);
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::VolumeDownMenuClick(TObject *Sender)
{
	if (mSettingsP->AudioVolume > AUDIO_VOLUME_MIN)
	{
		mSettingsP->AudioVolume -= 10;
		if (mSettingsP->AudioVolume < AUDIO_VOLUME_MIN)
		{
			mSettingsP->AudioVolume = AUDIO_VOLUME_MIN;
		}
		mAudioContextP->Volume = mSettingsP->AudioVolume;
		AnsiString Message = "Volume DOWN (" + AnsiString(mSettingsP->AudioVolume) + ")";
		AddEmulatorMessage(Message);
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MuteMenuClick(TObject *Sender)
{
	mSettingsP->Mute ^= true;
	SetAudioSettings();
	AddEmulatorMessage(mSettingsP->Mute == true ? "Audio mute On" : "Audio mute Off");

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::AudioRendererMenuClick(TObject *Sender)
{
	TMenuItem *AudioRendererItem = dynamic_cast<TMenuItem *>(Sender);
	AudioRendererItem->Checked = true;
	mSettingsP->AudioRenderer = AudioRendererItem->Caption;

	// Restart Audio
	delete mAudioContextP;
	mAudioContextP = new TAudioContext(mSettingsP);
    if (mAudioContextP->Start() == false)
	{
		mSettingsP->Mute = true;
	}

	SetAudioSettings();

	// Update debug forms
	FormDebugPSG->AudioContext = mAudioContextP;
	FormDebugTapeAudioWorkshop->AudioContext = mAudioContextP;
	FormDebugMEA8000->AudioContext = mAudioContextP;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::AudioSettingsMenuClick(TObject *Sender)
{
	TFormAudioSettings* AudioSettingsForm = new TFormAudioSettings(this);
	AudioSettingsForm->Settings = mSettingsP;
	AudioSettingsForm->AudioContext = mAudioContextP;
    AudioSettingsForm->Emulator = mEmulatorP;
    AudioSettingsForm->ShowHint = ShowHint;
	if (AudioSettingsForm->ShowModal() == mrOk)
	{
		// Restart Audio
		delete mAudioContextP;
		mAudioContextP = new TAudioContext(mSettingsP);
    if (mAudioContextP->Start() == false)
		{
			mSettingsP->Mute = true;
		}

		SetAudioSettings();
	}

	delete AudioSettingsForm;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::LoadSnapshotMenuClick(TObject *Sender)
{
	if ( (mDrawingContextP->Fullscreen == false)
			 || (mSettingsP->FullscreenMenu == true) )
	{
		OpenDialog->Title = "Load snapshot";
		OpenDialog->Filter = SNAPSHOT_FILTER;
        OpenDialog->FileName = mEmulatorP->SnapshotFilename;
		OpenDialog->InitialDir = mSettingsP->SnapshotDirectory;
		if (OpenDialog->Execute() == true)
		{
			// Stop playing tape
			if (PlayTapeMenu->Checked == true)
			{
				PlayTapeMenuClick(Sender);
			}

			if (mEmulatorP->LoadSnapshot(OpenDialog->FileName) == true)
			{
				ReloadSnapshotMenu->Enabled = true;
				UnloadSnapshotMenu->Enabled = true;
				UpdateSnapshotMenu->Enabled = true;
				mSettingsP->SnapshotDirectory = ExtractFilePath(mEmulatorP->SnapshotFilename);

				AnsiString Filename = ExtractFileName(mEmulatorP->SnapshotFilename);
				AddEmulatorMessage("Snapshot=" + Filename);

				if (mInfoFormP)
				{
					mInfoFormP->Snapshot = "Snapshot=" + Filename;
				}

				// Remove Pause
				Pause(false);
			}
		}
	}
	else
	{
		if (mMouseNecessary == false)
		{
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(TRUE);
			}
		}
		Screen->Cursor = mOldCursor;

		// Create openfile dialog
		mFullscreenDialogForm = new TDialogOpenSNA(mDrawingContextP);
		mFullscreenDialogForm->OnEndEvent = OnFullscreenOpenSNADialogEnd;

		TDialogOpenSNA* FilesDialog = (TDialogOpenSNA*)mFullscreenDialogForm;
		// Set initial path
		if (mEmulatorP->SnapshotFilename.Length())
		{
			FilesDialog->InitialPath = mSettingsP->SnapshotDirectory;
			FilesDialog->SelectedFile = ExtractFileName(mEmulatorP->SnapshotFilename);
		}
		else
		{
			FilesDialog->InitialPath = mSettingsP->SnapshotDirectory;
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ReloadSnapshotMenuClick(TObject *Sender)
{
	if (ReloadSnapshotMenu->Enabled == true)
	{
		// Stop playing tape
		if (PlayTapeMenu->Checked == true)
		{
			PlayTapeMenuClick(Sender);
		}

		mEmulatorP->ReloadSnapshot();
		AddEmulatorMessage("Snapshot reloaded");

		// Remove Pause
		Pause(false);
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::UnloadSnapshotMenuClick(TObject *Sender)
{
	if (UnloadSnapshotMenu->Enabled == true)
	{
		mEmulatorP->UnloadSnapshot();
		AddEmulatorMessage("Snapshot unloaded");

		if (mInfoFormP)
		{
			mInfoFormP->Snapshot = "";
		}

		ReloadSnapshotMenu->Enabled = false;
		UnloadSnapshotMenu->Enabled = false;
		UpdateSnapshotMenu->Enabled = false;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SaveSnapshotMenuClick(TObject *Sender)
{
	SaveDialog->Title = "Save snapshot";
	SaveDialog->Filter = SNAPSHOT_FILTER;
	SaveDialog->InitialDir = mSettingsP->SnapshotDirectory;
	SaveDialog->DefaultExt = SNAPSHOT_EXT;
	if (SaveDialog->Execute() == true)
	{
		if (mEmulatorP->SaveSnapshot(SaveDialog->FileName) == true)
		{
			ReloadSnapshotMenu->Enabled = true;
			UnloadSnapshotMenu->Enabled = true;
			UpdateSnapshotMenu->Enabled = true;
			AddEmulatorMessage("Snapshot saved");
			AddEmulatorMessage("Snapshot=" + ExtractFileName(mEmulatorP->SnapshotFilename));
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::UpdateSnapshotMenuClick(TObject *Sender)
{
	if (UpdateSnapshotMenu->Enabled == true)
	{
		mEmulatorP->UpdateSnapshot();
		AddEmulatorMessage("Snapshot updated");
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::PlaybackSessionMenuClick(TObject *Sender)
{
	if ( (mDrawingContextP->Fullscreen == false)
			 || (mSettingsP->FullscreenMenu == true) )
	{
		OpenDialog->Title = "Load session record";
		OpenDialog->Filter = PLAYBACK_FILTER;
        OpenDialog->FileName = mEmulatorP->SnapshotFilename;
		OpenDialog->InitialDir = mSettingsP->SnapshotDirectory;
		if (OpenDialog->Execute() == true)
		{
			// Stop playing tape
			if (PlayTapeMenu->Checked == true)
			{
				PlayTapeMenuClick(Sender);
			}

			if (mEmulatorP->PlaySNRSession(OpenDialog->FileName) == true)
      {
		    AddEmulatorMessage("Playing session \"" + ExtractFileName(OpenDialog->FileName) + "\".");

        // Update drives Hint
        if (mEmulatorP->DiskAFilename.IsEmpty() == false)
        {
          PanelControlDriveAPosition->Hint = ExtractFileName(mEmulatorP->DiskAFilename);
        }
        if (mEmulatorP->DiskBFilename.IsEmpty() == false)
        {
          PanelControlDriveBPosition->Hint = ExtractFileName(mEmulatorP->DiskBFilename);
        }
      }
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::EmulatorSettingsMenuClick(TObject *Sender)
{
	// Stop background emulation
	TIdleEvent oldEvent = Application->OnIdle;
	Application->OnIdle = NULL;

	FormEmulatorSettings->Settings = mSettingsP;
	if (FormEmulatorSettings->ShowModal() == mrOk)
	{
		// Remove Pause
		Pause(false);
		ResetEmulator();
	}

	// Restart background emulation
	Application->OnIdle = oldEvent;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::OpenCapriceFolderMenuClick(TObject *Sender)
{
	// Open screenshots folder using default viewer
	ShellExecute(Handle,
							 _T("open"),
							 ExpandFileName(ExtractFilePath(Application->ExeName)).c_str(),
							 0,
							 0,
							 SW_NORMAL);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ExitMenuClick(TObject *Sender)
{
	Close();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Drives Menu
//
void __fastcall TFormMain::DriveACatalogMenuClick(TObject *Sender)
{
  if (DriveACatalogMenu->Enabled == false)
    return;
    
	// No DriveA disk ?
	if (mEmulatorP->DiskAFilename.IsEmpty() == true)
	{
		AddEmulatorMessage("Insert a disk into Drive A.");
		return;
	}

	if ( (mDrawingContextP->Fullscreen == false)
			 || (mSettingsP->FullscreenMenu == true) )
	{
		TFormDiskCatalog* Dialog = new TFormDiskCatalog(this);
		Dialog->Tag = (int)mEmulatorP;
		if (Dialog->ShowModal() == mrOk)
		{
			if (Dialog->RUNRequested == true)
			{
				mEmulatorP->RUNFilename(Dialog->SelectedFilename);
			}
			else if (Dialog->CPMRequested == true)
			{
				mEmulatorP->ExecuteCPM();
			}
			else
			{
				mEmulatorP->LOADFilename(Dialog->SelectedFilename);
			}
		}
		delete Dialog;
	}
	else
	{
		if (mMouseNecessary == false)
		{
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(TRUE);
			}
		}
		Screen->Cursor = mOldCursor;

		// Create fullscreen dialog
		mFullscreenDialogForm = new TDialogDiskCatalog(mDrawingContextP);
		mFullscreenDialogForm->OnEndEvent = OnFullscreenDiskCatalogDialogEnd;

		TDialogDiskCatalog* Dialog = (TDialogDiskCatalog*)mFullscreenDialogForm;
		Dialog->ShowCatalog(mEmulatorP);
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EnableAutoStartMenuClick(TObject *Sender)
{
	mSettingsP->AutoStartEnable = mSettingsP->AutoStartEnable == false ? true : false;
	EnableAutoStartMenu->Checked = mSettingsP->AutoStartEnable;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::LoadDriveAStoredImageMenuClick(TObject *Sender)
{
  TMenuItem* Item = (TMenuItem*)Sender;
  LoadDriveA(Item->Caption, true);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::LoadimageintoDriveAMenuClick(TObject *Sender)
{
	if ( (mDrawingContextP->Fullscreen == false)
			 || (mSettingsP->FullscreenMenu == true) )
	{
		OpenDialog->Title = "Load disk image on Drive A";
		OpenDialog->FileName = mEmulatorP->DiskAFilename;
		OpenDialog->Filter = DISKIMAGE_FILTER_OPEN;
		OpenDialog->InitialDir = mSettingsP->DiscDirectory;
		if (OpenDialog->Execute() == true)
		{
			LoadDriveA(OpenDialog->FileName, true);
		}
	}
	else
	{
		if (mMouseNecessary == false)
		{
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(TRUE);
			}
		}
		Screen->Cursor = mOldCursor;

		// Create openfile dialog
		mFullscreenDialogForm = new TDialogOpenDSK(mDrawingContextP);
		mFullscreenDialogForm->OnEndEvent = OnFullscreenOpenDSKDialogEnd;

		TDialogOpenDSK* FilesDialog = (TDialogOpenDSK*)mFullscreenDialogForm;
		FilesDialog->DriveAActive = true;
		// Set initial path
		if (mEmulatorP->DiskAFilename.Length())
		{
			FilesDialog->InitialPath = ExtractFilePath(mEmulatorP->DiskAFilename);
			FilesDialog->SelectedFile = ExtractFileName(mEmulatorP->DiskAFilename);
		}
		else if (mEmulatorP->DiskBFilename.Length())
		{
			FilesDialog->InitialPath = ExtractFilePath(mEmulatorP->DiskBFilename);
			FilesDialog->SelectedFile = ExtractFileName(mEmulatorP->DiskBFilename);
		}
		else
		{
			FilesDialog->InitialPath = mSettingsP->DiscDirectory;
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LoadimageintoDriveBMenuClick(TObject *Sender)
{
	if ( (mDrawingContextP->Fullscreen == false)
			 || (mSettingsP->FullscreenMenu == true) )
	{
		OpenDialog->Title = "Load disk image on Drive B";
		OpenDialog->FileName = mEmulatorP->DiskBFilename;
		OpenDialog->Filter = DISKIMAGE_FILTER_OPEN;
		OpenDialog->InitialDir = mSettingsP->DiscDirectory;
		if (OpenDialog->Execute() == true)
		{
			LoadDriveB(OpenDialog->FileName);
		}
	}
	else
	{
		if (mMouseNecessary == false)
		{
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(TRUE);
			}
		}
		Screen->Cursor = mOldCursor;

		// Create openfile dialog
		mFullscreenDialogForm = new TDialogOpenDSK(mDrawingContextP);
		mFullscreenDialogForm->OnEndEvent = OnFullscreenOpenDSKDialogEnd;

		TDialogOpenDSK* FilesDialog = (TDialogOpenDSK*)mFullscreenDialogForm;
		FilesDialog->DriveBActive = true;
		// Set initial path
		if (mEmulatorP->DiskBFilename.Length())
		{
			FilesDialog->InitialPath = ExtractFilePath(mEmulatorP->DiskBFilename);
			FilesDialog->SelectedFile = ExtractFileName(mEmulatorP->DiskBFilename);
		}
		else if (mEmulatorP->DiskAFilename.Length())
		{
			FilesDialog->InitialPath = ExtractFilePath(mEmulatorP->DiskAFilename);
			FilesDialog->SelectedFile = ExtractFileName(mEmulatorP->DiskAFilename);
		}
		else
		{
			FilesDialog->InitialPath = mSettingsP->DiscDirectory;
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SwapDrivesMenuClick(TObject *Sender)
{
	SwapDrives();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EjectbothMenuClick(TObject *Sender)
{
	EjectDriveA();
	EjectDriveB();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DriveAEjectMenuClick(TObject *Sender)
{
	EjectDriveA();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DriveBEjectMenuClick(TObject *Sender)
{
	EjectDriveB();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DriveAUpdateDiskImageMenuClick(TObject *Sender)
{
  if (mEmulatorP->DriveAAltered == true)
  {
    // Virtual disk
    if (ExtractFilePath(mEmulatorP->DiskAFilename).Length() == 0)
    {
      // Request real filename
	    if (mDrawingContextP->Fullscreen == false)
	    {
		    SaveDialog->Title = DriveAString + " - " + mEmulatorP->DiskAFilename;
        SaveDialog->FileName = mEmulatorP->DiskAFilename;
        SaveDialog->Filter = DISKIMAGE_FILTER_SAVE;
        SaveDialog->DefaultExt = DISKIMAGE_DSK_EXT;
        if (SaveDialog->Execute() == true)
        {
          mEmulatorP->ForceDiskAFilename(SaveDialog->FileName);
        }
      }
    }

    if (ExtractFilePath(mEmulatorP->DiskAFilename).Length() > 0)
    {
      if (mEmulatorP->UpdateDisk(DriveA) == errNone)
      {
			  AddEmulatorMessage(DriveAString + " disk image has been updated.");
      }
    }
    else
    {
      AddEmulatorMessage("BAD disk image path on " + DriveAString);
    }
  }
  else
  {
    AddEmulatorMessage("NO modified disk image on " + DriveAString);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DriveBUpdateDiskImageMenuClick(TObject *Sender)
{
  if (mEmulatorP->DriveBAltered == true)
  {
    // Virtual disk
    if (ExtractFilePath(mEmulatorP->DiskBFilename).Length() == 0)
    {
      // Request real filename
	    if (mDrawingContextP->Fullscreen == false)
	    {
		    SaveDialog->Title = DriveBString + " - " + mEmulatorP->DiskBFilename;
        SaveDialog->FileName = mEmulatorP->DiskBFilename;
        SaveDialog->Filter = DISKIMAGE_FILTER_SAVE;
        SaveDialog->DefaultExt = DISKIMAGE_DSK_EXT;
        if (SaveDialog->Execute() == true)
        {
          mEmulatorP->ForceDiskBFilename(SaveDialog->FileName);
        }
      }
    }

    if (ExtractFilePath(mEmulatorP->DiskBFilename).Length() > 0)
    {
      if (mEmulatorP->UpdateDisk(DriveB) == errNone)
      {
			  AddEmulatorMessage(DriveBString + " disk image has been updated.");
      }
    }
    else
    {
      AddEmulatorMessage("BAD disk image path on " + DriveBString);
    }
  }
  else
  {
    AddEmulatorMessage("NO modified disk image on " + DriveBString);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DriveAReloadDiskImageMenuClick(TObject *Sender)
{
  // Virtual disk
  if (ExtractFilePath(mEmulatorP->DiskAFilename).Length() == 0)
  {
    AddEmulatorMessage("Cannot reload virtual disk !!");
  }
  else
  {
    LoadDriveA(mEmulatorP->DiskAFilename, false);
    AddEmulatorMessage(DriveAString + " disk image has been reloaded.");
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DriveBReloadDiskImageMenuClick(TObject *Sender)
{
  // Virtual disk
  if (ExtractFilePath(mEmulatorP->DiskBFilename).Length() == 0)
  {
    AddEmulatorMessage("Cannot reload virtual disk !!");
  }
  else
  {
    LoadDriveB(mEmulatorP->DiskBFilename);
    AddEmulatorMessage(DriveBString + " disk image has been reloaded.");
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DriveAWriteProtectionMenuClick(TObject *Sender)
{
	if (mEmulatorP->DiskAFilename.IsEmpty() == false)
	{
		if (mEmulatorP->DriveAWriteProtected == false)
		{
			mEmulatorP->DriveAWriteProtected = true;
			AddEmulatorMessage(DriveAString + " is now write protected.");
		}
		else
		{
			mEmulatorP->DriveAWriteProtected = false;
			AddEmulatorMessage(DriveAString + " is no longer write protected.");

			// Disk image is read only ?
			if (mEmulatorP->NativeCPC->FDC->DriveA->read_only)
			{
				// Advise user
				Application->MessageBox(_T("Drive A disk image file has READ ONLY attribute.\r\nHence, modifications will not be saved!!"),
																_T("READ ONLY"),
																MB_OK | MB_ICONEXCLAMATION);
			}
		}

    UpdateDriveAMenu();
		PanelControlDriveAPosition->Color = DriveAWriteProtectionMenu->Checked ? clRed : cl3DLight;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DriveBWriteProtectionMenuClick(TObject *Sender)
{
	if (mEmulatorP->DiskBFilename.IsEmpty() == false)
	{
		if (mEmulatorP->DriveBWriteProtected == false)
		{
			mEmulatorP->DriveBWriteProtected = true;
			AddEmulatorMessage(DriveBString + " is now write protected.");
		}
		else
		{
			mEmulatorP->DriveBWriteProtected = false;
			AddEmulatorMessage(DriveBString + " is no longer write protected.");

			// Disk image is read only ?
			if (mEmulatorP->NativeCPC->FDC->DriveB->read_only)
			{
				// Advise user
				Application->MessageBox(_T("Drive B disk image file has READ ONLY attribute.\r\nHence, modifications will not be saved!!"),
																_T("READ ONLY"),
																MB_OK | MB_ICONEXCLAMATION);
			}
		}

    UpdateDriveBMenu();
		PanelControlDriveBPosition->Color = DriveBWriteProtectionMenu->Checked ? clRed : cl3DLight;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DriveAFlipSidesMenuClick(TObject *Sender)
{
  FlipSidesDriveA();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DriveBFlipSidesMenuClick(TObject *Sender)
{
  mEmulatorP->DriveBFlipped = (mEmulatorP->DriveBFlipped == false) ? true : false;
  UpdateDriveBMenu();

  AddEmulatorMessage(DriveBString + " sides have been flipped.");

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DriveACreateDATADiskMenuClick(TObject *Sender)
{
	CreateDriveA(NEW_DISK_FILENAME);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DriveBCreateDATADiskMenuClick(TObject *Sender)
{
	CreateDriveB(NEW_DISK_FILENAME);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DriveAAddToFavouriteMenuClick(TObject *Sender)
{
  AddFavouriteDisk(mEmulatorP->DiskAFilename);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DriveBAddToFavouriteMenuClick(TObject *Sender)
{
  AddFavouriteDisk(mEmulatorP->DiskBFilename);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ConfirmDiskImagesUpdatesMenuClick(TObject *Sender)
{
	if (ConfirmDiskImagesUpdatesMenu->Checked == false)
	{
		ConfirmDiskImagesUpdatesMenu->Checked = true;
  }
  else
  {
		ConfirmDiskImagesUpdatesMenu->Checked = false;
  }

  mEmulatorP->ConfirmDiskImageUpdates = ConfirmDiskImagesUpdatesMenu->Checked;
  mSettingsP->ConfirmDiskImageUpdates = ConfirmDiskImagesUpdatesMenu->Checked;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::NeverUpdateDiskImagesMenuClick(TObject *Sender)
{
	if (NeverUpdateDiskImagesMenu->Checked == false)
	{
		NeverUpdateDiskImagesMenu->Checked = true;
    ConfirmDiskImagesUpdatesMenu->Enabled = false; 
  }
  else
  {
		NeverUpdateDiskImagesMenu->Checked = false;
    ConfirmDiskImagesUpdatesMenu->Enabled = true; 
  }

  mEmulatorP->NeverUpdateDiskImages = NeverUpdateDiskImagesMenu->Checked;
  mSettingsP->NeverUpdateDiskImages = NeverUpdateDiskImagesMenu->Checked;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Drive Popup Menu
//
void __fastcall TFormMain::DrivePopupMenuPopup(TObject *Sender)
{
  // Get Sender
  TPanel* ParentPanel = (TPanel*)DrivePopupMenu->PopupComponent;
  TDiskDrive Drive = (ParentPanel == PanelControlDriveAPosition) ? DriveA : DriveB;

  if (Drive == DriveA)
  {
    PopupDriveFlipSidesMenu->Enabled = DriveAFlipSidesMenu->Enabled;
    PopupDriveFlipSidesMenu->Checked = DriveAFlipSidesMenu->Checked;
    PopupDriveUpdateDiskImageMenu->Enabled = DriveAUpdateDiskImageMenu->Enabled;
    PopupDriveReloadDiskImageMenu->Enabled = DriveAReloadDiskImageMenu->Enabled;
    PopupDriveAddToFavouriteMenu->Enabled = DriveAAddToFavouriteMenu->Enabled;

    // Events
    PopupDriveFlipSidesMenu->OnClick = DriveAFlipSidesMenuClick;
    PopupDriveCreateDATADiskMenu->OnClick = DriveACreateDATADiskMenuClick;
    PopupDriveUpdateDiskImageMenu->OnClick = DriveAUpdateDiskImageMenuClick;
    PopupDriveReloadDiskImageMenu->OnClick = DriveAReloadDiskImageMenuClick;
    PopupDriveAddToFavouriteMenu->OnClick = DriveAAddToFavouriteMenuClick;
  }
  else
  {
    PopupDriveFlipSidesMenu->Enabled = DriveBFlipSidesMenu->Enabled;
    PopupDriveFlipSidesMenu->Checked = DriveBFlipSidesMenu->Checked;
    PopupDriveUpdateDiskImageMenu->Enabled = DriveBUpdateDiskImageMenu->Enabled;
    PopupDriveReloadDiskImageMenu->Enabled = DriveBReloadDiskImageMenu->Enabled;
    PopupDriveAddToFavouriteMenu->Enabled = DriveBAddToFavouriteMenu->Enabled;

    PopupDriveFlipSidesMenu->OnClick = DriveBFlipSidesMenuClick;
    PopupDriveCreateDATADiskMenu->OnClick = DriveBCreateDATADiskMenuClick;
    PopupDriveUpdateDiskImageMenu->OnClick = DriveBUpdateDiskImageMenuClick;
    PopupDriveReloadDiskImageMenu->OnClick = DriveBReloadDiskImageMenuClick;
    PopupDriveAddToFavouriteMenu->OnClick = DriveBAddToFavouriteMenuClick;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Tape Menu
//
void __fastcall TFormMain::PlayTapeMenuClick(TObject *Sender)
{
  if (PlayTapeMenu->Enabled == false)
    return;

	if (PlayTapeMenu->Checked == false)
	{
		PlayTape();
	}
	else
	{
		StopTape();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RewindTapeMenuClick(TObject *Sender)
{
	mEmulatorP->RewindTape();
	AddEmulatorMessage("Tape rewind");

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RewindToCounter000MenuClick(TObject *Sender)
{
	mEmulatorP->RewindToTapeCounter000();
	AddEmulatorMessage("Tape rewind to 000");

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::GoToPreviousTagMenuClick(TObject *Sender)
{
	if (mEmulatorP->GoToPreviousMessage() == 0)
	{
		AddEmulatorMessage("Begin of tape");
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::GoToNextTagMenuClick(TObject *Sender)
{
	if (mEmulatorP->GoToNextMessage() == 0)
	{
		AddEmulatorMessage("End of tape");
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ResetCounterTo000MenuClick(TObject *Sender)
{
	mEmulatorP->ResetTapeCounterTo000();
	AddEmulatorMessage("Tape counter reset to 000");

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AutoPlayTapeMenuClick(TObject *Sender)
{
	mEmulatorP->RewindTape();
  PlayTape();
  //mEmulatorP->StartAutoType("RUN\"\n\n");
  mEmulatorP->StartAutoPlay();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::EnableAutoPlayAtInsertMenuClick(TObject *Sender)
{
	mSettingsP->AutoPlayEnable = mSettingsP->AutoPlayEnable == false ? true : false;
	EnableAutoPlayAtInsertMenu->Checked = mSettingsP->AutoPlayEnable;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::InsertTapeMenuClick(TObject *Sender)
{
  if (InsertTapeMenu->Enabled == false) return;

	if ( (mDrawingContextP->Fullscreen == false)
			 || (mSettingsP->FullscreenMenu == true) )
	{
		OpenDialog->Title = "Load tape image";
		OpenDialog->FileName = mEmulatorP->TapeFilename;
		OpenDialog->Filter = TAPEIMAGE_FILTER;
		OpenDialog->InitialDir = mSettingsP->TapeDirectory;
		if (OpenDialog->Execute() == true)
		{
			InsertTape(OpenDialog->FileName, true);
		}
	}
	else
	{
		if (mMouseNecessary == false)
		{
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(TRUE);
			}
		}
		Screen->Cursor = mOldCursor;

		// Create openfile dialog
		mFullscreenDialogForm = new TDialogOpenCDT(mDrawingContextP);
		mFullscreenDialogForm->OnEndEvent = OnFullscreenOpenCDTDialogEnd;

		TDialogOpenCDT* FilesDialog = (TDialogOpenCDT*)mFullscreenDialogForm;
		// Set initial path
		if (mEmulatorP->TapeFilename.Length())
		{
			FilesDialog->InitialPath = ExtractFilePath(mEmulatorP->TapeFilename);
			FilesDialog->SelectedFile = ExtractFileName(mEmulatorP->TapeFilename);
		}
		else
		{
			FilesDialog->InitialPath = mSettingsP->TapeDirectory;
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EjectTapeMenuClick(TObject *Sender)
{
	if (mEmulatorP->TapeFilename.Length())
	{
		mEmulatorP->StopTape();
		mEmulatorP->EjectTape();
		AddEmulatorMessage("Tape ejected");

		if (mInfoFormP)
		{
			mInfoFormP->TapeFilename = "";
		}

		SpeedButtonControlTapeRewind->Enabled = false;
		SpeedButtonControlTapePrevious->Enabled = false;
		SpeedButtonControlTapePlayStop->Enabled = false;
		SpeedButtonControlTapeNext->Enabled = false;

		EjectTapeMenu->Enabled = false;
		PlayTapeMenu->Enabled = false;
		AutoPlayTapeMenu->Enabled = false;
		RewindTapeMenu->Enabled = false;
		RewindToCounter000Menu->Enabled = false;
		ResetCounterTo000Menu->Enabled = false;
		GoToPreviousTagMenu->Enabled = false;
		GoToNextTagMenu->Enabled = false;
		InsertTapeMenu->Enabled = true;

		// Signal CDT event to all subscribers
		for (int Index=0; Index < mOnEmulatorTapeEventList->Count; Index++)
		{
			TEmulatorTapeEvent* Event = (TEmulatorTapeEvent*)mOnEmulatorTapeEventList->Items[Index];
			((TEmulatorTapeEvent)*Event)(mEmulatorP->NativeCPC->Tape);
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Inputs Menu
//
//void __fastcall TFormMain::DecathlonModeMenuClick(TObject *Sender)
//{
//	// Deactivate Combat School mode
//	if (CombatSchoolModeMenu->Checked == true)
//	{
//		CombatSchoolModeMenuClick(Sender);
//	}
//
//	if (DecathlonModeMenu->Checked == false)
//	{
//		DecathlonModeMenu->Checked = true;
//		mEmulatorP->StartDecathlonMode();
//    if (!(mSettingsP->OnscreenDisplay & OSD_ICONS))
//    {
//		  AddEmulatorMessage("Decathlon started (Use Left and Right to adjust speed)");
//    }
//	}
//	else
//	{
//		DecathlonModeMenu->Checked = false;
//		mEmulatorP->StopDecathlonMode();
//    if (!(mSettingsP->OnscreenDisplay & OSD_ICONS))
//    {
//		  AddEmulatorMessage("Decathlon stopped");
//    }
//	}
//
//	NOT_USED(Sender);
//}
////---------------------------------------------------------------------------

//void __fastcall TFormMain::CombatSchoolModeMenuClick(TObject *Sender)
//{
//	// Deactivate Decathlon mode
//	if (DecathlonModeMenu->Checked == true)
//	{
//		DecathlonModeMenuClick(Sender);
//	}
//
//	if (CombatSchoolModeMenu->Checked == false)
//	{
//		CombatSchoolModeMenu->Checked = true;
//		mEmulatorP->StartCombatSchoolMode();
//    if (!(mSettingsP->OnscreenDisplay & OSD_ICONS))
//    {
//		  AddEmulatorMessage("Combat School started (Use Left and Right to adjust speed)");
//    }
//	}
//	else
//	{
//		CombatSchoolModeMenu->Checked = false;
//		mEmulatorP->StopCombatSchoolMode();
//    if (!(mSettingsP->OnscreenDisplay & OSD_ICONS))
//    {
//		  AddEmulatorMessage("Combat School stopped");
//    }
//	}
//
//	NOT_USED(Sender);
//}
////---------------------------------------------------------------------------


//void __fastcall TFormMain::KeyboardAsJoystickMenuClick(TObject *Sender)
//{
//	if (KeyboardAsJoystickMenu->Checked == false)
//	{
//		KeyboardAsJoystickMenu->Checked = true;
//		AddEmulatorMessage("Keyboard as Joystick");
//		// Start keyboard as joystick
//		mEmulatorP->StartKeyboardAsJoystick();
//	}
//	else
//	{
//		KeyboardAsJoystickMenu->Checked = false;
//		AddEmulatorMessage("Keyboard no longer used as Joystick");
//		// Stop keyboard as joystick
//		mEmulatorP->StopKeyboardAsJoystick();
//	}
//
//	NOT_USED(Sender);
//}
////---------------------------------------------------------------------------

void __fastcall TFormMain::MouseAsLightpenMenuClick(TObject *Sender)
{
	if (MouseAsLightpenMenu->Checked == false)
	{
		if (MouseAsMagnumLightPhaserMenu->Checked == true)
		{
			MouseAsMagnumLightPhaserMenu->Checked = false;
			mEmulatorP->UnplugMagnumLightPhaser();
		}
		else if (MouseAsGunStickMenu->Checked == true)
		{
			MouseAsGunStickMenu->Checked = false;
		}
		else if (MouseAsWestPhaserMenu->Checked == true)
		{
			mEmulatorP->UnplugWestPhaser();
			MouseAsWestPhaserMenu->Checked = false;
		}
#ifdef ENABLE_AMX_MOUSE
		else if (MouseAsAMXMouseMenu->Checked == true)
		{
			mEmulatorP->UnplugAMXMouse();
			MouseAsAMXMouseMenu->Checked = false;
      // Enable Screen popup menu
      PanelCPCScreen->PopupMenu = PopupMenuScreen;
		}
#endif /* ENABLE_AMX_MOUSE */
		else
		{
			if (mDrawingContextP->Fullscreen == true)
			{
				Screen->Cursor = crFullscreenCursor;
				if (mSettingsP->FullscreenMenu == false)
				{
					ShowCursor(TRUE);
				}
			}
		}

		MouseAsLightpenMenu->Checked = true;
		AddEmulatorMessage("Mouse as Lightpen");
		mMouseNecessary = true;

		// Set mouse behaviour
		OnEmulatorMouseDown = NULL;
		OnEmulatorMouseUp = NULL;
		OnEmulatorMouseMove = OnLightpenMove;
	}
	else
	{
		MouseAsLightpenMenu->Checked = false;
		mMouseNecessary = false;

		if (mDrawingContextP->Fullscreen == true)
		{
			Screen->Cursor = mOldCursor;
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(FALSE);
			}
		}

		AddEmulatorMessage("Mouse no longer used as Lightpen");

		// Reset mouse behaviour
		OnEmulatorMouseDown = NULL;
		OnEmulatorMouseUp = NULL;
		OnEmulatorMouseMove = NULL;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MouseAsMagnumLightPhaserMenuClick(TObject *Sender)
{
	if (MouseAsMagnumLightPhaserMenu->Checked == false)
	{
		if (MouseAsLightpenMenu->Checked == true)
		{
			MouseAsLightpenMenu->Checked = false;
		}
		else if (MouseAsGunStickMenu->Checked == true)
		{
			MouseAsGunStickMenu->Checked = false;
		}
		else if (MouseAsWestPhaserMenu->Checked == true)
		{
			mEmulatorP->UnplugWestPhaser();
			MouseAsWestPhaserMenu->Checked = false;
		}
#ifdef ENABLE_AMX_MOUSE
		else if (MouseAsAMXMouseMenu->Checked == true)
		{
			mEmulatorP->UnplugAMXMouse();
			MouseAsAMXMouseMenu->Checked = false;
      // Enable Screen popup menu
      PanelCPCScreen->PopupMenu = PopupMenuScreen;
		}
#endif /* ENABLE_AMX_MOUSE */
		else
		{
			if (mDrawingContextP->Fullscreen == true)
			{
				Screen->Cursor = crFullscreenCursor;
				if (mSettingsP->FullscreenMenu == false)
				{
					ShowCursor(TRUE);
				}
			}
		}

		MouseAsMagnumLightPhaserMenu->Checked = true;
		mEmulatorP->PlugMagnumLightPhaser();
		AddEmulatorMessage("Mouse as Magnum light phaser");
		mMouseNecessary = true;

		// Set mouse behaviour
		OnEmulatorMouseDown = OnMagnumLightPhaserPressed;
		OnEmulatorMouseUp = OnMagnumLightPhaserReleased;
		OnEmulatorMouseMove = NULL;
	}
	else
	{
		MouseAsMagnumLightPhaserMenu->Checked = false;
		mEmulatorP->UnplugMagnumLightPhaser();
		mMouseNecessary = false;

		if (mDrawingContextP->Fullscreen == true)
		{
			Screen->Cursor = mOldCursor;
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(FALSE);
			}
		}

		AddEmulatorMessage("Mouse no longer used as Magnum light phaser");

		// Reset mouse behaviour
		OnEmulatorMouseDown = NULL;
		OnEmulatorMouseUp = NULL;
		OnEmulatorMouseMove = NULL;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MouseAsGunStickMenuClick(TObject *Sender)
{
	if (MouseAsGunStickMenu->Checked == false)
	{
		if (MouseAsLightpenMenu->Checked == true)
		{
			MouseAsLightpenMenu->Checked = false;
		}
		else if (MouseAsMagnumLightPhaserMenu->Checked == true)
		{
			MouseAsMagnumLightPhaserMenu->Checked = false;
			mEmulatorP->UnplugMagnumLightPhaser();
		}
		else if (MouseAsWestPhaserMenu->Checked == true)
		{
			mEmulatorP->UnplugWestPhaser();
			MouseAsWestPhaserMenu->Checked = false;
		}
#ifdef ENABLE_AMX_MOUSE
		else if (MouseAsAMXMouseMenu->Checked == true)
		{
			mEmulatorP->UnplugAMXMouse();
			MouseAsAMXMouseMenu->Checked = false;
      // Enable Screen popup menu
      PanelCPCScreen->PopupMenu = PopupMenuScreen;
		}
#endif /* ENABLE_AMX_MOUSE */
		else
		{
			if (mDrawingContextP->Fullscreen == true)
			{
				Screen->Cursor = crFullscreenCursor;
				if (mSettingsP->FullscreenMenu == false)
				{
					ShowCursor(TRUE);
				}
			}
		}

		MouseAsGunStickMenu->Checked = true;
		AddEmulatorMessage("Mouse as GunStick");
		mMouseNecessary = true;

		// Set mouse behaviour
		OnEmulatorMouseDown = OnGunStickPressed;
		OnEmulatorMouseUp = OnGunStickReleased;
		OnEmulatorMouseMove = NULL;
	}
	else
	{
		MouseAsGunStickMenu->Checked = false;
		mMouseNecessary = false;

		if (mDrawingContextP->Fullscreen == true)
		{
			Screen->Cursor = mOldCursor;
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(FALSE);
			}
		}

		AddEmulatorMessage("Mouse no longer used as GunStick");

		// Reset mouse behaviour
		OnEmulatorMouseDown = NULL;
		OnEmulatorMouseUp = NULL;
		OnEmulatorMouseMove = NULL;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MouseAsWestPhaserMenuClick(TObject *Sender)
{
	if (MouseAsWestPhaserMenu->Checked == false)
	{
		if (MouseAsLightpenMenu->Checked == true)
		{
			MouseAsLightpenMenu->Checked = false;
		}
		else if (MouseAsMagnumLightPhaserMenu->Checked == true)
		{
			MouseAsMagnumLightPhaserMenu->Checked = false;
			mEmulatorP->UnplugMagnumLightPhaser();
		}
		else if (MouseAsGunStickMenu->Checked == true)
		{
			MouseAsGunStickMenu->Checked = false;
		}
#ifdef ENABLE_AMX_MOUSE
		else if (MouseAsAMXMouseMenu->Checked == true)
		{
			mEmulatorP->UnplugAMXMouse();
			MouseAsAMXMouseMenu->Checked = false;
      // Enable Screen popup menu
      PanelCPCScreen->PopupMenu = PopupMenuScreen;
		}
#endif /* ENABLE_AMX_MOUSE */
		else
		{
			if (mDrawingContextP->Fullscreen == true)
			{
				Screen->Cursor = crFullscreenCursor;
				if (mSettingsP->FullscreenMenu == false)
				{
					ShowCursor(TRUE);
				}
			}
		}

		MouseAsWestPhaserMenu->Checked = true;
		mEmulatorP->PlugWestPhaser();
		OnEmulatorMouseMove = OnWestPhaserMove;
		AddEmulatorMessage("Mouse as WestPhaser");
		mMouseNecessary = true;

		// Set mouse behaviour
		OnEmulatorMouseDown = OnWestPhaserPressed;
		OnEmulatorMouseUp = OnWestPhaserReleased;
		OnEmulatorMouseMove = NULL;
	}
	else
	{
		MouseAsWestPhaserMenu->Checked = false;
    mEmulatorP->UnplugWestPhaser();
		mMouseNecessary = false;

		if (mDrawingContextP->Fullscreen == true)
		{
			Screen->Cursor = mOldCursor;
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(FALSE);
			}
		}

		AddEmulatorMessage("Mouse no longer used as WestPhaser");

		// Reset mouse behaviour
		OnEmulatorMouseDown = NULL;
		OnEmulatorMouseUp = NULL;
		OnEmulatorMouseMove = NULL;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MouseAsAMXMouseMenuClick(TObject *Sender)
{
	if (MouseAsAMXMouseMenu->Checked == false)
	{
		if (MouseAsLightpenMenu->Checked == true)
		{
			MouseAsLightpenMenu->Checked = false;
		}
		else if (MouseAsMagnumLightPhaserMenu->Checked == true)
		{
			MouseAsMagnumLightPhaserMenu->Checked = false;
			mEmulatorP->UnplugMagnumLightPhaser();
		}
		else if (MouseAsGunStickMenu->Checked == true)
		{
			MouseAsGunStickMenu->Checked = false;
		}
		else if (MouseAsWestPhaserMenu->Checked == true)
		{
			mEmulatorP->UnplugWestPhaser();
			MouseAsWestPhaserMenu->Checked = false;
		}
		else
		{
			if (mDrawingContextP->Fullscreen == true)
			{
				Screen->Cursor = crFullscreenCursor;
				if (mSettingsP->FullscreenMenu == false)
				{
					ShowCursor(TRUE);
				}
			}
		}

		MouseAsAMXMouseMenu->Checked = true;
		mEmulatorP->PlugAMXMouse();
		AddEmulatorMessage("Mouse as AMX Mouse");
		mMouseNecessary = true;

		// Set mouse behaviour
		OnEmulatorMouseDown = OnAMXMousePressed;
		OnEmulatorMouseUp = OnAMXMouseReleased;
		OnEmulatorMouseMove = OnAMXMouseMove;

    // Disable Screen popup menu
    PanelCPCScreen->PopupMenu = NULL;
	}
	else
	{
		MouseAsAMXMouseMenu->Checked = false;
    mEmulatorP->UnplugAMXMouse();
		mMouseNecessary = false;

		if (mDrawingContextP->Fullscreen == true)
		{
			Screen->Cursor = mOldCursor;
			if (mSettingsP->FullscreenMenu == false)
			{
				ShowCursor(FALSE);
			}
		}

		AddEmulatorMessage("Mouse no longer used as AMX Mouse");

		// Reset mouse behaviour
		OnEmulatorMouseDown = NULL;
		OnEmulatorMouseUp = NULL;
		OnEmulatorMouseMove = NULL;

    // Enable Screen popup menu
    PanelCPCScreen->PopupMenu = PopupMenuScreen;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AMXMouseSpeed100MenuClick(TObject *Sender)
{
  EmulatorP->SetAMXMouseXFactor(1.0);
  AMXMouseSpeed100Menu->Checked = true;
  AddEmulatorMessage("AMX Mouse speed at 100%");

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::AMXMouseSpeed200MenuClick(TObject *Sender)
{
  EmulatorP->SetAMXMouseXFactor(2.0);
  AMXMouseSpeed200Menu->Checked = true;
  AddEmulatorMessage("AMX Mouse speed at 200%");

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::UseHardwareKeyboardMappingMenuClick(TObject *Sender)
{
  if (UseHardwareKeyboardMappingMenu->Checked == false)
  {
    UseHardwareKeyboardMappingMenu->Checked = true;
  }
  else
  {
    UseHardwareKeyboardMappingMenu->Checked = false;
  }

  mSettingsP->UseHardwareKeyboardMapping = UseHardwareKeyboardMappingMenu->Checked;

	// Restart emulator with new keyboard mapping
	EmulatorP->UpdateKeyboardMapping();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::KeyboardMappingMenuClick(TObject *Sender)
{
	// Stop background emulation
	TIdleEvent oldEvent = Application->OnIdle;
	Application->OnIdle = NULL;

	FormKeyboardMapping->Settings = mSettingsP;
	FormKeyboardMapping->KeyboardCPC = mEmulatorP->KeyboardCPC;
	if (FormKeyboardMapping->ShowModal() == mrOk)
	{
		// Clear ASCII keylist
		mSettingsP->ClearNonASCIIKeys();
		// Update ASCII keylist
		for (tKeyboardMappingKey* KeyP = FormKeyboardMapping->GetFirstASCIIKey();
				 KeyP != NULL;
				 KeyP = FormKeyboardMapping->GetNextASCIIKey())
		{
			mSettingsP->AddNonASCIIKey(KeyP);
		}

		// Clear Hardware keylist
		mSettingsP->ClearHardwareKeys();
		// Update Hardware keylist
		for (tKeyboardMappingKey* KeyP = FormKeyboardMapping->GetFirstHardwareKey();
				 KeyP != NULL;
				 KeyP = FormKeyboardMapping->GetNextHardwareKey())
		{
			mSettingsP->AddHardwareKey(KeyP);
		}

		// Clear Joystick keylist
		mSettingsP->ClearJoystickKeys();
		// Update Hardware keylist
		for (tKeyboardMappingKey* KeyP = FormKeyboardMapping->GetFirstJoystickKey();
				 KeyP != NULL;
				 KeyP = FormKeyboardMapping->GetNextJoystickKey())
		{
			mSettingsP->AddJoystickKey(KeyP);
		}

		// Restart emulator with new keyboard mapping
		mEmulatorP->UpdateKeyboardMapping();

    // Update menu
    UseHardwareKeyboardMappingMenu->Checked = mSettingsP->UseHardwareKeyboardMapping;
	}

	// Restart background emulation
	Application->OnIdle = oldEvent;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AutoTypeFromClipboardMenuClick(TObject *Sender)
{
	if (OpenClipboard(NULL))
	{
		HANDLE clip = GetClipboardData(CF_TEXT);

		if (clip)
		{
			mEmulatorP->StartAutoType((char*)clip);
		}

		CloseClipboard();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::GameControllersSettingsMenuClick(TObject *Sender)
{
	TIdleEvent oldEvent = Application->OnIdle;
	Application->OnIdle = NULL;

	// Stop game controllers
	mInputContextP->Stop();

	FormGameControllerSettings->Settings = mSettingsP;
	FormGameControllerSettings->InputContextP = mInputContextP;
	FormGameControllerSettings->CurrentProfile = mInputContextP->ProfileFilename;
	FormGameControllerSettings->DefaultProfile = mSettingsP->DefaultProfileFilename;
	if (EmulatorP->DiskAFilename.Length())
	{
		FormGameControllerSettings->DriveAProfile = ChangeFileExt(ExtractFileName(EmulatorP->DiskAFilename), PROFILE_EXT);
	}
	else
	{
		FormGameControllerSettings->DriveAProfile = "";
	}
	if (EmulatorP->DiskBFilename.Length())
	{
		FormGameControllerSettings->DriveBProfile = ChangeFileExt(ExtractFileName(EmulatorP->DiskBFilename), PROFILE_EXT);
	}
	else
	{
		FormGameControllerSettings->DriveBProfile = "";
	}

	TStringList* ProfileP = new TStringList();
	AnsiString ProfilePath = ExpandFileName(mSettingsP->ProfileDirectory);

	if (FormGameControllerSettings->ShowModal() == mrOk)
	{
		// Store modifications into current profile
		mInputContextP->SaveGameControllersProfile(ProfileP);

		// Save current profile
		mInputContextP->ProfileFilename = FormGameControllerSettings->SelectedProfile;
		ProfileP->SaveToFile(ProfilePath + mInputContextP->ProfileFilename);

		// Reload current profile to apply modifications
		mInputContextP->LoadGameControllersProfile(ProfileP);
	}
	else
	{
    // Reset current profile
    mInputContextP->NewGameControllersProfile(false);

		// Reload current profile
    if (FileExists(ProfilePath + mInputContextP->ProfileFilename))
    {
		  ProfileP->LoadFromFile(ProfilePath + mInputContextP->ProfileFilename);
		  mInputContextP->LoadGameControllersProfile(ProfileP);
    }
	}

	delete ProfileP;

	// Restart game controllers
	mInputContextP->OnWINKeyDown = OnWINInputKeyDown;
	mInputContextP->OnWINKeyUp = OnWINInputKeyUp;
	mInputContextP->Start(Handle);

	Application->OnIdle = oldEvent;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LoadDefaultProfileMenuClick(TObject *Sender)
{
	mInputContextP->ProfileFilename = ExtractFileName(mSettingsP->DefaultProfileFilename);
	AnsiString ProfilePath = ExpandFileName(mSettingsP->ProfileDirectory);
	AnsiString ProfileFilename = ProfilePath + mInputContextP->ProfileFilename;
	if (FileExists(ProfileFilename) == true)
	{
		TStringList* DefaultProfileP = new TStringList();
		DefaultProfileP->LoadFromFile(ProfileFilename);
		mInputContextP->LoadGameControllersProfile(DefaultProfileP);
		delete DefaultProfileP;

    AddEmulatorMessage("Default profile loaded");
	}
	else
	{
		mInputContextP->NewGameControllersProfile(true);
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LoadDriveAProfileClick(TObject *Sender)
{
  AnsiString ProfileFilename = ExtractFileName(mSettingsP->DefaultProfileFilename);
	if (mEmulatorP->DiskAFilename.Length())
	{
		ProfileFilename = ChangeFileExt(ExtractFileName(mEmulatorP->DiskAFilename), PROFILE_EXT);
	}

  // Different profile ?
  if (ProfileFilename != mInputContextP->ProfileFilename)
  {
    mInputContextP->ProfileFilename = ProfileFilename;
    AnsiString ProfilePath = ExpandFileName(mSettingsP->ProfileDirectory);
    AnsiString ProfileFilename = ProfilePath + mInputContextP->ProfileFilename;
    if (FileExists(ProfileFilename) == true)
    {
      TStringList* ProfileP = new TStringList();
      ProfileP->LoadFromFile(ProfileFilename);
      mInputContextP->LoadGameControllersProfile(ProfileP);
      delete ProfileP;

      AddEmulatorMessage("Profile loaded=" + mInputContextP->ProfileFilename);
    }
    else
    {
      // Load default profile
      LoadDefaultProfileMenuClick(Sender);
    }
  }
}
//---------------------------------------------------------------------------


//
// Display Menu
//
void __fastcall TFormMain::ReduceDisplayMenuClick(TObject *Sender)
{
	if (mDrawingContextP->Fullscreen == false)
	{
		int Width = (int)((float)ClientWidth / mDrawingContextP->ZoomFactor);
		// Back to default screen size
		if (Width < CPC_VISIBLE_SCR_WIDTH)
		{
			Width = CPC_VISIBLE_SCR_WIDTH;
		}

		int Height;
		if (PanelControl->Visible == true)
		{
			Height = PanelCPCScreen->Height;
			Height = (int)((float)Height / mDrawingContextP->ZoomFactor);
		}
		else
		{
			Height = (int)((float)ClientHeight / mDrawingContextP->ZoomFactor);
		}

		// Back to default screen size
		if (Height < CPC_VISIBLE_SCR_HEIGHT)
		{
			Height = CPC_VISIBLE_SCR_HEIGHT;
		}

		if (PanelControl->Visible == true)
		{
			// Add panel height
			Height += PanelControl->Height;
		}

		// Update client area if needed
		if (Width != ClientWidth)
		{
			ClientWidth = Width;
		}
		if (Height != ClientHeight)
		{
			ClientHeight = Height;
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EnlargeDisplayMenuClick(TObject *Sender)
{
	if (mDrawingContextP->Fullscreen == false)
	{
		ClientWidth = (int)((float)ClientWidth * mDrawingContextP->ZoomFactor);
		if (PanelControl->Visible == true)
		{
			int Height = (int)((float)PanelCPCScreen->Height * mDrawingContextP->ZoomFactor);
			ClientHeight = Height + PanelControl->Height;
		}
		else
		{
			ClientHeight = (int)((float)ClientHeight * mDrawingContextP->ZoomFactor);
		}
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FullscreenMenuClick(TObject *Sender)
{
	if (mDrawingContextP->Fullscreen == false)
	{
		SetFullscreen();
	}
	else
	{
		SetWindowed();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DisplaySettingsMenuClick(TObject *Sender)
{
	TFormDisplaySettings* DisplaySettingsForm = new TFormDisplaySettings(this);
	DisplaySettingsForm->Settings = mSettingsP;
	DisplaySettingsForm->DrawingContext = mDrawingContextP;
  DisplaySettingsForm->NativeCPC = mEmulatorP->NativeCPC;

	if (DisplaySettingsForm->ShowModal() == mrOk)
  {
    // Apply new settings
    if (FormDebugDSKViewer) FormDebugDSKViewer->Settings = mSettingsP;
    if (FormDebugCDTViewer) FormDebugCDTViewer->Settings = mSettingsP;
    if (FormDebugger) FormDebugger->Settings = mSettingsP;
    if (FormDebugZ80Editor) FormDebugZ80Editor->Settings = mSettingsP;
    if (FormDebugMemoryEditor) FormDebugMemoryEditor->Settings = mSettingsP;
    if (FormDebugCRTC) FormDebugCRTC->Settings = mSettingsP;
    if (FormDebugGateArray) FormDebugGateArray->Settings = mSettingsP;
    if (FormDebugPSG) FormDebugPSG->Settings = mSettingsP;
    if (FormDebugFDC) FormDebugFDC->Settings = mSettingsP;
    if (FormDebugTapeAudioWorkshop) FormDebugTapeAudioWorkshop->Settings = mSettingsP;
    if (FormDebugMEA8000) FormDebugMEA8000->Settings = mSettingsP;
    if (FormDebugTextCapture) FormDebugTextCapture->Settings = mSettingsP;
    if (FormCheatScripts) FormCheatScripts->Settings = mSettingsP;
    if (FormPrinter) FormPrinter->Settings = mSettingsP;
#if (__BORLANDC__ != 0x551)
    if (FormGraphicsExplorer) FormGraphicsExplorer->Settings = mSettingsP;
#endif
	//roger
//	if (FormDebugROMOperations) FormDebugROMOperations->Settings = mSettingsP;
  }

	delete DisplaySettingsForm;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ControlPanelMenuClick(TObject *Sender)
{
  // Prevent keeping aspect ratio
  OnCanResize = NULL;

	// Adapt form size
	if (PanelControl->Visible == false)
	{
		Height += PanelControl->Height;
        mFormFrameHeight += PanelControl->Height;
		PanelControl->Visible = true;
		SubscribeToDiskEvents(OnDiskEvent);
		SubscribeToFDCEvents(OnFDCEvent);
		// Force panel update
		mEmulatorP->DriveAModified = true;
		mEmulatorP->DriveBModified = true;
	}
	else
	{
		PanelControl->Visible = false;
		Height -= PanelControl->Height;
    mFormFrameHeight -= PanelControl->Height;
		UnsubscribeToDiskEvents(OnDiskEvent);
		UnsubscribeToFDCEvents(OnFDCEvent);
	}

	mSettingsP->ControlPanelVisible = PanelControl->Visible;
	ControlPanelMenu->Checked = PanelControl->Visible;

  // Restore aspect ratio check
  OnCanResize = FormCanResize;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ToggleColorMonochromeMenuClick(TObject *Sender)
{
	// Update Emulator Palette
	mSettingsP->ColorMonitor = (mSettingsP->ColorMonitor == true) ? false : true;
	mEmulatorP->SetScreenType(mSettingsP->ColorMonitor);
	mEmulatorP->GetPalette(mDrawingContextP->Palette);
	mDrawingContextP->SetupPalette();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::NightmodeMenuClick(TObject *Sender)
{
	if (NightmodeMenu->Checked == false)
	{
		NightmodeMenu->Checked = true;
	}
	else
	{
		NightmodeMenu->Checked = false;
	}

	// Update Emulator Palette
	mEmulatorP->ToggleNightMode();
	mEmulatorP->GetPalette(mDrawingContextP->Palette);
	mDrawingContextP->SetupPalette();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ScreenshotMenuClick(TObject *Sender)
{
	String Filename = Date().FormatString(_T("yyyy-mm-dd"))
                    + "_"
                    + Time().FormatString(_T("hh-nn-ss"))
										+ BITMAP_EXT;
	String FullFilename = ExpandFileName(mSettingsP->ScreenshotDirectory)
												+ Filename;

	if (mDrawingContextP->SaveScreenshot(FullFilename, true) == true)
	{
		AddEmulatorMessage("Screenshot \"" + Filename + "\" has been created.");
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SmallScreenshotMenuClick(TObject *Sender)
{
	String Filename = Date().FormatString(_T("yyyy-mm-dd"))
                    + "_"
                    + Time().FormatString(_T("hh-nn-ss"))
										+ BITMAP_EXT;
	String FullFilename = ExpandFileName(mSettingsP->ScreenshotDirectory)
												+ Filename;

	if (mDrawingContextP->SaveScreenshot(FullFilename, false) == true)
	{
		AddEmulatorMessage("Screenshot \"" + Filename + "\" has been created.");
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::OpenScreenshotsFolderMenuClick(TObject *Sender)
{
	// Open screenshots folder using default viewer
	ShellExecute(Handle,
							 _T("open"),
							 ExpandFileName(mSettingsP->ScreenshotDirectory).c_str(),
							 0,
							 0,
							 SW_NORMAL);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Debug Menu
//
void __fastcall TFormMain::DebuggerMenuClick(TObject *Sender)
{
	FormDebugger->Visible = !FormDebugger->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SaveContextMenuClick(TObject *Sender)
{
#ifdef _DEBUG

	String Filename = Date().FormatString(_T("yyyy-mm-dd"))
                    + "_"
                    + Time().FormatString(_T("hh-nn-ss"));
	SaveDialog->Title = "Context content";
  SaveDialog->FileName = ExtractFilePath(Application->ExeName)
                         + String("Context ")
                         + Filename;
  SaveDialog->Filter = "All files|*.*";
  SaveDialog->DefaultExt = _T("bin");
  if (SaveDialog->Execute() == true)
  {
    mEmulatorP->SaveContext(SaveDialog->FileName);
  }

#endif /* _DEBUG */

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::EngineDataMenuClick(TObject *Sender)
{
   //	FormDebugEngine->Visible = !FormDebugEngine->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Z80EditorMenuClick(TObject *Sender)
{
	FormDebugZ80Editor->Visible = !FormDebugZ80Editor->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MemoryEditorMenuClick(TObject *Sender)
{
	FormDebugMemoryEditor->Visible = !FormDebugMemoryEditor->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::GraphicsExplorerMenuClick(TObject *Sender)
{
	FormGraphicsExplorer->Visible = !FormGraphicsExplorer->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CRTCRegistersMenuClick(TObject *Sender)
{
	FormDebugCRTC->Visible = !FormDebugCRTC->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::GateArrayRegistersMenuClick(TObject *Sender)
{
	FormDebugGateArray->Visible = !FormDebugGateArray->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PSGOperationsMenuClick(TObject *Sender)
{
	FormDebugPSG->AudioContext = mAudioContextP;
	FormDebugPSG->Visible = !FormDebugPSG->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FDCOperationsMenuClick(TObject *Sender)
{
	FormDebugFDC->Visible = !FormDebugFDC->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DSKViewerMenuClick(TObject *Sender)
{
	FormDebugDSKViewer->Emulator = mEmulatorP;
	FormDebugDSKViewer->Visible = !FormDebugDSKViewer->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CDTViewerMenuClick(TObject *Sender)
{
	FormDebugCDTViewer->Emulator = mEmulatorP;
	FormDebugCDTViewer->Visible = !FormDebugCDTViewer->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TapeAudioWorkshopMenuClick(TObject *Sender)
{
	FormDebugTapeAudioWorkshop->Emulator = mEmulatorP;
	FormDebugTapeAudioWorkshop->AudioContext = mAudioContextP;
	FormDebugTapeAudioWorkshop->Visible = !FormDebugTapeAudioWorkshop->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TextCaptureMenuClick(TObject *Sender)
{
	FormDebugTextCapture->Visible = !FormDebugTextCapture->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::MEA8000MenuClick(TObject *Sender)
{
	FormDebugMEA8000->AudioContext = mAudioContextP;
	FormDebugMEA8000->Visible = !FormDebugMEA8000->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FileEditorMenuClick(TObject *Sender)
{
#ifdef _DEBUG
	//FormDebugFileEditor->Emulator = mEmulatorP;
	//FormDebugFileEditor->Visible = !FormDebugFileEditor->Visible;
#endif /* _DEBUG */

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PrinterMenuClick(TObject *Sender)
{
	FormPrinter->Visible = !FormPrinter->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheatScriptsMenuClick(TObject *Sender)
{
	FormCheatScripts->Visible = !FormCheatScripts->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ROMOperationsMenuClick(TObject *Sender)
{
	//FormDebugROMOperations->Visible = !FormDebugROMOperations->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EmulatorSchedulingMenuClick(TObject *Sender)
{
	//FormDebugEmulatorScheduling->Visible = !FormDebugEmulatorScheduling->Visible;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LightgunMenuClick(TObject *Sender)
{
#ifdef _DEBUG
	//FormDebugLightgun->OnNextLightgunTriggerEvent = OnEmulatorNextLightgunTrigger;
	//FormDebugLightgun->Visible = !FormDebugLightgun->Visible;
#endif /* _DEBUG */

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Help Menu
//
void __fastcall TFormMain::HintsMenuClick(TObject *Sender)
{
	if (mDrawingContextP->Fullscreen == false)
	{
		TIdleEvent oldEvent = Application->OnIdle;
		Application->OnIdle = NULL;

		// Show Hints form
		FormHints->ShowModal();

		Application->OnIdle = oldEvent;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
  

void __fastcall TFormMain::AboutMenuClick(TObject *Sender)
{
TIdleEvent oldEvent = Application->OnIdle;

	Application->OnIdle = NULL;

	// Show About form
	FormAbout->AudioContext = mAudioContextP;
	FormAbout->DrawingContext = mDrawingContextP;
	FormAbout->InputContext = mInputContextP;
	FormAbout->Settings = mSettingsP;
	FormAbout->ShowModal();

	Application->OnIdle = oldEvent;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


//
// Engine
//
inline void __fastcall TFormMain::ExecuteVDUCondition(void)
{
#ifdef _DEBUG
	// Signal events to all subscribers
	for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
	{
		TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
		if (((TEmulatorCPCEvent)*Event)(EC_DEBUG_VDU_START, mEmulatorP->NativeCPC) == true)
    {
      // Pause emulation
      if (mEmulatorP->Paused == false)
      {
        Pause(true);
      }
    }
  }
#endif /* _DEBUG */

  //
  // Update Emulateur display
  //
  mDrawingContextP->DrawBegin();

  // Display Emulator screen
  mDrawingContextP->DrawEmulatorScreen();

  // Display fullspeed performance
  if (mSettingsP->OnscreenDisplay & OSD_MESSAGES)
  {
    if (mDrawingContextP->Fullscreen == true)
    {
      if (mFullSpeed == true)
      {
        String Text = String(mEmulatorPerformance) + _T("%");
        mDrawingContextP->DrawEmulatorText(Text,
                                           mDrawingContextP->EmulatorScreenLeft,
                                           mDrawingContextP->EmulatorScreenTop);
      }
    }
  }

#ifdef _DEBUG
  // Display Onscreen debug data
  if (mSettingsP->OnscreenDisplay & OSD_DEBUG)
  {
#ifdef SHOW_FPS
    if (mFPSDisplayed != -1)
    {
      String Text = _T("FPS=") + String(mFPSDisplayed);
      mDrawingContextP->DrawEmulatorText(Text,
                                         mDrawingContextP->EmulatorScreenLeft,
                                         mDrawingContextP->EmulatorScreenTop+10);
    }
#endif /* SHOW_FPS */
  }
#endif /* _DEBUG */

  // Show Tape play duration
  if (mSettingsP->OnscreenDisplay & OSD_TAPE)
  {
    if (mEmulatorP->IsTapePlaying() == true)
    {
      mDrawingContextP->DrawTape(mEmulatorP->GetTapePlayDuration(),
                                 mEmulatorP->GetTapeTotalDuration());
    }
  }

  // Show floppy bitmap
  if (mSettingsP->OnscreenDisplay & OSD_DISK)
  {
    mDrawingContextP->DrawFloppy(mEmulatorP->DriveLed, mEmulatorP->Paused);
  }

  // Display Info
  if (mInfoFormP)
  {
    mInfoFormP->OnDisplay(mDrawingContextP);
  }

  DisplayEmulatorMessage();

  if (mFullscreenDialogForm)
  {
    mFullscreenDialogForm->OnDisplay(mDrawingContextP);
  }

  // Show VDU Beam position
  if (mEmulatorP->StepByStepActive == true)
  {
    if (mDrawingContextP->Fullscreen == false)
    {
      int BeamX = Engine_GetVideoBeamPosX(mEmulatorP->NativeCPC);
      int BeamY = Engine_GetVideoBeamPosY(mEmulatorP->NativeCPC);

      // Show VDU Beam position
      mDrawingContextP->DrawVDUBeamPosition(BeamX, BeamY);
    }
  }

  //
  // Onscreen Icons
  //
  if (mSettingsP->OnscreenDisplay & OSD_ICONS)
  {
    mDrawingContextP->DrawOnscreenIconsBegin();

    // Show Mute bitmap
#ifndef SHOW_ALL_OSD_ICONS
    if (mSettingsP->Mute == true)
#endif /* SHOW_ALL_OSD_ICONS */
    {
      mDrawingContextP->DrawOnscreenIcon(MuteId);
    }

//    // Show Joystick bitmap
//#ifndef SHOW_ALL_OSD_ICONS
//    if (KeyboardAsJoystickMenu->Checked == true)
//#endif /* SHOW_ALL_OSD_ICONS */
//    {
//      mDrawingContextP->DrawOnscreenIcon(JoystickId);
//    }

    // Show Playback bitmap
#ifndef SHOW_ALL_OSD_ICONS
    if (mEmulatorP->PlayingSNRStream == true)
#endif /* SHOW_ALL_OSD_ICONS */
    {
      mDrawingContextP->DrawOnscreenIcon(PlaybackSessionId);
    }

//    // Show Decathlon bitmap
//#ifndef SHOW_ALL_OSD_ICONS
//    if (DecathlonModeMenu->Checked == true)
//#endif /* SHOW_ALL_OSD_ICONS */
//    {
//      mDrawingContextP->DrawOnscreenIcon(DecathlonId);
//    }
//    // Show Combat School bitmap
//#ifndef SHOW_ALL_OSD_ICONS
//    else if (CombatSchoolModeMenu->Checked == true)
//#endif /* SHOW_ALL_OSD_ICONS */
//    {
//      mDrawingContextP->DrawOnscreenIcon(CombatSchoolId);
//    }

    mDrawingContextP->DrawOnscreenIconsFinalize();
  }

  // Check all graphics operation are performed before swapping
  mDrawingContextP->DrawFinalize();

  // Error check
  int Error = mDrawingContextP->GetLastError();
  if (Error != 0)
  {
#ifdef _DEBUG
		String msg = _T("ExecuteVDUCondition Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("ERROR"), MB_OK);
#endif /* _DEBUG */
    // Pause emulation
    Pause(true);
  }

#ifdef SHOW_FPS
  mFPSCounter++;
#endif /* SHOW_FPS */

#ifdef _DEBUG
	// Signal events to all subscribers
	for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
	{
		TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
		if (((TEmulatorCPCEvent)*Event)(EC_DEBUG_VDU_END, mEmulatorP->NativeCPC) == true)
    {
      // Pause emulation
      if (mEmulatorP->Paused == false)
      {
        Pause(true);
      }
    }
  }
#endif /* _DEBUG */
}
//---------------------------------------------------------------------------

inline void __fastcall TFormMain::ExecutePSGCondition(void)
{
  // Unqueue processed buffers
  mAudioContextP->UnqueueSoundBuffers();
  // Fill sound streams
  mAudioContextP->FillEmulatorStream(mEmulatorP->GetAudioEmulatorData(),
                                     mEmulatorP->GetAudioEmulatorDataSize());
  if (mSettingsP->UseTechniMusique == true)
  {
    mAudioContextP->FillTMPIStream(mEmulatorP->GetAudioTMPIData(),
                                   mEmulatorP->GetAudioTMPIDataSize());
  }
  if (mSettingsP->UsePlayCity == true)
  {
    mAudioContextP->FillPlayCityStream(mEmulatorP->GetAudioPlayCityData(),
                                       mEmulatorP->GetAudioPlayCityDataSize());
  }

#ifdef USE_AUDIO_FILLED_BUFFER
  // Init Audio buffers
  tNativeCPC* NativeCPC = mEmulatorP->NativeCPC;
  NativeCPC->PSG->FilledBufferSize = 0;
  NativeCPC->PSG->snd_bufferptr = NativeCPC->PSG->pbSndBuffer;
#ifdef ENABLE_TMPI_SPEECH
  NativeCPC->TMPISpeech->FilledBufferSize = 0;
  NativeCPC->TMPISpeech->bufferptr = (tShort*)NativeCPC->TMPISpeech->pbBufferStart;
#endif /* ENABLE_TMPI_SPEECH */
#ifdef ENABLE_PLAYCITY
  NativeCPC->PlayCity->FilledBufferSize = 0;
  NativeCPC->PlayCity->snd_bufferptr = NativeCPC->PlayCity->pbSndBuffer;
#endif /* ENABLE_PLAYCITY */
#endif /* USE_AUDIO_FILLED_BUFFER */

  // Update FDC motor sound
  bool MotorState = mEmulatorP->NativeCPC->FDC->motor ? true : false;
  if (MotorState != mFDCMotorState)
  {
    mFDCMotorState = MotorState;
    if (MotorState)
    {
      mAudioContextP->PlayFloppyMotor();
    }
    else
    {
      mAudioContextP->StopFloppyMotor();
    }
  }

  // Update FDC head sound
  if (mEmulatorP->NativeCPC->FDC->headsteps)
  {
    mAudioContextP->PlayFloppyHead(mEmulatorP->NativeCPC->FDC->headsteps);
    mEmulatorP->NativeCPC->FDC->headsteps = 0;
  }

  // Update Cassette motor sound
  if ( mEmulatorP->NativeCPC->Tape->tape_motor
       && mEmulatorP->NativeCPC->Tape->tape_play_button )
  {
    MotorState = true;
  }
  else
  {
    MotorState = false;
  }
  if (MotorState != mTapeMotorState)
  {
    mTapeMotorState = MotorState;
    if (mTapeMotorState)
    {
      mAudioContextP->PlayTapeMotor();
    }
    else
    {
      mAudioContextP->StopTapeMotor();
    }
  }

  // Play Tape motor relay
  if (mEmulatorP->NativeCPC->Tape->relay_activations == 2)
  {
    mEmulatorP->NativeCPC->Tape->relay_activations = 0;
    mAudioContextP->PlayTapeDoubleRelay();
  }
  else if (mEmulatorP->NativeCPC->Tape->relay_activations)
  {
    mEmulatorP->NativeCPC->Tape->relay_activations--;
    mAudioContextP->PlayTapeRelay();
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::OnIdleNormalSpeed(TObject *Sender, bool &Done)
{
bool ExecuteOK = true;

	Done = false;

	if ( (mBaseTimerP->IsNextIdleTimerElapsed() == true)
       || (mEmulatorP->StepByStepActive == true) )
	{
		// Reinit realtime
		mBaseTimerP->StartCycleTimer();

#ifdef _DEBUG
    // Signal events to all subscribers
    for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
    {
      TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
      if (((TEmulatorCPCEvent)*Event)(EC_DEBUG_IDLE_ELAPSED, mEmulatorP->NativeCPC) == true)
      {
        // Pause emulation
        if (mEmulatorP->Paused == false)
        {
          Pause(true);
        }
      }
    }
#endif /* _DEBUG */
	}

	// ===============
	// Cycle end
	// ===============
  if (mExecuteCondition & EC_CYCLE_COUNT)
	{
		mCompteurCycleSecond = 0;

		ExecuteOK = mBaseTimerP->IsCycleTimerElapsed();
	}

	//
	// Execute emulator
	//
	if (ExecuteOK == true)
	{
    // Execute emulator !!
    mExecuteCondition = mEmulatorP->Execute();

    // Signal events to all subscribers
    for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
    {
      TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
      if (((TEmulatorCPCEvent)*Event)(mExecuteCondition, mEmulatorP->NativeCPC) == true)
      {
        // Pause emulation
        if (mEmulatorP->Paused == false)
        {
          Pause(true);
        }
      }
    }

    // Signal FDC events to all subscribers
    if (mEmulatorP->FDCModified == true)
    {
      for (int Index=0; Index < mOnEmulatorFDCEventList->Count; Index++)
      {
        TEmulatorFDCEvent* Event = (TEmulatorFDCEvent*)mOnEmulatorFDCEventList->Items[Index];
        ((TEmulatorFDCEvent)*Event)(mEmulatorP->NativeCPC->FDC);
      }

      mEmulatorP->FDCModified = false;
    }

		// =======================
		// VDU Frame completed
		// =======================
		if (mExecuteCondition & (EC_FRAME_COMPLETE | EC_END_INSTRUCTION) )
		{
#ifdef SHOW_FPS
      if (mBaseTimerP->IsSecondTimerElapsed() == true)
      {
        mFPSDisplayed = mFPSCounter;
        mFPSCounter = 0;
      }
#endif /* SHOW_FPS */

			ExecuteVDUCondition();
		}

		// =======================
		// Sound buffer full
		// =======================
#ifndef USE_AUDIO_FILLED_BUFFER
		if (mExecuteCondition & EC_SOUND_BUFFER)
#endif /* USE_AUDIO_FILLED_BUFFER */
		{
      if (mSettingsP->Mute == false)
      {
			  ExecutePSGCondition();
      }
      else
      {
        mEmulatorP->NativeCPC->Tape->relay_activations = 0;
        mEmulatorP->NativeCPC->FDC->headsteps = 0;
      }
		}

		// ====================
		// Cyclic operations
		// ====================
    if (mExecuteCondition & EC_CYCLE_COUNT)
		{
			// Update Game Controller inputs
			if (mInputContextP->NbGameControllers > 0)
			{
				mInputContextP->UpdateGameControllers();
			}

			if (mDrawingContextP->Fullscreen == false)
			{
				// Signal drive events to all subscribers
				if (mEmulatorP->DriveAModified == true)
				{
					for (int Index=0; Index < mOnEmulatorDiskEventList->Count; Index++)
					{
						TEmulatorDiskEvent* Event = (TEmulatorDiskEvent*)mOnEmulatorDiskEventList->Items[Index];
						((TEmulatorDiskEvent)*Event)(mEmulatorP->NativeCPC->FDC->DriveA);
					}
					mEmulatorP->DriveAModified = false;
				}
				if (mEmulatorP->DriveBModified == true)
				{
					for (int Index=0; Index < mOnEmulatorDiskEventList->Count; Index++)
					{
						TEmulatorDiskEvent* Event = (TEmulatorDiskEvent*)mOnEmulatorDiskEventList->Items[Index];
						((TEmulatorDiskEvent)*Event)(mEmulatorP->NativeCPC->FDC->DriveB);
					}
					mEmulatorP->DriveBModified = false;
				}
			}

			// Add Emulator messages
			while (mEmulatorP->NbSimpleMessages)
			{
				AddEmulatorMessage(mEmulatorP->SimpleMessage);
			}
		}
	}

	// Initialise next cycle reset time
	mBaseTimerP->UpdateNextIdleTimer();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnIdleFullSpeed(TObject *Sender, bool &Done)
{
	Done = false;

	if (mBaseTimerP->IsNextIdleTimerElapsed() == true)
	{
    // Reinit realtime
    mBaseTimerP->StartCycleTimer();
  }

  // Performance display update
  if (mExecuteCondition & EC_CYCLE_COUNT)
  {
    mCompteurCycleSecond++;

    if (mBaseTimerP->IsSecondTimerElapsed() == true)
    {
      // Calculer indice de performance
      mEmulatorPerformance = mCompteurCycleSecond * 100 / CYCLES_PER_SECOND;
      mCompteurCycleSecond = 0;

      // Afficher indice de performance dans le titre
      String NewTitle = mApplicationTitle + _T(" [") + AnsiString(mEmulatorPerformance) + _T("%]");
      Caption = NewTitle;

#ifdef SHOW_FPS
      mFPSDisplayed = mFPSCounter;
      mFPSCounter = 0;
#endif /* SHOW_FPS */
    }
  }

  //
  // Execute emulator !!
  //
  mExecuteCondition = mEmulatorP->Execute();

  // Signal events to all subscribers
  for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
  {
    TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
    if (((TEmulatorCPCEvent)*Event)(mExecuteCondition, mEmulatorP->NativeCPC) == true)
    {
      // Pause emulation
      if (mEmulatorP->Paused == false)
      {
        Pause(true);
      }
    }
  }

  // Signal FDC events to all subscribers
  if (mEmulatorP->FDCModified == true)
  {
    for (int Index=0; Index < mOnEmulatorFDCEventList->Count; Index++)
    {
      TEmulatorFDCEvent* Event = (TEmulatorFDCEvent*)mOnEmulatorFDCEventList->Items[Index];
      ((TEmulatorFDCEvent)*Event)(mEmulatorP->NativeCPC->FDC);
    }

    mEmulatorP->FDCModified = false;
  }

  if (mBaseTimerP->IsCycleTimerElapsed() == true)
  {
    // =======================
    // VDU Frame completed
    // =======================
    ExecuteVDUCondition();

    // =======================
    // Sound buffer full
    // =======================
    if (mSettingsP->Mute == false)
    {
      ExecutePSGCondition();
    }
    else
    {
      mEmulatorP->NativeCPC->Tape->relay_activations = 0;
      mEmulatorP->NativeCPC->FDC->headsteps = 0;
    }

    // ===============
    // Cycle end
    // ===============
    // Update Game Controller inputs
    if (mInputContextP->NbGameControllers > 0)
    {
      mInputContextP->UpdateGameControllers();
    }

    if (mDrawingContextP->Fullscreen == false)
    {
      // Signal drive events to all subscribers
      if (mEmulatorP->DriveAModified == true)
      {
        for (int Index=0; Index < mOnEmulatorDiskEventList->Count; Index++)
        {
          TEmulatorDiskEvent* Event = (TEmulatorDiskEvent*)mOnEmulatorDiskEventList->Items[Index];
          ((TEmulatorDiskEvent)*Event)(mEmulatorP->NativeCPC->FDC->DriveA);
        }
        mEmulatorP->DriveAModified = false;
      }
      if (mEmulatorP->DriveBModified == true)
      {
        for (int Index=0; Index < mOnEmulatorDiskEventList->Count; Index++)
        {
          TEmulatorDiskEvent* Event = (TEmulatorDiskEvent*)mOnEmulatorDiskEventList->Items[Index];
          ((TEmulatorDiskEvent)*Event)(mEmulatorP->NativeCPC->FDC->DriveB);
        }
        mEmulatorP->DriveBModified = false;
      }
    }

    // Add Emulator messages
    while (mEmulatorP->NbSimpleMessages)
    {
      AddEmulatorMessage(mEmulatorP->SimpleMessage);
    }
  }

  // Initialise next cycle reset time
  mBaseTimerP->UpdateNextIdleTimer();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnIdlePaused(TObject *Sender, bool &Done)
{
	Done = false;

  if (mEmulatorP->Paused == true)
  {
    if (mBaseTimerP->IsCycleTimerElapsed() == true)
    {
#ifdef SHOW_FPS
      if (mBaseTimerP->IsSecondTimerElapsed() == true)
      {
        mFPSDisplayed = mFPSCounter;
        mFPSCounter = 0;
      }
#endif /* SHOW_FPS */

      ExecuteVDUCondition();
    }
  }
  else if (mEmulatorP->StepByStepActive == true)
  {
  	// Execute emulator !!
		mExecuteCondition = mEmulatorP->Execute();

#ifdef SHOW_FPS
    if (mBaseTimerP->IsSecondTimerElapsed() == true)
    {
      mFPSDisplayed = mFPSCounter;
      mFPSCounter = 0;
    }
#endif /* SHOW_FPS */

    ExecuteVDUCondition();

    // Signal events to all subscribers
    for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
    {
      TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
      if (((TEmulatorCPCEvent)*Event)(mExecuteCondition, mEmulatorP->NativeCPC) == true)
      {
        // Pause emulation
        if (mEmulatorP->Paused == false)
        {
          Pause(true);
        }
      }
    }

    // Signal FDC events to all subscribers
    if (mEmulatorP->FDCModified == true)
    {
      for (int Index=0; Index < mOnEmulatorFDCEventList->Count; Index++)
      {
        TEmulatorFDCEvent* Event = (TEmulatorFDCEvent*)mOnEmulatorFDCEventList->Items[Index];
        ((TEmulatorFDCEvent)*Event)(mEmulatorP->NativeCPC->FDC);
      }

      mEmulatorP->FDCModified = false;
    }

    // Signal drive events to all subscribers
    if ( (mEmulatorP->DriveAModified == true)
         || (mEmulatorP->DriveBModified == true) )
    {
      for (int Index=0; Index < mOnEmulatorDiskEventList->Count; Index++)
      {
        TEmulatorDiskEvent* Event = (TEmulatorDiskEvent*)mOnEmulatorDiskEventList->Items[Index];

        if (mEmulatorP->DriveAModified == true)
        {
          ((TEmulatorDiskEvent)*Event)(mEmulatorP->NativeCPC->FDC->DriveA);
        }

        if (mEmulatorP->DriveBModified == true)
        {
          ((TEmulatorDiskEvent)*Event)(mEmulatorP->NativeCPC->FDC->DriveB);
        }
      }
      mEmulatorP->DriveAModified = false;
      mEmulatorP->DriveBModified = false;
    }

    if (mBaseTimerP->IsCycleTimerElapsed() == true)
    {
      // Add Emulator messages
      while (mEmulatorP->NbSimpleMessages)
      {
        AddEmulatorMessage(mEmulatorP->SimpleMessage);
      }
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SetAudioSettings(void)
{
  if (mSettingsP->Mute == true)
  {
    mFDCMotorState = false;
    mTapeMotorState = false;
    MuteMenu->Checked = true;
    VolumeUpMenu->Enabled = false;
    VolumeDownMenu->Enabled = false;
    SpeedButtonControlMute->Down = true;
    mAudioContextP->Stop();
    mEmulatorP->AudioStop();
  }
  else
  {
    MuteMenu->Checked = false;
    VolumeUpMenu->Enabled = true;
    VolumeDownMenu->Enabled = true;
    SpeedButtonControlMute->Down = false;
    mAudioContextP->Start();
    mEmulatorP->AudioStart();
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SwapDrives(void)
{
  if (SwapDrivesMenu->Enabled == false)
    return;

  mEmulatorP->SwapDrives();

  if (mInfoFormP)
  {
    if (EmulatorP->DiskAFilename.IsEmpty() == false)
    {
      String ReadOnly = (mEmulatorP->DriveAWriteProtected == true) ? _T("[READ ONLY] ") : _T("");
      mInfoFormP->DriveAFilename = ReadOnly + "DriveA=" + ExtractFileName(EmulatorP->DiskAFilename);
    }
    else
    {
      mInfoFormP->DriveAFilename = "";
    }
    if (EmulatorP->DiskBFilename.IsEmpty() == false)
    {
      String ReadOnly = (mEmulatorP->DriveBWriteProtected == true) ? _T("[READ ONLY] ") : _T("");
      mInfoFormP->DriveBFilename = ReadOnly + "DriveB=" + ExtractFileName(EmulatorP->DiskBFilename);
    }
    else
    {
      mInfoFormP->DriveBFilename = "";
    }
  }

  if ( (EmulatorP->DiskAFilename.IsEmpty() == false)
       || (EmulatorP->DiskBFilename.IsEmpty() == false) )
  {
    AddEmulatorMessage("Drives swapped");
  }
  if (EmulatorP->DiskAFilename.IsEmpty() == false)
  {
    AddEmulatorMessage("DriveA=" + ExtractFileName(mEmulatorP->DiskAFilename));
  }
  if (EmulatorP->DiskBFilename.IsEmpty() == false)
  {
    AddEmulatorMessage("DriveB=" + ExtractFileName(mEmulatorP->DiskBFilename));
  }

  // Update drives info
  UpdateDriveAMenu();
  UpdateDriveBMenu();

  // Swap Panel Drive Hints
  AnsiString Temp = PanelControlDriveBPosition->Hint;
  PanelControlDriveBPosition->Hint = PanelControlDriveAPosition->Hint;
  PanelControlDriveAPosition->Hint = Temp;
  PanelControlDriveAPosition->Color = DriveAWriteProtectionMenu->Checked ? clRed : cl3DLight;
  PanelControlDriveBPosition->Color = DriveBWriteProtectionMenu->Checked ? clRed : cl3DLight;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FlipSidesDriveA(void)
{
  if (DriveAFlipSidesMenu->Enabled == false)
    return;

  mEmulatorP->DriveAFlipped = (mEmulatorP->DriveAFlipped == false) ? true : false;
  UpdateDriveAMenu();

  AddEmulatorMessage(DriveAString + " sides have been flipped.");
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::EjectDriveA(void)
{
  if (mEmulatorP->DiskAFilename.IsEmpty() == false)
  {
    bool SaveAndEject = true;

    if (mSettingsP->NeverUpdateDiskImages == true)
    {
      SaveAndEject = false;
    }
    else if (mEmulatorP->DriveAAltered == false)
    {
      SaveAndEject = false;
    }
    // Virtual disk
    else if (ExtractFilePath(mEmulatorP->DiskAFilename).Length() == 0)
    {
      // Request real filename
	    if (mDrawingContextP->Fullscreen == false)
	    {
		    SaveDialog->Title = "Drive A - " + mEmulatorP->DiskAFilename;
        SaveDialog->FileName = mEmulatorP->DiskAFilename;
        SaveDialog->Filter = DISKIMAGE_FILTER_SAVE;
        SaveDialog->DefaultExt = DISKIMAGE_DSK_EXT;
        if (SaveDialog->Execute() == true)
        {
          mEmulatorP->ForceDiskAFilename(SaveDialog->FileName);
        }
        else
        {
          SaveAndEject = false;
        }
      }
    }

    if (SaveAndEject == true)
    {
      mEmulatorP->SaveAndEjectDisk(DriveA);
    }
    else
    {
      mEmulatorP->EjectDisk(DriveA);
    }

    AddEmulatorMessage("Drive A ejected");

    if (mInfoFormP)
    {
      mInfoFormP->DriveAFilename = "";
    }

    PanelControlDriveAPosition->Hint = DefaultPanelControlDriveHint;
    PanelControlDriveAPosition->Color = cl3DLight;

    // Update Menu
    if (mEmulatorP->DiskBFilename.IsEmpty() == true)
    {
      SwapDrivesMenu->Enabled = false;
      EjectbothMenu->Enabled = false;
    }
    UpdateDriveAMenu();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::EjectDriveB(void)
{
  if (mEmulatorP->DiskBFilename.IsEmpty() == false)
  {
    bool SaveAndEject = true;

    if (mSettingsP->NeverUpdateDiskImages == true)
    {
      SaveAndEject = false;
    }
    else if (mEmulatorP->DriveBAltered == false)
    {
      SaveAndEject = false;
    }
    // Virtual disk
    else if (ExtractFilePath(mEmulatorP->DiskBFilename).Length() == 0)
    {
      // Request real filename
	    if (mDrawingContextP->Fullscreen == false)
	    {
		    SaveDialog->Title = "Drive B - " + mEmulatorP->DiskBFilename;
        SaveDialog->FileName = mEmulatorP->DiskBFilename;
        SaveDialog->Filter = DISKIMAGE_FILTER_SAVE;
        SaveDialog->DefaultExt = DISKIMAGE_DSK_EXT;
        if (SaveDialog->Execute() == true)
        {
          mEmulatorP->ForceDiskBFilename(SaveDialog->FileName);
        }
        else
        {
          SaveAndEject = false;
        }
      }
    }

    if (SaveAndEject == true)
    {
      mEmulatorP->SaveAndEjectDisk(DriveB);
    }
    else
    {
      mEmulatorP->EjectDisk(DriveB);
    }

    AddEmulatorMessage("Drive B ejected");

    if (mInfoFormP)
    {
      mInfoFormP->DriveBFilename = "";
    }

    PanelControlDriveBPosition->Hint = DefaultPanelControlDriveHint;
    PanelControlDriveBPosition->Color = cl3DLight;

    // Update Menu
    if (mEmulatorP->DiskAFilename.IsEmpty() == true)
    {
      SwapDrivesMenu->Enabled = false;
      EjectbothMenu->Enabled = false;
    }
    UpdateDriveBMenu();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::LoadDriveA(AnsiString Filename, bool CheckAutoStart)
{
  AnsiString ContainerFilename = Filename;
  if (Filename.LowerCase().Pos(ZIP_EXT) != 0)
  {
    TZIPContainer* ZIPContainer = new TZIPContainer(Filename);
    int iNbFiles = 0;

    // Archive contains files
    if (ZIPContainer->NbFiles)
    {
      for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
      {
        AnsiString InsideFileName = ZIPContainer->Files[Loop];
        AnsiString LowerInsideFilename = InsideFileName.LowerCase();

        // Add any supported files
        if ( (LowerInsideFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
             || (LowerInsideFilename.Pos(DISKIMAGE_IPF_EXT) != 0)
#ifdef _DEBUG
             || (LowerInsideFilename.Pos(DISKIMAGE_RAW_EXT) != 0)
#endif /* _DEBUG */
           )
        {
          Filename += "\\" + InsideFileName;
          iNbFiles++;
        }
      }
    }

    // Too many files ?
    if (iNbFiles > 1)
    {
      FormZIPCatalog->Clear();
      for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
      {
        AnsiString InsideFileName = ZIPContainer->Files[Loop];
        AnsiString LowerInsideFilename = InsideFileName.LowerCase();

        // Add any supported files
        if ( (LowerInsideFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
             || (LowerInsideFilename.Pos(DISKIMAGE_IPF_EXT) != 0) )
        {
          FormZIPCatalog->AddFile(InsideFileName);
        }
      }
      if (FormZIPCatalog->ShowModal() == mrOk)
      {
        Filename = ContainerFilename + "\\" + FormZIPCatalog->SelectedFile;
      }
      else
      {
        Filename = "";
      }
    }

    delete ZIPContainer;
  }

  mEmulatorP->LoadDisk(Filename, DriveA);

  if (mEmulatorP->DiskAFilename.IsEmpty() == false)
  {
    Filename = ExtractFileName(mEmulatorP->DiskAFilename);
    AddEmulatorMessage("DriveA=" + Filename);
    AnsiString DiscDirectory = ExtractFilePath(mEmulatorP->DiskAFilename);
    if (DiscDirectory.IsEmpty() == false)
    {
      mSettingsP->DiscDirectory = DiscDirectory;
      AddRecentDisk(mEmulatorP->DiskAFilename);
    }
    else
    {
      DiscDirectory = ExtractFilePath(ContainerFilename);
      if (DiscDirectory.IsEmpty() == false)
      {
        mSettingsP->DiscDirectory = DiscDirectory;
        AddRecentDisk(ContainerFilename);
      }
    }

    // Update Menu
    SwapDrivesMenu->Enabled = true;
    EjectbothMenu->Enabled = true;
    UpdateDriveAMenu();

    if (mEmulatorP->DriveAWriteProtected == true)
    {
      AddEmulatorMessage("Drive A is write protected !!");
    }

    if (mInfoFormP)
    {
			String ReadOnly = (mEmulatorP->DriveAWriteProtected == true) ? _T("[READ ONLY] ") : _T("");
      mInfoFormP->DriveAFilename = ReadOnly + "DriveA=" + Filename;
    }

    // Load controller profile
    if (mInputContextP->NbGameControllers > 0)
    {
      // Load driveA or default profile
      LoadDriveAProfileClick(NULL);
    }

    PanelControlDriveAPosition->Hint = Filename;
    PanelControlDriveAPosition->Color = DriveAWriteProtectionMenu->Checked ? clRed : cl3DLight;

    // DriveA autostart ?
    if (CheckAutoStart == true)
    {
      if ( (mSettingsP->AutoStartEnable) && (mEmulatorP->AutoTypeInProgress == false) )
      {
        AnsiString Command = mEmulatorP->DetectAutoStart(DriveA);
        if (Command.Length() > 0)
        {
#ifdef AUTORUN_REMOVE_FILE_EXTENSION
          // Remove file extension
          int ExtensionPos = Command.Pos(".");
          if (ExtensionPos > 0)
          {
            Command = Command.SubString(1, ExtensionPos - 1);
            Command += "\r";
          }
#endif /* AUTORUN_REMOVE_FILE_EXTENSION */

          mEmulatorP->StartAutoType(Command);
        }
        else
        {
          AddEmulatorMessage("Unable to find a program to start!");
        }
      }
    }
  }
  else
  {
    PanelControlDriveAPosition->Hint = DefaultPanelControlDriveHint;
    PanelControlDriveAPosition->Color = cl3DLight;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::LoadDriveB(AnsiString Filename)
{
  AnsiString ContainerFilename = Filename;
  if (Filename.LowerCase().Pos(ZIP_EXT) != 0)
  {
    TZIPContainer* ZIPContainer = new TZIPContainer(Filename);
    int iNbFiles = 0;

    // Archive contains files
    if (ZIPContainer->NbFiles)
    {
      for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
      {
        AnsiString InsideFileName = ZIPContainer->Files[Loop];
        AnsiString LowerInsideFilename = InsideFileName.LowerCase();

        // Add any supported files
        if ( (LowerInsideFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
             || (LowerInsideFilename.Pos(DISKIMAGE_IPF_EXT) != 0) )
        {
          Filename += "\\" + InsideFileName;
          iNbFiles++;
        }
      }
    }

    // Too many files ?
    if (iNbFiles > 1)
    {
      FormZIPCatalog->Clear();
      for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
      {
        AnsiString InsideFileName = ZIPContainer->Files[Loop];
        AnsiString LowerInsideFilename = InsideFileName.LowerCase();

        // Add any supported files
        if ( (LowerInsideFilename.Pos(DISKIMAGE_DSK_EXT) != 0)
             || (LowerInsideFilename.Pos(DISKIMAGE_IPF_EXT) != 0) )
        {
          FormZIPCatalog->AddFile(InsideFileName);
        }
      }
      if (FormZIPCatalog->ShowModal() == mrOk)
      {
        Filename = ContainerFilename + "\\" + FormZIPCatalog->SelectedFile;
      }
      else
      {
        Filename = "";
      }
    }

    delete ZIPContainer;
  }

  mEmulatorP->LoadDisk(Filename, DriveB);

  if (mEmulatorP->DiskBFilename.IsEmpty() == false)
  {
    Filename = ExtractFileName(mEmulatorP->DiskBFilename);
    AddEmulatorMessage("DriveB=" + Filename);
    AnsiString DiscDirectory = ExtractFilePath(mEmulatorP->DiskBFilename);
    if (DiscDirectory.IsEmpty() == false)
    {
      mSettingsP->DiscDirectory = DiscDirectory;
      AddRecentDisk(mEmulatorP->DiskBFilename);
    }
    else
    {
      DiscDirectory = ExtractFilePath(ContainerFilename);
      if (DiscDirectory.IsEmpty() == false)
      {
        mSettingsP->DiscDirectory = DiscDirectory;
        AddRecentDisk(ContainerFilename);
      }
    }

    // Update Menu
    SwapDrivesMenu->Enabled = true;
    EjectbothMenu->Enabled = true;
    UpdateDriveBMenu();

    if (mEmulatorP->DriveBWriteProtected == true)
    {
      AddEmulatorMessage("Drive B is write protected !!");
    }

    if (mInfoFormP)
    {
      String ReadOnly = (mEmulatorP->DriveBWriteProtected == true) ? _T("[READ ONLY] ") : _T("");
      mInfoFormP->DriveBFilename = ReadOnly + "DriveB=" + Filename;
    }

    PanelControlDriveBPosition->Hint = Filename;
    PanelControlDriveBPosition->Color = DriveBWriteProtectionMenu->Checked ? clRed : cl3DLight;
  }
  else
  {
    PanelControlDriveBPosition->Hint = DefaultPanelControlDriveHint;
    PanelControlDriveBPosition->Color = cl3DLight;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::CreateDriveA(AnsiString Filename)
{
  EjectDriveA();

  mEmulatorP->CreateVirtualDisk(Filename,
                                DriveA,
												        DEFAULT_DISK_FORMAT);

  mSettingsP->DiscDirectory = ExtractFilePath(Filename);
  PanelControlDriveAPosition->Color = cl3DLight;

  if (mEmulatorP->DiskAFilename.IsEmpty() == false)
  {
    AnsiString Filename = ExtractFileName(mEmulatorP->DiskAFilename);
    AddEmulatorMessage("DriveA=" + Filename);
    if (mInfoFormP)
    {
      mInfoFormP->DriveAFilename = "DriveA=" + Filename;
    }

    PanelControlDriveAPosition->Hint = Filename;

    // Update Menu
    SwapDrivesMenu->Enabled = true;
    EjectbothMenu->Enabled = true;
    UpdateDriveAMenu();
  }
  else
  {
    PanelControlDriveAPosition->Hint = DefaultPanelControlDriveHint;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::CreateDriveB(AnsiString Filename)
{
  EjectDriveB();

  mEmulatorP->CreateVirtualDisk(Filename,
                                DriveB,
												        DEFAULT_DISK_FORMAT);

	mSettingsP->DiscDirectory = ExtractFilePath(Filename);
	PanelControlDriveBPosition->Color = cl3DLight;

  if (mEmulatorP->DiskBFilename.IsEmpty() == false)
  {
    AnsiString Filename = ExtractFileName(mEmulatorP->DiskBFilename);
    AddEmulatorMessage("DriveB=" + Filename);
    if (mInfoFormP)
    {
      mInfoFormP->DriveBFilename = "DriveB=" + Filename;
    }

    PanelControlDriveBPosition->Hint = Filename;

    // Update Menu
    SwapDrivesMenu->Enabled = true;
    EjectbothMenu->Enabled = true;
    UpdateDriveBMenu();
  }
  else
  {
    PanelControlDriveBPosition->Hint = DefaultPanelControlDriveHint;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::InsertTape(AnsiString Filename, bool CheckAutoPlay)
{
  AnsiString ContainerFilename = Filename;
  if (Filename.LowerCase().Pos(ZIP_EXT) != 0)
  {
    TZIPContainer* ZIPContainer = new TZIPContainer(Filename);
    int iNbFiles = 0;

    // Archive contains files
    if (ZIPContainer->NbFiles)
    {
      for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
      {
        AnsiString InsideFileName = ZIPContainer->Files[Loop];
        AnsiString LowerInsideFilename = InsideFileName.LowerCase();

        // Add any supported files
        if ( (LowerInsideFilename.Pos(TAPEIMAGE_EXT) != 0)
             || (LowerInsideFilename.Pos(WAVTAPE_EXT) != 0)
             || (LowerInsideFilename.Pos(CSWTAPE_EXT) != 0) )
        {
          Filename += "\\" + InsideFileName;
          iNbFiles++;
        }
      }
    }

    // Too many files ?
    if (iNbFiles > 1)
    {
      FormZIPCatalog->Clear();
      for (int Loop=0; Loop < ZIPContainer->NbFiles; Loop++)
      {
        AnsiString InsideFileName = ZIPContainer->Files[Loop];
        AnsiString LowerInsideFilename = InsideFileName.LowerCase();

        // Add any supported files
        if ( (LowerInsideFilename.Pos(TAPEIMAGE_EXT) != 0)
             || (LowerInsideFilename.Pos(WAVTAPE_EXT) != 0)
             || (LowerInsideFilename.Pos(CSWTAPE_EXT) != 0) )
        {
          FormZIPCatalog->AddFile(InsideFileName);
        }
      }
      if (FormZIPCatalog->ShowModal() == mrOk)
      {
        Filename = ContainerFilename + "\\" + FormZIPCatalog->SelectedFile;
      }
      else
      {
        Filename = "";
      }
    }

    delete ZIPContainer;
  }

  mEmulatorP->StopTape();
  mEmulatorP->EjectTape();

  PlayTapeMenu->Checked = false;
  InsertTapeMenu->Enabled = true;
  SpeedButtonControlTapePlayStop->Down = false;

  bool TapeInserted = false;

  AnsiString LowerFilename = Filename.LowerCase();
  if (LowerFilename.Pos(TAPEIMAGE_EXT) != 0)
  {
    if (mEmulatorP->InsertTape(Filename) == errNone)
    {
      TapeInserted = true;
    }
  }
  // Tape WAV audio
  else if (LowerFilename.Pos(WAVTAPE_EXT) != 0)
  {
    // Convert Audio WAV to Tape blocks
    TCursor OldCursor = Screen->Cursor;
    Screen->Cursor = crHourGlass;
    tULong Length;
    tUChar* BlockDataP = mAudioContextP->ConvertWAVToCSW(Filename,
                                                         &Length);
    if (BlockDataP)
    {
      tULong DataLength;
      tULong SampleRate;
      tUChar StartPolarity;
      tUChar* DataP = mAudioContextP->InflateDataFromCSW(BlockDataP,
                                                         Length,
                                                         &DataLength,
                                                         &SampleRate,
                                                         &StartPolarity);
      if (DataP)
      {
        if (mEmulatorP->InsertTapeCSW(Filename,
                                      DataP,
                                      DataLength,
                                      SampleRate,
                                      StartPolarity) == errNone)
        {
          TapeInserted = true;
        }
        else
        {
          free(DataP);
        }
      }

      free(BlockDataP);
    }
    Screen->Cursor = OldCursor;
  }
  // Tape CSW
  else if (LowerFilename.Pos(CSWTAPE_EXT) != 0)
  {
    tULong Length;
    tULong SampleRate;
    tUChar StartPolarity;
    tUChar* DataP = mAudioContextP->InflateDataFromCSW(Filename,
                                                       &Length,
                                                       &SampleRate,
                                                       &StartPolarity);
    if (DataP)
    {
      if (mEmulatorP->InsertTapeCSW(Filename,
                                    DataP,
                                    Length,
                                    SampleRate,
                                    StartPolarity) == errNone)
      {
        TapeInserted = true;
      }
      else
      {
        free(DataP);
      }
    }
  }

  if (TapeInserted == true)
  {
    mEmulatorP->RewindTape();
    AddEmulatorMessage("Tape=" + ExtractFileName(mEmulatorP->TapeFilename));
    AnsiString FilePath = ExtractFilePath(mEmulatorP->TapeFilename);
    if (FilePath.IsEmpty() == false)
    {
      mSettingsP->TapeDirectory = FilePath;
    }

    if (mInfoFormP)
    {
      mInfoFormP->TapeFilename = "Tape=" + ExtractFileName(EmulatorP->TapeFilename);
    }
  }

  bool NavigationAllowed = mEmulatorP->IsTapeNavigationAllowed();

  // Update UI
  PlayTapeMenu->Enabled = TapeInserted;
  AutoPlayTapeMenu->Enabled = TapeInserted;
  RewindTapeMenu->Enabled = TapeInserted;
  RewindToCounter000Menu->Enabled = NavigationAllowed;
  ResetCounterTo000Menu->Enabled = NavigationAllowed;
  GoToPreviousTagMenu->Enabled = NavigationAllowed;
  GoToNextTagMenu->Enabled = NavigationAllowed;
  EjectTapeMenu->Enabled = TapeInserted;
  SpeedButtonControlTapeRewind->Enabled = TapeInserted;
  SpeedButtonControlTapePrevious->Enabled = NavigationAllowed;
  SpeedButtonControlTapePlayStop->Enabled = TapeInserted;
  SpeedButtonControlTapeNext->Enabled = NavigationAllowed;

  if (TapeInserted == true)
  {
    // Signal CDT event to all subscribers
    for (int Index=0; Index < mOnEmulatorTapeEventList->Count; Index++)
    {
      TEmulatorTapeEvent* Event = (TEmulatorTapeEvent*)mOnEmulatorTapeEventList->Items[Index];
      ((TEmulatorTapeEvent)*Event)(mEmulatorP->NativeCPC->Tape);
    }

    // Auto Play
    if (CheckAutoPlay == true)
    {
      if (mSettingsP->AutoPlayEnable)
      {
        AutoPlayTapeMenuClick(NULL);
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PlayTape(void)
{
  mEmulatorP->PlayTape();
  AddEmulatorMessage("Tape playing");

  PlayTapeMenu->Checked = true;
  InsertTapeMenu->Enabled = false;
  EjectTapeMenu->Enabled = true;
  SpeedButtonControlTapePlayStop->Down = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::StopTape(void)
{
  mEmulatorP->StopTape();
  AddEmulatorMessage("Tape stopped");

  PlayTapeMenu->Checked = false;
  InsertTapeMenu->Enabled = true;
  EjectTapeMenu->Enabled = false;
  SpeedButtonControlTapePlayStop->Down = false;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SubscribeToCPCEvents(TEmulatorCPCEvent Callback)
{
  // Add event
  TEmulatorCPCEvent* Event = new TEmulatorCPCEvent;
  *Event = Callback;
  mOnEmulatorCPCEventsList->Add(Event);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::UnsubscribeToCPCEvents(TEmulatorCPCEvent Callback)
{
  // Delete event
  for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
  {
    TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
    if (*Event == Callback)
    {
      delete Event;
      mOnEmulatorCPCEventsList->Delete(Index);
      break;
    }
  }

  // Step by step no longer needed
  if ( (mOnEmulatorCPCEventsList->Count == 0)
       || (FormDebugger->Visible == false) )
  {
    if ( (mEmulatorP->StepByStepActive == true) && (mEmulatorP->Paused == true) )
    {
      StopStepByStep(true); // Stay in pause
    }
    else if (mEmulatorP->StepByStepActive == true)
    {
      StopStepByStep(false);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MaximizeEmulatorEndEventPriority(TEmulatorCPCEvent Callback)
{
  if (Callback == NULL)
     return;

  UnsubscribeToCPCEvents(Callback);

  // Insert event at first position
  TEmulatorCPCEvent* Event = new TEmulatorCPCEvent;
  *Event = Callback;
  mOnEmulatorCPCEventsList->Insert(0, Event);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SubscribeToFDCEvents(TEmulatorFDCEvent Callback)
{
  // Add event
  TEmulatorFDCEvent* Event = new TEmulatorFDCEvent;
  *Event = Callback;
  mOnEmulatorFDCEventList->Add(Event);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::UnsubscribeToFDCEvents(TEmulatorFDCEvent Callback)
{
  // Delete event
  for (int Index=0; Index < mOnEmulatorFDCEventList->Count; Index++)
  {
    TEmulatorFDCEvent* Event = (TEmulatorFDCEvent*)mOnEmulatorFDCEventList->Items[Index];
    if (*Event == Callback)
    {
      delete Event;
      mOnEmulatorFDCEventList->Delete(Index);
      break;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SubscribeToDiskEvents(TEmulatorDiskEvent Callback)
{
  // Add event
  TEmulatorDiskEvent* Event = new TEmulatorDiskEvent;
  *Event = Callback;
  mOnEmulatorDiskEventList->Add(Event);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::UnsubscribeToDiskEvents(TEmulatorDiskEvent Callback)
{
  // Delete event
  for (int Index=0; Index < mOnEmulatorDiskEventList->Count; Index++)
  {
    TEmulatorDiskEvent* Event = (TEmulatorDiskEvent*)mOnEmulatorDiskEventList->Items[Index];
    if (*Event == Callback)
    {
      delete Event;
      mOnEmulatorDiskEventList->Delete(Index);
      break;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SubscribeToTapeEvents(TEmulatorTapeEvent Callback)
{
  // Add event
  TEmulatorTapeEvent* Event = new TEmulatorTapeEvent;
  *Event = Callback;
  mOnEmulatorTapeEventList->Add(Event);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::UnsubscribeToTapeEvents(TEmulatorTapeEvent Callback)
{
  // Delete event
  for (int Index=0; Index < mOnEmulatorTapeEventList->Count; Index++)
  {
    TEmulatorTapeEvent* Event = (TEmulatorTapeEvent*)mOnEmulatorTapeEventList->Items[Index];
    if (*Event == Callback)
    {
      delete Event;
      mOnEmulatorTapeEventList->Delete(Index);
      break;
    }
  }
}
//---------------------------------------------------------------------------


//
// Inputs
//
void __fastcall TFormMain::OnRemoveGameController(AnsiString Name)
{
  AddEmulatorMessage("Game controller \"" + Name + "\" has been disconnected !!");

  // Update menu
  if (mInputContextP->NbGameControllers == 0)
  {
    // Enable game controler settings access
    GameControllerSeparator->Visible = false;
    GameControllersSettingsMenu->Visible = false;
    LoadDefaultProfileMenu->Visible = false;
    LoadDriveAProfile->Visible = false;

    // Stop game controller acquisition
    mInputContextP->Stop();
  }
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::OnWINInputKeyDown(WORD &Key, TShiftState Shift)
{
bool Treated = false;

  if (mFullscreenDialogForm)
  {
    mFullscreenDialogForm->OnKeyDown(Key);
  }
  else if (mEmulatorP != NULL)
  {
    if (Key < 0x0100)
    {
      Treated = mEmulatorP->OnWINKeyPress(Key, Shift);

      if (Treated == true)
      {
        // Stop Playback play
        if (mEmulatorP->PlayingSNRStream == true)
        {
          mEmulatorP->StopSNRSession();
        }
      }
    }

    if (Treated == false)
    {
      if (Shift.Contains(ssAlt))
      {
        Treated = OnEmulatorKeyDownWithAlt(Key);
      }
      else if (Shift.Contains(ssShift) && Shift.Contains(ssCtrl))
      {
        Treated = OnEmulatorKeyDownWithCtrlAndShift(Key);
      }
      else if (Shift.Contains(ssShift))
      {
        Treated = OnEmulatorKeyDownWithShift(Key);
      }
      else if (Shift.Contains(ssCtrl))
      {
        Treated = OnEmulatorKeyDownWithCtrl(Key);
      }
      else
      {
        Treated = OnEmulatorKeyDown(Key);
      }
    }
  }

  return Treated;
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::OnEmulatorKeyDown(WORD &Key)
{
TObject *Sender = this;
bool Treated = true;

  switch (Key)
  {
    case VK_F1:
    case VK_PAUSE:
    {
      PauseMenuClick(Sender);
    }
    break;

    case VK_F2:
    {
      LoadimageintoDriveAMenuClick(Sender);
    }
    break;

    case VK_F3:
    {
      LoadimageintoDriveBMenuClick(Sender);
    }
    break;

    case VK_F4:
    {
      SwapDrives();
    }
    break;

    case VK_F5:
    {
      DriveACatalogMenuClick(Sender);
    }
    break;

    case VK_F6:
    {
      ToggleCPUSpeedMenuClick(Sender);
    }
    break;

    case VK_F7:
    //case VK_VOLUME_DOWN:
    {
      if (VolumeDownMenu->Enabled == true)
      {
        VolumeDownMenuClick(Sender);
      }
    }
    break;

    case VK_F8:
    //case VK_VOLUME_UP:
    {
      if (VolumeUpMenu->Enabled == true)
      {
        VolumeUpMenuClick(Sender);
      }
    }
    break;

    case VK_VOLUME_MUTE:
    {
      MuteMenuClick(Sender);
    }
    break;

    case VK_F9:
    {
      if (mDrawingContextP->Fullscreen == true)
      {
        if (mDrawingContextP->DecreaseBrightness() == true)
        {
		      AnsiString Message = AnsiString("Brightness DOWN (") + AnsiString(mSettingsP->FullscreenBrightness) + AnsiString(")");
          AddEmulatorMessage(Message);
        }
      }
      else
      {
        ReduceDisplayMenuClick(Sender);
      }
    }
    break;

    case VK_F10:
    {
      if (mDrawingContextP->Fullscreen == true)
      {
        if (mDrawingContextP->IncreaseBrightness() == true)
        {
		      AnsiString Message = AnsiString("Brightness UP (") + AnsiString(mSettingsP->FullscreenBrightness) + AnsiString(")");
          AddEmulatorMessage(Message);
        }
      }
      else
      {
        EnlargeDisplayMenuClick(Sender);
      }
    }
    break;

    case VK_F11:
    {
      FullscreenMenuClick(Sender);
    }
    break;

    case VK_F12:
    {
      if (mSettingsP->SmallScreenshotShortcut == false)
      {
        ScreenshotMenuClick(Sender);
      }
      else
      {
        SmallScreenshotMenuClick(Sender);
      }
    }
    break;

    default:
      Treated = false;
  }

  return Treated;
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::OnEmulatorKeyDownWithAlt(WORD &Key)
{
bool Treated = true;

  switch (Key)
  {
    case VK_F4:
    {
      Close();
    }
    break;

    default:
      Treated = false;
  }

  return Treated;
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::OnEmulatorKeyDownWithCtrl(WORD &Key)
{
TObject *Sender = this;
bool Treated = true;

  switch (Key)
  {
    case VK_F1:
    {
      LoadSnapshotMenuClick(Sender);
    }
    break;

    case VK_F2:
    {
      ReloadSnapshotMenuClick(Sender);
    }
    break;

    case VK_F3:
    {
      UnloadSnapshotMenuClick(Sender);
    }
    break;

    case VK_F4:
    {
      UpdateSnapshotMenuClick(Sender);
    }
    break;

    case VK_F5:
    {
      PlayTapeMenuClick(Sender);
    }
    break;

    case VK_F6:
    {
      InsertTapeMenuClick(Sender);
    }
    break;

    case VK_F7:
    {
      GoToPreviousTagMenuClick(Sender);
    }
    break;

    case VK_F8:
    {
      GoToNextTagMenuClick(Sender);
    }
    break;

    case VK_F12:
    {
      ResetMenuClick(Sender);
    }
    break;

    default:
      Treated = false;
  }

  if (Treated == true)
  {
    // CONTROL key has been released ?
    if ((GetKeyState(VK_CONTROL) & 0x8000) == 0)
    {
      Perform(WM_KEYUP, VK_CONTROL, (int)0L);
    }
  }

  return Treated;
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::OnEmulatorKeyDownWithShift(WORD &Key)
{
TObject *Sender = this;
bool Treated = true;

  switch (Key)
  {
    case VK_F1:
    {
      HintsMenuClick(Sender);
    }
    break;

//    case VK_F2:
//    {
//      DecathlonModeMenuClick(Sender);
//    }
//    break;

//	case VK_F3:
//	{
//	  CombatSchoolModeMenuClick(Sender);
//	}
//	break;

    case VK_F4:
    {
      DriveAFlipSidesMenuClick(Sender);
    }
    break;

//	case VK_F5:
//	{
//	  KeyboardAsJoystickMenuClick(Sender);
//	}
//    break;

    case VK_F6:
    {
      MouseAsLightpenMenuClick(Sender);
    }
    break;

    case VK_F7:
    {
      MouseAsMagnumLightPhaserMenuClick(Sender);
    }
    break;

    case VK_F8:
    {
      MouseAsGunStickMenuClick(Sender);
    }
    break;

    case VK_F9:
    {
      MouseAsWestPhaserMenuClick(Sender);
    }
    break;

    case VK_F10:
    {
      MouseAsAMXMouseMenuClick(Sender);
    }
    break;

    default:
      Treated = false;
  }

  if (Treated == true)
  {
    // SHIFT key has been released ?
    if ((GetKeyState(VK_SHIFT) & 0x8000) == 0)
    {
      Perform(WM_KEYUP, VK_SHIFT, (int)0L);
    }
  }

  return Treated;
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::OnEmulatorKeyDownWithCtrlAndShift(WORD &Key)
{
	NOT_USED(Key);

	return false;
}
//---------------------------------------------------------------------------


bool __fastcall TFormMain::OnWINInputKeyUp(WORD &Key, TShiftState Shift)
{
bool Treated = false;

  if (mFullscreenDialogForm == NULL)
  {
    if (Key < 0x0100)
    {
      if (mEmulatorP != NULL)
      {
        Treated = mEmulatorP->OnWINKeyRelease(Key, Shift);
      }
    }

#ifdef USE_VK_SNAPSHOT
    if (Treated == false)
    {
      switch (Key)
      {
        case VK_SNAPSHOT:
        {
          if ( !Shift.Contains(ssAlt)
               && !Shift.Contains(ssCtrl)
               && !Shift.Contains(ssShift) )
          {
            ScreenshotMenuClick(NULL);
          }
        }
        break;
      }
    }
#endif /* USE_VK_SNAPSHOT */
  }

  return Treated;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::OnCPCInputKeyDown(tUChar Keycode)
{
  mEmulatorP->OnCPCKeyPress(Keycode);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnCPCInputKeyUp(tUChar Keycode)
{
  mEmulatorP->OnCPCKeyRelease(Keycode);
}
//---------------------------------------------------------------------------


//
// Mouse
//
void __fastcall TFormMain::PanelCPCScreenMouseDown(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y)
{
	mInputContextP->MouseDown(Button, X, Y);

	NOT_USED(Sender);
	NOT_USED(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelCPCScreenMouseUp(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y)
{
	mInputContextP->MouseUp(Button, X, Y);

	NOT_USED(Sender);
	NOT_USED(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelCPCScreenMouseMove(TObject *Sender,
			TShiftState Shift, int X, int Y)
{
	mInputContextP->MouseMove(X, Y);

	NOT_USED(Sender);
	NOT_USED(Shift);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelCPCScreenMouseClick(TObject *Sender)
{
	mInputContextP->MouseClick();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelCPCScreenMouseDblClick(TObject *Sender)
{
	mInputContextP->MouseDblClick();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelCPCScreenMouseEnter(TObject *Sender)
{
  // Reset AMX Mouse
  EmulatorP->ResetAMXMouse();

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::OnInputMouseDown(TMouseButton Button, int X, int Y)
{
	if (mFullscreenDialogForm)
	{
		mFullscreenDialogForm->OnMouseDown(Button,
																			 X - mDrawingContextP->OffscreenOriginLeft,
																			 Y - mDrawingContextP->OffscreenOriginTop);
	}
	else
	{
		if (OnEmulatorMouseDown != NULL)
		{
			OnEmulatorMouseDown(Button,
                          X - mDrawingContextP->EmulatorScreenLeft,
													Y - mDrawingContextP->EmulatorScreenTop);
		}
	}

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnInputMouseUp(TMouseButton Button, int X, int Y)
{
	if (mFullscreenDialogForm)
	{
		mFullscreenDialogForm->OnMouseMove(X - mDrawingContextP->OffscreenOriginLeft,
																			 Y - mDrawingContextP->OffscreenOriginTop);
	}
	else
	{
		if (OnEmulatorMouseUp != NULL)
		{
			OnEmulatorMouseUp(Button);
		}
	}

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnInputMouseMove(int X, int Y)
{
	if (mFullscreenDialogForm == NULL)
	{
		if (OnEmulatorMouseMove != NULL)
		{
			OnEmulatorMouseMove(X - mDrawingContextP->EmulatorScreenLeft,
													Y - mDrawingContextP->EmulatorScreenTop);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnInputMouseClick(int X, int Y)
{
  if (mFullscreenDialogForm)
  {
    mFullscreenDialogForm->OnMouseClick(X - mDrawingContextP->OffscreenOriginLeft,
                                        Y - mDrawingContextP->OffscreenOriginTop);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnInputMouseDblClick(int X, int Y)
{
  if (mFullscreenDialogForm)
  {
    mFullscreenDialogForm->OnMouseDblClick(X - mDrawingContextP->OffscreenOriginLeft,
                                           Y - mDrawingContextP->OffscreenOriginTop);
  }
}
//---------------------------------------------------------------------------


//
// Lightpen
//
void __fastcall TFormMain::OnLightpenMove(int X, int Y)
{
int OffscreenX = X * CPC_VISIBLE_SCR_WIDTH / mDrawingContextP->EmulatorScreenWidth;
int OffscreenY = Y * CPC_VISIBLE_SCR_HEIGHT / mDrawingContextP->EmulatorScreenHeight;

  EmulatorP->OnLightpenMove(OffscreenX,
                            OffscreenY);
}
//---------------------------------------------------------------------------


//
// Magnum Light Phaser
//
void __fastcall TFormMain::OnMagnumLightPhaserPressed(TMouseButton Button, int X, int Y)
{
  OnEmulatorMouseMove = OnMagnumLightPhaserMove;
  OnMagnumLightPhaserMove(X, Y);
  
  mEmulatorP->OnMagnumLightPhaserPressed();

  if (mRunTillNextLightgunTrigger == true)
  {
#ifdef _DEBUG
    //FormDebugLightgun->OnReleaseLightgunTriggerEvent = OnMagnumLightPhaserReleased;
#endif /* _DEBUG */
    OnEmulatorMouseMove = NULL;
    OnEmulatorMouseUp = NULL;
    mRunTillNextLightgunTrigger = false;
    mEmulatorP->Paused = true;
    PauseMenu->Checked = true;
  }

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnMagnumLightPhaserReleased(TMouseButton Button)
{
  mEmulatorP->OnMagnumLightPhaserReleased();
  OnEmulatorMouseMove = NULL;
  OnEmulatorMouseUp = OnMagnumLightPhaserReleased;

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnMagnumLightPhaserMove(int X, int Y)
{
int OffscreenX = X * CPC_VISIBLE_SCR_WIDTH / mDrawingContextP->EmulatorScreenWidth;
int OffscreenY = Y * CPC_VISIBLE_SCR_HEIGHT / mDrawingContextP->EmulatorScreenHeight;

  EmulatorP->OnLightpenMove(OffscreenX,
                            OffscreenY);
}
//---------------------------------------------------------------------------


//
// GunStick
//
void __fastcall TFormMain::OnGunStickPressed(TMouseButton Button, int X, int Y)
{
  OnEmulatorMouseMove = OnGunStickMove;
  OnGunStickMove(X, Y);

  mEmulatorP->OnGunStickPressed();

  if (mRunTillNextLightgunTrigger == true)
  {
#ifdef _DEBUG
    //FormDebugLightgun->OnReleaseLightgunTriggerEvent = OnGunStickReleased;
#endif /* _DEBUG */
    OnEmulatorMouseMove = NULL;
    OnEmulatorMouseUp = NULL;
    mRunTillNextLightgunTrigger = false;
    mEmulatorP->Paused = true;
    PauseMenu->Checked = true;
  }

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnGunStickReleased(TMouseButton Button)
{
  mEmulatorP->OnGunStickReleased();
  OnEmulatorMouseMove = NULL;
  OnEmulatorMouseUp = OnGunStickReleased;

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnGunStickMove(int X, int Y)
{
int OffscreenX = X * CPC_VISIBLE_SCR_WIDTH / mDrawingContextP->EmulatorScreenWidth;
int OffscreenY = Y * CPC_VISIBLE_SCR_HEIGHT / mDrawingContextP->EmulatorScreenHeight;

  EmulatorP->OnPhaserMove(OffscreenX,
                          OffscreenY);
}
//---------------------------------------------------------------------------


//
// West Phaser
//
void __fastcall TFormMain::OnWestPhaserPressed(TMouseButton Button, int X, int Y)
{
  //OnEmulatorMouseMove = OnWestPhaserMove;
  OnWestPhaserMove(X, Y);

  mEmulatorP->OnWestPhaserPressed();

  if (mRunTillNextLightgunTrigger == true)
  {
#ifdef _DEBUG
    //FormDebugLightgun->OnReleaseLightgunTriggerEvent = OnWestPhaserReleased;
#endif /* _DEBUG */
    OnEmulatorMouseMove = NULL;
    OnEmulatorMouseUp = NULL;
    mRunTillNextLightgunTrigger = false;
    mEmulatorP->Paused = true;
    PauseMenu->Checked = true;
  }

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnWestPhaserReleased(TMouseButton Button)
{
  mEmulatorP->OnWestPhaserReleased();
  OnEmulatorMouseUp = OnWestPhaserReleased;

	NOT_USED(Button);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnWestPhaserMove(int X, int Y)
{
int OffscreenX = X * CPC_VISIBLE_SCR_WIDTH / mDrawingContextP->EmulatorScreenWidth;
int OffscreenY = Y * CPC_VISIBLE_SCR_HEIGHT / mDrawingContextP->EmulatorScreenHeight;

  EmulatorP->OnPhaserMove(OffscreenX, OffscreenY);
}
//---------------------------------------------------------------------------

//
// AMX Mouse
//
void __fastcall TFormMain::OnAMXMousePressed(TMouseButton Button, int X, int Y)
{
  if (Button == mbRight)
  {
    mEmulatorP->OnAMXMouseButtonRPressed();
  }
  else if (Button == mbMiddle)
  {
    mEmulatorP->OnAMXMouseButtonMPressed();
  }
  else
  {
    mEmulatorP->OnAMXMouseButtonLPressed();
  }

	NOT_USED(X);
	NOT_USED(Y);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnAMXMouseReleased(TMouseButton Button)
{
  if (Button == mbRight)
  {
    mEmulatorP->OnAMXMouseButtonRReleased();
  }
  else if (Button == mbMiddle)
  {
    mEmulatorP->OnAMXMouseButtonMReleased();
  }
  else
  {
    mEmulatorP->OnAMXMouseButtonLReleased();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnAMXMouseMove(int X, int Y)
{
int OffscreenX = X * CPC_VISIBLE_SCR_WIDTH / mDrawingContextP->EmulatorScreenWidth;
int OffscreenY = Y * CPC_VISIBLE_SCR_HEIGHT / mDrawingContextP->EmulatorScreenHeight;

  EmulatorP->OnMouseMove(OffscreenX, OffscreenY);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::SetFullscreen(void)
{
  SetFocus();

  // Stop step by step
  if ( (mEmulatorP->StepByStepActive == true) && (mEmulatorP->Paused == true) )
  {
    StopStepByStep(true); // Stay in pause
  }
  else if (mEmulatorP->StepByStepActive == true)
  {
    StopStepByStep(false);
  }

  // Save windowed state
  mFullScreenOldStyle = GetWindowLong(Handle, GWL_STYLE) | WS_VISIBLE;
  mFullScreenOldExStyle = GetWindowLong(Handle, GWL_EXSTYLE);
  mFullScreenOldTop = Top;
  mFullScreenOldLeft = Left;
  mFullScreenOldWidth = Width;
  mFullScreenOldHeight = Height;
  mFullScreenControlPanelVisible = PanelControl->Visible;

  // Set fullscreen
  LONG dwStyle = mFullScreenOldStyle & ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
  LONG dwExStyle = mFullScreenOldExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_CONTROLPARENT);
  SetWindowLong(Handle, GWL_STYLE, dwStyle);
  SetWindowLong(Handle, GWL_EXSTYLE, dwExStyle);

  if (mSettingsP->KeepFullscreen == true)
  {
    mSettingsP->Fullscreen = true;
  }
  mDrawingContextP->Fullscreen = true;
  mInputContextP->Fullscreen = true;
  PanelControl->Visible = false;

  if (mInfoFormP)
  {
    mInfoFormP->Fullscreen = mDrawingContextP->Fullscreen;
  }

  if (mSettingsP->FullscreenMenu == false)
  {
    Menu = NULL; // Hide menu
  }
  // Disable problematic menu
  EmulatorSettingsMenu->Enabled = false;
  OpenCapriceFolderMenu->Enabled = false;
  ReduceDisplayMenu->Enabled = false;
  EnlargeDisplayMenu->Enabled = false;
  DisplaySettingsMenu->Enabled = false;
  ControlPanelMenu->Enabled = false;
  OpenScreenshotsFolderMenu->Enabled = false;
  ExtraMenu->Enabled = false;
  HelpMenu->Enabled = false;

  if (mMouseNecessary == false)
  {
    if (mSettingsP->FullscreenMenu == false)
    {
      ShowCursor(FALSE);
    }
  }
  else
  {
    // Set fullscreen cursor
    mOldCursor = Screen->Cursor;
    Screen->Cursor = crFullscreenCursor;
  }

  HMONITOR hMonitor = MonitorFromWindow(Handle,
                                        MONITOR_DEFAULTTONEAREST);
  MONITORINFO mi = { sizeof(mi) };
  GetMonitorInfo(hMonitor, &mi);
  SetWindowPos(Handle,
               HWND_TOPMOST,
               mi.rcMonitor.left,
               mi.rcMonitor.top,
               mi.rcMonitor.right - mi.rcMonitor.left,
               mi.rcMonitor.bottom - mi.rcMonitor.top,
               SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

  AddEmulatorMessage("Entering Fullscreen mode (Press F11 to exit, F9 & F10 to adjust brightness)");
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SetWindowed(void)
{
  if (mDrawingContextP->Fullscreen == true)
  {
    // Set first to resize correctly
    mSettingsP->Fullscreen = false;
    mDrawingContextP->Fullscreen = false;
    mInputContextP->Fullscreen = false;

    if (mInfoFormP)
    {
      mInfoFormP->Fullscreen = mDrawingContextP->Fullscreen;
    }

    // Restore windowed state
    SetWindowLong(Handle, GWL_EXSTYLE, mFullScreenOldExStyle);
    SetWindowLong(Handle, GWL_STYLE, mFullScreenOldStyle);
    PanelControl->Visible = mFullScreenControlPanelVisible;
    SetWindowPos(Handle,
                 HWND_TOP,
                 mFullScreenOldLeft,
                 mFullScreenOldTop,
                 mFullScreenOldWidth,
                 mFullScreenOldHeight,
                 SWP_NOZORDER | SWP_FRAMECHANGED);

    mFullScreenOldStyle = 0;
    mFullScreenOldExStyle = 0;

    if (mSettingsP->FullscreenMenu == false)
    {
      Menu = MainMenu; // Show menu
    }
    // Enable problematic menu
    EmulatorSettingsMenu->Enabled = true;
    OpenCapriceFolderMenu->Enabled = true;
    ReduceDisplayMenu->Enabled = true;
    EnlargeDisplayMenu->Enabled = true;
    DisplaySettingsMenu->Enabled = true;
    ControlPanelMenu->Enabled = true;
    OpenScreenshotsFolderMenu->Enabled = true;
    ExtraMenu->Enabled = true;
    HelpMenu->Enabled = true;

    if (mMouseNecessary == false)
    {
      if (mSettingsP->FullscreenMenu == false)
      {
        ShowCursor(TRUE);
      }
    }
    Screen->Cursor = mOldCursor;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AddEmulatorMessage(String NewMessage)
{
  // get display duration
  TBaseTime TillEvent;
  mBaseTimerP->SetNextOccurenceS(&TillEvent, MESSAGE_EMULATOR_DURATION);

  TEmulatorMessage* MessageP = new TEmulatorMessage(NewMessage, TillEvent);
  // Insert message at first position
  mEmulatorMessages->Insert(0, MessageP);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::AddEmulatorMessage(String NewMessage, int Duration)
{
  // get display duration
  TBaseTime TillEvent;
  mBaseTimerP->SetNextOccurenceS(&TillEvent, Duration);

  TEmulatorMessage* MessageP = new TEmulatorMessage(NewMessage, TillEvent);
  // Insert message at first position
  mEmulatorMessages->Insert(0, MessageP);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::RemoveEmulatorMessage(int IndexToRemove)
{
	if (IndexToRemove < mEmulatorMessages->Count)
	{
		TEmulatorMessage* MessageP = (TEmulatorMessage*)mEmulatorMessages->Items[IndexToRemove];
		delete MessageP;
		mEmulatorMessages->Delete(IndexToRemove);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::DisplayEmulatorMessage(void)
{
  if (mSettingsP->OnscreenDisplay & OSD_MESSAGES)
  {
    // Draw all messages
    mDrawingContextP->DrawEmulatorMessages(mEmulatorMessages);

    // Update messages
    for (int Index=0; Index < mEmulatorMessages->Count; Index++)
    {
      TEmulatorMessage* MessageP = (TEmulatorMessage*)mEmulatorMessages->Items[Index];

      if (MessageP->IsDurationElapsed(mBaseTimerP) == true)
      {
        // Message to remove ?
        if (MessageP->DecreaseTransparency() == true)
        {
          RemoveEmulatorMessage(Index);
          Index--; // Synchronize index
        }
      }
    }
  }
  else
  {
    // Remove all pending messages
    for (int Index=0; Index < mEmulatorMessages->Count; Index++)
    {
      RemoveEmulatorMessage(Index);
      Index--; // Synchronize index
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::OnFullscreenOpenDSKDialogEnd(TDialogFormEndCause EndCause)
{
TDialogOpenDSK* FileDialog = (TDialogOpenDSK*)mFullscreenDialogForm;

  // Load
  if (EndCause == Button1)
  {
    AnsiString Filename = FileDialog->SelectedFile;

    if (FileDialog->DriveAActive == true)
    {
      LoadDriveA(Filename, true);
    }
    else
    {
      LoadDriveB(Filename);
    }
  }
  // New
  else if (EndCause == Button2)
  {
    if (FileDialog->DriveAActive == true)
    {
      CreateDriveA(FileDialog->CurrentPath + NEW_DISK_FILENAME);
    }
    else
    {
      CreateDriveB(FileDialog->CurrentPath + NEW_DISK_FILENAME);
    }
  }

  // Exit dialog mode
  delete mFullscreenDialogForm;
  mFullscreenDialogForm = NULL;

  if (mMouseNecessary == true)
  {
    Screen->Cursor = crFullscreenCursor;
  }
  else
  {
    if (mSettingsP->FullscreenMenu == false)
    {
      ShowCursor(FALSE);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnFullscreenOpenSNADialogEnd(TDialogFormEndCause EndCause)
{
TDialogOpenSNA* FileDialog = (TDialogOpenSNA*)mFullscreenDialogForm;

  // Load
  if (EndCause == Button1)
  {
    if (mEmulatorP->LoadSnapshot(FileDialog->SelectedFile) == true)
    {
      ReloadSnapshotMenu->Enabled = true;
      UnloadSnapshotMenu->Enabled = true;
      UpdateSnapshotMenu->Enabled = true;
      mSettingsP->SnapshotDirectory = ExtractFilePath(mEmulatorP->SnapshotFilename);

      AnsiString Filename = ExtractFileName(mEmulatorP->SnapshotFilename);
      AddEmulatorMessage("Snapshot=" + Filename);

      if (mInfoFormP)
      {
        mInfoFormP->Snapshot = "Snapshot=" + Filename;
      }

      // Remove Pause
      Pause(false);
    }
  }

  // Exit dialog mode
  delete mFullscreenDialogForm;
  mFullscreenDialogForm = NULL;

  if (mMouseNecessary == true)
  {
    Screen->Cursor = crFullscreenCursor;
  }
  else
  {
    if (mSettingsP->FullscreenMenu == false)
    {
      ShowCursor(FALSE);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnFullscreenOpenCDTDialogEnd(TDialogFormEndCause EndCause)
{
TDialogOpenCDT* FileDialog = (TDialogOpenCDT*)mFullscreenDialogForm;

  // Load
  if (EndCause == Button1)
  {
    InsertTape(FileDialog->SelectedFile, true);
  }

  // Exit dialog mode
  delete mFullscreenDialogForm;
  mFullscreenDialogForm = NULL;

  if (mMouseNecessary == true)
  {
    Screen->Cursor = crFullscreenCursor;
  }
  else
  {
    if (mSettingsP->FullscreenMenu == false)
    {
      ShowCursor(FALSE);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnFullscreenDiskCatalogDialogEnd(TDialogFormEndCause EndCause)
{
TDialogDiskCatalog* FileDialog = (TDialogDiskCatalog*)mFullscreenDialogForm;

  // RUN
  if (EndCause == Button1)
  {
    mEmulatorP->RUNFilename(FileDialog->SelectedFile);
  }
  else if (EndCause == Button2)
  {
    mEmulatorP->LOADFilename(FileDialog->SelectedFile);
  }
  else if (EndCause == Button3)
  {
    mEmulatorP->ExecuteCPM();
  }

  // Exit dialog mode
  delete mFullscreenDialogForm;
  mFullscreenDialogForm = NULL;

  if (mMouseNecessary == true)
  {
    Screen->Cursor = crFullscreenCursor;
  }
  else
  {
    if (mSettingsP->FullscreenMenu == false)
    {
      ShowCursor(FALSE);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::OnEmulatorNextLightgunTrigger(void)
{
  mRunTillNextLightgunTrigger = true;
  mEmulatorP->Paused = false;
  PauseMenu->Checked = false;

  // Activate Step by Step
  mEmulatorP->StartStepByStep();
  mAudioContextP->Pause();

  // Deactivate Pause display during Step by step
  delete mInfoFormP;
  mInfoFormP = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ResumeStepByStep(TEmulatorCPCEvent FirstCallback)
{
  // Callback goes first
  MaximizeEmulatorEndEventPriority(FirstCallback);

  mEmulatorP->StartStepByStep();
  Pause(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::StopStepByStep(bool Paused)
{
  mEmulatorP->StopStepByStep();
  Pause(Paused);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::OnDiskEvent(tDrive* DriveP)
{
  if (DriveP->signal_content_change == false)
    return;
    
  if (DriveP->dataP != NULL)
  {
    AnsiString DrivePosition = AnsiString::Format("%.2d:00",
            ARRAYOFCONST(((int)DriveP->current_track)));

    if (DriveP == mEmulatorP->NativeCPC->FDC->DriveA)
    {
      PanelControlDriveAPosition->Caption = DrivePosition;
    }
    else
    {
      PanelControlDriveBPosition->Caption = DrivePosition;
    }
  }
  else
  {
    if (DriveP == mEmulatorP->NativeCPC->FDC->DriveA)
    {
      PanelControlDriveAPosition->Caption = "";
    }
    else
    {
      PanelControlDriveBPosition->Caption = "";
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::OnFDCEvent(tFDC* FDC)
{
  if (FDC->signal_new_command)
  {
    if ( (FDC->active_drive != NULL)
         && (FDC->active_drive->dataP != NULL) )
    {
      tUChar Command = FDC->command[CMD_CODE] & 0x1f;
      tULong current_track = FDC->active_drive->current_track;
      AnsiString FDCOperation = "";
      if (    (Command == 0x06)   // Read Data
           || (Command == 0x0C)   // Read Deleted Data
           || (Command == 0x05)   // Write Data
           || (Command == 0x09)   // Write Deleted Data
           || (Command == 0x02) ) // Read a Track
      {
        if (current_track < 100)
        {
          if (current_track < 10) FDCOperation += "0";
          FDCOperation += AnsiString(current_track);
        }
        else
        {
          FDCOperation += "99";
        }
        FDCOperation += ":";
        if ((FDC->result[RES_ST0] & ST0_NT_MASK) == ST0_NT)
        {
          FDCOperation += AnsiString::IntToHex(FDC->command[CMD_R],2);
        }
        else
        {
          FDCOperation += "00";
        }
      }
      else if (    (Command == 0x0D)   // Format a Track
                || (Command == 0x0A) ) // Read ID
      {
        if (current_track < 100)
        {
          if (current_track < 10) FDCOperation += "0";
          FDCOperation += AnsiString(current_track);
        }
        else
        {
          FDCOperation += "99";
        }
        FDCOperation += ":00";
      }

      if (FDCOperation.IsEmpty() == false)
      {
        // Update DriveA Status
        if (FDC->active_drive == mEmulatorP->NativeCPC->FDC->DriveA)
        {
          PanelControlDriveAPosition->Caption = FDCOperation;
        }
        // Update DriveB Status
        else
        {
          PanelControlDriveBPosition->Caption = FDCOperation;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SpeedButtonControlResetClick(TObject *Sender)
{
  ResetMenuClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlMuteClick(TObject *Sender)
{
  MuteMenuClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlDriveAClick(TObject *Sender)
{
	if (mEmulatorP->DiskAFilename.IsEmpty() == true)
	{
		// Load disk
		OpenDialog->Title = "Load disk image on Drive A";
		OpenDialog->FileName = mEmulatorP->DiskAFilename;
		OpenDialog->Filter = DISKIMAGE_FILTER_OPEN;
		OpenDialog->InitialDir = mSettingsP->DiscDirectory;
		if (OpenDialog->Execute() == true)
		{
			LoadDriveA(OpenDialog->FileName, true);
		}
	}
	else
	{
		// Eject disk
		EjectDriveA();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlDriveBClick(TObject *Sender)
{
	if (mEmulatorP->DiskBFilename.IsEmpty() == true)
	{
		// Load disk
		OpenDialog->Title = "Load disk image on Drive B";
		OpenDialog->FileName = mEmulatorP->DiskBFilename;
		OpenDialog->Filter = DISKIMAGE_FILTER_OPEN;
		OpenDialog->InitialDir = mSettingsP->DiscDirectory;
		if (OpenDialog->Execute() == true)
		{
			LoadDriveB(OpenDialog->FileName);
		}
	}
	else
	{
		// Eject disk
		EjectDriveB();
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelControlDriveAPositionClick(TObject *Sender)
{
	DriveAWriteProtectionMenuClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelControlDriveBPositionClick(TObject *Sender)
{
  DriveBWriteProtectionMenuClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlSwapDrivesClick(
      TObject *Sender)
{
	SwapDrives();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlTapeClick(TObject *Sender)
{
  if (mEmulatorP->TapeFilename.IsEmpty() == true)
  {
    // Insert tape
    OpenDialog->Title = "Load tape image";
    OpenDialog->FileName = mEmulatorP->TapeFilename;
    OpenDialog->Filter = TAPEIMAGE_FILTER;
    OpenDialog->InitialDir = mSettingsP->TapeDirectory; 
    if (OpenDialog->Execute() == true)
    {
      InsertTape(OpenDialog->FileName, true);
    }
  }
  else
  {
    // Eject tape
    EjectTapeMenuClick(Sender);
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlTapeRewindClick(
      TObject *Sender)
{
  RewindTapeMenuClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlTapePreviousClick(
      TObject *Sender)
{
  GoToPreviousTagMenuClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlTapePlayStopClick(
      TObject *Sender)
{
  PlayTapeMenuClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SpeedButtonControlTapeNextClick(TObject *Sender)
{
  GoToNextTagMenuClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ResetEmulator(void)
{
  bool bResetOK = false;

  // Snapshot will be unloaded
	ReloadSnapshotMenu->Enabled = false;
	UnloadSnapshotMenu->Enabled = false;
	UpdateSnapshotMenu->Enabled = false;

  // Save emulator context
  TEmulatorContext* ContextP = new TEmulatorContext;
  mEmulatorP->Save(ContextP);

  do
  {
    Err EmuError = mEmulatorP->Restart(mSettingsP);
    if (EmuError != errNone)
    {
#ifdef _DEBUG
			String msg = _T("Unable to restart emulator. Error=") + String::IntToHex(EmuError,8);
			Application->MessageBox(msg.c_str(), _T("ResetEmulator"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    mExecuteCondition = EC_FRAME_COMPLETE;

    // Signal RESET to all subscribers
    for (int Index=0; Index < mOnEmulatorCPCEventsList->Count; Index++)
    {
      TEmulatorCPCEvent* Event = (TEmulatorCPCEvent*)mOnEmulatorCPCEventsList->Items[Index];
      if (((TEmulatorCPCEvent)*Event)(EC_RESET, mEmulatorP->NativeCPC) == true)
      {
        // Pause emulation
        if (mEmulatorP->Paused == false)
        {
          Pause(true);
        }
      }
    }

    // Restore emulator context
    mEmulatorP->Restore(ContextP);

    // Stop playing tape
    if (PlayTapeMenu->Checked == true)
    {
      PlayTapeMenuClick(NULL);
    }

    bResetOK = true;
  }
  while (0);

  delete ContextP;

  // Bad reset
  if (bResetOK == false)
  {
    Pause(true);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::SetRealCPCSpeed(void)
{
  // Set 100% CPC speed
  mFullSpeed = false;
  delete mBaseTimerP;
  mBaseTimerP = new TBaseTimer(CYCLES_PER_SECOND);

  if (mEmulatorP->Paused == true)
  {
    mIdleEventBeforePause = OnIdleNormalSpeed;
  }
  else
  {
    Application->OnIdle = OnIdleNormalSpeed;
  }

  AddEmulatorMessage("Real CPC speed emulation");

	mBaseTimerP->StartSecondTimer();

  // Restore title without speed
  Caption = mApplicationTitle;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SetFullSpeed(void)
{
  // Full speed
  mFullSpeed = true;

  if (mEmulatorP->Paused == true)
  {
    mIdleEventBeforePause = OnIdleFullSpeed;
  }
  else
  {
    Application->OnIdle = OnIdleFullSpeed;
  }

  AddEmulatorMessage("FULL speed emulation");

  mBaseTimerP->StartSecondTimer();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SetSpeed300Percent(void)
{
  // Set 300% CPC speed
  mFullSpeed = false;
  delete mBaseTimerP;

  mBaseTimerP = new TBaseTimer(CYCLES_PER_SECOND * 3);

  if (mEmulatorP->Paused == true)
  {
    mIdleEventBeforePause = OnIdleNormalSpeed;
  }
  else
  {
    Application->OnIdle = OnIdleNormalSpeed;
  }

  AddEmulatorMessage("300% speed emulation");

  // Restore title without speed
  Caption = mApplicationTitle;

  mBaseTimerP->StartSecondTimer();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SetSpeed200Percent(void)
{
  // Set 200% CPC speed
  mFullSpeed = false;
  delete mBaseTimerP;
  mBaseTimerP = new TBaseTimer(CYCLES_PER_SECOND * 2);

  if (mEmulatorP->Paused == true)
  {
    mIdleEventBeforePause = OnIdleNormalSpeed;
  }
  else
  {
    Application->OnIdle = OnIdleNormalSpeed;
  }

  AddEmulatorMessage("200% speed emulation");

  // Restore title without speed
  Caption = mApplicationTitle;

  mBaseTimerP->StartSecondTimer();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::SetSpeedCustom(void)
{
  // Set Custom CPC speed
  mFullSpeed = false;
  delete mBaseTimerP;
  int CustomSpeed = (int)((float)mSettingsP->MaxCustomSpeed / 100. * CYCLES_PER_SECOND);
  if (CustomSpeed == 0) CustomSpeed = 100;
  mBaseTimerP = new TBaseTimer(CustomSpeed);

  if (mEmulatorP->Paused == true)
  {
    mIdleEventBeforePause = OnIdleNormalSpeed;
  }
  else
  {
    Application->OnIdle = OnIdleNormalSpeed;
  }

  AddEmulatorMessage("Custom speed emulation (" + AnsiString(mSettingsP->MaxCustomSpeed) + "%)");

  // Restore title without speed
  Caption = mApplicationTitle;

  mBaseTimerP->StartSecondTimer();
}
//---------------------------------------------------------------------------


int __fastcall TFormMain::GetCommandParamList(TStringList* ParamList)
{
  ParamList->Clear();

  AnsiString CommandLine = CmdLine;
  char* Command = AnsiString(CommandLine).c_str();

  int Quotation = 0;
  AnsiString Param = "";

  while (*Command != 0)
  {
    if (*Command == '\"')
    {
      // Double quotation mark inside quoted text
      if ( (*(Command+1) == '\"') && ((Quotation & 1) == 1) )
      {
        Param += *Command;

        // Pass next quotation mark
        Command++;
      }
      else
      {
        Quotation++;
      }
    }
    // Parameter separator
    else if ( (*Command == ' ') && ((Quotation & 1) == 0) )
    {
      // End of parameter
      if (Param.IsEmpty() == false)
      {
        ParamList->Add(Param);
        Param = "";
      }
    }
    else
    {
      Param += *Command;
    }

    Command++;
  }

  // Last parameter
  if (Param.IsEmpty() == false)
  {
    ParamList->Add(Param);
  }

  return ParamList->Count;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AddRecentDisk(AnsiString Filename)
{
  // Delete already existing entries
  int Position = mRecentDiskFilenames->IndexOf(Filename);
  while (Position != -1)
  {
    mRecentDiskFilenames->Delete(Position);
    Position = mRecentDiskFilenames->IndexOf(Filename);
  }

  // Insert the file at first position
  mRecentDiskFilenames->Insert(0, Filename);

  // Limit entries
  while (mRecentDiskFilenames->Count > NB_MAX_RECENT_FILES)
  {
    mRecentDiskFilenames->Delete(NB_MAX_RECENT_FILES-1);
  }

  // Update menu
  DriveALoadRecentImageMenu->Enabled = true;
  DriveALoadRecentImageMenu->Clear();
  for (int Loop=0; Loop < mRecentDiskFilenames->Count; Loop++)
  {
    TMenuItem* NewItem = new TMenuItem(NULL);
    NewItem->Caption = mRecentDiskFilenames->Strings[Loop];
    NewItem->OnClick = LoadDriveAStoredImageMenuClick;

    DriveALoadRecentImageMenu->Add(NewItem);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::AddFavouriteDisk(AnsiString Filename)
{
  // Delete already existing entries
  int Position = mFavouriteDiskFilenames->IndexOf(Filename);
  while (Position != -1)
  {
    mFavouriteDiskFilenames->Delete(Position);
    Position = mFavouriteDiskFilenames->IndexOf(Filename);
  }

  // Insert the file at first position
  mFavouriteDiskFilenames->Insert(0, Filename);

  // Limit entries
  while (mFavouriteDiskFilenames->Count > NB_MAX_FAVOURITE_FILES)
  {
    mFavouriteDiskFilenames->Delete(NB_MAX_FAVOURITE_FILES-1);
  }

  // Update menu
  DriveALoadFavouriteImageMenu->Enabled = true;
  DriveALoadFavouriteImageMenu->Clear();
  for (int Loop=0; Loop < mFavouriteDiskFilenames->Count; Loop++)
  {
    TMenuItem* NewItem = new TMenuItem(NULL);
    NewItem->Caption = mFavouriteDiskFilenames->Strings[Loop];
    NewItem->OnClick = LoadDriveAStoredImageMenuClick;

    DriveALoadFavouriteImageMenu->Add(NewItem);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::UpdateDriveAMenu(void)
{
  // Update DriveA menu
  if (mEmulatorP->DiskAFilename.IsEmpty() == false)
  {
    DriveACatalogMenu->Enabled = true;
    DriveAFlipSidesMenu->Enabled = mEmulatorP->DriveADoubleSided;
    DriveAFlipSidesMenu->Checked = mEmulatorP->DriveAFlipped;
    DriveAEjectMenu->Enabled = true;
    DriveAUpdateDiskImageMenu->Enabled = mSettingsP->NeverUpdateDiskImages == false ? true : false;
    DriveAReloadDiskImageMenu->Enabled = true;
    DriveAWriteProtectionMenu->Enabled = true;
    DriveAWriteProtectionMenu->Checked = mEmulatorP->DriveAWriteProtected;
    DriveAAddToFavouriteMenu->Enabled = true;
  }
  else
  {
    DriveACatalogMenu->Enabled = false;
    DriveAFlipSidesMenu->Enabled = false;
    DriveAFlipSidesMenu->Checked = false;
    DriveAEjectMenu->Enabled = false;
    DriveAUpdateDiskImageMenu->Enabled = false;
    DriveAReloadDiskImageMenu->Enabled = false;
    DriveAWriteProtectionMenu->Enabled = false;
    DriveAWriteProtectionMenu->Checked = false;
    DriveAAddToFavouriteMenu->Enabled = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::UpdateDriveBMenu(void)
{
  // Update DriveB menu
  if (mEmulatorP->DiskBFilename.IsEmpty() == false)
  {
    DriveBFlipSidesMenu->Enabled = mEmulatorP->DriveBDoubleSided;
    DriveBFlipSidesMenu->Checked = mEmulatorP->DriveBFlipped;
    DriveBEjectMenu->Enabled = true;
    DriveBUpdateDiskImageMenu->Enabled = mSettingsP->NeverUpdateDiskImages == false ? true : false;
    DriveBReloadDiskImageMenu->Enabled = true;
    DriveBWriteProtectionMenu->Enabled = true;
    DriveBWriteProtectionMenu->Checked = mEmulatorP->DriveBWriteProtected;
    DriveBAddToFavouriteMenu->Enabled = true;
  }
  else
  {
    DriveBFlipSidesMenu->Enabled = false;
    DriveBFlipSidesMenu->Checked = false;
    DriveBEjectMenu->Enabled = false;
    DriveBUpdateDiskImageMenu->Enabled = false;
    DriveBReloadDiskImageMenu->Enabled = false;
    DriveBWriteProtectionMenu->Enabled = false;
    DriveBWriteProtectionMenu->Checked = false;
    DriveBAddToFavouriteMenu->Enabled = false;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::UpdateShowHints(void)
{
  ShowHint = mSettingsP->ShowHints;
  ShowHintsMenu->Checked = ShowHint;

  // Update windows
  if (FormDebugDSKViewer) FormDebugDSKViewer->ShowHint = ShowHint;
  if (FormDebugCRTC) FormDebugCRTC->ShowHint = ShowHint;
  if (FormDebugGateArray) FormDebugGateArray->ShowHint = ShowHint;
  if (FormDebugFDC) FormDebugFDC->ShowHint = ShowHint;
  if (FormDebugZ80Editor) FormDebugZ80Editor->ShowHint = ShowHint;
  if (FormDebugMemoryEditor) FormDebugMemoryEditor->ShowHint = ShowHint;
  if (FormDebugPSG) FormDebugPSG->ShowHint = ShowHint;
  if (FormDebugCDTViewer) FormDebugCDTViewer->ShowHint = ShowHint;
  if (FormDebugTapeAudioWorkshop) FormDebugTapeAudioWorkshop->ShowHint = ShowHint;
  if (FormDebugMEA8000) FormDebugMEA8000->ShowHint = ShowHint;
  if (FormDebugTextCapture) FormDebugTextCapture->ShowHint = ShowHint;
  if (FormDebugger) FormDebugger->ShowHint = ShowHint;
  if (FormPrinter) FormPrinter->ShowHint = ShowHint;
  if (FormCheatScripts) FormCheatScripts->ShowHint = ShowHint;
  if (FormGraphicsExplorer) FormGraphicsExplorer->ShowHint = ShowHint;
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::ShowHintsMenuClick(TObject *Sender)
{
  mSettingsP->ShowHints = ShowHintsMenu->Checked == true ? false : true;
  UpdateShowHints();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


