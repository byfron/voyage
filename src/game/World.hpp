#pragma once

#include "TileMap.hpp"
#include "WorldProperties.hpp"

/**
 * Main container of game objects
 */
class World {

public:
	World();
	//void onLogin(PlayerSession);

	typedef std::shared_ptr<World> Ptr;

	TileMap::Ptr getTileMap() { return _tilemap; }

private:

	TileMap::Ptr _tilemap;
	WorldProperties _properties;
};
