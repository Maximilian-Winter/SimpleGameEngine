//World Scene Class 
//Class to manage Scene Data of Objects
//eg: World Matrix, Position
#ifndef _WORLDSCENEDATA_H_
#define _WORLDSCENEDATA_H_



// Includes
#include "MaterialManager.h"
#include "ModelManager.h"
#include "PhysicWorld.h"
#include "FrustumCulling.h"
#include "LightManager.h"


struct SceneObjectData
{
	SceneObjectData()
	{
	}

	SceneObjectData(const SceneObjectData& other)
	{
		*this = other;
	}

	VerticesRenderData ObjectRenderData;
	DirectX::XMFLOAT4X4 WorldMatrix;
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 QuaternionRotation;
	DirectX::XMFLOAT3 Scale;
};

class SceneWorldData
{
private:

public:

	enum SceneObjectType
	{
		StaticObject, AnimatedObject, TerrainObject
	};

	enum SceneLightType
	{
		DirectionalLight, PointLight, SpotLight, CapsuleLight, ImageBasedLight
	};

	enum CollisionBodyType
	{
		CollisionPlane, CollisionSphere, CollisionBox, CollisionCylinder, CollisionCone, CollisionStaticTriangleMesh, CollisionConvexHullMesh, CollisionConvexDecompositionMesh, CollisionMeshFile
	};

	enum CullingVolumeType
	{
		CullPoint, CullCube, CullSphere, CullBox
	};

	struct SceneObject
	{
		SceneObject()
		{
		}

		SceneObject(const SceneObject& other)
		{
			*this = other;
		}

		SceneObjectType ObjectType;
		CullingVolumeType CullingType;
		float AABB_width;
		float AABB_height;
		float AABB_depth;

		std::string Filename;
		std::string ObjectName;
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 QuaternionRotation;
		DirectX::XMFLOAT3 Scale;
		DirectX::XMFLOAT4X4 WorldMatrix;
	};

	SceneWorldData();
	SceneWorldData(const SceneWorldData&);
	~SceneWorldData();

	void Init(ID3D11Device*, MaterialManager*);

	bool AddObjectToScene(ID3D11Device* device, MaterialManager* materialManager, SceneObjectType objectType, CullingVolumeType cullingVolumeType, CollisionBodyType physicBodyType,  std::string modelFilename, std::string& modelName, DirectX::FXMVECTOR  position, DirectX::FXMVECTOR  quaternionRotation, DirectX::FXMVECTOR  scale, float mass);

	//bool AddLight(ID3D11Device* device, MaterialManager* materialManager, SceneObjectType objectType, CullingVolumeType cullingVolumeType, CollisionBodyType physicBodyType, std::string modelFilename, std::string& modelName, DirectX::FXMVECTOR  position, DirectX::FXMVECTOR  quaternionRotation, DirectX::FXMVECTOR  scale, float mass);

	bool LoadSceneFile(ID3D11Device* device, MaterialManager* materialManager, std::string objectDescriptionFilename);

	void RemoveObject(int objectIndex);
	void RemoveObject(std::string name);
	
	std::string GetValidSceneObjectName(std::string name);

	bool ApplyCentralForceToObject(std::string name, DirectX::FXMVECTOR force);

	bool ApplyCentralImpulseToObject(std::string name, DirectX::FXMVECTOR impulse);

	bool ApplyForceToObject(std::string name, DirectX::FXMVECTOR force, DirectX::FXMVECTOR rel_position);

	bool ApplyImpulseToObject(std::string name, DirectX::FXMVECTOR impulse, DirectX::FXMVECTOR rel_position);

	void SetPositionFromObject(std::string name, DirectX::FXMVECTOR position);
	void SetRotationFromObject(std::string name, DirectX::FXMVECTOR rotation);

	void SetPositionFromObject(int objectIndex, DirectX::FXMVECTOR position);
	void SetRotationFromObject(int objectIndex, DirectX::FXMVECTOR rotation);

	int GetNumberOfSceneObjects();
	int GetSubsetCountOfObject(int objectIndex);

	SceneObjectData GetSubsetObjectData(MaterialManager* materialManager, int objectIndex, int subsetIndex);
	DirectX::XMMATRIX GetObjectWorldMatrix(int objectIndex);

	std::vector<PhysicDebugDraw::Lines>& GetDebugLinesToDraw();
	
	void PrepareFrameRenderData(MaterialManager* MaterialManager, float screenDepth, DirectX::FXMVECTOR CameraPos, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);
	void ClearSceneRenderData();

	std::vector<SceneObjectData>& GetDeferredFrameRenderData();
	std::vector<SceneObjectData>& GetForwardFrameRenderData();
	std::vector<SceneObjectData>& GetTransparentFrameRenderData();

	std::vector<DirectionalLightData>& GetFrameDirectionalLightData();
	std::vector<ImageBasedLightData>& GetFrameImageBasedLightData();
	std::vector<PointLightData>& GetFramePointLightData(); 
	std::vector<CapsuleLightData>& GetFrameCapsuleLightData();
	std::vector<SpotLightData>& GetFrameSpotLightData();

	void UpdateAnimations(ID3D11DeviceContext*, float, int);
	void UpdatePhysic(float deltaTime);

	LightManager& GetLightManager();

private:

	void SortTransparentFrameRenderDataByDepthFromCamera(DirectX::FXMVECTOR CameraPos);
	bool TestIfSceneObjectIsInFrustum(SceneObject& sceneObject);

	ModelManager m_ModelManager;
	std::vector<SceneObject> m_SceneObjects;
	
	std::vector<SceneObjectData> m_DeferredFrameRenderData;
	std::vector<SceneObjectData> m_ForwardFrameRenderData;
	std::vector<SceneObjectData> m_TransparentFrameRenderData;

	std::vector<DirectionalLightData> m_FrameDirectionalLightData;
	std::vector<ImageBasedLightData> m_FrameImageBasedLightData;
	std::vector<PointLightData> m_FramePointLightData;
	std::vector<CapsuleLightData> m_FrameCapsuleLightData;
	std::vector<SpotLightData> m_FrameSpotLightData;
								
	LightManager m_LightManager;

	PhysicWorld m_Physic;
	FrustumCulling m_FrustumCulling;
};

#endif
