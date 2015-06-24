#pragma once

class CPoint
{
public:
  /**
   * The x coordinate of the point.
  */
  float x;

  /**
   * The y coordinate of the point.
   */
  float y;

  /**
   * Constructs a point whose coordinates are (0.0,0.0).
   */
  CPoint();

  /**
   * Constructs a point whose coordinates are (x,y);
   */
  CPoint(float, float);
  CPoint(int, int);
  ~CPoint() {}
};