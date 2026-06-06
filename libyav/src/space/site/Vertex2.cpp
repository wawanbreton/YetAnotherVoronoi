// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Vertex2.h"

#include <boost/geometry/algorithms/distance.hpp>


namespace yav::space::site
{

Vertex2::Vertex2(const geometry::Point2& position)
    : position_(position)
{
}

double Vertex2::distanceTo(const geometry::Point2& point) const
{
    return boost::geometry::distance(position_, point);
}

} // namespace yav::space::site
