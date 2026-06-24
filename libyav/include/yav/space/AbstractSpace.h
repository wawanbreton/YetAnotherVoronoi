// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <optional>
#include <set>
#include <vector>

#include "yav/geometry/Box2.h"
#include "yav/geometry/Point2.h"
#include "yav/geometry/Segment2.h"

namespace yav
{

class AbstractSite;
struct ClosestSite;

/** Top-level user input container for Voronoi diagram generation. */
class AbstractSpace
{
public:
    AbstractSpace();

    const std::vector<std::shared_ptr<AbstractSite>>& sites() const;

    virtual double distance(const std::shared_ptr<AbstractSite>& site, const Point2& position) const = 0;

    virtual std::optional<Point2> calculateBisectorVertexAlongSegment(
        const std::shared_ptr<AbstractSite>& closest_site_start,
        const std::shared_ptr<AbstractSite>& closest_site_end,
        const Segment2& segment) const
        = 0;

    /*!
     * Since this function is present only to optimize the calculations, the default implementation that returns always false is good enough
     * to work. Properly implementing it may save a lot of calculations though.
     */
    virtual bool isBisectorFlatWithinRegion(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const Box2& region) const;

    virtual Point2 calculateEquidistantPosition(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const std::shared_ptr<AbstractSite>& site3) const
        = 0;

    virtual std::shared_ptr<AbstractSite>
        findClosestSite(const Point2& position, const std::set<std::shared_ptr<AbstractSite>>& candidate_sites) const = 0;

    virtual Segment2 closestSegmentToSide(const std::shared_ptr<AbstractSite>& site, const Segment2& side) const = 0;

    void calculateAutoBoundingBox(const double scale = 1.2);
    const Box2& boundingBox() const;
    void setBoundingBox(const Box2& box);

protected:
    std::shared_ptr<AbstractSite> addSite(const std::shared_ptr<AbstractSite>& site);

private:
    std::vector<std::shared_ptr<AbstractSite>> sites_;
    Box2 bounding_box_;
};

} // namespace yav
