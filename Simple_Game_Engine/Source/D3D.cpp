#include "D3D.h"


D3D::D3D()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;
	m_depthStencilBuffer = 0;
	m_depthStencilStateLess = 0;
	m_depthStencilView = 0;
	m_rasterStateCullBackCW = 0;
	m_rasterStateNoCulling = 0;
	m_depthDisabledStencilState = 0;
	m_depthStencilStateLessEqual = 0;
	m_alphaEnableBlendingState = 0;
	m_alphaDisableBlendingState = 0;
	
	m_rasterStateCullFrontCW = 0;
	m_additiveAlphaBlendingState = 0;
	m_depthStencilStateGreaterEqual = 0;

	m_oldBlendState = 0;
	m_oldDepthStencilState = 0;
	m_oldRasterState = 0;

	m_d2dRenderTarget = 0;
}

D3D::D3D(const D3D& other)
{
}

D3D::~D3D()
{
}

bool D3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDescLightVolume;
	D3D11_DEPTH_STENCIL_DESC depthStencilDescForwardLightning;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;
	D3D11_BLEND_DESC AdditiveblendStateDescription;

	// Store the vsync setting.
	m_vsync_enabled = vsync;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if(FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if(FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM_SRGB display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(i=0; i<numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if(displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get the adapter (video card) description.
	result = adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return false;
	}

	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// Set the refresh rate of the back buffer.
	if(m_vsync_enabled)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
    swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, &featureLevel, 1,
										   D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if(FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStateLess);
	if(FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilStateLess, 1);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}


	// Initialize the description of the third stencil state.
	ZeroMemory(&depthStencilDescForwardLightning, sizeof(depthStencilDescForwardLightning));

	// Set up the description of the stencil state for forward lightning.
	depthStencilDescForwardLightning.DepthEnable = TRUE;
	depthStencilDescForwardLightning.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescForwardLightning.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDescForwardLightning.StencilEnable = FALSE;
	depthStencilDescForwardLightning.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDescForwardLightning.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	const D3D11_DEPTH_STENCILOP_DESC noSkyStencilOp = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_EQUAL };
	depthStencilDescForwardLightning.FrontFace = noSkyStencilOp;
	depthStencilDescForwardLightning.BackFace = noSkyStencilOp;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDescForwardLightning, &m_depthStencilStateLessEqual);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the fourth stencil state.
	ZeroMemory(&depthStencilDescLightVolume, sizeof(depthStencilDescLightVolume));

	// Set up the description of the stencil state for Light Volume Rendering.
	depthStencilDescLightVolume.DepthEnable = TRUE;
	depthStencilDescLightVolume.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDescLightVolume.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	depthStencilDescLightVolume.StencilEnable = TRUE;
	depthStencilDescLightVolume.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDescLightVolume.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDescLightVolume.FrontFace = noSkyStencilOp;
	depthStencilDescLightVolume.BackFace = noSkyStencilOp;


	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDescLightVolume, &m_depthStencilStateGreaterEqual);
	if (FAILED(result))
	{
		return false;
	}


	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Create the back face and clockwise culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateCullBackCW);
	if(FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateCullBackCW);
	
	// Setup the rasterizer state for fron face clockwise culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state for light volume.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateCullFrontCW);
	if (FAILED(result))
	{
		return false;
	}

	// Create the back face and counter clockwise culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateCullBackCCW);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the rasterizer state for front ccw culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_FRONT;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state for light volume.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateCullFrontCCW);
	if (FAILED(result))
	{
		return false;
	}

	// Setup a raster description which turns off back face culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the no culling rasterizer state.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateNoCulling);
	if (FAILED(result))
	{
		return false;
	}

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the second blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// Clear the blend state description.
	ZeroMemory(&AdditiveblendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Setup an Additive Alpha Blendingblend state description.
	AdditiveblendStateDescription.AlphaToCoverageEnable = FALSE;
	AdditiveblendStateDescription.IndependentBlendEnable = FALSE;

	const D3D11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		TRUE,
		D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_ALL,
	};

	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		AdditiveblendStateDescription.RenderTarget[i] = defaultRenderTargetBlendDesc;
	}

	// Create the second blend state using the description.
	result = m_device->CreateBlendState(&AdditiveblendStateDescription, &m_additiveAlphaBlendingState);
	if (FAILED(result))
	{
		return false;
	}
	// Setup the viewport for rendering.
    m_viewport.Width = (float)screenWidth;
    m_viewport.Height = (float)screenHeight;
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
    m_viewport.TopLeftX = 0.0f;
    m_viewport.TopLeftY = 0.0f;

	// Create the viewport.
    m_deviceContext->RSSetViewports(1, &m_viewport);

	// Setup the projection matrix.
	fieldOfView = DirectX::XM_PIDIV4;
	screenAspect = screenWidth / screenHeight;

	// Create the projection matrix for 3D rendering.
	DirectX::XMStoreFloat4x4(&m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth));

    // Initialize the world matrix to the identity matrix.
	DirectX::XMStoreFloat4x4(&m_worldMatrix, DirectX::XMMatrixIdentity());

	// Create an orthographic projection matrix for 2D rendering.
	DirectX::XMStoreFloat4x4(&m_orthoMatrix, DirectX::XMMatrixOrthographicLH(screenWidth, screenHeight, screenNear, screenDepth));


	// create the D2D factory
	ID2D1Factory* d2dfactory;
	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &d2dfactory);

	// set up the D2D render target using the back buffer
	IDXGISurface *dxgiBackbuffer;
	result = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackbuffer));
	if (FAILED(result))
	{
		return false;
	}

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	result = d2dfactory->CreateDxgiSurfaceRenderTarget(dxgiBackbuffer, props, &m_d2dRenderTarget);

	// Release the dxgi back buffer.
	dxgiBackbuffer->Release();
	dxgiBackbuffer = 0;
	// Release the factory.
	d2dfactory->Release();
	d2dfactory = 0;
	if (FAILED(result))
	{
		return false;
	}
	

    return true;
}

void D3D::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if(m_oldBlendState)
	{
		m_oldBlendState->Release();
		m_oldBlendState = 0;
	}

	if(m_oldDepthStencilState)
	{
		m_oldDepthStencilState->Release();
		m_oldDepthStencilState = 0;
	}

	if(m_oldRasterState)
	{
		m_oldRasterState->Release();
		m_oldRasterState = 0;
	}

	if(m_rasterStateCullFrontCW)
	{
		m_rasterStateCullFrontCW->Release();
		m_rasterStateCullFrontCW = 0;
	}

	if (m_rasterStateCullBackCW)
	{
		m_rasterStateCullBackCW->Release();
		m_rasterStateCullBackCW = 0;
	}

	if (m_rasterStateCullFrontCCW)
	{
		m_rasterStateCullFrontCCW->Release();
		m_rasterStateCullFrontCCW = 0;
	}

	if (m_rasterStateCullBackCCW)
	{
		m_rasterStateCullBackCCW->Release();
		m_rasterStateCullBackCCW = 0;
	}

	if (m_rasterStateNoCulling)
	{
		m_rasterStateNoCulling->Release();
		m_rasterStateNoCulling = 0;
	}

	if(m_additiveAlphaBlendingState)
	{
		m_additiveAlphaBlendingState->Release();
		m_additiveAlphaBlendingState = 0;
	}
	if (m_depthStencilStateLessEqual)
	{
		m_depthStencilStateLessEqual->Release();
		m_depthStencilStateLessEqual = 0;
	}
	if(m_depthStencilStateGreaterEqual)
	{
		m_depthStencilStateGreaterEqual->Release();
		m_depthStencilStateGreaterEqual = 0;
	}

	if(m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if(m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}

	if(m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}



	if(m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if(m_depthStencilStateLess)
	{
		m_depthStencilStateLess->Release();
		m_depthStencilStateLess = 0;
	}

	if(m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if(m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_d2dRenderTarget)
	{
		m_d2dRenderTarget->Release();
		m_d2dRenderTarget = 0;
	}

	if(m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if(m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if(m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
	
	return;
}

void D3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
    
	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void D3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	return;
}

ID3D11Device* D3D::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* D3D::GetDeviceContext()
{
	return m_deviceContext;
}

DirectX::XMMATRIX D3D::GetProjectionMatrix()
{
	return DirectX::XMLoadFloat4x4(&m_projectionMatrix);
}

DirectX::XMMATRIX D3D::GetWorldMatrix()
{
	return DirectX::XMLoadFloat4x4(&m_worldMatrix);
}

DirectX::XMMATRIX D3D::GetOrthoMatrix()
{
	return DirectX::XMLoadFloat4x4(&m_orthoMatrix);
}

void D3D::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3D::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilStateLess, 1);
	return;
}

void D3D::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void D3D::TurnOnAlphaBlending()
{
	float blendFactor[4];
	

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3D::TurnOffAlphaBlending()
{
	float blendFactor[4];
	
	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	
	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3D::TurnOnBackFaceCWCulling()
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateCullBackCW);

	return;
}

void D3D::TurnOnFrontFaceCWCulling()
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateCullFrontCW);

	return;
}

void D3D::TurnOnBackFaceCCWCulling()
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateCullBackCCW);

	return;
}

void D3D::TurnOnFrontFaceCCWCulling()
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateCullFrontCCW);

	return;
}

void D3D::TurnOffCulling()
{
	// Set the no back face culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateNoCulling);

	return;
}
 
void D3D::SetBackBufferToRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return;
}

void D3D::BindBackBufferToPipeline()
{
	SetBackBufferToRenderTarget();
	ResetViewport();
}

void D3D::SetDepthStencilView(ID3D11DepthStencilView* DepthStencilView)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, DepthStencilView);

	return;
}

void D3D::ResetViewport()
{
	// Set the viewport.
    m_deviceContext->RSSetViewports(1, &m_viewport);

	return;
}

ID2D1RenderTarget* D3D::GetD2DRenderTarget()
{
	return m_d2dRenderTarget;
}

void D3D::TurnStencilStateGreaterEqualOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilStateGreaterEqual, 1);
}

void D3D::TurnStencilStateLessEqualOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilStateLessEqual, 1);
}

void D3D::TurnStencilStateLessOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilStateLess, 1);
}

void D3D::TurnAdditiveAlphaBlendingOn()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_additiveAlphaBlendingState, blendFactor, 0xffffffff);
}

void D3D::TurnAdditiveAlphaBlendingOff()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

//Helper function to convert string to wstring
std::wstring D3D::StringToWString(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void D3D::SaveRenderTargetToFile()
{
	//Get Date and Time for a Timestamp
	time_t currentTime;
	struct tm localTime;

	time(&currentTime);                   // Get the current time
	localtime_s(&localTime, &currentTime);  // Convert the current time to the local time

	int Day = localTime.tm_mday;
	int Month = localTime.tm_mon + 1;
	int Year = localTime.tm_year + 1900;
	int Hour = localTime.tm_hour;
	int Min = localTime.tm_min;
	int Sec = localTime.tm_sec;

	std::string FileName = "data/Render Target Output/" + std::to_string(Hour) + "_" + std::to_string(Min) + "_" + std::to_string(Sec) + "-" + std::to_string(Day) + "_" + std::to_string(Month) + "_" + std::to_string(Year) + "-Screenshot.bmp";

	std::wstring wFilename = StringToWString(FileName);
	LPCWSTR wpFilename = wFilename.c_str();

	ID3D11Resource* RenderTargetResource;
	m_renderTargetView->GetResource(&RenderTargetResource);
	 
	DirectX::SaveWICTextureToFile(m_deviceContext, RenderTargetResource, GUID_ContainerFormatBmp, wpFilename);

	return;
}