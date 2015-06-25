#include "TmxMap.h"
#include <fstream>
#include <string.h>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <iomanip>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 512
#define SCROLL_SPEED 2.5f

TmxMap::TmxMap(const string& filename, const string& spritename)
{

  LoadMap(filename);
  m_screenWidth = SCREEN_WIDTH;
  m_screenHeight = SCREEN_HEIGHT;

  m_widthTileNumber = m_screenWidth / tileWidth + 1;
  m_heightTileNumber = m_screenHeight / tileHeight;

  m_firstTilePos.x = tileWidth / 2.f;
  m_firstTilePos.y = tileHeight / 2.f;

  myVelocity.y = 0;
  m_pixelWidth = width * tileWidth;
  m_pixelHeight = height * tileHeight;

  m_prevPos = myPosition;
  m_tileSet = new TileSprite(spritename.c_str(), RGB(0xff, 0x00, 0xff));
  m_tileSet->Initialize(tileWidth, tileHeight);

  m_tileRow = m_tileColumn = 0;
}


void TmxMap::LoadMap(const string& filename)
{

  ifstream f(filename);
  char s[50];
  // map width
  f.getline(s, 50);
  width = atoi(strtok(s, " "));

  // map height
  height = atoi(strtok(NULL, " "));

  // map tileWidth
  tileWidth = atoi(strtok(NULL, " "));

  // map tileHeight
  tileHeight = atoi(strtok(NULL, " "));

  // map layer's number
  int layersNumber = atoi(strtok(NULL, " "));

  // map Ox position
  myPosition.x = (float)atof(strtok(NULL, " "));

  // map Oy position
  myPosition.y = (float)atof(strtok(NULL, " "));

  // map data
  string line;
  stringstream stream;
  int a;
  for (int i = 0; i < layersNumber; i++)
  {
    f.getline(s, 500);
    string name = string(strtok(s, " "));

    TmxMapLayer* layer = new TmxMapLayer(name, width, height);

    for (int m = 0; m < height; m++)
    {
      getline(f, line);
      stream.clear();
      stream << line;
      for (int n = 0; n < width; n++)
      {
        stream >> a;
        layer->SetAt(a, m, n);
      }
    }

    m_layers.insert(pair<string, TmxMapLayer*>(layer->m_name, layer));
  }
  f.close();
}

void TmxMap::Draw()
{
  for (auto l : m_layers)
  {
    auto layer = l.second;
    float y = m_firstTilePos.y, x = 0.0f;

    for (int i = m_tileRow; i < m_tileRow + m_heightTileNumber; i++)
    {
      x = m_firstTilePos.x;
      for (int j = m_tileColumn; j < m_tileColumn + m_widthTileNumber; j++)
      {
        int index = layer->GetAt(i, j);
        if (index)
        {
          m_tileSet->myPosition.x = x;
          m_tileSet->myPosition.y = y;
          m_tileSet->DrawFrame(index - 1);
        }
        x += tileWidth;
      }
      y += tileHeight;
    }
  }
}

CPoint TmxMap::GetPositionToTileCoordinate(const CPoint& position)
{
  CPoint point;

  point.x = floorf((position.x - myPosition.x + m_pixelWidth / 2) / tileWidth);
  point.y = floorf((position.y - myPosition.y + m_pixelHeight / 2) / tileHeight);
  return point;
}

CPoint TmxMap::GetTileCoordinateToPosition(const CPoint& position)
{
  CPoint point;

  point.x = (position.x * tileWidth) + myPosition.x - m_pixelWidth / 2;
  point.y = (position.y * tileHeight) + myPosition.y - m_pixelHeight / 2;
  return point;

}

vector<CRectangle> TmxMap::GetSurroundingTiles(const CPoint& position, const string& layername)
{
  vector<CRectangle> tiles;
  CPoint pos = GetPositionToTileCoordinate(position);
  int x = (int)pos.x;
  int y = (int)pos.y;
  auto layer = m_layers.at(layername);
  CPoint posTile, tilePos;

  for (int i = -1; i < 2; i++)
  {
    for (int j = -1; j < 2; j++)
    {
      if ((i || j) && x + j >= 0 && x + j < width && y + i >= 0 && y + i < height)
      {
        if (layer->GetAt(y + i, x + i))
        {
          posTile.x = x + j + .0f;
          posTile.y = y + i + .0f;
          tilePos = GetTileCoordinateToPosition(posTile);
          CRectangle r((float)tilePos.x, (float)tilePos.y, tileWidth, tileHeight);
          tiles.push_back(r);
        }
      }
    }
  }

  return tiles;
}

vector<CRectangle> TmxMap::GetSurroundingTiles(const CRectangle& rect, const string& layername)
{
  // the numbers of tiles that are contained by rect
  int x = (int)ceil((float)rect.width / tileWidth);
  int y = (int)ceil((float)rect.height / tileHeight);
  vector<CRectangle> tiles;
  CPoint pos(0.f, rect.y + y * tileHeight), tileIndex, tilePosition;

  auto layer = m_layers.at(layername);

  if (!layer)
    return tiles;

  for (int i = 0; i < y + 2; i++)
  {
    pos.x = rect.x + rect.width / 2 - tileWidth;
    for (int j = 0; j < x + 2; j++)
    {
      tileIndex = GetPositionToTileCoordinate(pos);
      if (layer->GetAt((int)tileIndex.y, (int)tileIndex.x) > 0)
      {
        tilePosition = GetTileCoordinateToPosition(tileIndex);
        CRectangle r((float)tilePosition.x, (float)tilePosition.y, tileWidth, tileHeight);
        tiles.push_back(r);
      }
      pos.x += tileWidth;
    }
    pos.y -= tileHeight + 0.f;
  }
  return tiles;
}

void TmxMap::RemoveTile(const CPoint& position, const string& layerName)
{
  CPoint pos = GetPositionToTileCoordinate(position);
  auto layer = m_layers.at(layerName);
  layer->SetAt(0, (int)pos.y, (int)pos.x);
}

TmxMap::~TmxMap()
{

  for (auto l : m_layers) {
    delete l.second;
  }

  delete m_tileSet;
}

void TmxMap::Scroll(DIRECTION scrollDirection)
{
  float dt = scrollDirection == DIRECTION::DIR_LEFT ? SCROLL_SPEED : -SCROLL_SPEED;
  if (myPosition.x + dt >= m_screenWidth - m_pixelWidth / 2 && myPosition.x + dt <= m_pixelWidth / 2)
  {
    myVelocity.x = dt;
    return;
  }

  myVelocity.x = 0.f;
}

void TmxMap::Update(float dt)
{
  m_prevPos = myPosition;

 
  //CGameObject::Update(dt);

  ResolveCollision();
  // update the position of the map
  myPosition += myVelocity;

  // update the postion of the first tile to be drawn 
  int x = (int)myPosition.x % tileWidth + (myPosition.x > 0 ? -tileWidth : 0);
  if (abs(x) == tileWidth)
  {
    x = 0;
  }
  m_firstTilePos.x = x + tileWidth / 2 + 0.0f;
  m_firstTilePos.y = (int)myPosition.y % tileHeight + 0.0f + tileHeight / 2;

  // update the row of the first tile to be drawn
  m_tileRow = (m_pixelHeight / 2 - (int)myPosition.y) / tileHeight;

  // update the column of the first tile to be drawn
  m_tileColumn = (m_pixelWidth / 2 - (int)myPosition.x) / tileWidth;

  myVelocity.x = 0.f;

}

void TmxMap::ResolveCollision()
{
  if ((myCollisionSide & CollisionSide::CS_Left && myVelocity.x > 0) || (myCollisionSide & CollisionSide::CS_Right && myVelocity.x < 0))
  {
    myVelocity.x = 0;
    myPosition = m_prevPos;
  }

  myCollisionSide = CollisionSide::CS_None;
}

void TmxMap::Save()
{
  ofstream g("data/savedmap.bin");
  g << width << " " << height << " " << tileWidth << " " << tileHeight << " " << m_layers.size() << " " << myPosition.x << " " << myPosition.y << "\n";

  for (auto layer : m_layers)
  {
    g << layer.first << "\n";
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        g << layer.second->GetAt(i, j) << " ";
      }
      g << "\n";
    }
  }
  g.close();
}

void TmxMap::Restore()
{
  LoadMap("data/savedmap.bin");
  myVelocity.x = 0.f;
}