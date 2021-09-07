//Material Manager Class
//Class to manage the Material data of 3D Objects.
#ifndef _MATERIALMANAGER_H_
#define _MATERIALMANAGER_H_

//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include "TextureManager.h"
#include "DataFileContainer.h"
#include "Singleton.h"
#include "ModelTypes.h"



class MaterialManager : public Singleton<MaterialManager>
{
	friend class Singleton<MaterialManager>;
private:
	struct SurfaceMaterialData
	{
		SurfaceMaterialData()
		{
			MaterialName = "";
			AlbedoMapFilename = "";
			RoughnessMapFilename = "";
			MetalnessMapFilename = "";
			Albedo = { DirectX::XMFLOAT3(0.0f, 0.0f,0.0f) };
			Roughness = 0.0f;
			Metalness = 0.0f;
			f0 = 0.0f;
			Transparency = 1.0f;
			RenderForward = false;
			IsTransparent = false;
			UseAlphaChannelTransparency = false;
			HasAlbedoMap = false;
			HasRoughnessMap = false;
			HasMetalnessMap = false;
			HasNormalMap = false;

		}
		std::string MaterialName;
		std::string AlbedoMapFilename;
		std::string RoughnessMapFilename;
		std::string MetalnessMapFilename;
		std::string NormalMapFilename;
		DirectX::XMFLOAT3 Albedo;
		float Roughness;
		float Metalness;
		float f0;
		float Transparency;
		bool RenderForward;
		bool IsTransparent;
		bool UseAlphaChannelTransparency;
		bool HasAlbedoMap;
		bool HasRoughnessMap;
		bool HasMetalnessMap;
		bool HasNormalMap;
	};

public:
	
	MaterialManager();
	MaterialManager(const MaterialManager&);
	~MaterialManager();

	bool Initialize(ID3D11Device* device, TextureManager* textureManager);
	bool AddMaterialFile(ID3D11Device* device, std::string fileName);
	SurfaceMaterial GetMaterial(std::string materialName);

	bool SetMaterialAlbedoColor(std::string materialName, DirectX::FXMVECTOR albedoColor);
	bool SetMaterialRoughness(std::string materialName, float roughness);
	bool SetMaterialMetalness(std::string materialName, float metalness);
	bool SetMaterialAlbedoMap(ID3D11Device* device, std::string materialName, std::string albedoMapFilename);
	bool SetMaterialRoughnessMap(ID3D11Device* device, std::string materialName, std::string roughnessMapFilename);
	bool SetMaterialMetalnessMap(ID3D11Device* device, std::string materialName, std::string metalnessMapFilename);
	bool SetMaterialNormalMap(ID3D11Device* device, std::string materialName, std::string normalMapFilename);

private:

	DataFileContainer m_MaterialFile;
	std::vector<SurfaceMaterialData> m_Materials;
	TextureManager* m_TextureManager;
	

};
#endif