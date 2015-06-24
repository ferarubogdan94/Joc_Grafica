#pragma once
#include <string>

class TmxMapLayer
{
public:
  std::string m_name;
  int m_width;
  int m_height;
  TmxMapLayer(const std::string&,int, int);
  ~TmxMapLayer();

  int operator[] (int) const;

  /**
  * Gets the element specified by row and column;
  * @param row - the element's row
  * @param column - the element's column
  * Returns the element if row and column are valid, -1 othewise
  */
  int GetAt(int, int);

  /**
  * Sets a specified element.
  * @param element - the element to be set
  * @param row - the element's row
  * @param column - the element's column
  */
  void SetAt(int, int,int);

private:
  int *m_data;

};
