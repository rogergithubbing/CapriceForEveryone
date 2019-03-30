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


#ifndef TFormHintsH
#define TFormHintsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
//---------------------------------------------------------------------------
class TFormHints : public TForm
{
__published:	// Composants gérés par l'EDI
  TRichEdit *RichEditHints;
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  
private:	// Déclarations utilisateur
  //
  // Methods
  //
  void __fastcall ShowTitle(void);
  void __fastcall ShowForeword(void);
  void __fastcall ShowHints(void);
  void __fastcall ShowRunDiscProgram(void);
  void __fastcall ShowDriveACatalog(void);
  void __fastcall ShowConcerningIPF(void);
  void __fastcall ShowRecentDiskImages(void);
  void __fastcall ShowFavouriteDiskImages(void);
  void __fastcall ShowRunTapeProgram(void);
  void __fastcall ShowPlayAudioTape(void);
  void __fastcall ShowParameters(void);
  void __fastcall ShowDefaultFolders(void);
  void __fastcall ShowUseDirectX(void);
  void __fastcall ShowForceGDI(void);
  void __fastcall ShowScanlinesPattern(void);
  void __fastcall ShowGameControllers(void);
  void __fastcall ShowPause(void);
  void __fastcall ShowKeyboardMappings(void);
  void __fastcall ShowAudio(void);
  void __fastcall ShowSiliconDisc(void);
  void __fastcall ShowFloppyIconSize(void);
  void __fastcall ShowOnscreenDisplay(void);
  void __fastcall ShowDecathlon(void);
  void __fastcall ShowCombatSchool(void);
  void __fastcall ShowAutostart(void);
  void __fastcall ShowBrightness(void);
  void __fastcall ShowSettingsOverride(void);
  void __fastcall ShowScreenshot(void);
  void __fastcall ShowWindowsPriority(void);
  void __fastcall ShowTurboSpeed(void);
  void __fastcall ShowColdResetAtRealSpeed(void);
  void __fastcall ShowCommandLine(void);
  void __fastcall ShowDrivePopup(void);
  void __fastcall ShowShortcuts(void);

public:		// Déclarations utilisateur
  //
  // API
  //
  __fastcall TFormHints(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHints *FormHints;
//---------------------------------------------------------------------------
#endif
