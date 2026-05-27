// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/Cell.h"

#include <spdlog/spdlog.h>

namespace yav::voronoi
{

Cell::Cell(const std::size_t primitive_id)
    : primitive_id_(primitive_id)
{
}

std::size_t Cell::primitiveId() const
{
    return primitive_id_;
}

void Cell::addPatch(const std::shared_ptr<CellPatch>& patch)
{
    if (!patch)
    {
        spdlog::error("Cannot add a null patch to cell {}", primitive_id_);
        return;
    }

    patches_.push_back(patch);
}

const std::vector<std::shared_ptr<CellPatch>>& Cell::patches() const
{
    return patches_;
}

} // namespace yav::voronoi
