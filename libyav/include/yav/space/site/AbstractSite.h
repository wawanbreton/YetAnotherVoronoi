// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>

namespace yav::space::site
{

/** Base polymorphic site for geometric primitives participating in Voronoi generation. */
class AbstractSite
{
public:
    using Ptr = std::shared_ptr<AbstractSite>;

    AbstractSite();
    virtual ~AbstractSite();
};

} // namespace yav::space::site
