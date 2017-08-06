/*
Deferred Shader Class
Fill the G-Buffer with Graphic data
Author: Maximilian Winter
*/
#ifndef _DEFERREDSHADER_H_
#define _DEFERREDSHADER_H_


// Includes

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

#include "ShaderHelper.h"

class DeferredShader
{
private:

public:
	DeferredShader();
	DeferredShader(const DeferredShader&);
	~DeferredShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*);
	void ShutdownShader();

	void RenderShader(ID3D11DeviceContext*, int, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
};

#endif