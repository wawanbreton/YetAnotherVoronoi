// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "yav/geometry/Box2.h"
#include "yav/geometry/Point2.h"

namespace yav
{

/** Abstract geometric site used to evaluate Voronoi distances in 2D. */
class AbstractSite
{
public:
    using Ptr = std::shared_ptr<AbstractSite>;

    AbstractSite();

    virtual double distanceTo(const Point2& point) const = 0;

    /** Returns representative points used for global bounds estimation. */
    virtual std::vector<Point2> definingPoints() const = 0;

    virtual bool intersects(const Box2& box) const = 0;

    virtual Point2 centroid() const = 0;
};

} // namespace yav
