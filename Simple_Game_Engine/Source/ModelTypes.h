#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>


struct SurfaceMaterial
{
	ID3D11ShaderResourceView* AlbedoMap;
	ID3D11ShaderResourceView* RoughnessMap;
	ID3D11ShaderResourceView* MetalnessMap;
	ID3D11ShaderResourceView* NormalMap;
	DirectX::XMFLOAT3 Albedo;
	float Roughness;
	float Metalness;
	float F0;
	float Transparency;
	bool RenderForward;
	bool IsTransparent;
	bool UseAlphaChannelTransparency;
	bool HasAlbedoMap;
	bool HasRoughnessMap;
	bool HasMetalnessMap;
	bool HasNormalMap;
};

//Structure to transfer the data to the render class.
struct VerticesRenderData
{
	SurfaceMaterial Material;
	int start;
	int drawAmount;
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VertexBufferStride;
	UINT VertexBufferOffset;
};



struct Subset
{
	Subset() :
		materialName(""),
		start(0),
		drawAmount(0)
	{
	}

	Subset(std::string materialName, int start, int drawAmount)
		:materialName(materialName), start(start), drawAmount(drawAmount)
	{
	}
	std::string materialName;
	int start;
	int drawAmount;
};

struct StaticVertexDXMath
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
};


struct StaticVertex
{
	StaticVertex() : x(0), y(0), z(0), tu(0), tv(0), nx(0), ny(0), nz(0)
	{
	}

	StaticVertex(float x, float y, float z, float tu, float tv, float nx, float ny, float nz)
		:x(x), y(y), z(z), tu(tu), tv(tv), nx(nx), ny(ny), nz(nz)
	{}

	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
};