#include "RigidBodys.h"

DirectX::XMMATRIX RigidBodys::GetRigidBodyTransformMatrix(std::string name)
{
	auto it = std::find_if(rigidBodys.begin(), rigidBodys.end(), [&name](const RigidBodyData& obj) {return obj.name == name; });

	if (it != rigidBodys.end())
	{
		return it->GetTransformMatrix();
	}

	return DirectX::XMMatrixIdentity();
}

DirectX::XMVECTOR RigidBodys::GetRigidBodyPosition(std::string name)
{
	auto it = std::find_if(rigidBodys.begin(), rigidBodys.end(), [&name](const RigidBodyData& obj) {return obj.name == name; });

	if (it != rigidBodys.end())
	{
		return it->GetPosition();
	}

	return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

DirectX::XMVECTOR RigidBodys::GetRigidBodyRotation(std::string name)
{
	auto it = std::find_if(rigidBodys.begin(), rigidBodys.end(), [&name](const RigidBodyData& obj) {return obj.name == name; });

	if (it != rigidBodys.end())
	{
		return it->GetRotation();
	}

	return DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

btRigidBody * RigidBodys::GetRigidBody(std::string name)
{
	auto it = std::find_if(rigidBodys.begin(), rigidBodys.end(), [&name](const RigidBodyData& obj) {return obj.name == name; });

	if (it != rigidBodys.end())
	{
		return it->rigidBody;
	}

	return nullptr;
}

RigidBodyData::RigidBodyData()
{
	rigidBody = nullptr;
	name = "";
}

RigidBodyData::RigidBodyData(const RigidBodyData & other)
{
	*this = other;
}

RigidBodyData::RigidBodyData(std::string Name, btRigidBody * Body)
{
	name = Name;
	rigidBody = Body;
}

RigidBodyData::~RigidBodyData()
{

}

DirectX::XMMATRIX RigidBodyData::GetTransformMatrix()
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);

	float x = trans.getOrigin().getX();
	float y = trans.getOrigin().getY();
	float z = trans.getOrigin().getZ();

	float rotx = trans.getRotation().getX();
	float roty = trans.getRotation().getY();
	float rotz = trans.getRotation().getZ();
	float rotw = trans.getRotation().getW();

	// Calculate tranform matrix from quaternion orientation and position and store it.
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotx, roty, rotz, rotw));
	DirectX::XMMATRIX tranlateMatrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(x, y, z, 0.0f));
	DirectX::XMMATRIX transformMatrix = DirectX::XMMatrixIdentity() * rotationMatrix * tranlateMatrix;

	return transformMatrix;
}

DirectX::XMVECTOR RigidBodyData::GetPosition()
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);

	float x = trans.getOrigin().getX();
	float y = trans.getOrigin().getY();
	float z = trans.getOrigin().getZ();

	return DirectX::XMVectorSet(x, y, z, 0.0f);
}

DirectX::XMVECTOR RigidBodyData::GetRotation()
{
	btTransform trans;
	rigidBody->getMotionState()->getWorldTransform(trans);

	float rotx = trans.getRotation().getX();
	float roty = trans.getRotation().getY();
	float rotz = trans.getRotation().getZ();
	float rotw = trans.getRotation().getW();

	return DirectX::XMVectorSet(rotx, roty, rotz, rotw);
}