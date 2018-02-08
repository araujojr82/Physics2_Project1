#ifndef _MapUtilities_HG_
#define _MapUtilities_HG_

#include "ModelUtilities.h" 
//#include <fstream>
//#include <vector>
#include <chrono>
#include <iomanip> // put_time

#include <sstream>						// "String stream"
#include <glm/vec3.hpp>
//#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>			// glm::value_ptr

#include "cTriangle.h"
#include "cRandomMap.h"

int searchAVertexByPoint( std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices, float x, float y, float z, char floatingAxis );

void divideTriangleInTwo( std::vector<cTriangle> &theNewTriangles,
	std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices,
	int v1pos, int v2pos, int v3pos,
	char floatingAxis, float min, float max );

void createTrianglesFromVertices( std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices,
	std::vector<cTriangle> &meshTriangles,
	int v1pos, int v2pos, int v3pos,
	int divisions, float min, float max,
	char floatingAxis ); // how many divisions will it make

cMesh createMap( int shaderID );
cMesh createCube( int shaderID );

void saveToPlyFile( std::vector<cTriangle> &meshTriangles,
					std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices );


#endif