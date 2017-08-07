#include "PointLightShader.h"


PointLightShader::PointLightShader()
{
	m_DeferredVertexShader = 0;
	m_DeferredPixelShader = 0;
	m_DeferredHullShader = 0;
	m_DeferredDomainShader = 0;
	m_DeferredLightBuffer = 0;
	m_DeferredPointLightScaleBuffer = 0;

	m_forwardVertexShader = 0;
	m_forwardPixelShader = 0;
	m_forwardLayout = 0;
	m_forwardLightBuffer = 0;

}


PointLightShader::PointLightShader(const PointLightShader& other)
{
}


PointLightShader::~PointLightShader()
{
}


bool PointLightShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the Forward vertex and pixel shaders.
	result = InitializeForwardShader(device, hwnd, L"ForwardPointLightShader.hlsl");
	if (!result)
	{
		return false;
	}

	// Initialize the Deferred vertex and pixel shaders.
	result = InitializeDeferredShader(device, hwnd, L"DeferredPointLightShader.hlsl");
	if(!result)
	{
		return false;
	}


	return true;
}


void PointLightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownDeferredShader();
	ShutdownForwardShader();

	
}

bool PointLightShader::RenderForward(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, DirectX::FXMVECTOR lightPosition, DirectX::FXMVECTOR lightColor, float lightRange)
{
	bool result;

	//Load the data into the shader buffers.
	result = SetForwardPSLightningParameters(deviceContext, lightPosition, lightColor, lightRange);
	if (!result)
	{
		return false;
	}

	//Render the shader.
	RenderForwardShader(deviceContext, indexCount, indexStart);

	return true;
}


bool PointLightShader::RenderDeferred(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR lightPosition, DirectX::FXMVECTOR lightColor, float lightRange)
{
	bool result;

	//Load the data into the shader buffers.
	result = SetDeferredPSLightningParameters(deviceContext, lightPosition, lightColor, lightRange, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	//Render the shader.
	RenderDeferredShader(deviceContext);

	return true;
}


bool PointLightShader::InitializeForwardShader(ID3D11Device* device, HWND hwnd, WCHAR* forwardShaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	result = ShaderHelper::CreateVertexShader(device, hwnd, forwardShaderFilename, "ForwardLightningVertexShader", &vertexShaderBuffer, &m_forwardVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, forwardShaderFilename, "ForwardPointLightPixelShader", &pixelShaderBuffer, &m_forwardPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateInputLayoutDescFromVertexShaderSignature(vertexShaderBuffer, device, &m_forwardLayout);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateConstantBuffer(device, D3D11_USAGE_DYNAMIC, sizeof(LightBufferType), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, &m_forwardLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}


bool PointLightShader::InitializeDeferredShader(ID3D11Device* device, HWND hwnd, WCHAR* hlslFilename)
{
	HRESULT result;

	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;


	result = ShaderHelper::CreateVertexShader(device, hwnd, hlslFilename, "DeferredPointLightVertexShader", &vertexShaderBuffer, &m_DeferredVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateHullShader(device, hwnd, hlslFilename, "DeferredPointLightHullShader", &hullShaderBuffer, &m_DeferredHullShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateDomainShader(device, hwnd, hlslFilename, "DeferredPointLightDomainShader", &domainShaderBuffer, &m_DeferredDomainShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, hlslFilename, "DeferredPointLightPixelShader", &pixelShaderBuffer, &m_DeferredPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateConstantBuffer(device, D3D11_USAGE_DYNAMIC, sizeof(LightBufferType), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, &m_DeferredLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateConstantBuffer(device, D3D11_USAGE_DYNAMIC, sizeof(PointLightScaleMatrix), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0, &m_DeferredPointLightScaleBuffer);
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


void PointLightShader::ShutdownForwardShader()
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


void PointLightShader::ShutdownDeferredShader()
{
	if(m_DeferredLightBuffer)
	{
		m_DeferredLightBuffer->Release();
		m_DeferredLightBuffer = 0;
	}
	if(m_DeferredPointLightScaleBuffer)
	{
		m_DeferredPointLightScaleBuffer->Release();
		m_DeferredPointLightScaleBuffer = 0;
	}
	if(m_DeferredPixelShader)
	{
		m_DeferredPixelShader->Release();
		m_DeferredPixelShader = 0;
	}
	if(m_DeferredVertexShader)
	{
		m_DeferredVertexShader->Release();
		m_DeferredVertexShader = 0;
	}
	if(m_DeferredHullShader)
	{
		m_DeferredHullShader->Release();
		m_DeferredHullShader = 0;
	}
	if(m_DeferredDomainShader)
	{
		m_DeferredDomainShader->Release();
		m_DeferredDomainShader = 0;
	}
}

bool PointLightShader::SetForwardPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR lightPosition, DirectX::FXMVECTOR lightColor, float lightRange)
{
	HRESULT result;
	LightBufferType* ShaderLightningBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//Store the data from the lightning vectors in DirectX::XMFLOAT3 vars
	DirectX::XMFLOAT3 lightPos;
	DirectX::XMFLOAT3 lightCol;

	DirectX::XMStoreFloat3(&lightPos, lightPosition);
	DirectX::XMStoreFloat3(&lightCol, lightColor);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_forwardLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	ShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	ShaderLightningBuffer->lightPosition = lightPos;
	ShaderLightningBuffer->lightRange = 1.0f / lightRange;
	ShaderLightningBuffer->lightColor = ShaderHelper::GammaToLinear(lightCol);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_forwardLightBuffer, 0);

	// Set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(2, 1, &m_forwardLightBuffer);

	return true;
}

bool PointLightShader::SetDeferredPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR lightPosition, DirectX::FXMVECTOR lightColor, float lightRange, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	PointLightScaleMatrix* PointlightScale;
	LightBufferType* PixelShaderLightningBuffer;

	DirectX::XMMATRIX mLightWorldScale = DirectX::XMMatrixScaling(lightRange, lightRange, lightRange);

	DirectX::XMMATRIX mLightWorldTrans = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(lightPosition), DirectX::XMVectorGetY(lightPosition), DirectX::XMVectorGetZ(lightPosition));

	DirectX::XMMATRIX mView = viewMatrix;
	DirectX::XMMATRIX mProj = projectionMatrix;
	DirectX::XMMATRIX mWorldViewProjection = mLightWorldScale * mLightWorldTrans * mView * mProj;

	// Transpose the matrices to prepare them for the shader and store them in DirectX::XMFLOAT4X4.
	DirectX::XMFLOAT4X4 LightMat;
	DirectX::XMStoreFloat4x4(&LightMat, DirectX::XMMatrixTranspose(mWorldViewProjection));

	result = deviceContext->Map(m_DeferredPointLightScaleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.

	PointlightScale = (PointLightScaleMatrix*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	PointlightScale->wvp = LightMat;
	// Unlock the constant buffer.
	deviceContext->Unmap(m_DeferredPointLightScaleBuffer, 0);
	// Finally set the light constant buffer in the domain shader.
	deviceContext->DSSetConstantBuffers(0, 1, &m_DeferredPointLightScaleBuffer);

	//Store the data from the lightning vectors in DirectX::XMFLOAT3 vars
	DirectX::XMFLOAT3 lightPos;
	DirectX::XMFLOAT3 lightCol;

	DirectX::XMStoreFloat3(&lightPos, lightPosition);
	DirectX::XMStoreFloat3(&lightCol, lightColor);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_DeferredLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	PixelShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	PixelShaderLightningBuffer->lightPosition = lightPos;
	PixelShaderLightningBuffer->lightRange = 1.0f / lightRange;
	PixelShaderLightningBuffer->lightColor = ShaderHelper::GammaToLinear(lightCol);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_DeferredLightBuffer, 0);
	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(1, 1, &m_DeferredLightBuffer);

	return true;

}

void PointLightShader::RenderForwardShader(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart)
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

void PointLightShader::RenderDeferredShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout( NULL );
	deviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

	// Set the shaders
	deviceContext->VSSetShader(m_DeferredVertexShader, NULL, 0);
	deviceContext->HSSetShader(m_DeferredHullShader, NULL, 0);
	deviceContext->DSSetShader(m_DeferredDomainShader, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(m_DeferredPixelShader, NULL, 0);

	deviceContext->Draw(2, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->HSSetShader(NULL, NULL, 0);
	deviceContext->DSSetShader(NULL, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);

	ID3D11ShaderResourceView *arrRV[1] = { NULL };
	deviceContext->PSSetShaderResources(4, 1, arrRV);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
