#include "Transform.h"

const Transform Transform::Identity = FromTransformMatrix(nullptr, DirectX::XMMatrixIdentity());

Transform::Transform()
	: LocalPosition(0.f, 0.f, 0.f),
	  LocalRotation()
	, LocalScale(1.f, 1.f, 1.f)
	, Parent(nullptr)
{
	DirectX::XMStoreFloat4(&LocalRotation, DirectX::XMQuaternionIdentity());
	CalculateInternals();
}

Transform::Transform(Transform& parent)
	: LocalPosition(0.f, 0.f, 0.f),
	  LocalRotation()
	, LocalScale(1.f, 1.f, 1.f)
	, Parent(&parent)
{
	DirectX::XMStoreFloat4(&LocalRotation, DirectX::XMQuaternionIdentity());
	CalculateInternals();
}



Transform::Transform(Transform& parent, DirectX::FXMVECTOR position, DirectX::FXMVECTOR quaternionRotation,
                     DirectX::FXMVECTOR scale, bool localSpace) : Parent(&parent)
{
	if(localSpace)
	{
		DirectX::XMStoreFloat3(&LocalPosition, position);
		DirectX::XMStoreFloat4(&LocalRotation, quaternionRotation);
		DirectX::XMStoreFloat3(&LocalScale, scale);
		CalculateInternals();
	}
	else
	{
		DirectX::XMStoreFloat3(&Position, position);
		DirectX::XMStoreFloat4(&Rotation, quaternionRotation);
		DirectX::XMStoreFloat3(&Scale, scale);
		CalculateInternals(false);
	}
}


Transform::Transform(Transform* parent)
	: LocalPosition(0.f, 0.f, 0.f),
	LocalRotation()
	, LocalScale(1.f, 1.f, 1.f)
	, Parent(parent)
{
	DirectX::XMStoreFloat4(&LocalRotation, DirectX::XMQuaternionIdentity());
	CalculateInternals();
}



Transform::Transform(Transform* parent, DirectX::FXMVECTOR position, DirectX::FXMVECTOR quaternionRotation,
	DirectX::FXMVECTOR scale, bool localSpace) : Parent(parent)
{
	if (localSpace)
	{
		DirectX::XMStoreFloat3(&LocalPosition, position);
		DirectX::XMStoreFloat4(&LocalRotation, quaternionRotation);
		DirectX::XMStoreFloat3(&LocalScale, scale);
		CalculateInternals();
	}
	else
	{
		DirectX::XMStoreFloat3(&Position, position);
		DirectX::XMStoreFloat4(&Rotation, quaternionRotation);
		DirectX::XMStoreFloat3(&Scale, scale);
		CalculateInternals(false);
	}
}

void Transform::CalculateInternals(bool fromLocal)
{
	if (fromLocal)
	{
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

		const DirectX::XMMATRIX ScaleMatrix = DirectX::XMMatrixScaling(LocalScale.x, LocalScale.y, LocalScale.z);

		const DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&LocalRotation));
		//RotationMatrix =  DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&Rotation));

		const DirectX::XMMATRIX TranslateMatrix = DirectX::XMMatrixTranslation(LocalPosition.x, LocalPosition.y, LocalPosition.z);

		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, ScaleMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, RotationMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, TranslateMatrix);
		if(Parent != nullptr)
		{
			worldMatrix = DirectX::XMMatrixMultiply(Parent->GetLocalToWorldSpaceTransformMatrix(), worldMatrix);
		}
		else
		{
			worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixIdentity(), worldMatrix);
		}
		

		XMStoreFloat4x4(&LocalToWorldSpaceMatrix, worldMatrix);
		XMStoreFloat4x4(&WorldToLocalSpaceMatrix, DirectX::XMMatrixInverse(nullptr, worldMatrix));
		DirectX::XMStoreFloat3(&Position, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&LocalPosition), worldMatrix));
		DirectX::XMStoreFloat4(&Rotation, DirectX::XMVector3Transform(DirectX::XMLoadFloat4(&LocalRotation), worldMatrix));
		DirectX::XMStoreFloat3(&Scale, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&LocalScale), worldMatrix));
	}
	else
	{
		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

		DirectX::XMMATRIX ScaleMatrix = DirectX::XMMatrixScaling(Scale.x, Scale.y, Scale.z);

		DirectX::XMMATRIX RotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&Rotation));
		//RotationMatrix =  DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&Rotation));

		DirectX::XMMATRIX TranslateMatrix;
		TranslateMatrix = DirectX::XMMatrixTranslation(Position.x, Position.y, Position.z);

		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, ScaleMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, RotationMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, TranslateMatrix);

		XMStoreFloat4x4(&LocalToWorldSpaceMatrix, worldMatrix);
		const DirectX::XMMATRIX worldToLocalMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);
		XMStoreFloat4x4(&WorldToLocalSpaceMatrix, worldToLocalMatrix);
		DirectX::XMStoreFloat3(&LocalPosition, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&Position), worldToLocalMatrix));
		DirectX::XMStoreFloat4(&LocalRotation, DirectX::XMVector3Transform(DirectX::XMLoadFloat4(&Rotation), worldToLocalMatrix));
		DirectX::XMStoreFloat3(&LocalScale, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&Scale), worldToLocalMatrix));
	}
}

Transform Transform::operator*(Transform rhs) const
{
	return FromTransformMatrix(this->Parent, this->GetLocalToWorldSpaceTransformMatrix() * rhs.GetLocalToWorldSpaceTransformMatrix());
}

DirectX::XMMATRIX Transform::GetLocalToWorldSpaceTransformMatrix() const
{
	return DirectX::XMLoadFloat4x4(&LocalToWorldSpaceMatrix);
}

DirectX::XMMATRIX Transform::GetWorldToLocalTransformMatrix() const
{
	return DirectX::XMLoadFloat4x4(&WorldToLocalSpaceMatrix);
}

DirectX::FXMVECTOR Transform::GetLocalPosition()
{
	return DirectX::XMLoadFloat3(&LocalPosition);
}

DirectX::FXMVECTOR Transform::GetWorldPosition()
{
	return DirectX::XMLoadFloat3(&Position);
}

DirectX::FXMVECTOR Transform::GetLocalRotation()
{
	return DirectX::XMLoadFloat4(&LocalRotation);
}

DirectX::FXMVECTOR Transform::GetWorldRotation()
{
	return DirectX::XMLoadFloat4(&Rotation);
}

DirectX::FXMVECTOR Transform::GetLocalScale()
{
	return DirectX::XMLoadFloat3(&LocalScale);
}

DirectX::FXMVECTOR Transform::GetWorldScale()
{
	return DirectX::XMLoadFloat3(&Scale);
}

void Transform::SetLocalPosition(DirectX::FXMVECTOR position)
{
	DirectX::XMStoreFloat3(&LocalPosition, position);
	CalculateInternals();
}

void Transform::SetWorldPosition(DirectX::FXMVECTOR position)
{
	DirectX::XMStoreFloat3(&Position, position);
	CalculateInternals(false);
}

void Transform::SetLocalRotation(DirectX::FXMVECTOR rotation)
{
	DirectX::XMStoreFloat4(&LocalRotation, rotation);
	CalculateInternals();
}

void Transform::SetWorldRotation(DirectX::FXMVECTOR rotation)
{
	DirectX::XMStoreFloat4(&Rotation, rotation);
	CalculateInternals(false);
}

void Transform::SetLocalScale(DirectX::FXMVECTOR scale)
{
	DirectX::XMStoreFloat3(&LocalScale, scale);
	CalculateInternals();
}

void Transform::SetWorldScale(DirectX::FXMVECTOR scale)
{
	DirectX::XMStoreFloat3(&Scale, scale);
	CalculateInternals(false);
}

Transform Transform::FromTransformMatrix(Transform* parent, const DirectX::XMMATRIX m)
{
	DirectX::XMVECTOR pos;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;
	DirectX::XMMatrixDecompose(&scale, &rotation, &pos, m);

	return Transform(parent, pos, rotation, scale);
}