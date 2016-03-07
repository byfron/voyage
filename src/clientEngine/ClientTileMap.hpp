#pragma once
#include <memory>
#include <game/TileMap.hpp>

class ClientTileMap {
public:

	ClientTileMap();
	void fill(int x, int y, int w, int h, const std::vector<uint32_t> & tiles);
	typedef std::shared_ptr<ClientTileMap> Ptr;

private:

	TilePosition _playerPosition;
	uint32_t _tile_data[MAX_TILEMAP_SIZE_X][MAX_TILEMAP_SIZE_Y];		
};
