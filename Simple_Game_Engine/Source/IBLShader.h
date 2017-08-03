/*
IBL Shader Class
Render a object with IBL
Author: Maximilian Winter
Date: 14.08.2015
*/
#ifndef _IBLSHADER_H_
#define _IBLSHADER_H_

// Includes
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

class IBLShader
{
	struct IBLCubemaps
	{
		ID3D11ShaderResourceView* diffuseProbe;
		ID3D11ShaderResourceView* specularProbe;

		ID3D11ShaderResourceView* diffuseProbeMDR;
		ID3D11ShaderResourceView* specularProbeMDR;

		ID3D11ShaderResourceView* diffuseProbeLDR;
		ID3D11ShaderResourceView* specularProbeLDR;

		ID3D11ShaderResourceView* specularIntegration;
	};
public:
	IBLShader();
	IBLShader(const IBLShader&);
	~IBLShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool ReloadShader(ID3D11Device*);

	bool RenderForward(ID3D11DeviceContext*, int, int, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	bool RenderDeferred(ID3D11DeviceContext*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);

private:
	bool InitializeForwardShader(ID3D11Device*, HWND, WCHAR*);

	bool InitializeDeferredShader(ID3D11Device*, HWND, WCHAR*);

	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetForwardPSLightningParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	bool SetDeferredPSLightningParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);

	void RenderForwardShader(ID3D11DeviceContext*, int, int);
	void RenderDeferredShader(ID3D11DeviceContext*);

	void ShutdownForwardShader();


	ID3D11VertexShader* m_forwardVertexShader;
	ID3D11PixelShader* m_forwardPixelShader;
	ID3D11InputLayout* m_forwardLayout;
	ID3D11SamplerState* m_forwardSampleState;
	
	ID3D11VertexShader* m_deferredVertexShader;
	ID3D11PixelShader* m_deferredPixelShader;
	ID3D11SamplerState* m_deferredSampleState;

	ID3D11SamplerState* m_AnisoSampleState;
	ID3D11SamplerState* m_ClampSampleState;
};

#endif