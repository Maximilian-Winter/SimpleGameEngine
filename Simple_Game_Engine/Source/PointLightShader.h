/*
Point Light Shader Class
Manage the Rendering of Point Lights
Author: Maximilian Winter
Date: 10.01.2015
*/
#ifndef _POINTLIGHTSHADER_H_
#define _POINTLIGHTSHADER_H_

// Includes

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#include "ShaderHelper.h"

// Class name: LightShaderClass

class PointLightShader
{
private:

	struct LightBufferType
	{
		DirectX::XMFLOAT3 lightPosition;
		float lightRange;
		DirectX::XMFLOAT3 lightColor;
		float pad;
	};

	struct PointLightScaleMatrix
	{
		DirectX::XMFLOAT4X4 wvp;
	};

public:
	PointLightShader();
	PointLightShader(const PointLightShader&);
	~PointLightShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderForward(ID3D11DeviceContext*, int, int, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float);

	bool RenderDeferred(ID3D11DeviceContext*, DirectX::CXMMATRIX, DirectX::CXMMATRIX, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float);

private:

	bool InitializeForwardShader(ID3D11Device*, HWND, WCHAR*);
	bool InitializeDeferredShader(ID3D11Device*, HWND, WCHAR*);

	void ShutdownForwardShader();
	void ShutdownDeferredShader();

	bool SetForwardPSLightningParameters(ID3D11DeviceContext*, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float);
	bool SetDeferredPSLightningParameters(ID3D11DeviceContext*, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float, DirectX::CXMMATRIX, DirectX::CXMMATRIX);

	void RenderForwardShader(ID3D11DeviceContext*, int, int);
	void RenderDeferredShader(ID3D11DeviceContext*);

private:
	ID3D11VertexShader* m_DeferredVertexShader;
	ID3D11PixelShader* m_DeferredPixelShader;
	ID3D11HullShader* m_DeferredHullShader;
	ID3D11DomainShader* m_DeferredDomainShader;
	ID3D11Buffer* m_DeferredLightBuffer;
	ID3D11Buffer* m_DeferredPointLightScaleBuffer;

	ID3D11VertexShader* m_forwardVertexShader;
	ID3D11PixelShader* m_forwardPixelShader;
	ID3D11InputLayout* m_forwardLayout;
	ID3D11Buffer* m_forwardLightBuffer;


};

#endif