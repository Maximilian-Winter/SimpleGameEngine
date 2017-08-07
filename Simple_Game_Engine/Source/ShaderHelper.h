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

	static HRESULT CompileShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFilename, LPCSTR shaderEntryPoint, LPCSTR shaderTarget, ID3D10Blob** pShaderBuffer);

	static HRESULT CreateVertexShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFilename, LPCSTR shaderEntryPoint ,ID3D10Blob** pVertexShaderBuffer, ID3D11VertexShader** pVertexShader);
	static HRESULT CreatePixelShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFilename, LPCSTR shaderEntryPoint, ID3D10Blob** pPixelShaderBuffer, ID3D11PixelShader** pPixelShader);
	static HRESULT CreateHullShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFilename, LPCSTR shaderEntryPoint, ID3D10Blob** pHullShaderBuffer, ID3D11HullShader** pHullShader);
	static HRESULT CreateDomainShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFilename, LPCSTR shaderEntryPoint, ID3D10Blob** pDomainShaderBuffer, ID3D11DomainShader** pDomainShader);

	static HRESULT CreateConstantBuffer(ID3D11Device* device, D3D11_USAGE bufferUsage, UINT byteWidth, D3D11_BIND_FLAG bindFlag, D3D11_CPU_ACCESS_FLAG accessFlag, UINT miscFlags, UINT byteStride, ID3D11Buffer** buffer);

	static HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
private:

};

#endif
