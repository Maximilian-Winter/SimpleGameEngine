//TextureManager
//Class to manage Texture load and release.
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include <d3d11.h>
#include <vector>
#include <algorithm>
#include "Texture.h"
#include <string>

class TextureManager
{
	public:

		struct TextureData
		{
			std::string Filename;
			Texture Texture;
		};

		TextureManager();
		TextureManager(const TextureManager& other);
		~TextureManager();
		
		bool AddTexture(ID3D11Device* device, std::string Filename);
		ID3D11ShaderResourceView* GetTexture(std::string TextureName);

	private:

	std::vector<TextureData> Textures;
};
#endif