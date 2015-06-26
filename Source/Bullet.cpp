#include "Bullet.h"

Bullet::Bullet(BulletType type, Vec2 initPos, std::shared_ptr<CPlayer> player,std::shared_ptr<TmxMap> map)
{
	oldDelta = 0;
	m_pMap = map;
	typeMask = type;
	if (type == BulletType::Player_Bullet)
	{
		m_pSprite = new Sprite("data/player_bullet.bmp", RGB(0xff, 0x00, 0xff));
	}
	else if (type == BulletType::Bee_Bullet)
	{
		m_pSprite = new Sprite("data/bee_bullet.bmp", RGB(0xff, 0x00, 0xff));
	}
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
	/*
	long delta = (long)(m_pMap.lock()->m_pixelWidth / 2 - m_pMap.lock()->myPosition.x);
	if (fabsl(delta - oldDelta) > 1)
	{
		myPosition.x -= (delta - oldDelta);
	}
	oldDelta = delta;
	*/
	//CGameObject::Update(dt);
	myPosition += myVelocity*dt;
	// Update sprites
	m_pSprite->myPosition = myPosition;

	// Get velocity
	// float v = myVelocity.Length();

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommended to be used in complex projects.


	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see MSDN reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
}

void Bullet::Draw() const
{
	m_pSprite->Draw();
}



void Bullet::ResolveCollision()
{
	if (myCollisionSide & CS_Left || myCollisionSide & CS_Right || myCollisionMask & CF_Wall || myCollisionMask & CF_Enemy)
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

