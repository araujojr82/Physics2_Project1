#pragma once
#ifndef _globalGameStuff_HG_
#define _globalGameStuff_HG_

// This is all the global 'game' stuff, so:
// - not OpenGL rendering stuff (unless we can't help it)
// - just 'engine' or game play stuff

#include "cGameObject.h"
#include <vector>
#include <glm/vec3.hpp>
#include "cLightManager.h"
//#include "cModelAssetLoader.h"
//#include "cCamera.h"
#include "cMouseCamera.h"

// Remember to #include <vector>...
extern std::vector< cGameObject* >  g_vecGameObjects;
extern int g_selectedSphere;

extern bool bIsWireframe;

// Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName( std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects );
cGameObject* findObjectByUniqueID( unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects );


//extern glm::vec3 g_cameraXYZ;				// (theMain.cpp)
//extern glm::vec3 g_cameraTarget_XYZ;		// (theMain.cpp)


extern cLightManager*	g_pLightManager;	// (theMain.cpp)

											// Super basic physics update function
//void PhysicsStep( double deltaTime );

//extern cModelAssetLoader* g_pModelAssetLoader;	// (ModelUtilies.cpp)

												// The teapots
												//extern cGameObject* pTP0;
const std::string THEBALLNAME = "ball";			// Now that we have a lookup

extern cMouseCamera* g_pTheMouseCamera;		// (theMain.cpp)

#endif
