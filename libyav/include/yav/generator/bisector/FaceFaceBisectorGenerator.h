// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/generator/bisector/AbstractBisectorGenerator.h"

namespace yav::generator::bisector
{

/** Generates a plane bisector between two triangle sites. */
class FaceFaceBisectorGenerator final : public AbstractBisectorGenerator
{
public:
    FaceFaceBisectorGenerator();

    bool canHandle(space::site::SiteKind first_kind, space::site::SiteKind second_kind) const override;
    std::shared_ptr<voronoi::equisurface::AbstractBisector> generate(
        const std::shared_ptr<space::site::AbstractSite>& first_site,
        const std::shared_ptr<space::site::AbstractSite>& second_site) const override;
};

} // namespace yav::generator::bisector
