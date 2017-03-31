#pragma once

#include <utils/InputManager.hpp>
#include <graphics/GraphicsEngine.hpp>
#include <utils/Interval.hpp>
#include <common/math.hpp>

struct UserAction;

namespace PlayerInput {


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
			FACING_UP_LEFT,
			FACING_UP_RIGHT,
			FACING_DOWN_LEFT,
			FACING_DOWN_RIGHT,
			SHOOTING,
			FOCUSING
			};

	UserAction getUserAction();
	void getMovementVector(Vec3f & direction, int32_t & action);
	void getLookatVector(Vec3f & lookat_vector,
			     CardinalDirection & lookat_direction,
			     int32_t & action);

}
