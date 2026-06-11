// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/geometry/Point3.h"

#include <boost/geometry/arithmetic/arithmetic.hpp>


yav::Point3 operator-(const yav::Point3& lhs, const yav::Point3& rhs)
{
    yav::Point3 result = lhs;
    boost::geometry::subtract_point(result, rhs);
    return result;
}
