/**
 * File: CTimer.h
 *
 * Desc: This class handles all timing functionality. This includes counting
 *	the number of frames per second, to scaling vectors and values
 *	relative to the time that has passed since the previous frame.
 *
 * Original design by Adam Hoult & Gary Simmons. Modified by Mihai Popescu.
 */

#ifndef _CTIMER_H_
#define _CTIMER_H_

#include "Main.h"

/** 
 * Maximum frame time sample count.
 */
const ULONG MAX_SAMPLE_COUNT = 50; 

/**
 * Desc : Game Timer class, queries performance hardware if available, and 
 *		calculates all the various values required for frame rate based
 *		vector / value scaling.
 */
 class CTimer
{
public:
	CTimer();
	virtual ~CTimer();

	void Tick( float fLockFPS = 0.0f );
	unsigned long GetFrameRate( LPTSTR lpszString = NULL, size_t size = 0 ) const;
	float GetTimeElapsed() const;
	long long getCurrentTime() const
	{
		return m_CurrentTime;
	}

private:
	/**
	 * Has Performance Counter.
	 */
	bool m_PerfHardware;			 

	/**
	 * Amount to scale counter.
	 */
	float m_TimeScale;	

	/**
	 * Time elapsed since previous frame.
	 */
	float m_TimeElapsed;		

	/**
	 * Current Performance Counter.
 	 */
	__int64	m_CurrentTime;

	/**
	 * Performance Counter last frame.
	 */
	__int64	m_LastTime;		

	/**
	 * Performance Frequency.
	 */
	__int64	m_PerfFreq;					 

	float m_FrameTime[MAX_SAMPLE_COUNT];
	ULONG m_SampleCount;

	/**
	 * Stores current framerate.
	 */
	unsigned long		m_FrameRate;

	/**
	 * Elapsed frames in any given second.
	 */
	unsigned long		m_FPSFrameCount; 

	/** 
	 * How much time has passed during FPS sample.
	 */
	float						m_FPSTimeElapsed;
};

#endif