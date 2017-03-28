//
// Created by rubio on 27/03/17.
//

#ifndef VOYAGE_VISIBILITYMANAGER_HPP
#define VOYAGE_VISIBILITYMANAGER_HPP

#include <pumpkin.hpp>
#include <game/World.hpp>
#include <visibility/Vector.hpp>

class VisibilityManager {
public:

    std::vector<Vec2f> getPolygon() { return m_polygon; }

	void update(const Vec2f & player_pos, World::Ptr world) {

		std::vector<visibility::LineSegment> collision_polygons =
				world->getGameLevel().getCollisionLineSegments();

			std::vector<visibility::vec2> visi_polygon =
				visibility::VisibilityPolygon(player_pos, collision_polygons);

			m_polygon.clear();
			for (auto v : visi_polygon) {
				m_polygon.push_back(Vec2f(v.data[0], v.data[1]));
			}

	}

	// Remove this when stencil is done
	void render(float delta) {

			// std::vector<Vec2f> triangles;
			// Triangulate::Process(m_polygon, triangles);
			// updateDynamicVertexBuffer(triangles);

			// bgfx::setState(m_craftStencilState)

			// bgfx::setIndexBuffer(m_dibh);
			// bgfx::setVertexBuffer(m_dvbh);

			// bgfx::submit(m_viewId,
			// 			 m_shader->getHandle())

		if (m_polygon.size())
            pumpkin::DebugManager::push_polygon(m_polygon);
	}

protected:

			std::vector<Vec2f> m_polygon;
};


#endif //VOYAGE_VISIBILITYMANAGER_HPP
