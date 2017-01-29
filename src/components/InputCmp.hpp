#pragma once

#include <utils/InputManager.hpp>
#include <graphics/GraphicsEngine.hpp>
#include <utils/Interval.hpp>
#include <common/math.hpp>

enum class CardinalDirection {
	East,
	North,
	West,
	South,
};

enum class Action {
	IDLE,
	RUNNING,
	FACING_UP,
	FACING_DOWN,
	FACING_LEFT,
	FACING_RIGHT,
	SHOOTING		
};


class InputCmp {

public:

	InputCmp(const std::string &/*config*/) {

	}

	void updateMovementVector() {

		//TODO: make input manager more generic to allow for pads/key remaps
		Eigen::Vector3f direction = Eigen::Vector3f(0.0f, 0.0f, 0.0f);

		if (pumpkin::InputManager::m_keys & KEY_MOVE_UP) {
			direction += pumpkin::GraphicsEngine::camera().getForwardDir();
			m_action |= (1 << (int)Action::FACING_UP);
		}
		if (pumpkin::InputManager::m_keys & KEY_MOVE_DOWN) {
			direction += -pumpkin::GraphicsEngine::camera().getForwardDir();
			m_action |= (1 << (int)Action::FACING_DOWN);
		}
		if (pumpkin::InputManager::m_keys & KEY_MOVE_RIGHT) {
			direction += -pumpkin::GraphicsEngine::camera().getRightDir();
			m_action |= (1 << (int)Action::FACING_RIGHT);
		}
		if (pumpkin::InputManager::m_keys & KEY_MOVE_LEFT) {
			direction += pumpkin::GraphicsEngine::camera().getRightDir();
			m_action |= (1 << (int)Action::FACING_LEFT);
		}

		if (direction.norm() > 0) {
			m_action |= (1 << (int)Action::RUNNING);
		}
		
		m_move_vector = direction;
	}

	void updateLookatVector() {

		if (pumpkin::InputManager::isLeftMouseButtonPressed())
		{
			MultiInterval<float> mint(Interval<float>(-M_PI, M_PI), 4);
			int idx = mint.indexContains(pumpkin::InputManager::m_mouse_angle);
			m_lookat_direction = CardinalDirection(idx);

			switch(m_lookat_direction) {
			case CardinalDirection::East:
				m_action |= (1 << (int)Action::FACING_RIGHT);
				break;
			case CardinalDirection::West:
				m_action |= (1 << (int)Action::FACING_LEFT);
				break;
			case CardinalDirection::North:
				m_action |= (1 << (int)Action::FACING_UP);
				break;
			case CardinalDirection::South:
				m_action |= (1 << (int)Action::FACING_DOWN);
				break;				
			}
			
			m_action |= (1 << (int)Action::SHOOTING);
			
//			m_lookat_vector = InputManager::bla bla
		}
	}

	void reset() {		
		m_action = 0;
		m_move_vector = Vec3f::Zero();
	}
	
	void update() {

		reset();
		updateMovementVector();
		updateLookatVector();
	}       

	
	Vec3f m_move_vector;
	Vec3f m_lookat_vector;
	CardinalDirection m_lookat_direction;
	int32_t m_action;

};


