#ifndef _SCORE_H_
#define _SCORE_H_

#include <vector>
#include "Sprite.h"
#include "Vec2.h"

#define MAX_SCORE 9999

using namespace std;

/**
 * The Score class keep score of a player in a game and provides some methods for drowing it on the screen.
 */
class Score
{

public:
	Score(float x = 0.f, float y = 0.f);

	~Score();

	/**
	 * Change the score value to a given value.
	 */
	void setScore(unsigned int score);

	/**
	 * Adds to actual score a value.
	 */
	void addToActualScore(unsigned int score);

	/**
	 * Draws score at a certain position on the window.
	 */
	void drawScore(int x = 0, int y = 0);

private:
	/**
	 * Vector containing sprites for all figures from 0 to 9.
	 */
	vector<Sprite*> m_vNumbers;

	/**
	 * The score value.
	 */
	unsigned int m_Score;

	/**
	 * The position where the score will be drawn on the window.
	 */
	Vec2 m_Position;
};

#endif