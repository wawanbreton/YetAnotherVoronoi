// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Triangle.h"

namespace yav::space::site
{

Triangle::Triangle(const std::array<geometry::Point3, 3>& vertices)
    : vertices_(vertices)
{
}

SiteKind Triangle::siteKind() const
{
    return SiteKind::Triangle;
}

const std::array<geometry::Point3, 3>& Triangle::vertices() const
{
    return vertices_;
}

} // namespace yav::space::site
