/*
Shader Manager Class
Mangae different Shaders of the application
*/
#ifndef _SHADERHELPER_H_
#define _SHADERHELPER_H_

#include <d3d11.h>
#include <D3Dcompiler.h>
#include <D3D11Shader.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>

class ShaderHelper
{
public:
	static void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
	static DirectX::XMFLOAT3 GammaToLinear(const DirectX::XMFLOAT3& color);

	static HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
private:

};

#endif
