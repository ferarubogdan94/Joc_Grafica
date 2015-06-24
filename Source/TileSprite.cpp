#include "TileSprite.h"


void TileSprite::Initialize(int tileWidth, int tileHeight)
{
  myFrameStartCrop.x = 0;
  myFrameStartCrop.y = 0;

  myFrameCrop = myFrameStartCrop;

  myFrameWidth = tileWidth;
  myFrameHeight = tileHeight;

  myFrameIndex = 0;

  myInitializedState = true;
}

void TileSprite::DrawFrame(int index)
{
  int x = Sprite::GetWidth() / myFrameWidth;
  int y = Sprite::GetHeight() / myFrameHeight;

  myFrameCrop.y = index / x * myFrameHeight;
  myFrameCrop.x = index % x * myFrameWidth;

  Sprite::Draw();
}


void TileSprite::DrawFrame(POINT framePoint)
{
  myFrameCrop = framePoint;

  Sprite::Draw();
}