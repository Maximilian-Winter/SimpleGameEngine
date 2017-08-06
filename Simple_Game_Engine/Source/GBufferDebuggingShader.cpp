#include "GBufferDebuggingShader.h"


GBufferDebuggingShader::GBufferDebuggingShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
}


GBufferDebuggingShader::GBufferDebuggingShader(const GBufferDebuggingShader& other)
{
}


GBufferDebuggingShader::~GBufferDebuggingShader()
{
}


bool GBufferDebuggingShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"GBufferDebugging.hlsl");
	if(!result)
	{
		return false;
	}

	return true;
}


void GBufferDebuggingShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	
}


bool GBufferDebuggingShader::Render(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* Depth, ID3D11ShaderResourceView* BaseColorSpecInt,ID3D11ShaderResourceView* Normal,
			      ID3D11ShaderResourceView* SpecPow)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext,Depth,BaseColorSpecInt, Normal, SpecPow);
	if(!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext);

	return true;
}


bool GBufferDebuggingShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* hlslFilename)
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
	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GBufferDebugVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
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
	result = D3DCompileFromFile(hlslFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "GBufferDebugPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
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

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	return true;
}


void GBufferDebuggingShader::ShutdownShader()
{
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

bool GBufferDebuggingShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* Depth, ID3D11ShaderResourceView* BaseColorSpecInt, ID3D11ShaderResourceView* Normal, 
					   ID3D11ShaderResourceView* SpecPow)
{
	// Set shader texture resources in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &Depth);
	deviceContext->PSSetShaderResources(1, 1, &BaseColorSpecInt);
	deviceContext->PSSetShaderResources(2, 1, &Normal);
	deviceContext->PSSetShaderResources(3, 1, &SpecPow);

	return true;
}

void GBufferDebuggingShader::RenderShader(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetInputLayout( NULL );
	deviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);
	deviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	
	// Set the shaders
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->GSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	deviceContext->Draw(16, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
	deviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}
