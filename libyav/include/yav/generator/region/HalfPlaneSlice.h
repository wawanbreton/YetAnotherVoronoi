// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/generator/region/AbstractVoronoiRegion.h"
#include "yav/geometry/Point3.h"

namespace yav::generator::region
{

/** Half-space restriction used as an edge-site region approximation. */
class HalfPlaneSlice final : public AbstractVoronoiRegion
{
public:
    HalfPlaneSlice(const geometry::Point3& normal, double offset);

private:
    geometry::Point3 normal_;
    double offset_;
};

} // namespace yav::generator::region
