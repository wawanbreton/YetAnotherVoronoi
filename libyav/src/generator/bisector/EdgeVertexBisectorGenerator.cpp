// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/EdgeVertexBisectorGenerator.h"

#include <boost/geometry/core/access.hpp>

#include "yav/geometry/Point3Operations.h"
#include "yav/space/site/Edge.h"
#include "yav/space/site/Vertex.h"
#include "yav/voronoi/equisurface/Paraboloid.h"

namespace yav::generator::bisector
{

EdgeVertexBisectorGenerator::EdgeVertexBisectorGenerator() = default;

std::shared_ptr<voronoi::equisurface::AbstractBisector> EdgeVertexBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    auto edge_site = std::dynamic_pointer_cast<space::site::Edge>(first_site);
    auto vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(second_site);

    if (! edge_site || ! vertex_site)
    {
        edge_site = std::dynamic_pointer_cast<space::site::Edge>(second_site);
        vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(first_site);
    }

    if (! edge_site || ! vertex_site)
    {
        return nullptr;
    }

    const geometry::Point3 edge_midpoint = geometry::Point3Operations::midpoint(edge_site->segment().first, edge_site->segment().second);
    const double offset = boost::geometry::get<2>(edge_midpoint) - boost::geometry::get<2>(vertex_site->position());

    return std::make_shared<voronoi::equisurface::Paraboloid>(vertex_site->position(), 1.0, offset);
}

} // namespace yav::generator::bisector
