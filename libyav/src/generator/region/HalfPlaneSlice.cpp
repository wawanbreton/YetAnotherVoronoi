// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/HalfPlaneSlice.h"

#include "yav/geometry/Point3Operations.h"

namespace yav::generator::region
{

HalfPlaneSlice::HalfPlaneSlice(const geometry::Point3& normal, const double offset)
    : normal_(normal)
    , offset_(offset)
{
}

bool HalfPlaneSlice::contains(const geometry::Point3& position) const
{
    return geometry::Point3Operations::dotProduct(normal_, position) + offset_ >= 0.0;
}

std::string HalfPlaneSlice::regionName() const
{
    return "HalfPlaneSlice";
}

} // namespace yav::generator::region
