#pragma once

#include <networking/ClientNetworkManager.hpp>
#include <game/GameEngine.hpp>
#include <networking/NetworkMessagePool.hpp>
#include <visibility/VisibilityManager.hpp>

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
	void createPlayer(uint32_t index, float x, float y);

	void receiveData()  { _netManager.receiveData(); }

//	ClientTileMap::Ptr getTileMap() { return _tileMap; };

	ClientNetworkManager & getNetManager() { return _netManager; }
	NetworkMessagePool & msg_pool() { return _netMsgPool; }

	typedef std::shared_ptr<ClientEngine> Ptr;

	static Entity::Id playerEntityId() { return _player_entity_id; }

private:

	void _registerHandlers();
	NetworkMessagePool _netMsgPool;
	ClientNetworkManager  _netManager;
	VisibilityManager _visManager;
	static Entity::Id _player_entity_id;

};
