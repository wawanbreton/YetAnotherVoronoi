// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"

#include <string>

namespace yav::generator::region
{

/** Base class for analytical site-domain constraints used during clipping. */
class AbstractVoronoiRegion
{
public:
    AbstractVoronoiRegion();
    virtual ~AbstractVoronoiRegion();

    virtual bool contains(const geometry::Point3& position) const = 0;
    virtual std::string regionName() const = 0;
};

} // namespace yav::generator::region
