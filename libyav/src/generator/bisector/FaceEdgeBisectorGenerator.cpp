// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/FaceEdgeBisectorGenerator.h"

#include "yav/space/site/Edge.h"
#include "yav/space/site/Triangle.h"
#include "yav/voronoi/equisurface/Paraboloid.h"

#include <boost/geometry/core/access.hpp>
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

double averageZ(const std::array<geometry::Point3, 3>& vertices)
{
    return (boost::geometry::get<2>(vertices[0]) + boost::geometry::get<2>(vertices[1])
               + boost::geometry::get<2>(vertices[2]))
        / 3.0;
}

} // namespace

FaceEdgeBisectorGenerator::FaceEdgeBisectorGenerator() = default;

bool FaceEdgeBisectorGenerator::canHandle(const space::site::SiteKind first_kind, const space::site::SiteKind second_kind) const
{
    const bool direct_order = first_kind == space::site::SiteKind::Triangle && second_kind == space::site::SiteKind::Edge;
    const bool reverse_order = first_kind == space::site::SiteKind::Edge && second_kind == space::site::SiteKind::Triangle;
    return direct_order || reverse_order;
}

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
        spdlog::error("FaceEdgeBisectorGenerator requires one triangle and one edge site");
        return nullptr;
    }

    const geometry::Point3 edge_midpoint = scale(add(edge_site->vertices()[0], edge_site->vertices()[1]), 0.5);
    const double offset = averageZ(triangle_site->vertices()) - boost::geometry::get<2>(edge_midpoint);

    return std::make_shared<voronoi::equisurface::Paraboloid>(edge_midpoint, 1.0, offset);
}

} // namespace yav::generator::bisector
