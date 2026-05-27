// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <boost/geometry.hpp>

namespace yav::geometry
{

/** 3D Cartesian point used by input and output geometry entities. */
using Point3 = boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian>;

} // namespace yav::geometry
