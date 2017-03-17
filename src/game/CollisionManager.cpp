#include "CollisionManager.hpp"
#include <iostream>

namespace CollisionManager {

	Vec3f computeCorrectionVector(World::Ptr world,
				      const Vec3f & body_pos,
				      GeometryUtils::Polygon player,
				      const Vec3f & move_vec) {

		GameLevel & level = world->getGameLevel();

		std::vector<GeometryUtils::Polygon> collision_candidates =
			level.getCollisionPolygonsNearby(body_pos);

		// Check that all points lie in walkable tiles
		Vec2f accum_mtd = Vec2f::Zero();

		std::cout << "candidates:" << collision_candidates.size() << std::endl;
			
		for (auto cand: collision_candidates) {

			for (auto v : cand.vertices) {
				std::cout << v << std::endl;
			}
//			if (not mask.isWalkable(candidate))
			{
				Vec2f MTD = Vec2f::Zero();
				if (GeometryUtils::IntersectMTD(player, cand, MTD)) {
					accum_mtd += MTD;
					player.vertices[0] -= MTD;
					player.vertices[1] -= MTD;
					player.vertices[2] -= MTD;
					player.vertices[3] -= MTD;
				}
			}
		}

		return Vec3f(accum_mtd(0), accum_mtd(1), 0.0f);
	}

};
