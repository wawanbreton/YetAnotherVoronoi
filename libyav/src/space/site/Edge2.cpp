// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Edge2.h"

#include <boost/geometry/algorithms/distance.hpp>


namespace yav::space::site
{

Edge2::Edge2(const geometry::Segment2& segment)
    : segment_(segment)
{
}

double Edge2::distanceTo(const geometry::Point2& point) const
{
    return boost::geometry::distance(point, segment_);
}

geometry::Point2 Edge2::basePoint() const
{
    return segment_.first;
}

std::vector<geometry::Point2> Edge2::definingPoints() const
{
    return { segment_.first, segment_.second };
}

} // namespace yav::space::site
