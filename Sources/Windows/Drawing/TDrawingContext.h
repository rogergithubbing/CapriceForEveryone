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


#ifndef TDrawingContextH
#define TDrawingContextH

#include "..\..\Engine\Native_CPC.h"
#include "..\Settings\TSettings.h"
#include "..\Emulator\TEmulatorMessage.h"

#ifdef _USE_OPENGL
#include "..\OpenGL\glew.h"
#include "..\OpenGL\wglew.h"
#include <gl\glu.h>
#endif /* _USE_OPENGL */

#ifdef _USE_DIRECTX
#pragma warn -8010
#include "..\Direct3D\d3dx9.h"
#pragma warn +8010
#endif /* _USE_DIRECTX */


#ifdef _USE_DIRECTX

#define D3DFVF_DIALOGVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )
struct D3DDialogVertex
{
  float x, y, z, rhw;
  D3DCOLOR diffuse;
};

#endif /* _USE_DIRECTX */


//
// Bitmaps drawing structure
//
typedef struct
{
  HBITMAP GDIHandle;
  HBITMAP GDIMaskHandle;
  int Width;
  int Height;
  int ScreenWidth;
  int ScreenHeight;
  int ScreenLeft;
  int ScreenTop;
  int ScreenRight;
  int ScreenBottom;
#ifdef _USE_OPENGL
  GLuint GLTexture;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  LPDIRECT3DTEXTURE9 D3DTexture;
  LPDIRECT3DVERTEXBUFFER9 D3DVertexBuffer;
#endif /* _USE_DIRECTX */
} tDrawingBitmapContext;

typedef enum
{
  // Onscreen bitmaps first
  // Should be first to match OnscreenIcons structure
  PlaybackSessionId = 0,
  MuteId,
  JoystickId,
  DecathlonId,
  CombatSchoolId,
  // Others bitmaps
  SpinningFloppyId,
  StaticFloppyId,
  TapeId,
  // Should be the last
  NbDrawingBitmapsId
} tDrawingBitmapId;

#define NB_OSD_BITMAPS          5


// Functions prototypes
typedef void __fastcall (__closure *TDrawingFloppy)(bool bActive, bool bPause);
typedef void __fastcall (__closure *TDrawingEmulatorScreen)(void);
typedef void __fastcall (__closure *TDrawingTape)(int PlayDuration, int TotalDuration);
typedef void __fastcall (__closure *TDrawingEmulatorMessages)(TList* MessageList);
typedef void __fastcall (__closure *TDrawingEmulatorText)(String Text, int X, int Y);
typedef void __fastcall (__closure *TDrawingBegin)(void);
typedef void __fastcall (__closure *TDrawingFinalize)(void);
typedef void __fastcall (__closure *TDrawingPlayback)(void);
typedef void __fastcall (__closure *TDrawingOnscreenIconsBegin)(void);
typedef void __fastcall (__closure *TDrawingOnscreenIconsFinalize)(void);
typedef void __fastcall (__closure *TDrawingOnscreenIcon)(tDrawingBitmapId Icon);


class TDrawingContext
{
private:
  //
  // Members
  //
  TSettings* mSettingsP;
  HWND mHostWindowHandle;
  TCanvas* mHostCanvas;
  BITMAPINFO* mEmuScreenBITMAPINFO;
  HANDLE mAmstradFontHandle;
  HANDLE mDialogFontHandle;
  HFONT mEmulatorMessageFont;
  HFONT mDialogFont;
  HFONT mInfoFont;
  tDrawingBitmapContext mBitmapContext[NbDrawingBitmapsId];
  HBITMAP mGDIScanlinesMaskHBITMAP;
  BYTE* mGDIScanlinesMaskBits;
  BITMAPINFO* mGDIScanlinesMaskBITMAPINFO;
  int mEmulatorMessageFontLeading;
  int mDialogFontLeading;
  int mInfoFontLeading;
  bool mFullscreen;
  int mDisplayWidth;
  int mDisplayHeight;
  int mBitsPerPixel;
  bool mUseOpenGL;
  bool mUseDirectX;
  int mLastError;
  Graphics::TBitmap* mOffscreenBitmap;
  HDC mOnscreenHDC;
  HDC mEmuScreenHDC;
  HBITMAP mEmuScreenHBITMAP;
  BITMAP mEmuScreenBITMAP;
  void* mEmuScreenDataP;
  int mOffscreenOriginLeft;
  int mOffscreenOriginTop;
  int mEmulatorScreenLeft;
  int mEmulatorScreenTop;
  int mEmulatorScreenRight;
  int mEmulatorScreenBottom;
  int mEmulatorScreenWidth;
  int mEmulatorScreenHeight;
  int mSpinningFloppyRotateLeft;
  int mSpinningFloppyRotateTop;
  TColor mPalette[NATIVECPC_COLOURS_NBELEMENT];
  tEngineGraphicFormat* mEngineGraphicFormatP;
  // scanlines
  bool mDrawScanlines;
  int mScanlinesPattern;
  int mScanlinesWidth;
  int mScanlinesHeight;
  int mScanlinesNbAlias;
  tUChar* mScanlinesData;
  tUChar* mScanlinesAlias;
  //
  TDrawingFloppy mDrawFloppy;
  TDrawingTape mDrawTape;
  TDrawingEmulatorScreen mDrawEmulatorScreen;
  TDrawingEmulatorMessages mDrawEmulatorMessages;
  TDrawingEmulatorText mDrawEmulatorText;
  TDrawingBegin mDrawBegin;
  TDrawingFinalize mDrawFinalize;
  TDrawingOnscreenIconsBegin mDrawOnscreenIconsBegin;
  TDrawingOnscreenIconsFinalize mDrawOnscreenIconsFinalize;
  TDrawingOnscreenIcon mDrawOnscreenIcon;
  //
#ifdef _USE_OPENGL
  HGLRC mGLOnscreenHGLRC;
  GLuint mGLEmulatorMessageFontBase;
  GLuint mGLDialogFontBase;
  GLuint mGLInfoFontBase;
  GLuint mGLEmuScreenTexture;
  GLuint mGLScanlinesTexture;
  GLfloat mGLFloppyRotationAngle;
  GLfloat mGLFloppyTransparency;
  GLfloat mGLFullscreenBrightness;
  GLint mGLPreferredFormatTexture2D;
#endif /* _USE_OPENGL */
  //
#ifdef _USE_DIRECTX
  LPDIRECT3D9 mD3D;
  D3DPRESENT_PARAMETERS mD3DPresentParam;
  LPDIRECT3DDEVICE9 mD3DDevice;
  LPD3DXLINE mD3DLine;
  LPDIRECT3DTEXTURE9 mD3DEmuScreenTexture;
  LPDIRECT3DVERTEXBUFFER9 mD3DScreenVertexBuffer;
  LPDIRECT3DTEXTURE9 mD3DScanlinesTexture;
  LPDIRECT3DVERTEXBUFFER9 mD3DScreenFloppyVertexBuffer;
  LPD3DXFONT mD3DEmulatorMessageFont;
  LPD3DXFONT mD3DInfoFont;
  LPD3DXFONT mD3DDialogFont;
  float mD3DFloppyRotationAngle;
  int mD3DFloppyTransparency;
  int mD3DFullscreenBrightness;
  DWORD mOldMinFilter;
  DWORD mOldMagFilter;
#endif /* _USE_DIRECTX */
  //
  // Methods
  //
  float __fastcall GetZoomFactor(void);
  inline HDC __fastcall GetOnscreenHDC() { return mOnscreenHDC; };
  inline BITMAP __fastcall GetEmuScreenBITMAP() { return mEmuScreenBITMAP; };
  inline bool __fastcall GetFullscreen() { return mFullscreen; };
  void __fastcall SetFullscreen(bool Value);
  inline bool __fastcall GetDrawScanlines() { return mDrawScanlines; };
  inline void __fastcall SetDrawScanlines(bool Value) { mDrawScanlines = Value; };
  inline int __fastcall GetScanlinesPattern() { return mScanlinesPattern; };
  void __fastcall SetScanlinesPattern(int Value);
  void __fastcall UpdateScanlinesTexture(void);
  bool __fastcall LoadScanlinesPattern(void);
  bool __fastcall ReadScanlinesRGBValue(AnsiString Param, int* ValueP);
  inline int __fastcall GetEmulatorMessageFontLeading() { return mEmulatorMessageFontLeading; };
  inline int __fastcall GetDialogFontLeading() { return mDialogFontLeading; };
  inline int __fastcall GetInfoFontLeading() { return mInfoFontLeading; };
  inline int __fastcall GetUseOpenGL() { return mUseOpenGL; };
  inline int __fastcall GetUseDirectX() { return mUseDirectX; };
  inline HFONT __fastcall GetDialogFont() { return mDialogFont; };
	inline HFONT __fastcall GetInfoFont() { return mInfoFont; };
  double __fastcall GetEmulatorAspectRatio(void);
	HBITMAP __fastcall LoadResourceBitmap(String ResourceBitmapName);
	HBITMAP __fastcall CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);
  inline int __fastcall GetDisplayWidth() { return mDisplayWidth; };
  inline int __fastcall GetDisplayHeight() { return mDisplayHeight; };
  inline int __fastcall GetOffscreenOriginLeft() { return mOffscreenOriginLeft; };
  inline int __fastcall GetOffscreenOriginTop() { return mOffscreenOriginTop; };
  inline int __fastcall GetEmulatorScreenLeft() { return mEmulatorScreenLeft; };
  inline int __fastcall GetEmulatorScreenTop() { return mEmulatorScreenTop; };
  inline int __fastcall GetEmulatorScreenRight() { return mEmulatorScreenRight; };
  inline int __fastcall GetEmulatorScreenBottom() { return mEmulatorScreenBottom; };
  inline int __fastcall GetEmulatorScreenWidth() { return mEmulatorScreenWidth; };
  inline int __fastcall GetEmulatorScreenHeight() { return mEmulatorScreenHeight; };
  inline TColor* __fastcall GetPalette(void) { return mPalette; };
  inline int __fastcall GetBitsPerPixel(void) { return mBitsPerPixel; };
  int __fastcall GetMonitorVRefresh(void);
  unsigned int __fastcall CreateBitmapTextureGL(HBITMAP hBitmap);
  LPBITMAP __fastcall CreateTransparentBitmap(HBITMAP hBitmap);
  unsigned int __fastcall CreateTransparentBitmapTextureGL(HBITMAP hBitmap);
#ifdef _USE_DIRECTX
  HRESULT __fastcall CreateTransparentBitmapTextureD3D(HBITMAP hBitmap,
                                                       LPDIRECT3DDEVICE9 DeviceP,
                                                       LPDIRECT3DTEXTURE9 * TextureP);
#endif /* _USE_DIRECTX */
#ifdef _USE_OPENGL
  inline GLuint __fastcall GetGLDialogFontBase(void) { return mGLDialogFontBase; };
  inline GLuint __fastcall GetGLInfoFontBase(void) { return mGLDialogFontBase; };
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  inline LPDIRECT3DDEVICE9 __fastcall GetD3DDevice(void) { return mD3DDevice; };
  inline LPD3DXLINE __fastcall GetD3DLine(void) { return mD3DLine; };
  inline LPD3DXFONT __fastcall GetD3DInfoFont(void) { return mD3DInfoFont; };
  inline LPD3DXFONT __fastcall GetD3DDialogFont(void) { return mD3DDialogFont; };
#endif /* _USE_DIRECTX */
  inline TDrawingBegin __fastcall GetDrawBegin(void) { return mDrawBegin; };
  inline TDrawingFinalize __fastcall GetDrawFinalize(void) { return mDrawFinalize; };
  inline TDrawingEmulatorScreen __fastcall GetDrawEmulatorScreen(void) { return mDrawEmulatorScreen; };
  inline TDrawingFloppy __fastcall GetDrawFloppy(void) { return mDrawFloppy; };
  inline TDrawingTape __fastcall GetDrawTape(void) { return mDrawTape; };
  inline TDrawingEmulatorMessages __fastcall GetDrawEmulatorMessages(void) { return mDrawEmulatorMessages; };
  inline TDrawingEmulatorText __fastcall GetDrawEmulatorText(void) { return mDrawEmulatorText; };
  inline TDrawingOnscreenIconsBegin __fastcall GetDrawOnscreenIconsBegin(void) { return mDrawOnscreenIconsBegin; };
  inline TDrawingOnscreenIconsFinalize __fastcall GetDrawOnscreenIconsFinalize(void) { return mDrawOnscreenIconsFinalize; };
  inline TDrawingOnscreenIcon __fastcall GetDrawOnscreenIcon(void) { return mDrawOnscreenIcon; };
  void __fastcall setupPixelFormatGL(HDC hDC);
  void __fastcall CreateScanlineTextureGDI(int ScanlineMask);
  void __fastcall ReleaseD3DDevice(void);
  bool __fastcall InitD3D(void);
  bool __fastcall InitD3DDevice(void);
  bool __fastcall InitGL(void);
  void __fastcall DrawBeginD3D(void);
  void __fastcall DrawBeginGL(void);
  void __fastcall DrawBeginGDI(void);
  void __fastcall DrawFinalizeD3D(void);
  void __fastcall DrawFinalizeGL(void);
  void __fastcall DrawFinalizeGDI(void);
  void __fastcall DrawEmulatorScreenD3D(void);
  void __fastcall DrawEmulatorScreenGL(void);
  void __fastcall DrawEmulatorScreenGDI(void);
  void __fastcall DrawSpinningFloppyD3D(bool bActive, bool bPause);
  void __fastcall DrawStaticFloppyD3D(bool bActive, bool bPause);
  void __fastcall DrawSpinningFloppyGL(bool bActive, bool bPause);
  void __fastcall DrawStaticFloppyGL(bool bActive, bool bPause);
  void __fastcall DrawStaticFloppyGDI(bool bActive, bool bPause);
  void __fastcall DrawTapeD3D(int PlayDuration, int TotalDuration);
  void __fastcall DrawTapeGL(int PlayDuration, int TotalDuration);
  void __fastcall DrawTapeGDI(int PlayDuration, int TotalDuration);
  void __fastcall DrawEmulatorMessagesD3D(TList* MessageList);
  void __fastcall DrawEmulatorMessagesGL(TList* MessageList);
  void __fastcall DrawEmulatorMessagesGDI(TList* MessageList);
	void __fastcall DrawEmulatorTextD3D(String Text, int X, int Y);
	void __fastcall DrawEmulatorTextGL(String Text, int X, int Y);
	void __fastcall DrawEmulatorTextGDI(String Text, int X, int Y);
  void __fastcall DrawOnscreenIconsBeginD3D(void);
  void __fastcall DrawOnscreenIconsBeginGL(void);
  void __fastcall DrawOnscreenIconsBeginGDI(void);
  void __fastcall DrawOnscreenIconsFinalizeD3D(void);
  void __fastcall DrawOnscreenIconsFinalizeGL(void);
  void __fastcall DrawOnscreenIconsFinalizeGDI(void);
  void __fastcall DrawOnscreenIconD3D(tDrawingBitmapId Icon);
  void __fastcall DrawOnscreenIconGL(tDrawingBitmapId Icon);
  void __fastcall DrawOnscreenIconGDI(tDrawingBitmapId Icon);
  unsigned char* __fastcall CreateScreenshot(bool Mode2, int* SizeP);

public:
  //
  // Properties
  //
  __property HDC OnscreenHDC = { read=GetOnscreenHDC };
  __property BITMAP EmuScreenBITMAP = { read=GetEmuScreenBITMAP };
  __property bool Fullscreen = { read=GetFullscreen,write=SetFullscreen };
  __property bool DrawScanlines = { read=GetDrawScanlines,write=SetDrawScanlines };
  __property int EmulatorMessageFontLeading = { read=GetEmulatorMessageFontLeading };
  __property int DialogFontLeading = { read=GetDialogFontLeading };
  __property int InfoFontLeading = { read=GetInfoFontLeading };
  __property bool UseOpenGL = { read=GetUseOpenGL };
  __property bool UseDirectX = { read=GetUseDirectX };
  __property HFONT InfoFont = { read=GetInfoFont };
  __property HFONT DialogFont = { read=GetDialogFont };
  __property int DisplayWidth = { read=GetDisplayWidth };
  __property int DisplayHeight = { read=GetDisplayHeight };
  __property int OffscreenOriginLeft = { read=GetOffscreenOriginLeft };
  __property int OffscreenOriginTop = { read=GetOffscreenOriginTop };
  __property int EmulatorScreenLeft = { read=GetEmulatorScreenLeft };
  __property int EmulatorScreenTop = { read=GetEmulatorScreenTop };
  __property int EmulatorScreenRight = { read=GetEmulatorScreenRight };
  __property int EmulatorScreenBottom = { read=GetEmulatorScreenBottom };
  __property int EmulatorScreenWidth = { read=GetEmulatorScreenWidth };
  __property int EmulatorScreenHeight = { read=GetEmulatorScreenHeight };
  __property double EmulatorAspectRatio = { read=GetEmulatorAspectRatio };
  __property TColor* Palette = { read=GetPalette };
  __property int BitsPerPixel = { read=GetBitsPerPixel };
  __property int MonitorVRefresh = { read=GetMonitorVRefresh };
  __property float ZoomFactor = { read=GetZoomFactor };
  __property TDrawingFloppy DrawFloppy = { read=GetDrawFloppy };
  __property TDrawingTape DrawTape = { read=GetDrawTape };
  __property TDrawingEmulatorScreen DrawEmulatorScreen = { read=GetDrawEmulatorScreen };
  __property TDrawingEmulatorMessages DrawEmulatorMessages = { read=GetDrawEmulatorMessages };
  __property TDrawingEmulatorText DrawEmulatorText = { read=GetDrawEmulatorText };
  __property TDrawingBegin DrawBegin = { read=GetDrawBegin };
  __property TDrawingFinalize DrawFinalize = { read=GetDrawFinalize };
  __property TDrawingOnscreenIconsBegin DrawOnscreenIconsBegin = { read=GetDrawOnscreenIconsBegin };
  __property TDrawingOnscreenIconsFinalize DrawOnscreenIconsFinalize = { read=GetDrawOnscreenIconsFinalize };
  __property TDrawingOnscreenIcon DrawOnscreenIcon = { read=GetDrawOnscreenIcon };
#ifdef _USE_OPENGL
  __property GLuint GLDialogFontBase = { read=GetGLDialogFontBase };
  __property GLuint GLInfoFontBase = { read=GetGLInfoFontBase };
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
  __property LPDIRECT3DDEVICE9 D3DDevice = { read=GetD3DDevice };
  __property LPD3DXLINE D3DLine = { read=GetD3DLine };
  __property LPD3DXFONT D3DInfoFont = { read=GetD3DInfoFont };
  __property LPD3DXFONT D3DDialogFont = { read=GetD3DDialogFont };
#endif /* _USE_DIRECTX */
  //
  // API
  //
  __fastcall TDrawingContext(TSettings* SettingsP);
  __fastcall ~TDrawingContext();

  TStringList* __fastcall GetInfo(void);

  bool __fastcall Init(HWND Handle, bool RequestOpenGL, bool ForceGDI);

  void __fastcall CreateEmulatorMessageFont();
  void __fastcall CreateDialogFont();
  void __fastcall CreateInfoFont();
#ifdef _USE_OPENGL
  GLuint __fastcall GLBuildFontList(HFONT Font);
  void __fastcall   GLBuildEmulatorMessageFontList();
#endif /* _USE_OPENGL */
	SIZE __fastcall GetEmulatorMessageExtent(String Message);
	SIZE __fastcall GetDialogExtent(String Message);
	SIZE __fastcall GetInfoExtent(String Message);

	bool __fastcall CreateEmulatorOffscreenBitmap();

	void __fastcall OnDisplayResize(int NewWidth, int NewHeight);

  void __fastcall SetupPalette(void);
  void __fastcall setupPaletteOpenGL(void);
  void __fastcall setupPaletteGDI(void);

  void __fastcall DrawVDUBeamPosition(int EmuScreenX, int EmuScreenY);

  int __fastcall GetLastError(void);

	HANDLE __fastcall LoadResourceFont(String FontName);
	void __fastcall FreeResourceFont(HANDLE FontHandle);

  bool __fastcall IncreaseBrightness(void);
  bool __fastcall DecreaseBrightness(void);

  bool __fastcall SaveScreenshot(AnsiString Filename, bool Mode2);
  bool __fastcall CompareScreenshot(AnsiString Filename);
};

//---------------------------------------------------------------------------
#endif

