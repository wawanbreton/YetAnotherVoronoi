// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>

#include "yav/geometry/Point2.h"

namespace yav::space::primitive
{

/** Input primitive grouping one logical object and all associated sites. */
class AbstractPrimitive
{
public:
    using Ptr = std::shared_ptr<AbstractPrimitive>;

    AbstractPrimitive();

    virtual double distanceTo(const geometry::Point2& point) const = 0;
};

} // namespace yav::space::primitive
