#pragma once

#include <game/World.hpp>
#include <entities/Entity.hpp>
#include <entities/System.hpp>
#include <networking/NetworkManager.hpp>
#include <components/BodyCmp.hpp>

class GameEngine {
public:


	template <typename S>
	void add(std::shared_ptr<S> system) {
		_systemManager->add<S>(system);
	}

	void run_frame(float dt) {

		_netManager->receiveData();
		_systemManager->update_all(dt);
	}
	
	virtual void init() {

		// create system manager
		_systemManager = std::make_shared<SystemManager>(_entityManager,
								 _eventManager);

		// Create subsystems. Systems defined here will be present
		// in both server and client side
		add<BodySystem>(std::make_shared<BodySystem>());
	}

protected:

	bool _running;
	bool _finished;
	
	EntityManager _entityManager;
	EventManager _eventManager;
	SystemManager::Ptr _systemManager;
	World::Ptr _world;
	std::shared_ptr<NetworkManager> _netManager;
};
