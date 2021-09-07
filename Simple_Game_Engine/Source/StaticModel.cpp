#include "StaticModel.h"

#include "DataFileContainer.h"
#include "Logger.h"
#include "MaterialManager.h"

StaticModel::StaticModel()
{
}

StaticModel::StaticModel(ID3D11Device* device, SceneGraph& sceneGraph, std::string staticModelFilename): m_AABBWidth(0), m_AABBHeight(0), m_AABBDepth(0)
{
	LoadModel(device, sceneGraph, staticModelFilename);
}

StaticModel::StaticModel(const StaticModel&)
{
}

StaticModel::~StaticModel()
{
}

bool StaticModel::LoadModel(ID3D11Device* device, SceneGraph& sceneGraph, std::string staticModelFilename)
{
	DataFileContainer MeshInputFile;
	DataFileContainer MaterialInputFile;
	MaterialInputFile.ClearDataContainer();
	MeshInputFile.ClearDataContainer();
	bool result = MeshInputFile.LoadDataContainerBinaryFile(staticModelFilename);
	if (!result)
	{
		Logger::GetInstance()->Log("ModelLoadingError: ", "MeshFileNotFound!");
		return false;
	}

	std::string materialLibraryFilename;
	result = MeshInputFile.GetStringValueByName("Model_Information", "MaterialLibrary", materialLibraryFilename);
	if (!result)
	{
		Logger::GetInstance()->Log("ModelLoadingError", "MaterialLibraryNotFound");
		return false;
	}


	result = MaterialManager::GetInstance()->AddMaterialFile(device, materialLibraryFilename);
	if (!result)
	{
		Logger::GetInstance()->Log("ModelLoadFailed", "MaterialLibraryNotFound");
		return false;
	}

	std::map<int, SceneGraphNode*> nodeIDToSceneGraphNode;
	for (int i = 0; i < MeshInputFile.GetNumberOfRootSections(); i++)
	{
		std::string sectionName;
		std::vector<int> sectionIndex;
		sectionIndex.push_back(i);
		if (MeshInputFile.GetSectionName(sectionIndex, sectionName)&& sectionName != "Model_Information")
		{
			int numberOfMeshes = 0;
			result = MeshInputFile.GetIntValueByName(sectionName, "Number_of_Meshes", numberOfMeshes);
			if (!result)
			{
				Logger::GetInstance()->Log("ModelLoadingError", "AttributeNumberOfMeshesNotFound");
				return false;
			}

			int nodeId = 0;
			result = MeshInputFile.GetIntValueByName(sectionName, "Node_ID", nodeId);
			if (!result)
			{
				Logger::GetInstance()->Log("ModelLoadingError", "AttributeNodeIDNotFound");
				return false;
			}

			int parentNodeId = 0;
			result = MeshInputFile.GetIntValueByName(sectionName, "Parent_Node_ID", parentNodeId);
			if (!result)
			{
				Logger::GetInstance()->Log("ModelLoadingError", "AttributeParentNodeIDNotFound");
				return false;
			}

			std::string nodeName = "";
			result = MeshInputFile.GetStringValueByName(sectionName, "Node_Name", nodeName);
			if (!result)
			{
				Logger::GetInstance()->Log("ModelLoadingError", "AttributeNodeNameNotFound");
				return false;
			}

			std::vector<float> nodePos;
			result = MeshInputFile.GetFloatVectorByName(sectionName, "Node_Position", nodePos);
			if (!result)
			{
				Logger::GetInstance()->Log("ModelLoadingError", "AttributeNode_PositionNotFound");
				return false;
			}

			std::vector<float> nodeRot;
			result = MeshInputFile.GetFloatVectorByName(sectionName, "Node_Rotation", nodeRot);
			if (!result)
			{
				Logger::GetInstance()->Log("ModelLoadingError", "AttributeNode_RotationNotFound");
				return false;
			}

			std::vector<float> nodeScale;
			result = MeshInputFile.GetFloatVectorByName(sectionName, "Node_Scale", nodeScale);
			if (!result)
			{
				Logger::GetInstance()->Log("ModelLoadingError", "AttributeNode_ScaleNotFound");
				return false;
			}

			DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(nodePos[0], nodePos[1], nodePos[2]);
			DirectX::XMFLOAT4 rotation = DirectX::XMFLOAT4(nodeRot[0], nodeRot[1], nodeRot[2], nodeRot[3]);
			DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(nodeScale[0], nodeScale[1], nodeScale[2]);
			if (numberOfMeshes > 0)
			{
				StaticGeometryNode* node;
				if(parentNodeId >= 0)
				{
					node = new StaticGeometryNode(parentNodeId, nodeId, Transform(nodeIDToSceneGraphNode[parentNodeId]->GetNodeTransform(), DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat4(&rotation), DirectX::XMLoadFloat3(&scale)), nodeName);
				}
				else
				{
					node = new StaticGeometryNode(parentNodeId, nodeId, Transform(nullptr, DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat4(&rotation), DirectX::XMLoadFloat3(&scale)), nodeName);
				}
				
				nodeIDToSceneGraphNode[node->GetNodeID()] = node;

				for (int t = 0; t < numberOfMeshes; t++)
				{
					std::vector<float> vertices;
					std::vector<StaticVertex> vertexData;
					result = MeshInputFile.GetFloatVectorByName(sectionName + ".Mesh_" + std::to_string(t), "Vertices", vertices);
					if (!result)
					{
						Logger::GetInstance()->Log("ModelLoadingError", "AttributeVerticesNotFound");
						return false;
					}


					for (int i = 0; i < vertices.size(); i += 8)
					{
						vertexData.push_back(StaticVertex(vertices[i], vertices[i + 1], vertices[i + 2], vertices[i + 3], vertices[i + 4], vertices[i + 5], vertices[i + 6], vertices[i + 7]));
					}

					std::vector<int> indicies;
					result = MeshInputFile.GetIntVectorByName(sectionName + +".Mesh_" + std::to_string(t), "Indicies", indicies);
					if (!result)
					{
						Logger::GetInstance()->Log("ModelLoadingError", "AttributeIndiciesNotFound");
						return false;
					}

					std::vector<Subset> subsets;
					int numberOfSubsets = 0;

					result = MeshInputFile.GetIntValueByName(sectionName + ".Mesh_" + std::to_string(t), "Subset_Count", numberOfSubsets);
					if (!result)
					{
						Logger::GetInstance()->Log("ModelLoadingError", "AttributeSubsetCountNotFound");
						return false;
					}

					for (int g = 0; g < numberOfSubsets; g++)
					{
						std::string meshSubsetSection = ".Mesh_" + std::to_string(t) + "_Subset_" + std::to_string(g);
						std::string subsetMaterialName = "";
						int drawAmount = 0;
						int subsetStart = 0;

						result = MeshInputFile.GetStringValueByName(sectionName + meshSubsetSection, "Subset_Material_Name", subsetMaterialName);
						if (!result)
						{
							Logger::GetInstance()->Log("ModelLoadingError", "NoSubset_Material_NameFound");
							return false;
						}

						result = MeshInputFile.GetIntValueByName(sectionName + meshSubsetSection, "Subset_Draw_Amount", drawAmount);
						if (!result)
						{
							Logger::GetInstance()->Log("ModelLoadingError", "NoSubset_Draw_AmountFound");
							return false;
						}

						result = MeshInputFile.GetIntValueByName(sectionName + meshSubsetSection, "Subset_Start", subsetStart);
						if (!result)
						{
							Logger::GetInstance()->Log("ModelLoadingError", "NoSubset_StartFound");
							return false;
						}
						subsets.push_back(Subset(subsetMaterialName, subsetStart, drawAmount));
					}

					std::vector<StaticVertex>m_MeshVertices;

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

						m_MeshVertices.push_back(StaticVertex(vertices[i], vertices[i + 1], vertices[i + 2], vertices[i + 3], vertices[i + 4], vertices[i + 5], vertices[i + 6], vertices[i + 7]));
					}

					// Save width height and depth of the object aligned bounding box.
					float AABB_width = maxVertex.x - minVertex.x;
					float AABB_height = maxVertex.y - minVertex.y;
					float AABB_depth = maxVertex.z - minVertex.z;


					node->AddStaticMeshToNode(device, m_MeshVertices ,indicies, subsets, AABB_width, AABB_height, AABB_depth);

				}
			}
			else
			{
				EmptyNode* node;
				if (parentNodeId >= 0)
				{
					node = new EmptyNode(parentNodeId, nodeId, Transform(nodeIDToSceneGraphNode[parentNodeId]->GetNodeTransform(), DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat4(&rotation), DirectX::XMLoadFloat3(&scale)), nodeName);
				}
				else
				{
					node = new EmptyNode(parentNodeId, nodeId, Transform(nullptr, DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat4(&rotation), DirectX::XMLoadFloat3(&scale)), nodeName);
				}
				nodeIDToSceneGraphNode[node->GetNodeID()] = node;
			}
		}

	}
	int rootNodeID;
	for (auto node_id_to_scene_graph_nodeFirst : nodeIDToSceneGraphNode)
	{
		int nodeId = node_id_to_scene_graph_nodeFirst.second->GetNodeID();
		if (node_id_to_scene_graph_nodeFirst.second->GetParentNodeID() == -1)
		{
			m_ModelRootSceneGraphNode = node_id_to_scene_graph_nodeFirst.second;
		}

		for (auto node_id_to_scene_graph_nodeSecond : nodeIDToSceneGraphNode)
		{
			if (nodeId == node_id_to_scene_graph_nodeSecond.second->GetParentNodeID())
			{
				node_id_to_scene_graph_nodeFirst.second->AddChild(node_id_to_scene_graph_nodeSecond.second);
			}
		}
	}

	if(!(m_ModelRootSceneGraphNode->GetNodeID() >= 0))
	{
		m_ModelRootSceneGraphNode = nodeIDToSceneGraphNode[0];
	}

	sceneGraph.AddChild(m_ModelRootSceneGraphNode);

	return true;
}
