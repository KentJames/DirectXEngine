// DirectXTut.cpp : Defines the entry point for the console application.
//



#include "Engine/SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass System;
	bool result;



		//List ALL available display modes for the graphics card.
	std::wstringstream DisplayModes;

	DEVMODE dm;
	// initialize the DEVMODE structure
	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(dm);


	OutputDebugString(L"Monitor Resolution: \n \n");
	if (0 != EnumDisplaySettings(NULL, ENUM_REGISTRY_SETTINGS, &dm)){
		DisplayModes << L"Display Mode:" << dm.dmPelsWidth << L"," << dm.dmPelsHeight << L"\n \n";
		OutputDebugString(DisplayModes.str().c_str());
	}


	for( int iModeNum = 0; EnumDisplaySettings( NULL, iModeNum, &dm ) != 0; iModeNum++ ) {
		DisplayModes<< "Mode #" << iModeNum << " = " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << " : " << dm.dmDisplayFrequency << "Hz BPP: " << dm.dmBitsPerPel << std::endl;
		
	}
	OutputDebugString(DisplayModes.str().c_str());
	
	
	// Create the system object.
	//System = new SystemClass;
	/*if(System)
	{
		return 0;
	}*/

	// Initialize and run the system object.
	result = System.Initialize();
	if(result)
	{
		System.Run();
	}

	// Shutdown and release the system object.
	System.Shutdown();
	//delete System;
	//System = 0;

	return 0;
}

