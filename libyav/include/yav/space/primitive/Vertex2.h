// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point2.h"
#include "yav/space/primitive/AbstractPrimitive.h"

namespace yav::space::primitive
{

/** Vertex site represented by a single position in 3D. */
class Vertex2 final : public AbstractPrimitive
{
public:
    explicit Vertex2(const geometry::Point2& position);

    virtual double distanceTo(const geometry::Point2& point) const override;

private:
    geometry::Point2 position_;
};

} // namespace yav::space::primitive
