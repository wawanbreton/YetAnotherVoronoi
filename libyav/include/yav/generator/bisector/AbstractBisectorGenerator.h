// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/space/site/AbstractSite.h"
#include "yav/space/site/SiteKind.h"
#include "yav/voronoi/equisurface/AbstractBisector.h"

#include <memory>

namespace yav::generator::bisector
{

/** Polymorphic bisector builder for one pair of site categories. */
class AbstractBisectorGenerator
{
public:
    AbstractBisectorGenerator();
    virtual ~AbstractBisectorGenerator();

    virtual bool canHandle(space::site::SiteKind first_kind, space::site::SiteKind second_kind) const = 0;
    virtual std::shared_ptr<voronoi::equisurface::AbstractBisector> generate(
        const std::shared_ptr<space::site::AbstractSite>& first_site,
        const std::shared_ptr<space::site::AbstractSite>& second_site) const = 0;
};

} // namespace yav::generator::bisector
