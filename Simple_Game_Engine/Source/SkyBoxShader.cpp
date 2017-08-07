#include "SkyboxShader.h"

SkyboxShader::SkyboxShader()
{
	m_VertexShader = 0;
	m_PixelShader = 0;
	m_Layout = 0;
}


SkyboxShader::SkyboxShader(const SkyboxShader& other)
{
}


SkyboxShader::~SkyboxShader()
{
	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}


	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}
}

bool SkyboxShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the forward vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"SkyboxShader.hlsl");
	if (!result)
	{
		return false;
	}

	std::string skyboxName = "data/skymap.dds";

	//Convert the specular integration filename into wcstring
	size_t tSkyboxFilename = skyboxName.length() + 1;
	wchar_t * wcSkyboxFilename = new wchar_t[tSkyboxFilename];

	size_t SpecularIntegrationFilenameConvertedChars = 0;
	mbstowcs_s(&SpecularIntegrationFilenameConvertedChars, wcSkyboxFilename, tSkyboxFilename, skyboxName.c_str(), _TRUNCATE);

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, wcSkyboxFilename, nullptr, &m_EnviormentMap);
	if (FAILED(result))
	{
		return false;
	}

	//Create sphere for the sky box.

	int LatLines = 10;
	int LongLines = 10;

	m_NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	m_NumSphereFaces = ((LatLines - 3)*(LongLines)* 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<VertexType> vertices(m_NumSphereVertices);

	DirectX::XMVECTOR currVertPos = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].position.x = 0.0f;
	vertices[0].position.y = 0.0f;
	vertices[0].position.z = 1.0f;

	for (DWORD i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		DirectX::XMMATRIX Rotationx = DirectX::XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			DirectX::XMMATRIX Rotationy = DirectX::XMMatrixRotationZ(sphereYaw);
			currVertPos = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = DirectX::XMVector3Normalize(currVertPos);
			vertices[i*LongLines + j + 1].position.x = DirectX::XMVectorGetX(currVertPos);
			vertices[i*LongLines + j + 1].position.y = DirectX::XMVectorGetY(currVertPos);
			vertices[i*LongLines + j + 1].position.z = DirectX::XMVectorGetZ(currVertPos);
		}
	}

	vertices[m_NumSphereVertices - 1].position.x = 0.0f;
	vertices[m_NumSphereVertices - 1].position.y = 0.0f;
	vertices[m_NumSphereVertices - 1].position.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	result = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_sphereVertBuffer);


	std::vector<DWORD> indices(m_NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i*LongLines + j + 1;
			indices[k + 1] = i*LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i*LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = m_NumSphereVertices - 1;
		indices[k + 1] = (m_NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (m_NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = m_NumSphereVertices - 1;
	indices[k + 1] = (m_NumSphereVertices - 1) - LongLines;
	indices[k + 2] = m_NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * m_NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&indexBufferDesc, &iinitData, &m_sphereIndexBuffer);

	return true;
}

bool SkyboxShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = ShaderHelper::CreateVertexShader(device, hwnd, shaderFilename, "SkyboxVertexShader", &vertexShaderBuffer, &m_VertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, shaderFilename, "SkyboxPixelShader", &pixelShaderBuffer, &m_PixelShader);
	if (FAILED(result))
	{
		return false;
	}
	
	result = ShaderHelper::CreateInputLayoutDescFromVertexShaderSignature(vertexShaderBuffer, device, &m_Layout);
	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}

bool  SkyboxShader::Render(ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = SetShadingParameters(deviceContext);
	if (!result)
	{
		return false;
	}

	RenderShader(deviceContext);

	return true;
}

bool  SkyboxShader::SetShadingParameters(ID3D11DeviceContext* deviceContext)
{
	// Set shader enviorment map as resource in the pixel shader.
	deviceContext->PSSetShaderResources(4, 1, &m_EnviormentMap);

	//Set the grounds vertex buffer
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	//Set the spheres index buffer
	deviceContext->IASetIndexBuffer(m_sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//Set the spheres vertex buffer
	deviceContext->IASetVertexBuffers(0, 1, &m_sphereVertBuffer, &stride, &offset);

	return true;
}

void  SkyboxShader::RenderShader(ID3D11DeviceContext* deviceContext)
{
	//Set the VS and PS shader
	deviceContext->VSSetShader(m_VertexShader, 0, 0);
	deviceContext->PSSetShader(m_PixelShader, 0, 0);

	deviceContext->DrawIndexed(m_NumSphereFaces * 3, 0, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
}