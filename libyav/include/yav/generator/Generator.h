// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
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

    std::tuple<Diagram, std::vector<std::shared_ptr<VoronoiQuadtreeNode>>> generate(const Space2& input_space) const;

private:
    static std::vector<std::shared_ptr<AbstractSite>> uniqueSitesFromCrossings(
        const std::vector<std::pair<std::shared_ptr<AbstractSite>, std::shared_ptr<AbstractSite>>>& edge_site_pairs);

    static void addApproximationFromLeaf(const VoronoiQuadtreeNode& leaf_node, Diagram& diagram, const yav::AbstractSpace& space);

private:
    std::vector<std::shared_ptr<VoronoiQuadtreeNode>> build(const Space2& input_space) const;
    std::shared_ptr<VoronoiQuadtreeNode> initialize(const Space2& input_space) const;
    void update(VoronoiQuadtreeNode& node) const;
    bool isLeaf(const VoronoiQuadtreeNode& node) const;
    void subdivide(VoronoiQuadtreeNode& node) const;
    void propagate(VoronoiQuadtreeNode& node, const std::vector<std::shared_ptr<VoronoiQuadtreeNode>>& leaves) const;
    void compact(const std::shared_ptr<VoronoiQuadtreeNode>& parent) const;

    static constexpr size_t maximum_level_ = 6;
};

} // namespace yav
