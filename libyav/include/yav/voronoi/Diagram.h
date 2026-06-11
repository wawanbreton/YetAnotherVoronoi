// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "yav/geometry/Segment2.h"

namespace yav
{

namespace space::site
{
class AbstractSite;
}

namespace voronoi
{

class Cell;

/** Full Voronoi diagram approximation composed of 2D boundary segments. */
class Diagram
{
public:
    Diagram();

    void addBoundarySegment(
        const geometry::Segment2& segment,
        const std::vector<std::shared_ptr<space::site::AbstractSite>>& adjacent_sites);

    const std::vector<geometry::Segment2>& boundarySegments() const;
    const std::vector<std::shared_ptr<Cell>>& cells() const;

private:
    std::shared_ptr<Cell> findOrCreateCell(const std::shared_ptr<space::site::AbstractSite>& site);

    std::vector<geometry::Segment2> boundary_segments_;
    std::vector<std::shared_ptr<Cell>> cells_;
    std::unordered_map<const space::site::AbstractSite*, std::shared_ptr<Cell>> cells_by_site_;
};

} // namespace voronoi

} // namespace yav
