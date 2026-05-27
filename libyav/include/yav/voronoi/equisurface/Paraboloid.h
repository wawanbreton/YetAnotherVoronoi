// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include "yav/geometry/Point3.h"
#include "yav/voronoi/equisurface/AbstractBisector.h"

namespace yav::voronoi::equisurface
{

/** Axis-aligned paraboloid approximation for point-to-edge/face bisectors. */
class Paraboloid final : public AbstractBisector
{
public:
    Paraboloid(const geometry::Point3& focus, double scale, double offset);

    double evaluateAt(const geometry::Point3& position) const override;

    const geometry::Point3& focus() const;
    double scale() const;
    double offset() const;

private:
    geometry::Point3 focus_;
    double scale_;
    double offset_;
};

} // namespace yav::voronoi::equisurface
