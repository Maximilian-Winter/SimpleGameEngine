#include "LineListShader.h"


LineListShader::LineListShader()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_vertexBuffer = 0;

}


LineListShader::LineListShader(const LineListShader& other)
{
}


LineListShader::~LineListShader()
{
}


bool LineListShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Initialize the Forward vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"LineShader.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}


void LineListShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	
}


bool LineListShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* forwardShaderFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC vertexBufferDesc;

	result = ShaderHelper::CreateVertexShader(device, hwnd, forwardShaderFilename, "LineVertexShader", &vertexShaderBuffer, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = ShaderHelper::CreatePixelShader(device, hwnd, forwardShaderFilename, "LinePixelShader", &pixelShaderBuffer, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the Vertex stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = 2 * sizeof(ColoredPointBufferType);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&vertexBufferDesc, NULL, &m_vertexBuffer);
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

bool LineListShader::DrawLine(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR from, DirectX::FXMVECTOR to, int r, int g, int b, int a)
{
	ColoredPointBufferType* v = NULL;

	D3D11_MAPPED_SUBRESOURCE mapData;

	if (FAILED(deviceContext->Map(m_vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapData)))
	{
		return false;
	}

	v = (ColoredPointBufferType*)mapData.pData;
	
	v[0].point.x = DirectX::XMVectorGetX(from);
	v[0].point.y = DirectX::XMVectorGetY(from);
	v[0].point.z = DirectX::XMVectorGetZ(from);
	v[0].color.x = r;
	v[0].color.y = g;
	v[0].color.z = b;
	v[0].color.w = a;

	v[1].point.x = DirectX::XMVectorGetX(to);
	v[1].point.y = DirectX::XMVectorGetY(to);
	v[1].point.z = DirectX::XMVectorGetZ(to);
	v[1].color.x = r;
	v[1].color.y = g;
	v[1].color.z = b;
	v[1].color.w = a;

	deviceContext->Unmap(m_vertexBuffer, NULL);

	deviceContext->IASetInputLayout(m_layout);

	UINT Stride = sizeof(ColoredPointBufferType);
	UINT Offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &Stride, &Offset);

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	deviceContext->Draw(2, 0);
}


void LineListShader::ShutdownShader()
{

	// Release all directx elemnts

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}


	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	
}
