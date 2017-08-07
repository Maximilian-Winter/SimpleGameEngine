#include "SpotLightShader.h"


SpotLightShader::SpotLightShader()
{
	m_deferredVertexShader = 0;
	m_deferredPixelShader = 0;
	m_deferredHullShader = 0;
	m_deferredDomainShader = 0;
	m_deferredLightBuffer = 0;
	m_deferredSpotLightScaleBuffer = 0;

	m_forwardVertexShader = 0;
	m_forwardPixelShader = 0;
	m_forwardLayout = 0;
	m_forwardLightBuffer = 0;

}

SpotLightShader::SpotLightShader(const SpotLightShader& other)
{
}

SpotLightShader::~SpotLightShader()
{
}

bool SpotLightShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeForwardShader(device, hwnd, L"ForwardSpotLightShader.hlsl");
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and pixel shaders.
	result = InitializeDeferredShader(device, hwnd, L"DeferredSpotLightShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void SpotLightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownForwardShader();
	ShutdownDeferredShader();
	
}


bool SpotLightShader::RenderForward(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, DirectX::FXMVECTOR spotLightPosition, DirectX::FXMVECTOR spotLightColor, DirectX::FXMVECTOR spotLightDir, float spotLightRange, float spotLightInnerAngel, float spotLightOuterAngel)
{
	bool result;

	//Load the data into the shader buffers.
	result = SetForwardPSLightningParameters(deviceContext, spotLightPosition, spotLightColor, spotLightDir, spotLightRange, spotLightInnerAngel, spotLightOuterAngel);
	if (!result)
	{
		return false;
	}


	//Render the shader.
	RenderForwardShader(deviceContext, indexCount, indexStart);

	return true;
}

bool SpotLightShader::RenderDeferred(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR spotLightPosition, DirectX::FXMVECTOR spotLightColor, DirectX::FXMVECTOR spotLightDir, float spotLightRange, float spotLightInnerAngel, float spotLightOuterAngel)
{
	bool result;
	
	//Load the data into the shader buffers.
	result = SetDeferredPSLightningParameters(deviceContext, spotLightPosition, spotLightColor, spotLightDir, spotLightRange, spotLightInnerAngel, spotLightOuterAngel, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderDeferredShader(deviceContext);

	return true;
}

bool SpotLightShader::InitializeForwardShader(ID3D11Device* device, HWND hwnd, WCHAR* forwardShaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	result = ShaderHelper::CreateVertexShader(device, hwnd, forwardShaderFilename, "ForwardLightningVertexShader", &vertexShaderBuffer, &m_forwardVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, forwardShaderFilename, "ForwardSpotLightPixelShader", &pixelShaderBuffer, &m_forwardPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateConstantBuffer(device, D3D11_USAGE_DYNAMIC, sizeof(LightBufferType), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, &m_forwardLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateInputLayoutDescFromVertexShaderSignature(vertexShaderBuffer, device, &m_forwardLayout);
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

bool SpotLightShader::InitializeDeferredShader(ID3D11Device* device, HWND hwnd, WCHAR* hlslFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;

	result = ShaderHelper::CreateVertexShader(device, hwnd, hlslFilename, "DeferredSpotLightVertexShader", &vertexShaderBuffer, &m_deferredVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateHullShader(device, hwnd, hlslFilename, "DeferredSpotLightHullShader", &hullShaderBuffer, &m_deferredHullShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateDomainShader(device, hwnd, hlslFilename, "DeferredSpotLightDomainShader", &domainShaderBuffer, &m_deferredDomainShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, hlslFilename, "DeferredSpotLightPixelShader", &pixelShaderBuffer, &m_deferredPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateConstantBuffer(device, D3D11_USAGE_DYNAMIC, sizeof(LightBufferType), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, &m_deferredLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateConstantBuffer(device, D3D11_USAGE_DYNAMIC, sizeof(SpotLightScaleData), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, &m_deferredSpotLightScaleBuffer);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	hullShaderBuffer->Release();
	hullShaderBuffer = 0;

	domainShaderBuffer->Release();
	domainShaderBuffer = 0;

	return true;
}

void SpotLightShader::ShutdownForwardShader()
{

	if (m_forwardVertexShader)
	{
		m_forwardVertexShader->Release();
		m_forwardVertexShader = 0;
	}


	if (m_forwardPixelShader)
	{
		m_forwardPixelShader->Release();
		m_forwardPixelShader = 0;
	}

	if (m_forwardLayout)
	{
		m_forwardLayout->Release();
		m_forwardLayout = 0;
	}

	if (m_forwardLightBuffer)
	{
		m_forwardLightBuffer->Release();
		m_forwardLightBuffer = 0;
	}

	
}

void SpotLightShader::ShutdownDeferredShader()
{

	if(m_deferredSpotLightScaleBuffer)
	{
		m_deferredSpotLightScaleBuffer->Release();
		m_deferredSpotLightScaleBuffer = 0;
	}

	// Release the light constant buffer.
	if(m_deferredLightBuffer)
	{
		m_deferredLightBuffer->Release();
		m_deferredLightBuffer = 0;
	}

	// Release the pixel shader.
	if(m_deferredPixelShader)
	{
		m_deferredPixelShader->Release();
		m_deferredPixelShader = 0;
	}

	// Release the vertex shader.
	if(m_deferredVertexShader)
	{
		m_deferredVertexShader->Release();
		m_deferredVertexShader = 0;
	}

	if(m_deferredHullShader)
	{
		m_deferredHullShader->Release();
		m_deferredHullShader = 0;
	}

	if(m_deferredDomainShader)
	{
		m_deferredDomainShader->Release();
		m_deferredDomainShader = 0;
	}
	
	
}

bool SpotLightShader::SetForwardPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR spotLightPosition, DirectX::FXMVECTOR spotLightColor, DirectX::FXMVECTOR spotLightDir, float spotLightRange, float spotLightInnerAngel, float spotLightOuterAngel)
{
	HRESULT result;
	LightBufferType* ShaderLightningBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Convert angle in radians to sin/cos values
	float fCosInnerAngle = cosf(spotLightInnerAngel);
	float fSinOuterAngle = sinf(spotLightOuterAngel);
	float fCosOuterAngle = cosf(spotLightOuterAngel);

	//Store the data from the lightning vectors in DirectX::XMFLOAT3 vars
	DirectX::XMFLOAT3 lightPos;
	DirectX::XMFLOAT3 lightCol;
	DirectX::XMFLOAT3 lightDir;

	DirectX::XMStoreFloat3(&lightPos, spotLightPosition);
	DirectX::XMStoreFloat3(&lightCol, spotLightColor);
	DirectX::XMStoreFloat3(&lightDir, DirectX::XMVectorNegate(spotLightDir));

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_forwardLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	ShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.

	ShaderLightningBuffer->SpotLightPos = lightPos;
	ShaderLightningBuffer->SpotLightRangeRcp = 1.0f / spotLightRange;
	ShaderLightningBuffer->SpotDirToLight = lightDir;
	ShaderLightningBuffer->SpotCosOuterCone = fCosOuterAngle;
	ShaderLightningBuffer->SpotColor = ShaderHelper::GammaToLinear(lightCol);
	ShaderLightningBuffer->SpotCosConeAttRange = fCosInnerAngle - fCosOuterAngle;


	// Unlock the constant buffer.
	deviceContext->Unmap(m_forwardLightBuffer, 0);

	// Set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(2, 1, &m_forwardLightBuffer);

	return true;
}

bool SpotLightShader::SetDeferredPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR spotLightPosition, DirectX::FXMVECTOR spotLightColor, DirectX::FXMVECTOR spotLightDir, float spotLightRange, float spotLightInnerAngel, float spotLightOuterAngel, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	SpotLightScaleData* SpotLightScale;
	LightBufferType* PixelShaderLightningBuffer;

	// Convert angle in radians to sin/cos values
	float fCosInnerAngle = cosf(spotLightInnerAngel);
	float fSinOuterAngle = sinf(spotLightOuterAngel);
	float fCosOuterAngle = cosf(spotLightOuterAngel);

	// Scale matrix from the cone local space to the world angles and range
	DirectX::XMMATRIX mLightWorldScale = DirectX::XMMatrixScaling(spotLightRange, spotLightRange, spotLightRange);

	// Rotate and translate matrix from cone local space to lights world space
	DirectX::XMFLOAT3 vUp = (DirectX::XMVectorGetY(spotLightDir) > 0.9 || DirectX::XMVectorGetY(spotLightDir) < -0.9) ? DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XMVectorGetY(spotLightDir)) : DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR vecRight = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&vUp), spotLightDir);
	vecRight = DirectX::XMVector3Normalize(vecRight);
	DirectX::XMStoreFloat3(&vUp, DirectX::XMVector3Cross(spotLightDir, vecRight));
	DirectX::XMStoreFloat3(&vUp, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vUp)));
	DirectX::XMVECTOR spotLightPosPlusDir = DirectX::XMVectorAdd(spotLightPosition, spotLightDir);
	DirectX::XMVECTOR vAt = DirectX::XMVectorScale(spotLightPosPlusDir, spotLightRange);
	DirectX::XMMATRIX m_LightWorldTransRotate;
	m_LightWorldTransRotate = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 vRight;
	DirectX::XMStoreFloat3(&vRight, vecRight);

	DirectX::XMFLOAT3 spotDir;
	DirectX::XMStoreFloat3(&spotDir, spotLightDir);

	DirectX::XMFLOAT3 spotPos;
	DirectX::XMStoreFloat3(&spotPos, spotLightPosition);

	// Store m_LightWorldTransRotate matrix in XMFLOAT4X4 to access the data to set the values in the constant buffer 
	DirectX::XMFLOAT4X4 lwtr;
	DirectX::XMStoreFloat4x4(&lwtr, m_LightWorldTransRotate);

	for (int i = 0; i < 3; i++)
	{
		lwtr.m[0][i] = (&vRight.x)[i];
		lwtr.m[1][i] = (&vUp.x)[i];
		lwtr.m[2][i] = (&spotDir.x)[i];
		lwtr.m[3][i] = (&spotPos.x)[i];
	}

	m_LightWorldTransRotate = DirectX::XMLoadFloat4x4(&lwtr);

	DirectX::XMMATRIX mView = viewMatrix;
	DirectX::XMMATRIX mProj = projectionMatrix;
	DirectX::XMMATRIX mWorldViewProjection = mLightWorldScale * m_LightWorldTransRotate * mView * mProj;

	// Transpose the matrices to prepare them for the shader and store them in DirectX::XMFLOAT4X4.
	DirectX::XMFLOAT4X4 mWVP;
	DirectX::XMStoreFloat4x4(&mWVP, DirectX::XMMatrixTranspose(mWorldViewProjection));

	result = deviceContext->Map(m_deferredSpotLightScaleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.

	SpotLightScale = (SpotLightScaleData*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.

	SpotLightScale->WolrdViewProj = mWVP;
	SpotLightScale->fSinAngle = fSinOuterAngle;
	SpotLightScale->fCosAngle = fCosOuterAngle;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_deferredSpotLightScaleBuffer, 0);
	// Finally set the light constant buffer in the domain shader.
	deviceContext->DSSetConstantBuffers(0, 1, &m_deferredSpotLightScaleBuffer);

	//Store the data from the lightning vectors in DirectX::XMFLOAT3 vars
	DirectX::XMFLOAT3 lightPos;
	DirectX::XMFLOAT3 lightCol;
	DirectX::XMFLOAT3 lightDir;

	DirectX::XMStoreFloat3(&lightPos, spotLightPosition);
	DirectX::XMStoreFloat3(&lightCol, spotLightColor);
	DirectX::XMStoreFloat3(&lightDir, DirectX::XMVectorNegate(spotLightDir));

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_deferredLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	PixelShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.

	PixelShaderLightningBuffer->SpotLightPos = lightPos;
	PixelShaderLightningBuffer->SpotLightRangeRcp = 1.0f / spotLightRange;
	PixelShaderLightningBuffer->SpotDirToLight = lightDir;
	PixelShaderLightningBuffer->SpotCosOuterCone = fCosOuterAngle;
	PixelShaderLightningBuffer->SpotColor = ShaderHelper::GammaToLinear(lightCol);
	PixelShaderLightningBuffer->SpotCosConeAttRange = fCosInnerAngle - fCosOuterAngle;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_deferredLightBuffer, 0);
	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(1, 1, &m_deferredLightBuffer);

	return true;

}

void SpotLightShader::RenderForwardShader(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_forwardLayout);

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(m_forwardVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_forwardPixelShader, NULL, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, indexStart, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
}

void SpotLightShader::RenderDeferredShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout( NULL );
	deviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

	// Set the shaders
	deviceContext->VSSetShader(m_deferredVertexShader, NULL, 0);
	deviceContext->HSSetShader(m_deferredHullShader, NULL, 0);
	deviceContext->DSSetShader(m_deferredDomainShader, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(m_deferredPixelShader, NULL, 0);

	deviceContext->Draw(1, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->HSSetShader(NULL, NULL, 0);
	deviceContext->DSSetShader(NULL, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}
