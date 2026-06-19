// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space2.h"

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/closest_points.hpp>
#include <boost/geometry/algorithms/detail/make/make.hpp>
#include <boost/geometry/algorithms/distance.hpp>
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

double Space2::distance(const std::shared_ptr<AbstractSite>& site, const Point2& position) const
{
    if (auto site_vertex = std::dynamic_pointer_cast<Vertex2>(site))
    {
        return boost::geometry::distance(site_vertex->basePoint(), position);
    }

    spdlog::warn("Unusupported site type for distance calculation");
    return std::numeric_limits<double>::infinity();
}

Point2 Space2::calculateEquidistantPosition(
    const std::shared_ptr<AbstractSite>& site1,
    const std::shared_ptr<AbstractSite>& site2,
    const std::shared_ptr<AbstractSite>& site3) const
{
    auto vertex1 = std::dynamic_pointer_cast<Vertex2>(site1);
    auto vertex2 = std::dynamic_pointer_cast<Vertex2>(site2);
    auto vertex3 = std::dynamic_pointer_cast<Vertex2>(site3);

    if (vertex1 && vertex2 && vertex3)
    {
        const Point2 point1 = vertex1->basePoint();
        const Point2 point2 = vertex2->basePoint();
        const Point2 point3 = vertex3->basePoint();

        Point2 midpoint12;
        Point2 midpoint13;
        boost::geometry::centroid(Segment2(point1, point2), midpoint12);
        boost::geometry::centroid(Segment2(point1, point3), midpoint13);

        const Line2 bisector12 = boost::geometry::detail::make::make_perpendicular_line<double>(point1, point2, midpoint12);
        const Line2 bisector13 = boost::geometry::detail::make::make_perpendicular_line<double>(point1, point3, midpoint13);

        Point2 equidistant_position;
        if (boost::geometry::arithmetic::intersection_point(bisector12, bisector13, equidistant_position))
        {
            return equidistant_position;
        }

        spdlog::warn("Unable to calculate equidistant point from perpendicular bisectors, defaulting to centroid");
        return (point1 + point2 + point3) / 3.0;
    }
    else
    {
        spdlog::warn("Unusupported combination of sites for equidistant point calculation");
    }

    return Point2();
}

Segment2 Space2::closestSegmentToSide(const std::shared_ptr<AbstractSite>& site, const Segment2& side) const
{
    if (auto vertex = std::dynamic_pointer_cast<Vertex2>(site))
    {
        Segment2 shortest_segment;
        boost::geometry::closest_points(side, vertex->basePoint(), shortest_segment);
        return shortest_segment;
    }

    spdlog::warn("Unusupported site type for closest distance to side calculation");
    return Segment2();
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

bool Space2::isBisectorFlatWithinRegion(
    const std::shared_ptr<AbstractSite>& site1,
    const std::shared_ptr<AbstractSite>& site2,
    const Box2& region) const
{
    auto vertex1 = std::dynamic_pointer_cast<Vertex2>(site1);
    auto vertex2 = std::dynamic_pointer_cast<Vertex2>(site2);
    if (vertex1 && vertex2)
    {
        // Vertices bisector is always an infinite line, so definitely flat :)
        return true;
    }

    return AbstractSpace::isBisectorFlatWithinRegion(site1, site2, region);
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
