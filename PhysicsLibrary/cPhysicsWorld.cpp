#include "cPhysicsWorld.h"
#include <algorithm>

#include "CollisionDetection.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 GRAVITY = glm::vec3( 0.0f, 0.0f, -9.82f );

namespace nPhysics
{
	cPhysicsWorld::~cPhysicsWorld()
	{

	}
	
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
					return CollideSpherePlane( bodyA, sphereA, bodyB, planeB );
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
					return CollideSphereSphere( bodyA, sphereA, bodyB, sphereB );
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
					return CollideSpherePlane( bodyB, sphereB, bodyA, planeA );
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

	//void cPhysicsWorld::SolveCollision( cRigidBody* bodyA, cRigidBody* bodyB, sCollisionManifold collisionResult, int c )
	//{
	//	eShapeType typeA = bodyA->GetShape()->GetShapeType();
	//	eShapeType typeB = bodyB->GetShape()->GetShapeType();

	//	if( typeA == nPhysics::SHAPE_TYPE_SPHERE ) {
	//		cSphereShape * sphereA = dynamic_cast< cSphereShape* > ( bodyA->GetShape() );

	//		if( typeB == nPhysics::SHAPE_TYPE_SPHERE ) {
	//			cSphereShape * sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );

	//			CollideSphereSphere( bodyA, sphereA, bodyB, sphereB );
	//		}
	//		else if( typeB == nPhysics::SHAPE_TYPE_PLANE ) {
	//			cPlaneShape* planeB = dynamic_cast< cPlaneShape* >( bodyB->GetShape() );

	//			CollideSpherePlane( bodyA, sphereA, bodyB, planeB );
	//		}
	//	}
	//	else if( typeA == nPhysics::SHAPE_TYPE_PLANE ) {
	//		cPlaneShape* planeA = dynamic_cast< cPlaneShape* >( bodyA->GetShape() );
	//		if( typeB == nPhysics::SHAPE_TYPE_PLANE ) {
	//			 Do Nothing
	//		}
	//		else if( typeB == nPhysics::SHAPE_TYPE_SPHERE ) {
	//			cSphereShape * sphereB = dynamic_cast< cSphereShape* > ( bodyB->GetShape() );
	//			CollideSpherePlane( bodyB, sphereB, bodyA, planeA );
	//		}
	//	}
	//	return;
	//}

	bool cPhysicsWorld::CollideSpherePlane( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cPlaneShape* planeB )
	{		
		//bodyA->mPosition = bodyA->mPrevPosition;

		glm::vec3 pNormal;
		planeB->GetPlaneNormal( pNormal );
		
		bodyA->mVelocity = glm::reflect( bodyA->mVelocity, pNormal );
		bodyA->mVelocity -= bodyA->mVelocity * 0.2f;

		return true;
	}

	bool cPhysicsWorld::CollideSphereSphere( cRigidBody* bodyA, cSphereShape* sphereA, cRigidBody* bodyB, cSphereShape* sphereB )
	{
		float massA = bodyA->mMass;
		float massB = bodyB->mMass;
		float totalMass = massA + massB;

		//bodyA->mPosition = bodyA->mPrevPosition;
		//bodyB->mPosition = bodyB->mPrevPosition;

		glm::vec3 velA = bodyA->mVelocity;
		glm::vec3 velB = bodyB->mVelocity;
		float cR = 0.2f;

		bodyA->mVelocity = ( cR * massB*( velB - velA ) + massA * velA + massB * velB ) / totalMass;
		bodyB->mVelocity = ( cR * massB*( velA - velB ) + massB * velB + massA * velA ) / totalMass;

		return true;
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

		// Apply forces
		for( int i = 0; i != numBodies; i++ )
		{
			cRigidBody* theBody = mRigidBody[i];
			glm::vec3 force = GRAVITY;
			eShapeType type = theBody->GetShape()->GetShapeType();
			if( type != nPhysics::SHAPE_TYPE_PLANE )
				theBody->ApplyForce( force );
		}

		// Apply impulses to resolve collisions
		for( int k = 0; k < 10; k++ ) 
		{ 
			for( int i = 0; i != collisionResults.size(); i++ ) 
			{
				for( int j = 0; j != collisionResults[i].contacts.size(); j++ ) 
				{
						SolveCollision( vecColliders1[i], vecColliders2[i], collisionResults[i], j );
				}
			}
		}

		// Integrate velocity and impulse of objects
		for( int i = 0; i != numBodies; i++ )
		{
			cRigidBody* theBody = mRigidBody[i];
			glm::vec3 force = theBody->mAcceleration;					
			glm::vec3 acceleration = force * theBody->mInvMass;

			eShapeType type = mRigidBody[i]->GetShape()->GetShapeType();
			if( type != nPhysics::SHAPE_TYPE_PLANE )
				this->myIntegrator.RK4( mRigidBody[i]->mPosition, mRigidBody[i]->mVelocity, acceleration, deltaTime );
				//this->myIntegrator.Euler( mRigidBody[i]->mPosition, mRigidBody[i]->mVelocity, acceleration, deltaTime );
		}

		// Correct position to avoid sinking!
		for( int i = 0; i != collisionResults.size(); i++ )
		{
			cRigidBody* m1 = vecColliders1[i];
			cRigidBody* m2 = vecColliders2[i];
			float totalMass = m1->mInvMass + m2->mInvMass;

			eShapeType type1 = m1->GetShape()->GetShapeType();
			eShapeType type2 = m2->GetShape()->GetShapeType();

			float LinearProjectionPercent = 0.8f;
			float PenetrationSlack = 0.01f;
			
			float depth = fmaxf( collisionResults[i].depth - PenetrationSlack, 0.0f );
			float scalar = ( totalMass == 0.0f ) ? 0.0f : depth / totalMass;
			glm::vec3 correction = collisionResults[i].normal * scalar * LinearProjectionPercent;

			m1->mPosition = m1->mPosition - correction * m1->mInvMass;
			m2->mPosition = m2->mPosition + correction * m2->mInvMass;
	
		}

		int x = 0;
		// Clear acceleration
		for( int i = 0; i != numBodies; i++ ) 
		{
			cRigidBody* theBody = mRigidBody[i];
			theBody->mAcceleration = glm::vec3( 0.0f );	

			x++;
		}

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
		//result.depth = distance;

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
		glm::vec3 relativeVel = bodyB->mVelocity - bodyA->mVelocity;

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

		eShapeType typeA = bodyA->GetShape()->GetShapeType();
		eShapeType typeB = bodyB->GetShape()->GetShapeType();

		if( typeA != nPhysics::SHAPE_TYPE_PLANE )
			bodyA->mVelocity = bodyA->mVelocity - impulse * invMass1;
		
		if( typeB != nPhysics::SHAPE_TYPE_PLANE )
			bodyB->mVelocity = bodyB->mVelocity + impulse * invMass2;

		return;
	}

}