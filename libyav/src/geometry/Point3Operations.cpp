// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Point3Operations.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/core/access.hpp>

#include <cmath>

namespace yav::geometry
{

Point3 Point3Operations::makePoint(const double x_coordinate, const double y_coordinate, const double z_coordinate)
{
    Point3 output;
    boost::geometry::set<0>(output, x_coordinate);
    boost::geometry::set<1>(output, y_coordinate);
    boost::geometry::set<2>(output, z_coordinate);
    return output;
}

Point3 Point3Operations::add(const Point3& first_value, const Point3& second_value)
{
    Point3 output = first_value;
    boost::geometry::add_point(output, second_value);
    return output;
}

Point3 Point3Operations::subtract(const Point3& first_value, const Point3& second_value)
{
    Point3 output = first_value;
    boost::geometry::subtract_point(output, second_value);
    return output;
}

Point3 Point3Operations::scale(const Point3& value, const double factor)
{
    Point3 output = value;
    boost::geometry::multiply_value(output, factor);
    return output;
}

Point3 Point3Operations::midpoint(const Point3& first_value, const Point3& second_value)
{
    Point3 output = first_value;
    boost::geometry::add_point(output, second_value);
    boost::geometry::multiply_value(output, 0.5);
    return output;
}

double Point3Operations::dotProduct(const Point3& first_value, const Point3& second_value)
{
    return boost::geometry::dot_product(first_value, second_value);
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
