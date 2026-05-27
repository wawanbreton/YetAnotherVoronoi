// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/CellPatch.h"

#include <spdlog/spdlog.h>

namespace yav::voronoi
{

CellPatch::CellPatch() = default;

void CellPatch::setBisector(const std::shared_ptr<equisurface::AbstractBisector>& bisector)
{
    if (!bisector)
    {
        spdlog::error("Cannot assign a null bisector to patch");
        return;
    }

    bisector_ = bisector;
}

std::shared_ptr<equisurface::AbstractBisector> CellPatch::bisector() const
{
    return bisector_;
}

void CellPatch::addConstraint(const std::string& constraint_expression)
{
    constraints_.push_back(constraint_expression);
}

const std::vector<std::string>& CellPatch::constraints() const
{
    return constraints_;
}

void CellPatch::addAdjacentPatch(const std::shared_ptr<CellPatch>& patch)
{
    if (!patch)
    {
        spdlog::error("Cannot add a null adjacent patch");
        return;
    }

    adjacent_patches_.push_back(patch);
}

const std::vector<std::weak_ptr<CellPatch>>& CellPatch::adjacentPatches() const
{
    return adjacent_patches_;
}

} // namespace yav::voronoi
