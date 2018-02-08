#include "cVAOMeshManager.h"

#include "globalOpenGL_GLFW.h"	// glad (gl.h) and GLFW
//#include <glad/glad.h>		// For all the OpenGL calls
//#include <GLFW/glfw3.h>		// For all the OpenGL calls


#include "cMesh.h"	// NOTE: This is in the CPP file, NOT the .h file

//// The vertex layout as defined by the shader
//// Where's the Best Place to put this?? 
//// (good question)

#include "sVertex_xyz_rgba_n_uv2_bt.h"			

cVAOMeshManager::cVAOMeshManager()
{

	return;
}

cVAOMeshManager::~cVAOMeshManager()
{

	return;
}

bool cVAOMeshManager::loadMeshIntoVAO( cMesh &theMesh, int shaderID, bool bKeepMesh /*=false*/ )
{
	// ********************************************************************
	//        ___  __  ___  ___         __        ___ ___  ___  __
	//	\  / |__  |__)  |  |__  \_/    |__) |  | |__ | __ | __ |__)
	//	 \/  |___ |  \  |  |___ / \    |__) \__/ |   |    |___ |  \
	//
	// ********************************************************************

	sVAOInfo theVAOInfo;

	// Create a Vertex Array Object (VAO)
	glGenVertexArrays( 1, &( theVAOInfo.VAO_ID ) );
	glBindVertexArray( theVAOInfo.VAO_ID );


	glGenBuffers( 1, &( theVAOInfo.vertex_buffer_ID ) );
	glBindBuffer( GL_ARRAY_BUFFER, theVAOInfo.vertex_buffer_ID );

	// Allocate the global vertex array
	//cVertex* pVertices = new cVertex[theMesh.numberOfVertices];
	sVertex_xyz_rgba_n_uv2_bt* pVertices = new sVertex_xyz_rgba_n_uv2_bt[theMesh.numberOfVertices];

	for( int index = 0; index < theMesh.numberOfVertices; index++ )
	{
		pVertices[index].x = theMesh.pVertices[index].x;
		pVertices[index].y = theMesh.pVertices[index].y;
		pVertices[index].z = theMesh.pVertices[index].z;

		pVertices[index].r = theMesh.pVertices[index].r;
		pVertices[index].g = theMesh.pVertices[index].g;
		pVertices[index].b = theMesh.pVertices[index].b;
		pVertices[index].a = theMesh.pVertices[index].a;

		pVertices[index].nx = theMesh.pVertices[index].nx;
		pVertices[index].ny = theMesh.pVertices[index].ny;
		pVertices[index].nz = theMesh.pVertices[index].nz;

		// Other additions: texture coords, tangent, and bi-normal
		pVertices[index].u1 = theMesh.pVertices[index].u1;
		pVertices[index].v1 = theMesh.pVertices[index].v1;
		pVertices[index].u2 = theMesh.pVertices[index].u2;
		pVertices[index].v2 = theMesh.pVertices[index].v2;

		pVertices[index].bx = theMesh.pVertices[index].bx;
		pVertices[index].by = theMesh.pVertices[index].by;
		pVertices[index].bz = theMesh.pVertices[index].bz;

		pVertices[index].tx = theMesh.pVertices[index].tx;
		pVertices[index].ty = theMesh.pVertices[index].ty;
		pVertices[index].tz = theMesh.pVertices[index].tz;

		// Or, since these are the same, you could simply assign one to the other...
		//		pVertices[index] = theMesh.pVertices[index];
	}

	// Copy the local vertex array into the GPUs memory
	//int sizeOfGlobalVertexArrayInBytes = sizeof(cVertex) * theMesh.numberOfVertices;
	int sizeOfGlobalVertexArrayInBytes = sizeof( sVertex_xyz_rgba_n_uv2_bt ) * theMesh.numberOfVertices;
	glBufferData( GL_ARRAY_BUFFER,
		sizeOfGlobalVertexArrayInBytes,		// sizeof(vertices), 
		pVertices,
		GL_STATIC_DRAW );

	// Get rid of local vertex array
	delete[] pVertices;
	// ******************************************************************************
	
	glGenBuffers( 1, &( theVAOInfo.index_buffer_ID ) );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, theVAOInfo.index_buffer_ID ); // It's now an Index Buffer

																		 // Make a temporary array that matches what will be on the GPU...
																		 // (GPU expects a 1D array of integers)

																		 // Number of indices = number of tris x 3 (3 indices per triangle)
	int numberOfIndices = theMesh.numberOfTriangles * 3;

	unsigned int* indexArray = new unsigned int[numberOfIndices];

	// Copy the triangle data into this 1D array 
	int triIndex = 0;		// Index into the triangle array (from mesh)
	int indexIndex = 0;		// Index into the "vertex index" array (1D)
	for( ; triIndex < theMesh.numberOfTriangles; triIndex++, indexIndex += 3 )
	{
		indexArray[indexIndex + 0] = theMesh.pTriangles[triIndex].vertex_ID_0;
		indexArray[indexIndex + 1] = theMesh.pTriangles[triIndex].vertex_ID_1;
		indexArray[indexIndex + 2] = theMesh.pTriangles[triIndex].vertex_ID_2;
	}
	// note number of indices is number of triangles x 3
	int sizeOfIndexArrayInBytes = sizeof( unsigned int ) * numberOfIndices;

	glBufferData( GL_ELEMENT_ARRAY_BUFFER,		// "index buffer" or "vertex index buffer"
		sizeOfIndexArrayInBytes,
		indexArray,
		GL_STATIC_DRAW );

	// Don't need local array anymore (like the on in CPU RAM)
	delete[] indexArray;

	// At this point, the vertex and index buffers are stored on GPU

	// Now set up the vertex layout (for this shader)

	GLuint vpos_location = glGetAttribLocation( shaderID, "vPos" );		// program, "vPos");	// 6
	GLuint vcol_location = glGetAttribLocation( shaderID, "vCol" );		// program, "vCol");	// 13
	GLuint vnorm_location = glGetAttribLocation( shaderID, "vNorm" );		// program, "vCol");	// 13
	
	// Added for Texture
	GLuint vUVx2_location = glGetAttribLocation( shaderID, "uvX2" );		// program, "vCol");	// 13

	// Size of the vertex we are using in the array.
	// This is called the "stride" of the vertices in the vertex buffer
	const unsigned int VERTEX_SIZE_OR_STRIDE_IN_BYTES = sizeof( sVertex_xyz_rgba_n_uv2_bt );

	glEnableVertexAttribArray( vpos_location );
	const unsigned int OFFSET_TO_X_IN_CVERTEX = offsetof( sVertex_xyz_rgba_n_uv2_bt, x );
	glVertexAttribPointer( vpos_location,
							3,				// now vec3, not vec2   
							GL_FLOAT,
							GL_FALSE,
							VERTEX_SIZE_OR_STRIDE_IN_BYTES,		//sizeof(float) * 9,	// cVertex is 6 floats in size
							reinterpret_cast<void*>( static_cast<uintptr_t>( OFFSET_TO_X_IN_CVERTEX ) ) );	// 64-bit


	glEnableVertexAttribArray( vcol_location );
	const unsigned int OFFSET_TO_R_IN_CVERTEX = offsetof( sVertex_xyz_rgba_n_uv2_bt, r );
	glVertexAttribPointer( vcol_location,
							4,				// Was 3, but now has alpha
							GL_FLOAT,
							GL_FALSE,
							VERTEX_SIZE_OR_STRIDE_IN_BYTES,		// sizeof(float) * 9,		// (  sizeof(cVertex)  );
							reinterpret_cast<void*>( static_cast<uintptr_t>( OFFSET_TO_R_IN_CVERTEX ) ) );	// 64-bit

	// Now we also have normals on the vertex
	glEnableVertexAttribArray( vnorm_location );
	const unsigned int OFFSET_TO_NX_IN_CVERTEX = offsetof( sVertex_xyz_rgba_n_uv2_bt, nx );
	glVertexAttribPointer( vnorm_location,
							3,
							GL_FLOAT,
							GL_FALSE,
							VERTEX_SIZE_OR_STRIDE_IN_BYTES,		// sizeof(float) * 9,		// (  sizeof(cVertex)  );
							reinterpret_cast<void*>( static_cast<uintptr_t>( OFFSET_TO_NX_IN_CVERTEX ) ) );	// 64-bit

	// ***************************************************************
	// Now we also have UVs on the vertex
	glEnableVertexAttribArray( vUVx2_location );
	const unsigned int OFFSET_TO_UVs_IN_CVERTEX = offsetof( sVertex_xyz_rgba_n_uv2_bt, u1 );
	glVertexAttribPointer( vUVx2_location,
							4,				// because "vec4"
							GL_FLOAT,			// vec is a float 
							GL_FALSE,			// normalize or not
							VERTEX_SIZE_OR_STRIDE_IN_BYTES,		// sizeof(float) * 9,		// (  sizeof(cVertex)  );
							reinterpret_cast<void*>( static_cast<uintptr_t>( OFFSET_TO_UVs_IN_CVERTEX ) ) );	// 64-bit
	// *******************************************************************

	// Copy the information into the VAOInfo structure
	theVAOInfo.numberOfIndices = theMesh.numberOfTriangles * 3;
	theVAOInfo.numberOfTriangles = theMesh.numberOfTriangles;
	theVAOInfo.numberOfVertices = theMesh.numberOfVertices;
	theVAOInfo.friendlyName = theMesh.name;
	theVAOInfo.shaderID = shaderID;

	theMesh.CalculateExtents();
	theVAOInfo.scaleForUnitBBox = theMesh.scaleForUnitBBox;

	// Store the VAO info by mesh name
	this->m_mapNameToVAO[theMesh.name] = theVAOInfo;

	//std::string myArray[10];
	//myArray[3] = "Michael"

	// CRITICAL 
	// Unbind the VAO first!!!!
	glBindVertexArray( 0 );	// 

	// Unbind (release) everything
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray( vcol_location );
	glDisableVertexAttribArray( vpos_location );
	glDisableVertexAttribArray( vnorm_location );
	glDisableVertexAttribArray( vUVx2_location );

	// Save this mesh? 
	if( bKeepMesh )
	{	// Make a COPY for later...
		//std::map< std::string, cMesh > m_mapNameToMesh;
		this->m_mapNameToMesh[theMesh.name] = theMesh;
	}//if (bKeepMesh)

	return true;
}

bool cVAOMeshManager::lookupMeshFromName( std::string name, cMesh &theMesh )
{
	// Search for mesh by name using iterator based find
	std::map< std::string, cMesh >::iterator itMesh =
		this->m_mapNameToMesh.find( name );

	// Find it? 
	if( itMesh == this->m_mapNameToMesh.end() )
	{	// Nope. 
		return false;
	}
	// Found the mesh (DIDN'T return .end())
	theMesh = itMesh->second;		// "return" the mesh by reference 
	return true;
}

bool cVAOMeshManager::lookupVAOFromName( std::string name, sVAOInfo &theVAOInfo )
{
	// look up in map for the name of the mesh we want to draw

	//	std::map< std::string, sVAOInfo > m_mapNameToVAO;
	// "Interator" is a class that can access inside a container
	std::map< std::string, sVAOInfo >::iterator itVAO = this->m_mapNameToVAO.find( name );

	// Did I find something?
	if( itVAO == this->m_mapNameToVAO.end() )
	{	// ON NO! we DIDN'T!
		return false;
	}
	// DID find what we were looking for, so 
	//	ISN'T pointing to the "end()" built-in iterator
	theVAOInfo = itVAO->second;		// Because the "second" thing is the sVAO

	return true;
}