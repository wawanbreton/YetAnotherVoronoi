// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

namespace yav::generator::region
{

/** Base class for analytical site-domain constraints used during clipping. */
class AbstractVoronoiRegion
{
public:
    AbstractVoronoiRegion();
    virtual ~AbstractVoronoiRegion();
};

} // namespace yav::generator::region
