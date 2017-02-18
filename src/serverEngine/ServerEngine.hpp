#pragma once

#include <game/GameEngine.hpp>
#include <networking/ServerNetworkManager.hpp>
#include <common/GameMessages.hpp>
#include <components/ServerPlayerSystem.hpp>
#include "handlers/ServerLoginHandler.hpp"
#include <chrono>
#include "voyage.pb.h"

typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_point_t;

class ServerEngine : public GameEngine {
public:

	void start() {
	}

	void processFrame();
	void init(int port);
	void createWorld();
	Entity createPlayerEntity(int);
	void createSubsystems();

	voyage::sc_worldState computeWorldState();

	ServerNetworkManager & getNetManager() { return _netManager; }
	
protected:

	time_point_t _lastEndTime;
	std::chrono::milliseconds _maxDeltaTime;
	ServerNetworkManager  _netManager;
};

