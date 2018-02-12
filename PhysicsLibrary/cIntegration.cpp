#include "cIntegration.h"

cIntegration::cIntegration()
{

}

sRK4State cIntegration::RK4Eval( const sRK4State& state, float dt, const const sRK4State& deltaState )
{
	sRK4State s;
	s.Pos = state.Pos + deltaState.Pos * dt;
	s.Vel = state.Vel + deltaState.Vel * dt;

	sRK4State dS;
	dS.Pos = s.Vel;
	dS.Vel = state.Acc;

	return dS;
}

void cIntegration::RK4( glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt )
{
	float halfDt = dt * 0.5f;

	sRK4State state( pos, vel, acc );
	sRK4State a = RK4Eval( state, 0.0f, sRK4State() );
	sRK4State b = RK4Eval( state, halfDt, a );
	sRK4State c = RK4Eval( state, halfDt, b );
	sRK4State d = RK4Eval( state, dt, c );

	glm::vec3 rkVel = ( a.Pos + ( ( b.Pos + c.Pos )*2.0f ) + d.Pos ) * ( 1.0f / 6.0f );
	glm::vec3 rkAcc = ( a.Vel + ( ( b.Vel + c.Vel )*2.0f ) + d.Vel ) * ( 1.0f / 6.0f );

	pos += rkVel * dt;
	vel += rkAcc * dt;

	// Slow down just a little
	vel = vel * 0.98f;	

	// Check if the ball is almost stopping and stop it
	if( fabsf( vel.x ) < 0.01f ) {
		vel.x = 0.0f;
	}
	if( fabsf( vel.y ) < 0.01f ) {
		vel.y = 0.0f;
	}
	if( fabsf( vel.z ) < 0.01f ) {
		vel.z = 0.0f;
	}

	return;
}

void cIntegration::Euler( glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration, float dt )
{
	const float damping = 0.98f;

	//float invMass = InvMass();

	//vec3 acceleration = forces * invMass;
	velocity = velocity + acceleration * dt;
	velocity = velocity * damping;

	if( fabsf( velocity.x ) < 0.01f ) {
		velocity.x = 0.0f;
	}
	if( fabsf( velocity.y ) < 0.01f ) {
		velocity.y = 0.0f;
	}
	if( fabsf( velocity.z ) < 0.01f ) {
		velocity.z = 0.0f;
	}

	// Integrate position
	position = position + velocity * dt;

	return;
}