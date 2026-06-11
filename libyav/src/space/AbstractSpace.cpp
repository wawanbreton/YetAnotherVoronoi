// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/AbstractSpace.h"


namespace yav::space
{

AbstractSpace::AbstractSpace() = default;

const std::vector<std::shared_ptr<site::AbstractSite>>& AbstractSpace::sites() const
{
    return sites_;
}

std::shared_ptr<site::AbstractSite> AbstractSpace::addSite(const std::shared_ptr<site::AbstractSite>& site)
{
    sites_.push_back(site);
    return site;
}

} // namespace yav::space
