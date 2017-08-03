//Texture
//A simple class to load a dds texture and use it as ID3D11ShaderResourceView*.

#ifndef _TEXTURE_H_
#define _TEXTURE_H_


//Include

#include <d3d11.h>
#include "DDSUtil\DDSTextureLoader\DDSTextureLoader.h"


//Class name: Texture

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	void SetTexture(ID3D11ShaderResourceView*);

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif