// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/Slab.h"


namespace yav::generator::region
{

Slab::Slab(const geometry::Point3& normal, const double minimum_projection, const double maximum_projection)
    : normal_(normal)
    , minimum_projection_(minimum_projection)
    , maximum_projection_(maximum_projection)
{
}

} // namespace yav::generator::region
