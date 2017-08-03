#include "UserInput.h"


UserInput::UserInput()
{

	for (int i = 0; i < 256;i++)
	{
		m_FunctionKeys[i].IsPressed = false;
		m_CharacterKeys[i].IsPressed = false;
	}

	m_screenWidth = 0;
	m_screenHeight = 0;

	m_mouseX = 0;
	m_mouseY = 0;

	m_lastMouseX = 0;
	m_lastMouseY = 0;
}

UserInput::UserInput(const UserInput& other)
{
}

UserInput::~UserInput()
{

}

void UserInput::SetWindowWidthAndHeigth(int screenWidth, int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void UserInput::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;

	return;
}

void UserInput::GetMouseDeltaXY(int& deltaX, int& deltaY)
{
	float mouseXDelta = m_mouseX - m_lastMouseX;
	float mouseYDelta = m_mouseY - m_lastMouseY;

	deltaX = mouseXDelta;
	deltaY = mouseYDelta;

	return;
}

void UserInput::SetMouseLocation(int mouseX, int mouseY)
{
	m_lastMouseX = m_mouseX;
	m_lastMouseY = m_mouseY;
	m_mouseX = mouseX;
	m_mouseY = mouseY;
	return;
}

void UserInput::FunctionKeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_FunctionKeys[input].IsPressed = true;
	return;
}

void UserInput::FunctionKeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_FunctionKeys[input].IsPressed = false;
	return;
}

bool UserInput::IsFunctionKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_FunctionKeys[key].IsPressed;
}

void UserInput::CharacterKeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_CharacterKeys[input].IsPressed = true;
	return;
}

void UserInput::CharacterKeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_CharacterKeys[input].IsPressed = false;
	return;
}

bool UserInput::IsCharacterKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_CharacterKeys[key].IsPressed;
}

void UserInput::ResetCharacterKeys()
{
	for (int i = 0; i < 256; i++)
	{
		m_CharacterKeys[i].IsPressed = false;
	}
}

void UserInput::ResetFunctionKeys()
{
	for (int i = 0; i < 256; i++)
	{
		m_FunctionKeys[i].IsPressed = false;
	}
}
