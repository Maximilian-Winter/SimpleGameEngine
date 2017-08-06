#include "DeferredShader.h"


DeferredShader::DeferredShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
}


DeferredShader::DeferredShader(const DeferredShader& other)
{
}


DeferredShader::~DeferredShader()
{
}


bool DeferredShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"FillGbuffer.hlsl");
	if(!result)
	{
		return false;
	}

	return true;
}


void DeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	
}

bool DeferredShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart)
{
	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount, indexStart);

	return true;
}



bool DeferredShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* hlslFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "ForwardLightningVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			ShaderHelper::OutputShaderErrorMessage(errorMessage, hwnd, hlslFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, hlslFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "FillGbufferPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			ShaderHelper::OutputShaderErrorMessage(errorMessage, hwnd, hlslFilename);
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, hlslFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateInputLayoutDescFromVertexShaderSignature(vertexShaderBuffer, device, &m_layout);
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


void DeferredShader::ShutdownShader()
{
	// Release the layout.
	if(m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	// Release the pixel shader.
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
	
	
}

void DeferredShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, int indexStart)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, indexStart, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);


	
}
