// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/Edge.h"

namespace yav::space::site
{

Edge::Edge(const geometry::Segment3& segment)
    : segment_(segment)
{
}

SiteKind Edge::siteKind() const
{
    return SiteKind::Edge;
}

const geometry::Segment3& Edge::segment() const
{
    return segment_;
}

} // namespace yav::space::site
