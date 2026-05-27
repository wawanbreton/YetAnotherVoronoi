// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"

#include <string>

namespace yav::voronoi::equisurface
{

/** Base equisurface used to separate two Voronoi sites analytically. */
class AbstractBisector
{
public:
    AbstractBisector();
    virtual ~AbstractBisector();

    virtual double evaluateAt(const geometry::Point3& position) const = 0;
    virtual std::string surfaceName() const = 0;
};

} // namespace yav::voronoi::equisurface
