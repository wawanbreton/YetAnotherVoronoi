// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"
#include "yav/space/site/AbstractSite.h"

#include <array>

namespace yav::space::site
{

/** Triangle site representing a 2D manifold piece in 3D space. */
class Triangle final : public AbstractSite
{
public:
    Triangle(std::size_t site_id, const std::array<geometry::Point3, 3>& vertices);

    SiteKind siteKind() const override;
    const std::array<geometry::Point3, 3>& vertices() const;

private:
    std::array<geometry::Point3, 3> vertices_;
};

} // namespace yav::space::site
