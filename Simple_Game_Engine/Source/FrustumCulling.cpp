#include "FrustumCulling.h"


FrustumCulling::FrustumCulling()
{
}


FrustumCulling::FrustumCulling(const FrustumCulling& other)
{
}


FrustumCulling::~FrustumCulling()
{
}


void FrustumCulling::ConstructFrustum(float screenDepth, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	float zMinimum, r;
	DirectX::XMFLOAT4X4 matrix;

	DirectX::XMFLOAT4X4 viewMat;
	DirectX::XMFLOAT4X4 projMat;
	
	DirectX::XMStoreFloat4x4(&viewMat, viewMatrix);
	DirectX::XMStoreFloat4x4(&projMat, projectionMatrix);

	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projMat._43 / projMat._33;
	r = screenDepth / (screenDepth - zMinimum);
	projMat._33 = r;
	projMat._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix));

	// Calculate near plane of frustum.
	m_planes[0].x = matrix._14 + matrix._13;
	m_planes[0].y = matrix._24 + matrix._23;
	m_planes[0].z = matrix._34 + matrix._33;
	m_planes[0].w = matrix._44 + matrix._43;
	DirectX::XMStoreFloat4(&m_planes[0], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_planes[0])));

	// Calculate far plane of frustum.
	m_planes[1].x = matrix._14 - matrix._13; 
	m_planes[1].y = matrix._24 - matrix._23;
	m_planes[1].z = matrix._34 - matrix._33;
	m_planes[1].w = matrix._44 - matrix._43;
	DirectX::XMStoreFloat4(&m_planes[1], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_planes[1])));

	// Calculate left plane of frustum.
	m_planes[2].x = matrix._14 + matrix._11; 
	m_planes[2].y = matrix._24 + matrix._21;
	m_planes[2].z = matrix._34 + matrix._31;
	m_planes[2].w = matrix._44 + matrix._41;
	DirectX::XMStoreFloat4(&m_planes[2], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_planes[2])));

	// Calculate right plane of frustum.
	m_planes[3].x = matrix._14 - matrix._11;
	m_planes[3].y = matrix._24 - matrix._21;
	m_planes[3].z = matrix._34 - matrix._31;
	m_planes[3].w = matrix._44 - matrix._41;
	DirectX::XMStoreFloat4(&m_planes[3], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_planes[3])));

	// Calculate top plane of frustum.
	m_planes[4].x = matrix._14 - matrix._12;
	m_planes[4].y = matrix._24 - matrix._22;
	m_planes[4].z = matrix._34 - matrix._32;
	m_planes[4].w = matrix._44 - matrix._42;
	DirectX::XMStoreFloat4(&m_planes[4], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_planes[4])));

	// Calculate bottom plane of frustum.
	m_planes[5].x = matrix._14 + matrix._12;
	m_planes[5].y = matrix._24 + matrix._22;
	m_planes[5].z = matrix._34 + matrix._32;
	m_planes[5].w = matrix._44 + matrix._42;
	DirectX::XMStoreFloat4(&m_planes[5], DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&m_planes[5])));

	return;
}


bool FrustumCulling::CheckPoint(Point point)
{
	int i;
	
	// Check if the point is inside all six planes of the view frustum.
	for(i=0; i<6; i++) 
	{
		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet(point.x, point.y, point.z, 0.0f))) < 0.0f)
		{
			return false;
		}
	}

	return true;
}


bool FrustumCulling::CheckCube(Cube cube)
{
	int i;

	// Check if any one point of the cube is in the view frustum.
	for(i=0; i<6; i++) 
	{
		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter - cube.radius), (cube.yCenter - cube.radius), (cube.zCenter - cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}
		
		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter + cube.radius), (cube.yCenter - cube.radius), (cube.zCenter - cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter - cube.radius), (cube.yCenter + cube.radius), (cube.zCenter - cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter + cube.radius), (cube.yCenter + cube.radius), (cube.zCenter - cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter - cube.radius), (cube.yCenter - cube.radius), (cube.zCenter + cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter + cube.radius), (cube.yCenter - cube.radius), (cube.zCenter + cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter - cube.radius), (cube.yCenter + cube.radius), (cube.zCenter + cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}
		
		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((cube.xCenter + cube.radius), (cube.yCenter + cube.radius), (cube.zCenter + cube.radius), 0.0f))) >= 0.0f)
		{
			continue;
		}


		return false;
	}

	return true;
}


bool FrustumCulling::CheckSphere(Sphere sphere)
{
	int i;

	// Check if the radius of the sphere is inside the view frustum.
	for(i=0; i<6; i++) 
	{
		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet(sphere.xCenter, sphere.yCenter, sphere.zCenter, 0.0f))) < -sphere.radius)
		{
			return false;
		}
	}

	return true;
}


bool FrustumCulling::CheckRectangle(Rectangle rectangle)
{
	int i;

	// Check if any of the 6 planes of the rectangle are inside the view frustum.
	for (i = 0; i < 6; i++)
	{
		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter - rectangle.xSize), (rectangle.yCenter - rectangle.ySize), (rectangle.zCenter - rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter + rectangle.xSize), (rectangle.yCenter - rectangle.ySize), (rectangle.zCenter - rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter - rectangle.xSize), (rectangle.yCenter + rectangle.ySize), (rectangle.zCenter - rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter + rectangle.xSize), (rectangle.yCenter + rectangle.ySize), (rectangle.zCenter - rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter - rectangle.xSize), (rectangle.yCenter - rectangle.ySize), (rectangle.zCenter + rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter + rectangle.xSize), (rectangle.yCenter - rectangle.ySize), (rectangle.zCenter + rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter - rectangle.xSize), (rectangle.yCenter + rectangle.ySize), (rectangle.zCenter + rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}

		if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&m_planes[i]), DirectX::XMVectorSet((rectangle.xCenter + rectangle.xSize), (rectangle.yCenter + rectangle.ySize), (rectangle.zCenter + rectangle.zSize), 0.0f))) >= 0.0f)
		{
			continue;
		}
		return false;
	}

	return true;

}
