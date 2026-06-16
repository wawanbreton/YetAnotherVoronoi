// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/VoronoiQuadtreeNode.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <ranges>

#include "yav/space/site/AbstractSite.h"


namespace yav
{

const std::array<Point2, VoronoiQuadtreeNode::corners_count> VoronoiQuadtreeNode::corner_deltas_{
    Point2(-1.0, -1.0),
    Point2(1.0, -1.0),
    Point2(1.0, 1.0),
    Point2(-1.0, 1.0),
};

VoronoiQuadtreeNode::VoronoiQuadtreeNode(
    const Point2& center,
    const double width,
    const size_t level,
    const std::shared_ptr<VoronoiQuadtreeNode>& parent)
    : center_(center)
    , width_(width)
    , level_(level)
    , parent_(parent)
{
}

bool VoronoiQuadtreeNode::isLeaf() const
{
    return children_.empty();
}

bool VoronoiQuadtreeNode::isTerminal() const
{
    // TODO: optimize this, we don't need to build the set
    return allRelatedSites().size() <= 1;
}

size_t VoronoiQuadtreeNode::level() const
{
    return level_;
}

double VoronoiQuadtreeNode::width() const
{
    return width_;
}

const Point2& VoronoiQuadtreeNode::center() const
{
    return center_;
}

const std::shared_ptr<VoronoiQuadtreeNode>& VoronoiQuadtreeNode::parent() const
{
    return parent_;
}

const std::array<VoronoiQuadtreeNode::Ptr, VoronoiQuadtreeNode::corners_count>& VoronoiQuadtreeNode::children() const
{
    return children_;
}

void VoronoiQuadtreeNode::split()
{
    const double child_width = width_ / 2.0;
    const double child_center_offset = width_ / 4.0;

    for (size_t i = 0; i < corners_count; ++i)
    {
        const Point2 child_center = center_ + corner_deltas_[i] * child_center_offset;
        children_[i] = std::make_shared<VoronoiQuadtreeNode>(child_center, child_width, level_ + 1, shared_from_this());
    }
}

void VoronoiQuadtreeNode::pruneChildren()
{
    for (Ptr& child : children_)
    {
        child.reset();
    }
}

Point2 VoronoiQuadtreeNode::cornerAt(const size_t corner_index) const
{
    return center_ + corner_deltas_[corner_index] * width_ / 2.0;
}

Point2 VoronoiQuadtreeNode::edgeMidpointAt(const size_t edge_index) const
{
    switch (edge_index)
    {
    case 0:
        return Point2(center_.get<0>(), center_.get<1>() - width_ / 2.0);
    case 1:
        return Point2(center_.get<0>() + width_ / 2.0, center_.get<1>());
    case 2:
        return Point2(center_.get<0>(), center_.get<1>() + width_ / 2.0);
    case 3:
        return Point2(center_.get<0>() - width_ / 2.0, center_.get<1>());
    default:
        return center_;
    }
}

bool VoronoiQuadtreeNode::containsPoint(const Point2& point, const double tolerance) const
{
    const double half_width = width_ / 2.0 + tolerance;
    const double dx = std::abs(point.get<0>() - center_.get<0>());
    const double dy = std::abs(point.get<1>() - center_.get<1>());
    return dx <= half_width && dy <= half_width;
}

const std::array<std::optional<ClosestSite>, VoronoiQuadtreeNode::corners_count>& VoronoiQuadtreeNode::cornerClosestSites() const
{
    return corner_closest_sites_;
}

const std::optional<ClosestSite>& VoronoiQuadtreeNode::cornerClosestSiteAt(const size_t corner_index) const
{
    return corner_closest_sites_.at(corner_index);
}

std::optional<ClosestSite>& VoronoiQuadtreeNode::cornerClosestSiteAt(size_t corner_index)
{
    return corner_closest_sites_.at(corner_index);
}

void VoronoiQuadtreeNode::setCornerClosestSite(const size_t corner_index, const std::optional<ClosestSite>& closest_site)
{
    corner_closest_sites_.at(corner_index) = closest_site;
}

const std::vector<std::shared_ptr<AbstractSite>>& VoronoiQuadtreeNode::interiorSites() const
{
    return interior_sites_;
}

void VoronoiQuadtreeNode::setInteriorSites(const std::vector<std::shared_ptr<AbstractSite>>& sites)
{
    interior_sites_ = sites;
}

void VoronoiQuadtreeNode::addInteriorSite(const std::shared_ptr<AbstractSite>& site)
{
    interior_sites_.push_back(site);
}

void VoronoiQuadtreeNode::addEdgeSites(const std::vector<AbstractSite::Ptr>& sites)
{
    edge_sites_.insert(edge_sites_.end(), sites.begin(), sites.end());
}

std::set<AbstractSite::Ptr> VoronoiQuadtreeNode::allRelatedSites() const
{
    std::set<AbstractSite::Ptr> result;

    for (const std::optional<ClosestSite>& corner_closest_site : corner_closest_sites_)
    {
        if (corner_closest_site.has_value())
        {
            result.insert(corner_closest_site->site);
        }
    }
    result.insert(interior_sites_.begin(), interior_sites_.end());
    result.insert(edge_sites_.begin(), edge_sites_.end());

    return result;
}

} // namespace yav
