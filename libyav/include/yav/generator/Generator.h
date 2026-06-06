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
class AbstractSpace;
class Space2;
} // namespace space

namespace generator
{

/** Main analytical Voronoi pipeline orchestrating bisectors and cell patches. */
class Generator
{
public:
    Generator();

    voronoi::Diagram generate(const space::Space2& input_space) const;
};

} // namespace generator

} // namespace yav
