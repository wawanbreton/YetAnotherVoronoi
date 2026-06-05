// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>

namespace yav::voronoi
{

class OctreeNode
{
public:
    using Ptr = std::shared_ptr<OctreeNode>;

    enum class XPosition
    {
        Left = 0x00,
        Right = 0x01
    };

    enum class YPosition
    {
        Front = 0x00,
        Rear = 0x02
    };

    enum class ZPosition
    {
        Top = 0x00,
        Bottom = 0x04
    };

    enum class Position : uint8_t
    {
        LeftFrontTop = XPosition::Left | YPosition::Front | ZPosition::Top,
        LeftFrontBottom = XPosition::Left | YPosition::Front | ZPosition::Bottom,
        LeftRearTop = XPosition::Left | YPosition::Rear | ZPosition::Top,
        LeftRearBottom = XPosition::Left | YPosition::Rear| ZPosition::Bottom,
        RightFrontTop = XPosition::Right | YPosition::Front | ZPosition::Top,
        RightFrontBottom = XPosition::Right | YPosition::Front | ZPosition::Bottom,
        RightRearTop = XPosition::Right | YPosition::Rear | ZPosition::Top,
        RightRearBottom = XPosition::Right | YPosition::Rear| ZPosition::Bottom,
    }

    explicit OctreeNode();

private:
    std::optional<std::array<Ptr, 8>> children_;
    std::array<std::optional<uint32_t>> corners_closest_primitives_;
};

} // namespace yav::voronoi
