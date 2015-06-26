#ifndef _BULLET_H_
#define _BULLET_H_

#include "CGameObject.h"
#include "Vec2.h"
#include "Sprite.h"
#include "CPlayer.h"
#include "TmxMap.h"

#define SPEED 60

enum BulletType
{
	Bee_Bullet,
	Player_Bullet,
};

enum BulletCollision
{
	BulletC_Bee = 1,
};
class Bullet :public CGameObject
{
public:
	Bullet(BulletType type,Vec2 initPos,std::shared_ptr<CPlayer>,std::shared_ptr<TmxMap>);
	virtual ~Bullet();

public:
	void Init(const Vec2& position);

	virtual GameObjectType GetObjectType() const
	{
		return GOT_Bullet;
	}

	virtual CollisionFlag GetCollisionResponseFlag() const
	{
		return CF_Bullet;
	}

	virtual void ResolveCollision();

	virtual void Update(float dt);

	virtual void Draw() const;

	virtual int	GetWidth() const
	{
		return m_pSprite->GetWidth();
	}

	virtual int GetHeight() const
	{
		return m_pSprite->GetHeight();
	}

	/**
	* Returns true if this bullet is expired and false otherwise.
	*/
	virtual bool Expired() const
	{
		return !m_bIsAlive;
	}

	/**
	* Returns the bounding rectangle of the bullet.
	*/
	virtual CRectangle GetRectangle() const;

private:
	/**
	* Returns the direction of the bullet( 1 for right and -1 for left).
	*/
	int checkDirection(BulletType type,std::shared_ptr<CPlayer>,Vec2);

	std::weak_ptr<TmxMap> m_pMap;

	Sprite*	m_pSprite;

	bool m_bIsAlive;

	int typeMask;

	long oldDelta;
};

#endif