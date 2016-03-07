#include "ClientTileMap.hpp"
#include <iostream>

ClientTileMap::ClientTileMap() {
	
	for (int i = 0; i < MAX_TILEMAP_SIZE_X; i++)
		for (int j = 0; j < MAX_TILEMAP_SIZE_Y; j++) {
			_tile_data[i][j] = 0;
		}
	
}

void ClientTileMap::fill(int x, int y, int w, int h, const std::vector<uint32_t> & tiles) {
	int idx = 0;
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++) {
			_tile_data[x + i][y + j] = tiles[idx];
			idx++;
		}
}
