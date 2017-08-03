#include "SceneWorldData.h"

SceneWorldData::SceneWorldData()
{	
}

SceneWorldData::SceneWorldData(const SceneWorldData& other)
{
}

SceneWorldData::~SceneWorldData()
{

}

void SceneWorldData::Init(ID3D11Device* device, MaterialManager* materialManager)
{
	// Init the model manager
	m_ModelManager.Init(device, materialManager);
}

bool SceneWorldData::AddObjectToScene(ID3D11Device * device, MaterialManager* materialManager, SceneObjectType objectType, CullingVolumeType cullingVolumeType, CollisionBodyType physicBodyType, std::string modelFilename, std::string& objectName, DirectX::FXMVECTOR position, DirectX::FXMVECTOR quaternionRotation, DirectX::FXMVECTOR scale, float mass)
{
	bool result;
	SceneObject TempWorldObject;
	TempWorldObject.ObjectType = objectType;
	TempWorldObject.CullingType = cullingVolumeType;
	float width = 0, height = 0, depth = 0;

	switch (objectType)
	{
		case StaticObject:
		{
			result = m_ModelManager.AddStaticModel(device, materialManager, modelFilename);
			if (!result)
			{
				return false;
			}
			m_ModelManager.GetStaticModel(modelFilename).GetWidthHeightAndDepth(width, height, depth);

			break;
		}

		case AnimatedObject:
		{
			result = m_ModelManager.AddAnimatedModel(device, materialManager, modelFilename);
			if (!result)
			{
				return false;
			}
			m_ModelManager.GetAnimatedModel(modelFilename).GetWidthHeightAndDepth(width, height, depth);
			break;
		}


		case TerrainObject:
		{
			result = m_ModelManager.AddTerrainModel(device, materialManager, modelFilename);
			if (!result)
			{
				return false;
			}
			m_ModelManager.GetTerrainModel(modelFilename).GetWidthHeightAndDepth(width, height, depth);
			break;
		}
	
	}

	// Store axis aligned bounding box depth width and height.
	TempWorldObject.AABB_depth = depth;
	TempWorldObject.AABB_height = height;
	TempWorldObject.AABB_width = width;

	objectName = GetValidSceneObjectName(objectName);
	TempWorldObject.ObjectName = objectName;

	TempWorldObject.Filename = modelFilename;

	XMStoreFloat3(&TempWorldObject.Position, position);
	XMStoreFloat4(&TempWorldObject.QuaternionRotation, quaternionRotation);
	XMStoreFloat3(&TempWorldObject.Scale, scale);

	//Build Transform to "World" Matrix

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX ScaleMatrix;
	ScaleMatrix = DirectX::XMMatrixScaling(DirectX::XMVectorGetX(scale), DirectX::XMVectorGetY(scale), DirectX::XMVectorGetZ(scale));

	DirectX::XMMATRIX RotationMatrix;
	//RotationMatrix =  DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&Rotation));

	RotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternionRotation);

	DirectX::XMMATRIX TranslateMatrix;
	TranslateMatrix = DirectX::XMMatrixTranslation(DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position));

	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, ScaleMatrix);
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, RotationMatrix);
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, TranslateMatrix);

	XMStoreFloat4x4(&TempWorldObject.WorldMatrix, worldMatrix);

	// Handle physics for object
	switch (physicBodyType)
	{
	case CollisionPlane:
	{
		m_Physic.AddPlane(objectName, position, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		break;
	}

	case CollisionSphere:
	{
		m_Physic.AddSphere(objectName, position, max(max(width, depth), height) / 2, mass);
		break;
	}


	case CollisionBox:
	{
		m_Physic.AddBox(objectName, position, width, height, depth, mass);
		break;
	}

	case CollisionCylinder:
	{
		m_Physic.AddCylinder(objectName, position, max(width, depth), height, mass);
		break;
	}

	case CollisionCone:
	{
		m_Physic.AddCone(objectName, position, max(width, depth), height, mass);
		break;
	}

	case CollisionStaticTriangleMesh:
	{
		switch (objectType)
		{
		case StaticObject:
		{
			m_Physic.AddStaticTriangleMesh(objectName, position, m_ModelManager.GetStaticModel(modelFilename).GetIndicies(),m_ModelManager.GetStaticModel(modelFilename).GetVertices());
			break;
		}

		case AnimatedObject:
		{
			
			break;
		}


		case TerrainObject:
		{
			
			break;
		}

		}
		break;
	}

	case CollisionConvexHullMesh:
	{
		switch (objectType)
		{
		case StaticObject:
		{
			m_Physic.AddConvexHullMesh(objectName, position, m_ModelManager.GetStaticModel(modelFilename).GetIndicies(), m_ModelManager.GetStaticModel(modelFilename).GetVertices(), mass);
			break;
		}

		case AnimatedObject:
		{

			break;
		}


		case TerrainObject:
		{

			break;
		}

		}
		break;
	}

	case CollisionConvexDecompositionMesh:
	{
		switch (objectType)
		{
		case StaticObject:
		{
			m_Physic.AddConvexDecompMesh(objectName, position, m_ModelManager.GetStaticModel(modelFilename).GetIndicies(), m_ModelManager.GetStaticModel(modelFilename).GetVertices(), mass);
			break;
		}

		case AnimatedObject:
		{

			break;
		}


		case TerrainObject:
		{

			break;
		}

		}
		break;
	}

	case CollisionMeshFile:
	{
		switch (objectType)
		{
		case StaticObject:
		{
			std::string collisionMeshFile;
			if (m_ModelManager.GetStaticModel(modelFilename).GetCollisionMeshFilename(collisionMeshFile))
			{
				m_Physic.AddCollisionMeshFromFile(objectName, position, collisionMeshFile, mass);
			}
			else
			{
				m_Physic.AddConvexHullMesh(objectName, position, m_ModelManager.GetStaticModel(modelFilename).GetIndicies(), m_ModelManager.GetStaticModel(modelFilename).GetVertices(), mass);
			}
			break;
		}

		case AnimatedObject:
		{

			break;
		}


		case TerrainObject:
		{

			break;
		}

		}
		break;
	}
	
	}

	m_SceneObjects.push_back(TempWorldObject);

	return true;
}

bool SceneWorldData::LoadSceneFile(ID3D11Device * device, MaterialManager * materialManager, std::string objectDescriptionFilename)
{
	bool result;

	ConfigData objectDescFile;
	
	SceneObjectType objectType = StaticObject;
	CullingVolumeType cullingVolumeType = CullPoint;
	CollisionBodyType physicBodyType = CollisionBox;

	std::string objectName = "";
	std::string modelFilename = "";
	DirectX::XMVECTOR position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR quaternionRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR scale = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float mass = 0.0f;

	//Clear the config data object and use it to parse the material file
	objectDescFile.ClearConfigData();
	result = objectDescFile.LoadConfigDataFile(objectDescriptionFilename);
	if (result)
	{
		int numberOfSections = objectDescFile.GetNumberOfRootSections();
		
		if (numberOfSections > 0)
		{
			for (int i = 0; i < numberOfSections; i++)
			{

				std::string objectSectionName;
				std::string objectTypeStr;
				std::string cullingVolumeTypeStr;
				std::string collisionBodyTypeStr;

				std::vector<float> pos, rot, sca;

				objectDescFile.GetSectionName(objectDescFile.GetIndiciePathToRootSecton(i), objectSectionName);

				result = objectDescFile.GetStringValueByName(objectSectionName, "Name", objectName);
				if (!result)
				{
					objectName = "JoeDoe";
				}


				result = objectDescFile.GetStringValueByName(objectSectionName, "ModelFilename", modelFilename);
				if (!result)
				{
					return false;
				}


				result = objectDescFile.GetStringValueByName(objectSectionName, "ObjectType", objectTypeStr);
				if (!result)
				{
					return false;
				}

				if (objectTypeStr == "StaticObject")
				{
					objectType = StaticObject;
				}
				else if (objectTypeStr == "AnimatedObject")
				{
					objectType = AnimatedObject;
				}
				else if (objectTypeStr == "TerrainObject")
				{
					objectType = TerrainObject;
				}
				else
				{
					return false;
				}


				result = objectDescFile.GetStringValueByName(objectSectionName, "CullingVolumeType", cullingVolumeTypeStr);
				if (!result)
				{
					return false;
				}

				if (cullingVolumeTypeStr == "CullPoint")
				{
					cullingVolumeType = CullPoint;
				}
				else if (cullingVolumeTypeStr == "CullCube")
				{
					cullingVolumeType = CullCube;
				}
				else if (cullingVolumeTypeStr == "CullSphere")
				{
					cullingVolumeType = CullSphere;
				}
				else if (cullingVolumeTypeStr == "CullBox")
				{
					cullingVolumeType = CullBox;
				}
				else
				{
					return false;
				}


				result = objectDescFile.GetStringValueByName(objectSectionName, "CollisionBodyType", collisionBodyTypeStr);
				if (!result)
				{
					return false;
				}

				if (collisionBodyTypeStr == "CollisionPlane")
				{
					physicBodyType = CollisionPlane;
				}
				else if (collisionBodyTypeStr == "CollisionSphere")
				{
					physicBodyType = CollisionSphere;
				}
				else if (collisionBodyTypeStr == "CollisionBox")
				{
					physicBodyType = CollisionBox;
				}
				else if (collisionBodyTypeStr == "CollisionCylinder")
				{
					physicBodyType = CollisionCylinder;
				}
				else if (collisionBodyTypeStr == "CollisionCone")
				{
					physicBodyType = CollisionCone;
				}
				else if (collisionBodyTypeStr == "CollisionStaticTriangleMesh")
				{
					physicBodyType = CollisionStaticTriangleMesh;
				}
				else if (collisionBodyTypeStr == "CollisionConvexHullMesh")
				{
					physicBodyType = CollisionConvexHullMesh;
				}
				else if (collisionBodyTypeStr == "CollisionConvexDecompositionMesh")
				{
					physicBodyType = CollisionConvexDecompositionMesh;
				}
				else if (collisionBodyTypeStr == "CollisionMeshFile")
				{
					physicBodyType = CollisionMeshFile;
				}
				else
				{
					return false;
				}

				result = objectDescFile.GetFloatVectorByName(objectSectionName, "Position", pos);
				if (!result)
				{
					position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				}

				if (pos.size() == 3)
				{
					position = DirectX::XMVectorSet(pos[0], pos[1], pos[2], 0.0f);
				}
				else
				{
					position = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				}

				result = objectDescFile.GetFloatVectorByName(objectSectionName, "Rotation", rot);
				if (!result)
				{
					quaternionRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				}

				if (rot.size() == 4)
				{
					quaternionRotation = DirectX::XMVectorSet(rot[0], rot[1], rot[2], rot[3]);
				}
				else
				{
					quaternionRotation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
				}

				result = objectDescFile.GetFloatVectorByName(objectSectionName, "Scale", sca);
				if (!result)
				{
					scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
				}

				if (sca.size() == 3)
				{
					scale = DirectX::XMVectorSet(sca[0], sca[1], sca[2], 0.0f);
				}
				else
				{
					scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
				}

				result = objectDescFile.GetFloatValueByName(objectSectionName, "Mass", mass);
				if (!result)
				{
					mass = 0.0f;
				}

				result = AddObjectToScene(device, materialManager, objectType, cullingVolumeType, physicBodyType, modelFilename, objectName, position, quaternionRotation, scale, mass);
				if (!result)
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void SceneWorldData::RemoveObject(int objectIndex)
{
	m_Physic.RemoveRigidBody(m_SceneObjects[objectIndex].ObjectName);
	m_SceneObjects.erase(m_SceneObjects.begin() + objectIndex);
}

void SceneWorldData::RemoveObject(std::string name)
{
	for (auto it = m_SceneObjects.begin(); it < m_SceneObjects.end(); it++)
	{
		if (it->ObjectName == name)
		{
			m_Physic.RemoveRigidBody(it->ObjectName);
			m_SceneObjects.erase(it);
		}
	}

}

std::string SceneWorldData::GetValidSceneObjectName(std::string name)
{
	auto it = std::find_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj) {return obj.ObjectName == name; });

	if (it != m_SceneObjects.end())
	{
		int nameCount = std::count_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj)
		{
			if (obj.ObjectName.find(name + "_D") != std::string::npos)
			{
				return true;
			}
			
			if (obj.ObjectName.find(name + "_D") == std::string::npos)
			{
				return false;
			}
		});

		name = (name + "_D") + std::to_string(nameCount + 1);
	}

	return name;
}

bool SceneWorldData::ApplyCentralForceToObject(std::string name, DirectX::FXMVECTOR force)
{
	auto it = std::find_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj) {return obj.ObjectName == name; });

	if (it != m_SceneObjects.end())
	{
		m_Physic.GetRigidBodys().GetRigidBody(name)->activate(true);
		m_Physic.GetRigidBodys().GetRigidBody(name)->applyCentralForce(btVector3(DirectX::XMVectorGetX(force), DirectX::XMVectorGetY(force), DirectX::XMVectorGetZ(force)));
	}

	return true;
}

bool SceneWorldData::ApplyCentralImpulseToObject(std::string name, DirectX::FXMVECTOR impulse)
{
	auto it = std::find_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj) {return obj.ObjectName == name; });

	if (it != m_SceneObjects.end())
	{
		m_Physic.GetRigidBodys().GetRigidBody(name)->activate(true);
		m_Physic.GetRigidBodys().GetRigidBody(name)->applyCentralImpulse(btVector3(DirectX::XMVectorGetX(impulse), DirectX::XMVectorGetY(impulse), DirectX::XMVectorGetZ(impulse)));
	}

	return true;
}

bool SceneWorldData::ApplyForceToObject(std::string name, DirectX::FXMVECTOR force, DirectX::FXMVECTOR rel_position)
{
	auto it = std::find_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj) {return obj.ObjectName == name; });

	if (it != m_SceneObjects.end())
	{
		m_Physic.GetRigidBodys().GetRigidBody(name)->activate(true);
		m_Physic.GetRigidBodys().GetRigidBody(name)->applyForce(btVector3(DirectX::XMVectorGetX(force), DirectX::XMVectorGetY(force), DirectX::XMVectorGetZ(force)), btVector3(DirectX::XMVectorGetX(rel_position), DirectX::XMVectorGetY(rel_position), DirectX::XMVectorGetZ(rel_position)));
	}

	return true;
}

bool SceneWorldData::ApplyImpulseToObject(std::string name, DirectX::FXMVECTOR impulse, DirectX::FXMVECTOR rel_position)
{
	auto it = std::find_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj) {return obj.ObjectName == name; });

	if (it != m_SceneObjects.end())
	{
		m_Physic.GetRigidBodys().GetRigidBody(name)->activate(true);
		m_Physic.GetRigidBodys().GetRigidBody(name)->applyImpulse(btVector3(DirectX::XMVectorGetX(impulse), DirectX::XMVectorGetY(impulse), DirectX::XMVectorGetZ(impulse)), btVector3(DirectX::XMVectorGetX(rel_position), DirectX::XMVectorGetY(rel_position), DirectX::XMVectorGetZ(rel_position)));
	}

	return true;
}

void SceneWorldData::SetPositionFromObject(std::string name, DirectX::FXMVECTOR position)
{
	auto it = std::find_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj) {return obj.ObjectName == name; });

	if (it != m_SceneObjects.end())
	{
		XMStoreFloat3(&it->Position, position);

		DirectX::XMMATRIX TranslateMatrix;
		TranslateMatrix = DirectX::XMMatrixTranslation(it->Position.x, it->Position.y, it->Position.z);

		DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&it->QuaternionRotation));

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity() * RotationMatrix * TranslateMatrix;

		XMStoreFloat4x4(&it->WorldMatrix, worldMatrix);
	}
}

void SceneWorldData::SetRotationFromObject(std::string name, DirectX::FXMVECTOR rotation)
{
	auto it = std::find_if(m_SceneObjects.begin(), m_SceneObjects.end(), [&name](const SceneObject& obj) {return obj.ObjectName == name; });

	if (it != m_SceneObjects.end())
	{
		XMStoreFloat4(&it->QuaternionRotation, rotation);

		DirectX::XMMATRIX TranslateMatrix;
		TranslateMatrix = DirectX::XMMatrixTranslation(it->Position.x, it->Position.y, it->Position.z);

		DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity() * RotationMatrix * TranslateMatrix;

		XMStoreFloat4x4(&it->WorldMatrix, worldMatrix);
	}
}

void SceneWorldData::SetPositionFromObject(int objectIndex, DirectX::FXMVECTOR position)
{
	XMStoreFloat3(&m_SceneObjects[objectIndex].Position, position);

	DirectX::XMMATRIX TranslateMatrix;
	TranslateMatrix = DirectX::XMMatrixTranslation(m_SceneObjects[objectIndex].Position.x, m_SceneObjects[objectIndex].Position.y, m_SceneObjects[objectIndex].Position.z);

	DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&m_SceneObjects[objectIndex].QuaternionRotation));

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity() * RotationMatrix * TranslateMatrix;

	XMStoreFloat4x4(&m_SceneObjects[objectIndex].WorldMatrix, worldMatrix);
}

void SceneWorldData::SetRotationFromObject(int objectIndex, DirectX::FXMVECTOR rotation)
{
	XMStoreFloat4(&m_SceneObjects[objectIndex].QuaternionRotation, rotation);

	DirectX::XMMATRIX TranslateMatrix;
	TranslateMatrix = DirectX::XMMatrixTranslation(m_SceneObjects[objectIndex].Position.x, m_SceneObjects[objectIndex].Position.y, m_SceneObjects[objectIndex].Position.z);

	DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity() * RotationMatrix * TranslateMatrix;

	XMStoreFloat4x4(&m_SceneObjects[objectIndex].WorldMatrix, worldMatrix);
}

int SceneWorldData::GetNumberOfSceneObjects()
{
	return m_SceneObjects.size();
}

int SceneWorldData::GetSubsetCountOfObject(int subsetIndex)
{
	SceneObjectType objectType = m_SceneObjects[subsetIndex].ObjectType;

	switch (objectType)
	{
	case StaticObject:
	{
		return m_ModelManager.GetStaticModel(m_SceneObjects[subsetIndex].Filename).GetSubsetCount();
	}

	case AnimatedObject:
	{
		return m_ModelManager.GetAnimatedModel(m_SceneObjects[subsetIndex].Filename).GetSubsetCount();
	}


	case TerrainObject:
	{
		return m_ModelManager.GetTerrainModel(m_SceneObjects[subsetIndex].Filename).GetSubsetCount();
	}

	default:
	{
		return 0;
	}
	}
}

SceneObjectData SceneWorldData::GetSubsetObjectData(MaterialManager* materialManager, int objectIndex, int subsetIndex)
{
	SceneObjectData TempObjectData;

	SceneObjectType& objectType = m_SceneObjects[objectIndex].ObjectType;

	switch (objectType)
	{
	case StaticObject:
	{
		TempObjectData.ObjectRenderData = m_ModelManager.GetStaticModel(m_SceneObjects[objectIndex].Filename).GetSubsetRenderData(materialManager, subsetIndex);
		break;
	}

	case AnimatedObject:
	{
		TempObjectData.ObjectRenderData = m_ModelManager.GetAnimatedModel(m_SceneObjects[objectIndex].Filename).GetSubsetRenderData(materialManager, subsetIndex);
		break;
	}


	case TerrainObject:
	{
		TempObjectData.ObjectRenderData = m_ModelManager.GetTerrainModel(m_SceneObjects[objectIndex].Filename).GetSubsetRenderData(materialManager, subsetIndex);
		break;
	}

	}	
	TempObjectData.WorldMatrix = m_SceneObjects[objectIndex].WorldMatrix;
	TempObjectData.Position = m_SceneObjects[objectIndex].Position;
	TempObjectData.QuaternionRotation = m_SceneObjects[objectIndex].QuaternionRotation;
	TempObjectData.Scale = m_SceneObjects[objectIndex].Scale;
	return TempObjectData;
}

DirectX::XMMATRIX SceneWorldData::GetObjectWorldMatrix(int objectIndex)
{
	return XMLoadFloat4x4(&m_SceneObjects[objectIndex].WorldMatrix);
}

std::vector<PhysicDebugDraw::Lines>& SceneWorldData::GetDebugLinesToDraw()
{
	return m_Physic.GetDebugLinesToDraw();
}

void SceneWorldData::PrepareFrameRenderData(MaterialManager* MaterialManager, float screenDepth, DirectX::FXMVECTOR CameraPos, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	m_FrustumCulling.ConstructFrustum(screenDepth, viewMatrix, projectionMatrix);

	// Add the scene objects to their corresponding frame buffer.
	for (int objectIndex = 0; objectIndex < m_SceneObjects.size(); objectIndex++)
	{
		if (TestIfSceneObjectIsInFrustum(m_SceneObjects[objectIndex]))
		{
			for (int subIndex = 0; subIndex < GetSubsetCountOfObject(objectIndex); subIndex++)
			{
				SceneObjectData ObjectData = GetSubsetObjectData(MaterialManager, objectIndex, subIndex);

				if (ObjectData.ObjectRenderData.Material.RenderForward == false && ObjectData.ObjectRenderData.Material.IsTransparent == false)
				{
					m_DeferredFrameRenderData.push_back(ObjectData);
				}

				if (ObjectData.ObjectRenderData.Material.RenderForward == true && ObjectData.ObjectRenderData.Material.IsTransparent == false)
				{
					m_ForwardFrameRenderData.push_back(ObjectData);
				}

				if (ObjectData.ObjectRenderData.Material.IsTransparent == true)
				{
					m_TransparentFrameRenderData.push_back(ObjectData);
				}
			}
		}
	}

	// Add the scene lights to their corresponding frame buffer.

	for (int lightIndex = 0; lightIndex < m_LightManager.GetNumberOfDirectionalLights(); lightIndex++)
	{
		m_FrameDirectionalLightData.push_back(m_LightManager.GetDirectionalLightData(lightIndex));
	}

	for (int lightIndex = 0; lightIndex < m_LightManager.GetNumberOfPointLights(); lightIndex++)
	{
		m_FramePointLightData.push_back(m_LightManager.GetPointLightData(lightIndex));
	}

	for (int lightIndex = 0; lightIndex < m_LightManager.GetNumberOfSpotLights(); lightIndex++)
	{
		m_FrameSpotLightData.push_back(m_LightManager.GetSpotLightData(lightIndex));
	}

	for (int lightIndex = 0; lightIndex < m_LightManager.GetNumberOfCapsuleLights(); lightIndex++)
	{
		m_FrameCapsuleLightData.push_back(m_LightManager.GetCapsuleLightData(lightIndex));
	}

	for (int lightIndex = 0; lightIndex < m_LightManager.GetNumberOfImageBasedLights(); lightIndex++)
	{
		m_FrameImageBasedLightData.push_back(m_LightManager.GetIBLData(lightIndex));
	}

	SortTransparentFrameRenderDataByDepthFromCamera(CameraPos);
}

void SceneWorldData::SortTransparentFrameRenderDataByDepthFromCamera(DirectX::FXMVECTOR CameraPos)
{
	struct DepthData
	{
		float Depth;
		int ObjectIndex;

		DepthData(float k, const int& s) : Depth(k), ObjectIndex(s) {}
	};

	struct greater_than_key
	{
		inline bool operator() (const DepthData& depthData1, const DepthData& depthData2)
		{
			return (depthData1.Depth > depthData2.Depth);
		}
	};

	std::vector<DepthData> DepthSortVector;

	for (int ObjIndex = 0; ObjIndex < m_TransparentFrameRenderData.size(); ObjIndex++)
	{

		float distX = DirectX::XMVectorGetX(XMLoadFloat3(&m_TransparentFrameRenderData[ObjIndex].Position)) - DirectX::XMVectorGetX(CameraPos);
		float distY = DirectX::XMVectorGetY(XMLoadFloat3(&m_TransparentFrameRenderData[ObjIndex].Position)) - DirectX::XMVectorGetY(CameraPos);
		float distZ = DirectX::XMVectorGetZ(XMLoadFloat3(&m_TransparentFrameRenderData[ObjIndex].Position)) - DirectX::XMVectorGetZ(CameraPos);

		float Dist = distX*distX + distY*distY + distZ*distZ;

		DepthSortVector.push_back(DepthData(Dist, ObjIndex));
	}

	std::sort(DepthSortVector.begin(), DepthSortVector.end(), greater_than_key());
	
	std::vector<SceneObjectData> TempRenderDataSortVector;

	for (int ObjIndex = 0; ObjIndex < m_TransparentFrameRenderData.size(); ObjIndex++)
	{
		TempRenderDataSortVector.push_back(m_TransparentFrameRenderData[DepthSortVector[ObjIndex].ObjectIndex]);
	}

	m_TransparentFrameRenderData = TempRenderDataSortVector;

	return;
}

bool SceneWorldData::TestIfSceneObjectIsInFrustum(SceneObject & sceneObject)
{
	float width = sceneObject.AABB_width;
	float height = sceneObject.AABB_height;
	float depth = sceneObject.AABB_depth;

	switch (sceneObject.CullingType)
	{
	case CullPoint:
	{
		return m_FrustumCulling.CheckPoint(FrustumCulling::Point(sceneObject.Position.x, sceneObject.Position.y, sceneObject.Position.z));
		break;
	}

	case CullCube:
	{
		return m_FrustumCulling.CheckCube(FrustumCulling::Cube(sceneObject.Position.x, sceneObject.Position.y, sceneObject.Position.z, max(max((width / 2), (depth / 2)), (height / 2))));
		break;
	}

	case CullSphere:
	{
		return m_FrustumCulling.CheckSphere(FrustumCulling::Sphere(sceneObject.Position.x, sceneObject.Position.y, sceneObject.Position.z, max(max((width / 2), (depth / 2)), (height / 2))));
		break;
	}

	case CullBox:
	{
		return m_FrustumCulling.CheckRectangle(FrustumCulling::Rectangle(sceneObject.Position.x, sceneObject.Position.y, sceneObject.Position.z, (width / 2), (height / 2), (depth / 2)));
		break;
	}

	default:
		break;
	}
	return false;
}

void SceneWorldData::ClearSceneRenderData()
{
	// Clear all frame buffers.
	m_DeferredFrameRenderData.clear();
	m_ForwardFrameRenderData.clear();
	m_TransparentFrameRenderData.clear();

	m_FrameDirectionalLightData.clear();
	m_FrameImageBasedLightData.clear();
	m_FramePointLightData.clear();
	m_FrameCapsuleLightData.clear();
	m_FrameSpotLightData.clear();

}

std::vector<SceneObjectData>& SceneWorldData::GetDeferredFrameRenderData()
{
	return m_DeferredFrameRenderData;
}

std::vector<SceneObjectData>& SceneWorldData::GetForwardFrameRenderData()
{
	return m_ForwardFrameRenderData;
}

std::vector<SceneObjectData>& SceneWorldData::GetTransparentFrameRenderData()
{
	return m_TransparentFrameRenderData;
}

std::vector<DirectionalLightData>& SceneWorldData::GetFrameDirectionalLightData()
{
	return m_FrameDirectionalLightData;
}

std::vector<ImageBasedLightData>& SceneWorldData::GetFrameImageBasedLightData()
{
	return m_FrameImageBasedLightData;
}

std::vector<PointLightData>& SceneWorldData::GetFramePointLightData()
{
	return m_FramePointLightData;
}

std::vector<CapsuleLightData>& SceneWorldData::GetFrameCapsuleLightData()
{
	return m_FrameCapsuleLightData;
}

std::vector<SpotLightData>& SceneWorldData::GetFrameSpotLightData()
{
	return m_FrameSpotLightData;
}

void SceneWorldData::UpdatePhysic(float deltaTime)
{
	// Get rigid body from the physic world.
	RigidBodys bodys = m_Physic.GetRigidBodys();

	// Update the bullet physic engine.
	m_Physic.UpdateWorld(deltaTime);

	// Update scene objects according to physics
	for (auto it = m_SceneObjects.begin(); it < m_SceneObjects.end(); it++)
	{
		DirectX::XMStoreFloat3(&it->Position, bodys.GetRigidBodyPosition(it->ObjectName));
		DirectX::XMStoreFloat4(&it->QuaternionRotation, bodys.GetRigidBodyRotation(it->ObjectName));
		DirectX::XMStoreFloat4x4(&it->WorldMatrix, bodys.GetRigidBodyTransformMatrix(it->ObjectName));
	}
}

LightManager& SceneWorldData::GetLightManager()
{
	return m_LightManager;
}

void SceneWorldData::UpdateAnimations(ID3D11DeviceContext* deviceContext, float deltaTime, int animSet)
{
	for (int i = 0; i < m_SceneObjects.size(); i++)
	{
		SceneObjectType objectType = m_SceneObjects[i].ObjectType;

		switch (objectType)
		{
		case AnimatedObject:
		{
			m_ModelManager.GetAnimatedModel(m_SceneObjects[i].ObjectName).UpdateAnimation(deviceContext, deltaTime, animSet);
			break;
		}
		}
	
	}
}
