// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/AbstractSpace.h"

#include <ranges>

#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/algorithms/expand.hpp>

#include "yav/space/site/AbstractSite.h"


namespace bg = boost::geometry;

namespace yav
{

AbstractSpace::AbstractSpace() = default;

const std::vector<std::shared_ptr<AbstractSite>>& AbstractSpace::sites() const
{
    return sites_;
}

bool AbstractSpace::isBisectorFlatWithinRegion(
    const std::shared_ptr<AbstractSite>& site1,
    const std::shared_ptr<AbstractSite>& site2,
    const Box2& region) const
{
    return false;
}

std::shared_ptr<AbstractSite> AbstractSpace::addSite(const std::shared_ptr<AbstractSite>& site)
{
    sites_.push_back(site);
    return site;
}

void AbstractSpace::calculateAutoBoundingBox(const double scale)
{
    auto all_sites_points = sites_
                          | std::views::transform(
                                [](const AbstractSite::Ptr& site)
                                {
                                    return site->definingPoints();
                                })
                          | std::ranges::views::join;

    bool first = true;
    for (const AbstractSite::Ptr& site : sites_)
    {
        for (const Point2& point : site->definingPoints())
        {
            if (first)
            {
                bg::envelope(point, bounding_box_);
                first = false;
            }
            else
            {
                bg::expand(bounding_box_, point);
            }
        }
    }

    if (scale != 1.0)
    {
        const Point2 diagonal = bounding_box_.max_corner() - bounding_box_.min_corner();
        const double span = std::max(diagonal.get<0>(), diagonal.get<1>()) * scale;
        const Point2 center = bg::return_centroid<Point2>(bounding_box_);
        const Point2 new_half_diagonal = Point2(span / 2.0, span / 2.0);
        bg::expand(bounding_box_, center - new_half_diagonal);
        bg::expand(bounding_box_, center + new_half_diagonal);
    }
}

const Box2& AbstractSpace::boundingBox() const
{
    return bounding_box_;
}

void AbstractSpace::setBoundingBox(const Box2& box)
{
    bounding_box_ = box;
}

} // namespace yav
