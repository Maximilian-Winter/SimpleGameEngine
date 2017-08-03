#include "IBLShader.h"

IBLShader::IBLShader()
{
	m_forwardVertexShader = 0;
	m_forwardPixelShader = 0;
	m_forwardLayout = 0;
	m_forwardSampleState = 0;

	m_ClampSampleState = 0;
	m_AnisoSampleState = 0;

	m_deferredVertexShader = 0;
	m_deferredPixelShader = 0;
	m_deferredSampleState = 0;

}


IBLShader::IBLShader(const IBLShader& other)
{
}


IBLShader::~IBLShader()
{
}

bool IBLShader::InitializeForwardShader(ID3D11Device* device, HWND hwnd, WCHAR* forwardShaderFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_SAMPLER_DESC AnsioamplerDesc;
	D3D11_SAMPLER_DESC ClampSamplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(forwardShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardIBLVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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
	result = D3DCompileFromFile(forwardShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardIBLPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
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
	samplerDesc.BorderColor[0] = 0.0;
	samplerDesc.BorderColor[1] = 0.0;
	samplerDesc.BorderColor[2] = 0.0;
	samplerDesc.BorderColor[3] = 0.0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_forwardSampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	AnsioamplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	AnsioamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	AnsioamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	AnsioamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	AnsioamplerDesc.MipLODBias = 0.0f;
	AnsioamplerDesc.MaxAnisotropy = 16;
	AnsioamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	AnsioamplerDesc.BorderColor[0] = 0.0;
	AnsioamplerDesc.BorderColor[1] = 0.0;
	AnsioamplerDesc.BorderColor[2] = 0.0;
	AnsioamplerDesc.BorderColor[3] = 0.0;
	AnsioamplerDesc.MinLOD = 0;
	AnsioamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&AnsioamplerDesc, &m_AnisoSampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	ClampSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	ClampSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	ClampSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	ClampSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	ClampSamplerDesc.MipLODBias = 0.0f;
	ClampSamplerDesc.MaxAnisotropy = 1;
	ClampSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	ClampSamplerDesc.BorderColor[0] = 0.0;
	ClampSamplerDesc.BorderColor[1] = 0.0;
	ClampSamplerDesc.BorderColor[2] = 0.0;
	ClampSamplerDesc.BorderColor[3] = 0.0;
	ClampSamplerDesc.MinLOD = 0;
	ClampSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&ClampSamplerDesc, &m_ClampSampleState);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool IBLShader::InitializeDeferredShader(ID3D11Device* device, HWND hwnd, WCHAR* deferredShaderFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(deferredShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "DeferredIBLVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
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
	result = D3DCompileFromFile(deferredShaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "DeferredIBLPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
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
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_deferredPixelShader);
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
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool IBLShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the forward vertex and pixel shaders.
	result = InitializeForwardShader(device, hwnd, L"ForwardIBLShader.hlsl");
	if (!result)
	{
		return false;
	}

	// Initialize the deferred vertex and pixel shaders.
	result = InitializeDeferredShader(device, hwnd, L"DeferredIBLShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}

void IBLShader::ShutdownForwardShader()
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

	if (m_deferredVertexShader)
	{
		m_deferredVertexShader->Release();
		m_deferredVertexShader = 0;
	}

	if (m_deferredPixelShader)
	{
		m_deferredPixelShader->Release();
		m_deferredPixelShader = 0;
	}

	if (m_deferredSampleState)
	{
		m_deferredSampleState->Release();
		m_deferredSampleState = 0;
	}


	if (m_AnisoSampleState)
	{
		m_AnisoSampleState->Release();
		m_AnisoSampleState = 0;
	}

	if (m_ClampSampleState)
	{
		m_ClampSampleState->Release();
		m_ClampSampleState = 0;
	}

	return;
}

void IBLShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownForwardShader();

	return;
}

bool IBLShader::ReloadShader(ID3D11Device* device)
{
	return true;
}

void IBLShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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
	for (i = 0; i<bufferSize; i++)
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

bool IBLShader::SetForwardPSLightningParameters(ID3D11DeviceContext* deviceContext ,ID3D11ShaderResourceView* diffuseProbe, ID3D11ShaderResourceView* specularProbe, ID3D11ShaderResourceView* specularIntegration)
{
	// Set shader irradiance map as resource in the pixel shader.
	deviceContext->PSSetShaderResources(4, 1, &diffuseProbe);

	// Set shader radiance map as resource in the pixel shader.
	deviceContext->PSSetShaderResources(5, 1, &specularProbe);

	// Set shader brdf lut map as resource in the pixel shader.
	deviceContext->PSSetShaderResources(6, 1, &specularIntegration);

	return true;
}

bool IBLShader::RenderForward(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart, ID3D11ShaderResourceView* diffuseProbe, ID3D11ShaderResourceView* specularProbe, ID3D11ShaderResourceView* specularIntegration)
{
	bool result;

	//Load the data into the shader buffers.
	result = SetForwardPSLightningParameters(deviceContext, diffuseProbe, specularProbe, specularIntegration);
	if (!result)
	{
		return false;
	}

	//Render the shader.
	RenderForwardShader(deviceContext, indexCount, indexStart);

	return true;
}

bool IBLShader::SetDeferredPSLightningParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* diffuseProbe, ID3D11ShaderResourceView* specularProbe, ID3D11ShaderResourceView* specularIntegration)
{
	// Set shader irradiance map as resource in the pixel shader.
	deviceContext->PSSetShaderResources(4, 1, &diffuseProbe);

	// Set shader radiance map as resource in the pixel shader.
	deviceContext->PSSetShaderResources(5, 1, &specularProbe);

	// Set shader brdf lut map as resource in the pixel shader.
	deviceContext->PSSetShaderResources(6, 1, &specularIntegration);

	return true;
}

bool IBLShader::RenderDeferred(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* diffuseProbe, ID3D11ShaderResourceView* specularProbe, ID3D11ShaderResourceView* specularIntegration)
{
	bool result;

	//Load the data into the shader buffers.
	result = SetDeferredPSLightningParameters(deviceContext, diffuseProbe, specularProbe, specularIntegration);
	if (!result)
	{
		return false;
	}

	//Render the shader.
	RenderDeferredShader(deviceContext);

	return true;
}

void IBLShader::RenderForwardShader(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart)
{

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_forwardLayout);

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(m_forwardVertexShader, NULL, 0);
	deviceContext->PSSetShader(m_forwardPixelShader, NULL, 0);

	// Set the sampler states in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_forwardSampleState);
	deviceContext->PSSetSamplers(1, 1, &m_AnisoSampleState);
	deviceContext->PSSetSamplers(2, 1, &m_ClampSampleState);
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, indexStart, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);



	return;
}

void IBLShader::RenderDeferredShader(ID3D11DeviceContext* deviceContext)
{

	// Primitive settings
	deviceContext->IASetInputLayout(NULL);
	deviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Set the shaders
	deviceContext->VSSetShader(m_deferredVertexShader, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(m_deferredPixelShader, NULL, 0);

	// Set the sampler states in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_deferredSampleState);
	deviceContext->PSSetSamplers(1, 1, &m_AnisoSampleState);
	deviceContext->PSSetSamplers(2, 1, &m_ClampSampleState);

	deviceContext->Draw(4, 0);

	ID3D11ShaderResourceView *arrRV[1] = { NULL };
	deviceContext->PSSetShaderResources(4, 1, arrRV);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	return;
}