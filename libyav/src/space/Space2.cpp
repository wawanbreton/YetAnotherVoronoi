// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space2.h"

#include <algorithm>

#include <spdlog/spdlog.h>

#include "yav/space/Primitive.h"
#include "yav/space/site/Edge2.h"
#include "yav/space/site/Vertex2.h"


namespace yav::space
{

Space2::Space2() = default;

std::shared_ptr<Primitive> Space2::addEdge(const geometry::Segment2& edge)
{
    spdlog::debug("Add edge {}", edge);

    return addPrimitive(
        { std::make_shared<site::Edge2>(edge), std::make_shared<site::Vertex2>(edge.first), std::make_shared<site::Vertex2>(edge.second) });
}

std::shared_ptr<Primitive> Space2::addVertex(const geometry::Point2& vertex)
{
    spdlog::debug("Add vertex {}", vertex);

    return addPrimitive({ std::make_shared<site::Vertex2>(vertex) });
}

generator::ClosestPrimitive Space2::findClosestPrimitive(const geometry::Point2& position) const
{
    double closest_distance;
    Primitive::Ptr closest_primitive;
    for (const Primitive::Ptr& primitive : primitives())
    {
        const double distance = primitive->distanceTo(position);
        if (! closest_primitive || distance < closest_distance)
        {
            closest_distance = distance;
            closest_primitive = primitive;
        }
    }

    return generator::ClosestPrimitive{ closest_primitive, closest_distance };
}

} // namespace yav::space
