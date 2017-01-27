#pragma once

#include <components/PlayerComponent.hpp>
#include <components/AnimationComponent.hpp>
#include <clientEngine/ClientEngine.hpp>
#include <pumpkin.hpp>
#include <Eigen/Dense>

class MapComponent {
public:
	MapComponent(const std::string & conf_file) : m_tileMap(conf_file) {}
	void update(float delta ) {
		m_tileMap.update(delta);
	}

private:

	pumpkin::TileMap m_tileMap;

};

class MapSystem : public System<MapSystem> {
public:
	void update(EntityManager & em, EventManager &evm, float delta ) {
		em.each<MapComponent>([delta](Entity entity,
					      MapComponent &map) {

			       //update graphics with player state
				map.update(delta);
		});
	}
};

class GameClient : public pumpkin::GraphicsEngine {
public:

	GameClient() {

	}

	~GameClient() {}

	void createPlayer(Entity::Id id) {

		_gameEngine.entityManager().assign<AnimationComponent>(id,
			       std::string(CONFIG_FILE_PATH) + "main_character_anim.cfg");

		//NOTE: same speed as camera. TODO: Load from config file!!!
		_gameEngine.entityManager().assign<PlayerState>(id, 5.0f);
	}

	void createMap(Entity::Id id) {
		_gameEngine.entityManager().assign<MapComponent>(id, std::string(CONFIG_FILE_PATH) +
								 "map.cfg");
	}

	void init_engine() override {

		_gameEngine.init() ; //ugly with a engine from pumpkin

		Entity player1 = _gameEngine.entityManager().create();
		Entity map = _gameEngine.entityManager().create(); //should be tiles?

		createPlayer(player1.id());
		createMap(map.id());

		_gameEngine.add<StateSystem>(std::make_shared<StateSystem>());
		_gameEngine.add<AnimationSystem>(std::make_shared<AnimationSystem>());
		_gameEngine.add<MapSystem>(std::make_shared<MapSystem>());

	}

	void frame(const float dt) override {
		_gameEngine.run_frame(dt);
	}

private:

	ClientEngine _gameEngine;


};
