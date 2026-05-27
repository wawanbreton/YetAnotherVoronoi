// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"
#include "yav/space/site/AbstractSite.h"

#include <array>

namespace yav::space::site
{

/** Edge site represented by two 3D endpoints. */
class Edge final : public AbstractSite
{
public:
    Edge(std::size_t site_id, const std::array<geometry::Point3, 2>& vertices);

    SiteKind siteKind() const override;
    const std::array<geometry::Point3, 2>& vertices() const;

private:
    std::array<geometry::Point3, 2> vertices_;
};

} // namespace yav::space::site
