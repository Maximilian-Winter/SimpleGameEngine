#pragma once


#include <DirectXMath.h>

#include "ModelTypes.h"

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
