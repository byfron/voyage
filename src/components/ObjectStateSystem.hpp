#pragma once

#include <entities/System.hpp>
#include <game/World.hpp>
#include "GraphicsCmp.hpp"
#include "InputCmp.hpp"
#include "BulletCmp.hpp"
#include "BodyCmp.hpp"
#include "CollisionComponent.hpp"
#include <pumpkin.hpp>

struct Quad {
	Vec2f vertex[4];
	int num_vertices = 4;
};


class ObjectStateSystem : public System<ObjectStateSystem> {
public:
	ObjectStateSystem(World::Ptr world) : m_world(world) {

	}

	void update(EntityManager & em, EventManager &evm, float delta ) {

		GameMap::CollisionMask mask = m_world->getMapCollisionMask();
		GameMap::Ptr game_map = m_world->getGameMap();

		// Update all player states
		em.each<InputCmp, CollisionComponent, BodyCmp>
			([&em, delta, game_map, mask](Entity entity,
						      InputCmp &input,
						      CollisionComponent &col,
						      BodyCmp &body) {

			// Updates inputs
			input.update();

			// Check collision
			Vec3f move_vec = input.m_move_vector *
				body.m_moveSpeed * delta;
		        Vec3f pos = body.m_position + move_vec;

			Vec2i tile = game_map->getTileCoords(body.m_position);

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

			GeometryUtils::Polygon player = body.getPolygon();

			// Check that all points lie in walkable tiles
			Vec2f accum_mtd = Vec2f::Zero();

			for (auto candidate: collision_candidates) {

				//TODO move to debug
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

			// Move player
			move_vec = input.m_move_vector *
				body.m_moveSpeed * delta - Vec3f(accum_mtd(0), accum_mtd(1), 0.0f);


			body.m_moveVec = move_vec;

			// Move camera
			pumpkin::GraphicsEngine::camera().moveAlong(move_vec);

			// Make character face the camera
			Eigen::MatrixXf rot = pumpkin::GraphicsEngine::camera().
				getTowardsCameraRotation(body.m_position);

			body.m_rotation = rot;

			body.m_action_id = input.m_action;

			if (body.m_action_id & (1 << (int)Action::SHOOTING)) {

				// //TODO: this should be done acoordaing to the weapon!
				// //Create a bullet
				Vec3f motion_vec = input.m_lookat_vector;
				Vec3f hand_pos = Vec3f(body.m_position(0), body.m_position(1), -0.5);
				Entity bullet = em.create();
				float speed = 10.0;

				//rotate the bullet to face lookatvec
				Eigen::AngleAxis<float> aa(pumpkin::InputManager::m_mouse_angle
							   + M_PI/4, Vec3f(0.0,0.0,1.0));

				Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
				rot.block(0,0,3,3) = aa.matrix();

				em.assign<BodyCmp>(bullet.id(),
						   speed,
						   motion_vec*speed*delta,
						   hand_pos,
						   rot);

				em.assign<GraphicsCmp>(bullet.id());
//				em.assign<BulletCmp>(bullet.id());

			}

		   });

		// Update networking players


		// Update networking objects



	}

	// In server:
	// void update(EntityManager & em, EventManager &evm, float delta ) {
	// 	em.each<AICmp, CharacterCmp>([delta](Entity entity, PlayerState &state) {

	// }

private:

	World::Ptr m_world;
};
