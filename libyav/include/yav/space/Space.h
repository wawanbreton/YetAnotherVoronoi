// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "yav/geometry/Point3.h"
#include "yav/geometry/Segment3.h"
#include "yav/geometry/Triangle3.h"

namespace yav::space
{

class Primitive;

/** Top-level user input container for Voronoi diagram generation. */
class Space
{
public:
    Space();

    std::shared_ptr<Primitive> addFace(const geometry::Triangle3& triangle);
    std::shared_ptr<Primitive> addEdge(const geometry::Segment3& segment);
    std::shared_ptr<Primitive> addVertex(const geometry::Point3& vertex);

    const std::vector<std::shared_ptr<Primitive>>& primitives() const;

private:
    std::shared_ptr<Primitive> addPrimitive();

    std::vector<std::shared_ptr<Primitive>> primitives_;
};

} // namespace yav::space
