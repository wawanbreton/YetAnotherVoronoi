// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"
#include "yav/space/site/AbstractSite.h"

namespace yav::space::site
{

/** Vertex site represented by a single position in 3D. */
class Vertex final : public AbstractSite
{
public:
    explicit Vertex(const geometry::Point3& position);

    const geometry::Point3& position() const;

private:
    geometry::Point3 position_;
};

} // namespace yav::space::site
