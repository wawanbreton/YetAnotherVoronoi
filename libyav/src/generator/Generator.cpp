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

    if (input_space.boundingBox().min_corner() == input_space.boundingBox().max_corner())
    {
        spdlog::warn(
            "The bounding box of the space has not been set, please use calculateAutoBoundingBox or set the bounding box manually");
        return {};
    }

    Diagram diagram;

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

    const std::vector<AbstractSite::Ptr> unique_sites = uniqueSitesFromCrossings(crossing_sites);

    if (crossings.size() == 2)
    {
        diagram.addBoundarySegment(Segment2(crossings[0], crossings[1]), unique_sites);
        return;
    }

    if (crossings.size() == 3)
    {
        const Point2 equidistant_point = space.calculateEquidistantPosition(unique_sites[0], unique_sites[1], unique_sites[2]);
        diagram.addBoundarySegment(Segment2(equidistant_point, crossings[0]), unique_sites);
        diagram.addBoundarySegment(Segment2(equidistant_point, crossings[1]), unique_sites);
        diagram.addBoundarySegment(Segment2(equidistant_point, crossings[2]), unique_sites);
    }

    // if (crossings.size() == 4)
    // {
    //     diagram.addBoundarySegment(Segment2(crossings[0], crossings[1]), unique_sites);
    //     diagram.addBoundarySegment(Segment2(crossings[2], crossings[3]), unique_sites);
    //     return;
    // }

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
            // The node for sure does not contain a bisector, so we are done with it
            continue;
        }

        if (node->level() >= maximum_level_ || containsFlatBisector(node, input_space))
        {
            leaves.push_back(node);
        }
        else
        {
            node->split();

            const std::set<AbstractSite::Ptr> all_related_sites = node->allRelatedSites();

            for (auto [child_index, child] : node->children() | std::views::enumerate)
            {
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
    const std::set<AbstractSite::Ptr>& candidate_sites,
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
            const Segment2 closest_segment_to_side = input_space.closestSegmentToSide(site, side.segment);
            const double distance_site_to_side = boost::geometry::distance(closest_segment_to_side.first, closest_segment_to_side.second);
            const double distance_corner_start_to_side
                = input_space.distance(side.closest_site_start->value().site, closest_segment_to_side.first);
            const double distance_corner_end_to_side
                = input_space.distance(side.closest_site_end->value().site, closest_segment_to_side.first);
            if (distance_site_to_side < distance_corner_start_to_side || distance_site_to_side < distance_corner_end_to_side)
            {
                node.addEdgeSite(FaceSite{ site, side.segment, distance_site_to_side });
                break;
            }
        }
    }
}

bool Generator::containsFlatBisector(const std::shared_ptr<VoronoiQuadtreeNode>& node, const AbstractSpace& input_space)
{
    if (! node->edgeSites().empty())
    {
        return false;
    }

    constexpr size_t flat_bisector_corners_sites = 2;
    std::set<AbstractSite::Ptr> unique_corner_sites(node->interiorSites().begin(), node->interiorSites().end());
    for (size_t corner_index = 0;
         corner_index < VoronoiQuadtreeNode::corners_count && unique_corner_sites.size() <= flat_bisector_corners_sites;
         ++corner_index)
    {
        unique_corner_sites.insert(node->cornerClosestSiteAt(corner_index)->site);
    }

    if (unique_corner_sites.size() != flat_bisector_corners_sites)
    {
        return false;
    }

    auto adressable_sites = unique_corner_sites | std::ranges::views::all;
    return input_space.isBisectorFlatWithinRegion(adressable_sites.front(), adressable_sites.back(), node->region());
}

} // namespace yav
