#ifndef _STATICMODEL_H_
#define _STATICMODEL_H_
#include <d3d12.h>
#include <directxmath.h>
#include <string>
#include <vector>
#include <wrl/client.h>


#include "SceneGraph.h"
#include "StaticMesh.h"


class StaticModel
{
public:

	StaticModel();
	StaticModel(ID3D11Device* device, SceneGraph& sceneGraph, std::string staticModelFilename);
	StaticModel(const StaticModel&);
	~StaticModel();

	bool LoadModel(ID3D11Device* device, SceneGraph& sceneGraph, std::string staticModelFilename);
private:
	float m_AABBWidth;
	float m_AABBHeight;
	float m_AABBDepth;

	SceneGraphNode* m_ModelRootSceneGraphNode;
};

#endif
