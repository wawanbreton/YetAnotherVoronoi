// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/FaceVertexBisectorGenerator.h"

#include <boost/geometry/core/access.hpp>

#include "yav/geometry/Point3Operations.h"
#include "yav/space/site/Triangle.h"
#include "yav/space/site/Vertex.h"
#include "yav/voronoi/equisurface/Paraboloid.h"

namespace yav::generator::bisector
{

FaceVertexBisectorGenerator::FaceVertexBisectorGenerator() = default;

std::shared_ptr<voronoi::equisurface::AbstractBisector> FaceVertexBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    auto triangle_site = std::dynamic_pointer_cast<space::site::Triangle>(first_site);
    auto vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(second_site);

    if (! triangle_site || ! vertex_site)
    {
        triangle_site = std::dynamic_pointer_cast<space::site::Triangle>(second_site);
        vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(first_site);
    }

    if (! triangle_site || ! vertex_site)
    {
        return nullptr;
    }

    const double offset
        = geometry::Point3Operations::meanZFromTriangle(triangle_site->triangle()) - boost::geometry::get<2>(vertex_site->position());
    return std::make_shared<voronoi::equisurface::Paraboloid>(vertex_site->position(), 1.0, offset);
}

} // namespace yav::generator::bisector
