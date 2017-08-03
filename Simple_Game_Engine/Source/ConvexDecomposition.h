#ifndef _CONVEXDECOMPOSITION_H_
#define _CONVEXDECOMPOSITION_H_

#include <btBulletDynamicsCommon.h>
#include <LinearMath\btIDebugDraw.h>
#include <BulletCollision\CollisionShapes\btShapeHull.h>
#include <BulletCollision\CollisionShapes\btConvexHullShape.h>
#include <Bullet3Geometry\b3ConvexHullComputer.h>
#include <Extras\ConvexDecomposition\ConvexDecomposition.h>
#include <Extras\HACD\hacdHACD.h>
#include <LinearMath\btSerializer.h>
#include <Extras\Serialize\BulletWorldImporter\btBulletWorldImporter.h>

struct ConvexDecompositionData
{
	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

	btAlignedObjectArray<btTriangleMesh*> m_trimeshes;
};

class MyConvexDecomposition : public ConvexDecomposition::ConvexDecompInterface
{
public:

	btAlignedObjectArray<btConvexHullShape*> m_convexShapes;
	btAlignedObjectArray<btVector3> m_convexCentroids;

	MyConvexDecomposition(ConvexDecompositionData* convexData);

	virtual void ConvexDecompResult(ConvexDecomposition::ConvexResult &result);

	int   	mBaseCount;
	int		mHullCount;
	btVector3	centroid;
	ConvexDecompositionData* mConvexData;
};
#endif 