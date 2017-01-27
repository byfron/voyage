#pragma once

#include <networking/ClientNetworkManager.hpp>
#include <entities/Entity.hpp>
#include <entities/System.hpp>
#include "PlayerData.hpp"

class ClientEngine {
public:
	~ClientEngine() {}
	void init();
	void start() {};
	void clientLoop();
	void requestQuit();
	void processFrame();
	void initPlayer(PlayerData & playerData);

	void run_frame(float dt) {
		_systemManager->update_all(dt);
	}

	EntityManager & entityManager() { return _entityManager; }
	EventManager & eventManager() { return _eventManager; }

	template <typename S>
	void add(std::shared_ptr<S> system) {
		_systemManager->add<S>(system);
	}

//	ClientTileMap::Ptr getTileMap() { return _tileMap; };
	ClientNetworkManager::Ptr getNetworkManager() { return _netManager; }
	typedef std::shared_ptr<ClientEngine> Ptr;


private:

	void _registerHandlers();

	EntityManager _entityManager;
	EventManager _eventManager;

//	ClientTileMap::Ptr _tileMap;
	SystemManager::Ptr _systemManager;
	ClientNetworkManager::Ptr _netManager;

	bool _running;


};
