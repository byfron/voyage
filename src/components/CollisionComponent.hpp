#pragma once

class CollisionComponent {

public:
	CollisionComponent() {};
	float radius;
};


class CollisionSystem : public System<CollisionSystem> {


	struct CollisionCandidate {
		Vec2i m_pos;
		float m_radius;
		uint32_t m_entity_id;
	};

	// get collision mask from tilemap
	// alternative option. make each wall also a CollisionComponent/WallComponent
	CollisionSystem() {}

	void init(TileMap::CollisionMask * cm) {
		collision_mask = cm;
	}

	void update(EntityManager & em, EventManager & evm, float delta) {

		// copy current state of collision mask
		TileMap::CollisionMask collision_grid = *m_collision_mask;


		em.each<CollisionComponent, Body>([delta](Entity entity,
							  CollisionComponent & collision,
							  Body & body) {

		  // check if entity is colliding with a map object
		  // we shall do this here ONLY if walls are objects I guess
		  if (collision_grid.isColliding(body.m_tile_position.x,
						 body.m_tile_position.y)) {

			  std::cout << "isColliding!!!" << std::endl;
//			  body.stepBack();
		  }


//			  = entity.getId();

		  // add colliding tiles acoording to radius
		  // std::vector<Vec2i> tiles =
		  // 	  TileMapUtils::getTilesWithinRadius(body.m_tile_position.x,
		  // 					     body.m_tile_position.y,
		  // 					     collision.radius);
		  // for (auto tile_pos : tiles) {
		  // 	  if (collision_grid.isUsed(tile_pos(0), tile_pos(1))) {

		  // 	  }
		  // 	  collision_candidates.push_back(
		  // 		  Candidate {} );
		  // }

		  });

		// send collision messages

	  }

	TileMap::CollisionMask * m_collision_mask;
};
