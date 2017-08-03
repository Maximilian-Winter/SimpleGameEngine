//Class that handle UI Stuff:
//- UI-Text
//- UI-Texture Elements
//

#ifndef _USERINTERFACE_H_
#define _USERINTERFACE_H_


// Includes

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <vector>

#include "Bitmap.h"
#include "D3D.h"
#include "TextureShader.h"
#include "CPUPerformance.h"
#include "FPSTimer.h"
#include "Timer.h"
#include "UserInput.h"
#include "StateManager.h"


class UserInterface
{
private:

	struct UI_Rect_Element
	{
		UI_Rect_Element()
		{
			UI_LayoutRect = D2D1::RectF(0.0f, 0.0f, 0.0f, 0.0f);
		}

		UI_Rect_Element(float x1, float y1, float x2, float y2)
		{
			UI_LayoutRect = D2D1::RectF(x1, y1, x2, y2);
		}

		void SetRectSize(float x1, float y1, float x2, float y2)
		{
			UI_LayoutRect = D2D1::RectF(x1, y1, x2, y2);
		}

		bool CheckIfPointIsInRect(float x, float y)
		{
			if (x >= UI_LayoutRect.left)
			{
				if (y >= UI_LayoutRect.top)
				{
					if (x <= UI_LayoutRect.right)
					{
						if (y <= UI_LayoutRect.bottom)
						{

						}
						else
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		D2D1_RECT_F UI_LayoutRect;
	};

	struct UI_Line_Element
	{
		UI_Line_Element()
		{
			point0.x = 0.0f;
			point0.y = 0.0f;

			point1.x = 0.0f;
			point1.y = 0.0f;

			ColorR = 0.0f;
			ColorG = 0.0f;
			ColorB = 0.0f;
		}

		UI_Line_Element(float x1, float y1, float x2, float y2, float r, float g, float b)
		{
			point0.x = x1;
			point0.y = y1;

			point1.x = x2;
			point1.y = y2;

			ColorR = r;
			ColorG = g;
			ColorB = b;
		}

		void SetRectSize(float x1, float y1, float x2, float y2)
		{
			point0.x = x1;
			point0.y = y1;

			point1.x = x2;
			point1.y = y2;
		}

		void SetColor(float r, float g, float b)
		{
			ColorR = r;
			ColorG = g;
			ColorB = b;
		}

		float ColorR, ColorG, ColorB;
		D2D1_POINT_2F point0;
		D2D1_POINT_2F point1;
	};

	struct UI_Text
	{
		UI_Text()
		{
			Text = "";
			FontWeight = DWRITE_FONT_WEIGHT_NORMAL;
			FontStyle = DWRITE_FONT_STYLE_NORMAL;
			FontStretch = DWRITE_FONT_STRETCH_NORMAL;
			FontSize = 50.0f;
			ColorR = 1.0f;
			ColorG = 1.0f;
			ColorB = 1.0f;
		}
		UI_Text(std::string text, std::string fontName, float fontSize, float colorR, float colorG, float colorB, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch, float textBoxStartX, float textBoxStartY, float width, float height)
		{
			Rect.SetRectSize(textBoxStartX, textBoxStartY, textBoxStartX + width, textBoxStartY + height);
			Text = text;
			FontName = fontName;
			FontWeight = fontWeight;
			FontStyle = fontStyle;
			FontStretch = fontStretch;
			FontSize = fontSize;
			ColorR = colorR;
			ColorG = colorG;
			ColorB = colorB;
		}

		void SetTextBoxPosAndSize(float StartX, float StartY, float width, float height)
		{
			Rect.SetRectSize(StartX, StartY, StartX + width, StartY + height);
		}

		void SetColor(float r, float g, float b)
		{
			ColorR = r;
			ColorG = g;
			ColorB = b;
		}
		
		void SetFontSettings(std::string fontName, float fontSize, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE fontStyle, DWRITE_FONT_STRETCH fontStretch)
		{
			FontName = fontName;
			FontWeight = fontWeight;
			FontStyle = fontStyle;
			FontStretch = fontStretch;
			FontSize = fontSize;
		}

		void SetText(std::string text)
		{
			Text = text;
		}

		UI_Rect_Element Rect;
		std::string Text;
		std::string FontName;
		float FontSize;
		float ColorR, ColorG, ColorB;
		DWRITE_FONT_WEIGHT FontWeight;
		DWRITE_FONT_STYLE FontStyle;
		DWRITE_FONT_STRETCH FontStretch;
	};

public:
	UserInterface();
	UserInterface(const UserInterface&);
	~UserInterface();

	bool Initialize(D3D*, HWND, HINSTANCE, int, int, DirectX::CXMMATRIX, UserInput*, StateManager*);
	void Shutdown();

	bool Frame(ID3D11DeviceContext*);
	bool RenderUI(ID2D1RenderTarget*);
	bool PrepareDebuginfo();
	bool RenderMouseCursor(D3D*, bool);
	void GetMouseLocation(int& , int&);
	void GetMouseDeltaXY(int&, int&);
	void GetApplicationIsPausedState(bool& );
	
	void AddTextString(std::string Text, std::string FontName, float FontSize, float ColorR, float ColorG, float ColorB,  DWRITE_FONT_WEIGHT FontWeight, DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch, float TextBoxStartX, float TextBoxStartY, float Width, float Height);
	void AddTemporaryTextString(std::string Text, std::string FontName, float FontSize, float ColorR, float ColorG, float ColorB, DWRITE_FONT_WEIGHT FontWeight, DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch, float TextBoxStartX, float TextBoxStartY, float Width, float Height);

	void AddTemporaryLine(float x1, float y1, float x2, float y2, float r, float g, float b);

	bool IsFunctionKeyDown(unsigned int);
	bool IsCharKeyPressed(unsigned int);

	
private:
	
	std::wstring StringToWString(const std::string& s);
	bool ProcessInput();

	TextureShader m_UIElement;	
	Bitmap m_MouseCursor;
	FPSTimer m_Fps;
	CPUPerformance m_Cpu;
	Timer m_Timer;

	std::vector<UI_Text> UI_TextStrings;
	std::vector<UI_Text> UI_TemporaryTextStrings;
	std::vector<UI_Line_Element> UI_TemporaryLines;

	UserInput* m_UserInput;
	
	StateManager* m_StateManager;

	DirectX::XMFLOAT4X4 m_BaseViewMatrix;

	int m_mouseX;
	int m_mouseY;

	int m_mouseXDelta;
	int m_mouseYDelta;

	bool m_ApplicationIsPaused;

	unsigned int m_FrameCounter;

	unsigned int m_LastFrameDuration;


};
#endif