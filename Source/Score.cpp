#include "Score.h"

Score::Score(float x, float y)
{
	char path[] = "data/numbers/_.bmp";
	for (int i = 0; i < 10; i++)
	{
		path[13] = '0' + i;
		m_vNumbers.push_back(new Sprite(path, RGB(0xff, 0xff, 0xff)));
	}
	m_Score = 0;
	m_Position.x = x;
	m_Position.y = y;
}

Score::~Score() {
	for (auto it = m_vNumbers.begin(); it != m_vNumbers.end(); it++) {
		delete *it;
	}
	m_vNumbers.clear();
}

void Score::setScore(unsigned int score)
{
	if (score > MAX_SCORE)
	{
		return;
	}
	m_Score = score;
}

void Score::addToActualScore(unsigned int scoreToAdd) {
	if (m_Score + scoreToAdd > MAX_SCORE) 
	{
		return;
	}
	m_Score += scoreToAdd;
}

void Score::drawScore(int a, int b)
{
	char figures[11];
	int x = (int)m_Position.x + a;
	int y = (int)m_Position.y + b;

	_itoa(m_Score, figures, 10);
	for (int i = 0; figures[i] != '\0'; i++) 
	{
		int indexOfNumber = figures[i] - '0';
		m_vNumbers.at(indexOfNumber)->DrawWithOffset(x, y);
		x += m_vNumbers.at(indexOfNumber)->GetWidth();
	}
}