// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "yav/geometry/Point2.h"

namespace yav::space::site
{

/** Abstract geometric site used to evaluate Voronoi distances in 2D. */
class AbstractSite
{
public:
    using Ptr = std::shared_ptr<AbstractSite>;

    AbstractSite();

    virtual double distanceTo(const geometry::Point2& point) const = 0;

    /** Returns a point guaranteed to belong to the site geometry. */
    virtual geometry::Point2 basePoint() const = 0;

    /** Returns representative points used for global bounds estimation. */
    virtual std::vector<geometry::Point2> definingPoints() const = 0;
};

} // namespace yav::space::site
