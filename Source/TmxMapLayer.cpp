#include "TmxMapLayer.h"

using namespace std;

TmxMapLayer::TmxMapLayer(const string& name,int width, int height) :
m_name(name),
m_width(width),
m_height(height)
{
  m_data = NULL;
  m_data = new int[m_width * m_height];
}

int TmxMapLayer::operator[](int index) const
{
  if (index < 0 || index >= m_height * m_width)
  {
    return -1;
  }
  return m_data[index];
}

int TmxMapLayer::GetAt(int row, int column)
{
  if (row < 0 || row >= m_height ||
    column < 0 || column >= m_width)
  {
    return -1;
  }

  return m_data[row * m_width + column];
}

void TmxMapLayer::SetAt(int element, int row, int column)
{
  if (row < 0 || row >= m_height ||
    column < 0 || column >= m_width)
  {
    return;
  }

  m_data[row * m_width + column] = element;
}

TmxMapLayer::~TmxMapLayer()
{
  if (m_data != NULL)
  {
    delete[] m_data;
  }
}