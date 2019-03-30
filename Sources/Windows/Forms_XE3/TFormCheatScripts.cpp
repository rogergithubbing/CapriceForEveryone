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
#include "TFormCheatScripts.h"
#include "TFormMain.h"
#include "..\Cheat\TCheatActionKeyDown.h"
#include "..\Cheat\TCheatActionKeyUp.h"
#include "..\Cheat\TCheatActionWait.h"
#include "..\Cheat\TCheatActionPoke.h"
#include "..\Cheat\TCheatActionExit.h"
#include "..\Cheat\TCheatActionDiff.h"
#include "..\Cheat\TCheatActionEqual.h"
#include "..\Cheat\TCheatActionSwapDrives.h"
#include "..\Cheat\TCheatActionFlipSides.h"
#include "..\Cheat\TCheatActionPause.h"
#include "..\Cheat\TCheatActionEnd.h"
#include "..\Cheat\TCheatActionDriveA.h"
#include "..\Cheat\TCheatActionDriveB.h"
#include "..\Cheat\TCheatActionScreenDiff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


#define SCRIPTS_FILTER              "Cheat scripts (.script)|*.script|"

#define SCRIPT_EXT                  "*.script"


// Actions
#define ACTION_KEYDOWN              "keydown="
#define ACTION_KEYUP                "keyup="
#define ACTION_WAIT                 "wait="
#define ACTION_POKE                 "poke="
#define ACTION_EXIT                 "exit="
#define ACTION_DIFF                 "diff="
#define ACTION_EQUAL                "equal="
#define ACTION_SWAPDRIVES           "swapdrives"
#define ACTION_FLIPSIDES            "flipsides"
#define ACTION_PAUSE                "pause"
#define ACTION_END                  "end"
#define ACTION_DRIVEA               "drivea="
#define ACTION_DRIVEB               "driveb="
#define ACTION_SCREENDIFF           "screendiff="


// Instructions
static _TCHAR Instructions[] =
	_T("- Press a key:\r\n")
	_T("Command: keydown=XX\r\n")
	_T("with XX the key code\r\n")
	_T("\r\n")
	_T("- Release a key:\r\n")
	_T("Command: keyup=XX\r\n")
	_T("with XX the key code or \"all\" for all keys \r\n")
	_T("\r\n")
	_T("- Wait a while:\r\n")
	_T("Command: wait=XX\r\n")
	_T("with XX duration in ms\r\n")
	_T("\r\n")
	_T("- Write a byte in RAM:\r\n")
	_T("Command: poke=XXXX,YY\r\n")
	_T("with XX the address in decimal (use & for hexa).\r\n")
	_T("with YY the value in decimal (use & for hexa).\r\n")
	_T("\r\n")
	_T("- Test byte in RAM and branch to label if different:\r\n")
	_T("- Test byte in RAM and branch to label if equal:\r\n")
	_T("Command: diff=XXXX,YY,ZZ:label\r\n")
	_T("Command: equal=XXXX,YY,ZZ:label\r\n")
	_T("with XX the address in decimal (use & for hexa).\r\n")
	_T("with YY the value in decimal (use & for hexa).\r\n")
	_T("with ZZ the mask in decimal (use & for hexa).\r\n")
	_T("\r\n")
	_T("- Pause emulator:\r\n")
	_T("Command: pause\r\n")
	_T("Script is paused while emulator is paused.\r\n")
	_T("\r\n")
	_T("- Terminate script:\r\n")
	_T("Command: end\r\n")
	_T("\r\n")
	_T("- Swap drives A and B:\r\n")
	_T("Command: swapdrives\r\n")
	_T("\r\n")
	_T("- Flip sides of drive A:\r\n")
	_T("Command: flipsides\r\n")
	_T("\r\n")
	_T("- Load disk into drive A:\r\n")
	_T("- Load disk into drive B:\r\n")
	_T("Command: drivea=filename\r\n")
	_T("Command: driveb=filename\r\n")
	_T("Disk image should be located into default disk folder.\r\n")
	_T("\r\n")
	_T("Label are defined using ':' before label title in a single line.\r\n")
	_T("\r\n")
	_T("\r\n")
	_T("Script example to press and release SHIFT + 'Q' key\r\n")
	_T("KeyDown=21\r\n")
	_T("KeyDown=67\r\n")
	_T("Wait=100\r\n")
	_T("KeyUp=all\r\n")
	_T("\r\n")
	_T("All actions are performed at once until an action is expecting some time.\r\n")
	_T("\r\n")
	_T("Comment can be added using ';' to start comment until end of line.\r\n")
	_T("\r\n");


TFormCheatScripts *FormCheatScripts;
//---------------------------------------------------------------------------
__fastcall TFormCheatScripts::TFormCheatScripts(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mListActions = new TList();

  MemoInstructions->Text = Instructions;

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::FormDestroy(TObject *Sender)
{
  // Reset list of actions
  while (mListActions->Count)
  {
    delete ((TCheatAction*)mListActions->Items[0]);
    mListActions->Delete(0);
  }
  delete mListActions;

  if (FormMain)
  {
    FormMain->UnsubscribeToCPCEvents(OnCPCEvent);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  ShowHint = mSettingsP->ShowHints;

  if (mSettingsP)
  {
    // Apply theme
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
    LabelSelectedScript->Font->Color = mSettingsP->ThemeText2Color;
    ListBoxScripts->Color = mSettingsP->ThemeBackgroundColor;
    ListBoxScripts->Font->Color = mSettingsP->ThemeText1Color;
    MemoInstructions->Font->Color = mSettingsP->ThemeText1Color;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  if (ButtonExecute->Enabled == true)
  {
    UpdateScriptsList();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::UpdateScriptsList(void)
{
  AnsiString Path = mSettingsP->CheatScriptsDirectory;

  TStringList* FileList = new TStringList();

  // First add folders
  TSearchRec sr;
  /*if (FindFirst(Path + "*.*", faDirectory, sr) == 0)
  {
    do
    {
      if (sr.Attr & faDirectory)
      {
        if (sr.Name != ".")
        {
          FileList->Add("\\" + sr.Name);
        }
      }
    }
    while (FindNext(sr) == 0);
    FindClose(sr);
  }*/
  // Add all supported script files
  if (FindFirst(Path + SCRIPT_EXT, faAnyFile, sr) == 0)
  {
    do
    {
      FileList->Add(sr.Name);
    }
    while (FindNext(sr) == 0);
    FindClose(sr);
  }

  // Display scripts files
  FileList->Sort();
  ListBoxScripts->Clear();
  ListBoxScripts->Items->AddStrings(FileList);

  // Select first script
  if (ListBoxScripts->Items->Count)
  {
    ListBoxScripts->ItemIndex = 0;
    LoadScript(ListBoxScripts->Items->Strings[ListBoxScripts->ItemIndex]);
    ButtonDelete->Enabled = true;
    PageControl->ActivePage = TabSheetScripts;
  }
  else
  {
    ButtonDelete->Enabled = false;
    PageControl->ActivePage = TabSheetCode;
    mSelectedScriptFilename = "";
    LabelSelectedScript->Caption = "";
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::LoadScript(AnsiString Filename)
{
  AnsiString FullFilename = mSettingsP->CheatScriptsDirectory + Filename;

  MemoCode->Lines->LoadFromFile(FullFilename);
  mSelectedScriptFilename = FullFilename;

  // Update "Selected script"
  LabelSelectedScript->Caption = Filename;
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ButtonNewClick(TObject *Sender)
{
  // Select Code page
  PageControl->ActivePage = TabSheetCode;

  ListBoxScripts->ItemIndex = -1;

  MemoCode->Clear();
  MemoCode->SetFocus();

  // Update "Selected script"
  ButtonDelete->Enabled = false;
  mSelectedScriptFilename = "";
  LabelSelectedScript->Caption = "";

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ButtonSaveClick(TObject *Sender)
{
  SaveDialog->InitialDir = mSettingsP->CheatScriptsDirectory;
  SaveDialog->Filter = SCRIPTS_FILTER;
  SaveDialog->DefaultExt = SCRIPT_EXT;
  SaveDialog->FileName = (mSelectedScriptFilename.IsEmpty() == false) ? mSelectedScriptFilename : AnsiString("New");

  if (SaveDialog->Execute() == true)
  {
    MemoCode->Lines->SaveToFile(SaveDialog->FileName);
    UpdateScriptsList();
    
    // Select filename
    ListBoxScripts->ItemIndex = ListBoxScripts->Items->IndexOf(ExtractFileName(SaveDialog->FileName));
    LoadScript(ListBoxScripts->Items->Strings[ListBoxScripts->ItemIndex]);
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ButtonDeleteClick(TObject *Sender)
{
  if (Application->MessageBox(_T("Are you sure to delete selected file ?"),
                              _T("Delete script"),
                              MB_YESNO | MB_DEFBUTTON2) == ID_YES)
  {
    DeleteFile(mSelectedScriptFilename);
    UpdateScriptsList();
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ButtonExecuteClick(TObject *Sender)
{
  // Reset list of actions
  while (mListActions->Count)
  {
    delete ((TCheatAction*)mListActions->Items[0]);
    mListActions->Delete(0);
  }
  mListActions->Clear();

  if (PageControl->ActivePage == TabSheetScripts)
  {
    // Load script
    AnsiString Filename = mSettingsP->CheatScriptsDirectory
                          + ListBoxScripts->Items->Strings[ListBoxScripts->ItemIndex];

    MemoCode->Clear();

    if (FileExists(Filename) == true)
    {
      MemoCode->Lines->LoadFromFile(Filename);
    }
  }

  //
  // Prepare list of actions
  //
  AnsiString Label = NULL;
  for (int Loop=0; Loop < MemoCode->Lines->Count; Loop++)
  {
    AnsiString Command = MemoCode->Lines->Strings[Loop].Trim();
    AnsiString LowerCommand = Command.LowerCase();

    TCheatAction* NewAction;

    // LABEL
    if (Command.Pos(":") == 1)
    {
      Label = Command.SubString(2, Command.Length()-1);
    }
    // KEY DOWN Action
    else if (LowerCommand.Pos(ACTION_KEYDOWN))
    {
      NewAction = (TCheatAction*) new TCheatActionKeyDown(Command);
    }
    // KEY UP Action
    else if (LowerCommand.Pos(ACTION_KEYUP))
    {
      NewAction = (TCheatAction*) new TCheatActionKeyUp(Command);
    }
    // WAIT Action
    else if (LowerCommand.Pos(ACTION_WAIT))
    {
      NewAction = (TCheatAction*) new TCheatActionWait(Command);
    }
    // POKE Action
    else if (LowerCommand.Pos(ACTION_POKE))
    {
      NewAction = (TCheatAction*) new TCheatActionPoke(Command);
    }
    // EXIT Action
    else if (LowerCommand.Pos(ACTION_EXIT))
    {
      NewAction = (TCheatAction*) new TCheatActionExit(Command);
    }
    // SCREENDIFF Action
    else if (LowerCommand.Pos(ACTION_SCREENDIFF))
    {
      NewAction = (TCheatAction*) new TCheatActionScreenDiff(Command);
    }
    // DIFF Action
    else if (LowerCommand.Pos(ACTION_DIFF))
    {
      NewAction = (TCheatAction*) new TCheatActionDiff(Command);
    }
    // EQUAL Action
    else if (LowerCommand.Pos(ACTION_EQUAL))
    {
      NewAction = (TCheatAction*) new TCheatActionEqual(Command);
    }
    // SWAPDRIVES Action
    else if (LowerCommand.Pos(ACTION_SWAPDRIVES))
    {
      NewAction = (TCheatAction*) new TCheatActionSwapDrives(Command);
    }
    // FLIPSIDES Action
    else if (LowerCommand.Pos(ACTION_FLIPSIDES))
    {
      NewAction = (TCheatAction*) new TCheatActionFlipSides(Command);
    }
    // PAUSE Action
    else if (LowerCommand.Pos(ACTION_PAUSE))
    {
      NewAction = (TCheatAction*) new TCheatActionPause(Command);
    }
    // END Action
    else if (LowerCommand.Pos(ACTION_END))
    {
      NewAction = (TCheatAction*) new TCheatActionEnd();
    }
    // DRIVEA Action
    else if (LowerCommand.Pos(ACTION_DRIVEA))
    {
      NewAction = (TCheatAction*) new TCheatActionDriveA(Command);
    }
    // DRIVEB Action
    else if (LowerCommand.Pos(ACTION_DRIVEB))
    {
      NewAction = (TCheatAction*) new TCheatActionDriveB(Command);
    }
    else
    {
      NewAction = NULL;
    }

    // Only add valid action
    if (NewAction)
    {
      if (NewAction->Valid == true)
      {
        mListActions->Add((void*)NewAction);
        // Affect Label
        NewAction->Label = Label;
        Label = NULL;
      }
      else
      {
        delete NewAction;
      }
    }
  }

  // Add empty action for last label
  if (Label != NULL)
  {
    TCheatAction* NewAction = new TCheatAction();
    NewAction->Label = Label;
    mListActions->Add((void*)NewAction);
  }

  if (mListActions->Count)
  {
    // Start execution
    mCurrentActionIndex = 0;
    TCheatAction* Action = (TCheatAction*)mListActions->Items[mCurrentActionIndex];
    Action->Start(FormMain->NativeCPC);

    FormMain->SubscribeToCPCEvents(OnCPCEvent);

    ButtonExecute->Enabled = false;
  }

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ButtonStopClick(TObject *Sender)
{
  // Reset list of actions
  while (mListActions->Count)
  {
    delete ((TCheatAction*)mListActions->Items[0]);
    mListActions->Delete(0);
  }
  mListActions->Clear();

  ButtonExecute->Enabled = true;

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


bool __fastcall TFormCheatScripts::OnCPCEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;

  if ((Condition & EC_CYCLE_COUNT) == 0)
    return Pause;

  if (mListActions->Count == 0)
    return Pause;

  if (mCurrentActionIndex >= mListActions->Count)
    return Pause;

  // Execute actions
  TCheatAction* Action = (TCheatAction*)mListActions->Items[mCurrentActionIndex];
  while (Action != NULL)
  {
    Action->Execute(NativeCPC);
    if (Action->IsFinished() == true)
    {
      // By default, Go to next action
      mCurrentActionIndex++;

      // Jump to another action ?      
      if (Action->NextAction() != NULL)
      {
        AnsiString NextActionLabel = Action->NextAction();
        for (int Loop=0; Loop < mListActions->Count; Loop++)
        {
          TCheatAction* NextAction = (TCheatAction*)mListActions->Items[Loop];
          if (NextAction->Label == NextActionLabel)
          {
            // Jump to this action
            mCurrentActionIndex = Loop;
            break;
          }
        }
      }

      // Still actions ?
      if ( (mCurrentActionIndex < mListActions->Count)
           && (Action->IsScriptTerminated() == false) )
      {
        // Start next action
        Action = (TCheatAction*)mListActions->Items[mCurrentActionIndex];
        Action->Start(FormMain->NativeCPC);
      }
      else
      {
        // Terminate execution
        FormMain->UnsubscribeToCPCEvents(OnCPCEvent);
        ButtonExecute->Enabled = true;
        Action = NULL;
      }
    }
    else
    {
      Action = NULL;
    }
  }

  return Pause;
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ListBoxScriptsClick(TObject *Sender)
{
  LoadScript(ListBoxScripts->Items->Strings[ListBoxScripts->ItemIndex]);

  NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ListBoxScriptsDblClick(TObject *Sender)
{
  LoadScript(ListBoxScripts->Items->Strings[ListBoxScripts->ItemIndex]);
  ButtonExecute->OnClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::ExecuteScript(AnsiString Filename)
{
  UpdateScriptsList();
  ListBoxScripts->ItemIndex = ListBoxScripts->Items->IndexOf(Filename);
  if (ListBoxScripts->ItemIndex != -1)
  {
    LoadScript(ListBoxScripts->Items->Strings[ListBoxScripts->ItemIndex]);
    ButtonExecute->OnClick(NULL);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormCheatScripts::StopCurrentScript(void)
{
  // Terminate execution
  FormMain->UnsubscribeToCPCEvents(OnCPCEvent);
}
//---------------------------------------------------------------------------

void __fastcall TFormCheatScripts::PageControlDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = PageControl->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }

  // Get sheet corresponding to TabIndex
  int TabCount=0;
  TTabSheet* Sheet = NULL;
  for (int Loop=0; Loop < PageControl->PageCount; Loop++)
  {
    if (PageControl->Pages[Loop]->TabVisible == true)
    {
      if (TabCount == TabIndex)
      {
        Sheet = PageControl->Pages[Loop];
        break;
      }

      TabCount++;
    }
  }

  // Draw tab
  Canvas->FillRect(Rect);
  AnsiString Text = Sheet->Caption;
  int TextWidth = Canvas->TextWidth(Text);
  int TextHeight = Canvas->TextHeight(Text);
  int TextPosX = (Rect.Width() - TextWidth) / 2;
  int TextPosY = (PageControl->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

  NOT_USED(Control);
}
//---------------------------------------------------------------------------

