// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include <spdlog/spdlog.h>

#include "yav/generator/QuadtreeNode.h"
#include "yav/space/Space2.h"
#include "yav/voronoi/Diagram.h"


namespace yav::generator
{

Generator::Generator() = default;

voronoi::Diagram Generator::generate(const space::Space2& input_space) const
{
    auto tree = std::make_shared<QuadtreeNode>(geometry::Point2{}, 0.0);

    voronoi::Diagram diagram;

    return diagram;
}

} // namespace yav::generator
