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

#define _USE_OPENAL

#include <stdio.h>
#include <string.h>
#include <vcl.h>
#include <math.h>
#include <tchar.h>
#pragma hdrstop
#include "TAudioContext.h"
#include "..\..\Common\Native_RLE.h"
#include "..\ZLib\TZIPContainer.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//=================================
// Options
//

#undef USE_PPI_HYSTERESIS
#define USE_PPI_HYSTERESIS

//=================================


#ifdef _USE_OPENAL

// OpenAL buffers
#define EMULATOR_NB_BUFFERS           10
#define FLOPPY_HEAD_NB_BUFFERS        2
#define TMPI_NB_BUFFERS               10
#define PLAYCITY_NB_BUFFERS           10

#endif /* _USE_OPENAL */


/*
 * Struct that holds the RIFF data of the Wave file.
 * The RIFF data is the meta data information that holds,
 * the ID, size and format of the wave file
 */
typedef struct
{
  char chunkID[4];
  long chunkSize;//size not including chunkSize and chunkID
  char format[4];
}tRIFF_Header;

typedef struct
{
  char chunkID[4];
  long chunkSize;//size not including chunkSize or chunkID
}tRIFF_Chunk_Header;

/*
 * Struct to hold fmt subchunk data for WAVE files.
 */
typedef struct
{
  short audioFormat;
  short numChannels;
  long sampleRate;
  long byteRate;
  short blockAlign;
  short bitsPerSample;
}tWAVE_Format;

/*
* Struct to hold the data of the wave file
*/
typedef struct
{
  char subChunkID[4]; //should contain the word data
  long subChunk2Size; //Stores the size of the data block
  char ChunkData;
}tWAVE_Data;

typedef struct
{
  tRIFF_Header RIFF_Header;
  tRIFF_Chunk_Header WAVE_Format_Header;
  tWAVE_Format WAVE_Format;
  tWAVE_Data WAVE_Data;
}tWAVE_file_header;


__fastcall TAudioContext::TAudioContext(TSettings* SettingsP)
//
// Constructor
//
{
  mLastError = 0;
  mSettingsP = SettingsP;

  mDevicesList = new TStringList();
  GetDeviceList();

  mQuietMode = false;
  mTapeCreateOutputAudioFile = false;
  mTapeCreateCDTFile = false;
  mTapeCreateCSWFile = false;

  // First time settings read
  if (SettingsP->TapeAudioUseDefault == true)
  {
    RestoreDefaultTapeAudioParameters();
  }
  else
  {
	mTapeAutoGainActive = SettingsP->TapeAudioAutoGainActive;
	mTapeLPF1Active = SettingsP->TapeAudioLPF1Active;
	mTapeLPF1_a = (float)SettingsP->TapeAudioLPF1_A.ToDouble();
	mTapeLPF2Active = SettingsP->TapeAudioLPF2Active;
	mTapeLPF2_Order = SettingsP->TapeAudioLPF2Order;
	mTapeLPF2_Frequency = SettingsP->TapeAudioLPF2Freq;
	mTapeHPF2Active = SettingsP->TapeAudioHPF2Active;
	mTapeHPF2_Order = SettingsP->TapeAudioHPF2Order;
	mTapeHPF2_Frequency = SettingsP->TapeAudioHPF2Freq;
	mTapePPIThreshold8 = SettingsP->TapeAudioPPIThreshold8;
	mTapePPIThreshold16 = SettingsP->TapeAudioPPIThreshold16;
	mTapeCreateCSWFile = SettingsP->TapeAudioCreateCSW;
  }


/*
int aa;
aa=2;
  float* pFloatAqui;
  float variableFloat;
  variableFloat = 3.2f;
  pFloat1 = pFloatAqui;
  pFloat1 = &variableFloat;
	#ifdef _USE_OPENAL

	int a;
	a=1;
  #endif

  */


  // roger this line sets mDevicesList->Count to 0 if in the header I don't declare something silly as "float* pFloat1;" , i dunno why...
  mTapeLPF2_A = NULL;

  mTapeLPF2_d1 = NULL;
  mTapeLPF2_d2 = NULL;
  mTapeHPF2_A = NULL;
  mTapeHPF2_d1 = NULL;
  mTapeHPF2_d2 = NULL;

#ifdef _USE_OPENAL

  mALdev = NULL;
  mALctx = NULL;

  mALEmulatorSource = 0;
  mALEmulatorBuffersP = NULL;
  mALEmulatorQueuedbuffersP = NULL;
  mALFloppyMotorSource = 0;
  mALFloppyMotorBuffer = 0;
  mALTapeMotorSource = 0;
  mALTapeMotorBuffer = 0;
  mALTapeRelaySource = 0;
  mALTapeRelayBuffer = 0;
  mALTapeDoubleRelaySource = 0;
  mALTapeDoubleRelayBuffer = 0;

  mFloppyHeadSampleData = NULL;
  mALFloppyHeadSource = 0;
  mALFloppyHeadBuffersP = NULL;
  mALFloppyHeadQueuedbuffersP = NULL;

  mALTMPISource = 0;

//roger this line also makes mDevicesList->Count fail
//mALTMPIBuffersP = NULL;

  mALTMPIQueuedbuffersP = NULL;

  mALPlayCitySource = 0;
  mALPlayCityBuffersP = NULL;
  mALPlayCityQueuedbuffersP = NULL;
  
  do
  {
	// Clear error code
	alGetError();

	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") == AL_FALSE)
	{
#ifdef _DEBUG
			Application->MessageBox(_T("NO ALC Extension"), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
	  continue;
	}

	//
	// Find Audio Renderer among available devices
	//
	bool bAudioRendererFound = false;

//roger

	for (int Loop=0; Loop < mDevicesList->Count; Loop++)
	{
			if (mDevicesList->Strings[Loop] == mSettingsP->AudioRenderer)
	  {
		bAudioRendererFound = true;
		break;
	  }
	}
	if (bAudioRendererFound == false) mSettingsP->AudioRenderer = "";

    //
    // Open device
    //
    if (mSettingsP->AudioRenderer.Length())
    {
      mALdev = alcOpenDevice(mSettingsP->AudioRenderer.c_str());
    }
    else
    {
      mALdev = alcOpenDevice(NULL);
    }
    ALenum ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alcOpenDevice Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
			continue;
    }
    if (mALdev == 0)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("mALdev NULL"), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    mALctx = alcCreateContext(mALdev, NULL);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alcCreateContext Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    alcMakeContextCurrent(mALctx);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alcCreateContext Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    //
    // Set listener properties
    //
    ALfloat listenerPos[]={0.0f, 0.0f, 0.0f};
    ALfloat listenerVel[]={0.0f, 0.0f, 0.0f};
    ALfloat listenerOri[]={0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f}; // “at”, then “up”
    // Position ...
    alListenerfv(AL_POSITION, listenerPos);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alListenerfv AL_POSITION Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
      continue;
    }
    // Velocity ...
    alListenerfv(AL_VELOCITY, listenerVel);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alListenerfv AL_VELOCITY Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
      continue;
    }
    // Orientation ...
    alListenerfv(AL_ORIENTATION, listenerOri);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alListenerfv AL_ORIENTATION Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    //
    // Audio volume
    //
    int AudioVolume = mSettingsP->AudioVolume;
    if (AudioVolume > AUDIO_VOLUME_MAX) AudioVolume = AUDIO_VOLUME_MAX;
    if (AudioVolume < AUDIO_VOLUME_MIN) AudioVolume = AUDIO_VOLUME_MIN;
    alListenerf(AL_GAIN, AudioVolume / 100.f);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alListenerf AL_GAIN Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    //
    // Load floppy sound
    //
    CreateResourceSource("SOUND_FDC_MOTOR_6128",
						 &mALFloppyMotorSource,
                         &mALFloppyMotorBuffer);
    // Make floppy motor sound looping
    alSourcei(mALFloppyMotorSource, AL_LOOPING, AL_TRUE);

    // Load and create floppy head sample
    LoadResourceData("SOUND_FDC_HEAD_6128",
                     &mFloppyHeadSampleData,
                     &mFloppyHeadSampleFormat,
                     &mFloppyHeadSampleFreq,
                     &mFloppyHeadSampleSize);
    if (mFloppyHeadSampleData == NULL)
    {
#ifdef _DEBUG
            //roger
			//Application->MessageBox(_T("mFloppyHeadSampleData NULL"), _T("TAudioContext::TAudioContext"), MB_OK);
#endif /* _DEBUG */
	  continue;
    }

    CreateFloppyHeadSource();

    //
    // Load tape sound
    //
    CreateResourceSource("SOUND_K7_MOTOR_464",
                         &mALTapeMotorSource,
                         &mALTapeMotorBuffer);
    CreateResourceSource("SOUND_K7_RELAY_464",
                         &mALTapeRelaySource,
                         &mALTapeRelayBuffer);
    CreateResourceSource("SOUND_K7_RELAYDBL_464",
                         &mALTapeDoubleRelaySource,
                         &mALTapeDoubleRelayBuffer);
    // Make tape motor sound looping
    alSourcei(mALTapeMotorSource, AL_LOOPING, AL_TRUE);

    //
    // Set GAIN
    //
    SetAudioGainFloppy(mSettingsP->AudioGainFloppy);
    SetAudioGainTape(mSettingsP->AudioGainTape);
  }
  while (0);
  
#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


__fastcall TAudioContext::~TAudioContext()
//
// Destructor
//
{
#ifdef _USE_OPENAL

  // Stop emulator
  if (mALEmulatorSource)
  {
    alSourceStop(mALEmulatorSource);
    alDeleteBuffers(EMULATOR_NB_BUFFERS, mALEmulatorBuffersP);
    alDeleteSources(1, &mALEmulatorSource);
  }

  // Stop TMPI
  if (mALTMPISource)
  {
    alSourceStop(mALTMPISource);
    alDeleteBuffers(TMPI_NB_BUFFERS, mALTMPIBuffersP);
    alDeleteSources(1, &mALTMPISource);
  }

  // Stop PlayCity
  if (mALPlayCitySource)
  {
    alSourceStop(mALPlayCitySource);
    alDeleteBuffers(PLAYCITY_NB_BUFFERS, mALPlayCityBuffersP);
    alDeleteSources(1, &mALPlayCitySource);
  }

  // Stop floppy motor
  if (mALFloppyMotorSource)
  {
    alSourceStop(mALFloppyMotorSource);
    alDeleteBuffers(1, &mALFloppyMotorBuffer);
    alDeleteSources(1, &mALFloppyMotorSource);
  }

  // Stop floppy head
  if (mALFloppyHeadSource)
  {
    alSourceStop(mALFloppyHeadSource);
    alDeleteBuffers(FLOPPY_HEAD_NB_BUFFERS, mALFloppyHeadBuffersP);
    alDeleteSources(1, &mALFloppyHeadSource);
  }

  // Stop tape motor
  if (mALTapeMotorSource)
  {
    alSourceStop(mALTapeMotorSource);
    alDeleteBuffers(1, &mALTapeMotorBuffer);
    alDeleteSources(1, &mALTapeMotorSource);
  }

  // Stop tape relay
  if (mALTapeRelaySource)
  {
    alSourceStop(mALTapeRelaySource);
    alDeleteBuffers(1, &mALTapeRelayBuffer);
    alDeleteSources(1, &mALTapeRelaySource);
  }

  // Stop tape double relay
  if (mALTapeDoubleRelaySource)
  {
    alSourceStop(mALTapeDoubleRelaySource);
    alDeleteBuffers(1, &mALTapeDoubleRelayBuffer);
    alDeleteSources(1, &mALTapeDoubleRelaySource);
  }

  // Close OpenAL
  alcMakeContextCurrent(NULL);
  alcDestroyContext(mALctx);
  alcCloseDevice(mALdev);

  delete [] mFloppyHeadSampleData;

  delete [] mALEmulatorBuffersP;
  delete [] mALEmulatorQueuedbuffersP;
  delete [] mALTMPIBuffersP;
  delete [] mALTMPIQueuedbuffersP;
  delete [] mALPlayCityBuffersP;
  delete [] mALPlayCityQueuedbuffersP;
  delete [] mALFloppyHeadBuffersP;
  delete [] mALFloppyHeadQueuedbuffersP;

#endif /* _USE_OPENAL */

  delete mDevicesList;
}
//---------------------------------------------------------------------------


TStringList* __fastcall TAudioContext::GetInfo(void)
{
  TStringList* InfoStrings = new TStringList;

#ifdef _USE_OPENAL

  InfoStrings->Add("OpenAL");
  InfoStrings->Add("======");
  InfoStrings->Add("AL_VERSION = " + AnsiString((char*)alGetString(AL_VERSION)));
  InfoStrings->Add("AL_VENDOR = " + AnsiString((char*)alGetString(AL_VENDOR)));
  InfoStrings->Add("AL_RENDERER = " + AnsiString((char*)alGetString(AL_RENDERER)));
  InfoStrings->Add("ALC_EXTENSIONS = " + AnsiString((char*)alcGetString(NULL, ALC_EXTENSIONS)));
  for (int Loop=0; Loop < mDevicesList->Count; Loop++)
  {
    InfoStrings->Add("ALC_ALL_DEVICES_SPECIFIER = " + mDevicesList->Strings[Loop]);
  }
  InfoStrings->Add("ALC_DEFAULT_DEVICE_SPECIFIER = " + AnsiString((char*)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER)));
  InfoStrings->Add("ALC_DEVICE_SPECIFIER = " + AnsiString((char*)alcGetString(NULL, ALC_DEVICE_SPECIFIER)));
  InfoStrings->Add("");

#endif /* _USE_OPENAL */

  return InfoStrings;
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::GetDeviceList(void)
{
  mDevicesList->Clear();

#ifdef _USE_OPENAL

  if (alcIsExtensionPresent(NULL, (ALCchar*)"ALC_ENUMERATION_EXT") == AL_TRUE)
  {
    char* deviceList = (char *)alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
    // go through device list (each device terminated with a single NULL,
    // list terminated with double NULL)
    if (deviceList)
    {
      char* device = deviceList;
      while (*device != NULL)
      {
        AnsiString NewDevice = AnsiString(device);
        mDevicesList->Add(NewDevice);
        device += NewDevice.Length() + 1; // Go begin next string
      }
    }
  }

#endif /*_USE_OPENAL */
}
//---------------------------------------------------------------------------


const AnsiString __fastcall TAudioContext::GetDevice(int Value)
{
  return(mDevicesList->Strings[Value]);
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::RestoreDefaultTapeAudioParameters(void)
{
  mTapeAutoGainActive = true;
  mTapeLPF1Active = false;
  mTapeLPF1_a = 0.40f;
  mTapeLPF2Active = true;
  mTapeLPF2_Order = 2;
  mTapeLPF2_Frequency = 4000;
  mTapeHPF2Active = true;
  mTapeHPF2_Order = 2;
  mTapeHPF2_Frequency = 500;
  mTapePPIThreshold8 = 132; // OK with 3D Pool and Gauntlet 2
  mTapePPIThreshold16 = 128;

  // Save settings
  mSettingsP->TapeAudioUseDefault = false;
  mSettingsP->TapeAudioAutoGainActive = mTapeAutoGainActive;
  mSettingsP->TapeAudioLPF1Active = mTapeLPF1Active;
  mSettingsP->TapeAudioLPF1_A = AnsiString::FormatFloat("0.000", mTapeLPF1_a);
  mSettingsP->TapeAudioLPF2Active = mTapeLPF2Active;
  mSettingsP->TapeAudioLPF2Order = mTapeLPF2_Order;
  mSettingsP->TapeAudioLPF2Freq = mTapeLPF2_Frequency;
  mSettingsP->TapeAudioHPF2Active = mTapeHPF2Active;
  mSettingsP->TapeAudioHPF2Order = mTapeHPF2_Order;
  mSettingsP->TapeAudioHPF2Freq = mTapeHPF2_Frequency;
  mSettingsP->TapeAudioPPIThreshold8 = mTapePPIThreshold8;
  mSettingsP->TapeAudioPPIThreshold16 = mTapePPIThreshold16;
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::SetTapePPIThreshold8(tUChar Value)
{
  mTapePPIThreshold8 = Value;
  mSettingsP->TapeAudioPPIThreshold8 = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapePPIThreshold16(tShort Value)
{
  mTapePPIThreshold16 = Value;
  mSettingsP->TapeAudioPPIThreshold16 = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeAutoGainActive(bool Value)
{
  mTapeAutoGainActive = Value;
  mSettingsP->TapeAudioAutoGainActive = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeLPF1Active(bool Value)
{
  mTapeLPF1Active = Value;
  mSettingsP->TapeAudioLPF1Active = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeLPF1_A(float Value)
{
  mTapeLPF1_a = Value;
  mSettingsP->TapeAudioLPF1_A = AnsiString::FormatFloat("0.000", Value);
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeLPF2Active(bool Value)
{
  mTapeLPF2Active = Value;
  mSettingsP->TapeAudioLPF2Active = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeLPF2Order(tUChar Value)
{
  mTapeLPF2_Order = Value;
  mSettingsP->TapeAudioLPF2Order = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeLPF2Freq(tUShort Value)
{
  mTapeLPF2_Frequency = Value;
  mSettingsP->TapeAudioLPF2Freq = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeHPF2Active(bool Value)
{
  mTapeHPF2Active = Value;
  mSettingsP->TapeAudioHPF2Active = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeHPF2Order(tUChar Value)
{
  mTapeHPF2_Order = Value;
  mSettingsP->TapeAudioHPF2Order = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeHPF2Freq(tUShort Value)
{
  mTapeHPF2_Frequency = Value;
  mSettingsP->TapeAudioHPF2Freq = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetTapeCreateCSWFile(bool Value)
{
  mTapeCreateCSWFile = Value;
  mSettingsP->TapeAudioCreateCSW = Value;
};
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetSpeakerStereo(bool Value)
{
  mSettingsP->AudioSpeakerStereo = Value;

#ifdef _USE_OPENAL

  // Stop emulator
  if (mALEmulatorSource)
  {
    alSourceStop(mALEmulatorSource);
    alDeleteBuffers(EMULATOR_NB_BUFFERS, mALEmulatorBuffersP);
    alDeleteSources(1, &mALEmulatorSource);

    CreateEmulatorStream();
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------
void __fastcall TAudioContext::SetPlayCityStereo(bool Value)
{
  mSettingsP->AudioPlayCityStereo = Value;

#ifdef _USE_OPENAL

  // Stop PlayCity
  if (mALPlayCitySource)
  {
    alSourceStop(mALPlayCitySource);
    alDeleteBuffers(PLAYCITY_NB_BUFFERS, mALPlayCityBuffersP);
    alDeleteSources(1, &mALPlayCitySource);

    CreatePlayCityStream();
  }
#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::Start(void)
{
bool StartOK = false;

  do
  {
    // Init emulator audio
    if (CreateEmulatorStream() == false)
      continue;

    // Init TMPI audio
    if (CreateTMPIStream() == false)
      continue;

    // Init PlayCity audio
    if (CreatePlayCityStream() == false)
      continue;

    StartOK = true;
  }
  while (0);

  return (StartOK);
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::Stop(void)
{
#ifdef _USE_OPENAL

  // Clear error
  alGetError();

  //
  // Emulator
  //
  if (mALEmulatorSource)
  {
    alSourceStop(mALEmulatorSource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALEmulatorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // TMPI
  //
  if (mALTMPISource)
  {
    alSourceStop(mALTMPISource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALTMPISource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // PlayCity
  //
  if (mALPlayCitySource)
  {
    alSourceStop(mALPlayCitySource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALPlayCitySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Floppy motor
  //
  if (mALFloppyMotorSource)
  {
    alSourceStop(mALFloppyMotorSource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALFloppyMotorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Floppy head
  //
  if (mALFloppyHeadSource)
  {
    alSourceStop(mALFloppyHeadSource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALFloppyHeadSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Tape motor
  //
  if (mALTapeMotorSource)
  {
    alSourceStop(mALTapeMotorSource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALTapeMotorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Tape relay
  //
  if (mALTapeRelaySource)
  {
    alSourceStop(mALTapeRelaySource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALTapeRelaySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Tape double relay
  //
  if (mALTapeDoubleRelaySource)
  {
    alSourceStop(mALTapeDoubleRelaySource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop mALTapeDoubleRelaySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Stop"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::Pause()
{
#ifdef _USE_OPENAL

  ALint val;
  ALenum Error;

  //
  // Pause emulator
  //
  alGetSourcei(mALEmulatorSource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALEmulatorSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALEmulatorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Pause TMPI
  //
  alGetSourcei(mALTMPISource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALTMPISource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTMPISource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Pause PlayCity
  //
  alGetSourcei(mALPlayCitySource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALPlayCitySource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALPlayCitySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Pause floppy motor
  //
  alGetSourcei(mALFloppyMotorSource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALFloppyMotorSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALFloppyMotorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Pause floppy head
  //
  alGetSourcei(mALFloppyHeadSource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALFloppyHeadSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALFloppyHeadSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Pause tape motor
  //
  alGetSourcei(mALTapeMotorSource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALTapeMotorSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTapeMotorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Pause tape relay
  //
  alGetSourcei(mALTapeRelaySource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALTapeRelaySource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTapeRelaySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  //
  // Pause tape double relay
  //
  alGetSourcei(mALTapeDoubleRelaySource, AL_SOURCE_STATE, &val);
  if (val == AL_PLAYING)
  {
    alSourcePause(mALTapeDoubleRelaySource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTapeDoubleRelaySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::Resume()
{
#ifdef _USE_OPENAL

  ALint val;
  ALenum Error;

  // Resume emulator
  alGetSourcei(mALEmulatorSource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALEmulatorSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALEmulatorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  // Resume TMPI
  alGetSourcei(mALTMPISource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALTMPISource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTMPISource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  // Resume PlayCity
  alGetSourcei(mALPlayCitySource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALPlayCitySource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALPlayCitySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  // Resume floppy motor
  alGetSourcei(mALFloppyMotorSource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALFloppyMotorSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALFloppyMotorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  // Resume floppy head
  alGetSourcei(mALFloppyHeadSource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALFloppyHeadSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALFloppyHeadSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  // Resume tape motor
  alGetSourcei(mALTapeMotorSource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALTapeMotorSource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTapeMotorSource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  // Resume tape relay
  alGetSourcei(mALTapeRelaySource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALTapeRelaySource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTapeRelaySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

  // Resume tape double relay
  alGetSourcei(mALTapeDoubleRelaySource, AL_SOURCE_STATE, &val);
  if (val == AL_PAUSED)
  {
    alSourcePlay(mALTapeDoubleRelaySource);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourcePause mALTapeDoubleRelaySource Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::Pause"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::CreateEmulatorStream(void)
{
bool InitOK = false;

  do
  {
#ifdef _USE_OPENAL

    // Clear error code
    alGetError();

    // Crate emulator source
    alGenSources((ALuint)1, &mALEmulatorSource);
    ALenum ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGenSources Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreateEmulatorStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }
    if (mALEmulatorSource == 0)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("mALEmulatorSource NULL"), _T("TAudioContext::CreateEmulatorStream"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Create audio buffers
    mALEmulatorBuffersP = new ALuint[EMULATOR_NB_BUFFERS];
    alGenBuffers(EMULATOR_NB_BUFFERS, mALEmulatorBuffersP);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGenBuffers Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreateEmulatorStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }

    mALEmulatorQueuedbuffersP = new ALboolean[EMULATOR_NB_BUFFERS];
    for (ALuint loop=0; loop < EMULATOR_NB_BUFFERS; loop++)
    {
      mALEmulatorQueuedbuffersP[loop] = AL_FALSE;
    }

    // Set GAIN
    SetAudioGainSpeaker(mSettingsP->AudioGainSpeaker);

#endif /* _USE_OPENAL */

    InitOK = true;
  }
  while (0);

  return InitOK;
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::CreateTMPIStream(void)
{
bool InitOK = false;

  do
  {
#ifdef _USE_OPENAL

    // Clear error code
    alGetError();

    // Crate emulator source
    alGenSources((ALuint)1, &mALTMPISource);
    ALenum ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGenSources Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreateTMPIStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }
    if (mALTMPISource == 0)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("mALEmulatorSource NULL"), _T("TAudioContext::CreateTMPIStream"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Create audio buffers
    mALTMPIBuffersP = new ALuint[TMPI_NB_BUFFERS];
    alGenBuffers(TMPI_NB_BUFFERS, mALTMPIBuffersP);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG

			String msg = _T("alGenBuffers Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreateTMPIStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }

    mALTMPIQueuedbuffersP = new ALboolean[TMPI_NB_BUFFERS];
    for (ALuint loop=0; loop < TMPI_NB_BUFFERS; loop++)
    {
      mALTMPIQueuedbuffersP[loop] = AL_FALSE;
    }

    // Set GAIN
    SetAudioGainTMPI(mSettingsP->AudioGainTMPI);

#endif /* _USE_OPENAL */

    InitOK = true;
  }
  while (0);

  return InitOK;
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::CreatePlayCityStream(void)
{
bool InitOK = false;

  do
  {
#ifdef _USE_OPENAL

    // Clear error code
    alGetError();

    // Crate emulator source
    alGenSources((ALuint)1, &mALPlayCitySource);
    ALenum ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGenSources Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreatePlayCityStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }
    if (mALPlayCitySource == 0)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("mALEmulatorSource NULL"), _T("TAudioContext::CreatePlayCityStream"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Create audio buffers
    mALPlayCityBuffersP = new ALuint[PLAYCITY_NB_BUFFERS];
    alGenBuffers(PLAYCITY_NB_BUFFERS, mALPlayCityBuffersP);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG

			String msg = _T("alGenBuffers Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreatePlayCityStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }

    mALPlayCityQueuedbuffersP = new ALboolean[PLAYCITY_NB_BUFFERS];
    for (ALuint loop=0; loop < PLAYCITY_NB_BUFFERS; loop++)
    {
      mALPlayCityQueuedbuffersP[loop] = AL_FALSE;
    }

    // Set GAIN
    SetAudioGainPlayCity(mSettingsP->AudioGainPlayCity);

#endif /* _USE_OPENAL */

    InitOK = true;
  }
  while (0);

  return InitOK;
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::UnqueueSoundBuffers(void)
{
  if (mALEmulatorSource)
  {
    UnqueueSoundBuffers(mALEmulatorSource,
                        mALEmulatorBuffersP,
                        mALEmulatorQueuedbuffersP,
                        EMULATOR_NB_BUFFERS);
  }

  if (mALTMPISource)
  {
    UnqueueSoundBuffers(mALTMPISource,
                        mALTMPIBuffersP,
                        mALTMPIQueuedbuffersP,
                        TMPI_NB_BUFFERS);
  }

  if (mALPlayCitySource)
  {
    UnqueueSoundBuffers(mALPlayCitySource,
                        mALPlayCityBuffersP,
                        mALPlayCityQueuedbuffersP,
                        PLAYCITY_NB_BUFFERS);
  }

  if (mALFloppyHeadSource)
  {
    UnqueueSoundBuffers(mALFloppyHeadSource,
                        mALFloppyHeadBuffersP,
                        mALFloppyHeadQueuedbuffersP,
                        FLOPPY_HEAD_NB_BUFFERS);
  }
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::UnqueueSoundBuffers(ALuint Source,
                                                   ALuint* BuffersP,
                                                   ALboolean* QueuedbuffersP,
                                                   ALuint NbBuffers)
{
#ifdef _USE_OPENAL

  do
  {
    // Clear error
    alGetError();

    // Get number of processed buffers
    ALint processed;
    alGetSourcei(Source, AL_BUFFERS_PROCESSED, &processed);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGetSourcei AL_BUFFERS_PROCESSED Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::UnqueueSoundBuffers"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    // Buffers to enqueue ?
    if (processed > 0)
    {
      // Get processed buffers
      ALuint* freebufferP = new ALuint[processed];

      alSourceUnqueueBuffers(Source, processed, freebufferP);
      Error = alGetError();
      if (Error != AL_NO_ERROR)
      {
#ifdef _DEBUG
				String msg = _T("alSourceUnqueueBuffers Error=") + String::IntToHex(Error,8);
				Application->MessageBox(msg.c_str(), _T("TAudioContext::UnqueueSoundBuffers"), MB_OK);
#endif /* _DEBUG */
        mLastError = Error;
        delete [] freebufferP;
        continue;
      }

      // Update queued state
      for (ALint processedloop=0; processedloop < processed; processedloop++)
      {
        for (ALuint bufferloop=0; bufferloop < NbBuffers; bufferloop++)
        {
          if (freebufferP[processedloop] == BuffersP[bufferloop])
          {
            QueuedbuffersP[bufferloop] = AL_FALSE;
          }
        }
      }
      
      delete [] freebufferP;
    }
  }
  while (0);
#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::FillEmulatorStream(const void* DataP, int DataSize)
{
bool BufferFilled = false;

#ifdef _USE_OPENAL

  do
  {
    // Clear error
    alGetError();
    
    // Get first free buffer
    ALuint buffer = 0;
    ALuint bufferindex = EMULATOR_NB_BUFFERS;
    for (ALuint loop=0; loop < EMULATOR_NB_BUFFERS; loop++)
    {
      if (mALEmulatorQueuedbuffersP[loop] == AL_FALSE)
      {
        buffer = mALEmulatorBuffersP[loop];
        bufferindex = loop;
        break;
      }
    }

    if (buffer == 0)
    {
      continue;
    }

    // STEREO ?
    ALenum AudioFormat;
    if (mSettingsP->AudioSpeakerStereo == true)
    {
      AudioFormat = (mSettingsP->AudioSpeaker16Bits == false) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }
    // MONO
    else
    {
      AudioFormat = (mSettingsP->AudioSpeaker16Bits == false) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    }
    // Create buffer
    alBufferData(buffer,
                 AudioFormat,
                 DataP,
                 DataSize,
                 mSettingsP->AudioSpeakerFreq);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alBufferData Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::FillEmulatorStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    alSourceQueueBuffers(mALEmulatorSource, 1, &buffer);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceQueueBuffers Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::FillEmulatorStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    // Set buffer as queued
    mALEmulatorQueuedbuffersP[bufferindex] = AL_TRUE;

    // Play source
    ALint val;
    alGetSourcei(mALEmulatorSource, AL_SOURCE_STATE, &val);
    if (val != AL_PLAYING)
    {
      alSourcePlay(mALEmulatorSource);
      Error = alGetError();
      if (Error != AL_NO_ERROR)
      {
#ifdef _DEBUG
				String msg = _T("alSourcePlay Error=") + String::IntToHex(Error,8);
				Application->MessageBox(msg.c_str(), _T("TAudioContext::FillEmulatorStream"), MB_OK);
#endif /* _DEBUG */
        mLastError = Error;
        continue;
      }
    }

    BufferFilled = true;
  }
  while (0);

#endif /* _USE_OPENAL */

  return (BufferFilled);
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::FillTMPIStream(const void* DataP, int DataSize)
{
bool BufferFilled = false;

#ifdef _USE_OPENAL

  do
  {
    // Clear error
    alGetError();
    
    // Get first free buffer
    ALuint buffer = 0;
    ALuint bufferindex = TMPI_NB_BUFFERS;
    for (ALuint loop=0; loop < TMPI_NB_BUFFERS; loop++)
    {
      if (mALTMPIQueuedbuffersP[loop] == AL_FALSE)
      {
        buffer = mALTMPIBuffersP[loop];
        bufferindex = loop;
        break;
      }
    }

    if (buffer == 0)
    {
      continue;
    }

    // MONO 16 bits
    alBufferData(buffer,
                 AL_FORMAT_MONO16,
                 DataP,
                 DataSize,
                 mSettingsP->AudioTMPIFreq);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alBufferData Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::FillTMPIStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    alSourceQueueBuffers(mALTMPISource, 1, &buffer);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceQueueBuffers Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::FillTMPIStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    // Set buffer as queued
    mALTMPIQueuedbuffersP[bufferindex] = AL_TRUE;

    // Play source
    ALint val;
    alGetSourcei(mALTMPISource, AL_SOURCE_STATE, &val);
    if (val != AL_PLAYING)
    {
      alSourcePlay(mALTMPISource);
      Error = alGetError();
      if (Error != AL_NO_ERROR)
      {
#ifdef _DEBUG
				String msg = _T("alSourcePlay Error=") + String::IntToHex(Error,8);
				Application->MessageBox(msg.c_str(), _T("TAudioContext::FillTMPIStream"), MB_OK);
#endif /* _DEBUG */
        mLastError = Error;
        continue;
      }
    }

    BufferFilled = true;
  }
  while (0);

#endif /* _USE_OPENAL */

  return (BufferFilled);
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::FillPlayCityStream(const void* DataP, int DataSize)
{
bool BufferFilled = false;

#ifdef _USE_OPENAL

  do
  {
    // Clear error
    alGetError();
    
    // Get first free buffer
    ALuint buffer = 0;
    ALuint bufferindex = PLAYCITY_NB_BUFFERS;
    for (ALuint loop=0; loop < PLAYCITY_NB_BUFFERS; loop++)
    {
      if (mALPlayCityQueuedbuffersP[loop] == AL_FALSE)
      {
        buffer = mALPlayCityBuffersP[loop];
        bufferindex = loop;
        break;
      }
    }

    if (buffer == 0)
    {
      continue;
    }

    // STEREO ?
    ALenum AudioFormat;
    if (mSettingsP->AudioPlayCityStereo == true)
    {
      AudioFormat = (mSettingsP->AudioPlayCity16Bits == false) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }
    // MONO
    else
    {
      AudioFormat = (mSettingsP->AudioPlayCity16Bits == false) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    }
    // Create buffer
    alBufferData(buffer,
                 AudioFormat,
                 DataP,
                 DataSize,
                 mSettingsP->AudioPlayCityFreq);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alBufferData Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::FillPlayCityStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    alSourceQueueBuffers(mALPlayCitySource, 1, &buffer);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceQueueBuffers Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::FillPlayCityStream"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    // Set buffer as queued
    mALPlayCityQueuedbuffersP[bufferindex] = AL_TRUE;

    // Play source
    ALint val;
    alGetSourcei(mALPlayCitySource, AL_SOURCE_STATE, &val);
    if (val != AL_PLAYING)
    {
      alSourcePlay(mALPlayCitySource);
      Error = alGetError();
      if (Error != AL_NO_ERROR)
      {
#ifdef _DEBUG
				String msg = _T("alSourcePlay Error=") + String::IntToHex(Error,8);
				Application->MessageBox(msg.c_str(), _T("TAudioContext::FillPlayCityStream"), MB_OK);
#endif /* _DEBUG */
        mLastError = Error;
        continue;
      }
    }

    BufferFilled = true;
  }
  while (0);

#endif /* _USE_OPENAL */

  return (BufferFilled);
}
//---------------------------------------------------------------------------


#ifdef _USE_OPENAL
void __fastcall TAudioContext::CreateResourceSource(String SoundName,
																										ALuint* SoundSourceP,
																										ALuint* SoundBufferP)
{
  *SoundSourceP = NULL;
  *SoundBufferP = NULL;

  do
  {
    // Clear error
    alGetError();

    HRSRC Resource = FindResource(HInstance, SoundName.c_str(), RT_RCDATA);
    if (Resource == NULL)
    {
      continue;
    }

    HGLOBAL Address = LoadResource(HInstance, Resource);
    if (Address == NULL)
    {
      continue;
    }

    tWAVE_file_header* WavFile = (tWAVE_file_header*)LockResource(Address);

    if (*((long*)&WavFile->RIFF_Header.format[0]) != *((long*)&("WAVE")))
    {
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }

    // Check PCM data format
    if (WavFile->WAVE_Format.audioFormat != 1)
    {
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }

    // Get sound format
    ALenum SoundFormat;
    if (WavFile->WAVE_Format.numChannels == 1)
    {
      if (WavFile->WAVE_Format.bitsPerSample == 8)
      {
        SoundFormat = AL_FORMAT_MONO8;
      }
      else
      {
        SoundFormat = AL_FORMAT_MONO16;
      }
    }
    else
    {
      if (WavFile->WAVE_Format.bitsPerSample == 8)
      {
        SoundFormat = AL_FORMAT_STEREO8;
      }
      else
      {
        SoundFormat = AL_FORMAT_STEREO16;
      }
    }

    // Create sound source
    alGenSources((ALuint)1, SoundSourceP);
    ALenum ALError = alGetError();
		if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGenSources Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::LoadResourceSound"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }
    if (SoundSourceP == 0)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("mALsource NULL"), _T("TAudioContext::LoadResourceSound"), MB_OK);
#endif /* _DEBUG */
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }

    // Create sound buffer
    alGenBuffers((ALuint)1, SoundBufferP);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alGenBuffers Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::LoadResourceSound"), MB_OK);
#endif /* _DEBUG */
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }

    // Put sound data into buffer
    alBufferData(*SoundBufferP,
                 SoundFormat,
                 &WavFile->WAVE_Data.ChunkData,
                 WavFile->WAVE_Data.subChunk2Size,
                 WavFile->WAVE_Format.sampleRate);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alBufferData Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::LoadResourceSound"), MB_OK);
#endif /* _DEBUG */
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }

    // Bind buffer to source
    alSourcei(*SoundSourceP, AL_BUFFER, *SoundBufferP);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
      mLastError = ALError;
#ifdef _DEBUG
			String msg = _T("alSourcei AL_BUFFER Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::LoadResourceSound"), MB_OK);
#endif /* _DEBUG */
    }

    UnlockResource((VOID*)WavFile);
    FreeResource(Address);
  }
  while (0);
}
//---------------------------------------------------------------------------
#endif /* _USE_OPENAL */


#ifdef _USE_OPENAL
void __fastcall TAudioContext::LoadResourceData(String SoundName,
																								unsigned char ** DataP,
																								ALenum* FormatP,
																								ALsizei* FreqP,
																								ALsizei* SizeP)
{
  *DataP = NULL;
  *FormatP = 0;
  *SizeP = 0;

  do
  {
    HRSRC Resource = FindResource(HInstance, SoundName.c_str(), RT_RCDATA);
    if (Resource == NULL)
    {
      continue;
    }

    HGLOBAL Address = LoadResource(HInstance, Resource);
    if (Address == NULL)
    {
      continue;
    }

    tWAVE_file_header* WavFile = (tWAVE_file_header*)LockResource(Address);

    if (*((long*)&WavFile->RIFF_Header.format[0]) != *((long*)&("WAVE")))
    {
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }

    // Check PCM data format
    if (WavFile->WAVE_Format.audioFormat != 1)
    {
      UnlockResource((VOID*)WavFile);
      FreeResource(Address);
      continue;
    }

    // Get sound format
    if (WavFile->WAVE_Format.numChannels == 1)
    {
      if (WavFile->WAVE_Format.bitsPerSample == 8)
      {
        *FormatP = AL_FORMAT_MONO8;
      }
      else
      {
        *FormatP = AL_FORMAT_MONO16;
      }
    }
    else
    {
      if (WavFile->WAVE_Format.bitsPerSample == 8)
      {
        *FormatP = AL_FORMAT_STEREO8;
      }
      else
      {
        *FormatP = AL_FORMAT_STEREO16;
      }
    }

    // Get sound size
    *SizeP = WavFile->WAVE_Data.subChunk2Size;

    // Get sound frequency
    *FreqP = WavFile->WAVE_Format.sampleRate;

    // Create sound data buffer
    *DataP = new unsigned char[*SizeP];

    // Copy sound to buffer
    memcpy(*DataP, &WavFile->WAVE_Data.ChunkData, *SizeP);

    UnlockResource((VOID*)WavFile);
    FreeResource(Address);
  }
  while (0);
}
//---------------------------------------------------------------------------
#endif /* _USE_OPENAL */


void __fastcall TAudioContext::PlayFloppyMotor(void)
{
#ifdef _USE_OPENAL

  ALint val;
  alGetSourcei(mALFloppyMotorSource, AL_SOURCE_STATE, &val);
  if (val != AL_PLAYING)
  {
    alGetError();
	alSourcePlay(mALFloppyMotorSource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::PlayFloppyMotor"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------
void __fastcall TAudioContext::StopFloppyMotor(void)
{
#ifdef _USE_OPENAL

  alGetError();
  alSourceStop(mALFloppyMotorSource);
  ALenum Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourceStop Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::StopFloppyMotor"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


bool __fastcall TAudioContext::CreateFloppyHeadSource(void)
{
bool InitOK = false;

  do
  {
#ifdef _USE_OPENAL

    // Clear error
    alGetError();
    
    // Crate floppy head source
    alGenSources((ALuint)1, &mALFloppyHeadSource);
    ALenum ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGenSources Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreateFloppyHeadSource"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }
    if (mALFloppyHeadSource == 0)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("mALFloppyHeadSource NULL"), _T("TAudioContext::CreateFloppyHeadSource"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Create audio buffers
    mALFloppyHeadBuffersP = new ALuint[FLOPPY_HEAD_NB_BUFFERS];
    alGenBuffers(FLOPPY_HEAD_NB_BUFFERS, mALFloppyHeadBuffersP);
    ALError = alGetError();
    if (ALError != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alGenBuffers Error=") + String::IntToHex(ALError,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::CreateFloppyHeadSource"), MB_OK);
#endif /* _DEBUG */
      mLastError = ALError;
      continue;
    }

    mALFloppyHeadQueuedbuffersP = new ALboolean[FLOPPY_HEAD_NB_BUFFERS];
    for (ALuint loop=0; loop < FLOPPY_HEAD_NB_BUFFERS; loop++)
    {
      mALFloppyHeadQueuedbuffersP[loop] = AL_FALSE;
    }

#endif /* _USE_OPENAL */

    InitOK = true;
  }
  while (0);

  return InitOK;
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::PlayFloppyHead(int NbSteps)
{
#ifdef _USE_OPENAL

unsigned char* BufferData = NULL;

  do
  {
    // Clear error
    alGetError();

    // Get first free buffer
    ALuint buffer = 0;
    ALuint bufferindex = FLOPPY_HEAD_NB_BUFFERS;
    for (ALuint loop=0; loop < FLOPPY_HEAD_NB_BUFFERS; loop++)
    {
      if (mALFloppyHeadQueuedbuffersP[loop] == AL_FALSE)
      {
        buffer = mALFloppyHeadBuffersP[loop];
        bufferindex = loop;
        break;
      }
    }

    if (buffer == 0)
    {
      continue;
    }

    // Create entire head steps buffer
    ALsizei BufferSize = mFloppyHeadSampleSize * NbSteps;
    BufferData = new unsigned char [BufferSize];

    // Copy head sample
    unsigned char* TempData = BufferData;
    for (int loop=0; loop < NbSteps; loop++)
    {
      memcpy(TempData, mFloppyHeadSampleData, mFloppyHeadSampleSize);
      TempData += mFloppyHeadSampleSize;
    }

    // Create sound buffer
    alBufferData(buffer,
                 mFloppyHeadSampleFormat,
                 BufferData,
                 BufferSize,
                 mFloppyHeadSampleFreq);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alBufferData Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::PlayFloppyHead"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    alSourceQueueBuffers(mALFloppyHeadSource, 1, &buffer);
    Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceQueueBuffers Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::PlayFloppyHead"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
      continue;
    }

    // Set buffer as queued
    mALFloppyHeadQueuedbuffersP[bufferindex] = AL_TRUE;

    // Play source
    ALint val;
    alGetSourcei(mALFloppyHeadSource, AL_SOURCE_STATE, &val);
    if (val != AL_PLAYING)
    {
      alSourcePlay(mALFloppyHeadSource);
      Error = alGetError();
      if (Error != AL_NO_ERROR)
      {
#ifdef _DEBUG
				String msg = _T("alSourcePlay Error=") + String::IntToHex(Error,8);
				Application->MessageBox(msg.c_str(), _T("TAudioContext::PlayFloppyHead"), MB_OK);
#endif /* _DEBUG */
        mLastError = Error;
        continue;
      }
    }
  }
  while (0);

  delete [] BufferData;

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------

void __fastcall TAudioContext::SetAudioVolume(const float Value)
{
#ifdef _USE_OPENAL

  // Clear error
  alGetError();

  alListenerf(AL_GAIN, Value / 100.f);

  ALenum ALError = alGetError();
  if (ALError != AL_NO_ERROR)
  {
    mLastError = ALError;
#ifdef _DEBUG
		String msg = _T("alListenerf AL_GAIN Error=") + String::IntToHex(ALError,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioVolume"), MB_OK);
#endif /* _DEBUG */
  }

#endif /* _USE_OPENAL */
}
//------------------------------------------------------------------------------

void __fastcall TAudioContext::SetAudioGainSpeaker(const float Value)
{
#ifdef _USE_OPENAL

  // Clear error
  alGetError();

  alSourcef(mALEmulatorSource, AL_GAIN, Value / 100.f);

  ALenum Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourcef AL_GAIN Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioGainSpeaker"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

#endif /* _USE_OPENAL */
}
//------------------------------------------------------------------------------

void __fastcall TAudioContext::SetAudioGainTMPI(const float Value)
{
#ifdef _USE_OPENAL

  // Clear error
  alGetError();

  alSourcef(mALTMPISource, AL_GAIN, Value / 100.f);

  ALenum Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourcef AL_GAIN Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioGainTMPI"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

#endif /* _USE_OPENAL */
}
//------------------------------------------------------------------------------

void __fastcall TAudioContext::SetAudioGainPlayCity(const float Value)
{
#ifdef _USE_OPENAL

  // Clear error
  alGetError();

  alSourcef(mALPlayCitySource, AL_GAIN, Value / 100.f);

  ALenum Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourcef AL_GAIN Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioGainPlayCity"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

#endif /* _USE_OPENAL */
}
//------------------------------------------------------------------------------

void __fastcall TAudioContext::SetAudioGainFloppy(const float Value)
{
#ifdef _USE_OPENAL

  // Clear error
  alGetError();

  alSourcef(mALFloppyMotorSource, AL_GAIN, Value / 100.f);
  alSourcef(mALFloppyHeadSource, AL_GAIN, Value / 100.f);

  ALenum Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourcef AL_GAIN Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioGainFloppy"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

#endif /* _USE_OPENAL */
}
//------------------------------------------------------------------------------


void __fastcall TAudioContext::SetAudioGainTape(const float Value)
{
#ifdef _USE_OPENAL

  // Clear error
  alGetError();

  alSourcef(mALTapeMotorSource, AL_GAIN, Value / 100.f);
  ALenum Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourcef TapeMotorSource AL_GAIN Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioGainTape"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

  alSourcef(mALTapeRelaySource, AL_GAIN, Value / 100.f);
  Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourcef TapeRelaySource AL_GAIN Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioGainTape"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

  alSourcef(mALTapeDoubleRelaySource, AL_GAIN, Value / 100.f);
  Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourcef TapeDoubleRelaySource AL_GAIN Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::SetAudioGainTape"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }

#endif /* _USE_OPENAL */
}
//------------------------------------------------------------------------------


void __fastcall TAudioContext::PlayTapeMotor(void)
{
#ifdef _USE_OPENAL

  ALint val;
  alGetSourcei(mALTapeMotorSource, AL_SOURCE_STATE, &val);
  if (val != AL_PLAYING)
  {
    alGetError();
    alSourcePlay(mALTapeMotorSource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::PlayTapeMotor"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------
void __fastcall TAudioContext::StopTapeMotor(void)
{
#ifdef _USE_OPENAL

  alGetError();
  alSourceStop(mALTapeMotorSource);
  ALenum Error = alGetError();
  if (Error != AL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("alSourceStop Error=") + String::IntToHex(Error,8);
		Application->MessageBox(msg.c_str(), _T("TAudioContext::StopTapeMotor"), MB_OK);
#endif /* _DEBUG */
    mLastError = Error;
  }
  
#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::PlayTapeRelay(void)
{
#ifdef _USE_OPENAL

  ALint val;
  alGetSourcei(mALTapeRelaySource, AL_SOURCE_STATE, &val);
  if (val != AL_PLAYING)
  {
    alGetError();
    alSourcePlay(mALTapeRelaySource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::PlayTapeRelay"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------
void __fastcall TAudioContext::PlayTapeDoubleRelay(void)
{
#ifdef _USE_OPENAL

  ALint val;
  alGetSourcei(mALTapeDoubleRelaySource, AL_SOURCE_STATE, &val);
  if (val != AL_PLAYING)
  {
    alGetError();
    alSourcePlay(mALTapeDoubleRelaySource);
    ALenum Error = alGetError();
    if (Error != AL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("alSourceStop Error=") + String::IntToHex(Error,8);
			Application->MessageBox(msg.c_str(), _T("TAudioContext::PlayTapeDoubleRelay"), MB_OK);
#endif /* _DEBUG */
      mLastError = Error;
    }
  }

#endif /* _USE_OPENAL */
}
//---------------------------------------------------------------------------


tUChar* __fastcall TAudioContext::ConvertWAVToCDT(AnsiString Filename,
                                                  tULong* LengthP)
{
  if (FileExists(Filename) == false)
    return 0;

  // Ouverture du fichier en lecture
  int Handle = FileOpen(Filename, fmOpenRead);
  tUChar* pbTapeImage = 0;

  // Create DEBUG WAV File
  AnsiString DebugFilename = ChangeFileExt(Filename, ".debug.wav");
  int DebugHandle = 0;
  if (mTapeCreateOutputAudioFile == true)
  {
    DeleteFile(DebugFilename);
    DebugHandle = FileCreate(DebugFilename);
  }

  // Create CDT File
  AnsiString CDTFilename = ChangeFileExt(Filename, ".cdt");
  int CDTHandle = 0;
  if (mTapeCreateCDTFile == true)
  {
    DeleteFile(CDTFilename);
    CDTHandle = FileCreate(CDTFilename);
  }

  do
  {
    // Read RIFF Header
    tRIFF_Header RIFF_Header;
    int LengthToRead = sizeof(tRIFF_Header);
    if (FileRead(Handle, &RIFF_Header, LengthToRead) != LengthToRead)
    {
      continue;
    }

    int HeaderLength = LengthToRead;

    // Copy WAVE header
    if (DebugHandle)
    {
      FileWrite(DebugHandle, &RIFF_Header, LengthToRead);
    }

    // Check Header
    if (*((long*)&RIFF_Header.format[0]) != *((long*)&("WAVE")))
    {
      continue;
    }

    tUChar SampleLength;

    // Read chunk until data
    bool DataFound = false;
    bool FormatFound = false;
    tRIFF_Chunk_Header Chunk_Header;
    tWAVE_Format WAVE_Format;
    do
    {
      // Read next chunk header
      LengthToRead = sizeof(tRIFF_Chunk_Header);
      if (FileRead(Handle, &Chunk_Header, LengthToRead) != LengthToRead)
      {
        break;
      }
      
      // Copy Chunk header
      if (DebugHandle)
      {
        FileWrite(DebugHandle, &Chunk_Header, LengthToRead);
      }

      HeaderLength += LengthToRead;

      // Format chunk
      if (*((long*)&Chunk_Header.chunkID[0]) == *((long*)&("fmt ")))
      {
        // Read chunk data
        LengthToRead = Chunk_Header.chunkSize;
        if (FileRead(Handle, &WAVE_Format, LengthToRead) != LengthToRead)
        {
          break;
        }
        // Copy Chunk data
        if (DebugHandle)
        {
          FileWrite(DebugHandle, &WAVE_Format, LengthToRead);
        }
        HeaderLength += LengthToRead;

        // Check PCM data format
        if (WAVE_Format.audioFormat != 1)
        {
          break;
        }

        SampleLength = (tUChar)((WAVE_Format.bitsPerSample / 8) * WAVE_Format.numChannels);
        FormatFound = true;
      }
      // Data chunk
      else if (*((long*)&Chunk_Header.chunkID[0]) == *((long*)&("data")))
      {
        DataFound = true;
        break;
      }
      // Other chunks
      else
      {
        // Allocate memory for chunk data
        tUChar* ChunkData = (tUChar*)malloc(Chunk_Header.chunkSize);
        LengthToRead = Chunk_Header.chunkSize;
        if (FileRead(Handle, ChunkData, LengthToRead) != LengthToRead)
        {
          free(ChunkData);
          break;
        }
        // Copy Chunk data
        if (DebugHandle)
        {
          FileWrite(DebugHandle, ChunkData, LengthToRead);
        }
        HeaderLength += LengthToRead;
        free(ChunkData);
      }
    }
    while (1);
    if (FormatFound == false) continue;
    if (DataFound == false) continue;

    // Read data length
    tUChar* pbAudioData = (tUChar*)malloc(Chunk_Header.chunkSize);
    if (!pbAudioData) continue;
    if (FileRead(Handle, pbAudioData, Chunk_Header.chunkSize) != Chunk_Header.chunkSize)
    {
      free(pbAudioData);
      continue;
    }

    // Allocate memory for debug samples
    tULong DebugAudioDataLength = RIFF_Header.chunkSize+8-HeaderLength;
    tUChar* pbDebugAudioData = 0;
    if (DebugHandle)
    {
      pbDebugAudioData = (tUChar*)malloc(DebugAudioDataLength);
    }

    long dwTapePulseCycles = 3500000L / WAVE_Format.sampleRate;
    tULong NbSamples = Chunk_Header.chunkSize / SampleLength;
    long dwCompressedSize = NbSamples >> 3; // 1 sample = 1 bit. 

    // Allocate memory for Tape blocks
    *LengthP = TAPE_CDT_HEADER_SIZE+3+9+dwCompressedSize;
    pbTapeImage = (tUChar*)malloc(*LengthP);
    tUChar* pbTapeImagePtr = pbTapeImage;
    if (!pbTapeImage)
    {
      free(pbAudioData);
      continue;
    }

    // Create CDT Header
    memcpy(pbTapeImagePtr, "ZXTape!\032", 8);
    *(pbTapeImagePtr+8) = 1;
    pbTapeImagePtr += TAPE_CDT_HEADER_SIZE;

    // Create First Pause block
    *pbTapeImagePtr = 0x20; // start off with a pause block
    *(tUShort*)(pbTapeImagePtr+1) = 2000; // set the length to 2 seconds
    pbTapeImagePtr += 3;

    // Create direct recording block
    *(pbTapeImagePtr) = 0x15; // direct recording block
    *(tUShort*)(pbTapeImagePtr+1) = (tUShort)dwTapePulseCycles; // number of T states per sample
    *(tUShort*)(pbTapeImagePtr+3) = 2000; // pause after block
    *(pbTapeImagePtr+5) = dwCompressedSize & 7 ? dwCompressedSize & 7 : 8; // bits used in last byte
    *(tULong*)(pbTapeImagePtr+6) = dwCompressedSize & 0x00ffffff; // data length
    pbTapeImagePtr += 9;

    tULong dwBit = 8;
    tUChar bByte = 0;
    
    //
    // PCM 16 bits
    //
    if (WAVE_Format.bitsPerSample == 16)
    {
      // Get maximum peak
      tShort Maximum;
      if (WAVE_Format.numChannels == 2)
      {
        Maximum = GetAudioMaximum16BitsStereo((tShort*)pbAudioData, NbSamples);
      }
      else
      {
        Maximum = GetAudioMaximum16BitsMono((tShort*)pbAudioData, NbSamples);
      }

      //
      // Create bits array
      //
      tShort* SamplePtr = (tShort*)pbAudioData;
      tULong SampleLoop = NbSamples;
      tShort* DebugSamplePtr = (tShort*)pbDebugAudioData;

      float AutoGain = 32760.f / (float)Maximum;
      InitTapeLowFilter1(*SamplePtr);
      InitButterworthLPF(WAVE_Format.sampleRate);
      InitButterworthHPF(WAVE_Format.sampleRate);

      while (SampleLoop--)
      {
        dwBit--;

        // Left/Mono channel first
        tShort Sample = *(SamplePtr++);
        // Ignore Right channel
        if (WAVE_Format.numChannels == 2) SamplePtr++;

        //
        // Audio filtering
        //
        float fSample = (float)Sample;

        // Automatic Gain
        if (mTapeAutoGainActive == true)
        {
          fSample *= AutoGain;
        }

        // Low Pass Filter 1
        if (mTapeLPF1Active == true)
        {
          fSample = ComputeTapeLowFilter1(fSample);
        }

        // Low Pass Filter 2
        if (mTapeLPF2Active == true)
        {
          fSample = ComputeButterworthLPF(fSample);
        }

        // High Pass Filter 2
        if (mTapeHPF2Active == true)
        {
          fSample = ComputeButterworthHPF(fSample);
        }

        // Integer Saturation
        if (fSample > 32767.) Sample = 32767;
        else if (fSample < -32768.) Sample = -32768;
        else Sample = (tShort)fSample;

        // PPI Level
        tUChar PPILevel=0;
        if (Sample > mTapePPIThreshold16) PPILevel=1;
        bByte |= (tUChar)(PPILevel << dwBit);

        // got all 8 bits?
        if (!dwBit)
        {
          *pbTapeImagePtr++ = bByte;
          dwBit = 8;
          bByte = 0;
        }

        if (DebugHandle)
        {
          *(DebugSamplePtr++) = Sample;
          if (WAVE_Format.numChannels == 2)
          {
            *(DebugSamplePtr++) = PPILevel ? 9600 : -9600;
          }
        }
      }

      FinalizeButterworthLPF();
      FinalizeButterworthHPF();
    }

    //
    // PCM 8 bits
    //
    else if (WAVE_Format.bitsPerSample == 8)
    {
      // Create bits array
      tUChar* SamplePtr = (tUChar*)pbAudioData;
      tUChar* DebugSamplePtr = pbDebugAudioData;
      tULong SampleLoop = NbSamples;
      while (SampleLoop--)
      {
        dwBit--;

        // Left/Mono channel first
        tUChar Sample = *(SamplePtr++);
        // Ignore Right channel
        if (WAVE_Format.numChannels == 2) SamplePtr++;

        // PPI Level
        tUChar PPILevel=0;
        if (Sample > mTapePPIThreshold8) PPILevel=1;
        bByte |= (tUChar)(PPILevel << dwBit);

        // got all 8 bits?
        if (!dwBit)
        {
          *pbTapeImagePtr++ = bByte;
          dwBit = 8;
          bByte = 0;
        }

        if (DebugHandle)
        {
          *(DebugSamplePtr++) = Sample;
          if (WAVE_Format.numChannels == 2)
          {
            *(DebugSamplePtr++) = PPILevel ? 160 : 96;
          }
        }
      }
    }

    // Ignore others
    else
    {
      delete [] pbTapeImage;
      pbTapeImage = 0;
    }

    if (DebugHandle)
    {
      // Write debug audio data
      FileWrite(DebugHandle, pbDebugAudioData, DebugAudioDataLength);
    }

    free(pbAudioData);
    free(pbDebugAudioData);
  }
  while (0);

  FileClose(Handle);
  if (DebugHandle)
  {
    FileClose(DebugHandle);
    // Notify user
    if (mQuietMode == false)
    {
		  String Message = _T("Audio debug file:\r\n")
			  							 + String(DebugFilename)
				  						 + _T("\r\nhas just been created.");
		  Application->MessageBox(Message.c_str(), _T("Debug"), MB_OK);
    }
	}
	if (CDTHandle)
	{
		// Write debug audio data
		FileWrite(CDTHandle, pbTapeImage, *LengthP);
		// Close file
		FileClose(CDTHandle);
		// Notify user
    if (mQuietMode == false)
    {
		  String Message = _T("CDT file:\r\n")
			  							 + String(CDTFilename)
				  						 + _T("\r\nhas just been created.");
		  Application->MessageBox(Message.c_str(), _T("Debug"), MB_OK);
    }
	}

  return pbTapeImage;
}
//---------------------------------------------------------------------------
tShort __fastcall TAudioContext::GetAudioMaximum16BitsStereo(tShort* DataP, tULong NbSamples)
{
  tShort Maximum = 0;

  while (NbSamples--)
  {
    // Left/Mono channel first
    tShort Sample = *(DataP++);
    // Ignore Right channel
    DataP++;

    if (Sample > Maximum)
    {
      Maximum = Sample;
    }
  }

  return (Maximum);
}
//---------------------------------------------------------------------------
tShort __fastcall TAudioContext::GetAudioMaximum16BitsMono(tShort* DataP, tULong NbSamples)
{
  tShort Maximum = 0;

  while (NbSamples--)
  {
    // Mono channel first
    tShort Sample = *(DataP++);

    if (Sample > Maximum)
    {
      Maximum = Sample;
    }
  }

  return (Maximum);
}
//---------------------------------------------------------------------------
tUChar* __fastcall TAudioContext::ConvertWAVToCSW(AnsiString Filename,
                                                  tULong* LengthP)
{
  if (Filename == "")
    return 0;

  // Archive Container
  AnsiString InArchiveFilename = "";
  if (Filename.LowerCase().Pos(CONTAINER_ZIP_EXT))
  {
    int ExtensionPos = Filename.LowerCase().Pos(CONTAINER_ZIP_EXT);
    InArchiveFilename = Filename.SubString(ExtensionPos+5, Filename.Length());
    Filename = Filename.SubString(1, ExtensionPos+3);
  }

  if (FileExists(Filename) == false)
    return 0;

  // Ouverture du fichier en lecture
  int Handle = FileOpen(Filename, fmOpenRead);

  AnsiString ContainerFilename = Filename;
  // Met le curseur à la fin du fichier pour récupérer la taille du fichier
  int DataLength = FileSeek(Handle, 0, 2);
  // Remet le curseur en début de fichier pour la prochaine lecture
  FileSeek(Handle, 0, 0);
  tUChar* DataP = (tUChar*)malloc(DataLength);
  tUChar* DataEndP = DataP + DataLength;
  // Read file
  FileRead(Handle, DataP, DataLength);
  FileClose(Handle);

  // Load file from archive container
  if (InArchiveFilename.IsEmpty() == false)
  {
    TZIPContainer* ZIPContainer = new TZIPContainer(Filename);

    // Allocate memory
    int UncompressedSize = ZIPContainer->GetFileUncompressedSize(InArchiveFilename);
    tUChar* FileData = (tUChar*)malloc(UncompressedSize);

    // Read file from archive
    if (ZIPContainer->ReadFile(InArchiveFilename, FileData, UncompressedSize) != UNZ_OK)
    {
      free(FileData);
      return 0;
    }

    delete ZIPContainer;

    // Prepare disk operation
    free(DataP);
    DataP = FileData;
    DataLength = UncompressedSize;
    DataEndP = DataP + DataLength;
    Filename = InArchiveFilename;
  }

  tUChar* pbTapeImage = 0;

  // Create DEBUG WAV File
  AnsiString DebugFilename = ChangeFileExt(Filename, ".debug.wav");
  if (InArchiveFilename.IsEmpty() == false)
  {
    DebugFilename = ExtractFilePath(ContainerFilename);
    DebugFilename += Filename;
    DebugFilename += ".debug.wav";
  }
  int DebugHandle = 0;
  if (mTapeCreateOutputAudioFile == true)
  {
    DeleteFile(DebugFilename);
    DebugHandle = FileCreate(DebugFilename);
  }

  // Create CSW File
  AnsiString CSWFilename = ChangeFileExt(Filename, ".csw");
  if (InArchiveFilename.IsEmpty() == false)
  {
    CSWFilename = ExtractFilePath(ContainerFilename);
    CSWFilename += Filename;
    CSWFilename += ".csw";
  }
  int CSWHandle = 0;
  if (mTapeCreateCSWFile == true)
  {
    DeleteFile(CSWFilename);
    CSWHandle = FileCreate(CSWFilename);
  }

  tUChar* CurrentDataP = DataP;

  do
  {
    // Read RIFF Header
    tRIFF_Header* RIFF_Header;
    int LengthToRead = sizeof(tRIFF_Header);
    if ((DataEndP - CurrentDataP) < LengthToRead)
    {
      continue;
    }
    RIFF_Header = (tRIFF_Header*)CurrentDataP;
    CurrentDataP += LengthToRead;

    int HeaderLength = LengthToRead;

    // Copy WAVE header
    if (DebugHandle)
    {
      FileWrite(DebugHandle, RIFF_Header, LengthToRead);
    }

    // Check Header
    if (*((long*)&RIFF_Header->format[0]) != *((long*)&("WAVE")))
    {
      continue;
    }

    tUChar SampleLength;

    // Read chunk until data
    bool DataFound = false;
    bool FormatFound = false;
    tRIFF_Chunk_Header* Chunk_Header;
    tWAVE_Format* WAVE_Format;
    do
    {
      // Read next chunk header
      LengthToRead = sizeof(tRIFF_Chunk_Header);
      if ((DataEndP - CurrentDataP) < LengthToRead)
      {
        break;
      }
      Chunk_Header = (tRIFF_Chunk_Header*)CurrentDataP;
      CurrentDataP += LengthToRead;

      // Copy Chunk header
      if (DebugHandle)
      {
        FileWrite(DebugHandle, Chunk_Header, LengthToRead);
      }

      HeaderLength += LengthToRead;

      // Format chunk
      if (*((long*)&Chunk_Header->chunkID[0]) == *((long*)&("fmt ")))
      {
        // Read chunk data
        LengthToRead = Chunk_Header->chunkSize;
        if ((DataEndP - CurrentDataP) < LengthToRead)
        {
          break;
        }
        WAVE_Format = (tWAVE_Format*)CurrentDataP;
        CurrentDataP += LengthToRead;
        // Copy Chunk data
        if (DebugHandle)
        {
          FileWrite(DebugHandle, WAVE_Format, LengthToRead);
        }
        HeaderLength += LengthToRead;

        // Check PCM data format
        if (WAVE_Format->audioFormat != 1)
        {
          break;
        }

        SampleLength = (tUChar)((WAVE_Format->bitsPerSample / 8) * WAVE_Format->numChannels);
        FormatFound = true;
      }
      // Data chunk
      else if (*((long*)&Chunk_Header->chunkID[0]) == *((long*)&("data")))
      {
        DataFound = true;
        break;
      }
      // Other chunks
      else
      {
        // Allocate memory for chunk data
        LengthToRead = Chunk_Header->chunkSize;
        if ((DataEndP - CurrentDataP) < LengthToRead)
        {
          break;
        }
        tUChar* ChunkData = CurrentDataP;
        CurrentDataP += LengthToRead;
        // Copy Chunk data
        if (DebugHandle)
        {
          FileWrite(DebugHandle, ChunkData, LengthToRead);
        }
        HeaderLength += LengthToRead;
      }
    }
    while (1);
    if (FormatFound == false) continue;
    if (DataFound == false) continue;

    if ((DataEndP - CurrentDataP) < Chunk_Header->chunkSize)
    {
      continue;
    }
    tUChar* pbAudioData = CurrentDataP;
    //CurrentDataP += Chunk_Header->chunkSize;

    // Allocate memory for debug samples
    tULong DebugAudioDataLength = RIFF_Header->chunkSize+8-HeaderLength;
    tUChar* pbDebugAudioData = 0;
    if (DebugHandle)
    {
      pbDebugAudioData = (tUChar*)malloc(DebugAudioDataLength);
    }

    tULong NbSamples = Chunk_Header->chunkSize / SampleLength;

    // Allocate memory for CSW v1.1
    pbTapeImage = (tUChar*)malloc(*LengthP+NbSamples); // Allocate more than expected
    tUChar* pbTapeImagePtr = pbTapeImage;
    if (!pbTapeImage)
    {
      continue;
    }

    // Create CSW V1.1 Header
    memcpy(pbTapeImagePtr, "Compressed Square Wave", 22);
    *(pbTapeImagePtr + 0x16) = 0x1A; // Terminator code
    *(pbTapeImagePtr + 0x17) = 0x01; // Major revision
    *(pbTapeImagePtr + 0x18) = 0x01; // Minor revision
    *(pbTapeImagePtr + 0x19) = WAVE_Format->sampleRate & 0xFF; // Sample Rate
    *(pbTapeImagePtr + 0x1A) = (WAVE_Format->sampleRate >> 8) & 0xFF; // Sample Rate
    *(pbTapeImagePtr + 0x1B) = 0x01; // Compression type RLE
    *(pbTapeImagePtr + 0x1C) = 0x00; // Flags
    *(pbTapeImagePtr + 0x1D) = 0x00;
    *(pbTapeImagePtr + 0x1E) = 0x00;
    *(pbTapeImagePtr + 0x1F) = 0x00;
    pbTapeImagePtr += TAPE_CSW_V1_HEADER_SIZE;

    bool firstPulseGiven = false;
    tULong CSWSampleCount = 1; // First pulse
    tUChar LastSampleLevel;

    //
    // PCM 16 bits
    //
    if (WAVE_Format->bitsPerSample == 16)
    {
      // Get maximum peak
      tShort Maximum;
      if (WAVE_Format->numChannels == 2)
      {
        Maximum = GetAudioMaximum16BitsStereo((tShort*)pbAudioData, NbSamples);
      }
      else
      {
        Maximum = GetAudioMaximum16BitsMono((tShort*)pbAudioData, NbSamples);
      }

      //
      // Create bits array
      //
      tShort* SamplePtr = (tShort*)pbAudioData;
      tULong SampleLoop = NbSamples;
      tShort* DebugSamplePtr = (tShort*)pbDebugAudioData;

      float AutoGain = 32760.f / (float)Maximum;
      InitTapeLowFilter1(*SamplePtr);
      InitButterworthLPF(WAVE_Format->sampleRate);
      InitButterworthHPF(WAVE_Format->sampleRate);
#ifdef USE_PPI_HYSTERESIS
      tUChar PPILevel=0;
#endif /* USE_PPI_HYSTERESIS */
      while (SampleLoop--)
      {
        // Left/Mono channel first
        tShort Sample = *(SamplePtr++);
        // Ignore Right channel
        if (WAVE_Format->numChannels == 2) SamplePtr++;

        //
        // Audio filtering
        //
        float fSample = (float)Sample;

        // Automatic Gain
        if (mTapeAutoGainActive == true)
        {
          fSample *= AutoGain;
        }

        // Low Pass Filter 1
        if (mTapeLPF1Active == true)
        {
          fSample = ComputeTapeLowFilter1(fSample);
        }

        // Low Pass Filter 2
        if (mTapeLPF2Active == true)
        {
          fSample = ComputeButterworthLPF(fSample);
        }

        // High Pass Filter 2
        if (mTapeHPF2Active == true)
        {
          fSample = ComputeButterworthHPF(fSample);
        }

        // Integer Saturation
        if (fSample > 32767.) Sample = 32767;
        else if (fSample < -32768.) Sample = -32768;
        else Sample = (tShort)fSample;

        // PPI Level
#ifndef USE_PPI_HYSTERESIS
        tUChar PPILevel=0;
        if (Sample > mTapePPIThreshold16) PPILevel=1;
#else /* USE_PPI_HYSTERESIS */
        if (PPILevel)
        {
          if (Sample < -mTapePPIThreshold16) PPILevel=0;
        }
        else
        {
          if (Sample > mTapePPIThreshold16) PPILevel=1;
        }
#endif /* USE_PPI_HYSTERESIS */

        // CSW treatment
        if (firstPulseGiven == false)
        {
          *(pbTapeImagePtr + 0x1C) = PPILevel & 0x01; // Flags
          LastSampleLevel = PPILevel;
          firstPulseGiven = true;
        }
        else
        {
          if (LastSampleLevel == PPILevel)
          {
            CSWSampleCount++;
          }
          else
          {
            // Add
            if (CSWSampleCount < 256)
            {
              *pbTapeImagePtr++ = (tUChar)CSWSampleCount;
            }
            else
            {
              *pbTapeImagePtr++ = 0x00;
              *pbTapeImagePtr++ = CSWSampleCount & 0xFF;
              *pbTapeImagePtr++ = (CSWSampleCount >> 8) & 0xFF;
              *pbTapeImagePtr++ = (CSWSampleCount >> 16) & 0xFF;
              *pbTapeImagePtr++ = (CSWSampleCount >> 24) & 0xFF;
            }
            CSWSampleCount = 1;
            LastSampleLevel = PPILevel;
          }
        }

        if (DebugHandle)
        {
          *(DebugSamplePtr++) = Sample;
          if (WAVE_Format->numChannels == 2)
          {
            *(DebugSamplePtr++) = PPILevel ? 9600 : -9600;
          }
        }
      }

      FinalizeButterworthLPF();
      FinalizeButterworthHPF();
    }

    //
    // PCM 8 bits
    //
    else if (WAVE_Format->bitsPerSample == 8)
    {
      // Create bits array
      tUChar* SamplePtr = (tUChar*)pbAudioData;
      tUChar* DebugSamplePtr = pbDebugAudioData;
      tULong SampleLoop = NbSamples;
#ifdef USE_PPI_HYSTERESIS
      tUChar PPILevel=0;
      tUChar mTapePPIThreshold8Hys = mTapePPIThreshold8;
      if (mTapePPIThreshold8Hys >= 128)
      {
        mTapePPIThreshold8Hys = 128 - (mTapePPIThreshold8Hys - 128);
      }
      else
      {
        mTapePPIThreshold8Hys = 128 + (128 - mTapePPIThreshold8Hys);
      }
#endif /* USE_PPI_HYSTERESIS */
      while (SampleLoop--)
      {
        // Left/Mono channel first
        tUChar Sample = *(SamplePtr++);
        // Ignore Right channel
        if (WAVE_Format->numChannels == 2) SamplePtr++;

        // PPI Level
#ifndef USE_PPI_HYSTERESIS
        tUChar PPILevel=0;
        if (Sample > mTapePPIThreshold8) PPILevel=1;
#else /* USE_PPI_HYSTERESIS */
        if (PPILevel)
        {
          if (Sample < mTapePPIThreshold8Hys) PPILevel=0;
        }
        else
        {
          if (Sample > mTapePPIThreshold8) PPILevel=1;
        }
#endif /* USE_PPI_HYSTERESIS */

        // CSW treatment
        if (firstPulseGiven == false)
        {
          *(pbTapeImagePtr + 0x1C) = PPILevel & 0x01; // Flags
          LastSampleLevel = PPILevel;
          firstPulseGiven = true;
        }
        else
        {
          if (LastSampleLevel == PPILevel)
          {
            CSWSampleCount++;
          }
          else
          {
            // Add
            if (CSWSampleCount < 256)
            {
              *pbTapeImagePtr++ = (tUChar)CSWSampleCount;
            }
            else
            {
              *pbTapeImagePtr++ = 0x00;
              *pbTapeImagePtr++ = CSWSampleCount & 0xFF;
              *pbTapeImagePtr++ = (CSWSampleCount >> 8) & 0xFF;
              *pbTapeImagePtr++ = (CSWSampleCount >> 16) & 0xFF;
              *pbTapeImagePtr++ = (CSWSampleCount >> 24) & 0xFF;
            }
            CSWSampleCount = 1;
            LastSampleLevel = PPILevel;
          }
        }


        if (DebugHandle)
        {
          *(DebugSamplePtr++) = Sample;
          if (WAVE_Format->numChannels == 2)
          {
            *(DebugSamplePtr++) = PPILevel ? 160 : 96;
          }
        }
      }
    }

    // Ignore others
    else
    {
      delete [] pbTapeImage;
      pbTapeImage = 0;
    }

    if (DebugHandle)
    {
      // Write debug audio data
      FileWrite(DebugHandle, pbDebugAudioData, DebugAudioDataLength);
    }

    // Get Data Length
    *LengthP = pbTapeImagePtr - pbTapeImage;

    free(pbDebugAudioData);
  }
  while (0);

  if (DebugHandle)
  {
    FileClose(DebugHandle);
    // Notify user
    if (mQuietMode == false)
    {
		  String Message = _T("Audio debug file:\r\n")
			  							 + String(DebugFilename)
				  						 + _T("\r\nhas just been created.");
		  Application->MessageBox(Message.c_str(), _T("Debug"), MB_OK);
    }
	}
	if (CSWHandle)
	{
		// Write debug audio data
		FileWrite(CSWHandle, pbTapeImage, *LengthP);
		// Close file
		FileClose(CSWHandle);
		// Notify user
    if (mQuietMode == false)
    {
		  String Message = _T("CSW file:\r\n")
			  							 + String(CSWFilename)
				  						 + _T("\r\nhas just been created.");
		  Application->MessageBox(Message.c_str(), _T("Debug"), MB_OK);
    }
	}

  return pbTapeImage;
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::InitTapeLowFilter1(tShort Sample)
{
  mTapeLPF1_b = 1.f - mTapeLPF1_a;
  mTapeLPF1_LastSample = (float)Sample;
}
//---------------------------------------------------------------------------
float __fastcall TAudioContext::ComputeTapeLowFilter1(float Sample)
{
  float Result = (Sample * mTapeLPF1_b) + (mTapeLPF1_LastSample * mTapeLPF1_a);
  mTapeLPF1_LastSample = Result;

  return Result;
}
//---------------------------------------------------------------------------


/*
 *                            COPYRIGHT
 *
 *  Copyright (C) 2014 Exstrom Laboratories LLC
 *
 *  Exstrom Laboratories LLC contact:
 *  stefan(AT)exstrom.com
 *
 *  Exstrom Laboratories LLC
 *  Longmont, CO 80503, USA
 *
*/
void __fastcall TAudioContext::InitButterworthLPF(long SampleRate)
{
  float s = (float)SampleRate;
  float f = (float)mTapeLPF2_Frequency;
  float a = (float)tan(M_PI*f/s);
  float a2 = a*a;

  // Allocate memory for parameters
  mTapeLPF2_A = new float[mTapeLPF2_Order];
  mTapeLPF2_d1 = new float[mTapeLPF2_Order];
  mTapeLPF2_d2 = new float[mTapeLPF2_Order];

  // Allocate memory for samples
  mTapeLPF2_w0 = new float[mTapeLPF2_Order];
  mTapeLPF2_w1 = new float[mTapeLPF2_Order];
  mTapeLPF2_w2 = new float[mTapeLPF2_Order];

  for (tUChar n=0; n < mTapeLPF2_Order; n++)
  {
    float r = (float)sin(M_PI*(2.0*n+1.0)/(4.0*mTapeLPF2_Order));
    s = (float)(a2 + 2.0*a*r + 1.0);

	mTapeLPF2_A[n] = a2/s;
    mTapeLPF2_d1[n] = (float)(2.0*(1-a2)/s);
    mTapeLPF2_d2[n] = (float)(-(a2 - 2.0*a*r + 1.0)/s);

	mTapeLPF2_w0[n] = 0.f;
    mTapeLPF2_w1[n] = 0.f;
    mTapeLPF2_w2[n] = 0.f;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAudioContext::FinalizeButterworthLPF(void)
{
  delete [] mTapeLPF2_A;
  mTapeLPF2_A = NULL;
  delete [] mTapeLPF2_d1;
  mTapeLPF2_d1 = NULL;
  delete [] mTapeLPF2_d2;
  mTapeLPF2_d2 = NULL;

  delete [] mTapeLPF2_w0;
  mTapeLPF2_w0 = NULL;
  delete [] mTapeLPF2_w1;
  mTapeLPF2_w1 = NULL;
  delete [] mTapeLPF2_w2;
  mTapeLPF2_w2 = NULL;
}
//---------------------------------------------------------------------------
float __fastcall TAudioContext::ComputeButterworthLPF(float x)
{
  for(tUChar i=0; i<mTapeLPF2_Order; i++)
  {
    mTapeLPF2_w0[i] = mTapeLPF2_d1[i]*mTapeLPF2_w1[i] + mTapeLPF2_d2[i]*mTapeLPF2_w2[i] + x;
	x = (float)(mTapeLPF2_A[i]*(mTapeLPF2_w0[i] + 2.0*mTapeLPF2_w1[i] + mTapeLPF2_w2[i]));
    mTapeLPF2_w2[i] = mTapeLPF2_w1[i];
    mTapeLPF2_w1[i] = mTapeLPF2_w0[i];
  }
  
  return x;
}
//---------------------------------------------------------------------------


void __fastcall TAudioContext::InitButterworthHPF(long SampleRate)
{
  float s = (float)SampleRate;
  float f = (float)mTapeHPF2_Frequency;
  float a = (float)tan(M_PI*f/s);
  float a2 = a*a;

  // Allocate memory for parameters
  mTapeHPF2_A = new float[mTapeHPF2_Order];
  mTapeHPF2_d1 = new float[mTapeHPF2_Order];
  mTapeHPF2_d2 = new float[mTapeHPF2_Order];

  // Allocate memory for samples
  mTapeHPF2_w0 = new float[mTapeHPF2_Order];
  mTapeHPF2_w1 = new float[mTapeHPF2_Order];
  mTapeHPF2_w2 = new float[mTapeHPF2_Order];

  for (tUChar n=0; n < mTapeHPF2_Order; n++)
  {
    float r = (float)sin(M_PI*(2.0*n+1.0)/(4.0*mTapeHPF2_Order));
    s = (float)(a2 + 2.0*a*r + 1.0);
    
    mTapeHPF2_A[n] = 1.0f/s;
    mTapeHPF2_d1[n] = (float)(2.0*(1-a2)/s);
    mTapeHPF2_d2[n] = (float)(-(a2 - 2.0*a*r + 1.0)/s);

    mTapeHPF2_w0[n] = 0.f;
    mTapeHPF2_w1[n] = 0.f;
    mTapeHPF2_w2[n] = 0.f;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAudioContext::FinalizeButterworthHPF(void)
{
  delete [] mTapeHPF2_A;
  mTapeHPF2_A = NULL;
  delete [] mTapeHPF2_d1;
  mTapeHPF2_d1 = NULL;
  delete [] mTapeHPF2_d2;
  mTapeHPF2_d2 = NULL;

  delete [] mTapeHPF2_w0;
  mTapeHPF2_w0 = NULL;
  delete [] mTapeHPF2_w1;
  mTapeHPF2_w1 = NULL;
  delete [] mTapeHPF2_w2;
  mTapeHPF2_w2 = NULL;
}
//---------------------------------------------------------------------------
float __fastcall TAudioContext::ComputeButterworthHPF(float x)
{
  for(tUChar i=0; i<mTapeHPF2_Order; i++)
  {
    mTapeHPF2_w0[i] = mTapeHPF2_d1[i]*mTapeHPF2_w1[i] + mTapeHPF2_d2[i]*mTapeHPF2_w2[i] + x;
    x = (float)(mTapeHPF2_A[i]*(mTapeHPF2_w0[i] - 2.0*mTapeHPF2_w1[i] + mTapeHPF2_w2[i]));
    mTapeHPF2_w2[i] = mTapeHPF2_w1[i];
    mTapeHPF2_w1[i] = mTapeHPF2_w0[i];
  }

  return x;
}
//---------------------------------------------------------------------------


tULong __fastcall TAudioContext::CreatePCMSineAudio(tUShort SamplingRate,
                                                     tUShort Frequency,
                                                     tShort Amplitude,
                                                     tULong NbPeriods,
                                                     tShort** SamplesPP)
{
  float s = (float)SamplingRate;
  float f = (float)Frequency;
  float a = (float)Amplitude;

  // Determine NbSamples
  tULong NbSamples = (tULong)(s/f*(float)NbPeriods);
  if (NbSamples)
  {
    // Allocate memory
    *SamplesPP = new tShort[NbSamples];

    // Create sine audio
    tShort* SamplePtr = *SamplesPP;
    for (tULong n=0; n < NbSamples; n++)
    {
      // sine wave: y(t) = amplitude * sin(2 * PI * frequency * time), time = s / sample_rate
      *(SamplePtr++) = (tUShort)(a * (float)sin(2.0 * M_PI * f * n / s));
    }
  }
  
  return NbSamples;
}
//---------------------------------------------------------------------------


tShort __fastcall TAudioContext::GetTapeFilterOutputAmplitude(tUShort Frequency, tShort InputAmplitude)
{
  tShort OutputAmplitude = 0;
  tShort* Samples;
  tULong NbSamples = CreatePCMSineAudio(44100, Frequency, InputAmplitude, 100, &Samples);

  if (NbSamples)
  {
    tShort* SamplePtr = Samples;
    tULong Loop = NbSamples;

    InitTapeLowFilter1(*SamplePtr);
    InitButterworthLPF(44100);
    InitButterworthHPF(44100);

    while (Loop--)
    {
      float fSample = (float)(*SamplePtr);

      // Low Pass Filter 1
      if (mTapeLPF1Active == true)
      {
        fSample = ComputeTapeLowFilter1(fSample);
      }

      // Low Pass Filter 2
      if (mTapeLPF2Active == true)
      {
        fSample = ComputeButterworthLPF(fSample);
      }

      // High Pass Filter 2
      if (mTapeHPF2Active == true)
      {
        fSample = ComputeButterworthHPF(fSample);
      }

      // Integer Saturation
      if (fSample > 32767.) *SamplePtr = 32767;
      else if (fSample < -32768.) *SamplePtr = -32768;
      else *SamplePtr = (tShort)fSample;

      SamplePtr++;
    }

    FinalizeButterworthLPF();
    FinalizeButterworthHPF();

    OutputAmplitude = GetAudioMaximum16BitsMono(Samples, NbSamples); 

    delete [] Samples;
  }

  return OutputAmplitude;
}
//---------------------------------------------------------------------------


#if 0 // Not Used
bool __fastcall TAudioContext::CreateWAV16M(AnsiString Filename,
                                            tULong sampleRate,
                                            tUShort* Samples,
                                            int Count,
                                            bool QuietMode)
{
  int FileHandle = FileCreate(Filename);
  if (FileHandle == -1) return false;

  do
  {
    // Allocate memory
    int FileSize = sizeof(tWAVE_file_header)
                   - 1 /* WAVE_Data.ChunkData */
                   + (Count * 2 /* 16 bits sample */);
    tUChar* FileData = new tUChar[FileSize];
    if (!FileData) continue;

    tWAVE_file_header* WAVHeaderP = (tWAVE_file_header*)FileData;

    // RIFF header
    memcpy(WAVHeaderP->RIFF_Header.chunkID, "RIFF", 4);
    WAVHeaderP->RIFF_Header.chunkSize = FileSize;
    memcpy(WAVHeaderP->RIFF_Header.format, "WAVE", 4);

    // WAV Format header
    memcpy(WAVHeaderP->WAVE_Format_Header.chunkID, "fmt ", 4);
    WAVHeaderP->WAVE_Format_Header.chunkSize = sizeof(tWAVE_Format);

    // WAV Format
    WAVHeaderP->WAVE_Format.audioFormat = 1; // PCM
    WAVHeaderP->WAVE_Format.numChannels = 1; // Mono
    WAVHeaderP->WAVE_Format.sampleRate = (long)sampleRate;
    WAVHeaderP->WAVE_Format.byteRate = sampleRate * 16;
    WAVHeaderP->WAVE_Format.blockAlign = 2;
    WAVHeaderP->WAVE_Format.bitsPerSample = 16;

    // WAV Data
    memcpy(WAVHeaderP->WAVE_Data.subChunkID, "data", 4);
    WAVHeaderP->WAVE_Data.subChunk2Size = Count * 2; // 16 bits sample
    memcpy((char*)&WAVHeaderP->WAVE_Data.ChunkData,
           (char*)Samples,
           WAVHeaderP->WAVE_Data.subChunk2Size);

    // Write file
    FileWrite(FileHandle, FileData, FileSize);

    delete [] FileData;

    // Advise user
    if (QuietMode == false)
    {
		  String Message = _T("File \"")
			  							 + String(Filename)
				  						 + _T("\" has been created.");
		  Application->MessageBox(Message.c_str(), _T("WAV Audio file"), MB_OK);
    }
	}
  while (0);

  FileClose(FileHandle);

  return true;
}
//---------------------------------------------------------------------------
#endif


tUChar* __fastcall TAudioContext::InflateDataFromCSW(AnsiString Filename,
                                                     tULong* LengthP,
                                                     tULong* SampleRateP,
                                                     tUChar* StartPolarityP)
{
  if (Filename.IsEmpty() == true)
    return 0;

  // Archive Container
  AnsiString InArchiveFilename = "";
  if (Filename.LowerCase().Pos(CONTAINER_ZIP_EXT))
  {
    int ExtensionPos = Filename.LowerCase().Pos(CONTAINER_ZIP_EXT);
    InArchiveFilename = Filename.SubString(ExtensionPos+5, Filename.Length());
    Filename = Filename.SubString(1, ExtensionPos+3);
  }

  if (FileExists(Filename) == false)
    return 0;

  // Ouverture du fichier en lecture
  tUChar* FileDataP = 0;
  int iHandle = FileOpen(Filename, fmOpenRead);
  // Met le curseur à la fin du fichier pour récupérer la taille du fichier
  int FileLength = FileSeek(iHandle, 0, 2);
  if (FileLength > 0)
  {
    // Remet le curseur en début de fichier pour la prochaine lecture
    FileSeek(iHandle, 0, 0);
    // Allocate memory to read file
    FileDataP = (tUChar*)malloc(FileLength);
    // Read file
    if (FileDataP)
    {
      FileRead(iHandle, FileDataP, FileLength);
    }
  }
  FileClose(iHandle);

  if (!FileDataP) return 0;

  // Load file from archive container
  if (InArchiveFilename.IsEmpty() == false)
  {
    TZIPContainer* ZIPContainer = new TZIPContainer(Filename);

    // Allocate memory
    int UncompressedSize = ZIPContainer->GetFileUncompressedSize(InArchiveFilename);
    tUChar* CSWData = (tUChar*)malloc(UncompressedSize);

    // Read file from archive
    if (ZIPContainer->ReadFile(InArchiveFilename, CSWData, UncompressedSize) != UNZ_OK)
    {
      free(CSWData);
      return 0;
    }

    delete ZIPContainer;

    // Prepare disk operation
    free(FileDataP);
    FileDataP = CSWData;
    FileLength = UncompressedSize;
    Filename = InArchiveFilename;
  }

  tUChar* InflateDataP = InflateDataFromCSW(FileDataP,
                                            FileLength,
                                            LengthP,
                                            SampleRateP,
                                            StartPolarityP);

  free(FileDataP);

  return InflateDataP;
}
//---------------------------------------------------------------------------
tUChar* __fastcall TAudioContext::InflateDataFromCSW(tUChar* FileDataP,
                                                     tULong  FileLength,
                                                     tULong* LengthP,
                                                     tULong* SampleRateP,
                                                     tUChar* StartPolarityP)
{
  if (!FileDataP)
    return FileDataP;

  // Check Header

  if (strncmp((char*)FileDataP, "Compressed Square Wave", 22) != 0)
  {
	return 0;
  }

  tULong SampleRate = 0;
  tULong InflateDataSize = 0;
  tUChar CompressionType;
  tUChar StartPolarity = 0;
  tUChar HeaderSize = 0x20; // Version 1 Default

  tUChar* PulsesP = 0;

  //
  // Major Revision 1
  //
  if (*(FileDataP+0x17) == 1)
  {
    // Get sample rate
    SampleRate =   ((*(FileDataP+0x19)) << 0)
                 + ((*(FileDataP+0x1A)) << 8);

    // Get Compression Type
    CompressionType = *(FileDataP+0x1B);

    // Get Start Polarity
    StartPolarity = *(FileDataP+0x1C) & 0x01;

    // Get inflate size
    InflateDataSize = FileLength - HeaderSize;

    // RLE
    if (CompressionType == 1)
    {
      PulsesP = (tUChar*)malloc(InflateDataSize);
      // Copy data from file to buffer
      memcpy(PulsesP,
             FileDataP + HeaderSize,
             InflateDataSize);
    }
  }
  //
  // Major Revision 2
  //
  else if (*(FileDataP+0x17) == 2)
  {
    // Get sample rate
    SampleRate =   ((*(FileDataP+0x19)) << 0)
                 + ((*(FileDataP+0x1A)) << 8)
                 + ((*(FileDataP+0x1B)) << 16)
                 + ((*(FileDataP+0x1C)) << 24);

    // Get inflate size
    InflateDataSize =   ((*(FileDataP+0x1D)) << 0)
                      + ((*(FileDataP+0x1E)) << 8)
                      + ((*(FileDataP+0x1F)) << 16)
                      + ((*(FileDataP+0x20)) << 24);

    // Get Compression Type
    CompressionType = *(FileDataP+0x21);

    // Get Start Polarity
    StartPolarity = *(FileDataP+0x22) & 0x01;

    // Add HDR to header
    HeaderSize = 0x34 + *(FileDataP+0x23);

    // Allocate memory to read file
    PulsesP = (tUChar*)malloc(InflateDataSize);

    //
    // RLE
    //
    if (CompressionType == 1)
    {
      // Copy data from file to buffer
      memcpy(PulsesP,
             FileDataP + HeaderSize,
             InflateDataSize);
    }
    //
    // Z-RLE
    //
    else if ( (CompressionType == 2) && (*(FileDataP+0x17) > 1) )
    {
      z_stream strm;

      /* allocate inflate state */
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      strm.avail_in = 0;
      strm.next_in = Z_NULL;
      if (inflateInit(&strm) != Z_OK)
      {
        // Cancel
        free(PulsesP);
        return 0;
      }

      // inflate
      strm.avail_in = FileLength - HeaderSize;
      strm.avail_out = InflateDataSize;
      strm.next_in = FileDataP + HeaderSize;
      strm.next_out = PulsesP;
      if (inflate(&strm, Z_NO_FLUSH) != Z_OK)
      {
        InflateDataSize = 0;
        free(PulsesP);
        PulsesP = 0;
      }

      // Clean-up inflating
      inflateEnd(&strm);
    }
    else
    {
      // Cancel
      InflateDataSize = 0;
      free(PulsesP);
      PulsesP = 0;
    }
  }

  *StartPolarityP = StartPolarity;
  *LengthP = InflateDataSize;
  *SampleRateP = SampleRate;

  // Return buffer
  return PulsesP;
}
//---------------------------------------------------------------------------

