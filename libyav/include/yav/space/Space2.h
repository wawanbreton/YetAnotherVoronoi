// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <vector>

#include "yav/generator/ClosestSite.h"
#include "yav/geometry/Point2.h"
#include "yav/geometry/Segment2.h"
#include "yav/space/AbstractSpace.h"

namespace yav
{

/** Top-level user input container for Voronoi diagram generation. */
class Space2 : public AbstractSpace
{
public:
    Space2();

    std::shared_ptr<AbstractSite> addEdge(const Segment2& segment);
    std::shared_ptr<AbstractSite> addVertex(const Point2& vertex);

    virtual Point2 calculateBisectorVertexAlongSegment(
        const std::shared_ptr<AbstractSite>& closest_site_start,
        const std::shared_ptr<AbstractSite>& closest_site_end,
        const Segment2& segment) const override;
};

} // namespace yav
