// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space2.h"

#include <spdlog/spdlog.h>

#include "yav/space/primitive/Edge2.h"
#include "yav/space/primitive/Vertex2.h"


namespace yav::space
{

Space2::Space2() = default;

std::shared_ptr<primitive::AbstractPrimitive> Space2::addEdge(const geometry::Segment2& edge)
{
    spdlog::debug("Add edge2 {}", edge);

    return addPrimitive(std::make_shared<primitive::Edge2>(edge));
}

std::shared_ptr<primitive::AbstractPrimitive> Space2::addVertex(const geometry::Point2& vertex)
{
    spdlog::debug("Add vertex3 {}", vertex);

    return addPrimitive(std::make_shared<primitive::Vertex2>(vertex));
}

generator::ClosestPrimitive Space2::findClosestPrimitive(const geometry::Point2& position) const
{
    double closest_distance;
    primitive::AbstractPrimitive::Ptr closest_primitive;
    for (const primitive::AbstractPrimitive::Ptr& primitive : primitives())
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
