// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <fmt/format.h>

namespace yav
{

using Point3 = boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian>;

} // namespace yav


yav::Point3 operator-(const yav::Point3& lhs, const yav::Point3& rhs);

template<>
struct fmt::formatter<yav::Point3> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::Point3& point, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({},{},{})", point.get<0>(), point.get<1>(), point.get<2>());
    }
};
