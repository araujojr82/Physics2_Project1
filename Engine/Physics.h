#ifndef _Physics_HG_
#define _Physics_HG_

#include "Physics\cAABBBroadPhase.h"
#include "Physics\cAABBv2.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>

struct pointTriangles
{
	glm::vec3 point;
	sTriAABB triangle;
};

//#include "cGameObject.h"
class cGameObject;

// Sphere-Sphere
// Sphere-Plane
// Sphere-Triangle
// Triangle-Triangle
// Mesh-Mesh
// Sphere-Mesh

// Our objects are vectors of pointers, so we might as well pass pointers
bool PenetrationTestSphereSphere( cGameObject* pA, cGameObject* pB );

void bounceSphereAgainstPlane( cGameObject* pA, cGameObject* pB, glm::vec3 tNormal );

float calculateMovAngle( glm::vec3 vel );

glm::vec3 returnNormal( glm::vec3 vertex[3] );
glm::vec3 returnNormal2( glm::vec3 vertex[3], glm::vec3 centerPoint );

std::vector< pointTriangles > findClosestPointsOfAABB( glm::vec3 thePoint, cAABBv2* theAABB );

#endif