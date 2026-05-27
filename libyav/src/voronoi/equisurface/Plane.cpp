// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/equisurface/Plane.h"

#include <boost/geometry/core/access.hpp>

namespace yav::voronoi::equisurface
{
namespace
{

double dotProduct(const geometry::Point3& first_value, const geometry::Point3& second_value)
{
    return boost::geometry::get<0>(first_value) * boost::geometry::get<0>(second_value)
        + boost::geometry::get<1>(first_value) * boost::geometry::get<1>(second_value)
        + boost::geometry::get<2>(first_value) * boost::geometry::get<2>(second_value);
}

} // namespace

Plane::Plane(const geometry::Point3& normal, const double offset)
    : normal_(normal)
    , offset_(offset)
{
}

double Plane::evaluateAt(const geometry::Point3& position) const
{
    return dotProduct(normal_, position) + offset_;
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
