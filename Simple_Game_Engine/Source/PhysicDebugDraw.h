#ifndef _PHYSICDEBUGDRAW_H_
#define _PHYSICDEBUGDRAW_H_

#include <vector>
#include <LinearMath\btIDebugDraw.h>
#include <DirectXMath.h>

class PhysicDebugDraw : public btIDebugDraw
{

public:

	struct Lines
	{
		Lines(float fromX, float fromY, float fromZ, float toX, float toY, float toZ, float r, float g, float b)
		{
			from.x = fromX;
			from.y = fromY;
			from.z = fromZ;

			to.x = toX;
			to.y = toY;
			to.z = toZ;

			color.x = r;
			color.y = g;
			color.z = b;
		}

		DirectX::XMFLOAT3 from;
		DirectX::XMFLOAT3 to;
		DirectX::XMFLOAT3 color;
	};

	PhysicDebugDraw() :
		mode(DBG_NoDebug)
	{

	}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		m_linesToDraw.push_back(Lines(from.getX(), from.getY(), from.getZ(), to.getX(), to.getY(), to.getZ(), color.getX(), color.getY(), color.getZ()));
	}

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{}

	void reportErrorWarning(const char* text)
	{}

	void draw3dText(const btVector3& location, const char* text) { }

	void setDebugMode(int mode) { this->mode = mode; }

	int getDebugMode() const { return this->mode; }

	std::vector<Lines>& GetLinesToDraw()
	{
		return m_linesToDraw;
	}

private:

	std::vector<Lines> m_linesToDraw;
	int mode;
};
#endif
