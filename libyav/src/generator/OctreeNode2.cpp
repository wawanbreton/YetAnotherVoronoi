// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/OctreeNode2.h"

#include <spdlog/spdlog.h>

namespace yav::generator
{

const std::array<geometry::Point2, 4> OctreeNode2::corner_deltas_{ geometry::Point2(-1, -1),
                                                                   geometry::Point2(-1, 1),
                                                                   geometry::Point2(1, -1),
                                                                   geometry::Point2(1, 1) };
const double OctreeNode2::sqrt2{ std::sqrt(2.0) };

OctreeNode2::OctreeNode2(const geometry::Point2& center, const double width)
    : center_(center)
    , width_(width)
{
}

bool OctreeNode2::isLeaf() const
{
    return ! children_.empty();
}

void OctreeNode2::split()
{
    children_.resize(4);

    const double center_distance = sqrt2 * (width_ / 4.0);
    const double child_width = width_ / 2.0;

    for (size_t i = 0; i < children_.size(); ++i)
    {
        auto child = std::make_shared<OctreeNode2>(center_ + corner_deltas_[i] * center_distance, child_width);
        child->setClosestPrimitive(i, corners_closest_primitives_[i]);
        children_[i] = child;
    }
}

const std::optional<ClosestPrimitive>& OctreeNode2::getClosestPrimitive(const size_t position) const
{
    return corners_closest_primitives_[position];
}

geometry::Point2 OctreeNode2::positionAt(const size_t position) const
{
    return center_ + corner_deltas_[position] * sqrt2 * (width_ / 2.0);
}

void OctreeNode2::setClosestPrimitive(const size_t position, const std::optional<ClosestPrimitive>& closest_primitive)
{
    corners_closest_primitives_[position] = closest_primitive;
}


} // namespace yav::generator
