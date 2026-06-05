// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"
#include "yav/geometry/Segment3.h"
#include "yav/geometry/Triangle3.h"
#include "yav/space/AbstractSpace.h"

namespace yav::space
{

class Primitive;

/** Top-level user input container for Voronoi diagram generation. */
class Space3 : public AbstractSpace
{
public:
    Space3();

    std::shared_ptr<Primitive> addFace(const geometry::Triangle3& triangle);
    std::shared_ptr<Primitive> addEdge(const geometry::Segment3& segment);
    std::shared_ptr<Primitive> addVertex(const geometry::Point3& vertex);
};

} // namespace yav::space
