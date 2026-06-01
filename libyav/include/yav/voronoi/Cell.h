// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace yav
{

namespace space
{
class Primitive;
}

namespace voronoi
{

class CellPatch;

/** Voronoi cell associated with one input primitive. */
class Cell
{
public:
    using Ptr = std::shared_ptr<Cell>;

    explicit Cell(const std::shared_ptr<space::Primitive>& primitive);

    std::shared_ptr<space::Primitive> primitive() const;

    void addPatch(const std::shared_ptr<CellPatch>& patch);
    const std::vector<std::shared_ptr<CellPatch>>& patches() const;

private:
    std::shared_ptr<space::Primitive> primitive_;
    std::vector<std::shared_ptr<CellPatch>> patches_;
};

} // namespace voronoi

} // namespace yav
