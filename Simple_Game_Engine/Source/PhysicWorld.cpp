#include "PhysicWorld.h"

PhysicWorld::PhysicWorld()
{
	m_broadphase = new btDbvtBroadphase();
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_solver = new btSequentialImpulseConstraintSolver;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	/*
	debugDraw.setDebugMode(
	btIDebugDraw::DBG_DrawWireframe |
	btIDebugDraw::DBG_DrawAabb //|
	//btIDebugDraw::DBG_DrawContactPoints |
	//btIDebugDraw::DBG_DrawText |
	//btIDebugDraw::DBG_DrawConstraintLimits |
	//btIDebugDraw::DBG_DrawConstraints //|
	);
	m_dynamicsWorld->setDebugDrawer(&debugDraw);
	*/

}

PhysicWorld::~PhysicWorld()
{
	for (int i = 0; i < m_rigidBodys.rigidBodys.size(); i++)
	{
		m_dynamicsWorld->removeCollisionObject(m_rigidBodys.rigidBodys[i].rigidBody);
		btMotionState* motionState = m_rigidBodys.rigidBodys[i].rigidBody->getMotionState();
		btCollisionShape* shape = m_rigidBodys.rigidBodys[i].rigidBody->getCollisionShape();
		delete m_rigidBodys.rigidBodys[i].rigidBody;
		delete shape;
		delete motionState;
	}

	delete m_dynamicsWorld;
	delete m_solver;
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;
}

// Remove collision body from the dynamic world.

void PhysicWorld::RemoveRigidBody(int index)
{
	m_dynamicsWorld->removeCollisionObject(m_rigidBodys.rigidBodys[index].rigidBody);
	btMotionState* motionState = m_rigidBodys.rigidBodys[index].rigidBody->getMotionState();
	btCollisionShape* shape = m_rigidBodys.rigidBodys[index].rigidBody->getCollisionShape();
	delete m_rigidBodys.rigidBodys[index].rigidBody;
	delete shape;
	delete motionState;
}

void PhysicWorld::RemoveRigidBody(std::string name)
{
	for (auto it = m_rigidBodys.rigidBodys.begin(); it < m_rigidBodys.rigidBodys.end(); it++)
	{
		if (it->name == name)
		{
			m_dynamicsWorld->removeCollisionObject(it->rigidBody);
			btMotionState* motionState = it->rigidBody->getMotionState();
			btCollisionShape* shape = it->rigidBody->getCollisionShape();
			delete it->rigidBody;
			delete shape;
			delete motionState;
		}
	}
}

// Add collision body to the dynamic world

btRigidBody * PhysicWorld::AddPlane(std::string name, float x, float y, float z, float dirX, float dirY, float dirZ)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(dirX, dirY, dirZ), 0);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));

	return body;
}

btRigidBody * PhysicWorld::AddPlane(std::string name, DirectX::FXMVECTOR position, DirectX::FXMVECTOR direction)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position)));
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(DirectX::XMVectorGetX(direction), DirectX::XMVectorGetY(direction), DirectX::XMVectorGetZ(direction)), 0);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));

	return body;
}

btRigidBody * PhysicWorld::AddSphere(std::string name, float x, float y, float z, float radius, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btSphereShape* sphere = new btSphereShape(radius);
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		sphere->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));
	return body;
}

btRigidBody * PhysicWorld::AddSphere(std::string name, DirectX::FXMVECTOR position, float radius, float mass)
{
	return AddSphere(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), radius, mass);
}

btRigidBody * PhysicWorld::AddBox(std::string name, float x, float y, float z, float width, float height, float depth, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btBoxShape* box = new btBoxShape(btVector3(width / 2.0, height / 2.0, depth / 2.0));
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		box->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));
	return body;
}

btRigidBody * PhysicWorld::AddBox(std::string name, DirectX::FXMVECTOR position, float width, float height, float depth, float mass)
{
	return AddBox(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), width, height, depth, mass);
}

btRigidBody * PhysicWorld::AddCylinder(std::string name, float x, float y, float z, float diameter, float height, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btCylinderShape* cylinder = new btCylinderShape(btVector3(diameter / 2.0, height / 2.0, diameter / 2.0));
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		cylinder->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cylinder, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));
	return body;
}

btRigidBody * PhysicWorld::AddCylinder(std::string name, DirectX::FXMVECTOR position, float diameter, float height, float mass)
{
	return AddCylinder(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), diameter, height, mass);
}

btRigidBody * PhysicWorld::AddCone(std::string name, float x, float y, float z, float diameter, float height, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btConeShape* cone = new btConeShape(diameter, height);
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		cone->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, cone, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));
	return body;
}

btRigidBody * PhysicWorld::AddCone(std::string name, DirectX::FXMVECTOR position, float diameter, float height, float mass)
{
	return AddCone(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), diameter, height, mass);
}

btRigidBody * PhysicWorld::AddStaticTriangleMesh(std::string name, float x, float y, float z, std::vector<int> indicies, std::vector<float> vertices)
{
	btTriangleMesh* triMesh = new btTriangleMesh();

	for (int i = 0; i < vertices.size(); i += 9)
	{
		// For whatever your source of triangles is
		//   give the three points of each triangle:
		btVector3 v0(vertices[i], vertices[i + 1], vertices[i + 2]);
		btVector3 v1(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		btVector3 v2(vertices[i + 6], vertices[i + 7], vertices[i + 8]);


		// Then add the triangle to the mesh:
		triMesh->addTriangle(v0, v1, v2);
	}

	for (int i = 0; i < indicies.size(); i++)
	{
		triMesh->addIndex(indicies[i]);
	}

	btCollisionShape *mTriMeshShape = new btBvhTriangleMeshShape(triMesh, true);

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btMotionState* motion = new btDefaultMotionState(t);

	btVector3 inertia(0, 0, 0);

	btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, mTriMeshShape, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));

	//delete mTriMesh;

	return body;
}

btRigidBody * PhysicWorld::AddStaticTriangleMesh(std::string name, DirectX::FXMVECTOR position, std::vector<int> indicies, std::vector<float> vertices)
{
	return AddStaticTriangleMesh(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), indicies, vertices);
}

btRigidBody * PhysicWorld::AddConvexHullMesh(std::string name, float x, float y, float z, std::vector<int> indicies, std::vector<float> vertices, float mass)
{
	btTriangleMesh* trimesh = new btTriangleMesh();
	btVector3 localScaling(6.f, 6.f, 6.f);

	// Build triangle mesh, indice array and vertex array.
	for (int i = 0; i < vertices.size(); i += 9)
	{
		// For whatever your source of triangles is
		//   give the next point:
		btVector3 v0(vertices[i], vertices[i + 1], vertices[i + 2]);
		btVector3 v1(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
		btVector3 v2(vertices[i + 6], vertices[i + 7], vertices[i + 8]);

		trimesh->addTriangle(v0, v1, v2);
	}

	for (int i = 0; i < indicies.size(); i++)
	{
		trimesh->addIndex(indicies[i]);
	}

	btConvexShape* tmpConvexShape = new btConvexTriangleMeshShape(trimesh);

	//create a hull approximation
	btShapeHull* hull = new btShapeHull(tmpConvexShape);
	btScalar margin = tmpConvexShape->getMargin();
	hull->buildHull(margin);

	btConvexHullShape* convexHullShape = new btConvexHullShape();
	bool updateLocalAabb = false;

	for (int i = 0; i < hull->numVertices(); i++)
	{
		convexHullShape->addPoint(hull->getVertexPointer()[i], updateLocalAabb);
	}
	convexHullShape->recalcLocalAabb();

	convexHullShape->setLocalScaling(btVector3(0.9, 0.9, 0.9));

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btMotionState* motion = new btDefaultMotionState(t);

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		convexHullShape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, convexHullShape, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));

	// convexHullMesh;


	delete trimesh;
	delete tmpConvexShape;
	delete hull;


	return body;
}

btRigidBody * PhysicWorld::AddConvexHullMesh(std::string name, DirectX::FXMVECTOR position, std::vector<int> indicies, std::vector<float> vertices, float mass)
{
	return AddConvexHullMesh(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), indicies, vertices, mass);
}

btRigidBody * PhysicWorld::AddConvexDecompMesh(std::string name, float x, float y, float z, std::vector<int> indicies, std::vector<float> vertices, float mass)
{
	float* verticeArr = new float[vertices.size()];
	float* IndicesArr = new float[indicies.size()];

	// Build indice array and vertex array.
	for (int i = 0; i < vertices.size(); i++)
	{
		verticeArr[i] = vertices[i];
	}

	for (int i = 0; i < indicies.size(); i++)
	{
		IndicesArr[i] = indicies[i];
	}

	// Convex decomposition.
	unsigned int depth = 5;
	float cpercent = 5;
	float ppercent = 15;
	unsigned int maxv = 16;
	float skinWidth = 0.0;

	ConvexDecomposition::DecompDesc desc;
	desc.mVcount = (vertices.size() / 3);
	desc.mVertices = verticeArr;
	desc.mTcount = (indicies.size() / 3);
	desc.mIndices = (unsigned int *)IndicesArr;
	desc.mDepth = depth;
	desc.mCpercent = cpercent;
	desc.mPpercent = ppercent;
	desc.mMaxVertices = maxv;
	desc.mSkinWidth = skinWidth;

	ConvexDecompositionData convexDecompResult;
	MyConvexDecomposition	convexDecomposition(&convexDecompResult);
	desc.mCallback = &convexDecomposition;


	//-----------------------------------------------
	// HACD
	//-----------------------------------------------

	std::vector< HACD::Vec3<HACD::Real> > points;
	std::vector< HACD::Vec3<long> > triangles;

	for (int i = 0; i< (vertices.size() / 3); i++)
	{
		int index = i * 3;
		HACD::Vec3<HACD::Real> vertex(verticeArr[index], verticeArr[index + 1], verticeArr[index + 2]);
		points.push_back(vertex);
	}

	for (int i = 0; i < (indicies.size() / 3); i++)
	{
		int index = i * 3;
		HACD::Vec3<long> triangle(IndicesArr[index], IndicesArr[index + 1], IndicesArr[index + 2]);
		triangles.push_back(triangle);
	}


	HACD::HACD myHACD;
	myHACD.SetPoints(&points[0]);
	myHACD.SetNPoints(points.size());
	myHACD.SetTriangles(&triangles[0]);
	myHACD.SetNTriangles(triangles.size());
	myHACD.SetCompacityWeight(0.1);
	myHACD.SetVolumeWeight(0.0);

	// HACD parameters
	// Recommended parameters: 2 100 0 0 0 0
	size_t nClusters = 2;
	double concavity = 100;
	bool invert = false;
	bool addExtraDistPoints = false;
	bool addNeighboursDistPoints = false;
	bool addFacesPoints = false;

	myHACD.SetNClusters(nClusters);                     // minimum number of clusters
	myHACD.SetNVerticesPerCH(100);                      // max of 100 vertices per convex-hull
	myHACD.SetConcavity(concavity);                     // maximum concavity
	myHACD.SetAddExtraDistPoints(addExtraDistPoints);
	myHACD.SetAddNeighboursDistPoints(addNeighboursDistPoints);
	myHACD.SetAddFacesPoints(addFacesPoints);

	myHACD.Compute();
	nClusters = myHACD.GetNClusters();

	std::string meshFileNameWRL = name + ".wrl";
	myHACD.Save(meshFileNameWRL.c_str(), false);

	btCompoundShape* compound = new btCompoundShape();
	//convexDecompResult.m_collisionShapes.push_back(compound);

	btTransform trans;
	trans.setIdentity();

	for (int c = 0; c<nClusters; c++)
	{
		//generate convex result
		size_t nPoints = myHACD.GetNPointsCH(c);
		size_t nTriangles = myHACD.GetNTrianglesCH(c);

		float* vertices = new float[nPoints * 3];
		unsigned int* triangles = new unsigned int[nTriangles * 3];

		HACD::Vec3<HACD::Real> * pointsCH = new HACD::Vec3<HACD::Real>[nPoints];
		HACD::Vec3<long> * trianglesCH = new HACD::Vec3<long>[nTriangles];
		myHACD.GetCH(c, pointsCH, trianglesCH);

		// points
		for (size_t v = 0; v < nPoints; v++)
		{
			vertices[3 * v] = pointsCH[v].X();
			vertices[3 * v + 1] = pointsCH[v].Y();
			vertices[3 * v + 2] = pointsCH[v].Z();
		}
		// triangles
		for (size_t f = 0; f < nTriangles; f++)
		{
			triangles[3 * f] = trianglesCH[f].X();
			triangles[3 * f + 1] = trianglesCH[f].Y();
			triangles[3 * f + 2] = trianglesCH[f].Z();
		}

		delete[] pointsCH;
		delete[] trianglesCH;

		ConvexDecomposition::ConvexResult r(nPoints, vertices, nTriangles, triangles);
		convexDecomposition.ConvexDecompResult(r);
	}

	for (int i = 0; i < convexDecomposition.m_convexShapes.size(); i++)
	{
		btVector3 centroid = convexDecomposition.m_convexCentroids[i];
		trans.setOrigin(centroid);
		btConvexHullShape* convexShape = convexDecomposition.m_convexShapes[i];

		compound->addChildShape(trans, convexShape);
		//btRigidBody* body;
		//body = localCreateRigidBody(1.0, trans, convexShape);
		/*
		btMotionState* motion = new btDefaultMotionState(trans);

		btVector3 inertia(0, 0, 0);

		convexShape->calculateLocalInertia(1.0f, inertia);

		btRigidBody::btRigidBodyConstructionInfo info(btScalar(mass), motion, convexShape, inertia);
		btRigidBody* body = new btRigidBody(info);
		m_dynamicsWorld->addRigidBody(body);*/
	}

	compound->setLocalScaling(btVector3(0.168, 0.168, 0.168));
	compound->recalculateLocalAabb();

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btMotionState* motion = new btDefaultMotionState(t);

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		compound->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, compound, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));

	delete verticeArr;
	delete IndicesArr;

	return body;
}

btRigidBody * PhysicWorld::AddConvexDecompMesh(std::string name, DirectX::FXMVECTOR position, std::vector<int> indicies, std::vector<float> vertices, float mass)
{
	return AddConvexDecompMesh(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), indicies, vertices, mass);
}

btRigidBody * PhysicWorld::AddCollisionMeshFromFile(std::string name, float x, float y, float z, std::string filename, float mass)
{
	btBulletWorldImporter loader(0);//don't store info into the world
	loader.setVerboseMode(false);
	if (!loader.loadFile(filename.c_str())) return NULL;
	btCollisionShape* shape = NULL;
	if (loader.getNumCollisionShapes()>0) shape = loader.getCollisionShapeByIndex(0);

	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));

	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		shape->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
	btRigidBody* body = new btRigidBody(info);
	m_dynamicsWorld->addRigidBody(body);
	m_rigidBodys.rigidBodys.push_back(RigidBodyData(name, body));

	return body;
}

btRigidBody * PhysicWorld::AddCollisionMeshFromFile(std::string name, DirectX::FXMVECTOR position, std::string filename, float mass)
{
	return AddCollisionMeshFromFile(name, DirectX::XMVectorGetX(position), DirectX::XMVectorGetY(position), DirectX::XMVectorGetZ(position), filename, mass);
}

RigidBodys & PhysicWorld::GetRigidBodys()
{
	return m_rigidBodys;
}

std::vector<PhysicDebugDraw::Lines>& PhysicWorld::GetDebugLinesToDraw()
{
	return m_debugDraw.GetLinesToDraw();
}

void PhysicWorld::UpdateWorld(float deltaTime)
{
	m_dynamicsWorld->stepSimulation(deltaTime);
	//m_dynamicsWorld->debugDrawWorld();
}


