#include "Visibility.hpp"
#include <iostream>

namespace visibility {

std::vector<vec2> VisibilityPolygon(vec2 point, const std::vector<LineSegment>& segments)
{
	LineSegmentDistComparator cmp_dist{ point };
	std::set<LineSegment, LineSegmentDistComparator> state{ cmp_dist };
	std::vector<VisibilityEvent> events;
	events.reserve(segments.size() * 2 + 1);

	for (auto&& segment : segments)
	{
		// Sort line segment endpoints and add them as events
		// Skip line segments collinear with the point
		if (orientation(point, segment.a, segment.b) == Orientation::Collinear)
		{
			continue;
		}
		else if (orientation(point, segment.a, segment.b) == Orientation::RightTrun)
		{
			events.emplace_back(VisibilityEventType::StartVertex, segment);
			events.emplace_back(VisibilityEventType::EndVertex, LineSegment{ segment.b, segment.a });
		}
		else
		{
			events.emplace_back(VisibilityEventType::StartVertex, LineSegment{ segment.b, segment.a });
			events.emplace_back(VisibilityEventType::EndVertex, segment);
		}

		// Initialize state by adding line segments that are intersected by vertical ray from the point
		auto a = segment.a, b = segment.b;
		if (a[0] > b[0]) std::swap(a, b);

		auto abp = orientation(a, b, point);
		if ((approx_equal(b[0], point[0]) || (a[0] < point[0] && point[0] < b[0])) && abp == Orientation::RightTrun)
		{
			state.insert(segment);
		}
	}

	std::cout <<"udt"<<std::endl;

	// sort events by angle
	AngleComparator cmp_angle{ point };
	std::sort(events.begin(), events.end(), [&cmp_angle](const VisibilityEvent& a, const VisibilityEvent& b) {
            return approx_equal(a.point(), b.point()) ?
                a.type == VisibilityEventType::EndVertex && b.type == VisibilityEventType::StartVertex : // if the points are equal, sort end vertices first
                cmp_angle(a.point(), b.point());
        });

	// find the visibility polygon
	std::vector<vec2> vertices;
	for (auto&& event : events)
	{
		if (event.type == VisibilityEventType::EndVertex) state.erase(event.segment);

		if (state.empty())
		{
			vertices.push_back(event.point());
		}
		else if (cmp_dist(event.segment, *state.begin()))
		{
			vec2 intersection;
			auto result = Ray{ point, event.point() - point }.Intersects(*state.begin(), intersection);
//			assert(result && "Ray intersects a line segment iff the line segment is in the state");

			if (event.type == VisibilityEventType::StartVertex)
			{
				vertices.push_back(intersection);
				vertices.push_back(event.point());
			}
			else
			{
				vertices.push_back(event.point());
				vertices.push_back(intersection);
			}
		}

		if (event.type == VisibilityEventType::StartVertex) state.insert(event.segment);
	}


	// remove collinear points
	auto top = vertices.begin();
	for (auto it = vertices.begin(); it != vertices.end(); ++it)
	{
		auto prev = top == vertices.begin() ? vertices.end() - 1 : top - 1;
		auto next = it + 1 == vertices.end() ? vertices.begin() : it + 1;
		if (orientation(*prev, *it, *next) != Orientation::Collinear) *top++ = *it;
	}
	vertices.erase(top, vertices.end());

	return vertices;
}

}
