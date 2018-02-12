#include "cRigidBody.h"

namespace nPhysics
{
	cRigidBody::cRigidBody( const sRigidBodyDesc& desc, iShape* shape )
		: mShape( shape )
		, mPosition( desc.Position )
		, mPrevPosition( desc.PrevPosition )
		, mVelocity( desc.Velocity )
		, mMass( desc.Mass )
		, mInvMass( desc.invMass )
		, mRotation( desc.Rotation )
		, mAcceleration( desc.Acceleration )
		, mAngularVelocity( desc.AngularVelocity )
	{
		
	}
	cRigidBody::~cRigidBody()
	{

	}

	iShape* cRigidBody::GetShape()
	{
		return mShape;
	}

	void cRigidBody::GetTransform(glm::mat4& transformOut)
	{
		transformOut = glm::mat4_cast(mRotation);
		transformOut[3][0] = mPosition.x;
		transformOut[3][1] = mPosition.y;
		transformOut[3][2] = mPosition.z;
		transformOut[3][3] = 1.f;
	}

	void cRigidBody::GetPosition(glm::vec3& positionOut)
	{
		positionOut = mPosition;
	}

	void cRigidBody::GetVelocity( glm::vec3& velocityOut )
	{
		velocityOut = mVelocity;
	}

	void cRigidBody::SetVelocity( glm::vec3 velocityIn )
	{
		mVelocity = velocityIn;
		return;
	}

	void cRigidBody::GetRotation(glm::vec3& rotationOut)
	{
		rotationOut = glm::eulerAngles(mRotation);
	}

	void cRigidBody::SetTransform( glm::mat4 transformIn )
	{
		/*transformOut = glm::mat4_cast( mRotation );
		transformOut[3][0] = mPosition.x;
		transformOut[3][1] = mPosition.y;
		transformOut[3][2] = mPosition.z;
		transformOut[3][3] = 1.f;*/
	}

	void cRigidBody::SetPosition( glm::vec3 positionIn )
	{
		mPosition = positionIn;
	}

	void cRigidBody::SetRotation( glm::vec3 rotationIn )
	{
		mRotation = rotationIn;
		// glm::angleAxis( Euler angles )?
	}

	void cRigidBody::ApplyForce( glm::vec3 force )
	{
		mAcceleration += force * mMass;
	}

	void cRigidBody::ApplyForceAtPoint( glm::vec3 force, glm::vec3 velocity )
	{
		
	}


	void cRigidBody::ApplyImpulse( glm::vec3 impulse )
	{
		mVelocity += impulse * mMass;
	}

	void cRigidBody::ApplyImpulseAtPoint( glm::vec3 impulse, glm::vec3 relativePoint )
	{
		mVelocity += impulse * mInvMass;
		mAngularVelocity += glm::cross( relativePoint, impulse * mInvMass );
	}

}