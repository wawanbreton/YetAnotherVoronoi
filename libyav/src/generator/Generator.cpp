// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/generator/Generator.h"

#include <spdlog/spdlog.h>

#include "yav/generator/OctreeNode2.h"
#include "yav/space/Space2.h"
#include "yav/voronoi/Diagram.h"


namespace yav::generator
{

Generator::Generator() = default;

voronoi::Diagram Generator::generate(const space::Space2& input_space) const
{
    auto tree = std::make_shared<OctreeNode2>(geometry::Point2{}, 0.0);
    std::vector<OctreeNode2::Ptr> nodes_to_evaluate{ tree };

    while (! nodes_to_evaluate.empty())
    {
        OctreeNode2::Ptr node = nodes_to_evaluate.back();
        nodes_to_evaluate.pop_back();

        for (size_t i = 0; i < 4; ++i)
        {
            if (! node->isClosestPrimitiveSet(i))
            {
                node->setClosestPrimitive(i, input_space.findClosestPrimitive(node->positionAt(i)));
            }
        }
    }

    voronoi::Diagram diagram;

    return diagram;
}

} // namespace yav::generator
