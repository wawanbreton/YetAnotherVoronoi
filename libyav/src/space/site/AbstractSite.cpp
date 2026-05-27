// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/site/AbstractSite.h"

namespace yav::space::site
{

AbstractSite::AbstractSite(const std::size_t site_id)
    : site_id_(site_id)
{
}

AbstractSite::~AbstractSite() = default;

std::size_t AbstractSite::siteId() const
{
    return site_id_;
}

} // namespace yav::space::site
