// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>
#include <memory>
#include <vector>

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

    std::optional<Segment2> calculateSegmentPartClosestToSite(
        const Segment2& segment,
        const std::shared_ptr<AbstractSite>& closest_site_start,
        const std::shared_ptr<AbstractSite>& other_site) const override;

    bool isBisectorFlatWithinRegion(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const Box2& region) const override;

    std::optional<Point2> calculateEquidistantPosition(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const std::shared_ptr<AbstractSite>& site3) const override;

    std::shared_ptr<AbstractSite>
        findClosestSite(const Point2& position, const std::set<std::shared_ptr<AbstractSite>>& candidate_sites) const override;

private:
    std::optional<Point2>
        calculateVerticesBisectorAlongSegment(const Point2& vertex1, const Point2& vertex2, const Segment2& segment) const;

    std::vector<Point2> calculateEdgeVertexBisectorAlongSegment(
        const Point2& vertex_site_position,
        const Segment2& edge_site_segment,
        const Segment2& segment) const;

    std::vector<Point2> calculateEdgeVertexBisectorAlongSegment(
        const Point2& vertex_site_position,
        const Segment2& edge_site_segment,
        const Segment2& segment,
        const PointPositionOnSegment position_on_segment) const;
};

} // namespace yav
