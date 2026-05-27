// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/space/site/SiteKind.h"

#include <cstddef>

namespace yav::space::site
{

/** Base polymorphic site for geometric primitives participating in Voronoi generation. */
class AbstractSite
{
public:
    explicit AbstractSite(std::size_t site_id);
    virtual ~AbstractSite();

    std::size_t siteId() const;
    virtual SiteKind siteKind() const = 0;

private:
    std::size_t site_id_;
};

} // namespace yav::space::site
