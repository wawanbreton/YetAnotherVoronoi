// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <fmt/format.h>

#include "yav/geometry/Point2.h"

namespace yav
{

using Segment2 = boost::geometry::model::segment<Point2>;

} // namespace yav

template<>
struct fmt::formatter<yav::Segment2> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::Segment2& point, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}-{}", point.first, point.second);
    }
};