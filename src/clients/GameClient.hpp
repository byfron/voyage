#pragma once

#include <engine/ResourceManager.hpp>
#include <engine/Engine.hpp>
#include <clientEngine/ClientEngine.hpp>
#include <entities/Entity.hpp>
#include <entities/System.hpp>

// TODO:
/*
  * Fix installation of shaders/textures
  * Clean up a bit/ write diagrams
  * Implement simple map+two entities and move them with two clients
*/

class Graphics {
public:
	Graphics(ResourceManager * rm, uint32_t id) : _rm(rm), _resource_id(id) {}
	void update(float delta) {
		_rm->getResource(_resource_id)->update(delta);
	}
private:

	ResourceManager *_rm;
	uint32_t _resource_id;
};

class GraphicsSystem : public System {
public:
	void update(EntityManager & em, /*EventManager &evm*/ float delta ) {
		em.each<Graphics>([delta](Entity entity, Graphics &graphics) {
				graphics.update(delta);
		});
		
	}	
};


class GameClient : public Engine {
public:
	~GameClient() {}


	void init_engine() override {

		_gameEngine.init() ;

		// create tilemap entity (this would be a copy of server entity
		// after first map message)
		Entity tilemap = _gameEngine.entityManager().create();
		_gameEngine.entityManager().assign<Graphics>(tilemap.id(), &_resource_manager, 1);
		
		// create character entity (same)
		Entity player1 = _gameEngine.entityManager().create();		
		_gameEngine.entityManager().assign<Graphics>(player1.id(), &_resource_manager, 0);


		

		
		_gameEngine.add<GraphicsSystem>(std::make_shared<GraphicsSystem>());
//		_gameEngine.add<SoundSystem>();
	}

	void frame(const float dt) override {
		_gameEngine.run_frame(dt);		
	}
	
private:

	ClientEngine _gameEngine;

	
};
