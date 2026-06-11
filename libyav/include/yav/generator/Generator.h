// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace yav
{

namespace voronoi
{
class Diagram;
}

namespace space
{
namespace site
{
class AbstractSite;
}

class Space2;
} // namespace space

namespace generator
{

class VoronoiQuadtreeNode;

/** Main analytical Voronoi pipeline orchestrating the 2D quadtree approximation. */
class Generator
{
public:
    Generator();

    voronoi::Diagram generate(const space::Space2& input_space) const;

private:
    static std::vector<std::shared_ptr<space::site::AbstractSite>> uniqueSitesFromCrossings(
        const std::vector<std::pair<std::shared_ptr<space::site::AbstractSite>, std::shared_ptr<space::site::AbstractSite>>>&
            edge_site_pairs);

    static void addApproximationFromLeaf(const VoronoiQuadtreeNode& leaf_node, voronoi::Diagram& diagram);

private:
    std::vector<std::shared_ptr<VoronoiQuadtreeNode>> build(const space::Space2& input_space) const;
    std::shared_ptr<VoronoiQuadtreeNode> initialize(const space::Space2& input_space) const;
    void update(VoronoiQuadtreeNode& node) const;
    bool isLeaf(const VoronoiQuadtreeNode& node) const;
    void subdivide(VoronoiQuadtreeNode& node) const;
    void propagate(VoronoiQuadtreeNode& node, const std::vector<std::shared_ptr<VoronoiQuadtreeNode>>& leaves) const;
    void compact(const std::shared_ptr<VoronoiQuadtreeNode>& parent) const;

    static constexpr size_t maximum_level_ = 6;
};

} // namespace generator

} // namespace yav
