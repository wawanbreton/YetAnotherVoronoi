// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Point3Operations.h"

#include <cmath>

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/geometry/arithmetic/dot_product.hpp>
#include <boost/geometry/core/access.hpp>

namespace yav
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

double Point3Operations::meanZFromSegment(const Segment3& segment)
{
    Point3 centroid;
    boost::geometry::centroid(segment, centroid);
    return boost::geometry::get<2>(centroid);
}

double Point3Operations::meanZFromTriangle(const Triangle3& triangle)
{
    Point3 centroid;
    boost::geometry::centroid(triangle, centroid);
    return boost::geometry::get<2>(centroid);
}

} // namespace yav
