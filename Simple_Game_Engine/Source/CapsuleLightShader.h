/*
Capsule Light Shader Class
Manage the Rendering of Capsule Lights
Author: Maximilian Winter
Date: 10.01.2015
*/
#ifndef _CAPUSULELIGHTSHADER_H_
#define _CAPUSULELIGHTSHADER_H_


// Includes
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>


class CapsuleLightShader
{
private:

	struct LightBufferType
	{
		DirectX::XMFLOAT3 CapsuleLightPos;
		float CapsuleLightRange;
		DirectX::XMFLOAT3 CapsuleDir;
		float CapsuleLen;
		DirectX::XMFLOAT3 CapsuleColor;
		float pad5;
	};

	struct CapsuleLightScaleData
	{
		DirectX::XMFLOAT4X4 WolrdViewProj;
		float HalfCapsuleLen;
		float CapsuleRange;
		float pad[2];;
	};

public:
	CapsuleLightShader();
	CapsuleLightShader(const CapsuleLightShader&);
	~CapsuleLightShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool RenderForward(ID3D11DeviceContext*, int, int, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float, float);

	bool RenderDeferred(ID3D11DeviceContext*, DirectX::CXMMATRIX, DirectX::CXMMATRIX, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float, float);

private:
	bool InitializeForwardShader(ID3D11Device*, HWND, WCHAR*);
	bool InitializeDeferredShader(ID3D11Device*, HWND, WCHAR*);

	void ShutdownForwardShader();
	void ShutdownDeferredShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	const DirectX::XMFLOAT3 GammaToLinear(const DirectX::XMFLOAT3& color);

	bool SetForwardPSLightningParameters(ID3D11DeviceContext*, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float, float);

	bool SetDeferredPSLightningParameters(ID3D11DeviceContext*, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float, float, DirectX::CXMMATRIX, DirectX::CXMMATRIX);
	
	void RenderForwardShader(ID3D11DeviceContext*, int, int);
	void RenderDeferredShader(ID3D11DeviceContext*);

private:
	ID3D11VertexShader* m_deferredVertexShader;
	ID3D11PixelShader* m_deferredPixelShader;
	ID3D11HullShader* m_deferredHullShader;
	ID3D11DomainShader* m_deferredDomainShader;
	ID3D11Buffer* m_deferredLightBuffer;
	ID3D11Buffer* m_deferredCapsuleLightScaleBuffer;
	ID3D11InputLayout* m_deferredLayout;
	ID3D11SamplerState* m_deferredSampleState;

	ID3D11InputLayout* m_forwardLayout;
	ID3D11SamplerState* m_forwardSampleState;
	ID3D11VertexShader* m_forwardVertexShader;
	ID3D11PixelShader* m_forwardPixelShader;
	ID3D11Buffer* m_forwardLightBuffer;

};

#endif