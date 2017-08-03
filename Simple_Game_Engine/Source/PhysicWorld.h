// Rigid body world class
// Handle all physic stuff and collision detection for rigid bodys.

#ifndef _PHYSICWORLD_H_
#define _PHYSICWORLD_H_

// Includes
#include <DirectXMath.h>

#include "ConvexDecomposition.h"
#include "RigidBodys.h"
#include "PBStaticModel.h"
#include "PhysicDebugDraw.h"
#include "Userinterface.h"


class PhysicWorld
{
public:
	PhysicWorld();
	~PhysicWorld();

	// Remove collision body from the dynamic world.
	void RemoveRigidBody(int index);
	void RemoveRigidBody(std::string name);

	// Add collision body to the dynamic world.
	btRigidBody* AddPlane(std::string name, float x, float y, float z, float dirX, float dirY, float dirZ);
	btRigidBody* AddPlane(std::string name, DirectX::FXMVECTOR position, DirectX::FXMVECTOR direction);
	btRigidBody* AddSphere(std::string name, float x, float y, float z, float radius, float mass);
	btRigidBody* AddSphere(std::string name, DirectX::FXMVECTOR position, float radius, float mass);
	btRigidBody* AddBox(std::string name, float x, float y, float z, float width, float height, float depth, float mass);
	btRigidBody* AddBox(std::string name, DirectX::FXMVECTOR position, float width, float height, float depth, float mass);
	btRigidBody* AddCylinder(std::string name, float x, float y, float z, float diameter, float height, float mass);
	btRigidBody* AddCylinder(std::string name, DirectX::FXMVECTOR position, float diameter, float height, float mass);
	btRigidBody* AddCone(std::string name, float x, float y, float z, float diameter, float height, float mass);
	btRigidBody* AddCone(std::string name, DirectX::FXMVECTOR position, float diameter, float height, float mass);
	btRigidBody* AddStaticTriangleMesh(std::string name, float x, float y, float z, std::vector<int> indicies, std::vector<float> vertices);
	btRigidBody* AddStaticTriangleMesh(std::string name, DirectX::FXMVECTOR position, std::vector<int> indicies, std::vector<float> vertices);
	btRigidBody* AddConvexHullMesh(std::string name, float x, float y, float z, std::vector<int> indicies, std::vector<float> vertices, float mass);
	btRigidBody* AddConvexHullMesh(std::string name, DirectX::FXMVECTOR position, std::vector<int> indicies, std::vector<float> vertices, float mass);
	btRigidBody* AddConvexDecompMesh(std::string name, float x, float y, float z, std::vector<int> indicies, std::vector<float> vertices, float mass);
	btRigidBody* AddConvexDecompMesh(std::string name, DirectX::FXMVECTOR position, std::vector<int> indicies, std::vector<float> vertices, float mass);
	btRigidBody* AddCollisionMeshFromFile(std::string name, float x, float y, float z, std::string filename, float mass);
	btRigidBody* AddCollisionMeshFromFile(std::string name, DirectX::FXMVECTOR position, std::string filename, float mass);
	RigidBodys& GetRigidBodys();
	// Lines that visualize physics stuff for debug purpose.
	std::vector<PhysicDebugDraw::Lines>& GetDebugLinesToDraw();
	// Update all physics by delta time.
	void UpdateWorld(float deltaTime);

private:
	// World base members.
	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	RigidBodys m_rigidBodys;
	PhysicDebugDraw m_debugDraw;
};

#endif