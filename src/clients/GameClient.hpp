#pragma once

#include <components/BodyCmp.hpp>
#include <components/InputCmp.hpp>
#include <components/AnimationComponent.hpp>
#include <components/ObjectStateSystem.hpp>
#include <components/ScriptSystem.hpp>
#include <components/GraphicsCmp.hpp>
#include <clientEngine/ClientEngine.hpp>
#include <pumpkin.hpp>
#include <Eigen/Dense>
#include <tilemap.pb.h>
#include <utils/Configuration.hpp>
#include <graphics/TileMap.hpp>

class MapComponent {
public:
	MapComponent(pumpkin::TileMap & tmap) : m_tileMap(tmap) {
	}
	void update(float delta ) {
		m_tileMap.update(delta);
	}

private:

	pumpkin::TileMap & m_tileMap;

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
					       std::string(CONFIG_FILE_PATH) +
					       "main_character_anim2.cfg");

		//NOTE: same speed as camera. TODO: Load from config file!!!
		_gameEngine.entityManager().assign<BodyCmp>(id, "cfg");
		_gameEngine.entityManager().assign<InputCmp>(id, "cfg");


//		_gameEngine.entityManager().assign<ScriptCmp>(id, "test.py");
		_gameEngine.entityManager().assign<CollisionComponent>(id);
		_gameEngine.entityManager().assign<DebugGraphicsCmp>(id);

	}

	void createMap(Entity::Id id) {
		_gameEngine.entityManager().assign<MapComponent>(id,
		 _gameEngine.getWorld()->getGameMap()->getTileMap());
	}

	void init_engine() override {

		_gameEngine.init() ; //ugly with a engine from pumpkin

		Entity player1 = _gameEngine.entityManager().create();
		Entity map = _gameEngine.entityManager().create(); //should be tiles?

		createPlayer(player1.id());
		createMap(map.id());


		_gameEngine.add<ObjectStateSystem>(std::make_shared<ObjectStateSystem>
						   (_gameEngine.getWorld()));
		_gameEngine.add<BodySystem>(std::make_shared<BodySystem>());
		_gameEngine.add<CollisionSystem>(std::make_shared<CollisionSystem>());

		// graphics stuff

		// TODO: Refactor graphics engine into RenderSystem.

		_gameEngine.add<AnimationSystem>(std::make_shared<AnimationSystem>());
		_gameEngine.add<GraphicsSystem>(std::make_shared<GraphicsSystem>());
		_gameEngine.add<MapSystem>(std::make_shared<MapSystem>());

		//////////////////


		_gameEngine.add<ScriptSystem<BodyCmp> >(std::make_shared<ScriptSystem<BodyCmp> >());
//		_gameEngine.add<DebugGraphicsSystem>(std::make_shared<DebugGraphicsSystem>());

	}

	void frame(const float dt) override {
		_gameEngine.run_frame(dt);
	}

private:

	ClientEngine _gameEngine;


};
