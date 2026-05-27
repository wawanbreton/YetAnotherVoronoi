// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/Cone.h"

#include <boost/geometry/access.hpp>

#include <cmath>

namespace yav::generator::region
{
namespace
{

double dotProduct(const geometry::Point3& first_value, const geometry::Point3& second_value)
{
    return boost::geometry::get<0>(first_value) * boost::geometry::get<0>(second_value)
        + boost::geometry::get<1>(first_value) * boost::geometry::get<1>(second_value)
        + boost::geometry::get<2>(first_value) * boost::geometry::get<2>(second_value);
}

double norm(const geometry::Point3& value)
{
    return std::sqrt(dotProduct(value, value));
}

geometry::Point3 difference(const geometry::Point3& first_value, const geometry::Point3& second_value)
{
    geometry::Point3 output;
    boost::geometry::set<0>(output, boost::geometry::get<0>(first_value) - boost::geometry::get<0>(second_value));
    boost::geometry::set<1>(output, boost::geometry::get<1>(first_value) - boost::geometry::get<1>(second_value));
    boost::geometry::set<2>(output, boost::geometry::get<2>(first_value) - boost::geometry::get<2>(second_value));
    return output;
}

} // namespace

Cone::Cone(const geometry::Point3& apex, const geometry::Point3& axis, const double aperture_radians)
    : apex_(apex)
    , axis_(axis)
    , aperture_radians_(aperture_radians)
{
}

bool Cone::contains(const geometry::Point3& position) const
{
    const geometry::Point3 direction_vector = difference(position, apex_);
    const double direction_norm = norm(direction_vector);
    const double axis_norm = norm(axis_);

    if (direction_norm == 0.0 || axis_norm == 0.0)
    {
        return true;
    }

    const double cosine_value = dotProduct(direction_vector, axis_) / (direction_norm * axis_norm);
    const double clamped_cosine_value = std::clamp(cosine_value, -1.0, 1.0);
    const double angle = std::acos(clamped_cosine_value);

    return angle <= aperture_radians_;
}

std::string Cone::regionName() const
{
    return "Cone";
}

} // namespace yav::generator::region
