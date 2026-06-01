// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/voronoi/Diagram.h"

#include <spdlog/spdlog.h>


namespace yav::voronoi
{

Diagram::Diagram() = default;

void Diagram::addCell(const std::shared_ptr<space::Primitive>& primitive, const std::shared_ptr<Cell>& cell)
{
    if (! cells_.insert({ primitive, cell }).second)
    {
        spdlog::warn("Primitive has been inserted already");
    }
}

std::shared_ptr<Cell> Diagram::findCell(const std::shared_ptr<space::Primitive>& primitive) const
{
    const auto iterator = cells_.find(primitive);

    if (iterator == cells_.end())
    {
        return nullptr;
    }

    return iterator->second;
}

const std::unordered_map<std::shared_ptr<space::Primitive>, std::shared_ptr<Cell>>& Diagram::cells() const
{
    return cells_;
}

} // namespace yav::voronoi
