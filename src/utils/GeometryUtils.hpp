#pragma once

#include <common/math.hpp>
#include <utils/FbxLoader.hpp>

namespace GeometryUtils {	
	
	struct AABB {

		AABB() {};
		AABB(Vec3f min, Vec3f max) : m_min(min), m_max(max) {}

		Vec3f m_min;
		Vec3f m_max;
	};
	
	struct Polygon {
		std::vector<Vec2f> vertices;		
		Vec2f centroid() {
			Vec2f av = Vec2f(0.0, 0.0);		
			for (auto v : vertices) {
				av += v;
			}
			return av/vertices.size();
		}

		// TODO
		bool isConvex() { return true; }
		
		int num_vertices() { return vertices.size(); }
	};

	Polygon convertToPolygon(FbxLoader::FPolygon p);
			
	bool isWithin(const Vec3f & point, std::vector<Polygon> A);
	
	bool IntersectMTD(GeometryUtils::Polygon A, GeometryUtils::Polygon B, Vec2f& MTD);
			
	bool linePlaneIntersection(Vec3f ray, Vec3f rayOrigin, Vec3f normal,
				   Vec3f point_in_plane, Vec3f & contact);
	
}
