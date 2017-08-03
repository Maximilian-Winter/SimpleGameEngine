#include "ModelManager.h"


ModelManager::ModelManager()
{
}

ModelManager::ModelManager(const ModelManager& other)
{

}

ModelManager::~ModelManager()
{
	for (int i = 0; i < m_StaticModels.size(); i++)
	{
		m_StaticModels[i].Model.Shutdown();
	}

	for (int i = 0; i < m_AnimatedModels.size(); i++)
	{
		m_AnimatedModels[i].Model.Shutdown();
	}

	for (int i = 0; i < m_TerrainModels.size(); i++)
	{
		m_TerrainModels[i].Model.Shutdown();
	}

}

bool ModelManager::Init(ID3D11Device* device, MaterialManager* materialManager)
{
	bool result;
	result = AddStaticModel(device, materialManager, "data\model\TestBlock.smo");
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelManager::AddStaticModel(ID3D11Device* device, MaterialManager* materialManager, std::string modelFilename)
{
	bool result;

	//Check if model exist
	auto it = std::find_if(m_StaticModels.begin(), m_StaticModels.end(), [&modelFilename](const StaticModelData& obj) {return obj.Filename == modelFilename; });

	if (it != m_StaticModels.end())
	{
		return true;
	}

	StaticModelData TempData;

	result = TempData.Model.Initialize(device, materialManager, modelFilename);
	if (!result)
	{
		return false;
	}

	TempData.Filename = modelFilename;

	m_StaticModels.push_back(TempData);

	return true;

}

PBStaticModel& ModelManager::GetStaticModel(std::string Filename)
{
	//Check if model exist
	auto it = std::find_if(m_StaticModels.begin(), m_StaticModels.end(), [&Filename](const StaticModelData& obj) {return obj.Filename == Filename; });

	if (it != m_StaticModels.end())
	{
		return it->Model;
	}

	return m_StaticModels[0].Model;
}

bool ModelManager::AddAnimatedModel(ID3D11Device* device, MaterialManager* materialManager, std::string modelFilename)
{
	bool result;

	//Check if model exist
	auto it = std::find_if(m_AnimatedModels.begin(), m_AnimatedModels.end(), [&modelFilename](const AnimatedModelData& obj) {return obj.Filename == modelFilename; });

	if (it != m_AnimatedModels.end())
	{
		return true;
	}

	AnimatedModelData TempData;

	result = TempData.Model.Initialize(device, modelFilename, materialManager);
	if (!result)
	{
		return false;
	}

	TempData.Filename = modelFilename;

	m_AnimatedModels.push_back(TempData);

	return true;

}

AnimatedModel& ModelManager::GetAnimatedModel(std::string Filename)
{
	//Check if model exist
	auto it = std::find_if(m_AnimatedModels.begin(), m_AnimatedModels.end(), [&Filename](const AnimatedModelData& obj) {return obj.Filename == Filename; });

	if (it != m_AnimatedModels.end())
	{
		return it->Model;
	}

	return m_AnimatedModels[0].Model;
}

bool ModelManager::AddTerrainModel(ID3D11Device* device, MaterialManager* materialManager, std::string modelFilename)
{
	bool result;

	//Check if model exist
	auto it = std::find_if(m_TerrainModels.begin(), m_TerrainModels.end(), [&modelFilename](const TerrainModelData& obj) {return obj.Filename == modelFilename; });

	if (it != m_TerrainModels.end())
	{
		return true;
	}

	TerrainModelData TempData;

	result = TempData.Model.Initialize(device, materialManager, modelFilename);
	if (!result)
	{
		return false;
	}

	TempData.Filename = modelFilename;

	m_TerrainModels.push_back(TempData);

	return true;

}

Terrain& ModelManager::GetTerrainModel(std::string Filename)
{
	//Check if model exist
	auto it = std::find_if(m_TerrainModels.begin(), m_TerrainModels.end(), [&Filename](const TerrainModelData& obj) {return obj.Filename == Filename; });

	if (it != m_TerrainModels.end())
	{
		return it->Model;
	}

	return m_TerrainModels[0].Model;
}