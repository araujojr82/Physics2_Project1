#include "cPhysicsWorld.h"
#include <algorithm>

#include "CollisionDetection.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 GRAVITY = glm::vec3( 0.0f, 0.0f, -1.0f );

namespace nPhysics
{
	cPhysicsWorld::~cPhysicsWorld()
	{

	}

	//void cPhysicsWorld::TimeStep( float deltaTime )
	//{

	//	std::vector<cRigidBody*>::iterator itRigidBody = mRigidBody.begin();
	//	while( itRigidBody != mRigidBody.end() )
	//	{
	//		cRigidBody* rb = *itRigidBody;

	//		eShapeType shape = rb->GetShape()->GetShapeType();

	//		if( shape == SHAPE_TYPE_PLANE ) break;

	//		////Explicity Euler integration (RK4)
	//		//// New position is based on velocity over time
	//		//rb->mPosition += rb->mVelocity * ( float )deltaTime;
	//		//
	//		//// New velocity is based on acceleration over time
	//		//rb->mVelocity += ( ( float )deltaTime * rb->mAcceleration ) + ( ( float )deltaTime * GRAVITY );			

	//		glm::vec3 acc = GRAVITY;

	//		this->myIntegrator.RK4( rb->mPosition, rb->mVelocity, acc, deltaTime );

	//		itRigidBody++;
	//	}

	//	float* mDt = &deltaTime;

	//	//while itRb
	//	//{
	//	//	Integrate Rigidbodies
	//	//}


	//	int numBodies = mRigidBody.size();

	//	for( int i = 0; i != numBodies; i++ )
	//	{
	//		for( int j = 0; j != numBodies; j++ )
	//		{
	//			if( i == j ) continue; //Don't test for itself

	//			Collide( mRigidBody[i], mRigidBody[j], deltaTime );

	//			////Add colliding pairs
	//			//collidingPairs.push_back( std::make_pair( mRigidBody[iA], mRigidBody[iB] ))
	//		}

	//	}

	//	// clear forces
	//	//just zeroes out the acceleration vector for each rigid body.
	//	itRigidBody = mRigidBody.begin();
	//	while( itRigidBody != mRigidBody.end() )
	//	{
	//		cRigidBody* rb = *itRigidBody;
	//		rb->mAcceleration = glm::vec3( 0.0f );

	//		rb->mPrevPosition = rb->mPosition;

	//		itRigidBody++;
	//	}

	//	////
	//	//if( mCollisionListener )
	//	//{
	//	//	//rip thorugh that list of rigid body pairs
	//	//	//call
	//	//	while( itPair != collidingPairs.end() )
	//	//	{
	//	//		mCollisionListener->HandleCollision( pair.first, pair.second );
	//	//	}

	//	//}

	//}

	void cPhysicsWorld::AddRigidBody( iRigidBody* rigidBody )
	{
		cRigidBody* rb = dynamic_cast< cRigidBody* >( rigidBody );
		if( !rb )
		{
			return;
		}
		std::vector<cRigidBody*>::iterator itRigidBody;
		itRigidBody = std::find( mRigidBody.begin(), mRigidBody.end(), rb );
		if( itRigidBody == mRigidBody.end() )
		{
			mRigidBody.push_back( rb );
		}
	}
	void cPhysicsWorld::RemoveRigidBody( iRigidBody* rigidBody )
	{
		cRigidBody* rb = dynamic_cast< cRigidBody* >( rigidBody );
		if( !rb )
		{
			return;
		}
		std::vector<cRigidBody*>::iterator itRigidBody;

		itRigidBody = std::remove( mRigidBody.begin(), mRigidBody.end(), rb );
		if( itRigidBody != mRigidBody.end() )
		{
			mRigidBody.resize( mRigidBody.size() - 1 );
		}
	}

	bool cPhysicsWorld::Collide( cRigidBody* bodyA, cRigidBody* bodyB, float deltaTime )
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
				intersect_moving_sphere_plane( pA, radius, bodyA->mVelocity, pNormal, pConst, result, collisionPoint );
				if( result == 0 )
				{
					bodyA->SetPosition( bodyA->mPrevPosition );
					return CollideSpherePlane( bodyA, sphereA, bodyB, planeB, deltaTime, deltaTime );
				}
				else
					break;
			}
			break;
			case nPhysics::SHAPE_TYPE_SPHERE:
			{
				glm::vec3 pA;
				bodyA->GetPosition( pA );
				float radiusA;
				sphereA->GetSphereRadius( radiusA );

				cSphereShape* sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );

				glm::vec3 pB;
				bodyB->GetPosition( pB );
				float radiusB;
				sphereB->GetSphereRadius( radiusB );

				float result;

				intersect_moving_sphere_sphere( pA, radiusA, bodyA->mVelocity, pB, radiusB, bodyB->mVelocity, result );
				if( result == 0 )
				{
					bodyA->SetPosition( bodyA->mPrevPosition );
					bodyB->SetPosition( bodyB->mPrevPosition );
					return CollideSphereSphere( bodyA, sphereA, bodyB, sphereB, deltaTime );
				}
				else
					break;
			}
			break;
			}
		}
		break;
		case nPhysics::SHAPE_TYPE_PLANE:
		{
			cPlaneShape * planeA = dynamic_cast< cPlaneShape* >( bodyA->GetShape() );
			switch( typeB )
			{
			case nPhysics::SHAPE_TYPE_PLANE:
				break;
			case nPhysics::SHAPE_TYPE_SPHERE:
			{	// do sphere-plane collision 
				cSphereShape* sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );
				//return CollideSpherePlane( bodyB, sphereB, bodyA, planeA );

				glm::vec3 pB;
				bodyB->GetPosition( pB );
				float radius;
				sphereB->GetSphereRadius( radius );

				glm::vec3 pNormal;
				planeA->GetPlaneNormal( pNormal );
				float pConst;
				planeA->GetPlaneConst( pConst );

				float result;
				glm::vec3 collisionPoint;

				// check for collision
				intersect_moving_sphere_plane( pB, radius, bodyB->mVelocity, pNormal, pConst, result, collisionPoint );
				if( result == 0 )
				{
					bodyB->SetPosition( bodyB->mPrevPosition );
					return CollideSpherePlane( bodyB, sphereB, bodyA, planeA, deltaTime, deltaTime );
				}
				else
					break;
			}
			break;
			}
		}
		break;
		}

		return false;
	}

	bool cPhysicsWorld::CollideSpherePlane( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cPlaneShape* planeB, float t, float deltaTime )
	{
		// Returns true or false if Sphere is colliding with plane

		glm::vec3 q;
		//float t;

		glm::vec3 pNormal;
		planeB->GetPlaneNormal( pNormal );

		//use glm::reflect( velocity, normal )

		bodyA->mVelocity = glm::reflect( bodyA->mVelocity, pNormal );
		bodyA->mVelocity -= bodyA->mVelocity * 0.2f;

		//glm::vec3 nComponent = glm::project();
		//	bodyA->mVelocity, pNormal );

		//bodyA->mVelocity -= nComponent * 0.2f;
		//sphereBody->mPosition = ( c + v * t );

		//mIntegrator.RK4( sphereBody->mPosition, sphereBody->mVeloity, spereBody - mAcceleration, mGravity, mDt*( 1.f - t ) );

		glm::vec3 acc = GRAVITY;
		this->myIntegrator.RK4( bodyA->mPosition, bodyA->mVelocity, acc, t );
		return true;

	}

	bool cPhysicsWorld::CollideSphereSphere( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cSphereShape* sphereB, float t )
	{
		// Returns true or false if Sphere is colliding with sphere

		//Check for collision  return false if not

		bodyA->mPosition = bodyA->mPrevPosition;
		bodyB->mPosition = bodyB->mPrevPosition;

		// move the spheres back to the point of collision
		//bodyA->mPosition = bodyA->mPrevPosition + bodyA->mVelocity * t;
		//bodyB->mPosition = bodyB->mPrevPosition + bodyB->mVelocity * t;

		float massA = bodyA->mMass;
		float massB = bodyB->mMass;
		float totalMass = massA + massB;

		glm::vec3 velA = bodyA->mVelocity;
		glm::vec3 velB = bodyB->mVelocity;
		float cR = 0.2f;

		bodyA->mVelocity = ( cR * massB*( velB - velA ) + massA * velA + massB * velB ) / totalMass;
		bodyB->mVelocity = ( cR * massB*( velA - velB ) + massB * velB + massA * velA ) / totalMass;

		glm::vec3 acc = GRAVITY;

		this->myIntegrator.RK4( bodyA->mPosition, bodyA->mVelocity, acc, t );
		this->myIntegrator.RK4( bodyB->mPosition, bodyB->mVelocity, acc, t );

		//mIntegrator.RK4( sphereBodyA->mPosition, sphereBodyA->mVeloity, sphereBodyA - mAcceleration, mGravity, mDt*( 1.f - t ) );
		//mIntegrator.RK4( sphereBodyB->mPosition, sphereBodyB->mVeloity, sphereBodyB - mAcceleration, mGravity, mDt*( 1.f - t ) );
		//return true;

		return false;

	}

	void cPhysicsWorld::TimeStep( float deltaTime )
	{
		collisionResults.clear();
		vecColliders1.clear();
		vecColliders2.clear();	

		int numBodies = mRigidBody.size();

		for( int i = 0; i != numBodies; i++ )
		{
			for( int j = 0; j != numBodies; j++ )
			{
				if( i == j ) continue; //Don't test for itself

				sCollisionManifold result;
				result = FindCollision( mRigidBody[i], mRigidBody[j] );

				if( result.colliding )
				{
					vecColliders1.push_back( mRigidBody[i] );
					vecColliders2.push_back( mRigidBody[j] );
					collisionResults.push_back( result );
				}
			}
		}

		// Apply impulses to resolve collisions
		for( int k = 0; k < 10; k++ ) { // Apply impulses
			for( int i = 0; i != collisionResults.size(); i++ ) {
				for( int j = 0; j != collisionResults[i].contacts.size(); j++ ) {
					//if( colliders1[i]->HasVolume() && colliders2[i]->HasVolume() ) {
						SolveCollision( vecColliders1[i], vecColliders2[i], collisionResults[i], j );
					//}
				}
			}
		}

		// Integrate velocity and impulse of objects
		for( int i = 0; i != numBodies; i++ ) {

			glm::vec3 force = GRAVITY;
			if( mRigidBody[i]->mMass != 0.0f )
				this->myIntegrator.RK4( mRigidBody[i]->mPosition, mRigidBody[i]->mVelocity, force, deltaTime );
		}

		//// Correct position to avoid sinking!
		//if( DoLinearProjection ) {
		//	for( int i = 0, size = results.size(); i < size; ++i ) {
		//		if( !colliders1[i]->HasVolume() && !colliders2[i]->HasVolume() ) {
		//			continue;
		//		}

		//		RigidbodyVolume* m1 = ( RigidbodyVolume* )colliders1[i];
		//		RigidbodyVolume* m2 = ( RigidbodyVolume* )colliders2[i];
		//		float totalMass = m1->InvMass() + m2->InvMass();

		//		if( totalMass == 0.0f ) {
		//			continue;
		//		}

		//		float depth = fmaxf( results[i].depth - PenetrationSlack, 0.0f );
		//		float scalar = ( totalMass == 0.0f ) ? 0.0f : depth / totalMass;
		//		vec3 correction = results[i].normal * scalar * LinearProjectionPercent;

		//		m1->position = m1->position - correction * m1->InvMass();
		//		m2->position = m2->position + correction * m2->InvMass();

		//		m1->SynchCollisionVolumes();
		//		m2->SynchCollisionVolumes();
		//	}
		//}


	}

	sCollisionManifold cPhysicsWorld::FindCollision( cRigidBody* bodyA, cRigidBody* bodyB )
	{
		sCollisionManifold result;

		eShapeType typeA = bodyA->GetShape()->GetShapeType();
		eShapeType typeB = bodyB->GetShape()->GetShapeType();

		if( typeA == nPhysics::SHAPE_TYPE_SPHERE ) {
			cSphereShape * sphereA = dynamic_cast< cSphereShape* > ( bodyA->GetShape() );

			if( typeB == nPhysics::SHAPE_TYPE_SPHERE ) {
				cSphereShape * sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );

				result = FindColSphereSphere( bodyA, sphereA, bodyB, sphereB );
			}
			else if( typeB == nPhysics::SHAPE_TYPE_PLANE ) {
				cPlaneShape* planeB = dynamic_cast< cPlaneShape* >( bodyB->GetShape() );

				result = FindColSpherePlane( bodyA, sphereA, bodyB, planeB );
				result.normal = result.normal * -1.0f;
			}
		}
		else if( typeA == nPhysics::SHAPE_TYPE_PLANE ) {
			cPlaneShape* planeA = dynamic_cast< cPlaneShape* >( bodyA->GetShape() );
			if( typeB == nPhysics::SHAPE_TYPE_PLANE ) {
				// Do Nothing
			}
			else if( typeB == nPhysics::SHAPE_TYPE_SPHERE ) {
				cSphereShape * sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );
				result = FindColSpherePlane( bodyB, sphereB, bodyA, planeA );
			}
		}

		return result;
	}

	sCollisionManifold cPhysicsWorld::FindColSphereSphere( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cSphereShape* sphereB )
	{
		sCollisionManifold result;

		float radA, radB;
		sphereA->GetSphereRadius( radA );
		sphereB->GetSphereRadius( radB );

		float radii = radA + radB;
		glm::vec3 distance = bodyB->mPosition - bodyA->mPosition;

		if( glm::length2( distance ) - radii * radii > 0 || glm::length2( distance ) == 0.0f ) {
			return result;
		}
		glm::normalize( distance );
		
		result.colliding = true;
		result.normal = distance;
		result.depth = fabsf( glm::length( distance ) - radii ) * 0.5f;

		// Distance to intersection point
		float dtp = radA - result.depth;
		glm::vec3 contact = bodyA->mPosition + distance * dtp;

		result.contacts.push_back( contact );

		return result;
	}

	glm::vec3 ClosestPoint( glm::vec3 planeNormal, float planeConst, glm::vec3 thePoint ) 
	{
		float distance = glm::dot( planeNormal, thePoint ) - planeConst;	
		return thePoint - planeNormal * distance;
	}

	bool CMP( float x, float y )
	{
		return ( fabsf( x - y ) <= FLT_EPSILON * fmaxf( 1.0f, fmaxf( fabsf( x ), fabsf( y ) ) ) );
	}

	sCollisionManifold cPhysicsWorld::FindColSpherePlane( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cPlaneShape* planeB )
	{
		sCollisionManifold result;

		float radA;
		sphereA->GetSphereRadius( radA );

		glm::vec3 pNormal;
		planeB->GetPlaneNormal( pNormal );
		float pConst;
		planeB->GetPlaneConst( pConst );

		glm::vec3 closestPoint = ClosestPoint( pNormal, pConst, bodyA->mPosition );

		float distanceSq = glm::length2( closestPoint - bodyA->mPosition );
		if( distanceSq > radA * radA ) {
			return result;
		}

		glm::vec3 normal;
		if( CMP( distanceSq, 0.0f ) ) {
			if( CMP( glm::length2( closestPoint - bodyB->mPosition ), 0.0f ) ) {
				return result;

			}
			// Closest point is at the center of the sphere
			normal = glm::normalize( closestPoint - bodyB->mPosition );
		}
		else {
			normal = glm::normalize( bodyA->mPosition - closestPoint );
		}

		glm::vec3 outsidePoint = bodyA->mPosition - normal * radA;

		float distance = glm::length( closestPoint - outsidePoint );

		result.colliding = true;
		result.contacts.push_back( closestPoint + ( outsidePoint - closestPoint ) * 0.5f );
		result.normal = normal;
		result.depth = distance * 0.5f;

		return result;
	}

	void cPhysicsWorld::SolveCollision( cRigidBody* bodyA, cRigidBody* bodyB, sCollisionManifold collisionResult, int c )
	{

		float invMass1 = bodyA->mInvMass;
		float invMass2 = bodyB->mInvMass;
		float invMassSum = invMass1 + invMass2;

		if( invMassSum == 0.0f ) {
			return; // Both objects have infinate mass!
		}

		glm::vec3 r1 = collisionResult.contacts[c] - bodyA->mPosition;
		glm::vec3 r2 = collisionResult.contacts[c] - bodyB->mPosition;

		// Relative velocity
		glm::vec3 relativeVel = bodyB->mVelocity - bodyB->mVelocity;

		// Relative collision normal
		glm::vec3 relativeNorm = collisionResult.normal;
		glm::normalize( relativeNorm );

		// Moving away from each other? Do nothing!
		if( glm::dot( relativeVel, relativeNorm ) > 0.0f ) {
			return;
		}

		float e = 0.5f; // fminf( A.cor, B.cor );

		float numerator = ( -( 1.0f + e ) * glm::dot( relativeVel, relativeNorm ) );
		float d1 = invMassSum;

		glm::vec3 d2 = glm::cross( glm::cross( r1, relativeNorm ), r1 );
		glm::vec3 d3 = glm::cross( glm::cross( r2, relativeNorm ), r2 );
		float denominator = d1 + glm::dot( relativeNorm, d2 + d3 );

		float j = ( denominator == 0.0f ) ? 0.0f : numerator / denominator;
		if( collisionResult.contacts.size() > 0 && j != 0.0f ) {
			j /= ( float )collisionResult.contacts.size();
		}

		glm::vec3 impulse = relativeNorm * j;

		bodyA->mVelocity = bodyA->mVelocity - impulse * invMass1;
		bodyB->mVelocity = bodyB->mVelocity - impulse * invMass2;

		return;
	}

}