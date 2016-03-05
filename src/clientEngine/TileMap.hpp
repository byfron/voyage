#pragma once
#include <memory>

struct TilePosition {
	int tilex;
	int tiley;
};	

class TileMap {
public:

	
	typedef std::shared_ptr<TileMap> Ptr;

private:

	TilePosition _playerPosition;
	
	
};
