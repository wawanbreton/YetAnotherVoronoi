// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "yav/geometry/Segment2.h"

namespace yav
{

class AbstractSite;

/** Voronoi cell approximation for one input site in 2D. */
class Cell
{
public:
    using Ptr = std::shared_ptr<Cell>;

    explicit Cell(const std::shared_ptr<AbstractSite>& site);

    const std::shared_ptr<AbstractSite>& site() const;
    const std::vector<Segment2>& boundarySegments() const;

    void addBoundarySegment(const Segment2& segment);

private:
    std::shared_ptr<AbstractSite> site_;
    std::vector<Segment2> boundary_segments_;
};

} // namespace yav
