// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/Cell.h"


namespace yav
{

Cell::Cell(const std::shared_ptr<AbstractSite>& site)
    : site_(site)
{
}

const std::shared_ptr<AbstractSite>& Cell::site() const
{
    return site_;
}

const std::vector<Segment2>& Cell::boundarySegments() const
{
    return boundary_segments_;
}

void Cell::addBoundarySegment(const Segment2& segment)
{
    boundary_segments_.push_back(segment);
}

} // namespace yav
