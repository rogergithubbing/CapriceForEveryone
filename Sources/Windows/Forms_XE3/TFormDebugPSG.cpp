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
#include <math.h>
#include <tchar.h>
#pragma hdrstop
#include "TFormDebugPSG.h"
#include "TFormMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"


//=================================
// Options

//=================================


static const AnsiString cTitle = "PSG";
static const AnsiString cTitlePaused  = " - [PAUSED]";
static const AnsiString cTitleMonitoring  = " - [Monitoring]";

static const int EnvelopeShape0[] =
{
/*  X,  Y */
    0, 10,
    1,  0,
   10,  0,
   -1, -1     // Must be the last
};
static const int EnvelopeShape4[] =
{
/*  X,  Y */
    0,  0,
    1, 10,
    1,  0,
   10,  0,
   -1, -1     // Must be the last
};
static const int EnvelopeShape8[] =
{
/*  X,  Y */
    0, 10,
    1,  0,
    1, 10,
    2,  0,
    2, 10,
    3,  0,
    3, 10,
    4,  0,
    4, 10,
    5,  0,
    5, 10,
    6,  0,
    6, 10,
    7,  0,
    7, 10,
    8,  0,
    8, 10,
    9,  0,
    9, 10,
   10,  0,
   -1, -1     // Must be the last
};
static const int EnvelopeShape10[] =
{
/*  X,  Y */
    0, 10,
    1,  0,
    2, 10,
    3,  0,
    4, 10,
    5,  0,
    6, 10,
    7,  0,
    8, 10,
    9,  0,
   10, 10,
   -1, -1     // Must be the last
};
static const int EnvelopeShape11[] =
{
/*  X,  Y */
    0, 10,
    1,  0,
    1, 10,
   10, 10,
   -1, -1     // Must be the last
};
static const int EnvelopeShape12[] =
{
/*  X,  Y */
    0,  0,
    1, 10,
    1,  0,
    2, 10,
    2,  0,
    3, 10,
    3,  0,
    4, 10,
    4,  0,
    5, 10,
    5,  0,
    6, 10,
    6,  0,
    7, 10,
    7,  0,
    8, 10,
    8,  0,
    9, 10,
    9,  0,
   10, 10,
   10,  0,
   -1, -1     // Must be the last
};
static const int EnvelopeShape13[] =
{
/*  X,  Y */
    0,  0,
    1, 10,
   10, 10,
   -1, -1     // Must be the last
};
static const int EnvelopeShape14[] =
{
/*  X,  Y */
    0,  0,
    1, 10,
    2,  0,
    3, 10,
    4,  0,
    5, 10,
    6,  0,
    7, 10,
    8,  0,
    9, 10,
   10,  0,
   -1, -1     // Must be the last
};
static const int EnvelopeShape15[] =
{
/*  X,  Y */
    0,  0,
    1, 10,
    1,  0,
   10,  0,
   -1, -1     // Must be the last
};


TFormDebugPSG *FormDebugPSG;
//---------------------------------------------------------------------------
__fastcall TFormDebugPSG::TFormDebugPSG(TComponent* Owner)
  : TForm(Owner)
{
  mSettingsP = NULL;
  mAudioContextP = NULL;

  for (int i=0; i < PSG_REGISTERS_NBELEMENT; i++)
  {
    mDisplayValueHexa[i] = true;
  }

  // Init component lists
  for (int Loop=0; Loop < PSG_REGISTERS_NBELEMENT; Loop++)
  {
    mRadioButtonRegister[Loop] = (TRadioButton*)FindComponent("RadioButtonR" + AnsiString(Loop));
    mPanelRegisterFormat[Loop] = (TPanel*)FindComponent("PanelFormatR" + AnsiString(Loop));
    mPanelRegister[Loop] = (TPanel*)FindComponent("PanelR" + AnsiString(Loop));
    mLabelRegister[Loop] = (TLabel*)FindComponent("LabelR" + AnsiString(Loop));
  }

  // Apply theme
  Font->Name = "Amstrad CPC464 Regular";
  Font->Size = 6;
  for (int Loop=0; Loop <= 15; Loop++)
  {
    TPanel* PanelP = (TPanel*)FindComponent("PanelFormatR" + AnsiString(Loop));
    if (PanelP)
    {
      PanelP->Font->Color = clBlack;
    }
  }
  PanelLeftSampleMax->Font->Color = clBlack;
  PanelRightSampleMax->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::FormShow(TObject *Sender)
{
  if (FormMain == NULL)
  {
    Close();
    return;
  }

  FormMain->SubscribeToCPCEvents(OnEmulatorEvent);

  StopMonitoring();

  PanelLeftSampleMaxClick(Sender);
  PanelRightSampleMaxClick(Sender);

  tNativeCPC* NativeCPC = FormMain->NativeCPC;
  InitRegisters(NativeCPC);
  UpdateChannels(NativeCPC->PSG);

  mLastEnvelopeShape = -1;
  UpdateEnveloppe(NativeCPC->PSG);
  UpdateNoise(NativeCPC->PSG);

  mPaused = FormMain->NativeCPC->paused;
  Caption = Title;

  // Charts range
  if (mAudioContextP != NULL)
  {
    if (mAudioContextP->Speaker16Bits == true)
    {
      ChartLeft->LeftAxis->Maximum = 16383;
      ChartLeft->LeftAxis->Minimum = -2048;
      ChartRight->LeftAxis->Maximum = 16383;
      ChartRight->LeftAxis->Minimum = -2048;
    }
    else
    {
      ChartLeft->LeftAxis->Maximum = 255;
      ChartLeft->LeftAxis->Minimum = 120;
      ChartRight->LeftAxis->Maximum = 255;
      ChartRight->LeftAxis->Minimum = 120;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::FormHide(TObject *Sender)
{
  // Unsubscribe to step by step event
  if (FormMain != NULL)
  {
    FormMain->UnsubscribeToCPCEvents(OnEmulatorEvent);
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


AnsiString __fastcall TFormDebugPSG::GetTitle(void)
{
  AnsiString Title = cTitle;
  if (mPaused == true) Title += cTitlePaused;
  if (mMonitoring == true) Title += cTitleMonitoring;
  return Title;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::SetSettings(TSettings* Value)
{
  mSettingsP = Value;

  if (    (mSettingsP->PSGFormLeft != 0)
       || (mSettingsP->PSGFormTop != 0)
       || (mSettingsP->PSGFormWidth != 0)
       || (mSettingsP->PSGFormHeight != 0) )
  {
    Position = poDesigned;
    Left = mSettingsP->PSGFormLeft;
    Top = mSettingsP->PSGFormTop;
    Width = mSettingsP->PSGFormWidth;
    Height = mSettingsP->PSGFormHeight;
  }

  ShowHint = mSettingsP->ShowHints;

  if (mSettingsP)
  {
    // Apply Theme
    Color = mSettingsP->ThemeBackgroundColor;
    Font->Color = mSettingsP->ThemeText1Color;
#if (__BORLANDC__ != 0x551)
    ChartEnvelope->Series[0]->Color = mSettingsP->ThemeText2Color;
    ChartLeft->Series[0]->Color = mSettingsP->ThemeText2Color;
    ChartRight->Series[0]->Color = mSettingsP->ThemeText2Color;
#endif
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::FormResize(TObject *Sender)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->PSGFormWidth = Width;
    mSettingsP->PSGFormHeight = Height;
	}

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::WMMove(TMessage &Message)
{
  if ( (Visible == true) && (mSettingsP) )
  {
    mSettingsP->PSGFormLeft = Left;
    mSettingsP->PSGFormTop = Top;
  }

	NOT_USED(Message);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::InitRegisters(tNativeCPC* NativeCPC)
{
  // Initialiser registers value
  for (int Index=0; Index < PSG_REGISTERS_NBELEMENT; Index++)
  {
    mLastDisplayedRegistersP[Index] = NativeCPC->PSG->RegisterAY.Index[Index];

    if (mDisplayValueHexa[Index])
    {
      mPanelRegister[Index]->Caption = AnsiString::IntToHex(mLastDisplayedRegistersP[Index], 2);
    }
    else
    {
      mPanelRegister[Index]->Caption = mLastDisplayedRegistersP[Index];
    }
  }
  
  mLastDisplayedRegisterSelected = NativeCPC->PSG->reg_select & 0x0f;
  mRadioButtonRegister[mLastDisplayedRegisterSelected]->Checked = true;
  mLabelRegister[mLastDisplayedRegisterSelected]->Font->Color = mSettingsP->ThemeText2Color;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::UpdateRegisters(tNativeCPC* NativeCPC)
{
  // Initialiser registers value
  for (int Index=0; Index < PSG_REGISTERS_NBELEMENT; Index++)
  {
    if (mLastDisplayedRegistersP[Index] != (tULong)NativeCPC->PSG->RegisterAY.Index[Index])
    {
      mLastDisplayedRegistersP[Index] = (tULong)NativeCPC->PSG->RegisterAY.Index[Index];

      if (mDisplayValueHexa[Index])
      {
        mPanelRegister[Index]->Caption = AnsiString::IntToHex(mLastDisplayedRegistersP[Index], 2);
      }
      else
      {
        mPanelRegister[Index]->Caption = mLastDisplayedRegistersP[Index];
      }
    }
  }

  if (mLastDisplayedRegisterSelected != NativeCPC->PSG->reg_select)
  {
    mLabelRegister[mLastDisplayedRegisterSelected]->Font->Color = mSettingsP->ThemeText1Color;
    mLastDisplayedRegisterSelected = NativeCPC->PSG->reg_select & 0x0f;
    mRadioButtonRegister[mLastDisplayedRegisterSelected]->Checked = true;
    mLabelRegister[mLastDisplayedRegisterSelected]->Font->Color = mSettingsP->ThemeText2Color;
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::PanelLeftSampleMaxClick(TObject *Sender)
{
  mLeftSampleMax = 0;
  AnsiString Message = AnsiString(mLeftSampleMax);
  PanelLeftSampleMax->Caption = Message;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::PanelRightSampleMaxClick(TObject *Sender)
{
	mRightSampleMax = 0;
	AnsiString Message = AnsiString(mRightSampleMax);
	PanelRightSampleMax->Caption = Message;

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::ButtonMonitorStartStopClick(TObject *Sender)
{
  if (ButtonMonitorStartStop->Caption == "Start")
  {
    if (FormMain->NativeCPC->PSG->snd_enabled)
    {
      mMonitoring = true;
      ButtonMonitorStartStop->Caption = "Stop";
      Caption = Title;
      //FormMain->ResumeStepByStep(OnEmulatorEvent);
    }
    else
    {
			Application->MessageBox(_T("Unable to display audio samples while muted."),
															_T("Audio monitoring"),
															MB_OK);
    }
  }
  else
  {
    StopMonitoring();
  }

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::PanelFormatClick(TObject *Sender)
{
  UpdateRegisterFormat(((TPanel*)Sender)->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::UpdateRegisterFormat(int Index)
{
  mDisplayValueHexa[Index] ^= 1; // Invert

  if (mDisplayValueHexa[Index])
  {
    mPanelRegisterFormat[Index]->Caption = "&&";
  }
  else
  {
    mPanelRegisterFormat[Index]->Caption = "";
  }
  
  UpdateRegister(Index);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::UpdateRegister(int Index)
{
  if (mDisplayValueHexa[Index])
  {
    mPanelRegister[Index]->Caption = AnsiString::IntToHex(mLastDisplayedRegistersP[Index], 2);
  }
  else
  {
    mPanelRegister[Index]->Caption = mLastDisplayedRegistersP[Index];
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::UpdateChannels(tPSG* PSG)
{
  //
  // Channel A
  //
  PanelChannelA->Color = PSG->Channel_EnA ? clLime : clRed;
  PanelChannelA->Font->Color = PSG->Channel_EnA ? clBlack : clWhite;
  int Freq = CLOCK_AY_INPUT / 16;
  Freq /= (((PSG->RegisterAY.UChar.TonAHi & 0x0f) << 8) + PSG->RegisterAY.UChar.TonALo + 1);
  PanelChannelAFreq->Caption = AnsiString(Freq) + " Hz";
  PanelChannelAFreq->Color = PSG->Ton_EnA ? clLime : clRed;
  PanelChannelAFreq->Font->Color = PSG->Ton_EnA ? clBlack : clWhite;
  PanelChannelANoise->Color = PSG->Noise_EnA ? clLime : clRed;
  PanelChannelANoise->Font->Color = PSG->Noise_EnA ? clBlack : clWhite;
  PanelChannelAEnv->Color = PSG->Envelope_EnA ? clRed : clLime;
  PanelChannelAEnv->Font->Color = PSG->Envelope_EnA ? clWhite : clBlack;
  int Amplitude = (int)((double)100. / sqrt((float) (1 << (15 - (PSG->RegisterAY.UChar.AmplitudeA & 0x0f)))));
  ProgressBarChannelAAmplitude->Position = Amplitude;

  //
  // Channel B
  //
  PanelChannelB->Color = PSG->Channel_EnB ? clLime : clRed;
  PanelChannelB->Font->Color = PSG->Channel_EnB ? clBlack : clWhite;
  Freq = CLOCK_AY_INPUT / 16;
  Freq /= (((PSG->RegisterAY.UChar.TonBHi & 0x0f) << 8) + PSG->RegisterAY.UChar.TonBLo + 1);
  PanelChannelBFreq->Caption = AnsiString(Freq) + " Hz";
  PanelChannelBFreq->Color = PSG->Ton_EnB ? clLime : clRed;
  PanelChannelBFreq->Font->Color = PSG->Ton_EnB ? clBlack : clWhite;
  PanelChannelBNoise->Color = PSG->Noise_EnB ? clLime : clRed;
  PanelChannelBNoise->Font->Color = PSG->Noise_EnB ? clBlack : clWhite;
  PanelChannelBEnv->Color = PSG->Envelope_EnB ? clRed : clLime;
  PanelChannelBEnv->Font->Color = PSG->Envelope_EnB ? clWhite : clBlack;
  Amplitude = (int)((double)100. / sqrt((float) (1 << (15 - (PSG->RegisterAY.UChar.AmplitudeB & 0x0f)))));
  ProgressBarChannelBAmplitude->Position = Amplitude;

  //
  // Channel C
  //
  PanelChannelC->Color = PSG->Channel_EnC ? clLime : clRed;
  PanelChannelC->Font->Color = PSG->Channel_EnC ? clBlack : clWhite;
  Freq = CLOCK_AY_INPUT / 16;
  Freq /= (((PSG->RegisterAY.UChar.TonCHi & 0x0f) << 8) + PSG->RegisterAY.UChar.TonCLo + 1);
  PanelChannelCFreq->Caption = AnsiString(Freq) + " Hz";
  PanelChannelCFreq->Color = PSG->Ton_EnC ? clLime : clRed;
  PanelChannelCFreq->Font->Color = PSG->Ton_EnC ? clBlack : clWhite;
  PanelChannelCNoise->Color = PSG->Noise_EnC ? clLime : clRed;
  PanelChannelCNoise->Font->Color = PSG->Noise_EnC ? clBlack : clWhite;
  PanelChannelCEnv->Color = PSG->Envelope_EnC ? clRed : clLime;
  PanelChannelCEnv->Font->Color = PSG->Envelope_EnC ? clWhite : clBlack;
  Amplitude = (int)((double)100. / sqrt((float) (1 << (15 - (PSG->RegisterAY.UChar.AmplitudeC & 0x0f)))));
  ProgressBarChannelCAmplitude->Position = Amplitude;
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::UpdateEnveloppe(tPSG* PSG)
{
  // Envelope Frequency
  int Freq = CLOCK_AY_INPUT / 256;
  Freq /= ((PSG->RegisterAY.UChar.EnvelopeHi & 0x0f) << 8) + PSG->RegisterAY.UChar.EnvelopeLo + 1;
  PanelEnveloppeFreq->Caption = AnsiString(Freq) + " Hz";

  // Envelope Shape
  if (mLastEnvelopeShape != (int)PSG->RegisterAY.UChar.EnvType)
  {
    mLastEnvelopeShape = (int)(PSG->RegisterAY.UChar.EnvType & 0x0f); 

    ChartEnvelope->Series[0]->Clear();
    
    int* Shape = NULL;
    if ( (mLastEnvelopeShape & 0xc) == 0)
    {
      Shape = (int*)EnvelopeShape0;
    }
    if ( (mLastEnvelopeShape & 0xc) == 4)
    {
      Shape = (int*)EnvelopeShape4;
    }
    else
    {
      switch (mLastEnvelopeShape)
      {
        case  8: Shape = (int*)EnvelopeShape8; break;
        case  9: Shape = (int*)EnvelopeShape0; break;
        case 10: Shape = (int*)EnvelopeShape10; break;
        case 11: Shape = (int*)EnvelopeShape11; break;
        case 12: Shape = (int*)EnvelopeShape12; break;
        case 13: Shape = (int*)EnvelopeShape13; break;
        case 14: Shape = (int*)EnvelopeShape14; break;
        case 15: Shape = (int*)EnvelopeShape15; break;
      }
    }

    if (Shape)
    {
      while (*Shape != -1)
      {
        ChartEnvelope->Series[0]->AddXY(*(Shape++),
                                        *(Shape++),
                                        "",
                                        mSettingsP->ThemeText1Color);
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::UpdateNoise(tPSG* PSG)
{
  // Noise Frequency
  int Freq = CLOCK_AY_INPUT / 16;
  Freq /= (PSG->RegisterAY.UChar.Noise & 0x1f) + 1;
  PanelNoiseFreq->Caption = AnsiString(Freq) + " Hz";
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::PanelChannelAClick(TObject *Sender)
{
  tPSG* PSG = FormMain->NativeCPC->PSG;
  PSG->Channel_EnA ^= 1; // Invert
  UpdateChannels(PSG);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::PanelChannelBClick(TObject *Sender)
{
  tPSG* PSG = FormMain->NativeCPC->PSG;
  PSG->Channel_EnB ^= 1; // Invert
  UpdateChannels(PSG);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormDebugPSG::PanelChannelCClick(TObject *Sender)
{
  tPSG* PSG = FormMain->NativeCPC->PSG;
  PSG->Channel_EnC ^= 1; // Invert
  UpdateChannels(PSG);

	NOT_USED(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormDebugPSG::StopMonitoring(void)
{
  mMonitoring = false;
  ButtonMonitorStartStop->Caption = "Start";
  Caption = Title;
  
  // Clear display
  mLoopLeft = 0;
  ChartLeft->Series[0]->Clear();
  mLoopRight = 0;
  ChartRight->Series[0]->Clear();
  PanelLeftSample->Caption = "";
  PanelRightSample->Caption = "";
}
//---------------------------------------------------------------------------


bool __fastcall TFormDebugPSG::OnEmulatorEvent(tULong Condition, tNativeCPC* NativeCPC)
{
  bool Pause = NativeCPC->paused;
  tPSG* PSG = NativeCPC->PSG;

  // Update registers
  UpdateRegisters(NativeCPC);
  UpdateChannels(PSG);
  UpdateEnveloppe(PSG);
  UpdateNoise(PSG);

  if ( (mPaused == true) && (Pause == false) )
  {
    mPaused = false;
    Caption = Title;
  }

  //
  // Runtime monitoring
  //
  if ( (mMonitoring == true) && (!NativeCPC->step_by_step) )
  {
    int Count = FormMain->EmulatorP->GetAudioEmulatorDataSize();
    Count /= mAudioContextP->SpeakerStereo == true ? 2 : 1;
    Count /= mAudioContextP->Speaker16Bits == true ? 2 : 1;

    if (mAudioContextP->Speaker16Bits == true)
    {
      // 16 Bits rendering
      tUShort* AudioData = (tUShort*)FormMain->EmulatorP->GetAudioEmulatorData();
      tUShort Left = *AudioData++;
      tUShort Right = *AudioData++;
      // Get samples average
      for (int Loop=1; Loop < Count; Loop++)
      {
        Left = (Left + (*AudioData++)) / 2;
        Right = (Right + (*AudioData++)) / 2;
      }
      ChartLeft->Series[0]->AddXY(mLoopLeft++, Left, "", mSettingsP->ThemeText1Color);
      ChartRight->Series[0]->AddXY(mLoopRight++, Right, "", mSettingsP->ThemeText1Color);

      // Update Left channel max
      if (Left > mLeftSampleMax)
      {
        mLeftSampleMax = Left;
        PanelLeftSampleMax->Caption = AnsiString(Left);
      }
      // Update Right channel max
      if (Right > mRightSampleMax)
      {
        mRightSampleMax = Right;
        PanelRightSampleMax->Caption = AnsiString(Right);
      }
    }
    else
    {
      // 8 Bits rendering
      tUChar* AudioData = (tUChar*)FormMain->EmulatorP->GetAudioEmulatorData();
      tUChar Left = *AudioData++;
      tUChar Right = *AudioData++;
      // Get samples average
      for (int Loop=1; Loop < Count; Loop++)
      {
        Left = (Left + (*AudioData++)) / 2;
        Right = (Right + (*AudioData++)) / 2;
      }
      ChartLeft->Series[0]->AddXY(mLoopLeft++, Left, "", mSettingsP->ThemeText1Color);
      ChartRight->Series[0]->AddXY(mLoopRight++, Right, "", mSettingsP->ThemeText1Color);

      // Update Left channel
      if (PSG->Left_Chan_Sample > mLeftSampleMax)
      {
        mLeftSampleMax = Left;
        PanelLeftSampleMax->Caption = AnsiString(Left);
      }
      // Update Right channel max
      if (Right > mRightSampleMax)
      {
        mRightSampleMax = Right;
        PanelRightSampleMax->Caption = AnsiString(Right);
      }
    }

    // Remove oldest samples
    Count = ChartLeft->Series[0]->Count() - ChartLeft->Width;
    while (Count-- > 0)
    {
      ChartLeft->Series[0]->Delete(0);
    }
    Count = ChartRight->Series[0]->Count() - ChartRight->Width;
    while (Count-- > 0)
    {
      ChartRight->Series[0]->Delete(0);
    }
  }

  if ( ((Condition & EC_END_INSTRUCTION) == 0) && (Pause == false) )
    return false;

  if (Pause == true)
  {
    if (mPaused == false)
    {
      mPaused = true;
      Caption = Title;
    }
  }

  if (!PSG->snd_enabled)
  {
    StopMonitoring();
    return Pause;
  }

  if ( (mMonitoring == false) || (!NativeCPC->step_by_step) )
    return Pause;

  //
  // Update Left channel
  //
  if (PSG->signal_Left_Chan_Ready)
  {
    PSG->signal_Left_Chan_Ready = 0;

    AnsiString Message = AnsiString(PSG->Left_Chan_Sample);
    PanelLeftSample->Caption = Message;

    if (PSG->Left_Chan_Sample > mLeftSampleMax)
    {
      mLeftSampleMax = PSG->Left_Chan_Sample;
      PanelLeftSampleMax->Caption = Message;
    }

    // Update oscilloscope
    ChartLeft->Series[0]->AddXY(mLoopLeft++, PSG->Left_Chan_Sample, "", mSettingsP->ThemeText1Color);
    int Count = ChartLeft->Series[0]->Count();
    while (Count > ChartLeft->Width)
    {
      ChartLeft->Series[0]->Delete(0);
      Count--;
    }
  }

  //
  // Update Right channel
  //
  if (PSG->signal_Right_Chan_Ready)
  {
    PSG->signal_Right_Chan_Ready = 0;

    AnsiString Message = AnsiString(PSG->Right_Chan_Sample);
    PanelRightSample->Caption = Message;

    if (PSG->Right_Chan_Sample > mRightSampleMax)
    {
      mRightSampleMax = PSG->Right_Chan_Sample;
      PanelRightSampleMax->Caption = Message;
    }

    // Update oscilloscope
    ChartRight->Series[0]->AddXY(mLoopRight++, PSG->Right_Chan_Sample, "", mSettingsP->ThemeText1Color);
    int Count = ChartRight->Series[0]->Count();
    while (Count > ChartRight->Width)
    {
      ChartRight->Series[0]->Delete(0);
      Count--;
    }
  }

  return Pause;
}
//---------------------------------------------------------------------------

