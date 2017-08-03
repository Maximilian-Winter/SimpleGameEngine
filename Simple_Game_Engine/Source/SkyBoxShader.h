/*
Skybox Shader Class
Render a skybox
Author: Maximilian Winter
Date: 14.01.2016
*/
#ifndef _SKYBOXSHADER_H_
#define _SKYBOXSHADER_H_

// Includes
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>

#include "DDSUtil\DDSTextureLoader\DDSTextureLoader.h"

class SkyboxShader
{
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

public:
	SkyboxShader();
	SkyboxShader(const SkyboxShader&);
	~SkyboxShader();

	bool Initialize(ID3D11Device*, HWND);

	bool Render(ID3D11DeviceContext*);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	bool SetShadingParameters(ID3D11DeviceContext*);
	void RenderShader(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* m_EnviormentMap;

	ID3D11Buffer* m_sphereIndexBuffer;
	ID3D11Buffer* m_sphereVertBuffer;
	DirectX::XMFLOAT4X4 m_sphereMatrix;

	int m_NumSphereVertices;
	int m_NumSphereFaces;

	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;
	ID3D11InputLayout* m_Layout;
	ID3D11SamplerState* m_SampleState;
};

#endif