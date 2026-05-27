// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Primitive.h"

#include <spdlog/spdlog.h>

namespace yav::space
{

Primitive::Primitive(const std::size_t primitive_id)
    : primitive_id_(primitive_id)
{
}

std::size_t Primitive::primitiveId() const
{
    return primitive_id_;
}

void Primitive::addSite(const std::shared_ptr<site::AbstractSite>& site)
{
    if (!site)
    {
        spdlog::error("Cannot add a null site to primitive {}", primitive_id_);
        return;
    }

    sites_.push_back(site);
}

const std::vector<std::shared_ptr<site::AbstractSite>>& Primitive::sites() const
{
    return sites_;
}

} // namespace yav::space
