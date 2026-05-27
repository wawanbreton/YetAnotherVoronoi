// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/space/Primitive.h"
#include "yav/voronoi/CellPatch.h"

#include <memory>
#include <vector>

namespace yav::voronoi
{

/** Voronoi cell associated with one input primitive. */
class Cell
{
public:
    explicit Cell(const std::shared_ptr<space::Primitive>& primitive);

    std::shared_ptr<space::Primitive> primitive() const;
    void addPatch(const std::shared_ptr<CellPatch>& patch);
    const std::vector<std::shared_ptr<CellPatch>>& patches() const;

private:
    std::shared_ptr<space::Primitive> primitive_;
    std::vector<std::shared_ptr<CellPatch>> patches_;
};

} // namespace yav::voronoi
