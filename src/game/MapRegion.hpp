#pragma once
#include <game/TileMap.hpp>

/*
 * Defines a discretization of the tilemap to facilitate client updates
 */
class MapRegion {
public:

	MapRegion(TilePosition tl, TilePosition br) : _topLeft(tl), _bottomRight(br) {
	}
	
	bool tileInRegion(int x, int y) {
		if (x > _topLeft.tilex && x < _bottomRight.tilex &&
		    y > _topLeft.tiley && y < _bottomRight.tiley) return true;
		else return false;
	}
	
	typedef std::shared_ptr<MapRegion> Ptr;
	TilePosition getTopLeft() { return _topLeft; }
	TilePosition getBottomRight() { return _bottomRight; }
	
private:
	int _biome;
	TilePosition _topLeft;
	TilePosition _bottomRight;
	MapRegion::Ptr _northRegion;
	MapRegion::Ptr _southRegion;
	MapRegion::Ptr _westRegion;
	MapRegion::Ptr _easthRegion;
};
