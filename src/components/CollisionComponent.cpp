#include "CollisionComponent.hpp"

void CollisionSystem::update(EntityManager & em, EventManager & evm, float delta) {

// 		// copy current state of collision mask
	//GameLevel::CollisionMask mask = m_world->getLevelCollisionMask();

// 		// solve candidates with characters
 		em.each<CollisionComponent, BodyCmp >
			([delta](Entity entity,
				 CollisionComponent & collision,
				 BodyCmp & body) {


		      // Update collision geometry
		      collision.update(body.m_aabb, body.getTransform());

		      // Check if entity collides with map
		      //if (not mask.isWalkable(tile))
			  {

			      // send message
//			      evm.emit<CollideEvent>(entity, other_entity);

//			      std::cout << "isColliding!!!" << std::endl;
 		  }

	      });
	}
