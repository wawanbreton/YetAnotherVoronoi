// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/generator/region/AbstractVoronoiRegion.h"
#include "yav/geometry/Point3.h"

namespace yav::generator::region
{

/** Slab region bounded by two parallel planes, used as a face-site local domain. */
class Slab final : public AbstractVoronoiRegion
{
public:
    Slab(const geometry::Point3& normal, double minimum_projection, double maximum_projection);

private:
    geometry::Point3 normal_;
    double minimum_projection_;
    double maximum_projection_;
};

} // namespace yav::generator::region
