// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include <algorithm>
#include <array>
#include <queue>
#include <ranges>
#include <unordered_set>

#include <boost/geometry/algorithms/distance.hpp>
#include <spdlog/spdlog.h>

#include "yav/generator/VoronoiQuadtreeNode.h"
#include "yav/space/Space2.h"
#include "yav/space/site/AbstractSite.h"
#include "yav/voronoi/Diagram.h"


namespace bg = boost::geometry;

namespace yav
{

Generator::Generator(const size_t maximum_level)
    : maximum_level_(maximum_level)
{
}

std::tuple<Diagram::Ptr, VoronoiQuadtreeNode::Ptr, std::vector<VoronoiQuadtreeNode::Ptr>>
    Generator::generate(const Space2& input_space) const
{
    if (input_space.sites().empty())
    {
        spdlog::warn("Requested Voronoi generation on an empty 2D space");
        return {};
    }

    if (input_space.boundingBox().min_corner() == input_space.boundingBox().max_corner())
    {
        spdlog::warn(
            "The bounding box of the space has not been set, please use calculateAutoBoundingBox or set the bounding box manually");
        return {};
    }

    auto diagram = std::make_shared<Diagram>();

    const auto [root, leaves] = build(input_space);

    spdlog::info("Built tree from {} sites and {} quadtree leaves", input_space.sites().size(), leaves.size());

    for (const VoronoiQuadtreeNode::Ptr& leaf : leaves)
    {
        addApproximationFromLeaf(*leaf, diagram, input_space);
    }

    spdlog::info("Generated Voronoi approximation");

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

void Generator::addApproximationFromLeaf(const VoronoiQuadtreeNode& leaf_node, const Diagram::Ptr& diagram, const AbstractSpace& space)
{
    std::vector<Point2> crossings;
    std::vector<std::pair<AbstractSite::Ptr, AbstractSite::Ptr>> crossing_sites;

    std::optional<FaceSite> edge_site;
    if (! leaf_node.edgeSites().empty())
    {
        edge_site = leaf_node.edgeSites().front();
    }

    for (size_t first_corner_index = 0; first_corner_index < VoronoiQuadtreeNode::corners_count; ++first_corner_index)
    {
        const size_t second_corner_index = (first_corner_index + 1) % VoronoiQuadtreeNode::corners_count;
        const AbstractSite::Ptr& first_corner_site = leaf_node.cornerClosestSiteAt(first_corner_index);
        const AbstractSite::Ptr& second_corner_site = leaf_node.cornerClosestSiteAt(second_corner_index);
        const Segment2 edge(leaf_node.cornerAt(first_corner_index), leaf_node.cornerAt(second_corner_index));

        if (edge_site.has_value() && edge_site->face_index == first_corner_index)
        {
            crossings.push_back(edge_site->closest_segment_part.first);
            crossings.push_back(edge_site->closest_segment_part.second);
            crossing_sites.push_back({ first_corner_site, edge_site->site });
        }
        else if (first_corner_site != second_corner_site)
        {
            std::optional<Point2> bisector_intersection
                = space.calculateBisectorVertexAlongSegment(first_corner_site, second_corner_site, edge);
            if (bisector_intersection.has_value())
            {
                crossings.push_back(*bisector_intersection);
                crossing_sites.push_back({ first_corner_site, second_corner_site });
            }
        }
    }

    const std::vector<AbstractSite::Ptr> unique_sites = uniqueSitesFromCrossings(crossing_sites);

    if (crossings.size() == 2)
    {
        diagram->addBoundarySegment(Segment2(crossings[0], crossings[1]), unique_sites);
    }
    else if (crossings.size() == 3)
    {
        const Point2 equidistant_point = space.calculateEquidistantPosition(unique_sites[0], unique_sites[1], unique_sites[2]);
        diagram->addBoundarySegment(Segment2(equidistant_point, crossings[0]), unique_sites);
        diagram->addBoundarySegment(Segment2(equidistant_point, crossings[1]), unique_sites);
        diagram->addBoundarySegment(Segment2(equidistant_point, crossings[2]), unique_sites);
    }

    else if (! crossings.empty())
    {
        spdlog::debug("Leaf generated an unsupported number of crossings ({})", crossings.size());
    }
}

bool Generator::isEmpty(const VoronoiQuadtreeNode& node)
{
    return node.allRelatedSites().size() == 1;
}

Generator::NodeState Generator::calculateNodeState(const VoronoiQuadtreeNode& node, const AbstractSpace& space) const
{
    std::set<AbstractSite::Ptr> unique_corner_sites = node.uniqueCornerClosestSites();
    const std::vector<FaceSite>& edge_sites = node.edgeSites();
    const std::vector<AbstractSite::Ptr>& interior_sites = node.interiorSites();

    // Interior sites are allowed as long as they also are corner closest sites
    for (const AbstractSite::Ptr& interior_site : interior_sites)
    {
        if (! unique_corner_sites.contains(interior_site))
        {
            return NodeState::Undefined;
        }
    }

    const size_t total_exterior_sites = unique_corner_sites.size() + edge_sites.size();
    if (total_exterior_sites <= 1)
    {
        return NodeState::Terminal;
    }

    if (total_exterior_sites == 2)
    {
        return NodeState::Approximateable;
    }

    if (total_exterior_sites == 3)
    {
        if (edge_sites.empty() || edge_sites.size() == 1)
        {
            std::vector<AbstractSite::Ptr> unique_sites_vector;
            unique_sites_vector.reserve(total_exterior_sites);
            unique_sites_vector.insert(unique_sites_vector.end(), unique_corner_sites.begin(), unique_corner_sites.end());
            if (edge_sites.size() == 1)
            {
                unique_sites_vector.push_back(edge_sites.front().site);
            }

            const Point2 equidistant_point
                = space.calculateEquidistantPosition(unique_sites_vector[0], unique_sites_vector[1], unique_sites_vector[2]);
            if (node.containsPoint(equidistant_point)
                && std::ranges::all_of(
                    std::views::cartesian_product(unique_sites_vector, unique_sites_vector),
                    [&node, &space](const std::pair<AbstractSite::Ptr, AbstractSite::Ptr>& sites)
                    {
                        return sites.first == sites.second || space.isBisectorFlatWithinRegion(sites.first, sites.second, node.region());
                    }))
            {
                return NodeState::Approximateable;
            }
        }
    }

    return NodeState::Undefined;
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

        const NodeState node_state = calculateNodeState(*node, input_space);

        switch (node_state)
        {
        case NodeState::Terminal:
            // The node for sure does not contain a bisector, so we are done with it
            break;

        case NodeState::Approximateable:
            // Node is defined enough for approximation, mark it as leaf to be approximated
            leaves.push_back(node);
            break;

        case NodeState::Undefined:
            // This node has too many related sites, split it to finely define it
            node->split();

            const std::set<AbstractSite::Ptr> all_related_sites = node->allRelatedSites();

            for (auto [child_index, child] : node->children() | std::views::enumerate)
            {
                // Assign obvious child V-site, which is the one of the parent at the shared corner
                child->setCornerClosestSite(child_index, node->cornerClosestSiteAt(child_index));

                // Assign child I-sites, which is a subset of the parent I-sites
                dispatchInteriorSites(*child, node->interiorSites());

                // Assign remaining children V-sites, which is a subset of the parent I/V/F-sites
                updateCornerClosestSites(*child, all_related_sites, input_space, child_index);

                // Assign child F-sites, which is a subset of the parent I/V/F-sites (and requires the child V-sites)
                updateFacesClosestSites(*child, all_related_sites, input_space);

                node_queue.push(child);
            }

            break;
        }
    }

    return std::make_tuple(root, leaves);
}

VoronoiQuadtreeNode::Ptr Generator::initialize(const Space2& input_space) const
{
    const std::vector<std::shared_ptr<AbstractSite>>& sites = input_space.sites();

    auto root = std::make_shared<VoronoiQuadtreeNode>(input_space.boundingBox(), 0, nullptr);

    root->setInteriorSites(sites);

    // TODO: optimize by not copying the vector to a set
    updateCornerClosestSites(*root, std::set<AbstractSite::Ptr>(sites.begin(), sites.end()), input_space);

    return root;
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
    const AbstractSpace& input_space,
    const std::optional<size_t>& ignore_corner_index)
{
    for (size_t corner_index = 0; corner_index < VoronoiQuadtreeNode::corners_count; ++corner_index)
    {
        if (ignore_corner_index.has_value() && corner_index == ignore_corner_index.value())
        {
            continue;
        }

        node.setCornerClosestSite(corner_index, input_space.findClosestSite(node.cornerAt(corner_index), candidate_sites));
    }
}

void Generator::updateFacesClosestSites(
    VoronoiQuadtreeNode& node,
    const std::set<AbstractSite::Ptr>& candidate_sites,
    const AbstractSpace& input_space)
{
    // F-sites can't be the same as existing I/V-sites
    std::set<AbstractSite::Ptr> node_related_sites(node.interiorSites().begin(), node.interiorSites().end());
    node_related_sites.insert(node.cornerClosestSites().begin(), node.cornerClosestSites().end());

    struct NodeSide
    {
        Segment2 segment;
        double segment_length;
        size_t index;
        AbstractSite::Ptr closest_site_start;
        AbstractSite::Ptr closest_site_end;
    };

    std::vector<NodeSide> sides;
    sides.reserve(VoronoiQuadtreeNode::corners_count);
    for (size_t start_corner_index = 0; start_corner_index < VoronoiQuadtreeNode::corners_count; ++start_corner_index)
    {
        const size_t end_corner_index = (start_corner_index + 1) % VoronoiQuadtreeNode::corners_count;
        const AbstractSite::Ptr& closest_site_start = node.cornerClosestSiteAt(start_corner_index);
        const AbstractSite::Ptr& closest_site_end = node.cornerClosestSiteAt(end_corner_index);
        if (closest_site_start != closest_site_end)
        {
            NodeSide side;
            side.segment = Segment2(node.cornerAt(start_corner_index), node.cornerAt(end_corner_index));
            side.segment_length = bg::distance(side.segment.first, side.segment.second);
            side.index = start_corner_index;
            side.closest_site_start = closest_site_start;
            side.closest_site_end = closest_site_end;
            sides.push_back(std::move(side));
        }
    }

    if (sides.empty())
    {
        return;
    }

    for (const AbstractSite::Ptr& site : candidate_sites)
    {
        if (node_related_sites.contains(site))
        {
            continue;
        }

        for (const NodeSide& side : sides)
        {
            std::optional<Point2> equidistant_corner_start
                = input_space.calculateBisectorVertexAlongSegment(side.closest_site_start, site, side.segment);
            if (! equidistant_corner_start.has_value())
            {
                continue;
            }

            std::optional<Point2> equidistant_corner_end
                = input_space.calculateBisectorVertexAlongSegment(side.closest_site_end, site, side.segment);
            if (! equidistant_corner_end.has_value())
            {
                continue;
            }

            double covered_distance_start = bg::distance(side.segment.first, *equidistant_corner_start);
            double covered_distance_end = bg::distance(side.segment.second, *equidistant_corner_end);
            if ((covered_distance_start + covered_distance_end) < side.segment_length)
            {
                node.addEdgeSite(FaceSite{ site,
                                           static_cast<size_t>(side.index),
                                           side.segment,
                                           Segment2(*equidistant_corner_start, *equidistant_corner_end) });
                break;
            }
        }
    }
}

bool Generator::containsFlatBisector(const VoronoiQuadtreeNode& node, const AbstractSpace& input_space)
{
    if (! node.edgeSites().empty())
    {
        return false;
    }

    constexpr size_t flat_bisector_corners_sites = 2;
    std::set<AbstractSite::Ptr> unique_corner_sites(node.interiorSites().begin(), node.interiorSites().end());
    unique_corner_sites.insert(node.cornerClosestSites().begin(), node.cornerClosestSites().end());

    if (unique_corner_sites.size() != flat_bisector_corners_sites)
    {
        return false;
    }

    auto adressable_sites = unique_corner_sites | std::ranges::views::all;
    return input_space.isBisectorFlatWithinRegion(adressable_sites.front(), adressable_sites.back(), node.region());
}

} // namespace yav
