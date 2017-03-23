#pragma once

#include <common/math.hpp>
#include <utils/FbxLoader.hpp>
#include <visibility/Visibility.hpp>

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

		std::vector<visibility::LineSegment> toLineSegmentList() {

			std::vector<visibility::LineSegment> segments;

			assert(num_vertices() > 1 && "Number of polygon vertices too low to get a line");

			for (int i = 0; i < num_vertices()-1; i++) {
				segments.push_back(visibility::LineSegment(vertices[i], vertices[i+1]));
			}
			segments.push_back(visibility::LineSegment(
								   vertices[num_vertices()-1], vertices[0]));
			return segments;
		}
	};

	Polygon convertToPolygon(FbxLoader::FPolygon p);

	bool isWithin(const Vec3f & point, std::vector<Polygon> A);

	bool IntersectMTD(GeometryUtils::Polygon A, GeometryUtils::Polygon B, Vec2f& MTD);

	bool linePlaneIntersection(Vec3f ray, Vec3f rayOrigin, Vec3f normal,
				   Vec3f point_in_plane, Vec3f & contact);

}
