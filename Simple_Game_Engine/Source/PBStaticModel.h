//Physically Based Shading Model Class
//Hold the data of the 3D Model and provide an interface for rendering
#ifndef _PBSTATICMODEL_H_
#define _PBSTATICMODEL_H_


//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <istream>
#include <vector>
#include <sstream>
#include "MaterialManager.h"
#include "ModelTypes.h"


// Class name: PBStaticModel

class PBStaticModel
{
private:
	struct StaticModelType
	{
		StaticModelType()
		{}

		StaticModelType(float x, float y, float z, float tu, float tv, float nx, float ny, float nz)
			:x(x), y(y), z(z), tu(tu), tv(tv), nx(nx), ny(ny), nz(nz)
		{}

		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct SubsetType
	{
		SubsetType()
		{
		}

		SubsetType(std::string materialName, int start, int drawAmount)
			:materialName(materialName), start(start), drawAmount(drawAmount)
		{
		}
		std::string materialName;
		int start;
		int drawAmount;
	};


public:
	struct VertexPos
	{
		VertexPos(float X, float Y, float Z)
			:x(X), y(Y), z(Z)
		{
		}
		float x, y, z;
	};

	PBStaticModel();
	PBStaticModel(const PBStaticModel&);
	~PBStaticModel();

	bool Initialize(ID3D11Device*, MaterialManager*, std::string);
	void Shutdown();

	int GetIndexCount();
	int GetSubsetCount();
	int GetSubsetStart(int subset);
	int GetDrawAmountFromSubset(int subset);

	std::vector<float>& GetVertices();
	std::vector<int>& GetIndicies();

	bool GetCollisionMeshFilename(std::string& Filename);

	void GetWidthHeightAndDepth(float& width, float& height, float& depth);

	VerticesRenderData GetSubsetRenderData(MaterialManager*, int Subset);


private:

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();

	bool LoadModel(ID3D11Device*, std::string, MaterialManager*);
	void ReleaseModel();

	ID3D11Buffer* GetVertexBufferFromSubset(int Subset);
	ID3D11Buffer* GetIndexBufferFromSubset(int Subset);

	ID3D11Buffer *m_VertexBuffer, *m_IndexBuffer;

	int m_VertexCount, m_IndexCount;
	float m_AABB_width;
	float m_AABB_height;
	float m_AABB_depth;
	
	std::vector<StaticModelType> m_Model;
	std::vector<SubsetType> m_Subsets;
	std::vector<float> m_Vertices;
	std::vector<int> m_Indicies;
	
	std::string m_MeshFilename;
	std::string m_CollisionMeshFilename;
	bool m_HasCollisionMesh;
};

#endif