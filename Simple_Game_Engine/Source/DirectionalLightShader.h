/*
Directional Light Shader Class
Manage the Rendering of Directional Lights
Author: Maximilian Winter
Date: 10.01.2015
*/
#ifndef _DIRECTIONALLIGHTSHADER_H_
#define _DIRECTIONALLIGHTSHADER_H_



// Includes
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace std;


// Class name: DirectionalLightShader

class DirectionalLightShader
{
private:

	struct LightBufferType
	{
		DirectX::XMFLOAT3 lightDirection;
		float pad;
		DirectX::XMFLOAT3 lightColor;
		float pad3;
		DirectX::XMFLOAT3 AmbientDown;
		float pad4;
		DirectX::XMFLOAT3 AmbientRange;
		float pad5;
	};

public:
	DirectionalLightShader();
	DirectionalLightShader(const DirectionalLightShader&);
	~DirectionalLightShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	
	bool RenderForward(ID3D11DeviceContext*, int, int, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::CXMVECTOR);
	
	bool RenderDeferred(ID3D11DeviceContext*, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::CXMVECTOR);
private:
	DirectX::XMFLOAT3 GammaToLinear(const DirectX::XMFLOAT3& color);

	bool InitializeForwardShader(ID3D11Device*, HWND, WCHAR*);
	bool InitializeDeferredShader(ID3D11Device*, HWND, WCHAR*);

	bool SetForwardPSLightningParameters(ID3D11DeviceContext*, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::CXMVECTOR);

	bool SetDeferredPSLightningParameters(ID3D11DeviceContext*, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::CXMVECTOR);

	void RenderForwardShader(ID3D11DeviceContext* , int, int);
	void RenderDeferredShader(ID3D11DeviceContext*);
	
	void ShutdownForwardShader();
	void ShutdownDeferredShader();
	
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	



private:
	ID3D11VertexShader* m_deferredVertexShader;
	ID3D11PixelShader* m_deferredPixelShader;
	ID3D11InputLayout* m_deferredLayout;
	ID3D11SamplerState* m_deferredSampleState;
	ID3D11Buffer* m_deferredLightBuffer;

	ID3D11VertexShader* m_forwardVertexShader;
	ID3D11PixelShader* m_forwardPixelShader;
	ID3D11InputLayout* m_forwardLayout;
	ID3D11SamplerState* m_forwardSampleState;
	ID3D11Buffer* m_forwardLightBuffer;

	
};

#endif