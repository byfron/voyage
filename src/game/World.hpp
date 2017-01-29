#pragma once

#include "GameMap.hpp"
#include "WorldProperties.hpp"

/**
 * Main container of game objects
 */
class World {

public:
	World();
	//void onLogin(PlayerSession);

	typedef std::shared_ptr<World> Ptr;

	GameMap::Ptr getGameMap() { return _game_map; }
	GameMap::CollisionMask & getMapCollisionMask() { return _game_map->getCollisionMask(); }

private:

	GameMap::Ptr _game_map;
	WorldProperties _properties;
};
