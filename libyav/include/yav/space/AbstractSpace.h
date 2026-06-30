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

    virtual std::vector<Point2> calculateBisectorVerticesAlongSegment(
        const std::shared_ptr<AbstractSite>& closest_site_start,
        const std::shared_ptr<AbstractSite>& closest_site_end,
        const Segment2& segment,
        const std::shared_ptr<AbstractSite>& edge_site) const
        = 0;

    /*!
     * \brief Calculates the portion of the segment that is closest to the given other_site, knowing that the given closest_site_start is
     *        the closest to the start of the segment
     * \param segment The segment we want to split
     * \param closest_site_start The site that we know is the closest to the segment start
     * \param other_site The other to be tested against the segment
     * \return The portion of the segment that is closest to other_site than to closest_site_start, or nullopt if the whole segment
     *         is closer to closest_site_start
     * \note The returned segment shall be fully contained in the input segment, however it may be in inverted order
     */
    virtual std::optional<Segment2> calculateSegmentPartClosestToSite(
        const Segment2& segment,
        const std::shared_ptr<AbstractSite>& closest_site_start,
        const std::shared_ptr<AbstractSite>& other_site) const
        = 0;

    /*!
     * Since this function is present only to optimize the calculations, the default implementation that returns always false is good enough
     * to work. Properly implementing it may save a lot of calculations though.
     */
    virtual bool isBisectorFlatWithinRegion(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const Box2& region) const;

    virtual std::optional<Point2> calculateEquidistantPosition(
        const std::shared_ptr<AbstractSite>& site1,
        const std::shared_ptr<AbstractSite>& site2,
        const std::shared_ptr<AbstractSite>& site3) const
        = 0;

    virtual std::shared_ptr<AbstractSite>
        findClosestSite(const Point2& position, const std::set<std::shared_ptr<AbstractSite>>& candidate_sites) const = 0;

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
