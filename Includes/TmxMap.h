#pragma once
#include <string>
#include <vector>
#include <map>
#include "Sprite.h"
#include "Rectangle.h"
#include "TmxMapLayer.h"
#include "TileSprite.h"
#include "CPoint.h"
#include "CGameObject.h"
using namespace std;

enum  DIRECTION
{
  DIR_LEFT = 1,
  DIR_RIGHT = 2
};


class TmxMap : public CGameObject
{
public:

  /**
  * Constructs a new TmxMap.
  * @param filename - the name of the input file
  * @param spritename - the name of the tileset
  */
  TmxMap(const string&, const string&);

  ~TmxMap();

  /**
  * Converts screen position to tile coordinate
  */
  CPoint GetPositionToTileCoordinate(const CPoint&);

  /**
  * Converts tile coordinate to screen position
  */
  CPoint GetTileCoordinateToPosition(const CPoint&);


  /**
  * Gets the surrounding tiles of an rectangle
  */
  vector<CRectangle> GetSurroundingTiles(const CRectangle&, const string&);

  /**
  * Gets the surrounding tiles of a point
  */
  vector<CRectangle> GetSurroundingTiles(const CPoint&, const string&);

  /**
  * Erase a specified tile from a specified layer
  * @param position - the pixel coordinates of the tile
  * @param layerName - the name of the layer
  */
  void RemoveTile(const CPoint&, const string&);

  /**
  * Saves map position and tiles 
  */
  void Save();

  /**
  * Loads a saved map.
  */
  void Restore();
  void Scroll(DIRECTION);


  // Overrides
  int GetWidth() const { return m_pixelWidth; }
  int GetHeight() const { return m_pixelHeight; }
  void Draw();
  void Update(float);
  void ResolveCollision();

private:
  /**
  * Tile sprite used to draw all visible tiles of the map.
  */
  TileSprite* m_tileSet;

  /**
  * The width of the map in pixels.
  */
  int m_pixelWidth;

  /**
  * The height of the map in pixels.
  */
  int m_pixelHeight;

  /**
  * The width of the map in tiles.
  */
  int width;

  /**
  * The height of the map in tiles.
  */
  int height;
  
  /**
  * The width of a tile.
  */
  int tileWidth;
  
  /**
  * The height of a tile.
  */
  int tileHeight;

  /**
  * The width of the screen.
  */
  int m_screenWidth;
 
  /**
  * The height of the screen.
  */
  int m_screenHeight;

  /**
  * The number of tiles to be draw on width.
  */
  int m_widthTileNumber;

  /**
  * The number of tiles to be draw on height.
  */
  int m_heightTileNumber;

  /*
  * The position of the upper-left tile to be drawn.
  */
  CPoint m_firstTilePos;

  /**
  * The row of the tile to be drawn.
  */
  int m_tileRow;
  
  /**
  * The column of the tile to be drawn.
  */
  int m_tileColumn;
 
  /**
  * A map with string key representing the name of the TmxMapLayer and value the corresponding TmxMapLayer.
  */
  map<string, TmxMapLayer*> m_layers;

  /**
  * Loads the map.
  */
  void LoadMap(const string& filename);

  /**
  * The previous position before a collision.
  */
  Vec2 m_prevPos;

};

