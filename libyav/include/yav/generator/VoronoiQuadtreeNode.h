// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "yav/generator/ClosestSite.h"
#include "yav/geometry/Point2.h"

namespace yav::generator
{

/** One node of the 2D Voronoi quadtree approximation structure. */
class VoronoiQuadtreeNode
    : public std::enable_shared_from_this<VoronoiQuadtreeNode>
{
public:
    using Ptr = std::shared_ptr<VoronoiQuadtreeNode>;

    VoronoiQuadtreeNode(
        const geometry::Point2& center,
        double width,
        size_t level,
        const std::shared_ptr<VoronoiQuadtreeNode>& parent);

    bool isLeaf() const;
    bool isTerminal() const;

    size_t level() const;
    double width() const;
    const geometry::Point2& center() const;
    const std::shared_ptr<VoronoiQuadtreeNode>& parent() const;

    const std::vector<Ptr>& children() const;
    void split();
    void pruneChildren();

    geometry::Point2 cornerAt(size_t corner_index) const;
    geometry::Point2 edgeMidpointAt(size_t edge_index) const;

    bool containsPoint(const geometry::Point2& point, double tolerance) const;

    const std::array<ClosestSite, 4>& cornerClosestSites() const;
    ClosestSite cornerClosestSiteAt(size_t corner_index) const;
    void setCornerClosestSite(size_t corner_index, const ClosestSite& closest_site);

    const std::vector<std::shared_ptr<space::site::AbstractSite>>& candidateSites() const;
    void setCandidateSites(const std::vector<std::shared_ptr<space::site::AbstractSite>>& sites);

private:
    static const std::array<geometry::Point2, 4> corner_deltas_;

    geometry::Point2 center_;
    double width_;
    size_t level_;
    std::shared_ptr<VoronoiQuadtreeNode> parent_;
    std::vector<Ptr> children_;
    std::array<ClosestSite, 4> corner_closest_sites_;
    std::vector<std::shared_ptr<space::site::AbstractSite>> candidate_sites_;
};

} // namespace yav::generator
