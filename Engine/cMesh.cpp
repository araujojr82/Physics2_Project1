#include "cMesh.h"

#include <glm/glm.hpp>		// cross product (I hope)

cMesh::cMesh()
{
	this->numberOfVertices = 0;
	this->numberOfTriangles = 0;
	//
	this->maxExtent = 0.0f;
	this->scaleForUnitBBox = 1.0f;
	return;
}


cMesh::~cMesh()
{

	return;
}

void cMesh::CalculateExtents(void)
{
	// Assume 1st vertx is both max and min
	this->minXYZ.x = this->pVertices[0].x;
	this->minXYZ.y = this->pVertices[0].y;
	this->minXYZ.z = this->pVertices[0].z;
	this->maxXYZ.x = this->pVertices[0].x;
	this->maxXYZ.y = this->pVertices[0].y;
	this->maxXYZ.z = this->pVertices[0].z;


	for (int index = 0; index != this->numberOfVertices; index++)
	{
		if (this->pVertices[index].x < this->minXYZ.x)
		{
			this->minXYZ.x = this->pVertices[index].x;
		}
		if (this->pVertices[index].x > this->maxXYZ.x)
		{
			this->maxXYZ.x = this->pVertices[index].x;
		}
		
		// Y
		if (this->pVertices[index].y < this->minXYZ.y)
		{
			this->minXYZ.y = this->pVertices[index].y;
		}
		if (this->pVertices[index].y > this->maxXYZ.y)
		{
			this->maxXYZ.y = this->pVertices[index].y;
		}

		// Z
		if (this->pVertices[index].z < this->minXYZ.z)
		{
			this->minXYZ.z = this->pVertices[index].z;
		}
		if (this->pVertices[index].z > this->maxXYZ.z)
		{
			this->maxXYZ.z = this->pVertices[index].z;
		}

	}// for (int index...

	this->maxExtentXYZ.x = this->maxXYZ.x - this->minXYZ.x;
	this->maxExtentXYZ.y = this->maxXYZ.y - this->minXYZ.y;
	this->maxExtentXYZ.z = this->maxXYZ.z - this->minXYZ.z;

	this->maxExtent = this->maxExtentXYZ.x;
	if (this->maxExtent < this->maxExtentXYZ.y) 
	{	// Y is bigger
		this->maxExtent = this->maxExtentXYZ.y;
	}
	if (this->maxExtent < this->maxExtentXYZ.z) 
	{	// Z is bigger
		this->maxExtent = this->maxExtentXYZ.z;
	}
	this->scaleForUnitBBox = 1.0f / this->maxExtent;

	return;
}

// Takes an indexed model and makes just a vertex array model
void cMesh::FlattenIndexedModel(void)
{
	// 1. Make a copy of the original vertices
	// 2. Using the triangle array, replace the original data

	int origNumVertices = this->numberOfVertices;

	sVertex_xyz_rgba_n_uv2_bt* pVertOrig = new sVertex_xyz_rgba_n_uv2_bt[this->numberOfVertices]; // Heap

	for ( int index = 0; index < origNumVertices; index++ )
	{
		pVertOrig[index] = this->pVertices[index];
	}
	// We now have a copy of the original vertices

	// Adjust the original vertex array
	delete [] this->pVertices;		// GOTCHA!

//	int numberOfVertsTOTALGUESS = this->numberOfVertices * 4;

	// Actually, the number of vertices would be 
	//	the number of triangles x 3...
	int numberOfVertsNeeded = this->numberOfTriangles * 3;
	numberOfVertsNeeded += 30;	// Add a few, just in case...
	this->pVertices = new sVertex_xyz_rgba_n_uv2_bt[numberOfVertsNeeded];
	

	int triIndex = 0;
	int vertIndex = 0;
	for ( ; triIndex < this->numberOfTriangles; 
		  triIndex++, vertIndex += 3 )
	{
		// 3 4 132 80 
		int triVert0_index = this->pTriangles[triIndex].vertex_ID_0;
		int triVert1_index = this->pTriangles[triIndex].vertex_ID_1;
		int triVert2_index = this->pTriangles[triIndex].vertex_ID_2;
	
		sVertex_xyz_rgba_n_uv2_bt V0 = pVertOrig[triVert0_index];
		sVertex_xyz_rgba_n_uv2_bt V1 = pVertOrig[triVert1_index];
		sVertex_xyz_rgba_n_uv2_bt V2 = pVertOrig[triVert2_index];


		this->pVertices[vertIndex + 0] = V0;
		this->pVertices[vertIndex + 1] = V1;
		this->pVertices[vertIndex + 2] = V2;
	}//for ( ; 
	
	// Update the number of vertices
	// HACK: -3 YIKES!!! 
	this->numberOfVertices = vertIndex - 3;	

	return;
}

void cMesh::CalculateNormals( void )
{
	// Go through each triangle
	// calculate normal per vertex (3 vertices)
	// ADD this normal to the corresponding vertex

	// 2nd pass
	// go through all the vertices and normalize them
	// 2nd pass: normalize the normals
	for ( int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++ )
	{
		this->pVertices[vertIndex].nx = 0.0f;
		this->pVertices[vertIndex].ny = 0.0f;
		this->pVertices[vertIndex].nz = 0.0f;
	}

	for ( int triIndex = 0; triIndex != this->numberOfTriangles; triIndex++ )
	{
		cTriangle curTri = this->pTriangles[triIndex];

		//Calculate normal for each vertex
		glm::vec3 vertA = glm::vec3( this->pVertices[curTri.vertex_ID_0].x,
			this->pVertices[curTri.vertex_ID_0].y,
			this->pVertices[curTri.vertex_ID_0].z );

		glm::vec3 vertB = glm::vec3( this->pVertices[curTri.vertex_ID_1].x,
			this->pVertices[curTri.vertex_ID_1].y,
			this->pVertices[curTri.vertex_ID_1].z );

		glm::vec3 vertC = glm::vec3( this->pVertices[curTri.vertex_ID_2].x,
			this->pVertices[curTri.vertex_ID_2].y,
			this->pVertices[curTri.vertex_ID_2].z );

		// Cross of A-B and A-C (normal at vertex A)
		glm::vec3 normVec0 = glm::normalize( glm::cross( vertB - vertA, vertC - vertA ) );

		// Cross of B-A and B-C (normal at vertex B)
		glm::vec3 normVec1 = glm::normalize( glm::cross( vertA - vertB, vertC - vertB ) );

		// Cross of C-A and C-B (normal at vertex C)
		glm::vec3 normVec2 = glm::normalize( glm::cross( vertA - vertC, vertB - vertC ) );

		// HACK
		if ( this->name == "sphere" )
		{
			// Invert Normals so it will be iluminated "from the inside"
			normVec0.x = -normVec0.x; //( normVec0.x < 0.0 ? normVec0.x : -normVec0.x );
			normVec0.y = -normVec0.y; // ( normVec0.y < 0.0 ? normVec0.y : -normVec0.y );
			normVec0.z = -normVec0.z; // ( normVec0.z < 0.0 ? normVec0.z : -normVec0.z );
		}

		//normVec0 = glm::normalize( glm::cross( vertB - vertA, vertC - vertA ) );
		normVec1 = normVec0;
		normVec2 = normVec0;

		// Add the values to the current normals (vert A)
		this->pVertices[curTri.vertex_ID_0].nx += normVec0.x;
		this->pVertices[curTri.vertex_ID_0].ny += normVec0.y;
		this->pVertices[curTri.vertex_ID_0].nz += normVec0.z;

		// Add the values to the current normals (vert B)
		this->pVertices[curTri.vertex_ID_1].nx += normVec1.x;
		this->pVertices[curTri.vertex_ID_1].ny += normVec1.y;
		this->pVertices[curTri.vertex_ID_1].nz += normVec1.z;

		// Add the values to the current normals (vert C)
		this->pVertices[curTri.vertex_ID_2].nx += normVec2.x;
		this->pVertices[curTri.vertex_ID_2].ny += normVec2.y;
		this->pVertices[curTri.vertex_ID_2].nz += normVec2.z;

	}

	//// 2nd pass: normalize the normals
	//for (int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++)
	//{
	//	glm::vec3 norm = glm::vec3(this->pVertices[vertIndex].nx,
	//								this->pVertices[vertIndex].ny,
	//								this->pVertices[vertIndex].nz);

	//	// It's value DIV length
	//	glm::normalize(norm);

	//	this->pVertices[vertIndex].nx = norm.x;
	//	this->pVertices[vertIndex].ny = norm.y;
	//	this->pVertices[vertIndex].nz = norm.z;
	//}

	return;
}

void cMesh::CalculateNormals2( void )
{
	// Go through each triangle
	// calculate normal per vertex (3 vertices)
	// ADD this normal to the corresponding vertex

	// 2nd pass
	// go through all the vertices and normalize them
	// 2nd pass: normalize the normals
	for( int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++ )
	{
		this->pVertices[vertIndex].nx = 0.0f;
		this->pVertices[vertIndex].ny = 0.0f;
		this->pVertices[vertIndex].nz = 0.0f;
	}

	for( int triIndex = 0; triIndex != this->numberOfTriangles; triIndex++ )
	{
		cTriangle curTri = this->pTriangles[triIndex];

		glm::vec3 vertA = glm::vec3( this->pVertices[curTri.vertex_ID_0].x,
			this->pVertices[curTri.vertex_ID_0].y,
			this->pVertices[curTri.vertex_ID_0].z );

		glm::vec3 vertB = glm::vec3( this->pVertices[curTri.vertex_ID_1].x,
			this->pVertices[curTri.vertex_ID_1].y,
			this->pVertices[curTri.vertex_ID_1].z );

		glm::vec3 vertC = glm::vec3( this->pVertices[curTri.vertex_ID_2].x,
			this->pVertices[curTri.vertex_ID_2].y,
			this->pVertices[curTri.vertex_ID_2].z );

		glm::vec3 BA = vertB - vertA;
		glm::vec3 CA = vertC - vertA;

		this->pTriangles[triIndex].normal = glm::normalize( glm::cross( BA, CA ) );

		glm::vec3 vertNormal = this->pTriangles[triIndex].normal;

		// Add the values to the current normals (vert A)
		this->pVertices[curTri.vertex_ID_0].nx += vertNormal.x;
		this->pVertices[curTri.vertex_ID_0].ny += vertNormal.y;
		this->pVertices[curTri.vertex_ID_0].nz += vertNormal.z;

		// Add the values to the current normals (vert B)
		this->pVertices[curTri.vertex_ID_1].nx += vertNormal.x;
		this->pVertices[curTri.vertex_ID_1].ny += vertNormal.y;
		this->pVertices[curTri.vertex_ID_1].nz += vertNormal.z;

		// Add the values to the current normals (vert C)
		this->pVertices[curTri.vertex_ID_2].nx += vertNormal.x;
		this->pVertices[curTri.vertex_ID_2].ny += vertNormal.y;
		this->pVertices[curTri.vertex_ID_2].nz += vertNormal.z;

	}

	for( int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++ )
	{
		std::vector<cTriangle> trianglesForThisVert;
		glm::vec3 vertNormal = glm::vec3( 0.0f );

		for( int triIndex = 0; triIndex != this->numberOfTriangles; triIndex++ )
		{
			cTriangle curTri = this->pTriangles[triIndex];
			// Check if the triangle has this vertice
			if( curTri.vertex_ID_0 == vertIndex ||
				curTri.vertex_ID_1 == vertIndex ||
				curTri.vertex_ID_2 == vertIndex )
			{
				// If it does, add it to the triangles vector to calculate the vertice normal later
				trianglesForThisVert.push_back( curTri );
			}
		}
		
		//triangle tr1, tr2, tr3 // all share vertex v1
		for( int i = 0; i != trianglesForThisVert.size(); i++ )
		{
			vertNormal += trianglesForThisVert[i].normal;
		}

		vertNormal = glm::vec3( this->pVertices[vertIndex].nx, this->pVertices[vertIndex].ny, this->pVertices[vertIndex].nz );
		vertNormal += glm::normalize( vertNormal );

		this->pVertices[vertIndex].nx = vertNormal.x;
		this->pVertices[vertIndex].ny = vertNormal.y;
		this->pVertices[vertIndex].nz = vertNormal.z;
	}

	return;
}

void cMesh::CalculateUV( void )
{
	for( int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++ )
	{
		this->pVertices[vertIndex].u1 /= 10.0f;
		this->pVertices[vertIndex].v1 /= 10.0f;
	}

	//for( int vertIndex = 0; vertIndex != this->numberOfVertices; vertIndex++ )
	//{
	//	this->pVertices[vertIndex].u1 = 0.0f;
	//	this->pVertices[vertIndex].v1 = 0.0f;
	//}

	//for( int triIndex = 0; triIndex != this->numberOfTriangles; triIndex++ )
	//{
	//	cTriangle curTri = this->pTriangles[triIndex];

	//	glm::vec3 vertA = glm::vec3( this->pVertices[curTri.vertex_ID_0].x,
	//								 this->pVertices[curTri.vertex_ID_0].y,
	//								 this->pVertices[curTri.vertex_ID_0].z );

	//	glm::vec3 vertB = glm::vec3( this->pVertices[curTri.vertex_ID_1].x,
	//								 this->pVertices[curTri.vertex_ID_1].y,
	//								 this->pVertices[curTri.vertex_ID_1].z );

	//	glm::vec3 vertC = glm::vec3( this->pVertices[curTri.vertex_ID_2].x,
	//								 this->pVertices[curTri.vertex_ID_2].y,
	//								 this->pVertices[curTri.vertex_ID_2].z );

	//	glm::vec3 normA = glm::vec3( this->pVertices[curTri.vertex_ID_0].nx,
	//								 this->pVertices[curTri.vertex_ID_0].ny,
	//								 this->pVertices[curTri.vertex_ID_0].nz );

	//	glm::vec3 normB = glm::vec3( this->pVertices[curTri.vertex_ID_1].nx,
	//								 this->pVertices[curTri.vertex_ID_1].ny,
	//								 this->pVertices[curTri.vertex_ID_1].nz );

	//	glm::vec3 normC = glm::vec3( this->pVertices[curTri.vertex_ID_2].nx,
	//								 this->pVertices[curTri.vertex_ID_2].ny,
	//								 this->pVertices[curTri.vertex_ID_2].nz );

	//	// Set the U and V values for vertA or ID_0
	//	glm::vec3 uAxisA = glm::normalize( ( glm::vec3( normA ) ) );
	//	glm::vec3 vAxisA = glm::cross( uAxisA, normA );

	//	float eaveHeightA = 0.0f;
	//
	//	float eaveCorrectionA = glm::dot( vAxisA, vertA + vAxisA * ( eaveHeightA - vertA.y ) / vAxisA.y );

	//	glm::vec2 uvA = glm::vec2( glm::dot( vertA, uAxisA ), glm::dot( vAxisA, vertA ) - eaveCorrectionA );

	//	this->pVertices[curTri.vertex_ID_0].u1 = uvA.x;
	//	this->pVertices[curTri.vertex_ID_0].v1 = uvA.y;

	//	// Set the U and V values for vertB or ID_2
	//	glm::vec3 uAxisB = glm::normalize( ( glm::vec3( normB ) ) );
	//	glm::vec3 vAxisB = glm::cross( uAxisB, normB );

	//	float eaveHeightB = 0.0f;

	//	float eaveCorrectionB = glm::dot( vAxisB, vertB + vAxisB * ( eaveHeightB - vertB.y ) / vAxisB.y );

	//	glm::vec2 uvB = glm::vec2( glm::dot( vertB, uAxisB ), glm::dot( vAxisB, vertB ) - eaveCorrectionB );

	//	this->pVertices[curTri.vertex_ID_0].u1 = uvB.x;
	//	this->pVertices[curTri.vertex_ID_0].v1 = uvB.y;

	//	// Set the U and V values for vertC or ID_2
	//	glm::vec3 uAxisC = glm::normalize( ( glm::vec3( normC ) ) );
	//	glm::vec3 vAxisC = glm::cross( uAxisC, normC );

	//	float eaveHeightC = 0.0f;

	//	float eaveCorrectionC = glm::dot( vAxisC, vertC + vAxisC * ( eaveHeightC - vertC.y ) / vAxisC.y );

	//	glm::vec2 uvC = glm::vec2( glm::dot( vertC, uAxisC ), glm::dot( vAxisC, vertC ) - eaveCorrectionC );

	//	this->pVertices[curTri.vertex_ID_0].u1 = uvC.x;
	//	this->pVertices[curTri.vertex_ID_0].v1 = uvC.y;

	//}

	return;
}