#pragma once

#define NULL_RECT CRectangle(0,0,0,0)

/**
* Specifies an area in a coordinate space that is enclosed by the CRectangle object's
* upper-left point (x,y), its width and its height.
* Provides method to check for two rectangles intersection and determine the relative position of this rectangle.
*/
class CRectangle
{
public:

  /**
  * Constructs a new CRectangle whose upper-left corner is specified as (x,y) and whose width and height are specified by the arguments of the same name.
  */
  CRectangle(float x, float y, int width, int height) :
    x(x), y(y), width(width), height(height) {}


  /**
  * Returns true if this rectangle intersects a specifed rectangle.
  */
  bool IntersectsRect(const CRectangle&) const;

  /**
  * Returns the intersection rectangle of this rectangle and a specified rectangle.
  */
  CRectangle GetIntersectRect(const CRectangle&) const;

  /**
  * Checks if the upper-left corner of this rectangle is above of a specified rectangle.
  * @param rect - the rectangle to compare to
  * Returns true if this rectangle is above rect, false otherwise.
  */
  bool IsAbove(const CRectangle&) const;


  /**
  * Checks if the upper-left corner of this rectangle is below of a specified rectangle.
  * @param rect - the rectangle to compare to
  * Returns true if this rectangle is below rect, false otherwise.
  */
  bool IsBellow(const CRectangle&) const;



  bool IsLeft(const CRectangle&) const;
  bool IsRight(const CRectangle&) const;

  float x;
  float y;
  int width;
  int height;
};