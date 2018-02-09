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

	return;
}
