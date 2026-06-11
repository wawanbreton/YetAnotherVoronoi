// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include <array>
#include <unordered_set>

#include <spdlog/spdlog.h>

#include "yav/generator/VoronoiQuadtreeBuilder.h"
#include "yav/generator/VoronoiQuadtreeNode.h"
#include "yav/space/Space2.h"
#include "yav/space/site/AbstractSite.h"
#include "yav/voronoi/Diagram.h"


namespace yav::generator
{

namespace
{

std::vector<std::shared_ptr<space::site::AbstractSite>> uniqueSitesFromCrossings(
    const std::vector<std::pair<std::shared_ptr<space::site::AbstractSite>, std::shared_ptr<space::site::AbstractSite>>>&
        edge_site_pairs)
{
    std::unordered_set<const space::site::AbstractSite*> unique_sites;
    std::vector<std::shared_ptr<space::site::AbstractSite>> result;

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

void addApproximationFromLeaf(const VoronoiQuadtreeNode& leaf_node, voronoi::Diagram& diagram)
{
    static constexpr std::array<std::pair<size_t, size_t>, 4> edge_corners{ {
        { 0, 1 },
        { 1, 3 },
        { 2, 3 },
        { 0, 2 },
    } };

    std::vector<geometry::Point2> crossings;
    std::vector<std::pair<std::shared_ptr<space::site::AbstractSite>, std::shared_ptr<space::site::AbstractSite>>> crossing_sites;

    for (size_t edge_index = 0; edge_index < edge_corners.size(); ++edge_index)
    {
        const auto [first_corner_index, second_corner_index] = edge_corners[edge_index];
        const auto first_corner_site = leaf_node.cornerClosestSiteAt(first_corner_index).site;
        const auto second_corner_site = leaf_node.cornerClosestSiteAt(second_corner_index).site;

        if (! first_corner_site || ! second_corner_site || first_corner_site == second_corner_site)
        {
            continue;
        }

        crossings.push_back(leaf_node.edgeMidpointAt(edge_index));
        crossing_sites.push_back({ first_corner_site, second_corner_site });
    }

    if (crossings.size() == 2)
    {
        diagram.addBoundarySegment(geometry::Segment2(crossings[0], crossings[1]), uniqueSitesFromCrossings(crossing_sites));
        return;
    }

    if (crossings.size() == 4)
    {
        diagram.addBoundarySegment(geometry::Segment2(crossings[0], crossings[1]), uniqueSitesFromCrossings(crossing_sites));
        diagram.addBoundarySegment(geometry::Segment2(crossings[2], crossings[3]), uniqueSitesFromCrossings(crossing_sites));
        return;
    }

    if (! crossings.empty())
    {
        spdlog::debug("Leaf generated an unsupported number of crossings ({})", crossings.size());
    }
}

} // namespace

Generator::Generator() = default;

voronoi::Diagram Generator::generate(const space::Space2& input_space) const
{
    voronoi::Diagram diagram;

    if (input_space.sites().empty())
    {
        spdlog::warn("Requested Voronoi generation on an empty 2D space");
        return diagram;
    }

    VoronoiQuadtreeBuilder quadtree_builder(maximum_quadtree_level_);
    const auto leaves = quadtree_builder.build(input_space);

    for (const auto& leaf : leaves)
    {
        if (! leaf)
        {
            spdlog::error("Encountered null quadtree leaf during diagram extraction");
            continue;
        }

        addApproximationFromLeaf(*leaf, diagram);
    }

    spdlog::info(
        "Generated 2D Voronoi approximation from {} sites and {} quadtree leaves",
        input_space.sites().size(),
        leaves.size());

    return diagram;
}

} // namespace yav::generator
