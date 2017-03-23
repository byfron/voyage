#pragma once
#include <visibility/Vector.hpp>
#include <entities/System.hpp>
#include <game/World.hpp>
#include "BodyCmp.hpp"

class VisibilityCmp {

public:
	VisibilityCmp() {
	}

	float visibility_angle;
	std::vector<Vec2f> visi_polygon;

};

// In the server, all the players will run the visibility.
// In the client only the main player will
class VisibilitySystem : public System<VisibilitySystem> {

public:
	VisibilitySystem(World::Ptr world) : m_world(world) {}

	void update(EntityManager & em, EventManager &evm, float delta) {

		em.each<BodyCmp, VisibilityCmp>([delta, this](Entity entity,
												BodyCmp & body,
												VisibilityCmp &vis) {

			// Run visibility
			std::vector<visibility::LineSegment> collision_polygons =
				m_world->getGameLevel().getCollisionLineSegments();

			visibility::Vector<float,2> pos2 =
				visibility::Vector<float,2>(Vec2f(body.m_position[0], body.m_position[1]));
			std::vector<visibility::vec2> visi_polygon =
				visibility::VisibilityPolygon(pos2, collision_polygons);

			vis.visi_polygon.clear();
			for (auto v : visi_polygon) {
				std::cout << v << std::endl;
				vis.visi_polygon.push_back(Vec2f(v.data[0], v.data[1]));
			}
		});
	}

	World::Ptr m_world;

};
