#pragma once

#include <game/World.hpp>

namespace CollisionManager {

	Vec3f computeCorrectionVector(World::Ptr world,
				      const Vec3f & body_pos,
				      GeometryUtils::Polygon player,
				      const Vec3f & move_vec);
};
