// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Primitive.h"

#include <spdlog/spdlog.h>

namespace yav::space
{

Primitive::Primitive(const std::vector<std::shared_ptr<site::AbstractSite>>& sites)
    : sites_(sites)
{
}

const std::vector<std::shared_ptr<site::AbstractSite>>& Primitive::sites() const
{
    return sites_;
}

const double Primitive::distanceTo(const geometry::Point2& point) const
{
    return 0;
}

} // namespace yav::space
