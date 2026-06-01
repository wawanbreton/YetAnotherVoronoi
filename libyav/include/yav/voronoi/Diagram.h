// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <unordered_map>

namespace yav
{

namespace space
{
class Primitive;
}

namespace voronoi
{

class Cell;

/** Full analytical Voronoi diagram with all generated cells and patches. */
class Diagram
{
public:
    Diagram();

    void addCell(const std::shared_ptr<space::Primitive>& primitive, const std::shared_ptr<Cell>& cell);

    std::shared_ptr<Cell> findCell(const std::shared_ptr<space::Primitive>& primitive) const;
    const std::unordered_map<std::shared_ptr<space::Primitive>, std::shared_ptr<Cell>>& cells() const;

private:
    std::unordered_map<std::shared_ptr<space::Primitive>, std::shared_ptr<Cell>> cells_;
};

} // namespace voronoi

} // namespace yav
