#include "DeferredBuffer.h"


DeferredBuffer::DeferredBuffer()
{
	for(int i=0; i<BUFFER_COUNT; i++)
	{
		m_renderTargetTextureArray[i] = 0;
		m_renderTargetViewArray[i] = 0;
		m_shaderResourceViewArray[i] = 0;
	}

	m_DepthStencilState = 0;
	m_depthStencilView = 0;
	m_depthStencilViewReadOnly = 0;
}


DeferredBuffer::DeferredBuffer(const DeferredBuffer& other)
{
}


DeferredBuffer::~DeferredBuffer()
{
}


bool DeferredBuffer::Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenDepth, float screenNear)
{

	HRESULT result;

	D3D11_TEXTURE2D_DESC textureDescDepth;
	D3D11_TEXTURE2D_DESC textureDescBaseColorSpecInt;
	D3D11_TEXTURE2D_DESC textureDescNormal;
	D3D11_TEXTURE2D_DESC textureDescSpecularPower;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescBaseColorSpecInt;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescNormal;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDescSpecularPower;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescDepth;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescBaseColorSpecInt;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescNormal;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescSpecularPower;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;


	// Store the width and height of the render texture.
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;

	// Initialize the render target textures descriptions.
	ZeroMemory(&textureDescDepth, sizeof(textureDescDepth));
	ZeroMemory(&textureDescBaseColorSpecInt, sizeof(textureDescBaseColorSpecInt));
	ZeroMemory(&textureDescNormal, sizeof(textureDescNormal));
	ZeroMemory(&textureDescSpecularPower, sizeof(textureDescSpecularPower));

	// Setup the render target texture description.

	textureDescDepth.Width = textureWidth;
	textureDescDepth.Height = textureHeight;
	textureDescDepth.MipLevels = 1;
	textureDescDepth.ArraySize = 1;
	textureDescDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDescDepth.SampleDesc.Count = 1;
	textureDescDepth.Usage = D3D11_USAGE_DEFAULT;
	textureDescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL|D3D11_BIND_SHADER_RESOURCE;
	textureDescDepth.CPUAccessFlags = 0;
	textureDescDepth.MiscFlags = 0;

	textureDescBaseColorSpecInt.Width = textureWidth;
	textureDescBaseColorSpecInt.Height = textureHeight;
	textureDescBaseColorSpecInt.MipLevels = 1;
	textureDescBaseColorSpecInt.ArraySize = 1;
	textureDescBaseColorSpecInt.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescBaseColorSpecInt.SampleDesc.Count = 1;
	textureDescBaseColorSpecInt.Usage = D3D11_USAGE_DEFAULT;
	textureDescBaseColorSpecInt.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDescBaseColorSpecInt.CPUAccessFlags = 0;
	textureDescBaseColorSpecInt.MiscFlags = 0;

	textureDescNormal.Width = textureWidth;
	textureDescNormal.Height = textureHeight;
	textureDescNormal.MipLevels = 1;
	textureDescNormal.ArraySize = 1;
	textureDescNormal.Format = DXGI_FORMAT_R11G11B10_FLOAT;
	textureDescNormal.SampleDesc.Count = 1;
	textureDescNormal.Usage = D3D11_USAGE_DEFAULT;
	textureDescNormal.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDescNormal.CPUAccessFlags = 0;
	textureDescNormal.MiscFlags = 0;

	textureDescSpecularPower.Width = textureWidth;
	textureDescSpecularPower.Height = textureHeight;
	textureDescSpecularPower.MipLevels = 1;
	textureDescSpecularPower.ArraySize = 1;
	textureDescSpecularPower.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDescSpecularPower.SampleDesc.Count = 1;
	textureDescSpecularPower.Usage = D3D11_USAGE_DEFAULT;
	textureDescSpecularPower.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDescSpecularPower.CPUAccessFlags = 0;
	textureDescSpecularPower.MiscFlags = 0;

	result = device->CreateTexture2D(&textureDescDepth, NULL, &m_renderTargetTextureArray[0]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateTexture2D(&textureDescBaseColorSpecInt, NULL, &m_renderTargetTextureArray[1]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateTexture2D(&textureDescNormal, NULL, &m_renderTargetTextureArray[2]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateTexture2D(&textureDescSpecularPower, NULL, &m_renderTargetTextureArray[3]);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the render target view.

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Set up the render target view description for the base color, roughness render target.
	renderTargetViewDescBaseColorSpecInt.Format = textureDescBaseColorSpecInt.Format;
	renderTargetViewDescBaseColorSpecInt.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDescBaseColorSpecInt.Texture2D.MipSlice = 0;

	// Set up the render target view description for the normal render target.
	renderTargetViewDescNormal.Format = textureDescNormal.Format;
	renderTargetViewDescNormal.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDescNormal.Texture2D.MipSlice = 0;

	// Set up the render target view description for the metalness rendertarget.
	renderTargetViewDescSpecularPower.Format = textureDescSpecularPower.Format;
	renderTargetViewDescSpecularPower.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDescSpecularPower.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = device->CreateDepthStencilView(m_renderTargetTextureArray[0], &depthStencilViewDesc, &m_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Create read only depth stencil view.
	depthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH|D3D11_DSV_READ_ONLY_STENCIL;

	result = device->CreateDepthStencilView(m_renderTargetTextureArray[0], &depthStencilViewDesc, &m_depthStencilViewReadOnly);
	if(FAILED(result))
	{
		return false;
	}

	// Create the Render Target views.
	result = device->CreateRenderTargetView(m_renderTargetTextureArray[1], &renderTargetViewDescBaseColorSpecInt, &m_renderTargetViewArray[1]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateRenderTargetView(m_renderTargetTextureArray[2], &renderTargetViewDescNormal, &m_renderTargetViewArray[2]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateRenderTargetView(m_renderTargetTextureArray[3], &renderTargetViewDescSpecularPower, &m_renderTargetViewArray[3]);
	if(FAILED(result))
	{
		return false;
	}


	// Setup the description of the shader resource views.

	shaderResourceViewDescDepth.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDescDepth.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDescDepth.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDescDepth.Texture2D.MipLevels = 1;

	shaderResourceViewDescBaseColorSpecInt.Format = textureDescBaseColorSpecInt.Format;
	shaderResourceViewDescBaseColorSpecInt.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDescBaseColorSpecInt.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDescBaseColorSpecInt.Texture2D.MipLevels = 1;

	shaderResourceViewDescNormal.Format = textureDescNormal.Format;
	shaderResourceViewDescNormal.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDescNormal.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDescNormal.Texture2D.MipLevels = 1;

	shaderResourceViewDescSpecularPower.Format = textureDescSpecularPower.Format;
	shaderResourceViewDescSpecularPower.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDescSpecularPower.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDescSpecularPower.Texture2D.MipLevels = 1;

	// Create the shader resource views.

	result = device->CreateShaderResourceView(m_renderTargetTextureArray[0], &shaderResourceViewDescDepth, &m_shaderResourceViewArray[0]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateShaderResourceView(m_renderTargetTextureArray[1], &shaderResourceViewDescBaseColorSpecInt, &m_shaderResourceViewArray[1]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateShaderResourceView(m_renderTargetTextureArray[2], &shaderResourceViewDescNormal, &m_shaderResourceViewArray[2]);
	if(FAILED(result))
	{
		return false;
	}

	result = device->CreateShaderResourceView(m_renderTargetTextureArray[3], &shaderResourceViewDescSpecularPower, &m_shaderResourceViewArray[3]);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC descDepth;
	descDepth.DepthEnable = TRUE;
	descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	descDepth.DepthFunc = D3D11_COMPARISON_LESS;
	descDepth.StencilEnable = TRUE;
	descDepth.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	descDepth.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC stencilMarkOp = { D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_STENCIL_OP_REPLACE, D3D11_COMPARISON_ALWAYS };
	descDepth.FrontFace = stencilMarkOp;
	descDepth.BackFace = stencilMarkOp;

	result = device->CreateDepthStencilState(&descDepth, &m_DepthStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the viewport for rendering.
	m_viewport.Width = (float)textureWidth;
	m_viewport.Height = (float)textureHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	return true;
}


void DeferredBuffer::Shutdown()
{

	if(m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if(m_depthStencilViewReadOnly)
	{
		m_depthStencilViewReadOnly->Release();
		m_depthStencilViewReadOnly = 0;
	}

	if(m_DepthStencilState)
	{
		m_DepthStencilState->Release();
		m_DepthStencilState = 0;
	}

	for(int i=0; i<BUFFER_COUNT; i++)
	{
		if(m_shaderResourceViewArray[i])
		{
			m_shaderResourceViewArray[i]->Release();
			m_shaderResourceViewArray[i] = 0;
		}

		if(m_renderTargetViewArray[i])
		{
			m_renderTargetViewArray[i]->Release();
			m_renderTargetViewArray[i] = 0;
		}

		if(m_renderTargetTextureArray[i])
		{
			m_renderTargetTextureArray[i]->Release();
			m_renderTargetTextureArray[i] = 0;
		}
	}

	return;
}


void DeferredBuffer::BindDeferredBufferToPipeline(ID3D11DeviceContext* deviceContext)
{

	//Create Render Target view array because of m_renderTargetViewArray[0] is depth stencil

	ID3D11RenderTargetView* rt[3] = { m_renderTargetViewArray[1], m_renderTargetViewArray[2], m_renderTargetViewArray[3] };
	
	// Bind the render target view array and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(3, rt, m_depthStencilView);

	deviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);
	
	// Set the viewport.
	deviceContext->RSSetViewports(1, &m_viewport);

	ClearDeferredRenderTargets(deviceContext);
	
	return;
}


void DeferredBuffer::ClearDeferredRenderTargets(ID3D11DeviceContext* deviceContext)
{
	float color[4];

	// Setup the color to clear the buffer to.

		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		color[3] = 0.0f;

		deviceContext->ClearRenderTargetView(m_renderTargetViewArray[1], color);
		deviceContext->ClearRenderTargetView(m_renderTargetViewArray[2], color);
		deviceContext->ClearRenderTargetView(m_renderTargetViewArray[3], color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


ID3D11ShaderResourceView* DeferredBuffer::GetShaderResourceView(int view)
{
	return m_shaderResourceViewArray[view];
}


DeferredRenderData DeferredBuffer::GetDeferredRenderData()
{
	DeferredRenderData RenderData;
	RenderData.DepthData = m_shaderResourceViewArray[0];
	RenderData.AlbedoRoughnesData = m_shaderResourceViewArray[1];
	RenderData.NormalData = m_shaderResourceViewArray[2];
	RenderData.F0Metalness = m_shaderResourceViewArray[3];

	return RenderData;
}


ID3D11DepthStencilView* DeferredBuffer::GetDeferredDSV()
{
	return m_depthStencilView;
}


ID3D11DepthStencilView* DeferredBuffer::GetDeferredReadOnlyDSV()
{
	return m_depthStencilViewReadOnly;
}


void DeferredBuffer::SaveDeferredRenderTargetsToFile(ID3D11DeviceContext* deviceContext)
{
	ID3D11Texture2D* DepthTexture = nullptr;
	
	DirectX::SaveWICTextureToFile(deviceContext, m_renderTargetTextureArray[0], GUID_ContainerFormatBmp, L"data/Render Target Output/Depth.bmp");
	DirectX::SaveWICTextureToFile(deviceContext, m_renderTargetTextureArray[1], GUID_ContainerFormatBmp, L"data/Render Target Output/Color.bmp");
	DirectX::SaveWICTextureToFile(deviceContext, m_renderTargetTextureArray[2], GUID_ContainerFormatBmp, L"data/Render Target Output/Normal.bmp");
	DirectX::SaveWICTextureToFile(deviceContext, m_renderTargetTextureArray[3], GUID_ContainerFormatBmp, L"data/Render Target Output/Specular.bmp");
}