// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Triangle.h"

namespace yav::space::site
{

Triangle::Triangle(const geometry::Triangle3& triangle)
    : triangle_(triangle)
{
}

SiteKind Triangle::siteKind() const
{
    return SiteKind::Triangle;
}

const geometry::Triangle3& Triangle::triangle() const
{
    return triangle_;
}

} // namespace yav::space::site
