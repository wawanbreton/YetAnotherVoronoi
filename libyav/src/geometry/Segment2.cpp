// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Segment2.h"

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/distance.hpp>
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

std::optional<Segment2> intersectSegmentParts(const Segment2& segment, const Segment2& part1, const Segment2& part2)
{
    const Point2 vector = segment.second - segment.first;
    const double length_squared = bg::dot_product(vector, vector);

    std::pair<double, double> projected_part1;
    std::pair<double, double> projected_part2;

    projected_part1.first = bg::dot_product(part1.first - segment.first, vector) / length_squared;
    projected_part1.second = bg::dot_product(part1.second - segment.first, vector) / length_squared;
    projected_part2.first = bg::dot_product(part2.first - segment.first, vector) / length_squared;
    projected_part2.second = bg::dot_product(part2.second - segment.first, vector) / length_squared;

    std::tie(projected_part1.first, projected_part1.second) = std::minmax({ projected_part1.first, projected_part1.second });
    std::tie(projected_part2.first, projected_part2.second) = std::minmax({ projected_part2.first, projected_part2.second });

    if (projected_part1.first >= projected_part2.second || projected_part1.second <= projected_part2.first)
    {
        return std::nullopt;
    }

    const std::pair<double, double> projected_intersection
        = { std::max(projected_part1.first, projected_part2.first), std::min(projected_part1.second, projected_part2.second) };

    return Segment2(segment.first + vector * projected_intersection.first, segment.first + vector * projected_intersection.second);
}

} // namespace yav

yav::Segment2 operator-(const yav::Segment2& segment)
{
    return yav::Segment2(segment.second, segment.first);
}
