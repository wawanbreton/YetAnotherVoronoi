// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/region/HalfPlaneSlice.h"


namespace yav::generator::region
{

HalfPlaneSlice::HalfPlaneSlice(const geometry::Point3& normal, const double offset)
    : normal_(normal)
    , offset_(offset)
{
}

} // namespace yav::generator::region
