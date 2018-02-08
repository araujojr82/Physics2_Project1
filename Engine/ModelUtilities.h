#ifndef _ModelUtilities_HG_
#define _ModelUtilities_HG_

#include <fstream>
#include <string>
#include <vector>
#include "cMesh.h"

// Only "function signature"
void ReadFileToToken( std::ifstream &file, std::string token );

// Note the & symbol which is "by reference" which means
//	it's NOT a copy, but referring to the original object.
// ALSO note that this is a C++ thing (& isn't a thing in C)
bool LoadPlyFileIntoMesh( std::string filename, cMesh &theMesh );
bool LoadPlyFileIntoMeshWithUV( std::string filename, cMesh &theMesh );

bool CreateMeshFromVectors( cMesh &theMesh, std::vector<sVertex_xyz_rgba_n_uv2_bt> meshVertices, std::vector<cTriangle> meshTriangles );

#endif
