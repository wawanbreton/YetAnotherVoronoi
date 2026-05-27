// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/EdgeVertexBisectorGenerator.h"

#include "yav/space/site/Edge.h"
#include "yav/space/site/Vertex.h"
#include "yav/voronoi/equisurface/Paraboloid.h"

#include <boost/geometry/access.hpp>
#include <spdlog/spdlog.h>

namespace yav::generator::bisector
{
namespace
{

geometry::Point3 scale(const geometry::Point3& value, const double factor)
{
    geometry::Point3 output;
    boost::geometry::set<0>(output, boost::geometry::get<0>(value) * factor);
    boost::geometry::set<1>(output, boost::geometry::get<1>(value) * factor);
    boost::geometry::set<2>(output, boost::geometry::get<2>(value) * factor);
    return output;
}

geometry::Point3 add(const geometry::Point3& first_value, const geometry::Point3& second_value)
{
    geometry::Point3 output;
    boost::geometry::set<0>(output, boost::geometry::get<0>(first_value) + boost::geometry::get<0>(second_value));
    boost::geometry::set<1>(output, boost::geometry::get<1>(first_value) + boost::geometry::get<1>(second_value));
    boost::geometry::set<2>(output, boost::geometry::get<2>(first_value) + boost::geometry::get<2>(second_value));
    return output;
}

} // namespace

EdgeVertexBisectorGenerator::EdgeVertexBisectorGenerator() = default;

bool EdgeVertexBisectorGenerator::canHandle(const space::site::SiteKind first_kind, const space::site::SiteKind second_kind) const
{
    const bool direct_order = first_kind == space::site::SiteKind::Edge && second_kind == space::site::SiteKind::Vertex;
    const bool reverse_order = first_kind == space::site::SiteKind::Vertex && second_kind == space::site::SiteKind::Edge;
    return direct_order || reverse_order;
}

std::shared_ptr<voronoi::equisurface::AbstractBisector> EdgeVertexBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    auto edge_site = std::dynamic_pointer_cast<space::site::Edge>(first_site);
    auto vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(second_site);

    if (!edge_site || !vertex_site)
    {
        edge_site = std::dynamic_pointer_cast<space::site::Edge>(second_site);
        vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(first_site);
    }

    if (!edge_site || !vertex_site)
    {
        spdlog::error("EdgeVertexBisectorGenerator requires one edge and one vertex site");
        return nullptr;
    }

    const geometry::Point3 edge_midpoint = scale(add(edge_site->vertices()[0], edge_site->vertices()[1]), 0.5);
    const double offset = boost::geometry::get<2>(edge_midpoint) - boost::geometry::get<2>(vertex_site->position());

    return std::make_shared<voronoi::equisurface::Paraboloid>(vertex_site->position(), 1.0, offset);
}

} // namespace yav::generator::bisector
