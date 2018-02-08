#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include "../PhysicsInterfaces/iRigidBody.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/quaternion.hpp>

//enum eTypeOfObject
//{	// "Ok people, here's the deal:"
//	SPHERE = 0,		// - it's a sphere
//	PLANE = 1,		// - it's a plane
//	CAPSULE = 2,    // - it's a capsule
//	AABB = 3,		// 3- it's an axis-aligned box
//	UNKNOWN = 99	// I don't know
//};

class cGameObject
{
public:
	cGameObject();		// constructor
	~cGameObject();		// destructor

	nPhysics::iRigidBody* rigidBody;

	//glm::vec3 position;
	//glm::vec3 prevPosition;

	// Now orientation 
	void overwrtiteQOrientationFormEuler( glm::vec3 eulerAxisOrientation );
	// NOTE: Use THIS, not just setting the values
	void adjustQOrientationFormDeltaEuler( glm::vec3 eulerAxisOrientChange );
	glm::quat qOrientation;

	float scale;

	// **********************************
	// Add some physics things
	//bool bIsUpdatedInPhysics;	
	//glm::vec3 vel;			// Velocity
	//glm::vec3 accel;		// Acceleration
	//float angle;	
	//float mass;		// INFINITY				// 
	//float inverseMass;	// = 0.0f	// 
	//// Refers to the enum above
	//eTypeOfObject typeOfObject;		// (really an int)
	//float radius;

	// **********************************
	// Lightn
	bool bIsLight;		// is this object a light representation?
	int myLight;		// which light is this?

	bool bIsWireFrame;

	glm::vec4 diffuseColour;	//  The actual color when lights hit it

	std::string meshName;		// mesh I'd  like to draw

	// The texture of this object
	static const unsigned int NUMTEXTURES = 10;
	std::string textureNames[NUMTEXTURES];
	float textureBlend[NUMTEXTURES];			// 0 - 1

	std::string friendlyName;
	inline unsigned int getUniqueID( void ) { return this->m_UniqueID; }

private:
	unsigned int m_UniqueID;
	// Used when creating objects
	static unsigned int m_nextUniqueID;
	
};

#endif
