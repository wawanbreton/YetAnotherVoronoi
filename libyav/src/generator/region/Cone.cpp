// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/Cone.h"

#include "yav/geometry/Point3Operations.h"

#include <algorithm>
#include <cmath>

namespace yav::generator::region
{

Cone::Cone(const geometry::Point3& apex, const geometry::Point3& axis, const double aperture_radians)
    : apex_(apex)
    , axis_(axis)
    , aperture_radians_(aperture_radians)
{
}

bool Cone::contains(const geometry::Point3& position) const
{
    const geometry::Point3 direction_vector = geometry::Point3Operations::subtract(position, apex_);
    const double direction_norm = geometry::Point3Operations::norm(direction_vector);
    const double axis_norm = geometry::Point3Operations::norm(axis_);

    if (direction_norm == 0.0 || axis_norm == 0.0)
    {
        return true;
    }

    const double cosine_value = geometry::Point3Operations::dotProduct(direction_vector, axis_) / (direction_norm * axis_norm);
    const double clamped_cosine_value = std::clamp(cosine_value, -1.0, 1.0);
    const double angle = std::acos(clamped_cosine_value);

    return angle <= aperture_radians_;
}

std::string Cone::regionName() const
{
    return "Cone";
}

} // namespace yav::generator::region
