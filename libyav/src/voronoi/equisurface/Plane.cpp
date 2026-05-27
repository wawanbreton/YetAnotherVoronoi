// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/equisurface/Plane.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>

namespace yav::voronoi::equisurface
{

Plane::Plane(const geometry::Point3& normal, const double offset)
    : normal_(normal)
    , offset_(offset)
{
}

double Plane::evaluateAt(const geometry::Point3& position) const
{
    return boost::geometry::dot_product(normal_, position) + offset_;
}

std::string Plane::surfaceName() const
{
    return "Plane";
}

const geometry::Point3& Plane::normal() const
{
    return normal_;
}

double Plane::offset() const
{
    return offset_;
}

} // namespace yav::voronoi::equisurface
