#include "ClientEngine.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <signal.h>
#include <common/GameMessages.hpp>
#include "handlers/ClientLoginHandler.hpp"
#include "handlers/ClientCustomHandler.hpp"
#include "handlers/ClientEntitySpawnHandler.hpp"
#include "handlers/ClientWorldStateHandler.hpp"

#include <components/BodyCmp.hpp>
#include <components/ClientPlayerSystem.hpp>
#include <components/AnimationComponent.hpp>
#include <components/CollisionComponent.hpp>
#include <components/MapCmp.hpp>
#include <components/GraphicsCmp.hpp>
#include <components/ScriptSystem.hpp>
#include <components/VisibilityComponent.hpp>

//#include "handlers/ClientMapHandler.hpp"
#include "voyage.pb.h"
#include <iostream>

namespace spd = spdlog;
Entity::Id ClientEngine::_player_entity_id;

void ClientEngine::init() {

	std::cout << "Initializing ClientEngine";

	GameEngine::init();

	_netManager.start("127.0.0.1", 1234);
	_netManager.connect();
	_running = true;

	_registerHandlers();

	createWorld();
	//createPlayer();

	// Create client subsystems
	// Subsystems should be created at the end!
	createSubsystems();
}


void ClientEngine::createSubsystems() {

	// TODO: this goes up to the server as well

	// NOTE: Order matters

	// Player system
	std::shared_ptr<ClientPlayerSystem> ps =
		std::make_shared<ClientPlayerSystem>(_world, _netMsgPool, _netManager);
	ps->configure(_eventManager);
	add<ClientPlayerSystem>(ps);

	add<ScriptSystem<BodyCmp> >(std::make_shared<ScriptSystem<BodyCmp> >());
	add<CollisionSystem>(std::make_shared<CollisionSystem>(_world));
	add<GraphicsSystem>(std::make_shared<GraphicsSystem>());
	add<AnimationSystem>(std::make_shared<AnimationSystem>());
	add<MapDrawSystem>(std::make_shared<MapDrawSystem>(_world));
	add<VisibilitySystem>(std::make_shared<VisibilitySystem>(_world));

}

void ClientEngine::createWorld() {

	_world = std::make_shared<World>();

	// Entity map = _entityManager.createLocal();

	// _entityManager.assign<MapComponent>(map.id(),
	// 				    _world->getGameMap()->getTileMap());
}

void ClientEngine::createPlayer(uint32_t entity_id, float x, float y) {

	// NO! This id isNetweorked. will be assigned by the server
	//Entity player1 = _entityManager.createNetworked(id);

	Entity player_entity = _entityManager.createFromId(entity_id);
    _player_entity_id = player_entity.id();

	// TODO server spawn msg returns config file and initial params

	pumpkin::Configuration<pumpkin::Animation::Config> config
		(std::string(CONFIG_FILE_PATH) +
		 "main_character_anim3.cfg");
	_entityManager.assign<AnimationComponent>(player_entity.id(),
						  config.config());

	//TODO: This should be a response from a server createEntity message
	_entityManager.assign<PlayerCmp>(player_entity.id(), GameEngine::m_playerId);
	_entityManager.assign<BodyCmp>(player_entity.id(), "cfg");
	_entityManager.assign<NetworkCmp>(player_entity.id());
	_entityManager.assign<CollisionComponent>(player_entity.id());
	_entityManager.assign<VisibilityCmp>(player_entity.id());

	// Client-side components
	_entityManager.assign<DebugGraphicsCmp>(player_entity.id());


	BodyCmp *body = _entityManager.getComponentPtr<BodyCmp>(player_entity.id());
	body->m_position(0) = x;
	body->m_position(1) = y;


// 	Entity player_entity2 = _entityManager.create();
// 	_entityManager.assign<PlayerCmp>(player2.id());
// 	_entityManager.assign<CollisionComponent>(player2.id());
// 	_entityManager.assign<NetworkCmp>(player2.id());
// 	_entityManager.assign<BodyCmp>(player2.id(), "cfg");
// 	_entityManager.assign<AnimationComponent>(player2.id(),
// 						  std::string(CONFIG_FILE_PATH) +
// 						  "main_character_anim3.cfg");
}

void ClientEngine::_registerHandlers() {

	_netManager.registerHandler(std::make_shared<ClientLoginHandler>
				    (ClientEngine::Ptr(this)),
				    {
					    ID_SC_LOGIN_ACCEPTED,
					    ID_CONNECTION_REQUEST_ACCEPTED
				    });

//	_netManager.registerHandler(std::make_shared<ClientCustomHandler<voyage::sc_worldState> >
//				     (&_eventManager), {ID_SC_WORLD_STATE});

	_netManager.registerHandler(std::make_shared<ClientWorldStateHandler>
				    (ClientEngine::Ptr(this)), {ID_SC_WORLD_STATE});

	_netManager.registerHandler(std::make_shared<ClientEntitySpawnHandler>
				    (ClientEngine::Ptr(this)),
				    {
					    ID_SC_SPAWN_PLAYER,
					    ID_SC_SPAWN_ENTITY,
					    ID_SC_DESTROY_ENTITY,
				    });


}

void ClientEngine::requestQuit() {
	_running = false;
}
