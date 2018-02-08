// Include glad and GLFW in correct order
#include "globalOpenGL_GLFW.h"

#include <iostream>						// C++ cin, cout, etc.
#include <glm/vec3.hpp>					// glm::vec3
#include <glm/vec4.hpp>					// glm::vec4
#include <glm/mat4x4.hpp>				// glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>			// glm::value_ptr
#include <glm/gtc/quaternion.hpp>

#include <stdlib.h>
#include <stdio.h>
// Add the file stuff library (file stream>
#include <fstream>
#include <sstream>						// "String stream"f
#include <istream>
#include <string>
#include <vector>						// smart array, "array" in most languages
#include <random>
#include <time.h>

#include "Utilities.h"
#include "ModelUtilities.h"
#include "cMesh.h"
#include "cShaderManager.h" 
#include "cGameObject.h"
#include "cVAOMeshManager.h"

#include "Physics.h"

#include "cLightManager.h"


// Include all the things that are accessed in other files
#include "globalGameStuff.h"

#include "cMouseCamera.h"

// Euclides: Control selected object for movement
int g_GameObjNumber = 0;				// game object vector position number 
int g_LightObjNumber = 0;				// light object vector position

int g_targetShip = 0;
int g_selectedShip = 0;

bool g_lookAtON = false;
bool g_moveRiders = false;
bool g_movingViper = false;

int g_NUMBER_OF_LIGHTS = 2;

bool bIsWireframe;

// Remember to #include <vector>...
std::vector< cGameObject* > g_vecGameObjects;

cMouseCamera* g_pTheMouseCamera = NULL;
float g_lastX = 0.0f;
float g_lastY = 0.0f;
bool g_firstMouse = true;

cVAOMeshManager*	g_pVAOManager = 0;		// or NULL or nullptr

cShaderManager*		g_pShaderManager;		// Heap, new (and delete)
cLightManager*		g_pLightManager;

cBasicTextureManager*	g_pTextureManager = 0;

cDebugRenderer*			g_pDebugRenderer = 0;

// This contains the AABB grid for the terrain...
cAABBBroadPhase* g_terrainAABBBroadPhase = 0;

// Other uniforms:
GLint uniLoc_materialDiffuse = -1;
GLint uniLoc_materialAmbient = -1;
GLint uniLoc_ambientToDiffuseRatio = -1; 	// Maybe	// 0.2 or 0.3
GLint uniLoc_materialSpecular = -1;  // rgb = colour of HIGHLIGHT only
									 // w = shininess of the 
GLint uniLoc_bIsDebugWireFrameObject = -1;

GLint uniLoc_eyePosition = -1;	// Camera position
GLint uniLoc_mModel = -1;
GLint uniLoc_mView = -1;
GLint uniLoc_mProjection = -1;

struct sWindowConfig
{
public:
	int height = 480;
	int width = 640;
	std::string title = "TO BE REPLACED...";
};

struct sGOparameters		// for the Game Objects' input file
{
	std::string meshname;
	int nObjects;
	float x, y, z, scale;
	std::string random;
	float rangeX, rangeY, rangeZ, rangeScale;
};

struct sMeshparameters		// for the Meshes' input file
{
	std::string meshname;
	std::string meshFilename;
};

//struct pointTriangles
//{
//	glm::vec3 point;
//	sTriAABB triangle;
//};

//std::vector< glm::vec3 > g_vecPoints;
std::vector< pointTriangles > g_vecPoints;

// Forward declare the Functions
void loadConfigFile( std::string fileName, sWindowConfig& wConfig );
sGOparameters parseObjLine( std::ifstream &source );
void loadObjectsFile( std::string fileName );
sMeshparameters parseMeshLine( std::ifstream &source );
void loadMeshesFile( std::string fileName, GLint ShaderID );
void loadLightObjects();
void PhysicsStep( double deltaTime );
void DrawObject( cGameObject* pTheGO );
float generateRandomNumber( float min, float max );
void mouse_callback( GLFWwindow* window, double xpos, double ypos );
void scroll_callback( GLFWwindow* window, double xoffset, double yoffset );
void ProcessCameraInput( GLFWwindow *window, double deltaTime );

void DrawRenderStuff( glm::mat4 view, glm::mat4 projection )
{
	for( int index = 0; index != ::g_vecGameObjects.size(); index++ )
	{
		cGameObject* pTheGO = ::g_vecGameObjects[index];
		if( pTheGO->meshName == "ball" )
		{
			glm::vec3 color = glm::vec3( 1.0f, 1.0f, 0.0f );
			glm::vec3 center = pTheGO->position;
			glm::vec3 radiusEnd = center + glm::vec3( pTheGO->radius, 0.0f, 0.0f );
			::g_pDebugRenderer->addLine( center, radiusEnd, color, false );

			radiusEnd = center + glm::vec3( 0.0f, pTheGO->radius, 0.0f );
			::g_pDebugRenderer->addLine( center, radiusEnd, color, false );

			radiusEnd = center + glm::vec3( 0.0f, 0.0f, pTheGO->radius );
			::g_pDebugRenderer->addLine( center, radiusEnd, color, false );
		}
	}
	::g_pDebugRenderer->RenderDebugObjects( view, projection );
}


static void error_callback( int error, const char* description )
{
	fprintf( stderr, "Error: %s\n", description );
}

int main( void )
{
	GLFWwindow* window;
	GLint mvp_location; //vpos_location, vcol_location;
	glfwSetErrorCallback( error_callback );

	if( !glfwInit() )
		exit( EXIT_FAILURE );

	sWindowConfig wConfig;

	loadConfigFile( "config.txt", wConfig );
	
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );

	#ifdef NDEBUG				// nondebug
		window = glfwCreateWindow( wConfig.width, wConfig.height,
			wConfig.title.c_str(),
			glfwGetPrimaryMonitor(),	// Runs in full screen
			NULL );
	#else						// debug code
		window = glfwCreateWindow( wConfig.width, wConfig.height,
			wConfig.title.c_str(),
			NULL, 
			NULL );
	#endif


	if( !window )
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}

	glfwSetKeyCallback( window, key_callback );
	
	glfwMakeContextCurrent( window );
	gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress );
	glfwSwapInterval( 1 );
	glfwSetCursorPosCallback( window, mouse_callback );
	glfwSetScrollCallback( window, scroll_callback );

	std::cout << glGetString( GL_VENDOR ) << " "
		<< glGetString( GL_RENDERER ) << ", "
		<< glGetString( GL_VERSION ) << std::endl;
	std::cout << "Shader language version: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

	::g_pShaderManager = new cShaderManager();

	cShaderManager::cShader vertShader;
	cShaderManager::cShader fragShader;

	vertShader.fileName = "simpleVert.glsl";
	fragShader.fileName = "simpleFrag.glsl";

	// General error string, used for a number of items during start up
	std::string error;

	::g_pShaderManager->setBasePath( "assets//shaders//" );

	// Shader objects are passed by reference so that
	//	we can look at the results if we wanted to. 
	if( !::g_pShaderManager->createProgramFromFile(
		"mySexyShader", vertShader, fragShader ) )
	{
		std::cout << "Oh no! All is lost!!! Blame Loki!!!" << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
		// Should we exit?? 
		return -1;
		//		exit(
	}
	std::cout << "The shaders compiled and linked OK" << std::endl;

	::g_pDebugRenderer = new cDebugRenderer();
	if( !::g_pDebugRenderer->initialize( error ) )
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
	}

	//Load models
	::g_pVAOManager = new cVAOMeshManager();

	GLint sexyShaderID = ::g_pShaderManager->getIDFromFriendlyName( "mySexyShader" );

	loadMeshesFile( "meshlist.txt", sexyShaderID );
	loadObjectsFile( "objects.txt" );
	//newMapObject( sexyShaderID );

	GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName( "mySexyShader" );

	// Get the uniform locations for this shader
	mvp_location = glGetUniformLocation( currentProgID, "MVP" );		// program, "MVP");
	uniLoc_materialDiffuse = glGetUniformLocation( currentProgID, "materialDiffuse" );
	uniLoc_materialAmbient = glGetUniformLocation( currentProgID, "materialAmbient" );
	uniLoc_ambientToDiffuseRatio = glGetUniformLocation( currentProgID, "ambientToDiffuseRatio" );
	uniLoc_materialSpecular = glGetUniformLocation( currentProgID, "materialSpecular" );
	uniLoc_eyePosition = glGetUniformLocation( currentProgID, "eyePosition" );

	uniLoc_mModel = glGetUniformLocation( currentProgID, "mModel" );
	uniLoc_mView = glGetUniformLocation( currentProgID, "mView" );
	uniLoc_mProjection = glGetUniformLocation( currentProgID, "mProjection" );

	::g_pLightManager = new cLightManager();

	::g_pLightManager->CreateLights( g_NUMBER_OF_LIGHTS );	// There are 10 lights in the shader
	// Change ZERO (the SUN) light position
	::g_pLightManager->vecLights[0].position = glm::vec3( 0.0f, 0.0f, 2.0f );
	::g_pLightManager->vecLights[0].attenuation.y = 1.0f;		// Change the linear attenuation
	::g_pLightManager->vecLights[0].attenuation.z = 0.2f;		

	// Change Other lights parameters==========================
	{
		//::g_pLightManager->vecLights[1].position = glm::vec3( 0.0f, 0.0f, 50.0f );
		//::g_pLightManager->vecLights[1].attenuation.y = 0.0017f;
	}
	//=========================================================
	::g_pLightManager->LoadShaderUniformLocations( currentProgID );

	//loadLightObjects();

	// Texture 
	::g_pTextureManager = new cBasicTextureManager();
	::g_pTextureManager->SetBasePath( "assets/textures" );
	if( !::g_pTextureManager->Create2DTextureFromBMPFile( "Rough_rock_014_COLOR.bmp", true ) )
	{
		std::cout << "Didn't load the texture. Oh no!" << std::endl;
	}
	else
	{
		std::cout << "Texture is loaded! Hazzah!" << std::endl;
	}
	::g_pTextureManager->Create2DTextureFromBMPFile( "Rough_rock_015_COLOR.bmp", true );
	::g_pTextureManager->Create2DTextureFromBMPFile( "Red_Marble_001_COLOR.bmp", true );
	
	cMesh terrainMesh;
	
	//::g_pTheMouseCamera = new cMouseCamera( glm::vec3( 5.0f, 5.0f, 3.0f ) );
	//::g_pTheMouseCamera = new cMouseCamera( glm::vec3( 5.0f, 5.0f, 3.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), -90.f, 0.f );
	::g_pTheMouseCamera = new cMouseCamera( glm::vec3( 4.0f, 4.0f, 4.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), -135.f, -32.f );

	//std::cout << "Camera Pos: "
	//	<< ::g_pTheMouseCamera->Position.x << ", "
	//	<< ::g_pTheMouseCamera->Position.y << ", "
	//	<< ::g_pTheMouseCamera->Position.z
	//	<< " | WorldUp: "
	//	<< ::g_pTheMouseCamera->WorldUp.x << ", "
	//	<< ::g_pTheMouseCamera->WorldUp.y << ", "
	//	<< ::g_pTheMouseCamera->WorldUp.z
	//	<< " | Yaw: " << ::g_pTheMouseCamera->Yaw
	//	<< " | Pitch: " << ::g_pTheMouseCamera->Pitch
	//	<< std::endl;

	::g_lastX = wConfig.width / 2.0f;
	::g_lastY = wConfig.height / 2.0f;

	glEnable( GL_DEPTH );

	// Gets the "current" time "tick" or "step"
	double lastTimeStep = glfwGetTime();

	// Main game or application loop
	while( !glfwWindowShouldClose( window ) )
	{
		float ratio;
		int width, height;
		glm::mat4x4 matProjection;			// was "p"

		glfwGetFramebufferSize( window, &width, &height );
		ratio = width / ( float )height;
		glViewport( 0, 0, width, height );

		// Clear colour AND depth buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		::g_pShaderManager->useShaderProgram( "mySexyShader" );
		GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName( "mySexyShader" );

		// Update all the light uniforms...
		// (for the whole scene)
		::g_pLightManager->CopyLightInformationToCurrentShader();

		// Changing the projection to include zoom 
		// Projection and view don't change per scene (maybe)
		matProjection = glm::perspective( glm::radians( ::g_pTheMouseCamera->Zoom ),
						ratio,
						1.0f,			// Near (as big as possible)
						100000.0f );	// Far (as small as possible)

		// View or "camera" matrix
		glm::mat4 matView = glm::mat4( 1.0f );	// was "v"

		// Now the view matrix is taken right from the camera class
		matView = ::g_pTheMouseCamera->GetViewMatrix();

		glUniformMatrix4fv( uniLoc_mView, 1, GL_FALSE,
			( const GLfloat* )glm::value_ptr( matView ) );
		glUniformMatrix4fv( uniLoc_mProjection, 1, GL_FALSE,
			( const GLfloat* )glm::value_ptr( matProjection ) );

		// Set ALL texture units and binding for ENTIRE SCENE (is faster)
		{
			// 0 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,
				::g_pTextureManager->getTextureIDFromName("Rough_rock_014_COLOR.bmp"));
				//::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[0]));
			// 1
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,
				::g_pTextureManager->getTextureIDFromName("Rough_rock_015_COLOR.bmp"));
				//::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[1]));
			// 2..  and so on... 
			glActiveTexture( GL_TEXTURE2 );
			glBindTexture( GL_TEXTURE_2D,
				::g_pTextureManager->getTextureIDFromName( "Red_Marble_001_COLOR.bmp" ) );
			//::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[1]));

			// Set sampler in the shader
			// NOTE: You shouldn't be doing this during the draw call...
			GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");
			GLint textSampler00_ID = glGetUniformLocation(curShaderID, "myAmazingTexture00");
			GLint textSampler01_ID = glGetUniformLocation(curShaderID, "myAmazingTexture01");
			// And so on (up to 10, or whatever number of textures)... 

			GLint textBlend00_ID = glGetUniformLocation(curShaderID, "textureBlend00");
			GLint textBlend01_ID = glGetUniformLocation(curShaderID, "textureBlend01");

			// This connects the texture sampler to the texture units... 
			glUniform1i(textSampler00_ID, 0);		// Enterprise
			glUniform1i(textSampler01_ID, 1);		// GuysOnSharkUnicorn
		}


		// Enable blend ("alpha") transparency for the scene
		// NOTE: You "should" turn this OFF, then draw all NON-Transparent objects
		//       Then turn ON, sort objects from far to near ACCORDING TO THE CAMERA
		//       and draw them
		glEnable( GL_BLEND );		// Enables "blending"
									//glDisable( GL_BLEND );
									// Source == already on framebuffer
									// Dest == what you're about to draw
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );



		// Draw the scene
		unsigned int sizeOfVector = ( unsigned int )::g_vecGameObjects.size();	//*****//
		for( int index = 0; index != sizeOfVector; index++ )
		{
			cGameObject* pTheGO = ::g_vecGameObjects[index];

			DrawObject( pTheGO );						

		}//for ( int index = 0...

		if( bIsWireframe )
		{
			DrawRenderStuff( matView, matProjection );
		}

		std::map< unsigned long long, cAABBv2* >::iterator itAABB;

		std::stringstream ssTitle;
		ssTitle << "Physics 2: Project 1";

		glfwSetWindowTitle( window, ssTitle.str().c_str() );

		glfwSwapBuffers( window );
		glfwPollEvents();

		// Essentially the "frame time"
		// Now many seconds that have elapsed since we last checked
		double curTime = glfwGetTime();
		double deltaTime = curTime - lastTimeStep;

		// Update camera
		ProcessCameraInput( window, deltaTime );

		// Physics Calculation
		PhysicsStep( deltaTime );
		
		lastTimeStep = curTime;

	}// while ( ! glfwWindowShouldClose(window) )


	glfwDestroyWindow( window );
	glfwTerminate();

	// 
	delete ::g_pShaderManager;
	delete ::g_pVAOManager;

	//    exit(EXIT_SUCCESS);
	return 0;
}

//Load Config.txt
void loadConfigFile( std::string fileName, sWindowConfig& wConfig )
{
	// TODO change this config formating
	std::ifstream infoFile( fileName );
	if( !infoFile.is_open() )
	{	// File didn't open...
		std::cout << "Can't find config file" << std::endl;
		std::cout << "Using defaults" << std::endl;
	}
	else
	{	// File DID open, so read it... 
		std::string a;

		infoFile >> a;	// "Game"	//std::cin >> a;
		infoFile >> a;	// "Config"
		infoFile >> a;	// "width"
		infoFile >> wConfig.width;	// 1080
		infoFile >> a;	// "height"
		infoFile >> wConfig.height;	// 768
		infoFile >> a;		// Title_Start

		std::stringstream ssTitle;		// Inside "sstream"
		bool bKeepReading = true;
		do
		{
			infoFile >> a;		// Title_End??
			if( a != "Title_End" )
			{
				ssTitle << a << " ";
			}
			else
			{	// it IS the end! 
				bKeepReading = false;
				wConfig.title = ssTitle.str();
			}
		} while( bKeepReading );
	}
}

// Generate real random numbers
float generateRandomNumber( float min, float max )
{

	time_t curTime;
	time( &curTime );

	unsigned int seed = (int)time;

	std::default_random_engine generator( seed );
	std::uniform_real_distribution<float> distribution( min, max );

	float randomNumber = 0.0;

	randomNumber = distribution( generator );
	return randomNumber;

}

bool CheckDistance( glm::vec3 position, glm::vec3 fromPoint, float minDistance, float maxDistance )
{
	float thisDistance = 0.0f;
	thisDistance = glm::distance( position, fromPoint );

	if( thisDistance >= minDistance || thisDistance <= maxDistance ) return true;

	return false;
}

glm::vec3 GetRandomPosition()
{
	glm::vec3 center = glm::vec3( 0.0f );
	glm::vec3 newPosition = glm::vec3( 0.0f );
	bool validPosition = false;

	while( !validPosition )
	{
		newPosition = glm::vec3( generateRandomNumber( -200, 200 ),
			generateRandomNumber( -200, 200 ),
			generateRandomNumber( -200, 200 ) );
		validPosition = CheckDistance( newPosition, center, 30.0f, 100.0f );
	}

	return newPosition;
}

//Load objects.txt
void loadObjectsFile( std::string fileName )
{
	//sGOparameters sGOpar;
	std::vector <sGOparameters> allObjects;

	std::ifstream objectsFile( fileName );
	if( !objectsFile.is_open() )
	{	// File didn't open...
		std::cout << "Can't find config file" << std::endl;
		std::cout << "Using defaults" << std::endl;
	}
	else
	{	// File DID open, so loop through the file and pushback to structure
		while( !objectsFile.eof() && objectsFile.is_open() ) {
			allObjects.push_back( parseObjLine( objectsFile ) );
		}
		objectsFile.close();  //Closing "costfile.txt"
	}

	for( int index = 0; index != allObjects.size(); index++ )
	{
		// Check, Number of Objects must be at least 1
		if( allObjects[index].nObjects == 0 ) allObjects[index].nObjects = 1;

		// Create the number of gameObjects specified in the file for each line 
		for( int i = 0; i != allObjects[index].nObjects; i++ )
		{
			// Create a new GO
			cGameObject* pTempGO = new cGameObject();

			pTempGO->meshName = allObjects[index].meshname; // Set the name of the mesh

			//pTempGO->diffuseColour = glm::vec4( 0.5f, 0.5f, 0.5f, 1.0f );
			pTempGO->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
			//pTempGO->rotation = glm::vec3( 0.0f );
			pTempGO->overwrtiteQOrientationFormEuler( glm::vec3( 0.0f, 0.0f, 0.0f ) );


			// SOME OBJECTS ARE RANDOMLY PLACED WHEN RANDOM=TRUE ON FILE
			if( allObjects[index].random == "true" )
			{
				pTempGO->position.x = generateRandomNumber( -allObjects[index].rangeX, allObjects[index].rangeX );
				pTempGO->position.y = generateRandomNumber( -allObjects[index].rangeY, allObjects[index].rangeY );
				pTempGO->position.z = generateRandomNumber( -allObjects[index].rangeZ, allObjects[index].rangeZ );
				pTempGO->scale = allObjects[index].rangeScale;
			}
			else
			{   // position and scale are fixed
				pTempGO->position.x = allObjects[index].x;
				pTempGO->position.y = allObjects[index].y;
				pTempGO->position.z = allObjects[index].z;
				pTempGO->scale = allObjects[index].scale;
			}
			// NO VELOCITY
			pTempGO->vel = glm::vec3( 0.0f );
			if( pTempGO->meshName == "ball" )
			{
				pTempGO->textureBlend[0] = 1.0f;
				pTempGO->textureNames[0] = "Rough_rock_015_COLOR.bmp";
				pTempGO->textureBlend[1] = 0.0f;
				pTempGO->textureNames[1] = "Red_Marble_001_COLOR.bmp";

				cMesh tempMesh;
				::g_pVAOManager->lookupMeshFromName( "ball", tempMesh );				
				pTempGO->radius = tempMesh.maxExtent / 2 * pTempGO->scale;

			}
			else
			{
				pTempGO->textureBlend[0] = 1.0f;
				pTempGO->textureNames[0] = "Rough_rock_014_COLOR.bmp";
				pTempGO->bIsUpdatedInPhysics = false;
			}

			::g_vecGameObjects.push_back( pTempGO );
		}
	}
}

// Parse the file line to fit into the structure
sGOparameters parseObjLine( std::ifstream &source ) {

	sGOparameters sGOpar;

	//Scanning a line from the file
	source >> sGOpar.meshname >> sGOpar.nObjects
		>> sGOpar.x >> sGOpar.y >> sGOpar.z >> sGOpar.scale
		>> sGOpar.random
		>> sGOpar.rangeX >> sGOpar.rangeY >> sGOpar.rangeZ
		>> sGOpar.rangeScale;


	return sGOpar;
}

//Load meshlist.txt
void loadMeshesFile( std::string fileName, GLint ShaderID )
{
	std::vector <sMeshparameters> allMeshes;

	std::ifstream objectsFile( fileName );
	if( !objectsFile.is_open() )
	{	// File didn't open...
		std::cout << "Can't find config file" << std::endl;
		std::cout << "Using defaults" << std::endl;
	}
	else
	{	// File DID open, so loop through the file and pushback to structure
		while( !objectsFile.eof() && objectsFile.is_open() ) {
			allMeshes.push_back( parseMeshLine( objectsFile ) );
		}
		objectsFile.close();  //Closing "costfile.txt"
	}

	for( int index = 0; index != allMeshes.size(); index++ )
	{
		cMesh testMesh;
		testMesh.name = allMeshes[index].meshname;
		//if( !LoadPlyFileIntoMesh( allMeshes[index].meshFilename, testMesh ) )
		if( !LoadPlyFileIntoMeshWithUV( allMeshes[index].meshFilename, testMesh ) )
		{
			std::cout << "Didn't load model" << std::endl;
			// do something??
		}
		if( testMesh.name == "ball" )
		{
			if( !::g_pVAOManager->loadMeshIntoVAO( testMesh, ShaderID, true ) )
			{
				std::cout << "Could not load mesh into VAO" << std::endl;
			}
		}
		else
		{
			if( !::g_pVAOManager->loadMeshIntoVAO( testMesh, ShaderID ) )
			{
				std::cout << "Could not load mesh into VAO" << std::endl;
			}
		}		
	}
}

// Parse the file line to fit into the structure
sMeshparameters parseMeshLine( std::ifstream &source ) {

	sMeshparameters sMeshpar;

	//Scanning a line from the file
	source >> sMeshpar.meshname >> sMeshpar.meshFilename;

	return sMeshpar;
}

void loadLightObjects()
{
	for( int index = 0; index < g_NUMBER_OF_LIGHTS; index++ )
	{
		// Create a new GO
		cGameObject* pTempGO = new cGameObject();

		pTempGO->meshName = "sphere"; // Set the name of the mesh

									  // position is based on light position
		pTempGO->position = ::g_pLightManager->vecLights[index].position;

		if( index == 0 ) pTempGO->scale = 3.0f;
		else pTempGO->scale = 1.0f;

		// Each light is initially white
		pTempGO->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );

		pTempGO->bIsLight = true;
		pTempGO->myLight = index;

		::g_vecGameObjects.push_back( pTempGO );
	}
}

// Update the world 1 "step" in time
void PhysicsStep( double deltaTime )
{
	// Distance                          m
	// Velocity = distance / time		 m/s
	// Accleration = velocity / time     m/s/s

	// Distance = time * velocity
	// velocity = time * acceleration

	glm::vec3 triangleNormal = glm::vec3( 0.0f );

	// NO GRAVITY
	const glm::vec3 GRAVITY = glm::vec3( 0.0f, 0.0f, -1.1f );
	//const glm::vec3 GRAVITY = glm::vec3( 0.0f, 0.0f, 0.0f );

	// Identical to the 'render' (drawing) loop
	for( int index = 0; index != ::g_vecGameObjects.size(); index++ )
	{
		cGameObject* pCurGO = ::g_vecGameObjects[index];

		// Is this object to be updated?
		if( !pCurGO->bIsUpdatedInPhysics )
		{	// DON'T update this
			continue;		// Skip everything else in the for
		}

		pCurGO->prevPosition = pCurGO->position;

		// Explicity Euler integration (RK4)
		// New position is based on velocity over time
		glm::vec3 deltaPosition = ( float )deltaTime * pCurGO->vel;
		pCurGO->position += deltaPosition;

		// New velocity is based on acceleration over time
		glm::vec3 deltaVelocity;
		deltaVelocity = ( ( float )deltaTime * pCurGO->accel ) + ( ( float )deltaTime * GRAVITY );

		pCurGO->vel += deltaVelocity;

		//unsigned long long ballID = cAABBv2::calculateID( pCurGO->position );

		//cAABBv2* theCurAABB = new cAABBv2( pCurGO->position, 1.0f );

		//::g_terrainAABBBroadPhase->areThereTrianglesInAABB( pCurGO->position, theCurAABB );			

		// Stores the closests points in a Vector to be used bellow
		//glm::vec3 thePointToTest = pCurGO->position;
		////::g_vecPoints = 
		//::g_vecPoints = findClosestPointsOfAABB( thePointToTest, theCurAABB );

		switch( pCurGO->typeOfObject )
		{
		case eTypeOfObject::SPHERE:
			//	// Compare this to EVERY OTHER object in the scene
			for( int indexEO = 0; indexEO != ::g_vecGameObjects.size(); indexEO++ )
			{
				// Don't test for myself
				if( index == indexEO )
					continue;		// It's me!!

				cGameObject* pOtherObject = ::g_vecGameObjects[indexEO];
				// Is another object

				switch( pOtherObject->typeOfObject )
				{
				case eTypeOfObject::SPHERE:

					break;
				case eTypeOfObject::PLANE:

					//for( int i_point = 0; i_point != ::g_vecPoints.size(); i_point++ )
					//{	// Check if any point is in contact with the pCurGO

					//	// TODO Implement the collision with Y axis as well, this would be the logic
					//	// The Pythagorean distance 
					//	float distance = glm::distance( pCurGO->position, glm::vec3( ::g_vecPoints[i_point].point.x,
					//		::g_vecPoints[i_point].point.y,
					//		::g_vecPoints[i_point].point.z ) );
					//	float tempRadius = pCurGO->radius; // *1.2;

					//	if( distance <= pCurGO->radius )
					//	{	// COLLISION!!

					//		glm::vec3 triVertex[3];
					//		triVertex[0] = g_vecPoints[i_point].triangle.verts[0];
					//		triVertex[1] = g_vecPoints[i_point].triangle.verts[1];
					//		triVertex[2] = g_vecPoints[i_point].triangle.verts[2];							

					//		glm::vec3 theCenter = ::g_vecPoints[i_point].triangle.calcCentre();

					//		triangleNormal = returnNormal2( triVertex, theCenter );

					//		float length = glm::distance( theCenter, triangleNormal );

					//		bounceSphereAgainstPlane( pCurGO, pOtherObject, triangleNormal );
					//		
					//		// Return sphere to previous position before the impact
					//		if( pCurGO->prevPosition != glm::vec3( NULL ) )
					//			pCurGO->position = pCurGO->prevPosition;
					//		
					//		break;
					//	}
					//}
					break;
				}
			}
		}

		// Calculate the normal using the center as reference
		glm::vec3 velEndPoint = pCurGO->vel + pCurGO->position;
		glm::vec3 velBeginPoint = ( glm::normalize( pCurGO->vel ) * glm::vec3( pCurGO->radius, pCurGO->radius, pCurGO->radius ) );
		glm::vec3 color = glm::vec3( 1.0f, 1.0f, 1.0f );

		::g_pDebugRenderer->addLine( pCurGO->position, velEndPoint, color, false );

		//break;

	}//for ( int index...

	return;
}

// Draw a single object
void DrawObject( cGameObject* pTheGO )
{
	// Is there a game object? 
	if( pTheGO == 0 )	//if ( ::g_GameObjects[index] == 0 )
	{	// Nothing to draw
		return;		// Skip all for loop code and go to next
	}

	// Was near the draw call, but we need the mesh name
	std::string meshToDraw = pTheGO->meshName;		//::g_GameObjects[index]->meshName;

	sVAOInfo VAODrawInfo;
	if( ::g_pVAOManager->lookupVAOFromName( meshToDraw, VAODrawInfo ) == false )
	{	// Didn't find mesh
		std::cout << "WARNING: Didn't find mesh " << meshToDraw << " in VAO Manager" << std::endl;
		return;
	}

	// There IS something to draw

	// 'model' or 'world' matrix
	glm::mat4x4 mModel = glm::mat4x4( 1.0f );	//		mat4x4_identity(m);

	glm::mat4 trans = glm::mat4x4( 1.0f );
	trans = glm::translate( trans,
		pTheGO->position );
	mModel = mModel * trans;

	// Now with quaternion rotation
	// Like many things in GML, the conversion is done in the constructor
	glm::mat4 postRotQuat = glm::mat4( pTheGO->qOrientation );
	mModel = mModel * postRotQuat;

	// assume that scale to unit bounding box
	// ************* BEWARE *****************
	//			float finalScale = VAODrawInfo.scaleForUnitBBox * ::g_vecGameObjects[index]->scale;
	// We have taken out the scale adjustment so the scale is AS IT IS FROM THE MODEL
	float finalScale = pTheGO->scale;

	glm::mat4 matScale = glm::mat4x4( 1.0f );
	matScale = glm::scale( matScale,
		glm::vec3( finalScale,
			finalScale,
			finalScale ) );
	mModel = mModel * matScale;

	glUniformMatrix4fv( uniLoc_mModel, 1, GL_FALSE,
		( const GLfloat* )glm::value_ptr( mModel ) );
	
	glm::mat4 mWorldInTranpose = glm::inverse( glm::transpose( mModel ) );
	
	glUniform4f( uniLoc_materialDiffuse,
		pTheGO->diffuseColour.r,
		pTheGO->diffuseColour.g,
		pTheGO->diffuseColour.b,
		pTheGO->diffuseColour.a );
	//...and all the other object material colours

	if( pTheGO->bIsWireFrame )
	{
		glUniform1f( uniLoc_bIsDebugWireFrameObject, 1.0f );	// TRUE
	}
	else
	{
		glUniform1f( uniLoc_bIsDebugWireFrameObject, 0.0f );	// FALSE
	}

	// Set up the textures
	std::string textureName = pTheGO->textureNames[0];
	GLuint texture00Number
		= ::g_pTextureManager->getTextureIDFromName( textureName );
	// Texture binding... (i.e. set the 'active' texture
	GLuint texture00Unit = 0;							// Texture units go from 0 to 79 (at least)
	glActiveTexture( texture00Unit + GL_TEXTURE0 );		// GL_TEXTURE0 = 33984
	glBindTexture( GL_TEXTURE_2D, texture00Number );

	// 0 
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D,
		::g_pTextureManager->getTextureIDFromName( pTheGO->textureNames[0] ) );
	// 1
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_2D,
		::g_pTextureManager->getTextureIDFromName( pTheGO->textureNames[1] ) );
	// 2..  and so on... 

	// Set sampler in the shader
	// NOTE: You shouldn't be doing this during the draw call...
	GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName( "mySexyShader" );
	GLint textSampler00_ID = glGetUniformLocation( curShaderID, "myAmazingTexture00" );
	GLint textSampler01_ID = glGetUniformLocation( curShaderID, "myAmazingTexture01" );
	//// And so on (up to 10, or whatever number of textures)... 

	GLint textBlend00_ID = glGetUniformLocation( curShaderID, "textureBlend00" );
	GLint textBlend01_ID = glGetUniformLocation( curShaderID, "textureBlend01" );

	//// This connects the texture sampler to the texture units... 
	//glUniform1i( textSampler00_ID, 0  );		// Enterprise
	//glUniform1i( textSampler01_ID, 1  );		// GuysOnSharkUnicorn
	// .. and so on

	// And the blending values
	glUniform1f( textBlend00_ID, pTheGO->textureBlend[0] );
	glUniform1f( textBlend01_ID, pTheGO->textureBlend[1] );
	// And so on...
	
	if( bIsWireframe )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
//		glEnable(GL_DEPTH_TEST);		// Test for z and store in z buffer
		glDisable( GL_CULL_FACE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );	// Default
		glEnable( GL_DEPTH_TEST );		// Test for z and store in z buffer
		glEnable( GL_CULL_FACE );
	}

	glCullFace( GL_BACK );	

	glBindVertexArray( VAODrawInfo.VAO_ID );

	glDrawElements( GL_TRIANGLES,
		VAODrawInfo.numberOfIndices,		// testMesh.numberOfTriangles * 3,	// How many vertex indices
		GL_UNSIGNED_INT,					// 32 bit int 
		0 );
	// Unbind that VAO
	glBindVertexArray( 0 );

	return;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback( GLFWwindow* window, double xpos, double ypos )
{
	if( ::g_firstMouse )
	{
		::g_lastX = xpos;
		::g_lastY = ypos;
		::g_firstMouse = false;
	}

	//float xoffset = xpos - ::g_lastX;
	//float yoffset = ::g_lastY - ypos; // reversed since y-coordinates go from bottom to top
	float xoffset = ::g_lastX - xpos;
	float yoffset = ::g_lastY - ypos; // reversed since y-coordinates go from bottom to top

	::g_lastX = xpos;
	::g_lastY = ypos;

	::g_pTheMouseCamera->ProcessMouseMovement( xoffset, yoffset );
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback( GLFWwindow* window, double xoffset, double yoffset )
{
	::g_pTheMouseCamera->ProcessMouseScroll( yoffset );
}

void printCameraDetails()
{
	std::cout << "Camera Pos: "
		<< ::g_pTheMouseCamera->Position.x << ", "
		<< ::g_pTheMouseCamera->Position.y << ", "
		<< ::g_pTheMouseCamera->Position.z
		<< " | WorldUp: "
		<< ::g_pTheMouseCamera->WorldUp.x << ", "
		<< ::g_pTheMouseCamera->WorldUp.y << ", "
		<< ::g_pTheMouseCamera->WorldUp.z
		<< " | Yaw: " << ::g_pTheMouseCamera->Yaw
		<< " | Pitch: " << ::g_pTheMouseCamera->Pitch
		<< std::endl;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void ProcessCameraInput( GLFWwindow *window, double deltaTime )
{
	if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
		glfwSetWindowShouldClose( window, true );

	if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS )
		::g_pTheMouseCamera->ProcessKeyboard( FORWARD, deltaTime );
	if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS )
		::g_pTheMouseCamera->ProcessKeyboard( BACKWARD, deltaTime );
	if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS )
		::g_pTheMouseCamera->ProcessKeyboard( LEFT, deltaTime );
	if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS )
		::g_pTheMouseCamera->ProcessKeyboard( RIGHT, deltaTime );
	if( glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS )
		::g_pTheMouseCamera->ProcessKeyboard( UP, deltaTime );
	if( glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS )
		::g_pTheMouseCamera->ProcessKeyboard( DOWN, deltaTime );
}