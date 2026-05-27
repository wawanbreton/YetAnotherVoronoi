// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "yav/geometry/Point3.h"

namespace yav::space
{

class Primitive;

/** Top-level user input container for Voronoi diagram generation. */
class Space
{
public:
    Space();

    std::shared_ptr<Primitive> addFace(const std::array<geometry::Point3, 3>& triangle_vertices);
    std::shared_ptr<Primitive> addEdge(const std::array<geometry::Point3, 2>& edge_vertices);
    std::shared_ptr<Primitive> addVertex(const geometry::Point3& vertex_position);

    const std::vector<std::shared_ptr<Primitive>>& primitives() const;

private:
    std::vector<std::shared_ptr<Primitive>> primitives_;
};

} // namespace yav::space
