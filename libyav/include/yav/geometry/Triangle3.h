// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <vector>

#include <boost/geometry/algorithms/append.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/core/exterior_ring.hpp>
#include <boost/geometry/core/interior_rings.hpp>
#include <boost/geometry/core/ring_type.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/geometries/ring.hpp>
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
    using type = polygon_tag;
};

template<>
struct point_type<yav::geometry::Triangle3>
{
    using type = yav::geometry::Point3;
};

template<>
struct ring_const_type<yav::geometry::Triangle3>
{
    using type = boost::geometry::model::ring<yav::geometry::Point3>;
};

template<>
struct ring_mutable_type<yav::geometry::Triangle3>
{
    using type = boost::geometry::model::ring<yav::geometry::Point3>;
};

template<>
struct exterior_ring<yav::geometry::Triangle3>
{
    using ring_type = boost::geometry::model::ring<yav::geometry::Point3>;

    static inline ring_type get(yav::geometry::Triangle3& triangle)
    {
        ring_type ring;
        boost::geometry::append(ring, triangle.p1);
        boost::geometry::append(ring, triangle.p2);
        boost::geometry::append(ring, triangle.p3);
        boost::geometry::append(ring, triangle.p1);
        return ring;
    }

    static inline ring_type get(const yav::geometry::Triangle3& triangle)
    {
        ring_type ring;
        boost::geometry::append(ring, triangle.p1);
        boost::geometry::append(ring, triangle.p2);
        boost::geometry::append(ring, triangle.p3);
        boost::geometry::append(ring, triangle.p1);
        return ring;
    }
};

template<>
struct interior_const_type<yav::geometry::Triangle3>
{
    using type = std::vector<boost::geometry::model::ring<yav::geometry::Point3>>;
};

template<>
struct interior_mutable_type<yav::geometry::Triangle3>
{
    using type = std::vector<boost::geometry::model::ring<yav::geometry::Point3>>;
};

template<>
struct interior_rings<yav::geometry::Triangle3>
{
    using interior_type = std::vector<boost::geometry::model::ring<yav::geometry::Point3>>;

    static inline interior_type get(yav::geometry::Triangle3&)
    {
        return {};
    }

    static inline interior_type get(const yav::geometry::Triangle3&)
    {
        return {};
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
