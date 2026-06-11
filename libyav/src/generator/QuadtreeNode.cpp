// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/QuadtreeNode.h"

#include <spdlog/spdlog.h>

namespace yav
{

const std::array<Point2, 4> QuadtreeNode::corner_deltas_{ Point2(-1, -1),
                                                          Point2(-1, 1),
                                                          Point2(1, -1),
                                                          Point2(1, 1) };
const double QuadtreeNode::sqrt2{ std::sqrt(2.0) };

QuadtreeNode::QuadtreeNode(const Point2& center, const double width)
    : center_(center)
    , width_(width)
{
}

bool QuadtreeNode::isLeaf() const
{
    return ! children_.empty();
}

void QuadtreeNode::split()
{
    children_.resize(4);

    const double center_distance = sqrt2 * (width_ / 4.0);
    const double child_width = width_ / 2.0;

    for (size_t i = 0; i < children_.size(); ++i)
    {
        children_[i] = std::make_shared<QuadtreeNode>(center_ + corner_deltas_[i] * center_distance, child_width);
    }
}

Point2 QuadtreeNode::positionAt(const size_t position) const
{
    return center_ + corner_deltas_[position] * sqrt2 * (width_ / 2.0);
}


} // namespace yav
