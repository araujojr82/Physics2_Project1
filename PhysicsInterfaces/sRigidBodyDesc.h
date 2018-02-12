#pragma once
//#include <glm\game_math.h>

namespace nPhysics
{
	struct sRigidBodyDesc
	{
		sRigidBodyDesc()
			: Position( 0.f, 0.f, 0.f )
			, PrevPosition( 0.f, 0.f, 0.f )
			, Velocity( 0.f, 0.f, 0.f)
			, Rotation( 0.f, 0.f, 0.f )
			, Acceleration( 0.f, 0.f, 0.f )
			, Mass( 0.f )
			, invMass( 0.f )
		{

		}
		glm::vec3 Position;
		glm::vec3 PrevPosition;
		glm::vec3 Velocity;
		glm::vec3 Rotation;
		glm::vec3 Acceleration;
		glm::vec3 AngularVelocity;

		float Mass;
		float invMass;
	};
}