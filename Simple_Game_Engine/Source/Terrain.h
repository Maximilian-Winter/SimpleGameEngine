//Class to handle terrain data.
#ifndef _TERRAIN_H_
#define _TERRAIN_H_


// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <stdio.h>
#include "MaterialManager.h"
#include "PBStaticModel.h"

// Globals
const int TEXTURE_REPEAT = 16;


class Terrain
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct HeightMapType 
	{ 
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType 
	{ 
		float x, y, z;
	};

public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device*, MaterialManager*, std::string);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetSubsetCount();

	void GetWidthHeightAndDepth(float& width, float& height, float& depth);

	VerticesRenderData GetSubsetRenderData(MaterialManager* materialManager , int subsetIndex);

private:
	bool LoadHeightMap(std::string);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void ShutdownHeightMap();

	void CalculateTextureCoordinates();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	
private:
	int m_terrainWidth;
	int m_terrainHeight;
	int m_vertexCount;
	int m_indexCount;

	float m_AABB_width;
	float m_AABB_height;
	float m_AABB_depth;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	HeightMapType* m_heightMap;
	std::string m_MaterialName;
};

#endif