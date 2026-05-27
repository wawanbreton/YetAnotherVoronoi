// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/EdgeEdgeBisectorGenerator.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>

#include "yav/geometry/Point3Operations.h"
#include "yav/space/site/Edge.h"
#include "yav/voronoi/equisurface/Plane.h"


namespace yav::generator::bisector
{

EdgeEdgeBisectorGenerator::EdgeEdgeBisectorGenerator() = default;

std::shared_ptr<voronoi::equisurface::AbstractBisector> EdgeEdgeBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    const auto first_edge = std::dynamic_pointer_cast<space::site::Edge>(first_site);
    const auto second_edge = std::dynamic_pointer_cast<space::site::Edge>(second_site);

    if (! first_edge || ! second_edge)
    {
        return nullptr;
    }

    const geometry::Point3 first_midpoint = geometry::Point3Operations::midpoint(first_edge->vertices()[0], first_edge->vertices()[1]);
    const geometry::Point3 second_midpoint = geometry::Point3Operations::midpoint(second_edge->vertices()[0], second_edge->vertices()[1]);

    geometry::Point3 normal = second_midpoint;
    boost::geometry::subtract_point(normal, first_midpoint);
    const geometry::Point3 midpoint = geometry::Point3Operations::midpoint(first_midpoint, second_midpoint);
    const double offset = -boost::geometry::dot_product(normal, midpoint);

    return std::make_shared<voronoi::equisurface::Plane>(normal, offset);
}

} // namespace yav::generator::bisector
