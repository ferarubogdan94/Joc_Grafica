#include "Bee.h"

#define JUMP_SPEED_IMPULSE -300
#define WALK_SPEED_IMPULSE 2.5f
#define BEE_STEPS 36
#define BEE_STAND_TIME (double)0.1
#define BEE_POS_STEP_X float(1.7)

const double Bee::values[] = { 0.00, 2.78, 5.47, 8.00, 10.28, 12.26, 13.86, 15.04, 15.76, 16.00, 15.76, 15.04,\
13.86, 12.26, 10.28, 8.00, 5.47, 2.78, 0.00, -2.78, -5.47, -8.00, -10.28, -12.26, -13.86, -15.04, -15.76, -16.00,\
-15.76, -15.04, -13.86, -12.26, -10.28, -8.00, -5.47, -2.78};

Bee::Bee(std::shared_ptr<TmxMap> map)
{
	m_pSprite = new AnimatedSprite("data/bee.bmp", RGB(0xff, 0x00, 0xff));
	m_bIsAlive = true;
	m_pMap = map;
	oldDelta = 0;
}

Bee::~Bee()
{
	delete m_pSprite;
}

void Bee::Init(const Vec2& position)
{
	standStill = 0.f;
	valuesPointer = BEE_STEPS - 1;
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

void Bee::Update(float dt)
{
	if (!IsAlive()) { return; }
	ResolveCollision();

	// Update position of the bee related to the position of the map.
	long delta = (long)(m_pMap.lock()->m_pixelWidth / 2 - m_pMap.lock()->myPosition.x);
	if (fabsl(delta-oldDelta) > 1)
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
	if (standStill >= BEE_STAND_TIME)
	{
		if (m_State & BeeState::FlyRight) 
		{
			myPosition.x += BEE_POS_STEP_X;
			if (valuesPointer >= BEE_STEPS) 
			{
				valuesPointer = 0;
			}
			myPosition.y = m_referencePosition.y + (float)values[valuesPointer++];
		}

		else if ((m_State & BeeState::FlyLeft)) 
		{
			myPosition.x -= BEE_POS_STEP_X;
			if (valuesPointer < 0) 
			{
				valuesPointer = BEE_STEPS - 1;
			}
			myPosition.y = m_referencePosition.y - (float)values[valuesPointer--];
		}

		m_pSprite->myPosition = myPosition;
		standStill = 0.0;
	}

	m_pSprite->Update(dt);

	// NOTE: for each async sound played Windows creates a thread for you
	// but only one, so you cannot play multiple sounds at once.
	// This creation/destruction of threads also leads to bad performance
	// so this method is not recommended to be used in complex projects.


	// NOTE: For sound you also can use MIDI but it's Win32 API it is a bit hard
	// see MSDN reference: http://msdn.microsoft.com/en-us/library/ms711640.aspx
	// In this case you can use a C++ wrapper for it. See the following article:
	// http://www.codeproject.com/KB/audio-video/midiwrapper.aspx (with code also)
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
			//			myPosition.y = m_prevPosition.y - GetHeight() / 2;
			myPosition.y = (float)standFrame.bottom;
		}

		if (myCollisionSide & BeeStandFrame::BSF_Top)
		{
			//myPosition.y = m_prevPosition.y + GetHeight() / 2;
			myPosition.y = (float)standFrame.top;
		}

		if (myCollisionSide & BeeStandFrame::BSF_Left)
		{
			//			m_State = PlayerState::Idle;
			//			myPosition.x = m_prevPosition.x + GetWidth() / 2 - 1.5f;
			myPosition.x = (float)standFrame.left;
			m_State = BeeState::FlyRight;
			valuesPointer = 0;
		}

		if (myCollisionSide & BeeStandFrame::BSF_Right)
		{
			//m_State = PlayerState::Idle;
			//myPosition.x = m_prevPosition.x - GetWidth() / 2 + 1.5f;
			m_State = BeeState::FlyLeft;
			myPosition.x = (float)standFrame.right;
			valuesPointer = BEE_STEPS - 1;
		}
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


