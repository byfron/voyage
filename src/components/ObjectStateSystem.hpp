#pragma once

#include <entities/System.hpp>
#include <game/World.hpp>
#include "InputCmp.hpp"
#include "BulletCmp.hpp"
#include "BodyCmp.hpp"
#include <pumpkin.hpp>

class World;

class ObjectStateSystem : public System<ObjectStateSystem> {
public:
	ObjectStateSystem(World::Ptr world) : m_world(world) {

	}

	void update(EntityManager & em, EventManager &evm, float delta ) {
	       
		GameMap::CollisionMask mask = m_world->getMapCollisionMask();
		GameMap::Ptr game_map = m_world->getGameMap();
		
		// Update all player states
		em.each<InputCmp, BodyCmp>([&em, delta, game_map, mask](Entity entity,
								       InputCmp &input,
								       BodyCmp &body) {

			// Updates inputs
			input.update();

			// Check collision
			// in case of collision with map, project motion vector in tile
		        Vec3f pos = body.m_position + input.m_move_vector *
				body.m_moveSpeed * delta;

			Vec2i tile = game_map->getTileCoords(pos);
			if (mask.isColliding(tile)) {
				//project vector in

				pumpkin::GraphicsEngine::camera().setSpeed(0.0);
//				std::vector<Vec3f> edge = TileMapUtils::getClosestEdge(game_map, tile);
//				TileMapUtils::getClosestEdgeVector(corners, 
								
			}else {
				
				pumpkin::GraphicsEngine::camera().setSpeed(body.m_moveSpeed);
				// Updates character state
				body.m_position += input.m_move_vector *
					body.m_moveSpeed * delta;
			}


			//body.m_tile_pos = TileMapUtils::computeTilePos(body.m_position);
			body.m_action_id = input.m_action;

			if (body.m_action_id & (1 << (int)Action::SHOOTING)) {
				
				//Create a bullet
				Entity bullet = em.create();
				em.assign<GraphicsCmp>(bullet.id(), Configuration<>());
				em.assign<BodyCmp>(bullet.id());
				em.assign<BulletCmp>(bullet.id());
				
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
