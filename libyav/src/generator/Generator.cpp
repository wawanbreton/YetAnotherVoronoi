// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include <algorithm>
#include <array>
#include <queue>
#include <ranges>
#include <unordered_set>

#include <spdlog/spdlog.h>

#include "yav/generator/VoronoiQuadtreeNode.h"
#include "yav/space/Space2.h"
#include "yav/space/site/AbstractSite.h"
#include "yav/voronoi/Diagram.h"


namespace yav
{

Generator::Generator() = default;

std::tuple<Diagram, VoronoiQuadtreeNode::Ptr, std::vector<VoronoiQuadtreeNode::Ptr>> Generator::generate(const Space2& input_space) const
{
    if (input_space.sites().empty())
    {
        spdlog::warn("Requested Voronoi generation on an empty 2D space");
        return {};
    }

    Diagram diagram;

    const auto [root, leaves] = build(input_space);

    for (const VoronoiQuadtreeNode::Ptr& leaf : leaves)
    {
        addApproximationFromLeaf(*leaf, diagram, input_space);
    }

    spdlog::info("Generated 2D Voronoi approximation from {} sites and {} quadtree leaves", input_space.sites().size(), leaves.size());

    return { diagram, root, leaves };
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

void Generator::addApproximationFromLeaf(const VoronoiQuadtreeNode& leaf_node, Diagram& diagram, const AbstractSpace& space)
{
    std::vector<Point2> crossings;
    std::vector<std::pair<AbstractSite::Ptr, AbstractSite::Ptr>> crossing_sites;

    for (size_t first_corner_index = 0; first_corner_index < VoronoiQuadtreeNode::corners_count; ++first_corner_index)
    {
        const size_t second_corner_index = (first_corner_index + 1) % VoronoiQuadtreeNode::corners_count;
        const std::optional<ClosestSite>& first_corner = leaf_node.cornerClosestSiteAt(first_corner_index);
        const std::optional<ClosestSite>& second_corner = leaf_node.cornerClosestSiteAt(second_corner_index);

        if (! first_corner.has_value() || ! second_corner.has_value())
        {
            continue;
        }

        const AbstractSite::Ptr& first_corner_site = first_corner->site;
        const AbstractSite::Ptr& second_corner_site = second_corner->site;

        if (first_corner_site == second_corner_site)
        {
            continue;
        }

        crossings.push_back(space.calculateBisectorVertexAlongSegment(
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

std::tuple<VoronoiQuadtreeNode::Ptr, std::vector<VoronoiQuadtreeNode::Ptr>> Generator::build(const Space2& input_space) const
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

        if (node->isTerminal())
        {
        }
        else if (node->level() >= maximum_level_)
        {
            // propagate(*node, leaves);
            // compact(node->parent());
            leaves.push_back(node);
        }
        else
        {
            node->split();

            std::set<AbstractSite::Ptr> all_related_sites = node->allRelatedSites();

            for (auto [child_index, child] : node->children() | std::views::enumerate)
            {
                // propagate(*child, leaves);

                // Assign obvious child V-site, which is the one of the parent at the shared corner
                child->setCornerClosestSite(child_index, node->cornerClosestSiteAt(child_index).value());

                // Assign child I-sites, which is a subset of the parent I-sites
                dispatchInteriorSites(*child, node->interiorSites());

                // Assign remaining children V-sites, which is a subset of the parent I/V/F-sites
                updateCornerClosestSites(*child, all_related_sites, input_space);

                // Assign child F-sites, which is a subset of the parent I/V/F-sites (and requires the child V-sites)
                updateFacesClosestSites(*child, all_related_sites, input_space);

                node_queue.push(child);
            }
        }
    }

    return std::make_tuple(root, leaves);
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
    root->setInteriorSites(sites);
    // TODO: optimize by not copying the vector to a set
    updateCornerClosestSites(*root, std::set<AbstractSite::Ptr>(sites.begin(), sites.end()), input_space);
    return root;
}

bool Generator::isLeaf(const VoronoiQuadtreeNode& node) const
{
    return node.level() >= maximum_level_ || node.isTerminal();
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

                const std::optional<ClosestSite>& node_corner_site = node.cornerClosestSiteAt(node_corner_index);
                const std::optional<ClosestSite>& leaf_corner_site = leaf->cornerClosestSiteAt(leaf_corner_index);

                if (node_corner_site.value_or(ClosestSite{ nullptr, std::numeric_limits<double>::infinity() }).distance
                    < leaf_corner_site.value_or(ClosestSite{ nullptr, std::numeric_limits<double>::infinity() }).distance)
                {
                    leaf->setCornerClosestSite(leaf_corner_index, *node_corner_site);
                }
                else
                {
                    node.setCornerClosestSite(node_corner_index, *leaf_corner_site);
                }
            }
        }
    }
}

void Generator::compact(const std::shared_ptr<VoronoiQuadtreeNode>& parent) const
{
    const bool are_all_children_terminal = std::ranges::all_of(
        parent->children(),
        [](const VoronoiQuadtreeNode::Ptr& child)
        {
            return child->isTerminal();
        });

    if (! are_all_children_terminal)
    {
        return;
    }

    // Intentional no-op in this 2D prototype: pruning removes per-child boundary detail needed for extraction.
}

void Generator::dispatchInteriorSites(VoronoiQuadtreeNode& node, const std::vector<std::shared_ptr<AbstractSite>>& candidate_sites)
{
    for (const AbstractSite::Ptr& interior_site : candidate_sites)
    {
        // Optimize this by distributing the interior sites to the proper child instead (based on position relative to center)
        if (node.containsPoint(interior_site->basePoint()))
        {
            node.addInteriorSite(interior_site);
        }
    }
}

void Generator::updateCornerClosestSites(
    VoronoiQuadtreeNode& node,
    const std::set<std::shared_ptr<AbstractSite>>& candidate_sites,
    const AbstractSpace& input_space)
{
    for (size_t corner_index = 0; corner_index < VoronoiQuadtreeNode::corners_count; ++corner_index)
    {
        std::optional<ClosestSite>& closest_site = node.cornerClosestSiteAt(corner_index);
        if (closest_site.has_value())
        {
            continue;
        }

        closest_site = input_space.findClosestSite(node.cornerAt(corner_index), candidate_sites);
    }
}

void Generator::updateFacesClosestSites(
    VoronoiQuadtreeNode& node,
    std::set<AbstractSite::Ptr>& candidate_sites,
    const AbstractSpace& input_space)
{
    // F-sites can't be the same as existing I/V-sites
    std::set<AbstractSite::Ptr> node_related_sites(node.interiorSites().begin(), node.interiorSites().end());
    for (size_t corner_index = 0; corner_index < VoronoiQuadtreeNode::corners_count; ++corner_index)
    {
        node_related_sites.insert(node.cornerClosestSiteAt(corner_index)->site);
    }

    struct NodeSide
    {
        Segment2 segment;
        std::optional<ClosestSite>* closest_site_start;
        std::optional<ClosestSite>* closest_site_end;
    };

    std::array<NodeSide, VoronoiQuadtreeNode::corners_count> sides;
    for (size_t start_corner_index = 0; start_corner_index < VoronoiQuadtreeNode::corners_count; ++start_corner_index)
    {
        const size_t end_corner_index = (start_corner_index + 1) % VoronoiQuadtreeNode::corners_count;
        sides[start_corner_index].segment = Segment2(node.cornerAt(start_corner_index), node.cornerAt(end_corner_index));
        sides[start_corner_index].closest_site_start = &node.cornerClosestSiteAt(start_corner_index);
        sides[start_corner_index].closest_site_end = &node.cornerClosestSiteAt(end_corner_index);
    }

    for (const AbstractSite::Ptr& site : candidate_sites)
    {
        if (node_related_sites.contains(site))
        {
            continue;
        }

        for (size_t side_index = 0; side_index < VoronoiQuadtreeNode::corners_count; ++side_index)
        {
            const NodeSide& side = sides[side_index];
            double distance_to_side = input_space.closestDistanceToSide(site, side.segment);
            if (distance_to_side < side.closest_site_start->value().distance || distance_to_side < side.closest_site_end->value().distance)
            {
                node.addEdgeSite(FaceSite{ site, side.segment, distance_to_side });
                break;
            }
        }
    }
}

} // namespace yav
