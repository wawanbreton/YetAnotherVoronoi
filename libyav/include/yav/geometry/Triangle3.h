// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry/core/access.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/tags.hpp>
#include <boost/geometry/geometries/triangle.hpp>
#include <fmt/format.h>

#include "yav/geometry/Point3.h"

namespace yav::geometry
{

struct Triangle3
{
    Point3 p1;
    Point3 p2;
    Point3 p3;
};

} // namespace yav::geometry


// Register the Triangle type with Boost.Geometry
namespace boost::geometry::traits
{
template<>
struct tag<yav::geometry::Triangle3>
{
    using type = triangle_tag;
};

template<>
struct point_type<yav::geometry::Triangle3>
{
    using type = yav::geometry::Point3;
};

template<std::size_t Index, std::size_t Dimension>
struct indexed_access<yav::geometry::Triangle3, Index, Dimension>
{
    static inline double get(const yav::geometry::Triangle3& triangle)
    {
        static_assert(Index < 3, "Triangle3 index out of bounds");
        if constexpr (Index == 0)
        {
            return boost::geometry::get<Dimension>(triangle.p1);
        }
        else if constexpr (Index == 1)
        {
            return boost::geometry::get<Dimension>(triangle.p2);
        }
        else
        {
            return boost::geometry::get<Dimension>(triangle.p3);
        }
    }

    static inline void set(yav::geometry::Triangle3& triangle, const double value)
    {
        static_assert(Index < 3, "Triangle3 index out of bounds");
        if constexpr (Index == 0)
        {
            boost::geometry::set<Dimension>(triangle.p1, value);
        }
        else if constexpr (Index == 1)
        {
            boost::geometry::set<Dimension>(triangle.p2, value);
        }
        else
        {
            boost::geometry::set<Dimension>(triangle.p3, value);
        }
    }
};
} // namespace boost::geometry::traits

template<>
struct fmt::formatter<yav::geometry::Triangle3> : formatter<std::string_view>
{
    template<typename FormatContext>
    auto format(const yav::geometry::Triangle3& triangle, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}={}={}", triangle.p1, triangle.p2, triangle.p3);
    }
};
