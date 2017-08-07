#include "ShaderHelper.h"

void ShaderHelper::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
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

DirectX::XMFLOAT3 ShaderHelper::GammaToLinear(const DirectX::XMFLOAT3 & color)
{
	return DirectX::XMFLOAT3(color.x * color.x, color.y * color.y, color.z * color.z);
}

HRESULT ShaderHelper::CompileShader(ID3D11Device * device, HWND hwnd, WCHAR * shaderFilename, LPCSTR shaderEntryPoint, LPCSTR shaderTarget, ID3D10Blob ** pShaderBuffer)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	// Compile the vertex shader code.
	result = D3DCompileFromFile(shaderFilename, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, shaderEntryPoint, shaderTarget, D3D10_SHADER_ENABLE_STRICTNESS, 0, pShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			ShaderHelper::OutputShaderErrorMessage(errorMessage, hwnd, shaderFilename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(hwnd, shaderFilename, L"Shader Compiler Error", MB_OK);
		}

		return result;
	}

	return result;
}

HRESULT ShaderHelper::CreateVertexShader(ID3D11Device* device, HWND hwnd, WCHAR* shaderFilename, LPCSTR shaderEntryPoint, ID3D10Blob** pVertexShaderBuffer, ID3D11VertexShader** pVertexShader)
{
	HRESULT result;
	result = CompileShader(device, hwnd, shaderFilename, shaderEntryPoint, "vs_5_0", pVertexShaderBuffer);
	if (FAILED(result))
	{
		return result;
	}

	result = device->CreateVertexShader((*pVertexShaderBuffer)->GetBufferPointer(), (*pVertexShaderBuffer)->GetBufferSize(), NULL, pVertexShader);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT ShaderHelper::CreatePixelShader(ID3D11Device * device, HWND hwnd, WCHAR * shaderFilename, LPCSTR shaderEntryPoint, ID3D10Blob ** pPixelShaderBuffer, ID3D11PixelShader** pPixelShader)
{
	HRESULT result;
	result = CompileShader(device, hwnd, shaderFilename, shaderEntryPoint, "ps_5_0", pPixelShaderBuffer);
	if (FAILED(result))
	{
		return result;
	}

	result = device->CreatePixelShader((*pPixelShaderBuffer)->GetBufferPointer(), (*pPixelShaderBuffer)->GetBufferSize(), NULL, pPixelShader);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT ShaderHelper::CreateHullShader(ID3D11Device * device, HWND hwnd, WCHAR * shaderFilename, LPCSTR shaderEntryPoint, ID3D10Blob ** pHullShaderBuffer, ID3D11HullShader ** pHullShader)
{
	HRESULT result;
	result = CompileShader(device, hwnd, shaderFilename, shaderEntryPoint, "hs_5_0", pHullShaderBuffer);
	if (FAILED(result))
	{
		return result;
	}

	result = device->CreateHullShader((*pHullShaderBuffer)->GetBufferPointer(), (*pHullShaderBuffer)->GetBufferSize(), NULL, pHullShader);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT ShaderHelper::CreateDomainShader(ID3D11Device * device, HWND hwnd, WCHAR * shaderFilename, LPCSTR shaderEntryPoint, ID3D10Blob ** pDomainShaderBuffer, ID3D11DomainShader ** pDomainShader)
{
	HRESULT result;
	result = CompileShader(device, hwnd, shaderFilename, shaderEntryPoint, "ds_5_0", pDomainShaderBuffer);
	if (FAILED(result))
	{
		return result;
	}

	result = device->CreateDomainShader((*pDomainShaderBuffer)->GetBufferPointer(), (*pDomainShaderBuffer)->GetBufferSize(), NULL, pDomainShader);
	if (FAILED(result))
	{
		return result;
	}

	return result;
}

HRESULT ShaderHelper::CreateConstantBuffer(ID3D11Device* device, D3D11_USAGE bufferUsage, UINT byteWidth, D3D11_BIND_FLAG bindFlag, D3D11_CPU_ACCESS_FLAG accessFlag, UINT miscFlags, UINT byteStride, ID3D11Buffer** buffer)
{
	HRESULT result;

	D3D11_BUFFER_DESC bufferDesc;
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	bufferDesc.Usage = bufferUsage;
	bufferDesc.ByteWidth = byteWidth;
	bufferDesc.BindFlags = bindFlag;
	bufferDesc.CPUAccessFlags = accessFlag;
	bufferDesc.MiscFlags = miscFlags;
	bufferDesc.StructureByteStride = byteStride;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&bufferDesc, NULL, buffer);

	return result;
}

HRESULT ShaderHelper::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob * pShaderBlob, ID3D11Device * pD3DDevice, ID3D11InputLayout ** pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (int i = 0; i< shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}
