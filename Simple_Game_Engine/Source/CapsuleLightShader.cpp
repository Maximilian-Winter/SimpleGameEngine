#include "CapsuleLightShader.h"


CapsuleLightShader::CapsuleLightShader()
{
	m_deferredVertexShader = 0;
	m_deferredPixelShader = 0;
	m_deferredHullShader = 0;
	m_deferredDomainShader = 0;

	m_deferredLayout = 0;
	m_deferredSampleState = 0;
	m_deferredLightBuffer = 0;
	m_deferredCapsuleLightScaleBuffer = 0;

	m_forwardVertexShader = 0;
	m_forwardPixelShader = 0;
	m_forwardLayout = 0;
	m_forwardSampleState = 0;
	m_forwardLightBuffer = 0;

}


CapsuleLightShader::CapsuleLightShader(const CapsuleLightShader& other)
{
}


CapsuleLightShader::~CapsuleLightShader()
{
}


bool CapsuleLightShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeForwardShader(device, hwnd, L"ForwardCapsuleLightShader.hlsl");
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and pixel shaders.
	result = InitializeDeferredShader(device, hwnd, L"DeferredCapsuleLightShader.hlsl");
	if(!result)
	{
		return false;
	}

	return true;
}


void CapsuleLightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownForwardShader();
	ShutdownDeferredShader();
	
	return;
}


const DirectX::XMFLOAT3 CapsuleLightShader::GammaToLinear(const DirectX::XMFLOAT3& color)
{
	return DirectX::XMFLOAT3(color.x * color.x, color.y * color.y, color.z * color.z);
}


bool CapsuleLightShader::RenderForward(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, DirectX::FXMVECTOR capsuleLightPosition,
	DirectX::FXMVECTOR capsuleLightColor, DirectX::FXMVECTOR capsuleLightDir, float capsuleLightLength, float capsuleLightRange)
{
	bool result;

	//Load the data into the shader buffers.
	result = SetForwardPSLightningParameters(deviceContext, capsuleLightPosition, capsuleLightColor, capsuleLightDir, capsuleLightLength, capsuleLightRange);
	if (!result)
	{
		return false;
	}

	//Render the shader.
	RenderForwardShader(deviceContext, indexCount, indexStart);

	return true;
}



bool CapsuleLightShader::RenderDeferred(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR capsuleLightPosition, DirectX::FXMVECTOR capsuleLightColor, DirectX::FXMVECTOR capsuleLightDir, float capsuleLightLength, float capsuleLightRange)
{
	bool result;
	
	//Load the data into the shader buffers.
	result = SetDeferredPSLightningParameters(deviceContext, capsuleLightPosition, capsuleLightColor, capsuleLightDir, capsuleLightLength, capsuleLightRange, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderDeferredShader(deviceContext);

	return true;
}


bool CapsuleLightShader::InitializeForwardShader(ID3D11Device* device, HWND hwnd, WCHAR* forwardShaderFilename)
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
	result = D3DCompileFromFile(forwardShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardCapsuleLightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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
	result = D3DCompileFromFile(forwardShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardCapsuleLightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
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


bool CapsuleLightShader::InitializeDeferredShader(ID3D11Device* device, HWND hwnd, WCHAR* hlslFilename)
{
	HRESULT result;

	ID3D10Blob* errorMessage;

	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	ID3D10Blob* hullShaderBuffer;
	ID3D10Blob* domainShaderBuffer;

	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC CapsuleLightScaleDesc;


	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

    // Compile the vertex shader code.
	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CapsuleLightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, hlslFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, hlslFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CapsuleLightHullShader", "hs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &hullShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, hlslFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, hlslFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CapsuleLightDomainShader", "ds_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &domainShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, hlslFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, hlslFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

    // Compile the pixel shader code.
	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "CapsuleLightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, hlslFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, hlslFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_deferredVertexShader);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &m_deferredHullShader);
	if(FAILED(result))
	{
		return false;
	}
 
	
	result = device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &m_deferredDomainShader);
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



	// Release the shader buffers since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	hullShaderBuffer->Release();
	hullShaderBuffer = 0;

	domainShaderBuffer->Release();
	domainShaderBuffer = 0;

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


	CapsuleLightScaleDesc.Usage = D3D11_USAGE_DYNAMIC;
	CapsuleLightScaleDesc.ByteWidth = sizeof(CapsuleLightScaleData);
	CapsuleLightScaleDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CapsuleLightScaleDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CapsuleLightScaleDesc.MiscFlags = 0;
	CapsuleLightScaleDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&CapsuleLightScaleDesc, NULL, &m_deferredCapsuleLightScaleBuffer);
	if(FAILED(result))
	{
		return false;
	}


	return true;
}


void CapsuleLightShader::ShutdownForwardShader()
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


void CapsuleLightShader::ShutdownDeferredShader()
{
	if(m_deferredCapsuleLightScaleBuffer)
	{
		m_deferredCapsuleLightScaleBuffer->Release();
		m_deferredCapsuleLightScaleBuffer = 0;
	}

	// Release the light constant buffer.
	if(m_deferredLightBuffer)
	{
		m_deferredLightBuffer->Release();
		m_deferredLightBuffer = 0;
	}

	// Release the sampler state.
	if(m_deferredSampleState)
	{
		m_deferredSampleState->Release();
		m_deferredSampleState = 0;
	}

	// Release the layout.
	if(m_deferredLayout)
	{
		m_deferredLayout->Release();
		m_deferredLayout = 0;
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
	
	return;
}


void CapsuleLightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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

bool CapsuleLightShader::SetForwardPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR capsuleLightPosition, DirectX::FXMVECTOR capsuleLightColor, DirectX::FXMVECTOR capsuleLightDir, float capsuleLightLength, float capsuleLightRange)
{
	HRESULT result;
	LightBufferType* ShaderLightningBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//Store the data from the lightning vectors in DirectX::XMFLOAT3 vars
	DirectX::XMFLOAT3 lightDir;
	DirectX::XMFLOAT3 lightCol;
	DirectX::XMFLOAT3 lightPos;

	DirectX::XMStoreFloat3(&lightDir, capsuleLightDir);
	DirectX::XMStoreFloat3(&lightCol, capsuleLightColor);
	DirectX::XMStoreFloat3(&lightPos, capsuleLightPosition);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_forwardLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	ShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	ShaderLightningBuffer->CapsuleLightPos = lightPos;
	ShaderLightningBuffer->CapsuleLightRange = 1.0f / capsuleLightRange;
	ShaderLightningBuffer->CapsuleDir = lightDir;
	ShaderLightningBuffer->CapsuleLen = capsuleLightLength;
	ShaderLightningBuffer->CapsuleColor = GammaToLinear(lightCol);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_forwardLightBuffer, 0);

	// Set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(2, 1, &m_forwardLightBuffer);

	return true;
}

bool CapsuleLightShader::SetDeferredPSLightningParameters(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR capsuleLightPosition, DirectX::FXMVECTOR capsuleLightColor, DirectX::FXMVECTOR capsuleLightDir, float capsuleLightLength, float capsuleLightRange, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	CapsuleLightScaleData* DomainShaderLightBuffer;
	LightBufferType* PixelShaderLightningBuffer;

	DirectX::XMFLOAT3 LightColor;
	DirectX::XMStoreFloat3(&LightColor, capsuleLightColor);

	DirectX::XMFLOAT3 LightPosition;
	DirectX::XMStoreFloat3(&LightPosition, capsuleLightPosition);

	DirectX::XMFLOAT3 LightDir;
	DirectX::XMStoreFloat3(&LightDir, capsuleLightDir);

	// Rotate and translate matrix from capsule local space to lights world space

	DirectX::XMFLOAT3 vUp = (LightDir.y > 0.9 || LightDir.y < -0.9) ? DirectX::XMFLOAT3(0.0f, 0.0f, LightDir.y) : DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR vecRight = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&vUp), capsuleLightDir);
	vecRight = DirectX::XMVector3Normalize(vecRight);
	DirectX::XMStoreFloat3(&vUp, DirectX::XMVector3Cross(capsuleLightDir, vecRight));
	DirectX::XMStoreFloat3(&vUp, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vUp)));

	DirectX::XMVECTOR capsuleLightPositionAddHalf = DirectX::XMVectorAdd(capsuleLightPosition, DirectX::XMVectorReplicate(0.5f));
	DirectX::XMVECTOR capsuleLightDirectionScaleLength = DirectX::XMVectorScale(capsuleLightDir, capsuleLightLength);
	DirectX::XMVECTOR vecCenterPos = DirectX::XMVectorMultiply(capsuleLightPositionAddHalf, capsuleLightDirectionScaleLength);
	DirectX::XMVECTOR vAt = DirectX::XMVectorAdd(vecCenterPos, capsuleLightDirectionScaleLength);
	DirectX::XMMATRIX m_LightWorldTransRotate;
	m_LightWorldTransRotate = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 vRight;
	DirectX::XMStoreFloat3(&vRight, vecRight);

	DirectX::XMFLOAT3 vCenterPos;
	DirectX::XMStoreFloat3(&vCenterPos, vecCenterPos);

	// Store m_LightWorldTransRotate matrix in XMFLOAT4X4 to access the data to set the values in the constant buffer 
	DirectX::XMFLOAT4X4 lwtr;
	DirectX::XMStoreFloat4x4(&lwtr, m_LightWorldTransRotate);

	for (int i = 0; i < 3; i++)
	{
		lwtr.m[0][i] = (&vRight.x)[i];
		lwtr.m[1][i] = (&vUp.x)[i];
		lwtr.m[2][i] = (&LightDir.x)[i];
		lwtr.m[3][i] = (&vCenterPos.x)[i];
	}

	m_LightWorldTransRotate = DirectX::XMLoadFloat4x4(&lwtr);

	DirectX::XMMATRIX mView = viewMatrix;
	DirectX::XMMATRIX mProj = projectionMatrix;
	DirectX::XMMATRIX mWorldViewProjection = m_LightWorldTransRotate * mView * mProj;

	// Transpose the matrices to prepare them for the shader and store them in DirectX::XMFLOAT4X4.
	DirectX::XMFLOAT4X4 mWVP;
	DirectX::XMStoreFloat4x4(&mWVP, DirectX::XMMatrixTranspose(mWorldViewProjection));

	result = deviceContext->Map(m_deferredCapsuleLightScaleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.

	DomainShaderLightBuffer = (CapsuleLightScaleData*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.

	DomainShaderLightBuffer->WolrdViewProj = mWVP;
	DomainShaderLightBuffer->HalfCapsuleLen = 0.5f * capsuleLightLength;
	DomainShaderLightBuffer->CapsuleRange = capsuleLightRange;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_deferredCapsuleLightScaleBuffer, 0);
	// Finally set the light constant buffer in the domain shader with the updated values.
	deviceContext->DSSetConstantBuffers(0, 1, &m_deferredCapsuleLightScaleBuffer);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_deferredLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	// Get a pointer to the data in the constant buffer.
	PixelShaderLightningBuffer = (LightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.

	PixelShaderLightningBuffer->CapsuleLightPos = LightPosition;
	PixelShaderLightningBuffer->CapsuleLightRange = 1.0f / capsuleLightRange;
	PixelShaderLightningBuffer->CapsuleDir = LightDir;
	PixelShaderLightningBuffer->CapsuleLen = capsuleLightLength;
	PixelShaderLightningBuffer->CapsuleColor = GammaToLinear(LightColor);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_deferredLightBuffer, 0);
	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(1, 1, &m_deferredLightBuffer);

	return true;

}

void CapsuleLightShader::RenderForwardShader(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart)
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

void CapsuleLightShader::RenderDeferredShader(ID3D11DeviceContext* deviceContext)
{


	deviceContext->PSSetSamplers(0, 1, &m_deferredSampleState);

	deviceContext->IASetInputLayout( NULL );
	deviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

	// Set the shaders
	deviceContext->VSSetShader(m_deferredVertexShader, NULL, 0);
	deviceContext->HSSetShader(m_deferredHullShader, NULL, 0);
	deviceContext->DSSetShader(m_deferredDomainShader, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(m_deferredPixelShader, NULL, 0);

	deviceContext->Draw(2, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->HSSetShader(NULL, NULL, 0);
	deviceContext->DSSetShader(NULL, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
