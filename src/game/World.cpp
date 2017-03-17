#include "World.hpp"
#include <iostream>

World::World() : _game_level("test2.fbx") {

	std::cout << "initializing world" << std::endl;
//	_game_map = std::make_shared<GameMap>(std::string(CONFIG_FILE_PATH) + "map.cfg");
}
