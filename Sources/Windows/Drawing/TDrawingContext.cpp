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
#include "TDrawingContext.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//=================================
// Options
//
#undef FORCE_VIDEO_RENDERING
//#define FORCE_VIDEO_RENDERING       8

#undef FORCE_FLOPPY_BITMAP_DRAWING
//#define FORCE_FLOPPY_BITMAP_DRAWING

#undef USE_GDI_SCANLINES
//#define USE_GDI_SCANLINES
//
//=================================


//===================
// PATCH begin
#ifdef _PATCH_ENABLE

#endif /* _PATCH_ENABLE */
// PATCH end
//===================


#define MESSAGE_FONTNAME        "Amstrad CPC464 Regular"
#define DIALOG_FONTNAME         "BlueHighway-Regular"
#define INFO_FONTNAME           "BlueHighway-Regular"

// LowerCase
#define PATTERN_EXT                ".pat"

#define OPENGL_VERSION_MAJOR            3
#define OPENGL_VERSION_MINOR            1

#define ZOOM_FACTOR_D3D                 2.0f
#define ZOOM_FACTOR_GL                  2.0f
#define ZOOM_FACTOR_GDI                 2.0f

#define BRIGHTNESS_INITIALVALUE         96
#define BRIGHTNESS_MAXVALUE             250
#define BRIGHTNESS_MINVALUE             10
#define BRIGHTNESS_STEP                 10

#define DIALOGFONT_SIZE                 0.04f
#define INFOFONT_SIZE                   0.04f

#define EMULATORMESSAGE_OFFSETX         5
#define EMULATORMESSAGE_OFFSETY         5
#define EMULATORMESSAGE_FIRSTPOS        0
#define EMULATORMESSAGE_POSSTEP         10

// Floppy display
#define FLOPPY_BITMAP_POS_X                   0
#define FLOPPY_BITMAP_POS_Y                   0
#define FLOPPY_ALPHASTEP_GL                   0.1f
#define FLOPPY_ALPHASTEP_D3D                  25
#define FLOPPY_ROTATION_STEP                  5.0f

// Tape display
#define TAPE_BITMAP_POS_X                     0
#define TAPE_BITMAP_POS_Y                     8
#define TAPE_DURATION_TEXT_POS_X              11
#define TAPE_DURATION_TEXT_POS_Y              36
#define TAPE_TOTAL_DURATION_TEXT_POS_Y        2
#define TAPE_POS_Y_OFFSET_GL                  8

// Pattern keywords (lowercase)
#define PATTERN_WIDTH                         "width"
#define PATTERN_HEIGHT                        "height"
#define PATTERN_NBALIAS                       "nbalias"
#define PATTERN_ALIAS                         "alias"
#define PATTERN_PIXEL                         "pixel"

#define PATTERN_MAX_ALIAS                     10


//
// Onscreen Icons
//
typedef struct
{
  char* BitmapResource;
  char* BitmapMaskResource;
  // Left Position from Screen Left (valid if positive)
  int Left;
  // Right Position from Screen Right (valid if positive)
  int Right;
  // Top Position from Screen Top (valid if positive)
  int Top;
  // Bottom Position from Screen Bottom (valid if positive)
  int Bottom;
} tOnscreenIcon;

static const tOnscreenIcon OnscreenIcons[] =
{
  { "OSD_PLAYBACK", "OSD_PLAYBACKMASK", 0, -1, -1, 0 },
  { "OSD_MUTE", "OSD_MUTEMASK", -1, 0, -1, 0 },
  { "OSD_JOYSTICK", "OSD_JOYSTICKMASK", -1, 32, -1, 0 },
  { "OSD_DECATHLON", "OSD_DECATHLONMASK", -1, 64, -1, 0 },
  { "OSD_COMBATSCHOOL", "OSD_COMBATSCHOOLMASK", -1, 64, -1, 0 },
};


//
// BMP FORMAT
//
#pragma pack(push, 1)

typedef struct
{
	tUChar SignatureB;
	tUChar SignatureM;
	tULong FileSize;
  tULong Reserved;
  tULong Offset;
}tBitmapFileHeader;

typedef struct
{
  tULong Size;
  tULong ImageWidth;
  tULong ImageHeight;
  tUShort NbPlanes;
  tUShort BitsPerPixel;
  tULong CompressionType;
  tULong ImageSize;
  tULong HorizontalResolution;
  tULong VerticalResolution;
  tULong NbColors;
  tULong NbImportantColors;
}tBitmapHeader;


#define SCREENSHOT_NB_COLORS            32
#define SCREENSHOT_COLOR_SIZE           4
#define SCREENSHOT_IMAGE_WIDTH          384L
#define SCREENSHOT_IMAGE_WIDTH_MODE2    768L
#define SCREENSHOT_IMAGE_HEIGHT         272L


static const tBitmapFileHeader BitmapFileHeader =
{
  'B', 'M',
  0,
  0,
  0
};

static const tBitmapHeader BitmapHeader =
{
  sizeof(tBitmapHeader),
  0,
  0,
  1,                                  // Number of planes
  0,                                  // Bits per pixel
  0,                                  // Type of compression
  0,                                  //
  0,                                  // Horizontal resolution
  0,                                  // Vertical resolution
  0,                                  // Number of colors
  0                                   // Number of important colors
};

#pragma pack(pop)


struct
{
  BITMAPINFOHEADER bmiHeader;
  RGBQUAD bmiColors[NATIVECPC_COLOURS_NBELEMENT];
}tEmulatorBitmapInfo;


#ifdef _USE_DIRECTX

#define D3DFVF_BITMAPVERTEX ( D3DFVF_XYZRHW | D3DFVF_TEX3 )
struct D3DBitmapVertex
{
    float x, y, z, rhw;
    float tu, tv;
};
#define D3DFVF_SCREENVERTEX ( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEX2 )
struct D3DScreenVertex
{
    float x, y, z, rhw;
    float tu1, tv1;
    float tu2, tv2;
};

#define D3D_BITMAP_VERTEXBUFFER_SIZE    (4 * sizeof(D3DBitmapVertex))

#endif /* _USE_DIRECTX */


__fastcall TDrawingContext::TDrawingContext(TSettings* SettingsP)
//
// Constructor
//
{
  mSettingsP = SettingsP;
  
  mOnscreenHDC = NULL;
  mEmuScreenHDC = NULL;
  mEmuScreenHBITMAP = NULL;
  mEmuScreenBITMAPINFO = NULL;

  mGDIScanlinesMaskBits = NULL;
  mGDIScanlinesMaskBITMAPINFO = NULL;

  mHostCanvas = NULL;
  mOffscreenBitmap = NULL;

  mEmulatorMessageFont = NULL;
  mDialogFont = NULL;
  mInfoFont = NULL;

  mScanlinesWidth = 0;
  mScanlinesHeight = 0;
  mScanlinesData = NULL;
  mScanlinesNbAlias = 0;
  mScanlinesAlias = NULL;

  for (int Loop=0; Loop < NbDrawingBitmapsId; Loop++)
  {
    tDrawingBitmapContext* BitmapContextP = &mBitmapContext[Loop];

    BitmapContextP->GDIHandle = NULL;
    BitmapContextP->GDIMaskHandle = NULL;
    BitmapContextP->Width = -1;
    BitmapContextP->Height = -1;
    BitmapContextP->ScreenWidth = -1;
    BitmapContextP->ScreenHeight = -1;
    BitmapContextP->ScreenLeft = -1;
    BitmapContextP->ScreenTop = -1;
    BitmapContextP->ScreenRight = -1;
    BitmapContextP->ScreenBottom = -1;
#ifdef _USE_OPENGL
    BitmapContextP->GLTexture = 0;
#endif /* _USE_OPENGL */
#ifdef _USE_DIRECTX
    BitmapContextP->D3DTexture = NULL;
    BitmapContextP->D3DVertexBuffer = NULL;
#endif /* _USE_DIRECTX */
  }

  // Get Engine Graphic format
  mEngineGraphicFormatP = new tEngineGraphicFormat;
  try
  {
    Engine_GetGraphicFormat(mEngineGraphicFormatP);
  }
  catch(...)
  {
    mEngineGraphicFormatP = NULL;
  }
  
  // Wrap graphic routines to default GDI functions
  mDrawBegin = DrawBeginGDI;
  mDrawFinalize = DrawFinalizeGDI;
  mDrawEmulatorScreen = DrawEmulatorScreenGDI;
  mDrawFloppy = DrawStaticFloppyGDI;
  mDrawTape = DrawTapeGDI;
  mDrawEmulatorMessages = DrawEmulatorMessagesGDI;
  mDrawEmulatorText = DrawEmulatorTextGDI;
  mDrawOnscreenIconsBegin = DrawOnscreenIconsBeginGDI;
  mDrawOnscreenIconsFinalize = DrawOnscreenIconsFinalizeGDI;
  mDrawOnscreenIcon = DrawOnscreenIconGDI;

  mBitmapContext[SpinningFloppyId].GDIHandle = LoadResourceBitmap("OSD_FLOPPY_SPIN");
  mBitmapContext[StaticFloppyId].GDIHandle = LoadResourceBitmap("OSD_FLOPPYGDI");
  mBitmapContext[StaticFloppyId].GDIMaskHandle = LoadResourceBitmap("OSD_FLOPPYGDIMASK");
  mBitmapContext[TapeId].GDIHandle = LoadResourceBitmap("OSD_TAPE");
  mBitmapContext[TapeId].GDIMaskHandle = LoadResourceBitmap("OSD_TAPEMASK");
  for (int Loop=0; Loop < NB_OSD_BITMAPS; Loop++)
  {
    mBitmapContext[Loop].GDIHandle = LoadResourceBitmap(OnscreenIcons[Loop].BitmapResource);
    mBitmapContext[Loop].GDIMaskHandle = LoadResourceBitmap(OnscreenIcons[Loop].BitmapMaskResource);
  }

  mGDIScanlinesMaskHBITMAP = NULL;
  CreateScanlineTextureGDI(0);

  // Get floppy bitmap size
  BITMAP Bitmap;
  tDrawingBitmapContext* BitmapContextP = &mBitmapContext[SpinningFloppyId];
  GetObject(BitmapContextP->GDIHandle, sizeof(BITMAP), &Bitmap);
  BitmapContextP->Width = Bitmap.bmWidth;
  BitmapContextP->Height = Bitmap.bmHeight;
  BitmapContextP->ScreenWidth = Bitmap.bmWidth * SettingsP->DriveIconSizeFactor / 100;
  BitmapContextP->ScreenWidth &= 0xFFFFFFFE; // Keep even part for perfect rotation
  BitmapContextP->ScreenHeight = BitmapContextP->ScreenWidth;

  // Get GDI floppy bitmap size
  BitmapContextP = &mBitmapContext[StaticFloppyId];
  GetObject(BitmapContextP->GDIHandle, sizeof(BITMAP), &Bitmap);
  BitmapContextP->Width = Bitmap.bmWidth;
  BitmapContextP->Height = Bitmap.bmHeight;
  BitmapContextP->ScreenWidth = Bitmap.bmWidth * SettingsP->DriveIconSizeFactor / 100;
  BitmapContextP->ScreenHeight = Bitmap.bmHeight * SettingsP->DriveIconSizeFactor / 100;

  // Get tape bitmap size
  BitmapContextP = &mBitmapContext[TapeId];
  GetObject(BitmapContextP->GDIHandle, sizeof(BITMAP), &Bitmap);
  BitmapContextP->ScreenWidth = Bitmap.bmWidth;
  BitmapContextP->ScreenHeight = Bitmap.bmHeight;

  // Get Onscreen icons bitmap size
  for (int Loop=0; Loop < NB_OSD_BITMAPS; Loop++)
  {
    BitmapContextP = &mBitmapContext[Loop];
    GetObject(mBitmapContext[Loop].GDIHandle, sizeof(BITMAP), &Bitmap);
    BitmapContextP->ScreenWidth = Bitmap.bmWidth;
    BitmapContextP->ScreenHeight = Bitmap.bmHeight;
  }

  mAmstradFontHandle = LoadResourceFont("FONT_AMSTRAD");
  mDialogFontHandle = LoadResourceFont("FONT_DIALOG");

  mLastError = 0;

  mUseOpenGL = false;
  mUseDirectX = false;
  mFullscreen = false;
  mDrawScanlines = SettingsP->DrawScanlines;
  //mScanlinesPattern = SettingsP->ScanlinesPattern;

  //
  // Get number of bits per pixel
  //
#if FORCE_VIDEO_RENDERING == 32
  mBitsPerPixel = 32;
#elif FORCE_VIDEO_RENDERING == 24
  mBitsPerPixel = 24;
#elif FORCE_VIDEO_RENDERING == 8
  mBitsPerPixel = 8;
#else
  mBitsPerPixel = GetDeviceCaps(GetDC(NULL), BITSPIXEL);
#endif
  if (mEngineGraphicFormatP)
  {
    // Check max bits per pixel
    if (mBitsPerPixel > (int)mEngineGraphicFormatP->MaxBitPerPixel)
    {
      mBitsPerPixel = (int)mEngineGraphicFormatP->MaxBitPerPixel;
    }
  }

#ifdef _USE_OPENGL

  mGLOnscreenHGLRC = NULL;
  mGLEmuScreenTexture = 0;
  mGLScanlinesTexture = 0;
  mGLEmulatorMessageFontBase = 0;
  mGLDialogFontBase = 0;
  mGLDialogFontBase = 0;
  mGLFloppyRotationAngle = 0.0f;
  mGLFloppyTransparency = 0.0f;
  mGLFullscreenBrightness = SettingsP->FullscreenBrightness;
  if (mGLFullscreenBrightness < BRIGHTNESS_MINVALUE)
  {
    mGLFullscreenBrightness = BRIGHTNESS_MINVALUE;
  }
  else if (mGLFullscreenBrightness > BRIGHTNESS_MAXVALUE)
  {
    mGLFullscreenBrightness = BRIGHTNESS_MAXVALUE;
  }

#endif /* _USE_OPENGL */

#ifdef _USE_DIRECTX

  mD3D = NULL;
  mD3DDevice = NULL;
  mD3DLine = NULL;
  mD3DEmuScreenTexture = NULL;
  mD3DScanlinesTexture = NULL;
  mD3DScreenVertexBuffer = NULL;
  mD3DScreenFloppyVertexBuffer = NULL;
  mD3DEmulatorMessageFont = NULL;
  mD3DInfoFont = NULL;
  mD3DDialogFont = NULL;
  mD3DFloppyRotationAngle = 0.0f;
  mD3DFloppyTransparency = 0.0f;
  mD3DFullscreenBrightness =
  mD3DFullscreenBrightness = SettingsP->FullscreenBrightness;
  if (mD3DFullscreenBrightness < BRIGHTNESS_MINVALUE)
  {
    mD3DFullscreenBrightness = BRIGHTNESS_MINVALUE;
  }
  else if (mD3DFullscreenBrightness > BRIGHTNESS_MAXVALUE)
  {
    mD3DFullscreenBrightness = BRIGHTNESS_MAXVALUE;
  }

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


__fastcall TDrawingContext::~TDrawingContext()
//
// Destructor
//
{
  FreeResourceFont(mAmstradFontHandle);
  FreeResourceFont(mDialogFontHandle);

  DeleteObject(mEmuScreenHBITMAP);
  DeleteObject(mGDIScanlinesMaskHBITMAP);
  
  DeleteObject(mEmulatorMessageFont);
  DeleteObject(mDialogFont);
  DeleteObject(mInfoFont);

  for (int Loop=0; Loop < NbDrawingBitmapsId; Loop++)
  {
    DeleteObject(mBitmapContext[Loop].GDIHandle);
    DeleteObject(mBitmapContext[Loop].GDIMaskHandle);
  }

  DeleteDC(mEmuScreenHDC);
  delete [] mEmuScreenBITMAPINFO;

  delete [] mGDIScanlinesMaskBits;
  delete [] mGDIScanlinesMaskBITMAPINFO;

  delete [] mScanlinesData;
  delete [] mScanlinesAlias;

  if (mOffscreenBitmap != NULL)
  {
    delete mOffscreenBitmap;
  }
  else
  {
    ReleaseDC((HWND)mHostWindowHandle, mOnscreenHDC);
  }

  if (mHostCanvas != NULL)
  {
    ReleaseDC((HWND)mHostWindowHandle, mHostCanvas->Handle);
  }
  delete mHostCanvas;

#ifdef _USE_OPENGL

  glDeleteTextures(1, &mGLEmuScreenTexture);
  glDeleteTextures(1, &mGLScanlinesTexture);
  for (int Loop=0; Loop < NbDrawingBitmapsId; Loop++)
  {
    glDeleteTextures(1, &mBitmapContext[Loop].GLTexture);
  }

  if (mGLEmulatorMessageFontBase)
  {
    glDeleteLists(mGLEmulatorMessageFontBase, 96);
  }
  if (mGLDialogFontBase)
  {
    glDeleteLists(mGLDialogFontBase, 96);
  }
  if (mGLDialogFontBase)
  {
    glDeleteLists(mGLDialogFontBase, 96);
  }
  wglMakeCurrent(mOnscreenHDC, NULL);
  // Prevent crash after OBS Studio use.
  if (mGLOnscreenHGLRC != NULL)
  {
    wglDeleteContext(mGLOnscreenHGLRC);
  }

#endif /* _USE_OPENGL */

#ifdef _USE_DIRECTX

  ReleaseD3DDevice();

  if (mD3DDevice != NULL)
  {
    mD3DDevice->Release();
  }
  if (mD3D != NULL)
  {
    mD3D->Release();
  }

#endif /* _USE_DIRECTX */

  delete mEngineGraphicFormatP;
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::ReleaseD3DDevice(void)
{
#ifdef _USE_DIRECTX

  if (mD3DEmuScreenTexture != NULL)
  {
    mD3DEmuScreenTexture->Release();
    mD3DEmuScreenTexture = NULL;
  }
  if (mD3DScreenVertexBuffer != NULL)
  {
    mD3DScreenVertexBuffer->Release();
    mD3DScreenVertexBuffer = NULL;
  }
  if (mD3DScanlinesTexture != NULL)
  {
    mD3DScanlinesTexture->Release();
    mD3DScanlinesTexture = NULL;
  }

  if (mD3DScreenFloppyVertexBuffer != NULL)
  {
    mD3DScreenFloppyVertexBuffer->Release();
    mD3DScreenFloppyVertexBuffer = NULL;
  }

  for (int Loop=0; Loop < NbDrawingBitmapsId; Loop++)
  {
    tDrawingBitmapContext* BitmapContextP = &mBitmapContext[Loop];
    if (BitmapContextP->D3DTexture != NULL)
    {
      BitmapContextP->D3DTexture->Release();
      BitmapContextP->D3DTexture = NULL;
    }
    if (BitmapContextP->D3DVertexBuffer != NULL)
    {
      BitmapContextP->D3DVertexBuffer->Release();
      BitmapContextP->D3DVertexBuffer = NULL;
    }
  }

  if (mD3DEmulatorMessageFont != NULL)
  {
    mD3DEmulatorMessageFont->Release();
    mD3DEmulatorMessageFont = NULL;
  }
  if (mD3DInfoFont != NULL)
  {
    mD3DInfoFont->Release();
    mD3DInfoFont = NULL;
  }
  if (mD3DDialogFont != NULL)
  {
    mD3DDialogFont->Release();
    mD3DDialogFont = NULL;
  }
  if (mD3DLine != NULL)
  {
    mD3DLine->Release();
    mD3DLine = NULL;
  }

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


TStringList* __fastcall TDrawingContext::GetInfo(void)
{
  TStringList* InfoStrings = new TStringList;
  HDC hDC = GetDC(NULL);

  InfoStrings->Add("Display");
  InfoStrings->Add("=======");
  InfoStrings->Add("Width=" + AnsiString(mDisplayWidth));
  InfoStrings->Add("Height=" + AnsiString(mDisplayHeight));
  InfoStrings->Add("BitsPerPixel=" + AnsiString(mBitsPerPixel));
  InfoStrings->Add("");

  InfoStrings->Add("Windows GDI");
  InfoStrings->Add("===========");
  InfoStrings->Add("DRIVERVERSION=" + AnsiString(GetDeviceCaps(hDC, DRIVERVERSION)));
  InfoStrings->Add("TECHNOLOGY=" + AnsiString::IntToHex(GetDeviceCaps(hDC, TECHNOLOGY),8));
  InfoStrings->Add("BITSPIXEL=" + AnsiString(GetDeviceCaps(hDC, BITSPIXEL)));
  InfoStrings->Add("PLANES=" + AnsiString(GetDeviceCaps(hDC, PLANES)));
  InfoStrings->Add("VREFRESH=" + AnsiString(GetDeviceCaps(hDC, VREFRESH)));
  InfoStrings->Add("BLTALIGNMENT=" + AnsiString(GetDeviceCaps(hDC, BLTALIGNMENT)));
  InfoStrings->Add("SHADEBLENDCAPS=" + AnsiString::IntToHex(GetDeviceCaps(hDC, SHADEBLENDCAPS),8));
  // RASTERCAPS
  int RasterCaps = GetDeviceCaps(hDC, RASTERCAPS);
  AnsiString Info = "RASTERCAPS=" + AnsiString::IntToHex(RasterCaps,8);
  if ((RasterCaps & RC_BITBLT) == RC_BITBLT) Info += " [BITBLT]";
  if ((RasterCaps & RC_STRETCHBLT) == RC_STRETCHBLT) Info += " [STRETCHBLT]";
  if ((RasterCaps & RC_STRETCHDIB) == RC_STRETCHDIB) Info += " [STRETCHDIB]";
  if ((RasterCaps & RC_DI_BITMAP) == RC_DI_BITMAP) Info += " [DI_BITMAP]";
  if ((RasterCaps & RC_PALETTE) == RC_PALETTE) Info += " [PALETTE]";
  if ((RasterCaps & RC_SCALING) == RC_SCALING) Info += " [SCALING]";
  InfoStrings->Add(Info);
  //
  InfoStrings->Add("CURVECAPS=" + AnsiString::IntToHex(GetDeviceCaps(hDC, CURVECAPS),8));
  InfoStrings->Add("LINECAPS=" + AnsiString::IntToHex(GetDeviceCaps(hDC, LINECAPS),8));
  InfoStrings->Add("POLYGONALCAPS=" + AnsiString::IntToHex(GetDeviceCaps(hDC, POLYGONALCAPS),8));
  InfoStrings->Add("TEXTCAPS=" + AnsiString::IntToHex(GetDeviceCaps(hDC, TEXTCAPS),8));
  InfoStrings->Add("");

#ifdef _USE_OPENGL

  if (mUseOpenGL == true)
  {
    InfoStrings->Add("OpenGL");
    InfoStrings->Add("======");
    InfoStrings->Add("GL_VERSION = " + AnsiString((char*)glGetString(GL_VERSION)));
    InfoStrings->Add("GL_VENDOR = " + AnsiString((char*)glGetString(GL_VENDOR)));
    InfoStrings->Add("GL_RENDERER = " + AnsiString((char*)glGetString(GL_RENDERER)));

    int OpenGLVersion[2];
  	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	  glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
    InfoStrings->Add("GL_MAJOR_VERSION = " + AnsiString(OpenGLVersion[0]));
    InfoStrings->Add("GL_MINOR_VERSION = " + AnsiString(OpenGLVersion[1]));

    InfoStrings->Add("SWAP_INTERVAL = " + AnsiString(wglGetSwapIntervalEXT()));

    if (wglewIsSupported("WGL_ARB_create_context") == 1)
    {
      InfoStrings->Add("WGL_ARB_create_context supported");
    }

    InfoStrings->Add("GL_EXTENSIONS = " + AnsiString((char*)glGetString(GL_EXTENSIONS)));
    InfoStrings->Add("");
  }

#endif /* _USE_OPENGL */

#ifdef _USE_DIRECTX

  if (mUseDirectX == true)
  {
    InfoStrings->Add("DirectX 9");
    InfoStrings->Add("=========");
    InfoStrings->Add("");
  }

#endif /* _USE_DIRECTX */

  ReleaseDC(NULL, hDC);

  return InfoStrings;
}
//---------------------------------------------------------------------------


bool __fastcall TDrawingContext::Init(HWND Handle,
                                      bool RequestOpenGL,
                                      bool ForceGDI)
{
  // Initialise screen
  mHostWindowHandle = Handle;
  mOnscreenHDC = GetDC(Handle);
  CreateEmulatorMessageFont();

  mHostCanvas = new TCanvas;
  mHostCanvas->Handle = GetDC(Handle);

  mUseOpenGL = false;
  mUseDirectX = false;

  // Try DirectX
  if ( (RequestOpenGL == false) && (ForceGDI == false) )
  {
    mUseDirectX = InitD3D();
  }

  // No DirectX, try OpenGL
  if ( (mUseDirectX == false) && (ForceGDI == false) )
  {
    mUseOpenGL = InitGL();
  }

  if ( (mUseOpenGL == false) && (mUseDirectX == false) )
  {
    ReleaseDC(mHostWindowHandle, mOnscreenHDC);
    mOnscreenHDC = NULL;
  }

  return cTrue;
}
//---------------------------------------------------------------------------

bool __fastcall TDrawingContext::InitD3D(void)
{
bool InitOK = false;

#ifdef _USE_DIRECTX

  do
  {
    HRESULT Result;

    if (mEngineGraphicFormatP == NULL)
      continue;

    mD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (mD3D == NULL)
    {
#ifdef _DEBUG
			Application->MessageBox(_T("Direct3DCreate9 NULL"), _T("TDrawingContext::InitD3D"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    D3DDISPLAYMODE d3ddm;
    if (FAILED(Result = mD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
                                                    &d3ddm)))
    {
#ifdef _DEBUG
			String msg = _T("GetAdapterDisplayMode Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3D"), MB_OK);
#else /* _DEBUG */
      NOT_USED(Result);
#endif /* _DEBUG */
      continue;
    }

    // Presentation parameters
    ZeroMemory(&mD3DPresentParam, sizeof(mD3DPresentParam));

    mD3DPresentParam.Windowed             = TRUE;
    mD3DPresentParam.SwapEffect           = D3DSWAPEFFECT_FLIP;
    mD3DPresentParam.BackBufferFormat     = d3ddm.Format;
    mD3DPresentParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    mD3DPresentParam.BackBufferWidth      = mDisplayWidth;
		mD3DPresentParam.BackBufferHeight     = mDisplayHeight;
    mD3DPresentParam.BackBufferCount      = 1;
    mD3DPresentParam.hDeviceWindow        = mHostWindowHandle;

    if (FAILED(Result = mD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                           D3DDEVTYPE_HAL,
                                           mHostWindowHandle,
                                           D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                           &mD3DPresentParam,
                                           &mD3DDevice)))
    {
#ifdef _DEBUG
			String msg = _T("CreateDevice Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3D"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Device Init
    if (InitD3DDevice() == false)
      continue;

    // Init DirectX OK
    InitOK = true;

    // Wrap graphic routines to DirectX functions
    mDrawBegin = DrawBeginD3D;
    mDrawFinalize = DrawFinalizeD3D;
    mDrawEmulatorScreen = DrawEmulatorScreenD3D;
    if (mSettingsP->ShowStaticDriveIcon == false)
    {
      mDrawFloppy = DrawSpinningFloppyD3D;
    }
    else
    {
      mDrawFloppy = DrawStaticFloppyD3D;
    }
    mDrawTape = DrawTapeD3D;
    mDrawEmulatorMessages = DrawEmulatorMessagesD3D;
    mDrawEmulatorText = DrawEmulatorTextD3D;
    mDrawOnscreenIconsBegin = DrawOnscreenIconsBeginD3D;
    mDrawOnscreenIconsFinalize = DrawOnscreenIconsFinalizeD3D;
    mDrawOnscreenIcon = DrawOnscreenIconD3D;
  }
  while (0);

#endif /* _USE_DIRECTX */

  return InitOK;
}
//---------------------------------------------------------------------------
bool __fastcall TDrawingContext::InitD3DDevice(void)
{
bool InitOK = false;

#ifdef _USE_DIRECTX

  do
  {
    HRESULT Result;

    // Line drawing support
    if (FAILED(Result = D3DXCreateLine(mD3DDevice, &mD3DLine)))
    {
#ifdef _DEBUG
			String msg = _T("D3DXCreateLine Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#else /* _DEBUG */
      NOT_USED(Result);
#endif /* _DEBUG */
      continue;
    }

    //
    // Screen texture
    //
    if (FAILED(Result = mD3DDevice->CreateVertexBuffer(4 * sizeof(D3DScreenVertex),
                                                       D3DUSAGE_WRITEONLY,
                                                       D3DFVF_SCREENVERTEX,
                                                       D3DPOOL_DEFAULT,
                                                       &mD3DScreenVertexBuffer,
                                                       NULL)))
    {
#ifdef _DEBUG
			String msg = _T("CreateVertexBuffer Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

	  if (FAILED(Result = D3DXCreateTexture(mD3DDevice,
		                                      mEngineGraphicFormatP->ScreenWidth,
		                                      mEngineGraphicFormatP->VisibleScreenHeight,
                                          1,
                                          D3DUSAGE_DYNAMIC,
                                          D3DFMT_R8G8B8,
                                          D3DPOOL_DEFAULT,
                                          &mD3DEmuScreenTexture)))
    {
#ifdef _DEBUG
			String msg = _T("D3DXCreateTexture Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    //
    // Tape texture
    //
    tDrawingBitmapContext* BitmapContextP = &mBitmapContext[TapeId];
	  if (FAILED(Result = CreateTransparentBitmapTextureD3D(BitmapContextP->GDIHandle,
                                                          mD3DDevice,
                                                          &BitmapContextP->D3DTexture)))
    {
      continue;
    }
    if (FAILED(Result = mD3DDevice->CreateVertexBuffer(D3D_BITMAP_VERTEXBUFFER_SIZE,
                                                       D3DUSAGE_WRITEONLY,
                                                       D3DFVF_BITMAPVERTEX,
                                                       D3DPOOL_DEFAULT,
                                                       &BitmapContextP->D3DVertexBuffer,
                                                       NULL)))
    {
#ifdef _DEBUG
			String msg = _T("CreateVertexBuffer Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }


    //
    // Spinning Floppy texture
    //
    BitmapContextP = &mBitmapContext[SpinningFloppyId];
	  if (FAILED(Result = CreateTransparentBitmapTextureD3D(BitmapContextP->GDIHandle,
                                                          mD3DDevice,
                                                          &BitmapContextP->D3DTexture)))
    {
      continue;
    }
    if (FAILED(Result = mD3DDevice->CreateVertexBuffer(D3D_BITMAP_VERTEXBUFFER_SIZE,
                                                       D3DUSAGE_WRITEONLY,
                                                       D3DFVF_BITMAPVERTEX,
                                                       D3DPOOL_DEFAULT,
                                                       &BitmapContextP->D3DVertexBuffer,
                                                       NULL)))
    {
#ifdef _DEBUG
			String msg = _T("CreateVertexBuffer Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }
    if (FAILED(Result = mD3DDevice->CreateVertexBuffer(D3D_BITMAP_VERTEXBUFFER_SIZE,
                                                       D3DUSAGE_WRITEONLY,
                                                       D3DFVF_BITMAPVERTEX,
                                                       D3DPOOL_DEFAULT,
                                                       &mD3DScreenFloppyVertexBuffer,
                                                       NULL)))
    {
#ifdef _DEBUG
			String msg = _T("CreateVertexBuffer Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    //
    // Static Floppy texture
    //
    BitmapContextP = &mBitmapContext[StaticFloppyId];
	  if (FAILED(Result = CreateTransparentBitmapTextureD3D(BitmapContextP->GDIHandle,
                                                          mD3DDevice,
                                                          &BitmapContextP->D3DTexture)))
    {
      continue;
    }
    if (FAILED(Result = mD3DDevice->CreateVertexBuffer(D3D_BITMAP_VERTEXBUFFER_SIZE,
                                                       D3DUSAGE_WRITEONLY,
                                                       D3DFVF_BITMAPVERTEX,
                                                       D3DPOOL_DEFAULT,
                                                       &BitmapContextP->D3DVertexBuffer,
                                                       NULL)))
    {
#ifdef _DEBUG
			String msg = _T("CreateVertexBuffer Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }


    //
    // Onscreen Icons textures
    //
    int Loop;
    for (Loop=0; Loop < NB_OSD_BITMAPS; Loop++)
    {
      BitmapContextP = &mBitmapContext[Loop];
	    if (FAILED(Result = CreateTransparentBitmapTextureD3D(BitmapContextP->GDIHandle,
                                                            mD3DDevice,
                                                            &BitmapContextP->D3DTexture)))
      {
        break;
      }

      if (FAILED(Result = mD3DDevice->CreateVertexBuffer(D3D_BITMAP_VERTEXBUFFER_SIZE,
                                                         D3DUSAGE_WRITEONLY,
                                                         D3DFVF_BITMAPVERTEX,
                                                         D3DPOOL_DEFAULT,
                                                         &BitmapContextP->D3DVertexBuffer,
                                                         NULL)))
      {
#ifdef _DEBUG
			  String msg = _T("CreateVertexBuffer Result=") + String::IntToHex(Result,8);
			  Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
        break;
      }
    }
    if (Loop < NB_OSD_BITMAPS)
    {
      continue;
    }

    
    // Create scanline texture
    UpdateScanlinesTexture();

    mD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    mD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    mD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // Create emulator message font
	  if (FAILED(Result = D3DXCreateFont(mD3DDevice,
                                       8,
                                       0,
                                       0,
                                       0,
                                       FALSE,
                                       DEFAULT_CHARSET,
                                       OUT_DEFAULT_PRECIS,
                                       DEFAULT_QUALITY,
		                                   DEFAULT_PITCH | FF_DONTCARE,
                                       MESSAGE_FONTNAME,
                                       &mD3DEmulatorMessageFont)))
    {
#ifdef _DEBUG
			String msg = _T("D3DXCreateFont Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Create Info message font
    int InfoFontSize = (int)((float)mEmulatorScreenHeight * INFOFONT_SIZE);
	  if (FAILED(Result = D3DXCreateFont(mD3DDevice,
                                       InfoFontSize,
                                       0,
                                       0,
                                       0,
                                       FALSE,
                                       DEFAULT_CHARSET,
                                       OUT_RASTER_PRECIS,
                                       NONANTIALIASED_QUALITY,
		                                   DEFAULT_PITCH | FF_DONTCARE,
                                       INFO_FONTNAME,
                                       &mD3DInfoFont)))
    {
#ifdef _DEBUG
			String msg = _T("D3DXCreateFont Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Create Dialog font
    int DialogFontSize = (int)((float)mEmulatorScreenHeight * DIALOGFONT_SIZE);
	  if (FAILED(Result = D3DXCreateFont(mD3DDevice,
                                       DialogFontSize,
                                       0,
                                       0,
                                       0,
                                       FALSE,
                                       DEFAULT_CHARSET,
                                       OUT_RASTER_PRECIS,
                                       NONANTIALIASED_QUALITY,
		                                   DEFAULT_PITCH | FF_DONTCARE,
                                       DIALOG_FONTNAME,
                                       &mD3DDialogFont)))
    {
#ifdef _DEBUG
			String msg = _T("D3DXCreateFont Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitD3DDevice"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    // Device init OK
    InitOK = true;
  }
  while (0);

#endif /* _USE_DIRECTX */

  return InitOK;
}
//---------------------------------------------------------------------------

bool __fastcall TDrawingContext::InitGL(void)
{
bool InitOK = false;

#ifdef _USE_OPENGL

  do
  {
    //
    // Init OpenGL
    //
    // Clear error code
    glGetError();

    setupPixelFormatGL(mOnscreenHDC);

    mGLOnscreenHGLRC = wglCreateContext(mOnscreenHDC);
    if (mGLOnscreenHGLRC == NULL)
    {
#ifdef _DEBUG
      mLastError = glGetError();
			String msg = _T("wglCreateContext Error=") + String::IntToHex(mLastError,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitGL"), MB_OK);
#endif /* _DEBUG */
			continue;
		}

		wglMakeCurrent(mOnscreenHDC, mGLOnscreenHGLRC);

		mLastError = glewInit();
		if (mLastError != GLEW_OK)
		{
#ifdef _DEBUG
			String msg = _T("glewInit Error=") + String::IntToHex(mLastError,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitGL"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    if (mBitsPerPixel == 8)
    {
      if (wglCreateContextAttribsARB != NULL)
      {
        int attribList[] =
        {
          WGL_CONTEXT_MAJOR_VERSION_ARB, OPENGL_VERSION_MAJOR,
          WGL_CONTEXT_MINOR_VERSION_ARB, OPENGL_VERSION_MINOR,
          WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
          WGL_CONTEXT_FLAGS_ARB, 0,
          0
        };

        HGLRC hrcARB = wglCreateContextAttribsARB(mOnscreenHDC, 0, attribList);
        if (hrcARB != NULL)
        {
          wglMakeCurrent(NULL, NULL);
          wglDeleteContext(mGLOnscreenHGLRC);
          mGLOnscreenHGLRC = hrcARB;
          wglMakeCurrent(mOnscreenHDC, mGLOnscreenHGLRC);
        }
        else
        {
#ifdef _DEBUG
          int Error = GetLastError();
					String msg = _T("wglCreateContextAttribsARB Error=") + String::IntToHex(Error,8);
					Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitGL"), MB_OK);
#endif /* _DEBUG */
				}
      }
    }

    // Disable monitor vsync
    // Emulator is managing 50Hz video framerate. Do not need to add monitor synchro for full speed.
    wglSwapIntervalEXT(0);
    //wglSwapIntervalEXT(1); // Enable monitor VSync

    if (mBitsPerPixel == 8)
    {
      setupPaletteOpenGL();
    }

    // Do not use smooth shading
    glShadeModel(GL_FLAT);

    // Do not use vector normalization
    glDisable(GL_NORMALIZE);

    // Disable dithering
    glDisable(GL_DITHER);

    // Create and configure emulator screen texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mGLEmuScreenTexture);
    glBindTexture(GL_TEXTURE_2D, mGLEmuScreenTexture);

    // Create scanline texture
    UpdateScanlinesTexture();
    if (mLastError != GL_NO_ERROR)
    {
      continue;
    }

    // Create floppy
    glActiveTexture(GL_TEXTURE2);
    mBitmapContext[SpinningFloppyId].GLTexture =
        CreateTransparentBitmapTextureGL(mBitmapContext[SpinningFloppyId].GDIHandle);
    if (mBitmapContext[SpinningFloppyId].GLTexture == 0)
    {
      continue;
    }
    mBitmapContext[StaticFloppyId].GLTexture =
        CreateTransparentBitmapTextureGL(mBitmapContext[StaticFloppyId].GDIHandle);
    if (mBitmapContext[StaticFloppyId].GLTexture == 0)
    {
      continue;
    }

    // Create tape
    glActiveTexture(GL_TEXTURE2);
    mBitmapContext[TapeId].GLTexture =
        CreateTransparentBitmapTextureGL(mBitmapContext[TapeId].GDIHandle);
    if (mBitmapContext[TapeId].GLTexture == 0)
    {
      continue;
    }

    // Create Onscreen Icons
    glActiveTexture(GL_TEXTURE2);
    int Loop;
    for (Loop=0; Loop < NB_OSD_BITMAPS; Loop++)
    {
      mBitmapContext[Loop].GLTexture = CreateTransparentBitmapTextureGL(mBitmapContext[Loop].GDIHandle);
      if (mBitmapContext[Loop].GLTexture == 0)
      {
        break;
      }
    }
    if (Loop < NB_OSD_BITMAPS)
    {
      continue;
    }

    // Preferred format
    if (glGetInternalformativ != NULL)
    {
      glGetInternalformativ(GL_TEXTURE_2D,
                            GL_RGBA8,
                            GL_TEXTURE_IMAGE_FORMAT,
                            1,
                            &mGLPreferredFormatTexture2D);
    }

    // Check error before validating OpenGL
    mLastError = glGetError();
    if (mLastError != GL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("Error=") + String::IntToHex(mLastError,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::InitGL"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    GLBuildEmulatorMessageFontList();

    // Init OpenGL OK
    InitOK = true;

    // Wrap graphic routines to OpenGL functions
    mDrawBegin = DrawBeginGL;
    mDrawFinalize = DrawFinalizeGL;
    mDrawEmulatorScreen = DrawEmulatorScreenGL;
    if (mSettingsP->ShowStaticDriveIcon == false)
    {
      mDrawFloppy = DrawSpinningFloppyGL;
    }
    else
    {
      mDrawFloppy = DrawStaticFloppyGL;
    }
    mDrawTape = DrawTapeGL;
    mDrawEmulatorMessages = DrawEmulatorMessagesGL;
    mDrawEmulatorText = DrawEmulatorTextGL;
    mDrawOnscreenIconsBegin = DrawOnscreenIconsBeginGL;
    mDrawOnscreenIconsFinalize = DrawOnscreenIconsFinalizeGL;
    mDrawOnscreenIcon = DrawOnscreenIconGL;
  }
  while (0);

#endif /* _USE_OPENGL */

  return InitOK;
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::CreateEmulatorMessageFont()
{
  DeleteObject(mEmulatorMessageFont);

  mEmulatorMessageFont = CreateFont(
        -8,                             // Height Of Font
        0,                              // Width Of Font
        0,                              // Angle Of Escapement
        0,                              // Orientation Angle
        FW_NORMAL,                      // Font Weight
        FALSE,                          // Italic
        FALSE,                          // Underline
        FALSE,                          // Strikeout
        ANSI_CHARSET,                   // Character Set Identifier
        OUT_RASTER_PRECIS,              // Output Precision
        CLIP_DEFAULT_PRECIS,            // Clipping Precision
        ANTIALIASED_QUALITY,            // Output Quality
        FF_DONTCARE | DEFAULT_PITCH,    // Family And Pitch
        _T(MESSAGE_FONTNAME));              // Font Name

  if (mEmulatorMessageFont == NULL)
  {
#ifdef _DEBUG
		Application->MessageBox(_T("CreateFont Error"), _T("CreateEmulatorMessageFont"), MB_OK);
#endif /* _DEBUG */
  }
  else
  {
    HFONT oldFont = (HFONT)SelectObject(mOnscreenHDC, mEmulatorMessageFont);
    TEXTMETRIC TextMetrics;
    GetTextMetrics(mOnscreenHDC, &TextMetrics);
    SelectObject(mOnscreenHDC, oldFont);

    // Get font internal leading
    mEmulatorMessageFontLeading = TextMetrics.tmInternalLeading;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::CreateDialogFont()
{
  DeleteObject(mDialogFont);

  // Get font size
  int Size = (int)((float)mEmulatorScreenHeight * DIALOGFONT_SIZE);

  mDialogFont = CreateFont(
        Size,                           // Height Of Font
        0,                              // Width Of Font
        0,                              // Angle Of Escapement
        0,                              // Orientation Angle
        FW_BOLD,                        // Font Weight
        FALSE,                          // Italic
        FALSE,                          // Underline
        FALSE,                          // Strikeout
        ANSI_CHARSET,                   // Character Set Identifier
        OUT_RASTER_PRECIS,              // Output Precision
        CLIP_DEFAULT_PRECIS,            // Clipping Precision
        NONANTIALIASED_QUALITY,         // Output Quality
        FF_DONTCARE | DEFAULT_PITCH,    // Family And Pitch
        _T(DIALOG_FONTNAME));           // Font Name

  if (mDialogFont == NULL)
  {
#ifdef _DEBUG
		Application->MessageBox(_T("CreateFont Error"), _T("CreateDialogFont"), MB_OK);
#endif /* _DEBUG */
  }
  else
  {
    HFONT oldFont = (HFONT)SelectObject(mOnscreenHDC, mDialogFont);
    TEXTMETRIC TextMetrics;
    GetTextMetrics(mOnscreenHDC, &TextMetrics);
    SelectObject(mOnscreenHDC, oldFont);

    // Get font internal leading
    mDialogFontLeading = TextMetrics.tmInternalLeading;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::CreateInfoFont()
{
  DeleteObject(mInfoFont);

  // Get font size
  int Size = (int)((float)mEmulatorScreenHeight * INFOFONT_SIZE);

  mInfoFont = CreateFont(
        Size,                           // Height Of Font
        0,                              // Width Of Font
        0,                              // Angle Of Escapement
        0,                              // Orientation Angle
        FW_NORMAL,                      // Font Weight
        FALSE,                          // Italic
        FALSE,                          // Underline
        FALSE,                          // Strikeout
        ANSI_CHARSET,                   // Character Set Identifier
        OUT_RASTER_PRECIS,              // Output Precision
        CLIP_DEFAULT_PRECIS,            // Clipping Precision
        NONANTIALIASED_QUALITY,         // Output Quality
        FF_DONTCARE | DEFAULT_PITCH,    // Family And Pitch
        _T(INFO_FONTNAME));             // Font Name

  if (mInfoFont == NULL)
  {
#ifdef _DEBUG
		Application->MessageBox(_T("CreateFont Error"), _T("CreateInfoFont"), MB_OK);
#endif /* _DEBUG */
	}
	else
	{
		HFONT oldFont = (HFONT)SelectObject(mOnscreenHDC, mInfoFont);
		TEXTMETRIC TextMetrics;
		GetTextMetrics(mOnscreenHDC, &TextMetrics);
		SelectObject(mOnscreenHDC, oldFont);

		// Get font internal leading
		mInfoFontLeading = TextMetrics.tmInternalLeading;
	}
}
//---------------------------------------------------------------------------


#ifdef _USE_OPENGL
GLuint __fastcall TDrawingContext::GLBuildFontList(HFONT font)
{
GLuint FontBase;

  if (mOnscreenHDC == NULL)
    return 0;

	HFONT oldfont = (HFONT)SelectObject(mOnscreenHDC, font);

	do
	{
		FontBase = glGenLists(96);
		if (FontBase == 0)
		{
#ifdef _DEBUG
			Application->MessageBox(_T("glGenLists Error"), _T("BuildFont"), MB_OK);
#endif /* _DEBUG */
			continue;
    }

    glGetError();
    // Builds 96 characters starting at character 32
    if (wglUseFontBitmaps(mOnscreenHDC,
                          32,
                          96,
                          FontBase) == FALSE)
    {
      glDeleteLists(FontBase, 96);
      FontBase = 0;
#ifdef _DEBUG
      GLenum GLError = glGetError();
			String msg = _T("wglUseFontBitmaps Error = ") + String::IntToHex(GLError,8);
			Application->MessageBox(msg.c_str(), _T("BuildFont"), MB_OK);
#endif /* _DEBUG */
		}
  }
  while (0);

  SelectObject(mOnscreenHDC, oldfont);

  return (FontBase);
}
//---------------------------------------------------------------------------
#endif /* _USE_OPENGL */


#ifdef _USE_OPENGL
void __fastcall TDrawingContext::GLBuildEmulatorMessageFontList(void)
{
  mGLEmulatorMessageFontBase = GLBuildFontList(mEmulatorMessageFont);
}
//---------------------------------------------------------------------------
#endif /* _USE_OPENGL */


int __fastcall TDrawingContext::GetLastError(void)
{
int Error = mLastError;

#ifdef _USE_OPENGL
  if (mUseOpenGL == true)
  {
    Error = glGetError();
  }
#endif /* _USE_OPENGL */

  mLastError = ERROR_SUCCESS;

  return (Error);
}
//---------------------------------------------------------------------------


float __fastcall TDrawingContext::GetZoomFactor(void)
{
  if (mUseDirectX == true)
  {
    return ZOOM_FACTOR_D3D;
  }
  else if (mUseOpenGL == true)
  {
    return ZOOM_FACTOR_GL;
  }

  return ZOOM_FACTOR_GDI;
}
//---------------------------------------------------------------------------


SIZE __fastcall TDrawingContext::GetEmulatorMessageExtent(String Message)
{
SIZE extent;

	HFONT oldFont = (HFONT)SelectObject(mHostCanvas->Handle, mEmulatorMessageFont);
	GetTextExtentPoint32(mHostCanvas->Handle, Message.c_str(), Message.Length(), &extent);
	SelectObject(mHostCanvas->Handle, oldFont);

	return extent;
}
//---------------------------------------------------------------------------
SIZE __fastcall TDrawingContext::GetDialogExtent(String Message)
{
SIZE extent;

	HFONT oldFont = (HFONT)SelectObject(mHostCanvas->Handle, mDialogFont);
	GetTextExtentPoint32(mHostCanvas->Handle, Message.c_str(), Message.Length(), &extent);
	SelectObject(mHostCanvas->Handle, oldFont);

	return extent;
}
//---------------------------------------------------------------------------
SIZE __fastcall TDrawingContext::GetInfoExtent(String Message)
{
SIZE extent;

	HFONT oldFont = (HFONT)SelectObject(mHostCanvas->Handle, mInfoFont);
	GetTextExtentPoint32(mHostCanvas->Handle, Message.c_str(), Message.Length(), &extent);
	SelectObject(mHostCanvas->Handle, oldFont);

  return extent;
}
//---------------------------------------------------------------------------
int __fastcall TDrawingContext::GetMonitorVRefresh(void)
{
  HDC hDC = GetDC(NULL);
  int VRefresh = GetDeviceCaps(hDC, VREFRESH);
  ReleaseDC(NULL, hDC);

  return (VRefresh);
}
//---------------------------------------------------------------------------


bool __fastcall TDrawingContext::CreateEmulatorOffscreenBitmap()
{
VOID* bits;
bool BitmapOK = false;

  if (mEngineGraphicFormatP == NULL)
    return false;

  int sWidth = mEngineGraphicFormatP->ScreenWidth;
  int sHeight = mEngineGraphicFormatP->ScreenHeight;

  //
  // Create BITMAPINFO structure
  //
  mEmuScreenBITMAPINFO = (BITMAPINFO*)new unsigned char[sizeof(tEmulatorBitmapInfo)];
  memset(mEmuScreenBITMAPINFO, 0, sizeof(tEmulatorBitmapInfo));
	mEmuScreenBITMAPINFO->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  mEmuScreenBITMAPINFO->bmiHeader.biWidth = sWidth;
  mEmuScreenBITMAPINFO->bmiHeader.biHeight = -sHeight;
  mEmuScreenBITMAPINFO->bmiHeader.biPlanes = 1;
  mEmuScreenBITMAPINFO->bmiHeader.biBitCount = mBitsPerPixel;
  mEmuScreenBITMAPINFO->bmiHeader.biCompression = BI_RGB;
  mEmuScreenBITMAPINFO->bmiHeader.biClrUsed = NATIVECPC_COLOURS_NBELEMENT;
  mEmuScreenHDC = CreateCompatibleDC(NULL);
  mEmuScreenHBITMAP = CreateDIBSection(mEmuScreenHDC,
                                       (BITMAPINFO*)mEmuScreenBITMAPINFO,
                                       DIB_PAL_COLORS,
                                       &bits,
                                       NULL,
                                       0);
  if (mEmuScreenHBITMAP != NULL)
  {
    GetObject(mEmuScreenHBITMAP, sizeof(BITMAP), &mEmuScreenBITMAP);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(mEmuScreenHDC, mEmuScreenHBITMAP);

    // Get offscreen bitmap origin
    mEmuScreenDataP = (void*)((unsigned char*)mEmuScreenBITMAP.bmBits
                      + (mEmuScreenBITMAP.bmWidthBytes * mEngineGraphicFormatP->OffscreenVisibleY));

    SelectObject(mEmuScreenHDC, oldBitmap);

    BitmapOK = true;
  }
  else
  {
#ifdef _DEBUG
		Application->MessageBox(_T("CreateDIBSection mEmuScreenHBITMAP NULL"), _T("CreateEmulatorOffscreenBitmap"), MB_OK);
#endif /* _DEBUG */

    mEmuScreenDataP = NULL;
  }

  return (BitmapOK);
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::CreateScanlineTextureGDI(int ScanlineMask)
{
#ifdef USE_GDI_SCANLINES

RGBQUAD palette[2];

  if (mEngineGraphicFormatP == NULL) return;

  // this will give you white (1) and black (0)
  palette[0].rgbBlue  = 255;
  palette[0].rgbGreen = 255;
  palette[0].rgbRed   = 255;
  palette[1].rgbBlue  = (tUChar)ScanlineMask;
  palette[1].rgbGreen = (tUChar)ScanlineMask;
  palette[1].rgbRed   = (tUChar)ScanlineMask;

  HDC memory=CreateCompatibleDC(NULL);

  int ImageWidth = mEngineGraphicFormatP->VisibleScreenWidth;
  int ImageHeight = mEngineGraphicFormatP->VisibleScreenHeight * 2;

  // Create scanlines mask
  delete [] mGDIScanlinesMaskBits;
  mGDIScanlinesMaskBits = new BYTE [ImageWidth * ImageHeight];
  memset(mGDIScanlinesMaskBits, 0, ImageWidth * ImageHeight);
  unsigned char * dataP = mGDIScanlinesMaskBits;
  for (int row=0; row < ImageHeight; row+=2, dataP+=(ImageWidth/8))
  {
    for (int col=0; col < (ImageWidth/8); col++)
    {
      *(dataP++) = 0xff;
    }
  }

  // using a PBITMAPINFO in order to allocate room for palette
  int BitmapInfoSize = sizeof(BITMAPINFO)+sizeof(RGBQUAD)*2;
  mGDIScanlinesMaskBITMAPINFO = (PBITMAPINFO)new char[BitmapInfoSize];
  memset(mGDIScanlinesMaskBITMAPINFO, 0, BitmapInfoSize);

  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biWidth=ImageWidth;
  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biHeight=ImageHeight;
  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biPlanes=1;
  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biBitCount=1;
  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biCompression=BI_RGB;
  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biClrUsed=2; // palette is two colors long
  mGDIScanlinesMaskBITMAPINFO->bmiHeader.biClrImportant=2;
  memcpy(mGDIScanlinesMaskBITMAPINFO->bmiColors,palette,sizeof(RGBQUAD)*2);

  mGDIScanlinesMaskHBITMAP = CreateDIBitmap(memory,
                                            &mGDIScanlinesMaskBITMAPINFO->bmiHeader,
                                            CBM_INIT,
                                            mGDIScanlinesMaskBits,
                                            mGDIScanlinesMaskBITMAPINFO,
                                            DIB_RGB_COLORS);

  DeleteDC(memory);

#else /*USE_GDI_SCANLINES */

  NOT_USED(ScanlineMask);

#endif /* USE_GDI_SCANLINES */
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::setupPixelFormatGL(HDC hDC)
{
  const PIXELFORMATDESCRIPTOR pfd =
  {
    sizeof(PIXELFORMATDESCRIPTOR), // nSize
    1, // nVersion
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // dwFlags
    PFD_TYPE_RGBA, // iPixelType
    32, // cColorBits
    0,0,0,0,0,0, // cRedBits, cRedShift, cGreenBits, cGreenShift, cBlueBits, cBlueShift
    0,0, // cAlphaBits, cAlphaShift
    0,0,0,0,0, // cAccumBits, cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits
    0, // cDepthBits
    0, // cStencilBits
    0, // cAuxBuffers
    PFD_MAIN_PLANE, // iLayerType
    0, // bReserved
    0,0,0 // dwLayerMask, dwVisibleMask, dwDamageMask
  };

  int PixelFormat = ChoosePixelFormat(hDC, &pfd);
  if (SetPixelFormat(hDC, PixelFormat, &pfd) == FALSE)
  {
    mLastError = ::GetLastError();
  }
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::UpdateScanlinesTexture(void)
{
  const unsigned char * scanline_bitmap = NULL;
  int width = 0;
  int height = 0;

  mDrawScanlines = mSettingsP->DrawScanlines;

  if (LoadScanlinesPattern() == true)
  {
    scanline_bitmap = mScanlinesData;
    width = mScanlinesWidth;
    height = mScanlinesHeight;
  }

  if (scanline_bitmap == NULL)
  {
    mDrawScanlines = false;
    return;
  }

#ifdef _USE_DIRECTX

  if (mUseDirectX == true)
  {
    HRESULT Result;

    if (mD3DScanlinesTexture != NULL)
    {
      mD3DScanlinesTexture->Release();
    }
	  if (FAILED(Result = D3DXCreateTexture(mD3DDevice,
		                                      width,
		                                      height,
                                          1,
                                          D3DUSAGE_DYNAMIC,
                                          D3DFMT_A8B8G8R8,
                                          D3DPOOL_DEFAULT,
                                          &mD3DScanlinesTexture)))
    {
#ifdef _DEBUG
			String msg = _T("D3DXCreateTexture Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::UpdateScanlinesTexture"), MB_OK);
#else /* _DEBUG */
      NOT_USED(Result);
#endif /* _DEBUG */
    }

    D3DLOCKED_RECT lockedRect = {0}; // "out" parameter from LockRect function below
    if (FAILED(mD3DScanlinesTexture->LockRect(0, &lockedRect, NULL, 0)))
    {
#ifdef _DEBUG
			String msg = _T("LockRect Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::UpdateScanlinesTexture"), MB_OK);
#endif /* _DEBUG */
    }

    // Copy scanlines pattern (take care of pitch!!)
    tULong* BitmapPix = (tULong*)scanline_bitmap;
    for (int y = 0; y < height; y++)
    {
        tULong* CurRow = (tULong*)((tUChar*)lockedRect.pBits + (y * lockedRect.Pitch));
        int x = width;
        while(x--)
        {
          *CurRow++ = *(BitmapPix++);
        }
    }

    // unlock when done
    mD3DScanlinesTexture->UnlockRect(0);

    mD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	  mD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    mD3DDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		mD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,       D3DTOP_MODULATE);
	  mD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1,     D3DTA_TEXTURE);
	  mD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2,     D3DTA_CURRENT);
  }
  else

#endif /* _USE_DIRECTX */

#ifdef _USE_OPENGL

  if (mUseOpenGL == true)
  {
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE1);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 (GLsizei)width,
                 (GLsizei)height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 scanline_bitmap);
    mLastError = glGetError();
    if (mLastError != GL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("glTexImage2D Error=") + String::IntToHex(mLastError,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::UpdateScanlinesTexture"), MB_OK);
#endif /* _DEBUG */
    }

    glDisable(GL_TEXTURE_2D);
  }

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------


bool __fastcall TDrawingContext::LoadScanlinesPattern(void)
{
  int MonitorWidth = mDisplayWidth;
  int MonitorHeight = mDisplayHeight;

  // Open pattern file
  bool FileExist = false;
  AnsiString PatternFilename = mSettingsP->ScanlinesDirectory
                               + AnsiString(MonitorWidth) + "x"
                               + AnsiString(MonitorHeight) + PATTERN_EXT;
  if (FileExists(PatternFilename) == true)
  {
    TStringList* FileContent = new TStringList();
    FileContent->LoadFromFile(PatternFilename);

    FileExist = true;

    // Free current scanlines pattern
    mScanlinesWidth = 0;
    mScanlinesHeight = 0;
    delete [] mScanlinesData;
    mScanlinesData = NULL;
    mScanlinesNbAlias = 0;
    delete [] mScanlinesAlias;
    mScanlinesAlias = NULL;

    // Parse all pattern lines
    for (int Loop=0; Loop < FileContent->Count; Loop++)
    {
      AnsiString Line = FileContent->Strings[Loop].LowerCase().Trim();
      int ParamPos = Line.Pos("=");
      int CommentPos = Line.Pos(";");
      int ParamLength = Line.Length() - ParamPos;
      if (CommentPos > ParamPos)
      {
        ParamLength = CommentPos - ParamPos - 1;
      }
      AnsiString Param = Line.SubString(ParamPos+1, ParamLength).Trim();

      // Pattern width
      if (Line.Pos(PATTERN_WIDTH) == 1)
      {
        try
        {
          mScanlinesWidth = Param.ToInt();
          if (mScanlinesWidth > 256) mScanlinesWidth = 256;
        }
        catch (...)
        {
          mScanlinesWidth = 0;
        }
      }
      // Pattern height
      else if (Line.Pos(PATTERN_HEIGHT) == 1)
      {
        try
        {
          mScanlinesHeight = Param.ToInt();
          if (mScanlinesHeight > 256) mScanlinesHeight = 256;
        }
        catch (...)
        {
          mScanlinesHeight = 0;
        }
      }
      // Pattern nb values
      else if (Line.Pos(PATTERN_NBALIAS) == 1)
      {
        try
        {
          mScanlinesNbAlias = Param.ToInt();
          if (mScanlinesNbAlias > PATTERN_MAX_ALIAS) mScanlinesNbAlias = PATTERN_MAX_ALIAS;
        }
        catch (...)
        {
          mScanlinesNbAlias = 0;
        }

        delete [] mScanlinesAlias;
        mScanlinesAlias = NULL;

        if (mScanlinesNbAlias)
        {
          mScanlinesAlias = new tUChar[mScanlinesNbAlias];
          memset(mScanlinesAlias, 255, mScanlinesNbAlias); // No mask by default
        }
      }
      // Pattern value
      else if (Line.Pos(PATTERN_ALIAS) == 1)
      {
        do
        {
          int StartCoordPos = Line.Pos("[");
          int EndCoordPos = Line.Pos("]");
          AnsiString IndexString = Line.SubString(StartCoordPos+1, EndCoordPos-StartCoordPos-1).Trim();

          int Index;
          // Get Index
          try
          {
            Index = IndexString.SubString(1, EndCoordPos-1).Trim().ToInt();
          }
          catch (...) { continue; }

          // Invalid index ?
          if (Index >= mScanlinesNbAlias)
            continue;

          try
          {
            mScanlinesAlias[Index] = (tUChar)Param.ToInt();
          }
          catch (...)
          {
            mScanlinesAlias[Index] = 255;
          }
        }
        while (0);
      }
      // Pattern pixel
      else if (Line.Pos(PATTERN_PIXEL) == 1)
      {
        // Allocate pattern
        if ( (mScanlinesData == NULL)
             && (mScanlinesWidth != 0)
             && (mScanlinesHeight != 0) )
        {
          int PatternSize = mScanlinesWidth * mScanlinesHeight * 4; // Width * Height * RGBA
          mScanlinesData = new tUChar[PatternSize];
          memset(mScanlinesData, 255, PatternSize); // No mask by default
        }

        do
        {
          int StartCoordPos = Line.Pos("[");
          int EndCoordPos = Line.Pos("]");
          AnsiString Coordinates = Line.SubString(StartCoordPos+1, EndCoordPos-StartCoordPos-1).Trim();
          int SeparatorPos = Coordinates.Pos(",");

          int X,Y,R,G,B,A;
          // Get X
          try
          {
            X = Coordinates.SubString(1, SeparatorPos-1).Trim().ToInt();
          }
          catch (...) { continue; }
          // Get Y
          try
          {
            Y = Coordinates.SubString(SeparatorPos+1, Coordinates.Length()).Trim().ToInt();
          }
          catch (...) { continue; }

          // Invalid coordinates ?
          if ( (X >= mScanlinesWidth) || (Y >= mScanlinesHeight) )
            continue;

          // Get R
          SeparatorPos = Param.Pos(",");
          if (ReadScanlinesRGBValue(Param.SubString(1, SeparatorPos-1), &R) == false)
            continue;

          // Get G
          Param = Param.SubString(SeparatorPos+1, Param.Length());
          SeparatorPos = Param.Pos(",");
          if (ReadScanlinesRGBValue(Param.SubString(1, SeparatorPos-1), &G) == false)
            continue;

          // Get B
          Param = Param.SubString(SeparatorPos+1, Param.Length());
          SeparatorPos = Param.Pos(",");
          // No Alpha ?
          if (SeparatorPos == 0)
          {
            A = 255;
            if (ReadScanlinesRGBValue(Param, &B) == false)
              continue;
          }
          else
          {
            if (ReadScanlinesRGBValue(Param.SubString(1, SeparatorPos-1), &B) == false)
              continue;

            // Get A
            if (ReadScanlinesRGBValue(Param.SubString(SeparatorPos+1, Param.Length()), &A) == false)
              continue;
          }

          // Set pattern
          tUChar* PatternP = mScanlinesData + (Y * mScanlinesWidth * 4) + (X * 4);
          *(PatternP++) = (tUChar)R;
          *(PatternP++) = (tUChar)G;
          *(PatternP++) = (tUChar)B;
          *PatternP = (tUChar)A;
        }
        while (0);
      }
    }

    delete FileContent;
  }

  return FileExist;
}
//---------------------------------------------------------------------------
bool __fastcall TDrawingContext::ReadScanlinesRGBValue(AnsiString Value, int* ValueP)
{
  int StartValuePos = Value.Pos("(");
  int EndValuePos = Value.Pos(")");

  if ( (StartValuePos) && (EndValuePos) )
  {
    // Get Index
    int Index;
    try
    {
      Index = Value.SubString(StartValuePos+1, EndValuePos-StartValuePos-1).Trim().ToInt();
      if (Index >= mScanlinesNbAlias)
        return false;
        
      *ValueP = mScanlinesAlias[Index];
    }
    catch (...)
    {
      return false;
    }
  }
  else
  {
    try
    {
      *ValueP = Value.Trim().ToInt();
    }
    catch (...)
    {
      return false;
    }
  }

  return true;
}
//---------------------------------------------------------------------------


double __fastcall TDrawingContext::GetEmulatorAspectRatio(void)
{
  return ( (double)mEngineGraphicFormatP->VisibleScreenWidth
           / (double)mEngineGraphicFormatP->VisibleScreenHeight );
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::SetFullscreen(bool Value)
{
  mFullscreen = Value;
  UpdateScanlinesTexture();
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::SetScanlinesPattern(int Value)
{
  mScanlinesPattern = Value;
  UpdateScanlinesTexture();
}
//---------------------------------------------------------------------------


HBITMAP __fastcall TDrawingContext::LoadResourceBitmap(String ResourceBitmapName)
{
  // Load bitmap from resources
  HBITMAP hBitmap = (HBITMAP)LoadImage(HInstance,
                                       ResourceBitmapName.c_str(),
                                       IMAGE_BITMAP,
                                       0,
                                       0,
                                       LR_CREATEDIBSECTION);

  return hBitmap;
}
//---------------------------------------------------------------------------


HBITMAP __fastcall TDrawingContext::CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
HDC hdcMem, hdcMem2;
HBITMAP hbmMask;
BITMAP bm;

  // Create monochrome (1 bit) mask bitmap.

  GetObject(hbmColour, sizeof(bm), &bm);
  hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
  if (hbmMask == NULL)
    return NULL;

  // Get some HDCs that are compatible with the display driver

  hdcMem = CreateCompatibleDC(NULL);
  hdcMem2 = CreateCompatibleDC(NULL);

  SelectObject(hdcMem, hbmColour);
  SelectObject(hdcMem2, hbmMask);

  // Set the background colour of the colour image to the colour
  // you want to be transparent.
  SetBkColor(hdcMem, crTransparent);

  // Copy the bits from the colour image to the B+W mask... everything
  // with the background colour ends up white while everythig else ends up
  // black...Just what we wanted.
  if (BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY) == 0)
  {
    mLastError = ::GetLastError();
  }

  // Take our new mask and use it to turn the transparent colour in our
  // original colour image to black so the transparency effect will
  // work right.
  if (BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT) == 0)
  {
    mLastError = ::GetLastError();
  }

  // Clean up.
  DeleteDC(hdcMem);
  DeleteDC(hdcMem2);

  return hbmMask;
}
//---------------------------------------------------------------------------


unsigned int __fastcall TDrawingContext::CreateBitmapTextureGL(HBITMAP hBitmap)
{
  unsigned int Texture = 0;

#ifdef _USE_OPENGL

  // Get bitmap size
  BITMAP Bitmap;
  GetObject(hBitmap, sizeof(Bitmap), &Bitmap);

  // Create texture
  glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA,
               Bitmap.bmWidth,
               Bitmap.bmHeight,
               0,
               GL_BGR,
               GL_UNSIGNED_BYTE,
               Bitmap.bmBits);

#endif /* _USE_OPENGL */

  return Texture;
}
//---------------------------------------------------------------------------


LPBITMAP __fastcall TDrawingContext::CreateTransparentBitmap(HBITMAP hBitmap)
{
  // this structure is an equivalent of BITMAPINFO except it contains
  // a full-sized color table
  struct
  {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[256];
  }bmi;
  memset(&bmi, 0, sizeof(bmi));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

  // Get bitmap info
  GetDIBits(mOnscreenHDC, hBitmap, 0, 1, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
  if (bmi.bmiHeader.biBitCount <= 8)
  {
    // Get color table
    GetDIBits(mOnscreenHDC, hBitmap, 0, 1, NULL, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
  }

  BITMAP Bitmap;
  GetObject(hBitmap, sizeof(Bitmap), &Bitmap);
  tUChar* pixel = (tUChar*)Bitmap.bmBits;

  // First pixel is transparent
  RGBQUAD TransparentColor;
 //roger casca !
 // = bmi.bmiColors[*pixel];

  // Create transparent bitmap
  int NewBitmapSize = sizeof(BITMAP);
  NewBitmapSize += bmi.bmiHeader.biWidth * bmi.bmiHeader.biHeight * 4L;
  LPBITMAP TransparentBitmap = (LPBITMAP)malloc(NewBitmapSize);
  memset(TransparentBitmap, 0, NewBitmapSize);

  // Populate info
  TransparentBitmap->bmWidth  = bmi.bmiHeader.biWidth;
  TransparentBitmap->bmHeight = bmi.bmiHeader.biHeight;
  TransparentBitmap->bmBitsPixel = 32;
  TransparentBitmap->bmBits = &TransparentBitmap->bmBits + 1;

  //
  // Convert Bitmap to RGBA
  //
  tUChar* current_bits = (tUChar*)TransparentBitmap->bmBits;
  int w = bmi.bmiHeader.biWidth;
  int h = bmi.bmiHeader.biHeight;
  int padding;
  switch(bmi.bmiHeader.biBitCount)
  {
    // 8 bits bitmap with color table
    case 8:
    {
      padding=w % 2;
      while (h--)
      {
        w=bmi.bmiHeader.biWidth;
        while (w--)
        {
          RGBQUAD rgba=bmi.bmiColors[*pixel++];
          *current_bits++=rgba.rgbRed & 0xff;
          *current_bits++=rgba.rgbGreen & 0xff;
          *current_bits++=rgba.rgbBlue & 0xff;
          *current_bits++=255; // Alpha=255 => Opaque
        }
        pixel+=padding;
      }
    }
    break;

    // 24 bits bitmap
    case 24:
    {
      padding=(w*3) % 2;
      while (h--)
      {
        w=bmi.bmiHeader.biWidth;
        while (w--)
        {
          *current_bits++=pixel[2];
          *current_bits++=pixel[1];
          *current_bits++=pixel[0];
          *current_bits++=255; // Alpha=255 => Opaque
          pixel+=3;
        }
        pixel+=padding;
      }
    }
    break;

    // 32 bits bitmap
    case 32:
    {
      while (h--)
      {
        w=bmi.bmiHeader.biWidth;
        while (w--)
        {
          *current_bits++=pixel[2];
          *current_bits++=pixel[1];
          *current_bits++=pixel[0];
          *current_bits++=pixel[3];
          pixel+=4;
        }
      }
    }
    break;

    default:
    {
      free(TransparentBitmap);
      TransparentBitmap = NULL;
    }
  }

  if (TransparentBitmap != NULL)
  {
    // now make every transparent pixel transparent
    tULong Color =   ((0xFF) << 24)
                   | (TransparentColor.rgbBlue << 16)
                   | (TransparentColor.rgbGreen << 8)
                   | (TransparentColor.rgbRed << 0);
    tULong* pixels = (tULong*)TransparentBitmap->bmBits;
    int size = TransparentBitmap->bmWidth * TransparentBitmap->bmHeight;
    while (size--)
    {
      if (*pixels == Color)
      {
        *pixels &= 0x00ffffff; // Alpha=0 => Transparent
      }
      pixels++;
    }
  }

  return TransparentBitmap;
}
//---------------------------------------------------------------------------


unsigned int __fastcall TDrawingContext::CreateTransparentBitmapTextureGL(HBITMAP hBitmap)
{
  unsigned int Texture = 0;

#ifdef _USE_OPENGL

  LPBITMAP BitmapP = CreateTransparentBitmap(hBitmap);
  if (BitmapP != NULL)
  {
    // Clear error code
    glGetError();

    // Create texture
    glGenTextures(1, &Texture);
	  glBindTexture(GL_TEXTURE_2D, Texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 BitmapP->bmWidth,
                 BitmapP->bmHeight,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 BitmapP->bmBits);
    mLastError = glGetError();
    if (mLastError != GL_NO_ERROR)
    {
      glDeleteTextures(1, &Texture);
      Texture = 0;
#ifdef _DEBUG
			String msg = _T("glTexImage2D Error=") + String::IntToHex(mLastError,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::CreateTransparentBitmapTextureGL"), MB_OK);
#endif /* _DEBUG */
    }

    free(BitmapP);
  }

#endif /* _USE_OPENGL */

  return Texture;
}
//---------------------------------------------------------------------------


#ifdef _USE_DIRECTX
HRESULT __fastcall TDrawingContext::CreateTransparentBitmapTextureD3D(HBITMAP hBitmap,
                                                                      LPDIRECT3DDEVICE9 DeviceP,
                                                                      LPDIRECT3DTEXTURE9 * TexturePP)
{
  HRESULT Result;

  do
  {
    LPBITMAP BitmapP = CreateTransparentBitmap(hBitmap);
    if (BitmapP == NULL)
      continue;

	  if (FAILED(Result = D3DXCreateTexture(DeviceP,
		                                      BitmapP->bmWidth,
		                                      BitmapP->bmHeight,
                                          1,
                                          D3DUSAGE_DYNAMIC,
                                          D3DFMT_A8R8G8B8,
                                          D3DPOOL_DEFAULT,
                                          TexturePP)))
    {
#ifdef _DEBUG
			String msg = _T("D3DXCreateTexture Result=") + String::IntToHex(Result,8);
			Application->MessageBox(msg.c_str(), _T("TDrawingContext::CreateTransparentBitmapTextureD3D"), MB_OK);
#endif /* _DEBUG */
      free(BitmapP);
      continue;
    }

    // Lock texture memory
    D3DLOCKED_RECT lockedRect = {0}; // "out" parameter from LockRect function below
    if (FAILED((*TexturePP)->LockRect(0, &lockedRect, NULL, 0)))
    {
      free(BitmapP);
      continue;
    }

    // Copy bitmap (take care of pitch!!)
    tULong* BitmapPix = (tULong*)BitmapP->bmBits;
    for (int y = 0; y < BitmapP->bmHeight; y++)
    {
        tULong* CurRow = (tULong*)((tUChar*)lockedRect.pBits + (y * lockedRect.Pitch));
        int x = BitmapP->bmWidth;
        while(x--)
        {
          //*CurRow++ = *(BitmapPix++);

          //Convert ABGR to ARGB
          tULong Pixel = *(BitmapPix) & 0xff00ff00;
          Pixel |= (*(BitmapPix) & 0xff) << 16;
          Pixel |= (*(BitmapPix) >> 16) & 0xff;
          *CurRow++ = Pixel;
          BitmapPix++;
        }
    }

    // unlock when done
    (*TexturePP)->UnlockRect(0);

    free(BitmapP);
  }
  while (0);

  return Result;
}
//---------------------------------------------------------------------------
#endif /* _USE_DIRECTX */


void __fastcall TDrawingContext::SetupPalette(void)
{
  if (mUseDirectX == true)
  {
  }
  else if (mUseOpenGL == true)
  {
    setupPaletteOpenGL();
  }
  else
  {
    setupPaletteGDI();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::setupPaletteOpenGL(void)
{
#ifdef _USE_OPENGL
  GLfloat  ColorTableR[NATIVECPC_COLOURS_NBELEMENT];
  GLfloat  ColorTableG[NATIVECPC_COLOURS_NBELEMENT];
  GLfloat  ColorTableB[NATIVECPC_COLOURS_NBELEMENT];
  for (int i=0;
           i < NATIVECPC_COLOURS_NBELEMENT;
           i++)
  {
    ColorTableR[i] = (GLfloat)GetRValue(mPalette[i]) / 255.0f;
    ColorTableG[i] = (GLfloat)GetGValue(mPalette[i]) / 255.0f;
    ColorTableB[i] = (GLfloat)GetBValue(mPalette[i]) / 255.0f;
  }

  glPixelMapfv(GL_PIXEL_MAP_I_TO_R,
               NATIVECPC_COLOURS_NBELEMENT,
               ColorTableR);
  mLastError = glGetError();
  if (mLastError != GL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("glPixelMapuiv I_TO_R Error=") + String::IntToHex(mLastError,8);
		Application->MessageBox(msg.c_str(), _T("setupPaletteOpenGL"), MB_OK);
#endif /* _DEBUG */
  }
  glPixelMapfv(GL_PIXEL_MAP_I_TO_G,
               NATIVECPC_COLOURS_NBELEMENT,
               ColorTableG);
  mLastError = glGetError();
  if (mLastError != GL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("glPixelMapuiv I_TO_G Error=") + String::IntToHex(mLastError,8);
		Application->MessageBox(msg.c_str(), _T("setupPaletteOpenGL"), MB_OK);
#endif /* _DEBUG */
  }
  glPixelMapfv(GL_PIXEL_MAP_I_TO_B,
               NATIVECPC_COLOURS_NBELEMENT,
               ColorTableB);
  mLastError = glGetError();
  if (mLastError != GL_NO_ERROR)
  {
#ifdef _DEBUG
		String msg = _T("glPixelMapuiv I_TO_B Error=") + String::IntToHex(mLastError,8);
		Application->MessageBox(msg.c_str(), _T("setupPaletteOpenGL"), MB_OK);
#endif /* _DEBUG */
  }

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::setupPaletteGDI(void)
{
  if (mEmuScreenBITMAPINFO == NULL)
    return;

  RGBQUAD* pRGB = mEmuScreenBITMAPINFO->bmiColors;

  /* build a simple RGB color palette */
  for (int i=0; i<NATIVECPC_COLOURS_NBELEMENT; ++i)
  {
    pRGB[i].rgbRed = GetRValue(mPalette[i]);
    pRGB[i].rgbGreen = GetGValue(mPalette[i]);
    pRGB[i].rgbBlue = GetBValue(mPalette[i]);
    pRGB[i].rgbReserved = 0;
  }

  if (SetDIBColorTable(mEmuScreenHDC,
                   0,
                   NATIVECPC_COLOURS_NBELEMENT,
                   pRGB) == 0)
  {
#ifdef _DEBUG
		String msg = _T("SetDIBColorTable Error=") + String::IntToHex(GetLastError(),8);
		Application->MessageBox(msg.c_str(), _T("setupPaletteGDI"), MB_OK);
#endif /* _DEBUG */
  }
}
//---------------------------------------------------------------------------


HANDLE __fastcall TDrawingContext::LoadResourceFont(String FontName)
{
HANDLE Handle = 0;

  do
  {
    HRSRC Resource = FindResource(HInstance, FontName.c_str(), RT_RCDATA);
    if (Resource == NULL)
    {
      continue;
    }

    DWORD Length = SizeofResource(HInstance, Resource);
    HGLOBAL Address = LoadResource(HInstance, Resource);
    if (Address == NULL)
    {
      continue;
    }

    PVOID FontData = LockResource(Address);

    DWORD Installed;
    Handle = AddFontMemResourceEx(FontData, Length, 0, &Installed);
    if (Installed == 0)
    {
      Handle = 0;
    }

    UnlockResource(FontData);
    FreeResource(Address);
  }
  while(0);

  return Handle;
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::FreeResourceFont(HANDLE FontHandle)
{
  RemoveFontMemResourceEx(FontHandle);
}
//---------------------------------------------------------------------------


bool __fastcall TDrawingContext::IncreaseBrightness(void)
{
bool Ret = false;

#ifdef _USE_DIRECTX

  if (mUseDirectX == true)
  {
    if (mD3DFullscreenBrightness < BRIGHTNESS_MAXVALUE)
    {
      mD3DFullscreenBrightness += BRIGHTNESS_STEP;
      mSettingsP->FullscreenBrightness = mD3DFullscreenBrightness;
      Ret = true;
    }
  }
  else

#endif /* _USE_DIRECTX */

#ifdef _USE_OPENGL

  if (mUseOpenGL == true)
  {
    if (mGLFullscreenBrightness < BRIGHTNESS_MAXVALUE)
    {
      mGLFullscreenBrightness += BRIGHTNESS_STEP;
      mSettingsP->FullscreenBrightness = (int)mGLFullscreenBrightness;
      Ret = true;
    }
  }

#endif /* _USE_OPENGL */

  return Ret;
}
//---------------------------------------------------------------------------
bool __fastcall TDrawingContext::DecreaseBrightness(void)
{
bool Ret = false;

#ifdef _USE_DIRECTX

  if (mUseDirectX == true)
  {
    if (mD3DFullscreenBrightness > BRIGHTNESS_MINVALUE)
    {
      mD3DFullscreenBrightness -= BRIGHTNESS_STEP;
      mSettingsP->FullscreenBrightness = mD3DFullscreenBrightness;
      Ret = true;
    }
  }
  else

#endif /* _USE_DIRECTX */

#ifdef _USE_OPENGL

  if (mUseOpenGL == true)
  {
    if (mGLFullscreenBrightness > BRIGHTNESS_MINVALUE)
    {
      mGLFullscreenBrightness -= BRIGHTNESS_STEP;
      mSettingsP->FullscreenBrightness = (int)mGLFullscreenBrightness;
      Ret = true;
    }
  }

#endif /* _USE_OPENGL */

  return Ret;
}
//---------------------------------------------------------------------------


unsigned char* __fastcall TDrawingContext::CreateScreenshot(bool Mode2, int* SizeP)
{
  if (mEngineGraphicFormatP == NULL)
    return NULL;

  // Get Image Width
  int ImageWidth = (Mode2 == true) ? SCREENSHOT_IMAGE_WIDTH_MODE2 : SCREENSHOT_IMAGE_WIDTH;

  int ImageHeight = SCREENSHOT_IMAGE_HEIGHT;
  if (Mode2 == true) ImageHeight *= 2; // Double height

  // Get bytes per pixel
  int BytePerPixel = mBitsPerPixel / 8;

  //
  // Compute image size
  //
  int ImageSize = ImageWidth * ImageHeight * BytePerPixel;

  //
  // Compute screenshot size
  //
  int ScreenshotSize = sizeof(tBitmapFileHeader) + sizeof(tBitmapHeader);
  if (BytePerPixel == 1)
  {
    // Add palette for 8bits display
    ScreenshotSize += SCREENSHOT_NB_COLORS * SCREENSHOT_COLOR_SIZE;
  }
  int Offset = ScreenshotSize;
  ScreenshotSize += ImageSize;
  *SizeP = ScreenshotSize;


  unsigned char* ScreenshotP = new unsigned char [ScreenshotSize];
  if (ScreenshotP == NULL)
    return NULL;


  unsigned char* BufferP = ScreenshotP;

  //
  // Prepare file header
  //
  // Copy template
  memcpy(BufferP,
         &BitmapFileHeader,
         sizeof(tBitmapFileHeader));
  // Update fields
  tBitmapFileHeader* FileHeader = (tBitmapFileHeader*)BufferP;
  FileHeader->FileSize = ScreenshotSize;
  FileHeader->Offset = Offset;
  BufferP += sizeof(tBitmapFileHeader);

  //
  // Prepare image header
  //
  // Copy template
  memcpy(BufferP,
         &BitmapHeader,
         sizeof(tBitmapHeader));
  // Update fields
  tBitmapHeader* BitmapHeader = (tBitmapHeader*)BufferP;
  BitmapHeader->ImageWidth = ImageWidth;
  BitmapHeader->ImageHeight = ImageHeight;
  BitmapHeader->BitsPerPixel = mBitsPerPixel;
  BitmapHeader->ImageSize = ImageSize;
  BitmapHeader->NbColors = (BytePerPixel == 1) ? SCREENSHOT_NB_COLORS : 0;
  BufferP += sizeof(tBitmapHeader);

  // Add palette
  if (BitmapHeader->NbColors)
  {
    // Prepare palette
    for (int loop=0; loop < SCREENSHOT_NB_COLORS; loop++)
    {
      *(BufferP++) = GetBValue(mPalette[loop]); // Blue
      *(BufferP++) = GetGValue(mPalette[loop]); // Green
      *(BufferP++) = GetRValue(mPalette[loop]); // Red
      *(BufferP++) = 0;                         // Reserved
    }
  }

  // Transfer bitmap content from bottom left to top right using CPC Palette conversion
  int IncrementX = mEngineGraphicFormatP->ScreenWidth / ImageWidth;
  int IncrementY = ImageHeight / SCREENSHOT_IMAGE_HEIGHT;
  int lineCount = SCREENSHOT_IMAGE_HEIGHT - 1;
  do
  {
    for (int lineIndex=0; lineIndex < IncrementY; lineIndex++)
    {
      const tUChar* lineP = (tUChar*)mEmuScreenDataP + (lineCount * mEngineGraphicFormatP->ScreenWidth * BytePerPixel);
      // 32bits display
      if (BytePerPixel == 4)
      {
        for (int rowIndex=0; rowIndex < ImageWidth; rowIndex++)
        {
          *(BufferP++) = *(lineP+0); // Red
          *(BufferP++) = *(lineP+1); // Green
          *(BufferP++) = *(lineP+2); // Blue
          *(BufferP++) = *(lineP+3); // Alpha
          lineP += IncrementX * 4;
        }
      }
      // 24bits display
      else if (BytePerPixel == 3)
      {
        for (int rowIndex=0; rowIndex < ImageWidth; rowIndex++)
        {
          *(BufferP++) = *(lineP+0); // Red
          *(BufferP++) = *(lineP+1); // Green
          *(BufferP++) = *(lineP+2); // Blue
          lineP += IncrementX * 3;
        }
      }
      // 8Bits display
      else
      {
        for (int rowIndex=0; rowIndex < ImageWidth; rowIndex++)
        {
          *(BufferP++) = *lineP;
          lineP += IncrementX;
        }
      }
    }
  }
  while (lineCount--);

  return ScreenshotP;
}
//---------------------------------------------------------------------------


bool __fastcall TDrawingContext::SaveScreenshot(AnsiString Filename,
                                                bool Mode2)
{
  if (mEngineGraphicFormatP == NULL)
    return false;

  // Create screenshot
  int ScreenshotSize;
  unsigned char* ScreenshotP = CreateScreenshot(Mode2, &ScreenshotSize);
  if (ScreenshotP == NULL)
    return false;

  // Create file
  bool Created = false;
  int FileHandle = FileCreate(Filename);
  if (FileHandle != -1)
  {
    FileWrite(FileHandle, (char*)ScreenshotP, ScreenshotSize);
    FileClose(FileHandle);
    Created = true;
  }

  delete [] ScreenshotP;

  return Created;
}
//---------------------------------------------------------------------------

bool __fastcall TDrawingContext::CompareScreenshot(AnsiString Filename)
{
  if (mEngineGraphicFormatP == NULL)
    return false;

  // Create screenshot
  int ScreenshotSize;
  unsigned char* ScreenshotP = CreateScreenshot(false, &ScreenshotSize);
  if (ScreenshotP == NULL)
    return false;

  // Open screenshot file for comparison
  int FileHandle = FileOpen(Filename, fmOpenRead);
  if (FileHandle == -1)
    return false;

  // Get File size
  // Met le curseur à la fin du fichier pour récupérer la taille du fichier
  int FileSize = FileSeek(FileHandle, 0, 2);
  // Remet le curseur en début de fichier pour la prochaine lecture
  FileSeek(FileHandle, 0, 0);

  unsigned char* FileScreenshotP = new unsigned char [FileSize];
  if (FileScreenshotP == NULL)
  {
    delete [] ScreenshotP;
    return false;
  }
  FileRead(FileHandle, FileScreenshotP, FileSize);
  FileClose(FileHandle);

  bool Equal = false;
  do
  {
    //
    // Compare format and size
    //
    tBitmapHeader* BitmapHeader = (tBitmapHeader*)(ScreenshotP + sizeof(tBitmapFileHeader));
    tBitmapHeader* FileBitmapHeader = (tBitmapHeader*)(FileScreenshotP + sizeof(tBitmapFileHeader));
    if (BitmapHeader->ImageWidth != FileBitmapHeader->ImageWidth)
      continue;
    if (BitmapHeader->ImageHeight != FileBitmapHeader->ImageHeight)
      continue;
    if (BitmapHeader->BitsPerPixel != FileBitmapHeader->BitsPerPixel)
      continue;
    if (BitmapHeader->NbColors != FileBitmapHeader->NbColors)
      continue;

    // Get image size
    int ImageSize = BitmapHeader->ImageWidth * BitmapHeader->ImageHeight;
    ImageSize *= BitmapHeader->BitsPerPixel / 8;

    //
    // Compare images
    //
    tUChar* DataP = (tUChar*)BitmapHeader + sizeof(tBitmapHeader) + (BitmapHeader->NbColors * 4);
    tUChar* FileDataP = (tUChar*)FileBitmapHeader + sizeof(tBitmapHeader) + (BitmapHeader->NbColors * 4);
    int Loop = ImageSize;
    while (--Loop)
    {
      if (*DataP++ != *FileDataP++)
        break;
    }

    // Images are equal ?
    if (Loop == 0)
    {
      Equal = true;
    }
  }
  while (0);

  delete [] FileScreenshotP;
  delete [] ScreenshotP;

  return Equal;
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::OnDisplayResize(int NewWidth,
                                                 int NewHeight)
{
  if (mEngineGraphicFormatP == NULL)
    return;

  //
  // Clear current display
  //
#ifdef _USE_DIRECTX

  if (mUseDirectX == true)
  {
    mD3DDevice->Clear(0,
                      NULL,
                      D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                      D3DCOLOR_COLORVALUE(0.0f,0.0f,0.0f,1.0f),
                      1.0f,
                      0);
  }
  else

#endif /* _USE_DIRECTX */

#ifdef _USE_OPENGL

  if (mUseOpenGL == true)
  {
    if (mGLOnscreenHGLRC != NULL)
    {
      glViewport(0, 0, NewWidth, NewHeight);
      glLoadIdentity();
      glOrtho(0, NewWidth, NewHeight, 0, 0, 1);

      glClear(GL_DEPTH_BUFFER_BIT);
    }
  }
  else

#endif /* _USE_OPENGL */

  // GDI ??
  if (mHostCanvas != NULL)
  {
    // Blank screen
    HBRUSH hBlackBrush = CreateSolidBrush(RGB(0,0,0));
    RECT ClientArea;
    SetRect(&ClientArea, 0, 0, NewWidth, NewHeight);
    FillRect(mHostCanvas->Handle, &ClientArea, hBlackBrush);
    DeleteObject(hBlackBrush);
  }

  // Store new size
  mDisplayWidth = NewWidth;
  mDisplayHeight = NewHeight;

  // Windowed
  if (mFullscreen == false)
  {
    mEmulatorScreenLeft = 0;
    mEmulatorScreenTop = 0;
    mEmulatorScreenRight = NewWidth;
    mEmulatorScreenBottom = NewHeight;
    mEmulatorScreenWidth = NewWidth;
    mEmulatorScreenHeight = NewHeight;

    mOffscreenOriginTop = 0;
    mOffscreenOriginLeft = 0;
  }
  else // Fullscreen
  {
    double EmulatorRatio = (double)mEngineGraphicFormatP->VisibleScreenWidth
                           / (double)mEngineGraphicFormatP->VisibleScreenHeight;
    double ScreenRatio = (double)NewWidth / (double)NewHeight;

    if ( (mUseOpenGL == true) || (mUseDirectX == true))
    {
      mOffscreenOriginTop = 0;
      mOffscreenOriginLeft = 0;
    }

    if (mSettingsP->KeepAspectRatio == true)
    {
      // Screen is larger than emulator
      if (ScreenRatio > EmulatorRatio)
      {
        int EmulatorHeight = NewHeight;
        if (mScanlinesHeight)
        {
          EmulatorHeight = (NewHeight / mScanlinesHeight) * mScanlinesHeight;
        }
        mEmulatorScreenTop = (NewHeight - EmulatorHeight) / 2;
        mEmulatorScreenBottom = mEmulatorScreenTop + EmulatorHeight;

        mEmulatorScreenHeight = EmulatorHeight;
        mEmulatorScreenWidth = (int)((double)EmulatorHeight * EmulatorRatio);
        if (mScanlinesWidth)
        {
          mEmulatorScreenWidth = (mEmulatorScreenWidth / mScanlinesWidth) * mScanlinesWidth;
        }
        mEmulatorScreenLeft = (NewWidth - mEmulatorScreenWidth) / 2;
        mEmulatorScreenRight = mEmulatorScreenLeft + mEmulatorScreenWidth;

        if ( (mUseOpenGL == false) && (mUseDirectX == false) )
        {
          mOffscreenOriginTop = 0;
          mOffscreenOriginLeft = mEmulatorScreenLeft;
          mEmulatorScreenLeft = 0;
          mEmulatorScreenRight = mEmulatorScreenWidth;
        }
      }
      else // Screen is taller than emulator
      {
        int EmulatorWidth = NewWidth;
        if (mScanlinesWidth)
        {
          EmulatorWidth = (NewWidth / mScanlinesWidth) * mScanlinesWidth;
        }
        mEmulatorScreenLeft = (NewWidth - EmulatorWidth) / 2;
        mEmulatorScreenRight = mEmulatorScreenLeft + EmulatorWidth;

        mEmulatorScreenWidth = EmulatorWidth;
        mEmulatorScreenHeight = (int)((double)EmulatorWidth / EmulatorRatio);
        if (mScanlinesHeight)
        {
          mEmulatorScreenHeight = (mEmulatorScreenHeight / mScanlinesHeight) * mScanlinesHeight;
        }
        mEmulatorScreenTop = (NewHeight - mEmulatorScreenHeight) / 2;
        mEmulatorScreenBottom = mEmulatorScreenTop + mEmulatorScreenHeight;

        if ( (mUseOpenGL == false) && (mUseDirectX == false) )
        {
          mOffscreenOriginTop = mEmulatorScreenTop;
          mOffscreenOriginLeft = 0;
          mEmulatorScreenTop = 0;
          mEmulatorScreenBottom = mEmulatorScreenHeight;
        }
      }
    }
    else
    {
      // Fullscreen total
      mEmulatorScreenLeft = 0;
      mEmulatorScreenRight = NewWidth;
      mEmulatorScreenTop = 0;
      mEmulatorScreenBottom = NewHeight;

      mEmulatorScreenWidth = NewWidth;
      mEmulatorScreenHeight = NewHeight;

      mOffscreenOriginTop = 0;
      mOffscreenOriginLeft = 0;
    }
  }

  //
  // Rebuild fonts
  //
  CreateDialogFont();
  CreateInfoFont();

#ifdef _USE_OPENGL

  if (mUseOpenGL == true)
  {
    if (mGLDialogFontBase)
    {
      glDeleteLists(mGLDialogFontBase, 96);
    }
    mGLDialogFontBase = GLBuildFontList(mDialogFont);
    if (mGLDialogFontBase)
    {
      glDeleteLists(mGLDialogFontBase, 96);
    }
    mGLDialogFontBase = GLBuildFontList(mInfoFont);
  }

#endif /* _USE_OPENGL */

  // Update scanline texture
  UpdateScanlinesTexture();


  //
  // Update screen coordinates
  //
  // Spinning Floppy bitmap
  tDrawingBitmapContext* BitmapContextP = &mBitmapContext[SpinningFloppyId];
  BitmapContextP->ScreenLeft = mEmulatorScreenLeft + FLOPPY_BITMAP_POS_X;
  BitmapContextP->ScreenTop = mEmulatorScreenTop + FLOPPY_BITMAP_POS_Y;
  BitmapContextP->ScreenRight = BitmapContextP->ScreenLeft + BitmapContextP->ScreenWidth;
  BitmapContextP->ScreenBottom = BitmapContextP->ScreenTop + BitmapContextP->ScreenHeight;
  // Static Floppy bitmap
  BitmapContextP = &mBitmapContext[StaticFloppyId];
  BitmapContextP->ScreenLeft = mEmulatorScreenLeft + FLOPPY_BITMAP_POS_X;
  BitmapContextP->ScreenTop = mEmulatorScreenTop + FLOPPY_BITMAP_POS_Y;
  BitmapContextP->ScreenRight = BitmapContextP->ScreenLeft + BitmapContextP->ScreenWidth;
  BitmapContextP->ScreenBottom = BitmapContextP->ScreenTop + BitmapContextP->ScreenHeight;
  // Tape bitmap
  BitmapContextP = &mBitmapContext[TapeId];
  BitmapContextP->ScreenLeft = mEmulatorScreenLeft + TAPE_BITMAP_POS_X;
  BitmapContextP->ScreenTop = mEmulatorScreenTop + TAPE_BITMAP_POS_Y;
  BitmapContextP->ScreenRight = BitmapContextP->ScreenLeft + BitmapContextP->ScreenWidth;
  BitmapContextP->ScreenBottom = BitmapContextP->ScreenTop + BitmapContextP->ScreenHeight;
  // Onscreen Icons
  for (int Loop=0; Loop < NB_OSD_BITMAPS; Loop++)
  {
    BitmapContextP = &mBitmapContext[Loop];
    if (OnscreenIcons[Loop].Left >= 0)
    {
      BitmapContextP->ScreenLeft = mEmulatorScreenLeft + OnscreenIcons[Loop].Left;
      BitmapContextP->ScreenRight = BitmapContextP->ScreenLeft + BitmapContextP->ScreenWidth;
    }
    else
    {
      BitmapContextP->ScreenRight = mEmulatorScreenRight - OnscreenIcons[Loop].Right;
      BitmapContextP->ScreenLeft = BitmapContextP->ScreenRight - BitmapContextP->ScreenWidth;
    }

    if (OnscreenIcons[Loop].Top >= 0)
    {
      BitmapContextP->ScreenTop = mEmulatorScreenTop + OnscreenIcons[Loop].Top;
      BitmapContextP->ScreenBottom = BitmapContextP->ScreenTop + BitmapContextP->ScreenHeight;
    }
    else
    {
      BitmapContextP->ScreenBottom = mEmulatorScreenBottom - OnscreenIcons[Loop].Bottom;
      BitmapContextP->ScreenTop = BitmapContextP->ScreenBottom - BitmapContextP->ScreenHeight;
    }
  }

  // Spinning Floppy
  mSpinningFloppyRotateLeft = mEmulatorScreenLeft + FLOPPY_BITMAP_POS_X +
                              (mBitmapContext[SpinningFloppyId].ScreenWidth/2);
  mSpinningFloppyRotateTop = mEmulatorScreenTop + FLOPPY_BITMAP_POS_Y +
                             (mBitmapContext[SpinningFloppyId].ScreenWidth/2);

#ifdef _USE_OPENGL

  if (mUseOpenGL == true)
  {
    glActiveTexture(GL_TEXTURE0);
    if (mEmulatorScreenWidth >= ((int)mEngineGraphicFormatP->VisibleScreenWidth * 2))
    {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
      // Under mode 2 resolution, use antialiasing
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
  }

#endif /* _USE_OPENGL */

#ifdef _USE_DIRECTX

  if (mUseDirectX == true)
  {
    do
    {
      ReleaseD3DDevice();

      mD3DPresentParam.BackBufferWidth      = mDisplayWidth;
		  mD3DPresentParam.BackBufferHeight     = mDisplayHeight;
      mD3DDevice->Reset(&mD3DPresentParam);

      if (InitD3DDevice() == false)
      {
        // Restore GDI functions
        mDrawBegin = DrawBeginGDI;
        mDrawFinalize = DrawFinalizeGDI;
        mDrawEmulatorScreen = DrawEmulatorScreenGDI;
        mDrawFloppy = DrawStaticFloppyGDI;
        mDrawTape = DrawTapeGDI;
        mDrawEmulatorMessages = DrawEmulatorMessagesGDI;
        mDrawEmulatorText = DrawEmulatorTextGDI;
        mDrawOnscreenIconsBegin = DrawOnscreenIconsBeginGDI;
        mDrawOnscreenIconsFinalize = DrawOnscreenIconsFinalizeGDI;
        mDrawOnscreenIcon = DrawOnscreenIconGDI;
        continue;
      }

      UpdateScanlinesTexture();

      //
      // Emulator screen vertices
      //
      float Left = (float)mEmulatorScreenLeft;
      float Top = (float)mEmulatorScreenTop;
      float Right = (float)mEmulatorScreenRight;
      float Bottom = (float)mEmulatorScreenBottom;
      float Width = (float)CPC_VISIBLE_SCR_WIDTH;
      float Height = (float)CPC_VISIBLE_SCR_HEIGHT;
      if ( (mFullscreen == true) && (mScanlinesWidth) && (mScanlinesHeight) )
      {
        Width = (float)(mEmulatorScreenWidth / mScanlinesWidth);
        Height = (float)(mEmulatorScreenHeight / mScanlinesHeight);
      }

      D3DScreenVertex ScreenVertices[] =
      {
        //X,     Y,      Z,    RHW,   U1,   V1     U2,    V2,
        { Left,  Top,    0.0f, 1.0f,  0.0f, 0.0f,  0.0f,  0.0f },
        { Right, Top,    0.0f, 1.0f,  1.0f, 0.0f,  Width, 0.0f },
        { Left,  Bottom, 0.0f, 1.0f,  0.0f, 1.0f,  0.0f,  Height },
        { Right, Bottom, 0.0f, 1.0f,  1.0f, 1.0f,  Width, Height }
      };
      //adjust all the vertices to correctly line up texels with pixels
      for (int i=0; i<4; i++)
      {
          ScreenVertices[i].x -= 0.5f;
          ScreenVertices[i].y -= 0.5f;
      }
      // Prepare screen vertices
      void *pVertices = NULL;
      mD3DScreenVertexBuffer->Lock( 0, sizeof(ScreenVertices), (void**)&pVertices, 0);
      if (pVertices != NULL)
      {
        memcpy(pVertices, ScreenVertices, sizeof(ScreenVertices) );
        mD3DScreenVertexBuffer->Unlock();
      }

      //
      // Tape bitmap vertices
      //
      tDrawingBitmapContext* BitmapContextP = &mBitmapContext[TapeId];
      Left = (float)mEmulatorScreenLeft + TAPE_BITMAP_POS_X;
      Top  = (float)mEmulatorScreenTop + TAPE_BITMAP_POS_Y;
      Right = Left + BitmapContextP->ScreenWidth;
      Bottom = Top + BitmapContextP->ScreenHeight;
      D3DBitmapVertex TapeVertices[] =
      {
        //X,     Y,      Z,    RHW,   U,    V
        { Left,  Top,    0.0f, 1.0f,  0.0f, 1.0f },
        { Right, Top,    0.0f, 1.0f,  1.0f, 1.0f },
        { Left,  Bottom, 0.0f, 1.0f,  0.0f, 0.0f },
        { Right, Bottom, 0.0f, 1.0f,  1.0f, 0.0f }
      };
      //adjust all the vertices to correctly line up texels with pixels
      for (int i=0; i<4; i++)
      {
          TapeVertices[i].x -= 0.5f;
          TapeVertices[i].y -= 0.5f;
      }
      // Prepare tape vertices
      BitmapContextP->D3DVertexBuffer->Lock(0, sizeof(TapeVertices), (void**)&pVertices, 0);
      if (pVertices != NULL)
      {
        memcpy(pVertices, TapeVertices, sizeof(TapeVertices) );
        BitmapContextP->D3DVertexBuffer->Unlock();
      }

      //
      // Spinning Floppy bitmap vertices
      //
      BitmapContextP = &mBitmapContext[SpinningFloppyId];
      Left = (float)mEmulatorScreenLeft;
      Top  = (float)mEmulatorScreenTop;
      Right = Left + BitmapContextP->ScreenWidth;
      Bottom = Top + BitmapContextP->ScreenWidth;
      D3DBitmapVertex SpinningFloppyVertices[] =
      {
        //X,     Y,      Z,    RHW,   U,    V
        { Left,  Top,    0.0f, 1.0f,  0.0f, 1.0f },
        { Right, Top,    0.0f, 1.0f,  1.0f, 1.0f },
        { Left,  Bottom, 0.0f, 1.0f,  0.0f, 0.0f },
        { Right, Bottom, 0.0f, 1.0f,  1.0f, 0.0f }
      };
      // Prepare floppy vertices
      BitmapContextP->D3DVertexBuffer->Lock(0,
                                            sizeof(SpinningFloppyVertices),
                                            (void**)&pVertices, 0);
      if (pVertices != NULL)
      {
        memcpy(pVertices, SpinningFloppyVertices, sizeof(SpinningFloppyVertices) );
        BitmapContextP->D3DVertexBuffer->Unlock();
      }
      mD3DScreenFloppyVertexBuffer->Lock(0, sizeof(SpinningFloppyVertices), (void**)&pVertices, 0);
      if (pVertices != NULL)
      {
        memcpy(pVertices, SpinningFloppyVertices, sizeof(SpinningFloppyVertices) );
        mD3DScreenFloppyVertexBuffer->Unlock();
      }

      //
      // Static Floppy bitmap vertices
      //
      BitmapContextP = &mBitmapContext[StaticFloppyId];
      Left = (float)mEmulatorScreenLeft;
      Top  = (float)mEmulatorScreenTop;
      Right = Left + BitmapContextP->ScreenWidth;
      Bottom = Top + BitmapContextP->ScreenHeight;
      D3DBitmapVertex StaticFloppyVertices[] =
      {
        //X,     Y,      Z,    RHW,   U,    V
        { Left,  Top,    0.0f, 1.0f,  0.0f, 1.0f },
        { Right, Top,    0.0f, 1.0f,  1.0f, 1.0f },
        { Left,  Bottom, 0.0f, 1.0f,  0.0f, 0.0f },
        { Right, Bottom, 0.0f, 1.0f,  1.0f, 0.0f }
      };
      // Prepare floppy vertices
      BitmapContextP->D3DVertexBuffer->Lock(0,
                                            sizeof(StaticFloppyVertices),
                                            (void**)&pVertices, 0);
      if (pVertices != NULL)
      {
        memcpy(pVertices, StaticFloppyVertices, sizeof(StaticFloppyVertices) );
        BitmapContextP->D3DVertexBuffer->Unlock();
      }

      //
      // Onscreen icons vectices
      //
      for (int Loop=0; Loop < NB_OSD_BITMAPS; Loop++)
      {
        BitmapContextP = &mBitmapContext[Loop];
        // From Screen Left ?
        if (OnscreenIcons[Loop].Left >= 0)
        {
          Left = (float)mEmulatorScreenLeft + OnscreenIcons[Loop].Left;
          Right = Left + BitmapContextP->ScreenWidth;
        }
        else
        {
          Right = (float)mEmulatorScreenRight - OnscreenIcons[Loop].Right;
          Left = Right - BitmapContextP->ScreenWidth;
        }

        // From Screen Top ?
        if (OnscreenIcons[Loop].Top >= 0)
        {
          Top  = (float)mEmulatorScreenTop + OnscreenIcons[Loop].Top;
          Bottom = Top + BitmapContextP->ScreenHeight;
        }
        else
        {
          Bottom  = (float)mEmulatorScreenBottom - OnscreenIcons[Loop].Bottom;
          Top = Bottom - BitmapContextP->ScreenHeight;
        }
        D3DBitmapVertex Vertices[] =
        {
          //X,     Y,      Z,    RHW,   U,    V
          { Left,  Top,    0.0f, 1.0f,  0.0f, 1.0f },
          { Right, Top,    0.0f, 1.0f,  1.0f, 1.0f },
          { Left,  Bottom, 0.0f, 1.0f,  0.0f, 0.0f },
          { Right, Bottom, 0.0f, 1.0f,  1.0f, 0.0f }
        };
        //adjust all the vertices to correctly line up texels with pixels
        for (int i=0; i<4; i++)
        {
          Vertices[i].x -= 0.5f;
          Vertices[i].y -= 0.5f;
        }
        // Prepare onscreen icon vertices
        BitmapContextP->D3DVertexBuffer->Lock(0, sizeof(Vertices), (void**)&pVertices, 0);
        if (pVertices != NULL)
        {
          memcpy(pVertices, Vertices, sizeof(Vertices) );
          BitmapContextP->D3DVertexBuffer->Unlock();
        }
      }
      

      // Sampler settings
      if (mEmulatorScreenWidth >= ((int)mEngineGraphicFormatP->VisibleScreenWidth * 2))
      {
        mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
        mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
      }
      else
      {
        // Under mode 2 resolution, use antialiasing
        mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
        mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
      }
    }
    while (0);
  }

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawBeginD3D(void)
{
#ifdef _USE_DIRECTX

  mD3DDevice->BeginScene();

  // Reset last error
  mLastError = ERROR_SUCCESS;
  ::SetLastError(mLastError);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawBeginGL(void)
{
#ifdef _USE_OPENGL

  // Render display
  //SwapBuffers(mOnscreenHDC);
  //glFlush();

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  //glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  // Usefull ?
  /*if (mGLOnscreenHGLRC != NULL)
  {
    glViewport(0, 0, mDisplayWidth, mDisplayHeight);
    glLoadIdentity();
    glOrtho(0, mDisplayWidth, mDisplayHeight, 0, 0, 1);
  }*/

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawBeginGDI(void)
{
  mOffscreenBitmap = new Graphics::TBitmap;
  mOffscreenBitmap->Width = mEmulatorScreenWidth;
  mOffscreenBitmap->Height = mEmulatorScreenHeight;
  mOffscreenBitmap->HandleType = bmDIB;
  mOffscreenBitmap->PixelFormat = pf32bit;
  mOnscreenHDC = mOffscreenBitmap->Canvas->Handle;
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawFinalizeD3D(void)
{
#ifdef _USE_DIRECTX

  mD3DDevice->EndScene();

  // Device lost ?
  if (mD3DDevice->Present(NULL, NULL, NULL, NULL) == D3DERR_DEVICELOST)
  {
    HRESULT result = mD3DDevice->TestCooperativeLevel();

    if (result == D3DERR_DEVICENOTRESET)
    {
      // Restore device
      OnDisplayResize(mDisplayWidth, mDisplayHeight);
    }
  }

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawFinalizeGL(void)
{
#ifdef _USE_OPENGL

  // Make sure all commands are executed.
  //glFlush();
  //glFinish();
  // Request display swap
  SwapBuffers(mOnscreenHDC);
  // Make sure swapping is executed.
  //glFlush();
  glFinish();

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawFinalizeGDI(void)
{
  if (mOffscreenBitmap != NULL)
  {
    mHostCanvas->Draw(mOffscreenOriginLeft,
                      mOffscreenOriginTop,
                      mOffscreenBitmap);

    delete mOffscreenBitmap;
    mOffscreenBitmap = NULL;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawEmulatorScreenD3D(void)
{
#ifdef _USE_DIRECTX

#ifdef _DEBUG
  if (mEngineGraphicFormatP == NULL)
    return;
#endif /* _DEBUG */

  do
  {
    D3DLOCKED_RECT lockedRect = {0}; // "out" parameter from LockRect function below
    if (FAILED(mD3DEmuScreenTexture->LockRect(0, &lockedRect, NULL, 0)))
    {
      continue;
    }

    // Copy emulator screen (take care of pitch!!)
    tULong* BitmapPix = (tULong*)mEmuScreenDataP;
    for (int y = 0; y < (int)mEngineGraphicFormatP->VisibleScreenHeight; y++)
    {
        tULong* CurRow = (tULong*)((tUChar*)lockedRect.pBits + (y * lockedRect.Pitch));
        int x = mEngineGraphicFormatP->ScreenWidth;
        while(x--)
        {
          *CurRow++ = *(BitmapPix++);
        }
    }
    
    // unlock when done
    mD3DEmuScreenTexture->UnlockRect(0);

    // Apply emulator texture
    if ( (mDrawScanlines == true)
         && (mEmulatorScreenHeight >= (int)(mEngineGraphicFormatP->VisibleScreenHeight*2)) )
    {
      // Adjust brightness in fullscreen
      if (mFullscreen == true)
      {
        D3DCOLOR BrightnessValue;
        BrightnessValue = D3DCOLOR_XRGB(mD3DFullscreenBrightness,
                                        mD3DFullscreenBrightness,
                                        mD3DFullscreenBrightness);
        mD3DDevice->SetTextureStageState(2, D3DTSS_CONSTANT,  BrightnessValue);
        mD3DDevice->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT);
        mD3DDevice->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CONSTANT);
        mD3DDevice->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_MODULATE4X);
      }
      else
      {
        mD3DDevice->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_DISABLE);
      }

      mD3DDevice->SetTexture(0, mD3DEmuScreenTexture);
      mD3DDevice->SetTexture(1, mD3DScanlinesTexture);
      mD3DDevice->SetStreamSource(0, mD3DScreenVertexBuffer, 0, sizeof(D3DScreenVertex) );
      mD3DDevice->SetFVF(D3DFVF_SCREENVERTEX);
	    mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
      mD3DDevice->SetTexture(0, NULL);
      mD3DDevice->SetTexture(1, NULL);
    }
    else
    {
      mD3DDevice->SetTexture(0, mD3DEmuScreenTexture);
      mD3DDevice->SetStreamSource(0, mD3DScreenVertexBuffer, 0, sizeof(D3DScreenVertex) );
      mD3DDevice->SetFVF(D3DFVF_SCREENVERTEX);
	    mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
      mD3DDevice->SetTexture(0, NULL);
    }
  }
  while (0);

  mLastError = ::GetLastError();

#else /* _USE_DIRECTX */

  mLastError = E_FAIL;

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawEmulatorScreenGL(void)
{
#ifdef _USE_OPENGL

  if (mEngineGraphicFormatP == NULL)
    return;

  do
  {
    // Create texture from Emulator screen DIB
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mGLEmuScreenTexture);

    if (mBitsPerPixel == 32)
    {
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   mEngineGraphicFormatP->ScreenWidth,
                   mEngineGraphicFormatP->VisibleScreenHeight,
                   0,
                   GL_BGRA,
                   GL_UNSIGNED_BYTE,
                   mEmuScreenDataP);
    }
    else if (mBitsPerPixel == 24)
    {
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   mEngineGraphicFormatP->ScreenWidth,
                   mEngineGraphicFormatP->VisibleScreenHeight,
                   0,
                   GL_BGR,
                   GL_UNSIGNED_BYTE,
                   mEmuScreenDataP);
    }
    else
    {
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA,
                   mEngineGraphicFormatP->ScreenWidth,
                   mEngineGraphicFormatP->VisibleScreenHeight,
                   0,
                   GL_COLOR_INDEX,
                   GL_UNSIGNED_BYTE,
                   mEmuScreenDataP);
    }
    mLastError = glGetError();
    if (mLastError != GL_NO_ERROR)
    {
#ifdef _DEBUG
			String msg = _T("glTexImage2D Error=") + String::IntToHex(mLastError,8);
			Application->MessageBox(msg.c_str(), _T("DrawEmulatorScreen"), MB_OK);
#endif /* _DEBUG */
      continue;
    }

    if (mEmulatorScreenHeight >= ((int)mEngineGraphicFormatP->VisibleScreenHeight * 2))
    {
      if (mDrawScanlines == true)
      {
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Adjust brightness in fullscreen
        if (mFullscreen == true)
        {
          glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
          glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
          glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
          glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
          glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 4.0f);
          GLfloat Brightness = mGLFullscreenBrightness / BRIGHTNESS_MAXVALUE;
          glColor3f(Brightness, Brightness, Brightness);
        }
        else
        {
          glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
          glColor3f(1.0f, 1.0f, 1.0f);
        }
      }
      else
      {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glColor3f(1.0f, 1.0f, 1.0f);
      }
    }
    else
    {
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glColor3f(1.0f, 1.0f, 1.0f);
    }

    if ( (mDrawScanlines == true)
         && (mEmulatorScreenHeight > (int)mEngineGraphicFormatP->VisibleScreenHeight) )
    {
      float Width = (float)CPC_VISIBLE_SCR_WIDTH;
      float Height = (float)CPC_VISIBLE_SCR_HEIGHT;
      if (mFullscreen == true)
      {
        Width = (float)(mEmulatorScreenWidth / mScanlinesWidth);
        Height = (float)(mEmulatorScreenHeight / mScanlinesHeight);
      }

      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
        glVertex2i(mEmulatorScreenLeft, mEmulatorScreenTop);
        glTexCoord2f(0.0f, 1.0f);
        glMultiTexCoord2f(GL_TEXTURE1, 0.0f, Height);
        glVertex2i(mEmulatorScreenLeft, mEmulatorScreenBottom);
        glTexCoord2f(1.0f, 1.0f);
        glMultiTexCoord2f(GL_TEXTURE1, Width, Height);
        glVertex2i(mEmulatorScreenRight, mEmulatorScreenBottom);
        glTexCoord2f(1.0f, 0.0f);
        glMultiTexCoord2f(GL_TEXTURE1, Width, 0.0f);
        glVertex2i(mEmulatorScreenRight, mEmulatorScreenTop);
      glEnd();
    }
    else
    {
      glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2i(mEmulatorScreenLeft, mEmulatorScreenTop);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2i(mEmulatorScreenLeft, mEmulatorScreenBottom);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2i(mEmulatorScreenRight, mEmulatorScreenBottom);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2i(mEmulatorScreenRight, mEmulatorScreenTop);
      glEnd();
    }

    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
  }
  while (0);

#else /* _USE_OPENGL */

  mLastError = E_FAIL;

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawEmulatorScreenGDI()
{
#ifdef _DEBUG
  if (mEngineGraphicFormatP == NULL)
    return;
#endif /* _DEBUG */

  int RasterCaps = GetDeviceCaps(mOnscreenHDC, RASTERCAPS);

  if ((RasterCaps & RC_STRETCHDIB) == RC_STRETCHDIB)
  {
    if (StretchDIBits(mOnscreenHDC,
                      0,
                      0,
                      mEmulatorScreenWidth,
                      mEmulatorScreenHeight,
                      0,
                      mEngineGraphicFormatP->ScreenHeight - mEngineGraphicFormatP->VisibleScreenHeight,
                      mEngineGraphicFormatP->ScreenWidth,
                      mEngineGraphicFormatP->VisibleScreenHeight,
                      mEmuScreenDataP,
                      mEmuScreenBITMAPINFO,
                      DIB_RGB_COLORS,
                      SRCCOPY) == 0)
    {
      mLastError = ::GetLastError();
    }

#ifdef USE_GDI_SCANLINES
    if ( (mDrawScanlines == true)
         && (mEmulatorScreenHeight > (int)mEngineGraphicFormatP->VisibleScreenHeight) )

    {
      // Draw scanlines bitmap using AND operation
      StretchDIBits(mOnscreenHDC,
                    0,
                    0,
                    mEmulatorScreenWidth,
                    mEmulatorScreenHeight,
                    0,
                    0,
                    mEngineGraphicFormatP->VisibleScreenWidth,
                    mEngineGraphicFormatP->VisibleScreenHeight * 2,
                    mGDIScanlinesMaskBits,
                    mGDIScanlinesMaskBITMAPINFO,
                    DIB_RGB_COLORS,
                    SRCAND);
    }
#endif /* USE_GDI_SCANLINES */

  }
  else if ((RasterCaps & RC_STRETCHBLT) == RC_STRETCHBLT)
  {
    // Copy Emulator screen
    if (StretchBlt(mOnscreenHDC,
                   0,
                   0,
                   mEmulatorScreenWidth,
                   mEmulatorScreenHeight,
                   mEmuScreenHDC,
                   0,
                   0,
                   mEngineGraphicFormatP->ScreenWidth,
                   mEngineGraphicFormatP->VisibleScreenHeight,
                   SRCCOPY) == 0)
    {
      mLastError = ::GetLastError();
    }

#ifdef USE_GDI_SCANLINES
    if ( (mDrawScanlines == true)
         && (mEmulatorScreenHeight > (int)mEngineGraphicFormatP->VisibleScreenHeight) )
    {
      HDC hdcMask = CreateCompatibleDC(mOnscreenHDC);

      BITMAP bm;
      GetObject(mGDIScanlinesMaskHBITMAP, sizeof(bm), &bm);
      SelectObject(hdcMask, mGDIScanlinesMaskHBITMAP);

      // Draw scanlines bitmap using AND operation
      StretchBlt(mOnscreenHDC,
                 0,
                 0,
                 mEmulatorScreenWidth,
                 mEmulatorScreenHeight,
                 hdcMask,
                 0,
                 0,
                 bm.bmWidth,
                 bm.bmHeight,
                 SRCAND);

      DeleteDC(hdcMask);
    }
#endif /* USE_GDI_SCANLINES */
  }
  else
  {
    mLastError = E_FAIL;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawSpinningFloppyD3D(bool bActive, bool bPause)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[SpinningFloppyId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_DIRECTX

  bool DrawFloppy = false;

#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (bActive == true)
#else /* FORCE_FLOPPY_BITMAP_DRAWING */
  if (1)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    mD3DFloppyTransparency = 255;
    DrawFloppy = true;
  }
  else if (mD3DFloppyTransparency > 0)
  {
    mD3DFloppyTransparency -= FLOPPY_ALPHASTEP_D3D;
    if (mD3DFloppyTransparency > 0)
    {
      DrawFloppy = true;
    }
  }

  // Floppy image to draw ?
#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (DrawFloppy == true)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    // Save context
    DWORD oldMinFilter;
    DWORD oldMagFilter;
    mD3DDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &oldMinFilter);
    mD3DDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &oldMagFilter);

    // Bitmap rotation
    D3DBitmapVertex *pSourceVertices = NULL;
    BitmapContextP->D3DVertexBuffer->Lock(0, D3D_BITMAP_VERTEXBUFFER_SIZE, (void**)&pSourceVertices, 0);
    if (pSourceVertices != NULL)
    {
      D3DBitmapVertex *pVertices = NULL;
      mD3DScreenFloppyVertexBuffer->Lock(0, D3D_BITMAP_VERTEXBUFFER_SIZE, (void**)&pVertices, 0);
      if (pVertices != NULL)
      {
        // Translate to origin
        D3DXMATRIX matToOrigin;
        D3DXMatrixTranslation(&matToOrigin, -mSpinningFloppyRotateLeft, -mSpinningFloppyRotateTop, 1.0f);
        // Rotation - z rotation for 2D
        D3DXMATRIX matRot;
        D3DXMatrixRotationZ(&matRot, D3DXToRadian(mD3DFloppyRotationAngle));
        // Translate back
        D3DXMATRIX matTrans;
        D3DXMatrixTranslation(&matTrans, mSpinningFloppyRotateLeft, mSpinningFloppyRotateTop, 1.0f);
        // Final world matrix - local to world transform
        D3DXMATRIX matWorld = matToOrigin * matRot * matTrans;
        
        // Transform vertices by matWorld
        for (unsigned int i = 0;i < 4; ++i)
        {
          D3DXVec3TransformCoord((D3DXVECTOR3*)&pVertices[i],
                                 (D3DXVECTOR3*)&pSourceVertices[i],
                                 &matWorld);
        }

        mD3DScreenFloppyVertexBuffer->Unlock();
      }

      BitmapContextP->D3DVertexBuffer->Unlock();
    }

    // Set transparency
    DWORD AlphaValue;
    AlphaValue = D3DCOLOR_ARGB(mD3DFloppyTransparency,255,255,255);
    mD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_CONSTANT,  AlphaValue);
    mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);

    // Draw floppy texture
    mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    mD3DDevice->SetTexture(0, BitmapContextP->D3DTexture);
    mD3DDevice->SetStreamSource(0, mD3DScreenFloppyVertexBuffer, 0, sizeof(D3DBitmapVertex) );
    mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    mD3DDevice->SetFVF(D3DFVF_BITMAPVERTEX);
    mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    mD3DDevice->SetTexture(0, NULL);

    // Restore context
    mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, oldMinFilter);
    mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, oldMagFilter);

    // Update angle for next display
    if (bPause == false)
    {
      mD3DFloppyRotationAngle += FLOPPY_ROTATION_STEP;
      if (mD3DFloppyRotationAngle > 360.0f)
      {
        mD3DFloppyRotationAngle -= 360.0f;
      }
    }
  }

  mLastError = ::GetLastError();

#else /* _USE_DIRECTX */

  mLastError = E_FAIL;

  NOT_USED(bActive);
  NOT_USED(bPause);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawStaticFloppyD3D(bool bActive, bool bPause)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[StaticFloppyId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_DIRECTX

  bool DrawFloppy = false;

#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (bActive == true)
#else /* FORCE_FLOPPY_BITMAP_DRAWING */
  if (1)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    mD3DFloppyTransparency = 255;
    DrawFloppy = true;
  }
  else if (mD3DFloppyTransparency > 0)
  {
    mD3DFloppyTransparency -= FLOPPY_ALPHASTEP_D3D;
    if (mD3DFloppyTransparency > 0)
    {
      DrawFloppy = true;
    }
  }

  // Floppy image to draw ?
#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (DrawFloppy == true)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    // Save context
    DWORD oldMinFilter;
    DWORD oldMagFilter;
    mD3DDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &oldMinFilter);
    mD3DDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &oldMagFilter);

    // Set transparency
    DWORD AlphaValue;
    AlphaValue = D3DCOLOR_ARGB(mD3DFloppyTransparency,255,255,255);
    mD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    mD3DDevice->SetTextureStageState(0, D3DTSS_CONSTANT,  AlphaValue);
    mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);

    // Draw floppy texture
    mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    mD3DDevice->SetTexture(0, BitmapContextP->D3DTexture);
    mD3DDevice->SetStreamSource(0, BitmapContextP->D3DVertexBuffer, 0, sizeof(D3DBitmapVertex));
    mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    mD3DDevice->SetFVF(D3DFVF_BITMAPVERTEX);
    mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    mD3DDevice->SetTexture(0, NULL);

    // Restore context
    mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, oldMinFilter);
    mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, oldMagFilter);
  }

  mLastError = ::GetLastError();

  NOT_USED(bPause);

#else /* _USE_DIRECTX */

  mLastError = E_FAIL;

  NOT_USED(bActive);
  NOT_USED(bPause);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawSpinningFloppyGL(bool bActive, bool bPause)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[SpinningFloppyId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_OPENGL

  bool DrawFloppy = false;

#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (bActive == true)
#else /* FORCE_FLOPPY_BITMAP_DRAWING */
  if (1)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    mGLFloppyTransparency = 1.0f;
    DrawFloppy = true;
  }
  else if (mGLFloppyTransparency > 0)
  {
    mGLFloppyTransparency -= FLOPPY_ALPHASTEP_GL;
    if (mGLFloppyTransparency > 0)
    {
      DrawFloppy = true;
    }
  }

  // Floppy image to draw ?
#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (DrawFloppy == true)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, BitmapContextP->GLTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Draw Y inverted bitmap
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Rotate quad texture
    glTranslatef(mSpinningFloppyRotateLeft, mSpinningFloppyRotateTop, 0);
    glRotatef(mGLFloppyRotationAngle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-mSpinningFloppyRotateLeft, -mSpinningFloppyRotateTop, 0);

    glColor4f(1.0f, 1.0f, 1.0f, mGLFloppyTransparency);

    // Display bitmap
    glBegin(GL_QUADS);
      glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
      glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenTop);
      glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
      glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenBottom);
      glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
      glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenBottom);
      glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
      glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenTop);
    glEnd();

    glPopMatrix();
    
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    // Update angle for next display
    if (bPause == false)
    {
      mGLFloppyRotationAngle += FLOPPY_ROTATION_STEP;
      if (mGLFloppyRotationAngle > 360.0f)
      {
        mGLFloppyRotationAngle -= 360.0f;
      }
    }
  }

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawStaticFloppyGL(bool bActive, bool bPause)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[StaticFloppyId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_OPENGL

  bool DrawFloppy = false;

#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (bActive == true)
#else /* FORCE_FLOPPY_BITMAP_DRAWING */
  if (1)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    mGLFloppyTransparency = 1.0f;
    DrawFloppy = true;
  }
  else if (mGLFloppyTransparency > 0)
  {
    mGLFloppyTransparency -= FLOPPY_ALPHASTEP_GL;
    if (mGLFloppyTransparency > 0)
    {
      DrawFloppy = true;
    }
  }

  // Floppy image to draw ?
#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (DrawFloppy == true)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, BitmapContextP->GLTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, mGLFloppyTransparency);

    // Display bitmap
    glBegin(GL_QUADS);
      glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
      glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenTop);
      glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
      glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenBottom);
      glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
      glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenBottom);
      glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
      glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenTop);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
  }

#endif /* _USE_OPENGL */

	NOT_USED(bPause);
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawStaticFloppyGDI(bool bActive, bool bPause)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[StaticFloppyId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifndef FORCE_FLOPPY_BITMAP_DRAWING
  if (bActive == true)
#endif /* FORCE_FLOPPY_BITMAP_DRAWING */
  {
    int RasterCaps = GetDeviceCaps(mOnscreenHDC, RASTERCAPS);

    if ((RasterCaps & RC_STRETCHBLT) == RC_STRETCHBLT)
    {
      HDC hdcMask = CreateCompatibleDC(mOnscreenHDC);

      int OldMode = SetStretchBltMode(mOnscreenHDC, COLORONCOLOR);

      // Mask bitmap shape with black color.
      SelectObject(hdcMask, BitmapContextP->GDIMaskHandle);
      StretchBlt(mOnscreenHDC,
                 BitmapContextP->ScreenLeft,
                 BitmapContextP->ScreenTop,
                 BitmapContextP->ScreenWidth,
                 BitmapContextP->ScreenHeight,
                 hdcMask,
                 0,
                 0,
                 BitmapContextP->Width,
                 BitmapContextP->Height,
                 SRCAND);
      // Copy bitmap where black pixel keep other image pixel (OR operation).
      SelectObject(hdcMask, BitmapContextP->GDIHandle);
      StretchBlt(mOnscreenHDC,
                 BitmapContextP->ScreenLeft,
                 BitmapContextP->ScreenTop,
                 BitmapContextP->ScreenWidth,
                 BitmapContextP->ScreenHeight,
                 hdcMask,
                 0,
                 0,
                 BitmapContextP->Width,
                 BitmapContextP->Height,
                 SRCPAINT);

      SetStretchBltMode(mOnscreenHDC, OldMode);

      DeleteDC(hdcMask);
    }
	}

	NOT_USED(bPause);
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawTapeD3D(int PlayDuration,
                                             int TotalDuration)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[TapeId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_DIRECTX

  // Save context
  DWORD oldMinFilter;
  DWORD oldMagFilter;
	mD3DDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &oldMinFilter);
  mD3DDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &oldMagFilter);

  // Set transparency
  DWORD AlphaValue;
  AlphaValue = D3DCOLOR_ARGB(255,255,255,255);
  mD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_CONSTANT,  AlphaValue);
  mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);

  // Draw tape texture
	mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  mD3DDevice->SetTexture(0, BitmapContextP->D3DTexture);
  mD3DDevice->SetStreamSource(0, BitmapContextP->D3DVertexBuffer, 0, sizeof(D3DBitmapVertex) );
  mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  mD3DDevice->SetFVF(D3DFVF_BITMAPVERTEX);
	mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  
  // Restore context
	mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, oldMinFilter);
  mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, oldMagFilter);
  mD3DDevice->SetTexture(0, NULL);

  //
  // Show time elapsed
  //
  String Text = String::Format("%.2d:%.2d",
                               ARRAYOFCONST(((PlayDuration/60), (PlayDuration%60))));
  RECT textRect;
  SetRect(&textRect,
          EmulatorScreenLeft + TAPE_DURATION_TEXT_POS_X + 1,
          EmulatorScreenTop + TAPE_DURATION_TEXT_POS_Y + 1,
          mEmulatorScreenRight,
          mEmulatorScreenBottom);

  // Text shadow in black
  mD3DEmulatorMessageFont->DrawText(NULL,
                                    Text.c_str(),
                                    -1,
                                    &textRect,
                                    DT_SINGLELINE,
                                    D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
  // Text in plain white
  textRect.left--;
  textRect.top--;
  mD3DEmulatorMessageFont->DrawText(NULL,
                                    Text.c_str(),
                                    -1,
                                    &textRect,
                                    DT_SINGLELINE,
                                    D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

  //
  // Show total duration
  //
  Text = String::Format("%.2d:%.2d",
                        ARRAYOFCONST(((TotalDuration/60), (TotalDuration%60))));
  SetRect(&textRect,
          EmulatorScreenLeft + TAPE_DURATION_TEXT_POS_X + 1,
          EmulatorScreenTop + TAPE_TOTAL_DURATION_TEXT_POS_Y + 1,
          mEmulatorScreenRight,
          mEmulatorScreenBottom);

  // Text shadow in black
  mD3DEmulatorMessageFont->DrawText(NULL,
                                    Text.c_str(),
                                    -1,
                                    &textRect,
                                    DT_SINGLELINE,
                                    D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
  // Text in plain red
  textRect.left--;
  textRect.top--;
  mD3DEmulatorMessageFont->DrawText(NULL,
                                    Text.c_str(),
                                    -1,
                                    &textRect,
                                    DT_SINGLELINE,
                                    D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

  mLastError = ::GetLastError();

#else /* _USE_DIRECTX */

  mLastError = E_FAIL;

  NOT_USED(PlayDuration);
  NOT_USED(TotalDuration);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawTapeGL(int PlayDuration,
                                                int TotalDuration)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[TapeId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_OPENGL

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, BitmapContextP->GLTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Draw Y inverted bitmap
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

  // Display bitmap
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
    glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenTop);
    glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
    glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenBottom);
    glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
    glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenBottom);
    glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
    glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenTop);
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  // Store context
  glPushAttrib(GL_CURRENT_BIT);
  // Push the list base value
  glPushAttrib(GL_LIST_BIT);
  // Set a new list base value.
  glListBase(mGLEmulatorMessageFontBase - 32);

  //
  // Show time elapsed
  //
  AnsiString Text = AnsiString::Format("%.2d:%.2d",
                                       ARRAYOFCONST(((PlayDuration/60), (PlayDuration%60))));
  int TextLength = Text.Length();
  char* TextChar = Text.c_str();
  // Text in black shadow
  int Left = EmulatorScreenLeft + TAPE_DURATION_TEXT_POS_X;
  int Top = EmulatorScreenTop + TAPE_DURATION_TEXT_POS_Y + TAPE_POS_Y_OFFSET_GL;
  glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
  glRasterPos2i(Left + 1, Top + 1);
  glCallLists((GLsizei)TextLength,
              GL_UNSIGNED_BYTE,
              (GLvoid *)TextChar);
  // Text in plain white
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glRasterPos2i(Left, Top);
  glCallLists((GLsizei)TextLength,
              GL_UNSIGNED_BYTE,
              (GLvoid *)TextChar);

  //
  // Show total duration
  //
  Text = AnsiString::Format("%.2d:%.2d",
                            ARRAYOFCONST(((TotalDuration/60), (TotalDuration%60))));
  TextLength = Text.Length();
  TextChar = Text.c_str();
  // Text in black shadow
  Top = EmulatorScreenTop + TAPE_TOTAL_DURATION_TEXT_POS_Y + TAPE_POS_Y_OFFSET_GL;
  glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
  glRasterPos2i(Left + 1, Top + 1);
  glCallLists((GLsizei)TextLength,
              GL_UNSIGNED_BYTE,
              (GLvoid *)TextChar);
  // Text in plain red
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  glRasterPos2i(Left, Top);
  glCallLists((GLsizei)TextLength,
              GL_UNSIGNED_BYTE,
              (GLvoid *)TextChar);

  // Restore list
  glPopAttrib();
  glPopAttrib();

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawTapeGDI(int PlayDuration,
                                             int TotalDuration)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[TapeId];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

  //
  // Draw Bitmap
  //
  HDC hdcMask = CreateCompatibleDC(mOnscreenHDC);

  // Mask bitmap shape with black color.
  HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMask, BitmapContextP->GDIMaskHandle);
  StretchBlt(mOnscreenHDC,
             BitmapContextP->ScreenLeft,
             BitmapContextP->ScreenTop,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             hdcMask,
             0,
             0,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             SRCAND);
  // Copy bitmap where black pixel keep other image pixel (OR operation).
  SelectObject(hdcMask, BitmapContextP->GDIHandle);
  StretchBlt(mOnscreenHDC,
             BitmapContextP->ScreenLeft,
             BitmapContextP->ScreenTop,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             hdcMask,
             0,
             0,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             SRCPAINT);

  SelectObject(hdcMask, oldBitmap);
  DeleteDC(hdcMask);

  HFONT oldFont = (HFONT)SelectObject(mOnscreenHDC, mEmulatorMessageFont);
  SetBkMode(mOnscreenHDC, TRANSPARENT);

  //
  // Show play duration
	//
	String Text = String::Format("%.2d:%.2d",
															 ARRAYOFCONST(((PlayDuration/60), (PlayDuration%60))));
	int Left = EmulatorScreenLeft + TAPE_DURATION_TEXT_POS_X;
  int Top = EmulatorScreenTop + TAPE_DURATION_TEXT_POS_Y;
  SetTextColor(mOnscreenHDC, RGB(0, 0, 0)); // Black for shadow
  TextOut(mOnscreenHDC, Left+1, Top+1, Text.c_str(), Text.Length());
  SetTextColor(mOnscreenHDC, RGB(255, 255, 255)); // Text in white
  TextOut(mOnscreenHDC, Left, Top, Text.c_str(), Text.Length());

  //
  // Show total duration
  //
  Text = AnsiString::Format("%.2d:%.2d",
                            ARRAYOFCONST(((TotalDuration/60), (TotalDuration%60))));
  Top = EmulatorScreenTop + TAPE_TOTAL_DURATION_TEXT_POS_Y;
  SetTextColor(mOnscreenHDC, RGB(0, 0, 0)); // Black for shadow
  TextOut(mOnscreenHDC, Left+1, Top+1, Text.c_str(), Text.Length());
  SetTextColor(mOnscreenHDC, RGB(255, 0, 0)); // Text in red
  TextOut(mOnscreenHDC, Left, Top, Text.c_str(), Text.Length());

  SelectObject(mOnscreenHDC, oldFont);
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawEmulatorMessagesD3D(TList* MessageList)
{
#ifdef _USE_DIRECTX

  // Display all messages
  int DisplayBaseX = mEmulatorScreenLeft + EMULATORMESSAGE_OFFSETX;
  int DisplayBaseY = mEmulatorScreenBottom - EMULATORMESSAGE_POSSTEP;
  for (int Index=0; Index < MessageList->Count; Index++)
  {
    TEmulatorMessage* MessageP = (TEmulatorMessage*)MessageList->Items[Index];

    RECT textRect;
    SetRect(&textRect,
            DisplayBaseX+1,
            DisplayBaseY+1,
            mEmulatorScreenRight,
            mEmulatorScreenBottom);
    // Next message top
    DisplayBaseY -= EMULATORMESSAGE_POSSTEP;

    // Text shadow in black using alpha
    mD3DEmulatorMessageFont->DrawText(NULL,
                                      String(MessageP->Message).c_str(),
                                      -1,
                                      &textRect,
                                      DT_SINGLELINE,
                                      D3DXCOLOR(0.0f, 0.0f, 0.0f, MessageP->Transparency));

    // Text in white using alpha
    textRect.left--;
    textRect.top--;
    mD3DEmulatorMessageFont->DrawText(NULL,
                                      String(MessageP->Message).c_str(),
                                      -1,
                                      &textRect,
                                      DT_SINGLELINE,
                                      D3DXCOLOR(1.0f, 1.0f, 1.0f, MessageP->Transparency));
  }

  mLastError = ::GetLastError();

#else /* _USE_DIRECTX */

  mLastError = E_FAIL;

  NOT_USED(MessageList);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawEmulatorMessagesGL(TList* MessageList)
{
#ifdef _USE_OPENGL

  // Store context
  glPushAttrib(GL_CURRENT_BIT);

  // Text area clipping
  glEnable(GL_SCISSOR_TEST);
  glScissor(mEmulatorScreenLeft,
            mEmulatorScreenTop,
            mEmulatorScreenRight - mEmulatorScreenLeft,
            mEmulatorScreenBottom - mEmulatorScreenTop);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Push the list base value
  glPushAttrib(GL_LIST_BIT);
  // Set a new list base value.
  glListBase(mGLEmulatorMessageFontBase - 32);

  // Display all messages
  int DisplayBaseX = mEmulatorScreenLeft + EMULATORMESSAGE_OFFSETX;
  int DisplayBaseY = mEmulatorScreenBottom - EMULATORMESSAGE_OFFSETY;
  for (int Index=0; Index < MessageList->Count; Index++)
  {
    TEmulatorMessage* MessageP = (TEmulatorMessage*)MessageList->Items[Index];
    char* MessageString = AnsiString(MessageP->Message).c_str();
    int MessageLength = MessageP->Message.Length();

    // Text shadow in black using alpha
    glColor4f(0.0f, 0.0f, 0.0f, MessageP->Transparency);

    glRasterPos2i(DisplayBaseX+1, DisplayBaseY+1);

    // Call the lists to draw the text.
    glCallLists((GLsizei)MessageLength,
                GL_UNSIGNED_BYTE,
                (GLvoid *)MessageString);

    // Text in white using alpha
    glColor4f(1.0f, 1.0f, 1.0f, MessageP->Transparency);

    glRasterPos2i(DisplayBaseX, DisplayBaseY);
    DisplayBaseY -= EMULATORMESSAGE_POSSTEP;

    // Call the lists to draw the text.
    glCallLists((GLsizei)MessageLength,
                GL_UNSIGNED_BYTE,
                (GLvoid *)MessageString);
  }

  glDisable(GL_SCISSOR_TEST);

  // Restore list
  glPopAttrib();

  // Restore context
  glDisable(GL_BLEND);

  glPopAttrib();

#else /* _USE_OPENGL */

  mLastError = E_FAIL;

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawEmulatorMessagesGDI(TList* MessageList)
{
  // Get current clipping area
  HRGN restoreRegion = CreateRectRgn( 0, 0, 0, 0 );
  if (GetClipRgn(mOnscreenHDC, restoreRegion) != 1)
  {
    DeleteObject(restoreRegion);
    restoreRegion = NULL;
  }
  // Set clipping area
  HRGN ClipRegion = CreateRectRgn(mEmulatorScreenLeft,
                                  mEmulatorScreenTop,
                                  mEmulatorScreenRight,
                                  mEmulatorScreenBottom);
  SelectClipRgn(mOnscreenHDC, ClipRegion);

  int DisplayBaseX = mEmulatorScreenLeft + EMULATORMESSAGE_OFFSETX;
  int DisplayBaseY = mEmulatorScreenBottom - EMULATORMESSAGE_POSSTEP;
  HFONT oldFont = (HFONT)SelectObject(mOnscreenHDC, mEmulatorMessageFont);
  SetBkMode(mOnscreenHDC, TRANSPARENT);

  // Display all messages
  for (int Index=0; Index < MessageList->Count; Index++)
  {
    TEmulatorMessage* MessageP = (TEmulatorMessage*)MessageList->Items[Index];

    // Draw text shadow
    SetTextColor(mOnscreenHDC, RGB(0, 0, 0));
    TextOut(mOnscreenHDC,
            DisplayBaseX+1,
            DisplayBaseY+1,
            MessageP->Message.c_str(),
            MessageP->Message.Length());
    // Draw text
    SetTextColor(mOnscreenHDC, RGB(255, 255, 255));
    TextOut(mOnscreenHDC,
            DisplayBaseX,
            DisplayBaseY,
            MessageP->Message.c_str(),
            MessageP->Message.Length());
    DisplayBaseY -= EMULATORMESSAGE_POSSTEP;
  }

  SelectObject(mOnscreenHDC, oldFont);

  // Restore previous clipping area
  SelectClipRgn(mOnscreenHDC, restoreRegion);
  if (restoreRegion != NULL)
  {
    DeleteObject(restoreRegion);
  }
  DeleteObject(ClipRegion);
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawEmulatorTextD3D(String Text, int X, int Y)
{
#ifdef _USE_DIRECTX

  RECT textRect;
  SetRect(&textRect, X, Y, 0, 0);

	mD3DEmulatorMessageFont->DrawText(NULL,
                                    Text.c_str(),
                                    -1,
                                    &textRect,
                                    DT_NOCLIP,
		                                D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

  mLastError = ::GetLastError();

#else /* _USE_DIRECTX */

  mLastError = E_FAIL;

  NOT_USED(Text);
  NOT_USED(X);
  NOT_USED(Y);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawEmulatorTextGL(String Text, int X, int Y)
{
#ifdef _USE_OPENGL

  // Store context
  glPushAttrib(GL_CURRENT_BIT);

  // Push the list base value
  glPushAttrib(GL_LIST_BIT);
  // Set a new list base value.
  glListBase(mGLEmulatorMessageFontBase - 32);

  // Text in plain red in top-left corner
  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
  glRasterPos2i(X,
                Y+EMULATORMESSAGE_POSSTEP);

  // Call the lists to draw the text.
  glCallLists((GLsizei)Text.Length(),
              GL_UNSIGNED_BYTE,
              (GLvoid *)AnsiString(Text).c_str());

  // Restore list
  glPopAttrib();
  glPopAttrib();

#else /* _USE_OPENGL */

  mLastError = E_FAIL;

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------

void __fastcall TDrawingContext::DrawEmulatorTextGDI(String Text, int X, int Y)
{
  HFONT oldFont = (HFONT)SelectObject(mOnscreenHDC, mEmulatorMessageFont);
  SetBkMode(mOnscreenHDC, TRANSPARENT);

  // Draw text
  SetTextColor(mOnscreenHDC, RGB(255, 0, 0));
  TextOut(mOnscreenHDC,
          X,
          Y,
          Text.c_str(),
          Text.Length());

  SelectObject(mOnscreenHDC, oldFont);
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawOnscreenIconsBeginD3D(void)
{
#ifdef _USE_DIRECTX

  // Save context
	mD3DDevice->GetSamplerState(0, D3DSAMP_MINFILTER, &mOldMinFilter);
  mD3DDevice->GetSamplerState(0, D3DSAMP_MAGFILTER, &mOldMagFilter);

  // Set transparency
  DWORD AlphaValue;
  AlphaValue = D3DCOLOR_ARGB(204,204,204,204); // Slighly transparent bitmap
  mD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  mD3DDevice->SetTextureStageState(0, D3DTSS_CONSTANT,  AlphaValue);
  mD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CONSTANT);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawOnscreenIconsBeginGL(void)
{
#ifdef _USE_OPENGL
#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawOnscreenIconsBeginGDI(void)
{
  // Nothing to do
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawOnscreenIconsFinalizeD3D(void)
{
#ifdef _USE_DIRECTX

  // Restore context
	mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, mOldMinFilter);
  mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, mOldMagFilter);
  mD3DDevice->SetTexture(0, NULL);

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawOnscreenIconsFinalizeGL(void)
{
#ifdef _USE_OPENGL
#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawOnscreenIconsFinalizeGDI(void)
{
  // Nothing to do
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawOnscreenIconD3D(tDrawingBitmapId Icon)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[Icon];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_DIRECTX

  // Draw icon texture
	mD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  mD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  mD3DDevice->SetTexture(0, BitmapContextP->D3DTexture);
  mD3DDevice->SetStreamSource(0, BitmapContextP->D3DVertexBuffer, 0, sizeof(D3DBitmapVertex) );
  mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  mD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  mD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  mD3DDevice->SetFVF(D3DFVF_BITMAPVERTEX);
	mD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  mD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

#else /* _USE_DIRECTX */

  mLastError = E_FAIL;

#endif /* _USE_DIRECTX */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawOnscreenIconGL(tDrawingBitmapId Icon)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[Icon];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

#ifdef _USE_OPENGL

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, BitmapContextP->GLTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Draw Y inverted bitmap
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.9f, 0.9f, 0.9f, 0.9f); // Slighly transparent bitmap

  // Display bitmap
  glBegin(GL_QUADS);
    glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
    glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenTop);
    glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
    glVertex2i(BitmapContextP->ScreenLeft, BitmapContextP->ScreenBottom);
    glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
    glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenBottom);
    glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
    glVertex2i(BitmapContextP->ScreenRight, BitmapContextP->ScreenTop);
  glEnd();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

#endif /* _USE_OPENGL */
}
//---------------------------------------------------------------------------
void __fastcall TDrawingContext::DrawOnscreenIconGDI(tDrawingBitmapId Icon)
{
tDrawingBitmapContext* BitmapContextP = &mBitmapContext[Icon];

#ifdef _DEBUG
  if (!BitmapContextP->ScreenWidth)
    return;
#endif /* _DEBUG */

  HDC hdcMask = CreateCompatibleDC(mOnscreenHDC);

  // Mark bitmap shape with black color.
  HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMask, BitmapContextP->GDIMaskHandle);
  StretchBlt(mOnscreenHDC,
             BitmapContextP->ScreenLeft,
             BitmapContextP->ScreenTop,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             hdcMask,
             0,
             0,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             SRCAND);

  // Copy bitmap where black pixel keep other image pixel (OR operation).
  SelectObject(hdcMask, BitmapContextP->GDIHandle);
  StretchBlt(mOnscreenHDC,
             BitmapContextP->ScreenLeft,
             BitmapContextP->ScreenTop,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             hdcMask,
             0,
             0,
             BitmapContextP->ScreenWidth,
             BitmapContextP->ScreenHeight,
             SRCPAINT);

  SelectObject(hdcMask, oldBitmap);
  DeleteDC(hdcMask);
}
//---------------------------------------------------------------------------


void __fastcall TDrawingContext::DrawVDUBeamPosition(int EmuScreenX, int EmuScreenY)
{
  if (mEngineGraphicFormatP == NULL)
    return;

  int BeamX = mEmulatorScreenWidth * EmuScreenX / mEngineGraphicFormatP->VisibleScreenWidth;
  int BeamY = mEmulatorScreenHeight * EmuScreenY / mEngineGraphicFormatP->VisibleScreenHeight;

#ifdef _USE_DIRECTX

  //
  // DirectX
  //
  if (mUseDirectX == true)
  {
    D3DXVECTOR2 VertLine[] =
    {
      D3DXVECTOR2(BeamX, mEmulatorScreenTop),
      D3DXVECTOR2(BeamX, mEmulatorScreenBottom),
    };
    D3DXVECTOR2 HorzLine[] =
    {
      D3DXVECTOR2(mEmulatorScreenLeft, BeamY),
      D3DXVECTOR2(mEmulatorScreenRight, BeamY),
    };
    DWORD oldPattern = mD3DLine->GetPattern();
    D3DCOLOR BeamColor = D3DCOLOR_ARGB(255, 255, 255, 255);
    mD3DLine->SetPattern(0x55555555);
    mD3DLine->SetPatternScale(2.0f);
    mD3DLine->Begin();
    mD3DLine->Draw(VertLine, 2, BeamColor);
    mD3DLine->Draw(HorzLine, 2, BeamColor);
    mD3DLine->End();
    mD3DLine->SetPattern(oldPattern);
  }
  else

#endif /* _USE_DIRECTX */

#ifdef _USE_OPENGL

  //
  // OpenGL
  //
  if (mUseOpenGL == true)
  {
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glLineWidth(1.0f);
    glLineStipple(2, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
	  glBegin(GL_LINES);
	    glVertex2i(BeamX, mEmulatorScreenTop);
	    glVertex2i(BeamX, mEmulatorScreenBottom);
	    glVertex2i(mEmulatorScreenLeft, BeamY);
	    glVertex2i(mEmulatorScreenRight, BeamY);
	  glEnd();
    glDisable(GL_LINE_STIPPLE);
  }
  else

#endif /* _USE_OPENGL */

  //
  // GDI
  //
  {
    HPEN LineHPen = CreatePen(PS_DOT, 1, clWhite);
    HPEN oldPen = (HPEN)SelectObject(mOnscreenHDC, LineHPen);

    MoveToEx(mOnscreenHDC, BeamX, mEmulatorScreenTop, NULL);
    LineTo(mOnscreenHDC, BeamX, mEmulatorScreenBottom);
    MoveToEx(mOnscreenHDC, mEmulatorScreenLeft, BeamY, NULL);
    LineTo(mOnscreenHDC, mEmulatorScreenRight, BeamY);

    SelectObject(mOnscreenHDC, oldPen);
    DeleteObject(LineHPen);
  }
}
//---------------------------------------------------------------------------

