// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"
#include "yav/generator/region/AbstractVoronoiRegion.h"

namespace yav::generator::region
{

/** Half-space restriction used as an edge-site region approximation. */
class HalfPlaneSlice final : public AbstractVoronoiRegion
{
public:
    HalfPlaneSlice(const geometry::Point3& normal, double offset);

    bool contains(const geometry::Point3& position) const override;
    std::string regionName() const override;

private:
    geometry::Point3 normal_;
    double offset_;
};

} // namespace yav::generator::region
