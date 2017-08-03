// GBuffer debugging class to render the 4 render targets as small frame at the bottom
#ifndef _GBUFFERDEBUGGINGSHADER_H_
#define _GBUFFERDEBUGGINGSHADER_H_


// Includes
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>


class GBufferDebuggingShader
{
private:

	struct GBUFFER_UNPACK_DATA
	{
		DirectX::XMFLOAT4 PerspectiveValues;
		DirectX::XMFLOAT4X4  ViewInv;
	};


public:
	GBufferDebuggingShader();
	GBufferDebuggingShader(const GBufferDebuggingShader&);
	~GBufferDebuggingShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11SamplerState* m_sampleState;
};

#endif