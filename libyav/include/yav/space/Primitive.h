// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

#include "yav/geometry/Point2.h"

namespace yav::space
{

namespace site
{
class AbstractSite;
}

/** Input primitive grouping one logical object and all associated sites. */
class Primitive
{
public:
    using Ptr = std::shared_ptr<Primitive>;

    Primitive(const std::vector<std::shared_ptr<site::AbstractSite>>& sites);

    const std::vector<std::shared_ptr<site::AbstractSite>>& sites() const;

    const double distanceTo(const geometry::Point2& point) const;

private:
    const std::vector<std::shared_ptr<site::AbstractSite>> sites_;
};

} // namespace yav::space
