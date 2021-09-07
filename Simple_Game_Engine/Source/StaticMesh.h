#ifndef _STATICMESH_H_
#define _STATICMESH_H_

#include <string>
#include <vector>
#include <wrl/client.h>
#include <fstream>

#include "MaterialManager.h"
#include "ModelTypes.h"
#include "Transform.h"



class StaticMesh
{
public:
	StaticMesh();
	~StaticMesh();

	bool CreateMeshData(ID3D11Device* device, std::vector<StaticVertex> vertices, std::vector<int> indicies, std::vector<Subset> subsets, float aabbWidth, float aabbHeight, float aabbLength);

	VerticesRenderData StaticMesh::GetSubsetRenderData(int Subset);
	int GetSubsetCount();

	void Release();

private:

	ID3D11Buffer* m_VertexBuffer, * m_IndexBuffer;

	StaticVertexDXMath* vertices;
	unsigned long* indices;

	int m_VertexCount, m_IndexCount;

	float m_AABBWidth;
	float m_AABBHeight;
	float m_AABBDepth;

	std::vector<Subset> m_MeshSubsets;

	bool m_HasCollisionMesh;
};

#endif
