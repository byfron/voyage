#pragma once

#include <entities/System.hpp>
#include <game/World.hpp>
#include "GraphicsCmp.hpp"
#include "InputCmp.hpp"
#include "BulletCmp.hpp"
#include "BodyCmp.hpp"
#include "CollisionComponent.hpp"
#include <pumpkin.hpp>
#include <voyage.pb.h>

struct Quad {
	Vec2f vertex[4];
	int num_vertices = 4;
};

/* 
UPDATE:
- Get input, send to server, and perform prediction. 
- Check if server data available.
- Sync current pos with server data
*/

class EntityInputSystem : public System<EntityInputSystem>, public Receiver<voyage::sc_entityUpdate> {
public:

	EntityInputSystem(World::Ptr world) : m_world(world) {

	}

	
	// Receives an entity update message from server
	void receive(const voyage::sc_entityUpdate &entity_update) {

		std::cout << "Received entity update!" << std::endl;//entity_update.entityId() << std::endl;
		std::cout << "pos:" << entity_update.x() << "," << entity_update.y() << std::endl;

		// keep a map of received states from server
		
	}
	
	void update(EntityManager & em, EventManager &evm,/* ClientNetworkManager &net,*/ float delta ) {

		GameMap::CollisionMask mask = m_world->getMapCollisionMask();
		GameMap::Ptr game_map = m_world->getGameMap();

		// Update all player states

		// Player entity in server will not have a InputComponent
		// and this will never be run
		em.each<InputCmp, CollisionComponent, BodyCmp>
			([&em, delta, game_map, mask](Entity entity,
						      InputCmp &input,
						      CollisionComponent &col,
						      BodyCmp &body) {


			// Updates inputs
			input.update();

			// Add client input to a action queue
			
			// Inmediatelly send :
			// - move vector
			// - rotation angle
			// - action


			// Check if there's server updates on this entity
			// If so, get data and accumulate on top of input buffer
			// to compute new state


//			computeServerPosition(body, input_buffer);
//			body.m_auth_position = server_position;

			// ignore the angle by now
//			body.m_auth_rotAngle = server_rotAngle;			

			
			// reset input buffer
			
			// TODO send difference!

			//Refactor collisions out of here
			
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


			body.m_rotAngle = pumpkin::InputManager::m_mouse_angle
				+ M_PI/4;
			

			// Make character face the camera
			// Eigen::MatrixXf rot_cam = pumpkin::GraphicsEngine::camera().
			// 	getTowardsCameraRotation(body.m_position);

			//	body.m_rotation = rot;// * rot_cam;

//			body.m_action_id = input.m_action;

		// 	if (input.m_action & (1 << (int)Action::SHOOTING)) {

// 				// //TODO: this should be done acoordaing to the weapon!
// 				// //Create a bullet
// 				Vec3f motion_vec = input.m_lookat_vector;
// 				Vec3f hand_pos = Vec3f(body.m_position(0), body.m_position(1), -0.5);
// 				Entity bullet = em.create();
// 				float speed = 10.0;

// 				//rotate the bullet to face lookatvec
// 				Eigen::AngleAxis<float> aa(pumpkin::InputManager::m_mouse_angle
// 							   + M_PI/4, Vec3f(0.0,0.0,1.0));

// 				Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
// 				rot.block(0,0,3,3) = aa.matrix();

// 				em.assign<BodyCmp>(bullet.id(),
// 						   speed,
// 						   motion_vec*speed*delta,
// 						   hand_pos,
// 						   rot);

// 				em.assign<GraphicsCmp>(bullet.id());
// 				em.assign<CollisionComponent>(bullet.id());
								
// //				em.assign<BulletCmp>(bullet.id());

// 			}

		   });

		// Update rest of entities (NetworkComponent) 
		// (in server it will be all of them)
	}

private:

	World::Ptr m_world;
};
