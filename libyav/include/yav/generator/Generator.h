// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace yav
{

namespace voronoi
{
class Diagram;
}

namespace space
{
class Space2;
} // namespace space

namespace generator
{

/** Main analytical Voronoi pipeline orchestrating the 2D quadtree approximation. */
class Generator
{
public:
    Generator();

    voronoi::Diagram generate(const space::Space2& input_space) const;

private:
    static constexpr size_t maximum_quadtree_level_ = 6;
};

} // namespace generator

} // namespace yav
