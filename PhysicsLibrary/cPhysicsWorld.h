#pragma once
//#include <Physics\Interfaces\iPhysicsWorld.h>
#include <iPhysicsWorld.h>
#include <vector>
#include "cRigidBody.h"
#include "shapes.h"
#include "cIntegration.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp> // <- This includes vec3 and mat4

namespace nPhysics
{
	class cPhysicsWorld : public iPhysicsWorld
	{
	public:
		virtual ~cPhysicsWorld();

		virtual void TimeStep(float deltaTime);

		virtual void AddRigidBody(iRigidBody* rigidBody);
		virtual void RemoveRigidBody(iRigidBody* rigidBody);

		virtual bool Collide( cRigidBody* bodyA, cRigidBody* bodyB );
		virtual bool CollideSpherePlane( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cPlaneShape* planeB );
		virtual bool CollideSphereSphere( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cSphereShape* sphereB );

		cIntegration myIntegrator;

	private:
		
		std::vector<cRigidBody*> mRigidBody;
	};
}