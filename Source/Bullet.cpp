#include "Bullet.h"

Bullet::Bullet(BulletType type, Vec2 initPos, std::shared_ptr<CPlayer> player)
{
	oldDelta = 0;
	typeMask = type;
	if (type == BulletType::Player_Bullet)
	{
		m_pSprite = new Sprite("data/player_bullet.bmp", RGB(0xff, 0x00, 0xff));
	}
	else if (type == BulletType::Bee_Bullet)
	{
		m_pSprite = new Sprite("data/bee_bullet.bmp", RGB(0xff, 0x00, 0xff));
	}
	myCollisionSide = 0;
	myCollisionMask = 0;
	m_bIsAlive = true;
	float direction = (float)checkDirection(type, player, initPos);
	myVelocity = Vec2(direction*SPEED, 0);
	myPosition = initPos;
}

Bullet::~Bullet()
{
	delete m_pSprite;
}

int Bullet::checkDirection(BulletType type, std::shared_ptr<CPlayer> player, Vec2 shootingPosition)
{
	if (type == BulletType::Player_Bullet)
	{
		if (player->getPlayerOrientation() == PlayerOrientation::Left)
		{
			return -1;
		}
		
	}
	else if (type == BulletType::Bee_Bullet)
	{
		if (shootingPosition.x > player->myPosition.x)
		{
			return -1;
		}
	}
	return 1;
}

void Bullet::Init(const Vec2& position)
{
	myPosition = position;
	myBodyIsStatic = false;
	myCollisionSide = CollisionSide::CS_None;
}

void Bullet::Update(float dt)
{
	ResolveCollision();
	
	myPosition += myVelocity*dt;
	// Update sprites
	m_pSprite->myPosition = myPosition;

}

void Bullet::Draw() const
{
	m_pSprite->Draw();
}



void Bullet::ResolveCollision()
{
	if (myCollisionMask & BulletCollision::BulletC_Bee ||
		myCollisionSide & CS_Left || myCollisionSide & CS_Right ||
		myCollisionMask & CF_Wall || myCollisionMask & CF_Enemy)
	{
		m_bIsAlive = false;
	}

	myCollisionMask = CF_None;
}


CRectangle Bullet::GetRectangle() const
{
	float x = m_pSprite->myPosition.x - GetWidth() / 2 + 5.0f;
	float y = m_pSprite->myPosition.y - GetHeight() / 2;

	CRectangle r(x, y, GetWidth() - 10, GetHeight());
	return r;
}

