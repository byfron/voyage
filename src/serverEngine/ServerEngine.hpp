#pragma once

#include <game/GameEngine.hpp>
#include <networking/ServerNetworkManager.hpp>
#include <common/GameMessages.hpp>
#include "handlers/ServerLoginHandler.hpp"
#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_point_t;

class ServerEngine : public GameEngine {
public:

	void start() {
	}

	void processFrame() {
			       
		time_point_t now = std::chrono::high_resolution_clock::now();
		auto frameTime = now - _lastEndTime;
		_lastEndTime = now;
		//double delta = std::min(frameTime.count(), _maxDeltaTime);
		
		_netManager->receiveData();
		_systemManager->update_all(frameTime.count());
	}

	void init(int port) {

		GameEngine::init();
		
		//initialize server
		_netManager = std::make_shared<ServerNetworkManager>();
		networkManager()->start(port);
		_finished = false;

		//TODO: Decouple world from graphics to add the world/map class in the server
//		_world = std::make_shared<World>();

//		_maxDeltaTime((int)((1.0f / 60.0f) * 1000.0f));
	}

	ServerNetworkManager::Ptr networkManager() {
		return std::static_pointer_cast<ServerNetworkManager>(_netManager);
	}
	
protected:

	time_point_t _lastEndTime;
	std::chrono::milliseconds _maxDeltaTime;
};

