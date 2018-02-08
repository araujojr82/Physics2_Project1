#include "MapUtilities.h"

extern float generateRandomNumber( float min, float max );

int searchAVertexByPoint( std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices, float x, float y, float z, char floatingAxis )
{
	if( floatingAxis == 'y' )
	{
		for( int i = 0; i != meshVertices.size(); i++ )
		{
			if( meshVertices[i].x == x && meshVertices[i].z == z )
			{
				float distanceY = glm::distance( meshVertices[i].y, y );
				if( distanceY <= 0.2 )
				{
					return i;
				}
			}
		}
	}

	else if( floatingAxis == 'x' )
	{
		for( int i = 0; i != meshVertices.size(); i++ )
		{

			if( meshVertices[i].y == y && meshVertices[i].z == z )
			{
				float distanceX = glm::distance( meshVertices[i].x, x );
				if( distanceX <= 0.2 )
				{
					return i;
				}
			}
		}
	}
	else
	{
		for( int i = 0; i != meshVertices.size(); i++ )
		{

			if( meshVertices[i].x == x && meshVertices[i].y == y )
			{
				float distanceZ = glm::distance( meshVertices[i].z, z );
				if( distanceZ <= 0.2 )
				{
					return i;
				}
			}
		}
	}

	return -1;
}

void divideTriangleInTwo( std::vector<cTriangle> &theNewTriangles,
	std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices,	
	int v1pos, int v2pos, int v3pos,
	char floatingAxis, float min, float max, int firstVertice )
{
	// std::vector<cTriangle> theNewTriangles;
	cTriangle newTri1, newTri2;
	sVertex_xyz_rgba_n_uv2_bt v1, v2, v3, v4;
	int v4pos;

	v1 = meshVertices[v1pos];
	v2 = meshVertices[v2pos];
	v3 = meshVertices[v3pos];

	if( floatingAxis == 'x' )
	{
		v4.x = generateRandomNumber( min, max );
		v4.y = ( v2.y + v3.y ) / 2;
		v4.z = ( v2.z + v3.z ) / 2;
	}
	else if( floatingAxis == 'y' )
	{
		v4.x = ( v2.x + v3.x ) / 2;
		v4.y = generateRandomNumber( min, max );
		v4.z = ( v2.z + v3.z ) / 2;
	}
	else
	{
		v4.x = ( v2.x + v3.x ) / 2;
		v4.y = ( v2.y + v3.y ) / 2;
		v4.z = generateRandomNumber( min, max );
	}

	// Before storing, check if theres another vertice at this point
	// and use the same, else store the new vertice
	v4pos = searchAVertexByPoint( meshVertices, v4.x, v4.y, v4.z, floatingAxis );
	if( v4pos == -1 )
	{
		meshVertices.push_back( v4 );
		v4pos = meshVertices.size() - 1;
	}
	else if( v4pos < firstVertice )
	{
		meshVertices.push_back( v4 );
		v4pos = meshVertices.size() - 1;
	}

	meshVertices[v4pos].u1 = ( v2.u1 + v3.u1 ) / 2;
	meshVertices[v4pos].v1 = ( v2.v1 + v3.v1 ) / 2;

	// 1 3 4
	newTri1.vertex_ID_0 = v4pos;
	newTri1.vertex_ID_1 = v3pos;
	newTri1.vertex_ID_2 = v1pos;
	theNewTriangles.push_back( newTri1 );

	// 1 4 2
	newTri2.vertex_ID_0 = v4pos;
	newTri2.vertex_ID_1 = v1pos;
	newTri2.vertex_ID_2 = v2pos;
	theNewTriangles.push_back( newTri2 );

	return;
}

void createTrianglesFromVertices( 
	std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices,	
	std::vector<cTriangle> &meshTriangles,
	int v1pos, int v2pos, int v3pos,
	int divisions, float min, float max,
	char floatingAxis,
	int firstVertice ) // how many divisions will it make
{
	std::vector<cTriangle> trianglesToBeDivided;
	std::vector<cTriangle> tempVectorOfTriangles;

	cTriangle newTriangle;

	// Add the base triangle to start the division based on it
	newTriangle.vertex_ID_0 = v1pos;
	newTriangle.vertex_ID_1 = v3pos;
	newTriangle.vertex_ID_2 = v2pos;
	trianglesToBeDivided.push_back( newTriangle );

	// How many times it will be divided
	for( int i = 0; i != divisions; i++ )
	{
		tempVectorOfTriangles.clear();
		// For each pass, go through all triangles created
		for( int pos = 0; pos != trianglesToBeDivided.size(); pos++ )
		{
			divideTriangleInTwo( tempVectorOfTriangles, meshVertices,
				trianglesToBeDivided[pos].vertex_ID_0,
				trianglesToBeDivided[pos].vertex_ID_2,
				trianglesToBeDivided[pos].vertex_ID_1,
				floatingAxis, min, max, firstVertice );
		}
		trianglesToBeDivided.clear();
		trianglesToBeDivided = tempVectorOfTriangles;
	}

	for( int j = 0; j != trianglesToBeDivided.size(); j++ )
	{
		meshTriangles.push_back( trianglesToBeDivided[j] );
	}

	return;
}

cMesh createMap( int shaderID )
{
	cRandomMap theMap;
	theMap.newMap();

	int triangleCount = 0;
	int verticeCount = 0;	
	const int NUMBER_OF_DIVISIONS = 1;
	const float C_DELTA = 0.1f;
	//const int NUMBER_OF_DIVISIONS = 1;
	//const float C_DELTA = 0.0f;

	std::vector<sVertex_xyz_rgba_n_uv2_bt> meshVertices;
	sVertex_xyz_rgba_n_uv2_bt theVertex;	

	std::vector<cTriangle> meshTriangles;
	cTriangle theTriangle;

	float min, max;
	int v1, v2, v3, v4, v5, v6, v7, v8;
	v1 = 0;
	v2 = 0;
	v3 = 0;
	v4 = 0;
	v5 = 0;
	v6 = 0;
	v7 = 0;
	v8 = 0;

	for( int i = 0; i != theMap.tileMap.size(); i++ )
	{
		point p = theMap.tileMap[i].pos;

		int firstVertice;

		min = 1 * p.z;
		max = min;
		//max = min + C_DELTA;

		// ADD 4 Vertices of the floor tile
		theVertex.x = p.x - 0.5f;						//1st Vertex
		theVertex.y = p.y + 0.5f;
		theVertex.z = generateRandomNumber( min, max );
		theVertex.u1 = 0.0f;
		theVertex.v1 = 1.0f;
		meshVertices.push_back( theVertex );
		v1 = meshVertices.size() - 1;

		theVertex.x = p.x + 0.5f;						//2nd Vertex
		theVertex.y = p.y + 0.5f;
		theVertex.z = generateRandomNumber( min, max );
		theVertex.u1 = 1.0f;
		theVertex.v1 = 1.0f;
		meshVertices.push_back( theVertex );
		v2 = meshVertices.size() - 1;

		theVertex.x = p.x - 0.5f;						//3rd Vertex
		theVertex.y = p.y - 0.5f;
		theVertex.z = generateRandomNumber( min, max );
		theVertex.u1 = 0.0f;
		theVertex.v1 = 0.0f;
		meshVertices.push_back( theVertex );
		v3 = meshVertices.size() - 1;

		theVertex.x = p.x + 0.5f;						//4th Vertex
		theVertex.y = p.y - 0.5f;
		theVertex.z = generateRandomNumber( min, max );
		theVertex.u1 = 1.0f;
		theVertex.v1 = 0.0f;
		meshVertices.push_back( theVertex );
		v4 = meshVertices.size() - 1;

		////Create the 2 triangles
		//firstVertice = v1;
		//createTrianglesFromVertices( meshVertices, meshTriangles, v1, v3, v2, NUMBER_OF_DIVISIONS, min, max, 'z', firstVertice );
		//createTrianglesFromVertices( meshVertices, meshTriangles, v4, v2, v3, NUMBER_OF_DIVISIONS, min, max, 'z', firstVertice );

		//Create North Wall if exists
		if( theMap.tileMap[i].nWall == true )
		{
			max = p.y + 0.5f;
			min = max;
			//min = max - C_DELTA;
			
			// ADD 2 top Vertices of the north wall
			theVertex.x = p.x - 0.5f;						//5th Vertex
			theVertex.y = p.y + 0.5f;
			//theVertex.y = generateRandomNumber( min, max );
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 0.0f;
			theVertex.v1 = 1.0f;
			meshVertices.push_back( theVertex );
			v5 = meshVertices.size() - 1;

			theVertex.x = p.x + 0.5f;						//6th Vertex
			theVertex.y = p.y + 0.5f;
			//theVertex.y = generateRandomNumber( min, max );
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 1.0f;
			theVertex.v1 = 1.0f;
			meshVertices.push_back( theVertex );
			v6 = meshVertices.size() - 1;

			meshVertices.push_back( meshVertices[v1] );
			v1 = meshVertices.size() - 1;
			meshVertices[v1].u1 = 0.0f;
			meshVertices[v1].v1 = 0.0f;

			meshVertices.push_back( meshVertices[v2] );
			v2 = meshVertices.size() - 1;
			meshVertices[v2].u1 = 1.0f;
			meshVertices[v2].v1 = 0.0f;

			//firstVertice = v5;
			////Create the 2 triangles
			//createTrianglesFromVertices( meshVertices, meshTriangles, v5, v1, v6, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );
			//createTrianglesFromVertices( meshVertices, meshTriangles, v2, v6, v1, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );
		}

		//Create Right Wall if exists
		if( theMap.tileMap[i].rWall == true )
		{
			max = p.x + 0.5f;
			min = max;
			//min = max - C_DELTA;
			
			// ADD 2 top Vertices of the right wall
			theVertex.x = p.x + 0.5f;						//6th Vertex
			theVertex.y = p.y + 0.5f;
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 0.0f;
			theVertex.v1 = 1.0f;	
			meshVertices.push_back( theVertex );
			v6 = meshVertices.size() - 1;

			theVertex.x = p.x + 0.5f;						//7th Vertex
			theVertex.y = p.y - 0.5f;
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 1.0f;
			theVertex.v1 = 1.0f;
			meshVertices.push_back( theVertex );
			v7 = meshVertices.size() - 1;

			meshVertices.push_back( meshVertices[v4] );
			v4 = meshVertices.size() - 1;
			meshVertices[v4].u1 = 0.0f;
			meshVertices[v4].v1 = 0.0f;

			meshVertices.push_back( meshVertices[v2] );
			v2 = meshVertices.size() - 1;
			meshVertices[v2].u1 = 1.0f;
			meshVertices[v2].v1 = 0.0f;

			//firstVertice = v6;
			////Create the 2 triangles
			//createTrianglesFromVertices( meshVertices, meshTriangles, v6, v2, v7, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );
			//createTrianglesFromVertices( meshVertices, meshTriangles, v4, v7, v2, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );

		}

		//Create South Wall if exists
		if( theMap.tileMap[i].sWall == true )
		{
			min = p.y - 0.5f;
			max = min;
			//max = min + C_DELTA;

			// ADD 2 top Vertices of the south wall
			theVertex.x = p.x + 0.5f;						//7th Vertex
			theVertex.y = p.y - 0.5f;
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 0.0f;
			theVertex.v1 = 1.0f;
			meshVertices.push_back( theVertex );
			v7 = meshVertices.size() - 1;
			
			// ADD 2 top Vertices of the south wall
			theVertex.x = p.x - 0.5f;						//8th Vertex
			theVertex.y = p.y - 0.5f;
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 1.0f;
			theVertex.v1 = 1.0f;
			meshVertices.push_back( theVertex );
			v8 = meshVertices.size() - 1;

			meshVertices.push_back( meshVertices[v4] );
			v4 = meshVertices.size() - 1;
			meshVertices[v4].u1 = 0.0f;
			meshVertices[v4].v1 = 0.0f;

			meshVertices.push_back( meshVertices[v3] );
			v3 = meshVertices.size() - 1;
			meshVertices[v3].u1 = 1.0f;
			meshVertices[v3].v1 = 0.0f;

			//firstVertice = v7;
			////Create the 2 triangles
			//createTrianglesFromVertices( meshVertices, meshTriangles, v7, v4, v8, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );
			//createTrianglesFromVertices( meshVertices, meshTriangles, v3, v8, v4, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );
		}

		//Create Left Wall if exists
		if( theMap.tileMap[i].lWall == true )
		{
			min = p.x - 0.5f;
			max = min;
			//max = min + C_DELTA;

			// ADD 2 top Vertices of the left wall
			theVertex.x = p.x - 0.5f;						//8th Vertex
			theVertex.y = p.y - 0.5f;
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 0.0f;
			theVertex.v1 = 1.0f;
			meshVertices.push_back( theVertex );
			v8 = meshVertices.size() - 1;

			// ADD 2 top Vertices of the left wall
			theVertex.x = p.x - 0.5f;						//5th Vertex
			theVertex.y = p.y + 0.5f;
			theVertex.z = ( 1 * p.z ) + 1;
			theVertex.u1 = 1.0f;
			theVertex.v1 = 1.0f;
			meshVertices.push_back( theVertex );
			v5 = meshVertices.size() - 1;

			meshVertices.push_back( meshVertices[v1] );
			v1 = meshVertices.size() - 1;
			meshVertices[v1].u1 = 0.0f;
			meshVertices[v1].v1 = 0.0f;

			meshVertices.push_back( meshVertices[v3] );
			v3 = meshVertices.size() - 1;
			meshVertices[v3].u1 = 1.0f;
			meshVertices[v3].v1 = 0.0f;

			firstVertice = v8;
			//Create the 2 triangles
			createTrianglesFromVertices( meshVertices, meshTriangles, v8, v3, v5, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );
			createTrianglesFromVertices( meshVertices, meshTriangles, v1, v5, v3, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );
		}
	}

	// double check all vertices to eliminate gaps
	std::vector<sVertex_xyz_rgba_n_uv2_bt> tempVerticesVec;	
	tempVerticesVec = meshVertices;

	int j;

	for( int i = 0; i != meshVertices.size(); i++ )
	{
		for( int j = 0; j != tempVerticesVec.size(); j++ )
		{
			//float distanceZ = meshVertices[i].z - tempVerticesVec[j].z;
			//if( distanceZ <= 0.2f && distanceZ >= -0.2f )	// Test only the near vertices
			//{
			//	// Check to see if they are not the exact same 
			//	if( meshVertices[i].x == tempVerticesVec[j].x &&
			//		meshVertices[i].y == tempVerticesVec[j].y ) continue;

			//	// Check if the distance is between 0.2 apart, if they are they should probably be the sane
			//	glm::vec3 pointI = glm::vec3( meshVertices[i].x, meshVertices[i].y, meshVertices[i].z );
			//	glm::vec3 pointJ = glm::vec3( tempVerticesVec[j].x, tempVerticesVec[j].y, tempVerticesVec[j].z );

			//	if( glm::distance( pointI, pointJ ) <= 0.2f )
			//	{
			//		meshVertices[i].x = tempVerticesVec[j].x;
			//		meshVertices[i].y = tempVerticesVec[j].y;
			//	}
			//}
			// Check if the distance is between 0.2 apart, if they are they should probably be the sane
			glm::vec3 pointI = glm::vec3( meshVertices[i].x, meshVertices[i].y, meshVertices[i].z );
			glm::vec3 pointJ = glm::vec3( tempVerticesVec[j].x, tempVerticesVec[j].y, tempVerticesVec[j].z );

			if( glm::distance( pointI, pointJ ) <= 0.2f )
			{
				meshVertices[i].x = tempVerticesVec[j].x;
				meshVertices[i].y = tempVerticesVec[j].y;
				meshVertices[i].z = tempVerticesVec[j].z;
				break;
			}
		}
	}

	cMesh mapMesh;
	mapMesh.name = "map";
	if( CreateMeshFromVectors( mapMesh, meshVertices, meshTriangles ) )
	{
		saveToPlyFile( meshTriangles, meshVertices );
		return mapMesh;
	}

}

void saveToPlyFile( std::vector<cTriangle> &meshTriangles,
	std::vector<sVertex_xyz_rgba_n_uv2_bt> &meshVertices )
{
	// Save the generated map as a ply file
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t( now );

	std::stringstream ss;
	ss << std::put_time( std::localtime( &in_time_t ), "%Y-%m-%d" ); // %X" );

	std::string filename = "map" + ss.str() + ".ply";

	std::ofstream myfile;
	myfile.open( filename );
	myfile << "ply" << std::endl;
	myfile << "format ascii 1.0" << std::endl;
	myfile << "comment VCGLIB generated" << std::endl;
	myfile << "element vertex " << meshVertices.size() << std::endl;
	myfile << "property float x" << std::endl;
	myfile << "property float y" << std::endl;
	myfile << "property float z" << std::endl;
	myfile << "property float nx" << std::endl;
	myfile << "property float ny" << std::endl;
	myfile << "property float nz" << std::endl;
	myfile << "property float texture_u" << std::endl;
	myfile << "property float texture_v" << std::endl;
	myfile << "element face " << meshTriangles.size() << std::endl;
	myfile << "property list uchar int vertex_indices" << std::endl;
	myfile << "end_header" << std::endl;

	for( int i = 0; i != meshVertices.size(); i++ )
	{
		myfile << meshVertices[i].x << " "
			<< meshVertices[i].y << " "
			<< meshVertices[i].z << " "
			<< meshVertices[i].nx << " "
			<< meshVertices[i].ny << " "
			<< meshVertices[i].nz << " "
			<< meshVertices[i].u1 << " "
			<< meshVertices[i].v1 << std::endl;
	}

	for( int i = 0; i != meshTriangles.size(); i++ )
	{
		myfile << "3 " << meshTriangles[i].vertex_ID_0 << " "
			<< meshTriangles[i].vertex_ID_1 << " "
			<< meshTriangles[i].vertex_ID_2 << std::endl;
	}
	myfile.close();
}


cMesh createCube( int shaderID )
{

	int triangleCount = 0;
	int verticeCount = 0;
	int NUMBER_OF_DIVISIONS = 2;
	int firstVertice = 0;

	std::vector<sVertex_xyz_rgba_n_uv2_bt> meshVertices;
	sVertex_xyz_rgba_n_uv2_bt theVertex;

	std::vector<cTriangle> meshTriangles;
	cTriangle theTriangle;

	float min, max;
	int v1, v2, v3, v4, v5, v6, v7, v8;
	v1 = 0;
	v2 = 0;
	v3 = 0;
	v4 = 0;
	v5 = 0;
	v6 = 0;
	v7 = 0;
	v8 = 0;

	point p;
	p.x = 0;
	p.y = 0;
	p.z = 0;

	// ADD 4 Vertices of the floor tile
	theVertex.x = p.x - 0.5f;						//1st Vertex
	theVertex.y = p.y + 0.5f;
	theVertex.z = 1 * p.z;
	theVertex.u1 = 0.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v1 = meshVertices.size() - 1;

	theVertex.x = p.x + 0.5f;						//2nd Vertex
	theVertex.y = p.y + 0.5f;
	theVertex.z = 1 * p.z;
	theVertex.u1 = 1.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v2 = meshVertices.size() - 1;

	theVertex.x = p.x - 0.5f;						//3rd Vertex
	theVertex.y = p.y - 0.5f;
	theVertex.z = 1 * p.z;
	theVertex.u1 = 0.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v3 = meshVertices.size() - 1;

	theVertex.x = p.x + 0.5f;						//4th Vertex
	theVertex.y = p.y - 0.5f;
	theVertex.z = 1 * p.z;
	theVertex.u1 = 1.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v4 = meshVertices.size() - 1;

	//Create the 2 triangles
	min = 1 * p.z;
	max = 1 * p.z;
	createTrianglesFromVertices( meshVertices, meshTriangles, v1, v3, v2, NUMBER_OF_DIVISIONS, min, max, 'z', firstVertice );
	createTrianglesFromVertices( meshVertices, meshTriangles, v4, v2, v3, NUMBER_OF_DIVISIONS, min, max, 'z', firstVertice );

	// ADD 2 top Vertices of the north wall
	theVertex.x = p.x - 0.5f;						//5th Vertex
	theVertex.y = p.y + 0.5f;
	theVertex.z = ( 1 * p.z ) + 1;
	theVertex.u1 = 0.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v5 = meshVertices.size() - 1;

	theVertex.x = p.x + 0.5f;						//6th Vertex
	theVertex.y = p.y + 0.5f;
	theVertex.z = ( 1 * p.z ) + 1;
	theVertex.u1 = 1.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v6 = meshVertices.size() - 1;

	theVertex = meshVertices[v1];
	theVertex.u1 = 0.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v1 = meshVertices.size() - 1;

	theVertex = meshVertices[v2];
	theVertex.u1 = 1.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v2 = meshVertices.size() - 1;

	min = p.y + 0.5f;
	max = p.y + 0.5f;
	createTrianglesFromVertices( meshVertices, meshTriangles, v5, v1, v6, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );
	createTrianglesFromVertices( meshVertices, meshTriangles, v2, v6, v1, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );

	//Create Right Wall if exists
	// ADD 2 top Vertices of the right wall
	theVertex.x = p.x + 0.5f;						//6th Vertex
	theVertex.y = p.y + 0.5f;
	theVertex.z = ( 1 * p.z ) + 1;
	theVertex.u1 = 0.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v6 = meshVertices.size() - 1;

	theVertex.x = p.x + 0.5f;						//7th Vertex
	theVertex.y = p.y - 0.5f;
	theVertex.z = ( 1 * p.z ) + 1;
	theVertex.u1 = 1.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v7 = meshVertices.size() - 1;

	theVertex = meshVertices[v2];
	theVertex.u1 = 0.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v2 = meshVertices.size() - 1;
	
	theVertex = meshVertices[v4];
	theVertex.u1 = 1.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v4 = meshVertices.size() - 1;
	
	//Create the 2 triangles
	min = p.x + 0.5f;
	max = p.x + 0.5f;
	createTrianglesFromVertices( meshVertices, meshTriangles, v6, v2, v7, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );
	createTrianglesFromVertices( meshVertices, meshTriangles, v4, v7, v2, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );

	//Create South Wall if exists	
	// ADD 2 top Vertices of the right wall
	theVertex.x = p.x + 0.5f;						//7th Vertex
	theVertex.y = p.y - 0.5f;
	theVertex.z = ( 1 * p.z ) + 1;
	theVertex.u1 = 0.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v7 = meshVertices.size() - 1;

	theVertex.x = p.x - 0.5f;						//8th Vertex
	theVertex.y = p.y - 0.5f;
	theVertex.z = ( 1 * p.z ) + 1;
	theVertex.u1 = 1.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v8 = meshVertices.size() - 1;

	theVertex = meshVertices[v4];
	theVertex.u1 = 0.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v4 = meshVertices.size() - 1;

	theVertex = meshVertices[v3];
	theVertex.u1 = 1.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v3 = meshVertices.size() - 1;

	//Create the 2 triangles
	min = p.y - 0.5f;
	max = p.y - 0.5f;
	createTrianglesFromVertices( meshVertices, meshTriangles, v7, v4, v8, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );
	createTrianglesFromVertices( meshVertices, meshTriangles, v3, v8, v4, NUMBER_OF_DIVISIONS, min, max, 'y', firstVertice );

	//Create Left Wall if exists
	// ADD 2 top Vertices of the right wall
	theVertex = meshVertices[v8];
	theVertex.u1 = 0.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v8 = meshVertices.size() - 1;

	theVertex = meshVertices[v5];
	theVertex.u1 = 1.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v5 = meshVertices.size() - 1;
		
	theVertex = meshVertices[v3];
	theVertex.u1 = 0.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v3 = meshVertices.size() - 1;

	theVertex = meshVertices[v1];
	theVertex.u1 = 1.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v1 = meshVertices.size() - 1;

	//Create the 2 triangles
	min = p.x - 0.5f;
	max = p.x - 0.5f;
	createTrianglesFromVertices( meshVertices, meshTriangles, v8, v3, v5, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );
	createTrianglesFromVertices( meshVertices, meshTriangles, v1, v5, v3, NUMBER_OF_DIVISIONS, min, max, 'x', firstVertice );

	//Create Top
	// ADD 4 top Vertices of the Top

	theVertex = meshVertices[v8];
	theVertex.u1 = 0.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v8 = meshVertices.size() - 1;

	theVertex = meshVertices[v7];
	theVertex.u1 = 1.0f;
	theVertex.v1 = 1.0f;
	meshVertices.push_back( theVertex );
	v7 = meshVertices.size() - 1;

	theVertex = meshVertices[v5];
	theVertex.u1 = 0.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v5 = meshVertices.size() - 1;

	theVertex = meshVertices[v6];
	theVertex.u1 = 1.0f;
	theVertex.v1 = 0.0f;
	meshVertices.push_back( theVertex );
	v6 = meshVertices.size() - 1;

	//Create the 2 triangles
	min = ( 1 * p.z ) + 1;
	max = ( 1 * p.z ) + 1;
	createTrianglesFromVertices( meshVertices, meshTriangles, v8, v5, v7, NUMBER_OF_DIVISIONS, min, max, 'z', firstVertice );
	createTrianglesFromVertices( meshVertices, meshTriangles, v6, v7, v5, NUMBER_OF_DIVISIONS, min, max, 'z', firstVertice );

	cMesh mapMesh;
	mapMesh.name = "cube";

	if( CreateMeshFromVectors( mapMesh, meshVertices, meshTriangles ) )
	{
		saveToPlyFile( meshTriangles, meshVertices );
		return mapMesh;
	}

}