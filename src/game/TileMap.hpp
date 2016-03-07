#pragma once

#include "WorldProperties.hpp"
#include <memory>
#include <vector>

#define MAX_TILEMAP_SIZE_X 512
#define MAX_TILEMAP_SIZE_Y 512

class MapRegion;
typedef std::shared_ptr<MapRegion> MapRegionPtr;

class WorldProperties;

struct TilePosition {
	TilePosition() {};
	TilePosition(int x, int y) : tilex(x), tiley(y) {}
	int tilex;
	int tiley;
};

class TileMap {
public:

	TileMap();
	TileMap(std::string file);

	int loadTileMap();

	char getTileId(int x, int y);

	void generate(const WorldProperties & properties);
	
	MapRegionPtr getMapRegionFromTile(const int x, const int y);
	std::vector<uint32_t> getRegionTiles(MapRegionPtr);
	int getRegionW() { return _regionW; }
	int getRegionH() { return _regionH; }
	
	typedef std::shared_ptr<TileMap> Ptr;
	
private:
	std::vector<MapRegionPtr> _regions;
	int _regionW;
	int _regionH;

	//bit: walkable/not walkable (this may be implicit in the tileID?)
	//bit: dirty (has been updated)
	//byte: TileID (graphics)
	//bit: solid/non-solid: for visibility or other stuff
	//3 bits: 8 biomes
	//3 bits: 8 within-biome types
 	uint32_t _tile_data[MAX_TILEMAP_SIZE_X][MAX_TILEMAP_SIZE_Y];
};
