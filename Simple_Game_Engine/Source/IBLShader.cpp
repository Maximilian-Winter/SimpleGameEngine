#include "IBLShader.h"

IBLShader::IBLShader()
{
	m_forwardVertexShader = 0;
	m_forwardPixelShader = 0;
	m_forwardLayout = 0;

	m_deferredVertexShader = 0;
	m_deferredPixelShader = 0;
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
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	result = ShaderHelper::CreateVertexShader(device, hwnd, forwardShaderFilename, "ForwardLightningVertexShader", &vertexShaderBuffer, &m_forwardVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, forwardShaderFilename, "ForwardIBLPixelShader", &pixelShaderBuffer, &m_forwardPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreateInputLayoutDescFromVertexShaderSignature(vertexShaderBuffer, device, &m_forwardLayout);
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

bool IBLShader::InitializeDeferredShader(ID3D11Device* device, HWND hwnd, WCHAR* deferredShaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	result = ShaderHelper::CreateVertexShader(device, hwnd, deferredShaderFilename, "DeferredIBLVertexShader", &vertexShaderBuffer, &m_deferredVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, deferredShaderFilename, "DeferredIBLPixelShader", &pixelShaderBuffer, &m_deferredPixelShader);
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
}

void IBLShader::Shutdown()
{
	ShutdownForwardShader();
}

bool IBLShader::ReloadShader(ID3D11Device* device)
{
	return true;
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

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, indexStart, 0);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
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

	deviceContext->Draw(4, 0);

	ID3D11ShaderResourceView *arrRV[1] = { NULL };
	deviceContext->PSSetShaderResources(4, 1, arrRV);

	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);
	deviceContext->VSSetShader(NULL, NULL, 0);
	deviceContext->PSSetShader(NULL, NULL, 0);

	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}