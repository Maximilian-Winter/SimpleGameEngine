#include "TextureManager.h"


TextureManager::TextureManager()
{
	
}

TextureManager::TextureManager(const TextureManager& other)
{

}

TextureManager::~TextureManager()
{
	for (int i = 0; i < Textures.size(); i++)
	{
		Textures[i].Texture.Shutdown();
	}
}

bool TextureManager::AddTexture(ID3D11Device* device, std::string TextureFilename)
{
	bool result;
	
	//Check if texture exist
	auto it = std::find_if(Textures.begin(), Textures.end(), [&TextureFilename](const TextureData& obj) {return obj.Filename == TextureFilename; });

	if (it != Textures.end())
	{
		return true;
	}

	TextureData TempData;

	//Convert the string into wcstring
	size_t tFilename = TextureFilename.length() + 1;
	wchar_t * wcFilename = new wchar_t[tFilename];

	size_t FilenameConvertedChars = 0;
	mbstowcs_s(&FilenameConvertedChars, wcFilename, tFilename, TextureFilename.c_str(), _TRUNCATE);

	result = TempData.Texture.Initialize(device, wcFilename);
	if (FAILED(result))
	{
		return false;
	}

	TempData.Filename = TextureFilename;

	Textures.push_back(TempData);

	return true;

}
ID3D11ShaderResourceView* TextureManager::GetTexture(std::string TextureName)
{
	//Check if texture exist
	auto it = std::find_if(Textures.begin(), Textures.end(), [&TextureName](const TextureData& obj) {return obj.Filename == TextureName; });

	if (it != Textures.end())
	{
		return it->Texture.GetTexture();
	}

	return Textures[0].Texture.GetTexture();
}