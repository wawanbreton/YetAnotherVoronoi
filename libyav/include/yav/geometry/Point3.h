// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <fmt/format.h>

namespace yav::geometry
{

using Point3 = boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian>;

} // namespace yav::geometry


yav::geometry::Point3 operator-(const yav::geometry::Point3& lhs, const yav::geometry::Point3& rhs);

template<>
struct fmt::formatter<yav::geometry::Point3> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::geometry::Point3& point, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({},{},{})", point.get<0>(), point.get<1>(), point.get<2>());
    }
};
