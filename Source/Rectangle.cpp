#include "Rectangle.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))


bool CRectangle::IntersectsRect(const CRectangle& rect) const
{

  return (x <= rect.x + rect.width && x + width >= rect.x &&
    y <= rect.y + rect.height && y + height >= rect.y) ? true : false;
}



CRectangle CRectangle::GetIntersectRect(const CRectangle& rect) const
{
  if (!this->IntersectsRect(rect))
  {
    return NULL_RECT;
  }

  CRectangle rezRect(0, 0, 0, 0);

  rezRect.x = max(x, rect.x);
  rezRect.y = max(y, rect.y);
  rezRect.width = (int)(min(x + width, rect.x + rect.width) - rezRect.x);
  rezRect.height = (int)(min(y + height, rect.y + rect.height) - rezRect.y);

  return rezRect;
}



bool CRectangle::IsAbove(const CRectangle& rect) const
{
  return y < rect.y ? true : false;
}


bool CRectangle::IsBellow(const CRectangle& rect) const
{
  return y > rect.y ? true : false;
}


bool CRectangle::IsLeft(const CRectangle& rect) const
{
  return x < rect.x ? true : false;
}

bool CRectangle::IsRight(const CRectangle& rect) const
{
  return x > rect.x ? true : false;
}