// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/HalfPlaneSlice.h"

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

HalfPlaneSlice::HalfPlaneSlice(const geometry::Point3& normal, const double offset)
    : normal_(normal)
    , offset_(offset)
{
}

bool HalfPlaneSlice::contains(const geometry::Point3& position) const
{
    return dotProduct(normal_, position) + offset_ >= 0.0;
}

std::string HalfPlaneSlice::regionName() const
{
    return "HalfPlaneSlice";
}

} // namespace yav::generator::region
