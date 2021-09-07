#include "MaterialManager.h"

MaterialManager::MaterialManager()
{
	m_Materials.clear();
}

MaterialManager::MaterialManager(const MaterialManager& other)
{
}

MaterialManager::~MaterialManager()
{

}

bool MaterialManager::Initialize(ID3D11Device* device, TextureManager* TexManager)
{
	m_TextureManager = TexManager;

	AddMaterialFile(device, "data/models/default.matlib");
	return true;
}

bool MaterialManager::AddMaterialFile(ID3D11Device* device, std::string MaterialFilename)
{
	bool result;
	
	//Clear the config data object and use it to parse the material file
	m_MaterialFile.ClearDataContainer();
	result = m_MaterialFile.LoadDataContainerFile(MaterialFilename);
	if (result)
	{
		int NumberOfMaterials = m_MaterialFile.GetNumberOfRootSections();

		for (int i = 0; i < NumberOfMaterials; i++)
		{
			SurfaceMaterialData tempMaterial;
			std::string tempFilename;

			std::vector<int> RootIndicies;

			result = m_MaterialFile.GetSectionName(m_MaterialFile.GetIndiciePathToRootSecton(i), tempMaterial.MaterialName);
			if (!result)
			{
				return false;
			}
			
			for (int k = 0; k < m_Materials.size(); k++)
			{
				if (m_Materials[k].MaterialName == tempMaterial.MaterialName)
				{
					return true;
				}
			}

			//Add color value to the material
			std::vector<float> tempAlbedo;
			result = m_MaterialFile.GetFloatVectorByName(tempMaterial.MaterialName, "albedo", tempAlbedo);
			if (!result || tempAlbedo.size() != 3)
			{
				tempAlbedo.clear();
				tempAlbedo.push_back(1.0f);
				tempAlbedo.push_back(0.0f);
				tempAlbedo.push_back(0.0f);
			}
			tempMaterial.Albedo.x = tempAlbedo[0];
			tempMaterial.Albedo.y = tempAlbedo[1];
			tempMaterial.Albedo.z = tempAlbedo[2];

			float tempRoughness;
			result = m_MaterialFile.GetFloatValueByName(tempMaterial.MaterialName, "roughness", tempRoughness);
			if (!result)
			{
				tempRoughness = 0.5f;
			}
			tempMaterial.Roughness = tempRoughness;

			float tempMetalness;
			result = m_MaterialFile.GetFloatValueByName(tempMaterial.MaterialName, "metalness", tempMetalness);
			if (!result)
			{
				tempMetalness = 0.0f;
			}
			tempMaterial.Metalness = tempMetalness;

			float tempF0;
			result = m_MaterialFile.GetFloatValueByName(tempMaterial.MaterialName, "f0", tempF0);
			if (!result)
			{
				tempF0 = 0.06;
			}
			tempMaterial.f0 = tempF0;

			float tempTransparency;
			result = m_MaterialFile.GetFloatValueByName(tempMaterial.MaterialName, "transparency", tempTransparency);
			if (!result)
			{
				tempTransparency = 1.0;
			}
			tempMaterial.Transparency = tempTransparency;


			//Add the albedoMap to the texture manager
			result = m_MaterialFile.GetStringValueByName(tempMaterial.MaterialName, "albedo_map", tempFilename);
			if (result)
			{
				tempMaterial.AlbedoMapFilename = tempFilename;
				result = m_TextureManager->AddTexture(device, tempMaterial.AlbedoMapFilename);
				if (!result)
				{
					tempMaterial.AlbedoMapFilename = "";
					tempMaterial.HasAlbedoMap = false;
				}
				else
				{
					tempMaterial.HasAlbedoMap = true;
				}
			}
			else
			{
				tempMaterial.AlbedoMapFilename = "";
				tempMaterial.HasAlbedoMap = false;
			}
			

			//Add the roughnessMap to the texture manager
			result = m_MaterialFile.GetStringValueByName(tempMaterial.MaterialName, "roughness_map", tempFilename);
			if (result)
			{
				tempMaterial.RoughnessMapFilename = tempFilename;
				result = m_TextureManager->AddTexture(device, tempMaterial.RoughnessMapFilename);
				if (!result)
				{
					tempMaterial.RoughnessMapFilename = "";
					tempMaterial.HasRoughnessMap = false;
				}
				else
				{
					tempMaterial.HasRoughnessMap = true;
				}
			}
			else
			{
				tempMaterial.RoughnessMapFilename = "";
				tempMaterial.HasRoughnessMap = false;
			}
			

			//Add the metalnessMap to the texture manager
			result = m_MaterialFile.GetStringValueByName(tempMaterial.MaterialName, "metalness_map", tempFilename);
			if (result)
			{
				tempMaterial.MetalnessMapFilename = tempFilename;
				result = m_TextureManager->AddTexture(device, tempMaterial.MetalnessMapFilename);
				if (!result)
				{
					tempMaterial.MetalnessMapFilename = "";
					tempMaterial.HasMetalnessMap = false;
				}
				else
				{
					tempMaterial.HasMetalnessMap = true;
				}
			}
			else
			{
				tempMaterial.MetalnessMapFilename = "";
				tempMaterial.HasMetalnessMap = false;
			}

			//Add the normalMap to the texture manager
			result = m_MaterialFile.GetStringValueByName(tempMaterial.MaterialName, "normal_map", tempFilename);
			if (result)
			{
				tempMaterial.NormalMapFilename = tempFilename;
				result = m_TextureManager->AddTexture(device, tempMaterial.NormalMapFilename);
				if (!result)
				{
					tempMaterial.NormalMapFilename = "";
					tempMaterial.HasNormalMap = false;
				}
				else
				{
					tempMaterial.HasNormalMap = true;
				}
			}
			else
			{
				tempMaterial.NormalMapFilename = "";
				tempMaterial.HasNormalMap = false;
			}

			bool renderForward;
			result = m_MaterialFile.GetBoolValueByName(tempMaterial.MaterialName, "render_forward", renderForward);
			if (!result)
			{
				renderForward = false;
			}
			tempMaterial.RenderForward = renderForward;

			bool IsTransparent = false;
			result = m_MaterialFile.GetBoolValueByName(tempMaterial.MaterialName, "is_transparent", IsTransparent);
			if (!result)
			{
				IsTransparent = false;
			}
			tempMaterial.IsTransparent = IsTransparent;

			bool UseAlphaChannelTransparency = false;
			result = m_MaterialFile.GetBoolValueByName(tempMaterial.MaterialName, "use_alphachannel_transparency", UseAlphaChannelTransparency);
			if (!result)
			{
				UseAlphaChannelTransparency = false;
			}
			tempMaterial.UseAlphaChannelTransparency = UseAlphaChannelTransparency;

			m_Materials.push_back(tempMaterial);
		}
	}
	else
	{
		return false;
	}

	return true;
}

SurfaceMaterial MaterialManager::GetMaterial(std::string MaterialName)
{
	auto it = std::find_if(m_Materials.begin(), m_Materials.end(), [&MaterialName](const SurfaceMaterialData& obj) {return obj.MaterialName == MaterialName; });

	if (it != m_Materials.end())
	{
		SurfaceMaterial tempMaterial;

		tempMaterial.AlbedoMap = m_TextureManager->GetTexture(it->AlbedoMapFilename);
		tempMaterial.MetalnessMap = m_TextureManager->GetTexture(it->MetalnessMapFilename);
		tempMaterial.RoughnessMap = m_TextureManager->GetTexture(it->RoughnessMapFilename);
		tempMaterial.NormalMap = m_TextureManager->GetTexture(it->NormalMapFilename);
		tempMaterial.Albedo = it->Albedo;
		tempMaterial.Roughness = it->Roughness;
		tempMaterial.Metalness = it->Metalness;
		tempMaterial.F0 = it->f0;
		tempMaterial.Transparency = it->Transparency;
		tempMaterial.RenderForward = it->RenderForward;
		tempMaterial.IsTransparent = it->IsTransparent;
		tempMaterial.UseAlphaChannelTransparency = it->UseAlphaChannelTransparency;
		tempMaterial.HasAlbedoMap = (tempMaterial.AlbedoMap != nullptr);
		tempMaterial.HasRoughnessMap = (tempMaterial.RoughnessMap != nullptr);
		tempMaterial.HasMetalnessMap = (tempMaterial.MetalnessMap != nullptr);
		tempMaterial.HasNormalMap = (tempMaterial.NormalMap != nullptr);

		return tempMaterial;
	}

	return GetMaterial("Default");
}

bool MaterialManager::SetMaterialAlbedoColor(std::string materialName, DirectX::FXMVECTOR albedoColor)
{
	for (auto it = m_Materials.begin(); it < m_Materials.end(); it++)
	{
		if (materialName == it->MaterialName)
		{
			DirectX::XMStoreFloat3(&it->Albedo, albedoColor);
			it->HasAlbedoMap = false;
			return true;
		}
	}
	return false;
}

bool MaterialManager::SetMaterialRoughness(std::string materialName, float roughness)
{
	for (auto it = m_Materials.begin(); it < m_Materials.end(); it++)
	{
		if (materialName == it->MaterialName)
		{
			it->Roughness = roughness;
			it->HasRoughnessMap = false;
			return true;
		}
	}
	return false;
}

bool MaterialManager::SetMaterialMetalness(std::string materialName, float metalness)
{
	for (auto it = m_Materials.begin(); it < m_Materials.end(); it++)
	{
		if (materialName == it->MaterialName)
		{
			it->Metalness = metalness;
			it->HasMetalnessMap = false;
			return true;
		}
	}
	return false;
}

bool MaterialManager::SetMaterialAlbedoMap(ID3D11Device* device, std::string materialName, std::string albedoMapFilename)
{
	bool result;
	for (auto it = m_Materials.begin(); it < m_Materials.end(); it++)
	{
		if (materialName == it->MaterialName)
		{
			//Add the albedoMap to the texture manager
			result = m_TextureManager->AddTexture(device, albedoMapFilename);
			if (!result)
			{
				it->AlbedoMapFilename = "";
				it->HasAlbedoMap = false;
			}
			else
			{
				it->AlbedoMapFilename = albedoMapFilename;
				it->HasAlbedoMap = true;
			}
			return true;
		}
	}
	return false;
}

bool MaterialManager::SetMaterialRoughnessMap(ID3D11Device* device, std::string materialName, std::string roughnessMapFilename)
{
	bool result;
	for (auto it = m_Materials.begin(); it < m_Materials.end(); it++)
	{
		if (materialName == it->MaterialName)
		{
			//Add the albedoMap to the texture manager
			result = m_TextureManager->AddTexture(device, roughnessMapFilename);
			if (!result)
			{
				it->RoughnessMapFilename = "";
				it->HasRoughnessMap = false;
			}
			else
			{
				it->RoughnessMapFilename = roughnessMapFilename;
				it->HasRoughnessMap = true;
			}
			return true;
		}
	}
	return false;
}

bool MaterialManager::SetMaterialMetalnessMap(ID3D11Device* device, std::string materialName, std::string metalnessMapFilename)
{
	bool result;
	for (auto it = m_Materials.begin(); it < m_Materials.end(); it++)
	{
		if (materialName == it->MaterialName)
		{
			//Add the albedoMap to the texture manager
			result = m_TextureManager->AddTexture(device, metalnessMapFilename);
			if (!result)
			{
				it->MetalnessMapFilename = "";
				it->HasMetalnessMap = false;
			}
			else
			{
				it->MetalnessMapFilename = metalnessMapFilename;
				it->HasMetalnessMap = true;
			}
			return true;
		}
	}
	return false;
}

bool MaterialManager::SetMaterialNormalMap(ID3D11Device * device, std::string materialName, std::string normalMapFilename)
{
	bool result;
	for (auto it = m_Materials.begin(); it < m_Materials.end(); it++)
	{
		if (materialName == it->MaterialName)
		{
			//Add the albedoMap to the texture manager
			result = m_TextureManager->AddTexture(device, normalMapFilename);
			if (!result)
			{
				it->NormalMapFilename = "";
				it->HasNormalMap = false;
			}
			else
			{
				it->NormalMapFilename = normalMapFilename;
				it->HasNormalMap = true;
			}
			return true;
		}
	}
	return false;
}
