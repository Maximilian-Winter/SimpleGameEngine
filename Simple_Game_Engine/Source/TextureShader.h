// Texture Shader:  Render textures to screen
#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_


// Includes
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#include "ShaderHelper.h"

class TextureShader
{
private:
	struct ConstantBufferType
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

public:
	TextureShader();
	TextureShader(const TextureShader&);
	~TextureShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, DirectX::CXMMATRIX, DirectX::CXMMATRIX, DirectX::CXMMATRIX, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, DirectX::CXMMATRIX, DirectX::CXMMATRIX, DirectX::CXMMATRIX, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_constantBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif