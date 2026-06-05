// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/OctreeNode2.h"

#include <spdlog/spdlog.h>

namespace yav::generator
{

OctreeNode2::OctreeNode2() = default;

bool OctreeNode2::isLeaf() const
{
    return ! children_.empty();
}

void OctreeNode2::split()
{
    children_.resize(4);

    for (size_t i = 0; i < children_.size(); ++i)
    {
        auto child = std::make_shared<OctreeNode2>();
        child->setClosestPrimitive(i, corners_closest_primitives_[i]);
        children_[i] = child;
    }
}

bool OctreeNode2::isClosestPrimitiveSet(const size_t position) const
{
    return corners_closest_primitives_[position].has_value();
}

void OctreeNode2::setClosestPrimitive(const size_t position, ClosestPrimitive&& closest_primitive)
{
    corners_closest_primitives_[position] = std::move(closest_primitive);
}

geometry::Point2 OctreeNode2::positionAt(const size_t position) const
{
}

void OctreeNode2::setClosestPrimitive(const size_t position, const std::optional<ClosestPrimitive>& closest_primitive)
{
    corners_closest_primitives_[position] = closest_primitive;
}


} // namespace yav::generator
