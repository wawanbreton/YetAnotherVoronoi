// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>

#include "yav/geometry/Point3.h"
#include "yav/geometry/Segment3.h"
#include "yav/geometry/Triangle3.h"

namespace yav::geometry
{

/** Centralized operations for 3D points and vector-like computations. */
class Point3Operations
{
public:
    static Point3 midpoint(const Point3& first_value, const Point3& second_value);
    static double norm(const Point3& value);
    static double meanZFromSegment(const geometry::Segment3& segment);
    static double meanZFromTriangle(const geometry::Triangle3& triangle);
};

} // namespace yav::geometry
