#include "CGameObject.h"
#include "CGameApp.h"

#define PIXEL_PER_METERS 50

CGameObject::CGameObject()
: myCollisionSide(CS_None)
, myCollisionMask(CF_None)
, myBodyIsStatic(true)
{

}

CGameObject::~CGameObject()
{

}

void CGameObject::ResolveCollision()
{
	// on collision with objects we have to apply the third fundamental law of motion
	// F_Reaction = F_Action

	if (!myBodyIsStatic)
	{
		if ((myCollisionSide & CS_Left) || (myCollisionSide & CS_Right))
		{
			myVelocity.x = 0;
		}

		if ((myCollisionSide & CS_Top) || (myCollisionSide & CS_Bottom))
		{
			myVelocity.y = 0;
		}
	}

	myCollisionSide = CS_None;
}

void CGameObject::Update(float dt)
{
	Vec2 a;
	if (!myBodyIsStatic)
	{
		// apply gravity
		a.y = GRAVITATIONAL_CONSTANT * PIXEL_PER_METERS;

		// apply Stokes law. For more info see: http://en.wikipedia.org/wiki/Stokes'_law
		a += myVelocity * (-1);
	}

	// compute resultant acceleration
	a += myAcceleration;

	myResultant = a;

	// Update the sprites position and speed
	myPosition += myVelocity * dt + a * dt * dt / 2;
	myVelocity += a * dt;
	ResolveCollision();
}

void CGameObject::Draw() const
{

}