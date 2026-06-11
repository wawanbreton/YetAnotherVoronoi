// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/Diagram.h"

#include <spdlog/spdlog.h>

#include "yav/voronoi/Cell.h"


namespace yav
{

Diagram::Diagram() = default;

void Diagram::addBoundarySegment(
    const Segment2& segment,
    const std::vector<std::shared_ptr<AbstractSite>>& adjacent_sites)
{
    boundary_segments_.push_back(segment);

    for (const std::shared_ptr<AbstractSite>& adjacent_site : adjacent_sites)
    {
        if (! adjacent_site)
        {
            spdlog::error("Ignoring boundary segment attachment to a null site");
            continue;
        }

        findOrCreateCell(adjacent_site)->addBoundarySegment(segment);
    }
}

const std::vector<Segment2>& Diagram::boundarySegments() const
{
    return boundary_segments_;
}

const std::vector<std::shared_ptr<Cell>>& Diagram::cells() const
{
    return cells_;
}

std::shared_ptr<Cell> Diagram::findOrCreateCell(const std::shared_ptr<AbstractSite>& site)
{
    const auto map_it = cells_by_site_.find(site.get());
    if (map_it != cells_by_site_.end())
    {
        return map_it->second;
    }

    auto cell = std::make_shared<Cell>(site);
    cells_by_site_.insert({ site.get(), cell });
    cells_.push_back(cell);
    return cell;
}

} // namespace yav
