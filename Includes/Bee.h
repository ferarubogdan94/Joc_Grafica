#ifndef _BEE_H_
#define _BEE_H_

#include "Main.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "CGameObject.h"
#include "Rectangle.h"
#include <math.h>

enum BeeState
{
	BeeIdle = 0,
	FlyLeft = 1,
	FlyRight = 2
};

enum BeeCollision
{
	BC_None = 0,
	BC_StandFrame = 1,
};

enum BeeStandFrame
{
	BSF_None = 0,
	BSF_Top = 1,
	BSF_Buttom = 2,
	BSF_Right = 4,
	BSF_Left = 8,
};

/**
 * Player class handles all player manipulation, some minor physics,
 * update and management as well as rendering.
 */
class Bee : public CGameObject
{
public:
	Bee();
	virtual ~Bee();

public:
	void Init(const Vec2& position);

	virtual void Update(float dt);

	virtual GameObjectType GetObjectType() const
	{
		return GOT_Bee;
	}

	virtual CollisionFlag GetCollisionResponseFlag() const
	{
		return CF_Bee;
	}

	virtual void ResolveCollision();

	virtual void Draw() const;

	virtual int	GetWidth() const
	{
		return m_pSprite->GetWidth();
	}

	virtual int GetHeight() const
	{
		return m_pSprite->GetHeight();
	}

	bool IsAlive() const
	{
		return m_bIsAlive;
	}

	virtual CRectangle GetRectangle() const;

	/**
	 * Check if the bee is still in the stand frame.
	 */
	void checkStandingFrameCollision();

	BeeState m_State;

private:
	AnimatedSprite*	m_pSprite;

	/**
	 * A bounding rectangle in which the bee fly.
	 */
	RECT standFrame;

	/**
	 * Flag for checking the liveness of the bee.
	 */
	bool m_bIsAlive;

	/**
	* The reference position of the bee in the stand frame.
	*/
	Vec2 m_referencePosition;

	/**
	 * Discret values for modeling a sinusoidal motion.
	 */
	static const double values[36];
	int valuesPointer;

	/**
	 * The time in which the bee stands still.
	 */
	double standStill;	
};

#endif