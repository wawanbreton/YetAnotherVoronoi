// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/generator/region/AbstractVoronoiRegion.h"
#include "yav/geometry/Point3.h"

namespace yav::generator::region
{

/** Cone region centered on a vertex site, parameterized by aperture in radians. */
class Cone final : public AbstractVoronoiRegion
{
public:
    Cone(const geometry::Point3& apex, const geometry::Point3& axis, double aperture_radians);

private:
    geometry::Point3 apex_;
    geometry::Point3 axis_;
    double aperture_radians_;
};

} // namespace yav::generator::region
