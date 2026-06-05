// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Point2.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>


yav::geometry::Point2 operator-(const yav::geometry::Point2& lhs, const yav::geometry::Point2& rhs)
{
    yav::geometry::Point2 result = lhs;
    boost::geometry::subtract_point(result, rhs);
    return result;
}

yav::geometry::Point2 operator+(const yav::geometry::Point2& lhs, const yav::geometry::Point2& rhs)
{
    yav::geometry::Point2 result = lhs;
    boost::geometry::add_point(result, rhs);
    return result;
}

yav::geometry::Point2 operator*(const yav::geometry::Point2& point, const double scale)
{
    yav::geometry::Point2 result = point;
    boost::geometry::multiply_value(result, scale);
    return result;
}
