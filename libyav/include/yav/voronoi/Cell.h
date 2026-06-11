// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "yav/geometry/Segment2.h"

namespace yav::space::site
{
class AbstractSite;
}

namespace yav::voronoi
{

/** Voronoi cell approximation for one input site in 2D. */
class Cell
{
public:
    using Ptr = std::shared_ptr<Cell>;

    explicit Cell(const std::shared_ptr<space::site::AbstractSite>& site);

    const std::shared_ptr<space::site::AbstractSite>& site() const;
    const std::vector<geometry::Segment2>& boundarySegments() const;

    void addBoundarySegment(const geometry::Segment2& segment);

private:
    std::shared_ptr<space::site::AbstractSite> site_;
    std::vector<geometry::Segment2> boundary_segments_;
};

} // namespace yav::voronoi
