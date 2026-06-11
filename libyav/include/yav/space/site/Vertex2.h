// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point2.h"
#include "yav/space/site/AbstractSite.h"

namespace yav
{

/** Vertex site represented by a single position in 2D. */
class Vertex2 final : public AbstractSite
{
public:
    explicit Vertex2(const Point2& position);

    virtual double distanceTo(const Point2& point) const override;
    virtual Point2 basePoint() const override;
    virtual std::vector<Point2> definingPoints() const override;

private:
    Point2 position_;
};

} // namespace yav
