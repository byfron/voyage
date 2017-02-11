#pragma once

#include <entities/System.hpp>
#include <game/World.hpp>
#include <common/math.hpp>
#include "BodyCmp.hpp"
#include <utils/GeometryUtils.hpp>

struct CollisionEvent {
	Entity left;
	Entity right;
};

class CollisionComponent {

public:

	void update(const GeometryUtils::AABB & aabb, const Eigen::MatrixXf & transform) {
		Vec4f vmin = transform * VEC_TO_HOMOG(aabb.m_min);
		Vec4f vmax = transform * VEC_TO_HOMOG(aabb.m_max);
		m_geometry.m_min = VEC_FROM_HOMOG(vmin);
		m_geometry.m_max = VEC_FROM_HOMOG(vmax);
	}
	
	CollisionComponent() {};
	
	GeometryUtils::AABB m_geometry;
};

// NOTE: We distinguish between characters and body
// Characters have a position, tile, action state a direction of movemenet,
// Objects have a position, tile, direction mov. 
// FUCK could be the same?

class CollisionSystem : public System<CollisionSystem> {

public:

// 	struct CollisionCandidate {
// 		Vec2i m_pos;
// 		float m_radius;
// 		uint32_t m_entity_id;
// 	};

// 	// get collision mask from tilemap
// 	// alternative option. make each wall also a CollisionComponent/WallComponent
 	CollisionSystem(World::Ptr world) : m_world(world) {}

// 	void init(TileMap::CollisionMask * cm) {
// 		collision_mask = cm;
// 	}

 	void update(EntityManager & em, EventManager & evm, float delta) {

// 		// copy current state of collision mask
		GameMap::CollisionMask mask = m_world->getMapCollisionMask();
		GameMap::Ptr game_map = m_world->getGameMap();


// 		// solve candidates with characters
 		em.each<CollisionComponent, BodyCmp >
			([delta, game_map, mask](Entity entity,
						 CollisionComponent & collision,
						 BodyCmp & body) {


		      // Update collision geometry
		      collision.update(body.m_aabb, body.getTransform());

		      Vec2i tile = game_map->getTileCoords(body.m_position);
		      
		      // Check if entity collides with map
		      if (not mask.isWalkable(tile)) {

			      // send message
//			      evm.emit<CollideEvent>(entity, other_entity);
			       
			      std::cout << "isColliding!!!" << std::endl;
 		  }		  		      

	      });
	}
							      
// 		  // check if entity is colliding with a map object
// 		  // we shall do this here ONLY if walls are objects I guess
// 		  if (collision_grid.isColliding(body.m_tile_position.x,
// 						 body.m_tile_position.y)) {

// 			  std::cout << "isColliding!!!" << std::endl;
// //			  body.stepBack();
// 		  }
		  
// //		  = entity.getId();

// 		  // add colliding tiles acoording to radius
// 		  // std::vector<Vec2i> tiles =
// 		  // 	  TileMapUtils::getTilesWithinRadius(body.m_tile_position.x,
// 		  // 					     body.m_tile_position.y,
// 		  // 					     collision.radius);
// 		  // for (auto tile_pos : tiles) {
// 		  // 	  if (collision_grid.isUsed(tile_pos(0), tile_pos(1))) {

// 		  // 	  }
// 		  // 	  collision_candidates.push_back(
// 		  // 		  Candidate {} );
// 		  // }

// 		  });

// 		// solve candidates with other objects
// 		em.each<CollisionComponent, ObjectCmp >([delta](Entity entity,


// 		// send collision messages

// 	  }

private:

	World::Ptr m_world;

// 	TileMap::CollisionMask * m_collision_mask;
};
