#include "globalOpenGL_GLFW.h"
#include "globalGameStuff.h"

#include <iostream>

bool isShiftKeyDown( int mods, bool bByItself = true );
bool isCtrlKeyDown( int mods, bool bByItself = true );
bool isAltKeyDown( int mods, bool bByItself = true );

float forceToApply = 0.2f;

bool findNextObject()
{
	bool found = false;
	int i = 0;
	if( ::g_selectedSphere == -1 )
	{
		i = 0;
	}		
	else
	{
		i = ::g_selectedSphere + 1;	// The next Object after the Current
		if( i > ::g_vecGameObjects.size() )
		{
			i = 0;
		}
	}
	
	while( !found )
	{
		for( ; i != ::g_vecGameObjects.size(); i++ )
		{
			if( ::g_vecGameObjects[i]->meshName == "ball" )
			{
				::g_selectedSphere = i;
				found = true;
				break;
			}
		}
		i = 0;
	}

	if( found ) return true;
	
	return false;
}

void setSpheresColor()
{
	for( int i = 0; i != ::g_vecGameObjects.size(); i++ )
	{
		if( i == ::g_selectedSphere )
		{
			::g_vecGameObjects[i]->textureBlend[0] = 0.0f;
			::g_vecGameObjects[i]->textureBlend[1] = 1.0f;
		}
		else
		{
			::g_vecGameObjects[i]->textureBlend[0] = 1.0f;
			::g_vecGameObjects[i]->textureBlend[1] = 0.0f;
		}
		
	}
	return;
}

/*static*/ void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		glfwSetWindowShouldClose( window, GLFW_TRUE );

	if( key == GLFW_KEY_SPACE && action == GLFW_PRESS )
	{	
		if( ::g_selectedSphere == NULL )
			findNextObject();

		setSpheresColor();

	}

	if( key == GLFW_KEY_TAB && action == GLFW_PRESS )
	{
		findNextObject();
		setSpheresColor();
		std::cout << "selected sphere :" << ::g_selectedSphere << std::endl;
	}
	
	if( key == GLFW_KEY_ENTER && action == GLFW_PRESS )
	{
		::bIsWireframe = !::bIsWireframe;
	}
	
	const float CAMERASPEED = 0.1f;

	const float CAM_ACCELL_THRUST = 100.0f;

	switch( key )
	{

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

	case GLFW_KEY_UP:
		if( ::g_selectedSphere != -1 )
		{	
			glm::vec3 target;
			::g_vecGameObjects[g_selectedSphere]->rigidBody->GetPosition( target );				
			glm::vec3 angle = target - ::g_pTheMouseCamera->Position;
			angle = glm::normalize( angle );
			angle *= ( 0.1f );

			::g_vecGameObjects[::g_selectedSphere]->rigidBody->ApplyImpulse( angle );
			//::g_vecGameObjects[::g_selectedSphere]->rigidBody->ApplyImpulse( glm::vec3( 0.0f, -forceToApply, 0.0f ) );
		}
		break;

	case GLFW_KEY_DOWN:
		if( ::g_selectedSphere != -1 )
		{
			glm::vec3 target;
			::g_vecGameObjects[g_selectedSphere]->rigidBody->GetPosition( target );
			glm::vec3 angle = target - ::g_pTheMouseCamera->Position;
			angle = glm::normalize( angle );
			angle *= ( 0.1f );

			angle *= ( -1.0f );

			::g_vecGameObjects[::g_selectedSphere]->rigidBody->ApplyImpulse( angle );
		}
		break;

	case GLFW_KEY_LEFT:
		if( ::g_selectedSphere != -1 )
		{
			glm::vec3 target;
			::g_vecGameObjects[g_selectedSphere]->rigidBody->GetPosition( target );
			glm::vec3 angle = target - ::g_pTheMouseCamera->Position;
			angle = glm::normalize( angle );

			float theta = glm::radians( 90.0f );
			float cs = cos( theta );
			float sn = sin( theta );
			glm::vec3 rotateAngle = glm::vec3( 0.0f );
			rotateAngle.x = angle.x * cs - angle.y * sn;
			rotateAngle.y = angle.x * sn + angle.y * cs;
			angle = rotateAngle;
			angle *= ( 0.1f );

			::g_vecGameObjects[::g_selectedSphere]->rigidBody->ApplyImpulse( angle );
		}		
		break;

	case GLFW_KEY_RIGHT:
		if( ::g_selectedSphere != -1 )
		{
			glm::vec3 target;
			::g_vecGameObjects[g_selectedSphere]->rigidBody->GetPosition( target );
			glm::vec3 angle = target - ::g_pTheMouseCamera->Position;
			angle = glm::normalize( angle );

			float theta = glm::radians( -90.0f );
			float cs = cos( theta );
			float sn = sin( theta );
			glm::vec3 rotateAngle = glm::vec3( 0.0f );
			rotateAngle.x = angle.x * cs - angle.y * sn;
			rotateAngle.y = angle.x * sn + angle.y * cs;
			angle = rotateAngle;
			angle *= ( 0.1f );

			::g_vecGameObjects[::g_selectedSphere]->rigidBody->ApplyImpulse( angle );			
		}
		break;

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