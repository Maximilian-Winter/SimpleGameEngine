// Animated Model
// Manage animated model data e.g. loading, calculate animations.

#ifndef _ANIMATEDMODEL_H_
#define _ANIMATEDMODEL_H_


// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <istream>
#include <vector>
#include <sstream>
#include "PBStaticModel.h"


class AnimatedModel
{

private:

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex()
		{
			pos.x = 0;
			pos.y = 0;
			pos.z = 0;

			texCoord.x = 0;
			texCoord.y = 0;

			normal.x = 0;
			normal.y = 0;
			normal.z = 0;

		}

		Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x,y,z), texCoord(u, v), normal(nx, ny, nz){}

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 normal;

		///////////////**************new**************////////////////////
		// Will not be sent to shader
		int StartWeight;
		int WeightCount;
		///////////////**************new**************////////////////////

	};

	struct Joint
	{
		Joint()
		{
			name = "";
			parentID = 0;

			pos.x = 0;
			pos.y = 0;
			pos.z = 0;

			orientation.x = 0;
			orientation.y = 0;
			orientation.z = 0;
			orientation.w = 0;
		}

		std::string name;
		int parentID;

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 orientation;
	};

	///////////////**************new**************////////////////////
	struct BoundingBox
	{
		BoundingBox()
		{
			min.x = 0;
			min.y = 0;
			min.z = 0;

			max.x = 0;
			max.y = 0;
			max.z = 0;
		}

		DirectX::XMFLOAT3 min;
		DirectX::XMFLOAT3 max;
	};

	struct FrameData
	{
		FrameData()
		{
			frameID = 0;
			frameData.clear();
		}

		int frameID;
		std::vector<float> frameData;
	};
	struct AnimJointInfo
	{
		AnimJointInfo()
		{
			name = "";
			parentID = 0;

			flags = 0;
			startIndex = 0;
		}
		std::string name;
		int parentID;

		int flags;
		int startIndex;
	};

	struct ModelAnimation
	{
		ModelAnimation()
		{
			numFrames = 0;
			numJoints = 0;
			frameRate = 0;
			numAnimatedComponents = 0;

			frameTime = 0;
			totalAnimTime = 0;
			currAnimTime = 0;

		}
		int numFrames;
		int numJoints;
		int frameRate;
		int numAnimatedComponents;

		float frameTime;
		float totalAnimTime;
		float currAnimTime;

		std::vector<AnimJointInfo> jointInfo;
		std::vector<BoundingBox> frameBounds;
		std::vector<Joint>	baseFrameJoints;
		std::vector<FrameData>	frameData;
		std::vector<std::vector<Joint>> frameSkeleton;
	};
	///////////////**************new**************////////////////////

	struct Weight
	{
		Weight()
		{
			jointID = 0;
			bias = 0;

			pos.x = 0;
			pos.y = 0;
			pos.z = 0;

			normal.x = 0;
			normal.y = 0;
			normal.z = 0;

		}
		int jointID;
		float bias;
		DirectX::XMFLOAT3 pos;
		///////////////**************new**************////////////////////
		DirectX::XMFLOAT3 normal;
		///////////////**************new**************////////////////////
	};

	struct ModelSubset
	{
		ModelSubset()
		{
			texArrayIndex = 0;
			numTriangles = 0;

			vertices.clear();
			jointSpaceNormals.clear();
			indices.clear();
			weights.clear();

			positions.clear();

			vertBuff = 0;
			indexBuff = 0;
		}

		int texArrayIndex;
		int numTriangles;

		std::vector<Vertex> vertices;
		std::vector<DirectX::XMFLOAT3> jointSpaceNormals;
		std::vector<DWORD> indices;
		std::vector<Weight> weights;

		std::vector<DirectX::XMFLOAT3> positions;

		ID3D11Buffer* vertBuff; 
		ID3D11Buffer* indexBuff;
	};

	struct Model3D
	{
		Model3D()
		{
			numSubsets = 0;
			numJoints = 0;

			joints.clear();
			subsets.clear();
			animations.clear();

		}
		int numSubsets;
		int numJoints;

		std::vector<Joint> joints;
		std::vector<ModelSubset> subsets;

		///////////////**************new**************////////////////////
		std::vector<ModelAnimation> animations;
		///////////////**************new**************////////////////////
	};

public:

	AnimatedModel();
	AnimatedModel(const AnimatedModel&);
	~AnimatedModel();

	bool Initialize(ID3D11Device*, std::string, MaterialManager*);
	bool LoadAnimation(std::string);
	void Shutdown();

	void UpdateAnimation(ID3D11DeviceContext*, float deltaTime, int animation);

	int GetSubsetCount();
	
	void GetWidthHeightAndDepth(float& width, float& height, float& depth);

	VerticesRenderData GetSubsetRenderData(MaterialManager*, int Subset);

private:

	void RenderBuffers(ID3D11DeviceContext*, int Subset);

	float m_AABB_width;
	float m_AABB_height;
	float m_AABB_depth;

	bool LoadModel(ID3D11Device*, std::string);
	void ReleaseModel();

	Model3D MD5Model;
};

#endif