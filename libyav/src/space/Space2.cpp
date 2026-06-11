// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space2.h"

#include <spdlog/spdlog.h>

#include "yav/space/site/Edge2.h"
#include "yav/space/site/Vertex2.h"


namespace yav
{

Space2::Space2() = default;

std::shared_ptr<AbstractSite> Space2::addEdge(const Segment2& edge)
{
    spdlog::debug("Add edge2 {}", edge);

    return addSite(std::make_shared<Edge2>(edge));
}

std::shared_ptr<AbstractSite> Space2::addVertex(const Point2& vertex)
{
    spdlog::debug("Add vertex2 {}", vertex);

    return addSite(std::make_shared<Vertex2>(vertex));
}

ClosestSite Space2::findClosestSite(const Point2& position) const
{
    double closest_distance;
    AbstractSite::Ptr closest_primitive;
    for (const AbstractSite::Ptr& primitive : sites())
    {
        const double distance = primitive->distanceTo(position);
        if (! closest_primitive || distance < closest_distance)
        {
            closest_distance = distance;
            closest_primitive = primitive;
        }
    }

    return ClosestSite{ closest_primitive, closest_distance };
}

} // namespace yav
