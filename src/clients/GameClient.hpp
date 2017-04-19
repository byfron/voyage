#pragma once

#include <components/BodyComponent.hpp>
#include <components/AnimationComponent.hpp>
#include <clientEngine/ClientEngine.hpp>
#include <pumpkin.hpp>
#include <Eigen/Dense>
#include <tilemap.pb.h>
#include <utils/Configuration.hpp>

class GameClient : public pumpkin::GraphicsEngine {
public:

	GameClient() {
		auto console = spd::stdout_logger_mt("Client");
	}

	~GameClient() {}

	void init_engine() override {
		_gameEngine.init() ; //ugly with a engine from pumpkin
	}

	void frame(const float dt) override {
		_gameEngine.receiveData();
		_gameEngine.run_frame(dt);

	}

private:

	ClientEngine _gameEngine;


};
