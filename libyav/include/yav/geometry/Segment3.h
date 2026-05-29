// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <fmt/format.h>

#include "yav/geometry/Point3.h"

namespace yav::geometry
{

using Segment3 = boost::geometry::model::segment<Point3>;

} // namespace yav::geometry

template<>
struct fmt::formatter<yav::geometry::Segment3> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::geometry::Segment3& point, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}-{}", point.first, point.second);
    }
};