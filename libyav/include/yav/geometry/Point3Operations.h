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
    static Point3 makePoint(double x_coordinate, double y_coordinate, double z_coordinate);
    static Point3 add(const Point3& first_value, const Point3& second_value);
    static Point3 subtract(const Point3& first_value, const Point3& second_value);
    static Point3 scale(const Point3& value, double factor);
    static Point3 midpoint(const Point3& first_value, const Point3& second_value);
    static double dotProduct(const Point3& first_value, const Point3& second_value);
    static double norm(const Point3& value);
    static double meanZFromSegment(const std::array<Point3, 2>& vertices);
    static double meanZFromTriangle(const std::array<Point3, 3>& vertices);
};

} // namespace yav::geometry
