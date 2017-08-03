#ifndef _RIGIDBODYS_H_
#define _RIGIDBODYS_H_

#include <DirectXMath.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <algorithm>

struct RigidBodyData
{
	RigidBodyData();

	RigidBodyData(const RigidBodyData& other);

	RigidBodyData(std::string Name, btRigidBody* Body);

	~RigidBodyData();

	DirectX::XMMATRIX GetTransformMatrix();

	DirectX::XMVECTOR GetPosition();

	DirectX::XMVECTOR GetRotation();

	std::string name;
	btRigidBody* rigidBody;
};

struct RigidBodys
{
	DirectX::XMMATRIX GetRigidBodyTransformMatrix(std::string name);

	DirectX::XMVECTOR GetRigidBodyPosition(std::string name);

	DirectX::XMVECTOR GetRigidBodyRotation(std::string name);

	btRigidBody* GetRigidBody(std::string name);

	std::vector<RigidBodyData> rigidBodys;
};

#endif