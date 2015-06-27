#include "Bee.h"

#define JUMP_SPEED_IMPULSE -300
#define WALK_SPEED_IMPULSE 2.5f
#define BEE_STEPS 36
#define BEE_STAND_TIME (double)0.1
#define BEE_POS_STEP_X float(1.7)
#define SPEED 1.5f
#define AMPLITUDE 5.0f
#define PERIOD 5.0f


Bee::Bee(std::shared_ptr<TmxMap> map, std::shared_ptr<CPlayer> player)
{
	lastShootingTime = 0;
	m_shouldShoot = false;
	m_pSprite = new AnimatedSprite("data/bee.bmp", RGB(0xff, 0x00, 0xff));
	m_bIsAlive = true;
	m_pMap = map;
	m_pPlayer = player;
	oldDelta = 0;

	myVelocity.x = -SPEED;
}

Bee::~Bee()
{
	delete m_pSprite;
}

void Bee::Init(const Vec2& position)
{
	lastShootingTime = time(0);
	standStill = 0.f;
	isActive = false;

	myPosition = position;
	myBodyIsStatic = false;

	m_State = BeeState::FlyLeft;
	myCollisionSide = BeeCollision::BC_None;

	m_referencePosition.y = position.y - 32;
	m_referencePosition.x = position.x;

	// initialize stand frame
	standFrame.right = (long)position.x;
	standFrame.bottom = (long)position.y;
	standFrame.top = (long)position.y + 64;
	standFrame.left = (long)position.x - 128;

	// Animation frame crop rectangle
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = 32;
	r.bottom = 32;

	m_pSprite->Initialize(r, 3, 1 / 8.f);
	m_pSprite->Play(true);
}

bool Bee::checkActive()
{
	if (isActive)
	{
		// check if the bee went out from the left side or from the right side
		if (standFrame.right < 0 || m_pMap.lock()->m_screenWidth <= standFrame.left)
		{
			isActive = false;
		}
	}
	else
	{
		// check if the bee went in from the left side or from the right side
		if (m_pMap.lock()->m_screenWidth > standFrame.left && standFrame.left > 0.0f || standFrame.right > 0 && standFrame.left < 0)
		{
			isActive = true;
		}
	}

	return isActive;
}

void Bee::checkIfShouldShoot()
{
	if (!isActive && !Expired()  )
	{
		return;
	}

	m_shouldShoot = false;

	//check if this bee is in the right position to shoot on Ox axis.
	if (((m_State & BeeState::FlyLeft) && myPosition.x < m_pPlayer.lock()->myPosition.x) ||
		((m_State & BeeState::FlyRight) && myPosition.x > m_pPlayer.lock()->myPosition.x))
	{
		return;
	}

	//check if this bee is in the right position to shoot on Oy axis and if it's the right time.
	if (((m_pPlayer.lock()->myPosition.y - 33.f) < myPosition.y) &&
		((m_pPlayer.lock()->myPosition.y + 31.f) > myPosition.y))
	{
		time_t delta = time(0) - lastShootingTime;
		if (delta > SHOOTING_FREQUENCY)
		{
			m_shouldShoot = true;
			lastShootingTime = time(0);
		}
		return;
	}	
}

void Bee::Update(float dt)
{
	if (!IsAlive())
	{
		return;
	}

	ResolveCollision();
	checkIfShouldShoot();

	// Update position of the bee related to the position of the map.
	long delta = (long)(m_pMap.lock()->m_pixelWidth / 2 - m_pMap.lock()->myPosition.x);
	if (fabsl(delta - oldDelta) > 1)
	{
		standFrame.right -= (delta - oldDelta);
		standFrame.left -= (delta - oldDelta);
		myPosition.x -= (delta - oldDelta);
	}
	oldDelta = delta;

	if (!checkActive()) {
		return;
	}

	standStill += dt;
	if (standStill >= 1000)
	{
		standStill = 0.0;
	}

	myPosition += myVelocity;
	myPosition.y = AMPLITUDE * sinf(PERIOD * (float)standStill) + standFrame.bottom;
	m_pSprite->myPosition = myPosition;
	m_pSprite->Update(dt);
}

void Bee::Draw() const
{
	if (!IsAlive())
	{
		return;
	}

	if (!isActive)
	{
		return;
	}

	CGameObject::Draw();

	RECT r;
	if (m_State == BeeState::FlyLeft)
	{
		r.left = 0;
		r.top = 0;
		r.right = 32;
		r.bottom = 32;
	}

	if (m_State == BeeState::FlyRight)
	{
		r.left = 0;
		r.top = 32;
		r.right = 32;
		r.bottom = 64;
	}

	m_pSprite->SetFrameStartCrop(r);

	m_pSprite->Draw();
}

void Bee::checkStandingFrameCollision()
{
	if (myPosition.x > standFrame.right && m_State == BeeState::FlyRight)
	{
		myCollisionMask |= BeeCollision::BC_StandFrame;
		myCollisionSide |= BeeStandFrame::BSF_Right;
	}
	else if (myPosition.x < standFrame.left && m_State == BeeState::FlyLeft)
	{
		myCollisionMask |= BeeCollision::BC_StandFrame;
		myCollisionSide |= BeeStandFrame::BSF_Left;
	}
}

void Bee::ResolveCollision()
{
	checkStandingFrameCollision();

	if (myCollisionMask & BeeCollision::BC_StandFrame)
	{
		if (myCollisionSide & BeeStandFrame::BSF_Buttom)
		{
			myPosition.y = (float)standFrame.bottom;
		}

		if (myCollisionSide & BeeStandFrame::BSF_Top)
		{
	
			myPosition.y = (float)standFrame.top;
		}

		if (myCollisionSide & BeeStandFrame::BSF_Left)
		{
			myPosition.x = (float)standFrame.left;
			m_State = BeeState::FlyRight;
			myVelocity.x = SPEED;
		}

		if (myCollisionSide & BeeStandFrame::BSF_Right)
		{
			m_State = BeeState::FlyLeft;
			myPosition.x = (float)standFrame.right;
			myVelocity.x = -SPEED;
		}
	}

	if (myCollisionMask & BeeCollision::BC_Player || myCollisionMask & BeeCollision::BC_Bullet)
	{
		m_bIsAlive = false;
	}

	myCollisionMask = BeeStandFrame::BSF_None;
	myCollisionSide = BeeCollision::BC_None;
}

CRectangle Bee::GetRectangle() const
{
	float x = m_pSprite->myPosition.x - GetWidth() / 2;
	float y = m_pSprite->myPosition.y - GetHeight() / 2;

	CRectangle r(x, y, GetWidth(), GetHeight());
	return r;
}


