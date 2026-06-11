// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "yav/generator/ClosestSite.h"
#include "yav/geometry/Point2.h"

namespace yav
{

class AbstractSite;

/** One node of the 2D Voronoi quadtree approximation structure. */
class VoronoiQuadtreeNode
    : public std::enable_shared_from_this<VoronoiQuadtreeNode>
{
public:
    using Ptr = std::shared_ptr<VoronoiQuadtreeNode>;

    VoronoiQuadtreeNode(
        const Point2& center,
        double width,
        size_t level,
        const std::shared_ptr<VoronoiQuadtreeNode>& parent);

    bool isLeaf() const;
    bool isTerminal() const;

    size_t level() const;
    double width() const;
    const Point2& center() const;
    const std::shared_ptr<VoronoiQuadtreeNode>& parent() const;

    const std::vector<Ptr>& children() const;
    void split();
    void pruneChildren();

    Point2 cornerAt(size_t corner_index) const;
    Point2 edgeMidpointAt(size_t edge_index) const;

    bool containsPoint(const Point2& point, double tolerance) const;

    const std::array<ClosestSite, 4>& cornerClosestSites() const;
    ClosestSite cornerClosestSiteAt(size_t corner_index) const;
    void setCornerClosestSite(size_t corner_index, const ClosestSite& closest_site);

    const std::vector<std::shared_ptr<AbstractSite>>& candidateSites() const;
    void setCandidateSites(const std::vector<std::shared_ptr<AbstractSite>>& sites);

private:
    static const std::array<Point2, 4> corner_deltas_;

    Point2 center_;
    double width_;
    size_t level_;
    std::shared_ptr<VoronoiQuadtreeNode> parent_;
    std::vector<Ptr> children_;
    std::array<ClosestSite, 4> corner_closest_sites_;
    std::vector<std::shared_ptr<AbstractSite>> candidate_sites_;
};

} // namespace yav
