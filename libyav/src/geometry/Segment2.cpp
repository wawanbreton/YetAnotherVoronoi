// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Segment2.h"

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>


namespace bg = boost::geometry;

namespace yav
{

Segment2 rotate90(const Segment2& segment, const bool ccw)
{
    Point2 midpoint;
    bg::centroid(segment, midpoint);

    const Point2& start = segment.first;
    const Point2& end = segment.second;
    const Point2 half_segment = (end - start) / 2.0;
    const Point2 half_segment_rotated(-half_segment.y(), half_segment.x());

    Segment2 result(midpoint - half_segment_rotated, midpoint + half_segment_rotated);
    if (! ccw)
    {
        result = -result;
    }

    return result;
}

PointPositionOnSegment projectedPointsLiesOnSegment(const Segment2& segment, const Point2& point)
{
    const Point2 vector = segment.second - segment.first;

    const double projection = bg::dot_product(point - segment.first, vector);
    const double length_squared = bg::dot_product(vector, vector);

    if (projection < 0)
    {
        return PointPositionOnSegment::Before;
    }
    if (projection > length_squared)
    {
        return PointPositionOnSegment::After;
    }
    return PointPositionOnSegment::Inside;
}

} // namespace yav

yav::Segment2 operator-(const yav::Segment2& segment)
{
    return yav::Segment2(segment.second, segment.first);
}
