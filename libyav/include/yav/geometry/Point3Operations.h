// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"

#include <array>

namespace yav::geometry
{

/** Centralized operations for 3D points and vector-like computations. */
class Point3Operations
{
public:
    static Point3 midpoint(const Point3& first_value, const Point3& second_value);
    static Point3 centroidFromTriangle(const std::array<Point3, 3>& vertices);
    static double norm(const Point3& value);
    static double meanZFromSegment(const std::array<Point3, 2>& vertices);
    static double meanZFromTriangle(const std::array<Point3, 3>& vertices);
};

} // namespace yav::geometry
