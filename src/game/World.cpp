#include "World.hpp"
#include <iostream>

World::World() {

	std::cout << "initializing world" << std::endl;
	_game_map = std::make_shared<GameMap>(std::string(CONFIG_FILE_PATH) + "map.cfg");
}
