#pragma once

#include <glm/vec3.hpp>

struct sRK4State
{
	sRK4State() :
		Pos( glm::vec3( 0.0f ) ),
		Vel( glm::vec3( 0.0f ) ),
		Acc( glm::vec3( 0.0f ) ){}

	sRK4State( glm::vec3 p, glm::vec3 v, glm::vec3 a ) : 
		Pos( p ),
		Vel( v ),
		Acc( a ){}

	glm::vec3 Pos;
	glm::vec3 Vel;
	glm::vec3 Acc;

};

class cIntegration
{
public:
	cIntegration();
	//~cIntegration();

	sRK4State cIntegration::RK4Eval( const sRK4State& state, float dt, const const sRK4State& deltaState );
	void RK4( glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt );
	void Euler( glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration, float dt );
	
};
