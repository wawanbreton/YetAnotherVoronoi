// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <set>
#include <vector>

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

    virtual Point2 calculateBisectorVertexAlongSegment(
        const std::shared_ptr<AbstractSite>& closest_site_start,
        const std::shared_ptr<AbstractSite>& closest_site_end,
        const Segment2& segment) const
        = 0;

    virtual ClosestSite findClosestSite(const Point2& position, const std::set<std::shared_ptr<AbstractSite>>& candidate_sites) const = 0;

protected:
    std::shared_ptr<AbstractSite> addSite(const std::shared_ptr<AbstractSite>& site);

private:
    std::vector<std::shared_ptr<AbstractSite>> sites_;
};

} // namespace yav
