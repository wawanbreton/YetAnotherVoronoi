// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/voronoi/Cell.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace yav::voronoi
{

/** Full analytical Voronoi diagram with all generated cells and patches. */
class Diagram
{
public:
    Diagram();

    void addCell(const std::shared_ptr<Cell>& cell);
    std::shared_ptr<Cell> findCell(std::size_t primitive_id) const;
    const std::vector<std::shared_ptr<Cell>>& cells() const;

private:
    std::vector<std::shared_ptr<Cell>> cells_;
};

} // namespace yav::voronoi
