#pragma once

#include "GameLevel.hpp"
#include "WorldProperties.hpp"

/**
 * Main container of game objects
 */
class World {

public:
	World();
	//void onLogin(PlayerSession);

	typedef std::shared_ptr<World> Ptr;

	GameLevel::CollisionMask & getLevelCollisionMask() { return _game_level.getCollisionMask(); }
	GameLevel & getGameLevel() { return _game_level; }

private:

//	GameMap::Ptr _game_map;
//	Scene::Ptr _game_map;

	GameLevel _game_level;
	WorldProperties _properties;
};
