// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/voronoi/CellPatch.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace yav::voronoi
{

/** Voronoi cell associated with one input primitive. */
class Cell
{
public:
    explicit Cell(std::size_t primitive_id);

    std::size_t primitiveId() const;
    void addPatch(const std::shared_ptr<CellPatch>& patch);
    const std::vector<std::shared_ptr<CellPatch>>& patches() const;

private:
    std::size_t primitive_id_;
    std::vector<std::shared_ptr<CellPatch>> patches_;
};

} // namespace yav::voronoi
