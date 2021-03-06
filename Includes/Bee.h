#ifndef _BEE_H_
#define _BEE_H_

#include "Main.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "CGameObject.h"
#include "Rectangle.h"
#include "TmxMap.h"
#include <math.h>
#include "CPlayer.h"
#include "CTimer.h"
#include <ctime>

#define SHOOTING_FREQUENCY 3

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
	BC_Player = 2,
	BC_Bullet = 4,
};

enum BeeStandFrame
{
	BSF_None = 0,
	BSF_Top = 1,
	BSF_Buttom = 2,
	BSF_Right = 4,
	BSF_Left = 8,
};


class Bee : public CGameObject
{
public:
	Bee(std::shared_ptr<TmxMap>,std::shared_ptr<CPlayer>);
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

	virtual bool Expired() const
	{
		return !m_bIsAlive;
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

	/**
	* A bounding rectangle in which the bee fly.
	*/
	RECT standFrame;

	/**
	* Checks if this bee should be activ and if it does then activates it(turn isActive to true) and 
	* return true otherwise if it is active and shouldn't then deactivate(turn isActive to false) and
	* return false.
	*/
	bool checkActive();

	/**
	* Returns true if this bee should shoot.
	*/
	bool shouldShoot() {
		return m_shouldShoot;
	}
	
private:

	void checkIfShouldShoot();

	/**
	* A pointer to map that help us to manage the position of the bee relating to the map position.
	*/
	std::weak_ptr<TmxMap> m_pMap;


	std::weak_ptr<CPlayer> m_pPlayer;
	/**
	* A flag to tell whether this bee position is in the screen or not.
	*/
	bool isActive;

	/**
	* Flag for checking the liveness of the bee.
	*/
	bool m_bIsAlive;

	AnimatedSprite*	m_pSprite;

	/**
	* The reference position of the bee in the stand frame.
	*/
	Vec2 m_referencePosition;

	/**
	 * The time in which the bee stands still.
	 */
	double standStill;	

	long oldDelta;

	
	/**
	* The last time that this bee has shot.
	*/
	time_t lastShootingTime;

	/**
	* Boolean value that is true if and only if this bee should shoot.	
	*/
	bool m_shouldShoot;
};

#endif