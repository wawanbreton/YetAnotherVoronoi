// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/equisurface/Paraboloid.h"

#include <boost/geometry/core/access.hpp>

namespace yav::voronoi::equisurface
{

Paraboloid::Paraboloid(const geometry::Point3& focus, const double scale, const double offset)
    : focus_(focus)
    , scale_(scale)
    , offset_(offset)
{
}

double Paraboloid::evaluateAt(const geometry::Point3& position) const
{
    const double delta_x = boost::geometry::get<0>(position) - boost::geometry::get<0>(focus_);
    const double delta_y = boost::geometry::get<1>(position) - boost::geometry::get<1>(focus_);
    const double delta_z = boost::geometry::get<2>(position) - boost::geometry::get<2>(focus_);

    return scale_ * (delta_x * delta_x + delta_y * delta_y) - delta_z + offset_;
}

const geometry::Point3& Paraboloid::focus() const
{
    return focus_;
}

double Paraboloid::scale() const
{
    return scale_;
}

double Paraboloid::offset() const
{
    return offset_;
}

} // namespace yav::voronoi::equisurface
