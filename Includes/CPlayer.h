/**
 * Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
 */

#ifndef _CPLAYER_H_
#define _CPLAYER_H_

#include "Main.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "CGameObject.h"

enum PlayerState
{
	WalkRight,
	WalkLeft,
	Idle,
	Jump
};

/**
 * Name : CPlayer (Class)
 * Desc : Player class handles all player manipulation, some minor physics,
 * update and management as well as rendering.
 */
class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();

public:
	void Init(const Vec2& position);
	virtual void Update(float dt);

	virtual GameObjectType GetObjectType() const 
	{
		return GOT_Player;
	}

	virtual CollisionFlag GetCollisionResponseFlag() const
	{ 
		return CF_Player;
	}

	void ResolveCollision();
	virtual void Draw() const;

	virtual int	GetWidth() const 
	{
		return m_pSprite->GetWidth();
	}

	virtual int GetHeight() const 
	{
		return m_pSprite->GetHeight();
	}

	void WalkRight();
	void WalkLeft();
	void Jump();

	bool IsAlive() const
	{
		return m_bIsAlive;
	}

	/**
	 * Returns the bounding rectangle of the player.
	 */
	virtual CRectangle GetRectangle() const;

	PlayerState m_State;

private:
	AnimatedSprite*	m_pSprite;

	bool m_bIsAlive;
	
public:
	/**
	* The previous position of the player.
	*/
	Vec2 m_prevPosition;

	int m_life;

	long m_score;
};

#endif