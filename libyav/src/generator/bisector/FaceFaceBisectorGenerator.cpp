// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/FaceFaceBisectorGenerator.h"

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>

#include "yav/geometry/Point3Operations.h"
#include "yav/space/site/Triangle.h"
#include "yav/voronoi/equisurface/Plane.h"

namespace yav::generator::bisector
{
FaceFaceBisectorGenerator::FaceFaceBisectorGenerator() = default;

std::shared_ptr<voronoi::equisurface::AbstractBisector> FaceFaceBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    const auto first_triangle = std::dynamic_pointer_cast<space::site::Triangle>(first_site);
    const auto second_triangle = std::dynamic_pointer_cast<space::site::Triangle>(second_site);

    if (! first_triangle || ! second_triangle)
    {
        return nullptr;
    }

    geometry::Point3 first_center;
    boost::geometry::centroid(first_triangle->triangle(), first_center);
    geometry::Point3 second_center;
    boost::geometry::centroid(second_triangle->triangle(), second_center);
    geometry::Point3 normal = second_center;
    boost::geometry::subtract_point(normal, first_center);
    const geometry::Point3 midpoint = geometry::Point3Operations::midpoint(first_center, second_center);
    const double offset = -boost::geometry::dot_product(normal, midpoint);

    return std::make_shared<voronoi::equisurface::Plane>(normal, offset);
}

} // namespace yav::generator::bisector
