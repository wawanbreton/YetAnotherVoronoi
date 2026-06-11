// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "yav/generator/ClosestSite.h"
#include "yav/geometry/Point2.h"

namespace yav
{

class QuadtreeNode
{
public:
    using Ptr = std::shared_ptr<QuadtreeNode>;

    enum class XPosition : uint8_t
    {
        Left = 0x00,
        Right = 0x01
    };

    enum class YPosition : uint8_t
    {
        Front = 0x00,
        Rear = 0x02
    };

    enum class Position : uint8_t
    {
        LeftFront = static_cast<uint8_t>(XPosition::Left) | static_cast<uint8_t>(YPosition::Front),
        LeftRear = static_cast<uint8_t>(XPosition::Left) | static_cast<uint8_t>(YPosition::Rear),
        RightFront = static_cast<uint8_t>(XPosition::Right) | static_cast<uint8_t>(YPosition::Front),
        RightRear = static_cast<uint8_t>(XPosition::Right) | static_cast<uint8_t>(YPosition::Rear),
    };

    explicit QuadtreeNode(const Point2& center, const double width);

    bool isLeaf() const;

    void split();

    Point2 positionAt(const size_t position) const;

private:
    static const std::array<Point2, 4> corner_deltas_;
    static const double sqrt2;

    const Point2 center_;
    const double width_;
    std::vector<Ptr> children_;
};

} // namespace yav
