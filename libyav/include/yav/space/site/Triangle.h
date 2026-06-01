// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <array>

#include "yav/geometry/Triangle3.h"
#include "yav/space/site/AbstractSite.h"

namespace yav::space::site
{

/** Triangle site representing a 2D manifold piece in 3D space. */
class Triangle final : public AbstractSite
{
public:
    explicit Triangle(const geometry::Triangle3& vertices);

    const geometry::Triangle3& triangle() const;

private:
    geometry::Triangle3 triangle_;
};

} // namespace yav::space::site
