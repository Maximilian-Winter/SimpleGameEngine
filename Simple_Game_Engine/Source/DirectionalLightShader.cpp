#include "DirectionalLightShader.h"


DirectionalLightShader::DirectionalLightShader()
{
	m_deferredVertexShader = 0;
	m_deferredPixelShader = 0;
	m_deferredLightBuffer = 0;

	m_forwardVertexShader = 0;
	m_forwardPixelShader = 0;
	m_forwardLayout = 0;
	m_forwardLightBuffer = 0;

}


DirectionalLightShader::DirectionalLightShader(const DirectionalLightShader& other)
{
}


DirectionalLightShader::~DirectionalLightShader()
{
}


bool DirectionalLightShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeForwardShader(device, hwnd, L"ForwardDirectionalLightShader.hlsl");
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and pixel shaders.
	result = InitializeDeferredShader(device, hwnd, L"DeferredDirectionalLightShader.hlsl");
	if(!result)
	{
		return false;
	}

	return true;
}

void DirectionalLightShader::Shutdown()
{
	ShutdownDeferredShader();
	ShutdownForwardShader();
}

bool DirectionalLightShader::RenderForward(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, DirectX::FXMVECTOR lightDirection, DirectX::FXMVECTOR lightColor, DirectX::FXMVECTOR AmbientDown, DirectX::CXMVECTOR AmbientUp)
{
	bool result;

	//Load the data into the shader buffers.
	result = SetForwardPSLightningParameters(deviceContext, lightDirection, lightColor, AmbientDown, AmbientUp);
	if (!result)
	{
		return false;
	}

	//Render the shader.
	RenderForwardShader(deviceContext, indexCount, indexStart);

	return true;
}

bool DirectionalLightShader::RenderDeferred(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR lightDirection, DirectX::FXMVECTOR lightColor, DirectX::FXMVECTOR AmbientDown, DirectX::CXMVECTOR AmbientUp)
{
	bool result;
	
	//Load the data into the shader buffers.
	result = SetDeferredPSLightningParameters(deviceContext, lightDirection, lightColor, AmbientDown, AmbientUp);
	if (!result)
	{
		return false;
	}

	//Render the shader.
	RenderDeferredShader(deviceContext);

	return true;
}

bool DirectionalLightShader::InitializeForwardShader(ID3D11Device* device, HWND hwnd, WCHAR* forwardShaderFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_BUFFER_DESC lightBufferDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(forwardShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardLightningVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			ShaderHelper::OutputShaderErrorMessage(errorMessage, hwnd, forwardShaderFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, forwardShaderFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(forwardShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardDirectionalLightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			ShaderHelper::OutputShaderErrorMessage(errorMessage, hwnd, forwardShaderFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, forwardShaderFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_forwardVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_forwardPixelShader);
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

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_forwardLightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool DirectionalLightShader::InitializeDeferredShader(ID3D11Device* device, HWND hwnd, WCHAR* deferredShaderFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_BUFFER_DESC lightBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(deferredShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "DeferredDirectionalLightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			ShaderHelper::OutputShaderErrorMessage(errorMessage, hwnd, deferredShaderFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, deferredShaderFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(deferredShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "DeferredDirectionalLightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			ShaderHelper::OutputShaderErrorMessage(errorMessage, hwnd, deferredShaderFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, deferredShaderFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_deferredVertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_deferredPixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_deferredLightBuffer);
	if(FAILED(result))
	{
		return false;
	}
	
	return true;
}

void DirectionalLightShader::ShutdownForwardShader()
{
	// Release all directx elemnts
	
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

void DirectionalLightShader::ShutdownDeferredShader()
{


	// Release the light constant buffer.
	if (m_deferredLightBuffer)
	{
		m_deferredLightBuffer->Release();
		m_deferredLightBuffer = 0;
	}

	// Release the pixel shader.
	if (m_deferredPixelShader)
	{
		m_deferredPixelShader->Release();
		m_deferredPixelShader = 0;
	}

	// Release the vertex shader.
	if (m_deferredVertexShader)
	{
		m_deferredVertexShader->Release();
		m_deferredVertexShader = 0;
	}

	
}

bool DirectionalLightShader::SetForwardPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR lightDirection, DirectX::FXMVECTOR lightColor, DirectX::FXMVECTOR AmbientDown, DirectX::CXMVECTOR AmbientUp)
{
	HRESULT result;
	LightBufferType* ShaderLightningBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//Store the data from the lightning vectors in DirectX::XMFLOAT3 vars
	DirectX::XMFLOAT3 lightDir;
	DirectX::XMFLOAT3 lightCol;
	DirectX::XMFLOAT3 DownCol;
	DirectX::XMFLOAT3 UpCol;
	DirectX::XMFLOAT3 ColorRange;

	DirectX::XMVECTOR TempLightDir;

	TempLightDir = DirectX::XMVector3Normalize(lightDirection);
	DirectX::XMStoreFloat3(&lightDir, DirectX::XMVectorNegate(TempLightDir));
	DirectX::XMStoreFloat3(&lightCol, lightColor);
	DirectX::XMStoreFloat3(&DownCol, AmbientDown);
	DirectX::XMStoreFloat3(&UpCol, AmbientUp);
	DirectX::XMStoreFloat3(&ColorRange, DirectX::XMVectorSubtract(AmbientUp, AmbientDown));

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_forwardLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	ShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	ShaderLightningBuffer->lightDirection = lightDir;
	ShaderLightningBuffer->lightColor = ShaderHelper::GammaToLinear(lightCol);
	ShaderLightningBuffer->AmbientDown = ShaderHelper::GammaToLinear(DownCol);
	ShaderLightningBuffer->AmbientRange = ShaderHelper::GammaToLinear(ColorRange);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_forwardLightBuffer, 0);

	// Set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(2, 1, &m_forwardLightBuffer);

	return true;
}

bool DirectionalLightShader::SetDeferredPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR lightDirection, DirectX::FXMVECTOR lightColor, DirectX::FXMVECTOR AmbientDown, DirectX::CXMVECTOR AmbientUp)
{
	HRESULT result;
	LightBufferType* ShaderLightningBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//Store the data from the lightning vectors in DirectX::XMFLOAT3 vars
	DirectX::XMFLOAT3 lightDir;
	DirectX::XMFLOAT3 lightCol;
	DirectX::XMFLOAT3 DownCol;
	DirectX::XMFLOAT3 UpCol;
	DirectX::XMFLOAT3 ColorRange;

	DirectX::XMVECTOR TempLightDir;

	TempLightDir = DirectX::XMVector3Normalize(lightDirection);
	DirectX::XMStoreFloat3(&lightDir, DirectX::XMVectorNegate(TempLightDir));
	DirectX::XMStoreFloat3(&lightCol, lightColor);
	DirectX::XMStoreFloat3(&DownCol, AmbientDown);
	DirectX::XMStoreFloat3(&UpCol, AmbientUp);
	DirectX::XMStoreFloat3(&ColorRange, DirectX::XMVectorSubtract(AmbientUp, AmbientDown));

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_deferredLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	ShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	ShaderLightningBuffer->lightDirection = lightDir;
	ShaderLightningBuffer->lightColor = ShaderHelper::GammaToLinear(lightCol);
	ShaderLightningBuffer->AmbientDown = ShaderHelper::GammaToLinear(DownCol);
	ShaderLightningBuffer->AmbientRange = ShaderHelper::GammaToLinear(ColorRange);


	// Unlock the constant buffer.
	deviceContext->Unmap(m_deferredLightBuffer, 0);

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(1, 1, &m_deferredLightBuffer);

	return true;
}

void DirectionalLightShader::RenderForwardShader(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart)
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

void DirectionalLightShader::RenderDeferredShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout(NULL);
	deviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Set the shaders
	deviceContext->VSSetShader(m_deferredVertexShader, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(m_deferredPixelShader, NULL, 0);

	deviceContext->Draw(4, 0);

	
	ID3D11ShaderResourceView *arrRV[1] = { NULL };
	deviceContext->PSSetShaderResources(4, 1, arrRV);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}