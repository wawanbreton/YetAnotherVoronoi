// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/VertexVertexBisectorGenerator.h"

#include "yav/space/site/Vertex.h"
#include "yav/voronoi/equisurface/Plane.h"

#include <boost/geometry/core/access.hpp>
#include <spdlog/spdlog.h>

namespace yav::generator::bisector
{
namespace
{

geometry::Point3 add(const geometry::Point3& first_value, const geometry::Point3& second_value)
{
    geometry::Point3 output;
    boost::geometry::set<0>(output, boost::geometry::get<0>(first_value) + boost::geometry::get<0>(second_value));
    boost::geometry::set<1>(output, boost::geometry::get<1>(first_value) + boost::geometry::get<1>(second_value));
    boost::geometry::set<2>(output, boost::geometry::get<2>(first_value) + boost::geometry::get<2>(second_value));
    return output;
}

geometry::Point3 scale(const geometry::Point3& value, const double factor)
{
    geometry::Point3 output;
    boost::geometry::set<0>(output, boost::geometry::get<0>(value) * factor);
    boost::geometry::set<1>(output, boost::geometry::get<1>(value) * factor);
    boost::geometry::set<2>(output, boost::geometry::get<2>(value) * factor);
    return output;
}

geometry::Point3 subtract(const geometry::Point3& first_value, const geometry::Point3& second_value)
{
    return add(first_value, scale(second_value, -1.0));
}

double dotProduct(const geometry::Point3& first_value, const geometry::Point3& second_value)
{
    return boost::geometry::get<0>(first_value) * boost::geometry::get<0>(second_value)
        + boost::geometry::get<1>(first_value) * boost::geometry::get<1>(second_value)
        + boost::geometry::get<2>(first_value) * boost::geometry::get<2>(second_value);
}

} // namespace

VertexVertexBisectorGenerator::VertexVertexBisectorGenerator() = default;

bool VertexVertexBisectorGenerator::canHandle(const space::site::SiteKind first_kind, const space::site::SiteKind second_kind) const
{
    return first_kind == space::site::SiteKind::Vertex && second_kind == space::site::SiteKind::Vertex;
}

std::shared_ptr<voronoi::equisurface::AbstractBisector> VertexVertexBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    const auto first_vertex = std::dynamic_pointer_cast<space::site::Vertex>(first_site);
    const auto second_vertex = std::dynamic_pointer_cast<space::site::Vertex>(second_site);

    if (!first_vertex || !second_vertex)
    {
        spdlog::error("VertexVertexBisectorGenerator requires two vertex sites");
        return nullptr;
    }

    const geometry::Point3 normal = subtract(second_vertex->position(), first_vertex->position());
    const geometry::Point3 midpoint = scale(add(first_vertex->position(), second_vertex->position()), 0.5);
    const double offset = -dotProduct(normal, midpoint);

    return std::make_shared<voronoi::equisurface::Plane>(normal, offset);
}

} // namespace yav::generator::bisector
