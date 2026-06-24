// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Vertex2.h"

#include <boost/geometry/algorithms/distance.hpp>


namespace bg = boost::geometry;

namespace yav
{

Vertex2::Vertex2(const Point2& position)
    : position_(position)
{
}

const Point2& Vertex2::position() const
{
    return position_;
}

double Vertex2::distanceTo(const Point2& point) const
{
    return bg::distance(position_, point);
}

std::vector<Point2> Vertex2::definingPoints() const
{
    return { position_ };
}

bool Vertex2::intersects(const Box2& box) const
{
    return bg::intersects(box, position_);
}

Point2 Vertex2::centroid() const
{
    return position_;
}

} // namespace yav
