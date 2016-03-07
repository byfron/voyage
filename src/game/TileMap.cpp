#include "TileMap.hpp"
#include "MapRegion.hpp"
#include <iostream>
#include <assert.h>

TileMap::TileMap() {

	std::cout << "creating tilemap" << std::endl;
	//DEBUG: generate random tilemap
	for (int i = 0; i < MAX_TILEMAP_SIZE_X; i++)
		for (int j = 0; j < MAX_TILEMAP_SIZE_Y; j++) {
			_tile_data[i][j] = uint32_t((std::rand()/float(RAND_MAX))*float(0xFF));
		}

	//make regions
	_regionW = 32;
	_regionH = 32;

	int num_regions_x = MAX_TILEMAP_SIZE_X/_regionW; 
	int num_regions_y = MAX_TILEMAP_SIZE_Y/_regionH;

	for (int i = 0; i < num_regions_x; i++)
		for (int j = 0; j < num_regions_y; j++) {

			TilePosition tl(i*_regionW, j*_regionH);
			TilePosition br((i+1)*_regionW, (j+1)*_regionH);
			_regions.push_back(std::make_shared<MapRegion>(tl,br));
			}
}

TileMap::TileMap(std::string file) {
	
}

MapRegion::Ptr TileMap::getMapRegionFromTile(const int x, const int y) {
	for (int i = 0; i < _regions.size(); i++) {
		if (_regions[i]->tileInRegion(x,y))
			return _regions[i];
	}

	assert(false);
}

std::vector<uint32_t> TileMap::getRegionTiles(MapRegionPtr r) {
	std::vector<uint32_t> tiles;

	int cornerx = r->getTopLeft().tilex;
	int cornery = r->getTopLeft().tiley;	

	//TODO: take into account map limits!?	
	for (int i = 0; i < _regionW; i++)
		for (int j = 0; j < _regionH; j++) {
			tiles.push_back(_tile_data[cornerx + i][cornery + j]);
		}
	return tiles;	
}
