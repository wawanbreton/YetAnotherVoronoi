// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/generator/bisector/AbstractBisectorGenerator.h"

namespace yav::generator::bisector
{

/** Generates a paraboloid approximation between edge and vertex sites. */
class EdgeVertexBisectorGenerator final : public AbstractBisectorGenerator
{
public:
    EdgeVertexBisectorGenerator();

    std::shared_ptr<voronoi::equisurface::AbstractBisector> generate(
        const std::shared_ptr<space::site::AbstractSite>& first_site,
        const std::shared_ptr<space::site::AbstractSite>& second_site) const override;
};

} // namespace yav::generator::bisector
