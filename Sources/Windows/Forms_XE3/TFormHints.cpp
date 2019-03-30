/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2016 by Frédéric Coste

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
#pragma hdrstop
#include "..\..\Engine\types.h"
#include "TFormHints.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormHints *FormHints;
//---------------------------------------------------------------------------
__fastcall TFormHints::TFormHints(TComponent* Owner)
	: TForm(Owner)
{
AnsiString Message;

	// Prepare hints content
	RichEditHints->Clear();

  ShowTitle();
  ShowForeword();
  ShowHints();
  ShowRunDiscProgram();
  ShowDriveACatalog();
  ShowConcerningIPF();
  ShowRecentDiskImages();
  ShowFavouriteDiskImages();
  ShowRunTapeProgram();
  ShowPlayAudioTape();
  ShowParameters();
  ShowDefaultFolders();
  ShowUseDirectX();
  ShowForceGDI();
  ShowScanlinesPattern();
  ShowGameControllers();
  ShowPause();
  ShowKeyboardMappings();
  ShowAudio();
  ShowSiliconDisc();
  ShowFloppyIconSize();
  ShowOnscreenDisplay();
  ShowDecathlon();
  ShowCombatSchool();
  ShowAutostart();
  ShowBrightness();
  ShowSettingsOverride();
  ShowScreenshot();
  ShowWindowsPriority();
  ShowTurboSpeed();
  ShowColdResetAtRealSpeed();
  ShowCommandLine();
  ShowDrivePopup();
  ShowShortcuts();

#if (__BORLANDC__ == 0x651) || (__BORLANDC__ == 0x680)
	// Show top of text
	RichEditHints->CaretPos = TPoint(0,0);
#endif /* __BORLANDC__ */
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::FormResize(TObject *Sender)
{
NOT_USED(Sender);

	RichEditHints->Invalidate();
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowTitle(void)
{
  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold;
  RichEditHints->SelAttributes->Size = 18;
  RichEditHints->Lines->Add("Hints about Caprice Forever");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowForeword(void)
{
AnsiString Message;

  Message  = "Caprice Forever is an Amstrad CPC 8-bit computers emulator.";
  RichEditHints->Lines->Add(Message);
  Message  = "Caprice Forever will not teach you how to use these computers.";
  Message += " It might be useful to begin by reading a user manual, now easily available in PDF.";
  RichEditHints->Lines->Add(Message);
  Message  = "Almost all programs can be run using CPC6128 default machine settings (BASIC 1.1, 128Ko).";
  Message += " Other peripherals or extra memory might be needed by rare specific software.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowHints(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Hints");

  Message  = "Many controls can show an help message when cursor stays on them.";
  RichEditHints->Lines->Add(Message);
  Message  = "Enabled by default, this display can be disabled.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Help! > Show hints";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowRunDiscProgram(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Run program from a disk");

  Message  = "First of all, insert a disk image including a program to run.";
  RichEditHints->Lines->Add(Message);
  Message  = "Browse disk content by using \"CAT\" command to find any BAS or BIN program and type \"RUN\"file\" command. ";
  Message += "No such files, try to run any file.";
  RichEditHints->Lines->Add(Message);
  Message  = "In case of something wrong, try \"|CPM\" command to launch CPM program.";
  RichEditHints->Lines->Add(Message);
  Message  = "Still problems, check your disk provider to find any clues.";
  RichEditHints->Lines->Add(Message);
  Message  = "Most of ever written programs for Amstrad CPC are freely available at CPC-Power.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowDriveACatalog(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Drive A Catalog");

  Message  = "It is possible to graphically launch a program from Drive A disk";
  Message += " by pressing F5 when a disk is inserted into Drive A.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Disk > Drive A Calalog...";
  RichEditHints->Lines->Add(Message);
  Message  = "A dialog window will show any file and will propose to RUN, LOAD or type |CPM.";
  RichEditHints->Lines->Add(Message);
  Message  = "Press F1 to RUN selected file.";
  RichEditHints->Lines->Add(Message);
  Message  = "Press F2 to LOAD selected file.";
  RichEditHints->Lines->Add(Message);
  Message  = "Press F3 to type |CPM.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowConcerningIPF(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Concerning IPF");

  Message  = "IPF, for Interchangeable Preservation Format, are unmodified original disk images.";
  Message += " For preservation purpose, these files contain enough data to be able to create a perfect clone.";
  RichEditHints->Lines->Add(Message);
  Message  = "To prevent copyright issues, distribution of IPF files has been forbidden by the SPS (http://www.softpres.org).";
  RichEditHints->Lines->Add(Message);
  Message  = "IPF can be read by Caprice Forever without any SPS libraries. Any modifications will be stored in DSK format.";
  Message += " As IPF files are difficult to get, current code is not finished, especially for protected software.";
  RichEditHints->Lines->Add(Message);
  Message  = "If you are experiencing problems using your own IPF files, please contact me.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowRecentDiskImages(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Recent disk images");

  Message  = "Last disk images are still available to be loaded into Drive A.";
  RichEditHints->Lines->Add(Message);
  Message  = "At emulator start, only existing files are kept.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Disk > Drive A Load recent image";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowFavouriteDiskImages(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Favourite disk images");

  Message  = "Favourite disk images can be stored for later direct access.";
  RichEditHints->Lines->Add(Message);
  Message  = "At emulator start, only existing files are kept.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Disk > Drive A Load favourite image";
  RichEditHints->Lines->Add(Message);

  Message  = "To declare a disk image as favourite, use following menu:";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Disk > Drive A > Add to favourite\r\n";
  Message += "Menu: Disk > Drive B > Add to favourite\r\n";
  Message += "Drive popup menu: Add to favourite";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowRunTapeProgram(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Run program from a tape");

  Message  = "First of all, insert a tape image.";
  RichEditHints->Lines->Add(Message);
  Message  = "If you are using a 664 or 6128 machine, first type \"|TAPE\".";
  RichEditHints->Lines->Add(Message);
  Message  = "Start loading tape by command \"RUN\"\" or CONTROL+ENTER CPC keys. then press ENTER.";
  RichEditHints->Lines->Add(Message);
  Message  = "Press tape player PLAY button (windows CTRL+F5).";
  RichEditHints->Lines->Add(Message);
  Message  = "Both PLAY Button can be pressed and RUN\" command can be executed using Auto Play menu.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Tape > Auto Play";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------



void __fastcall TFormHints::ShowPlayAudioTape(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Play audio tape");

  Message  = "Caprice Forever allows to play WAV audio format.";
  RichEditHints->Lines->Add(Message);
  Message  = "Each time a WAV file is inserted, audio filtering is performed.";
  RichEditHints->Lines->Add(Message);
  Message  = "For 8 bits audio, there are no filters.";
  RichEditHints->Lines->Add(Message);
  Message  = "For 16 bits audio, following filters are available:";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Automatic Gain";
  RichEditHints->Lines->Add(Message);
  Message  = "  - First order Low-Pass filter";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Butterworth Low-Pass filter";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Butterworth High-Pass filter";
  RichEditHints->Lines->Add(Message);
  Message  = "Audio filtering is available here:";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Extra > Tape audio workshop";
  RichEditHints->Lines->Add(Message);
  Message  = "Audio WAV is then converted as CSW audio compressed format.";
  RichEditHints->Lines->Add(Message);
  Message  = "By checking \"Create CSW\", result CSW file is created where WAV file is located.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------



void __fastcall TFormHints::ShowParameters(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Parameters");

  Message  = "All following parameters can be found into \"Caprice.ini\" file. ";
  Message += "This file must be updated when Caprice Forever is not running. ";
  Message += "Otherwise, all your modifications may be discarded at emulator exit.";
  RichEditHints->Lines->Add(Message);
  Message  = "Most of parameters are only taken into account at emulator start.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Help! > Open Caprice folder";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowDefaultFolders(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Default folders");

  Message  = "Relative folders, such as default ones, have the executable folder as parent.";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Game controller profiles=Profiles\\";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Roms=Roms\\";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Cheat scripts=Scripts\\";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Scanlines patterns=Scanlines\\";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: ProfileDirectory=Folder";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: RomsDirectory=Folder";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowUseDirectX(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("DirectX graphic renderer");

  Message  = "By default, graphic renderer is DirectX 9. ";
  RichEditHints->Lines->Add(Message);
  Message  = "For some compatibility purposes, it is possible to switch to OpenGL renderer. ";
  Message += "There should not have any differences between OpenGL and DirectX.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: UseOpenGL=X (X=0 for DirectX, X=1 for OpenGL)";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowForceGDI(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("GDI graphic renderer");

  Message  = "By default, graphic renderer is DirectX 9. ";
  RichEditHints->Lines->Add(Message);
  Message  = "In case of black screen using OpenGL or DirectX, it is possible to switch to Windows GDI renderer. ";
  Message += "But GDI rendering is not so optimized.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: ForceGDI=X (X=0 for OpenGL or DirectX, X=1 to force GDI)";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowScanlinesPattern(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Scanlines patterns");

  Message  = "When using DirectX or OpenGL graphic rendering, scanlines pattern can be customized ";
  Message += "for better visual experience.";
  RichEditHints->Lines->Add(Message);

  Message  = "Into the scanlines patterns folder, file corresponding to the display size will be loaded ";
  Message += "each time emulator is resizing.";
  RichEditHints->Lines->Add(Message);

  Message  = "Pattern file contains customizable plain text description.";
  RichEditHints->Lines->Add(Message);

  Message  = "Display size can be found by entering Pause mode or into Debug section of About window.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Pause";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Help! > About...";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  Message  = "Send me your favourites patterns to add them with the next release!!";
  RichEditHints->Lines->Add(Message);
  Message  = "(mail address in \"About\" window)";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowGameControllers(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Game controllers");

  Message  = "Caprice Forever should be able to use any Gamepad or Joystick.";
  RichEditHints->Lines->Add(Message);
  Message  = "Game controllers must be inserted and recognized by Windows before running Caprice Forever. ";
  Message += "Then, you can access to controllers configuration. ";
  Message += "Caprice Forever allows to prepare a specific profile for each DSK loaded into drive A.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Inputs > Game controller settings...";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowPause(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Pause");

  Message  = "At any time, you can pause the emulator by pressing F1 or Pause key.";
  RichEditHints->Lines->Add(Message);
  Message  = "Emulator information texts can be removed from display.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Display > Display settings...";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowKeyboardMappings(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Keyboard mappings");

  Message  = "Caprice Forever proposes 2 different ways to emulate keyboard:";
  RichEditHints->Lines->Add(Message);
  Message  = "  - ASCII keyboard";
  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold;
  RichEditHints->Lines->Add(Message);
  Message  = "  - Hardware keyboard";
  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold;
  RichEditHints->Lines->Add(Message);
  Message  = "By choosing ASCII Keyboard, pressing Windows \"A\" key will always give an \"A\" for the CPC, regardless of Amstrad OS ROM. ";
  Message += "Same for all ASCII symbols such as \"1\", \"&\", \"|\"... ";
  Message += "As you need Windows Shift key to generate ASCII \"1\" or \"&\", other Windows keys must be defined to emulate CPC special keys (SHIFT, CTRL, COPY, ENTER...). ";
  Message += "ASCII keyboard is selected by default.";
  RichEditHints->Lines->Add(Message);
  Message  = "Hardware keyboard will associate key codes. ";
  Message += "In that case, the \"A\" Windows Key can becomes a \"Q\" or an \"A\", depending on Amstrad OS ROM. ";
  Message += "Specific Windows keys must also be defined to emulate CPC special keys (COPY, ENTER...).";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Inputs > Use hardware mapping";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Inputs > Keyboard mapping...";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowAudio(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Audio");

  Message  = "To stop/restart audio rendering, toggle mute menu or windows keyboard mute key.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Mute";
  RichEditHints->Lines->Add(Message);

  Message  = "Main volume can be adjusted using menu or windows keyboard volume keys.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Audio volume up";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Audio volume down";
  RichEditHints->Lines->Add(Message);

  Message  = "If you are experiencing audio problems, you may try to use another audio device.";
  RichEditHints->Lines->Add(Message);
  Message  = "It is possible to adjust audio gain for speaker, floppy drives, Tape deck ";
  Message += "and Techni-Musique speech synthesizer.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Audio settings...";
  RichEditHints->Lines->Add(Message);
  Message  = "It is possible to enable/disable any of the 3 PSG output channels by clicking on ";
  Message += "the respective channel label into the Extra PSG window.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowSiliconDisc(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("256Ko Silicon disc");

  Message  = "To emulate Silicon Disc, you have to enable the use of Silicon Disc AND have to add Silicon Disc ROM into an empty ROM location.";
  RichEditHints->Lines->Add(Message);
  Message  = "To use Silicon Disc, insert floppy disc into drive A.";
  RichEditHints->Lines->Add(Message);
  Message  = "Type \"|LOADDISC\" to transfer data from floppy disc to silicon disc.";
  RichEditHints->Lines->Add(Message);
  Message  = "Type \"|SDISC\" to initialise silicon disc interface after loading data.";
  RichEditHints->Lines->Add(Message);
  Message  = "Finally type \"|C\" to access silicon disc as normal drive.";
  RichEditHints->Lines->Add(Message);
  Message  = "Use \"|SAVEDISC\" to put data back from silicon disc to floppy disc.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold;
  Message  = "Beware: Silicon Disc content is deleted by a cold reset.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Settings...";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowFloppyIconSize(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Floppy Icon");

  Message  = "Adjust floppy icon size (in percentage).";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: DriveIconSizeFactor=X (X=0 to disable display)";
  RichEditHints->Lines->Add(Message);

  Message  = "Under DirectX or OpenGL, it is possible to display a static floppy disk image instead of a spinning disk.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: ShowStaticDriveIcon=X (X=1 for static)";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowOnscreenDisplay(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Onscreen Display");

  Message  = "Following items on the screen can be shown/hidden:";
  RichEditHints->Lines->Add(Message);
  Message  = " - Status icons";
  RichEditHints->Lines->Add(Message);
  Message  = " - Emulator messages";
  RichEditHints->Lines->Add(Message);
  Message  = " - Tape play";
  RichEditHints->Lines->Add(Message);
  Message  = " - Disk operations";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Display > Display settings...";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowDecathlon(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("\"Decathlon\" mode");

  Message  = "\"Decathlon\" mode is a Joystick Left/Right automatic toggle. ";
  Message += "While activated, Joystick Left and Right can be used to adjust toggle speed.";
  RichEditHints->Lines->Add(Message);
  
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Inputs > Decathlon mode";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowCombatSchool(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("\"Combat School\" mode");

  Message  = "\"Combat School\" mode is a Joystick Left/Up/Right/Down automatic sequence. ";
  Message += "While activated, Joystick Left and Right can be used to adjust sequence speed.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Inputs > Combat School mode";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowAutostart(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Autostart");

  Message  = "When drag/dropping Disk file (DSK or IPF), it is possible to automatically execute file.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Disk > Drive A Auto Run at Load";
  RichEditHints->Lines->Add(Message);

  Message  = "When drag/dropping Tape file (CDT or WAV), it is possible to automatically start tape playing.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Tape > Auto Play at Insert";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowBrightness(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Brightness");

  Message  = "Display brightness can be adjusted only in fullscreen.";
  RichEditHints->Lines->Add(Message);
  Message  = "Press F9 to decrease brightness.";
  RichEditHints->Lines->Add(Message);
  Message  = "Press F10 to increase brightness.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowSettingsOverride(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Settings override");

  Message  = "Prepared settings file with \".ini\" extension can be drag&dropped into Caprice.";
  Message += "Those files can only contain parameters that will be overriden, see following examples:";
  RichEditHints->Lines->Add(Message);
  RichEditHints->Lines->Add("");
  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold;
  Message  = "For 464:";
  RichEditHints->Lines->Add(Message);
  Message  = "[Emulator]\r\nDevice=CPC_464_UK\r\n64kMemoryExtension=0\r\nFloppyDrive=0";
  RichEditHints->Lines->Add(Message);
  Message  = "[Roms]\r\nUpperROM7=Empty";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold;
  Message  = "For 6128:";
  RichEditHints->Lines->Add(Message);
  Message  = "[Emulator]\r\nDevice=CPC_6128_UK\r\n64kMemoryExtension=0\r\nFloppyDrive=1";
  RichEditHints->Lines->Add(Message);
  Message  = "[Roms]\r\nUpperROM7=Default AMSDOS";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowScreenshot(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Screenshot");

  Message  = "768x544 pixels screenshot can be created at any time by pressing windows F12 key.";
  Message += " Screenshots do not include emulator onscreen displays.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Display > Screenshot";
  RichEditHints->Lines->Add(Message);
  Message  = "384x272 pixels small screenshots can be created to only show MODE 0 and Mode 1 displays.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Display > Small screenshot";
  RichEditHints->Lines->Add(Message);
  Message  = "Windows Printscreen key can be used to capture entire screen including emulator onscreen displays.";
  RichEditHints->Lines->Add(Message);
  Message  = "Windows key shortcut can be affected to small screenshot format.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: SmallScreenshotShortcut=X (X=1 for small screenshots, 0 otherwise)";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowWindowsPriority(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Windows priority");

  Message  = "If you are experiencing realtime issues, you can try to change Caprice Forever process priority to force Windows";
  Message += " to allocate more CPU and realtime reactivity.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: WindowsPriority=X (X=0, 1 or 2)";
  RichEditHints->Lines->Add(Message);
  Message  = "1 = HIGH PRIORITY PROCESS";
  RichEditHints->Lines->Add(Message);
  Message  = "2 = REALTIME PRIORITY PROCESS";
  RichEditHints->Lines->Add(Message);
  Message  = "Otherwise DEFAULT PRIORITY PROCESS";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowTurboSpeed(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Turbo speed");

  Message  = "Emulation speed can be modified by pressing F6 to activate/deactivate turbo.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Turbo";
  RichEditHints->Lines->Add(Message);
  Message  = "Several speeds are available:";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Full speed = Max CPU rendering (Display still at 50Hz)";
  RichEditHints->Lines->Add(Message);
  Message  = "  - 300% = 3 times quicker";
  RichEditHints->Lines->Add(Message);
  Message  = "  - 200% = 2 times quicker";
  RichEditHints->Lines->Add(Message);
  Message  = "  - Custom = User configurable speed";
  RichEditHints->Lines->Add(Message);
  Message  = "Please note that custom speed may not be reached (Custom speed > Full speed) !!";
  RichEditHints->Lines->Add(Message);
  RichEditHints->SelAttributes->Color = clGreen;
  Message  = "Menu: Emulator > Maximum speed";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowColdResetAtRealSpeed(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Cold Reset at Real speed");

  Message  = "It is possible to force Cold Reset to restore real CPC speed.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->SelAttributes->Color = clBlue;
  Message  = "Parameter: ColdResetAtRealSpeed=X (X=0 or 1)";
  RichEditHints->Lines->Add(Message);
  Message  = "0 = Cold reset without changing emulation speed.";
  RichEditHints->Lines->Add(Message);
  Message  = "1 = Cold reset and restore real CPC speed.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowCommandLine(void)
{
AnsiString Message;

  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Command line");

  Message  = "Caprice Forever can be executed from Windows command line (DOS).";
  RichEditHints->Lines->Add(Message);

  Message  = "To allow Drag/Drop files into Caprice Forever shortcut from Explorer, default formats are:";
  RichEditHints->Lines->Add(Message);
  Message  = ">Caprice.exe <DriveAFilename> <DriveBFilename>";
  RichEditHints->Lines->Add(Message);
  Message  = ">Caprice.exe <TapeFilename>";
  RichEditHints->Lines->Add(Message);
  Message  = ">Caprice.exe <SnapshotFilename>";
  RichEditHints->Lines->Add(Message);
  Message  = ">Caprice.exe <ROMFilename>";
  RichEditHints->Lines->Add(Message);
  Message  = "Extension of the file will determine which case.";
  RichEditHints->Lines->Add(Message);
  RichEditHints->Lines->Add("");
  Message  = "Optional parameters can be used to launch Caprice Forever from scripts:";
  RichEditHints->Lines->Add(Message);
  Message  = ">Caprice.exe /DriveA=\"Filename\" /DriveB=\"Filename\" /Tape=\"Filename\" /SNA=\"Filename\" /Settings=\"Filename\" /Command=\"BASIC command\" /Fullscreen /Windowed /KeyboardAsJoystick";
  RichEditHints->Lines->Add(Message);
  RichEditHints->Lines->Add("");
  Message  = "Parameters:";
  RichEditHints->Lines->Add(Message);
  Message  = "/DriveA: disk image filename to be inserted into drive A.";
  RichEditHints->Lines->Add(Message);
  Message  = "/DriveB: disk image filename to be inserted into drive B.";
  RichEditHints->Lines->Add(Message);
  Message  = "/Tape: tape image filename to be inserted into cassette player.";
  RichEditHints->Lines->Add(Message);
  Message  = "/SNA: snapshot filename to be executed at emulator start.";
  RichEditHints->Lines->Add(Message);
  Message  = "/Settings: config filename to override emulator settings.";
  RichEditHints->Lines->Add(Message);
  Message  = "/Command: BASIC command to be executed at emulator start.";
  Message += " Add 2 double quotes symbols into script to add double quotes into command, e.g.: /Command=\"RUN\"\"DISC.BIN\"";
  RichEditHints->Lines->Add(Message);
  Message  = "/Fullscreen: to start in fullscreen display mode.";
  RichEditHints->Lines->Add(Message);
  Message  = "/Windowed: to start in windowed display mode.";
  RichEditHints->Lines->Add(Message);
  Message  = "/KeyboardAsJoystick: to start with keyboard as joystick input mode activated.";
  RichEditHints->Lines->Add(Message);
  Message  = "/Turbo: to start with turbo mode activated.";
  RichEditHints->Lines->Add(Message);
  Message  = "/Script: cheat script filename to be executed at startup.";
  RichEditHints->Lines->Add(Message);
  Message  = "/Quiet: to prevent popups display.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowDrivePopup(void)
{
  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Drive popup menu");

  AnsiString Message  = "Drive functions are directly available by mouse right-clicking on last FDC operation panel.";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::ShowShortcuts(void)
{
  RichEditHints->SelAttributes->Style = RichEditHints->SelAttributes->Style << fsBold << fsUnderline;
  RichEditHints->Lines->Add("Shortcuts");

  AnsiString Message  = "List of shortcuts available from Windows keyboard:";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");

  Message  = "F1/Pause: Pause";
  RichEditHints->Lines->Add(Message);
  Message  = "F2: Drive A Disk load";
  RichEditHints->Lines->Add(Message);
  Message  = "F3: Drive B Disk load";
  RichEditHints->Lines->Add(Message);
  Message  = "F4: Drives Swap";
  RichEditHints->Lines->Add(Message);
  Message  = "F5: Drive A Catalog";
  RichEditHints->Lines->Add(Message);
  Message  = "F6: Speed Turbo / Real";
  RichEditHints->Lines->Add(Message);
  Message  = "F7: Audio volume DOWN";
  RichEditHints->Lines->Add(Message);
  Message  = "F8: Audio volume UP";
  RichEditHints->Lines->Add(Message);
  Message  = "F9: Display Reduce / Brightness DOWN";
  RichEditHints->Lines->Add(Message);
  Message  = "F10: Display Enlarge / Brightness UP";
  RichEditHints->Lines->Add(Message);
  Message  = "F11: Display Fullscreen";
  RichEditHints->Lines->Add(Message);
  Message  = "F12: Screenshot";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");

  Message  = "Shift+F1: Hints";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F2: \"Decathlon mode\"";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F3: \"Combat School\" mode";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F4: Drive A Sides Flip";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F5: Keyboard as Joystick";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F6: Mouse as Lightpen";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F7: Mouse as Magnum Light Phaser";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F8: Mouse as Gunstick";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F9: Mouse as West Phaser";
  RichEditHints->Lines->Add(Message);
  Message  = "Shift+F10: Mouse as AMX Mouse";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");

  Message  = "Ctrl+F1: Snapshot Load";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F2: Snapshot Reload";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F3: Snapshot Unload";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F4: Snapshot Update";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F5: Tape Play/Stop";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F6: Tape Insert";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F7: Tape Go to previous Tag";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F8: Tape Go to next Tag";
  RichEditHints->Lines->Add(Message);
  Message  = "Ctrl+F12: Cold Reset";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");

  Message  = "Alt+F4: Quit";
  RichEditHints->Lines->Add(Message);

  RichEditHints->Lines->Add("");
  RichEditHints->Lines->Add("");
}
//---------------------------------------------------------------------------


void __fastcall TFormHints::FormKeyPress(TObject *Sender, char &Key)
{
	ModalResult = mrOk;

  NOT_USED(Sender);
  NOT_USED(Key);
}
//---------------------------------------------------------------------------

