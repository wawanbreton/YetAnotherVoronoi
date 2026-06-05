// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point2.h"
#include "yav/space/site/AbstractSite.h"

namespace yav::space::site
{

/** Vertex site represented by a single position in 3D. */
class Vertex2 final : public AbstractSite
{
public:
    explicit Vertex2(const geometry::Point2& position);

    const geometry::Point2& position() const;

private:
    geometry::Point2 position_;
};

} // namespace yav::space::site
