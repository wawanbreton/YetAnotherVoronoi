// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/VertexVertexBisectorGenerator.h"

#include "yav/geometry/Point3Operations.h"
#include "yav/space/site/Vertex.h"
#include "yav/voronoi/equisurface/Plane.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>

namespace yav::generator::bisector
{

VertexVertexBisectorGenerator::VertexVertexBisectorGenerator() = default;

std::shared_ptr<voronoi::equisurface::AbstractBisector> VertexVertexBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    const auto first_vertex = std::dynamic_pointer_cast<space::site::Vertex>(first_site);
    const auto second_vertex = std::dynamic_pointer_cast<space::site::Vertex>(second_site);

    if (!first_vertex || !second_vertex)
    {
        return nullptr;
    }

    geometry::Point3 normal = second_vertex->position();
    boost::geometry::subtract_point(normal, first_vertex->position());
    const geometry::Point3 midpoint = geometry::Point3Operations::midpoint(first_vertex->position(), second_vertex->position());
    const double offset = -boost::geometry::dot_product(normal, midpoint);

    return std::make_shared<voronoi::equisurface::Plane>(normal, offset);
}

} // namespace yav::generator::bisector
