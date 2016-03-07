#include "World.hpp"
#include <iostream>

World::World() {

	std::cout << "initializing world" << std::endl;
	_tilemap = std::make_shared<TileMap>();
}

