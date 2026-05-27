// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/space/site/SiteKind.h"

namespace yav::space::site
{

/** Base polymorphic site for geometric primitives participating in Voronoi generation. */
class AbstractSite
{
public:
    AbstractSite();
    virtual ~AbstractSite();

    virtual SiteKind siteKind() const = 0;
};

} // namespace yav::space::site
