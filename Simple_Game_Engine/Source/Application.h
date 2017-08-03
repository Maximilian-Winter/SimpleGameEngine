// Application Class 
// Class to init DirectX and handle the application data( eg: the Scene Data, the Renderer and the UI)
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

// Globals 
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f; 

// Includes

#include "D3D.h"
#include "Bitmap.h"
#include "Camera.h"
#include "LightManager.h"
#include "Timer.h"
#include "Map.h"
#include "AnimatedModel.h"
#include "DeferredBuffer.h"
#include "Terrain.h"
#include "UserInterface.h"
#include "UserInput.h"
#include "ShaderManager.h"
#include "StateManager.h"
#include "PBStaticModel.h"
#include "SceneWorldData.h"
#include "MaterialManager.h"
#include "TextureManager.h"


// Class name: Application

class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize(HINSTANCE, HWND, int, int, bool, bool, UserInput*, StateManager*);
	void Shutdown();
	bool Frame();

private:
	
	bool UpdateApplication();
	bool UpdateWorld();
	void MoveCamera();

	bool Render();
	bool RenderSceneDeferred();
	bool RenderLightsDeferred();
	bool RenderSceneForward();

private:

	D3D m_D3D;
	Camera m_Camera;
	Timer m_Timer;
	DeferredBuffer m_DeferredBuffers;
	UserInterface m_UserInterface;
	ShaderManager m_ShaderManager;
	StateManager* m_StateManager;
	SceneWorldData m_WorldSceneManager;
	MaterialManager m_MaterialManager;
	TextureManager m_TextureManager;

	float  m_MoveLeftRight;
	float m_MoveBackForward;

	float m_MoveUpDown;

	float m_CamYaw;
	float m_CamPitch;

	int  m_mouseXCurrentState;
	int  m_mouseYCurrentState;

	int  m_mouseXLastState;
	int  m_mouseYLastState;

	int m_CenterofScreenX;
	int m_CenterofScreenY;

	bool m_DetectInputLastFrame;
	bool m_AdditiveBlendingIsOn;

	float m_SpeedFactor;
};

#endif
