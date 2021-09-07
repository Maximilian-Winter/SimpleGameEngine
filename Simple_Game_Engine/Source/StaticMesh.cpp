#include "StaticMesh.h"

#include "Logger.h"

StaticMesh::StaticMesh(): m_VertexBuffer(nullptr), m_IndexBuffer(nullptr), m_VertexCount(0), m_IndexCount(0),
                          m_AABBWidth(0), m_AABBHeight(0),
                          m_AABBDepth(0),
                          m_HasCollisionMesh(false)
{
}

StaticMesh::~StaticMesh()
{

}

VerticesRenderData StaticMesh::GetSubsetRenderData(int Subset)
{
	VerticesRenderData TempData;

	TempData.drawAmount = m_MeshSubsets[Subset].drawAmount;
	TempData.start = m_MeshSubsets[Subset].start;
	TempData.Material = MaterialManager::GetInstance()->GetMaterial(m_MeshSubsets[Subset].materialName);
	TempData.IndexBuffer = m_IndexBuffer;
	TempData.VertexBuffer = m_VertexBuffer;
	TempData.VertexBufferOffset = 0;
	TempData.VertexBufferStride = sizeof(StaticVertexDXMath);

	return TempData;
}

int StaticMesh::GetSubsetCount()
{
	return m_MeshSubsets.size();
}

void StaticMesh::Release()
{
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}
}


bool StaticMesh::CreateMeshData(ID3D11Device* device, std::vector<StaticVertex> meshvertices, std::vector<int> meshindicies, std::vector<Subset> subsets, float aabbWidth, float aabbHeight, float aabbLength)
{
	m_AABBWidth = aabbWidth;
	m_AABBHeight = aabbHeight;
	m_AABBDepth = aabbLength;
	m_VertexCount = meshvertices.size();
	m_IndexCount = meshindicies.size();
	m_MeshSubsets = subsets;
	
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new StaticVertexDXMath[m_VertexCount];
	if (!vertices)
	{
		Logger::GetInstance()->Log("ModelLoadFailedBuffer", "vertices == null");
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_IndexCount];
	if (!indices)
	{
		Logger::GetInstance()->Log("ModelLoadFailedBuffer", "indices == null");
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_VertexCount; i++)
	{
		vertices[i].position = DirectX::XMFLOAT3(meshvertices[i].x, meshvertices[i].y, meshvertices[i].z);
		vertices[i].texture = DirectX::XMFLOAT2(meshvertices[i].tu, meshvertices[i].tv);
		vertices[i].normal = DirectX::XMFLOAT3(meshvertices[i].nx, meshvertices[i].ny, meshvertices[i].nz);
	}

	for (i = 0; i < m_IndexCount; i++)
	{
		indices[i] = meshindicies[i];
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(StaticVertexDXMath) * m_VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
	if (FAILED(result))
	{
		Logger::GetInstance()->Log("ModelLoadFailedBuffer", "CreateVertexBuffer");
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);
	if (FAILED(result))
	{
		Logger::GetInstance()->Log("ModelLoadFailedBuffer", "CreateIndexBuffer");
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;



    return true;
}


