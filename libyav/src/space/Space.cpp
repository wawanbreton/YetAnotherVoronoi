// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space.h"

#include <algorithm>

#include "yav/space/Primitive.h"
#include "yav/space/site/Edge.h"
#include "yav/space/site/Triangle.h"
#include "yav/space/site/Vertex.h"


namespace yav::space
{

Space::Space() = default;

std::shared_ptr<Primitive> Space::addFace(const std::array<geometry::Point3, 3>& triangle_vertices)
{
    auto primitive = std::make_shared<Primitive>();

    primitive->addSite(std::make_shared<site::Triangle>(triangle_vertices));

    for (size_t i = 0; i < 3; ++i)
    {
        const geometry::Point3& vertex_start = triangle_vertices[i];
        const geometry::Point3& vertex_end = triangle_vertices[(i + 1) % 3];

        primitive->addSite(std::make_shared<site::Edge>(std::array<geometry::Point3, 2>{ vertex_start, vertex_end }));
        primitive->addSite(std::make_shared<site::Vertex>(vertex_start));
    }

    primitives_.push_back(primitive);
    return primitive;
}

std::shared_ptr<Primitive> Space::addEdge(const std::array<geometry::Point3, 2>& edge_vertices)
{
    auto primitive = std::make_shared<Primitive>();

    primitive->addSite(std::make_shared<site::Edge>(edge_vertices));

    for (const geometry::Point3& vertex : edge_vertices)
    {
        primitive->addSite(std::make_shared<site::Vertex>(vertex));
    }

    primitives_.push_back(primitive);
    return primitive;
}

std::shared_ptr<Primitive> Space::addVertex(const geometry::Point3& vertex_position)
{
    auto primitive = std::make_shared<Primitive>();

    primitive->addSite(std::make_shared<site::Vertex>(vertex_position));

    primitives_.push_back(primitive);
    return primitive;
}

const std::vector<std::shared_ptr<Primitive>>& Space::primitives() const
{
    return primitives_;
}

} // namespace yav::space
