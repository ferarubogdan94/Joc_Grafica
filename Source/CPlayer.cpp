#include "CPlayer.h"

#define JUMP_SPEED_IMPULSE -300
#define WALK_SPEED_IMPULSE 2.5f

CPlayer::CPlayer()
{
  m_pSprite = new AnimatedSprite("data/girl.bmp", RGB(0xff, 0x00, 0xff));
  myCollisionMask = CF_Screen | CF_Wall;
  m_bIsAlive = true;
  playerOrientation = PlayerOrientation::Right;
}

CPlayer::~CPlayer()
{
  delete m_pSprite;
}

void CPlayer::Init(const Vec2& position)
{
  myPosition = position;
  myBodyIsStatic = false;
  m_State = PlayerState::Idle;
  myCollisionSide = CollisionSide::CS_None;
  m_prevPosition = position;
  m_life = 50;
  m_score = 0;

  // Animation frame crop rectangle
  RECT r;
  r.left = 0;
  r.top = 0;
  r.right = 32;
  r.bottom = 48;

  m_pSprite->Initialize(r, 4, 1 / 8.f);
  m_pSprite->Play(true);
}

void CPlayer::Update(float dt)
{

  CGameObject::Update(dt);

  // Update sprites
  m_pSprite->myPosition = myPosition;

  m_pSprite->Update(dt);


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

void CPlayer::Draw() const
{
  CGameObject::Draw();

  RECT r;
  if (m_State == PlayerState::Idle)
  {
    r.left = 0;
    r.top = 0;
    r.right = 32;
    r.bottom = 48;
  }

  if (m_State == PlayerState::WalkLeft)
  {
    r.left = 0;
    r.top = 48;
    r.right = 32;
    r.bottom = 96;

  }

  if (m_State == PlayerState::WalkRight)
  {
    r.left = 0;
    r.top = 96;
    r.right = 32;
    r.bottom = 144;
  }

  if (m_State == PlayerState::Jump)
  {
    r.left = 0;
    r.top = 144;
    r.right = 32;
    r.bottom = 192;
  }

  m_pSprite->SetFrameStartCrop(r);
  m_pSprite->Draw();
}


void CPlayer::WalkLeft()
{
  if (!(myCollisionSide & CollisionSide::CS_Left))
  {
    m_State = PlayerState::WalkLeft;
  }
}


void CPlayer::WalkRight()
{
  if (!(myCollisionSide & CollisionSide::CS_Right))
  {
    m_State = PlayerState::WalkRight;
  }

}


void CPlayer::Jump()
{
  if (myCollisionSide & CollisionSide::CS_Bottom)
  {
    myVelocity.y = JUMP_SPEED_IMPULSE;
    m_State = PlayerState::Jump;
  }
}

void CPlayer::ResolveCollision()
{
  if (myCollisionMask & CF_Wall)
  {
    if (myCollisionSide & CollisionSide::CS_Bottom)
    {
      myPosition.y = m_prevPosition.y - GetHeight() / 2 - 0.1f;
      myVelocity.y = 0.f;
    }

    if (myCollisionSide & CollisionSide::CS_Top)
    {
      myPosition.y = m_prevPosition.y + GetHeight() / 2 + 0.1f;
      myVelocity.y = 0.f;
    }

    if (myCollisionSide & CollisionSide::CS_Left)
    {
      m_State = PlayerState::Idle;
      myVelocity.x = 0.f;
    }

    if (myCollisionSide & CollisionSide::CS_Right)
    {
      m_State = PlayerState::Idle;
      myVelocity.x = 0.f;
    }
  }

  if (myCollisionMask & CF_Pickup)
  {
    m_score += 10;
  }

  if (myCollisionMask & CF_Enemy)
  {
      m_life -= 20;
     if (m_life <= 0)
    {
       m_life = 0;
      m_bIsAlive = false;
    }
  }


  myCollisionMask = CF_None;
  
}


CRectangle CPlayer::GetRectangle() const
{
  float x = m_pSprite->myPosition.x - GetWidth() / 2 + 5.0f;
  float y = m_pSprite->myPosition.y - GetHeight() / 2;

  CRectangle r(x, y, GetWidth() - 10, GetHeight());
  return r;
}

