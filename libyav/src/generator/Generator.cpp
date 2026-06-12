// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include <algorithm>
#include <array>
#include <queue>
#include <unordered_set>

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/detail/make/make.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/arithmetic/infinite_line_functions.hpp>
#include <spdlog/spdlog.h>

#include "yav/generator/VoronoiQuadtreeNode.h"
#include "yav/geometry/Line2.h"
#include "yav/space/Space2.h"
#include "yav/space/site/Vertex2.h"
#include "yav/voronoi/Diagram.h"


namespace yav
{

Generator::Generator() = default;

Diagram Generator::generate(const Space2& input_space) const
{
    Diagram diagram;

    if (input_space.sites().empty())
    {
        spdlog::warn("Requested Voronoi generation on an empty 2D space");
        return diagram;
    }

    const std::vector<VoronoiQuadtreeNode::Ptr> leaves = build(input_space);

    for (const VoronoiQuadtreeNode::Ptr& leaf : leaves)
    {
        addApproximationFromLeaf(*leaf, diagram);
    }

    spdlog::info("Generated 2D Voronoi approximation from {} sites and {} quadtree leaves", input_space.sites().size(), leaves.size());

    return diagram;
}

std::vector<std::shared_ptr<AbstractSite>> Generator::uniqueSitesFromCrossings(
    const std::vector<std::pair<std::shared_ptr<AbstractSite>, std::shared_ptr<AbstractSite>>>& edge_site_pairs)
{
    std::unordered_set<const AbstractSite*> unique_sites;
    std::vector<std::shared_ptr<AbstractSite>> result;

    for (const auto& [site_a, site_b] : edge_site_pairs)
    {
        if (site_a && unique_sites.insert(site_a.get()).second)
        {
            result.push_back(site_a);
        }

        if (site_b && unique_sites.insert(site_b.get()).second)
        {
            result.push_back(site_b);
        }
    }

    return result;
}

Point2 calculateBisectorVertexAlongSegment(
    const AbstractSite::Ptr& closest_site_start,
    const AbstractSite::Ptr& closest_site_end,
    const Segment2& segment)
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

void Generator::addApproximationFromLeaf(const VoronoiQuadtreeNode& leaf_node, Diagram& diagram)
{
    static constexpr std::array<std::pair<size_t, size_t>, 4> edge_corners{ {
        { 0, 1 },
        { 1, 3 },
        { 2, 3 },
        { 0, 2 },
    } };

    std::vector<Point2> crossings;
    std::vector<std::pair<AbstractSite::Ptr, AbstractSite::Ptr>> crossing_sites;

    for (size_t edge_index = 0; edge_index < edge_corners.size(); ++edge_index)
    {
        const auto [first_corner_index, second_corner_index] = edge_corners[edge_index];
        const AbstractSite::Ptr first_corner_site = leaf_node.cornerClosestSiteAt(first_corner_index).site;
        const AbstractSite::Ptr second_corner_site = leaf_node.cornerClosestSiteAt(second_corner_index).site;

        if (! first_corner_site || ! second_corner_site || first_corner_site == second_corner_site)
        {
            continue;
        }

        // crossings.push_back(leaf_node.edgeMidpointAt(edge_index));
        crossings.push_back(calculateBisectorVertexAlongSegment(
            first_corner_site,
            second_corner_site,
            Segment2(leaf_node.cornerAt(first_corner_index), leaf_node.cornerAt(second_corner_index))));
        crossing_sites.push_back({ first_corner_site, second_corner_site });
    }

    if (crossings.size() == 2)
    {
        diagram.addBoundarySegment(Segment2(crossings[0], crossings[1]), uniqueSitesFromCrossings(crossing_sites));
        return;
    }

    if (crossings.size() == 4)
    {
        diagram.addBoundarySegment(Segment2(crossings[0], crossings[1]), uniqueSitesFromCrossings(crossing_sites));
        diagram.addBoundarySegment(Segment2(crossings[2], crossings[3]), uniqueSitesFromCrossings(crossing_sites));
        return;
    }

    if (! crossings.empty())
    {
        spdlog::debug("Leaf generated an unsupported number of crossings ({})", crossings.size());
    }
}

std::vector<VoronoiQuadtreeNode::Ptr> Generator::build(const Space2& input_space) const
{
    VoronoiQuadtreeNode::Ptr root = initialize(input_space);
    if (! root)
    {
        return {};
    }

    std::vector<VoronoiQuadtreeNode::Ptr> leaves;
    std::queue<VoronoiQuadtreeNode::Ptr> node_queue;
    node_queue.push(root);

    while (! node_queue.empty())
    {
        const VoronoiQuadtreeNode::Ptr node = node_queue.front();
        node_queue.pop();

        update(*node);

        if (isLeaf(*node))
        {
            propagate(*node, leaves);
            compact(node->parent());
            leaves.push_back(node);
            continue;
        }

        subdivide(*node);
        for (const VoronoiQuadtreeNode::Ptr& child : node->children())
        {
            propagate(*child, leaves);
            node_queue.push(child);
        }
    }

    return leaves;
}

VoronoiQuadtreeNode::Ptr Generator::initialize(const Space2& input_space) const
{
    const std::vector<std::shared_ptr<AbstractSite>>& sites = input_space.sites();
    if (sites.empty())
    {
        spdlog::warn("Cannot initialize quadtree on an empty site set");
        return nullptr;
    }

    double min_x = std::numeric_limits<double>::infinity();
    double min_y = std::numeric_limits<double>::infinity();
    double max_x = -std::numeric_limits<double>::infinity();
    double max_y = -std::numeric_limits<double>::infinity();

    for (const std::shared_ptr<AbstractSite>& site : sites)
    {
        if (! site)
        {
            spdlog::error("Encountered null site while initializing the quadtree");
            continue;
        }

        for (const Point2& point : site->definingPoints())
        {
            min_x = std::min(min_x, point.get<0>());
            min_y = std::min(min_y, point.get<1>());
            max_x = std::max(max_x, point.get<0>());
            max_y = std::max(max_y, point.get<1>());
        }
    }

    if (! std::isfinite(min_x) || ! std::isfinite(min_y) || ! std::isfinite(max_x) || ! std::isfinite(max_y))
    {
        spdlog::error("Failed to estimate quadtree bounds from the input sites");
        return nullptr;
    }

    const double span_x = max_x - min_x;
    const double span_y = max_y - min_y;
    const double max_span = std::max(span_x, span_y);
    const double width = std::max(max_span * 1.2, 1.0);
    const Point2 center((min_x + max_x) / 2.0, (min_y + max_y) / 2.0);

    VoronoiQuadtreeNode::Ptr root = std::make_shared<VoronoiQuadtreeNode>(center, width, 0, nullptr);
    root->setCandidateSites(sites);
    return root;
}

void Generator::update(VoronoiQuadtreeNode& node) const
{
    for (size_t corner_index = 0; corner_index < node.cornerClosestSites().size(); ++corner_index)
    {
        const Point2 corner = node.cornerAt(corner_index);
        ClosestSite closest_site{ nullptr, std::numeric_limits<double>::infinity() };

        for (const std::shared_ptr<AbstractSite>& site : node.candidateSites())
        {
            if (! site)
            {
                spdlog::error("Encountered null candidate site while updating a quadtree node");
                continue;
            }

            const double distance = site->distanceTo(corner);
            if (distance < closest_site.distance)
            {
                closest_site.site = site;
                closest_site.distance = distance;
            }
        }

        node.setCornerClosestSite(corner_index, closest_site);
    }
}

bool Generator::isLeaf(const VoronoiQuadtreeNode& node) const
{
    return node.level() >= maximum_level_ || node.isTerminal();
}

void Generator::subdivide(VoronoiQuadtreeNode& node) const
{
    node.split();
}

void Generator::propagate(VoronoiQuadtreeNode& node, const std::vector<VoronoiQuadtreeNode::Ptr>& leaves) const
{
    constexpr double point_tolerance = 1e-9;

    for (const VoronoiQuadtreeNode::Ptr& leaf : leaves)
    {
        if (! leaf)
        {
            continue;
        }

        for (size_t node_corner_index = 0; node_corner_index < node.cornerClosestSites().size(); ++node_corner_index)
        {
            const Point2 node_corner = node.cornerAt(node_corner_index);
            if (! leaf->containsPoint(node_corner, point_tolerance))
            {
                continue;
            }

            for (size_t leaf_corner_index = 0; leaf_corner_index < leaf->cornerClosestSites().size(); ++leaf_corner_index)
            {
                const Point2 leaf_corner = leaf->cornerAt(leaf_corner_index);
                if (std::abs(node_corner.get<0>() - leaf_corner.get<0>()) > point_tolerance
                    || std::abs(node_corner.get<1>() - leaf_corner.get<1>()) > point_tolerance)
                {
                    continue;
                }

                const ClosestSite node_corner_site = node.cornerClosestSiteAt(node_corner_index);
                const ClosestSite leaf_corner_site = leaf->cornerClosestSiteAt(leaf_corner_index);

                if (node_corner_site.distance < leaf_corner_site.distance)
                {
                    leaf->setCornerClosestSite(leaf_corner_index, node_corner_site);
                }
                else
                {
                    node.setCornerClosestSite(node_corner_index, leaf_corner_site);
                }
            }
        }
    }
}

void Generator::compact(const std::shared_ptr<VoronoiQuadtreeNode>& parent) const
{
    if (! parent)
    {
        return;
    }

    const bool are_all_children_terminal = std::ranges::all_of(
        parent->children(),
        [](const VoronoiQuadtreeNode::Ptr& child)
        {
            return child && child->isTerminal();
        });

    if (! are_all_children_terminal)
    {
        return;
    }

    // Intentional no-op in this 2D prototype: pruning removes per-child boundary detail needed for extraction.
}

} // namespace yav
