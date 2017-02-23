#include "CollisionManager.hpp"

namespace CollisionManager {

	Vec3f computeCorrectionVector(World::Ptr world,
				      const Vec3f & body_pos,
				      GeometryUtils::Polygon player,
				      const Vec3f & move_vec) {

		GameMap::CollisionMask mask = world->getMapCollisionMask();
		GameMap::Ptr game_map = world->getGameMap();
		Vec2i tile = game_map->getTileCoords(body_pos);

		// Potential collision tiles
		std::vector<Vec2i> collision_candidates;
		collision_candidates.push_back(Vec2i(tile(0), tile(1)));
		collision_candidates.push_back(Vec2i(tile(0)+1, tile(1)));
		collision_candidates.push_back(Vec2i(tile(0)+1, tile(1)+1));
		collision_candidates.push_back(Vec2i(tile(0), tile(1)+1));
		collision_candidates.push_back(Vec2i(tile(0)-1, tile(1)));
		collision_candidates.push_back(Vec2i(tile(0)-1, tile(1)-1));
		collision_candidates.push_back(Vec2i(tile(0), tile(1)-1));
		collision_candidates.push_back(Vec2i(tile(0)+1, tile(1)-1));
		collision_candidates.push_back(Vec2i(tile(0)-1, tile(1)+1));


		// Check that all points lie in walkable tiles
		Vec2f accum_mtd = Vec2f::Zero();

		for (auto candidate: collision_candidates) {

			//Debug
			//game_map->getTileMap().highlightTile(candidate);

			GeometryUtils::Polygon cand;
			Vec2i tilec = candidate;
			cand.vertex[0] = Vec2f(tilec(0)*0.5, tilec(1)*0.5);
			cand.vertex[1] = Vec2f(tilec(0)*0.5, (tilec(1)+1)*0.5);
			cand.vertex[2] = Vec2f((tilec(0)+1)*0.5, (tilec(1)+1)*0.5);
			cand.vertex[3] = Vec2f((tilec(0)+1)*0.5, tilec(1)*0.5);

			if (not mask.isWalkable(candidate)) {
				Vec2f MTD = Vec2f::Zero();
				if (GeometryUtils::IntersectMTD(player, cand, MTD)) {
					accum_mtd += MTD;
					player.vertex[0] -= MTD;
					player.vertex[1] -= MTD;
					player.vertex[2] -= MTD;
					player.vertex[3] -= MTD;
				}
			}
		}

		return Vec3f(accum_mtd(0), accum_mtd(1), 0.0f);
	}
	
};
