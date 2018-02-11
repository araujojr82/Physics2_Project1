#pragma once

#include <glm\vec3.hpp>
#include <vector>

namespace nPhysics
{
	struct sCollisionManifold {
		sCollisionManifold() :
			colliding( false ),
			normal( glm::vec3( 0.0f ) ),
			depth( FLT_MAX )
		{
			contacts.clear();
		};

		bool colliding;
		glm::vec3 normal;
		float depth;
		std::vector<glm::vec3> contacts;
	};
}