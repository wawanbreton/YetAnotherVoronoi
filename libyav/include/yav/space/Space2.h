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

    double distance(const std::shared_ptr<AbstractSite>& site, const Point2& position) const override;

    std::optional<Point2> calculateBisectorVertexAlongSegment(
        const std::shared_ptr<AbstractSite>& closest_site_start,
        const std::shared_ptr<AbstractSite>& closest_site_end,
        const Segment2& segment) const override;

    bool isBisectorFlatWithinRegion(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const Box2& region) const override;

    Point2 calculateEquidistantPosition(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const std::shared_ptr<AbstractSite>& site3) const override;

    ClosestSite findClosestSite(const Point2& position, const std::set<std::shared_ptr<AbstractSite>>& candidate_sites) const override;

    Segment2 closestSegmentToSide(const std::shared_ptr<AbstractSite>& site, const Segment2& side) const override;
};

} // namespace yav
