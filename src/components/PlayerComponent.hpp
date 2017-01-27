#pragma once

#include <entities/System.hpp>
#include <utils/Interval.hpp>
#include <pumpkin.hpp>

// TODO:
/*
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

class PlayerInput {
public:
	PlayerState(float moveSpeed) : m_moveSpeed(moveSpeed) {
		m_position = Eigen::Vector3f(0.0f,1.5f,0.0f);//initial position (cfg!)
		m_direction = CardinalDirection::South;
		m_action = PlayerAction::Walking;
	}

	void computeOrientation() {

		//if (USER_PRESSING_FIRE)

		if (pumpkin::InputManager::isLeftMouseButtonPressed())
		{
			MultiInterval<float> mint(Interval<float>(-M_PI, M_PI), 4);
			int idx = mint.indexContains(pumpkin::InputManager::m_mouse_angle);
			m_direction = CardinalDirection(idx);
		}
		else {

		}
	}

	void computeAction() {
		if (pumpkin::InputManager::m_keys != 0) {
			m_action = PlayerAction::Walking;
		}
	}

	uint32_t getAnimationId() {

		if (m_action == PlayerAction::Walking) {
			//TODO: read from a table
			if (m_direction == CardinalDirection::East) {
				return 2;//AnimationId::RIGHT;
			}

			if (m_direction == CardinalDirection::West) {
				return 3;//AnimationId::LEFT;
			}

			if (m_direction == CardinalDirection::North) {
				return 1;//AnimationId::TOP;
			}
			if (m_direction == CardinalDirection::South) {
				return 0;//AnimationId::DOWN;
			}
		}
	}

	Eigen::Vector3f getMoveDirection() {

		Eigen::Vector3f direction = Eigen::Vector3f(0.0f, 0.0f, 0.0f);

		if (pumpkin::InputManager::m_keys & KEY_MOVE_UP) {
			direction += pumpkin::GraphicsEngine::camera().getForwardDir();
			m_direction = CardinalDirection::North;
		}
		if (pumpkin::InputManager::m_keys & KEY_MOVE_DOWN) {
			direction += -pumpkin::GraphicsEngine::camera().getForwardDir();
			m_direction = CardinalDirection::South;
		}
		if (pumpkin::InputManager::m_keys & KEY_MOVE_RIGHT) {
			direction += -pumpkin::GraphicsEngine::camera().getRightDir();
			m_direction = CardinalDirection::East;
		}
		if (pumpkin::InputManager::m_keys & KEY_MOVE_LEFT) {
			direction += pumpkin::GraphicsEngine::camera().getRightDir();
			m_direction = CardinalDirection::West;
		}

		return direction;

	}

	Eigen::Vector3f getPosition() const {
		return m_position;
	}

	void update(float dt, const TileMap::CollisionMask & mask) {


		getMoveDirection() * m_moveSpeed * dt;

		m_position += * m_moveSpeed * dt;
		computeAction();
		computeOrientation();

	}

private:

	float m_moveSpeed;
	CardinalDirection m_direction;
	PlayerAction m_action;
};

class StateSystem : public System<StateSystem> {

	StateSystem(World & world) : m_world(world) {

	}

	void update(EntityManager & em, EventManager &evm, float delta ) {

		TileMap::CollisionMask mask = m_world.getTilemapCollisionMask();

		em.each<PlayerInput, BodyComponent>([delta](Entity entity,
							    PlayerInput &pinput,
							    BodyComponent &body) {

			    input.update(delta, mask);
			    body.m_move_vector = input.getMoveVector();
 	        });
	}

	// void update(EntityManager & em, EventManager &evm, float delta ) {
	// 	em.each<AIComponent, BodyComponent>([delta](Entity entity, PlayerState &state) {

	// }

private:

	World & m_world;
};
