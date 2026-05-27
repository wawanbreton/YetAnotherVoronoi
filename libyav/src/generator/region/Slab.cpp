// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/Slab.h"

#include <boost/geometry/core/access.hpp>

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

} // namespace

Slab::Slab(const geometry::Point3& normal, const double minimum_projection, const double maximum_projection)
    : normal_(normal)
    , minimum_projection_(minimum_projection)
    , maximum_projection_(maximum_projection)
{
}

bool Slab::contains(const geometry::Point3& position) const
{
    const double projection = dotProduct(normal_, position);
    return projection >= minimum_projection_ && projection <= maximum_projection_;
}

std::string Slab::regionName() const
{
    return "Slab";
}

} // namespace yav::generator::region
