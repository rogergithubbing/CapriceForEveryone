//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

//---------------------------------------------------------------------------
USEFORM("Sources\Windows\Forms_XE3\TFormZIPCatalog.cpp", FormZIPCatalog);
USEFORM("Sources\Windows\Forms_XE3\TFormKeyboardMapping.cpp", FormKeyboardMapping);
USEFORM("Sources\Windows\Forms_XE3\TFormMain.cpp", FormMain);
USEFORM("Sources\Windows\Forms_XE3\TFormPrinter.cpp", FormPrinter);
USEFORM("Sources\Windows\Forms_XE3\TFormHints.cpp", FormHints);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugMemoryEditor.cpp", FormDebugMemoryEditor);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugPSG.cpp", FormDebugPSG);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugTapeAudioWorkshop.cpp", FormDebugTapeAudioWorkshop);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugMEA8000.cpp", FormDebugMEA8000);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugger.cpp", FormDebugger);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugGraphicsEditor.cpp", FormDebugGraphicsEditor);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugGraphicsExplorer.cpp", FormGraphicsExplorer);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugTextCapture.cpp", FormDebugTextCapture);
USEFORM("Sources\Windows\Forms_XE3\TFormEditKeycode.cpp", FormEditKeycode);
USEFORM("Sources\Windows\Forms_XE3\TFormEmulatorSettings.cpp", FormEmulatorSettings);
USEFORM("Sources\Windows\Forms_XE3\TFormGameControllerSettings.cpp", FormGameControllerSettings);
USEFORM("Sources\Windows\Forms_XE3\TFormDragDropDrives.cpp", FormDragDropDrives);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugZ80Editor.cpp", FormDebugZ80Editor);
USEFORM("Sources\Windows\Forms_XE3\TFormDiskCatalog.cpp", FormDiskCatalog);
USEFORM("Sources\Windows\Forms_XE3\TFormDisplaySettings.cpp", FormDisplaySettings);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugGateArray.cpp", FormDebugGateArray);
USEFORM("Sources\Windows\Forms_XE3\TFormAbout.cpp", FormAbout);
USEFORM("Sources\Windows\Forms_XE3\TFormAudioSettings.cpp", FormAudioSettings);
USEFORM("Sources\Windows\Forms_XE3\TFormCheatScripts.cpp", FormCheatScripts);
USEFORM("Sources\Windows\Forms_XE3\TFormCustomSpeed.cpp", FormCustomSpeed);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugEditValue.cpp", FormDebugEditValue);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugFDC.cpp", FormDebugFDC);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugGameController.cpp", FormDebugGameController);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugDSKViewer.cpp", FormDebugDSKViewer);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugBASICColors.cpp", FormDebugBASICColors);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugCDTViewer.cpp", FormDebugCDTViewer);
USEFORM("Sources\Windows\Forms_XE3\TFormDebugCRTC.cpp", FormDebugCRTC);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TFormAbout), &FormAbout);
		Application->CreateForm(__classid(TFormAudioSettings), &FormAudioSettings);
		Application->CreateForm(__classid(TFormCustomSpeed), &FormCustomSpeed);
		Application->CreateForm(__classid(TFormDebugger), &FormDebugger);
		Application->CreateForm(__classid(TFormDebugZ80Editor), &FormDebugZ80Editor);
		Application->CreateForm(__classid(TFormDebugMemoryEditor), &FormDebugMemoryEditor);
		Application->CreateForm(__classid(TFormDebugBASICColors), &FormDebugBASICColors);
		Application->CreateForm(__classid(TFormDebugEditValue), &FormDebugEditValue);
		Application->CreateForm(__classid(TFormDebugGameController), &FormDebugGameController);
		Application->CreateForm(__classid(TFormDebugGateArray), &FormDebugGateArray);
		Application->CreateForm(__classid(TFormDebugTapeAudioWorkshop), &FormDebugTapeAudioWorkshop);
		Application->CreateForm(__classid(TFormDebugTextCapture), &FormDebugTextCapture);
		Application->CreateForm(__classid(TFormDiskCatalog), &FormDiskCatalog);
		Application->CreateForm(__classid(TFormDisplaySettings), &FormDisplaySettings);
		Application->CreateForm(__classid(TFormDragDropDrives), &FormDragDropDrives);
		Application->CreateForm(__classid(TFormEditKeycode), &FormEditKeycode);
		Application->CreateForm(__classid(TFormEmulatorSettings), &FormEmulatorSettings);
		Application->CreateForm(__classid(TFormHints), &FormHints);
		Application->CreateForm(__classid(TFormKeyboardMapping), &FormKeyboardMapping);
		Application->CreateForm(__classid(TFormZIPCatalog), &FormZIPCatalog);
		Application->CreateForm(__classid(TFormDebugDSKViewer), &FormDebugDSKViewer);
		Application->CreateForm(__classid(TFormDebugCDTViewer), &FormDebugCDTViewer);
		Application->CreateForm(__classid(TFormGraphicsExplorer), &FormGraphicsExplorer);
		Application->CreateForm(__classid(TFormDebugCRTC), &FormDebugCRTC);
		Application->CreateForm(__classid(TFormDebugPSG), &FormDebugPSG);
		Application->CreateForm(__classid(TFormDebugFDC), &FormDebugFDC);
		Application->CreateForm(__classid(TFormDebugMEA8000), &FormDebugMEA8000);
		Application->CreateForm(__classid(TFormPrinter), &FormPrinter);
		Application->CreateForm(__classid(TFormCheatScripts), &FormCheatScripts);
		Application->CreateForm(__classid(TFormGameControllerSettings), &FormGameControllerSettings);

		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
