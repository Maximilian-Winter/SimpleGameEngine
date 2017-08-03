////////////////////////////////////////////////////////////////////////////////
// Filename: PBStaticModel.cpp
////////////////////////////////////////////////////////////////////////////////
#include "PBStaticModel.h"


PBStaticModel::PBStaticModel()
{
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
	m_VertexCount = 0;
	m_IndexCount = 0;

	m_AABB_width = 0;
	m_AABB_height = 0;
	m_AABB_depth = 0;

	m_CollisionMeshFilename = "";
	m_MeshFilename = "";

	m_HasCollisionMesh = false;
}

PBStaticModel::PBStaticModel(const PBStaticModel& other)
{
	
	*this = other;
}

PBStaticModel::~PBStaticModel()
{
}

bool PBStaticModel::Initialize(ID3D11Device* device, MaterialManager* MaterialManager, std::string filename)
{
	bool result;

	// Load in the model data,
	result = LoadModel(device,filename, MaterialManager);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}

void PBStaticModel::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}

int PBStaticModel::GetIndexCount()
{
	return m_IndexCount;
}

int PBStaticModel::GetSubsetCount()
{
	return m_Subsets.size();
}

int PBStaticModel::GetSubsetStart(int subset)
{

	int startIndex=m_Subsets[subset].start;
	return startIndex;
}

int PBStaticModel::GetDrawAmountFromSubset(int subset)
{
	int amount=m_Subsets[subset].drawAmount;
	return amount;
}

std::vector<float>& PBStaticModel::GetVertices()
{
	return m_Vertices;
}

std::vector<int>& PBStaticModel::GetIndicies()
{
	return m_Indicies;
}

bool PBStaticModel::GetCollisionMeshFilename(std::string & Filename)
{
	Filename = m_CollisionMeshFilename;

	return m_HasCollisionMesh;
}

void PBStaticModel::GetWidthHeightAndDepth(float& width, float& height, float& depth)
{
	width = m_AABB_width;
	height = m_AABB_height;
	depth = m_AABB_depth;
}

ID3D11Buffer* PBStaticModel::GetVertexBufferFromSubset(int Subset)
{
	return m_VertexBuffer;
}

ID3D11Buffer* PBStaticModel::GetIndexBufferFromSubset(int Subset)
{
	return m_IndexBuffer;
}

bool PBStaticModel::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_VertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_IndexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(i=0; i < m_VertexCount; i++)
	{
		vertices[i].position = DirectX::XMFLOAT3(m_Model[i].x, m_Model[i].y, m_Model[i].z);
		vertices[i].texture = DirectX::XMFLOAT2(m_Model[i].tu, m_Model[i].tv);
		vertices[i].normal = DirectX::XMFLOAT3(m_Model[i].nx, m_Model[i].ny, m_Model[i].nz);

		indices[i] = i;
		m_Indicies.push_back(i);
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
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
	if(FAILED(result))
	{
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
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void PBStaticModel::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}

	return;
}

VerticesRenderData PBStaticModel::GetSubsetRenderData(MaterialManager* MaterialManager, int Subset)
{
	VerticesRenderData TempData;

	TempData.drawAmount = m_Subsets[Subset].drawAmount;
	TempData.start = m_Subsets[Subset].start;
	TempData.Material = MaterialManager->GetMaterial(m_Subsets[Subset].materialName);
	TempData.IndexBuffer = m_IndexBuffer;
	TempData.VertexBuffer = m_VertexBuffer;
	TempData.VertexBufferOffset = 0;
	TempData.VertexBufferStride = sizeof(VertexType);

	return TempData;
}

bool PBStaticModel::LoadModel(ID3D11Device* d3d11Device, std::string filename, MaterialManager* MaterialManager)
{
	bool result;

	std::ifstream fin;
	std::string inputString;
	std::string MaterialFilename;
	int subsetCount = 0;

	m_MeshFilename = filename;

	ConfigData MeshInputFile;

	result = MeshInputFile.LoadConfigDataFile(filename);
	if (!result)
	{
		return false;
	}

	result = MeshInputFile.GetIntValueByName("Mesh", "Vertex_Count", m_VertexCount);
	if (!result)
	{
		return false;
	}

	m_IndexCount = m_VertexCount;

	result = MeshInputFile.GetIntValueByName("Mesh", "Subset_Count", subsetCount);
	if (!result)
	{
		return false;
	}

	result = MeshInputFile.GetStringValueByName("Mesh", "Material_Libary", MaterialFilename);
	if (!result)
	{
		return false;
	}

	m_HasCollisionMesh = MeshInputFile.GetStringValueByName("Mesh", "Collision_Mesh_File", m_CollisionMeshFilename);
	if (!m_HasCollisionMesh)
	{
		m_CollisionMeshFilename = "";
	}

	MaterialManager->AddMaterialFile(d3d11Device, MaterialFilename);

	//Read in and initliaze the subset material 
	for (int p = 0; p < subsetCount; p++)
	{
		std::string meshSection = "Mesh_Subset" + std::to_string(p);
		std::string subsetMaterialName = "";
		int drawAmount = 0;
		int subsetStart = 0;

		result = MeshInputFile.GetStringValueByName(meshSection, "Subset_Material_Name", subsetMaterialName);
		if (!result)
		{
			return false;
		}

		result = MeshInputFile.GetIntValueByName(meshSection, "Subset_Draw_Amount", drawAmount);
		if (!result)
		{
			return false;
		}

		result = MeshInputFile.GetIntValueByName(meshSection, "Subset_Start", subsetStart);
		if (!result)
		{
			return false;
		}
		m_Subsets.push_back(SubsetType(subsetMaterialName, subsetStart, drawAmount));
	}

	std::vector<float> vertices;

	result = MeshInputFile.GetFloatVectorByName("Mesh", "Vertices", vertices);
	if (!result)
	{
		return false;
	}

	if (m_VertexCount * 8 != vertices.size())
	{
		return false;
	}
	
	DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// Read in the vertex data and calculate object aligned bounding box.

	for (int i = 0; i < vertices.size(); i += 8)
	{
		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, vertices[i]);	// Find smallest x value in model
		minVertex.y = min(minVertex.y, vertices[i + 1]);	// Find smallest y value in model
		minVertex.z = min(minVertex.z, vertices[i + 2]);	// Find smallest z value in model

														//Get the largest vertex 
		maxVertex.x = max(maxVertex.x, vertices[i]);	// Find largest x value in model
		maxVertex.y = max(maxVertex.y, vertices[i + 1]);	// Find largest y value in model
		maxVertex.z = max(maxVertex.z, vertices[i + 2]);	// Find largest z value in model

		m_Model.push_back(StaticModelType(vertices[i], vertices[i + 1], vertices[i + 2], vertices[i + 3], vertices[i + 4], vertices[i + 5], vertices[i + 6], vertices[i + 7]));
	
		m_Vertices.push_back(vertices[i]);
		m_Vertices.push_back(vertices[i + 1]);
		m_Vertices.push_back(vertices[i + 2]);

	}

	// Save width height and depth of the object aligned bounding box.
	m_AABB_width = maxVertex.x - minVertex.x;
	m_AABB_height = maxVertex.y - minVertex.y;
	m_AABB_depth = maxVertex.z - minVertex.z;

	return true;
}

void PBStaticModel::ReleaseModel()
{
	return;
}