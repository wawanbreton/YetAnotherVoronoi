// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "yav/generator/VoronoiQuadtreeNode.h"

namespace yav::space
{
class Space2;
}

namespace yav::generator
{

/** Breadth-first quadtree construction based on the VO paper strategy in 2D. */
class VoronoiQuadtreeBuilder
{
public:
    explicit VoronoiQuadtreeBuilder(size_t maximum_level);

    std::vector<VoronoiQuadtreeNode::Ptr> build(const space::Space2& input_space) const;

private:
    VoronoiQuadtreeNode::Ptr initialize(const space::Space2& input_space) const;
    void update(VoronoiQuadtreeNode& node) const;
    bool isLeaf(const VoronoiQuadtreeNode& node) const;
    void subdivide(VoronoiQuadtreeNode& node) const;
    void propagate(VoronoiQuadtreeNode& node, const std::vector<VoronoiQuadtreeNode::Ptr>& leaves) const;
    void compact(const std::shared_ptr<VoronoiQuadtreeNode>& parent) const;

    size_t maximum_level_;
};

} // namespace yav::generator
