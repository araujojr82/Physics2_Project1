#include "cAABBBroadPhase.h"

#include <math.h>				/* floor */
#include <algorithm>			// for the std::copy

cAABBBroadPhase::cAABBBroadPhase()
{
	this->pDebugRenderer = NULL;
	return;
}

cAABBBroadPhase::~cAABBBroadPhase()
{
	return;
}

bool cAABBBroadPhase::genAABBGridFromMesh(cMesh &theMesh)
{
	// Assume that the debug renderer is valid
	//this->pDebugRenderer->addTriangle( drTri(glm::vec3(0.0f), glm::vec3(10.0f), glm::vec3(10.0f), glm::vec3(1.0f)) );
	
	cAABBv2* theNewABBB; // = new cAABBv2( glm::vec3( 0.0f ), 1.0f );

	for( int triIndex = 0; triIndex != theMesh.numberOfTriangles; triIndex++ )
	{
		int vertexID0 = theMesh.pTriangles[triIndex].vertex_ID_0;
		int vertexID1 = theMesh.pTriangles[triIndex].vertex_ID_1;
		int vertexID2 = theMesh.pTriangles[triIndex].vertex_ID_2;
		glm::vec3 AABBPos;

		glm::vec3 vertexPos0 = glm::vec3( theMesh.pVertices[vertexID0].x,
			theMesh.pVertices[vertexID0].y,
			theMesh.pVertices[vertexID0].z );

		glm::vec3 vertexPos1 = glm::vec3( theMesh.pVertices[vertexID1].x,
			theMesh.pVertices[vertexID1].y,
			theMesh.pVertices[vertexID1].z );

		glm::vec3 vertexPos2 = glm::vec3( theMesh.pVertices[vertexID2].x,
			theMesh.pVertices[vertexID2].y,
			theMesh.pVertices[vertexID2].z );

		for( int vertIndex = 0; vertIndex != 3; vertIndex++ )
		{
			glm::vec3 vertexPos;
			switch( vertIndex )
			{
			case 0: vertexPos = vertexPos0; break;
			case 1: vertexPos = vertexPos1; break;
			case 2: vertexPos = vertexPos2; break;
			}

			AABBPos = glm::vec3( floor( vertexPos.x ),
								 floor( vertexPos.y ),
								 floor( vertexPos.z ) );

			theNewABBB = new cAABBv2( AABBPos, 1.0f );

			unsigned long long vertexAABBID = cAABBv2::calculateID( vertexPos );
			if( areThereTrianglesInAABB( vertexPos, theNewABBB ) )
			{
				sTriAABB theTriAABB = sTriAABB( vertexPos0, vertexPos1, vertexPos2 );
				theNewABBB->vecTriangles.push_back( theTriAABB );
				break;
			}
			else
			{
				// Add the new AABB to the map of AABBs...
				this->mapIDToAABB[theNewABBB->getID()] = theNewABBB;
				sTriAABB theTriAABB = sTriAABB( vertexPos0, vertexPos1, vertexPos2 );
				theNewABBB->vecTriangles.push_back( theTriAABB );
				break;
			}		

		}
	}
	return true;
}

bool cAABBBroadPhase::areThereTrianglesInAABB(glm::vec3 testPoint, cAABBv2* &ptheAABB)
{
	// Generate an AABB ID for this test location
	unsigned long long testPointAABBID = cAABBv2::calculateID(testPoint);

	// Find an AABB IF it exists
	std::map< unsigned long long, cAABBv2* >::iterator itAABB = 
				this->mapIDToAABB.find( testPointAABBID );

	// Does it exist? 
	if (itAABB == this->mapIDToAABB.end())
	{
		// NOPE. There are NO AABBs at that location
		return false;
	}
	// Otherwise there IS an AABB
	ptheAABB = itAABB->second;
	return true;
}


