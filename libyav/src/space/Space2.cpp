// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space2.h"

#include <spdlog/spdlog.h>

#include "yav/space/site/Edge2.h"
#include "yav/space/site/Vertex2.h"


namespace yav::space
{

Space2::Space2() = default;

std::shared_ptr<site::AbstractSite> Space2::addEdge(const geometry::Segment2& edge)
{
    spdlog::debug("Add edge2 {}", edge);

    return addSite(std::make_shared<site::Edge2>(edge));
}

std::shared_ptr<site::AbstractSite> Space2::addVertex(const geometry::Point2& vertex)
{
    spdlog::debug("Add vertex3 {}", vertex);

    return addSite(std::make_shared<site::Vertex2>(vertex));
}

generator::ClosestSite Space2::findClosestSite(const geometry::Point2& position) const
{
    double closest_distance;
    site::AbstractSite::Ptr closest_primitive;
    for (const site::AbstractSite::Ptr& primitive : primitives())
    {
        const double distance = primitive->distanceTo(position);
        if (! closest_primitive || distance < closest_distance)
        {
            closest_distance = distance;
            closest_primitive = primitive;
        }
    }

    return generator::ClosestSite{ closest_primitive, closest_distance };
}

} // namespace yav::space
