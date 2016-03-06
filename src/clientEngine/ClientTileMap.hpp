#pragma once
#include <memory>
#include <game/TileMap.hpp>

class ClientTileMap {
public:

	
	typedef std::shared_ptr<ClientTileMap> Ptr;

private:

	TilePosition _playerPosition;
	
	
};
