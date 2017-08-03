//World Scene Class 
//Class to manage Scene Data of Objects
//eg: World Matrix, Position
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_



// Includes
#include "MaterialManager.h"
#include "PBStaticModel.h"
#include "Terrain.h"
#include "AnimatedModel.h"

class ModelManager
{
	struct StaticModelData
	{
		std::string Filename;
		PBStaticModel Model;
	};

	struct AnimatedModelData
	{
		std::string Filename;
		AnimatedModel Model;
	};

	struct TerrainModelData
	{
		std::string Filename;
		Terrain Model;
	};

public:
	ModelManager();
	ModelManager(const ModelManager& other);
	~ModelManager();

	bool Init(ID3D11Device* device, MaterialManager* materialManager);

	bool AddStaticModel(ID3D11Device* device, MaterialManager* materialManager, std::string modelFilename);
	PBStaticModel& GetStaticModel(std::string filename);

	bool AddAnimatedModel(ID3D11Device* device, MaterialManager* materialManager, std::string modelFilename);
	AnimatedModel& GetAnimatedModel(std::string filename);

	bool AddTerrainModel(ID3D11Device* device, MaterialManager* materialManager, std::string modelFilename);
	Terrain& GetTerrainModel(std::string filename);

private:
	std::vector<StaticModelData> m_StaticModels;
	std::vector<AnimatedModelData> m_AnimatedModels;
	std::vector<TerrainModelData> m_TerrainModels;
};


#endif