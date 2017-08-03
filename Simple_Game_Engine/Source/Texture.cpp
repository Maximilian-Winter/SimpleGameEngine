#include "Texture.h"


Texture::Texture()
{
	m_texture = 0;
}


Texture::Texture(const Texture& other)
{
	this->m_texture = other.m_texture;
}


Texture::~Texture()
{
}


bool Texture::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// Load the dds texture in.
	result = DirectX::CreateDDSTextureFromFile(device, filename, nullptr, &m_texture);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void Texture::Shutdown()
{
	// Release the dds texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

void Texture::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_texture = texture;
}