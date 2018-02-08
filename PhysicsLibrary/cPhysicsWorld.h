#pragma once
//#include <Physics\Interfaces\iPhysicsWorld.h>
#include <iPhysicsWorld.h>
#include <vector>
#include "cRigidBody.h"

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
		//virtual bool CollideSpherePlane( cRigidBody* bodyA, cSphereRigidBody* sphereA, cRigidBody* bodyB, cPlaneRigidBody* planeB );
		//virtual bool CollideSphereSphere( cRigidBody* bodyA, cSphereRigidBody* sphereA, cRigidBody* bodyB, cSphereRigidBody* sphereB );

	private:
		
		std::vector<cRigidBody*> mRigidBody;
	};
}