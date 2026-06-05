// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space3.h"

#include <spdlog/spdlog.h>

#include "yav/space/site/Edge.h"
#include "yav/space/site/Triangle.h"
#include "yav/space/site/Vertex.h"


namespace yav::space
{

Space::Space() = default;

std::shared_ptr<Primitive> Space::addFace(const geometry::Triangle3& triangle)
{
    spdlog::debug("Add triangle {}", triangle);

    return addPrimitive({ std::make_shared<site::Triangle>(triangle),
                          std::make_shared<site::Edge>(geometry::Segment3{ triangle.p1, triangle.p2 }),
                          std::make_shared<site::Edge>(geometry::Segment3{ triangle.p2, triangle.p3 }),
                          std::make_shared<site::Edge>(geometry::Segment3{ triangle.p3, triangle.p1 }),
                          std::make_shared<site::Vertex>(triangle.p1),
                          std::make_shared<site::Vertex>(triangle.p2),
                          std::make_shared<site::Vertex>(triangle.p3) });
}

std::shared_ptr<Primitive> Space::addEdge(const geometry::Segment3& edge)
{
    spdlog::debug("Add edge {}", edge);

    return addPrimitive(
        std::make_shared<site::Edge>(edge),
        std::make_shared<site::Vertex>(edge.first),
        std::make_shared<site::Vertex>(edge.second));
}

std::shared_ptr<Primitive> Space::addVertex(const geometry::Point3& vertex)
{
    spdlog::debug("Add vertex {}", vertex);

    return addPrimitive(std::make_shared<site::Vertex>(vertex));
}

} // namespace yav::space
