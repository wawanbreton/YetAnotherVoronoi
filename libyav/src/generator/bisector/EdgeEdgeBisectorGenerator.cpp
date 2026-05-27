// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/bisector/EdgeEdgeBisectorGenerator.h"

#include "yav/space/site/Edge.h"
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

EdgeEdgeBisectorGenerator::EdgeEdgeBisectorGenerator() = default;

bool EdgeEdgeBisectorGenerator::canHandle(const space::site::SiteKind first_kind, const space::site::SiteKind second_kind) const
{
    return first_kind == space::site::SiteKind::Edge && second_kind == space::site::SiteKind::Edge;
}

std::shared_ptr<voronoi::equisurface::AbstractBisector> EdgeEdgeBisectorGenerator::generate(
    const std::shared_ptr<space::site::AbstractSite>& first_site,
    const std::shared_ptr<space::site::AbstractSite>& second_site) const
{
    const auto first_edge = std::dynamic_pointer_cast<space::site::Edge>(first_site);
    const auto second_edge = std::dynamic_pointer_cast<space::site::Edge>(second_site);

    if (!first_edge || !second_edge)
    {
        spdlog::error("EdgeEdgeBisectorGenerator requires two edge sites");
        return nullptr;
    }

    const geometry::Point3 first_midpoint = scale(add(first_edge->vertices()[0], first_edge->vertices()[1]), 0.5);
    const geometry::Point3 second_midpoint = scale(add(second_edge->vertices()[0], second_edge->vertices()[1]), 0.5);

    const geometry::Point3 normal = subtract(second_midpoint, first_midpoint);
    const geometry::Point3 midpoint = scale(add(first_midpoint, second_midpoint), 0.5);
    const double offset = -dotProduct(normal, midpoint);

    return std::make_shared<voronoi::equisurface::Plane>(normal, offset);
}

} // namespace yav::generator::bisector
