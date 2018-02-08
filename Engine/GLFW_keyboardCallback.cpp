#include "globalOpenGL_GLFW.h"
#include "globalGameStuff.h"

#include <iostream>

bool isShiftKeyDown( int mods, bool bByItself = true );
bool isCtrlKeyDown( int mods, bool bByItself = true );
bool isAltKeyDown( int mods, bool bByItself = true );

/*static*/ void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GLFW_TRUE );

	if( key == GLFW_KEY_SPACE && action == GLFW_PRESS )
	{
		if( ::g_vecGameObjects[0]->textureBlend[0] == 1.0f )
		{
			::g_vecGameObjects[0]->textureBlend[0] = 0.0f;
			::g_vecGameObjects[0]->textureBlend[1] = 1.0f;
		}		
		else
		{
			::g_vecGameObjects[0]->textureBlend[0] = 1.0f;
			::g_vecGameObjects[0]->textureBlend[1] = 0.0f;
		}	
	}
	
	if( key == GLFW_KEY_ENTER && action == GLFW_PRESS )
	{
		::bIsWireframe = !::bIsWireframe;
	}

	//cGameObject* pTheBall = findObjectByFriendlyName( THEBALLNAME, ::g_vecGameObjects );

	const float CAMERASPEED = 0.1f;

	const float CAM_ACCELL_THRUST = 100.0f;

	//	const float CAMERASPEED = 100.0f;
	switch( key )
	{

	//case GLFW_KEY_N:
	//	if( pTheBall )
	//	{
	//		pTheBall->textureBlend[0] -= 0.01f;
	//		if( pTheBall->textureBlend[0] <= 0.0f )
	//		{
	//			pTheBall->textureBlend[0] = 0.0f;
	//		}
	//		pTheBall->textureBlend[1] = 1.0f - pTheBall->textureBlend[0];
	//	}
	//	break;
	//case GLFW_KEY_M:
	//	if( pTheBall )
	//	{
	//		pTheBall->textureBlend[0] += 0.01f;
	//		if( pTheBall->textureBlend[0] > 1.0f )
	//		{
	//			pTheBall->textureBlend[0] = 1.0f;
	//		}
	//		pTheBall->textureBlend[1] = 1.0f - pTheBall->textureBlend[0];
	//	}
	//	break;

	// CAMERA and lighting
	case GLFW_KEY_A:		// Left
		if( isShiftKeyDown( mods, true ) )
		{
			::g_pLightManager->vecLights[0].position.x -= CAMERASPEED;
			std::cout << "Light Position = "
				<< ::g_pLightManager->vecLights[0].position.x << ", "
				<< ::g_pLightManager->vecLights[0].position.y << ", "
				<< ::g_pLightManager->vecLights[0].position.z
				<< std::endl;
		}
		else
		{
			if( isAltKeyDown( mods, true ) )
			{

			}
		}
		break;
	case GLFW_KEY_D:		// Right
		if( isShiftKeyDown( mods, true ) )
		{
			::g_pLightManager->vecLights[0].position.x += CAMERASPEED;
			std::cout << "Light Position = "
				<< ::g_pLightManager->vecLights[0].position.x << ", "
				<< ::g_pLightManager->vecLights[0].position.y << ", "
				<< ::g_pLightManager->vecLights[0].position.z
				<< std::endl;
		}
		else
		{
			if( isAltKeyDown( mods, true ) )
			{
				
			}
		}
		break;
	case GLFW_KEY_W:		// Forward (along y)
		if( isShiftKeyDown( mods, true ) )
		{
			::g_pLightManager->vecLights[0].position.z += CAMERASPEED;
			std::cout << "Light Position = "
				<< ::g_pLightManager->vecLights[0].position.x << ", "
				<< ::g_pLightManager->vecLights[0].position.y << ", "
				<< ::g_pLightManager->vecLights[0].position.z
				<< std::endl;
		}
		else
		{
			if( isAltKeyDown( mods, true ) )
			{
				
			}
		}
		break;
	case GLFW_KEY_S:		// Backwards (along y)
		if( isShiftKeyDown( mods, true ) )
		{
			::g_pLightManager->vecLights[0].position.z -= CAMERASPEED;
			std::cout << "Light Position = "
				<< ::g_pLightManager->vecLights[0].position.x << ", "
				<< ::g_pLightManager->vecLights[0].position.y << ", "
				<< ::g_pLightManager->vecLights[0].position.z
				<< std::endl;
		}
		else
		{
			if( isAltKeyDown( mods, true ) )
			{

			}
		}
		break;
	case GLFW_KEY_Q:		// "Down" (along z axis)
		if( isShiftKeyDown( mods, true ) )
		{
			::g_pLightManager->vecLights[0].position.y -= CAMERASPEED;
			std::cout << "Light Position = "
				<< ::g_pLightManager->vecLights[0].position.x << ", "
				<< ::g_pLightManager->vecLights[0].position.y << ", "
				<< ::g_pLightManager->vecLights[0].position.z
				<< std::endl;
		}
		else
		{
			if( isCtrlKeyDown( mods, true ) )
			{

			}			
		}
		break;
	case GLFW_KEY_E:		// "Up" (along z axis)
		if( isShiftKeyDown( mods, true ) )
		{
			::g_pLightManager->vecLights[0].position.y += CAMERASPEED;
			std::cout << "Light Position = " 				
				<< ::g_pLightManager->vecLights[0].position.x << ", "
				<< ::g_pLightManager->vecLights[0].position.y << ", "
				<< ::g_pLightManager->vecLights[0].position.z
				<< std::endl;
		}
		else
		{
			if( isCtrlKeyDown( mods, true ) )
			{

			}
		}
		break;

	//case GLFW_KEY_1:
	//	::g_pLightManager->vecLights[0].attenuation.y *= 0.99f;	// less 1%
	//	std::cout << "Y Attenuation = " << ::g_pLightManager->vecLights[0].attenuation.y << std::endl;
	//	break;
	//case GLFW_KEY_2:
	//	::g_pLightManager->vecLights[0].attenuation.y *= 1.01f; // more 1%
	//	if( ::g_pLightManager->vecLights[0].attenuation.y <= 0.0f )
	//	{
	//		::g_pLightManager->vecLights[0].attenuation.y = 0.001f;	// Some really tiny value
	//	}
	//	std::cout << "Y Attenuation = " << ::g_pLightManager->vecLights[0].attenuation.y << std::endl;
	//	break;
	//case GLFW_KEY_3:	// Quad
	//	::g_pLightManager->vecLights[0].attenuation.z *= 0.99f;	// less 1%
	//	std::cout << "Z Attenuation = " << ::g_pLightManager->vecLights[0].attenuation.z << std::endl;
	//	break;
	//case GLFW_KEY_4:	//  Quad
	//	::g_pLightManager->vecLights[0].attenuation.z *= 1.01f; // more 1%
	//	if( ::g_pLightManager->vecLights[0].attenuation.z <= 0.0f )
	//	{
	//		::g_pLightManager->vecLights[0].attenuation.z = 0.001f;	// Some really tiny value
	//	}
	//	std::cout << "Z Attenuation = " << ::g_pLightManager->vecLights[0].attenuation.z << std::endl;
	//	break;

	case GLFW_KEY_UP:
		//pTheBall->position += glm::vec3( 0.0f, -0.1f, 0.0f );
		//pTheBall->vel += glm::vec3( 0.0f, -0.5f, 0.0f );
		break;

	case GLFW_KEY_DOWN:
		//pTheBall->position += glm::vec3( 0.0f, +0.1f, 0.0f );
		//pTheBall->vel += glm::vec3( 0.0f, +0.5f, 0.0f );
		break;

	case GLFW_KEY_LEFT:
		//pTheBall->position += glm::vec3( -0.1f, 0.0f, 0.0f ) ;
		//pTheBall->vel += glm::vec3( -0.5f, 0.0f, 0.0f );
		break;

	case GLFW_KEY_RIGHT:
		//pTheBall->position += glm::vec3( 0.1f, 0.0f, 0.0f );
		//pTheBall->vel += glm::vec3( 0.5f, 0.0f, 0.0f );
		break;

	//case GLFW_KEY_9:
	//	//pTheBall->position += glm::vec3( 0.0f, 0.0f, +0.1f );
	//	pTheBall->vel += glm::vec3( 0.0f, 0.0f, +0.5f );
	//	break;
	//case GLFW_KEY_0:
	//	//pTheBall->position += glm::vec3( 0.0f, 0.0f, -0.1f );
	//	pTheBall->vel += glm::vec3( 0.0f, 0.0f, -0.5f );
	//	break;

	}

	return;
}

// Helper functions
bool isShiftKeyDown( int mods, bool bByItself /*=true*/ )
{
	if( bByItself )
	{	// shift by itself?
		if( mods == GLFW_MOD_SHIFT )	{ return true; }
		else							{ return false; }
	}
	else
	{	// shift with anything else, too
		if( ( mods && GLFW_MOD_SHIFT ) == GLFW_MOD_SHIFT )	{ return true; }
		else												{ return false; }
	}
	// Shouldn't never get here, so return false? I guess?
	return false;
}

bool isCtrlKeyDown( int mods, bool bByItself /*=true*/ )
{
	if( bByItself )
	{	// shift by itself?
		if( mods == GLFW_MOD_CONTROL )	{ return true; }
		else							{ return false; }
	}
	else
	{	// shift with anything else, too
		if( ( mods && GLFW_MOD_CONTROL ) == GLFW_MOD_CONTROL )	{ return true; }
		else												{ return false; }
	}
	// Shouldn't never get here, so return false? I guess?
	return false;
}

bool isAltKeyDown( int mods, bool bByItself /*=true*/ )
{
	if( bByItself )
	{	// shift by itself?
		if( mods == GLFW_MOD_ALT ) { return true; }
		else { return false; }
	}
	else
	{	// shift with anything else, too
		if( ( mods && GLFW_MOD_ALT ) == GLFW_MOD_ALT ) { return true; }
		else { return false; }
	}
	// Shouldn't never get here, so return false? I guess?
	return false;
}