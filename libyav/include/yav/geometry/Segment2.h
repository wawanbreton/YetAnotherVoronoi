// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <optional>

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <fmt/format.h>

#include "yav/geometry/Point2.h"

namespace yav
{

using Segment2 = boost::geometry::model::segment<Point2>;

enum class PointPositionOnSegment
{
    Before,
    Inside,
    After
};

[[nodiscard]] Segment2 rotate90(const Segment2& segment, const bool ccw);

[[nodiscard]] PointPositionOnSegment projectedPointsLiesOnSegment(const Segment2& segment, const Point2& point);

[[nodiscard]] std::optional<Segment2> intersectSegmentParts(const Segment2& segment, const Segment2& part1, const Segment2& part2);

} // namespace yav

yav::Segment2 operator-(const yav::Segment2& segment);

template<>
struct fmt::formatter<yav::Segment2> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::Segment2& point, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}-{}", point.first, point.second);
    }
};
