// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/Slab.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>

namespace yav::generator::region
{

Slab::Slab(const geometry::Point3& normal, const double minimum_projection, const double maximum_projection)
    : normal_(normal)
    , minimum_projection_(minimum_projection)
    , maximum_projection_(maximum_projection)
{
}

bool Slab::contains(const geometry::Point3& position) const
{
    const double projection = boost::geometry::dot_product(normal_, position);
    return projection >= minimum_projection_ && projection <= maximum_projection_;
}

std::string Slab::regionName() const
{
    return "Slab";
}

} // namespace yav::generator::region
