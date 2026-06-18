// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <fmt/format.h>

namespace yav
{

using Point2 = boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>;

} // namespace yav


yav::Point2 operator-(const yav::Point2& lhs, const yav::Point2& rhs);

yav::Point2 operator+(const yav::Point2& lhs, const yav::Point2& rhs);

bool operator==(const yav::Point2& lhs, const yav::Point2& rhs);

yav::Point2 operator*(const yav::Point2& point, const double scale);

yav::Point2 operator/(const yav::Point2& point, const double divider);

template<>
struct fmt::formatter<yav::Point2> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::Point2& point, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({},{})", point.get<0>(), point.get<1>());
    }
};
