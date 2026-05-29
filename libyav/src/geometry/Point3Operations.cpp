// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Point3Operations.h"

#include <cmath>

#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/geometry/core/access.hpp>

namespace yav::geometry
{

Point3 Point3Operations::midpoint(const Point3& first_value, const Point3& second_value)
{
    Point3 output = first_value;
    boost::geometry::add_point(output, second_value);
    boost::geometry::multiply_value(output, 0.5);
    return output;
}

double Point3Operations::norm(const Point3& value)
{
    return std::sqrt(boost::geometry::dot_product(value, value));
}

double Point3Operations::meanZFromSegment(const geometry::Segment3& segment)
{
    return (boost::geometry::get<2>(segment.first) + boost::geometry::get<2>(segment.second)) / 2.0;
}

double Point3Operations::meanZFromTriangle(const geometry::Triangle3& triangle)
{
    return (boost::geometry::get<2>(triangle.p1) + boost::geometry::get<2>(triangle.p2) + boost::geometry::get<2>(triangle.p3)) / 3.0;
}

} // namespace yav::geometry
