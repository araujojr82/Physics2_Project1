#include "ModelUtilities.h" 
#include "globalGameStuff.h"

// Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName( std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects )
{
	// Linear search, baby!
	unsigned int numObjects = ( unsigned int )vec_pGameObjects.size();
	for( unsigned int index = 0; index != numObjects; index++ )
	{
		if( vec_pGameObjects[index]->friendlyName == friendlyName )
		{
			return vec_pGameObjects[index];
		}
	}
	// Didn't find it
	return NULL;
}

cGameObject* findObjectByUniqueID( unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects )
{
	// Linear search, baby!
	unsigned int numObjects = ( unsigned int )vec_pGameObjects.size();
	for( unsigned int index = 0; index != numObjects; index++ )
	{
		if( ID = vec_pGameObjects[index]->getUniqueID() )
		{
			return vec_pGameObjects[index];
		}
	}
	// Didn't find it
	return NULL;
}

void ReadFileToToken( std::ifstream &file, std::string token )
{
	bool bKeepReading = true;
	std::string garbage;
	do
	{
		file >> garbage;		// Title_End??
		if ( garbage == token )
		{
			return;
		}
	}while( bKeepReading );
	return;
}

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMesh( std::string filename, cMesh &theMesh )
{
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	std::ifstream plyFile( filename.c_str() );

	if ( ! plyFile.is_open() )
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	ReadFileToToken( plyFile, "vertex" );
//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken( plyFile, "face" );
	//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;

	ReadFileToToken( plyFile, "end_header" );

	// Allocate the appropriate sized array (+a little bit)
	theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt[theMesh.numberOfVertices];
	
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for ( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 		
		float x, y, z, nx, ny, nz, confidence, intensity;

		plyFile >> x;
		plyFile >> y;
		plyFile >> z;
		plyFile >> nx >> ny >> nz;
//		plyFile >> confidence;
//		plyFile >> intensity;

		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z; 
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
		theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;
	}

	// Load the triangle (or face) information, too
	for ( int count = 0; count < theMesh.numberOfTriangles; count++ )
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

//	theMesh.CalculateNormals();

	return true;
}

// Takes a file name, loads a mesh
bool LoadPlyFileIntoMeshWithUV( std::string filename, cMesh &theMesh )
{
	// Load the vertices
	// c_str() changes a string to a "c style char* string"
	//std::string fileWithPath = this->m_basePath + filename;
	//std::ifstream plyFile( fileWithPath.c_str() );
	std::ifstream plyFile( filename.c_str() );

	if( !plyFile.is_open() )
	{	// Didn't open file, so return
		return false;
	}
	// File is open, let's read it

	ReadFileToToken( plyFile, "vertex" );
	//	int numVertices = 0;
	plyFile >> theMesh.numberOfVertices;

	ReadFileToToken( plyFile, "face" );
	//	int numTriangles = 0;
	plyFile >> theMesh.numberOfTriangles;

	ReadFileToToken( plyFile, "end_header" );

	// Allocate the appropriate sized array (+a little bit)
	//theMesh.pVertices = new cVertex_xyz_rgb_n[theMesh.numberOfVertices];
	theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt[theMesh.numberOfVertices];
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		//end_header
		//-0.0312216 0.126304 0.00514924 0.850855 0.5 		
		float x, y, z, nx, ny, nz;
		// Added
		float u, v;		// Model now has texture coordinate

						// Typical vertex is now... 
						// 29.3068 -5e-006 24.3079 -0.949597 0.1875 -0.251216 0.684492 0.5

		plyFile >> x >> y >> z;
		plyFile >> nx >> ny >> nz;
		// 
		plyFile >> u >> v;			// ADDED


		theMesh.pVertices[index].x = x;	// vertices[index].x = x;
		theMesh.pVertices[index].y = y;	// vertices[index].y = y;
		theMesh.pVertices[index].z = z;
		theMesh.pVertices[index].r = 1.0f;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].g = 1.0f;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].b = 1.0f;	// vertices[index].r = 1.0f;
		theMesh.pVertices[index].nx = nx;	// vertices[index].g = 1.0f;
		theMesh.pVertices[index].ny = ny;	// vertices[index].b = 1.0f;
		theMesh.pVertices[index].nz = nz;	// vertices[index].r = 1.0f;

											// Only store the 1st UV.
		theMesh.pVertices[index].u1 = u;
		theMesh.pVertices[index].v1 = v;
	}

	// Load the triangle (or face) information, too
	for( int count = 0; count < theMesh.numberOfTriangles; count++ )
	{
		// 3 164 94 98 
		int discard = 0;
		plyFile >> discard;									// 3
		plyFile >> theMesh.pTriangles[count].vertex_ID_0;	// 164
		plyFile >> theMesh.pTriangles[count].vertex_ID_1;	// 94
		plyFile >> theMesh.pTriangles[count].vertex_ID_2;	// 98
	}

	theMesh.CalculateNormals2();

	return true;
}

bool CreateMeshFromVectors( cMesh &theMesh, std::vector<sVertex_xyz_rgba_n_uv2_bt> meshVertices, std::vector<cTriangle> meshTriangles )
{
	theMesh.numberOfVertices = meshVertices.size();
	theMesh.numberOfTriangles = meshTriangles.size();

	// Allocate the appropriate sized array (+a little bit)
	theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt[theMesh.numberOfVertices];
	
	theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

	// Read vertices
	for( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		theMesh.pVertices[index].x = meshVertices[index].x;
		theMesh.pVertices[index].y = meshVertices[index].y;
		theMesh.pVertices[index].z = meshVertices[index].z;
		theMesh.pVertices[index].r = 1.0f;
		theMesh.pVertices[index].g = 1.0f;
		theMesh.pVertices[index].b = 1.0f;
		theMesh.pVertices[index].nx = meshVertices[index].nx;
		theMesh.pVertices[index].ny = meshVertices[index].ny;
		theMesh.pVertices[index].nz = meshVertices[index].nz;
		theMesh.pVertices[index].u1 = meshVertices[index].u1;
		theMesh.pVertices[index].v1 = meshVertices[index].v1;
	}

	for( int count = 0; count < theMesh.numberOfTriangles; count++ )
	{
		theMesh.pTriangles[count].vertex_ID_0 = meshTriangles[count].vertex_ID_0;
		theMesh.pTriangles[count].vertex_ID_1 = meshTriangles[count].vertex_ID_1;
		theMesh.pTriangles[count].vertex_ID_2 = meshTriangles[count].vertex_ID_2;
	}

	theMesh.CalculateNormals2();

	//theMesh.CalculateUV();

	return true;

}