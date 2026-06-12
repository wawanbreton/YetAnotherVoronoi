// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Segment2.h"

#include <boost/geometry/algorithms/centroid.hpp>


namespace yav
{

Segment2 rotate90(const Segment2& segment, const bool ccw)
{
    Point2 midpoint;
    boost::geometry::centroid(segment, midpoint);

    const Point2& start = segment.first;
    const Point2& end = segment.second;
    const Point2 half_segment = (end - start) / 2.0;
    const Point2 half_segment_rotated(-half_segment.get<1>(), half_segment.get<0>());

    Segment2 result(midpoint - half_segment_rotated, midpoint + half_segment_rotated);
    if (! ccw)
    {
        result = -result;
    }

    return result;
}

} // namespace yav

yav::Segment2 operator-(const yav::Segment2& segment)
{
    return yav::Segment2(segment.second, segment.first);
}
