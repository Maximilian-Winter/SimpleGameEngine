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
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	result = ShaderHelper::CreateVertexShader(device, hwnd, hlslFilename, "GBufferDebugVertexShader", &vertexShaderBuffer, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, hlslFilename, "GBufferDebugPixelShader", &pixelShaderBuffer, &m_pixelShader);
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


void GBufferDebuggingShader::ShutdownShader()
{
	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
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
