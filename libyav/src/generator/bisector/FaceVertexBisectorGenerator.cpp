// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/FaceVertexBisectorGenerator.h"

#include "yav/space/site/Triangle.h"
#include "yav/space/site/Vertex.h"
#include "yav/voronoi/equisurface/Paraboloid.h"

#include <boost/geometry/core/access.hpp>
#include <spdlog/spdlog.h>

namespace yav::generator::bisector
{
namespace
{

double averageZ(const std::array<geometry::Point3, 3>& vertices)
{
    return (boost::geometry::get<2>(vertices[0]) + boost::geometry::get<2>(vertices[1])
               + boost::geometry::get<2>(vertices[2]))
        / 3.0;
}

} // namespace

FaceVertexBisectorGenerator::FaceVertexBisectorGenerator() = default;

bool FaceVertexBisectorGenerator::canHandle(const space::site::SiteKind first_kind, const space::site::SiteKind second_kind) const
{
    const bool direct_order = first_kind == space::site::SiteKind::Triangle && second_kind == space::site::SiteKind::Vertex;
    const bool reverse_order = first_kind == space::site::SiteKind::Vertex && second_kind == space::site::SiteKind::Triangle;
    return direct_order || reverse_order;
}

std::shared_ptr<voronoi::equisurface::AbstractBisector> FaceVertexBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    auto triangle_site = std::dynamic_pointer_cast<space::site::Triangle>(first_site);
    auto vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(second_site);

    if (!triangle_site || !vertex_site)
    {
        triangle_site = std::dynamic_pointer_cast<space::site::Triangle>(second_site);
        vertex_site = std::dynamic_pointer_cast<space::site::Vertex>(first_site);
    }

    if (!triangle_site || !vertex_site)
    {
        spdlog::error("FaceVertexBisectorGenerator requires one triangle and one vertex site");
        return nullptr;
    }

    const double offset = averageZ(triangle_site->vertices()) - boost::geometry::get<2>(vertex_site->position());
    return std::make_shared<voronoi::equisurface::Paraboloid>(vertex_site->position(), 1.0, offset);
}

} // namespace yav::generator::bisector
