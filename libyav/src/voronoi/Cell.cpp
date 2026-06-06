// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/Cell.h"


namespace yav::voronoi
{

Cell::Cell(const std::shared_ptr<space::site::AbstractSite>& site)
    : site_(site)
{
}

const std::shared_ptr<space::site::AbstractSite>& Cell::site() const
{
    return site_;
}

const std::vector<geometry::Segment2>& Cell::boundarySegments() const
{
    return boundary_segments_;
}

void Cell::addBoundarySegment(const geometry::Segment2& segment)
{
    boundary_segments_.push_back(segment);
}

} // namespace yav::voronoi
