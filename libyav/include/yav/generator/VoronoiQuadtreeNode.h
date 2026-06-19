// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <optional>
#include <set>
#include <vector>

#include "yav/generator/ClosestSite.h"
#include "yav/generator/FaceSite.h"
#include "yav/geometry/Box2.h"
#include "yav/geometry/Point2.h"

namespace yav
{

class AbstractSite;

/** One node of the 2D Voronoi quadtree approximation structure. */
class VoronoiQuadtreeNode : public std::enable_shared_from_this<VoronoiQuadtreeNode>
{
public:
    using Ptr = std::shared_ptr<VoronoiQuadtreeNode>;

    static constexpr size_t corners_count{ 4 };

    VoronoiQuadtreeNode(const Box2& region, size_t level, const VoronoiQuadtreeNode::Ptr& parent);

    bool isLeaf() const;
    bool isTerminal() const;

    size_t level() const;
    double width() const;
    Point2 center() const;
    const Box2& region() const;
    const std::shared_ptr<VoronoiQuadtreeNode>& parent() const;

    const std::array<Ptr, corners_count>& children() const;
    void split();
    void pruneChildren();

    Point2 cornerAt(size_t corner_index) const;

    bool containsPoint(const Point2& point) const;

    const std::array<std::optional<ClosestSite>, corners_count>& cornerClosestSites() const;
    const std::optional<ClosestSite>& cornerClosestSiteAt(size_t corner_index) const;
    std::optional<ClosestSite>& cornerClosestSiteAt(size_t corner_index);
    void setCornerClosestSite(size_t corner_index, const std::optional<ClosestSite>& closest_site);

    const std::vector<std::shared_ptr<AbstractSite>>& interiorSites() const;
    void setInteriorSites(const std::vector<std::shared_ptr<AbstractSite>>& sites);
    void addInteriorSite(const std::shared_ptr<AbstractSite>& site);

    const std::vector<FaceSite>& edgeSites() const;
    void addEdgeSite(const FaceSite& site);

    std::set<std::shared_ptr<AbstractSite>> allRelatedSites() const;

    VoronoiQuadtreeNode::Ptr findDeepestChildAt(const Point2& position);

private:
    static const std::array<Point2, corners_count> corner_deltas_;

    Box2 region_;
    size_t level_;
    std::shared_ptr<VoronoiQuadtreeNode> parent_;
    std::array<Ptr, corners_count> children_;
    std::array<std::optional<ClosestSite>, corners_count> corner_closest_sites_; // V-Sites as per the documentation
    std::vector<std::shared_ptr<AbstractSite>> interior_sites_; // I-Sites as per the documentation
    std::vector<FaceSite> edge_sites_; // F-Sites as per the documentation
};

} // namespace yav
