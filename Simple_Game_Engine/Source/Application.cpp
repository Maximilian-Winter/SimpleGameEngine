#include "Application.h"


Application::Application()
	: m_D3D(),
	  m_Camera(),
	  m_Timer(),
	  m_DeferredBuffers(),
	  m_UserInterface(),
	  m_ShaderManager(),
	  m_StateManager(),
	  m_WorldSceneManager(), m_SceneGraph(Transform(), "World"), m_AdditiveBlendingIsOn(false)
{
	//m_RenderQueue.clear();
	m_MoveLeftRight = 0;
	m_MoveBackForward = 0;
	m_MoveUpDown = 0;

	m_CamYaw = 0;
	m_CamPitch = 0;

	m_mouseXCurrentState = 0;
	m_mouseYCurrentState = 0;

	m_mouseXLastState = 0;
	m_mouseYLastState = 0;

	m_CenterofScreenX = 0;
	m_CenterofScreenY = 0;
	m_SpeedFactor = 0;

	m_DetectInputLastFrame = false;
}


Application::~Application()
{
}

bool Application::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, bool Fullscreen, bool VSync, UserInput* UserInput, StateManager* StateManager)
{
	bool result;

	//Set the State Manager from the System Class
	m_StateManager = StateManager;

	// Initialize the Direct3D object.
	result = m_D3D.Initialize(screenWidth, screenHeight, VSync, hwnd, Fullscreen, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize DirectX 11.", L"Error", MB_OK);
		return false;
	}

	result = m_ShaderManager.Initialize(m_D3D.GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Shader Manager Object.", L"Error", MB_OK);
		return false;
	}

	result = MaterialManager::GetInstance()->Initialize(m_D3D.GetDevice(), &m_TextureManager);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the material manager object.", L"Error", MB_OK);
		return false;
	}

	// Set the initial position of the camera and build the matrices needed for rendering.
	m_Camera.SetPosition(DirectX::XMVectorSet(-5.5f, 2.0f, 5.0f, 0.0f));
	m_Camera.Render();

	// Initialize the timer object.
	result = m_Timer.Initialize();
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the deferred buffers object.
	result = m_DeferredBuffers.Initialize(m_D3D.GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the deferred buffers object.", L"Error", MB_OK);
		return false;
	}


	// Initialize the texture shader object.
	result = m_UserInterface.Initialize(&m_D3D, hwnd, hinstance, screenWidth, screenHeight, m_Camera.GetViewMatrix(), UserInput, StateManager);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the user interface object.", L"Error", MB_OK);
		return false;
	}

	//Load scene from file.
	/*result = m_WorldSceneManager.LoadSceneFile(m_D3D.GetDevice(), MaterialManager::GetInstance(), "data/Scene.gsce");
	if (!result)
	{
		return false;
	}*/

	m_StaticModel.LoadModel(m_D3D.GetDevice(), m_SceneGraph, "ModularCharacters.smo");

	m_WorldSceneManager.GetLightManager().AddSpotLight(-10.0, 15.0, 10.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0, 0.15, 0.60);
	//m_WorldSceneManager.GetLightManager().AddCapsuleLight(-7.5, 1.0, 5.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 250, 40);
	//m_WorldSceneManager.GetLightManager().AddPointLight(DirectX::XMVectorSet(-7.50, 1.0, 10.0, 0.0f), DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), 100);
	//m_WorldSceneManager.GetLightManager().AddDirectionalLight(1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	m_WorldSceneManager.GetLightManager().AddSpotLight(-7.50, 1.0, 10.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 20, 0.5, 0.60);
	
	m_WorldSceneManager.GetLightManager().AddImageBasedLight(m_D3D.GetDevice(), "data/IBL_CubeMaps/skymapDiffuseHDR.dds", "data/IBL_CubeMaps/skymapSpecularHDR.dds", "data/IBL_CubeMaps/skymapBrdf.dds", DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f), 0);

	m_UserInterface.AddTextString("Created by Maximilian Winter 2015", "Seogoe UI", 20.0, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, screenHeight - 30, 400, 100);

	m_CenterofScreenX = screenWidth / 2;
	m_CenterofScreenY = screenHeight / 2;
	m_SpeedFactor = 0.05;

	// Reset start time
	m_Timer.Frame();

	return true;
}

void Application::Shutdown()
{
	m_D3D.Shutdown();
	m_DeferredBuffers.Shutdown();
	m_UserInterface.Shutdown();
	m_ShaderManager.Shutdown();
}

bool Application::Frame()
{
	bool result;

	// Update Application e.g.: Upadte Input, Timers , mouse coords 
	result = UpdateApplication();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool Application::UpdateApplication()
{
	bool result = false;

	// Update Timer.
	m_Timer.Frame();

	// Update UserInterface e.g.: Upadte Input, Timers , mouse coords 
	result = m_UserInterface.Frame();
	if (!result)
	{
		return false;
	}

	//Update Mouse Location
	m_UserInterface.GetMouseLocation(m_mouseXCurrentState, m_mouseYCurrentState);

	if (m_UserInterface.IsFunctionKeyDown('T'))
	{
		m_ShaderManager.ReloadIBLShader(m_D3D.GetDevice());
	}

	if (m_UserInterface.IsFunctionKeyDown(VK_CONTROL) && m_UserInterface.IsFunctionKeyDown('F'))
	{
		m_DeferredBuffers.SaveDeferredRenderTargetsToFile(m_D3D.GetDeviceContext());
		m_D3D.SaveRenderTargetToFile();
	}


	// Update World e.g.: animation, collison, camera 
	result = UpdateWorld();
	if (!result)
	{
		return false;
	}
	return true;

}

bool Application::UpdateWorld()
{
	bool result;

	if (m_UserInterface.IsFunctionKeyDown(VK_UP))
	{
		m_WorldSceneManager.ApplyCentralForceToObject("Ball", DirectX::XMVectorSet(0.0f, 0.0f, 50.5f, 0.0f));
	}

	if (m_UserInterface.IsFunctionKeyDown(VK_DOWN))
	{
		m_WorldSceneManager.ApplyCentralForceToObject("Ball", DirectX::XMVectorSet(0.0f, 0.0f, -50.5f, 0.0f));
	}

	if (m_UserInterface.IsFunctionKeyDown(VK_RIGHT))
	{
		m_WorldSceneManager.ApplyCentralForceToObject("Ball", DirectX::XMVectorSet(50.5f, 0.0f, 0.0f, 0.0f));
	}

	if (m_UserInterface.IsFunctionKeyDown(VK_LEFT))
	{
		m_WorldSceneManager.ApplyCentralForceToObject("Ball", DirectX::XMVectorSet(-50.5f, 0.0f, 0.0f, 0.0f));
	}

	if (m_UserInterface.IsFunctionKeyDown(VK_ADD))
	{
		m_WorldSceneManager.ApplyCentralForceToObject("Ball", DirectX::XMVectorSet(0.0f, 500.5f, 0.0f, 0.0f));
	}

	if (m_UserInterface.IsFunctionKeyDown(VK_SUBTRACT))
	{
		m_WorldSceneManager.ApplyCentralForceToObject("Ball", DirectX::XMVectorSet(0.0f, -500.5f, 0.0f, 0.0f));
	}


	if (m_StateManager->GetApplicationState(IsPaused) == false && m_StateManager->GetApplicationState(IsStarted) && m_DetectInputLastFrame == false)
	{
		static float forceY = 1000.0f;

		static int modelCount = 0;

		float SpeedFactor = m_SpeedFactor * m_Timer.GetTime();

		DirectX::XMVECTOR Force = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		MoveCamera();

		m_WorldSceneManager.UpdatePhysic(m_Timer.GetTime() * 0.001f);

		if (m_UserInterface.IsFunctionKeyDown('X'))
		{
			DirectX::XMVECTOR Position = m_Camera.GetPosition();
			DirectX::XMVECTOR Rotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMVECTOR Scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);

			std::string name = "Ball";
			result = m_WorldSceneManager.AddObjectToScene(m_D3D.GetDevice(), MaterialManager::GetInstance(), SceneWorldData::StaticObject, SceneWorldData::CullSphere, SceneWorldData::CollisionSphere, "data/models/ball.smo", name, Position, Rotation, Scale, 1.0f);
			if (!result)
			{
				return false;
			}


			Force = DirectX::XMVectorSet(25.0f, 25.0f, 25.0f, 1.0f);
			Force = m_Camera.GetLookAt() * Force;
			m_WorldSceneManager.ApplyCentralImpulseToObject(name, Force);

			modelCount++;

			m_DetectInputLastFrame = true;
		}

	
		if (m_UserInterface.IsFunctionKeyDown('E'))
		{
			Force = DirectX::XMVectorSet(0.0f, forceY, 0.0f, 0.0f);

			m_WorldSceneManager.ApplyCentralForceToObject("Barrel", Force);
			m_DetectInputLastFrame = true;
		}

		if (m_UserInterface.IsFunctionKeyDown('G'))
		{
			forceY += 10.0f;
			m_DetectInputLastFrame = true;
		}

		if (m_UserInterface.IsFunctionKeyDown('H'))
		{
			forceY -= 10.0f;
			m_DetectInputLastFrame = true;
		}

		m_UserInterface.AddTemporaryTextString("ForceY: " + std::to_string(forceY), "Seogoe UI", 20.0, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, 400, 400, 100);

		m_UserInterface.AddTemporaryTextString("Model Count: " + std::to_string(modelCount), "Seogoe UI", 20.0, 1.0f, 1.0f, 1.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10, 500, 400, 100);

		if (m_UserInterface.IsFunctionKeyDown('R'))
		{
			m_WorldSceneManager.UpdateAnimations(m_D3D.GetDeviceContext(), m_Timer.GetTime()*0.002, 0);
			m_DetectInputLastFrame = true;
		}

		if (m_UserInterface.IsFunctionKeyDown('Q'))
		{
			m_DetectInputLastFrame = false;
		}
	}
	else
	{
		if (!(m_UserInterface.IsFunctionKeyDown('E') || m_UserInterface.IsFunctionKeyDown('G') || m_UserInterface.IsFunctionKeyDown('H') || m_UserInterface.IsFunctionKeyDown('R') || m_UserInterface.IsFunctionKeyDown(VK_UP) || m_UserInterface.IsFunctionKeyDown(VK_DOWN) || m_UserInterface.IsFunctionKeyDown(VK_LEFT) || m_UserInterface.IsFunctionKeyDown(VK_RIGHT)|| m_UserInterface.IsFunctionKeyDown('X')))
		{
			m_DetectInputLastFrame = false;
		}
	}

	return true;

}

void Application::MoveCamera()
{
	float WalkingSpeedFactor = 0.01;

	if (m_UserInterface.IsFunctionKeyDown(VK_SHIFT))
	{
		WalkingSpeedFactor = 0.1;
	}

	// Get the fps for calculating the speed factor.
	WalkingSpeedFactor *= m_Timer.GetTime();

	// Handle the input.
	if (m_UserInterface.IsFunctionKeyDown('A'))
	{
		m_MoveLeftRight -= WalkingSpeedFactor;
	}

	// Handle the input.
	if (m_UserInterface.IsFunctionKeyDown('D'))
	{
		m_MoveLeftRight += WalkingSpeedFactor;
	}

	// Handle the input.
	if (m_UserInterface.IsFunctionKeyDown('W'))
	{
		m_MoveBackForward += WalkingSpeedFactor;
	}

	// Handle the input.
	if (m_UserInterface.IsFunctionKeyDown('S'))
	{
		m_MoveBackForward -= WalkingSpeedFactor;
	}
	m_Camera.SetMoveBackForward(m_MoveBackForward);
	m_Camera.SetMoveLeftRight(m_MoveLeftRight);
	m_MoveBackForward = 0.0f;
	m_MoveLeftRight = 0.0f;

	int dx, dy;
	m_UserInterface.GetMouseDeltaXY (dx, dy);

	if (dx > 0.0f)
	{
		m_CamYaw += dx * 0.005f;
	}

	if (dx < 0.0f)
	{
		m_CamYaw += dx * 0.005f;
	}

	if (dy > 0.0f)
	{
		m_CamPitch += dy * 0.005f;
	}

	if (dy < 0.0f)
	{
		m_CamPitch += dy * 0.005f;
	}

	// Handle the input.
	if (m_UserInterface.IsFunctionKeyDown('C'))
	{
		m_CamPitch = 0.0f;
		m_CamYaw = 0.0f;
	}

	m_Camera.SetCameraPitch(m_CamPitch);
	m_Camera.SetCameraYaw(m_CamYaw);

	m_mouseYLastState = m_mouseYCurrentState;
	m_mouseXLastState = m_mouseXCurrentState;
}

bool Application::Render()
{
	bool result;

	// Clear the scene.
	m_D3D.BeginScene(0.0, 0.0, 0.0, 1.0);

	//Update camera view matrix (position rotation)
	m_Camera.Render();

	//Prepare world scene data for rendering
	m_WorldSceneManager.PrepareFrameRenderData(MaterialManager::GetInstance(), SCREEN_DEPTH, m_Camera.GetPosition(), m_Camera.GetViewMatrix(), m_D3D.GetProjectionMatrix());

	// Render the scene to the render buffers.
	result = RenderSceneDeferred();
	if (!result)
	{
		return false;
	}

	//Render the lights with the deferred data.
	result = RenderLightsDeferred();
	if (!result)
	{
		return false;
	}

	//Render transparent object with forward shading.
	result = RenderSceneForward();
	if (!result)
	{
		return false;
	}
	 
	//m_ShaderManager.RenderGBufferDebuggingShader(m_D3D.GetDeviceContext(), m_DeferredBuffers.GetDeferredRenderData()); 

	//m_UserInterface.AddTemporaryLine(0, 0, 500, 1000, 1.0f, 0.0f, 0.0f);

	//Render User Interface
	result = m_UserInterface.RenderMouseCursor(&m_D3D, 0);
	if (!result)
	{
		return false;
	}

	result = m_UserInterface.RenderUI(m_D3D.GetD2DRenderTarget());
	if (!result)
	{
		return false;
	}
	
	// Present the rendered scene to the screen.
	m_D3D.EndScene();

	//Clear the scene render data buffer.
	m_WorldSceneManager.ClearSceneRenderData();

	return true;

}

bool Application::RenderSceneDeferred()
{
	bool result;

	DirectX::XMMATRIX viewMatrix, projectionMatrix;

	// Get the matrices from the camera and d3d objects.
	viewMatrix = m_Camera.GetViewMatrix();
	projectionMatrix = m_D3D.GetProjectionMatrix();

	// Set the render buffers to be the render target and change .
	m_DeferredBuffers.BindDeferredBufferToPipeline(m_D3D.GetDeviceContext());

	//Get the deferred render data.
	std::vector<SceneObjectData>& DeferredRenderData = m_WorldSceneManager.GetDeferredFrameRenderData();

	//Render the deferred render data using the deferred shader.
	for (int i = 0; i < DeferredRenderData.size(); i++)
	{
		SceneObjectData ObjectData = DeferredRenderData[i];

		result = m_ShaderManager.RenderFillGBufferShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix);
		if (!result)
		{
			return false;
		}
	}
	return true;
}

bool Application::RenderLightsDeferred()
{

	bool result;
	bool AdditiveBlendingIsOn = false;
	DirectX::XMMATRIX camViewMatrix, camProjectionMatrix;

	// Get the matrices
	camViewMatrix = m_Camera.GetViewMatrix();
	camProjectionMatrix = m_D3D.GetProjectionMatrix();

	// Use the first spot light as a Flashlight
	DirectX::XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, m_Camera.GetPosition());

	DirectX::XMFLOAT3 camLookAt;
	XMStoreFloat3(&camLookAt, m_Camera.GetLookAt());

	m_WorldSceneManager.GetLightManager().SetSpotLightPosition(0, camPos.x, camPos.y, camPos.z);
	m_WorldSceneManager.GetLightManager().SetSpotLightDirection(0, camLookAt.x - camPos.x, camLookAt.y - camPos.y, camLookAt.z - camPos.z);

	// Reset the render target back to the original back buffer and not the render buffers and set the read only depth stencil view from deferred rendering
	m_D3D.SetDepthStencilView(m_DeferredBuffers.GetDeferredReadOnlyDSV());
	m_D3D.ResetViewport();

	m_D3D.TurnZBufferOff();

	//Render All Directional Lights
	std::vector<DirectionalLightData>& DirectionalLights = m_WorldSceneManager.GetFrameDirectionalLightData();
	for (auto DirectionalLight : DirectionalLights)
	{
		result = m_ShaderManager.RenderDeferredDirectionalLightShader(m_D3D.GetDeviceContext(), m_DeferredBuffers.GetDeferredRenderData(), camViewMatrix, camProjectionMatrix, DirectionalLight);

		if (!result)
		{
			return false;
		}


		if (AdditiveBlendingIsOn == false)
		{
			m_D3D.TurnAdditiveAlphaBlendingOn();
			AdditiveBlendingIsOn = true;
		}
	}

	//Render All Image Based Lights
	std::vector<ImageBasedLightData>& ImageBasedLights = m_WorldSceneManager.GetFrameImageBasedLightData();
	for (auto ImageBasedLight : ImageBasedLights)
	{
		result = m_ShaderManager.RenderDeferredIBLShader(m_D3D.GetDeviceContext(), m_DeferredBuffers.GetDeferredRenderData(),camViewMatrix, camProjectionMatrix, ImageBasedLight);
		if (!result)
		{
			return false;
		}

		if (AdditiveBlendingIsOn == false)
		{
			m_D3D.TurnAdditiveAlphaBlendingOn();
			AdditiveBlendingIsOn = true;
		}
	}

	m_D3D.TurnStencilStateGreaterEqualOn();
	m_D3D.TurnOnFrontFaceCWCulling();

	//Render All Point Lights
	std::vector<PointLightData>& PointLights = m_WorldSceneManager.GetFramePointLightData();
	for (auto PointLight : PointLights)
	{
		result = m_ShaderManager.RenderDeferredPointLightShader(m_D3D.GetDeviceContext(), m_DeferredBuffers.GetDeferredRenderData(), camViewMatrix, camProjectionMatrix, PointLight);
		
		if (!result)
		{
			return false;
		}


		if (AdditiveBlendingIsOn == false)
		{
			m_D3D.TurnAdditiveAlphaBlendingOn();
			AdditiveBlendingIsOn = true;
		}
	}

	

	//Render All Capsule Lights
	std::vector<CapsuleLightData>& CapsuleLights = m_WorldSceneManager.GetFrameCapsuleLightData();
	for (auto CapsuleLight : CapsuleLights)
	{
		result = m_ShaderManager.RenderDeferredCapsuleLightShader(m_D3D.GetDeviceContext(), m_DeferredBuffers.GetDeferredRenderData(), camViewMatrix, camProjectionMatrix, CapsuleLight);

		if (!result)
		{
			return false;
		}


		if (AdditiveBlendingIsOn == false)
		{
			m_D3D.TurnAdditiveAlphaBlendingOn();
			AdditiveBlendingIsOn = true;
		}
	}

	
	
	//Render All Spot Lights
	std::vector<SpotLightData>& SpotLights = m_WorldSceneManager.GetFrameSpotLightData();
	for (auto SpotLight : SpotLights)
	{
		result = m_ShaderManager.RenderDeferredSpotLightShader(m_D3D.GetDeviceContext(), m_DeferredBuffers.GetDeferredRenderData(), camViewMatrix, camProjectionMatrix, SpotLight);

		if (!result)
		{
			return false;
		}

		if (AdditiveBlendingIsOn == false)
		{
			m_D3D.TurnAdditiveAlphaBlendingOn();
			AdditiveBlendingIsOn = true;
		}
	}

	if (AdditiveBlendingIsOn == true)
	{
		m_D3D.TurnAdditiveAlphaBlendingOff();
		AdditiveBlendingIsOn = false;
	}

	m_D3D.TurnOnBackFaceCWCulling();
	m_D3D.TurnStencilStateLessOn();

	m_D3D.TurnZBufferOn();

	return true;
}

bool Application::RenderSceneForward()
{
	bool result;
	bool AdditiveBlendingIsOn = false;

	DirectX::XMMATRIX viewMatrix, projectionMatrix;

	// Get the matrices from the camera and d3d objects.
	viewMatrix = m_Camera.GetViewMatrix();
	projectionMatrix = m_D3D.GetProjectionMatrix();

	// Use the first spot light as a Flashlight
	DirectX::XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, m_Camera.GetPosition());

	DirectX::XMFLOAT3 camLookAt;
	XMStoreFloat3(&camLookAt, m_Camera.GetLookAt());

	m_WorldSceneManager.GetLightManager().SetSpotLightPosition(0, camPos.x, camPos.y, camPos.z);
	m_WorldSceneManager.GetLightManager().SetSpotLightDirection(0, camLookAt.x - camPos.x, camLookAt.y - camPos.y, camLookAt.z - camPos.z);

	// Set the normal depth stencil view from deferred shading to prepare forward rendering.
	m_D3D.SetDepthStencilView(m_DeferredBuffers.GetDeferredDSV());

	// Set the depth stencil state to less eqal.
	m_D3D.TurnStencilStateLessEqualOn();

	// Render the skybox
	m_D3D.TurnOffCulling();
	m_ShaderManager.RenderSkyboxShader(m_D3D.GetDeviceContext(), m_Camera.GetViewMatrix(), m_D3D.GetProjectionMatrix(), m_Camera.GetPosition());
	m_D3D.TurnOnBackFaceCWCulling();

	std::vector<SceneObjectData> meshesToRender;
	m_SceneGraph.Update(meshesToRender);


	//Get forward render data
	std::vector<SceneObjectData>& ForwardRenderData = m_WorldSceneManager.GetForwardFrameRenderData();

	//Render all forward scene objects forward
	for (int ObjIndex = 0; ObjIndex < ForwardRenderData.size(); ObjIndex++)
	{
		SceneObjectData ObjectData = ForwardRenderData[ObjIndex];

		//Render all directional lights.
		std::vector<DirectionalLightData>& DirectionalLights = m_WorldSceneManager.GetFrameDirectionalLightData();
		for (auto DirectionalLight : DirectionalLights)
		{
			result = m_ShaderManager.RenderForwardDirectionalLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), DirectionalLight);

			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render all point lights.
		std::vector<PointLightData>& PointLights = m_WorldSceneManager.GetFramePointLightData();
		for (auto PointLight : PointLights)
		{
			result = m_ShaderManager.RenderForwardPointLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), PointLight);

			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render All Spot Lights
		std::vector<SpotLightData>& SpotLights = m_WorldSceneManager.GetFrameSpotLightData();
		for (auto SpotLight : SpotLights)
		{

			result = m_ShaderManager.RenderForwardSpotLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), SpotLight);

			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render all capsule lights.
		std::vector<CapsuleLightData>& CapsuleLights = m_WorldSceneManager.GetFrameCapsuleLightData();
		for (auto CapsuleLight : CapsuleLights)
		{

			result = m_ShaderManager.RenderForwardCapsuleLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), CapsuleLight);
			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render All Image Based Lights
		std::vector<ImageBasedLightData>& ImageBasedLights = m_WorldSceneManager.GetFrameImageBasedLightData();
		for (auto ImageBasedLight : ImageBasedLights)
		{

			result = m_ShaderManager.RenderForwardIBLShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), ImageBasedLight);
			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
			
		}

		if (AdditiveBlendingIsOn == true)
		{
			m_D3D.TurnAdditiveAlphaBlendingOff();
			AdditiveBlendingIsOn = false;
		}
	}

	for (int ObjIndex = 0; ObjIndex < meshesToRender.size(); ObjIndex++)
	{
		SceneObjectData ObjectData = meshesToRender[ObjIndex];

		//Render all directional lights.
		std::vector<DirectionalLightData>& DirectionalLights = m_WorldSceneManager.GetFrameDirectionalLightData();
		for (auto DirectionalLight : DirectionalLights)
		{
			result = m_ShaderManager.RenderForwardDirectionalLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), DirectionalLight);

			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render all point lights.
		std::vector<PointLightData>& PointLights = m_WorldSceneManager.GetFramePointLightData();
		for (auto PointLight : PointLights)
		{
			result = m_ShaderManager.RenderForwardPointLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), PointLight);

			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render All Spot Lights
		std::vector<SpotLightData>& SpotLights = m_WorldSceneManager.GetFrameSpotLightData();
		for (auto SpotLight : SpotLights)
		{

			result = m_ShaderManager.RenderForwardSpotLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), SpotLight);

			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render all capsule lights.
		std::vector<CapsuleLightData>& CapsuleLights = m_WorldSceneManager.GetFrameCapsuleLightData();
		for (auto CapsuleLight : CapsuleLights)
		{

			result = m_ShaderManager.RenderForwardCapsuleLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), CapsuleLight);
			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}
		}

		//Render All Image Based Lights
		std::vector<ImageBasedLightData>& ImageBasedLights = m_WorldSceneManager.GetFrameImageBasedLightData();
		for (auto ImageBasedLight : ImageBasedLights)
		{

			result = m_ShaderManager.RenderForwardIBLShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), ImageBasedLight);
			if (!result)
			{
				return false;
			}

			if (AdditiveBlendingIsOn == false)
			{
				m_D3D.TurnAdditiveAlphaBlendingOn();
				AdditiveBlendingIsOn = true;
			}

		}

		if (AdditiveBlendingIsOn == true)
		{
			m_D3D.TurnAdditiveAlphaBlendingOff();
			AdditiveBlendingIsOn = false;
		}
	}

	//Get transparent render data
	std::vector<SceneObjectData>& TransparentRenderData = m_WorldSceneManager.GetTransparentFrameRenderData();

	//Render all transparent scene objects forward
	for (int ObjIndex = 0; ObjIndex < TransparentRenderData.size(); ObjIndex++)
	{
		SceneObjectData ObjectData = TransparentRenderData[ObjIndex];
		m_D3D.TurnOnAlphaBlending();

		for (int DrawCount = 0; DrawCount < 2; DrawCount++)
		{
			//Render all directional lights.
			std::vector<DirectionalLightData>& DirectionalLights = m_WorldSceneManager.GetFrameDirectionalLightData();
			for (auto DirectionalLight : DirectionalLights)
			{
				result = m_ShaderManager.RenderForwardDirectionalLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), DirectionalLight);

				if (!result)
				{
					return false;
				}

				if (AdditiveBlendingIsOn == false)
				{
					m_D3D.TurnAdditiveAlphaBlendingOn();
					AdditiveBlendingIsOn = true;
				}
			}

			//Render all point lights.
			std::vector<PointLightData>& PointLights = m_WorldSceneManager.GetFramePointLightData();
			for (auto PointLight : PointLights)
			{
				result = m_ShaderManager.RenderForwardPointLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), PointLight);

				if (!result)
				{
					return false;
				}

				if (AdditiveBlendingIsOn == false)
				{
					m_D3D.TurnAdditiveAlphaBlendingOn();
					AdditiveBlendingIsOn = true;
				}
			}

			//Render All Spot Lights
			std::vector<SpotLightData>& SpotLights = m_WorldSceneManager.GetFrameSpotLightData();
			for (auto SpotLight : SpotLights)
			{

				result = m_ShaderManager.RenderForwardSpotLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), SpotLight);

				if (!result)
				{
					return false;
				}

				if (AdditiveBlendingIsOn == false)
				{
					m_D3D.TurnAdditiveAlphaBlendingOn();
					AdditiveBlendingIsOn = true;
				}
			}

			//Render all capsule lights.
			std::vector<CapsuleLightData>& CapsuleLights = m_WorldSceneManager.GetFrameCapsuleLightData();
			for (auto CapsuleLight : CapsuleLights)
			{

				result = m_ShaderManager.RenderForwardCapsuleLightShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), CapsuleLight);
				if (!result)
				{
					return false;
				}

				if (AdditiveBlendingIsOn == false)
				{
					m_D3D.TurnAdditiveAlphaBlendingOn();
					AdditiveBlendingIsOn = true;
				}
			}

			//Render all image based lights.
			std::vector<ImageBasedLightData>& ImageBasedLights = m_WorldSceneManager.GetFrameImageBasedLightData();
			for (auto ImageBasedLight : ImageBasedLights)
			{

				result = m_ShaderManager.RenderForwardIBLShader(m_D3D.GetDeviceContext(), ObjectData, viewMatrix, projectionMatrix, m_Camera.GetPosition(), ImageBasedLight);
				if (!result)
				{
					return false;
				}

				if (AdditiveBlendingIsOn == false)
				{
					m_D3D.TurnAdditiveAlphaBlendingOn();
					AdditiveBlendingIsOn = true;
				}

			}

			m_D3D.TurnOnBackFaceCCWCulling();
		}
		m_D3D.TurnOnBackFaceCWCulling();

		m_D3D.TurnOffAlphaBlending();
	}
	

	// Draw physic engine debug lines for aabbb or collision shapes.
	std::vector<PhysicDebugDraw::Lines>& debugLines = m_WorldSceneManager.GetDebugLinesToDraw();
	
	/*
	for (DebugDraw::Lines& debugLine : debugLines)
	{
		m_ShaderManager.RenderLine(m_D3D.GetDeviceContext(), viewMatrix, projectionMatrix, DirectX::XMLoadFloat3(&debugLine.from), DirectX::XMLoadFloat3(&debugLine.to), debugLine.color.x, debugLine.color.y, debugLine.color.z, 1.0f);
	}
	
	*/

	m_ShaderManager.RenderLine(m_D3D.GetDeviceContext(), viewMatrix, projectionMatrix, DirectX::XMVectorSet(0.0f, -100.0f, 0.0, 0.0f), DirectX::XMVectorSet(0.0f, 100.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 1.0f);

	m_ShaderManager.RenderLine(m_D3D.GetDeviceContext(), viewMatrix, projectionMatrix, DirectX::XMVectorSet(-100.0f, 0.0f, 0.0, 0.0f), DirectX::XMVectorSet(100.0f, 0.0f, 0.0f, 0.0f), 1.0f, 0.0f, 0.0f, 1.0f);

	m_ShaderManager.RenderLine(m_D3D.GetDeviceContext(), viewMatrix, projectionMatrix, DirectX::XMVectorSet(0.0f, 0.0f, -100.0, 0.0f), DirectX::XMVectorSet(0.0f, 0.0f, 100.0f, 0.0f), 0.0f, 1.0f, 0.0f, 1.0f);


	debugLines.clear();

	// Turn the 
	m_D3D.TurnStencilStateLessOn();

	return true;

}
