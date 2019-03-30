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


#ifndef TAudioContextH
#define TAudioContextH

#include "..\Settings\TSettings.h"
#include "..\..\Engine\Native_CPC.h"

#ifdef _USE_OPENAL
#include "..\OpenAL\al.h"
#include "..\OpenAL\alc.h"
#endif /* _USE_OPENAL */


#define AUDIO_VOLUME_MIN                        0
#define AUDIO_VOLUME_MAX                        300


class TAudioContext
{
private:

  //roger
  float* pFloat1;

  //
  // Members
  //
  TSettings* mSettingsP;
  int mLastError;
  TStringList* mDevicesList;
#ifdef _USE_OPENAL
  ALCdevice* mALdev;
  ALCcontext* mALctx;
  ALuint mALEmulatorSource;
  ALuint* mALEmulatorBuffersP;
  ALboolean* mALEmulatorQueuedbuffersP;
  ALuint mALFloppyMotorSource;
  ALuint mALFloppyMotorBuffer;
  unsigned char * mFloppyHeadSampleData;
  ALenum mFloppyHeadSampleFormat;
  ALsizei mFloppyHeadSampleSize;
  ALsizei mFloppyHeadSampleFreq;
  ALuint mALFloppyHeadSource;
  ALuint* mALFloppyHeadBuffersP;
  ALboolean* mALFloppyHeadQueuedbuffersP;
  ALuint mALTapeMotorSource;
  ALuint mALTapeMotorBuffer;
  ALuint mALTapeRelaySource;
  ALuint mALTapeRelayBuffer;
  ALuint mALTapeDoubleRelaySource;
  ALuint mALTapeDoubleRelayBuffer;
  ALuint mALTMPISource;
  ALuint* mALTMPIBuffersP;
  ALboolean* mALTMPIQueuedbuffersP;
  ALuint mALPlayCitySource;
  ALuint* mALPlayCityBuffersP;
  ALboolean* mALPlayCityQueuedbuffersP;
#endif /* _USE_OPENAL */
  bool mQuietMode;
  bool mTapeCreateOutputAudioFile;
  bool mTapeCreateCDTFile;
  bool mTapeCreateCSWFile;
  tUChar mTapePPIThreshold8;
  tShort mTapePPIThreshold16;
  bool mTapeAutoGainActive;
  bool mTapeLPF1Active; // Low pass filter 1
  float mTapeLPF1_a;
  float mTapeLPF1_b;
  float mTapeLPF1_LastSample;
  bool mTapeLPF2Active; // Butterworth Low pass filter
  tUChar mTapeLPF2_Order;
  tUShort mTapeLPF2_Frequency;
  float* mTapeLPF2_A;
  float* mTapeLPF2_d1;
  float* mTapeLPF2_d2;
  float* mTapeLPF2_w0;
  float* mTapeLPF2_w1;
  float* mTapeLPF2_w2;
  bool mTapeHPF2Active; // Butterworth High Pass Filter
  tUChar mTapeHPF2_Order;
  tUShort mTapeHPF2_Frequency;
  float* mTapeHPF2_A;
  float* mTapeHPF2_d1;
  float* mTapeHPF2_d2;
  float* mTapeHPF2_w0;
  float* mTapeHPF2_w1;
  float* mTapeHPF2_w2;
  //
  // Methods
  //
  void __fastcall GetDeviceList(void);
  const AnsiString __fastcall GetDevice(int Value);

  inline int __fastcall GetDevicesCount(void)
  {
   return mDevicesList->Count;
  };

  void __fastcall SetAudioVolume(const float Value);
  void __fastcall SetAudioGainSpeaker(const float Value);
  void __fastcall SetAudioGainTMPI(const float Value);
  void __fastcall SetAudioGainPlayCity(const float Value);
  void __fastcall SetAudioGainFloppy(const float Value);
  void __fastcall SetAudioGainTape(const float Value);
  inline bool __fastcall GetQuietMode(void) { return mQuietMode; };
  inline void __fastcall SetQuietMode(bool Value) { mQuietMode = Value; };
  inline bool __fastcall GetTapeCreateOutputAudioFile(void) { return mTapeCreateOutputAudioFile; };
  inline void __fastcall SetTapeCreateOutputAudioFile(bool Value) { mTapeCreateOutputAudioFile = Value; };
  inline bool __fastcall GetTapeCreateCDTFile(void) { return mTapeCreateCDTFile; };
  inline void __fastcall SetTapeCreateCDTFile(bool Value) { mTapeCreateCDTFile = Value; };
  inline bool __fastcall GetTapeCreateCSWFile(void) { return mTapeCreateCSWFile; };
  void __fastcall SetTapeCreateCSWFile(bool Value);
  inline tUChar __fastcall GetTapePPIThreshold8(void) { return mTapePPIThreshold8; };
  void __fastcall SetTapePPIThreshold8(tUChar Value);
  inline tShort __fastcall GetTapePPIThreshold16(void) { return mTapePPIThreshold16; };
  void __fastcall SetTapePPIThreshold16(tShort Value);
  inline bool __fastcall GetTapeAutoGainActive(void) { return mTapeAutoGainActive; };
  void __fastcall SetTapeAutoGainActive(bool Value);
  inline bool __fastcall GetTapeLPF1Active(void) { return mTapeLPF1Active; };
  void __fastcall SetTapeLPF1Active(bool Value);
  inline float __fastcall GetTapeLPF1_A(void) { return mTapeLPF1_a; };
  void __fastcall SetTapeLPF1_A(float Value);
  inline bool __fastcall GetTapeLPF2Active(void) { return mTapeLPF2Active; };
  void __fastcall SetTapeLPF2Active(bool Value);
  inline tUChar __fastcall GetTapeLPF2Order(void) { return mTapeLPF2_Order; };
  void __fastcall SetTapeLPF2Order(tUChar Value);
  inline tUShort __fastcall GetTapeLPF2Freq(void) { return mTapeLPF2_Frequency; };
  void __fastcall SetTapeLPF2Freq(tUShort Value);
  inline bool __fastcall GetTapeHPF2Active(void) { return mTapeHPF2Active; };
  void __fastcall SetTapeHPF2Active(bool Value);
  inline tUChar __fastcall GetTapeHPF2Order(void) { return mTapeHPF2_Order; };
  void __fastcall SetTapeHPF2Order(tUChar Value);
  inline tUShort __fastcall GetTapeHPF2Freq(void) { return mTapeHPF2_Frequency; };
  void __fastcall SetTapeHPF2Freq(tUShort Value);
  inline bool __fastcall GetSpeakerStereo(void) { return mSettingsP->AudioSpeakerStereo; };
  void __fastcall SetSpeakerStereo(bool Value);
  inline bool __fastcall GetSpeaker16Bits(void) { return mSettingsP->AudioSpeaker16Bits; };
  inline bool __fastcall GetPlayCityStereo(void) { return mSettingsP->AudioPlayCityStereo; };
  void __fastcall SetPlayCityStereo(bool Value);
  inline bool __fastcall GetPlayCity16Bits(void) { return mSettingsP->AudioPlayCity16Bits; };
#ifdef _USE_OPENAL
	void __fastcall CreateResourceSource(String SoundName,
																			 ALuint* SoundSourceP,
																			 ALuint* SoundBufferP);
	void __fastcall LoadResourceData(String SoundName,
																	 unsigned char ** DataP,
																	 ALenum* FormatP,
																	 ALsizei* FreqP,
																	 ALsizei* SizeP);
  void __fastcall UnqueueSoundBuffers(ALuint Source,
                                      ALuint* BuffersP,
                                      ALboolean* QueuedbuffersP,
                                      ALuint NbBuffers);
#endif /* _USE_OPENAL */
  bool __fastcall CreateFloppyHeadSource(void);
  tShort __fastcall GetAudioMaximum16BitsStereo(tShort* DataP, tULong NbSamples);
  tShort __fastcall GetAudioMaximum16BitsMono(tShort* DataP, tULong NbSamples);
  void __fastcall InitTapeLowFilter1(tShort Sample);
  float __fastcall ComputeTapeLowFilter1(float Sample);
  void __fastcall InitButterworthLPF(long SampleRate);
  void __fastcall FinalizeButterworthLPF(void);
  float __fastcall ComputeButterworthLPF(float x);
  void __fastcall InitButterworthHPF(long SampleRate);
  void __fastcall FinalizeButterworthHPF(void);
  float __fastcall ComputeButterworthHPF(float x);

  tULong __fastcall CreatePCMSineAudio(tUShort SamplingRate,
                                        tUShort Frequency,
                                        tShort Amplitude,
                                        tULong NbPeriods,
                                        tShort** SamplesPP);
  
public:
  //
  // Properties
  //
  __property int DevicesCount = { read=GetDevicesCount };
  __property const AnsiString Device[int Value] = { read=GetDevice };
  __property float Volume = { write=SetAudioVolume };
  __property float GainSpeaker = { write=SetAudioGainSpeaker };
  __property float GainTMPI = { write=SetAudioGainTMPI };
  __property float GainPlayCity = { write=SetAudioGainPlayCity };
  __property float GainFloppy = { write=SetAudioGainFloppy };
  __property float GainTape = { write=SetAudioGainTape };
  __property bool QuietMode = { read=GetQuietMode, write=SetQuietMode };
  __property bool TapeCreateOutputAudioFile = { read=GetTapeCreateOutputAudioFile, write=SetTapeCreateOutputAudioFile };
  __property bool TapeCreateCDTFile = { read=GetTapeCreateCDTFile, write=SetTapeCreateCDTFile };
  __property bool TapeCreateCSWFile = { read=GetTapeCreateCSWFile, write=SetTapeCreateCSWFile };
  __property tUChar TapePPIThreshold8 = { read=GetTapePPIThreshold8, write=SetTapePPIThreshold8 };
  __property tShort TapePPIThreshold16 = { read=GetTapePPIThreshold16, write=SetTapePPIThreshold16 };
  __property bool TapeAutoGainActive = { read=GetTapeAutoGainActive, write=SetTapeAutoGainActive };
  __property bool TapeLPF1Active = { read=GetTapeLPF1Active, write=SetTapeLPF1Active };
  __property float TapeLPF1_A = { read=GetTapeLPF1_A, write=SetTapeLPF1_A };
  __property bool TapeLPF2Active = { read=GetTapeLPF2Active, write=SetTapeLPF2Active };
  __property tUChar TapeLPF2Order = { read=GetTapeLPF2Order, write=SetTapeLPF2Order };
  __property tUShort TapeLPF2Freq = { read=GetTapeLPF2Freq, write=SetTapeLPF2Freq };
  __property bool TapeHPF2Active = { read=GetTapeHPF2Active, write=SetTapeHPF2Active };
  __property tUChar TapeHPF2Order = { read=GetTapeHPF2Order, write=SetTapeHPF2Order };
  __property tUShort TapeHPF2Freq = { read=GetTapeHPF2Freq, write=SetTapeHPF2Freq };
  __property bool SpeakerStereo = { read=GetSpeakerStereo, write=SetSpeakerStereo };
  __property bool Speaker16Bits = { read=GetSpeaker16Bits };
  __property bool PlayCityStereo = { read=GetPlayCityStereo, write=SetPlayCityStereo };
  __property bool PlayCity16Bits = { read=GetPlayCity16Bits };

  //
  // API
  //
  __fastcall TAudioContext(TSettings* SettingsP);
  __fastcall ~TAudioContext();

  TStringList* __fastcall GetInfo(void);

  bool __fastcall Start(void);
  void __fastcall Stop(void);
  void __fastcall Restart(TSettings* SettingsP);

  void __fastcall Pause(void);
  void __fastcall Resume(void);

  void __fastcall UnqueueSoundBuffers(void);

  bool __fastcall CreateEmulatorStream(void);
  bool __fastcall FillEmulatorStream(const void* DataP, int DataSize);

  bool __fastcall CreateTMPIStream(void);
  bool __fastcall FillTMPIStream(const void* DataP, int DataSize);

  bool __fastcall CreatePlayCityStream(void);
  bool __fastcall FillPlayCityStream(const void* DataP, int DataSize);

  void __fastcall PlayFloppyMotor(void);
  void __fastcall StopFloppyMotor(void);
  void __fastcall PlayFloppyHead(int NbSteps);

  void __fastcall PlayTapeMotor(void);
  void __fastcall StopTapeMotor(void);

  void __fastcall PlayTapeRelay(void);
  void __fastcall PlayTapeDoubleRelay(void);

  tUChar* __fastcall ConvertWAVToCDT(AnsiString Filename, tULong* LengthP);
  tUChar* __fastcall ConvertWAVToCSW(AnsiString Filename, tULong* LengthP);

  tShort __fastcall GetTapeFilterOutputAmplitude(tUShort Frequency, tShort InputAmplitude);

  void __fastcall RestoreDefaultTapeAudioParameters(void);

  // Not Used
  /*static bool __fastcall CreateWAV16M(AnsiString Filename,
                                      tULong sampleRate,
                                      tUShort* Samples,
                                      int Count,
                                      bool QuietMode);*/

  tUChar* __fastcall InflateDataFromCSW(AnsiString Filename,
                                        tULong* LengthP,
                                        tULong* SampleRateP,
                                        tUChar* StartPolarityP);
  tUChar* __fastcall InflateDataFromCSW(tUChar* FileDataP,
                                        tULong  FileLength,
                                        tULong* LengthP,
                                        tULong* SampleRateP,
                                        tUChar* StartPolarityP);
};
//---------------------------------------------------------------------------
#endif

