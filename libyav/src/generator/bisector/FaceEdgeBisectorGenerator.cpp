// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/FaceEdgeBisectorGenerator.h"

#include "yav/geometry/Point3Operations.h"
#include "yav/space/site/Edge.h"
#include "yav/space/site/Triangle.h"
#include "yav/voronoi/equisurface/Paraboloid.h"

namespace yav::generator::bisector
{

FaceEdgeBisectorGenerator::FaceEdgeBisectorGenerator() = default;

std::shared_ptr<voronoi::equisurface::AbstractBisector> FaceEdgeBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    auto triangle_site = std::dynamic_pointer_cast<space::site::Triangle>(first_site);
    auto edge_site = std::dynamic_pointer_cast<space::site::Edge>(second_site);

    if (!triangle_site || !edge_site)
    {
        triangle_site = std::dynamic_pointer_cast<space::site::Triangle>(second_site);
        edge_site = std::dynamic_pointer_cast<space::site::Edge>(first_site);
    }

    if (!triangle_site || !edge_site)
    {
        return nullptr;
    }

    const geometry::Point3 edge_midpoint =
        geometry::Point3Operations::midpoint(edge_site->vertices()[0], edge_site->vertices()[1]);
    const double offset = geometry::Point3Operations::meanZFromTriangle(triangle_site->vertices())
        - geometry::Point3Operations::meanZFromSegment(edge_site->vertices());

    return std::make_shared<voronoi::equisurface::Paraboloid>(edge_midpoint, 1.0, offset);
}

} // namespace yav::generator::bisector
