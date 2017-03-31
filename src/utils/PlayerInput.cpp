#include "PlayerInput.hpp"
#include <components/PlayerSystem.hpp>

namespace PlayerInput {

	UserAction getUserAction() {

		Vec3f motion_vec = Vec3f(0.0f, 0.0f, 0.0f);
		Vec3f lookat_vec = Vec3f(0.0f, 0.0f, 0.0f);
		CardinalDirection lookat_dir;
		int32_t action_code = 0;

		getMovementVector(motion_vec, action_code);
		getLookatVector(lookat_vec, lookat_dir, action_code);

		UserAction action;
		action.motion_vec = motion_vec;
		action.angle = pumpkin::InputManager::m_mouse_angle;
		action.action_code = action_code;
		return action;
	}

	void getActionCode(int32_t & action) {
		if (pumpkin::InputManager::isLeftMouseButtonJustPressed()) {
			action |= (1 << (int)Action::SHOOTING);
		}

		if (pumpkin::InputManager::isRightMouseButtonPressed()) {
			action |= (1 << (int)Action::FOCUSING);
		}
	}

	void getMovementVector(Vec3f & direction, int32_t & action) {

		//TODO: make input manager more generic to allow for pads/key remaps

		if (pumpkin::InputManager::m_keys & KEY_MOVE_UP) {
			direction += pumpkin::GraphicsEngine::camera().getForwardDir();

			if (pumpkin::InputManager::m_keys & KEY_MOVE_RIGHT) {
				direction += -pumpkin::GraphicsEngine::camera().getRightDir();
				action |= (1 << (int)Action::FACING_UP_RIGHT);
			}
			else if (pumpkin::InputManager::m_keys & KEY_MOVE_LEFT) {
				direction += pumpkin::GraphicsEngine::camera().getRightDir();
				action |= (1 << (int)Action::FACING_UP_LEFT);
			}
			else {
				action |= (1 << (int)Action::FACING_UP);
			}
		}
		else if (pumpkin::InputManager::m_keys & KEY_MOVE_DOWN) {
			direction += -pumpkin::GraphicsEngine::camera().getForwardDir();
			if (pumpkin::InputManager::m_keys & KEY_MOVE_RIGHT) {
				direction += -pumpkin::GraphicsEngine::camera().getRightDir();
				action |= (1 << (int)Action::FACING_DOWN_RIGHT);
			}
			else if (pumpkin::InputManager::m_keys & KEY_MOVE_LEFT) {
				direction += pumpkin::GraphicsEngine::camera().getRightDir();
				action |= (1 << (int)Action::FACING_DOWN_LEFT);
			}
			else {
				action |= (1 << (int)Action::FACING_DOWN);
			}
		}
		else if (pumpkin::InputManager::m_keys & KEY_MOVE_RIGHT) {
			direction += -pumpkin::GraphicsEngine::camera().getRightDir();
			action |= (1 << (int)Action::FACING_RIGHT);
		}
		else if (pumpkin::InputManager::m_keys & KEY_MOVE_LEFT) {
			direction += pumpkin::GraphicsEngine::camera().getRightDir();
			action |= (1 << (int)Action::FACING_LEFT);
		}

		if (direction.norm() > 0) {
			action |= (1 << (int)Action::RUNNING);
		}
	}

	void getLookatVector(Vec3f & lookat_vector,
			     CardinalDirection & lookat_direction,
			     int32_t & action) {

		if (pumpkin::InputManager::isLeftMouseButtonPressed())
		{
			MultiInterval<float> mint(Interval<float>(-M_PI, M_PI), 4);

			int idx = mint.indexContains(pumpkin::InputManager::m_mouse_angle);
			lookat_direction = CardinalDirection(idx);

			switch(lookat_direction) {
			case CardinalDirection::East:
				action |= (1 << (int)Action::FACING_RIGHT);
				break;
			case CardinalDirection::West:
				action |= (1 << (int)Action::FACING_LEFT);
				break;
			case CardinalDirection::North:
				action |= (1 << (int)Action::FACING_UP);
				break;
			case CardinalDirection::South:
				action |= (1 << (int)Action::FACING_DOWN);
				break;
			}
			// Rotations are expressed wrt North-west
			static Vec3f zero_angle_vec = Vec3f(-1.0,1.0,0.0);
			zero_angle_vec.normalize();
			Eigen::AngleAxis<float> aa(pumpkin::InputManager::m_mouse_angle,
						   Vec3f(0.0,0.0,1.0));
			lookat_vector = aa.matrix() * zero_angle_vec;
		}

		if (pumpkin::InputManager::isLeftMouseButtonJustPressed()) {
			action |= (1 << (int)Action::SHOOTING);
		}
	}
}
