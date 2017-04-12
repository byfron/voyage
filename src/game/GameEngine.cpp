#include "GameEngine.hpp"
#include <components/BodyCmp.hpp>
#include <components/PlayerSystem.hpp>

bool GameEngine::isClient = true;
Entity::Id GameEngine::m_playerId = Entity::Id(0);

void GameEngine::init() {

	// create system manager
	_systemManager = std::make_shared<SystemManager>(_entityManager,
							 _eventManager);

	// Create subsystems. Systems defined here will be present
	// in both server and client side

	// Body system
//	add<BodySystem>(std::make_shared<BodySystem>());

}
