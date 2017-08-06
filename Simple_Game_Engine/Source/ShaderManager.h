/*
Shader Manager Class
Mangae different Shaders of the application
*/
#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_


// Includes
#include "SceneWorldData.h"
#include "DeferredShader.h"
#include "DeferredBuffer.h"
#include "DirectionalLightShader.h"
#include "PointLightShader.h"
#include "CapsuleLightShader.h"
#include "SpotLightShader.h"
#include "SkyboxShader.h"
#include "GBufferDebuggingShader.h"
#include "LightManager.h"
#include "IBLShader.h"
#include "TextureShader.h"
#include "LineListShader.h"



class ShaderManager
{
private:
	struct GBUFFER_UNPACK_DATA
	{
		DirectX::XMFLOAT4 PerspectiveValues;
		DirectX::XMFLOAT4X4  ViewInv;
	};

	struct ObjectBufferVSType
	{
		DirectX::XMFLOAT4X4 WorldViewProjection;
		DirectX::XMFLOAT4X4 World;
	};

	struct MaterialBufferPSType
	{
		DirectX::XMFLOAT3 Albedo;
		float Roughness;
		float F0;
		float Metalness;
		float Transparency;
		int TextureInput;
	};

	struct EyePositionBufferPSType
	{
		DirectX::XMFLOAT3 EyePosition;
		float pad[1];
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

public:

	enum LightShaderType
	{
		D_IBLShader,						// Deferred image based lightning shader.
		D_DirectionalLightShader,			// Deferred directional lightning shader.
		D_PointLightShader,					// Deferred point lightning shader.
		D_SpotLightShader,					// Deferred spot lightning shader.
		D_CapsuleLightShader,				// Deferred capsule lightning shader.

		F_IBLShader,						// Forward image based lightning shader.
		F_DirectionalLightShader,			// Forward directional lightning shader.
		F_PointLightShader,					// Forward point lightning shader.
		F_SpotLightShader,					// Forward spot lightning shader.
		F_CapsuleLightShader				// Forward capsule lightning shader.
	};

	ShaderManager();
	ShaderManager(const ShaderManager&);
	~ShaderManager();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool SetVertexAndIndexBuffer(ID3D11DeviceContext* deviceContext, VerticesRenderData Vertices);

	bool SetForwardShadingVSMatrixBuffer(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);
	bool SetForwardShadingPSEyePositionBuffer(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR eyePosition);

	bool SetForwardShadingPSMaterialProperties(ID3D11DeviceContext* deviceContext, SurfaceMaterial& Material);
	
	bool SetDeferredShadingPSGBufferParameters(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);

	bool RenderFillGBufferShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);

	bool RenderSkyboxShader(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition);

	bool RenderForwardIBLShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix,
		DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, ImageBasedLightData IBLData);
	bool RenderDeferredIBLShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, ImageBasedLightData IBLData);

	bool ReloadIBLShader(ID3D11Device*);

	bool RenderForwardDirectionalLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix,
		DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, DirectionalLightData DirectionalLight);
	bool RenderDeferredDirectionalLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectionalLightData DirectionalLight);

	bool RenderForwardSpotLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, SpotLightData SpotLight);
	bool RenderDeferredSpotLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, SpotLightData SpotLight);

	bool RenderForwardPointLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix,
		DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, PointLightData PointLight);
	bool RenderDeferredPointLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, PointLightData PointLight);

	bool RenderForwardCapsuleLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, CapsuleLightData CapsuleLight);
	bool RenderDeferredCapsuleLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, CapsuleLightData CapsuleLight);

	bool RenderGBufferDebuggingShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData);

	bool RenderLine(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR from, DirectX::FXMVECTOR to, int r, int g, int b, int a);
private:

	DirectionalLightShader m_DirectionalLightShader;
	PointLightShader m_PointLightShader;
	CapsuleLightShader m_CapsuleLightShader;
	SpotLightShader m_SpotLightShader;
	DeferredShader m_DeferredFillGBufferShader;
	GBufferDebuggingShader m_GBufferDebugShader;
	SkyboxShader m_SkyboxShader;
	TextureShader m_TextureShader;
	IBLShader m_IBLShader;
	LineListShader m_LineShader;

	ID3D11Buffer* m_forwardObjectVSBuffer;
	ID3D11Buffer* m_forwardMaterialPSBuffer;
	ID3D11Buffer* m_forwardEyePositionBuffer;

	ID3D11Buffer* m_DeferredDataForUnpack;

	ID3D11SamplerState* m_LinearWrapSampleState;
	ID3D11SamplerState* m_LinearClampSampleState;
	ID3D11SamplerState* m_AnsiotropicWrapSampleState;
	ID3D11SamplerState* m_PointClampSampleState;

	ID3D11SamplerState* m_TestSampleState;
};

#endif
