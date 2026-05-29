// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>

#include "yav/geometry/Segment3.h"
#include "yav/space/site/AbstractSite.h"

namespace yav::space::site
{

/** Edge site represented by two 3D endpoints. */
class Edge final : public AbstractSite
{
public:
    explicit Edge(const geometry::Segment3& segment);

    SiteKind siteKind() const override;
    const geometry::Segment3& segment() const;

private:
    geometry::Segment3 segment_;
};

} // namespace yav::space::site
