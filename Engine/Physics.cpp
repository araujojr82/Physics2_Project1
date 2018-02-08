#include "Physics.h"
#include "cGameObject.h"
#include "cTriangle.h"

#include <glm/glm.hpp>		// glm::distance

//bool PenetrationTestSphereSphere( cGameObject* pA, cGameObject* pB )
//{
//	// If the distance between the two sphere is LT the sum or the radii,
//	//	they are touching or penetrating
//
//	float totalRadii = pA->radius + pB->radius;
//
//	// The Pythagorean distance 
//	float distance = glm::distance( pA->position, pB->position );
//
//	if ( distance <= totalRadii )
//	{
//		return true;
//	}
//
//	return false;
//}
//
//glm::vec3 sTriAABB::ClosestPtPointTriangle( glm::vec3 p )
//{
//	return this->ClosestPtPointTriangle( p, this->verts[0], this->verts[1], this->verts[2] );
//}
//
//glm::vec3 sTriAABB::ClosestPtPointTriangle( glm::vec3 p, glm::vec3 a,
//	glm::vec3 b, glm::vec3 c )
//{
//	// Check if P in vertex region outside A
//	glm::vec3 ab = b - a;
//	glm::vec3 ac = c - a;
//	glm::vec3 ap = p - a;
//	float d1 = glm::dot( ab, ap );
//	float d2 = glm::dot( ac, ap );
//	if( d1 <= 0.0f && d2 <= 0.0f ) return a; // barycentric coordinates (1,0,0)
//
//											 // Check if P in vertex region outside B
//	glm::vec3 bp = p - b;
//	float d3 = glm::dot( ab, bp );
//	float d4 = glm::dot( ac, bp );
//	if( d3 >= 0.0f && d4 <= d3 ) return b; // barycentric coordinates (0,1,0)
//
//										   // Check if P in edge region of AB, if so return projection of P onto AB
//	float vc = d1*d4 - d3*d2;
//	if( vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f ) {
//		float v = d1 / ( d1 - d3 );
//		return a + v * ab; // barycentric coordinates (1-v,v,0)
//	}
//
//	// Check if P in vertex region outside C
//	glm::vec3 cp = p - c;
//	float d5 = glm::dot( ab, cp );
//	float d6 = glm::dot( ac, cp );
//	if( d6 >= 0.0f && d5 <= d6 ) return c; // barycentric coordinates (0,0,1)
//
//										   // Check if P in edge region of AC, if so return projection of P onto AC
//	float vb = d5*d2 - d1*d6;
//	if( vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f ) {
//		float w = d2 / ( d2 - d6 );
//		return a + w * ac; // barycentric coordinates (1-w,0,w)
//	}
//
//	// Check if P in edge region of BC, if so return projection of P onto BC
//	float va = d3*d6 - d5*d4;
//	if( va <= 0.0f && ( d4 - d3 ) >= 0.0f && ( d5 - d6 ) >= 0.0f ) {
//		float w = ( d4 - d3 ) / ( ( d4 - d3 ) + ( d5 - d6 ) );
//		return b + w * ( c - b ); // barycentric coordinates (0,1-w,w)
//	}
//
//	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
//	float denom = 1.0f / ( va + vb + vc );
//	float v = vb * denom;
//	float w = vc * denom;
//	return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0f - v - w
//}
//
//void bounceSphereAgainstPlane( cGameObject* pA, cGameObject* pB, glm::vec3 tNormal )
//{
//	glm::vec3 incidentVector = pA->position - pA->prevPosition;
//	//glm::vec3 incidentVector = glm::normalize( pA->vel );
//
//	glm::vec3 newAvel = glm::reflect( incidentVector, tNormal );
//
//	//glm::normalize( newAvel );
//
//	pA->vel = newAvel;
//
//	pA->vel.z = 0.0f;
//
//	//float invMassSum = pA->inverseMass + pB->inverseMass;
//
//	////if( invMassSum == 0.0f ) {
//	////	return; // Both objects have infinate mass!
//	////}
//
//	//// Relative vel
//	//glm::vec3 relativeVel = pB->vel - pA->vel;
//
//	//// Relative collision normal
//	//glm::vec3 relativeNorm = tNormal;
//
//	////relativeNorm.x *= -1.0f;
//	////relativeNorm.y *= -1.0f;
//	////relativeNorm.z *= -1.0f;
//
//	////if( tNormal.x > 0.0f ) relativeNorm.x = 1.0f;
//	////if( tNormal.y > 0.0f ) relativeNorm.y = 1.0f;
//	////if( tNormal.z > 0.0f ) relativeNorm.z = 1.0f;
//
//	////if( tNormal.x < -0.0f ) relativeNorm.x = -1.0f;
//	////if( tNormal.y < -0.0f ) relativeNorm.y = -1.0f;
//	////if( tNormal.z < -0.0f ) relativeNorm.z = -1.0f;
//
//	////glm::normalize( relativeNorm );
//
//	//float result = glm::dot( relativeVel, relativeNorm );
//	//
//	//// Moving away from each other? Do nothing!
//	//if( result > 0.0f ) {
//	//	return;
//	//}
//
//	////float e = fminf( pA->cor, pB->cor );
//	//float e = fminf( 1.0f, 1.0f ); // Perfect ellastic collision have COR = 1.0
//
//	//float numerator = ( -( 1.0f + e ) * glm::dot( relativeVel, relativeNorm ) );
//	//float d1 = invMassSum;
//
//	//float denominator = d1;
//
//	//float j = ( denominator == 0.0f ) ? 0.0f : numerator / denominator;
//
//	////if( M.contacts.size() > 0.0f && j != 0.0f ) {
//	////	j /= ( float )M.contacts.size();
//	////}
//
//	//glm::vec3 impulse = relativeNorm * j;
//	//glm::vec3 newAvel = pA->vel - impulse * pA->inverseMass;
//	//glm::vec3 newBvel = pB->vel + impulse * pB->inverseMass;	// THIS SHOULD BE ZERO FOR STATIC OBJECTS (INFINITY MASS)
//
//	//pA->vel = newAvel;
//	//pB->vel = newBvel;
//
//	////HACK zero Z velocity
//	//pA->vel.z = 0.0f;
//
//	////Update the movement angles
//	////pA->angle = calculateMovAngle( pA->vel );
//	////pB->angle = calculateMovAngle( pB->vel );
//}
//
//float calculateMovAngle( glm::vec3 vel )
//{
//	float movAngle = 0.0f;
//
//	// Calculate the moving angle using the velocity
//	movAngle = atan2( vel.x, vel.z );
//
//	// Convert it from radians to degrees
//	movAngle = ( movAngle * 180 ) / M_PI;
//
//	return movAngle;
//}
//
//glm::vec3 returnNormal( glm::vec3 vertex[3] ){
//	glm::vec3 vertA = vertex[0];
//	glm::vec3 vertB = vertex[1];
//	glm::vec3 vertC = vertex[2];
//
//	glm::vec3 theNormal = glm::vec3( 0.0f );
//
//	glm::vec3 vectorU = ( vertB - vertA );
//	glm::vec3 vectorV = ( vertC - vertA );
//
//	theNormal.x = ( vectorU.y * vectorV.z ) - ( vectorU.z * vectorV.y );
//	theNormal.y = ( vectorU.z * vectorV.x ) - ( vectorU.x * vectorV.z );
//	theNormal.z = ( vectorU.x * vectorV.y ) - ( vectorU.y * vectorV.x );
//
//	return theNormal;
//}
//
//glm::vec3 returnNormal2( glm::vec3 vertex[3], glm::vec3 centerPoint ){
//	glm::vec3 vertA = vertex[0];
//	glm::vec3 vertB = vertex[1];
//	glm::vec3 vertC = vertex[2];
//
//	glm::vec3 theNormal = glm::vec3( 0.0f );
//
//	theNormal = glm::normalize( glm::cross( vertA - vertB, vertA - vertC ) );	
//
//	theNormal /= 5;
//	
//	// Adjust the Normal to the central point of the Triangle
//	// by summing the center to the normal.
//	theNormal += centerPoint;
//
//	return theNormal;
//}
//
//std::vector< pointTriangles > findClosestPointsOfAABB( glm::vec3 thePoint, cAABBv2* theAABB )
//{
//	std::vector< pointTriangles > newVecPoints;
//
//	cAABBv2* theCurAABB = theAABB;
//
//	int numberOfTriangles = theCurAABB->vecTriangles.size();
//
//	//std::fill( g_vecPoints.begin(), g_vecPoints.end(), glm::vec3( NULL ) );
//	newVecPoints.clear();
//
//	for( int triIndex = 0; triIndex != numberOfTriangles; triIndex++ )
//	{
//		// Get reference object for current triangle
//		// (so the line below isn't huge long...)
//		sTriAABB curTriangle = theCurAABB->vecTriangles[triIndex];
//
//		//cPhysTriangle& curTriangle = theAABB.vecTriangles[triIndex];
//
//		pointTriangles theClosestPoint;
//		theClosestPoint.point = curTriangle.ClosestPtPointTriangle( thePoint );
//		theClosestPoint.triangle = curTriangle;
//
//		newVecPoints.push_back( theClosestPoint );
//
//		//DrawDebugSphere( theClosestPoint.point, glm::vec4( 1, 1, 1, 1 ), 0.2f );
//	}
//
//	//collisionThingy:
//	//	- Which objects collided (sphere - triangle/sphere - sphere)
//	//	- Where, the speed, etc.
//
//
//	//	vecMyColisions.push_Back( collisionThingy );
//	// Calc response...
//	//		glm::reflect( 
//
//	return newVecPoints;
//}