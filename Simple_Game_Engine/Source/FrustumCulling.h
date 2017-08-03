// Simple directx math based frustum culling class.

#ifndef _FrustumCulling_H_
#define _FrustumCulling_H_

// Includes
#include <DirectXMath.h>

class FrustumCulling
{
public:
	struct Point
	{
		Point(float x, float y, float z)
			:x(x), y(y), z(z)
		{
		}
		float x, y, z;
	};

	struct Cube
	{
		Cube(float centerX, float centerY, float centerZ, float rad)
			:xCenter(centerX), yCenter(centerY), zCenter(centerZ), radius(rad)
		{
		}
		float xCenter, yCenter, zCenter;
		float radius;
	};

	struct Sphere
	{
		Sphere(float centerX, float centerY, float centerZ, float rad)
			:xCenter(centerX), yCenter(centerY), zCenter(centerZ), radius(rad)
		{
		}
		float xCenter, yCenter, zCenter;
		float radius;
	};

	struct Rectangle
	{
		Rectangle(float centerX, float centerY, float centerZ, float SizeX, float SizeY, float SizeZ)
			:xCenter(centerX), yCenter(centerY), zCenter(centerZ), 
			xSize(SizeX), ySize(SizeY), zSize(SizeZ)
		{
		}
		float xCenter, yCenter, zCenter;
		float xSize, ySize, zSize;
	};


	FrustumCulling();
	FrustumCulling(const FrustumCulling&);
	~FrustumCulling();
	
	void ConstructFrustum(float screenDepth, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix);

	bool CheckPoint(Point point);
	bool CheckCube(Cube cube);
	bool CheckSphere(Sphere sphere);
	bool CheckRectangle(Rectangle rectangle);

private:
	DirectX::XMFLOAT4 m_planes[6];
};


#endif