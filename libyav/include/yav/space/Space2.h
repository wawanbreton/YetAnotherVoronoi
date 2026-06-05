// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "yav/generator/ClosestPrimitive.h"
#include "yav/geometry/Point2.h"
#include "yav/geometry/Segment2.h"
#include "yav/space/AbstractSpace.h"

namespace yav::space
{

class Primitive;

/** Top-level user input container for Voronoi diagram generation. */
class Space2 : public AbstractSpace
{
public:
    Space2();

    std::shared_ptr<Primitive> addEdge(const geometry::Segment2& segment);
    std::shared_ptr<Primitive> addVertex(const geometry::Point2& vertex);

    yav::generator::ClosestPrimitive findClosestPrimitive(const geometry::Point2& position) const;
};

} // namespace yav::space
