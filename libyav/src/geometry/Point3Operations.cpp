// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Point3Operations.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/core/access.hpp>

#include <cmath>

namespace yav::geometry
{

Point3 Point3Operations::midpoint(const Point3& first_value, const Point3& second_value)
{
    Point3 output = first_value;
    boost::geometry::add_point(output, second_value);
    boost::geometry::multiply_value(output, 0.5);
    return output;
}

Point3 Point3Operations::centroidFromTriangle(const std::array<Point3, 3>& vertices)
{
    Point3 output = vertices[0];
    boost::geometry::add_point(output, vertices[1]);
    boost::geometry::add_point(output, vertices[2]);
    boost::geometry::multiply_value(output, 1.0 / 3.0);
    return output;
}

double Point3Operations::norm(const Point3& value)
{
    return std::sqrt(boost::geometry::dot_product(value, value));
}

double Point3Operations::meanZFromSegment(const std::array<Point3, 2>& vertices)
{
    return (boost::geometry::get<2>(vertices[0]) + boost::geometry::get<2>(vertices[1])) / 2.0;
}

double Point3Operations::meanZFromTriangle(const std::array<Point3, 3>& vertices)
{
    return (boost::geometry::get<2>(vertices[0]) + boost::geometry::get<2>(vertices[1])
               + boost::geometry::get<2>(vertices[2]))
        / 3.0;
}

} // namespace yav::geometry
