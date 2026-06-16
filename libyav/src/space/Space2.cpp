// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space2.h"

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/detail/make/make.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/arithmetic/infinite_line_functions.hpp>
#include <spdlog/spdlog.h>

#include "yav/geometry/Line2.h"
#include "yav/space/site/Edge2.h"
#include "yav/space/site/Vertex2.h"


namespace yav
{

Space2::Space2() = default;

std::shared_ptr<AbstractSite> Space2::addEdge(const Segment2& edge)
{
    spdlog::debug("Add edge2 {}", edge);

    return addSite(std::make_shared<Edge2>(edge));
}

std::shared_ptr<AbstractSite> Space2::addVertex(const Point2& vertex)
{
    spdlog::debug("Add vertex2 {}", vertex);

    return addSite(std::make_shared<Vertex2>(vertex));
}

Point2 Space2::calculateBisectorVertexAlongSegment(
    const AbstractSite::Ptr& closest_site_start,
    const AbstractSite::Ptr& closest_site_end,
    const Segment2& segment) const
{
    auto start_vertex = std::dynamic_pointer_cast<Vertex2>(closest_site_start);
    auto end_vertex = std::dynamic_pointer_cast<Vertex2>(closest_site_end);
    Point2 result;

    if (start_vertex && end_vertex)
    {
        const Point2 start = start_vertex->basePoint();
        const Point2 end = end_vertex->basePoint();
        const Segment2 sites_segment(start, end);
        const Segment2 segment_bisector = rotate90(sites_segment, false);
        const Line2 bisector = boost::geometry::detail::make::make_infinite_line<double>(segment_bisector);
        const Line2 infinite_segment = boost::geometry::detail::make::make_infinite_line<double>(segment);
        if (boost::geometry::arithmetic::intersection_point(bisector, infinite_segment, result))
        {
            return result;
        }
    }
    else
    {
        spdlog::warn("Unusupported combination of sites for bisector calculation");
    }

    spdlog::warn("Unable to calculate intersection point, defaulting to centroid");
    boost::geometry::centroid(segment, result);

    return result;
}

ClosestSite Space2::findClosestSite(const Point2& position, const std::set<std::shared_ptr<AbstractSite>>& candidate_sites) const
{
    ClosestSite closest_site = ClosestSite{ nullptr, std::numeric_limits<double>::infinity() };

    for (const AbstractSite::Ptr& site : candidate_sites)
    {
        const double distance = site->distanceTo(position);
        if (distance < closest_site.distance)
        {
            closest_site.site = site;
            closest_site.distance = distance;
        }
    }

    return closest_site;
}

} // namespace yav
