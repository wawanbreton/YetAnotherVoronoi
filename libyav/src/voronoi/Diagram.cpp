// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/Diagram.h"

#include <ranges>

namespace yav::voronoi
{

Diagram::Diagram() = default;

void Diagram::addCell(const std::shared_ptr<Cell>& cell)
{
    if (!cell)
    {
        return;
    }

    cells_.push_back(cell);
}

std::shared_ptr<Cell> Diagram::findCell(const std::size_t primitive_id) const
{
    const auto matching_cell_iterator = std::ranges::find_if(
        cells_,
        [primitive_id](const std::shared_ptr<Cell>& cell)
        {
            return cell && cell->primitiveId() == primitive_id;
        });

    if (matching_cell_iterator == cells_.end())
    {
        return nullptr;
    }

    return *matching_cell_iterator;
}

const std::vector<std::shared_ptr<Cell>>& Diagram::cells() const
{
    return cells_;
}

} // namespace yav::voronoi
