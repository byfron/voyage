#pragma once

#include "System.hpp"

class BodyComponent {

public:
	Vec3f m_position;
	Vec3f m_move_vector;
	Vec2i m_tile_position;
};

class BodySystem : public System<BodySystem> {

public:

	BodySystem() {
		m_map_size_x = 10;
		m_map_size_y = 10;
	}

	void update(EntityManager & em, EventManager & evm, float delta ) {

		em.each<BodyComponent>([delta](Entity entity, BodyComponent &body) {

				body.m_position += body.m_move_vector * delta;
				body.m_tile_position = computeTilePos(body.m_position);
 	        });
	}

	Vec2i computeTilePos(const Vec3f & pos_3D) {
		return Vec2i(pos_3D(x)/m_map_size_x, pos_3D(y)/m_map_size_y);
	}

private:

	int m_map_size_x;
	int m_map_size_y;
};
