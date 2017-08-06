#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
	m_forwardObjectVSBuffer = 0;
	m_forwardMaterialPSBuffer = 0;
	m_forwardEyePositionBuffer = 0;
	m_DeferredDataForUnpack = 0;

	m_LinearWrapSampleState = 0;
	m_LinearClampSampleState = 0;
	m_AnsiotropicWrapSampleState = 0;
	m_PointClampSampleState = 0;
	m_TestSampleState = 0;
}

ShaderManager::ShaderManager(const ShaderManager& other)
{
}

ShaderManager::~ShaderManager()
{
}

bool ShaderManager::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	D3D11_BUFFER_DESC ObjectBufferVSDesc;
	D3D11_BUFFER_DESC MaterialBufferPSDesc;
	D3D11_BUFFER_DESC EyePositionBufferPSDesc;
	D3D11_BUFFER_DESC GBufferUnpackDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	// Initialize the deferred shader object.
	result = m_GBufferDebugShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the GBuffer Debug Shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the deferred shader object.
	result = m_DeferredFillGBufferShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the deferred shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light shader object.
	result = m_DirectionalLightShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light shader object.
	result = m_PointLightShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Pointlight shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light shader object.
	result = m_CapsuleLightShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Capsulelight shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the light shader object.
	result = m_SpotLightShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Spotlight shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the ibl shader object.
	result = m_IBLShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the IBL shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the skybox shader object.
	result = m_SkyboxShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the skybox shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the line shader object.
	result = m_LineShader.Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the line shader object.", L"Error", MB_OK);
		return false;
	}

	// Setup the description of the object buffer that is in the vertex shader.
	ObjectBufferVSDesc.Usage = D3D11_USAGE_DYNAMIC;
	ObjectBufferVSDesc.ByteWidth = sizeof(ObjectBufferVSType);
	ObjectBufferVSDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ObjectBufferVSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ObjectBufferVSDesc.MiscFlags = 0;
	ObjectBufferVSDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&ObjectBufferVSDesc, NULL, &m_forwardObjectVSBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the material buffer that is in the pixel shader.
	MaterialBufferPSDesc.Usage = D3D11_USAGE_DYNAMIC;
	MaterialBufferPSDesc.ByteWidth = sizeof(MaterialBufferPSType);
	MaterialBufferPSDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MaterialBufferPSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MaterialBufferPSDesc.MiscFlags = 0;
	MaterialBufferPSDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&MaterialBufferPSDesc, NULL, &m_forwardMaterialPSBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the eye position buffer that is in the pixel shader.
	EyePositionBufferPSDesc.Usage = D3D11_USAGE_DYNAMIC;
	EyePositionBufferPSDesc.ByteWidth = sizeof(EyePositionBufferPSType);
	EyePositionBufferPSDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	EyePositionBufferPSDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	EyePositionBufferPSDesc.MiscFlags = 0;
	EyePositionBufferPSDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&EyePositionBufferPSDesc, NULL, &m_forwardEyePositionBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the Gbuffer Unpack dynamic constant buffer that is in the pixel shader.
	GBufferUnpackDesc.Usage = D3D11_USAGE_DYNAMIC;
	GBufferUnpackDesc.ByteWidth = sizeof(GBUFFER_UNPACK_DATA);
	GBufferUnpackDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GBufferUnpackDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GBufferUnpackDesc.MiscFlags = 0;
	GBufferUnpackDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&GBufferUnpackDesc, NULL, &m_DeferredDataForUnpack);
	if (FAILED(result))
	{
		return false;
	}

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
	result = device->CreateSamplerState(&samplerDesc, &m_LinearWrapSampleState);
	if (FAILED(result))
	{
		return false;
	}

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
	result = device->CreateSamplerState(&samplerDesc, &m_PointClampSampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0.0;
	samplerDesc.BorderColor[1] = 0.0;
	samplerDesc.BorderColor[2] = 0.0;
	samplerDesc.BorderColor[3] = 0.0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_AnsiotropicWrapSampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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
	result = device->CreateSamplerState(&samplerDesc, &m_LinearClampSampleState);
	if (FAILED(result))
	{
		return false;
	}

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
	result = device->CreateSamplerState(&samplerDesc, &m_TestSampleState);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void ShaderManager::Shutdown()
{

	m_PointLightShader.Shutdown();
	m_CapsuleLightShader.Shutdown();
	m_SpotLightShader.Shutdown();
	m_DirectionalLightShader.Shutdown();
	m_DeferredFillGBufferShader.Shutdown();
	m_GBufferDebugShader.Shutdown();
	m_TextureShader.Shutdown();
	m_IBLShader.Shutdown();
	m_LineShader.Shutdown();

	if (m_forwardObjectVSBuffer)
	{
		m_forwardObjectVSBuffer->Release();
		m_forwardObjectVSBuffer = 0;
	}

	if (m_forwardMaterialPSBuffer)
	{
		m_forwardMaterialPSBuffer->Release();
		m_forwardMaterialPSBuffer = 0;
	}

	if (m_forwardEyePositionBuffer)
	{
		m_forwardEyePositionBuffer->Release();
		m_forwardEyePositionBuffer = 0;
	}

	if (m_DeferredDataForUnpack)
	{
		m_DeferredDataForUnpack->Release();
		m_DeferredDataForUnpack = 0;
	}

	if (m_LinearWrapSampleState)
	{
		m_LinearWrapSampleState->Release();
		m_LinearWrapSampleState = 0;
	}

	if (m_LinearClampSampleState)
	{
		m_LinearClampSampleState->Release();
		m_LinearClampSampleState = 0;
	}

	if (m_AnsiotropicWrapSampleState)
	{
		m_AnsiotropicWrapSampleState->Release();
		m_AnsiotropicWrapSampleState = 0;
	}

	if (m_PointClampSampleState)
	{
		m_PointClampSampleState->Release();
		m_PointClampSampleState = 0;
	}
	if (m_TestSampleState)
	{
		m_TestSampleState->Release();
		m_TestSampleState = 0;
	}
}

bool ShaderManager::SetVertexAndIndexBuffer(ID3D11DeviceContext* deviceContext, VerticesRenderData Vertices)
{
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &Vertices.VertexBuffer, &Vertices.VertexBufferStride, &Vertices.VertexBufferOffset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(Vertices.IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool ShaderManager::SetForwardShadingVSMatrixBuffer(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ObjectBufferVSType* MatrixBufferVS;

	DirectX::XMMATRIX WorldViewProjection = worldMatrix * viewMatrix * projectionMatrix;

	// Transpose the matrices to prepare them for the shader and store them in DirectX::XMFLOAT4X4.
	DirectX::XMFLOAT4X4 WorldMatrix;
	DirectX::XMStoreFloat4x4(&WorldMatrix, DirectX::XMMatrixTranspose(worldMatrix));

	DirectX::XMFLOAT4X4 WVPMatrix;
	DirectX::XMStoreFloat4x4(&WVPMatrix, DirectX::XMMatrixTranspose(WorldViewProjection));

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_forwardObjectVSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	MatrixBufferVS = (ObjectBufferVSType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	MatrixBufferVS->WorldViewProjection = WVPMatrix;
	MatrixBufferVS->World = WorldMatrix;
	// Unlock the constant buffer.
	deviceContext->Unmap(m_forwardObjectVSBuffer, 0);

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(0, 1, &m_forwardObjectVSBuffer);

	return true;

}

bool ShaderManager::SetForwardShadingPSEyePositionBuffer(ID3D11DeviceContext* deviceContext, DirectX::FXMVECTOR eyePosition)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	EyePositionBufferPSType* EyePositionBufferPS;

	// Store the eyePositon in DirectX::XMFLOAT3
	DirectX::XMFLOAT3 eyePos;
	DirectX::XMStoreFloat3(&eyePos, eyePosition);

	// Lock the Pixelshader Object constant buffer so it can be written to.
	result = deviceContext->Map(m_forwardEyePositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	EyePositionBufferPS = (EyePositionBufferPSType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	EyePositionBufferPS->EyePosition = eyePos;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_forwardEyePositionBuffer, 0);

	// Set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(1, 1, &m_forwardEyePositionBuffer);


	return true;
}

bool ShaderManager::SetForwardShadingPSMaterialProperties(ID3D11DeviceContext* deviceContext, SurfaceMaterial& Material)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MaterialBufferPSType* MaterialBufferPS;


	// Lock the Pixelshader Object constant buffer so it can be written to.
	result = deviceContext->Map(m_forwardMaterialPSBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	MaterialBufferPS = (MaterialBufferPSType*)mappedResource.pData;

	if (Material.HasAlbedoMap && Material.HasMetalnessMap && Material.HasRoughnessMap && Material.HasNormalMap)
	{

		// Set the material textures as resource in the pixel shader.
		ID3D11ShaderResourceView* textures[4] = { Material.AlbedoMap, Material.RoughnessMap, Material.MetalnessMap, Material.NormalMap };

		deviceContext->PSSetShaderResources(0, 4, textures);

		// Copy the material variables into the constant buffer.
		MaterialBufferPS->Albedo = Material.Albedo;
		MaterialBufferPS->Roughness = Material.Roughness;
		MaterialBufferPS->F0 = Material.F0;
		MaterialBufferPS->Metalness = Material.Metalness;
		MaterialBufferPS->Transparency = Material.Transparency;
		if (Material.UseAlphaChannelTransparency)
		{
			MaterialBufferPS->TextureInput = 4;
		}
		else
		{
			MaterialBufferPS->TextureInput = 2;
		}
	}
	else if (Material.HasAlbedoMap && Material.HasMetalnessMap && Material.HasRoughnessMap)
	{
		// Set the material textures as resource in the pixel shader.
		ID3D11ShaderResourceView* textures[3] = { Material.AlbedoMap, Material.RoughnessMap, Material.MetalnessMap };

		deviceContext->PSSetShaderResources(0, 3, textures);

		// Copy the material variables into the constant buffer.
		MaterialBufferPS->Albedo = Material.Albedo;
		MaterialBufferPS->Roughness = Material.Roughness;
		MaterialBufferPS->F0 = Material.F0;
		MaterialBufferPS->Metalness = Material.Metalness;
		MaterialBufferPS->Transparency = Material.Transparency;
		if (Material.UseAlphaChannelTransparency)
		{
			MaterialBufferPS->TextureInput = 3;
		}
		else
		{
			MaterialBufferPS->TextureInput = 1;
		}
	}
	else
	{

		// Copy the material variables into the constant buffer.
		MaterialBufferPS->Albedo = Material.Albedo;
		MaterialBufferPS->Roughness = Material.Roughness;
		MaterialBufferPS->F0 = Material.F0;
		MaterialBufferPS->Metalness = Material.Metalness;
		MaterialBufferPS->Transparency = Material.Transparency;
		MaterialBufferPS->TextureInput = 0;
	}

	// Unlock the constant buffer.
	deviceContext->Unmap(m_forwardMaterialPSBuffer, 0);

	// Set the material constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(0, 1, &m_forwardMaterialPSBuffer);

	return true;
}

bool ShaderManager::SetDeferredShadingPSGBufferParameters(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	GBUFFER_UNPACK_DATA* GBufferData;

	// Set shader texture resources in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &RenderData.DepthData);
	deviceContext->PSSetShaderResources(1, 1, &RenderData.AlbedoRoughnesData);
	deviceContext->PSSetShaderResources(2, 1, &RenderData.NormalData);
	deviceContext->PSSetShaderResources(3, 1, &RenderData.F0Metalness);

	//Calculate matrix inverse
	DirectX::XMMATRIX matViewInv;
	DirectX::XMMATRIX view = viewMatrix;
	DirectX::XMVECTOR vDet;
	matViewInv = DirectX::XMMatrixInverse(&vDet, view);

	// Transpose the matrices to prepare them for the shader and store them in DirectX::XMFLOAT4X4.
	DirectX::XMFLOAT4X4 InvView;
	DirectX::XMStoreFloat4x4(&InvView, DirectX::XMMatrixTranspose(matViewInv));
	
	// Store Projection matrix in XMFLOAT4X4 to access the data to set the perspective values in the constant buffer
	DirectX::XMFLOAT4X4 ProjMat;
	DirectX::XMStoreFloat4x4(&ProjMat, projectionMatrix);

	result = deviceContext->Map(m_DeferredDataForUnpack, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	GBufferData = (GBUFFER_UNPACK_DATA*)mappedResource.pData;
	
	GBufferData->PerspectiveValues.x = 1.0f / ProjMat.m[0][0];
	GBufferData->PerspectiveValues.y = 1.0f / ProjMat.m[1][1];
	GBufferData->PerspectiveValues.z = ProjMat.m[3][2];
	GBufferData->PerspectiveValues.w = -ProjMat.m[2][2];
	GBufferData->ViewInv = InvView;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_DeferredDataForUnpack, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_DeferredDataForUnpack);

	return true;
}

bool ShaderManager::RenderGBufferDebuggingShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData)
{
	bool result;

	deviceContext->PSSetSamplers(0, 1, &m_PointClampSampleState);

	result = m_GBufferDebugShader.Render(deviceContext, RenderData.DepthData, RenderData.AlbedoRoughnesData, RenderData.NormalData, RenderData.F0Metalness);

	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderLine(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR from, DirectX::FXMVECTOR to, int r, int g, int b, int a)
{
	bool result;

	result = SetForwardShadingVSMatrixBuffer(deviceContext, DirectX::XMMatrixIdentity(), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_LinearWrapSampleState);

	m_LineShader.DrawLine(deviceContext, from, to, r, g, b, a);

	return true;

}

bool ShaderManager::RenderFillGBufferShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	bool result;

	result = SetVertexAndIndexBuffer(deviceContext, ObjectData.ObjectRenderData);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingVSMatrixBuffer(deviceContext, DirectX::XMLoadFloat4x4(&ObjectData.WorldMatrix), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSMaterialProperties(deviceContext, ObjectData.ObjectRenderData.Material);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_LinearWrapSampleState);

	result = m_DeferredFillGBufferShader.Render(deviceContext, ObjectData.ObjectRenderData.drawAmount, ObjectData.ObjectRenderData.start);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderSkyboxShader(ID3D11DeviceContext* deviceContext, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition)
{
	bool result;

	// Set the mat to the identity matrix.
	DirectX::XMMATRIX sphereWorldMat = DirectX::XMMatrixIdentity();

	//Define phere world matrix.
	DirectX::XMMATRIX Scale = DirectX::XMMatrixScaling(50.0f, 50.0f, 50.0f);
	//Make sure the sphere is always centered around camera
	DirectX::XMMATRIX Translation = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(eyePosition), DirectX::XMVectorGetY(eyePosition), DirectX::XMVectorGetZ(eyePosition));

	//Set sphere world matrix using the transformations
	sphereWorldMat = Scale * Translation;

	result = SetForwardShadingVSMatrixBuffer(deviceContext, sphereWorldMat, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSEyePositionBuffer(deviceContext, eyePosition);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_TestSampleState);

	result = m_SkyboxShader.Render(deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::ReloadIBLShader(ID3D11Device* device)
{
	bool result;

	result = m_IBLShader.ReloadShader(device);
	if (!result)
	{
		return false;
	}


	return true;

}

bool ShaderManager::RenderForwardIBLShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix,
	DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, ImageBasedLightData IBLData)
{
	bool result;

	result=SetVertexAndIndexBuffer(deviceContext, ObjectData.ObjectRenderData);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingVSMatrixBuffer(deviceContext, DirectX::XMLoadFloat4x4(&ObjectData.WorldMatrix), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSEyePositionBuffer(deviceContext, eyePosition);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSMaterialProperties(deviceContext, ObjectData.ObjectRenderData.Material);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_LinearWrapSampleState);
	deviceContext->PSSetSamplers(1, 1, &m_AnsiotropicWrapSampleState);
	deviceContext->PSSetSamplers(2, 1, &m_LinearClampSampleState);

	result = m_IBLShader.RenderForward(deviceContext, ObjectData.ObjectRenderData.drawAmount, ObjectData.ObjectRenderData.start, IBLData.DiffuseProbe, IBLData.SpecularProbe, IBLData.SpecularIntegration);
	if (!result)
	{
		return false;
	}


	return true;

}

bool ShaderManager::RenderDeferredIBLShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, ImageBasedLightData IBLData)
{
	bool result;

	result = SetDeferredShadingPSGBufferParameters(deviceContext, RenderData, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_PointClampSampleState);
	deviceContext->PSSetSamplers(1, 1, &m_AnsiotropicWrapSampleState);
	deviceContext->PSSetSamplers(2, 1, &m_LinearClampSampleState);

	result = m_IBLShader.RenderDeferred(deviceContext, IBLData.DiffuseProbe, IBLData.SpecularProbe, IBLData.SpecularIntegration);

	if (!result)
	{
		return false;
	}
	return true;

}

bool ShaderManager::RenderForwardDirectionalLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix,
	DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, DirectionalLightData DirectionalLight)
{
	bool result;

	result = SetVertexAndIndexBuffer(deviceContext, ObjectData.ObjectRenderData);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingVSMatrixBuffer(deviceContext, DirectX::XMLoadFloat4x4(&ObjectData.WorldMatrix), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSEyePositionBuffer(deviceContext, eyePosition);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSMaterialProperties(deviceContext, ObjectData.ObjectRenderData.Material);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_LinearWrapSampleState);

	result = m_DirectionalLightShader.RenderForward(deviceContext, ObjectData.ObjectRenderData.drawAmount, ObjectData.ObjectRenderData.start, DirectX::XMLoadFloat3(&DirectionalLight.AmbientLightDirection), DirectX::XMLoadFloat3(&DirectionalLight.AmbientLightColor), DirectX::XMLoadFloat3(&DirectionalLight.AmbientDown), DirectX::XMLoadFloat3(&DirectionalLight.AmbientUp));
	if (!result)
	{
		return false;
	}


	return true;

}

bool ShaderManager::RenderDeferredDirectionalLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectionalLightData DirectionalLight)
{
	bool result;

	result = SetDeferredShadingPSGBufferParameters(deviceContext, RenderData, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_PointClampSampleState);

	result = m_DirectionalLightShader.RenderDeferred(deviceContext, DirectX::XMLoadFloat3(&DirectionalLight.AmbientLightDirection), DirectX::XMLoadFloat3(&DirectionalLight.AmbientLightColor), DirectX::XMLoadFloat3(&DirectionalLight.AmbientDown), DirectX::XMLoadFloat3(&DirectionalLight.AmbientUp));

	if (!result)
	{
		return false;
	}
	return true;

}

bool ShaderManager::RenderForwardSpotLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, SpotLightData SpotLight)
{
	bool result;

	result = SetVertexAndIndexBuffer(deviceContext, ObjectData.ObjectRenderData);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingVSMatrixBuffer(deviceContext, DirectX::XMLoadFloat4x4(&ObjectData.WorldMatrix), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSEyePositionBuffer(deviceContext, eyePosition);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSMaterialProperties(deviceContext, ObjectData.ObjectRenderData.Material);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_LinearWrapSampleState);

	result = m_SpotLightShader.RenderForward(deviceContext, ObjectData.ObjectRenderData.drawAmount, ObjectData.ObjectRenderData.start, DirectX::XMLoadFloat3(&SpotLight.SpotLightPosition), DirectX::XMLoadFloat3(&SpotLight.SpotLightColor), DirectX::XMLoadFloat3(&SpotLight.SpotLightDir), SpotLight.SpotLightRange, SpotLight.SpotLightInnerAngel, SpotLight.SpotLightOuterAngel);

	if (!result)
	{
		return false;
	}

	return true;

}

bool ShaderManager::RenderDeferredSpotLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, SpotLightData SpotLight)
{
	bool result;

	result = SetDeferredShadingPSGBufferParameters(deviceContext, RenderData, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	
	deviceContext->PSSetSamplers(0, 1, &m_PointClampSampleState);

	result = m_SpotLightShader.RenderDeferred(deviceContext, viewMatrix, projectionMatrix, DirectX::XMLoadFloat3(&SpotLight.SpotLightPosition), DirectX::XMLoadFloat3(&SpotLight.SpotLightColor), DirectX::XMLoadFloat3(&SpotLight.SpotLightDir), SpotLight.SpotLightRange, SpotLight.SpotLightInnerAngel, SpotLight.SpotLightOuterAngel);

	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderForwardPointLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix,
	DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, PointLightData PointLight)
{
	bool result;

	result = SetVertexAndIndexBuffer(deviceContext, ObjectData.ObjectRenderData);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingVSMatrixBuffer(deviceContext, DirectX::XMLoadFloat4x4(&ObjectData.WorldMatrix), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSEyePositionBuffer(deviceContext, eyePosition);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSMaterialProperties(deviceContext, ObjectData.ObjectRenderData.Material);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_LinearWrapSampleState);

	result = m_PointLightShader.RenderForward(deviceContext, ObjectData.ObjectRenderData.drawAmount, ObjectData.ObjectRenderData.start, DirectX::XMLoadFloat3(&PointLight.PointLightPosition), DirectX::XMLoadFloat3(&PointLight.PointLightColor), PointLight.PointLightRange);

	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderDeferredPointLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, PointLightData PointLight)
{
	bool result;

	result = SetDeferredShadingPSGBufferParameters(deviceContext, RenderData, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_PointClampSampleState);

	result = m_PointLightShader.RenderDeferred(deviceContext, viewMatrix, projectionMatrix, DirectX::XMLoadFloat3(&PointLight.PointLightPosition), DirectX::XMLoadFloat3(&PointLight.PointLightColor), PointLight.PointLightRange);

	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderForwardCapsuleLightShader(ID3D11DeviceContext* deviceContext, SceneObjectData ObjectData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, DirectX::FXMVECTOR eyePosition, CapsuleLightData CapsuleLight)
{
	bool result;

	result = SetVertexAndIndexBuffer(deviceContext, ObjectData.ObjectRenderData);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingVSMatrixBuffer(deviceContext, DirectX::XMLoadFloat4x4(&ObjectData.WorldMatrix), viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSEyePositionBuffer(deviceContext, eyePosition);
	if (!result)
	{
		return false;
	}

	result = SetForwardShadingPSMaterialProperties(deviceContext, ObjectData.ObjectRenderData.Material);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_LinearWrapSampleState);

	result = m_CapsuleLightShader.RenderForward(deviceContext, ObjectData.ObjectRenderData.drawAmount, ObjectData.ObjectRenderData.start, DirectX::XMLoadFloat3(&CapsuleLight.CapsuleLightPosition), DirectX::XMLoadFloat3(&CapsuleLight.CapsuleLightColor), DirectX::XMLoadFloat3(&CapsuleLight.CapsuleLightDir), CapsuleLight.CapsuleLightLength, CapsuleLight.CapsuleLightRange);

	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManager::RenderDeferredCapsuleLightShader(ID3D11DeviceContext* deviceContext, DeferredRenderData RenderData, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, CapsuleLightData CapsuleLight)
{
	bool result;

	result = SetDeferredShadingPSGBufferParameters(deviceContext, RenderData, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_PointClampSampleState);

	result = m_CapsuleLightShader.RenderDeferred(deviceContext, viewMatrix, projectionMatrix, DirectX::XMLoadFloat3(&CapsuleLight.CapsuleLightPosition), DirectX::XMLoadFloat3(&CapsuleLight.CapsuleLightColor), DirectX::XMLoadFloat3(&CapsuleLight.CapsuleLightDir), CapsuleLight.CapsuleLightLength, CapsuleLight.CapsuleLightRange);

	if (!result)
	{
		return false;
	}

	return true;
}



