#pragma once

#include <game/World.hpp>
#include <entities/Entity.hpp>
#include <entities/System.hpp>
#include <networking/NetworkManager.hpp>


class GameEngine {
public:


	template <typename S>
	void add(std::shared_ptr<S> system) {
		_systemManager->add<S>(system);
	}

	void run_frame(float dt) {

		//	_netManager->receiveData();
		_systemManager->update_all(dt);
	}
	
	virtual void init();

	template <typename E>
	void dispatchMessage(const E & msg) {
		_eventManager.emit(msg);
	}

	EntityManager & entity_manager() { return _entityManager; }

	static bool isClient;
	static uint32_t m_playerId;

protected:

	bool _running;
	bool _finished;
	
	EntityManager _entityManager;
	EventManager _eventManager;
	SystemManager::Ptr _systemManager;
	World::Ptr _world;
//	std::shared_ptr<NetworkManager> _netManager;

};
