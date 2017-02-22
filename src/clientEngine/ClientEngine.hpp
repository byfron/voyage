#pragma once

#include <networking/ClientNetworkManager.hpp>
#include <game/GameEngine.hpp>
#include <networking/NetworkMessagePool.hpp>

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
	void createPlayer(uint32_t index, int x, int y);	

	void receiveData()  { _netManager.receiveData(); }
	
//	ClientTileMap::Ptr getTileMap() { return _tileMap; };

	ClientNetworkManager & getNetManager() { return _netManager; }
	NetworkMessagePool & msg_pool() { return _netMsgPool; }

	typedef std::shared_ptr<ClientEngine> Ptr;


private:

	void _registerHandlers();
	NetworkMessagePool _netMsgPool;
	ClientNetworkManager  _netManager;

};
