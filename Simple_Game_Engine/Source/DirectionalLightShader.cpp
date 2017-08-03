#include "DirectionalLightShader.h"


DirectionalLightShader::DirectionalLightShader()
{
	m_deferredVertexShader = 0;
	m_deferredPixelShader = 0;
	m_deferredLayout = 0;
	m_deferredSampleState = 0;
	m_deferredLightBuffer = 0;

	m_forwardVertexShader = 0;
	m_forwardPixelShader = 0;
	m_forwardLayout = 0;
	m_forwardSampleState = 0;
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
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownDeferredShader();
	ShutdownForwardShader();

	return;
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
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC lightBufferDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(forwardShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardDirectionalLightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, forwardShaderFilename);
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
			OutputShaderErrorMessage(errorMessage, hwnd, forwardShaderFilename);
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

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_forwardLayout);
	if (FAILED(result))
	{
		return false;
	}


	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_forwardSampleState);
	if (FAILED(result))
	{
		return false;
	}

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
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
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
			OutputShaderErrorMessage(errorMessage, hwnd, deferredShaderFilename);
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
			OutputShaderErrorMessage(errorMessage, hwnd, deferredShaderFilename);
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

	// Create the vertex input layout description.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
					   &m_deferredLayout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_deferredSampleState);
	if(FAILED(result))
	{
		return false;
	}


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

	if (m_forwardSampleState)
	{
		m_forwardSampleState->Release();
		m_forwardSampleState = 0;
	}
	
	if (m_forwardLightBuffer)
	{
		m_forwardLightBuffer->Release();
		m_forwardLightBuffer = 0;
	}

	return;
}

void DirectionalLightShader::ShutdownDeferredShader()
{


	// Release the light constant buffer.
	if (m_deferredLightBuffer)
	{
		m_deferredLightBuffer->Release();
		m_deferredLightBuffer = 0;
	}

	// Release the sampler state.
	if (m_deferredSampleState)
	{
		m_deferredSampleState->Release();
		m_deferredSampleState = 0;
	}

	// Release the layout.
	if (m_deferredLayout)
	{
		m_deferredLayout->Release();
		m_deferredLayout = 0;
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

	return;
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
	ShaderLightningBuffer->lightColor = GammaToLinear(lightCol);
	ShaderLightningBuffer->AmbientDown = GammaToLinear(DownCol);
	ShaderLightningBuffer->AmbientRange = GammaToLinear(ColorRange);

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
	ShaderLightningBuffer->lightColor = GammaToLinear(lightCol);
	ShaderLightningBuffer->AmbientDown = GammaToLinear(DownCol);
	ShaderLightningBuffer->AmbientRange = GammaToLinear(ColorRange);


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

	// Set the sampler states in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_forwardSampleState);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, indexStart, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);



	return;
}

void DirectionalLightShader::RenderDeferredShader(ID3D11DeviceContext* deviceContext)
{

	// Primitive settings

	deviceContext->PSSetSamplers(0, 1, &m_deferredSampleState);

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
	


	return;
}

void DirectionalLightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

DirectX::XMFLOAT3 DirectionalLightShader::GammaToLinear(const DirectX::XMFLOAT3& color)
{
	return DirectX::XMFLOAT3(color.x * color.x, color.y * color.y, color.z * color.z);
}

