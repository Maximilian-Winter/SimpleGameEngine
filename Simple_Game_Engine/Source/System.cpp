#include "System.h"


System::System()
	:m_UserInputClass(),
	m_Application(),
	m_ScreenWidth(0),
	m_ScreenHeight(0),
	m_Frames(0),
	m_ConfigFile(),
	m_StateManager()
{
}


System::System(const System& other)
{
}


System::~System()
{
}


bool System::Initialize()
{
	bool result;

	// Load the config file data
	result = m_ConfigFile.LoadConfigDataFile("main.cfg");

	if (result)
	{

		// Set the width and height of the screen to the values in the config file
		result = m_ConfigFile.GetIntValueByName("main", "s_width", m_ScreenWidth);
		if (!result)
		{
			m_ScreenWidth = 800;
		}
		result = m_ConfigFile.GetIntValueByName("main", "s_height", m_ScreenHeight);
		if (!result)
		{
			m_ScreenHeight = 600;
		}


		// Set Fullscreen and Vsync ON/OFF according to the values in the config file. 
		result = m_ConfigFile.GetBoolValueByName("main", "fullscreen", m_Fullscreen);
		if (!result)
		{
			m_Fullscreen = 0;
		}
		result = m_ConfigFile.GetBoolValueByName("main", "vsync", m_VSync);
		if (!result)
		{
			m_VSync = 0;
		}

		// Initialize the window and the windows api.
		InitWindow();

		// Set the window width and height in user input
		m_UserInputClass.SetWindowWidthAndHeigth(m_ScreenWidth, m_ScreenHeight);
		// Initialize the application object.
		result = m_Application.Initialize(m_hinstance, m_hwnd, m_ScreenWidth, m_ScreenHeight, m_Fullscreen, m_VSync, &m_UserInputClass, &m_StateManager);
		if(!result)
		{
			return false;
		}

		return true;
	}
	else
	{
		return false;
	}
}


void System::Shutdown()
{
	//Shutdown the application
	m_Application.Shutdown();
	// Shutdown the window.
	ShutdownWindow();
	
	return;
}


void System::Run()
{
	MSG msg;
	bool done, result;

	m_StateManager.SetApplicationState(IsStarted, true);

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool System::Frame()
{
	bool result;

	if (m_UserInputClass.IsFunctionKeyDown(VK_ESCAPE))
	{
		return false;
	}

	m_UserInputClass.SetMouseLocation(m_mouseX, m_mouseY);

	// Do the frame processing for the application object.
	result = m_Application.Frame();
	if(!result)
	{
		return false;
	}

	// Reset input values
	m_UserInputClass.ResetCharacterKeys();
	
	// Set the mouse cursor to the middle of the window if not paused, because of the camera movement
	/*if (!(m_StateManager.GetApplicationState(IsPaused)))
	{
		POINT currentMousePos;
		GetCursorPos(&currentMousePos);
		ScreenToClient(m_hwnd, &currentMousePos);

		POINT pt;
		pt.x = 100;
		pt.y = currentMousePos.y;

		ClientToScreen(m_hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}*/

	// Prevent variable overflow and set frame counter to 0
	if (m_Frames > 1000000)
	{
		m_Frames = 0;
	}
	else
	{
		m_Frames++;
	}
	
	return true;
}


LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{


	/*


		union LparamState
	{
		LPARAM lparam;

		struct
		{
			unsigned nRepeatCount : 16;
			unsigned nScanCode : 8;
			unsigned nExtended : 1;
			unsigned nReserved : 4;
			unsigned nContext : 1;
			unsigned nPrev : 1;
			unsigned nTrans : 1;
		};
	};


	LparamState keylpState;

	keylpState.lparam = lparam;

	keyState.nRepeatCount
	
	*/


	switch (umsg)
	{
		// Check if a character key has been pressed on the keyboard.
		case WM_CHAR:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_UserInputClass.CharacterKeyDown((unsigned int)wparam);
			return 0;
		}
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_UserInputClass.FunctionKeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_UserInputClass.FunctionKeyUp((unsigned int)wparam);
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			m_mouseX = GET_X_LPARAM(lparam);
			m_mouseY = GET_Y_LPARAM(lparam);
			// Any other messages send to the default message handler as our application won't make use of them.
			// If a key is released then send it to the input object so it can unset the state for that key.
			
			return 0;
		}
			
			
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void System::InitWindow()
{
	WNDCLASSEX WindowScreen;
	DEVMODE ScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"SGE Simple Game Engine";

	// Setup the windows class with default settings.
	WindowScreen.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	WindowScreen.lpfnWndProc = WndProc;
	WindowScreen.cbClsExtra = 0;
	WindowScreen.cbWndExtra = 0;
	WindowScreen.hInstance = m_hinstance;
	WindowScreen.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	WindowScreen.hIconSm = WindowScreen.hIcon;
	WindowScreen.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowScreen.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WindowScreen.lpszMenuName = NULL;
	WindowScreen.lpszClassName = m_applicationName;
	WindowScreen.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&WindowScreen);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_Fullscreen)
	{
		// If full screen set the screen to size from the config file and 32bit.
		memset(&ScreenSettings, 0, sizeof(ScreenSettings));
		ScreenSettings.dmSize = sizeof(ScreenSettings);
		ScreenSettings.dmPelsWidth = (unsigned long)m_ScreenWidth;
		ScreenSettings.dmPelsHeight = (unsigned long)m_ScreenHeight;
		ScreenSettings.dmBitsPerPel = 32;
		ScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&ScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_ScreenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_ScreenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_ScreenWidth, m_ScreenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void System::ShutdownWindow()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (m_Fullscreen)
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


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}