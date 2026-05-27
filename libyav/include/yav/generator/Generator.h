// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/generator/bisector/AbstractBisectorGenerator.h"
#include "yav/generator/region/AbstractVoronoiRegion.h"
#include "yav/space/Space.h"
#include "yav/voronoi/Diagram.h"

#include <memory>
#include <vector>

namespace yav::generator
{

/** Main analytical Voronoi pipeline orchestrating bisectors and cell patches. */
class Generator
{
public:
    Generator();

    voronoi::Diagram generate(const space::Space& input_space) const;

private:
    std::shared_ptr<bisector::AbstractBisectorGenerator> findBisectorGenerator(
        space::site::SiteKind first_kind,
        space::site::SiteKind second_kind) const;

    std::shared_ptr<region::AbstractVoronoiRegion> buildRegionForSite(
        const std::shared_ptr<space::site::AbstractSite>& site) const;

    std::vector<std::shared_ptr<bisector::AbstractBisectorGenerator>> bisector_generators_;
};

} // namespace yav::generator
