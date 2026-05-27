// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"
#include "yav/voronoi/equisurface/AbstractBisector.h"

namespace yav::voronoi::equisurface
{

/** Planar bisector represented by normal vector and offset term. */
class Plane final : public AbstractBisector
{
public:
    Plane(const geometry::Point3& normal, double offset);

    double evaluateAt(const geometry::Point3& position) const override;
    std::string surfaceName() const override;

    const geometry::Point3& normal() const;
    double offset() const;

private:
    geometry::Point3 normal_;
    double offset_;
};

} // namespace yav::voronoi::equisurface
