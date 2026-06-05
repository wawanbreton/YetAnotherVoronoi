// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <fmt/format.h>

namespace yav::geometry
{

using Point2 = boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>;

} // namespace yav::geometry


yav::geometry::Point2 operator-(const yav::geometry::Point2& lhs, const yav::geometry::Point2& rhs);

template<>
struct fmt::formatter<yav::geometry::Point2> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::geometry::Point2& point, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({},{})", point.get<0>(), point.get<1>());
    }
};
