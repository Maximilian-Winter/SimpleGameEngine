#include "UserInterface.h"


UserInterface::UserInterface()
:m_Fps(),
m_Cpu(),
m_Timer(),
m_MouseCursor(),
m_UIElement(),
m_mouseX(0),
m_mouseY(0),
m_ApplicationIsPaused(0),
m_FrameCounter(0)
{
}

UserInterface::UserInterface(const UserInterface& other)
{
}

UserInterface::~UserInterface()
{
}

bool UserInterface::Initialize(D3D* m_D3D, HWND hwnd, HINSTANCE hinstance, int screenWidth, int screenHeight, DirectX::CXMMATRIX baseViewMatrix, UserInput* UserInput, StateManager* StateManager)
{
	bool result;

	m_StateManager = StateManager;

	// Get the Input Data Object by Reference.
	m_UserInput = UserInput;

	// Initialize the UI Element Shader Object.
	result = m_UIElement.Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the UI shader object.", L"Error", MB_OK);
		return false;
	}

	m_Timer.Initialize();
	// Initialize the fps object.
	m_Fps.Initialize();

	// Initialize the cpu object.
	m_Cpu.Initialize();

	// Initialize the timer object.
	result = m_Timer.Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	result = m_MouseCursor.Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"data/mousecursor.dds", 30, 30);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the mousecursor object.", L"Error", MB_OK);
		return false;
	}

	DirectX::XMStoreFloat4x4(&m_BaseViewMatrix, baseViewMatrix);

	m_mouseXDelta = 0;
	m_mouseYDelta = 0;
	m_mouseX = 0;
	m_mouseY = 0;

	return result;
}

void UserInterface::Shutdown()
{
	m_UIElement.Shutdown();
	m_MouseCursor.Shutdown();
	m_Cpu.Shutdown();
	
	return;
}

bool UserInterface::Frame()
{
	bool result;

	m_Timer.Frame();
	m_Fps.Frame();
	m_Cpu.Frame();

	// Process Input .
	result = ProcessInput();
	if (!result)
	{
		return false;
	}

	result = PrepareDebuginfo();
	if (!result)
	{
		return false;
	}

	m_FrameCounter++;

	if (m_FrameCounter > 500000)
	{
		m_FrameCounter = 0;
	}
	return true;
}

bool UserInterface::RenderUI(ID2D1RenderTarget* D2DRenderTarget)
{
	bool result;
	
	//Create write factory
	IDWriteFactory *writeFactory;
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)(&writeFactory));
	if (FAILED(result))
	{
		return false;
	}

	D2DRenderTarget->BeginDraw();

	for (int i = 0; i < UI_TextStrings.size(); i++)
	{
		std::wstring FontName = StringToWString(UI_TextStrings[i].FontName);
		std::wstring Text = StringToWString(UI_TextStrings[i].Text);

		IDWriteTextFormat *TextFormat;
		result = writeFactory->CreateTextFormat(
			FontName.c_str(),
			NULL,
			UI_TextStrings[i].FontWeight,
			UI_TextStrings[i].FontStyle,
			UI_TextStrings[i].FontStretch,
			UI_TextStrings[i].FontSize,
			L"",
			&TextFormat);
		if (FAILED(result))
		{
			return false;
		}

		// create a brush
		ID2D1SolidColorBrush *ColoredBrush;
		result = D2DRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF(UI_TextStrings[i].ColorR, UI_TextStrings[i].ColorG, UI_TextStrings[i].ColorB, 1.0f)),
			&ColoredBrush
			);;
		if (FAILED(result))
		{
			return false;
		}

		//Render text strings to display
		D2DRenderTarget->DrawText(Text.c_str(), wcslen(Text.c_str()), TextFormat, UI_TextStrings[i].Rect.UI_LayoutRect, ColoredBrush);

		TextFormat->Release();
		TextFormat = 0;
		ColoredBrush->Release();
		ColoredBrush = 0;
	}

	for (int i = 0; i < UI_TemporaryTextStrings.size(); i++)
	{
		std::wstring FontName = StringToWString(UI_TemporaryTextStrings[i].FontName);
		std::wstring Text = StringToWString(UI_TemporaryTextStrings[i].Text);

		IDWriteTextFormat *TextFormat;
		result = writeFactory->CreateTextFormat(
			FontName.c_str(),
			NULL,
			UI_TemporaryTextStrings[i].FontWeight,
			UI_TemporaryTextStrings[i].FontStyle,
			UI_TemporaryTextStrings[i].FontStretch,
			UI_TemporaryTextStrings[i].FontSize,
			L"",
			&TextFormat);
		if (FAILED(result))
		{
			return false;
		}

		// create a brush
		ID2D1SolidColorBrush *ColoredBrush;
		result = D2DRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF(UI_TemporaryTextStrings[i].ColorR, UI_TemporaryTextStrings[i].ColorG, UI_TemporaryTextStrings[i].ColorB, 1.0f)),
			&ColoredBrush
			);;
		if (FAILED(result))
		{
			return false;
		}

		//Render text strings to display
		D2DRenderTarget->DrawText(Text.c_str(), wcslen(Text.c_str()), TextFormat, UI_TemporaryTextStrings[i].Rect.UI_LayoutRect, ColoredBrush);
		
		TextFormat->Release();
		TextFormat = 0;
		ColoredBrush->Release();
		ColoredBrush = 0;
	}

	for (int i = 0; i < UI_TemporaryLines.size(); i++)
	{
		// Create a brush with the given color
		ID2D1SolidColorBrush *ColoredBrush;
		result = D2DRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF(UI_TemporaryLines[i].ColorR, UI_TemporaryLines[i].ColorG, UI_TemporaryLines[i].ColorB, 1.0f)),
			&ColoredBrush
			);;
		if (FAILED(result))
		{
			return false;
		}

		// Render lines.
		D2DRenderTarget->DrawLine(UI_TemporaryLines[i].point0, UI_TemporaryLines[i].point1, ColoredBrush);

		ColoredBrush->Release();
		ColoredBrush = 0;
	}

	D2DRenderTarget->EndDraw();
	UI_TemporaryLines.clear();
	UI_TemporaryTextStrings.clear();

	writeFactory->Release();
	writeFactory = 0;

	return true;
}

bool UserInterface::PrepareDebuginfo()
{
	if (((m_FrameCounter % 100) == 0))
	{
		//Get frames per second
		m_LastFrameDuration = m_Timer.GetTime();
	}

	//Create fps string
	std::string framedurationString = "Frameduration: ";
	std::string tempString = std::to_string(m_LastFrameDuration);
	framedurationString += tempString;
	AddTemporaryTextString(framedurationString, "Seogoe UI",20.0f, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, 10, 200, 40);

	//Create fps string
	std::string fpsString = "Frames per seconds: ";
	if (m_LastFrameDuration == 0)
	{
		tempString = "0";
	}
	else
	{
		tempString = std::to_string(m_Fps.GetFps());
	}
	fpsString += tempString;
	AddTemporaryTextString(fpsString, "Seogoe UI", 20.0f, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, 40, 350, 40);

	//Create cpu string
	std::string cpuString = "CPU: ";
	tempString = std::to_string(m_Cpu.GetCpuPercentage());
	cpuString += tempString;
	cpuString += " %";
	AddTemporaryTextString(cpuString, "Seogoe UI", 20.0f, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, 70, 200, 40);

	//Create mouseX string
	std::string MouseX = "MouseX: ";
	tempString = std::to_string(m_mouseX);
	MouseX += tempString;
	AddTemporaryTextString(MouseX, "Seogoe UI", 20.0f, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, 100, 200, 40);

	//Create mouseY string
	std::string MouseY = "MouseY: ";
	tempString = std::to_string(m_mouseY);
	MouseY += tempString;
	AddTemporaryTextString(MouseY, "Seogoe UI", 20.0f, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, 130, 200, 40);

	return true;
}

bool UserInterface::RenderMouseCursor(D3D* m_D3D, bool RenderMouseCursorAlways)
{
	bool result;
	DirectX::XMMATRIX worldMatrix, orthoMatrix;

	worldMatrix = m_D3D->GetWorldMatrix();
	orthoMatrix = m_D3D->GetOrthoMatrix();

	// Render the mouse cursor.
	if (RenderMouseCursorAlways == true)
	{

		// Turn on the alpha blending before rendering the text.
		m_D3D->TurnOnAlphaBlending();
		m_D3D->TurnZBufferOff();

		result = m_MouseCursor.Render(m_D3D->GetDeviceContext(), m_mouseX, m_mouseY);
		if (!result)
		{
			return false;
		}

		result = m_UIElement.Render(m_D3D->GetDeviceContext(), m_MouseCursor.GetIndexCount(), worldMatrix, DirectX::XMLoadFloat4x4(&m_BaseViewMatrix), orthoMatrix, m_MouseCursor.GetTexture());
		if (!result)
		{
			return false;
		}

		// Turn off alpha blending after rendering the text.

		m_D3D->TurnOffAlphaBlending();
		m_D3D->TurnZBufferOn();

	}

	else if (m_ApplicationIsPaused == true && RenderMouseCursorAlways == false)
	{

		// Turn on the alpha blending before rendering the text.
		m_D3D->TurnOnAlphaBlending();
		m_D3D->TurnZBufferOff();

		result = m_MouseCursor.Render(m_D3D->GetDeviceContext(), m_mouseX, m_mouseY);
		if (!result)
		{
			return false;
		}

		result = m_UIElement.Render(m_D3D->GetDeviceContext(), m_MouseCursor.GetIndexCount(), worldMatrix, DirectX::XMLoadFloat4x4(&m_BaseViewMatrix), orthoMatrix, m_MouseCursor.GetTexture());
		if (!result)
		{
			return false;
		}

		// Turn off alpha blending after rendering the text.

		m_D3D->TurnOffAlphaBlending();
		m_D3D->TurnZBufferOn();

	}
	


	return true;
}

bool UserInterface::ProcessInput()
{

	m_UserInput->GetMouseLocation(m_mouseX, m_mouseY);
	m_UserInput->GetMouseDeltaXY(m_mouseXDelta, m_mouseYDelta);

	if (m_UserInput->IsFunctionKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if (m_UserInput->IsFunctionKeyDown(VK_PAUSE))
	{
		if (m_ApplicationIsPaused == false)
		{
			m_StateManager->SetApplicationState(IsPaused, true);
			m_ApplicationIsPaused = true;
		}
		else
		{
			m_StateManager->SetApplicationState(IsPaused, false);
			m_ApplicationIsPaused = false;
		}
	}

	return true;
}

void UserInterface::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;

	return;
}

void UserInterface::GetMouseDeltaXY(int& deltaX, int& deltaY)
{
	deltaX = m_mouseXDelta;
	deltaY = m_mouseYDelta;

	return;
}

void UserInterface::GetApplicationIsPausedState(bool& ApplicationIsPausedState)
{
	ApplicationIsPausedState = m_ApplicationIsPaused;

	return;
}

bool UserInterface::IsFunctionKeyDown(unsigned int key)
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_UserInput->IsFunctionKeyDown(key))
	{
		return true;
	}

	return false;
}

bool UserInterface::IsCharKeyPressed(unsigned int key)
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (m_UserInput->IsCharacterKeyDown(key))
	{
		return true;
	}

	return false;
}

void UserInterface::AddTextString(std::string Text, std::string FontName, float FontSize, float ColorR, float ColorG, float ColorB, DWRITE_FONT_WEIGHT FontWeight, DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch, float TextBoxStartX, float TextBoxStartY, float Width, float Height)
{
	UI_TextStrings.push_back(UI_Text(Text, FontName, FontSize, ColorR, ColorG, ColorB, FontWeight, FontStyle, FontStretch, TextBoxStartX, TextBoxStartY, Width, Height));
}

void UserInterface::AddTemporaryTextString(std::string Text, std::string FontName, float FontSize, float ColorR, float ColorG, float ColorB, DWRITE_FONT_WEIGHT FontWeight, DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch, float TextBoxStartX, float TextBoxStartY, float Width, float Height)
{
	UI_TemporaryTextStrings.push_back(UI_Text(Text, FontName, FontSize, ColorR, ColorG, ColorB, FontWeight, FontStyle, FontStretch, TextBoxStartX, TextBoxStartY, Width, Height));
}

void UserInterface::AddTemporaryLine(float x1, float y1, float x2, float y2, float r, float g, float b)
{
	UI_TemporaryLines.push_back(UI_Line_Element(x1, y1, x2, y2, r, g, b));
}

std::wstring UserInterface::StringToWString(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}


