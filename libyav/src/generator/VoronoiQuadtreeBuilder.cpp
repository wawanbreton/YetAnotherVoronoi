// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/VoronoiQuadtreeBuilder.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>

#include <spdlog/spdlog.h>

#include "yav/space/Space2.h"
#include "yav/space/site/AbstractSite.h"


namespace yav::generator
{

VoronoiQuadtreeBuilder::VoronoiQuadtreeBuilder(const size_t maximum_level)
    : maximum_level_(maximum_level)
{
}

std::vector<VoronoiQuadtreeNode::Ptr> VoronoiQuadtreeBuilder::build(const space::Space2& input_space) const
{
    auto root = initialize(input_space);
    if (! root)
    {
        return {};
    }

    std::vector<VoronoiQuadtreeNode::Ptr> leaves;
    std::queue<VoronoiQuadtreeNode::Ptr> node_queue;
    node_queue.push(root);

    while (! node_queue.empty())
    {
        const auto node = node_queue.front();
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
        for (const auto& child : node->children())
        {
            propagate(*child, leaves);
            node_queue.push(child);
        }
    }

    return leaves;
}

VoronoiQuadtreeNode::Ptr VoronoiQuadtreeBuilder::initialize(const space::Space2& input_space) const
{
    const auto& sites = input_space.primitives();
    if (sites.empty())
    {
        spdlog::warn("Cannot initialize quadtree on an empty site set");
        return nullptr;
    }

    double min_x = std::numeric_limits<double>::infinity();
    double min_y = std::numeric_limits<double>::infinity();
    double max_x = -std::numeric_limits<double>::infinity();
    double max_y = -std::numeric_limits<double>::infinity();

    for (const auto& site : sites)
    {
        if (! site)
        {
            spdlog::error("Encountered null site while initializing the quadtree");
            continue;
        }

        for (const auto& point : site->definingPoints())
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
    const geometry::Point2 center((min_x + max_x) / 2.0, (min_y + max_y) / 2.0);

    auto root = std::make_shared<VoronoiQuadtreeNode>(center, width, 0, nullptr);
    root->setCandidateSites(sites);
    return root;
}

void VoronoiQuadtreeBuilder::update(VoronoiQuadtreeNode& node) const
{
    for (size_t corner_index = 0; corner_index < node.cornerClosestSites().size(); ++corner_index)
    {
        const geometry::Point2 corner = node.cornerAt(corner_index);
        ClosestSite closest_site{ nullptr, std::numeric_limits<double>::infinity() };

        for (const auto& site : node.candidateSites())
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

bool VoronoiQuadtreeBuilder::isLeaf(const VoronoiQuadtreeNode& node) const
{
    return node.level() >= maximum_level_ || node.isTerminal();
}

void VoronoiQuadtreeBuilder::subdivide(VoronoiQuadtreeNode& node) const
{
    node.split();
}

void VoronoiQuadtreeBuilder::propagate(
    VoronoiQuadtreeNode& node,
    const std::vector<VoronoiQuadtreeNode::Ptr>& leaves) const
{
    constexpr double point_tolerance = 1e-9;

    for (auto& leaf : leaves)
    {
        if (! leaf)
        {
            continue;
        }

        for (size_t node_corner_index = 0; node_corner_index < node.cornerClosestSites().size(); ++node_corner_index)
        {
            const geometry::Point2 node_corner = node.cornerAt(node_corner_index);
            if (! leaf->containsPoint(node_corner, point_tolerance))
            {
                continue;
            }

            for (size_t leaf_corner_index = 0; leaf_corner_index < leaf->cornerClosestSites().size(); ++leaf_corner_index)
            {
                const geometry::Point2 leaf_corner = leaf->cornerAt(leaf_corner_index);
                if (std::abs(node_corner.get<0>() - leaf_corner.get<0>()) > point_tolerance
                    || std::abs(node_corner.get<1>() - leaf_corner.get<1>()) > point_tolerance)
                {
                    continue;
                }

                const auto node_corner_site = node.cornerClosestSiteAt(node_corner_index);
                const auto leaf_corner_site = leaf->cornerClosestSiteAt(leaf_corner_index);

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

void VoronoiQuadtreeBuilder::compact(const std::shared_ptr<VoronoiQuadtreeNode>& parent) const
{
    if (! parent)
    {
        return;
    }

    const bool are_all_children_terminal = std::ranges::all_of(
        parent->children(),
        [](const VoronoiQuadtreeNode::Ptr& child) { return child && child->isTerminal(); });

    if (! are_all_children_terminal)
    {
        return;
    }

    // Intentional no-op in this 2D prototype: pruning removes per-child boundary detail needed for extraction.
}

} // namespace yav::generator
