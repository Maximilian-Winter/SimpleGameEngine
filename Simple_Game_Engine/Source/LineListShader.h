/*
Line Shader
Author: Maximilian Winter
Date: 10.01.2015
*/
#ifndef _LINELISTSHADER_H_
#define _LINELISTSHADER_H_

// Includes

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#include "ShaderHelper.h"

// Class name: LightShaderClass
class LineListShader
{
private:

	struct ColoredPointBufferType
	{
		DirectX::XMFLOAT3 point;
		DirectX::XMFLOAT4 color;
	};

public:

	LineListShader();
	LineListShader(const LineListShader&);
	~LineListShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool DrawLine(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR from, DirectX::FXMVECTOR to, int r, int g, int b, int a);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*);

	void ShutdownShader();

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_vertexBuffer;

};

#endif