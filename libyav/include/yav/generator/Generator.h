// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace yav
{

namespace voronoi
{
class Diagram;
}

namespace space
{
class Space;

namespace site
{
class AbstractSite;
}
} // namespace space

namespace voronoi::equisurface
{
class AbstractBisector;
}

namespace generator
{

namespace bisector
{
class AbstractBisectorGenerator;
}

/** Main analytical Voronoi pipeline orchestrating bisectors and cell patches. */
class Generator
{
public:
    Generator();

    voronoi::Diagram generate(const space::Space& input_space) const;

private:
    std::shared_ptr<voronoi::equisurface::AbstractBisector> generateBisector(
        const std::shared_ptr<space::site::AbstractSite>& first_site,
        const std::shared_ptr<space::site::AbstractSite>& second_site) const;

    std::vector<std::shared_ptr<bisector::AbstractBisectorGenerator>> bisector_generators_;
};

} // namespace generator

} // namespace yav
