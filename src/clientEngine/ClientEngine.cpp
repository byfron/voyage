#include "ClientEngine.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <signal.h>
#include <common/GameMessages.hpp>
#include "handlers/ClientLoginHandler.hpp"
#include "handlers/ClientEntityUpdateHandler.hpp"


#include <components/BodyCmp.hpp>
#include <components/EntityInputSystem.hpp>
#include <components/AnimationComponent.hpp>
#include <components/CollisionComponent.hpp>
#include <components/MapCmp.hpp>
#include <components/GraphicsCmp.hpp>
#include <components/ScriptSystem.hpp>

//#include "handlers/ClientMapHandler.hpp"
#include "voyage.pb.h"
#include <iostream>

namespace spd = spdlog;

void ClientEngine::init() {

	std::cout << "Initializing ClientEngine";

	GameEngine::init();
	
	_netManager = std::make_shared<ClientNetworkManager>();
	networkManager()->start("127.0.0.1", 1234);
	networkManager()->connect();
	_running = true;

	_registerHandlers();

	createWorld();
	createPlayer();

	// Create client subsystems
	// Subsystems should be created at the end!
	createSubsystems();
}


void ClientEngine::createSubsystems() {

	// TODO: this goes up to the server as well

	// NOTE: Order matters
	add<EntityInputSystem>(std::make_shared<EntityInputSystem>(_world, networkManager()));
	add<ScriptSystem<BodyCmp> >(std::make_shared<ScriptSystem<BodyCmp> >());
	add<CollisionSystem>(std::make_shared<CollisionSystem>(_world));
	add<AnimationSystem>(std::make_shared<AnimationSystem>());
	add<GraphicsSystem>(std::make_shared<GraphicsSystem>());
	add<MapSystem>(std::make_shared<MapSystem>());
	
}

void ClientEngine::createWorld() {

	_world = std::make_shared<World>();

	Entity map = _entityManager.create();
		
	_entityManager.assign<MapComponent>(map.id(),
					    _world->getGameMap()->getTileMap());
}

void ClientEngine::createPlayer() {

	Entity player1 = _entityManager.create();

	_entityManager.assign<AnimationComponent>(player1.id(),
						  std::string(CONFIG_FILE_PATH) +
						  "main_character_anim3.cfg");
	
	//TODO: This should be a response from a server createEntity message
	_entityManager.assign<BodyCmp>(player1.id(), "cfg");
	_entityManager.assign<CollisionComponent>(player1.id());

	// Client-side components
	_entityManager.assign<InputCmp>(player1.id(), "cfg");
	_entityManager.assign<DebugGraphicsCmp>(player1.id());
}


void ClientEngine::_registerHandlers() {

	_netManager->registerHandler(std::make_shared<ClientLoginHandler>
				     (ClientEngine::Ptr(this)), {ID_SC_LOGIN_ACCEPTED});

	_netManager->registerHandler(std::make_shared<ClientEntityUpdateHandler>
				     (&_eventManager), {ID_SC_ENTITY_UPDATE});
	
//	_netManager->registerHandler(std::make_shared<ClientMapHandler>(ClientEngine::Ptr(this)),
//				     {ID_SC_REGION_DATA});
}

void ClientEngine::requestQuit() {
	_running = false;
}
