// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space.h"

#include "yav/space/site/Edge.h"
#include "yav/space/site/Triangle.h"
#include "yav/space/site/Vertex.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <ranges>

namespace yav::space
{

Space::Space()
    : next_primitive_id_(0)
    , next_site_id_(0)
{
}

std::shared_ptr<Primitive> Space::addFace(const std::array<geometry::Point3, 3>& triangle_vertices)
{
    auto primitive = std::make_shared<Primitive>(acquirePrimitiveId());

    auto triangle_site = std::make_shared<site::Triangle>(acquireSiteId(), triangle_vertices);
    primitive->addSite(triangle_site);

    std::array<std::array<geometry::Point3, 2>, 3> edge_vertices{};
    edge_vertices[0] = { triangle_vertices[0], triangle_vertices[1] };
    edge_vertices[1] = { triangle_vertices[1], triangle_vertices[2] };
    edge_vertices[2] = { triangle_vertices[2], triangle_vertices[0] };

    std::ranges::for_each(
        edge_vertices,
        [this, &primitive](const std::array<geometry::Point3, 2>& current_edge_vertices)
        {
            primitive->addSite(std::make_shared<site::Edge>(acquireSiteId(), current_edge_vertices));
        });

    std::ranges::for_each(
        triangle_vertices,
        [this, &primitive](const geometry::Point3& current_vertex)
        {
            primitive->addSite(std::make_shared<site::Vertex>(acquireSiteId(), current_vertex));
        });

    primitives_.push_back(primitive);
    return primitive;
}

std::shared_ptr<Primitive> Space::addEdge(const std::array<geometry::Point3, 2>& edge_vertices)
{
    auto primitive = std::make_shared<Primitive>(acquirePrimitiveId());

    primitive->addSite(std::make_shared<site::Edge>(acquireSiteId(), edge_vertices));

    std::ranges::for_each(
        edge_vertices,
        [this, &primitive](const geometry::Point3& current_vertex)
        {
            primitive->addSite(std::make_shared<site::Vertex>(acquireSiteId(), current_vertex));
        });

    primitives_.push_back(primitive);
    return primitive;
}

std::shared_ptr<Primitive> Space::addVertex(const geometry::Point3& vertex_position)
{
    auto primitive = std::make_shared<Primitive>(acquirePrimitiveId());
    primitive->addSite(std::make_shared<site::Vertex>(acquireSiteId(), vertex_position));

    primitives_.push_back(primitive);
    return primitive;
}

const std::vector<std::shared_ptr<Primitive>>& Space::primitives() const
{
    return primitives_;
}

std::size_t Space::acquirePrimitiveId()
{
    const std::size_t primitive_id = next_primitive_id_;
    ++next_primitive_id_;
    return primitive_id;
}

std::size_t Space::acquireSiteId()
{
    const std::size_t site_id = next_site_id_;
    ++next_site_id_;
    return site_id;
}

} // namespace yav::space
