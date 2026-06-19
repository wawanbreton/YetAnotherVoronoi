// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <set>
#include <vector>

namespace yav
{

class Diagram;
class AbstractSite;
class AbstractSpace;
class Space2;
class VoronoiQuadtreeNode;

/** Main analytical Voronoi pipeline orchestrating the 2D quadtree approximation. */
class Generator
{
public:
    Generator();

    std::tuple<Diagram, std::shared_ptr<VoronoiQuadtreeNode>, std::vector<std::shared_ptr<VoronoiQuadtreeNode>>>
        generate(const Space2& input_space) const;

private:
    static std::vector<std::shared_ptr<AbstractSite>> uniqueSitesFromCrossings(
        const std::vector<std::pair<std::shared_ptr<AbstractSite>, std::shared_ptr<AbstractSite>>>& edge_site_pairs);

    static void addApproximationFromLeaf(const VoronoiQuadtreeNode& leaf_node, Diagram& diagram, const yav::AbstractSpace& space);

private:
    std::tuple<std::shared_ptr<VoronoiQuadtreeNode>, std::vector<std::shared_ptr<VoronoiQuadtreeNode>>>
        build(const Space2& input_space) const;

    std::shared_ptr<VoronoiQuadtreeNode> initialize(const Space2& input_space) const;

    static void dispatchInteriorSites(VoronoiQuadtreeNode& node, const std::vector<std::shared_ptr<AbstractSite>>& candidate_sites);

    static void updateCornerClosestSites(
        VoronoiQuadtreeNode& node,
        const std::set<std::shared_ptr<AbstractSite>>& candidate_sites,
        const AbstractSpace& input_space);

    static void updateFacesClosestSites(
        VoronoiQuadtreeNode& node,
        const std::set<std::shared_ptr<AbstractSite>>& candidate_sites,
        const AbstractSpace& input_space);

    static bool containsFlatBisector(const std::shared_ptr<VoronoiQuadtreeNode>& node, const AbstractSpace& input_space);

    static constexpr size_t maximum_level_ = 6;
};

} // namespace yav
