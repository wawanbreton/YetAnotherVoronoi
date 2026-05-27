// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/Cone.h"


namespace yav::generator::region
{

Cone::Cone(const geometry::Point3& apex, const geometry::Point3& axis, const double aperture_radians)
    : apex_(apex)
    , axis_(axis)
    , aperture_radians_(aperture_radians)
{
}

} // namespace yav::generator::region
