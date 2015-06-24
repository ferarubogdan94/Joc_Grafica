#include "CPoint.h"

CPoint::CPoint(float x, float y) :
x(x), y(y)
{

}

CPoint::CPoint() 
{
  CPoint(0.f, 0.f);
}

CPoint::CPoint(int x, int y)
{
  CPoint();
  this->x = x + 0.f;
  this->y = y + 0.f;
}