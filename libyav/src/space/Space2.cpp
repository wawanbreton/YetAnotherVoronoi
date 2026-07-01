// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space2.h"

#include <armadillo>
#include <ranges>

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/closest_points.hpp>
#include <boost/geometry/algorithms/detail/make/make.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/geometry/arithmetic/infinite_line_functions.hpp>
#include <spdlog/spdlog.h>

#include "yav/geometry/Line2.h"
#include "yav/space/site/Edge2.h"
#include "yav/space/site/Vertex2.h"


namespace bg = boost::geometry;

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

std::optional<Segment2> Space2::calculateSegmentPartClosestToSite(
    const Segment2& segment,
    const std::shared_ptr<AbstractSite>& closest_site_start,
    const std::shared_ptr<AbstractSite>& other_site) const
{
    const auto closest_site_start_vertex = std::dynamic_pointer_cast<Vertex2>(closest_site_start);
    const auto closest_site_start_edge = std::dynamic_pointer_cast<Edge2>(closest_site_start);

    const auto other_site_vertex = std::dynamic_pointer_cast<Vertex2>(other_site);
    const auto other_site_edge = std::dynamic_pointer_cast<Edge2>(other_site);

    if (closest_site_start_vertex && other_site_vertex)
    {
        const std::optional<Point2> intersection
            = calculateVerticesBisectorAlongSegment(closest_site_start_vertex->position(), other_site_vertex->position(), segment);
        if (intersection.has_value())
        {
            return Segment2(*intersection, segment.second);
        }
    }
    else if (closest_site_start_vertex && other_site_edge || closest_site_start_edge && other_site_vertex)
    {
        const Vertex2::Ptr& site_vertex = closest_site_start_vertex ? closest_site_start_vertex : other_site_vertex;
        const Edge2::Ptr& site_edge = closest_site_start_edge ? closest_site_start_edge : other_site_edge;

        const Point2& site_vertex_position = site_vertex->position();
        const Segment2& site_edge_segment = site_edge->segment();

        std::vector<Point2> intersections = calculateEdgeVertexBisectorAlongSegment(site_vertex_position, site_edge_segment, segment);
        if (intersections.size() == 1)
        {
            return Segment2(intersections.front(), segment.second);
        }
        else if (intersections.size() == 2)
        {
            return Segment2(intersections.front(), intersections.back());
        }
    }
    else
    {
        spdlog::warn("Unusupported combination of sites for bisector calculation without edge site");
    }

    return std::nullopt;
}

std::optional<Point2> Space2::calculateEquidistantPosition(
    const std::shared_ptr<AbstractSite>& site1,
    const std::shared_ptr<AbstractSite>& site2,
    const std::shared_ptr<AbstractSite>& site3) const
{
    auto vertex1 = std::dynamic_pointer_cast<Vertex2>(site1);
    auto vertex2 = std::dynamic_pointer_cast<Vertex2>(site2);
    auto vertex3 = std::dynamic_pointer_cast<Vertex2>(site3);

    if (vertex1 && vertex2 && vertex3)
    {
        const Point2& point1 = vertex1->position();
        const Point2& point2 = vertex2->position();
        const Point2& point3 = vertex3->position();

        Point2 midpoint12;
        Point2 midpoint13;
        bg::centroid(Segment2(point1, point2), midpoint12);
        bg::centroid(Segment2(point1, point3), midpoint13);

        const Line2 bisector12 = bg::detail::make::make_perpendicular_line<double>(point1, point2, midpoint12);
        const Line2 bisector13 = bg::detail::make::make_perpendicular_line<double>(point1, point3, midpoint13);

        Point2 equidistant_position;
        if (bg::arithmetic::intersection_point(bisector12, bisector13, equidistant_position))
        {
            return equidistant_position;
        }
    }

    return std::nullopt;
}

std::optional<Point2>
    Space2::calculateVerticesBisectorAlongSegment(const Point2& vertex1, const Point2& vertex2, const Segment2& segment) const
{
    const Segment2 sites_segment(vertex1, vertex2);
    const Segment2 segment_bisector = rotate90(sites_segment, false);
    const Line2 bisector = bg::detail::make::make_infinite_line<double>(segment_bisector);
    const Line2 infinite_segment = bg::detail::make::make_infinite_line<double>(segment);
    Point2 result;
    if (bg::arithmetic::intersection_point(bisector, infinite_segment, result)
        && projectedPointsLiesOnSegment(segment, result) == PointPositionOnSegment::Inside)
    {
        return result;
    }

    return std::nullopt;
}

std::vector<Point2> Space2::calculateEdgeVertexBisectorAlongSegment(
    const Point2& vertex_site_position,
    const Segment2& edge_site_segment,
    const Segment2& segment) const
{
    const PointPositionOnSegment start_position_on_segment = projectedPointsLiesOnSegment(edge_site_segment, segment.first);
    const PointPositionOnSegment end_position_on_segment = projectedPointsLiesOnSegment(edge_site_segment, segment.second);

    if (start_position_on_segment == end_position_on_segment)
    {
        // Segment is fully contained in the same Voronoi region of the edge
        return calculateEdgeVertexBisectorAlongSegment(vertex_site_position, edge_site_segment, segment, start_position_on_segment);
    }
    else
    {
        // Segment is not fully contained in the same Voronoi region of the edge, we have to split it
    }

    return {};
}

std::vector<Point2> Space2::calculateEdgeVertexBisectorAlongSegment(
    const Point2& vertex_site_position,
    const Segment2& edge_site_segment,
    const Segment2& segment,
    const PointPositionOnSegment position_on_segment) const
{
    if (position_on_segment == PointPositionOnSegment::Before)
    {
        // Segment is in the Voronoi region of the first vertex, bisector is a straight line as for vertex/vertex
        std::optional<Point2> intersection = calculateVerticesBisectorAlongSegment(edge_site_segment.first, vertex_site_position, segment);
        return intersection.has_value() ? std::vector<Point2>{ intersection.value() } : std::vector<Point2>{};
    }
    else if (position_on_segment == PointPositionOnSegment::After)
    {
        // Segment is in the Voronoi region of the second vertex, bisector is a straight line as for vertex/vertex
        std::optional<Point2> intersection = calculateVerticesBisectorAlongSegment(edge_site_segment.second, vertex_site_position, segment);
        return intersection.has_value() ? std::vector<Point2>{ intersection.value() } : std::vector<Point2>{};
    }
    else
    {
        // Segment is in the edge Voronoi region, bisector is a parabol

        // --- Step 1: Get line equation from edge_site_segment (directrix) ---
        // Get edge line equation
        const Line2 line = bg::detail::make::make_infinite_line<double>(edge_site_segment);
        const double a = line.a; // Coefficients from line equation: a*x + b*y + c = 0
        const double b = line.b;
        const double c = line.c;
        const double denom = a * a + b * b; // Normalization factor

        // --- Step 2: Parameterize test segment: X(t) = S + t*(E - S) ---
        const Point2& S = segment.first;
        const Point2& E = segment.second;
        const Point2 SE = E - S;
        const double dx = SE.x();
        const double dy = SE.y();

        // --- Step 3: Substitute X(t) into parabola equation ---
        // Parabola: (x - x_p)² + (y - y_p)² = (a*x + b*y + c)² / denom
        const double x_p = vertex_site_position.x();
        const double y_p = vertex_site_position.y();

        // Precompute constants
        const double C1 = S.x() - x_p; // x_s - x_p
        const double C2 = S.y() - y_p; // y_s - y_p
        const double C3 = a * S.x() + b * S.y() + c; // Line equation at S
        const double C4 = a * dx + b * dy; // Derivative of line equation along segment

        // Coefficients of quadratic: A*t² + B*t + C = 0
        const double A = (dx * dx + dy * dy) * denom - C4 * C4;
        const double B = 2 * (dx * C1 + dy * C2) * denom - 2 * C3 * C4;
        const double C = (C1 * C1 + C2 * C2) * denom - C3 * C3;

        std::vector<Point2> intersections;

        if (std::abs(A) < 1e-12)
        {
            // Linear case: B*t + C = 0
            if (std::abs(B) > 1e-12)
            {
                double t = -C / B;
                if (t >= 0.0 && t <= 1.0)
                {
                    intersections.emplace_back(S.x() + t * dx, S.y() + t * dy);
                }
            }
        }
        else
        {
            // Quadratic case
            double discriminant = B * B - 4 * A * C;
            if (discriminant >= 0)
            {
                double sqrt_disc = std::sqrt(discriminant);
                double t1 = (-B + sqrt_disc) / (2 * A);
                double t2 = (-B - sqrt_disc) / (2 * A);

                for (double t : { t1, t2 })
                {
                    if (t >= 0.0 && t <= 1.0 && std::isfinite(t))
                    {
                        intersections.emplace_back(S.x() + t * dx, S.y() + t * dy);
                    }
                }
            }
        }

        // --- Step 4: Solve with Armadillo (handles all cases) ---
        // arma::vec coeffs = { C, B, A }; // Polynomial: C + B*t + A*t²
        // arma::cx_vec roots = arma::roots(coeffs); // Complex roots
        // constexpr double eps = 1e-10;
        // for (arma::cx_double root : roots)
        // {
        //     // 1. Skip non-real or infinite roots
        //     if (std::abs(root.imag()) > eps || ! std::isfinite(root.real()))
        //     {
        //         continue;
        //     }

        //     double t = root.real();
        //     // 2. Strict check: t must be in [0, 1] (with tiny tolerance for FP errors)
        //     if (t >= -eps && t <= 1.0 + eps)
        //     {
        //         intersections.emplace_back(S.x() + t * dx, S.y() + t * dy);
        //     }
        // }

        return intersections;
    }
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

AbstractSite::Ptr Space2::findClosestSite(const Point2& position, const std::set<AbstractSite::Ptr>& candidate_sites) const
{
    AbstractSite::Ptr closest_site;
    double closest_site_distance;

    for (const AbstractSite::Ptr& site : candidate_sites)
    {
        const double distance = site->distanceTo(position);
        if (! closest_site || distance < closest_site_distance)
        {
            closest_site = site;
            closest_site_distance = distance;
        }
    }

    return closest_site;
}

} // namespace yav
