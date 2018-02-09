#pragma once
//#include <glm/game_math.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp> // <- This includes vec3 and mat4

#include "iShape.h"

namespace nPhysics
{
	class iRigidBody
	{
	public:
		virtual ~iRigidBody() {}

		virtual iShape* GetShape() = 0;

		virtual void GetTransform( glm::mat4& transformOut) = 0;
		virtual void SetTransform( glm::mat4 transformIn ) = 0;
		virtual void GetPosition( glm::vec3& positionOut) = 0;
		virtual void SetPosition( glm::vec3 positionOut ) = 0;
		virtual void GetRotation( glm::vec3& rotationOut) = 0;
		virtual void SetRotation( glm::vec3 rotationOut ) = 0;

		virtual void GetVelocity( glm::vec3& velocityOut ) = 0;

		virtual void ApplyForce( glm::vec3 force ) = 0;
		virtual void ApplyForceAtPoint( glm::vec3 force, glm::vec3 velocity ) = 0;

		virtual void ApplyImpulse( glm::vec3 impulse ) = 0;
		virtual void ApplyImpulseAtPoint( glm::vec3 impulse, glm::vec3 relativePoint ) = 0;
		
	};
}