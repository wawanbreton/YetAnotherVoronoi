// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "yav/generator/ClosestPrimitive.h"
#include "yav/geometry/Point2.h"

namespace yav::generator
{

class OctreeNode2
{
public:
    using Ptr = std::shared_ptr<OctreeNode2>;

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

    explicit OctreeNode2();

    bool isLeaf() const;

    void split();

    bool isClosestPrimitiveSet(const size_t position) const;

    void setClosestPrimitive(const size_t position, ClosestPrimitive&& closest_primitive);

    geometry::Point2 positionAt(const size_t position) const;

private:
    void setClosestPrimitive(const size_t position, const std::optional<ClosestPrimitive>& closest_primitive);

    const geometry::Point2 center_;
    const geometry::Point2 half_width_;
    std::vector<Ptr> children_;
    std::array<std::optional<ClosestPrimitive>, 4> corners_closest_primitives_;
};

} // namespace yav::generator
