#pragma once

#include <networking/ClientNetworkManager.hpp>
#include <game/GameEngine.hpp>

class ClientEngine : public GameEngine {
public:
	~ClientEngine() {}
	void init();
	void start() {};
	void clientLoop();
	void requestQuit();
	
       	EntityManager & entityManager() { return _entityManager; }
	EventManager & eventManager() { return _eventManager; }

	World::Ptr getWorld() { return _world; }

	void createSubsystems();
	void createWorld();
	void createPlayer();	

	void receiveData()  { _netManager.receiveData(); }
	
//	ClientTileMap::Ptr getTileMap() { return _tileMap; };

	ClientNetworkManager & getNetManager() { return _netManager; }

	typedef std::shared_ptr<ClientEngine> Ptr;


private:

	void _registerHandlers();
	ClientNetworkManager  _netManager;

};
