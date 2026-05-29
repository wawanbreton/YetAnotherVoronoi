// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "yav/space/Space.h"

#include <algorithm>

#include <spdlog/spdlog.h>

#include "yav/space/Primitive.h"
#include "yav/space/site/Edge.h"
#include "yav/space/site/Triangle.h"
#include "yav/space/site/Vertex.h"


namespace yav::space
{

Space::Space() = default;

std::shared_ptr<Primitive> Space::addFace(const geometry::Triangle3& triangle)
{
    auto primitive = addPrimitive();

    primitive->addSite(std::make_shared<site::Triangle>(triangle));

    primitive->addSite(std::make_shared<site::Edge>(geometry::Segment3{ triangle.p1, triangle.p2 }));
    primitive->addSite(std::make_shared<site::Edge>(geometry::Segment3{ triangle.p2, triangle.p3 }));
    primitive->addSite(std::make_shared<site::Edge>(geometry::Segment3{ triangle.p3, triangle.p1 }));

    primitive->addSite(std::make_shared<site::Vertex>(triangle.p1));
    primitive->addSite(std::make_shared<site::Vertex>(triangle.p2));
    primitive->addSite(std::make_shared<site::Vertex>(triangle.p3));

    spdlog::debug("Add triangle {}", triangle);

    return primitive;
}

std::shared_ptr<Primitive> Space::addEdge(const geometry::Segment3& edge)
{
    auto primitive = addPrimitive();

    primitive->addSite(std::make_shared<site::Edge>(edge));

    primitive->addSite(std::make_shared<site::Vertex>(edge.first));
    primitive->addSite(std::make_shared<site::Vertex>(edge.second));

    spdlog::debug("Add edge {}", edge);

    return primitive;
}

std::shared_ptr<Primitive> Space::addVertex(const geometry::Point3& vertex)
{
    auto primitive = addPrimitive();

    primitive->addSite(std::make_shared<site::Vertex>(vertex));

    spdlog::debug("Add vertex {}", vertex);

    return primitive;
}

const std::vector<std::shared_ptr<Primitive>>& Space::primitives() const
{
    return primitives_;
}

std::shared_ptr<Primitive> Space::addPrimitive()
{
    auto primitive = std::make_shared<Primitive>();
    primitives_.push_back(primitive);
    return primitive;
}

} // namespace yav::space
