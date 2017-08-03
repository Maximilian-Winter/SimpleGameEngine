#ifndef _USERINPUT_H_
#define _USERINPUT_H_





class UserInput
{
	struct KeyboardKey
	{
		bool IsPressed;
	};

public:

	UserInput();
	UserInput(const UserInput&);
	~UserInput();

	void SetWindowWidthAndHeigth(int, int);

	void GetMouseLocation(int&, int&);
	void GetMouseDeltaXY(int&, int&);
	void SetMouseLocation(int, int);

	void FunctionKeyDown(unsigned int);
	void FunctionKeyUp(unsigned int);

	bool IsFunctionKeyDown(unsigned int);

	void CharacterKeyDown(unsigned int);
	void CharacterKeyUp(unsigned int);

	bool IsCharacterKeyDown(unsigned int);

	void ResetCharacterKeys();

	void ResetFunctionKeys();

private:

	KeyboardKey m_FunctionKeys[256];
	KeyboardKey m_CharacterKeys[256];

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	int m_lastMouseX, m_lastMouseY;
};
#endif