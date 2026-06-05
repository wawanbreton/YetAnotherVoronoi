// Copyright (C) 2026 Erwan MATHIEU
// SPDX-License-Identifier: AGPL-3.0-or-later

#pragma once

#include <memory>
#include <vector>

namespace yav::space
{

namespace site
{
class AbstractSite;
}

class Primitive;

/** Top-level user input container for Voronoi diagram generation. */
class AbstractSpace
{
public:
    AbstractSpace();

    const std::vector<std::shared_ptr<Primitive>>& primitives() const;

protected:
    std::shared_ptr<Primitive> addPrimitive(const std::vector<std::shared_ptr<site::AbstractSite>>& sites);

private:
    std::vector<std::shared_ptr<Primitive>> primitives_;
};

} // namespace yav::space
