#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <DirectXMath.h>
#include <memory>


class Transform
{
public:

	Transform();
	Transform(Transform& parent);
	Transform(Transform& parent, DirectX::FXMVECTOR position, DirectX::FXMVECTOR quaternionRotation, DirectX::FXMVECTOR scale, bool localSpace = true);
	Transform(Transform* parent);
	Transform(Transform* parent, DirectX::FXMVECTOR position, DirectX::FXMVECTOR quaternionRotation, DirectX::FXMVECTOR scale, bool localSpace = true);
	~Transform() = default;

	Transform operator*(Transform rhs) const;

	DirectX::XMMATRIX GetLocalToWorldSpaceTransformMatrix() const;
	DirectX::XMMATRIX GetWorldToLocalTransformMatrix() const;

	DirectX::FXMVECTOR GetLocalPosition();
	DirectX::FXMVECTOR GetWorldPosition();

	DirectX::FXMVECTOR GetLocalRotation();
	DirectX::FXMVECTOR GetWorldRotation();

	DirectX::FXMVECTOR GetLocalScale();
	DirectX::FXMVECTOR GetWorldScale();

	void SetLocalPosition(DirectX::FXMVECTOR position);
	void SetWorldPosition(DirectX::FXMVECTOR position);

	void SetLocalRotation(DirectX::FXMVECTOR rotation);
	void SetWorldRotation(DirectX::FXMVECTOR rotation);

	void SetLocalScale(DirectX::FXMVECTOR scale);
	void SetWorldScale(DirectX::FXMVECTOR scale);

	static Transform FromTransformMatrix(Transform* parent, const DirectX::XMMATRIX m);

	static const Transform Identity;

private:

	void CalculateInternals(bool fromLocal = true);

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 LocalPosition;
	DirectX::XMFLOAT4 Rotation;
	DirectX::XMFLOAT4 LocalRotation;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT3 LocalScale;

	DirectX::XMFLOAT4X4 LocalToWorldSpaceMatrix;
	DirectX::XMFLOAT4X4 WorldToLocalSpaceMatrix;

	Transform* Parent;
};

#endif
