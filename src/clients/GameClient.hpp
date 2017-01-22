#pragma once

#include <engine/ResourceManager.hpp>
#include <engine/Engine.hpp>
#include <clientEngine/ClientEngine.hpp>
#include <entities/Entity.hpp>
#include <entities/System.hpp>
#include <entities/Animation2D.hpp>
#include <entities/AnimationFactory.hpp>
#include <engine/TileMapChunk.hpp>
#include <Eigen/Dense>
#include <utils/Interval.hpp>

// TODO:
/*
  * Fix installation of shaders/textures
  * Clean up a bit/ write diagrams
  * Implement simple map+two entities and move them with two clients
*/
enum class PlayerAction {
	Idle,
	Walking,
	Rolling,
	Shooting
};

enum class CardinalDirection {
	East,
	North,
	West,
	South,
	// NorthEast,
	// NorthWest,
	// SouthWest,
	// SouthEast,
};

class PlayerState {
public:
	PlayerState(float moveSpeed) : m_moveSpeed(moveSpeed) {
		m_position = Eigen::Vector3f(0.0f,1.5f,0.0f);//initial position (cfg!)
		m_direction = CardinalDirection::South;
		m_action = PlayerAction::Walking;
	}

	void computeOrientation() {
		MultiInterval<float> mint(Interval<float>(-M_PI, M_PI), 4);
		int idx = mint.indexContains(InputManager::m_mouse_angle);
		m_direction = CardinalDirection(idx);
	}

	void computeAction() {
		if (InputManager::m_keys != 0) {
			m_action = PlayerAction::Walking;
		}
	}

	AnimationId getAnimationId() {

		if (m_action == PlayerAction::Walking) {
			//TODO: read from a table
			if (m_direction == CardinalDirection::East) {
				return AnimationId::RIGHT;
			}

			if (m_direction == CardinalDirection::West) {
				return AnimationId::LEFT;
			}

			if (m_direction == CardinalDirection::North) {
				return AnimationId::TOP;
			}
			if (m_direction == CardinalDirection::South) {
				return AnimationId::DOWN;
			}
		}
	}
	
	Eigen::Vector3f getMoveDirection() {

		Eigen::Vector3f direction = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
			
		if (InputManager::m_keys & KEY_MOVE_UP) {
			direction += Engine::camera().getForwardDir();
		}
		if (InputManager::m_keys & KEY_MOVE_DOWN) {
			direction += -Engine::camera().getForwardDir();
		}
		if (InputManager::m_keys & KEY_MOVE_RIGHT) {
			direction += -Engine::camera().getRightDir();
		}
		if (InputManager::m_keys & KEY_MOVE_LEFT) {
			direction += Engine::camera().getRightDir();
		}
		
		return direction;
		
	}

	Eigen::Vector3f getPosition() const {
		return m_position;
	}
	
	void update(float dt) {

//		m_action = PlayerAction::Idle;		
		m_position += getMoveDirection()* m_moveSpeed * dt;
		computeAction();
		computeOrientation();
		
	}

private:

//	TileIndex m_tile;
	 
	float m_moveSpeed;
	Eigen::Vector3f m_position;
	CardinalDirection m_direction;
	PlayerAction m_action;
};

class MapComponent {
public:
	MapComponent(const std::string & conf_file) : m_mapChunk(0,0,10,10) {}
	void update(float delta ) {
		m_mapChunk.update(delta);
	}
	
private:

	TileMapChunk m_mapChunk;
	
};

// shall the graphic elements be organized per texture?
class AnimationComponent {
public:
	AnimationComponent(const std::string & config_file) :		
		m_animation("", "", 0.5, 0.5) { //HOW to handle this?
		// this details should be "hidden" in pumpkin
		AnimationFactory factory(config_file);
		factory.generate(&m_animation);
	}
	
	void update(float delta ) {
		m_animation.update(delta);
	}

	Animation2D & getAnimation2D() {
		return m_animation;
	}
	
private:

	uint32_t m_resource_id;
	Animation2D m_animation;
};


// 1 modify PlayerState

// 2 paint Object state

class AnimationSystem : public System<AnimationSystem> {
public:
	void update(EntityManager & em, /*EventManager &evm*/ float delta ) {
		em.each<AnimationComponent, PlayerState>([delta](Entity entity,
								 AnimationComponent &anim,
								 PlayerState & state) {

//			 if (state.isDirty()) {
				 anim.getAnimation2D().setPosition(state.getPosition());
				 anim.getAnimation2D().switchToAnim(state.getAnimationId());
//				 state.clean();
//			 }
			 
			 // anim.getAnimation2D().setAnimation(
			 //	 AnimationManager::actionToAnim(state.getAction()));
			 anim.update(delta);
			 // now we can do whatever logic in the system which is the point!
		 });
	}

private:

};
	
class MapSystem : public System<MapSystem> {
public:
	void update(EntityManager & em, /*EventManager &evm*/ float delta ) {
		em.each<MapComponent>([delta](Entity entity,
					      MapComponent &map) {

			       //update graphics with player state
				map.update(delta);
		});      	
	}	
};


class StateSystem : public System<StateSystem> {

	void update(EntityManager & em, /*EventManager &evm*/ float delta ) {
		em.each<PlayerState>([delta](Entity entity, PlayerState &state) {
				state.update(delta);
 	        });

//		em.each<ObjectState>([delta](Entity entity,
//
//					     });      	
	}	

};

class GameClient : public Engine {
public:

	GameClient() {
	
	}
	
	~GameClient() {}

	void init_engine() override {

		_gameEngine.init() ; //ugly with a engine from pumpkin

		Entity player1 = _gameEngine.entityManager().create();	      
		Entity map = _gameEngine.entityManager().create(); //should be tiles?
	
		 
		_gameEngine.entityManager().assign<AnimationComponent>(player1.id(),
								       "config.cfg");

		//NOTE: same speed as camera. TODO: Load from config file!!!
		_gameEngine.entityManager().assign<PlayerState>(player1.id(), 5.0f); 

		_gameEngine.entityManager().assign<MapComponent>(map.id(),
		 						 "map.cfg");
		
		_gameEngine.add<StateSystem>(std::make_shared<StateSystem>());
		_gameEngine.add<AnimationSystem>(std::make_shared<AnimationSystem>());
		_gameEngine.add<MapSystem>(std::make_shared<MapSystem>());

//		_gameEngine.add<SoundSystem>();
	}

	void frame(const float dt) override {
		_gameEngine.run_frame(dt);		
	}
	
private:

	ClientEngine _gameEngine;

	
};
