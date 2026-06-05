// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Triangle3.h"


namespace yav::space::site
{

Triangle3::Triangle3(const geometry::Triangle3& triangle)
    : triangle_(triangle)
{
}

const geometry::Triangle3& Triangle3::triangle() const
{
    return triangle_;
}

} // namespace yav::space::site
