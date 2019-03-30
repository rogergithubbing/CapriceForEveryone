/*
    CaPriCe Forever - Amstrad CPC 464/664/6128 emulator
    Copyright (C) 2014-2015 by Frédéric Coste

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
#include "TFormGameControllerSettings.h"
#include "TFormEditKeycode.h"
#include "..\Inputs\TInputContext.h"
#include "..\Inputs\Keyboard.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


#undef DISABLE_ANALOG_MOUSE
//#define DISABLE_ANALOG_MOUSE


const AnsiString POVButtonsLabel[] =
{
  "Up",
  "Down",
  "Left",
  "Right"
};

const AnsiString AnalogAxis[] =
{
  "X",
  "Y",
  "Z",
  "Rx",
  "Ry",
  "Rz",
  "Slider"
};

const AnsiString AnalogButtonsLabel[] =
{
  "Max",
  "Min"
};

const AnsiString ButtonMouseLabel = "MOUSE"; 


TFormGameControllerSettings *FormGameControllerSettings;
//---------------------------------------------------------------------------
__fastcall TFormGameControllerSettings::TFormGameControllerSettings(TComponent* Owner)
  : TForm(Owner)
{
  mInputContextP = NULL;
  mSettingsP = NULL;

#ifdef DISABLE_ANALOG_MOUSE
  CheckBoxAnalogMouseX->Visible = false;
  CheckBoxAnalogMouseY->Visible = false;
#endif /* DISABLE_ANALOG_MOUSE */
}
//---------------------------------------------------------------------------


//
// Write access
//
void __fastcall TFormGameControllerSettings::SetCurrentProfile(AnsiString Value)
{
  mCurrentProfile = Value;
}
//---------------------------------------------------------------------------
void __fastcall TFormGameControllerSettings::SetDefaultProfile(AnsiString Value)
{
  mDefaultProfile = Value;
}
//---------------------------------------------------------------------------
void __fastcall TFormGameControllerSettings::SetDriveAProfile(AnsiString Value)
{
  mDriveAProfile = Value;
}
//---------------------------------------------------------------------------
void __fastcall TFormGameControllerSettings::SetDriveBProfile(AnsiString Value)
{
  mDriveBProfile = Value;
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::FormShow(TObject *Sender)
{
  // Valid input context ?
  if ( (mInputContextP == NULL) || (mSettingsP == NULL) )
  {
    ModalResult = mrCancel;
    return;
  }

  ShowHint = mSettingsP->ShowHints;

  // Apply Theme
  Color = mSettingsP->ThemeBackgroundColor;
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  Font->Color = mSettingsP->ThemeText1Color;
  ComboBoxProfiles->Font->Color = clBlack;
  ComboBoxAnalogAxis->Font->Color = clBlack;
  ComboBoxControllersList->Font->Color = clBlack;
  StringGridButtons->FixedColor = Color;
  StringGridPOV->FixedColor = Color;
  StringGridAnalog->FixedColor = Color;
  LabelSaveProfile->Font->Color = mSettingsP->ThemeText2Color;

  ButtonApply->Enabled = false;
   
  // Add game controllers
  ComboBoxControllersList->Clear();
  for (int loop=0; loop < mInputContextP->NbGameControllers; loop++)
  {
    ComboBoxControllersList->Items->Add(mInputContextP->GetDeviceName(loop));
  }
  ComboBoxControllersList->ItemIndex = 0;
  ComboBoxControllersList->Enabled = (ComboBoxControllersList->Items->Count == 1) ? false : true;

  // Add dummy actions
  mInputContextP->AddDummyActions();

  // Display device capabilities
  ComboBoxControllersListChange(Sender);

  // Restart game controllers
  mInputContextP->Start(Handle);

  // Display current profile
  PanelFromProfile->Caption = mCurrentProfile;

  // Init profile list
  ComboBoxProfiles->Clear();
  ComboBoxProfiles->Items->Add(mDefaultProfile);
  if (mDriveAProfile.Length())
  {
    ComboBoxProfiles->Items->Add(mDriveAProfile);
  }
  if (mDriveBProfile.Length())
  {
    ComboBoxProfiles->Items->Add(mDriveBProfile);
  }
  // Select current profile
  ComboBoxProfiles->ItemIndex = ComboBoxProfiles->Items->IndexOf(mCurrentProfile);
  if (ComboBoxProfiles->ItemIndex == -1)
  {
    // Select default profile
    ComboBoxProfiles->ItemIndex = 0;
  }

  Application->OnIdle = OnIdle;
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  // Stop game controllers
  mInputContextP->Stop();
  // Remove dummy actions
  mInputContextP->DeleteDummyActions();

	NOT_USED(Sender);
	NOT_USED(Action);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::ButtonPropertiesClick(
      TObject *Sender)
{
  // Call windows properties dialog
  mInputContextP->ShowProperties(Handle, ComboBoxControllersList->ItemIndex);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormGameControllerSettings::GetSelectedProfile(void)
{
  return ComboBoxProfiles->Items->Strings[ComboBoxProfiles->ItemIndex];
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::ComboBoxControllersListChange(
      TObject *Sender)
{
  // Get selected controller objects
  mSelectedDeviceP = mInputContextP->GetDevice(ComboBoxControllersList->ItemIndex);

#ifdef _USE_DIRECTINPUT8

  PageControlActions->ActivePage = NULL;

  //
  // Buttons
  //
  if (mSelectedDeviceP->NbButtons > 0)
  {
    if (PageControlActions->ActivePage == NULL)
    {
      PageControlActions->ActivePage = TabSheetButtons;
    }
    TabSheetButtons->TabVisible = true;
    StringGridButtons->RowCount = mSelectedDeviceP->NbButtons;
    StringGridButtons->ColWidths[0] = 60;
    StringGridButtons->ColWidths[1] = StringGridButtons->Width - StringGridButtons->ColWidths[0];
    for (int loop=0; loop < mSelectedDeviceP->NbButtons; loop++)
    {
      if (loop < 9)
      {
        StringGridButtons->Cells[0][loop] = " " + AnsiString(loop+1);
      }
      else
      {
        StringGridButtons->Cells[0][loop] = AnsiString(loop+1);
      }
      if (mSelectedDeviceP->ActionsP->Buttons[loop] != NULL)
      {
        if (mSelectedDeviceP->ActionsP->Buttons[loop]->MouseActive == true)
        {
          StringGridButtons->Cells[1][loop] = ButtonMouseLabel;
        }
        else
        {
          TInputKey* ButtonKeyP = mSelectedDeviceP->ActionsP->Buttons[loop]->ButtonKey;
          StringGridButtons->Cells[1][loop] = mInputContextP->GetPressedKeyString(ButtonKeyP);
        }
      }
      else
      {
        StringGridButtons->Cells[1][loop] = "";
      }
    }
  }
  else
  {
    TabSheetButtons->TabVisible = false;
  }

  //
  // POV
  //
  if (mSelectedDeviceP->NbPOV > 0)
  {
    if (PageControlActions->ActivePage == NULL)
    {
      PageControlActions->ActivePage = TabSheetPOV;
    }
    TabSheetPOV->TabVisible = true;
    StringGridPOV->ColWidths[0] = 60;
    StringGridPOV->ColWidths[1] = StringGridPOV->Width - StringGridPOV->ColWidths[0];

    // Add POVs
    ComboBoxPOVId->Clear();
    for (int loop=0; loop < mSelectedDeviceP->NbPOV; loop++)
    {
      ComboBoxPOVId->Items->Add(AnsiString(loop+1));
    }
    // Select first POV
    ComboBoxPOVId->ItemIndex = 0;
    ComboBoxPOVId->Enabled = (mSelectedDeviceP->NbPOV > 1) ? true : false;

    ComboBoxPOVIdChange(Sender);
  }
  else
  {
    TabSheetPOV->TabVisible = false;
  }

  //
  // Analogs
  //
  mSelectedAnalogP = NULL;
  ComboBoxAnalogAxis->Clear();
  if (mSelectedDeviceP->XAxisEnable == true) ComboBoxAnalogAxis->Items->Add(AnalogAxis[ANALOG_XAXIS_INDEX]);
  if (mSelectedDeviceP->YAxisEnable == true) ComboBoxAnalogAxis->Items->Add(AnalogAxis[ANALOG_YAXIS_INDEX]);
  if (mSelectedDeviceP->ZAxisEnable == true) ComboBoxAnalogAxis->Items->Add(AnalogAxis[ANALOG_ZAXIS_INDEX]);
  if (mSelectedDeviceP->RxAxisEnable == true) ComboBoxAnalogAxis->Items->Add(AnalogAxis[ANALOG_RXAXIS_INDEX]);
  if (mSelectedDeviceP->RyAxisEnable == true) ComboBoxAnalogAxis->Items->Add(AnalogAxis[ANALOG_RYAXIS_INDEX]);
  if (mSelectedDeviceP->RzAxisEnable == true) ComboBoxAnalogAxis->Items->Add(AnalogAxis[ANALOG_RZAXIS_INDEX]);
  for (int loop=0; loop < mSelectedDeviceP->NbSliders; loop++)
  {
    AnsiString Text = AnalogAxis[ANALOG_SLIDER1_INDEX] + AnsiString(loop+1);
    ComboBoxAnalogAxis->Items->Add(Text);
  }
  if (ComboBoxAnalogAxis->Items->Count > 0)
  {
    if (PageControlActions->ActivePage == NULL)
    {
      PageControlActions->ActivePage = TabSheetAnalog;
    }
    TabSheetAnalog->TabVisible = true;
    StringGridAnalog->ColWidths[0] = 60;
    StringGridAnalog->ColWidths[1] = StringGridAnalog->Width - StringGridAnalog->ColWidths[0];
    // Select first Analog
    ComboBoxAnalogAxis->ItemIndex = 0;
    ComboBoxAnalogAxis->Enabled = (ComboBoxAnalogAxis->Items->Count > 1) ? true : false;
    ComboBoxAnalogAxisChange(Sender);
  }
  else
  {
    TabSheetAnalog->TabVisible = false;
  }

#endif /* _USE_DIRECTINPUT8 */
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::ComboBoxControllersListDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxControllersList->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxControllersList->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxControllersList->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

	NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::FormResize(TObject *Sender)
{
  // Resize columns
  StringGridButtons->ColWidths[1] = StringGridButtons->Width - StringGridButtons->ColWidths[0];
  StringGridPOV->ColWidths[1] = StringGridPOV->Width - StringGridPOV->ColWidths[0];
  StringGridAnalog->ColWidths[1] = StringGridAnalog->Width - StringGridAnalog->ColWidths[0];

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::OnIdle(TObject *Sender, bool &Done)
{
  Done = false;

  // Update Game Controller inputs
  if (mInputContextP->UpdateGameControllers() == true)
  {
    StringGridButtons->Invalidate();
    StringGridPOV->Invalidate();
    StringGridAnalog->Invalidate();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::StringGridButtonsDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
  if (mSelectedDeviceP->ActionsP->Buttons[ARow] != NULL)
  {
    // Fixed column ?
    if (ACol == 0)
    {
      StringGridButtons->Canvas->Brush->Color = Color;
      StringGridButtons->Canvas->Font->Color = mSettingsP->ThemeText2Color;
    }
    else if (mSelectedDeviceP->ActionsP->Buttons[ARow]->ButtonKey->Pressed == true)
    {
      StringGridButtons->Canvas->Brush->Color = clRed;
      StringGridButtons->Canvas->Font->Color = clWhite;
    }
    else
    {
      if (State.Contains(gdSelected))
      {
        StringGridButtons->Canvas->Brush->Color = clHighlight;
        StringGridButtons->Canvas->Font->Color = clWhite;
      }
      else
      {
        StringGridButtons->Canvas->Brush->Color = Color;
        StringGridButtons->Canvas->Font->Color = Font->Color;
      }
    }

    // Draw cell
    StringGridButtons->Canvas->FillRect(Rect);
    int TextHeight = StringGridButtons->Canvas->TextHeight(StringGridButtons->Cells[ACol][ARow]);
    int TextPos = (StringGridButtons->RowHeights[ARow] - TextHeight) / 2;
    StringGridButtons->Canvas->TextOut(Rect.Left+5,
                                       Rect.Top+TextPos,
                                       StringGridButtons->Cells[ACol][ARow]);
    if (State.Contains(gdFocused))
    {
      StringGridButtons->Canvas->DrawFocusRect(Rect);
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormGameControllerSettings::StringGridPOVDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
  TInputActionPOV* POV = mSelectedDeviceP->ActionsP->POVs[ComboBoxPOVId->ItemIndex];

  if (POV->Buttons[ARow] != NULL)
  {
    // Fixed column ?
    if (ACol == 0)
    {
      StringGridPOV->Canvas->Brush->Color = Color;
      StringGridPOV->Canvas->Font->Color = mSettingsP->ThemeText2Color;
    }
    else if (POV->Buttons[ARow]->Pressed == true)
    {
      StringGridPOV->Canvas->Brush->Color = clRed;
      StringGridPOV->Canvas->Font->Color = clWhite;
    }
    else
    {
      if (State.Contains(gdSelected))
      {
        StringGridPOV->Canvas->Brush->Color = clHighlight;
        StringGridPOV->Canvas->Font->Color = clWhite;
      }
      else
      {
        StringGridPOV->Canvas->Brush->Color = Color;
        StringGridPOV->Canvas->Font->Color = Font->Color;
      }
    }

    // Draw cell
    StringGridPOV->Canvas->FillRect(Rect);
    int TextHeight = StringGridPOV->Canvas->TextHeight(StringGridPOV->Cells[ACol][ARow]);
    int TextPos = (StringGridPOV->RowHeights[ARow] - TextHeight) / 2;
    StringGridPOV->Canvas->TextOut(Rect.Left+5,
                                   Rect.Top+TextPos,
                                   StringGridPOV->Cells[ACol][ARow]);
    if (State.Contains(gdFocused))
    {
      StringGridPOV->Canvas->DrawFocusRect(Rect);
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::StringGridAnalogDrawCell(
      TObject *Sender, int ACol, int ARow, TRect &Rect,
      TGridDrawState State)
{
  if (mSelectedAnalogP != NULL)
  {
    if (mSelectedAnalogP->Buttons[ARow] != NULL)
    {
      // Fixed column ?
      if (ACol == 0)
      {
        StringGridAnalog->Canvas->Brush->Color = Color;
        StringGridAnalog->Canvas->Font->Color = mSettingsP->ThemeText2Color;
      }
      else if (mSelectedAnalogP->Buttons[ARow]->Pressed == true)
      {
        StringGridAnalog->Canvas->Brush->Color = clRed;
        StringGridAnalog->Canvas->Font->Color = clWhite;
      }
      else
      {
        if (State.Contains(gdSelected))
        {
          StringGridAnalog->Canvas->Brush->Color = clHighlight;
          StringGridAnalog->Canvas->Font->Color = clWhite;
        }
        else
        {
          StringGridAnalog->Canvas->Brush->Color = Color;
          StringGridAnalog->Canvas->Font->Color = Font->Color;
        }
      }

      // Draw cell
      StringGridAnalog->Canvas->FillRect(Rect);
      int TextHeight = StringGridAnalog->Canvas->TextHeight(StringGridAnalog->Cells[ACol][ARow]);
      int TextPos = (StringGridAnalog->RowHeights[ARow] - TextHeight) / 2;
      StringGridAnalog->Canvas->TextOut(Rect.Left+5,
                                        Rect.Top+TextPos,
                                        StringGridAnalog->Cells[ACol][ARow]);
      if (State.Contains(gdFocused))
      {
        StringGridAnalog->Canvas->DrawFocusRect(Rect);
      }
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::StringGridButtonsMouseDown(
			TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  // Get selected row
  int SelectedCol;
  int SelectedRow;
  StringGridButtons->MouseToCell(X, Y, SelectedCol, SelectedRow);
  StringGridButtons->Row = SelectedRow;

  TInputActionButton* ActionP = mSelectedDeviceP->ActionsP->Buttons[SelectedRow];

  // Prepare edit keycode form
  TFormEditKeycode *EditKeycode = new TFormEditKeycode(this);
  EditKeycode->Settings = mSettingsP;
  EditKeycode->Key = ActionP->ButtonKey->WINKeycode;

  //roger it does not work with expressions, you have to store it in a variable.
  TShiftState ShiftSt =  ActionP->ButtonKey->WINShiftState;
  EditKeycode->ShiftState = (TShiftState&)ShiftSt;

  EditKeycode->InputContextP = mInputContextP;
  EditKeycode->MouseEnable = true;
  EditKeycode->MouseButton = ActionP->MouseActive;

  if (EditKeycode->ShowModal() == mrOk)
  {
    ButtonApply->Enabled = true;

    // Key action
    TInputKey* InputKeyP = new TInputKey;

	// Mouse ?
	if (EditKeycode->MouseButton == false)
	{
	  InputKeyP->WINKeycode = EditKeycode->Key;

      //roger
	  TShiftState tShiftSt = EditKeycode->ShiftState;
	  InputKeyP->WINShiftState = (TShiftState&)tShiftSt;
    }

    mInputContextP->AddButtonAction(mSelectedDeviceP,
                                    SelectedRow,
                                    InputKeyP);

    // Mouse
    if (EditKeycode->MouseButton == true)
    {
      mInputContextP->AddButtonMouse(mSelectedDeviceP,
                                     SelectedRow);

      StringGridButtons->Cells[1][SelectedRow] = ButtonMouseLabel;
    }
    else
    {
      StringGridButtons->Cells[1][SelectedRow] = mInputContextP->GetPressedKeyString(InputKeyP);
    }
  }

  delete EditKeycode;

	NOT_USED(Sender);
	NOT_USED(Button);
	NOT_USED(Shift);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::StringGridPOVMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  // Get selected row
  int SelectedRow = StringGridPOV->Row;
  TInputActionPOV* POV = mSelectedDeviceP->ActionsP->POVs[ComboBoxPOVId->ItemIndex];
  TInputKey* Key = POV->Buttons[SelectedRow];

  // Prepare edit keycode form
  TFormEditKeycode *EditKeycode = new TFormEditKeycode(this);
  EditKeycode->Settings = mSettingsP;
  EditKeycode->Key = Key->WINKeycode;

  //roger
  TShiftState tss = Key->WINShiftState;
  EditKeycode->ShiftState = (TShiftState&)tss;
  EditKeycode->MouseEnable = false;
  EditKeycode->InputContextP = mInputContextP;

  if (EditKeycode->ShowModal() == mrOk)
  {
	TInputKey* InputKeyP = new TInputKey;
	InputKeyP->WINKeycode = EditKeycode->Key;

	//roger
	TShiftState tss = EditKeycode->ShiftState;
	InputKeyP->WINShiftState = (TShiftState&)tss;
    AnsiString KeyID = mInputContextP->GetPressedKeyString(InputKeyP);
    mInputContextP->AddPOVAction(mSelectedDeviceP,
                                 ComboBoxPOVId->ItemIndex,
                                 SelectedRow,
                                 InputKeyP);
    StringGridPOV->Cells[1][SelectedRow] = KeyID;
    ButtonApply->Enabled = true;
  }

  delete EditKeycode;

	NOT_USED(Sender);
	NOT_USED(Button);
	NOT_USED(Shift);
	NOT_USED(X);
	NOT_USED(Y);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::StringGridAnalogMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
  if (mSelectedAnalogP != NULL)
  {
    int SelectedRow = StringGridAnalog->Row;
    TInputKey* Key = mSelectedAnalogP->Buttons[SelectedRow];

    // Prepare edit keycode form
    TFormEditKeycode *EditKeycode = new TFormEditKeycode(this);
    EditKeycode->Settings = mSettingsP;
    EditKeycode->Key = Key->WINKeycode;

	//roger
	TShiftState tss =Key->WINShiftState;
	EditKeycode->ShiftState = (TShiftState&)tss;
	EditKeycode->MouseEnable = false;
	EditKeycode->InputContextP = mInputContextP;

	if (EditKeycode->ShowModal() == mrOk)
	{
	  TInputKey* InputKeyP = new TInputKey;
	  InputKeyP->WINKeycode = EditKeycode->Key;

	  //roger
	  TShiftState tss = EditKeycode->ShiftState;
	  InputKeyP->WINShiftState = (TShiftState&)tss;
      AnsiString KeyID = mInputContextP->GetPressedKeyString(InputKeyP);
      mInputContextP->AddAnalogAction(mSelectedDeviceP,
                                      mSelectedAnalogIndex,
                                      SelectedRow,
                                      InputKeyP);
      StringGridAnalog->Cells[1][SelectedRow] = KeyID;
      ButtonApply->Enabled = true;
    }

    delete EditKeycode;
  }

	NOT_USED(Sender);
	NOT_USED(Button);
	NOT_USED(Shift);
	NOT_USED(X);
	NOT_USED(Y);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::ComboBoxProfilesChange(
      TObject *Sender)
{
  if (ComboBoxProfiles->Focused())
  {
    ButtonApply->Enabled = true;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::ComboBoxProfilesDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxProfiles->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxProfiles->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxProfiles->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

	NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::ComboBoxPOVIdChange(TObject *Sender)
{
  TInputActionPOV* POV = mSelectedDeviceP->ActionsP->POVs[ComboBoxPOVId->ItemIndex];

  for (int loop=0; loop < POV->NbButtons; loop++)
  {
    StringGridPOV->Cells[0][loop] = POVButtonsLabel[loop];
    if (POV->Buttons[loop] != NULL)
    {
      StringGridPOV->Cells[1][loop] = mInputContextP->GetPressedKeyString(POV->Buttons[loop]);
    }
    else
    {
      StringGridPOV->Cells[1][loop] = "";
    }
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::ComboBoxPOVIdDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxPOVId->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxPOVId->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxPOVId->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

	NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::ComboBoxAnalogAxisChange(
      TObject *Sender)
{
  AnsiString SelectedAxis = ComboBoxAnalogAxis->Items->Strings[ComboBoxAnalogAxis->ItemIndex];
  if (SelectedAxis == AnalogAxis[ANALOG_XAXIS_INDEX])
  {
    mSelectedAnalogIndex = ANALOG_XAXIS_INDEX;
  }
  else if (SelectedAxis == AnalogAxis[ANALOG_YAXIS_INDEX])
  {
    mSelectedAnalogIndex = ANALOG_YAXIS_INDEX;
  }
  else if (SelectedAxis == AnalogAxis[ANALOG_ZAXIS_INDEX])
  {
    mSelectedAnalogIndex = ANALOG_ZAXIS_INDEX;
  }
  else if (SelectedAxis == AnalogAxis[ANALOG_RXAXIS_INDEX])
  {
    mSelectedAnalogIndex = ANALOG_RXAXIS_INDEX;
  }
  else if (SelectedAxis == AnalogAxis[ANALOG_RYAXIS_INDEX])
  {
    mSelectedAnalogIndex = ANALOG_RYAXIS_INDEX;
  }
  else if (SelectedAxis == AnalogAxis[ANALOG_RZAXIS_INDEX])
  {
    mSelectedAnalogIndex = ANALOG_RZAXIS_INDEX;
  }
  else if (SelectedAxis.Pos(AnalogAxis[ANALOG_SLIDER1_INDEX]) == 1)
  {
    // Get Slider index
    int Index = SelectedAxis.SubString(AnalogAxis[ANALOG_SLIDER1_INDEX].Length() + 1,
                                       AnalogAxis[ANALOG_SLIDER1_INDEX].Length() - 1).ToInt();
    mSelectedAnalogIndex = ANALOG_SLIDER1_INDEX + Index - 1;
  }
  else
  {
    mSelectedAnalogIndex = -1;
    mSelectedAnalogP = NULL;
  }

  if (mSelectedAnalogIndex == -1)
    return;

  mSelectedAnalogP = mSelectedDeviceP->ActionsP->Analogs[mSelectedAnalogIndex];
  TrackBarAnalogPressThreshold->Position = mSelectedAnalogP->PressThreshold;
  TrackBarAnalogDeadZone->Min = mSelectedAnalogP->DeadZoneMin;
  TrackBarAnalogDeadZone->Max = mSelectedAnalogP->DeadZoneMax;
  TrackBarAnalogDeadZone->Position = mSelectedAnalogP->DeadZone;
  CheckBoxAnalogMouseX->Checked = mSelectedAnalogP->MouseXActive;
  CheckBoxAnalogMouseY->Checked = mSelectedAnalogP->MouseYActive;

  // Update analog buttons labels
  for (int loop=0; loop < mSelectedAnalogP->NbButtons; loop++)
  {
    StringGridAnalog->Cells[0][loop] = AnalogButtonsLabel[loop];
    if (mSelectedAnalogP->Buttons[loop] != NULL)
    {
      StringGridAnalog->Cells[1][loop] = mInputContextP->GetPressedKeyString(mSelectedAnalogP->Buttons[loop]);
    }
    else
    {
      StringGridAnalog->Cells[1][loop] = "";
    }
  }

  // Redraw analog buttons list
  StringGridAnalog->Invalidate();

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::ComboBoxAnalogAxisDrawItem(TWinControl *Control,
          int Index, TRect &Rect, TOwnerDrawState State)
{
  TCanvas* Canvas = ComboBoxAnalogAxis->Canvas;

  if (State.Contains(odSelected))
  {
    Canvas->Brush->Color = clHighlight;
    Canvas->Font->Color = clWhite;
  }
  else
  {
    Canvas->Brush->Color = clWhite;
    Canvas->Font->Color = clBlack;
  }

  // Draw cell
  Canvas->FillRect(Rect);
  AnsiString Text = ComboBoxAnalogAxis->Items->Strings[Index];
  int TextHeight = Canvas->TextHeight(Text);
  int TextPos = (ComboBoxAnalogAxis->ItemHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+5, Rect.Top+TextPos, Text);
  if (State.Contains(odFocused))
  {
    Canvas->DrawFocusRect(Rect);
  }

	NOT_USED(Control);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::TrackBarAnalogDeadZoneChange(TObject *Sender)

{
  if ( (mSelectedAnalogP != NULL) && TrackBarAnalogDeadZone->Focused() )
  {
    mSelectedAnalogP->DeadZone = TrackBarAnalogDeadZone->Position;
    ButtonApply->Enabled = true;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::TrackBarAnalogPressThresholdChange(
      TObject *Sender)
{
  if ( (mSelectedAnalogP != NULL) && TrackBarAnalogPressThreshold->Focused() )
  {
    mSelectedAnalogP->PressThreshold = TrackBarAnalogPressThreshold->Position;
    ButtonApply->Enabled = true;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::CheckBoxAnalogMouseXClick(
      TObject *Sender)
{
  if (mSelectedAnalogP != NULL)
  {
    mSelectedAnalogP->MouseXActive = CheckBoxAnalogMouseX->Checked;
    ButtonApply->Enabled = true;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::CheckBoxAnalogMouseYClick(
      TObject *Sender)
{
  if (mSelectedAnalogP != NULL)
  {
    mSelectedAnalogP->MouseYActive = CheckBoxAnalogMouseY->Checked;
    ButtonApply->Enabled = true;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------



void __fastcall TFormGameControllerSettings::TabSheetButtonsShow(
      TObject *Sender)
{
  ActiveControl = StringGridButtons;  

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::TabSheetPOVShow(
      TObject *Sender)
{
  ActiveControl = StringGridPOV;  

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::TabSheetAnalogShow(
      TObject *Sender)
{
  ActiveControl = ComboBoxAnalogAxis;  

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormGameControllerSettings::ButtonResetClick(TObject *Sender)
{
  if (mInputContextP)
  {
    // Remove dummy actions
    mInputContextP->DeleteDummyActions();

    mInputContextP->NewGameControllersProfile(true);

    // Add dummy actions
    mInputContextP->AddDummyActions();

    // Display device capabilities
    ComboBoxControllersListChange(Sender);

    ButtonApply->Enabled = true;
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormGameControllerSettings::PageControlActionsDrawTab(TCustomTabControl *Control,
          int TabIndex, const TRect &Rect, bool Active)
{
  TCanvas* Canvas = PageControlActions->Canvas;

  if (Active == true)
  {
    Canvas->Font->Color = mSettingsP->ThemeText2Color;
  }
  else
  {
    Canvas->Font->Color = mSettingsP->ThemeText1Color;
  }

  // Get sheet corresponding to TabIndex
  int TabCount=0;
  TTabSheet* Sheet = NULL;
  for (int Loop=0; Loop < PageControlActions->PageCount; Loop++)
  {
    if (PageControlActions->Pages[Loop]->TabVisible == true)
    {
      if (TabCount == TabIndex)
      {
        Sheet = PageControlActions->Pages[Loop];
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
  int TextPosY = (PageControlActions->TabHeight - TextHeight) / 2;
  Canvas->TextOut(Rect.Left+TextPosX, Rect.Top+TextPosY, Text);

	NOT_USED(Control);
}
//---------------------------------------------------------------------------

