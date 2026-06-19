// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/VoronoiQuadtreeNode.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <ranges>

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/intersects.hpp>

#include "yav/space/site/AbstractSite.h"


namespace bg = boost::geometry;

namespace yav
{

const std::array<Point2, VoronoiQuadtreeNode::corners_count> VoronoiQuadtreeNode::corner_deltas_{
    Point2(-1.0, -1.0),
    Point2(1.0, -1.0),
    Point2(1.0, 1.0),
    Point2(-1.0, 1.0),
};

VoronoiQuadtreeNode::VoronoiQuadtreeNode(const Box2& region, const size_t level, const std::shared_ptr<VoronoiQuadtreeNode>& parent)
    : region_(region)
    , level_(level)
    , parent_(parent)
{
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
    return region_.max_corner().x() - region_.min_corner().x();
}

Point2 VoronoiQuadtreeNode::center() const
{
    return bg::return_centroid<Point2>(region_);
}

const Box2& VoronoiQuadtreeNode::region() const
{
    return region_;
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
    const Point2 node_center = center();

    for (size_t corner_index = 0; corner_index < corners_count; ++corner_index)
    {
        const Box2 child_region = bg::return_envelope<Box2>(Segment2(node_center, cornerAt(corner_index)));
        children_[corner_index] = std::make_shared<VoronoiQuadtreeNode>(child_region, level_ + 1, shared_from_this());
    }
}

Point2 VoronoiQuadtreeNode::cornerAt(const size_t corner_index) const
{
    return center() + corner_deltas_[corner_index] * width() / 2.0;
}

bool VoronoiQuadtreeNode::containsPoint(const Point2& point) const
{
    return bg::intersects(region_, point);
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

const std::vector<FaceSite>& VoronoiQuadtreeNode::edgeSites() const
{
    return edge_sites_;
}

void VoronoiQuadtreeNode::addEdgeSite(const FaceSite& site)
{
    edge_sites_.push_back(site);
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

    for (const FaceSite& face_site : edge_sites_)
    {
        result.insert(face_site.site);
    }

    return result;
}

VoronoiQuadtreeNode::Ptr VoronoiQuadtreeNode::findDeepestChildAt(const Point2& position)
{
    for (const VoronoiQuadtreeNode::Ptr& child : children_)
    {
        if (child && child->containsPoint(position))
        {
            return child->findDeepestChildAt(position);
        }
    }

    return shared_from_this();
}

} // namespace yav
