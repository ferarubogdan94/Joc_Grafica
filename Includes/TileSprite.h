#pragma once
#include "Sprite.h"

/**
* Draws a specified area of an image.
**/
class TileSprite
	: public Sprite
{
public:
	TileSprite(const char *szImageFile) : Sprite(szImageFile) { myInitializedState = false; }
	TileSprite(const char *szImageFile, const char *szMaskFile) : Sprite(szImageFile, szMaskFile) { myInitializedState = false; }
	TileSprite(const char *szImageFile, COLORREF crTransparentColor) : Sprite(szImageFile, crTransparentColor) { myInitializedState = false; }

public:
	//! Initialization function
	void Initialize(int,int);

	//! Overrides
	virtual int GetWidth() const { return myFrameWidth; }
	virtual int GetHeight() const { return myFrameHeight; }
	virtual int GetFrameCropX() const { return (int)myFrameCrop.x; }
	virtual int GetFrameCropY() const { return (int)myFrameCrop.y; }
  
  /**
  * Draws a frame specified by its index.
  * @param frameIndex - the index of the frame
  */
  virtual void DrawFrame(int);

  /**
  * Draws a frame specified by the upper-left point of the frame.
  * @param framePoint - the upper-left corner of the rectangle to draw
  */
  virtual void DrawFrame(POINT);
private:
	bool myInitializedState;    //!< tells if the class is initialized
	POINT myFrameStartCrop;		//!< first point of the frame (upper-left corner)
	int myFrameIndex;           //!< current frame number
	POINT myFrameCrop;          //!< current frame crop
	int myFrameWidth;           //!< frame width
	int myFrameHeight;          //!< frame height
};

