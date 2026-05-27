// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/FaceFaceBisectorGenerator.h"

#include "yav/geometry/Point3Operations.h"
#include "yav/space/site/Triangle.h"
#include "yav/voronoi/equisurface/Plane.h"

#include <spdlog/spdlog.h>

namespace yav::generator::bisector
{
namespace
{

geometry::Point3 centroid(const std::array<geometry::Point3, 3>& vertices)
{
    auto output = geometry::Point3Operations::add(vertices[0], vertices[1]);
    output = geometry::Point3Operations::add(output, vertices[2]);
    return geometry::Point3Operations::scale(output, 1.0 / 3.0);
}

} // namespace

FaceFaceBisectorGenerator::FaceFaceBisectorGenerator() = default;

std::shared_ptr<voronoi::equisurface::AbstractBisector> FaceFaceBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    const auto first_triangle = std::dynamic_pointer_cast<space::site::Triangle>(first_site);
    const auto second_triangle = std::dynamic_pointer_cast<space::site::Triangle>(second_site);

    if (!first_triangle || !second_triangle)
    {
        return nullptr;
    }

    const geometry::Point3 first_center = centroid(first_triangle->vertices());
    const geometry::Point3 second_center = centroid(second_triangle->vertices());
    const geometry::Point3 normal = geometry::Point3Operations::subtract(second_center, first_center);
    const geometry::Point3 midpoint = geometry::Point3Operations::midpoint(first_center, second_center);
    const double offset = -geometry::Point3Operations::dotProduct(normal, midpoint);

    return std::make_shared<voronoi::equisurface::Plane>(normal, offset);
}

} // namespace yav::generator::bisector
