#include "CollisionComponent.hpp"

void CollisionSystem::update(EntityManager & em, EventManager & evm, float delta) {

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
