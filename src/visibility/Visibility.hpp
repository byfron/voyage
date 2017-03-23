#ifndef GEOMETRY_VISIBILITY_HPP_
#define GEOMETRY_VISIBILITY_HPP_

#include <set>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>

#include "Floats.hpp"
#include "Vector.hpp"
#include "Primitives.hpp"

namespace visibility
{
    /* Compare 2 line segments based on their distance from given point
       Assumes: (1) the line segments are intersected by some ray from the origin
                (2) the line segments do not intersect except at their endpoints
                (3) no line segment is collinear with the origin
    */
    struct LineSegmentDistComparator
    {
        vec2 origin;

        LineSegmentDistComparator(vec2 origin) : origin(origin) {}

        bool operator()(const LineSegment& x, const LineSegment& y) const
        {
            auto a = x.a, b = x.b;
            auto c = y.a, d = y.b;

            assert(orientation(origin, a, b) != Orientation::Collinear && "AB must not be collinear with the origin.");
            assert(orientation(origin, c, d) != Orientation::Collinear && "CD must not be collinear with the origin.");

            // sort the endpoints so that if there are common endpoints, it will be a and c
            if (approx_equal(b, c) || approx_equal(b, d)) std::swap(a, b);
            if (approx_equal(a, d)) std::swap(c, d);

            // cases with common endpoints
            if (approx_equal(a, c))
            {
                if (approx_equal(b, d) || orientation(origin, a, d) != orientation(origin, a, b)) return false;
                return orientation(a, b, d) != orientation(a, b, origin);
            }

            // cases without common endpoints
            auto cda = orientation(c, d, a);
            auto cdb = orientation(c, d, b);
            if (cdb == Orientation::Collinear && cda == Orientation::Collinear)
            {
                return distance_squared(origin, a) < distance_squared(origin, c);
            }
            else if (cda == cdb || cda == Orientation::Collinear || cdb == Orientation::Collinear)
            {
                auto cdo = orientation(c, d, origin);
                return cdo == cda || cdo == cdb;
            }
            else
            {
                return orientation(a, b, origin) != orientation(a, b, c);
            }
        }
    };

    // compare angles clockwise starting at the positive y axis
    struct AngleComparator
    {
        vec2 vertex;

        AngleComparator(vec2 origin) : vertex(origin) {}

        bool operator()(const vec2& a, const vec2& b) const
        {
            auto is_a_left = strictly_less(a[0], vertex[0]);
            auto is_b_left = strictly_less(b[0], vertex[0]);
            if (is_a_left != is_b_left) return is_b_left;

            if (approx_equal(a[0], vertex[0]) && approx_equal(b[0], vertex[0]))
            {
                if (!strictly_less(a[1], vertex[1]) || !strictly_less(b[1], vertex[1]))
                {
                    return strictly_less(b[1], a[1]);
                }
                return strictly_less(a[1], b[1]);
            }

            auto oa = a - vertex;
            auto ob = b - vertex;
            auto det = cross(oa, ob);
            if (approx_equal(det, 0.f))
            {
                return length_squared(oa) < length_squared(ob);
            }
            return det < 0;
        }
    };

    // events used in the visibility polygon algorithm
    enum class VisibilityEventType
    {
        StartVertex,
        EndVertex
    };

    struct VisibilityEvent
    {
        VisibilityEventType type;
        LineSegment segment;

        VisibilityEvent() {}
        VisibilityEvent(VisibilityEventType type, const LineSegment& segment) : type(type), segment(segment) {}

        inline const vec2& point() const { return segment.a; }
    };

    /*! Calculate visibility polygon vertices in clockwise order
        \param position of the observer
        \param list of arbitrary ordered line segments (line segments collinear with the point are ignored)
        \return vector of vertices of the visibility polygon
     */
    std::vector<vec2> VisibilityPolygon(vec2 point, const std::vector<LineSegment>& segments);
}

#endif // GEOMETRY_VISIBILITY_HPP_
