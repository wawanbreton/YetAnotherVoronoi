// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Segment2.h"
#include "yav/space/primitive/AbstractPrimitive.h"

namespace yav::space::primitive
{

/** Edge site represented by two 3D endpoints. */
class Edge2 final : public AbstractPrimitive
{
public:
    explicit Edge2(const geometry::Segment2& segment);

    virtual double distanceTo(const geometry::Point2& point) const override;

private:
    geometry::Segment2 segment_;
};

} // namespace yav::space::primitive
