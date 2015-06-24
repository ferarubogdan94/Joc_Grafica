/**
 * author    Mihai Popescu
 * date      26/03/2012
 * copyright GNU Public License.
 */
#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H
#include "Sprite.h"

/**
 * This class is used to handle animated sprites.
 */
class AnimatedSprite : public Sprite
{
public:
    AnimatedSprite(const char *szImageFile) : Sprite(szImageFile) { myInitializedState = false; }
	AnimatedSprite(const char *szImageFile, const char *szMaskFile) : Sprite(szImageFile, szMaskFile) { myInitializedState = false; }
	AnimatedSprite(const char *szImageFile, COLORREF crTransparentColor) : Sprite(szImageFile, crTransparentColor) { myInitializedState = false; }

public:
	/**
	 * Initialization function.
	 */
	void Initialize(const RECT& rcFirstFrame, int iFrameCount, float fFrameDuration);
	
	/**
	 * Returns the current frame count.
	 */
	int GetFrameCount()
	{
		return myFrameCount; 
	}

	/**
	 * Starts playing the animated sprite.
	 */
	void Play(bool bLoop = false)
	{ 
		myPlayState = true;
		myLoopingState = bLoop;
	}

    /**
	 * Stops playing the animated sprite.
	 */
    void Stop()
	{ 
		myPlayState = false;
	}

	/**
	 * Returns true if the animation is playing.
	 */
	bool IsPlaying() const 
	{ 
		return myPlayState;
	}
    /**
	 * Sets the current frame index.
	 */
    inline void SetFrameIndex(int iFrameIndex);

    POINT GetFrameStartCrop() const
	{
		return myFrameStartCrop;
	}

    /**
	 * Sets the upper-left corner of the frame.
	 */
    void SetFrameStartCrop(RECT);

    virtual int GetWidth() const { return myFrameWidth; }
    virtual int GetHeight() const { return myFrameHeight; }
    virtual int GetFrameCropX() const { return (int)myFrameCrop.x; }
    virtual int GetFrameCropY() const { return (int)myFrameCrop.y; }

public:
	/**
	 * Handle switching to the next frame.
	 */
	virtual void Update(float dt);

private:
	/**
	 * Tells if the class is initialized.
	 */
	bool myInitializedState;

	/**
	 * Looping state
	 */
    bool myLoopingState;

	/**
	 * If the frame is playing
	 */
    bool myPlayState;

	/**
	 *  The duration of a frame. 
	 */
	float myFrameDuration;  

	/**
	 * Time accumulator.
	 */
	float myFrameTimer;  

	/**
	 * First point of the frame (upper-left corner).
	 */
	POINT myFrameStartCrop;		   

	/**
	 * Number of frames.
	 */
	int myFrameCount;           

	/**
	 *  Current frame number.
	 */
	int myFrameIndex;     

	/**
	 * Current frame crop.
	 */
    POINT myFrameCrop;           

	/**
	 *  Frame width.
	 */
    int myFrameWidth;           

	/**
	 * Frame height.
	 */
    int myFrameHeight;           
};

#endif 