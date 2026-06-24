// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Segment2.h"
#include "yav/space/site/AbstractSite.h"

namespace yav
{

/** Segment site represented by two 2D endpoints. */
class Edge2 final : public AbstractSite
{
public:
    explicit Edge2(const Segment2& segment);

    const Segment2& segment() const;

    double distanceTo(const Point2& point) const override;

    std::vector<Point2> definingPoints() const override;

    bool intersects(const Box2& box) const override;

    Point2 centroid() const override;

private:
    Segment2 segment_;
};

} // namespace yav
