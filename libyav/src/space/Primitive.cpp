// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Primitive.h"

#include <spdlog/spdlog.h>

namespace yav::space
{

Primitive::Primitive() = default;

void Primitive::addSite(const std::shared_ptr<site::AbstractSite>& site)
{
    if (!site)
    {
        spdlog::error("Cannot add a null site to primitive");
        return;
    }

    sites_.push_back(site);
}

const std::vector<std::shared_ptr<site::AbstractSite>>& Primitive::sites() const
{
    return sites_;
}

} // namespace yav::space
