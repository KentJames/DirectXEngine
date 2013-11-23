
#include "SystemClass.h"


SystemClass::SystemClass(){

	m_Input = 0;
	m_Graphics = 0;

}

SystemClass::SystemClass(const SystemClass& other){

}

SystemClass::~SystemClass(){

}

//Initialise graphics
bool SystemClass::Initialize() {

	int screenwidth,screenheight =0;
	bool result;

	InitializeWindows(screenwidth,screenheight);

	m_Input = new InputClass;

	if(!m_Input){
		return false;
	}

	m_Input->Initialize();

	m_Graphics = new GraphicsClass;

	if(!m_Graphics){
		return false;
	}

	result = m_Graphics->Initialize(screenwidth,screenheight,m_hwnd);

	if(!result){
		return false;
	}

	return true;

}

void SystemClass::Shutdown(){

	//Delete graphics object
	if(m_Graphics){

		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}
	//Delete input object
	if(m_Input){

		delete m_Input;
		m_Input = NULL;

	}

	ShutdownWindows();

	return;

}

void SystemClass::Run(){

	MSG msg;
	bool done, result;

	//Initialise message struct

	ZeroMemory(&msg,sizeof(MSG));

	done = false;

	while(!done){

		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT){

			done = true;
		}
		else{

			result = Frame();
			if(!result){
				done = true;
			}
		}
	}
	return;
}


bool SystemClass::Frame(){

	bool result;

	if(m_Input->IsKeyDown(VK_ESCAPE)){

		return false;

	}

	result = m_Graphics->Frame();
	if(!result)
	{
		return false;
	}

	return true;

}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd,UINT umsg,WPARAM wparam,LPARAM lparam){

	switch(umsg){

	case WM_KEYDOWN:
		{
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
	case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}
	default:
		{
			return DefWindowProc(hwnd,umsg,wparam,lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.

		DEVMODE dmsc;
		ZeroMemory(&dmsc, sizeof(dmsc));
		dmsc.dmSize = sizeof(dmsc);

		if(EnumDisplaySettings(NULL, ENUM_REGISTRY_SETTINGS, &dmsc) != 0){



		screenWidth  = dmsc.dmPelsWidth;
		screenHeight = dmsc.dmPelsHeight;

		}

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//Create rectangle abjuct to describe 
	RECT windowbounds = {0,0,1680,1050};
	AdjustWindowRect(&windowbounds, WS_OVERLAPPEDWINDOW, FALSE);

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
		WS_CLIPSIBLINGS| WS_CLIPCHILDREN | WS_POPUP | WS_CAPTION | WS_SYSMENU ,
				posX, posY, windowbounds.right - windowbounds.left, windowbounds.bottom - windowbounds.top, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(true);






	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}





LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage,WPARAM wparam,LPARAM lparam){

	switch(umessage){

		//Is window being destroyed?
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}
		default:
			{
				return ApplicationHandle->
					MessageHandler(hwnd,umessage,wparam,lparam);
			}
	}
}









