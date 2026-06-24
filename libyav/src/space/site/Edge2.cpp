// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Edge2.h"

#include <boost/geometry/algorithms/distance.hpp>


namespace bg = boost::geometry;

namespace yav
{

Edge2::Edge2(const Segment2& segment)
    : segment_(segment)
{
}

const Segment2& Edge2::segment() const
{
    return segment_;
}

double Edge2::distanceTo(const Point2& point) const
{
    return bg::distance(point, segment_);
}

std::vector<Point2> Edge2::definingPoints() const
{
    return { segment_.first, segment_.second };
}

bool Edge2::intersects(const Box2& box) const
{
    return bg::intersects(box, segment_);
}

Point2 Edge2::centroid() const
{
    return bg::return_centroid<Point2>(segment_);
}

} // namespace yav
