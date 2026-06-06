// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>

#include "yav/geometry/Point2.h"

namespace yav::space::site
{

class AbstractSite
{
public:
    using Ptr = std::shared_ptr<AbstractSite>;

    AbstractSite();

    virtual double distanceTo(const geometry::Point2& point) const = 0;
};

} // namespace yav::space::site
