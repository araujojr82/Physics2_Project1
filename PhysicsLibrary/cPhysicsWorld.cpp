#include "cPhysicsWorld.h"
#include <algorithm>

#include "CollisionDetection.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace nPhysics
{
	cPhysicsWorld::~cPhysicsWorld()
	{

	}

	void cPhysicsWorld::TimeStep(float deltaTime)
	{
		const glm::vec3 GRAVITY = glm::vec3( 0.0f, 0.0f, -1.0f );

		std::vector<cRigidBody*>::iterator itRigidBody = mRigidBody.begin();
		while (itRigidBody != mRigidBody.end())
		{
			cRigidBody* rb = *itRigidBody;

			eShapeType shape = rb->GetShape()->GetShapeType();

			if( shape == SHAPE_TYPE_PLANE ) break;

			//Explicity Euler integration (RK4)
			// New position is based on velocity over time
			rb->mPosition += rb->mVelocity * ( float )deltaTime;
			
			// New velocity is based on acceleration over time
			rb->mVelocity += ( ( float )deltaTime * rb->mAcceleration ) + ( ( float )deltaTime * GRAVITY );

			itRigidBody++;
		}

		float* mDt = &deltaTime;

		//while itRb
		//{
		//	Integrate Rigidbodies
		//}


		size_t numBodies = mRigidBody.size();


		for( size_t iA = 0; iA < numBodies - 1; iA++ )
		{

			for( size_t iB = 0; iB < numBodies - 1; iB++ )
			{
				if( iA == iB ) break; //Don't test for itself

				Collide( mRigidBody[iA], mRigidBody[iB] );

				////Add colliding pairs
				//collidingPairs.push_back( std::make_pair( mRigidBody[iA], mRigidBody[iB] ))
			}

		}

		// clear forces
		//just zeroes out the acceleration vector for each rigid body.
		itRigidBody = mRigidBody.begin();
		while( itRigidBody != mRigidBody.end() )
		{
			cRigidBody* rb = *itRigidBody;
			rb->mAcceleration = glm::vec3( 0.0f );

			itRigidBody++;
		}

		////
		//if( mCollisionListener )
		//{
		//	//rip thorugh that list of rigid body pairs
		//	//call
		//	while( itPair != collidingPairs.end() )
		//	{
		//		mCollisionListener->HandleCollision( pair.first, pair.second );
		//	}

		//}

	}

	void cPhysicsWorld::AddRigidBody(iRigidBody* rigidBody)
	{
		cRigidBody* rb = dynamic_cast<cRigidBody*>(rigidBody);
		if (!rb)
		{
			return;
		}
		std::vector<cRigidBody*>::iterator itRigidBody;
		itRigidBody = std::find(mRigidBody.begin(), mRigidBody.end(), rb);
		if (itRigidBody == mRigidBody.end())
		{
			mRigidBody.push_back(rb);
		}
	}
	void cPhysicsWorld::RemoveRigidBody(iRigidBody* rigidBody)
	{
		cRigidBody* rb = dynamic_cast<cRigidBody*>(rigidBody);
		if (!rb)
		{
			return;
		}
		std::vector<cRigidBody*>::iterator itRigidBody;
		
		itRigidBody = std::remove(mRigidBody.begin(), mRigidBody.end(), rb);
		if (itRigidBody != mRigidBody.end())
		{
			mRigidBody.resize(mRigidBody.size() - 1);
		}
	}

	bool cPhysicsWorld::Collide( cRigidBody* bodyA, cRigidBody* bodyB )
	{
		eShapeType typeA = bodyA->GetShape()->GetShapeType();
		eShapeType typeB = bodyB->GetShape()->GetShapeType();

		switch( typeA )
		{
		case nPhysics::SHAPE_TYPE_SPHERE:
			{
				cSphereShape * sphereA = dynamic_cast< cSphereShape* > ( bodyA->GetShape() );
				switch( typeB )
				{
					case nPhysics::SHAPE_TYPE_PLANE:
						{
							cPlaneShape* planeB = dynamic_cast< cPlaneShape* >( bodyB->GetShape() );
							
							glm::vec3 pA;
							bodyA->GetPosition( pA );
							float radius;
							sphereA->GetSphereRadius( radius );

							glm::vec3 pB;
							bodyA->GetPosition( pB );
							glm::vec3 pNormal;
							planeB->GetPlaneNormal( pNormal );
							float pConst;
							planeB->GetPlaneConst( pConst );

							float result;
							glm::vec3 collisionPoint;

							// check for collision
							if( intersect_moving_sphere_plane( pA, radius, bodyA->mVelocity, pNormal, pConst, result, collisionPoint ) )
								return CollideSpherePlane( bodyA, sphereA, bodyB, planeB );
							else
								break;
						}
						break;
					case nPhysics::SHAPE_TYPE_SPHERE:
						{
							cSphereShape* sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );
							//return CollideSphereSphere( bodyA, sphereA, bodyB, sphereB );
						}
						break;
				}
			}
			break;
		case nPhysics::SHAPE_TYPE_PLANE:
			{
				cPlaneShape * planeA = dynamic_cast< cPlaneShape* >( bodyB->GetShape() );
				switch( typeB )
				{
					case nPhysics::SHAPE_TYPE_PLANE:
						break;
					case nPhysics::SHAPE_TYPE_SPHERE:
						{	// do sphere-plane collision 
							cSphereShape* sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );
							//return CollideSpherePlane( bodyB, sphereB, bodyA, planeA );
						}
						break;
				}
			}
			break;
		}			

		return false;
	}


	bool cPhysicsWorld::CollideSpherePlane( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cPlaneShape* planeB )
	{
		// Returns true or false if Sphere is colliding with plane

		glm::vec3 q;
		float t;

		glm::vec3 pNormal;
		planeB->GetPlaneNormal( pNormal );

		//use glm::reflect( velocity, normal )

		bodyA->mVelocity = glm::reflect( bodyA->mVelocity, pNormal );

		//glm::vec3 nComponent = glm::project();
		//	bodyA->mVelocity, pNormal );

		//sphereBody->mVelocity -= nComponent * 0.2f;
		//sphereBody->mPosition = ( c + v * t );

		//mIntegrator.RK4( sphereBody->mPosition, sphereBody->mVeloity, spereBody - mAcceleration, mGravity, mDt*( 1.f - t ) );
		return true;

	}

	bool cPhysicsWorld::CollideSphereSphere( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cSphereShape* sphereB )
	{
		// Returns true or false if Sphere is colliding with sphere

		//Check for collision  return false if not

		//mt = ma + mb;

		//sphereBodyA->mVelocity = ( c*mb*( vb - va ) + ma * va + mb * vb ) / mt;
		//sphereBodyB->mVelocity = ( c*mb*( va - vb ) + mb * vb + ma * va ) / mt;

		//mIntegrator.RK4( sphereBodyA->mPosition, sphereBodyA->mVeloity, sphereBodyA - mAcceleration, mGravity, mDt*( 1.f - t ) );
		//mIntegrator.RK4( sphereBodyB->mPosition, sphereBodyB->mVeloity, sphereBodyB - mAcceleration, mGravity, mDt*( 1.f - t ) );
		//return true;

		return false;

	}



}