// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Edge.h"

namespace yav::space::site
{

Edge::Edge(const std::size_t site_id, const std::array<geometry::Point3, 2>& vertices)
    : AbstractSite(site_id)
    , vertices_(vertices)
{
}

SiteKind Edge::siteKind() const
{
    return SiteKind::Edge;
}

const std::array<geometry::Point3, 2>& Edge::vertices() const
{
    return vertices_;
}

} // namespace yav::space::site
